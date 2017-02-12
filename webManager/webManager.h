#ifndef WEBMANAGER_H
#define WEBMANAGER_H

#include <QObject>

#include "io.h"

class WebManager : public QObject
{
    Q_OBJECT

public:
    explicit WebManager(QObject *parent = 0);
    virtual ~WebManager();

    static bool firstRunConfiguration();

    void loadWebServerSettings();
};

#endif // WEBMANAGER_H
