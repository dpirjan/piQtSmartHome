#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <QObject>

class wiredSensor;
class wirelessSensor;
class actuator;

class SensorManager : public QObject
{
    Q_OBJECT
public:
    explicit SensorManager(QObject *parent = 0);
    ~SensorManager();
    void init();


private:
    QList<wiredSensor *> m_wiredSensorList;
    QList<wirelessSensor*> m_wirelessSensorList;
    QList<actuator *> m_actuatorList;
};

#endif // SENSORMANAGER_H
