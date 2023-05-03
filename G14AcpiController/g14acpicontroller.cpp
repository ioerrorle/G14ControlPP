#include "g14acpicontroller.h"
#include "G14Acpi/controlcodes.h"

G14AcpiController::G14AcpiController(QObject *parent)
    : QObject{parent}
{

}

bool G14AcpiController::init(QString &error)
{
    m_acpi = new G14Acpi(this);
    if (!m_acpi->initAtkAcpi(error))
        return false;

    connect(m_acpi, &G14Acpi::acpiEvent, this, &G14AcpiController::onAcpiEvent);
    connect(m_acpi, &G14Acpi::acpiEventError, this, &G14AcpiController::onAcpiEventError);

    m_cpuController = new RyzenController(this);
    if (!m_cpuController->init(error))
        return false;
}

bool G14AcpiController::setArmouryCratePlan(const G14ArmouryCratePlan plan, QString &error)
{
    const uint64_t planArg = static_cast<uint64_t>(plan);
    QByteArray result;
    bool retval;
    if ((retval = m_acpi->sendDeviceIoControl(G14AcpiCodes::Function::DEVS,
                                    G14AcpiCodes::CommandCode::DEVID_POWER_POLICY,
                                    planArg, result, error))) {
        emit armouryCratePlanChanged(plan);
    }
    return retval;
}

bool G14AcpiController::setCpuProfile(const CpuProfile cpuProfile, QString &error)
{
    bool retval;
    if ((retval = m_cpuController->setCpuProfile(cpuProfile, error))) {
        emit cpuProfileChanged(cpuProfile);
    }
    return retval;
}

bool G14AcpiController::setMaxBatteryCharge(const int chargeLevel, QString &error) {
    uint chargeLevelValue = qBound(1, chargeLevel, 100);
    QByteArray result;
    bool retval;
    if ((retval = m_acpi->sendDeviceIoControl(G14AcpiCodes::Function::DEVS,
                                              G14AcpiCodes::CommandCode::DEVID_BATTERY_MAX_CHARGE,
                                              chargeLevelValue, result, error)))
        emit maxChargeLevelChanged(chargeLevelValue);
    return retval;
}

void G14AcpiController::onAcpiEvent(const QByteArray &data)
{

}

void G14AcpiController::onAcpiEventError(const QString &error)
{

}
