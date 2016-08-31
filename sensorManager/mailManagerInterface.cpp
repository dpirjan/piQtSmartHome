#include <QDebug>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#include "mailManagerInterface.h"

#define MAIL_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.MailManager"
#define MAIL_MANAGER_SERVICE_PATH "/"
#define MAIL_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.MailManager.MailOperations"

mailManagerInterface::mailManagerInterface(QObject *parent) : QObject(parent)
{
    if(!QDBusConnection::systemBus().isConnected())
        qDebug() << "Cannot connect to the D-Bus system bus!";

    if(!connectToDBus())
        qDebug() << "Cannot connect to DBus service "
                 << MAIL_MANAGER_SERVICE_NAME
                 << " on interface "
                 << MAIL_MANAGER_SERVICE_INT;
}

mailManagerInterface::~mailManagerInterface()
{
    if(iface)
        delete(iface);
}

bool mailManagerInterface::connectToDBus()
{
    bool ret = true;
    iface = new QDBusInterface(MAIL_MANAGER_SERVICE_NAME,
                               MAIL_MANAGER_SERVICE_PATH,
                               MAIL_MANAGER_SERVICE_INT,
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

bool mailManagerInterface::connectToServer()
{
    QDBusReply<bool> reply = iface->call(QDBus::BlockWithGui,
                                         QLatin1String("connectToServer"));
    if (reply.isValid())
        qDebug() << "connectToServer reply was:" << reply.value();
    else
    {
        qDebug() << "DBus call error: " << iface->lastError();
        qDebug() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool mailManagerInterface::loginToServer()
{
    QDBusReply<bool> reply = iface->call(QDBus::BlockWithGui,
                                         QLatin1String("loginToServer"));
    if (reply.isValid())
        qDebug() << "loginToServer reply was:" << reply.value();
    else
    {
        qDebug() << "DBus call error: " << iface->lastError();
        qDebug() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool mailManagerInterface::sendMail(const QString &subject, const QString &message)
{
    //@TODO find a more elegant solution here (Thread vs AsyncCall)
    QDBusReply<bool> reply = iface->call(QDBus::BlockWithGui,
                                         QLatin1String("sendMail"),
                                         qVariantFromValue(subject),
                                         qVariantFromValue(message));
    if (reply.isValid())
        qDebug() << "sendMail reply was:" << reply.value();
    else
    {
        qDebug() << "DBus call error: " << iface->lastError();
        qDebug() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool mailManagerInterface::disconnectFromServer()
{
    QDBusReply<bool> reply = iface->call(QDBus::BlockWithGui,
                                         QLatin1String("disconnectFromServer"));
    if (reply.isValid())
        qDebug() << "disconnectFromServer reply was:" << reply.value();
    else
    {
        qDebug() << "DBus call error: " << iface->lastError();
        qDebug() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}
