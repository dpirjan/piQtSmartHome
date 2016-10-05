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
        qFatal("Cannot connect to the D-Bus system bus!");

    if(!connectToDBus())
        qFatal("Cannot connect to DBus service %s on interface %s!",
               MAIL_MANAGER_SERVICE_NAME, MAIL_MANAGER_SERVICE_INT);
}

mailManagerInterface::~mailManagerInterface()
{
    if(m_iface)
        delete(m_iface);
}

bool mailManagerInterface::connectToDBus()
{
    bool ret = true;
    m_iface = new QDBusInterface(MAIL_MANAGER_SERVICE_NAME,
                                 MAIL_MANAGER_SERVICE_PATH,
                                 MAIL_MANAGER_SERVICE_INT,
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

bool mailManagerInterface::connectToServer()
{
    QDBusReply<bool> reply = m_iface->call(QDBus::BlockWithGui,
                                           QLatin1String("connectToServer"));
    if(reply.isValid())
        qDebug() << "connectToServer reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

bool mailManagerInterface::loginToServer()
{
    QDBusReply<bool> reply = m_iface->call(QDBus::BlockWithGui,
                                           QLatin1String("loginToServer"));
    if(reply.isValid())
        qDebug() << "loginToServer reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}

void mailManagerInterface::sendMail(const QString &subject, const QString &message)
{
    QDBusPendingCall pCall = m_iface->asyncCall(QLatin1String("sendMail"),
                                                qVariantFromValue(subject),
                                                qVariantFromValue(message));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pCall, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(sendMailFinished(QDBusPendingCallWatcher*)));
}

void mailManagerInterface::sendMailFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<bool> reply = *call;

    if(reply.isValid())
        qDebug() << "sendMail reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }
}

bool mailManagerInterface::disconnectFromServer()
{
    QDBusReply<bool> reply = m_iface->call(QDBus::BlockWithGui,
                                           QLatin1String("disconnectFromServer"));
    if(reply.isValid())
        qDebug() << "disconnectFromServer reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}
