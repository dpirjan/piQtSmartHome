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
                    const QString&,
                    const bool&);
    sensor(const sensor&);
    sensor& operator=(const sensor&);
    virtual ~sensor();

    SystemType getSystemType() const;
    SensorType getSensorType() const;
    QString getZone() const;
    QString getNode() const;
    QString getAddress() const;
    QString getValue() const;
    bool getSendMail() const;

    void setValue(const QString&);

    void debugSensor() const;

private:
    SystemType m_system;
    SensorType m_type;
    QString m_zone;
    QString m_node;
    QString m_address;
    QString m_value;

    bool m_sendMail;
};

#endif // SENSOR_H
