#ifndef USERMANAGERINTERFACE_H
#define USERMANAGERINTERFACE_H

#include <QObject>
#include <QtDBus>

class QDBusInterface;

class UserManagerInterface : public QObject
{
    Q_OBJECT

    explicit UserManagerInterface(QObject *parent = 0);
    virtual ~UserManagerInterface();
    Q_DISABLE_COPY(UserManagerInterface)

    bool connectToDBus();

public:
    static UserManagerInterface &instance()
    {
        static UserManagerInterface obj;
        return obj;
    }

public slots:
    QStringList getAllUserNames() const;
    quint32 getLoginTimeout() const;

    bool checkPasswordForUser(const QString&, const QString&) const;
    bool checkPinForUser(const QString&, const QString&) const;

private:
    QDBusInterface *m_iface;
};

#endif // USERMANAGERINTERFACE_H
