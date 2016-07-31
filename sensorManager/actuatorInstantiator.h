#ifndef ACTUATORINSTANTIATOR_H
#define ACTUATORINSTANTIATOR_H

#include <QObject>
#include <QSettings>

#include "actuator.h"

using namespace piHome;

class actuatorInstantiator : public QObject
{
    Q_OBJECT
public:
    explicit actuatorInstantiator(QObject *parent = 0);
    virtual ~actuatorInstantiator();

public slots:
    void saveActuators(); // only for debugging will be removed in the future
    QList<actuator> loadActuators();

private:
    QSettings *m_settings;
};

#endif // ACTUATORINSTANTIATOR_H
