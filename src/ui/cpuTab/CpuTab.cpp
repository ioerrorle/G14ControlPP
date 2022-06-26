#include "CpuTab.h"
#include "ui_CpuTab.h"

CpuTab::CpuTab(ServiceController *serviceController,
               SettingsStorage *settingsStorage,
               QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CpuTab)
    , serviceController(serviceController)
    , settingsStorage(settingsStorage) {
    ui->setupUi(this);

    ui->deleteProfile->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->save->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(ui->cpuProfileDropdown,
            QOverload<int>::of(&QComboBox::activated),
            this,
            &CpuTab::onPowerProfileSelected);
    connect(ui->deleteProfile, &QPushButton::clicked, this, &CpuTab::onDeleteProfileClicked);
    connect(ui->save, &QPushButton::clicked, this, &CpuTab::onSaveClicked);
    connect(ui->apply, &QPushButton::clicked, this, &CpuTab::onApplyClicked);

    fillCpuModesList();
    loadCpuProfiles();
    loadCurrentCpuProfile();

    connect(ui->stapmLimitSetting,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &CpuTab::onPowerProfileChanged);
    connect(ui->stapmTimeSetting,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &CpuTab::onPowerProfileChanged);
    connect(ui->slowLimitSetting,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &CpuTab::onPowerProfileChanged);
    connect(ui->slowTimeSetting,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &CpuTab::onPowerProfileChanged);
    connect(ui->fastLimitSetting,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &CpuTab::onPowerProfileChanged);
    connect(ui->modeSetting,
            QOverload<int>::of(&QComboBox::activated),
            this,
            &CpuTab::onPowerProfileChanged);

    qTimer = new QTimer(this);
    qTimer->setInterval(250);
    connect(qTimer, &QTimer::timeout, this, &CpuTab::update);
}

void CpuTab::update() {
    CpuStatus currentCpuStatus;
    serviceController->getCpuStatus(currentCpuStatus);
    refreshCpuStatus(currentCpuStatus);
}

void CpuTab::refreshCpuStatus(const CpuStatus &cpuStatus) {
    ui->stapmLimit->setText(
        QString::asprintf("%.1f (%.1f)", cpuStatus.stapmLimit, cpuStatus.stapmValue));
    ui->stapmTime->setText(QString::asprintf("%.0f", cpuStatus.stapmTime));
    ui->fastLimit->setText(
        QString::asprintf("%.1f (%.1f)", cpuStatus.fastLimit, cpuStatus.fastValue));
    ui->slowLimit->setText(
        QString::asprintf("%.1f (%.1f)", cpuStatus.slowLimit, cpuStatus.slowValue));
    ui->slowTime->setText(QString::asprintf("%.0f", cpuStatus.slowTime));

    ui->coreTemp->setText(
        QString::asprintf("%.1f (%.1f)", cpuStatus.coreTempLimit, cpuStatus.coreTempValue));
    ui->apuTemp->setText(
        QString::asprintf("%.1f (%.1f)", cpuStatus.apuTempLimit, cpuStatus.apuTempValue));
    ui->dGpuTemp->setText(
        QString::asprintf("%.1f (%.1f)", cpuStatus.dgpuTempLimit, cpuStatus.dgpuTempValue));
    ui->mode->setText(cpuStatus.setPoint == 95.0f ? "Power saving" : "Performance");
}

void CpuTab::fillCpuModesList() {
    ui->modeSetting->addItem("Performance", SP_PERFORMANCE);
    ui->modeSetting->addItem("Power saving", SP_POWER_SAVING);
    ui->modeSetting->addItem("Default", SP_DEFAULT);
}

void CpuTab::loadCpuProfiles() {
    ui->cpuProfileDropdown->clear();

    ui->cpuProfileDropdown->addItem("<Default>", CPU_PRESET_DEFAULT);
    ui->cpuProfileDropdown->addItem("<Current>", CPU_PRESET_CURRENT);

    auto profiles = settingsStorage->getCpuProfiles();
    for (CpuProfile &profile : profiles) {
        ui->cpuProfileDropdown->addItem(profile.getName(), QVariant::fromValue(profile));
    }
}

void CpuTab::onPowerProfileSelected(int) {
    if (ui->cpuProfileDropdown->currentData() != CPU_PRESET_DEFAULT) {
        setInputEnabled(false);
        cpuProfileChanged = false;
    } else {
        setInputEnabled(true);
    }
    if (ui->cpuProfileDropdown->currentData() == CPU_PRESET_CURRENT) {
        auto cpuProfile = createPowerProfileFromCurrentCpuState();
        fillCpuProfileData(cpuProfile);
    } else {
        cpuProfileChanged = false;
        auto cpuProfile = ui->cpuProfileDropdown->currentData().value<CpuProfile>();
        fillCpuProfileData(cpuProfile);
    }
}

void CpuTab::setInputEnabled(bool enabled) {
    ui->stapmLimitSetting->setEnabled(enabled);
    ui->stapmTimeSetting->setEnabled(enabled);
    ui->slowLimitSetting->setEnabled(enabled);
    ui->slowTimeSetting->setEnabled(enabled);
    ui->fastLimitSetting->setEnabled(enabled);
    ui->modeSetting->setEnabled(enabled);
}

void CpuTab::fillCpuProfileData(const CpuProfile &profile) {
    fromDropdown = true;
    ui->stapmLimitSetting->setValue(profile.getStapmLimit());
    ui->stapmTimeSetting->setValue(profile.getStapmTime());
    ui->slowLimitSetting->setValue(profile.getSlowLimit());
    ui->slowTimeSetting->setValue(profile.getSlowTime());
    ui->fastLimitSetting->setValue(profile.getFastLimit());
    ui->modeSetting->setCurrentIndex(ui->modeSetting->findData(profile.getMode()));
    fromDropdown = false;
}

void CpuTab::loadCurrentCpuProfile() {
    fillCpuProfileData(createPowerProfileFromCurrentCpuState());
}

CpuProfile CpuTab::createPowerProfileFromData(const QString &name) {
    CpuProfile result(name,
                      (float) ui->fastLimitSetting->value(),
                      (float) ui->slowLimitSetting->value(),
                      (float) ui->slowTimeSetting->value(),
                      (float) ui->stapmLimitSetting->value(),
                      (float) ui->stapmTimeSetting->value(),
                      ui->modeSetting->currentData().value<SetPoint>());
    return result;
}

void CpuTab::saveCurrentCpuProfile(const QString &name) {
    CpuProfile profile = createPowerProfileFromData(name);
    //profile.setName(name);

    settingsStorage->saveCpuProfile(profile);

    loadCpuProfiles();

    int index = ui->cpuProfileDropdown->findText(name);
    ui->cpuProfileDropdown->setCurrentIndex(index);
}

void CpuTab::onDeleteProfileClicked(bool) {
//    if (ui->cpuProfileDropdown->currentData() != -1 && ui->cpuProfileDropdown->currentData() != 0) {
//        auto currentData = CpuProfile::fromQStringList(ui->cpuProfileDropdown->currentText(),
//                                                       ui->cpuProfileDropdown->currentData()
//                                                           .value<QStringList>());
//        SETT.deletePowerProfile(currentData.getName());
//        loadCpuProfiles();
//        onPowerProfileSelected(ui->cpuProfileDropdown->currentIndex());
//    }
}

void CpuTab::onSaveClicked(bool) {
    bool profileNameOk;
    QString profileName = QInputDialog::getText(this,
                                                tr("Save CPU profile"),
                                                tr("Profile name:"),
                                                QLineEdit::Normal,
                                                nullptr,
                                                &profileNameOk);
    if (profileNameOk && !profileName.isEmpty()) {
        bool profileExists = settingsStorage->cpuProfileExists(profileName);
        if (profileExists) {
            QMessageBox confirmOverride;
            confirmOverride.setText("CPU profile with this name already exists");
            confirmOverride.setInformativeText("Do you want to override it?");
            confirmOverride.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            confirmOverride.setDefaultButton(QMessageBox::Ok);
            int confirmOverrideResult = confirmOverride.exec();

            if (confirmOverrideResult == QMessageBox::Ok) {
                saveCurrentCpuProfile(profileName);
            }
        } else {
            saveCurrentCpuProfile(profileName);
        }
    }
}

void CpuTab::onApplyClicked(bool) {
    if (ui->cpuProfileDropdown->currentData() == CPU_PRESET_DEFAULT) {
        serviceController->reapplyArCratePlan();
    } else {
        auto cpuProfile = createPowerProfileFromData("");
        serviceController->applyCpuProfile(cpuProfile);
    }
}

void CpuTab::setSelected(bool selected) {
    if (selected) {
        qTimer->start();
    } else {
        qTimer->stop();
    }
}

void CpuTab::onPowerProfileChanged(int) {

}

CpuProfile CpuTab::createPowerProfileFromCurrentCpuState() {
    CpuStatus currentCpuStatus;
    serviceController->getCpuStatus(currentCpuStatus);
    CpuProfile currentCpuProfile("",
                              currentCpuStatus.stapmLimit,
                              currentCpuStatus.stapmTime,
                              currentCpuStatus.slowLimit,
                              currentCpuStatus.slowTime,
                              currentCpuStatus.fastLimit,
                              currentCpuStatus.setPoint == 95.0f ? SP_POWER_SAVING : SP_PERFORMANCE
                              );
    return currentCpuProfile;
}
