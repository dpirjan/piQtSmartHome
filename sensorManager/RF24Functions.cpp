#include "RF24Functions.h"
#include "RF24Interface.h"

#include <QDebug>
#include <QThread>
#include <QThread>
#include <QElapsedTimer>

#define RF24InterruptGPIO 17 //@TODO read this from settings file and take care it's BCM
#define RF24InterruptEDGE "INT_EDGE_FALLING" //@TODO read this from settings file

bool RF24Functions::m_rf24Initialized = false;
quint64 RF24Functions::m_counter = 0;

RF24Functions::RF24Functions(QObject *parent) : QObject(parent)
{
    qDebug() << "RF24Functions::constructor: " << QThread::currentThreadId();
    m_stop = false;
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

void RF24Functions::interruptHandler()
{
    qDebug() << "RF24Functions::interruptHandler";
    QElapsedTimer timer;
    timer.start();
#ifdef WIRINGPI
    qDebug() << "interruptHandler() counter " << ++RF24Functions::m_counter;
#endif
    qDebug() << "interruptHandler() took " << timer.elapsed() << "ms";
}

void RF24Functions::init()
{
    qDebug() << "RF24Functions::init: " << QThread::currentThreadId();
    QMutexLocker locker(&m_mutex);
    if(RF24Functions::m_rf24Initialized)
        return;

#ifdef WIRINGPI //@TODO change this to something more suggestive
    // Set up NRF24L01+ radio on SPI bus (see the above wiring)
    radio = new RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
    network = new RF24Network(*radio);
    mesh = new RF24Mesh(*radio, *network);

    attachInterrupt(RF24InterruptGPIO,
                    INT_EDGE_FALLING,
                    interruptHandler);

    // Set the nodeID to 0 for the master node
    mesh->setNodeID(0);

    // Connect to the mesh
    mesh->begin();
    radio->printDetails();

    // Mask interrupts in order to have only on receive package
    radio->maskIRQ(1,1,0);
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

        // @TODO move this to the interruptHandler.
        if(network->available())
            qDebug() << "New RF24 packet received <" << checkData()
                     << "> loop counter: " << ++counter;

        if(m_stop)
            break;

        delayMicroseconds(250000); //250ms
    }
#endif

    emit finished();
}

bool RF24Functions::checkData()
{
    bool tmpStat = false;

#ifdef WIRINGPI
    t_payload pp; // Payload used on RF
    RF24NetworkHeader hdr;
    size_t dataSize = network->peek(hdr);
    float *value = NULL;

    network->read(hdr, &pp, dataSize);

    qDebug() << "Received data: size=" << dataSize << " from node: "
             << mesh->getNodeID(hdr.from_node) << " (" << hdr.from_node
             << ") - " << "msgID=" << hdr.id;

    value = (float *) pp.val;

    switch(hdr.type)
    {
    case 'T':
    case 'H':
    case 'P':
    case 'G':
        qDebug() << "< " << hdr.type << "> from addr: " << hdr.from_node <<
                    " nodeID: " << pp.IDnode << "value: " << *value;
        qDebug() << "array: " << pp.val;
        tmpStat = true;
        break;
    case 'A':
        qDebug() << "Actuator !!!";
        tmpStat = true;
        break;
    default:
        network->read(hdr,0,0);
        qDebug() << "Header: " << hdr.type << " from node: " << hdr.from_node;
        break;
    }
#endif

    return tmpStat;
}

void RF24Functions::stop()
{
    qDebug() << "RF24Functions::stop: " << QThread::currentThreadId();
    QMutexLocker locker(&m_mutex);
    m_stop = true;
}
