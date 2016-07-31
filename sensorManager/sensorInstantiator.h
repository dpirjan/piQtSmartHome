#ifndef SENSORINSTANTIATOR_H
#define SENSORINSTANTIATOR_H

#include <QObject>
#include <QSettings>

#include "sensor.h"

using namespace piHome;

class sensorInstantiator : public QObject
{
    Q_OBJECT
public:
    explicit sensorInstantiator(QObject *parent = 0);
    virtual ~sensorInstantiator();

signals:

public slots:
    void saveSensors(); // only for debugging will be removed in the future
    QList<sensor> loadSensors();

private:
    QSettings *m_settings;

    QList<sensor> m_sensorList;
};

#endif // SENSORINSTANTIATOR_H
