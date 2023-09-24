#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "G14AppState/appstate.h"
#include "Rpc/client/rpcclient.h"
#include <QObject>

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = nullptr);

    void requestAppState();

private slots:
    void onAppStateUpdated(const AppState &appState);

private:
    AppState m_appState;

    RpcClient *m_rpcClient = nullptr;

signals:
    void appStateUpdated(const AppState &appState);

};

#endif // MAINCONTROLLER_H
