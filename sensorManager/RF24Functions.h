#ifndef RF24FUNCTIONS_H
#define RF24FUNCTIONS_H

#include <QObject>

#ifdef WIRINGPI
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

#define LAST_RX             7
#endif

class RF24Functions : public QObject
{
    Q_OBJECT

protected:
    explicit RF24Functions(QObject *parent = 0);
    virtual ~RF24Functions();
    Q_DISABLE_COPY(RF24Functions)

    static void interruptHandler(void *);

    friend class RF24Interface;

protected slots:
    void stop();
    void loop();

signals:
    void finished();

#ifdef WIRINGPI
public:
    // RF payload transmition:
    struct t_payload
    {
        uint32_t IDs;                   // sensor or actuator ID        (32 bits)
        float value;                    // a) the value read from the sensor  (32 bits)
                                        // b) preset value for actuator
        uint16_t IDnode;                // node ID - redundant information  (16 bits)
    } __attribute__((packed));

    // receive queue (ring buffer)
    struct t_RX {
        int16_t newD;                   // data is availlable to process
        RF24NetworkHeader h;            // data header
        uint8_t p[sizeof(t_payload)];   // data payload
    } __attribute__((packed));
#endif

private:
    int processInformation() const;
    int sendTask(const uint16_t&,
                 const uint32_t&,
                 const float&,
                 const int&,
                 const uint16_t&) const;

    bool m_stop;

#ifdef WIRINGPI
    static RF24 m_radio;
    static RF24Network m_network;

    static struct t_RX rx[LAST_RX + 1];

    static int head;                    // current RX index to be filled
    static int posRX;                   // message to be handled
    static int status;                  // used for tracking status                         //g_stat
    static bool packetReceived;         // used for notifying when communication occured    //g_isrRF_ON
#endif
};

#endif // RF24FUNCTIONS_H
