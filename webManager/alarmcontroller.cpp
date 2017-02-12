#include "alarmcontroller.h"
#include "requestmapper.h"
#include "alarmManagerInterface.h"

AlarmController::AlarmController(QObject* parent) : HttpRequestHandler(parent)
{
}

void AlarmController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = RequestMapper::sessionStore->getSession(request,
                                                                  response, true);

    if(request.getMethod() == "POST")
    {
        qDebug() << "POST: old: " << displayedGeneralState << displayedNightState << displayedVacationState;

        bool currentGeneralState, currentNightState, currentVacationState;

        currentGeneralState = AlarmManagerInterface::instance().getAlarmGeneralState();
        currentNightState = AlarmManagerInterface::instance().getAlarmNightState();
        currentVacationState = AlarmManagerInterface::instance().getAlarmVacationState();

        // first check if changes occured since the page was displayed
        if(displayedGeneralState != currentGeneralState ||
                displayedNightState != currentNightState ||
                displayedVacationState != currentVacationState)
        {
            qDebug() << "Changes occured since the page was loaded, reloading page!";
            displayedGeneralState = currentGeneralState;
            displayedNightState = currentNightState;
            displayedVacationState = currentVacationState;
        }
        else
        {
            bool postGeneralState, postNightState, postVacationState;
            uint8_t postCountChanges = 0;

            if(request.getParameter("generalModeSwitch") == "false")
                postGeneralState = false;
            else
                postGeneralState = true;

            if(request.getParameter("nightModeSwitch") == "false")
                postNightState = false;
            else
                postNightState = true;

            if( request.getParameter("vacationModeSwitch") == "false")
                postVacationState = false;
            else
                postVacationState = true;

            if(displayedGeneralState != postGeneralState)
                postCountChanges++;

            if(displayedNightState != postNightState)
                postCountChanges++;

            if(displayedVacationState != postVacationState)
                postCountChanges++;

            qDebug() << "Page requested changes: " << postCountChanges;
            // only one change at a moment, otherwise reload page
            if(postCountChanges == 1)
            {
                if(displayedGeneralState != postGeneralState)
                    AlarmManagerInterface::instance().setAlarmGeneralState(postGeneralState);

                if(displayedNightState != postNightState)
                    AlarmManagerInterface::instance().setAlarmNightState(postNightState);

                if(displayedVacationState != postVacationState)
                    AlarmManagerInterface::instance().setAlarmVacationState(postVacationState);

                displayedGeneralState = postGeneralState;
                displayedNightState = postNightState;
                displayedVacationState = postVacationState;
            }
        }

        qDebug() << "POST: new: "
                 << currentGeneralState
                 << currentNightState
                 <<  currentVacationState;
        qDebug() << "POST: page form: "
                 << request.getParameter("generalModeSwitch")
                 << request.getParameter("nightModeSwitch")
                 << request.getParameter("vacationModeSwitch");
    }

    if(request.getMethod() == "GET")
    {
        displayedGeneralState = AlarmManagerInterface::instance().getAlarmGeneralState();
        displayedNightState = AlarmManagerInterface::instance().getAlarmNightState();
        displayedVacationState = AlarmManagerInterface::instance().getAlarmVacationState();
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

    response.write("\t<h1>Alarm System state:</h1>\n");

    response.write("\t<form name=\"form\" id=\"form\" method=\"post\">\n");

    response.write("\t\t<label for=\"generalModeSwitchLabel\">General Mode State</label>\n");
    response.write("\t\t<select name=\"generalModeSwitch\" id=\"generalModeSwitch\" data-role=\"slider\" data-autosubmit=\"true\">\n");
    if(AlarmManagerInterface::instance().getAlarmGeneralState())
    {
        response.write("\t\t\t<option value=\"false\">Off</option>\n");
        response.write("\t\t\t<option value=\"true\" selected>On</option>\n");
    }
    else
    {
        response.write("\t\t\t<option value=\"false\">Off</option>\n");
        response.write("\t\t\t<option value=\"true\">On</option>\n");
    }
    response.write("\t\t</select>\n");

    response.write("\t\t<label for=\"nightModeSwitchLabel\">Night Mode State</label>\n");
    response.write("\t\t<select name=\"nightModeSwitch\" id=\"nightModeSwitch\" data-role=\"slider\" data-autosubmit=\"true\">\n");
    if(AlarmManagerInterface::instance().getAlarmNightState())
    {
        response.write("\t\t\t<option value=\"false\">Off</option>\n");
        response.write("\t\t\t<option value=\"true\" selected>On</option>\n");
    }
    else
    {
        response.write("\t\t\t<option value=\"false\">Off</option>\n");
        response.write("\t\t\t<option value=\"true\">On</option>\n");
    }
    response.write("\t\t</select>\n");

    response.write("\t\t<label for=\"vacationModeSwitchLabel\">Vacation Mode State</label>\n");
    response.write("\t\t<select name=\"vacationModeSwitch\" id=\"vacationModeSwitch\" data-role=\"slider\" data-autosubmit=\"true\">\n");
    if(AlarmManagerInterface::instance().getAlarmVacationState())
    {
        response.write("\t\t\t<option value=\"false\">Off</option>\n");
        response.write("\t\t\t<option value=\"true\" selected>On</option>\n");
    }
    else
    {
        response.write("\t\t\t<option value=\"false\">Off</option>\n");
        response.write("\t\t\t<option value=\"true\">On</option>\n");
    }
    response.write("\t\t</select>\n");

    response.write("\t\t<br>\n\t\t<br>\n\t\t<button id=\"submit\">Update</button>\n");

    response.write("\t</form>\n");
    response.write("</body>\n");
    response.write("</html>\n", true);
}
