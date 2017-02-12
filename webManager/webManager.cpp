#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>

#include <QCoreApplication>

#include "webManager.h"

#include "httplistener.h"
#include "requestmapper.h"

using namespace stefanfrings;

WebManager::WebManager(QObject *parent) : QObject(parent)
{
    loadWebServerSettings();
}

WebManager::~WebManager()
{
}

bool WebManager::firstRunConfiguration()
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
        settings.beginGroup("listener");
        settings.setValue("port", 8080);
        settings.setValue("minThreads", 4);
        settings.setValue("maxThreads", 100);
        settings.setValue("cleanupInterval", 60000);
        settings.setValue("readTimeout", 60000);
        settings.setValue("maxRequestSize", 16000);
        settings.setValue("maxMultiPartSize", 10000000);
        settings.setValue("sslKeyFile", "ssl/my.key");
        settings.setValue("sslCertFile", "ssl/my.cert");
        settings.endGroup();
        settings.beginGroup("sessions");
        settings.setValue("expirationTime", 3600000);
        settings.setValue("cookieName", "sessionid");
        settings.setValue("cookiePath", "/");
        settings.setValue("cookieComment", "Identifies the user");
        settings.setValue("cookieDomain", "");
        settings.endGroup();
    }

    return returnCode;
}

void WebManager::loadWebServerSettings()
{
    QString configFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).
            append(QDir::separator()).
            append(QCoreApplication::organizationName()).
            append(QDir::separator()).
            append(QCoreApplication::applicationName()).
            append(".conf");
    qDebug() << "Path: " << configFileName;

    // Session store
    QSettings* sessionSettings=new QSettings(configFileName,QSettings::IniFormat);
    sessionSettings->beginGroup("sessions");
    RequestMapper::sessionStore=new HttpSessionStore(sessionSettings, this);

    // HTTP server
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings,new RequestMapper(this), this);
}
