#ifndef SMARTHOMEINFO_H
#define SMARTHOMEINFO_H

#include <QString>

#include <QMetaType>
#include <QDBusMetaType>

class SmartHomeInfo
{
public:
    explicit SmartHomeInfo();
    SmartHomeInfo(const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&);
    SmartHomeInfo(const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&,
                  const QString&);
    SmartHomeInfo(const SmartHomeInfo&);
    SmartHomeInfo& operator=(const SmartHomeInfo&);
    virtual ~SmartHomeInfo();

    friend QDBusArgument &operator<<(QDBusArgument&,
                                     const SmartHomeInfo&);
    friend const QDBusArgument &operator>>(const QDBusArgument&,
                                           SmartHomeInfo&);

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

Q_DECLARE_METATYPE(SmartHomeInfo)

#endif // SMARTHOMEINFO_H
