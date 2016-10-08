#ifndef DATABASEMANAGERINTERFACE_H
#define DATABASEMANAGERINTERFACE_H

#include <QObject>
#include <QtDBus>

#include "homeAlarmInfo.h"
#include "smartHomeInfo.h"

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
    void insertIO(const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&);
    void getAllZones();
    void getAllCategories();
    QStringList getAllFromZone(const QString&) const;
    QStringList getAllFromCategory(const QString&) const;

signals:
    void zonesReceived(const QStringList&);
    void categoriesReceived(const QStringList&);

private slots:
    void insertHomeAlarmEntryFinished(QDBusPendingCallWatcher*);
    void insertSmartHomeEntryFinished(QDBusPendingCallWatcher*);
    void insertIOFinished(QDBusPendingCallWatcher*);
    void getAllZonesFinished(QDBusPendingCallWatcher*);
    void getAllCategoriesFinished(QDBusPendingCallWatcher*);

private:
    QDBusInterface *m_iface;
};

#endif // DATABASEMANAGERINTERFACE_H
