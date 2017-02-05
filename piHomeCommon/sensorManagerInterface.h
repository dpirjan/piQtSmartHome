#ifndef SENSORMANAGERINTERFACE_H
#define SENSORMANAGERINTERFACE_H

#include <QObject>

class QDBusInterface;

class sensorManagerInterface : public QObject
{
    Q_OBJECT

    explicit sensorManagerInterface(QObject *parent = 0);
    virtual ~sensorManagerInterface();
    Q_DISABLE_COPY(sensorManagerInterface)

    bool connectToDBus();

public:
    static sensorManagerInterface &instance()
    {
        static sensorManagerInterface obj;
        return obj;
    }

public slots:
    bool setActuatorValue(const QString&,
                          const QString&,
                          const QString&);

private:
    QDBusInterface *m_iface;
};

#endif // SENSORMANAGERINTERFACE_H
