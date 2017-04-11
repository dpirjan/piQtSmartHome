#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <QObject>

#include "watchdogHelper.h"

#define SENSOR_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.SensorManager"
#define SENSOR_MANAGER_SERVICE_PATH "/"
#define SENSOR_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.SensorManager.IOOperations"

class wiredSensor;
class wirelessSensor;
class actuator;

class SensorManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", SENSOR_MANAGER_SERVICE_INT)

public:
    explicit SensorManager(QObject *parent = 0);
    ~SensorManager();

    void init();

    bool connectService();

public slots:
    Q_SCRIPTABLE bool setActuatorValue(const QString&,
                                       const QString&,
                                       const QString&);
    Q_SCRIPTABLE QString getActuatorValue(const QString &,
                                       const QString &);
    Q_SCRIPTABLE QStringList checkActuator(const QString &);

private:
    QList<wiredSensor *> m_wiredSensorList;
    QList<wirelessSensor*> m_wirelessSensorList;
    QList<actuator *> m_actuatorList;

    WatchdogHelper *m_watchdog;
};

#endif // SENSORMANAGER_H
