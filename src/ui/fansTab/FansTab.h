#ifndef G14CONTROLPP_FANSTAB_H
#define G14CONTROLPP_FANSTAB_H

#include <QWidget>
#include <QToolTip>
#include <QTimer>
#include <QSlider>
#include <QInputDialog>
#include <QMessageBox>
#include "src/atkacpi/AcpiControlSingleton.h"
#include "src/ryzenadj/RyzenAdjSingleton.h"
#include "src/settings/Settings.h"
#include "src/ui/base/BaseTab.h"
#include "fanCurveView/FanCurveView.h"

namespace Ui {
    class FansTab;
}

class FansTab : public QWidget, public BaseTab {
Q_OBJECT

public:
    explicit FansTab(QWidget *parent = nullptr);

    void setSelected(bool selected) override;

private:
    Ui::FansTab *ui;

    //QSlider *cpuSliders[8];
    //QSlider *gpuSliders[8];
    FanCurveView *cpuFanEditor;
    FanCurveView *gpuFanEditor;
    QTimer *qTimer;
    bool curveChanged = false;

    void reloadFanCurves();

    void selectFanProfile(FansProfile &profile, bool selectIndex = false);

    bool saveFansProfile(QString &name, bool override);

    FansProfile createFansProfileFromCurrentSettings();

    void applySettings(ArmouryCratePlan &powerPlan, bool useDefaultFanCurves, FansProfile &fansProfile);

    void loadSettings(bool apply = false);

    void refresh();

public slots:

    void defaultFanCurvesChange(int state);

    void onSaveFanCurvesClicked(bool checked);

    void onFanCurveIndexChanged(int index);

    void onSliderValueChanged(int value);

    void onApplyClicked(bool checked);

    void onDeleteProfileClicked(bool checked = false);


};


#endif //G14CONTROLPP_FANSTAB_H
