#ifndef SENSOR_H
#define SENSOR_H

#include "common.h"

using namespace piHome;

class sensor
{
public:
    explicit sensor(const SystemType&,
                    const SensorType&,
                    const QString&,
                    const QString&,
                    const QString&);
    sensor(const sensor&);
    sensor& operator=(const sensor&);
    virtual ~sensor();

    SystemType getSystemType() const;
    SensorType getSensorType() const;
    QString getZone() const;
    QString getNode() const;
    QString getAddress() const;
    QString getValue() const;

    void setValue(const QString &value);

    void debugSensor() const;

public slots:
    virtual void interrupt() = 0;

private:
    SystemType m_system;
    SensorType m_type;
    QString m_zone;
    QString m_node;
    QString m_address;
    QString m_value;
};

#endif // SENSOR_H
