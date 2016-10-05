#ifndef UTILS
#define UTILS

#include <QDebug>
#include <signal.h>

void logHandler(QtMsgType type,
                const QMessageLogContext &context,
                const QString &msg);

void closeApplication(int sig);

void catchUnixSignal(const QList<int> &quitSignals);

void printStackTrace(int sig);

#endif // UTILS
