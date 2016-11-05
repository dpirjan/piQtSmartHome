#include "ioModel.h"
#include "databaseManagerInterface.h"

ioModel::ioModel(QObject *parent) : QAbstractListModel(parent)
{
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
            HomeAlarmInfo tmp(m_ioList.at(io).getZone(),
                              m_ioList.at(io).getNode(),
                              m_ioList.at(io).getCategory(),
                              m_ioList.at(io).getAddress());
            databaseManagerInterface::instance().insertHomeAlarmEntry(tmp);
            databaseManagerInterface::instance().getHomeAlarmEntriesForIO(m_ioList.at(io));
        }
        if(m_ioList.at(io).getSystemType() == "SmartHome")
        {
            qDebug() << "Action for SmartHome sensor";
            SmartHomeInfo tmp(m_ioList.at(io).getZone(),
                              m_ioList.at(io).getNode(),
                              m_ioList.at(io).getCategory(),
                              m_ioList.at(io).getAddress(),
                              QString::number(27));
            databaseManagerInterface::instance().insertSmartHomeEntry(tmp);
            databaseManagerInterface::instance().getSmartHomeEntriesForIO(m_ioList.at(io));
        }
    }
    if(m_ioList.at(io).getType() == "Actuator")
    {
        if(m_ioList.at(io).getSystemType() == "HomeAlarm")
        {
            qDebug() << "Action for HomeAlarm actuator";
            HomeAlarmInfo tmp(m_ioList.at(io).getZone(),
                              m_ioList.at(io).getNode(),
                              m_ioList.at(io).getCategory(),
                              m_ioList.at(io).getAddress());
            databaseManagerInterface::instance().insertHomeAlarmEntry(tmp);
            databaseManagerInterface::instance().getHomeAlarmEntriesForIO(m_ioList.at(io));
        }
        if(m_ioList.at(io).getSystemType() == "SmartHome")
        {
            qDebug() << "Action for SmartHome actuator";
            SmartHomeInfo tmp(m_ioList.at(io).getZone(),
                              m_ioList.at(io).getNode(),
                              m_ioList.at(io).getCategory(),
                              m_ioList.at(io).getAddress(),
                              "On");
            databaseManagerInterface::instance().insertSmartHomeEntry(tmp);
            databaseManagerInterface::instance().getSmartHomeEntriesForIO(m_ioList.at(io));
        }
    }
}

unsigned int ioModel::getSelectedIOIndex() const
{
    return m_selectedIOIndex;
}
