#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDebug>

namespace piHome {

typedef enum {
    HomeAlarm = 0x1,
    SmartHome = 0x2,
} SystemType;

typedef enum {
    Wired = 0x10,
    Wireless = 0x20,
} HardwareType;

typedef enum {
    PIR = 0x100,
    Contact = 0x200,
    Light = 0x400, // Light intensity
    Temperature = 0x800, // DHT22
    Humidity = 0x1000, // DHT22
    CO = 0x2000, // Carbon Monoxid
    CH4 = 0x4000, // Methane
    Vibration = 0x8000, // Knock sensor
    Flood = 0x10000, // Water presence
    WaterLevel = 0x20000, // Water level
    DoorLockedState = 0x40000, // Door locked state provided by IR sensor
    BatteryLevel = 0x80000, // Read battery voltage on Analog
    Siren = 0x100000,
    Buzzer = 0x200000,
    Relay = 0x400000,
    DoorBell = 0x800000,
    IR = 0x1000000,
    Pump = 0x2000000, // Water pump
    DoorLock = 0x4000000 // bolt or whatever else used for locking the door
} IOType;

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

inline QString typeToString(const IOType &value)
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
    case Humidity:
        returnString = "Humidity";
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
    case DoorLockedState:
        returnString = "DoorLockedState";
        break;
    case BatteryLevel:
        returnString = "BatteryLevel";
        break;
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
    case DoorLock:
        returnString = "DoorLock";
        break;
    default:
        qDebug() << "typeToString() - Value not supported!";
        break;
    }
    return returnString;
}

inline QString typeToString(const uint32_t &value)
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
    case Humidity:
        returnString = "Humidity";
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
    case DoorLockedState:
        returnString = "DoorLockedState";
        break;
    case BatteryLevel:
        returnString = "BatteryLevel";
        break;
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
    case DoorLock:
        returnString = "DoorLock";
        break;
    default:
        qDebug() << "typeToString() - Value not supported!";
        break;
    }
    return returnString;
}

inline IOType StringToType(const QString &str)
{
    bool found = false;
    IOType returnValue;
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
    if(str == "Humidity")
    {
        returnValue = Humidity;
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
    if(str == "DoorLockedState")
    {
        returnValue = DoorLockedState;
        found = true;
    }
    if(str == "BatteryLevel")
    {
        returnValue = BatteryLevel;
        found = true;
    }
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
    if(str == "DoorLock")
    {
        returnValue = DoorLock;
        found = true;
    }

    if(!found)
        qDebug() << "StringToType() - Value not supported!";

    return returnValue;
}

}

#endif // COMMON_H

