#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QDir>
#include <QQuickStyle>
#include <QDebug>

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

    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".config").
            append(QDir::separator()).
            append("piHome").
            append(QDir::separator());
    QFile settingsFile(settingsPath.append("uiManager.conf"));
    if(!settingsFile.exists())
    {
        qDebug() << "Application started for the first time, initializing, then quit.";
        qDebug() << "Please customize your configuration and then start again the application.";
        //Only create an empty file
        settingsFile.open(QIODevice::WriteOnly);
        settingsFile.close();
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
    engine.load(QUrl(QLatin1String("qrc:/qml/MainPage.qml")));
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
                "UIM",
                &uim);

    if(engine.rootObjects().isEmpty())
        qFatal("RootObjects empty!");

    return app.exec();
}
