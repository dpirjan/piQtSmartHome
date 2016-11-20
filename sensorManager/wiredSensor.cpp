#include <QElapsedTimer>

#include "wiredSensor.h"
#include "homeAlarmInfo.h"
#include "databaseManagerInterface.h"
#include "mailManagerInterface.h"
#include "wiringPiInterface.h"

wiredSensor::wiredSensor(const SystemType &system,
                         const IOType &type,
                         const QString &zone,
                         const QString &node,
                         const QString &address,
                         const bool &sendMail,
                         const QString &edge,
                         const int &timeout)
    : sensor(system, type, zone, node, address, sendMail)
    , m_wiredEdge(edge)
    , m_wiredTimeout(timeout)
{
    qDebug() << "wiredSensor ctor: " << this << " GPIO: " << address;
    // @TODO uncomment this when the CPU load problem is solved
//    wiringPiInterface::instance().setupInterrupt(address,
//                                                 edge,
//                                                 &wiredSensor::interruptHandler,
//                                                 this);
}

wiredSensor::wiredSensor(const wiredSensor &obj) : sensor(obj)
{
    m_wiredEdge = obj.getWiredEdge();
    m_wiredTimeout = obj.getWiredTimeout();

    qDebug() << "wiredSensor cpy ctor: " << this << " GPIO: " << obj.getAddress();
}

wiredSensor& wiredSensor::operator =(const wiredSensor &other)
{
    sensor::operator = (other);
    m_wiredEdge = other.getWiredEdge();
    m_wiredTimeout = other.getWiredTimeout();

    qDebug() << "wiredSensor = " << this << " GPIO: " << other.getAddress();

    return *this;
}

wiredSensor::~wiredSensor()
{
}

QString wiredSensor::getWiredEdge() const
{
    return m_wiredEdge;
}

int wiredSensor::getWiredTimeout() const
{
    return m_wiredTimeout;
}

void wiredSensor::debugSensor() const
{
    qDebug() << "==============================================";
    sensor::debugSensor();
    qDebug() << "Wired Edge " << m_wiredEdge;
    qDebug() << "Wired timeout " << m_wiredTimeout;
}

void wiredSensor::setWiredEdge(const QString &edge)
{
    m_wiredEdge = edge;
}

void wiredSensor::setWiredTimeout(const int &timeout)
{
    m_wiredTimeout = timeout;
}

void wiredSensor::interruptHandler(void *userData)
{
    qDebug() << "interruptHandler: " << userData;
    QElapsedTimer timer;
    timer.start();
    wiredSensor *sensor = reinterpret_cast<wiredSensor *>(userData);
    if(sensor)
    {
        qDebug() << "interruptHandler: " << sensor << " GPIO " << sensor->getAddress();
        sensor->interrupt();
    }
    else
        qCritical() << "interruptHandler: WrongData - " << userData;
    qDebug() << "interruptHandler() took " << timer.elapsed() << "ms";
}

void wiredSensor::interrupt()
{
    qDebug() << "Wired interrupt: " << this << "GPIO: " << getAddress();
    QElapsedTimer timer;
    timer.start();
    // Will create a homeAlarmInfo class instance and insert it in the
    // database containing the events.
    HomeAlarmInfo event(getZone(),getNode(),typeToString(getSensorType()),
                        getAddress());
    databaseManagerInterface::instance().insertHomeAlarmEntry(event);
    if(getSendMail())
    {
        QString subject = "Alarm Notification piHome";
        QString message = "Alarm from Zone: " + getZone() +
                " Node: " + getNode() +
                " Sensor: " + typeToString(getSensorType()) +
                " GPIO: " + getAddress();
        mailManagerInterface::instance().sendMail(subject,message);
    }

    qDebug() << "wiredInterrupt() took " << timer.elapsed() << "ms";
}
