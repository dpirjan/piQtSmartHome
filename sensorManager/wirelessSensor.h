#ifndef WIRELESSSENSOR_H
#define WIRELESSSENSOR_H

#include <QObject>

using namespace piHome;

class sensor;
class sensorInstantiator;

class wirelessSensor : public sensor
{
public:
    explicit wirelessSensor(const SystemType&,
                            const SensorType&,
                            const QString&,
                            const QString&,
                            const QString&,
                            const int&);
    wirelessSensor(const wirelessSensor &);
    wirelessSensor& operator=(const wirelessSensor &);
    virtual ~wirelessSensor();

    friend class sensorInstantiator;

    int getWirelessTimeout() const;

    void debugSensor() const;

protected:
    void setWirelessTimeout(const int&);

private:
    int m_wirelessTimeout;

    template<class T> friend class QList;

public slots:
    void interrupt();
};

#endif // WIRELESSSENSOR_H
