#include <QDir>
#include <QSettings>

#include "sensorInstantiator.h"

#ifdef WIRINGPI
#include <wiringPi.h>
#include <errno.h>
#endif

sensorInstantiator::sensorInstantiator(QObject *parent) : QObject(parent)
{
    QString filePath = QDir::homePath().append(QDir::separator()).append(".piHome").append(QDir::separator()).append("sensors.ini");
    m_settings = new QSettings(filePath, QSettings::NativeFormat);

    loadSensors();
}

sensorInstantiator::~sensorInstantiator()
{
    delete m_settings;
}

void sensorInstantiator::loadSensors()
{
    int numSensors = -1;
    QString wiredEdge;
    int wiredTimeout = -1, wirelessTimeout = -1;
#ifdef WIRINGPI
    bool wiringPiInitialized = false;
#endif

    m_settings->beginGroup("GenericSettings");
    numSensors = m_settings->value("NumberOfSensors").toInt();
    m_settings->endGroup();
    m_settings->beginGroup("WiredSettings");
    wiredEdge = m_settings->value("Edge").toString();
    wiredTimeout = m_settings->value("Timeout").toInt();
    m_settings->endGroup();
    m_settings->beginGroup("WirelessSettings");
    wirelessTimeout = m_settings->value("Timeout").toInt();
    m_settings->endGroup();

    for(int i = 1; i <= numSensors; i++) // for humans not machines
    {
        QString group = "Sensor";
        group.append(QString::number(i));
        m_settings->beginGroup(group);

        HardwareType hw = StringToHardwareType(m_settings->value("HardwareType").toString());
        if(hw == Wired)
        {
            wiredSensor tmp(StringToSystemType(m_settings->value("SystemType").toString()),
                            StringToSensorType(m_settings->value("SensorType").toString()),
                            m_settings->value("Zone").toString(),
                            m_settings->value("Node").toString(),
                            m_settings->value("Address").toString(),
                            wiredEdge,
                            wiredTimeout);
#ifdef WIRINGPI
            if(!wiringPiInitialized)
            {

                // Call for winringPiSetupGpio to initialize wiringPi using Broadcom pin numbers
                if(wiringPiSetupGpio() < 0)
                    qDebug() << "Unable to setup wiringPi: " << strerror(errno);
                wiringPiInitialized = true;
            }

            int pin = StringToGPIO(tmp.getAddress());
            int edgeType = StringToEdge(tmp.getWiredEdge());

            // Call for wiringPiISR() interrupt initalization function
            // the edgeType can be: INT_EDGE_FALLING, INT_EDGE_RISING,
            // INT_EDGE_BOTH or INT_EDGE_SETUP
            //
            // The pin number is supplied in the current mode – native
            // wiringPi, BCM_GPIO, physical or Sys modes.
            //
            // This function will work in any mode, and does not need root
            // privileges to work.
            //
            //

            if(wiringPiISR(pin, edgeType, &wiredSensor::interruptHandler, &tmp) < 0)
                qDebug() << "Unable to setup ISR on " << pin << " : " << strerror(errno);
#endif

            m_wiredSensorList.append(tmp);
        }
        else
        {
            wirelessSensor tmp(StringToSystemType(m_settings->value("SystemType").toString()),
                               StringToSensorType(m_settings->value("SensorType").toString()),
                               m_settings->value("Zone").toString(),
                               m_settings->value("Node").toString(),
                               m_settings->value("Address").toString(),
                               wirelessTimeout);
            m_wirelessSensorList.append(tmp);
        }

        m_settings->endGroup();
    }
}

QList<wiredSensor> sensorInstantiator::getWiredSensors() const
{
    return m_wiredSensorList;
}

QList<wirelessSensor> sensorInstantiator::getWirelessSensors() const
{
    return m_wirelessSensorList;
}

void sensorInstantiator::saveSensors()
{
    m_settings->clear();
    m_settings->beginGroup("GenericSettings");
    m_settings->setValue("NumberOfSensors", 13);
    m_settings->endGroup();
    m_settings->beginGroup("WiredSettings");
    m_settings->setValue("Edge", "FALLING");
    m_settings->setValue("Timeout", 300);
    m_settings->endGroup();
    m_settings->beginGroup("WirelessSettings");
    m_settings->setValue("Timeout", 300);
    m_settings->endGroup();
    m_settings->beginGroup("Sensor1");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Contact));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wired));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Door");
    m_settings->setValue("Address", "GPIO_23");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor2");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Contact));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wired));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Door");
    m_settings->setValue("Address", "GPIO_24");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor3");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Contact));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wired));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Door");
    m_settings->setValue("Address", "GPIO_25");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor4");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_PIR));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Living");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor5");
    m_settings->setValue("SystemType", systemTypeToString(SmartHome));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Light));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Living");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor6");
    m_settings->setValue("SystemType", systemTypeToString(SmartHome));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Temperature));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Living");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor7");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_CO));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Kitchen");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor8");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_CH4));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Kitchen");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor9");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Vibration));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor10");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Flood));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Box2");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor11");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Level));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Balcony");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor12");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Locked));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Door");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor13");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Battery));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Living");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
}
