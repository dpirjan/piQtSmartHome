#ifndef SETTINGSMANAGERINTERFACE_H
#define SETTINGSMANAGERINTERFACE_H

#include <QObject>
#include <QDBusInterface>
#include <QtDBus>

#define DATABASE_SETTINGS_SERVICE_NAME "org.raspberrypi.piHome.SettingsManager"
#define DATABASE_SETTINGS_SERVICE_PATH "/"
#define DATABASE_SETTINGS_SERVICE_INT  "org.raspberrypi.piHome.SettingsManager.DatabaseSettings"

class SettingsManagerInterface : public QObject
{
    Q_OBJECT

    explicit SettingsManagerInterface(QObject *parent = 0);
    virtual ~SettingsManagerInterface();
    Q_DISABLE_COPY(SettingsManagerInterface)

    bool connectToDBus();

public:
    static SettingsManagerInterface &instance()
    {
        static SettingsManagerInterface obj;
        return obj;
    }

public slots:
    QString getDatabasePath() const;
    QString getDatabaseName() const;

private:
    QDBusInterface *iface;
};

#endif // SETTINGSMANAGERINTERFACE_H
