#ifndef ACTUATORINFO_H
#define ACTUATORINFO_H

#include <QString>

#include <QMetaType>
#include <QDBusMetaType>

class ActuatorInfo
{
public:
    explicit ActuatorInfo();
    ActuatorInfo(const QString&,
                 const QString&,
                 const QString&,
                 const QString&,
                 const QString&);
    ActuatorInfo(const QString&,
                 const QString&,
                 const QString&,
                 const QString&,
                 const QString&,
                 const QString&);
    ActuatorInfo(const ActuatorInfo&);
    ActuatorInfo& operator=(const ActuatorInfo&);
    virtual ~ActuatorInfo();

    friend QDBusArgument &operator<<(QDBusArgument&,
                                     const ActuatorInfo&);
    friend const QDBusArgument &operator>>(const QDBusArgument&,
                                           ActuatorInfo&);

    static void registerMetaType();

    QString getZone() const;
    QString getNode() const;
    QString getCategory() const;
    QString getAddress() const;
    QString getValue() const;
    QString getTimestamp() const;

    void setValue(const QString&);

private:
    QString m_zone;
    QString m_node;
    QString m_category;
    QString m_address;
    QString m_value;
    QString m_timestamp;

    void setZone(const QString&);
    void setNode(const QString&);
    void setCategory(const QString&);
    void setAddress(const QString&);
    void setTimestamp(const QString&);

    template<class T> friend class QList;
};

Q_DECLARE_METATYPE(ActuatorInfo)

#endif // ACTUATORINFO_H
