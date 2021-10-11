#ifndef G14CONTROLPP_CPUTAB_H
#define G14CONTROLPP_CPUTAB_H

#include <QWidget>
#include <src/ryzenadj/RyzenAdjSingleton.h>
#include <src/settings/Settings.h>

namespace Ui {
    class CpuTab;
}

class CpuTab : public QWidget
{
    Q_OBJECT

public:
    explicit CpuTab(QWidget *parent = nullptr);
    void refresh();
public slots:
    void onPowerProfileSelected(int index);
    void onDeleteProfileClicked(bool checked = false);
    void onSaveClicked(bool checked = false);
    void onApplyClicked(bool checked = false);

private:
    Ui::CpuTab *ui;
    void loadPowerProfiles();
    void fillPowerProfileData(PowerProfile &profile);

    void loadCurrentPowerProfile();
};

#endif //G14CONTROLPP_CPUTAB_H
