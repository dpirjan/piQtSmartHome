#include <QDBusConnection>
#include <QDBusError>

#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>

#include <QCoreApplication>

#include "alarmManager.h"

AlarmManager::AlarmManager(QObject *parent) : QObject(parent)
{
    loadAlarmState();
}

AlarmManager::~AlarmManager()
{
    saveAlarmState();
}

bool AlarmManager::connectService()
{
    bool ret = true;

    if(!QDBusConnection::systemBus().registerService(
                ALARM_MANAGER_SERVICE_NAME))
    {
        qCritical() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    QDBusConnection::systemBus().registerObject(
                ALARM_MANAGER_SERVICE_PATH,
                this,
                QDBusConnection::ExportScriptableContents);

    if(ret == true)
        qDebug() << "Registered the alarm manager service to DBUS system bus";

    return ret;
}

bool AlarmManager::firstRunConfiguration()
{
    bool returnCode = false;

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).
            append(QDir::separator()).
            append(QCoreApplication::organizationName()).
            append(QDir::separator()).
            append(QCoreApplication::applicationName()).
            append(".conf");
    qDebug() << "Path: " << settingsPath;

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;
        QSettings settings(settingsPath, QSettings::NativeFormat);

        settings.clear();
        // writing default values to the configuration file
        settings.beginGroup("GeneralSettings");
        settings.setValue("GeneralState", "false");
        settings.setValue("NightState", "false");
        settings.setValue("VacationState", "false");
        settings.endGroup();
    }

    return returnCode;
}

void AlarmManager::loadAlarmState()
{
    QSettings settings;
    settings.beginGroup("GeneralSettings");
    m_alarmGeneralState = settings.value("GeneralState").toBool();
    m_alarmNightState = settings.value("NightState").toBool();
    m_alarmVacationState = settings.value("VacationState").toBool();
    settings.endGroup();

    qDebug() << "load state: " << m_alarmGeneralState << ", " <<
                m_alarmNightState << ", " << m_alarmVacationState;
}

void AlarmManager::saveAlarmState()
{
    QSettings settings;
    settings.beginGroup("GeneralSettings");
    settings.setValue("GeneralState", m_alarmGeneralState);
    settings.setValue("NightState", m_alarmNightState);
    settings.setValue("VacationState", m_alarmVacationState);
    settings.endGroup();
}

void AlarmManager::setAlarmGeneralState(const bool &state)
{
    qDebug() << m_alarmGeneralState << "/" << state;
    m_alarmGeneralState = state;
}

void AlarmManager::setAlarmNightState(const bool &state)
{
    qDebug() << m_alarmNightState << "/" << state;
    m_alarmNightState = state;
}

void AlarmManager::setAlarmVacationState(const bool &state)
{
    qDebug() << m_alarmVacationState << "/" << state;
    m_alarmVacationState = state;
}

bool AlarmManager::getAlarmGeneralState() const
{
    qDebug() << m_alarmGeneralState;
    return m_alarmGeneralState;
}

bool AlarmManager::getAlarmNightState() const
{
    qDebug() << m_alarmNightState;
    return m_alarmNightState;
}

bool AlarmManager::getAlarmVacationState() const
{
    qDebug() << m_alarmVacationState;
    return m_alarmVacationState;
}
