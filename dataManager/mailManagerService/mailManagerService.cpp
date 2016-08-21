#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTimer>

#include "mailManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("mailManager");

    qDebug() << "Mail Manager";

    if(mailManager::firstRunConfiguration())
    {
        qDebug() << "Application started for the first time, initializing, then quit.";
        qDebug() << "Please customize your configuration and then start again the application.";
        return 0;
    }

//    //Used for testing
//    mailManager::instance().saveServerCredentials();
//    mailManager::instance().saveSendMailDetails();

    if(!mailManager::instance().connectService())
        qDebug() << "Cannot register the mail manager service to session bus";

//    if(!mailManager::instance().connectToServer())
//        qDebug() << "Cannot connect to server";

//    if(!mailManager::instance().loginToServer())
//        qDebug() << "Cannot login to server";

//    if(!mailManager::instance().sendMail("Test",
//                     "Test message 4"))
//        qDebug() << "Cannot send mail to server";

//    if(!mailManager::instance().disconnectFromServer())
//        qDebug() << "Cannot disconnect from server";

    return app.exec();
}
