#include <stdio.h>

#include <QCoreApplication>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QtDBus>

#include "databaseManager.h"

DatabaseManager::DatabaseManager(const QString &path,
                                 const bool &firstTime,
                                 QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor DatabaseManager(" << path << ", " << firstTime << ")";
    if(firstTime)
        createDatabaseAndTable(path);
    else
        connectToDatabase(path);
}

DatabaseManager::~DatabaseManager()
{
    qDebug() << "Destructor DatabaseManager";
    if(m_db.isOpen())
        m_db.close();
}

bool DatabaseManager::connectService()
{
    bool ret = true;

    HomeAlarmInfo::registerMetaType();
    SmartHomeInfo::registerMetaType();

    if (!QDBusConnection::sessionBus().registerService(DATABASE_MANAGER_SERVICE_NAME))
    {
        qDebug() << QDBusConnection::sessionBus().lastError().message();
        ret = false;
    }

    QDBusConnection::sessionBus().registerObject(DATABASE_MANAGER_SERVICE_PATH,
                                                 this,
                                                 QDBusConnection::ExportScriptableContents);

    qDebug() << "Registered the database manager service to session bus";

    return ret;
}

bool DatabaseManager::connectToDatabase(const QString &path)
{
    bool ret = true;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    QSqlQuery query;
    if(!this->m_db.open())
    {
        qDebug() << "Connect to database " << path << " failed! "
                 << query.lastError().text();

        ret = false;
    }
    return ret;
}

bool DatabaseManager::createDatabaseAndTable(const QString &path)
{
    bool ret = true;
    if(!m_db.isOpen())
    {
        ret = connectToDatabase(path);
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
        qDebug() << "Create table alarmsystem failed! " <<
                    query.lastError().text();
        return ret;
    }

    ret = query.exec("CREATE TABLE smarthome "
                     "(id integer primary key autoincrement, "
                     "zone char(30), "
                     "node char(30), "
                     "sensor char(30), "
                     "value char(30), "
                     "timestamp char(30))");
    if(!ret)
    {
        qDebug() << "Create table smarthome failed! " <<
                    query.lastError().text();
        return ret;
    }

    return ret;
}

bool DatabaseManager::insertHomeAlarmEntry(const HomeAlarmInfo &entry)
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
        qDebug() << "Insert into alarmsystem table failed! "
                 << query.lastError();

    return ret;
}

bool DatabaseManager::insertSmartHomeEntry(const SmartHomeInfo &entry)
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
        qDebug() << "Insert into smarthome table failed! "
                 << query.lastError();

    return ret;
}

QList<HomeAlarmInfo> DatabaseManager::getAllHomeAlarmEntries()
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

    return tmpList;
}

QList<SmartHomeInfo> DatabaseManager::getAllSmartHomeEntries()
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

    return tmpList;
}
