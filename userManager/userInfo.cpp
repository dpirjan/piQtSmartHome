#include <QDebug>

#include "userInfo.h"

UserInfo::UserInfo(const QString &username,
                   const QString &password,
                   const QString &pin,
                   const QStringList &MACs)
    : m_userName(username)
    , m_password(password)
    , m_pin(pin)
    , m_macList(MACs)
{
    qDebug() << "UserInfo constructor";
}

UserInfo::UserInfo(const UserInfo &obj)
{
    m_userName = obj.m_userName;
    m_password = obj.m_password;
    m_pin = obj.m_pin;
    m_macList = obj.m_macList;
}

UserInfo& UserInfo::operator =(const UserInfo &obj)
{
    m_userName = obj.m_userName;
    m_password = obj.m_password;
    m_pin = obj.m_pin;
    m_macList = obj.m_macList;

    return *this;
}

UserInfo::~UserInfo()
{
}

QString UserInfo::getUserName() const
{
    return m_userName;
}

QString UserInfo::getPassword() const
{
    return m_password;
}

QString UserInfo::getPin() const
{
    return m_pin;
}

QStringList UserInfo::getMACs() const
{
    return m_macList;
}
