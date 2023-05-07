#include "servicecontroller.h"

ServiceController::ServiceController(QObject *parent)
    : QObject{parent}
{

}

ServiceController::~ServiceController()
{

}

bool ServiceController::init(QString &error)
{
    m_acpiController = new G14AcpiController(this);
    if (!m_acpiController->init(error)) {
        return false;
    }

    return true;
}
