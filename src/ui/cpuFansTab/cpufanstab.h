#ifndef CPUFANSTAB_H
#define CPUFANSTAB_H

#include <QObject>
#include <QWidget>

#include "src/controller/servicecontroller.h"
#include "src/storage/settingsstorage.h"
#include "src/ui/base/BaseTab.h"
#include "fanCurveView/FanCurveView.h"

namespace Ui {
class CpuFansTab;
}

class CpuFansTab : public QWidget, public BaseTab {
    Q_OBJECT
public:
    CpuFansTab(const ServiceController *serviceController,
               const SettingsStorage *settingsStorage,
               QWidget *parent = nullptr);
    void setSelected(bool selected);

private:
    Ui::CpuFansTab *ui;
    FanCurveView *cpuFanEditor;
    FanCurveView *gpuFanEditor;

    const ServiceController *m_serviceController;
    const SettingsStorage *m_settingsStorage;

    bool currentCpuProfileChanged = false;
    int changedCpuDropdownIndex = -1;
    void cleanCpuProfileChangesIfAny();

    void loadArCratePlans();
    void loadCpuModes();
    void loadCpuProfiles();

    void setCpuSettingsEnabled(bool enabled);
    void setCpuSettingsFromCurrentValues();
    void setCpuSettingsFromCpuProfile(const CpuProfile &profile);

    CpuProfile saveCurrentCpuProfile(const QString &name) const;

private slots:
    void onCpuProfileDropdownSelected(int index);
    void onCpuSettingChanged(int indexOrValue);
    void onSaveCpuProfileClicked(bool);
    void onDeleteCpuProfileClicked(bool);


public:
    void loadFansProfiles();

signals:

    // BaseTab interface

};

#endif // CPUFANSTAB_H
