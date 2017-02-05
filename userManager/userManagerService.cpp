#include <QCoreApplication>

#include "piHomeCommon.h"
#include "userManager.h"

#include <unistd.h>

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logHandler);
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("userManager");
    qDebug() << "Login Manager";

    QList <int> quitSignals = QList <int>()
            << SIGQUIT << SIGINT << SIGTERM << SIGHUP << SIGSEGV;
    catchUnixSignal(quitSignals);


    bool firstTime = UserManager::firstRunConfiguration();
    if(firstTime)
    {
        qDebug() << "Application started for the first time, initializing, then quit.";
        qDebug() << "Please customize your configuration set Encrypt to true and then start again the application.";
        return 0;
    }

    UserManager um;
    um.connectService();

//    qDebug() << "Password test======";
//    sleep(1);
//    um.checkPasswordForUser("User1", "Password1");
//    um.checkPasswordForUser("User1", "Passworld1");
//    um.checkPasswordForUser("Usr1", "Password1");
//    um.checkPasswordForUser("Usr1", "Passworld1");

//    qDebug() << "Pin test--------------------";
//    um.checkPinForUser("User1", "1234");
//    um.checkPinForUser("User1", "12345");
//    um.checkPinForUser("Usr1", "1234");
//    um.checkPinForUser("Usr1", "134");

//    qDebug() << um.getAllUsernames();

    return app.exec();
}
