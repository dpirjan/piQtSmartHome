#include <QCoreApplication>

#include "piHomeCommon.h"
#include "alarmManager.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logHandler);
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("alarmManager");
    qDebug() << "Alarm Manager";

    QList <int> quitSignals = QList <int>()
            << SIGQUIT << SIGINT << SIGTERM << SIGHUP << SIGSEGV;
    catchUnixSignal(quitSignals);

    bool firstTimeAlarm = AlarmManager::firstRunConfiguration();
    if(firstTimeAlarm)
    {
        qDebug() << "Application started for the first time, initializing, then quit.";
        qDebug() << "Please customize your configuration and then start again the application.";
        return 0;
    }

    AlarmManager am;
    am.connectService();

    return app.exec();
}
