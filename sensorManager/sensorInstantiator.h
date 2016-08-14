#ifndef SENSORINSTANTIATOR_H
#define SENSORINSTANTIATOR_H

#include <QObject>
#include <QSettings>

#include "wiredSensor.h"
#include "wirelessSensor.h"

using namespace piHome;

class sensorInstantiator : public QObject
{
    Q_OBJECT

    explicit sensorInstantiator(QObject *parent = 0);
    virtual ~sensorInstantiator();
    Q_DISABLE_COPY(sensorInstantiator)

    void loadSensors();

public:
    static sensorInstantiator &instance()
    {
        static sensorInstantiator obj;
        return obj;
    }

public slots:
    void saveSensors(); // only for debugging will be removed in the future
    QList<wiredSensor> getWiredSensors() const;
    QList<wirelessSensor> getWirelessSensors() const;

private:
    QSettings *m_settings;

    QList<wiredSensor> m_wiredSensorList;
    QList<wirelessSensor> m_wirelessSensorList;
};

#endif // SENSORINSTANTIATOR_H
