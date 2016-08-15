#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("mailManager");

    qDebug() << "Mail Manager";

    int ret = app.exec();

    return ret;
}
