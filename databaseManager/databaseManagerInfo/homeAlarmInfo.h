#ifndef HOMEALARMINFO_H
#define HOMEALARMINFO_H

#include <QString>

#include <QMetaType>
#include <QDBusMetaType>

class HomeAlarmInfo
{
public:
    explicit HomeAlarmInfo();
    HomeAlarmInfo(const QString&,
                  const QString&,
                  const QString&);
    HomeAlarmInfo(const QString&,
                  const QString&,
                  const QString&,
                  const QString&);
    HomeAlarmInfo(const HomeAlarmInfo&);
    HomeAlarmInfo& operator=(const HomeAlarmInfo&);
    virtual ~HomeAlarmInfo();

    friend QDBusArgument &operator<<(QDBusArgument&,
                                     const HomeAlarmInfo&);
    friend const QDBusArgument &operator>>(const QDBusArgument&,
                                           HomeAlarmInfo&);

    static void registerMetaType();


    QString getZone() const;
    QString getNode() const;
    QString getSensor() const;
    QString getTimestamp() const;

    void setZone(const QString&);
    void setNode(const QString&);
    void setSensor(const QString&);
    void setTimestamp(const QString&);

private:
    QString m_zone;
    QString m_node;
    QString m_sensor;
    QString m_timestamp;

    template<class T> friend class QList;
};

Q_DECLARE_METATYPE(HomeAlarmInfo)

#endif // HOMEALARMINFO_H
