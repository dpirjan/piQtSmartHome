#ifndef DATABASEMANAGERINTERFACE_H
#define DATABASEMANAGERINTERFACE_H

#include <QObject>
#include <QtDBus>

#include "homeAlarmInfo.h"
#include "smartHomeInfo.h"
#include "actuatorInfo.h"
#include "io.h"

class QDBusInterface;

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
    void insertHomeAlarmEntry(const HomeAlarmInfo&);
    void insertSmartHomeEntry(const SmartHomeInfo&);
    void insertActuatorInfoEntry(const ActuatorInfo&);
    void insertIO(const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&);
    QList<HomeAlarmInfo> getAllHomeAlarmEntries() const;
    QList<SmartHomeInfo> getAllSmartHomeEntries() const;
    QList<ActuatorInfo> getAllActuatorInfoEntries() const;
    QList<HomeAlarmInfo> getHomeAlarmEntriesForIO(const io&) const;
    QList<SmartHomeInfo> getSmartHomeEntriesForIO(const io&) const;
    QList<ActuatorInfo> getActuatorInfoEntriesForIO(const io&) const;
    void getAllZones();
    void getAllCategories();
    QList<io> getAllFromZone(const QString&) const;
    QList<io> getAllFromCategory(const QString&) const;

signals:
    void zonesReceived(const QStringList&);
    void categoriesReceived(const QStringList&);

private slots:
    void insertHomeAlarmEntryFinished(QDBusPendingCallWatcher*);
    void insertSmartHomeEntryFinished(QDBusPendingCallWatcher*);
    void insertActuatorInfoEntryFinished(QDBusPendingCallWatcher*);
    void insertIOFinished(QDBusPendingCallWatcher*);
    void getAllZonesFinished(QDBusPendingCallWatcher*);
    void getAllCategoriesFinished(QDBusPendingCallWatcher*);

private:
    QDBusInterface *m_iface;
};

#endif // DATABASEMANAGERINTERFACE_H
