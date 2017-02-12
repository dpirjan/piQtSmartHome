#include "requestmapper.h"

HttpSessionStore* RequestMapper::sessionStore=0;

RequestMapper::RequestMapper(QObject* parent) : HttpRequestHandler(parent)
{
}

void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    QByteArray sessionId=sessionStore->getSessionId(request,response);

    if(sessionId.isEmpty() && path != "/login")
    {
        qDebug("RequestMapper: redirect to login page");
        response.redirect("/login");
        return;
    }

    if(path == "/login")
        loginController.service(request, response);
    else if(path == "/list")
        listDataController.service(request, response);
    else if(path == "/alarm")
        alarmController.service(request, response);
    else if(path == "/lock")
        lockController.service(request, response);
    else
    {
        response.setStatus(404, "Not found");
        response.write("The URL is wrong, no such document.");
    }

    qDebug("RequestMapper: finished request");
}
