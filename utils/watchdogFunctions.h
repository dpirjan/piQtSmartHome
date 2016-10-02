#ifndef WATCHDOGFUNCTIONS_H
#define WATCHDOGFUNCTIONS_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QThread>

#include <systemd/sd-daemon.h>

class WatchdogFunctions : public QObject
{
    Q_OBJECT

public:
    explicit WatchdogFunctions(QObject *parent = 0);
    virtual ~WatchdogFunctions();
    Q_DISABLE_COPY(WatchdogFunctions)

    friend class WatchdogHelper;

protected slots:
    void watchdogTimerSetInterval(const quint64&);
    quint64 watchdogTimerGetInterval() const;
    void watchdogTimerInitialize();
    void watchdogTimerNotify();
    void watchdogTimerFailure(const int&, const QString&);
    void watchdogTimerStop();
    void setWatchdogThread(QThread*);

signals:
    void watchdogTimerNotifyFinished();
    void error(const int&, const QString&);

private:
    quint64 m_watchdogInterval;
    QTimer *m_watchdogTimer;
    QMutex m_watchdogMutex;
    bool m_watchdogStop;
    QThread *m_watchdogThread;
};

#endif // WATCHDOGFUNCTIONS_H
