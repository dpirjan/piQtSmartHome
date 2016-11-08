#ifndef RF24INTERFACE_H
#define RF24INTERFACE_H

/**
Wiring diagram NRF24L01 - Raspberry PI (all models)

PINS | NRF24L01 | PINS RPi | P1 Connector  |
=====|==========|==========|===============|
1    | GND      | Ground   | (25)          |
2    | VCC      | 3v3      | (1) or (17)   |
3    | CE       | GPIO22   | (15)          |
4    | CSN      | GPIO08   | (24)          |
5    | SCK      | SPI-CLK  | (23)          |
6    | MOSI     | SPI-MOSI | (19)          |
7    | MISO     | SPI-MISO | (21)          |
8    | IRQ      | GPIO2   | (13)            |
*/

#include <QObject>
#include <QMutex>

class QThread;
class RF24Functions;

class RF24Interface : public QObject
{
    Q_OBJECT

    explicit RF24Interface(QObject *parent = 0);
    virtual ~RF24Interface();
    Q_DISABLE_COPY(RF24Interface)

public:
    static RF24Interface &instance()
    {
        static RF24Interface obj;
        return obj;
    }

    static void interruptHandler(void);
    void init();

private slots:
    void finishedSlot();

private:
    QThread *m_thread;
    RF24Functions *m_functions;

    static quint64 m_counter;

    QMutex m_mutex;
    static bool m_RF24InterfaceInitialized;
};

#endif // RF24INTERFACE_H
