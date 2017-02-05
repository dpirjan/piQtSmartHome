#include "ioModel.h"
#include "databaseManagerInterface.h"
#include "sensorManagerInterface.h"

ioModel::ioModel(QObject *parent) : QAbstractListModel(parent)
{
    m_alarmEventModel = new alarmEventModel(this);
    m_smartHomeEventModel = new smartHomeEventModel(this);
    m_actuatorEventModel = new actuatorEventModel(this);
}

ioModel::~ioModel()
{
    if(m_alarmEventModel)
        m_alarmEventModel->deleteLater();
    if(m_smartHomeEventModel)
        m_smartHomeEventModel->deleteLater();
    if(m_actuatorEventModel)
        m_actuatorEventModel->deleteLater();
}

int ioModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_ioList.count();
}

QVariant ioModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if(!index.isValid())
        return ret;

    switch (role)
    {
    case SystemRole:
        ret = QVariant::fromValue(m_ioList.at(index.row()).getSystemType());
        break;
    case HardwareRole:
        ret = QVariant::fromValue(m_ioList.at(index.row()).getHardwareType());
        break;
    case TypeRole:
        ret = QVariant::fromValue(m_ioList.at(index.row()).getType());
        break;
    case CategoryRole:
        ret = QVariant::fromValue(m_ioList.at(index.row()).getCategory());
        break;
    case ZoneRole:
        ret = QVariant::fromValue(m_ioList.at(index.row()).getZone());
        break;
    case NodeRole:
        ret = QVariant::fromValue(m_ioList.at(index.row()).getNode());
        break;
    case AddressRole:
        ret = QVariant::fromValue(m_ioList.at(index.row()).getAddress());
        break;
    default:
        qCritical() << "Wrong " << role << " role selected! Should be between "
                    << SystemRole << " and " << AddressRole << ".";
        break;
    }

    return ret;
}

void ioModel::setIOList(const QList<io> &list)
{
    emit beginResetModel();
    m_ioList = list;
    emit endResetModel();
}

QHash<int, QByteArray> ioModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SystemRole] = "system";
    roles[HardwareRole] = "hardware";
    roles[TypeRole] = "type";
    roles[CategoryRole] = "category";
    roles[ZoneRole] = "zone";
    roles[NodeRole] = "node";
    roles[AddressRole] = "address";
    return roles;
}

void ioModel::setSelectedIOIndex(const unsigned int &io)
{
    m_selectedIOIndex = io;

    qDebug() << "Selected IO index [" << io << "]"
             << m_ioList.at(io).getSystemType()
             << m_ioList.at(io).getHardwareType()
             << m_ioList.at(io).getType()
             << m_ioList.at(io).getCategory()
             << m_ioList.at(io).getZone()
             << m_ioList.at(io).getNode()
             << m_ioList.at(io).getAddress();

    if(m_ioList.at(io).getType() == "Sensor")
    {
        if(m_ioList.at(io).getSystemType() == "HomeAlarm")
        {
            qDebug() << "Action for HomeAlarm sensor";
            m_alarmEventModel->setAlarmEventList(
                        databaseManagerInterface::instance().getHomeAlarmEntriesForIO(m_ioList.at(io)));
        }
        if(m_ioList.at(io).getSystemType() == "SmartHome")
        {
            qDebug() << "Action for SmartHome sensor";
            m_smartHomeEventModel->setSmartHomeEventList(
                        databaseManagerInterface::instance().getSmartHomeEntriesForIO(m_ioList.at(io)));
        }
    }

    if(m_ioList.at(io).getType() == "Actuator")
    {
            qDebug() << "Action for actuator";
            m_actuatorEventModel->setActuatorInfoEventList(
                        databaseManagerInterface::instance().getActuatorInfoEntriesForIO(m_ioList.at(io)));
    }
}

void ioModel::setValueIO(const QString &value)
{
    m_value = value;
    qDebug() << "SetValue: " << m_ioList.at(m_selectedIOIndex).getCategory()
             << "  " << value;
    if(m_ioList.at(m_selectedIOIndex).getType() == "Actuator")
        sensorManagerInterface::instance().setActuatorValue(
                    m_ioList.at(m_selectedIOIndex).getCategory(),
                    m_ioList.at(m_selectedIOIndex).getAddress(),
                    value);
}

unsigned int ioModel::getSelectedIOIndex() const
{
    qDebug() << "GetValue: " << m_selectedIOIndex;
    return m_selectedIOIndex;
}

QString ioModel::getValueIO() const
{
    return m_value;
}

alarmEventModel* ioModel::getAlarmEventModel() const
{
    return m_alarmEventModel;
}

smartHomeEventModel* ioModel::getSmartHomeEventModel() const
{
    return m_smartHomeEventModel;
}

actuatorEventModel* ioModel::getActuatorEventModel() const
{
    return m_actuatorEventModel;
}
