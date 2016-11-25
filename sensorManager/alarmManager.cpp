#include <QDBusConnection>
#include <QDBusError>

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "alarmManager.h"

AlarmManager::AlarmManager(QObject *parent) : QObject(parent)
{
    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("alarm.ini");
    m_settings = new QSettings(settingsPath, QSettings::NativeFormat);

    loadAlarmState();
}

AlarmManager::~AlarmManager()
{
    saveAlarmState();

    // settings file no longer needed
    delete m_settings;
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

    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("alarm.ini");

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;
        QSettings *settings = new QSettings(settingsPath,
                                            QSettings::NativeFormat);

        // writing default values to the configuration file
        settings->clear();
        settings->beginGroup("GeneralSettings");
        settings->setValue("GeneralState", false);
        settings->setValue("NightState", false);
        settings->setValue("VacationState", false);
        settings->endGroup();

        delete settings;
    }

    return returnCode;
}

void AlarmManager::loadAlarmState()
{
    m_settings->beginGroup("GeneralSettings");
    m_alarmGeneralState = m_settings->value("GeneralState").toBool();
    m_alarmNightState = m_settings->value("NightState").toBool();
    m_alarmVacationState = m_settings->value("VacationState").toBool();
    m_settings->endGroup();
}

void AlarmManager::saveAlarmState()
{
    m_settings->beginGroup("GeneralSettings");
    m_settings->setValue("GeneralState", m_alarmGeneralState);
    m_settings->setValue("NightState", m_alarmNightState);
    m_settings->setValue("VacationState", m_alarmVacationState);
    m_settings->endGroup();
}

void AlarmManager::setAlarmGeneralState(const bool &state)
{
    m_alarmGeneralState = state;
}

void AlarmManager::setAlarmNightState(const bool &state)
{
    m_alarmNightState = state;
}

void AlarmManager::setAlarmVacationState(const bool &state)
{
    m_alarmVacationState = state;
}

bool AlarmManager::getAlarmGeneralState() const
{
    return m_alarmGeneralState;
}

bool AlarmManager::getAlarmNightState() const
{
    return m_alarmNightState;
}

bool AlarmManager::getAlarmVacationState() const
{
    return m_alarmVacationState;
}
