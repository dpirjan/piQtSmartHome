#ifndef MAILMANAGER_H
#define MAILMANAGER_H

#include <QObject>
#include <QAbstractSocket>

class QTextStream;
class QSslSocket;
class QTcpSocket;
class QSettings;

#define MAIL_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.MailManager"
#define MAIL_MANAGER_SERVICE_PATH "/"
#define MAIL_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.MailManager.MailOperations"

class mailManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", MAIL_MANAGER_SERVICE_INT)
    /*
     * Basic algorithm for sending email through smtp:
     *      1. Connecting to the server and waiting for greeting message
     *      2. If response code = 220 then send a greeting back to server
     *          "EHLO yourdomain.com\r\n"
     *      3. If response code = 250 then identify from whom the mail is
     *          coming from by sending "MAIL FROM:<youremail@mail.com>\r\n"
     *      4. If response code = 250 then send whom the email is addressed to
     *          "RCPT TO:<send_to@mail.com>\r\n"
     *      5. If response code = 250 then now it is time to inititate sending
     *          the contents of the email by sending "DATA\r\n"
     *      6. If response code = 354 then send your contents for example
     *          "This is an email\r\n"
     *      7. If response code = 250 then your email has been sent and now you
     *          can close the connection by sending "QUIT\r\n"
     *
     * Basic algorithm for TLS/SSL smtp email sending:
     *      1. Connecting to the server and waiting for greeting message
     *      2. If response code = 220 then send a greeting back to server
     *          "EHLO yourdomain.com\r\n"
     *      3. If response code = 250 then initiate TLS communication by making
     *          a SSL/TLS handshake, for this I use the methods provided in
     *          QSslSocket class, more precisely
     *          QSslSocket::startClientEncryption().
     *          (make communictaion encrypted)
     *      4. If response code = 250 then you have successfully initiated a
     *          encrypted communication and now we can start authentification
     *          by sending "AUTH LOGIN\r\n"
     *      5. If response code = 334 then the server is waiting for username,
     *          since my aim was to use GMAIL as smtp server their API states
     *          that the username and password must be sent in base64 coding,
     *          it would look something like this "bXlfdXNlcm5hbWU=\r\n"
     *          (my_username)
     *      6. If response code = 334 then send the server password, and again
     *          in base64 coding "bXlfcGFzc3dvcmQ=\r\n" (my_password)
     *      7. If response code = 235 then you have successfully logged in and
     *          now you can identify from whom the mail is coming from by
     *          sending "MAIL FROM:<youremail@mail.com>\r\n"
     *      8. If response code = 250 then send whom the email is addressed to
     *          "RCPT TO:<send_to@mail.com>\r\n"
     *      9. If response code = 250 then now it is time to inititate sending
     *          the contents of the email by sending "DATA\r\n"
     *     10. If response code = 354 then send your contents for example
     *          "This is an email\r\n"
     *     11. If response code = 250 then your email has been sent and now you
     *          can close the connection by sending "QUIT\r\n"
     *
     * Important: Every command to server has to be finished with carriege
     * return and new line symbol in other words you have to finish the line
     * with "\r\n"
     *
     */
public:
    static mailManager &instance()
    {
        static mailManager obj;
        return obj;
    }

    static bool firstRunConfiguration();

    bool connectService();

public slots:
    Q_SCRIPTABLE bool connectToServer();
    Q_SCRIPTABLE bool loginToServer();
    Q_SCRIPTABLE bool sendMail(const QString&,
                  const QString&);
    Q_SCRIPTABLE bool disconnectFromServer();


private:
    explicit mailManager(QObject *parent = 0);
    virtual ~mailManager();
    Q_DISABLE_COPY(mailManager)

    void loadServerCredentials();
    void loadSendMailDetails();

    void sendToServer(const QString&);
    QString replyFromServer();

private slots:
    void connected();
    void disconnected();
    void stateChanged(QAbstractSocket::SocketState);
    void errorReceived(QAbstractSocket::SocketError);

private:
    enum ConnectionType
    {
        None = 0,  // None
        SSL,       // SSL
        TLS        // STARTTLS
    };
    enum AuthenticationType
    {
        Plain = 0,
        Encrypted
    };

    QString m_serverName;
    QString m_userName;
    QString m_password;
    int m_port;
    int m_timeout;
    ConnectionType m_connection;
    AuthenticationType m_authentication;

    QString m_message;
    QString m_response;

    QString m_sender;
    QStringList m_recipients;

    QTextStream *textStream;
    QTcpSocket *socket;

    QSettings *m_settings;
};

#endif // MAILMANAGER_H
