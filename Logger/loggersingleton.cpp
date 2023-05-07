#include "loggersingleton.h"

#include <stdio.h>
#include <QtDebug>

LoggerSingleton &LoggerSingleton::getInstance() {
    static LoggerSingleton instance;
    return instance;
}

void LoggerSingleton::addOutput(ILogger *loggerOutput, bool isInfoOnly)
{
    QMutexLocker locker(&mutex);
    m_loggerOutputs.insert(loggerOutput, isInfoOnly);
}

void LoggerSingleton::removeOutput(ILogger *loggerOutput)
{
    QMutexLocker locker(&mutex);
    m_loggerOutputs.remove(loggerOutput);
}

LoggerSingleton::LoggerSingleton()
{
    qInstallMessageHandler(this->messageHandler);
}

void LoggerSingleton::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    getInstance().logMessage(type, context, msg);
}

void LoggerSingleton::logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(stdout);
    out << (msg + "\n");
    QMutexLocker locker(&mutex);
    for (ILogger *logger : m_loggerOutputs.keys()) {
        if (!m_loggerOutputs.value(logger, false) || type != QtMsgType::QtDebugMsg) {
            logger->logMessage(type, context, msg);
        }
    }
}
