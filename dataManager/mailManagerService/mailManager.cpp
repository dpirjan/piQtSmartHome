#include <QDBusConnection>
#include <QDBusError>

#include <QSslSocket>
#include <QTextStream>
#include <QDir>
#include <QSettings>

#include "mailManager.h"

#define SERVER_READY "220"
#define ACTION_COMPLETED "250"
#define USER_AUTH "334"
#define PASS_AUTH "235"
#define DATA_RESPONSE "354"
#define DISCONNECT_COMPLETED "221"

mailManager::mailManager(QObject *parent) : QObject(parent)
{
    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("smtp.ini");
    m_settings = new QSettings(settingsPath, QSettings::NativeFormat);

    loadServerCredentials();
    loadSendMailDetails();

    textStream = new QTextStream(socket);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,
            SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

mailManager::~mailManager()
{
    qDebug() << "mailManager destructor";
    delete textStream;
    delete socket;
    delete m_settings;
}

bool mailManager::firstRunConfiguration()
{
    bool returnCode = false;

    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("smtp.ini");

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;

        QSettings *settings = new QSettings(settingsPath, QSettings::NativeFormat);

        settings->clear();
        settings->beginGroup("ServerSettings");
        settings->setValue("ServerName", "smtp.mail.com");
        settings->setValue("User", "user@mail.com");
        settings->setValue("Password", "password"); //@TODO use encryption
        settings->setValue("Port", 465);
        settings->setValue("Timeout_ms","30000");
        settings->setValue("Protocol", SSL);
        settings->setValue("Login", Encrypted);
        settings->endGroup();

        settings->beginGroup("SendMailSettings");
        settings->setValue("Sender", "sender@mail.com");
        settings->setValue("NumberOfRecipients", 2);
        settings->setValue("Recipient_1", "recipient1@mail.com");
        settings->setValue("Recipient_2", "recipient2@mail.com");
        settings->endGroup();

        delete settings;
    }

    return returnCode;
}

void mailManager::loadServerCredentials()
{
    m_settings->beginGroup("ServerSettings");
    m_serverName = m_settings->value("ServerName").toString();
    m_userName = m_settings->value("User").toString();
    m_password = m_settings->value("Password").toString(); //@TODO use encryption
    m_port = m_settings->value("Port").toInt();
    m_timeout = m_settings->value("Timeout_ms").toInt();

    switch(m_settings->value("Protocol").toInt())
    {
    case None:
        m_connection = None;
        socket = new QTcpSocket(this);
        break;
    case SSL:
        m_connection = SSL;
        socket = new QSslSocket(this);
        break;
    case TLS:
        m_connection = TLS;
        socket = new QSslSocket(this);
        break;
    default:
        qDebug() << "Invalid connection type specified!";
        break;
    }

    switch (m_settings->value("Login").toInt())
    {
    case Plain:
        m_authentication = Plain;
        break;
    case Encrypted:
        m_authentication = Encrypted;
        break;
    default:
        qDebug() << "Invalid authenthication type specified!";
        break;
    }

    m_settings->endGroup();
}

void mailManager::loadSendMailDetails()
{
    int recipients = -1;
    m_settings->beginGroup("SendMailSettings");
    m_sender = m_settings->value("Sender").toString();
    recipients = m_settings->value("NumberOfRecipients").toInt();
    for(int count = 1; count <= recipients; count++)
        m_recipients.append(m_settings->value(QString("Recipient_").append(QString::number(count))).toString());
    m_settings->endGroup();
}

bool mailManager::connectService()
{
    bool ret = true;

    if (!QDBusConnection::systemBus().registerService(MAIL_MANAGER_SERVICE_NAME))
    {
        qDebug() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    QDBusConnection::systemBus().registerObject(MAIL_MANAGER_SERVICE_PATH,
                                                 this,
                                                 QDBusConnection::ExportScriptableContents);

    qDebug() << "Registered the mail manager service to DBUS system bus";

    return ret;
}

void mailManager::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() <<"stateChanged() - " << socketState;
}

void mailManager::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << "errorReceived() - " << socketError << ": " << socket->errorString();
}

void mailManager::disconnected()
{
    qDebug() << "disconnected() - "  << socket->errorString();
}

void mailManager::connected()
{
    qDebug() << "connected()";
}

void mailManager::sendToServer(const QString &text)
{
    *textStream << text << "\r\n";
    textStream->flush();
}

QString mailManager::replyFromServer()
{
    do
    {
        if(!socket->waitForReadyRead())
            qDebug() << "Communication timeout!!";

        QString lineResponse;
        while(socket->canReadLine())
        {
            lineResponse = socket->readLine();
            m_response += lineResponse;
        }

        if(lineResponse[3] == ' ')
        {
            lineResponse.truncate(3);
            qDebug() << "Reply: " << lineResponse;
            return lineResponse;
        }

    } while(true);

    return "";
}

bool mailManager::connectToServer()
{
    QString serverReplyCode;

    switch(m_connection)
    {
    case None:
    case TLS:
        socket->connectToHost(m_serverName, m_port);
        break;
    case SSL:
        ((QSslSocket *) socket)->connectToHostEncrypted(m_serverName, m_port);
        break;
    default:
        qDebug() << "Connection type not supported!";
        return false;
        break;
    }

    if(!socket->waitForConnected(m_timeout))
    {
        qDebug() << "Connection error: " << socket->errorString();
        return false;
    }

    serverReplyCode = replyFromServer();
    if(serverReplyCode != SERVER_READY)
    {
        qDebug() << "Cannot connect to server!";
        return false;
    }

    sendToServer("EHLO " + m_serverName); // "EHLO localhost"

    serverReplyCode = replyFromServer();

    if(serverReplyCode != ACTION_COMPLETED)
    {
        qDebug() << "No EHLO reply from server!";
        return false;
    }

    if(m_connection == TLS)
    {
        sendToServer("STARTTLS");

        serverReplyCode = replyFromServer();

        if(serverReplyCode != SERVER_READY)
        {
            qDebug() << "No STARTTLS reply from server!";
            return false;
        }

        ((QSslSocket*) socket)->startClientEncryption();

        if (!((QSslSocket*) socket)->waitForEncrypted(m_timeout))
        {
            qDebug() << ((QSslSocket*) socket)->errorString();
            return false;
        }

        sendToServer("EHLO " + m_serverName); // "EHLO localhost"

        serverReplyCode = replyFromServer();

        if(serverReplyCode != ACTION_COMPLETED)
        {
            qDebug() << "No EHLO encrypted reply from server!";
            return false;
        }
    }

    return true;
}

bool mailManager::loginToServer()
{
    QString serverReplyCode;

    switch(m_authentication)
    {
    case Plain:
        sendToServer("AUTH PLAIN " + QByteArray().append((char) 0).append(m_userName).append((char) 0).append(m_password).toBase64());

        serverReplyCode = replyFromServer();
        if(serverReplyCode != PASS_AUTH)
        {
            qDebug() << "No AUTH LOGIN reply from server!(Plain)";
            return false;
        }
        break;

    case Encrypted:
        sendToServer("AUTH LOGIN");

        serverReplyCode = replyFromServer();
        if(serverReplyCode != USER_AUTH)
        {
            qDebug() << "No AUTH LOGIN reply from server!(Login)";
            return false;
        }

        sendToServer(QByteArray().append(m_userName).toBase64());

        serverReplyCode = replyFromServer();
        if(serverReplyCode != USER_AUTH)
        {
            qDebug() << "No USER AUTH reply from server!";
            return false;
        }

        sendToServer(QByteArray().append(m_password).toBase64());

        serverReplyCode = replyFromServer();
        if(serverReplyCode != PASS_AUTH)
        {
            qDebug() << "No PASSWORD AUTH reply from server!";
            return false;
        }
        break;

    default:
        qDebug() << "Authentication method not supported";
        return false;
        break;
    }

    return true;
}

bool mailManager::sendMail(const QString &subject,
                           const QString &message)
{
    QString serverReplyCode;

    m_message = "To: ";
    for(int count = 0; count < m_recipients.count(); count++)
        m_message.append(m_recipients.at(count)).append(", ");
    m_message.append("\n");
    m_message.append("From: " + m_sender + "\n");
    m_message.append("Subject: " + subject + "\n");
    m_message.append("Mime-Version: 1.0;\n");
    m_message.append("Content-Type: text/html; charset=\"utf8\";\n");
    m_message.append("Content-Transfer-Encoding: 8bit;\n");
    m_message.append("\n");
    m_message.append(message);
    m_message.replace(QString::fromLatin1("\n"),
                      QString::fromLatin1("\r\n"));

    m_message.replace(QString::fromLatin1("\r\n.\r\n"),
                      QString::fromLatin1("\r\n..\r\n"));

    qDebug() << "sendMail() :" << endl;
    qDebug() << "=======" << endl;
    qDebug() << m_message << endl;
    qDebug() << "-------" << endl;


    sendToServer("MAIL FROM:<"+ m_sender + ">");

    serverReplyCode = replyFromServer();
    if(serverReplyCode != ACTION_COMPLETED)
    {
        qDebug() << "No MAIL FROM reply from server!";
        return false;
    }

    for(int count = 0; count < m_recipients.count(); count++)
    {
        qDebug() << "RCPT TO: " << m_recipients.at(count);
        sendToServer("RCPT TO: <" + m_recipients.at(count) + ">");

        serverReplyCode = replyFromServer();
        if(serverReplyCode != ACTION_COMPLETED)
        {
            qDebug() << "No MAIL TO reply from server!";
            return false;
        }
    }

    sendToServer("DATA");

    serverReplyCode = replyFromServer();
    if(serverReplyCode != DATA_RESPONSE)
    {
        qDebug() << "No DATA reply from server!";
        return false;
    }

    sendToServer(m_message);
    sendToServer(".");

    serverReplyCode = replyFromServer();
    if(serverReplyCode != ACTION_COMPLETED)
    {
        qDebug() << "No MAIL MESSAGE reply from server!";
        return false;
    }

    return true;
}

bool mailManager::disconnectFromServer()
{
    QString serverReplyCode;

    sendToServer("QUIT");
    serverReplyCode = replyFromServer();
    if(serverReplyCode != DISCONNECT_COMPLETED)
    {
        qDebug() << "No QUIT reply from server!";
        return false;
    }

    return true;
}
