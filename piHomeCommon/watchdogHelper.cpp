#include <QThread>
#include <QDebug>

#include "watchdogFunctions.h"
#include "watchdogHelper.h"

WatchdogHelper::WatchdogHelper(const QString &appName, QObject *parent) :
    QObject(parent)

{
    qDebug() << "WatchdogHelper::constructor " << QThread::currentThreadId();
    m_watchdogAppName = appName;
}

WatchdogHelper::~WatchdogHelper()
{
    qDebug() << "WatchdogHelper::destructor " << QThread::currentThreadId();
}

void WatchdogHelper::init()
{
    qDebug() << "WatchdogHelper::init " << QThread::currentThreadId();
    m_watchdogThread = new QThread();
    m_watchdogThread->setObjectName(m_watchdogAppName);

    m_watchdogFunctions = new WatchdogFunctions();

    m_watchdogFunctions->watchdogTimerInitialize();
    m_watchdogFunctions->setWatchdogThread(m_watchdogThread);

    m_watchdogFunctions->moveToThread(m_watchdogThread);

    connect(m_watchdogThread, SIGNAL(started()),
            m_watchdogFunctions, SLOT(watchdogTimerNotify()));
    connect(m_watchdogFunctions, SIGNAL(watchdogTimerNotifyFinished()),
            this, SLOT(finishedSlot()));
    connect(m_watchdogFunctions, SIGNAL(error(int, QString)),
            this, SLOT(errorSlot(int,QString)));

    connect(m_watchdogThread, &QThread::finished,
            m_watchdogFunctions, &QObject::deleteLater);
    connect(m_watchdogThread, &QThread::finished,
            m_watchdogThread, &QObject::deleteLater);

    m_watchdogThread->start();
}

void WatchdogHelper::stop()
{
    qDebug() << "WatchdogHelper::stop " << QThread::currentThreadId();
    if(m_watchdogThread->isRunning())
        m_watchdogFunctions->watchdogTimerStop();
}

void WatchdogHelper::setInterval(const quint64 &interval)
{
    qDebug() << "WatchdogHelper::setInterval " << QThread::currentThreadId();
    m_watchdogFunctions->watchdogTimerSetInterval(interval);
}

quint64 WatchdogHelper::getInterval() const
{
    qDebug() << "WatchdogHelper::getInterval " << QThread::currentThreadId();
    return m_watchdogFunctions->watchdogTimerGetInterval();
}

void WatchdogHelper::error(const int &error, const QString &errorString)
{
    qDebug() << "WatchdogHelper::error: " << error << " - " << errorString
             << " " << QThread::currentThreadId();
    m_watchdogFunctions->watchdogTimerFailure(error, errorString);
}

void WatchdogHelper::errorSlot(const int &error, const QString &errorString)
{
    qDebug() << "WatchdogHelper::errorSlot: " << error << " - " << errorString
             << " " << QThread::currentThreadId();
    emit errorSignal(error, errorString);
}

void WatchdogHelper::finishedSlot()
{
    qDebug() << "WatchdogHelper::finishedSlot: " << QThread::currentThreadId();
    emit finishedSignal();
}
