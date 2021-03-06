
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>

#include <QDBusConnection>
#include <QDBusError>

#include "userManager.h"
#include "userInfo.h"
#include "simplecrypt.h"

SimpleCrypt *UserManager::m_crypto =
        new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023)); // @TODO update the key


UserManager::UserManager(QObject *parent) : QObject(parent)
{
    UserManager::m_crypto->setIntegrityProtectionMode(
                SimpleCrypt::ProtectionHash);
    UserManager::m_crypto->setCompressionMode(
                SimpleCrypt::CompressionAlways);

    loadUserSettings();

    m_watchdog = new WatchdogHelper("piHomeLogin");
    m_watchdog->init();
}

UserManager::~UserManager()
{
    for(int count = 0; count < m_userList.count(); count++)
        delete m_userList.takeAt(count);

    m_watchdog->stop();
    m_watchdog->deleteLater();
}

bool UserManager::connectService()
{
    bool ret = true;

    if(!QDBusConnection::systemBus().registerService(
                USER_MANAGER_SERVICE_NAME))
    {
        qCritical() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    QDBusConnection::systemBus().registerObject(
                USER_MANAGER_SERVICE_PATH,
                this,
                QDBusConnection::ExportScriptableContents);

    if(ret == true)
        qDebug() << "Registered the user manager service to DBUS system bus";

    return ret;
}

bool UserManager::firstRunConfiguration()
{
    bool returnCode = false;

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).
            append(QDir::separator()).
            append(QCoreApplication::organizationName()).
            append(QDir::separator()).
            append(QCoreApplication::applicationName()).
            append(".conf");
    qDebug() << "Path: " << settingsPath;

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;
        QSettings settings(settingsPath,QSettings::NativeFormat);

        // writing default values to the configuration file
        settings.clear();
        settings.beginGroup("GeneralSettings");
        settings.setValue("UserNumber", 3);
        settings.setValue("Encrypt", false);
        settings.setValue("ClearPassword", true);
        settings.setValue("LoginTimeout", 120);
        settings.endGroup();
        settings.beginGroup("User_1");
        settings.setValue("Username", "User1");
        settings.setValue("Password", "Password1");
        settings.setValue("Pin", "1234");
        settings.beginGroup("MAC");
        settings.setValue("Number", 2);
        settings.setValue("MAC_1", "A1:BB:CC:DD:EE:FF");
        settings.setValue("MAC_2", "A2:BB:CC:DD:EE:FF");
        settings.endGroup();
        settings.endGroup();
        settings.beginGroup("User_2");
        settings.setValue("Username", "User2");
        settings.setValue("Password", "Password2");
        settings.setValue("Pin", "2345");
        settings.beginGroup("MAC");
        settings.setValue("Number", 1);
        settings.setValue("MAC_1", "B1:CC:DD:EE:FF:11");
        settings.endGroup();
        settings.endGroup();
        settings.beginGroup("User_3");
        settings.setValue("Username", "User3");
        settings.setValue("Password", "Password3");
        settings.setValue("Pin", "3456");
        settings.beginGroup("MAC");
        settings.setValue("Number", 3);
        settings.setValue("MAC_1", "C1:DD:EE:FF:11:22");
        settings.setValue("MAC_2", "C2:DD:EE:FF:11:22");
        settings.setValue("MAC_3", "C3:DD:EE:FF:11:22");
        settings.endGroup();
        settings.endGroup();
    }

    return returnCode;
}

void UserManager::loadUserSettings()
{
    QSettings settings;
    settings.beginGroup("GeneralSettings");
    quint8 numUsers = settings.value("UserNumber").toUInt();
    m_enableEncryption = settings.value("Encrypt").toBool();
    m_toEncrypt = settings.value("ClearPassword").toBool();
    m_loginTimeout = settings.value("LoginTimeout").toUInt();
    settings.endGroup();

    for(int i = 1; i <= numUsers; i++) // for humans not machines
    {
        QString group = "User_";
        QStringList tmpMAC;
        group.append(QString::number(i));
        settings.beginGroup(group);
        QString username = settings.value("Username").toString();
        QString password = settings.value("Password").toString();
        QString pin = settings.value("Pin").toString();

        if(m_enableEncryption && m_toEncrypt)
        {
            // This means that last application run, the configuration was updated
            // and now the passwords are in clear format. Since encryption is enabled
            // passwords must be encrypted
            QString encrypted = UserManager::m_crypto->encryptToString(password);
            settings.setValue("Password",
                                 encrypted);
            password = encrypted;
            encrypted = UserManager::m_crypto->encryptToString(pin);
            settings.setValue("Pin", encrypted);
            pin = encrypted;
        }

        qDebug() << i << ". Username: " << username;
        qDebug() << i << ". Password: " << password;
        qDebug() << i << ". Pin: " << pin;
        settings.beginGroup("MAC");
        quint8 numMAC;
        numMAC = settings.value("Number").toUInt();
        for(int j = 1; j <= numMAC; j++)
        {
            QString subGroup = "MAC_";
            qDebug()  << i << ". MAC " << j << ":"
                      << settings.value(subGroup.append(QString::number(j))).toString();
            tmpMAC.append(settings.value(subGroup.append(QString::number(j))).toString());
        }
        UserInfo *tmp = new UserInfo(username,
                                     password,
                                     pin,
                                     tmpMAC);
        m_userList.append(tmp);
        settings.endGroup();
        settings.endGroup();
    }
    settings.sync();

    if(m_toEncrypt && m_enableEncryption)
    {
        settings.beginGroup("GeneralSettings");
        settings.setValue("ClearPassword", false);
        settings.endGroup();
        m_toEncrypt = false;
    }
}

QStringList UserManager::getAllUserNames() const
{
    QStringList tmp;
    for(int count = 0; count < m_userList.count(); count++)
        tmp.append(m_userList.at(count)->getUserName());
    return tmp;
}

quint32 UserManager::getLoginTimeout() const
{
    return m_loginTimeout;
}

bool UserManager::checkPasswordForUser(const QString &user,
                                       const QString &passwd) const
{
    bool ret = false;
    bool found = false;
    for(int count = 0; count < m_userList.count(); count++)
    {
        if(m_userList.at(count)->getUserName() == user)
        {
            found = true;
            if(UserManager::m_crypto->decryptToString(m_userList.at(count)->getPassword()) == passwd)
                ret = true;
        }
    }

    if(!found)
        qCritical() << "Username " << user << "provided not found!";

    if(!ret)
        qCritical() << "Password for user " << user << " doesn't match!";

    return ret;
}

bool UserManager::checkPinForUser(const QString &user,
                                  const QString &pin) const
{
    bool ret = false;
    bool found = false;
    for(int count = 0; count < m_userList.count(); count++)
    {
        if(m_userList.at(count)->getUserName() == user)
        {
            found = true;
            if(UserManager::m_crypto->decryptToString(m_userList.at(count)->getPin()) == pin)
                ret = true;
        }
    }

    if(!found)
        qCritical() << "Username " << user << "provided not found!";

    if(!ret)
        qCritical() << "Pin for user " << user << " doesn't match!";

    return ret;
}
