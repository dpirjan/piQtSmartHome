#ifndef ALARMCONTROLLER_H
#define ALARMCONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class AlarmController : public HttpRequestHandler {
    Q_OBJECT
public:
    AlarmController(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);

private:
    bool displayedGeneralState;
    bool displayedNightState;
    bool displayedVacationState;
};

#endif // ALARMCONTROLLER_H
