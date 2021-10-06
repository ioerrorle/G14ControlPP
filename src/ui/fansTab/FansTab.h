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
#include <src/atkacpi/AcpiControlSingleton.h>
#include <src/ryzenadj/RyzenAdjSingleton.h>
#include <src/settings/Settings.h>

class FansTab : public QWidget
{
    Q_OBJECT

public:
    explicit FansTab(QWidget *parent = nullptr);
    void refresh();

private:
    QLabel *gpuRPM;
    QLabel *cpuRPM;
    QComboBox *arCrateProfileSelector;
    QCheckBox *defaultFanCurves;
    QComboBox *fanCurveComboBox;
    QSlider *cpuSliders[8];
    QSlider *gpuSliders[8];

    void reloadFanCurves();
    void selectFanProfile(FansProfile &profile);

public slots:
    void defaultFanCurvesChange(int state);
    void onSaveFanCurvesClicked(bool checked);
    void onFanCurveIndexChanged(int index);

    void onApplyClicked(bool checked);
};


#endif //G14CONTROLPP_FANSTAB_H
