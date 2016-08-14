#include "actuator.h"

actuator::actuator(const SystemType &system,
                   const ActuatorType &type,
                   const HardwareType &hw,
                   const QString &zone,
                   const QString &node,
                   const QString &address,
                   const QString &value)
  : m_system(system)
  , m_type(type)
  , m_hard(hw)
  , m_zone(zone)
  , m_node(node)
  , m_address(address)
  , m_value(value)
{
}

actuator::actuator(const actuator &obj)
{
    m_system = obj.getSystemType();
    m_type = obj.getActuatorType();
    m_hard = obj.getHardwareType();
    m_zone = obj.getZone();
    m_node = obj.getNode();
    m_address = obj.getAddress();
    m_value = QString("");
}

actuator& actuator::operator=(const actuator &other)
{
    m_system = other.getSystemType();
    m_type = other.getActuatorType();
    m_hard = other.getHardwareType();
    m_zone = other.getZone();
    m_node = other.getNode();
    m_address = other.getAddress();
    m_value = other.getValue();

    return *this;
}

actuator::~actuator()
{
}

SystemType actuator::getSystemType() const
{
    return m_system;
}

ActuatorType actuator::getActuatorType() const
{
    return m_type;
}

HardwareType actuator::getHardwareType() const
{
    return m_hard;
}

QString actuator::getZone() const
{
    return m_zone;
}

QString actuator::getNode() const
{
    return m_node;
}

QString actuator::getAddress() const
{
    return m_address;
}

QString actuator::getValue() const
{
    return m_value;
}


void actuator::setValue(const QString &value)
{
    m_value = value;
}