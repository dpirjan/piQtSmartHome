#ifndef ALARMEVENTMODEL_H
#define ALARMEVENTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "homeAlarmInfo.h"

class alarmEventModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum AlarmEventRoles
    {
        ZoneRole = Qt::UserRole + 1, //257
        NodeRole,
        CategoryRole,
        AddressRole,
        TimestampRole
    };

    explicit alarmEventModel(QObject *parent = 0);
    virtual ~alarmEventModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &, int) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const;

    void setAlarmEventList(const QList<HomeAlarmInfo>&);

private:
    Q_DISABLE_COPY(alarmEventModel)

    QList<HomeAlarmInfo> m_alarmEventList;
};

#endif // ALARMEVENTMODEL_H
