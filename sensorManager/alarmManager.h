#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <QObject>

#include "io.h"

#define ALARM_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.AlarmManager"
#define ALARM_MANAGER_SERVICE_PATH "/"
#define ALARM_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.AlarmManager.AlarmOperations"

class QSettings;

class AlarmManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", ALARM_MANAGER_SERVICE_INT)

public:
    explicit AlarmManager(QObject *parent = 0);
    virtual ~AlarmManager();

    bool connectService();

    static bool firstRunConfiguration();

signals:
    Q_SCRIPTABLE void alarmEventNotification(); // @TODO update params

public slots:
    Q_SCRIPTABLE void setAlarmGeneralState(const bool&);
    Q_SCRIPTABLE void setAlarmNightState(const bool&);
    Q_SCRIPTABLE void setAlarmVacationState(const bool&);

    Q_SCRIPTABLE bool getAlarmGeneralState() const;
    Q_SCRIPTABLE bool getAlarmNightState() const;
    Q_SCRIPTABLE bool getAlarmVacationState() const;

private:
    void loadAlarmState();
    void saveAlarmState();

    QSettings *m_settings;

    bool m_alarmGeneralState;
    bool m_alarmNightState;
    bool m_alarmVacationState;
};

#endif // ALARMMANAGER_H
