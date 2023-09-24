#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include "G14AppState/appstate.h"
#include "G14AcpiController/g14acpicontroller.h"
#include "Rpc/proto/response/appstateresponse.h"
#include <QObject>


class ServiceController : public QObject
{
    Q_OBJECT
public:
    explicit ServiceController(QObject *parent = nullptr);
    ~ServiceController();

    bool init(QString &error);

public slots:
    void onAppStateRequested(g14rpc::AppStateResponse *appStateResponse);

private:
    AppState *m_appState = nullptr;

    G14AcpiController *m_acpiController = nullptr;

};

#endif // SERVICECONTROLLER_H
