#ifndef IOMODEL_H
#define IOMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "io.h"
#include "smartHomeEventModel.h"
#include "alarmEventModel.h"
#include "actuatorEventModel.h"

class ioModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(unsigned int selectedIOIndex WRITE setSelectedIOIndex READ getSelectedIOIndex)
    Q_PROPERTY(QString valueIO WRITE setValueIO READ getValueIO)

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
    virtual ~ioModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &, int) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const;

    void setIOList(const QList<io>&);

    alarmEventModel* getAlarmEventModel() const;
    smartHomeEventModel* getSmartHomeEventModel() const;
    actuatorEventModel* getActuatorEventModel() const;

private slots:
    void setSelectedIOIndex(const unsigned int&);
    void setValueIO(const QString&);
    unsigned int getSelectedIOIndex() const;
    QString getValueIO() const;

private:
    Q_DISABLE_COPY(ioModel)

    QList<io> m_ioList;

    unsigned int m_selectedIOIndex;
    QString m_value;

    alarmEventModel *m_alarmEventModel;
    smartHomeEventModel *m_smartHomeEventModel;
    actuatorEventModel *m_actuatorEventModel;
};

#endif // IOMODEL_H
