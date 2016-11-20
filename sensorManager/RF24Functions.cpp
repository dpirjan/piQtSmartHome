#include "RF24Functions.h"
#include "RF24Interface.h"
#include "sensorInstantiator.h"

#include <QDebug>
#include <QThread>
#include <QElapsedTimer>

#include <cstdlib>
#include <unistd.h>

#define RF24InterruptGPIO 17 //@TODO read this from settings file and take care it's BCM
#define RF24InterruptEDGE "INT_EDGE_FALLING" //@TODO read this from settings file

#ifdef WIRINGPI
RF24 RF24Functions::m_radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
RF24Network RF24Functions::m_network(RF24Functions::m_radio);
RF24Mesh RF24Functions::m_mesh(RF24Functions::m_radio, RF24Functions::m_network);
#endif

RF24Functions::RF24Functions(QObject *parent) : QObject(parent)
{
    qDebug() << "RF24Functions::constructor: " << QThread::currentThreadId();
    m_stop = false;

#ifdef WIRINGPI
    attachInterrupt(RF24InterruptGPIO,
                    INT_EDGE_FALLING,
                    interruptHandler);

    // Set the nodeID to 0 for the master node
    RF24Functions::m_mesh.setNodeID(0);

    // Connect to the mesh
    RF24Functions::m_mesh.begin();

    // Mask interrupts in order to have only on receive package
    RF24Functions::m_radio.maskIRQ(1, 1, 0);
#endif
}

RF24Functions::~RF24Functions()
{
    qDebug() << "RF24Functions::destructor: " << QThread::currentThreadId();
}

void RF24Functions::interruptHandler()
{
    QElapsedTimer timer;
    timer.start();
#ifdef WIRINGPI
    qDebug() << "RF24Functions::interruptHandler";

    if(RF24Functions::m_network.available())
    {
        t_payload pp; // Payload used on RF
        RF24NetworkHeader hdr;
        size_t dataSize = RF24Functions::m_network.peek(hdr);
        float *value = NULL;

        RF24Functions::m_network.read(hdr, &pp, dataSize);

        qDebug() << "Received data: size=" << dataSize << " from node: "
                 << RF24Functions::m_mesh.getNodeID(hdr.from_node) << " (" << hdr.from_node
                 << ") - " << "msgID=" << hdr.id;

        value = (float *) pp.val;

        // @TODO remove debugging messages after the validation complete
        if(hdr.type == 'T' || hdr.type == 'H' || hdr.type == 'P' || hdr.type == 'G')
        {
            qDebug() << "<" << hdr.type << "> from addr: " << hdr.from_node <<
                        " nodeID: " << pp.IDnode << "value: " << *value;
            qDebug() << "array: " << pp.val[0] << pp.val[1] << pp.val[2] << pp.val[3];
        }

        wirelessSensor *tmp;
        switch (hdr.type)
        {
        // @TODO use the IOType instead of T, H, P, G
        case 'T':
            // Temperature sensor
            tmp = sensorInstantiator::instance().findWirelessSensor(Temperature,
                                                                    QString::number(hdr.from_node));
            if(tmp)
                tmp->interrupt();
            break;
        case 'H':
            // Humidity sensor
            tmp = sensorInstantiator::instance().findWirelessSensor(Humidity,
                                                                    QString::number(hdr.from_node));
            if(tmp)
                tmp->interrupt();
        case 'P':
            // PIR sensor
            tmp = sensorInstantiator::instance().findWirelessSensor(PIR,
                                                                    QString::number(hdr.from_node));
            if(tmp)
                tmp->interrupt();
            break;
        case 'G':
            // CH4 sensor
            tmp = sensorInstantiator::instance().findWirelessSensor(CH4,
                                                                    QString::number(hdr.from_node));
            if(tmp)
                tmp->interrupt();
            break;
        default:
            RF24Functions::m_network.read(hdr,0,0);
            qDebug() << "Header: " << hdr.type << " from node: " << hdr.from_node;
            break;
        }
    }
#endif
    qDebug() << "RF24Functions::interruptHandler took " << timer.elapsed() << "ms";
}

void RF24Functions::loop()
{
    qDebug() << "RF24Functions::loop: " << QThread::currentThreadId();

    static quint64 counter;

    while(!m_stop)
    {
#ifdef WIRINGPI //@TODO change this to something more suggestive
        // Keep the network updated
        RF24Functions::m_mesh.update();

        // DHCP service runs on the master node and assign addresses to the unit nodes
        RF24Functions::m_mesh.DHCP();
#else
        qDebug() << "RF24Functions::loop: " << ++counter;
#endif
        usleep(250000); //250ms
    }

    emit finished();
}

void RF24Functions::stop()
{
    qDebug() << "RF24Functions::stop: " << QThread::currentThreadId();
    m_stop = true;
}
