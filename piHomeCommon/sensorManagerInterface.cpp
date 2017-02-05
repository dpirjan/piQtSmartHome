#include <QDebug>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#include "sensorManagerInterface.h"

#define SENSOR_MANAGER_SERVICE_NAME "org.raspberrypi.piHome.SensorManager"
#define SENSOR_MANAGER_SERVICE_PATH "/"
#define SENSOR_MANAGER_SERVICE_INT  "org.raspberrypi.piHome.SensorManager.IOOperations"

sensorManagerInterface::sensorManagerInterface(QObject *parent) : QObject(parent)
{
    if(!QDBusConnection::systemBus().isConnected())
        qCritical() << "Cannot connect to the D-Bus system bus!";

    if(!connectToDBus())
        qCritical() << "Cannot connect to DBus service "
                    << SENSOR_MANAGER_SERVICE_NAME
                    << " on interface "
                    << SENSOR_MANAGER_SERVICE_PATH;
}

sensorManagerInterface::~sensorManagerInterface()
{
    if(m_iface)
        delete(m_iface);
}

bool sensorManagerInterface::connectToDBus()
{
    bool ret = true;
    m_iface = new QDBusInterface(SENSOR_MANAGER_SERVICE_NAME,
                                 SENSOR_MANAGER_SERVICE_PATH,
                                 SENSOR_MANAGER_SERVICE_INT,
                                 QDBusConnection::systemBus());
    if(!m_iface->isValid())
    {
        qDebug() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    qDebug() << "Service: " << m_iface->service();
    qDebug() << "Path: " << m_iface->path();
    qDebug() << "Interface: " << m_iface->interface();

    return ret;
}

bool sensorManagerInterface::setActuatorValue(const QString &category,
                                              const QString &address,
                                              const QString &value)
{
    QDBusReply<bool> reply = m_iface->call(
                QDBus::BlockWithGui,
                QLatin1String("setActuatorValue"),
                qVariantFromValue(category),
                qVariantFromValue(address),
                qVariantFromValue(value));

    if(reply.isValid())
        qDebug() << "setActuatorValue reply was:" << reply.value();
    else
    {
        qCritical() << "DBus call error: " << m_iface->lastError();
        qCritical() << "DBus reply error: " << reply.error();
        return false;
    }

    return reply.value();
}
