#ifndef IO_H
#define IO_H

#include <QMetaType>
#include <QDBusMetaType>

#include "common.h"

class io
{
public:
    explicit io();
    io(const QString&,
                const QString&,
                const QString&,
                const QString&,
                const QString&,
                const QString&,
                const QString&);
    io(const io&);
    io& operator=(const io&);
    virtual ~io();

    friend QDBusArgument &operator<<(QDBusArgument&, const io&);
    friend const QDBusArgument &operator>>(const QDBusArgument&, io&);

    static void registerMetaType();

    QString getSystemType() const;
    QString getHardwareType() const;
    QString getType() const;
    QString getCategory() const;
    QString getZone() const;
    QString getNode() const;
    QString getAddress() const;

    void setSystemType(const QString&);
    void setHardwareType(const QString&);
    void setType(const QString&);
    void setCategory(const QString&);
    void setZone(const QString&);
    void setNode(const QString&);
    void setAddress(const QString&);

private:
    QString m_system;
    QString m_hardware;
    QString m_type;
    QString m_category;
    QString m_zone;
    QString m_node;
    QString m_address;

    template<class T> friend class QList;
};

Q_DECLARE_METATYPE(io)

#endif // IO_H
