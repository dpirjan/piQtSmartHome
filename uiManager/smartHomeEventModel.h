#ifndef SMARTHOMEEVENTMODEL_H
#define SMARTHOMEEVENTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "smartHomeInfo.h"

class smartHomeEventModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum SmartHomeEventRoles
    {
        ZoneRole = Qt::UserRole + 1, //257
        NodeRole,
        CategoryRole,
        AddressRole,
        TimestampRole,
        ValueRole
    };

    explicit smartHomeEventModel(QObject *parent = 0);
    virtual ~smartHomeEventModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &, int) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const;

    void setSmartHomeEventList(const QList<SmartHomeInfo>&);

private:
    Q_DISABLE_COPY(smartHomeEventModel)

    QList<SmartHomeInfo> m_smartHomeEventList;
};

#endif // SMARTHOMEEVENTMODEL_H
