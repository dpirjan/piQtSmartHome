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
    PIR = 7,
    Contact,
    Light, // Light intensity
    Temperature, // + humidity => DHT22
    CO, // Carbon Monoxid
    CH4, // Methane
    Vibration, // Knock sensor
    Flood, // Water presence
    WaterLevel, // Water level
    DoorLocked, // Door locked
    BatteryLevel // Read battery voltage on Analog
} SensorType;

typedef enum {
    Siren = 19,
    Buzzer,
    Relay,
    DoorBell,
    IR,
    Pump // Water pump
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
    case PIR:
        returnString = "PIR";
        break;
    case Contact:
        returnString = "Contact";
        break;
    case Light:
        returnString = "Light";
        break;
    case Temperature:
        returnString = "Temperature";
        break;
    case CO:
        returnString = "CO";
        break;
    case CH4:
        returnString = "CH4";
        break;
    case Vibration:
        returnString = "Vibration";
        break;
    case Flood:
        returnString = "Flood";
        break;
    case WaterLevel:
        returnString = "WaterLevel";
        break;
    case DoorLocked:
        returnString = "DoorLocked";
        break;
    case BatteryLevel:
        returnString = "BatteryLevel";
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
    if(str == "PIR")
    {
        returnValue = PIR;
        found = true;
    }
    if(str == "Contact")
    {
        returnValue = Contact;
        found = true;
    }
    if(str == "Light")
    {
        returnValue = Light;
        found = true;
    }
    if(str == "Temperature")
    {
        returnValue = Temperature;
        found = true;
    }
    if(str == "CO")
    {
        returnValue = CO;
        found = true;
    }
    if(str == "CH4")
    {
        returnValue = CH4;
        found = true;
    }
    if(str == "Vibration")
    {
        returnValue = Vibration;
        found = true;
    }
    if(str == "Flood")
    {
        returnValue = Flood;
        found = true;
    }
    if(str == "WaterLevel")
    {
        returnValue = WaterLevel;
        found = true;
    }
    if(str == "DoorLocked")
    {
        returnValue = DoorLocked;
        found = true;
    }
    if(str == "BatteryLevel")
    {
        returnValue = BatteryLevel;
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
    case Siren:
        returnString = "Siren";
        break;
    case Buzzer:
        returnString = "Buzzer";
        break;
    case Relay:
        returnString = "Relay";
        break;
    case DoorBell:
        returnString = "DoorBell";
        break;
    case IR:
        returnString = "IR";
        break;
    case Pump:
        returnString = "Pump";
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

    if(str == "Siren")
    {
        returnValue = Siren;
        found = true;
    }
    if(str == "Buzzer")
    {
        returnValue = Buzzer;
        found = true;
    }
    if(str == "Relay")
    {
        returnValue = Relay;
        found = true;
    }
    if(str == "DoorBell")
    {
        returnValue = DoorBell;
        found = true;
    }
    if(str == "IR")
    {
        returnValue = IR;
        found = true;
    }
    if(str == "Pump")
    {
        returnValue = Pump;
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

