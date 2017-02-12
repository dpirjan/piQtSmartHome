#include <QCoreApplication>

#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>

#include "piHomeCommon.h"
#include "webManager.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logHandler);
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("webManager");
    qDebug() << "Web Manager";

    QList <int> quitSignals = QList <int>()
            << SIGQUIT << SIGINT << SIGTERM << SIGHUP << SIGSEGV;
    catchUnixSignal(quitSignals);

    bool firstTimeWebServer = WebManager::firstRunConfiguration();
    if(firstTimeWebServer)
    {
        qDebug() << "Application started for the first time, initializing, then quit.";
        qDebug() << "Please customize your configuration and then start again the application.";
        return 0;
    }

    WebManager wm;

    return app.exec();
}
