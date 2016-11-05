#include <QDebug>
#include <QDateTime>

#include "homeAlarmInfo.h"

HomeAlarmInfo::HomeAlarmInfo() :
    m_zone(),
    m_node(),
    m_category(),
    m_address()
{
    m_timestamp = QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss.zzz");
    qDebug() << "DefHomeAlarmInfo Timestamp : " << m_timestamp;
}


HomeAlarmInfo::HomeAlarmInfo(const QString &zone,
                             const QString &node,
                             const QString &category,
                             const QString &address) :
    m_zone(zone),
    m_node(node),
    m_category(category),
    m_address(address)
{
    m_timestamp = QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss.zzz");
    qDebug() << "HomeAlarmInfo Timestamp : " << m_timestamp;
}

HomeAlarmInfo::HomeAlarmInfo(const QString &zone,
                             const QString &node,
                             const QString &category,
                             const QString &address,
                             const QString &timestamp) :
    m_zone(zone),
    m_node(node),
    m_category(category),
    m_address(address),
    m_timestamp(timestamp)
{
}

HomeAlarmInfo::HomeAlarmInfo(const HomeAlarmInfo &obj)
{
    m_zone = obj.getZone();
    m_node = obj.getNode();
    m_category = obj.getCategory();
    m_address = obj.getAddress();
    m_timestamp = obj.getTimestamp();
}

HomeAlarmInfo& HomeAlarmInfo::operator=(const HomeAlarmInfo &other)
{
    m_zone = other.getZone();
    m_node= other.getNode();
    m_category = other.getCategory();
    m_address = other.getAddress();
    m_timestamp = other.getTimestamp();

    return *this;
}

HomeAlarmInfo::~HomeAlarmInfo()
{
}

void HomeAlarmInfo::registerMetaType()
{
    qRegisterMetaType<HomeAlarmInfo>("HomeAlarmInfo");
    qDBusRegisterMetaType<HomeAlarmInfo>();

    qRegisterMetaType<QList<HomeAlarmInfo> >("QList<HomeAlarmInfo>");
    qDBusRegisterMetaType<QList<HomeAlarmInfo> >();
}

QDBusArgument &operator<<(QDBusArgument &argument, const HomeAlarmInfo &data)
{
    argument.beginStructure();
    QString zone = data.getZone();
    QString node = data.getNode();
    QString category = data.getCategory();
    QString address = data.getAddress();
    QString timestamp = data.getTimestamp();
    argument << zone;
    argument << node;
    argument << category;
    argument << address;
    argument << timestamp;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, HomeAlarmInfo &data)
{
    QString zone, node, category, address, timestamp;
    argument.beginStructure();
    argument >> zone;
    argument >> node;
    argument >> category;
    argument >> address;
    argument >> timestamp;
    argument.endStructure();
    data.setNode(node);
    data.setZone(zone);
    data.setCategory(category);
    data.setAddress(address);
    data.setTimestamp(timestamp);
    return argument;
}

void HomeAlarmInfo::setZone(const QString &zone)
{
    m_zone = zone;
}

QString HomeAlarmInfo::getZone() const
{
    return m_zone;
}

void HomeAlarmInfo::setNode(const QString &node)
{
    m_node = node;
}

QString HomeAlarmInfo::getNode() const
{
    return m_node;
}

void HomeAlarmInfo::setCategory(const QString &category)
{
    m_category = category;
}

QString HomeAlarmInfo::getCategory() const
{
    return m_category;
}

void HomeAlarmInfo::setAddress(const QString &address)
{
    m_address = address;
}

QString HomeAlarmInfo::getAddress() const
{
    return m_address;
}

void HomeAlarmInfo::setTimestamp(const QString &timestamp)
{
    m_timestamp = timestamp;
}

QString HomeAlarmInfo::getTimestamp() const
{
    return m_timestamp;
}
