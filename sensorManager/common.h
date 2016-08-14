#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDebug>

#ifdef WIRINGPI
#include<wiringPi.h>
#endif

namespace piHome {

typedef enum {
    HomeAlarm = 1,
    SmartHome
} SystemType;

typedef enum {
    Wired = 4,
    Wireless
} HardwareType;

typedef enum {
    Sensor_PIR = 7,
    Sensor_Contact,
    Sensor_Light, // Light intensity
    Sensor_Temperature, // + humidity => DHT22
    Sensor_CO, // Carbon Monoxid
    Sensor_CH4, // Methane
    Sensor_Vibration, // Knock sensor
    Sensor_Flood, // Water presence
    Sensor_Level, // Water level
    Sensor_Locked, // Door locked
    Sensor_Battery // Read battery voltage on Analog
} SensorType;

typedef enum {
    Actuator_Siren = 19,
    Actuator_Buzzer,
    Actuator_Relay,
    Actuator_DoorBell,
    Actuator_IR,
    Actuator_Pump // Water pump
} ActuatorType;

inline QString systemTypeToString(const SystemType &value)
{
    QString returnString;
    switch (value)
    {
    case HomeAlarm:
        returnString = "HomeAlarm";
        break;
    case SmartHome:
        returnString = "SmartHome";
        break;
    default:
        qDebug() << "systemTypeToString() - Value not supported!";
        break;
    }
    return returnString;
}

inline SystemType StringToSystemType(const QString &str)
{
    bool found = false;
    SystemType returnValue;
    if(str == "HomeAlarm")
    {
        returnValue = HomeAlarm;
        found = true;
    }
    if(str == "SmartHome")
    {
        returnValue = SmartHome;
        found = true;
    }

    if(!found)
        qDebug() << "StringToSystemType() - Value not supported!";

    return returnValue;
}

inline QString hardwareTypeToString(const HardwareType &value)
{
    QString returnString;
    switch (value)
    {
    case Wired:
        returnString = "Wired";
        break;
    case Wireless:
        returnString = "Wireless";
        break;
    default:
        qDebug() << "hardwareTypeToString() - Value not supported!";
        break;
    }
    return returnString;
}

inline HardwareType StringToHardwareType(const QString &str)
{
    bool found = false;
    HardwareType returnValue;
    if(str == "Wired")
    {
        returnValue = Wired;
        found = true;
    }
    if(str == "Wireless")
    {
        returnValue = Wireless;
        found = true;
    }
    if(!found)
        qDebug() << "StringToHardwareType() - Value not supported!";

    return returnValue;
}

inline QString sensorTypeToString(const SensorType &value)
{
    QString returnString;
    switch (value)
    {
    case Sensor_PIR:
        returnString = "Sensor_PIR";
        break;
    case Sensor_Contact:
        returnString = "Sensor_Contact";
        break;
    case Sensor_Light:
        returnString = "Sensor_Light";
        break;
    case Sensor_Temperature:
        returnString = "Sensor_Temperature";
        break;
    case Sensor_CO:
        returnString = "Sensor_CO";
        break;
    case Sensor_CH4:
        returnString = "Sensor_CH4";
        break;
    case Sensor_Vibration:
        returnString = "Sensor_Vibration";
        break;
    case Sensor_Flood:
        returnString = "Sensor_Flood";
        break;
    case Sensor_Level:
        returnString = "Sensor_Level";
        break;
    case Sensor_Locked:
        returnString = "Sensor_Locked";
        break;
    case Sensor_Battery:
        returnString = "Sensor_Battery";
        break;
    default:
        qDebug() << "sensorTypeToString() - Value not supported!";
        break;
    }
    return returnString;
}

inline SensorType StringToSensorType(const QString &str)
{
    bool found = false;
    SensorType returnValue;
    if(str == "Sensor_PIR")
    {
        returnValue = Sensor_PIR;
        found = true;
    }
    if(str == "Sensor_Contact")
    {
        returnValue = Sensor_Contact;
        found = true;
    }
    if(str == "Sensor_Light")
    {
        returnValue = Sensor_Light;
        found = true;
    }
    if(str == "Sensor_Temperature")
    {
        returnValue = Sensor_Temperature;
        found = true;
    }
    if(str == "Sensor_CO")
    {
        returnValue = Sensor_CO;
        found = true;
    }
    if(str == "Sensor_CH4")
    {
        returnValue = Sensor_CH4;
        found = true;
    }
    if(str == "Sensor_Vibration")
    {
        returnValue = Sensor_Vibration;
        found = true;
    }
    if(str == "Sensor_Flood")
    {
        returnValue = Sensor_Flood;
        found = true;
    }
    if(str == "Sensor_Level")
    {
        returnValue = Sensor_Level;
        found = true;
    }
    if(str == "Sensor_Locked")
    {
        returnValue = Sensor_Locked;
        found = true;
    }
    if(str == "Sensor_Battery")
    {
        returnValue = Sensor_Battery;
        found = true;
    }

    if(!found)
        qDebug() << "StringToSensorType() - Value not supported!";

    return returnValue;
}

inline QString actuatorTypeToString(const ActuatorType &value)
{
    QString returnString;
    switch (value)
    {
    case Actuator_Siren:
        returnString = "Actuator_Siren";
        break;
    case Actuator_Buzzer:
        returnString = "Actuator_Buzzer";
        break;
    case Actuator_Relay:
        returnString = "Actuator_Relay";
        break;
    case Actuator_DoorBell:
        returnString = "Actuator_DoorBell";
        break;
    case Actuator_IR:
        returnString = "Actuator_IR";
        break;
    case Actuator_Pump:
        returnString = "Actuator_Pump";
        break;
    default:
        qDebug() << "actuatorTypeToString() - Value not supported!";
        break;
    }
    return returnString;
}

inline ActuatorType StringToActuatorType(const QString &str)
{
    bool found = false;
    ActuatorType returnValue;

    if(str == "Actuator_Siren")
    {
        returnValue = Actuator_Siren;
        found = true;
    }
    if(str == "Actuator_Buzzer")
    {
        returnValue = Actuator_Buzzer;
        found = true;
    }
    if(str == "Actuator_Relay")
    {
        returnValue = Actuator_Relay;
        found = true;
    }
    if(str == "Actuator_DoorBell")
    {
        returnValue = Actuator_DoorBell;
        found = true;
    }
    if(str == "Actuator_IR")
    {
        returnValue = Actuator_IR;
        found = true;
    }
    if(str == "Actuator_Pump")
    {
        returnValue = Actuator_Pump;
        found = true;
    }

    if(!found)
        qDebug() << "StringToActuatorType() - Value not supported!";

    return returnValue;
}

#ifdef WIRINGPI
inline int StringToGPIO(const QString &gpio)
{
    int returnValue = -1;
    QString toRemove = "GPIO_";
    QString copy = gpio;
    if(gpio.contains(toRemove))
        returnValue = copy.remove(toRemove).toInt();

    return returnValue;
}

inline int StringToEdge(const QString &edge)
{
    int returnValue = -1;
    if(edge.contains("FALLING"))
        returnValue = 1; //INT_EDGE_FALLING;
    if(edge.contains("RISING"))
        returnValue = 2; //INT_EDGE_RISING;
    if(edge.contains("BOT"))
        returnValue = 3; //INT_EDGE_BOTH;

    return returnValue;
}
#endif

}

#endif // COMMON_H

