#include <QDir>

#include "actuatorInstantiator.h"
#include "databaseManagerInterface.h"

actuatorInstantiator::actuatorInstantiator(QObject *parent) : QObject(parent)
{
    QString filePath = QDir::homePath().append(QDir::separator()).append(".piHome").append(QDir::separator()).append("actuators.ini");
    m_settings = new QSettings(filePath, QSettings::NativeFormat);

    loadActuators();

    // settings file no longer needed
    m_settings->deleteLater();
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

    m_settings->beginGroup("GenericSettings");
    numActuators = m_settings->value("NumberOfActuators").toInt();
    m_settings->endGroup();

    for(int i = 1; i <= numActuators; i++) // for humans not machines
    {
        QString group = "Actuator";
        group.append(QString::number(i));
        m_settings->beginGroup(group);
        actuator *tmp = new actuator(
                    StringToSystemType(m_settings->value("SystemType").toString()),
                    StringToType(m_settings->value("ActuatorType").toString()),
                    StringToHardwareType(m_settings->value("HardwareType").toString()),
                    m_settings->value("Zone").toString(),
                    m_settings->value("Node").toString(),
                    m_settings->value("Address").toString());
        m_actuatorsList.append(tmp);

        databaseManagerInterface::instance().insertIO(
                    m_settings->value("SystemType").toString(),
                    m_settings->value("HardwareType").toString(),
                    "Actuator",
                    m_settings->value("ActuatorType").toString(),
                    m_settings->value("Zone").toString(),
                    m_settings->value("Node").toString(),
                    m_settings->value("Address").toString());

        m_settings->endGroup();
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

bool actuatorInstantiator::firstRunInitActuators()
{
    bool returnCode = false;

    QString settingsPath = QDir::homePath().
            append(QDir::separator()).
            append(".piHome").
            append(QDir::separator()).
            append("actuators.ini");

    qDebug() << "firstRunInitActuators()" << settingsPath << " " << QFile(settingsPath).exists();

    if(!QFile(settingsPath).exists())
    {
        returnCode = true;

        QSettings *settings = new QSettings(settingsPath, QSettings::NativeFormat);

        settings->clear();
        settings->beginGroup("GenericSettings");
        settings->setValue("NumberOfActuators", 6);
        settings->endGroup();
        settings->beginGroup("Actuator1");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("ActuatorType", typeToString(Siren));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Entrance Door");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Actuator2");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("ActuatorType", typeToString(Buzzer));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Kitchen");
        settings->setValue("Node", "Cooker");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Actuator3");
        settings->setValue("SystemType", systemTypeToString(SmartHome));
        settings->setValue("ActuatorType", typeToString(Relay));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Air Conditioning");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Actuator4");
        settings->setValue("SystemType", systemTypeToString(HomeAlarm));
        settings->setValue("ActuatorType", typeToString(DoorBell));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Door");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Actuator5");
        settings->setValue("SystemType", systemTypeToString(SmartHome));
        settings->setValue("ActuatorType", typeToString(IR));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Hall");
        settings->setValue("Node", "Air Conditioning");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();
        settings->beginGroup("Actuator6");
        settings->setValue("SystemType", systemTypeToString(SmartHome));
        settings->setValue("ActuatorType", typeToString(Pump));
        settings->setValue("HardwareType", hardwareTypeToString(Wireless));
        settings->setValue("Zone", "Balcony");
        settings->setValue("Node", "Plants");
        settings->setValue("Address", "SPI_0");
        settings->endGroup();

        delete settings;
    }

    return returnCode;
}
