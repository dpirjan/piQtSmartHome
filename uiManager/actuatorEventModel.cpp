#include <QDebug>

#include "actuatorEventModel.h"

actuatorEventModel::actuatorEventModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

actuatorEventModel::~actuatorEventModel()
{
}

int actuatorEventModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_actuatorInfoEventList.count();
}

QVariant actuatorEventModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if(!index.isValid())
        return ret;

    switch (role)
    {
    case ZoneRole:
        ret = QVariant::fromValue(m_actuatorInfoEventList.at(index.row()).getZone());
        break;
    case NodeRole:
        ret = QVariant::fromValue(m_actuatorInfoEventList.at(index.row()).getNode());
        break;
    case CategoryRole:
        ret = QVariant::fromValue(m_actuatorInfoEventList.at(index.row()).getCategory());
        break;
    case AddressRole:
        ret = QVariant::fromValue(m_actuatorInfoEventList.at(index.row()).getAddress());
        break;
    case TimestampRole:
        ret = QVariant::fromValue(m_actuatorInfoEventList.at(index.row()).getTimestamp());
        break;
    case ValueRole:
        ret = QVariant::fromValue(m_actuatorInfoEventList.at(index.row()).getValue());
        break;
    default:
        qCritical() << "Wrong " << role << " role selected! Should be between "
                    << ZoneRole << " and " << ValueRole << ".";
        break;
    }

    return ret;
}

QHash<int, QByteArray> actuatorEventModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ZoneRole] = "zone";
    roles[NodeRole] = "node";
    roles[CategoryRole] = "category";
    roles[AddressRole] = "address";
    roles[TimestampRole] = "timestamp";
    roles[ValueRole] = "value";
    return roles;
}

void actuatorEventModel::setActuatorInfoEventList(const QList<ActuatorInfo> &list)
{
    emit beginResetModel();
    m_actuatorInfoEventList = list;
    emit endResetModel();
}
