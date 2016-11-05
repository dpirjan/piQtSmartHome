#ifndef IOMODEL_H
#define IOMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "io.h"

class ioModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(unsigned int selectedIOIndex WRITE setSelectedIOIndex READ getSelectedIOIndex)

public:
    enum IORoles
    {
        SystemRole = Qt::UserRole + 1, // 257
        HardwareRole,
        TypeRole,
        CategoryRole,
        ZoneRole,
        NodeRole,
        AddressRole
    };

    explicit ioModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &, int) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const;

    void setIOList(const QList<io>&);

private slots:
    void setSelectedIOIndex(const unsigned int&);
    unsigned int getSelectedIOIndex() const;

private:
    Q_DISABLE_COPY(ioModel)

    QList<io> m_ioList;

    unsigned int m_selectedIOIndex;
};

#endif // IOMODEL_H
