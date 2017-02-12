#include <QTime>

#include "logincontroller.h"
#include "requestmapper.h"
#include "userManagerInterface.h"

LoginController::LoginController(QObject* parent) : HttpRequestHandler(parent)
{
}

void LoginController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = RequestMapper::sessionStore->getSession(request,
                                                                  response, true);
    QByteArray username=request.getParameter("username");
    QByteArray password=request.getParameter("password");

    response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
    response.write("<html>\n");
    response.write("<head>\n");
    response.write("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
    response.write("<link rel=\"stylesheet\" href=\"https://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.css\">\n");
    response.write("<script src=\"https://code.jquery.com/jquery-1.11.3.min.js\"></script>\n");
    response.write("<script src=\"https://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.js\"></script>\n");
    response.write("</head>\n");
    response.write("<body>\n");

    if (UserManagerInterface::instance().checkPasswordForUser(username, password))
    {
        response.write("<div data-role=\"page\" id=\"loginpage\">\n");
        response.write("<div data-role=\"header\">\n");
        response.write("<div data-role=\"navbar\">\n");
        response.write("\t<ul>\n");
        response.write("\t<li><a href=\"/list\">Events list</a></li>\n");
        response.write("\t<li><a href=\"/alarm\">Alarm status</a></li>\n");
        response.write("\t<li><a href=\"/lock\">Door lock</a></li>\n");
        response.write("\t</ul>\n");
        response.write("</div>\n");
        response.write("</div>\n");


        response.write("<br>\n<font color=\"green\">You logged in successfully at "
                       + QDateTime::currentDateTime().toString("HH:mm:ss").toLocal8Bit()
                       + " as " + username + "</font>\n<br>\n");

        session.set("username", username);
        session.set("logintime", QTime::currentTime());
    }
    else
    {
        response.write("<h1>Welcome to piHome.</h1>\n<h2>Please login.</h2>\n");
        response.write("<form method=\"POST\" action=\"/login\">\n");
        if (!username.isEmpty())
            response.write("<font size=\"3\" color=\"red\">Wrong credentials, please try again!</font>\n<br>\n<br>\n");

        response.write("\t<div class=\"container\">\n");
        response.write("\t\t<label><b>Username</b></label>\n");
        response.write("\t\t<input type=\"text\" placeholder=\"Enter Username\" name=\"username\" required>\n\t\t<br>\n");
        response.write("\t\t<label><b>Password</b></label>\n");
        response.write("\t\t<input type=\"password\" placeholder=\"Enter Password\" name=\"password\" required>\n\t\t<br>\n\t\t<br>\n");
        response.write("\t\t<input type='submit' value='Login'>\n");
        response.write("\t</div>\n");
        response.write("</form>\n");
    }

    response.write("</body>\n</html>\n", true);
}
