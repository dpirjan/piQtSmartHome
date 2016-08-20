#include "sensor.h"

sensor::sensor(const SystemType &system,
               const SensorType &type,
               const QString &zone,
               const QString &node,
               const QString &address)
    : m_system(system)
    , m_type(type)
    , m_zone(zone)
    , m_node(node)
    , m_address(address)
{
    m_value = QString("");
    m_sendMail = false;
}

sensor::sensor(const sensor &obj)
{
    m_system = obj.getSystemType();
    m_type = obj.getSensorType();
    m_zone = obj.getZone();
    m_node = obj.getNode();
    m_address = obj.getAddress();
    m_value = obj.getValue();
    m_sendMail = obj.getSendMail();
}

sensor& sensor::operator=(const sensor &other)
{
    m_system = other.getSystemType();
    m_type = other.getSensorType();
    m_zone = other.getZone();
    m_node = other.getNode();
    m_address = other.getAddress();
    m_value = other.getValue();
    m_sendMail = other.getSendMail();

    return *this;
}

sensor::~sensor()
{
}

void sensor::debugSensor() const
{
    qDebug() << "==============================================";
    qDebug() << systemTypeToString(m_system);
    qDebug() << sensorTypeToString(m_type);
    qDebug() << m_zone;
    qDebug() << m_node;
    qDebug() << m_address;
    qDebug() << m_value;
    qDebug() << m_sendMail;
}

SystemType sensor::getSystemType() const
{
    return m_system;
}

SensorType sensor::getSensorType() const
{
    return m_type;
}

QString sensor::getZone() const
{
    return m_zone;
}

QString sensor::getNode() const
{
    return m_node;
}

QString sensor::getAddress() const
{
    return m_address;
}

QString sensor::getValue() const
{
    return m_value;
}

bool sensor::getSendMail() const
{
    return m_sendMail;
}

void sensor::setValue(const QString &value)
{
    m_value = value;
}

void sensor::setSendMail(const bool &sendMail)
{
    m_sendMail = sendMail;
}

//void sensor::setNode(const QString &node)
//{
//    m_node = node;
//}

//void sensor::setZone(const QString &zone)
//{
//    m_zone = zone;
//}

//void sensor::setSensorType(const SensorType &value)
//{
//    m_type = value;
//}

//void sensor::setSystemType(const SystemType &value)
//{
//    m_system = value;
//}
