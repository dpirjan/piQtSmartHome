#include <QDebug>

#include "UIManager.h"
#include "databaseManagerInterface.h"

UIManager::UIManager(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor DatabaseManager";
}

UIManager::~UIManager()
{
    qDebug() << ">>>Destructor UIManager";
    if(m_zonesModel)
        m_zonesModel->deleteLater();
    if(m_categoriesModel)
        m_categoriesModel->deleteLater();

    m_watchdog->stop();
    m_watchdog->deleteLater();
    qDebug() << "<<<Destructor UIManager";
}

void UIManager::init()
{
    m_watchdog = new WatchdogHelper("piHomeUI");
    m_watchdog->init();

    m_zonesModel = new QStringListModel(this);
    m_categoriesModel = new QStringListModel(this);

    QObject::connect(&databaseManagerInterface::instance(),
                     SIGNAL(zonesReceived(QStringList)),
                     this,
                     SLOT(zonesReceived(QStringList)));
    QObject::connect(&databaseManagerInterface::instance(),
                     SIGNAL(categoriesReceived(QStringList)),
                     this,
                     SLOT(categoriesReceived(QStringList)));

    databaseManagerInterface::instance().getAllZones();
    databaseManagerInterface::instance().getAllCategories();
}

void UIManager::zonesReceived(const QStringList &list)
{
    qDebug() << "Zones: " << list;
    m_zonesModel->setStringList(list);
}

void UIManager::categoriesReceived(const QStringList &list)
{
    qDebug() << "Categories: " << list;
    m_categoriesModel->setStringList(list);
}

QStringListModel* UIManager::getZonesModel() const
{
    return m_zonesModel;
}

QStringListModel* UIManager::getCategoriesModel() const
{
    return m_categoriesModel;
}
