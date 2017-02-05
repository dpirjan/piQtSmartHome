#include <QDebug>

#include "alarmEventModel.h"

alarmEventModel::alarmEventModel(QObject *parent) : QAbstractListModel(parent)
{
}

alarmEventModel::~alarmEventModel()
{
}

int alarmEventModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_alarmEventList.count();
}

QVariant alarmEventModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if(!index.isValid())
        return ret;

    switch (role)
    {
    case ZoneRole:
        ret = QVariant::fromValue(m_alarmEventList.at(index.row()).getZone());
        break;
    case NodeRole:
        ret = QVariant::fromValue(m_alarmEventList.at(index.row()).getNode());
        break;
    case CategoryRole:
        ret = QVariant::fromValue(m_alarmEventList.at(index.row()).getCategory());
        break;
    case AddressRole:
        ret = QVariant::fromValue(m_alarmEventList.at(index.row()).getAddress());
        break;
    case TimestampRole:
        ret = QVariant::fromValue(m_alarmEventList.at(index.row()).getTimestamp());
        break;
    default:
        qCritical() << "Wrong " << role << " role selected! Should be between "
                    << ZoneRole << " and " << TimestampRole << ".";
        break;
    }

    return ret;
}

QHash<int, QByteArray> alarmEventModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ZoneRole] = "zone";
    roles[NodeRole] = "node";
    roles[CategoryRole] = "category";
    roles[AddressRole] = "address";
    roles[TimestampRole] = "timestamp";
    return roles;
}

void alarmEventModel::setAlarmEventList(const QList<HomeAlarmInfo> &list)
{
    emit beginResetModel();
    m_alarmEventList = list;
    emit endResetModel();
}
