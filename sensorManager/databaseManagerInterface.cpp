#include <QDebug>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#include "databaseManagerInterface.h"

#define DATABASE_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.DatabaseManager"
#define DATABASE_MANAGER_SERVICE_PATH "/"
#define DATABASE_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.DatabaseManager.DatabaseOperations"

databaseManagerInterface::databaseManagerInterface(QObject *parent) : QObject(parent)
{
    if(!QDBusConnection::systemBus().isConnected())
        qCritical() << "Cannot connect to the D-Bus system bus!";

    if(!connectToDBus())
        qCritical() << "Cannot connect to DBus service "
                    << DATABASE_MANAGER_SERVICE_NAME
                    << " on interface "
                    << DATABASE_MANAGER_SERVICE_INT;

    HomeAlarmInfo::registerMetaType();
    SmartHomeInfo::registerMetaType();
}

databaseManagerInterface::~databaseManagerInterface()
{
    if(m_iface)
        delete(m_iface);
}

bool databaseManagerInterface::connectToDBus()
{
    bool ret = true;
    m_iface = new QDBusInterface(DATABASE_MANAGER_SERVICE_NAME,
                                 DATABASE_MANAGER_SERVICE_PATH,
                                 DATABASE_MANAGER_SERVICE_INT,
                                 QDBusConnection::systemBus());
    if(!m_iface->isValid())
    {
        qDebug() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    qDebug() << "Service: " << m_iface->service();
    qDebug() << "Path: " << m_iface->path();
    qDebug() << "Interface: " << m_iface->interface();

    return ret;
}

bool databaseManagerInterface::insertHomeAlarmEntry(const HomeAlarmInfo &entry)
{
    QDBusReply<bool> reply = m_iface->call(QDBus::BlockWithGui,
                                           QLatin1String("insertHomeAlarmEntry"),
                                           qVariantFromValue(entry));
    if(reply.isValid())
        qDebug() << "insertHomeAlarmEntry reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool databaseManagerInterface::insertSmartHomeEntry(const SmartHomeInfo &entry)
{
    QDBusReply<bool> reply = m_iface->call(QDBus::BlockWithGui,
                                           "insertSmartHomeEntry",
                                           qVariantFromValue(entry));

    if(reply.isValid())
        qDebug() << "insertSmartHomeEntry reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}
