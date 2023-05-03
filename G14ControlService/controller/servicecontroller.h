#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include <QObject>
#include "appState/appstate.h"
#include "RyzenController/ryzencontroller.cpp"

class ServiceController : public QObject
{
    Q_OBJECT
public:
    explicit ServiceController(QObject *parent = nullptr);
    ~ServiceController();

private:
    AppState *m_appState = nullptr;

    RyzenController *m_RyzenController = nullptr;
signals:

};

#endif // SERVICECONTROLLER_H
