#include <QDir>
#include <QSettings>

#include "sensorInstantiator.h"
#include "mailManagerInterface.h"

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
    if(m_sendMail)
    {
        if(!mailManagerInterface::instance().disconnectFromServer())
            qDebug() << "Error occured while disconnecting from the smtp server ...";
    }
    delete m_settings;
}

void sensorInstantiator::loadSensors()
{
    int numSensors = -1;
    QString wiredEdge;
    int wiredTimeout = -1, wirelessTimeout = -1;
    bool smtpInitialized = true;
#ifdef WIRINGPI
    bool wiringPiInitialized = false;
#endif

    m_settings->beginGroup("GenericSettings");
    numSensors = m_settings->value("NumberOfSensors").toInt();
    m_sendMail = m_settings->value("SendMailOnAlarm").toBool();
    m_settings->endGroup();

    if(m_sendMail)
    {
        if(!mailManagerInterface::instance().connectToServer())
        {
            qDebug() << "Cannot connect to smtp server!";
            smtpInitialized = false;
        }
        if(!mailManagerInterface::instance().loginToServer())
        {
            qDebug() << "Cannot authenticate to smtp server!";
            smtpInitialized = false;
        }
    }

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

            tmp.setSendMail(m_sendMail & smtpInitialized);

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

            tmp.setSendMail(m_sendMail & smtpInitialized);

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

bool sensorInstantiator::firstRunInitSensors()
{
    bool returnCode = false;

    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("sensors.ini");

    qDebug() << "firstRunInitSensors()" << settingsPath << " " << QFile(settingsPath).exists();

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;

        QSettings *settings = new QSettings(settingsPath, QSettings::NativeFormat);

        settings->clear();
        settings->beginGroup("GenericSettings");
        settings->setValue("NumberOfSensors", 13);
        settings->setValue("SendMailOnAlarm", true);
        settings->endGroup();
        settings->beginGroup("WiredSettings");
        settings->setValue("Edge", "FALLING");
        settings->setValue("Timeout", 300);
        settings->endGroup();
        settings->beginGroup("WirelessSettings");
        settings->setValue("Timeout", 300);
        settings->endGroup();
        settings->beginGroup("Sensor1");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Contact));
        settings->setValue("HardwareType", hardwareTypeToString(Wired));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Door");
        settings->setValue("Address", "GPIO_23");
        settings->endGroup();
        settings->beginGroup("Sensor2");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Contact));
        settings->setValue("HardwareType", hardwareTypeToString(Wired));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Door");
        settings->setValue("Address", "GPIO_24");
        settings->endGroup();
        settings->beginGroup("Sensor3");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Contact));
        settings->setValue("HardwareType", hardwareTypeToString(Wired));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Door");
        settings->setValue("Address", "GPIO_25");
        settings->endGroup();
        settings->beginGroup("Sensor4");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_PIR));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Living");
        settings->setValue("Node", "Box1");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor5");
        settings->setValue("SystemType", systemTypeToString(SmartHome));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Light));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Living");
        settings->setValue("Node", "Box1");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor6");
        settings->setValue("SystemType", systemTypeToString(SmartHome));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Temperature));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Living");
        settings->setValue("Node", "Box1");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor7");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_CO));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Kitchen");
        settings->setValue("Node", "Box1");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor8");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_CH4));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Kitchen");
        settings->setValue("Node", "Box1");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor9");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Vibration));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Box1");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor10");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Flood));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Box2");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor11");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Level));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Balcony");
        settings->setValue("Node", "Box1");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor12");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Locked));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Door");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor13");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", sensorTypeToString(Sensor_Battery));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Living");
        settings->setValue("Node", "Box1");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();

        delete settings;
    }

    return returnCode;
}
