#ifndef RF24FUNCTIONS_H
#define RF24FUNCTIONS_H

#include <QObject>
#include <QMutex>

#ifdef WIRINGPI
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#endif

class RF24Functions : public QObject
{
    Q_OBJECT

protected:
    explicit RF24Functions(QObject *parent = 0);
    virtual ~RF24Functions();
    Q_DISABLE_COPY(RF24Functions)

    friend class RF24Interface;

protected slots:
    void loop();
    void stop();

signals:
    void error();
    void finished();

private:
    void init();

    bool m_stop;
    QMutex m_mutex;

    static bool m_rf24Initialized;

#ifdef WIRINGPI
    RF24 *radio;
    RF24Network *network;
    RF24Mesh *mesh;
#endif
};

#endif // RF24FUNCTIONS_H
