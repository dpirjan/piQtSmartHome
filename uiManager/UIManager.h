#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QStringListModel>

#include "watchdogHelper.h"

class UIManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString selectedZone WRITE setSelectedZone READ getSelectedZone)
    Q_PROPERTY(QString selectedCategory WRITE setSelectedCategory READ getSelectedCategory)
    Q_PROPERTY(QString selectedIO WRITE setSelectedIO READ getSelectedIO)

public:
    explicit UIManager(QObject *parent = 0);
    virtual ~UIManager();

    void init();

    QStringListModel* getZonesModel() const;
    QStringListModel* getCategoriesModel() const;
    QStringListModel* getIOModel() const;

private slots:
    void zonesReceived(const QStringList&);
    void categoriesReceived(const QStringList&);

    void setSelectedZone(const QString&);
    void setSelectedCategory(const QString&);
    void setSelectedIO(const QString&);

    QString getSelectedZone() const;
    QString getSelectedCategory() const;
    QString getSelectedIO() const;

private:
    WatchdogHelper *m_watchdog;
    QStringListModel *m_zonesModel;
    QStringListModel *m_categoriesModel;
    QStringListModel *m_IOModel;

    QString m_selectedZone;
    QString m_selectedCategory;
    QString m_selectedIO;
};

#endif // UIMANAGER_H
