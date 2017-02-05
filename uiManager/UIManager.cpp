#include <QDebug>

#include "UIManager.h"
#include "databaseManagerInterface.h"
#include "userManagerInterface.h"
#include "alarmManagerInterface.h"

UIManager::UIManager(QObject *parent) : QObject(parent)
{
    m_validLogin = false;

    connect(this, SIGNAL(validLoginUpdated()), this, SLOT(validLoginUpdateSlot()));

    m_loginTimeout = UserManagerInterface::instance().getLoginTimeout();
}

UIManager::~UIManager()
{
    if(m_zonesModel)
        m_zonesModel->deleteLater();
    if(m_categoriesModel)
        m_categoriesModel->deleteLater();
    if(m_IOModel)
        m_IOModel->deleteLater();

    m_watchdog->stop();
    m_watchdog->deleteLater();
}

void UIManager::init()
{
    m_watchdog = new WatchdogHelper("piHomeUI");
    m_watchdog->init();

    m_zonesModel = new QStringListModel(this);
    m_categoriesModel = new QStringListModel(this);
    m_IOModel = new ioModel(this);

    QObject::connect(&databaseManagerInterface::instance(),
                     SIGNAL(zonesReceived(QStringList)),
                     this,
                     SLOT(zonesReceived(QStringList)));
    QObject::connect(&databaseManagerInterface::instance(),
                     SIGNAL(categoriesReceived(QStringList)),
                     this,
                     SLOT(categoriesReceived(QStringList)));

    databaseManagerInterface::instance().getAllZones();
    databaseManagerInterface::instance().getAllCategories();
}

void UIManager::zonesReceived(const QStringList &list)
{
    m_zonesModel->setStringList(list);
}

void UIManager::categoriesReceived(const QStringList &list)
{
    m_categoriesModel->setStringList(list);
}

QStringListModel* UIManager::getZonesModel() const
{
    return m_zonesModel;
}

QStringListModel* UIManager::getCategoriesModel() const
{
    return m_categoriesModel;
}

ioModel* UIManager::getIOModel() const
{
    return m_IOModel;
}

alarmEventModel* UIManager::getAlarmEventModel() const
{
    if(m_IOModel)
        return m_IOModel->getAlarmEventModel();
    else
        return NULL;
}

smartHomeEventModel* UIManager::getSmartHomeEventModel() const
{
    if(m_IOModel)
        return m_IOModel->getSmartHomeEventModel();
    else
        return NULL;
}

actuatorEventModel* UIManager::getActuatorEventModel() const
{
    if(m_IOModel)
        return m_IOModel->getActuatorEventModel();
    else
        return NULL;
}

void UIManager::setSelectedZone(const QString &zone)
{
    QList<io> ioList;
    m_selectedZone = zone;
    ioList = databaseManagerInterface::instance().getAllFromZone(zone);
    m_IOModel->setIOList(ioList);
}

void UIManager::setSelectedCategory(const QString &category)
{
    QList<io> ioList;
    m_selectedCategory = category;
    ioList = databaseManagerInterface::instance().getAllFromCategory(category);
    m_IOModel->setIOList(ioList);
}

void UIManager::setSelectedUser(const QString &user)
{
    m_selectedUser = user;
}

void UIManager::setEnteredPassword(const QString &password)
{
    m_enteredPassword = password;
    m_validLogin = UserManagerInterface::instance().checkPasswordForUser(m_selectedUser, password);
    emit validLoginUpdated();
}

void UIManager::setEnteredPin(const QString &pin)
{
    m_enteredPin = pin;
    m_validLogin = UserManagerInterface::instance().checkPinForUser(m_selectedUser, pin);
    emit validLoginUpdated();
}

void UIManager::setValidLogin(const bool &value)
{
    m_validLogin = value;
    emit validLoginUpdated();
}

void UIManager::setAlarmGeneralState(const bool &state)
{
    AlarmManagerInterface::instance().setAlarmGeneralState(state);
    emit alarmGeneralStateChanged();
}

void UIManager::setAlarmNightState(const bool &state)
{
    AlarmManagerInterface::instance().setAlarmNightState(state);
    emit alarmNightStateChanged();
}

void UIManager::setAlarmVacationState(const bool &state)
{
    AlarmManagerInterface::instance().setAlarmVacationState(state);
    emit alarmVacationStateChanged();
}

QString UIManager::getSelectedZone() const
{
    return m_selectedZone;
}

QString UIManager::getSelectedCategory() const
{
    return m_selectedCategory;
}

QString UIManager::getSelectedUser() const
{
    return m_selectedUser;
}

QString UIManager::getEnteredPassword() const
{
    return m_enteredPassword;
}

QString UIManager::getEnteredPin() const
{
    return m_enteredPin;
}

bool UIManager::getValidLogin() const
{
    return m_validLogin;
}

QStringList UIManager::getUserList() const
{
    return UserManagerInterface::instance().getAllUserNames();
}

bool UIManager::getAlarmGeneralState() const
{
    return AlarmManagerInterface::instance().getAlarmGeneralState();
}

bool UIManager::getAlarmNightState() const
{
    return AlarmManagerInterface::instance().getAlarmNightState();
}

bool UIManager::getAlarmVacationState() const
{
    return AlarmManagerInterface::instance().getAlarmVacationState();
}

void UIManager::validLoginUpdateSlot()
{
    if(m_validLogin)
        QTimer::singleShot(m_loginTimeout * 1000, this, SLOT(invalidateLogin()));
}

void UIManager::invalidateLogin()
{
    qCritical() << "Timeout reached, invalidating login";
    setValidLogin(false);
}
