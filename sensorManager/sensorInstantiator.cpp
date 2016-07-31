#include <QDir>

#include "sensorInstantiator.h"

#ifdef WIRINGPI
#include <wiringPi.h>
#endif

sensorInstantiator::sensorInstantiator(QObject *parent) : QObject(parent)
{
    QString filePath = QDir::homePath().append(QDir::separator()).append(".piHome").append(QDir::separator()).append("sensors.ini");
    m_settings = new QSettings(filePath, QSettings::NativeFormat);
}

sensorInstantiator::~sensorInstantiator()
{
    m_settings->~QSettings();
}

QList<sensor> sensorInstantiator::loadSensors()
{
    int numSensors = -1;
    QString wiredEdge;
    int wiredTimeout = -1;
    bool initializeWired = false;

    m_settings->beginGroup("GenericSettings");
    numSensors = m_settings->value("NumberOfSensors").toInt();
    m_settings->endGroup();
    m_settings->beginGroup("WiredSettings");
    wiredEdge = m_settings->value("Edge").toString();
    wiredTimeout = m_settings->value("Timeout").toInt();
    m_settings->endGroup();

    for(int i = 1; i <= numSensors; i++) // for humans not machines
    {
        QString group = "Sensor";
        group.append(QString::number(i));
        m_settings->beginGroup(group);
        sensor tmp(StringToSystemType(m_settings->value("SystemType").toString()),
                   StringToSensorType(m_settings->value("SensorType").toString()),
                   StringToHardwareType(m_settings->value("HardwareType").toString()),
                   m_settings->value("Zone").toString(),
                   m_settings->value("Node").toString(),
                   m_settings->value("Address").toString());
        if(tmp.getHardwareType() == Wired)
        {
            tmp.setSupportInterrupt(true);
            tmp.setWiredEdge(wiredEdge);
            tmp.setWiredTimeout(wiredTimeout);
            // @TODO
            // Make link with the wiringPi
            if(!initializeWired)
            {
#ifdef WIRINGPI
                // Call for winringPi initialization function
                int wiringPiSetupGpio (void) ;
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
                //int wiringPiISR (int pin, int edgeType,  void (*function)(void)) ;
#endif
                initializeWired = true;
            }
        }
        if(tmp.getSensorType() == Sensor_CO ||
                tmp.getSensorType() == Sensor_CH4 ||
                tmp.getSensorType() == Sensor_Flood ||
                tmp.getSensorType() == Sensor_Vibration)
            tmp.setSupportInterrupt(true);
        m_sensorList.append(tmp);
        m_settings->endGroup();
    }

    return m_sensorList;
}

void sensorInstantiator::saveSensors()
{
    m_settings->clear();
    m_settings->beginGroup("GenericSettings");
    m_settings->setValue("NumberOfSensors", 11);
    m_settings->endGroup();
    m_settings->beginGroup("WiredSettings");
    m_settings->setValue("Edge", "FALLING");
    m_settings->setValue("Timeout", 300);
    m_settings->endGroup();
    m_settings->beginGroup("Sensor1");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_PIR));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Living");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor2");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Contact));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wired));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Door");
    m_settings->setValue("Address", "GPIO_17");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor3");
    m_settings->setValue("SystemType", systemTypeToString(SmartHome));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Light));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Living");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor4");
    m_settings->setValue("SystemType", systemTypeToString(SmartHome));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Temperature));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Living");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor5");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_CO));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Kitchen");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor6");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_CH4));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Kitchen");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor7");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Vibration));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor8");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Flood));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Box2");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor9");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Level));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Balcony");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor10");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Locked));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Door");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Sensor11");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("SensorType", sensorTypeToString(Sensor_Battery));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Living");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
}
