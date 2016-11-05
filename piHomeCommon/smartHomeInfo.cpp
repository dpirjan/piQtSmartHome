#include <QDebug>
#include <QDateTime>

#include "smartHomeInfo.h"

SmartHomeInfo::SmartHomeInfo() :
    m_zone(),
    m_node(),
    m_category(),
    m_address(),
    m_value()
{
    m_timestamp = QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss.zzz");
    qDebug() << "DefSmartHomeInfo Timestamp : " << m_timestamp;
}

SmartHomeInfo::SmartHomeInfo(const QString &zone,
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
    qDebug() << "SmartHomeInfo Timestamp : " << m_timestamp;
}

SmartHomeInfo::SmartHomeInfo(const QString &zone,
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

SmartHomeInfo::SmartHomeInfo(const SmartHomeInfo &obj)
{
    m_zone = obj.getZone();
    m_node = obj.getNode();
    m_category = obj.getCategory();
    m_address = obj.getAddress();
    m_value = obj.getValue();
    m_timestamp = obj.getTimestamp();
}

SmartHomeInfo& SmartHomeInfo::operator=(const SmartHomeInfo &other)
{
    m_zone = other.getZone();
    m_node= other.getNode();
    m_category = other.getCategory();
    m_address = other.getAddress();
    m_value = other.getValue();
    m_timestamp = other.getTimestamp();

    return *this;
}

SmartHomeInfo::~SmartHomeInfo()
{
}

void SmartHomeInfo::registerMetaType()
{
    qRegisterMetaType<SmartHomeInfo>("SmartHomeInfo");
    qDBusRegisterMetaType<SmartHomeInfo>();

    qRegisterMetaType<QList<SmartHomeInfo> >("QList<SmartHomeInfo>");
    qDBusRegisterMetaType<QList<SmartHomeInfo> >();
}

QDBusArgument &operator<<(QDBusArgument &argument, const SmartHomeInfo &data)
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

const QDBusArgument &operator>>(const QDBusArgument &argument, SmartHomeInfo &data)
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

QString SmartHomeInfo::getZone() const
{
    return m_zone;
}

QString SmartHomeInfo::getNode() const
{
    return m_node;
}

QString SmartHomeInfo::getCategory() const
{
    return m_category;
}

QString SmartHomeInfo::getAddress() const
{
    return m_address;
}

QString SmartHomeInfo::getValue() const
{
    return m_value;
}

QString SmartHomeInfo::getTimestamp() const
{
    return m_timestamp;
}

void SmartHomeInfo::setZone(const QString &zone)
{
    m_zone = zone;
}

void SmartHomeInfo::setNode(const QString &node)
{
    m_node = node;
}

void SmartHomeInfo::setCategory(const QString &category)
{
    m_category = category;
}

void SmartHomeInfo::setAddress(const QString &address)
{
    m_address = address;
}

void SmartHomeInfo::setValue(const QString &value)
{
    m_value = value;
}

void SmartHomeInfo::setTimestamp(const QString &timestamp)
{
    m_timestamp = timestamp;
}
