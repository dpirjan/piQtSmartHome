#include <QSslSocket>
#include <QTextStream>

#include "mailManager.h"

mailManager::mailManager(const QString &serverName,
                         const QString &userName,
                         const QString &password,
                         const int &port,
                         const int &timeout)
    : m_serverName(serverName)
    , m_userName(userName)
    , m_password(password)
    , m_port(port)
    , m_timeout(timeout)
{
    socket = new QSslSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

mailManager::~mailManager()
{
    qDebug() << "mailManager destructor";
    finished();
    delete textStream;
    delete socket;
}

void mailManager::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() <<"stateChanged() - " << socketState;
}

void mailManager::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << "errorReceived() - " <<socketError;
}

void mailManager::disconnected()
{
    qDebug() << "disconnected() - "  << socket->errorString();
}

void mailManager::connected()
{
    qDebug() << "connected()";
}

void mailManager::readyRead()
{
    qDebug() << "readyRead: "  << socket->errorString();
}

void mailManager::sendMail(const QString &sender,
                           const QString &recipient,
                           const QString &subject,
                           const QString &message)
{
    m_message = "To: " + recipient + "\n";
    m_message.append("From: " + sender + "\n");
    m_message.append("Subject: " + subject + "\n");
    m_message.append("Mime-Version: 1.0;\n");
    m_message.append("Content-Type: text/html; charset=\"utf8\";\n");
    m_message.append("Content-Transfer-Encoding: 8bit;\n");
    m_message.append("\n");
    m_message.append(message);
    m_message.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    m_message.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));
}
