#ifndef RF24INTERFACE_H
#define RF24INTERFACE_H

/**
Wiring diagram NRF24L01 - Raspberry PI (all models)

PINS | NRF24L01 | PINS RPi | P1 Connector  |
=====|==========|==========|===============|
1    | GND      | Ground   | (25)          |
2    | VCC      | 3v3      | (01) or (17)  |
3    | CE       | GPIO_22  | (15)          |
4    | CSN      | GPIO_08  | (24)          |
5    | SCK      | SPI-CLK  | (23)          |
6    | MOSI     | SPI-MOSI | (19)          |
7    | MISO     | SPI-MISO | (21)          |
8    | IRQ      | GPIO_00  | (11)          | BCM GPIO_17
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

    void init();

private slots:
    void finishedSlot();

private:
    QThread *m_thread;
    RF24Functions *m_functions;

    QMutex m_mutex;
    static bool m_RF24InterfaceInitialized;
};

#endif // RF24INTERFACE_H
