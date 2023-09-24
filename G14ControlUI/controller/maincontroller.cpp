#include "maincontroller.h"

MainController::MainController(QObject *parent)
    : QObject{parent}
    , m_rpcClient(new RpcClient(this))
{
    connect(m_rpcClient, &RpcClient::appStateResponse, this, &MainController::onAppStateUpdated);
}

void MainController::requestAppState()
{
    m_rpcClient->requestAppState();
}

void MainController::onAppStateUpdated(const AppState &appState)
{
    m_appState = appState;
    emit appStateUpdated(m_appState);
}
