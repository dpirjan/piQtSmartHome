#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QtDBus>

#include "databaseSettingsManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("settingsManager");
    qDebug() << "Settings Manager";

    databaseSettingsManager dsm;

    if(!dsm.connectService())
        qDebug() << "Cannot register the database manager settings service to session bus";

//  dsm.saveSettings();
    dsm.loadSettings();

    qDebug() << "main:" << dsm.getDatabaseName();
    qDebug() << "main:" << dsm.getDatabasePath();

    int ret = app.exec();

    return ret;
}
