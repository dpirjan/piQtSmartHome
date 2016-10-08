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
                     "sensor char(30), "
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
                     "sensor char(30), "
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
    qDebug() << "insertHomeAlarmEntry ("
             << entry.getZone()
             << ", "
             << entry.getNode()
             << ", "
             << entry.getSensor()
             << ", "
             << entry.getTimestamp()
             << ")";

    bool ret = true;
    QSqlQuery query;
    query.prepare("INSERT INTO alarmsystem (zone, node, sensor, timestamp) "
                  "VALUES (:zone, :node, :sensor, :timestamp)");
    query.bindValue(":zone", entry.getZone());
    query.bindValue(":node", entry.getNode());
    query.bindValue(":sensor", entry.getSensor());
    query.bindValue(":timestamp", entry.getTimestamp());
    ret = query.exec();
    if(!ret)
        qCritical() << "Insert into alarmsystem table failed! "
                    << query.lastError();

    return ret;
}

bool DatabaseManager::insertSmartHomeEntry(const SmartHomeInfo &entry) const
{
    bool ret = true;
    QSqlQuery query;
    query.prepare("INSERT INTO smarthome (zone, node, sensor, value, timestamp)"
                  " VALUES (:zone, :node, :sensor, :value, :timestamp)");
    query.bindValue(":zone", entry.getZone());
    query.bindValue(":node", entry.getNode());
    query.bindValue(":sensor", entry.getSensor());
    query.bindValue(":value", entry.getValue());
    query.bindValue(":timestamp", entry.getTimestamp());
    ret = query.exec();
    if(!ret)
        qCritical() << "Insert into smarthome table failed! "
                    << query.lastError();

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
    bool ret = true;
    QSqlQuery query;
    query.prepare("INSERT INTO io (system, hardware, type, category, zone, node, address)"
                  " VALUES (:system, :hardware, :type, :category, :zone, :node, :address)");
    query.bindValue(":system", system);
    query.bindValue(":hardware", hardware);
    query.bindValue(":type", type);
    query.bindValue(":category", category);
    query.bindValue(":zone", zone);
    query.bindValue(":node", node);
    query.bindValue(":address", address);
    ret = query.exec();
    if(!ret)
        qCritical() << "Insert into IO table failed! "
                    << query.lastError();

    return ret;
}

QList<HomeAlarmInfo> DatabaseManager::getAllHomeAlarmEntries() const
{
    int idZone, idNode, idSensor, idTimestamp;
    QList<HomeAlarmInfo> tmpList;
    QSqlQuery query;

    query.prepare("SELECT * FROM alarmsystem");
    query.exec();

    idZone = query.record().indexOf("zone");
    idNode = query.record().indexOf("node");
    idSensor = query.record().indexOf("sensor");
    idTimestamp = query.record().indexOf("timestamp");

    while(query.next())
    {
        HomeAlarmInfo tmp(query.value(idZone).toString(),
                          query.value(idNode).toString(),
                          query.value(idSensor).toString(),
                          query.value(idTimestamp).toString());
        tmpList.append(tmp);
    }

    qDebug() << "Returned " << tmpList.count() << " HomeAlarm entries.";

    return tmpList;
}

QList<SmartHomeInfo> DatabaseManager::getAllSmartHomeEntries() const
{
    int idZone, idNode, idSensor, idValue, idTimestamp;
    QList<SmartHomeInfo> tmpList;
    QSqlQuery query;

    query.prepare("SELECT * FROM smarthome");

    idZone = query.record().indexOf("zone");
    idNode = query.record().indexOf("node");
    idSensor = query.record().indexOf("sensor");
    idValue = query.record().indexOf("value");
    idTimestamp = query.record().indexOf("timestamp");

    while(query.next())
    {
        SmartHomeInfo tmp(query.value(idZone).toString(),
                          query.value(idNode).toString(),
                          query.value(idSensor).toString(),
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

QStringList DatabaseManager::getAllFromZone(const QString &zone) const
{
    QStringList tmpList;
    QSqlQuery query;

    query.prepare("SELECT category FROM io WHERE zone = (:zone)");
    query.bindValue(":zone", zone);



    if(query.exec())
    {
        int idCategory = query.record().indexOf("category");

        while(query.next())
            tmpList.append(query.value(idCategory).toString());
    }
    else
        qCritical() << "getAllFromZone query failed! " << query.lastError();

    qDebug() << "Returned " << tmpList.count() << " categories for zone "
             << zone << ".";

    return tmpList;
}

QStringList DatabaseManager::getAllFromCategory(const QString &category) const
{
    QStringList tmpList;
    QSqlQuery query;

    query.prepare("SELECT zone FROM io WHERE category = (:category)");
    query.bindValue(":category", category);

    if(query.exec())
    {
        int idZone = query.record().indexOf("zone");

        while(query.next())
            tmpList.append(query.value(idZone).toString());
    }
    else
        qCritical() << "getAllFromCategory query failed! " << query.lastError();

    qDebug() << "Returned " << tmpList.count() << " zones for category "
             << category << ".";

    return tmpList;
}
