#include <QCoreApplication>

#include "utils.h"
#include "sensorManager.h"
#include "sensorInstantiator.h"
#include "actuatorInstantiator.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logHandler);
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("sensorManager");
    qDebug() << "Sensor Manager";

    QList <int> quitSignals = QList <int>()
            << SIGQUIT << SIGINT << SIGTERM << SIGHUP << SIGSEGV;
    catchUnixSignal(quitSignals);

    bool firstTimeSensors = sensorInstantiator::firstRunInitSensors();
    bool firstTimeActuators = actuatorInstantiator::firstRunInitActuators();
    if(firstTimeActuators || firstTimeSensors)
    {
        qDebug() << "Application started for the first time, initializing, then quit.";
        qDebug() << "Please customize your configuration and then start again the application.";
        return 0;
    }

    SensorManager sm;
    sm.init();

    return app.exec();
}
