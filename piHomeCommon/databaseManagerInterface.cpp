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
    ActuatorInfo::registerMetaType();
    io::registerMetaType();
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

void databaseManagerInterface::insertHomeAlarmEntry(const HomeAlarmInfo &entry)
{
    QDBusPendingCall pCall = m_iface->asyncCall(
                QLatin1String("insertHomeAlarmEntry"),
                qVariantFromValue(entry));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pCall, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(insertHomeAlarmEntryFinished(QDBusPendingCallWatcher*)));
}

void databaseManagerInterface::insertHomeAlarmEntryFinished(
        QDBusPendingCallWatcher *call)
{
    QDBusReply<bool> reply = *call;

    if(reply.isValid())
        qDebug() << "insertHomeAlarmEntry reply was: " << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    call->deleteLater();
}

void databaseManagerInterface::insertSmartHomeEntry(const SmartHomeInfo &entry)
{
    QDBusPendingCall pCall = m_iface->asyncCall(QLatin1String("insertSmartHomeEntry"),
                                                qVariantFromValue(entry));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pCall, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(insertSmartHomeEntryFinished(QDBusPendingCallWatcher*)));
}

void databaseManagerInterface::insertSmartHomeEntryFinished(
        QDBusPendingCallWatcher *call)
{
    QDBusReply<bool> reply = *call;

    if(reply.isValid())
        qDebug() << "insertSmartHomeEntry reply was: " << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    call->deleteLater();
}

void databaseManagerInterface::insertActuatorInfoEntry(const ActuatorInfo &entry)
{
    QDBusPendingCall pCall = m_iface->asyncCall(QLatin1String("insertActuatorInfoEntry"),
                                                qVariantFromValue(entry));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pCall, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(insertActuatorInfoEntryFinished(QDBusPendingCallWatcher*)));
}

void databaseManagerInterface::insertActuatorInfoEntryFinished(
        QDBusPendingCallWatcher *call)
{
    QDBusReply<bool> reply = *call;

    if(reply.isValid())
        qDebug() << "insertActuatorInfoEntry reply was: " << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    call->deleteLater();
}

QList<HomeAlarmInfo> databaseManagerInterface::getAllHomeAlarmEntries() const
{
    QList<HomeAlarmInfo> tmpList;

    QDBusReply<QList<HomeAlarmInfo>> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAllHomeAlarmEntries"));

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllHomeAlarmEntries reply contains " << tmpList.count()
                 << "HomeAlarm entries";
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return tmpList;
}

QList<SmartHomeInfo> databaseManagerInterface::getAllSmartHomeEntries() const
{
    QList<SmartHomeInfo> tmpList;

    QDBusReply<QList<SmartHomeInfo>> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAllSmartHomeEntries"));

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllSmartHomeEntries reply contains " << tmpList.count()
                 << "SmartHome entries";
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return tmpList;
}

QList<ActuatorInfo> databaseManagerInterface::getAllActuatorInfoEntries() const
{
    QList<ActuatorInfo> tmpList;

    QDBusReply<QList<ActuatorInfo>> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAllActuatorInfoEntries"));

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllActuatorInfoEntries reply contains " << tmpList.count()
                 << "ActuatorInfo entries";
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return tmpList;
}

QList<HomeAlarmInfo> databaseManagerInterface::getHomeAlarmEntriesForIO(const io &obj) const
{
    QList<HomeAlarmInfo> tmpList;

    QDBusReply<QList<HomeAlarmInfo>> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getHomeAlarmEntriesForIO"),
                QVariant::fromValue(obj));

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getHomeAlarmEntriesForIO reply contains " << tmpList.count()
                 << "HomeAlarm entries";
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return tmpList;
}

QList<SmartHomeInfo> databaseManagerInterface::getSmartHomeEntriesForIO(const io &obj) const
{
    QList<SmartHomeInfo> tmpList;

    QDBusReply<QList<SmartHomeInfo>> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getSmartHomeEntriesForIO"),
                QVariant::fromValue(obj));

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllSmartHomeEntries reply contains " << tmpList.count()
                 << "SmartHome entries";
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return tmpList;
}

QList<ActuatorInfo> databaseManagerInterface::getActuatorInfoEntriesForIO(const io &obj) const
{
    QList<ActuatorInfo> tmpList;

    QDBusReply<QList<ActuatorInfo>> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getActuatorInfoEntriesForIO"),
                QVariant::fromValue(obj));

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllActuatorInfoEntries reply contains " << tmpList.count()
                 << "ActuatorInfo entries";
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return tmpList;
}

void databaseManagerInterface::insertIO(const QString &system,
                                        const QString &hardware,
                                        const QString &type,
                                        const QString &function,
                                        const QString &zone,
                                        const QString &node,
                                        const QString &address)
{
    QDBusPendingCall pCall = m_iface->asyncCall(QLatin1String("insertIO"),
                                                qVariantFromValue(system),
                                                qVariantFromValue(hardware),
                                                qVariantFromValue(type),
                                                qVariantFromValue(function),
                                                qVariantFromValue(zone),
                                                qVariantFromValue(node),
                                                qVariantFromValue(address));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pCall, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(insertIOFinished(QDBusPendingCallWatcher*)));
}

void databaseManagerInterface::insertIOFinished(QDBusPendingCallWatcher *call)
{
    QDBusReply<bool> reply = *call;

    if(reply.isValid())
        qDebug() << "insertIO reply was: " << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    call->deleteLater();
}

void databaseManagerInterface::getAllZones()
{
    QDBusPendingCall pCall = m_iface->asyncCall(
                QLatin1String("getAllZones"));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pCall, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(getAllZonesFinished(QDBusPendingCallWatcher*)));
}

void databaseManagerInterface::getAllZonesFinished(QDBusPendingCallWatcher *call)
{
    QStringList tmpList;
    QDBusReply<QStringList> reply = *call;

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllZones reply was: " << tmpList;
        emit zonesReceived(tmpList);
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    call->deleteLater();
}

void databaseManagerInterface::getAllCategories()
{
    QDBusPendingCall pCall = m_iface->asyncCall(
                QLatin1String("getAllCategories"));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pCall, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(getAllCategoriesFinished(QDBusPendingCallWatcher*)));
}

void databaseManagerInterface::getAllCategoriesFinished(
        QDBusPendingCallWatcher *call)
{
    QStringList tmpList;
    QDBusReply<QStringList> reply = *call;

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllCategoriesFinished reply was: " << tmpList;
        emit categoriesReceived(tmpList);
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    call->deleteLater();
}

QList<io> databaseManagerInterface::getAllFromZone(const QString &zone) const
{
    QList<io> tmpList;

    QDBusReply<QList<io>> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAllFromZone"),
                qVariantFromValue(zone));

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllFromZone reply contains " << tmpList.count() << "io(s)";
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return tmpList;
}

QList<io> databaseManagerInterface::getAllFromCategory(
        const QString &category) const
{
    QList<io> tmpList;

    QDBusReply<QList<io>> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("getAllFromCategory"),
                qVariantFromValue(category));

    if(reply.isValid())
    {
        tmpList = reply.value();
        qDebug() << "getAllFromCategory reply contains " << tmpList.count() << "io(s)";
    }
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
    }

    return tmpList;
}
