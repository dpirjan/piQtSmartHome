#include <QSettings>
#include <QDebug>

#include <QtDBus>

#include "databaseSettingsManager.h"

databaseSettingsManager::databaseSettingsManager(QObject *parent) : QObject(parent)
{
    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("settingsManager.ini");
    qDebug() << "settingsManager: " << settingsPath;
    m_settings = new QSettings(settingsPath, QSettings::NativeFormat);
}

databaseSettingsManager::~databaseSettingsManager()
{
    m_settings->~QSettings();
}

bool databaseSettingsManager::connectService()
{
    bool ret = true;

    if (!QDBusConnection::sessionBus().registerService(
                DATABASE_SETTINGS_SERVICE_NAME))
    {
        qDebug() << QDBusConnection::sessionBus().lastError().message();
        ret = false;
    }

    QDBusConnection::sessionBus().registerObject(
                DATABASE_SETTINGS_SERVICE_PATH,
                this,
                QDBusConnection::ExportScriptableContents);

    qDebug() << "Registered the database manager settings service to session bus";

    return ret;
}

void databaseSettingsManager::loadSettings()
{
    m_settings->beginGroup("DatabaseSettings");
    m_databaseFilePath = m_settings->value("databasePath").toString();
    m_databaseFileName = m_settings->value("databaseName").toString();
    m_settings->endGroup();
}

void databaseSettingsManager::saveSettings()
{
    m_databaseFileName = "database.sqlite";
    m_databaseFilePath = "/home/azanosch/.piHome/";

    m_settings->beginGroup("DatabaseSettings");
    m_settings->setValue("databasePath", m_databaseFilePath);
    m_settings->setValue("databaseName", m_databaseFileName);
    m_settings->endGroup();
}

QString databaseSettingsManager::getDatabaseName() const
{
    return m_databaseFileName;
}

QString databaseSettingsManager::getDatabasePath() const
{
    return m_databaseFilePath;
}
