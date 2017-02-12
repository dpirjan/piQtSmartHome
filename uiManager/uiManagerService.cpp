#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QDir>
#include <QQuickStyle>
#include <QDebug>
#include <QStandardPaths>

#include "piHomeCommon.h"
#include "UIManager.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logHandler);
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication::setApplicationName("uiManager");
    QGuiApplication::setOrganizationName("piHome");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qDebug() << "UI Manager";

    QList <int> quitSignals = QList <int>()
            << SIGQUIT << SIGINT << SIGTERM << SIGHUP << SIGSEGV;
    catchUnixSignal(quitSignals);

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).
            append(QDir::separator()).
            append(QCoreApplication::organizationName()).
            append(QDir::separator()).
            append(QCoreApplication::applicationName()).
            append(".conf");
    if(!QFile(settingsPath).exists())
    {
        qDebug() << "Application started for the first time, initializing, then quit.";
        qDebug() << "Please customize your configuration and then start again the application.";
        //Only setting style as Material
        QSettings settings(settingsPath, QSettings::NativeFormat);
        settings.setValue("style", "Material");
        return 0;
    }

    QSettings settings;
    qDebug() << settings.fileName();
    QString style = QQuickStyle::name();
    qDebug() << style << " : " << settings.value("style").toString();

    if (!style.isEmpty())
        settings.setValue("style", style);
    else
        QQuickStyle::setStyle(settings.value("style").toString());

    UIManager uim;
    uim.init();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(
                "zonesModel",
                uim.getZonesModel());
    engine.rootContext()->setContextProperty(
                "categoriesModel",
                uim.getCategoriesModel());
    engine.rootContext()->setContextProperty(
                "ioModel",
                uim.getIOModel());
    engine.rootContext()->setContextProperty(
                "alarmEventsModel",
                uim.getAlarmEventModel());
    engine.rootContext()->setContextProperty(
                "smartHomeEventsModel",
                uim.getSmartHomeEventModel());
    engine.rootContext()->setContextProperty(
                "actuatorEventsModel",
                uim.getActuatorEventModel());
    engine.rootContext()->setContextProperty(
                "UIM",
                &uim);
    engine.load(QUrl(QLatin1String("qrc:/qml/MainPage.qml")));

    if(engine.rootObjects().isEmpty())
        qFatal("RootObjects empty!");

    return app.exec();
}
