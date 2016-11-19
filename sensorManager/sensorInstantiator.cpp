#include <QDir>
#include <QSettings>

#include "sensorInstantiator.h"
#include "mailManagerInterface.h"
#include "databaseManagerInterface.h"

sensorInstantiator::sensorInstantiator(QObject *parent) : QObject(parent)
{
    QString filePath = QDir::homePath().append(QDir::separator());
    filePath.append(".piHome").append(QDir::separator()).append("sensors.ini");
    m_settings = new QSettings(filePath, QSettings::NativeFormat);

    loadSensors();
}

sensorInstantiator::~sensorInstantiator()
{
    delete m_settings;

    for(int count=0; count<m_wiredSensorList.count(); count++)
        delete m_wiredSensorList.takeAt(count);

    for(int count=0; count<m_wirelessSensorList.count(); count++)
        delete m_wirelessSensorList.takeAt(count);
}

void sensorInstantiator::loadSensors()
{
    int numSensors = -1;
    QString wiredEdge;
    int wiredTimeout = -1, wirelessTimeout = -1;
    bool smtpInitialized = true;

    m_settings->beginGroup("GenericSettings");
    numSensors = m_settings->value("NumberOfSensors").toInt();
    m_sendMail = m_settings->value("SendMailOnAlarm").toBool();
    m_settings->endGroup();

    if(m_sendMail)
    {
        if(!mailManagerInterface::instance().connectToServer())
        {
            qCritical() << "Cannot connect to smtp server!";
            smtpInitialized = false;
        }
        if(!mailManagerInterface::instance().loginToServer())
        {
            qCritical() << "Cannot authenticate to smtp server!";
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
            wiredSensor *tmp = new wiredSensor(
                        StringToSystemType(m_settings->value("SystemType").toString()),
                        StringToType(m_settings->value("SensorType").toString()),
                        m_settings->value("Zone").toString(),
                        m_settings->value("Node").toString(),
                        m_settings->value("Address").toString(),
                        m_sendMail & smtpInitialized,
                        wiredEdge,
                        wiredTimeout);
            m_wiredSensorList.append(tmp);

            databaseManagerInterface::instance().insertIO(
                        m_settings->value("SystemType").toString(),
                        m_settings->value("HardwareType").toString(),
                        "Sensor",
                        m_settings->value("SensorType").toString(),
                        m_settings->value("Zone").toString(),
                        m_settings->value("Node").toString(),
                        m_settings->value("Address").toString());
        }
        else
        {
            wirelessSensor *tmp = new wirelessSensor(
                        StringToSystemType(m_settings->value("SystemType").toString()),
                        StringToType(m_settings->value("SensorType").toString()),
                        m_settings->value("Zone").toString(),
                        m_settings->value("Node").toString(),
                        m_settings->value("Address").toString(),
                        m_sendMail & smtpInitialized,
                        wirelessTimeout);
            m_wirelessSensorList.append(tmp);

            databaseManagerInterface::instance().insertIO(
                        m_settings->value("SystemType").toString(),
                        m_settings->value("HardwareType").toString(),
                        "Sensor",
                        m_settings->value("SensorType").toString(),
                        m_settings->value("Zone").toString(),
                        m_settings->value("Node").toString(),
                        m_settings->value("Address").toString());
        }

        m_settings->endGroup();
    }
}

QList<wiredSensor *> sensorInstantiator::getWiredSensors() const
{
    return m_wiredSensorList;
}

QList<wirelessSensor *> sensorInstantiator::getWirelessSensors() const
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
        settings->setValue("NumberOfSensors", 14);
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
        settings->setValue("SensorType", typeToString(Contact));
        settings->setValue("HardwareType", hardwareTypeToString(Wired));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Entrance Door");
        settings->setValue("Address", "GPIO_23");
        settings->endGroup();
        settings->beginGroup("Sensor2");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(Contact));
        settings->setValue("HardwareType", hardwareTypeToString(Wired));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Entrance Door");
        settings->setValue("Address", "GPIO_24");
        settings->endGroup();
        settings->beginGroup("Sensor3");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(Contact));
        settings->setValue("HardwareType", hardwareTypeToString(Wired));
        settings->setValue("Zone", "Dressing");
        settings->setValue("Node", "Main");
        settings->setValue("Address", "GPIO_25");
        settings->endGroup();
        settings->beginGroup("Sensor4");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(PIR));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Living");
        settings->setValue("Node", "Main");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor5");
        settings->setValue("SystemType", systemTypeToString(SmartHome));
        settings->setValue("SensorType", typeToString(Light));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Master Bedroom");
        settings->setValue("Node", "Main");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor6");
        settings->setValue("SystemType", systemTypeToString(SmartHome));
        settings->setValue("SensorType", typeToString(Temperature));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Kids Bedroom");
        settings->setValue("Node", "Main");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor7");
        settings->setValue("SystemType", systemTypeToString(SmartHome));
        settings->setValue("SensorType", typeToString(Humidity));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Kids Bedroom");
        settings->setValue("Node", "Main");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor8");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(CO));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Kitchen");
        settings->setValue("Node", "Cooker");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor9");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(CH4));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Kitchen");
        settings->setValue("Node", "Cooker");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor10");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(Vibration));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Entrance Door");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor11");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(Flood));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Bathroom");
        settings->setValue("Node", "Main");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor12");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(WaterLevel));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Balcony");
        settings->setValue("Node", "Plants");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor13");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(DoorLocked));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Entrance Door");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Sensor14");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("SensorType", typeToString(BatteryLevel));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Toilet");
        settings->setValue("Node", "Main");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();

        delete settings;
    }

    return returnCode;
}
