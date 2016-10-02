#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>

// for stack trace
#include <execinfo.h>
#include <cxxabi.h>

#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QDir>

#include "utils.h"

void logHandler(QtMsgType type,
                const QMessageLogContext &context,
                const QString &msg)
{
    QString logFilePath = QDir::homePath().append(QDir::separator());
    logFilePath.append(".piHome");
    logFilePath.append(QDir::separator());
    logFilePath.append("logging");
    logFilePath.append(QDir::separator());
    logFilePath.append(QCoreApplication::applicationName());
    logFilePath.append(".log");

    QFile logFile(logFilePath);

    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream textStream(&logFile);

    QString text;
    if(context.file && context.line != 0 && context.function)
    {
        text.append(":<");
        text.append(QDateTime::currentDateTime().toString("dd-MMM-yyyy hh:mm:ss.zzz"));
        text.append(">:");
        QString filePath = context.file;
        QStringList parts = filePath.split(QDir::separator());
        text.append(parts.last());
        text.append(":");
        text.append(QString::number(context.line));
        text.append("-->");
        text.append(context.function);
    }
    text.append("] : ");
    text.append(msg);

    switch (type) {
    case QtDebugMsg:
        text.insert(0, "[DEBUG");
        textStream << text << endl;
        break;
    case QtWarningMsg:
        text.insert(0, "[WARNING");
        textStream << text << endl;
        break;
#if QT_VERSION > 0x050401
    case QtInfoMsg:
        text.insert(0, "[INFO");
        textStream << text << endl;
        break;
#endif
    case QtCriticalMsg:
        text.insert(0, "[CRITICAL");
        textStream << text << endl;
        break;
    case QtFatalMsg:
        text = QString("[FATAL");
        textStream << text << endl;
        logFile.close();
        abort();
    }

    logFile.close();
}

void closeApplication(int sig)
{
    qCritical() << "Close application"
             << QCoreApplication::applicationName()
             << "( signal =" << sig << "-" << strsignal(sig) << ")";

    QCoreApplication::quit();
}

void catchUnixSignal(const QList<int> &quitSignals)
{
    for(int sig = 0; sig < quitSignals.count(); sig++)
        if(quitSignals.at(sig) == SIGSEGV)
            signal(quitSignals.at(sig), printStackTrace);
        else
            signal(quitSignals.at(sig), closeApplication);
}

void printStackTrace(int sig)
{
    qCritical() << "Failed application ( signal = "
             << sig << "-" << strsignal(sig) << ")";
    qCritical() << "BackTrace: ";

    void *addrArray[64]; // used to store backtrace symbols

    size_t addrSize = backtrace(addrArray, sizeof(addrArray) / sizeof(void *));
    if(addrSize == 0)
    {
        qCritical() << "<empty, possibly corrupt>";
        exit(-1);
    }

    // convert addresses into strings "filename(function+address)"
    char **symbolArray = backtrace_symbols(addrArray, addrSize);

    // skip first stack frame
    for(size_t counter = 1; counter < addrSize; counter++)
    {
        char *mangledName = 0, *offsetBegin = 0, *offsetEnd = 0;

        for(char *p = symbolArray[counter]; *p; ++p)
        {
            if(*p == '(')
                mangledName = p;
            else if(*p == '+')
                offsetBegin = p;
            else if(*p == ')' && offsetBegin)
            {
                offsetEnd = p;
                break;
            }
        }

        // the line was processed
        if(mangledName && offsetBegin && offsetEnd && mangledName < offsetBegin)
        {
            *mangledName++ = '\0';
            *offsetBegin++ = '\0';
            *offsetEnd = '\0';

            // mangled name is now in [mangledName, offsetBegin) and caller
            // offset in [offsetBegin, offsetEnd). now apply
            // __cxa_demangle():

            int status;
            char* realName = abi::__cxa_demangle(mangledName, 0, 0, &status);
            if(status == 0)
                qCritical() << "[bt]: (" << counter << ")" << symbolArray[counter]
                         << ":" << realName << "+" << offsetBegin << offsetEnd;
            else
                // demangling failed. Output function name as a C function with
                // no arguments.
                qCritical() << "[bt]: (" << counter << ")"  << symbolArray[counter]
                         << ":" << mangledName << "+" << offsetBegin << offsetEnd;
        }
        else
            // couldn't parse the line? print the whole line.
            qCritical() << "[bt] (" << counter << ")"  << symbolArray[counter];
    }

    free(symbolArray);

    exit(1);
}
