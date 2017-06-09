#include "RF24Functions.h"
#include "RF24Interface.h"
#include "sensorInstantiator.h"

#include <QDebug>
#include <QThread>
#include <QElapsedTimer>

#include <cstdlib>
#include <unistd.h>

#define ID_Watchdog         0x08000000UL    // (bit_27) To read/configure watchdog
#define ID_GET_TASK         0x80000000UL

// Default value to wake-up [ms] (watchdog timestamp)
#define WDT_DT              4000

// Define message types
/*
 * Types of available messages:
 *  - GET_ALL : From Master to Unit, it requests to forward all
 *              sensors values and actuators status.
 *              network.write msg_type="A" 0x41
 *  - GET_DEV : From Master to Unit, it requests to get the value for
 *              a given sensor or the status for actuator
 *              network.write msg_type="D" 0x44
 *  - SET_ACT : From Master to Unit, it requests to drive an actuator
 *              network.write msg_type="C" 0x43
 *  - GET_DLT : From Master to Unit, read the time step for a sensor/actuator
 *              network.write msg_type="R" 0x52
 *  - SND_DLT : From Unit to Master, send the time step for a sensor/actuator
 *              network.write msg_type="t" 0x74
 *  - SET_DLT : From Master to Unit, set the time step for a sensor/actuator
 *              network.write msg_type="T" 0x54
 *  - GET_MSK : From Master to Unit, it requests the sensor mask
 *              (if sensor value will be delivered)
 *              network.write msg_type="G" 0x47
 *  - SND_MSK : From Unit to Master, return mask value for sensor
 *              network.write msg_type="m" 0x6D
 *  - CNF_MSK : From Master to Unit, configure mask value:
 *              val = 0 Sensor value isn't delivered (even when interrupt occours)
 *                      Remark:
 *                        When forced, the sensor value is sent to Master, means if
 *                        explicitly is requested then this task is prioritar
 *              val = 1 Sensor value is deliver without restriction
 *              network.write msg_type="M" 0x4D
 *  - SEND_VAL : From Unit to Master, respond with the sensor/actuator value
 *              network.write msg_type="V" 0x56
 *  - SEND_ERR : From Unit to Master, return SUCCESS or ERROR No
 *              network.write msg_type="E" 0x45
 */
#define SEND_VAL             0x56   // "V"
#define SEND_ERR             0x45   // "E"
#define GET_ALL              0x41   // "A"
#define GET_DEV              0x44   // "D"
#define SET_ACT              0x43   // "C"
#define GET_DLT              0x52   // "R"
#define SND_DLT              0x74   // "t"
#define SET_DLT              0x54   // "T"
#define GET_MSK              0x47   // "G"
#define SND_MSK              0x6D   // "m"
#define SET_MSK              0x4D   // "M"

// Maximum number of retries when send a sensor value
#define MAX_RETRIES          5

// Define Errors
#define RET_SUCCESS          0
#define READ_VAL_FAILED     -1
#define SEND_RETR_FAILED    -2
#define GET_ADDR_FAILED     -3
#define SEND_TEMP_FAILED    -4
#define SEND_HUMD_FAILED    -5
#define SEND_PIR_FAILED     -6
#define DETECT_PIR_FALSE    -7
#define GET_DHT22_FAILED    -8
#define SEND_CH4_FAILED     -9
#define SEND_CO_FAILED      -10
#define SEND_LIGHT_FAILED   -11
#define SEND_WATER_FAILED   -12

#define WRONG_ACTUATOR_ID   -20

#define WRONG_SENSOR_ID     -30

#define WRONG_UNIT_ID       -40

#define WRONG_HEADER_TYPE   -50

#define WRONG_CONFIG        -60

#define UNKNOWN_MSG_TYPE    -70

#define RX_RING_FULL        -80
#define RX_NO_DATA_AVAIL    -81

#define NO_ALARM_AVAIL      -90

// Aditional information when an error occours
#define RETRIES_FAILED    0xFF100000UL
#define ASSIGNED_TASK     0xFF200000UL

#ifdef WIRINGPI
#include <wiringPi.h>

#define RF24InterruptGPIO   17                  //@TODO read this from settings file
#define RF24InterruptEDGE   "INT_EDGE_FALLING"  //@TODO read this from settings file

RF24 RF24Functions::m_radio(22, 0);
RF24Network RF24Functions::m_network(RF24Functions::m_radio);

struct RF24Functions::t_RX RF24Functions::rx[LAST_RX + 1];

int RF24Functions::head = 0;
int RF24Functions::posRX = 0;
int RF24Functions::status = 0;                                      // g_stat
bool RF24Functions::packetReceived = false;                         // g_isrRF_ON
#endif

RF24Functions::RF24Functions(QObject *parent) : QObject(parent)
{
    qDebug() << "RF24Functions::constructor: " << QThread::currentThreadId();
    m_stop = false;

#ifdef WIRINGPI
    // Clear RX ring
    memset(RF24Functions::rx, 0, (LAST_RX + 1) * sizeof(t_RX));


    if(wiringPiISR(RF24InterruptGPIO, INT_EDGE_FALLING, RF24Functions::interruptHandler, NULL) < 0)
        qCritical() << "Unable to setup ISR on " << RF24InterruptGPIO << " : " << strerror(errno);
    else
        qDebug() << "Setup interrupt on " << RF24InterruptGPIO << " was successfull!";


    RF24Functions::m_radio.begin();
    RF24Functions::m_radio.setChannel(90);
    RF24Functions::m_radio.maskIRQ(1,1,0);   // maskIRQ(bool_tx_ok, bool_tx_fail, bool_rx_ready) where 1 means disable (mask)
    RF24Functions::m_radio.setPALevel(RF24_PA_MAX);
    RF24Functions::m_radio.setCRCLength(RF24_CRC_16);
    RF24Functions::m_radio.setRetries(15,15);
    RF24Functions::m_radio.setAutoAck(1);
    RF24Functions::m_radio.enableAckPayload();
    RF24Functions::m_network.returnSysMsgs = 1;

    RF24Functions::m_network.begin(/*channel 90, */ /*node address*/ 0);

    //@TODO: used for debugging purposes, to be removed later
    RF24Functions::m_radio.printDetails();

    RF24Functions::m_radio.startListening();
#endif
}

RF24Functions::~RF24Functions()
{
    qDebug() << "RF24Functions::destructor: " << QThread::currentThreadId();
}

void RF24Functions::interruptHandler(void *arg)
{
    Q_UNUSED(arg);
    QElapsedTimer timer;
    timer.start();

#ifdef WIRINGPI
    static quint64 interruptCounter;

    qDebug() << "RF24Functions::interruptHandler " << QThread::currentThreadId()
             << " counter:" << ++interruptCounter;

    RF24Functions::status = RET_SUCCESS;
    do
    {
        // Avoid overwrite current data when the ring queue is full!!
        if(RF24Functions::rx[RF24Functions::head].newD == 0)
        {
            RF24Functions::m_network.peek(RF24Functions::rx[RF24Functions::head].h);
            RF24Functions::m_network.read(RF24Functions::rx[RF24Functions::head].h,
                    RF24Functions::rx[RF24Functions::head].p,
                    sizeof(t_payload));

            if(RF24Functions::rx[RF24Functions::posRX].h.type != 0)
            {
                //discard data when dummy
                RF24Functions::rx[RF24Functions::head].newD = 1;
                RF24Functions::head += 1;  // Go to next buffer
                RF24Functions::head &= LAST_RX;
            }
            else
                RF24Functions::status = WRONG_HEADER_TYPE;
        }
        else
        {
            RF24Functions::status = RX_RING_FULL;
            break;
        }
    } while(RF24Functions::m_network.available());

    RF24Functions::packetReceived = true;
#endif

    qDebug() << "RF24Functions::interruptHandler took " << timer.elapsed() << "ms";
}

int RF24Functions::processInformation() const
{
    int tmpStat = RET_SUCCESS;
#ifdef WIRINGPI
    t_payload *g_pp;
    char task;

    qDebug() << "Current RX stack: posRX =" << RF24Functions::posRX
             << " head =" << RF24Functions::head;

    // Assume head is always in front of posRX
    while(RF24Functions::rx[RF24Functions::posRX].newD != 0)
    {
        task = RF24Functions::rx[RF24Functions::posRX].h.type;
        g_pp = (t_payload *) RF24Functions::rx[RF24Functions::posRX].p;
        // prepare task payload

        qDebug() << "Received data: msgID = "<< RF24Functions::rx[RF24Functions::posRX].h.id
                 << " from Node address ="<< RF24Functions::rx[RF24Functions::posRX].h.from_node
                 << " type ="<< task
                 << " IOType ="<< typeToString(g_pp->IDs)
                 << " value ="<< g_pp->value;

        wirelessSensor *tmp = sensorInstantiator::instance().findWirelessSensor(
                    (IOType)g_pp->IDs,
                    QString::number(RF24Functions::rx[RF24Functions::posRX].h.from_node));
        if(tmp)
        {
            tmp->setValue(QString::number(g_pp->value));
            tmp->interrupt();
        }
        else
            qWarning() << "Cannot find" << typeToString(g_pp->IDs) << "sensor with address"
                       << RF24Functions::rx[RF24Functions::posRX].h.from_node;

        // data was succesful extracted
        RF24Functions::rx[RF24Functions::posRX].newD = 0;
        // next buffer
        RF24Functions::posRX += 1;
        RF24Functions::posRX &= LAST_RX;
    }
#endif
    return tmpStat;
}

/**
  * Send a task from current Unit to the <node> Unit through the RF network
  *
  * @param [in] tskID        the task ID {GET_ALL, GET_DEV, SET_ACT, SET_DLT, GET_MSK, CNF_MSK}
  * @param [in] sensorID     the sensor or actuator code. See <sensorIDs> from common.h
  * @param [in] tskval       the additional value needed to fulfil task
  * @param [in] maxR         maximum nmber of retries when send through RF fails
  * @param [in] node         ID for the destination node
  * @return the status of RF transmition (success or error)
  */
int RF24Functions::sendTask(const uint16_t &taskID,
                            const uint32_t &sensorID,
                            const float &taskval,
                            const int &maxR,
                            const uint16_t &node) const
{
    int stat = SEND_RETR_FAILED;

    qDebug() << "RF24Functions::sendTask(" << taskID << ","
             << sensorID << "," << taskval << "," << maxR << "," << node <<")";

#ifdef WIRINGPI
    int retries = 0;
    t_payload pp;

    RF24NetworkHeader header(node, taskID); // Send <taskID> to <node> Unit

    pp.IDnode = node;
    pp.IDs = sensorID;
    pp.value = taskval;

    qDebug() << "Send task to Node addr ="<< pp.IDnode
                   << "IOType =" << typeToString(pp.IDs)
                   << "Task/Value =" << pp.value;

    RF24Functions::m_radio.stopListening();
    delay(1);

    while(retries < maxR)
    {
        // send sensors value to the master
        if (!RF24Functions::m_network.write(header, &pp, sizeof(t_payload)))
        {
            qDebug() << "Send failed! retrying ...";
            retries++;
            delay(100);
            stat = GET_ADDR_FAILED;
        }
        else
        {
            qDebug() << "Packet retries =" << retries;
            stat = RET_SUCCESS;
            break;
        }
    }
    delay(1);
    RF24Functions::m_radio.startListening();
#endif
    return stat;
}

void RF24Functions::loop()
{
    qDebug() << "RF24Functions::loop: " << QThread::currentThreadId();

    static quint64 counter;

    while(!m_stop)
    {
#ifdef WIRINGPI //@TODO change this to something more suggestive
        counter++;
        // Keep the network updated
        RF24Functions::m_network.update();

        if(RF24Functions::packetReceived)
        {
            RF24Functions::status = processInformation();
            RF24Functions::packetReceived = false;
        }

        //@TODO update this later
        if(counter == 400)
            RF24Functions::status = sendTask((uint16_t) SET_DLT, (uint32_t) Temperature, (float) 60000, MAX_RETRIES, 01); // 60 sec
        if (counter == 1000)
            RF24Functions::status = sendTask((uint16_t) SET_DLT, (uint32_t) Temperature, (float) 30000, MAX_RETRIES, 01); // 30 sec
        if (counter == 1500)
            RF24Functions::status = sendTask((uint16_t) GET_MSK, (uint32_t) Temperature, (float) 0, MAX_RETRIES, 01);
        if (counter == 1600)
            counter = 0;
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
