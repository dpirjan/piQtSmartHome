#include <QDir>

#include "actuatorInstantiator.h"

actuatorInstantiator::actuatorInstantiator(QObject *parent) : QObject(parent)
{
    QString filePath = QDir::homePath().append(QDir::separator()).append(".piHome").append(QDir::separator()).append("actuators.ini");
    m_settings = new QSettings(filePath, QSettings::NativeFormat);
}

actuatorInstantiator::~actuatorInstantiator()
{
    delete m_settings;
}

void actuator::debugActuator() const
{
    qDebug() << "==============================================";
    qDebug() << systemTypeToString(m_system);
    qDebug() << actuatorTypeToString(m_type);
    qDebug() << hardwareTypeToString(m_hard);
    qDebug() << m_zone;
    qDebug() << m_node;
    qDebug() << m_address;
    qDebug() << m_value;
}

QList<actuator> actuatorInstantiator::loadActuators()
{
    int numActuators = -1;
    QList<actuator> list;

    m_settings->beginGroup("GenericSettings");
    numActuators = m_settings->value("NumberOfActuators").toInt();
    m_settings->endGroup();

    for(int i = 1; i <= numActuators; i++) // for humans not machines
    {
        QString group = "Actuator";
        group.append(QString::number(i));
        m_settings->beginGroup(group);
        actuator tmp (
        StringToSystemType(m_settings->value("SystemType").toString()),
        StringToActuatorType(m_settings->value("ActuatorType").toString()),
        StringToHardwareType(m_settings->value("HardwareType").toString()),
        m_settings->value("Zone").toString(),
        m_settings->value("Node").toString(),
        m_settings->value("Address").toString());
        list.append(tmp);
        m_settings->endGroup();
    }

    return list;
}

void actuatorInstantiator::saveActuators()
{
    m_settings->clear();
    m_settings->beginGroup("GenericSettings");
    m_settings->setValue("NumberOfActuators", 6);
    m_settings->endGroup();
    m_settings->beginGroup("Actuator1");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("ActuatorType", actuatorTypeToString(Actuator_Siren));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Door");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Actuator2");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("ActuatorType", actuatorTypeToString(Actuator_Buzzer));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Kitchen");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Actuator3");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("ActuatorType", actuatorTypeToString(Actuator_Relay));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Kitchen");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Actuator4");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("ActuatorType", actuatorTypeToString(Actuator_DoorBell));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Door");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Actuator5");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("ActuatorType", actuatorTypeToString(Actuator_IR));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Hall");
    m_settings->setValue("Node", "Box2");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
    m_settings->beginGroup("Actuator6");
    m_settings->setValue("SystemType", systemTypeToString(HomeAlarm));
    m_settings->setValue("ActuatorType", actuatorTypeToString(Actuator_Pump));
    m_settings->setValue("HardwareType", hardwareTypeToString(Wireless));
    m_settings->setValue("Zone", "Balcony");
    m_settings->setValue("Node", "Box1");
    m_settings->setValue("Address", "SPI_0");
    m_settings->endGroup();
}
