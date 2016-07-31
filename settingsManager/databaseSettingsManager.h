#ifndef DATABASESETTINGSMANAGER_H
#define DATABASESETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QDBusVariant>

#define DATABASE_SETTINGS_SERVICE_NAME "org.raspberrypi.piHome.SettingsManager"
#define DATABASE_SETTINGS_SERVICE_PATH "/"
#define DATABASE_SETTINGS_SERVICE_INT  "org.raspberrypi.piHome.SettingsManager.DatabaseSettings"

class databaseSettingsManager : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", DATABASE_SETTINGS_SERVICE_INT)

public:
    explicit databaseSettingsManager(QObject *parent = 0);
    virtual ~databaseSettingsManager();

    bool connectService();

    void loadSettings();
    void saveSettings();

public slots:
    Q_SCRIPTABLE QString getDatabasePath() const;
    Q_SCRIPTABLE QString getDatabaseName() const;

private:
    QString m_databaseFilePath;
    QString m_databaseFileName;

    QSettings *m_settings;
};

#endif // DATABASESETTINGSMANAGER_H
