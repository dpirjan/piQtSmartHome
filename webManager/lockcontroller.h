#ifndef LOCKCONTROLLER_H
#define LOCKCONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class LockController : public HttpRequestHandler {
    Q_OBJECT
public:
    LockController(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);

private:
    bool displayedState;
    bool lockAvailable;
    QString lockAddress;
};

#endif // LOCKCONTROLLER_H
