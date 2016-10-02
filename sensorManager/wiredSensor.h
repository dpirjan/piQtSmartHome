#ifndef WIREDSENSOR_H
#define WIREDSENSOR_H

#include <QObject>

#include "common.h"
#include "sensor.h"

using namespace piHome;

class sensorInstantiator;

class wiredSensor : public sensor
{
public:
    explicit wiredSensor(const SystemType&,
                         const SensorType&,
                         const QString&,
                         const QString&,
                         const QString&,
                         const bool&,
                         const QString&,
                         const int&);
    wiredSensor(const wiredSensor &);
    wiredSensor& operator=(const wiredSensor&);
    virtual ~wiredSensor();

    friend class sensorInstantiator;

    QString getWiredEdge() const;
    int getWiredTimeout() const;

    void debugSensor() const;

    static void interruptHandler(void*);

protected:
    void setWiredEdge(const QString&);
    void setWiredTimeout(const int&);

private:
    QString m_wiredEdge;
    int m_wiredTimeout;

    template<class T> friend class QList;

    void interrupt();

//#ifdef WIRINGPI
    static bool m_wiringPiInitialized;
//#endif
};

#endif // WIREDSENSOR_H
