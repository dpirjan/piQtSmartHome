#ifndef USERRMANAGER_H
#define USERMANAGER_H

#include <QObject>

#include "watchdogHelper.h"
#include "simplecrypt.h"

#define USER_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.UserManager"
#define USER_MANAGER_SERVICE_PATH "/"
#define USER_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.UserManager.UserOperations"

class UserInfo;

class UserManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", USER_MANAGER_SERVICE_INT)

public:
    explicit UserManager(QObject *parent = 0);
    virtual ~UserManager();

    bool connectService();

    static bool firstRunConfiguration();

public slots:
    Q_SCRIPTABLE QStringList getAllUserNames() const;
    Q_SCRIPTABLE quint32 getLoginTimeout() const;

    Q_SCRIPTABLE bool checkPasswordForUser(const QString&, const QString&) const;
    Q_SCRIPTABLE bool checkPinForUser(const QString&, const QString&) const;

private:
    void loadUserSettings();

    WatchdogHelper *m_watchdog;

    QList<UserInfo *> m_userList;

    static SimpleCrypt *m_crypto;
    bool m_enableEncryption;
    bool m_toEncrypt;
    quint32 m_loginTimeout;
};

#endif // USERMANAGER_H
