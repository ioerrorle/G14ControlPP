#include "FansTab.h"
#include "ui_FansTab.h"

FansTab::FansTab(ServiceController *serviceController,
                 SettingsStorage *settingsStorage,
                 QWidget *parent)
    : QWidget(parent)
    , serviceController(serviceController)
    , settingsStorage(settingsStorage)
    , ui(new Ui::FansTab) {
    ui->setupUi(this);

    ui->deleteProfile->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->saveButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(ui->fanCurveComboBox,
            QOverload<int>::of(&QComboBox::activated),
            this,
            &FansTab::onFansProfileSelected);
    connect(ui->deleteProfile, &QPushButton::clicked, this, &FansTab::onDeleteFansProfileClicked);

    //CPU SECTION

    cpuFanEditor = new FanCurveView();
    dynamic_cast<QHBoxLayout *>(ui->cpuGroup->layout())->insertWidget(0, cpuFanEditor);
    //dynamic_cast<QHBoxLayout *>(ui->cpuGroup->layout())->setStretch(1, 1);

    ui->cpuRPM->setFixedWidth(250);
    ui->gpuRPM->setFixedWidth(250);

    //GPU SECTION

    gpuFanEditor = new FanCurveView();
    dynamic_cast<QHBoxLayout *>(ui->gpuGroup->layout())->insertWidget(0, gpuFanEditor);

    //BUTTONS SECTION

    connect(ui->saveButton, &QPushButton::clicked, this, &FansTab::onSaveFansProfileClicked);
    connect(ui->applyButton, &QPushButton::clicked, this, &FansTab::onApplyClicked);

    //ADD ALL

    //fill combo boxes
    for (const ArmouryCratePlan &plan : ArmouryCratePlan::Plans) {
        ui->arCrateProfileComboBox->addItem(plan.getName(), QVariant::fromValue(plan.getId()));
    }
    loadCurrentSettings();

    qTimer = new QTimer(this);
    qTimer->setInterval(250);
    connect(qTimer, &QTimer::timeout, this, &FansTab::update);
}

void FansTab::update() {
    float cpuTemp = serviceController->getCpuTemp();
    long cpuFanRpm;
    long gpuFanRpm;
    serviceController->getFansRpms(cpuFanRpm, gpuFanRpm);
    setStatusText(cpuTemp, cpuFanRpm, gpuFanRpm);
}

void FansTab::setStatusText(float apuTemp, long cpuFanRpm, long gpuFanRpm) {
    //todo move to separate thread
    ui->cpuRPM->setText(QString::asprintf("%.2f°C, %ld RPM", apuTemp, cpuFanRpm));
    ui->gpuRPM->setText(QString::number(gpuFanRpm) + " RPM");
}

void FansTab::onSaveFansProfileClicked(bool) {
    bool profileNameOk;
    QString profileName = QInputDialog::getText(this,
                                                tr("Save curves"),
                                                tr("Curve name:"),
                                                QLineEdit::Normal,
                                                nullptr,
                                                &profileNameOk);
    if (profileNameOk && !profileName.isEmpty()) {
        bool profileExists = checkIfFansProfileExists(profileName);
        if (profileExists) {
            QMessageBox confirmOverride;
            confirmOverride.setText("Fan curve with this name already exists");
            confirmOverride.setInformativeText("Do you want to override it?");
            confirmOverride.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            confirmOverride.setDefaultButton(QMessageBox::Ok);
            int confirmOverrideResult = confirmOverride.exec();

            if (confirmOverrideResult == QMessageBox::Ok) {
                saveFansProfile(profileName);
            }
        } else {
            saveFansProfile(profileName);
        }
    }
}

bool FansTab::checkIfFansProfileExists(const QString &fansProfileName) {
    return settingsStorage->fansProfileExists(fansProfileName);
}

void FansTab::loadSavedFansProfilesList() {
    ui->fanCurveComboBox->clear();
    auto fansProfiles = settingsStorage->getFansProfiles();
    fansProfiles.insert(0, FansProfile::Default);
    for (FansProfile &profile : fansProfiles) {
        ui->fanCurveComboBox->addItem(profile.getName(), QVariant::fromValue(profile));
    }
}

void FansTab::setupFanCurveEditors(const FansProfile &profile) {
    if (profile == FansProfile::Default) {
        uchar cpuFanTemps[8];
        uchar cpuFanSpeeds[8];
        uchar gpuFanSpeeds[8];
        uchar gpuFanTemps[8];

        serviceController->loadDefaultFanCurves(cpuFanTemps,
                                                cpuFanSpeeds,
                                                gpuFanTemps,
                                                gpuFanSpeeds);
    } else {
        auto cpuFanCurve = profile.getCpu();
        cpuFanEditor->setFanCurve(cpuFanCurve.getTemp(), cpuFanCurve.getSpeed());

        auto gpuFanCurve = profile.getGpu();
        gpuFanEditor->setFanCurve(gpuFanCurve.getTemp(), gpuFanCurve.getSpeed());
    }
}

void FansTab::saveFansProfile(const QString &name) {
    FansProfile profile = createFansProfileFromCurrentSettings(name);

    settingsStorage->saveFansProfile(profile);
}

FansProfile FansTab::createFansProfileFromCurrentSettings(const QString &name) const {
    uchar cpuFanTemps[8];
    uchar cpuFanSpeeds[8];
    uchar gpuFanSpeeds[8];
    uchar gpuFanTemps[8];

    cpuFanEditor->getFanCurve(cpuFanTemps, cpuFanSpeeds);
    gpuFanEditor->getFanCurve(gpuFanTemps, gpuFanSpeeds);

    return FansProfile(name,
                       FanCurve(cpuFanTemps, cpuFanSpeeds),
                       FanCurve(gpuFanTemps, gpuFanSpeeds));
}

FansProfile FansTab::getCurrentFansProfile() const {
    if (fanCurvesChanged) {
        return createFansProfileFromCurrentSettings(FansProfile::CurrentFansProfileName);
    } else {
        return ui->fanCurveComboBox->currentData().value<FansProfile>();
    }
}

void FansTab::setFanCurvesChanged() {}

void FansTab::onFansProfileSelected(int) {
    auto profile = ui->fanCurveComboBox->currentData().value<FansProfile>();
    setupFanCurveEditors(profile);
    this->fanCurvesChanged = false;
}

void FansTab::onApplyClicked(bool) {
    auto powerPlanId = ui->arCrateProfileComboBox->currentData().value<uchar>();
    auto fansProfile = getCurrentFansProfile();
    applySettings(powerPlanId, fansProfile);
}

void FansTab::applySettings(const uchar powerPlanId, const FansProfile &fansProfile) {
    serviceController->applyPowerPlanAndFansProfile(powerPlanId,
                                                    fansProfile.getCpu(),
                                                    fansProfile.getGpu());
}

void FansTab::loadCurrentSettings() {
    auto arCratePlanId = serviceController->getCurrentArCratePlanId();
    auto fansProfile = settingsStorage->getCurrentFansProfile(arCratePlanId);

    loadSavedFansProfilesList();
    selectArCratePlan(arCratePlanId);
    selectFansProfile(fansProfile);
}

void FansTab::selectArCratePlan(const uchar powerPlanId) {
    int arCratePlanIndex = ui->arCrateProfileComboBox->findData(QVariant::fromValue(powerPlanId));
    ui->arCrateProfileComboBox->setCurrentIndex(arCratePlanIndex);
}

void FansTab::selectFansProfile(const FansProfile &fansProfile) {
    int currentFansProfileIndex = ui->fanCurveComboBox->findData(QVariant::fromValue(fansProfile));
    if (currentFansProfileIndex != -1) {
        ui->fanCurveComboBox->setCurrentIndex(currentFansProfileIndex);
    }
    setupFanCurveEditors(fansProfile);
}

void FansTab::onDeleteFansProfileClicked(bool) {
//    auto currentData
//        = FansProfile::fromQStringList(ui->fanCurveComboBox->currentText(),
//                                       ui->fanCurveComboBox->currentData().value<QStringList>());
//    SETT.deleteFansProfile(currentData.getName());
//    loadSavedFansProfilesList();
}

void FansTab::setSelected(bool selected) {
    if (selected) {
        qTimer->start();
    } else {
        qTimer->stop();
    }
}
