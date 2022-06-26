#ifndef G14CONTROLPP_CPUTAB_H
#define G14CONTROLPP_CPUTAB_H

#include "src/ui/base/BaseTab.h"
#include <src/controller/servicecontroller.h>
#include <src/storage/settingsstorage.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QWidget>

enum CpuPresets {
    CPU_PRESET_DEFAULT = 0,
    CPU_PRESET_CURRENT = 1
};

namespace Ui {
class CpuTab;
}

class CpuTab : public QWidget, public BaseTab {
    Q_OBJECT

public:
    explicit CpuTab(ServiceController *serviceController,
                    SettingsStorage *settingsStorage,
                    QWidget *parent = nullptr);

    void setSelected(bool selected);

public slots:

    void onPowerProfileSelected(int);

    void onDeleteProfileClicked(bool);

    void onSaveClicked(bool);

    void onApplyClicked(bool);

    void onPowerProfileChanged(int);

private:
    Ui::CpuTab *ui;
    ServiceController *serviceController;
    SettingsStorage *settingsStorage;
    QTimer *qTimer;
    bool cpuProfileChanged;
    bool fromDropdown = false;

    void fillCpuModesList();

    void fillCpuProfileData(const CpuProfile &profile);

    void loadCurrentCpuProfile();

    CpuProfile createPowerProfileFromData(const QString &name);

    void saveCurrentCpuProfile(const QString &name);

    void loadCpuProfiles();

    void update();

    CpuProfile createPowerProfileFromCurrentCpuState();
    void refreshCpuStatus(const CpuStatus &cpuStatus);
    void setInputEnabled(bool enabled);
};

#endif //G14CONTROLPP_CPUTAB_H
