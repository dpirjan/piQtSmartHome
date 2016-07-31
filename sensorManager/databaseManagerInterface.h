#ifndef DATABASEMANAGERINTERFACE_H
#define DATABASEMANAGERINTERFACE_H

#include <QObject>
#include <QDBusInterface>
#include <QtDBus>

#include "homeAlarmInfo.h"
#include "smartHomeInfo.h"

#define DATABASE_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.DatabaseManager"
#define DATABASE_MANAGER_SERVICE_PATH "/"
#define DATABASE_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.DatabaseManager.DatabaseOperations"

class databaseManagerInterface : public QObject
{
    Q_OBJECT

    explicit databaseManagerInterface(QObject *parent = 0);
    virtual ~databaseManagerInterface();
    Q_DISABLE_COPY(databaseManagerInterface)

    bool connectToDBus();

public:
    static databaseManagerInterface &instance()
    {
        static databaseManagerInterface obj;
        return obj;
    }

public slots:
    bool insertHomeAlarmEntry(const HomeAlarmInfo&);
    bool insertSmartHomeEntry(const SmartHomeInfo&);

private:
    QDBusInterface *iface;
};

#endif // DATABASEMANAGERINTERFACE_H
