#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "common.h"

using namespace piHome;

class actuator
{
public:
    explicit actuator(const SystemType&,
                      const IOType&,
                      const HardwareType&,
                      const QString&,
                      const QString&,
                      const QString&,
                      const QString &value = QString(""));
    actuator(const actuator&);
    actuator& operator=(const actuator&);
    virtual ~actuator();

public:
    SystemType getSystemType() const;
    IOType getActuatorType() const;
    HardwareType getHardwareType() const;
    QString getZone() const;
    QString getNode() const;
    QString getAddress() const;
    QString getValue() const;

    void setValue(const QString&);

    void debugActuator() const;

private:
    SystemType m_system;
    IOType m_type;
    HardwareType m_hard;
    QString m_zone;
    QString m_node;
    QString m_address;
    QString m_value;

    template<class T> friend class QList;
};

#endif // ACTUATOR_H
