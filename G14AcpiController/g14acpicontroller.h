#ifndef G14ACPICONTROLLER_H
#define G14ACPICONTROLLER_H

#include <QObject>

#include "G14Acpi/g14acpi.h"
#include "G14ArmouryCrate/g14armourycrate.h"
#include "RyzenController/ryzencontroller.h"

class G14AcpiController : public QObject
{
    Q_OBJECT
public:
    explicit G14AcpiController(QObject *parent = nullptr);
    ~G14AcpiController();

    bool init(QString &error);

    bool setArmouryCratePlan(const G14ArmouryCratePlan plan, QString &error);
    bool setCpuProfile(const CpuProfile cpuProfile, QString &error);
    bool setMaxBatteryCharge(const int chargeLevel, QString &error);
private slots:
    void onAcpiEvent(const QByteArray &data);
    void onAcpiEventError(const QString &error);

private:
    G14Acpi *m_acpi = nullptr;
    RyzenController *m_cpuController = nullptr;

signals:
    void armouryCratePlanChanged(const G14ArmouryCratePlan &plan);
    void cpuProfileChanged(const CpuProfile &profile);
    void maxChargeLevelChanged(const int chargeLevel);

};

#endif // G14ACPICONTROLLER_H
