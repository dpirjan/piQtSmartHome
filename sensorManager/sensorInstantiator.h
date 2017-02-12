#ifndef SENSORINSTANTIATOR_H
#define SENSORINSTANTIATOR_H

#include <QObject>

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

    static bool firstRunInitSensors();

public slots:
    QList<wiredSensor *> getWiredSensors() const;
    QList<wirelessSensor *> getWirelessSensors() const;

    wirelessSensor* findWirelessSensor(const IOType&, const QString&) const;

private:
    bool m_sendMail;

    QList<wiredSensor *> m_wiredSensorList;
    QList<wirelessSensor *> m_wirelessSensorList;
};

#endif // SENSORINSTANTIATOR_H
