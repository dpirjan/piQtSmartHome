#include "settingsManagerInterface.h"

SettingsManagerInterface::SettingsManagerInterface(QObject *parent) : QObject(parent)
{
    if(!QDBusConnection::sessionBus().isConnected())
    {
        qDebug() << "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n";
    }

    if(!connectToDBus())
    {
        qDebug() << "Cannot connect to DBus service "
                 << DATABASE_SETTINGS_SERVICE_NAME
                 << " on interface "
                 << DATABASE_SETTINGS_SERVICE_INT;
    }
}

SettingsManagerInterface::~SettingsManagerInterface()
{
    if(iface)
        delete(iface);
}

bool SettingsManagerInterface::connectToDBus()
{
    bool ret = true;
    iface = new QDBusInterface(DATABASE_SETTINGS_SERVICE_NAME,
                               DATABASE_SETTINGS_SERVICE_PATH,
                               DATABASE_SETTINGS_SERVICE_INT,
                               QDBusConnection::sessionBus());
    if(!iface->isValid())
    {
        qDebug() << QDBusConnection::sessionBus().lastError().message();
        ret = false;
    }

    qDebug() << "Service: " << iface->service();
    qDebug() << "Path: " << iface->path();
    qDebug() << "Interface: " << iface->interface();

    return ret;
}

QString SettingsManagerInterface::getDatabaseName() const
{
    QDBusReply<QString> reply = iface->call(QDBus::Block, "getDatabaseName");
    if (reply.isValid())
        qDebug() << "Reply was:" << reply.value();
    else
    {
        qDebug() << "DBus call error: " << iface->lastError();
        qDebug() << "DBus reply error: " << reply.error();
        return QString();
    }
    return reply.value();
}

QString SettingsManagerInterface::getDatabasePath() const
{
    QDBusReply<QString> reply = iface->call(QDBus::Block, "getDatabasePath");
    if (reply.isValid())
        qDebug () << "Reply was:" << reply.value();
    else
    {
        qDebug() << "DBus call error: " << iface->lastError();
        qDebug() << "DBus reply error: " << reply.error();
        return QString();
    }
    return reply.value();
}
