#include <QDebug>

#include "UIManager.h"
#include "databaseManagerInterface.h"

UIManager::UIManager(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor DatabaseManager";
}

UIManager::~UIManager()
{
    qDebug() << ">>>Destructor UIManager";
    if(m_zonesModel)
        m_zonesModel->deleteLater();
    if(m_categoriesModel)
        m_categoriesModel->deleteLater();
    if(m_IOModel)
        m_IOModel->deleteLater();

    m_watchdog->stop();
    m_watchdog->deleteLater();
    qDebug() << "<<<Destructor UIManager";
}

void UIManager::init()
{
    m_watchdog = new WatchdogHelper("piHomeUI");
    m_watchdog->init();

    m_zonesModel = new QStringListModel(this);
    m_categoriesModel = new QStringListModel(this);
    m_IOModel = new QStringListModel(this);

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
    qDebug() << "Zones: " << list;
    m_zonesModel->setStringList(list);
}

void UIManager::categoriesReceived(const QStringList &list)
{
    qDebug() << "Categories: " << list;
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

QStringListModel* UIManager::getIOModel() const
{
    return m_IOModel;
}

void UIManager::setSelectedZone(const QString &zone)
{
    QStringList sensorList;
    m_selectedZone = zone;
    sensorList = databaseManagerInterface::instance().getAllFromZone(zone);
    qDebug() << "All sensors from zone " << zone << ": " << sensorList;
    m_IOModel->setStringList(sensorList);
}

void UIManager::setSelectedCategory(const QString &category)
{
    QStringList sensorList;
    m_selectedCategory = category;
    sensorList = databaseManagerInterface::instance().getAllFromCategory(category);
    qDebug() << "All sensors from category " << category << ": " << sensorList;
    m_IOModel->setStringList(sensorList);
}

void UIManager::setSelectedIO(const QString &io)
{
    m_selectedIO = io;
    qDebug() << "Selected IO: " << io << " from zone: " << m_selectedZone;
    //@TODO add call to select all entries from database for this specific sensor
}

QString UIManager::getSelectedZone() const
{
    return m_selectedZone;
}

QString UIManager::getSelectedCategory() const
{
    return m_selectedCategory;
}

QString UIManager::getSelectedIO() const
{
    return m_selectedIO;
}
