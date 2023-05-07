#ifndef LOGGERSINGLETON_H
#define LOGGERSINGLETON_H

#include <QMap>
#include <QMutex>

#include "ilogger.h"

class LoggerSingleton
{
public:
    static LoggerSingleton &getInstance();
    void addOutput(ILogger *loggerOutput, bool isInfoOnly);
    void removeOutput(ILogger *loggerOutput);

private:
    QMap<ILogger*, bool> m_loggerOutputs;

    LoggerSingleton();
    static void messageHandler(QtMsgType, const QMessageLogContext &, const QString &msg);
    QMutex mutex;
    void logMessage(QtMsgType type, const QMessageLogContext &, const QString &msg);
};

#endif // LOGGERSINGLETON_H
