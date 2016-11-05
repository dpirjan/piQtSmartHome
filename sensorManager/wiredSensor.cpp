#include <QElapsedTimer>

#include "wiredSensor.h"
#include "homeAlarmInfo.h"
#include "databaseManagerInterface.h"
#include "mailManagerInterface.h"

#ifdef WIRINGPI
#include <wiringPi.h>
#include <errno.h>

extern "C"
{
    int wiringPiSetupGpio(void);
    int wiringPiISR (int, int, void (*function)(void*), void*);
};

bool wiredSensor::m_wiringPiInitialized = false;
#endif

wiredSensor::wiredSensor(const SystemType &system,
                         const SensorType &type,
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

#ifdef WIRINGPI
            if(!wiredSensor::m_wiringPiInitialized)
            {

                // Call for winringPiSetupGpio to initialize wiringPi using Broadcom pin numbers
                if(wiringPiSetupGpio() < 0)
                    qCritical() << "Unable to setup wiringPi: " << strerror(errno);
                else
                    wiredSensor::m_wiringPiInitialized = true;
            }

            int pin = StringToGPIO(address);
            int edgeType = StringToEdge(edge);

            // Call for wiringPiISR() interrupt initalization function
            // the edgeType can be: INT_EDGE_FALLING, INT_EDGE_RISING,
            // INT_EDGE_BOTH or INT_EDGE_SETUP
            //
            // The pin number is supplied in the current mode – native
            // wiringPi, BCM_GPIO, physical or Sys modes.
            //
            // This function will work in any mode, and does not need root
            // privileges to work.
            //
            //

            if(wiringPiISR(pin, edgeType, &wiredSensor::interruptHandler, this) < 0)
                qCritical() << "Unable to setup ISR on " << pin << " : " << strerror(errno);
            else
                qDebug() << "Setup interrupt on " << this << " was successfull!";
#endif
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
    HomeAlarmInfo event(getZone(),getNode(),sensorTypeToString(getSensorType()),
                        getAddress());
    databaseManagerInterface::instance().insertHomeAlarmEntry(event);
    if(getSendMail())
    {
        QString subject = "Alarm Notification piHome";
        QString message = "Alarm from Zone: " + getZone() +
                " Node: " + getNode() +
                " Sensor: " + sensorTypeToString(getSensorType()) +
                " GPIO: " + getAddress();
        mailManagerInterface::instance().sendMail(subject,message);
    }

    qDebug() << "wiredInterrupt() took " << timer.elapsed() << "ms";
}
