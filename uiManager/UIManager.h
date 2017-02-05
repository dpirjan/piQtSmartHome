#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QStringListModel>

#include "ioModel.h"
#include "smartHomeEventModel.h"
#include "alarmEventModel.h"
#include "actuatorEventModel.h"
#include "watchdogHelper.h"

class UIManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString selectedZone WRITE setSelectedZone READ getSelectedZone)
    Q_PROPERTY(QString selectedCategory WRITE setSelectedCategory READ getSelectedCategory)
    Q_PROPERTY(QString selectedUser WRITE setSelectedUser READ getSelectedUser)
    Q_PROPERTY(QString enteredPassword WRITE setEnteredPassword READ getEnteredPassword)
    Q_PROPERTY(QString enteredPin WRITE setEnteredPin READ getEnteredPin)
    Q_PROPERTY(bool validLogin WRITE setValidLogin READ getValidLogin NOTIFY validLoginUpdated)
    Q_PROPERTY(QStringList userList READ getUserList NOTIFY userListUpdated)
    Q_PROPERTY(bool alarmGeneralState READ getAlarmGeneralState WRITE setAlarmGeneralState NOTIFY alarmGeneralStateChanged)
    Q_PROPERTY(bool alarmNightState READ getAlarmNightState WRITE setAlarmNightState NOTIFY alarmNightStateChanged)
    Q_PROPERTY(bool alarmVacationState READ getAlarmVacationState WRITE setAlarmVacationState NOTIFY alarmVacationStateChanged)

public:
    explicit UIManager(QObject *parent = 0);
    virtual ~UIManager();

    void init();

    QStringListModel* getZonesModel() const;
    QStringListModel* getCategoriesModel() const;

    ioModel* getIOModel() const;
    alarmEventModel* getAlarmEventModel() const;
    smartHomeEventModel* getSmartHomeEventModel() const;
    actuatorEventModel* getActuatorEventModel() const;

private slots:
    void zonesReceived(const QStringList&);
    void categoriesReceived(const QStringList&);

    void setSelectedZone(const QString&);
    void setSelectedCategory(const QString&);
    void setSelectedUser(const QString&);
    void setEnteredPassword(const QString&);
    void setEnteredPin(const QString&);
    void setValidLogin(const bool &);

    void setAlarmGeneralState(const bool&);
    void setAlarmNightState(const bool&);
    void setAlarmVacationState(const bool&);

    void invalidateLogin();
    void validLoginUpdateSlot();

    QString getSelectedZone() const;
    QString getSelectedCategory() const;
    QString getSelectedUser() const;
    QString getEnteredPassword() const;
    QString getEnteredPin() const;
    bool getValidLogin() const;
    QStringList getUserList() const;

    bool getAlarmGeneralState() const;
    bool getAlarmNightState() const;
    bool getAlarmVacationState() const;

signals:
    void userListUpdated(); // used only to avoid warning from qml
    void validLoginUpdated();

    void alarmGeneralStateChanged();
    void alarmNightStateChanged();
    void alarmVacationStateChanged();

private:
    WatchdogHelper *m_watchdog;

    QStringListModel *m_zonesModel;
    QStringListModel *m_categoriesModel;

    ioModel *m_IOModel;

    QString m_selectedZone;
    QString m_selectedCategory;
    QString m_selectedUser;
    QString m_enteredPassword;
    QString m_enteredPin;
    bool m_validLogin;
    quint32 m_loginTimeout;
};

#endif // UIMANAGER_H
