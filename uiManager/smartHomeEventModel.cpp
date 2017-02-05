#include <QDebug>

#include "smartHomeEventModel.h"

smartHomeEventModel::smartHomeEventModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

smartHomeEventModel::~smartHomeEventModel()
{
}

int smartHomeEventModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_smartHomeEventList.count();
}

QVariant smartHomeEventModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if(!index.isValid())
        return ret;

    switch (role)
    {
    case ZoneRole:
        ret = QVariant::fromValue(m_smartHomeEventList.at(index.row()).getZone());
        break;
    case NodeRole:
        ret = QVariant::fromValue(m_smartHomeEventList.at(index.row()).getNode());
        break;
    case CategoryRole:
        ret = QVariant::fromValue(m_smartHomeEventList.at(index.row()).getCategory());
        break;
    case AddressRole:
        ret = QVariant::fromValue(m_smartHomeEventList.at(index.row()).getAddress());
        break;
    case TimestampRole:
        ret = QVariant::fromValue(m_smartHomeEventList.at(index.row()).getTimestamp());
        break;
    case ValueRole:
        ret = QVariant::fromValue(m_smartHomeEventList.at(index.row()).getValue());
        break;
    default:
        qCritical() << "Wrong " << role << " role selected! Should be between "
                    << ZoneRole << " and " << ValueRole << ".";
        break;
    }

    return ret;
}

QHash<int, QByteArray> smartHomeEventModel::roleNames() const
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

void smartHomeEventModel::setSmartHomeEventList(const QList<SmartHomeInfo> &list)
{
    emit beginResetModel();
    m_smartHomeEventList = list;
    emit endResetModel();
}
