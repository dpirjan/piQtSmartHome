#include <QDebug>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#include "alarmManagerInterface.h"

#define ALARM_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.AlarmManager"
#define ALARM_MANAGER_SERVICE_PATH "/"
#define ALARM_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.AlarmManager.AlarmOperations"

AlarmManagerInterface::AlarmManagerInterface(QObject *parent) : QObject(parent)
{
    if(!QDBusConnection::systemBus().isConnected())
        qFatal("Cannot connect to the D-Bus system bus!");

    if(!connectToDBus())
        qFatal("Cannot connect to DBus service %s on interface %s!",
               ALARM_MANAGER_SERVICE_NAME, ALARM_MANAGER_SERVICE_INT);
}

AlarmManagerInterface::~AlarmManagerInterface()
{
    if(m_iface)
        delete m_iface;
}

bool AlarmManagerInterface::connectToDBus()
{
    bool ret = true;
    m_iface = new QDBusInterface(ALARM_MANAGER_SERVICE_NAME,
                                 ALARM_MANAGER_SERVICE_PATH,
                                 ALARM_MANAGER_SERVICE_INT,
                                 QDBusConnection::systemBus());
    if(!m_iface->isValid())
    {
        qCritical() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    qDebug() << "Service: " << m_iface->service();
    qDebug() << "Path: " << m_iface->path();
    qDebug() << "Interface: " << m_iface->interface();

    return ret;
}

void AlarmManagerInterface::setAlarmGeneralState(const bool &state)
{
    m_iface->call(QDBus::BlockWithGui,
                  QLatin1String("setAlarmGeneralState"),
                  qVariantFromValue(state));
}

void AlarmManagerInterface::setAlarmNightState(const bool &state)
{
    m_iface->call(QDBus::BlockWithGui,
                  QLatin1String("setAlarmNightState"),
                  qVariantFromValue(state));
}

void AlarmManagerInterface::setAlarmVacationState(const bool &state)
{
    m_iface->call(QDBus::BlockWithGui,
                  QLatin1String("setAlarmVacationState"),
                  qVariantFromValue(state));
}

bool AlarmManagerInterface::getAlarmGeneralState() const
{
    QDBusReply<bool> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAlarmGeneralState"));

    if(reply.isValid())
        qDebug() << "getAlarmGeneralState reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool AlarmManagerInterface::getAlarmNightState() const
{
    QDBusReply<bool> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAlarmNightState"));

    if(reply.isValid())
        qDebug() << "getAlarmNightState reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool AlarmManagerInterface::getAlarmVacationState() const
{
    QDBusReply<bool> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAlarmVacationState"));

    if(reply.isValid())
        qDebug() << "getAlarmVacationState reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}
