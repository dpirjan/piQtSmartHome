#ifndef WATCHDOGHELPER_H
#define WATCHDOGHELPER_H

#include <QObject>

class QThread;
class WatchdogFunctions;

class WatchdogHelper : public QObject
{
    Q_OBJECT

public:
    explicit WatchdogHelper(const QString &, QObject * parent = NULL);
    virtual ~WatchdogHelper();

public slots:
    void init();
    void stop();
    void setInterval(const quint64 &);
    quint64 getInterval() const;
    void error(const int &, const QString &);

signals:
    void finishedSignal();
    void errorSignal(const int &, const QString &);

private slots:
    void finishedSlot();
    void errorSlot(const int &, const QString &);

private:
    QThread *m_watchdogThread;
    WatchdogFunctions *m_watchdogFunctions;
    QString m_watchdogAppName;
};

#endif // WATCHDOGHELPER_H
