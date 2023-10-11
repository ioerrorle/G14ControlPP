#include "servicecontroller.h"



ServiceController::ServiceController(QObject *parent)
    : QObject{parent}
    , m_appState(new AppState)
    , m_settingsStorage(new SettingsStorage(this))
{

}

ServiceController::~ServiceController()
{
    delete m_appState;
}

bool ServiceController::init(QString &error)
{
    m_acpiController = new G14AcpiController(this);
    if (!m_acpiController->init(error)) {
        return false;
    }

    return true;
}

void ServiceController::onAppStateRequested(g14rpc::AppStateResponse *appStateResponse)
{
    appStateResponse->appState = *m_appState;
}

void ServiceController::readSettings()
{

}
