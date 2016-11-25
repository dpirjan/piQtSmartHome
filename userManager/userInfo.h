#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>

class UserInfo
{
public:
    explicit UserInfo(const QString&,
                      const QString&,
                      const QString&,
                      const QStringList&);
    UserInfo(const UserInfo&);
    UserInfo& operator=(const UserInfo&);
    virtual ~UserInfo();

public slots:
    QString getUserName() const;
    QString getPassword() const;
    QString getPin() const;
    QStringList getMACs() const;

private:
    QString m_userName;
    QString m_password;
    QString m_pin;
    QStringList m_macList;
};

#endif // USERINFO_H
