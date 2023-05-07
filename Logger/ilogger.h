#ifndef ILOGGER_H
#define ILOGGER_H

#include <QMessageLogContext>

class ILogger
{
public:
    virtual void logMessage(QtMsgType type, const QMessageLogContext &, const QString &msg) = 0;
};

#endif // ILOGGER_H
