#include <QDebug>
#include <QDateTime>

#include "actuatorInfo.h"

ActuatorInfo::ActuatorInfo() :
    m_zone(),
    m_node(),
    m_category(),
    m_address(),
    m_value()
{
    m_timestamp = QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss.zzz");
    qDebug() << "DefActuatorInfo Timestamp : " << m_timestamp;
}

ActuatorInfo::ActuatorInfo(const QString &zone,
                             const QString &node,
                             const QString &category,
                             const QString &address,
                             const QString &value) :
    m_zone(zone),
    m_node(node),
    m_category(category),
    m_address(address),
    m_value(value)
{
    m_timestamp = QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss.zzz");
    qDebug() << "ActuatorInfo Timestamp : " << m_timestamp;
}

ActuatorInfo::ActuatorInfo(const QString &zone,
                             const QString &node,
                             const QString &category,
                             const QString &address,
                             const QString &value,
                             const QString &timestamp) :
    m_zone(zone),
    m_node(node),
    m_category(category),
    m_address(address),
    m_value(value),
    m_timestamp(timestamp)
{
}

ActuatorInfo::ActuatorInfo(const ActuatorInfo &obj)
{
    m_zone = obj.getZone();
    m_node = obj.getNode();
    m_category = obj.getCategory();
    m_address = obj.getAddress();
    m_value = obj.getValue();
    m_timestamp = obj.getTimestamp();
}

ActuatorInfo& ActuatorInfo::operator=(const ActuatorInfo &other)
{
    m_zone = other.getZone();
    m_node= other.getNode();
    m_category = other.getCategory();
    m_address = other.getAddress();
    m_value = other.getValue();
    m_timestamp = other.getTimestamp();

    return *this;
}

ActuatorInfo::~ActuatorInfo()
{
}

void ActuatorInfo::registerMetaType()
{
    qRegisterMetaType<ActuatorInfo>("ActuatorInfo");
    qDBusRegisterMetaType<ActuatorInfo>();

    qRegisterMetaType<QList<ActuatorInfo> >("QList<ActuatorInfo>");
    qDBusRegisterMetaType<QList<ActuatorInfo> >();
}

QDBusArgument &operator<<(QDBusArgument &argument, const ActuatorInfo &data)
{
    argument.beginStructure();
    QString zone = data.getZone();
    QString node = data.getNode();
    QString category = data.getCategory();
    QString address = data.getAddress();
    QString value = data.getValue();
    QString timestamp = data.getTimestamp();
    argument << zone;
    argument << node;
    argument << category;
    argument << address;
    argument << value;
    argument << timestamp;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ActuatorInfo &data)
{
    QString zone, node, category, address, value, timestamp;
    argument.beginStructure();
    argument >> zone;
    argument >> node;
    argument >> category;
    argument >> address;
    argument >> value;
    argument >> timestamp;
    argument.endStructure();
    data.setNode(node);
    data.setZone(zone);
    data.setCategory(category);
    data.setAddress(address);
    data.setValue(value);
    data.setTimestamp(timestamp);
    return argument;
}

QString ActuatorInfo::getZone() const
{
    return m_zone;
}

QString ActuatorInfo::getNode() const
{
    return m_node;
}

QString ActuatorInfo::getCategory() const
{
    return m_category;
}

QString ActuatorInfo::getAddress() const
{
    return m_address;
}

QString ActuatorInfo::getValue() const
{
    return m_value;
}

QString ActuatorInfo::getTimestamp() const
{
    return m_timestamp;
}

void ActuatorInfo::setZone(const QString &zone)
{
    m_zone = zone;
}

void ActuatorInfo::setNode(const QString &node)
{
    m_node = node;
}

void ActuatorInfo::setCategory(const QString &category)
{
    m_category = category;
}

void ActuatorInfo::setAddress(const QString &address)
{
    m_address = address;
}

void ActuatorInfo::setValue(const QString &value)
{
    m_value = value;
}

void ActuatorInfo::setTimestamp(const QString &timestamp)
{
    m_timestamp = timestamp;
}
