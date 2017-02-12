#include <QDir>
#include <QSettings>
#include <QStandardPaths>

#include "sensorInstantiator.h"
#include "mailManagerInterface.h"
#include "databaseManagerInterface.h"

sensorInstantiator::sensorInstantiator(QObject *parent) : QObject(parent)
{
    loadSensors();
}

sensorInstantiator::~sensorInstantiator()
{
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

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).
            append(QDir::separator()).
            append(QCoreApplication::organizationName()).
            append(QDir::separator()).
            append("sensors.conf");
    QSettings settings(settingsPath, QSettings::NativeFormat);

    settings.beginGroup("GenericSettings");
    numSensors = settings.value("NumberOfSensors").toInt();
    m_sendMail = settings.value("SendMailOnAlarm").toBool();
    settings.endGroup();

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

    settings.beginGroup("WiredSettings");
    wiredEdge = settings.value("Edge").toString();
    wiredTimeout = settings.value("Timeout").toInt();
    settings.endGroup();
    settings.beginGroup("WirelessSettings");
    wirelessTimeout = settings.value("Timeout").toInt();
    settings.endGroup();

    for(int i = 1; i <= numSensors; i++) // for humans not machines
    {
        QString group = "Sensor";
        group.append(QString::number(i));
        settings.beginGroup(group);

        HardwareType hw = StringToHardwareType(settings.value("HardwareType").toString());
        if(hw == Wired)
        {
            wiredSensor *tmp = new wiredSensor(
                        StringToSystemType(settings.value("SystemType").toString()),
                        StringToType(settings.value("SensorType").toString()),
                        settings.value("Zone").toString(),
                        settings.value("Node").toString(),
                        settings.value("Address").toString(),
                        m_sendMail & smtpInitialized,
                        wiredEdge,
                        wiredTimeout);
            m_wiredSensorList.append(tmp);

            databaseManagerInterface::instance().insertIO(
                        settings.value("SystemType").toString(),
                        settings.value("HardwareType").toString(),
                        "Sensor",
                        settings.value("SensorType").toString(),
                        settings.value("Zone").toString(),
                        settings.value("Node").toString(),
                        settings.value("Address").toString());
        }
        else
        {
            wirelessSensor *tmp = new wirelessSensor(
                        StringToSystemType(settings.value("SystemType").toString()),
                        StringToType(settings.value("SensorType").toString()),
                        settings.value("Zone").toString(),
                        settings.value("Node").toString(),
                        settings.value("Address").toString(),
                        m_sendMail & smtpInitialized,
                        wirelessTimeout);
            m_wirelessSensorList.append(tmp);

            databaseManagerInterface::instance().insertIO(
                        settings.value("SystemType").toString(),
                        settings.value("HardwareType").toString(),
                        "Sensor",
                        settings.value("SensorType").toString(),
                        settings.value("Zone").toString(),
                        settings.value("Node").toString(),
                        settings.value("Address").toString());
        }

        settings.endGroup();
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

wirelessSensor* sensorInstantiator::findWirelessSensor(const IOType &sensorType,
                                                       const QString &sensorAddress) const
{
    bool found = false;
    int counter;
    for(counter = 0; counter < m_wirelessSensorList.count(); counter++)
        if(m_wirelessSensorList.at(counter)->getSensorType() == sensorType &&
                m_wirelessSensorList.at(counter)->getAddress() == sensorAddress)
        {
            // assuming is not possible to have two sensors with same type
            // at the same address
            found = true;
            qDebug() << "Found " << typeToString(sensorType) << " sensor with "
                     << sensorAddress << " address on position " << counter;
            break;
        }

    if(found)
        return m_wirelessSensorList.at(counter);
    else
    {
        qCritical() << "Cannot find specified sensor (" << typeToString(sensorType)
                    << ", " << sensorAddress << ") !";
        return NULL;
    }
}

bool sensorInstantiator::firstRunInitSensors()
{
    bool returnCode = false;

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).
            append(QDir::separator()).
            append(QCoreApplication::organizationName()).
            append(QDir::separator()).
            append("sensors.conf");

    qDebug() << "Path: " << settingsPath;

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;

        QSettings settings(settingsPath, QSettings::NativeFormat);

        settings.clear();
        settings.beginGroup("GenericSettings");
        settings.setValue("NumberOfSensors", 14);
        settings.setValue("SendMailOnAlarm", true);
        settings.endGroup();
        settings.beginGroup("WiredSettings");
        settings.setValue("Edge", "FALLING");
        settings.setValue("Timeout", 300);
        settings.endGroup();
        settings.beginGroup("WirelessSettings");
        settings.setValue("Timeout", 300);
        settings.endGroup();
        settings.beginGroup("Sensor1");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(Contact));
        settings.setValue("HardwareType", hardwareTypeToString(Wired));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Entrance Door");
        settings.setValue("Address", "GPIO_23");
        settings.endGroup();
        settings.beginGroup("Sensor2");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(Contact));
        settings.setValue("HardwareType", hardwareTypeToString(Wired));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Entrance Door");
        settings.setValue("Address", "GPIO_24");
        settings.endGroup();
        settings.beginGroup("Sensor3");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(Contact));
        settings.setValue("HardwareType", hardwareTypeToString(Wired));
        settings.setValue("Zone", "Dressing");
        settings.setValue("Node", "Main");
        settings.setValue("Address", "GPIO_25");
        settings.endGroup();
        settings.beginGroup("Sensor4");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(PIR));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Living");
        settings.setValue("Node", "Main");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor5");
        settings.setValue("SystemType", systemTypeToString(SmartHome));
        settings.setValue("SensorType", typeToString(Light));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Master Bedroom");
        settings.setValue("Node", "Main");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor6");
        settings.setValue("SystemType", systemTypeToString(SmartHome));
        settings.setValue("SensorType", typeToString(Temperature));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Kids Bedroom");
        settings.setValue("Node", "Main");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor7");
        settings.setValue("SystemType", systemTypeToString(SmartHome));
        settings.setValue("SensorType", typeToString(Humidity));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Kids Bedroom");
        settings.setValue("Node", "Main");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor8");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(CO));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Kitchen");
        settings.setValue("Node", "Cooker");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor9");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(CH4));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Kitchen");
        settings.setValue("Node", "Cooker");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor10");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(Vibration));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Entrance Door");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor11");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(Flood));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Bathroom");
        settings.setValue("Node", "Main");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor12");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(WaterLevel));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Balcony");
        settings.setValue("Node", "Plants");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor13");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(DoorLocked));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Entrance Door");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Sensor14");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("SensorType", typeToString(BatteryLevel));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Toilet");
        settings.setValue("Node", "Main");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
    }

    return returnCode;
}
