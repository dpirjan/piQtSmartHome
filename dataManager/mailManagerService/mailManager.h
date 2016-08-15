#ifndef MAILMANAGER_H
#define MAILMANAGER_H

#include <QObject>
#include <QAbstractSocket>

class QTextStream;
class QSslSocket;

class mailManager : public QObject
{
    Q_OBJECT

public:
    explicit mailManager(const QString&,
                         const QString&,
                         const QString&,
                         const int &port = 465,
                         const int &timeout = 30000);
    virtual ~mailManager();

    void sendMail(const QString &sender,
                  const QString &recipient,
                  const QString &subject,
                  const QString &message);

signals:
    void status(const QString&);
    void finished();

private slots:
    void stateChanged(QAbstractSocket::SocketState );
    void errorReceived(QAbstractSocket::SocketError );
    void disconnected();
    void connected();
    void readyRead();


private:
    QString m_serverName;
    QString m_userName;
    QString m_password;
    int m_port;
    int m_timeout;
    QString m_message;
    QString m_response;

    QTextStream *textStream;
    QSslSocket *socket;

    enum MailManagerState
    {
        Authentication = 0,
        User,
        Password,
        Recipient,
        Mail,
        Data,
        Init,
        Body,
        Quit,
        Close
    };

    MailManagerState m_state;
};

#endif // MAILMANAGER_H
