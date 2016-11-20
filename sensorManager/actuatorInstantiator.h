#ifndef ACTUATORINSTANTIATOR_H
#define ACTUATORINSTANTIATOR_H

#include <QObject>
#include <QSettings>

#include "actuator.h"

using namespace piHome;

class actuatorInstantiator : public QObject
{
    Q_OBJECT

    explicit actuatorInstantiator(QObject *parent = 0);
    virtual ~actuatorInstantiator();
    Q_DISABLE_COPY(actuatorInstantiator)

     void loadActuators();

public:
    static actuatorInstantiator &instance()
    {
        static actuatorInstantiator obj;
        return obj;
    }

    static bool firstRunInitActuators();

public slots:
    QList<actuator *> getActuators() const;

    actuator* findActuator(const IOType&, const QString &) const;

private:
    QSettings *m_settings;

    QList<actuator *> m_actuatorsList;
};

#endif // ACTUATORINSTANTIATOR_H
