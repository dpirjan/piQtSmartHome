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
        qDebug() << "Cannot connect to the D-Bus system bus!";

    if(!connectToDBus())
        qDebug() << "Cannot connect to DBus service "
                 << DATABASE_MANAGER_SERVICE_NAME
                 << " on interface "
                 << DATABASE_MANAGER_SERVICE_INT;

    HomeAlarmInfo::registerMetaType();
    SmartHomeInfo::registerMetaType();
}

databaseManagerInterface::~databaseManagerInterface()
{
    if(iface)
        delete(iface);
}

bool databaseManagerInterface::connectToDBus()
{
    bool ret = true;
    iface = new QDBusInterface(DATABASE_MANAGER_SERVICE_NAME,
                               DATABASE_MANAGER_SERVICE_PATH,
                               DATABASE_MANAGER_SERVICE_INT,
                               QDBusConnection::systemBus());
    if(!iface->isValid())
    {
        qDebug() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    qDebug() << "Service: " << iface->service();
    qDebug() << "Path: " << iface->path();
    qDebug() << "Interface: " << iface->interface();

    return ret;
}

bool databaseManagerInterface::insertHomeAlarmEntry(const HomeAlarmInfo &entry)
{
    QDBusReply<bool> reply = iface->call(QDBus::BlockWithGui,
                                         QLatin1String("insertHomeAlarmEntry"),
                                         qVariantFromValue(entry));
    if (reply.isValid())
        qDebug() << "insertHomeAlarmEntry reply was:" << reply.value();
    else
    {
        qDebug() << "DBus call error: " << iface->lastError();
        qDebug() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool databaseManagerInterface::insertSmartHomeEntry(const SmartHomeInfo &entry)
{
    QDBusReply<bool> reply = iface->call(QDBus::BlockWithGui,
                                         "insertSmartHomeEntry",
                                         qVariantFromValue(entry));

    if (reply.isValid())
        qDebug() << "insertSmartHomeEntry reply was:" << reply.value();
    else
    {
        qDebug() << "DBus call error: " << iface->lastError();
        qDebug() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}
