#include "FansTab.h"
#include "ui_FansTab.h"


FansTab::FansTab(QWidget *parent) : QWidget(parent), ui(new Ui::FansTab) {

    ui->setupUi(this);

    ui->deleteProfile->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->saveButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(ui->defaultFanCurves, &QCheckBox::stateChanged, this, &FansTab::defaultFanCurvesChange);
    connect(ui->fanCurveComboBox, QOverload<int>::of(&QComboBox::activated), this,
            &FansTab::onFanCurveIndexChanged);
    connect(ui->deleteProfile, &QPushButton::clicked, this, &FansTab::onDeleteProfileClicked);

    //CPU SECTION
    cpuSliders[0] = ui->cpuSpeed_0;
    cpuSliders[1] = ui->cpuSpeed_1;
    cpuSliders[2] = ui->cpuSpeed_2;
    cpuSliders[3] = ui->cpuSpeed_3;
    cpuSliders[4] = ui->cpuSpeed_4;
    cpuSliders[5] = ui->cpuSpeed_5;
    cpuSliders[6] = ui->cpuSpeed_6;
    cpuSliders[7] = ui->cpuSpeed_7;

    for (int i = 0; i < 8; i++) {
        auto *qSlider = cpuSliders[i];
        qSlider->setMaximum(100);

        connect(qSlider, &QSlider::valueChanged, this, &FansTab::onSliderValueChanged);
        connect(qSlider, &QSlider::sliderMoved, this, [&](int value) {
            QToolTip::showText(QCursor::pos(), QString("%1").arg(value), nullptr);
        });
    }

    //GPU SECTION

    gpuSliders[0] = ui->gpuSpeed_0;
    gpuSliders[1] = ui->gpuSpeed_1;
    gpuSliders[2] = ui->gpuSpeed_2;
    gpuSliders[3] = ui->gpuSpeed_3;
    gpuSliders[4] = ui->gpuSpeed_4;
    gpuSliders[5] = ui->gpuSpeed_5;
    gpuSliders[6] = ui->gpuSpeed_6;
    gpuSliders[7] = ui->gpuSpeed_7;

    for (int i = 0; i < 8; i++) {
        auto *qSlider = gpuSliders[i];
        qSlider->setMaximum(100);

        connect(qSlider, &QSlider::valueChanged, this, &FansTab::onSliderValueChanged);
        connect(qSlider, &QSlider::sliderMoved, this, [&](int value) {
            QToolTip::showText(QCursor::pos(), QString("%1").arg(value), nullptr);
        });
    }

    //BUTTONS SECTION

    connect(ui->saveButton, &QPushButton::clicked, this, &FansTab::onSaveFanCurvesClicked);
    connect(ui->applyButton, &QPushButton::clicked, this, &FansTab::onApplyClicked);

    //ADD ALL

    //fill combo boxes
    for (ArmouryCratePlan &plan : ARMOURY_CRATE_PLANS) {
        ui->arCrateProfileComboBox->addItem(plan.name, QVariant::fromValue(plan.id));
    }
    loadSettings(false);

    qTimer = new QTimer(this);
    qTimer->setInterval(250);
    connect(qTimer, &QTimer::timeout, this, &FansTab::refresh);
}

void FansTab::onSliderValueChanged(int value) {
    this->curveChanged = true;
    FansProfile profile = createFansProfileFromCurrentSettings();
    AcpiControlSingleton::fixFanCurve(FAN_CPU, profile.cpu);
    AcpiControlSingleton::fixFanCurve(FAN_GPU, profile.gpu);
    selectFanProfile(profile, false);
}

void FansTab::refresh() {
    RY.refreshTable();

    ui->cpuRPM->setText(QString::asprintf("%.2f°C, %ld RPM",
                                      RY.getApuTemp(),
                                      AcpiControlSingleton::getInstance().getCpuFanSpeed()));
    ui->gpuRPM->setText(QString::number(AcpiControlSingleton::getInstance().getGpuFanSpeed()) + " RPM");
}

void FansTab::defaultFanCurvesChange(int state) {
    if (state) {
        ui->fanCurveComboBox->clear();
        ui->fanCurveComboBox->setEnabled(false);
    } else {
        ui->fanCurveComboBox->setEnabled(true);
        reloadFanCurves();
    }
}

void FansTab::onSaveFanCurvesClicked(bool checked) {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Save curves"),
                                         tr("Curve name:"), QLineEdit::Normal,
                                         nullptr, &ok);
    if (ok && !text.isEmpty()) {
        bool result = saveFansProfile(text, false);
        if (!result) {
            QMessageBox msgBox;
            msgBox.setText("Fan curve with this name already exists");
            msgBox.setInformativeText("Do you want to override it?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();

            if (ret == QMessageBox::Ok) {
                bool result = saveFansProfile(text, true);
            } else {
                //it's either cancel or something strange
            }
        }

    }
}

bool FansTab::saveFansProfile(QString &name, bool override) {
    FansProfile profile = createFansProfileFromCurrentSettings();
    profile.name = name;

    bool result = SETT.saveFansProfile(profile, override);

    if (!result)
        return false;

    reloadFanCurves();
    selectFanProfile(profile, true);
    return true;
}

FansProfile FansTab::createFansProfileFromCurrentSettings() {
    FansProfile profile = {};
    if (!curveChanged) {
        profile.name = ui->fanCurveComboBox->currentText();
    }
    FanCurve cpuCurve = {};
    FanCurve gpuCurve = {};
    for (int i = 0; i < 8; i++) {
        cpuCurve.temp[i] = gpuCurve.temp[i] = 35 + i * 10;
        cpuCurve.speed[i] = cpuSliders[i]->value();
        gpuCurve.speed[i] = gpuSliders[i]->value();
    }
    profile.cpu = cpuCurve;
    profile.gpu = gpuCurve;

    return profile;
}

void FansTab::reloadFanCurves() {
    if (ui->fanCurveComboBox->isEnabled()) {
        ui->fanCurveComboBox->clear();
        ui->fanCurveComboBox->setEnabled(true);
        auto profiles = SETT.getFansProfiles();
        for (FansProfile &profile : profiles) {
            ui->fanCurveComboBox->addItem(profile.name, QVariant::fromValue(profile));
        }
    }
}

void FansTab::selectFanProfile(FansProfile &profile, bool selectIndex) {
    if (selectIndex) {
        int i = ui->fanCurveComboBox->findText(profile.name);
        if (i != -1) {
            ui->fanCurveComboBox->setCurrentIndex(i);
        }
    }
    //this->fanCurveComboBox->setCurrentText(profile.name);
    for (int i = 0; i < 8; i++) {
        cpuSliders[i]->setValue(profile.cpu.speed[i]);
        gpuSliders[i]->setValue(profile.gpu.speed[i]);
    }
}

void FansTab::onFanCurveIndexChanged(int index) {
    if (ui->fanCurveComboBox->isEnabled()) {
        auto profile = ui->fanCurveComboBox->itemData(index).value<FansProfile>();
        selectFanProfile(profile);
        this->curveChanged = false;
    }
}

void FansTab::onApplyClicked(bool checked) {
    auto powerPlanId = ui->arCrateProfileComboBox->currentData().value<uchar>();

    SETT.setCurrentPowerPlan(powerPlanId);
    SETT.setUseDefaultFanCurves(ui->defaultFanCurves->isChecked());
    FansProfile fansProfile = createFansProfileFromCurrentSettings();
    SETT.setCurrentFanCurveProfile(fansProfile);

    applySettings(ARMOURY_CRATE_PLANS[powerPlanId], ui->defaultFanCurves->isChecked(), fansProfile);
}

void FansTab::applySettings(ArmouryCratePlan &powerPlan, bool useDefaultFanCurves, FansProfile &fansProfile) {
    AcpiControlSingleton::getInstance().setPowerPlan(powerPlan.asusPlanCode);
    if (!useDefaultFanCurves) {
        AcpiControlSingleton::getInstance().setFanProfile(fansProfile);
    }
}

void FansTab::loadSettings(bool apply) {
    auto powerPlan = SETT.getCurrentPowerPlan();
    auto useDefaultFanCurves = SETT.getUseDefaultFanCurves();
    auto profile = SETT.getCurrentFanCurveProfile();

    int ppIndex = ui->arCrateProfileComboBox->findText(powerPlan.name);
    if (ppIndex != -1) {
        ui->arCrateProfileComboBox->setCurrentIndex(ppIndex);
    }
    ui->defaultFanCurves->setChecked(useDefaultFanCurves);
    reloadFanCurves();
    selectFanProfile(profile, true);
    ui->fanCurveComboBox->setEnabled(!useDefaultFanCurves);

    if (apply) {
        applySettings(powerPlan, useDefaultFanCurves, profile);
    }
}

void FansTab::onDeleteProfileClicked(bool checked) {
    auto currentData = ui->fanCurveComboBox->currentData().value<FansProfile>();
    SETT.deleteFansProfile(currentData);
    reloadFanCurves();
}

void FansTab::setSelected(bool selected) {
    if (selected) {
        qTimer->start();
    } else {
        qTimer->stop();
    }
}
