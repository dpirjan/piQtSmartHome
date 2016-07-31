#include <QDebug>
#include <QDateTime>

#include "homeAlarmInfo.h"

HomeAlarmInfo::HomeAlarmInfo() :
    m_zone(),
    m_node(),
    m_sensor()
{
    m_timestamp = QDateTime::currentDateTime().toString();
    qDebug() << "DefHomeAlarmInfo Timestamp : " << m_timestamp;
}


HomeAlarmInfo::HomeAlarmInfo(const QString &zone,
                             const QString &node,
                             const QString &sensor) :
    m_zone(zone),
    m_node(node),
    m_sensor(sensor)
{
    m_timestamp = QDateTime::currentDateTime().toString();
    qDebug() << "HomeAlarmInfo Timestamp : " << m_timestamp;
}

HomeAlarmInfo::HomeAlarmInfo(const QString &zone,
                             const QString &node,
                             const QString &sensor,
                             const QString &timestamp) :
  m_zone(zone),
  m_node(node),
  m_sensor(sensor),
  m_timestamp(timestamp)
{
}

HomeAlarmInfo::HomeAlarmInfo(const HomeAlarmInfo &obj)
{
    m_zone = obj.getZone();
    m_node = obj.getNode();
    m_sensor = obj.getSensor();
    m_timestamp = obj.getTimestamp();
}

HomeAlarmInfo& HomeAlarmInfo::operator=(const HomeAlarmInfo &other)
{
    m_zone = other.getZone();
    m_node= other.getNode();
    m_sensor = other.getSensor();
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
    QString sensor = data.getSensor();
    QString timestamp = data.getTimestamp();
    argument << zone;
    argument << node;
    argument << sensor;
    argument << timestamp;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, HomeAlarmInfo &data)
{
    QString zone, node, sensor, timestamp;
    argument.beginStructure();
    argument >> zone;
    argument >> node;
    argument >> sensor;
    argument >> timestamp;
    argument.endStructure();
    data.setNode(node);
    data.setZone(zone);
    data.setSensor(sensor);
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

void HomeAlarmInfo::setSensor(const QString &sensor)
{
    m_sensor = sensor;
}

QString HomeAlarmInfo::getSensor() const
{
    return m_sensor;
}

void HomeAlarmInfo::setTimestamp(const QString &timestamp)
{
    m_timestamp = timestamp;
}

QString HomeAlarmInfo::getTimestamp() const
{
    return m_timestamp;
}
