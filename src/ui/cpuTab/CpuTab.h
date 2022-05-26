#ifndef G14CONTROLPP_CPUTAB_H
#define G14CONTROLPP_CPUTAB_H

#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <src/ryzenadj/RyzenAdjSingleton.h>
#include <src/settings/Settings.h>
#include "src/ui/base/BaseTab.h"

namespace Ui {
    class CpuTab;
}

class CpuTab : public QWidget, public BaseTab {
Q_OBJECT

public:
    explicit CpuTab(QWidget *parent = nullptr);

    void setSelected(bool selected);

public slots:

    void onPowerProfileSelected(int index);

    void onDeleteProfileClicked(bool checked = false);

    void onSaveClicked(bool checked = false);

    void onApplyClicked(bool checked = false);

    void onPowerProfileChanged(int value);

private:
    Ui::CpuTab *ui;
    QTimer *qTimer;
    bool powerProfileChanged;
    bool fromDropdown = false;

    void loadPowerProfiles();

    void fillPowerProfileData(CpuProfile &profile);

    void loadCurrentPowerProfile();

    CpuProfile createPowerProfileFromData();

    bool saveCurrentPowerProfile(QString &name, bool override);

    void reloadPowerProfiles();

    void selectPowerProfile(CpuProfile &profile, bool selectIndex);

    void refresh();

    CpuProfile createPowerProfileFromCurrentCpuState();
};

#endif //G14CONTROLPP_CPUTAB_H
