#include "RF24Functions.h"
#include "RF24Interface.h"
#include "wiringPiInterface.h"

#include <QDebug>
#include <QThread>

#define RF24InterruptGPIO "GPIO_2" //@TODO read this from settings file
#define RF24InterruptEDGE "INT_EDGE_RISING"

bool RF24Functions::m_rf24Initialized = false;

RF24Functions::RF24Functions(QObject *parent) : QObject(parent)
{
    qDebug() << "RF24Functions::constructor: " << QThread::currentThreadId();
    m_stop = false;
    init();
}

RF24Functions::~RF24Functions()
{
    qDebug() << "RF24Functions::destructor: " << QThread::currentThreadId();
#ifdef WIRINGPI
    if(mesh)
        delete mesh;
    if(network)
        delete network;
    if(radio)
        delete radio;
#endif
}

void RF24Functions::init()
{
    qDebug() << "RF24Functions::init: " << QThread::currentThreadId();
    QMutexLocker locker(&m_mutex);
    if(RF24Functions::m_rf24Initialized)
        return;

    wiringPiInterface::instance().setupInterrupt(RF24InterruptGPIO,
                                                 RF24InterruptEDGE,
                                                 &RF24Interface::interruptHandler,
                                                 NULL);
#ifdef WIRINGPI //@TODO change this to something more suggestive
    // Set up NRF24L01+ radio on SPI bus (see the above wiring)
    radio = new RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
    network = new RF24Network(*radio);
    mesh = new RF24Mesh(*radio, *network);

    // Set the nodeID to 0 for the master node
    mesh->setNodeID(0);

    // Connect to the mesh
    mesh->begin();
    radio->printDetails();
#endif
    RF24Functions::m_rf24Initialized = true;
}

void RF24Functions::loop()
{
    qDebug() << "RF24Functions::loop: " << QThread::currentThreadId();
#ifdef WIRINGPI
    static quint64 counter;
    while(1)
    {
        // Keep the network updated
        mesh->update();
        // DHCP service runs on the master node and assign addresses to the unit nodes
        mesh->DHCP();
        // MASTER never sleeps as it is in receive and has to assign addresses
        if(network->available())
            qDebug() << "New RF24 packet received: " << ++counter;
        if(m_stop)
            break;
    }
#endif

    emit finished();
}

void RF24Functions::stop()
{
    qDebug() << "RF24Functions::stop: " << QThread::currentThreadId();
    QMutexLocker locker(&m_mutex);
    m_stop = true;
}
