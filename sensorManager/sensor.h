#ifndef SENSOR_H
#define SENSOR_H

#include "common.h"

using namespace piHome;

class sensorInstantiator;

class sensor
{
public:
    explicit sensor(const SystemType&,
                    const SensorType&,
                    const HardwareType&,
                    const QString&,
                    const QString&,
                    const QString&,
                    const QString &value = QString(""),
                    const bool &interrupt = false);
    sensor(const sensor &obj);
    sensor& operator=(const sensor &other);
    virtual ~sensor();

    friend class sensorInstantiator;

public:
    SystemType getSystemType() const;
    SensorType getSensorType() const;
    HardwareType getHardwareType() const;
    QString getZone() const;
    QString getNode() const;
    QString getAddress() const;
    QString getValue() const;
    bool getSupportInterrupt() const;

    QString getWiredEdge() const;
    int getWiredTimeout() const;

    void setValue(const QString &value);
    void setSupportInterrupt(const bool &value);

    void debugSensor() const;

protected:
    void setWiredEdge(const QString &edge);
    void setWiredTimeout(const int &timeout);

private:
    SystemType m_system;
    SensorType m_type;
    HardwareType m_hard;
    QString m_zone;
    QString m_node;
    QString m_address;
    QString m_value;
    bool m_supportInterrupt;

    QString m_wiredEdge;
    int m_wiredTimeout;

    template<class T> friend class QList;

public slots:
    void wiredInterrupt(void);
};

#endif // SENSOR_H
