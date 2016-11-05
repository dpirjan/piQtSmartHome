#include <stdio.h>

#include <QCoreApplication>

#include <QDebug>

#include <QDBusConnection>
#include <QDBusError>

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QSettings>
#include <QDir>

#include "databaseManager.h"

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor DatabaseManager";

    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("settingsManager.ini");
    m_settings = new QSettings(settingsPath, QSettings::NativeFormat);

    loadDatabaseSettings();

    QString dbPath = m_databaseFilePath;
    if(dbPath.endsWith(QDir::separator()))
        dbPath.append(m_databaseFileName);
    else
        dbPath.append(QDir::separator()).append(m_databaseFileName);

    if(!QFile::exists(dbPath))
        createDatabaseAndTable();
    else
        connectToDatabase();

    m_watchdog = new WatchdogHelper("piHomeDatabase");
    m_watchdog->init();
}

DatabaseManager::~DatabaseManager()
{
    qDebug() << "Destructor DatabaseManager";
    if(m_db.isOpen())
        m_db.close();

    m_watchdog->stop();
    m_watchdog->deleteLater();
}

bool DatabaseManager::firstRunConfiguration()
{
    bool returnCode = false;

    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("settingsManager.ini");

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;
        QSettings *settings = new QSettings(settingsPath,
                                            QSettings::NativeFormat);

        QString databaseFileName = "database.sqlite";
        QString databaseFilePath = QDir::homePath().append(QDir::separator());
        databaseFilePath.append(".piHome").append(QDir::separator());

        settings->clear();
        settings->beginGroup("DatabaseSettings");
        settings->setValue("databasePath", databaseFilePath);
        settings->setValue("databaseName", databaseFileName);
        settings->endGroup();

        delete settings;
    }

    return returnCode;
}

void DatabaseManager::loadDatabaseSettings()
{
    m_settings->beginGroup("DatabaseSettings");
    m_databaseFilePath = m_settings->value("databasePath").toString();
    m_databaseFileName = m_settings->value("databaseName").toString();
    m_settings->endGroup();
}

bool DatabaseManager::connectService()
{
    bool ret = true;

    HomeAlarmInfo::registerMetaType();
    SmartHomeInfo::registerMetaType();
    io::registerMetaType();

    if(!QDBusConnection::systemBus().registerService(
                DATABASE_MANAGER_SERVICE_NAME))
    {
        qCritical() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    QDBusConnection::systemBus().registerObject(
                DATABASE_MANAGER_SERVICE_PATH,
                this,
                QDBusConnection::ExportScriptableContents);

    if(ret == true)
        qDebug() << "Registered the database manager service to DBUS system bus";

    return ret;
}

bool DatabaseManager::connectToDatabase()
{
    bool ret = true;

    QString dbPath = m_databaseFilePath;
    if(dbPath.endsWith(QDir::separator()))
        dbPath.append(m_databaseFileName);
    else
        dbPath.append(QDir::separator()).append(m_databaseFileName);

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    QSqlQuery query;
    if(!this->m_db.open())
    {
        qCritical() << "Connect to database " << dbPath << " failed! "
                    << query.lastError().text();

        ret = false;
    }
    return ret;
}

bool DatabaseManager::createDatabaseAndTable()
{
    bool ret = true;

    if(!m_db.isOpen())
    {
        ret = connectToDatabase();
        if(!ret)
            return ret;
    }

    QSqlQuery query;
    ret = query.exec("CREATE TABLE alarmsystem "
                     "(id integer primary key autoincrement, "
                     "zone char(30), "
                     "node char(30), "
                     "category char(30), "
                     "address char(30), "
                     "timestamp char(30))");
    if(!ret)
    {
        qCritical() << "Create table alarmsystem failed! " <<
                       query.lastError().text();
        return ret;
    }
    else
        qDebug("Created table alarmsystem");

    ret = query.exec("CREATE TABLE smarthome "
                     "(id integer primary key autoincrement, "
                     "zone char(30), "
                     "node char(30), "
                     "category char(30), "
                     "address char(30), "
                     "value char(30), "
                     "timestamp char(30))");
    if(!ret)
    {
        qCritical() << "Create table smarthome failed! " <<
                       query.lastError().text();
        return ret;
    }
    else
        qDebug("Created table smarthome");

    ret = query.exec("CREATE TABLE io "
                     "(id integer primary key autoincrement, "
                     "system char(30), "
                     "hardware char(30), "
                     "type char(30), "
                     "category char(30), "
                     "zone char(30), "
                     "node char(30), "
                     "address char(30),"
                     "UNIQUE(system, "
                     "hardware, "
                     "type, "
                     "category, "
                     "zone, "
                     "node, "
                     "address))");
    if(!ret)
    {
        qCritical() << "Create table IO failed! " <<
                       query.lastError().text();
        return ret;
    }
    else
        qDebug("Created table IO");

    return ret;
}

bool DatabaseManager::insertHomeAlarmEntry(const HomeAlarmInfo &entry) const
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("INSERT INTO alarmsystem (zone, node, category, address, timestamp) "
                  "VALUES (:zone, :node, :category, :address, :timestamp)");
    query.bindValue(":zone", entry.getZone());
    query.bindValue(":node", entry.getNode());
    query.bindValue(":category", entry.getCategory());
    query.bindValue(":address", entry.getAddress());
    query.bindValue(":timestamp", entry.getTimestamp());
    ret = query.exec();
    if(!ret)
        qCritical() << "Insert into alarmsystem table failed! " << query.lastError();

    return ret;
}

bool DatabaseManager::insertSmartHomeEntry(const SmartHomeInfo &entry) const
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("INSERT INTO smarthome (zone, node, category, address, value, timestamp)"
                  " VALUES (:zone, :node, :category, :address, :value, :timestamp)");
    query.bindValue(":zone", entry.getZone());
    query.bindValue(":node", entry.getNode());
    query.bindValue(":category", entry.getCategory());
    query.bindValue(":address", entry.getAddress());
    query.bindValue(":value", entry.getValue());
    query.bindValue(":timestamp", entry.getTimestamp());
    ret = query.exec();
    if(!ret)
        qCritical() << "Insert into smarthome table failed! " << query.lastError();

    return ret;
}

bool DatabaseManager::insertIO(const QString &system,
                               const QString &hardware,
                               const QString &type,
                               const QString &category,
                               const QString &zone,
                               const QString &node,
                               const QString &address) const
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("INSERT INTO io (system, hardware, type, category, zone, node,"
                  " address) VALUES (:system, :hardware, :type, :category, "
                  ":zone, :node, :address)");
    query.bindValue(":system", system);
    query.bindValue(":hardware", hardware);
    query.bindValue(":type", type);
    query.bindValue(":category", category);
    query.bindValue(":zone", zone);
    query.bindValue(":node", node);
    query.bindValue(":address", address);
    ret = query.exec();
    if(!ret)
        qCritical() << "Insert into IO table failed! " << query.lastError();

    return ret;
}

QList<HomeAlarmInfo> DatabaseManager::getAllHomeAlarmEntries() const
{
    bool result = false;
    int idZone, idNode, idCategory, idAddress, idTimestamp;
    QList<HomeAlarmInfo> tmpList;
    QSqlQuery query;

    query.prepare("SELECT * FROM alarmsystem");

    result = query.exec();
    if(!result)
        qCritical() << "Error: " << query.lastQuery() << " err: "<< query.lastError();

    idZone = query.record().indexOf("zone");
    idNode = query.record().indexOf("node");
    idCategory = query.record().indexOf("category");
    idAddress = query.record().indexOf("address");
    idTimestamp = query.record().indexOf("timestamp");

    while(query.next())
    {
        HomeAlarmInfo tmp(query.value(idZone).toString(),
                          query.value(idNode).toString(),
                          query.value(idCategory).toString(),
                          query.value(idAddress).toString(),
                          query.value(idTimestamp).toString());
        tmpList.append(tmp);
    }

    qDebug() << "getAllHomeAlarmEntries returned " << tmpList.count()
             << " HomeAlarm entries.";

    return tmpList;
}

QList<SmartHomeInfo> DatabaseManager::getAllSmartHomeEntries() const
{
    bool result = false;
    int idZone, idNode, idCategory, idAddress, idValue, idTimestamp;
    QList<SmartHomeInfo> tmpList;
    QSqlQuery query;

    query.prepare("SELECT * FROM smarthome");
    result = query.exec();
    if(!result)
        qCritical() << "Error: " << query.lastQuery() << " err: "<< query.lastError();

    idZone = query.record().indexOf("zone");
    idNode = query.record().indexOf("node");
    idCategory = query.record().indexOf("category");
    idAddress = query.record().indexOf("address");
    idValue = query.record().indexOf("value");
    idTimestamp = query.record().indexOf("timestamp");

    while(query.next())
    {
        SmartHomeInfo tmp(query.value(idZone).toString(),
                          query.value(idNode).toString(),
                          query.value(idCategory).toString(),
                          query.value(idAddress).toString(),
                          query.value(idValue).toString(),
                          query.value(idTimestamp).toString());
        tmpList.append(tmp);
    }

    qDebug() << "getAllSmartHomeEntries returned " << tmpList.count()
             << " SmartHome entries.";

    return tmpList;
}

QList<HomeAlarmInfo> DatabaseManager::getHomeAlarmEntriesForIO(const io &obj) const
{
    bool result = false;
    int idZone, idNode, idCategory, idAddress, idTimestamp;
    QList<HomeAlarmInfo> tmpList;
    QSqlQuery query;

    query.prepare("SELECT * FROM alarmsystem WHERE zone = (:zone) AND "
                  "node = (:node) AND category = (:category) AND address = (:address)");
    query.bindValue(":zone", QVariant::fromValue(obj.getZone()));
    query.bindValue(":node", QVariant::fromValue(obj.getNode()));
    query.bindValue(":category", QVariant::fromValue(obj.getCategory()));
    query.bindValue(":address", QVariant::fromValue(obj.getAddress()));

    result = query.exec();
    if(!result)
        qCritical() << "Error: " << query.lastQuery() << " err: "<< query.lastError();

    idZone = query.record().indexOf("zone");
    idNode = query.record().indexOf("node");
    idCategory = query.record().indexOf("category");
    idAddress = query.record().indexOf("address");
    idTimestamp = query.record().indexOf("timestamp");

    while(query.next())
    {
        HomeAlarmInfo tmp(query.value(idZone).toString(),
                          query.value(idNode).toString(),
                          query.value(idCategory).toString(),
                          query.value(idAddress).toString(),
                          query.value(idTimestamp).toString());
        tmpList.append(tmp);
    }

    qDebug() << "getHomeAlarmEntriesForIO returned " << tmpList.count()
             << " HomeAlarm entries.";

    return tmpList;
}

QList<SmartHomeInfo> DatabaseManager::getSmartHomeEntriesForIO(const io &obj) const
{
    bool result = false;
    int idZone, idNode, idCategory, idAddress, idValue, idTimestamp;
    QList<SmartHomeInfo> tmpList;
    QSqlQuery query;

    query.prepare("SELECT * FROM smarthome WHERE zone = (:zone) AND "
                  "node = (:node) AND category = (:category) AND address = (:address)");
    query.bindValue(":zone", obj.getZone());
    query.bindValue(":node", obj.getNode());
    query.bindValue(":category", obj.getCategory());
    query.bindValue(":address", obj.getAddress());

    result = query.exec();
    if(!result)
        qCritical() << "Error: " << query.lastQuery() << " err: "<< query.lastError();

    idZone = query.record().indexOf("zone");
    idNode = query.record().indexOf("node");
    idCategory = query.record().indexOf("category");
    idAddress = query.record().indexOf("address");
    idValue = query.record().indexOf("value");
    idTimestamp = query.record().indexOf("timestamp");

    while(query.next())
    {
        SmartHomeInfo tmp(query.value(idZone).toString(),
                          query.value(idNode).toString(),
                          query.value(idCategory).toString(),
                          query.value(idAddress).toString(),
                          query.value(idValue).toString(),
                          query.value(idTimestamp).toString());
        tmpList.append(tmp);
    }

    qDebug() << "Returned " << tmpList.count() << " SmartHome entries.";

    return tmpList;
}

QStringList DatabaseManager::getAllZones() const
{
    QStringList tmpList;

    QSqlQueryModel model;
    model.setQuery("SELECT DISTINCT zone FROM io");

    for(int i = 0; i < model.rowCount(); ++i)
        tmpList.append(model.record(i).value("zone").toString());

    qDebug() << "Returned " << tmpList.count() << " zone(s).";

    return tmpList;
}

QStringList DatabaseManager::getAllCategories() const
{
    QStringList tmpList;

    QSqlQueryModel model;
    model.setQuery("SELECT DISTINCT category FROM io");

    for(int i = 0; i < model.rowCount(); ++i)
        tmpList.append(model.record(i).value("category").toString());

    qDebug() << "Returned " << tmpList.count() << " categories.";

    return tmpList;
}

QList<io> DatabaseManager::getAllFromZone(const QString &zone) const
{
    QList<io> tmpList;
    QSqlQuery query;

    query.prepare("SELECT * FROM io WHERE zone = (:zone)");
    query.bindValue(":zone", zone);

    if(query.exec())
    {
        int idSystem = query.record().indexOf("system");
        int idHardware = query.record().indexOf("hardware");
        int idType = query.record().indexOf("type");
        int idCategory = query.record().indexOf("category");
        int idZone = query.record().indexOf("zone");
        int idNode = query.record().indexOf("node");
        int idAddress = query.record().indexOf("address");

        while(query.next())
        {
            io tmp(query.value(idSystem).toString(),
                   query.value(idHardware).toString(),
                   query.value(idType).toString(),
                   query.value(idCategory).toString(),
                   query.value(idZone).toString(),
                   query.value(idNode).toString(),
                   query.value(idAddress).toString());
            tmpList.append(tmp);
        }
    }
    else
        qCritical() << "getAllFromZone query failed! " << query.lastError();

    qDebug() << "Returned " << tmpList.count() << " io(s) for zone "
             << zone << ".";

    return tmpList;
}

QList<io> DatabaseManager::getAllFromCategory(const QString &category) const
{
    QList<io> tmpList;
    QSqlQuery query;

    query.prepare("SELECT * FROM io WHERE category = (:category)");
    query.bindValue(":category", category);

    if(query.exec())
    {
        int idSystem = query.record().indexOf("system");
        int idHardware = query.record().indexOf("hardware");
        int idType = query.record().indexOf("type");
        int idCategory = query.record().indexOf("category");
        int idZone = query.record().indexOf("zone");
        int idNode = query.record().indexOf("node");
        int idAddress = query.record().indexOf("address");

        while(query.next())
        {
            io tmp(query.value(idSystem).toString(),
                   query.value(idHardware).toString(),
                   query.value(idType).toString(),
                   query.value(idCategory).toString(),
                   query.value(idZone).toString(),
                   query.value(idNode).toString(),
                   query.value(idAddress).toString());
            tmpList.append(tmp);
        }
    }
    else
        qCritical() << "getAllFromCategory query failed! " << query.lastError();

    qDebug() << "Returned " << tmpList.count() << " io(s) for category "
             << category << ".";

    return tmpList;
}
