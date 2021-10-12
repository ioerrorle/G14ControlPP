#ifndef G14CONTROLPP_FANSTAB_H
#define G14CONTROLPP_FANSTAB_H

#include <QtCore/QArgument>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QMessageBox>
#include <src/atkacpi/AcpiControlSingleton.h>
#include <src/ryzenadj/RyzenAdjSingleton.h>
#include <src/settings/Settings.h>
#include <QDebug>

namespace Ui {
    class FansTab;
}

class FansTab : public QWidget
{
    Q_OBJECT

public:
    explicit FansTab(QWidget *parent = nullptr);
    void refresh();

private:
    Ui::FansTab *ui;

    QSlider *cpuSliders[8];
    QSlider *gpuSliders[8];
    bool curveChanged = false;

    void reloadFanCurves();
    void selectFanProfile(FansProfile &profile, bool selectIndex = false);
    bool saveFansProfile(QString &name, bool override);
    FansProfile createFansProfileFromCurrentSettings();
    void applySettings(ArmouryCratePlan &powerPlan, bool useDefaultFanCurves, FansProfile &fansProfile);
    void loadSettings(bool apply = false);

public slots:
    void defaultFanCurvesChange(int state);
    void onSaveFanCurvesClicked(bool checked);
    void onFanCurveIndexChanged(int index);
    void onSliderValueChanged(int value);
    void onApplyClicked(bool checked);
    void onDeleteProfileClicked(bool checked = false);


};


#endif //G14CONTROLPP_FANSTAB_H
