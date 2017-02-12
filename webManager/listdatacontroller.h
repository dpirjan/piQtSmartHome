#ifndef LISTDATACONTROLLER_H
#define LISTDATACONTROLLER_H

#include <QList>

#include "httprequesthandler.h"
#include "homeAlarmInfo.h"

using namespace stefanfrings;

class ListDataController: public HttpRequestHandler
{
    Q_OBJECT

public:
    ListDataController(QObject* parent=0);
    void service(HttpRequest &request, HttpResponse &response);

private:
    QList<HomeAlarmInfo> m_list;
};

#endif // LISTDATACONTROLLER_H
