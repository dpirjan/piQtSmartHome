#include <QDir>
#include <QStandardPaths>

#include "actuatorInstantiator.h"
#include "databaseManagerInterface.h"

actuatorInstantiator::actuatorInstantiator(QObject *parent) : QObject(parent)
{
    loadActuators();
}

actuatorInstantiator::~actuatorInstantiator()
{
    for(int count = 0; count<m_actuatorsList.count(); count++)
        delete m_actuatorsList.takeAt(count);
}

void actuator::debugActuator() const
{
    qDebug() << "==============================================";
    qDebug() << systemTypeToString(m_system);
    qDebug() << typeToString(m_type);
    qDebug() << hardwareTypeToString(m_hard);
    qDebug() << m_zone;
    qDebug() << m_node;
    qDebug() << m_address;
    qDebug() << m_value;
}

void actuatorInstantiator::loadActuators()
{
    int numActuators = -1;

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).
            append(QDir::separator()).
            append(QCoreApplication::organizationName()).
            append(QDir::separator()).
            append("actuators.conf");
    QSettings settings(settingsPath, QSettings::NativeFormat);

    settings.beginGroup("GenericSettings");
    numActuators = settings.value("NumberOfActuators").toInt();
    settings.endGroup();

    for(int i = 1; i <= numActuators; i++) // for humans not machines
    {
        QString group = "Actuator";
        group.append(QString::number(i));
        settings.beginGroup(group);
        actuator *tmp = new actuator(
                    StringToSystemType(settings.value("SystemType").toString()),
                    StringToType(settings.value("ActuatorType").toString()),
                    StringToHardwareType(settings.value("HardwareType").toString()),
                    settings.value("Zone").toString(),
                    settings.value("Node").toString(),
                    settings.value("Address").toString());
        m_actuatorsList.append(tmp);

        databaseManagerInterface::instance().insertIO(
                    settings.value("SystemType").toString(),
                    settings.value("HardwareType").toString(),
                    "Actuator",
                    settings.value("ActuatorType").toString(),
                    settings.value("Zone").toString(),
                    settings.value("Node").toString(),
                    settings.value("Address").toString());

        settings.endGroup();
    }
}

QList<actuator *> actuatorInstantiator::getActuators() const
{
    return m_actuatorsList;
}

actuator* actuatorInstantiator::findActuator(const IOType &actuatorType,
                                             const QString &actuatorAddress) const
{
    bool found = false;
    int counter;
    for(counter = 0; counter < m_actuatorsList.count(); counter++)
        if(m_actuatorsList.at(counter)->getActuatorType() == actuatorType &&
                m_actuatorsList.at(counter)->getAddress() == actuatorAddress)
        {
            // assuming is not possible to have two actuators with same type
            // at the same address
            found = true;
            qDebug() << "Found " << typeToString(actuatorType)
                     << " actuator with " << actuatorAddress
                     << " address on position " << counter;
            break;
        }

    if(found)
        return m_actuatorsList.at(counter);
    else
    {
        qCritical() << "Cannot find specified actuator ("
                    << typeToString(actuatorType) << ", " << actuatorAddress
                    << ") !";
        return NULL;
    }
}

QList<actuator*> actuatorInstantiator::findActuators(const IOType &actuatorType) const
{
    QList <actuator *> retList;
    for(int counter = 0; counter < m_actuatorsList.count(); counter++)
    if(m_actuatorsList.at(counter)->getActuatorType() == actuatorType)
    {
        qDebug() << "Found " << typeToString(actuatorType)
                 << " actuator with " << m_actuatorsList.at(counter)->getAddress()
                 << " address on position " << counter;
        retList.append(m_actuatorsList.at(counter));
    }

    return retList;
}

bool actuatorInstantiator::firstRunInitActuators()
{
    bool returnCode = false;

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).
            append(QDir::separator()).
            append(QCoreApplication::organizationName()).
            append(QDir::separator()).
            append("actuators.conf");

    qDebug() << "Path: " << settingsPath;

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;

        QSettings settings(settingsPath, QSettings::NativeFormat);

        settings.clear();
        settings.beginGroup("GenericSettings");
        settings.setValue("NumberOfActuators", 7);
        settings.endGroup();
        settings.beginGroup("Actuator1");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("ActuatorType", typeToString(Siren));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Entrance Door");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Actuator2");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("ActuatorType", typeToString(Buzzer));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Kitchen");
        settings.setValue("Node", "Cooker");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Actuator3");
        settings.setValue("SystemType", systemTypeToString(SmartHome));
        settings.setValue("ActuatorType", typeToString(Relay));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Air Conditioning");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Actuator4");
        settings.setValue("SystemType", systemTypeToString(HomeAlarm));
        settings.setValue("ActuatorType", typeToString(DoorBell));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Door");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Actuator5");
        settings.setValue("SystemType", systemTypeToString(SmartHome));
        settings.setValue("ActuatorType", typeToString(IR));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Air Conditioning");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Actuator6");
        settings.setValue("SystemType", systemTypeToString(SmartHome));
        settings.setValue("ActuatorType", typeToString(Pump));
        settings.setValue("HardwareType", hardwareTypeToString(Wireless));
        settings.setValue("Zone", "Balcony");
        settings.setValue("Node", "Plants");
        settings.setValue("Address", "SPI_0");
        settings.endGroup();
        settings.beginGroup("Actuator7");
        settings.setValue("SystemType", systemTypeToString(SmartHome));
        settings.setValue("ActuatorType", typeToString(DoorLock));
        settings.setValue("HardwareType", hardwareTypeToString(Wired));
        settings.setValue("Zone", "Hall");
        settings.setValue("Node", "Door");
        settings.setValue("Address", "GPIO_17");
        settings.endGroup();
    }

    return returnCode;
}
