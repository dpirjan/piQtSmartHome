#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QStringListModel>

#include "ioModel.h"
#include "watchdogHelper.h"

class UIManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString selectedZone WRITE setSelectedZone READ getSelectedZone)
    Q_PROPERTY(QString selectedCategory WRITE setSelectedCategory READ getSelectedCategory)

public:
    explicit UIManager(QObject *parent = 0);
    virtual ~UIManager();

    void init();

    QStringListModel* getZonesModel() const;
    QStringListModel* getCategoriesModel() const;
    ioModel* getIOModel() const;

private slots:
    void zonesReceived(const QStringList&);
    void categoriesReceived(const QStringList&);

    void setSelectedZone(const QString&);
    void setSelectedCategory(const QString&);


    QString getSelectedZone() const;
    QString getSelectedCategory() const;


private:
    WatchdogHelper *m_watchdog;
    QStringListModel *m_zonesModel;
    QStringListModel *m_categoriesModel;
    ioModel *m_IOModel;

    QString m_selectedZone;
    QString m_selectedCategory;
};

#endif // UIMANAGER_H
