#ifndef G14CONTROLPP_FANSTAB_H
#define G14CONTROLPP_FANSTAB_H

#include "fanCurveView/FanCurveView.h"
#include "src/controller/servicecontroller.h"
#include "src/model/ArmouryCratePlan.h"
#include "src/model/FansProfile.h"
#include "src/storage/settingsstorage.h"
#include "src/ui/base/BaseTab.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSlider>
#include <QTimer>
#include <QToolTip>
#include <QWidget>

namespace Ui {
class FansTab;
}

class FansTab : public QWidget, public BaseTab {
    Q_OBJECT

public:
    FansTab(ServiceController *serviceController,
            SettingsStorage *settingsStorage,
            QWidget *parent = nullptr);

    void setSelected(bool selected) override;

private:
    ServiceController *serviceController;
    SettingsStorage *settingsStorage;

    Ui::FansTab *ui;
    FanCurveView *cpuFanEditor;
    FanCurveView *gpuFanEditor;
    QTimer *qTimer;
    bool fanCurvesChanged = false;

    void loadSavedFansProfilesList();

    void setupFanCurveEditors(const FansProfile &profile);

    void saveFansProfile(const QString &name);

    FansProfile createFansProfileFromCurrentSettings(const QString &name) const;

    FansProfile getCurrentFansProfile() const;

    void setFanCurvesChanged();

    void applySettings(const uchar powerPlanId, const FansProfile &fansProfile);

    void loadCurrentSettings();

    void setStatusText(float apuTemp, long cpuFanRpm, long gpuFanRpm);

    bool checkIfFansProfileExists(const QString &fansProfileName);

    void selectArCratePlan(const uchar powerPlanId);

    void selectFansProfile(const FansProfile &fansProfile);
public slots:

    void onSaveFansProfileClicked(bool checked);

    void onFansProfileSelected(int index);

    void onApplyClicked(bool checked);

    void onDeleteFansProfileClicked(bool checked);

    void update();
};

#endif //G14CONTROLPP_FANSTAB_H
