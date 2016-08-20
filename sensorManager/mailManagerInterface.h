#ifndef MAILMANAGERINTERFACE_H
#define MAILMANAGERINTERFACE_H

#include <QObject>
#include <QDBusInterface>
#include <QtDBus>

class QDBusInterface;

class mailManagerInterface : public QObject
{
    Q_OBJECT

    explicit mailManagerInterface(QObject *parent = 0);
    virtual ~mailManagerInterface();
    Q_DISABLE_COPY(mailManagerInterface)

    bool connectToDBus();

public:
    static mailManagerInterface &instance()
    {
        static mailManagerInterface obj;
        return obj;
    }

public slots:
    bool connectToServer();
    bool loginToServer();
    bool sendMail(const QString&,
                  const QString&);
    bool disconnectFromServer();

private:
    QDBusInterface *iface;
};

#endif // MAILMANAGERINTERFACE_H
