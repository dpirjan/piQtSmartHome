#ifndef ACTUATOREVENTMODEL_H
#define ACTUATOREVENTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "actuatorInfo.h"

class actuatorEventModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ActuatorInfoEventRoles
    {
        ZoneRole = Qt::UserRole + 1, //257
        NodeRole,
        CategoryRole,
        AddressRole,
        TimestampRole,
        ValueRole
    };

    explicit actuatorEventModel(QObject *parent = 0);
    virtual ~actuatorEventModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &, int) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const;

    void setActuatorInfoEventList(const QList<ActuatorInfo>&);

private:
    Q_DISABLE_COPY(actuatorEventModel)

    QList<ActuatorInfo> m_actuatorInfoEventList;
};

#endif // ACTUATOREVENTMODEL_H
