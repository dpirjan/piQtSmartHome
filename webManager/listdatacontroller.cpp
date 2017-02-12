#include <QTime>
#include "listdatacontroller.h"
#include "databaseManagerInterface.h"

ListDataController::ListDataController(QObject* parent) : HttpRequestHandler(parent)
{
}

void ListDataController::service(HttpRequest &request, HttpResponse &response)
{
    Q_UNUSED(request);

    qDebug() << "-->LIST REQUEST";
    response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");

    response.write("<html>\n");
    response.write("<head>\n");
    response.write("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
    response.write("<link rel=\"stylesheet\" href=\"https://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.css\">\n");
    response.write("<script src=\"https://code.jquery.com/jquery-1.11.3.min.js\"></script>\n");
    response.write("<script src=\"https://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.js\"></script>\n");
    response.write("<style>\n");
    response.write("th { border-bottom: 2px solid #d6d6d6;}\n");
    response.write("tr:nth-child(even) { background: #e9e9e9; }\n");
    response.write("</style>\n");
    response.write("</head>\n");

    response.write("<body>\n");
    response.write("<div data-role=\"page\" id=\"listpage\">\n");
    response.write("<div data-role=\"header\">\n");

    response.write("<div data-role=\"navbar\">");
    response.write("<ul>");
    response.write("<li><a href=\"/list\">Events list</a></li>");
    response.write("<li><a href=\"/alarm\">Alarm status</a></li>");
    response.write("<li><a href=\"/lock\">Door lock</a></li>");
    response.write("</ul>");
    response.write("<h1>Current time is on system is: " + QDateTime::currentDateTime().toString("yyyy-MMM-dd hh:mm:ss").toLatin1() + "</h1>\n");
    response.write("</div>");
    response.write("</div>");

    response.write("<div data-role=\"main\" class=\"ui-content\">\n");
    response.write("<table data-role=\"table\" data-mode=\"columntoggle\" class=\"ui-responsive ui-shadow\" id=\"eventTable\">\n");
    response.write("<thead>\n");
    response.write("<tr>\n<th data-priority=\"2\">Idx</th>\n<th>Zone</th>\n<th data-priority=\"1\">Node</th>\n<th>Category</th>\n<th>Date</th>\n</tr>\n");
    response.write("</thead>\n");
    response.write("<tbody>\n");

    m_list = databaseManagerInterface::instance().getAllHomeAlarmEntries();

    for(int i = 0; i < m_list.size(); i++)
    {
        response.write("<tr>\n<td>");
        response.write(QString::number(i + 1).toLatin1());
        response.write("</td>\n<td>");
        response.write(m_list.at(i).getZone().toLatin1());
        response.write("</td>\n<td>");
        response.write(m_list.at(i).getNode().toLatin1());
        response.write("</td>\n<td>");
        response.write(m_list.at(i).getCategory().toLatin1());
        response.write("</td>\n<td>");
        response.write(m_list.at(i).getTimestamp().toLatin1());
        response.write("</td>\n</tr>\n");
    }

    response.write("</tbody>\n");
    response.write("</table>\n");
    response.write("</div>\n");
    response.write("</div>\n");

    response.write("</body>\n</html>\n", true);
    qDebug() << "<--LIST REQUEST";
}

