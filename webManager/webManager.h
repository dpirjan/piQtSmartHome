#ifndef WEBMANAGER_H
#define WEBMANAGER_H

#include <QObject>

#include "io.h"
#include "watchdogHelper.h"

class WebManager : public QObject
{
    Q_OBJECT

public:
    explicit WebManager(QObject *parent = 0);
    virtual ~WebManager();

    static bool firstRunConfiguration();
    void loadWebServerSettings();

private:
    WatchdogHelper *m_watchdog;
};

#endif // WEBMANAGER_H
