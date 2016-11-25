#include <QDebug>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#include "userManagerInterface.h"

#define USER_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.UserManager"
#define USER_MANAGER_SERVICE_PATH "/"
#define USER_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.UserManager.UserOperations"

UserManagerInterface::UserManagerInterface(QObject *parent) : QObject(parent)
{
    if(!QDBusConnection::systemBus().isConnected())
        qFatal("Cannot connect to the D-Bus system bus!");

    if(!connectToDBus())
        qFatal("Cannot connect to DBus service %s on interface %s!",
               USER_MANAGER_SERVICE_NAME, USER_MANAGER_SERVICE_INT);
}

UserManagerInterface::~UserManagerInterface()
{
    if(m_iface)
        delete m_iface;
}

bool UserManagerInterface::connectToDBus()
{
    bool ret = true;
    m_iface = new QDBusInterface(USER_MANAGER_SERVICE_NAME,
                                 USER_MANAGER_SERVICE_PATH,
                                 USER_MANAGER_SERVICE_INT,
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

QStringList UserManagerInterface::getAllUserNames() const
{
    QDBusReply<QStringList> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAllUserNames"));

    if(reply.isValid())
        qDebug() << "getAllUserNames reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return reply.value();
}

quint32 UserManagerInterface::getLoginTimeout() const
{
    QDBusReply<quint32> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getLoginTimeout"));

    if(reply.isValid())
        qDebug() << "getLoginTimeout reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return reply.value();
}

bool UserManagerInterface::checkPasswordForUser(const QString &user,
                                                const QString &password) const
{
    QDBusReply<bool> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("checkPasswordForUser"),
                qVariantFromValue(user),
                qVariantFromValue(password));

    if(reply.isValid())
        qDebug() << "checkPasswordForUser reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool UserManagerInterface::checkPinForUser(const QString &user,
                                           const QString &pin) const
{
    QDBusReply<bool> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("checkPinForUser"),
                qVariantFromValue(user),
                qVariantFromValue(pin));

    if(reply.isValid())
        qDebug() << "checkPinForUser reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}
