#include "lockcontroller.h"
#include "requestmapper.h"
#include "sensorManagerInterface.h"
#include "common.h"

using namespace piHome;

LockController::LockController(QObject* parent) : HttpRequestHandler(parent)
{
    QStringList tmp = sensorManagerInterface::instance().checkActuator(typeToString(DoorLock));
    if(tmp.count() > 1)
    {
        qCritical() << "No more than one door lock currently supported!";
        lockAvailable = false;
    }
    else
    {
        lockAddress = tmp.first();
        lockAvailable = true;
    }
}

void LockController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = RequestMapper::sessionStore->getSession(request,
                                                                  response, true);

    if(request.getMethod() == "POST")
    {
        qDebug() << "POST old value: " << displayedState;

        bool currentState;
        QString tmpValue = sensorManagerInterface::instance().getActuatorValue(
                    typeToString(DoorLock),
                    lockAddress);

        if(tmpValue == "Locked")
            currentState = true;
        else
            currentState = false;

        if(displayedState != currentState)
        {
            qDebug() << "Changes occured since the page was loaded, reloading page!";
            displayedState = currentState;
        }
        else
        {
            bool postState;
            if(request.getParameter("doorLock") == "false")
                postState = false;
            else
                postState = true;

            if(displayedState != postState)
            {
                bool tmp;
                if(postState)
                {
                    // call set value and update the displayed state if operation successfully completed
                    tmp = sensorManagerInterface::instance().setActuatorValue(
                                typeToString(DoorLock),
                                lockAddress,
                                "Locked");
                    if(!tmp)
                        qCritical() << "Setting DoorLock sensor from " << lockAddress << "to \"Locked\" failed!";
                }
                else
                {
                    tmp = sensorManagerInterface::instance().setActuatorValue(
                                typeToString(DoorLock),
                                lockAddress,
                                "Locked");
                    if(!tmp)
                        qCritical() << "Setting DoorLock sensor from " << lockAddress << "to \"Unlocked\" failed!";
                }

                displayedState = postState;
            }
        }
        qDebug() << "POST current value: " << currentState;
        qDebug() << "POST page form value: " << request.getParameter("doorLock");
    }

    if(request.getMethod() == "GET")
    {
        // read the state from actuator
        QString tmpValue = sensorManagerInterface::instance().getActuatorValue(
                    typeToString(DoorLock),
                    lockAddress);

        if(tmpValue == "Locked")
            displayedState = true;
        else
            displayedState = false;
    }

    response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
    response.write("<html>\n");
    response.write("<head>\n");
    response.write("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
    response.write("<link rel=\"stylesheet\" href=\"https://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.css\">\n");
    response.write("<script src=\"https://code.jquery.com/jquery-1.11.3.min.js\"></script>\n");
    response.write("<script src=\"https://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.js\"></script>\n");
    response.write("</head>\n");

    response.write("<body>\n");
    response.write("\t<div data-role=\"page\" id=\"alarmpage\">\n");
    response.write("\t<div data-role=\"header\">\n");
    response.write("\t<div data-role=\"navbar\">\n");
    response.write("\t<ul>\n");
    response.write("\t\t<li><a href=\"/list\">Events list</a></li>\n");
    response.write("\t\t<li><a href=\"/alarm\">Alarm status</a></li>\n");
    response.write("\t\t<li><a href=\"/lock\">Door lock</a></li>\n");
    response.write("\t</ul>\n");
    response.write("\t</div>\n");
    response.write("\t</div>\n\n");

    response.write("\t<h1>Front door lock state:</h1>\n");

    response.write("\t<form name=\"form\" id=\"form\" method=\"post\">\n");

    response.write("\t\t<label for=\"doorLockLabel\">Door lock state</label>\n");
    response.write("\t\t<select name=\"doorLock\" id=\"doorLock\" data-role=\"slider\" data-autosubmit=\"true\">\n");
    if(displayedState)
    {
        // case Locked
        response.write("\t\t\t<option value=\"false\">Off</option>\n");
        response.write("\t\t\t<option value=\"true\" selected>On</option>\n");
    }
    else
    {
        // case Unlocked
        response.write("\t\t\t<option value=\"false\">Off</option>\n");
        response.write("\t\t\t<option value=\"true\">On</option>\n");
    }
    response.write("\t\t</select>\n");

    response.write("\t\t<br>\n\t\t<br>\n\t\t<button id=\"submit\">Update</button>\n");

    response.write("\t</form>\n");
    response.write("</body>\n");
    response.write("</html>\n", true);
}
