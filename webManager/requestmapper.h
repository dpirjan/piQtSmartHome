#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"
#include "httpsessionstore.h"
#include "listdatacontroller.h"
#include "logincontroller.h"
#include "alarmcontroller.h"
#include "lockcontroller.h"

using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler {
    Q_OBJECT

public:
    RequestMapper(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
    static HttpSessionStore* sessionStore;

private:
    ListDataController listDataController;
    LoginController loginController;
    AlarmController alarmController;
    LockController lockController;
};

#endif // REQUESTMAPPER_H
