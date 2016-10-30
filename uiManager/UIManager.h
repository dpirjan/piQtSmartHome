#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QStringListModel>

#include "watchdogHelper.h"

class UIManager : public QObject
{
    Q_OBJECT
public:
    explicit UIManager(QObject *parent = 0);
    virtual ~UIManager();

    void init();

    QStringListModel* getZonesModel() const;
    QStringListModel* getCategoriesModel() const;

private slots:
    void zonesReceived(const QStringList&);
    void categoriesReceived(const QStringList&);

private:
    WatchdogHelper *m_watchdog;
    QStringListModel *m_zonesModel;
    QStringListModel *m_categoriesModel;
};

#endif // UIMANAGER_H
