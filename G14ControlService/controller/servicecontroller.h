#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include <QObject>
#include <G14AcpiController/g14acpicontroller.h>
#include "appState/appstate.h"

class ServiceController : public QObject
{
    Q_OBJECT
public:
    explicit ServiceController(QObject *parent = nullptr);
    ~ServiceController();

    bool init(QString &error);

private:
    AppState *m_appState = nullptr;

    G14AcpiController *m_acpiController = nullptr;

};

#endif // SERVICECONTROLLER_H
