#include <QTimer>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QCoreApplication>

#include "watchdogFunctions.h"

#define WATCHDOG_TIMEOUT_US 2000000 // 2 sec in microseconds

WatchdogFunctions::WatchdogFunctions(QObject *parent) : QObject(parent)
{
    qDebug() << "WatchdogFunctions::constructor: " << QThread::currentThreadId();
    m_watchdogStop = false;

    QString env = qgetenv("WATCHDOG_USEC");
    m_watchdogInterval = env.toULongLong();
    qDebug() << "WATCHDOG_USEC: " << env << " value: " << m_watchdogInterval;

    if(m_watchdogInterval == 0)
    {
        m_watchdogInterval = WATCHDOG_TIMEOUT_US;
        watchdogTimerSetInterval(m_watchdogInterval);
    }

    m_watchdogTimer = new QTimer(this);
    m_watchdogTimer->setTimerType(Qt::PreciseTimer);

    connect(m_watchdogTimer, SIGNAL(timeout()), this, SLOT(watchdogTimerNotify()));
    qDebug() << "WatchdogFunctions::constructor: "
             << QThread::currentThreadId() << " : timer() "
             << m_watchdogTimer->interval() << "ms";
}

WatchdogFunctions::~WatchdogFunctions()
{
    qDebug() << "WatchdogFunctions::destructor: " << QThread::currentThreadId();
    m_watchdogTimer->deleteLater();
}

void WatchdogFunctions::watchdogTimerSetInterval(const quint64 &intervalInUSEC)
{
    qDebug() << "WatchdogFunctions::watchdogTimerSetInterval: "
             << intervalInUSEC << " - " << QThread::currentThreadId();

    QString toSend = "WATCHDOG_USEC=";
    toSend.append(QString::number(intervalInUSEC));

    m_watchdogInterval = intervalInUSEC;

    sd_notify(0, toSend.toLocal8Bit().data());
}

quint64 WatchdogFunctions::watchdogTimerGetInterval() const
{
    qDebug() << "WatchdogFunctions::watchdogTimerGetInterval: "
             << m_watchdogInterval << " - " << QThread::currentThreadId();
    return m_watchdogInterval;
}

void WatchdogFunctions::watchdogTimerInitialize()
{
    qDebug() << "WatchdogFunctions::watchdogTimerInitialized: "
             << QThread::currentThreadId() << " : timer() "
             << m_watchdogTimer->interval() << "ms";

    sd_notify(0, "READY=1");
    m_watchdogTimer->start(m_watchdogInterval / 1000); // us to ms
}

void WatchdogFunctions::watchdogTimerFailure(const int &errorCode, const QString &errorString)
{
    qDebug() << "WatchdogFunctions::watchdogTimerFailure: " << QThread::currentThreadId();

    qDebug() << "Failure: " << errorCode << " - " << errorString;
    sd_notifyf(0, "STATUS=Failed to start up: %s\n"
                  "ERRNO=%i",
               errorString.toLocal8Bit().data(),
               errorCode);

    emit error(errorCode, errorString);

    QMutexLocker locker(&m_watchdogMutex);
    m_watchdogStop = true;
}

void WatchdogFunctions::watchdogTimerStop()
{
    qDebug() << "WatchdogFunctions::watchdogTimerStop: " << QThread::currentThreadId();

    sd_notify(0, "STOPPING=1");

    QMutexLocker locker(&m_watchdogMutex);
    m_watchdogStop = true;
}

void WatchdogFunctions::watchdogTimerNotify()
{
    qDebug() << "WatchdogFunctions::watchdogTimerNotify: "
             << QThread::currentThreadId() << " : timer() "
             << m_watchdogTimer->interval() << "ms";
    Q_ASSERT(QThread::currentThread() == m_watchdogThread);

    sd_notify(0, "WATCHDOG=1");

    if(!m_watchdogStop)
        m_watchdogTimer->start(m_watchdogInterval / 1000); // us to ms
    else
    {
        m_watchdogTimer->stop();
        emit watchdogTimerNotifyFinished();
    }
}

void WatchdogFunctions::setWatchdogThread(QThread *thr)
{
    m_watchdogThread = thr;
}
