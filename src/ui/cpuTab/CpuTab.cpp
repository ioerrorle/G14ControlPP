#include "CpuTab.h"
#include "ui_CpuTab.h"

CpuTab::CpuTab(QWidget *parent)
        : QWidget(parent), ui(new Ui::CpuTab) {
    ui->setupUi(this);

    ui->deleteProfile->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->save->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(ui->powerProfileDropdown, QOverload<int>::of(&QComboBox::activated), this, &CpuTab::onPowerProfileSelected);
    connect(ui->deleteProfile, &QPushButton::clicked, this, &CpuTab::onDeleteProfileClicked);
    connect(ui->save, &QPushButton::clicked, this, &CpuTab::onSaveClicked);
    connect(ui->apply, &QPushButton::clicked, this, &CpuTab::onApplyClicked);

    loadPowerProfiles();
    loadCurrentPowerProfile();

    qTimer = new QTimer(this);
    qTimer->setInterval(250);
    connect(qTimer, &QTimer::timeout, this, &CpuTab::refresh);
}

void CpuTab::refresh() {
    RY.refreshTable();
    ui->stapmLimit->setText(QString::asprintf("%.1f (%.1f)", RY.getStapmLimit(), RY.getStapmValue()));
    ui->stapmTime->setText(QString::asprintf("%.0f", RY.getStapmTime()));
    ui->fastLimit->setText(QString::asprintf("%.1f (%.1f)", RY.getFastLimit(), RY.getFastValue()));
    ui->slowLimit->setText(QString::asprintf("%.1f (%.1f)", RY.getSlowLimit(), RY.getSlowValue()));
    ui->slowTime->setText(QString::asprintf("%.0f", RY.getSlowTime()));

    ui->coreTemp->setText(QString::asprintf("%.1f (%.1f)", RY.getCoreTempLimit(), RY.getCoreTemp()));
    ui->apuTemp->setText(QString::asprintf("%.1f (%.1f)", RY.getApuTempLimit(), RY.getApuTemp()));
    ui->dGpuTemp->setText(QString::asprintf("%.1f (%.1f)", RY.getDGpuTempLimit(), RY.getDGpuTemp()));
    ui->mode->setText(RY.getCclkSetpoint() == 95.0f ? "Power saving" : "Performance");
}

void CpuTab::loadPowerProfiles() {
    auto currentPowerProfiles = SETT.getPowerProfiles();
    for (PowerProfile profile : currentPowerProfiles) {
        ui->powerProfileDropdown->addItem(profile.name, QVariant::fromValue(profile));
    }

    ui->modeSetting->addItem("Performance", SP_PERFORMANCE);
    ui->modeSetting->addItem("Power saving", SP_POWER_SAVING);
    ui->modeSetting->addItem("Default", SP_DEFAULT);
}

void CpuTab::onPowerProfileSelected(int index) {
    auto powerProfile = ui->powerProfileDropdown->itemData(index).value<PowerProfile>();
    fillPowerProfileData(powerProfile);
}

void CpuTab::fillPowerProfileData(PowerProfile &profile) {
    ui->stapmLimitSetting->setValue(profile.stapmLimit);
    ui->stapmTimeSetting->setValue(profile.stapmTime);
    ui->slowLimitSetting->setValue(profile.slowLimit);
    ui->slowTimeSetting->setValue(profile.slowTime);
    ui->fastLimitSetting->setValue(profile.fastLimit);
    ui->modeSetting->setCurrentIndex(ui->modeSetting->findData(profile.mode));
}

void CpuTab::loadCurrentPowerProfile() {
    auto currentProfile = SETT.getCurrentPowerProfile();
    fillPowerProfileData(currentProfile);
}

PowerProfile CpuTab::createPowerProfileFromData() {
    PowerProfile result = {};
    result.fastLimit = ui->fastLimitSetting->value();
    result.slowLimit = ui->slowLimitSetting->value();
    result.slowTime = ui->slowTimeSetting->value();
    result.stapmLimit = ui->stapmLimitSetting->value();
    result.stapmTime = ui->stapmTimeSetting->value();
    result.mode = ui->modeSetting->currentData().value<Setpoint>();
    return result;
}

bool CpuTab::saveCurrentPowerProfile(QString &name, bool override) {
    PowerProfile profile = createPowerProfileFromData();
    profile.name = name;

    bool result = SETT.savePowerProfile(profile, override);

    if (!result)
        return false;

    reloadPowerProfiles();
    selectPowerProfile(profile, true);
    return true;
}

void CpuTab::onDeleteProfileClicked(bool checked) {
    auto currentData = ui->powerProfileDropdown->currentData().value<PowerProfile>();
    SETT.deletePowerProfile(currentData);
    reloadPowerProfiles();
}

void CpuTab::onSaveClicked(bool checked) {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Save curves"),
                                         tr("Curve name:"), QLineEdit::Normal,
                                         nullptr, &ok);
    if (ok && !text.isEmpty()) {
        bool result = saveCurrentPowerProfile(text, false);
        if (!result) {
            QMessageBox msgBox;
            msgBox.setText("Power plan with this name already exists");
            msgBox.setInformativeText("Do you want to override it?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();

            if (ret == QMessageBox::Ok) {
                bool result = saveCurrentPowerProfile(text, true);
            } else {
                //it's either cancel or something strange
            }
        }

    }
}

void CpuTab::onApplyClicked(bool checked) {
    auto powerProfile = createPowerProfileFromData();
    RY.setPowerProfile(powerProfile);
}

void CpuTab::reloadPowerProfiles() {
    ui->powerProfileDropdown->clear();
    auto profiles = SETT.getPowerProfiles(true);
    for (PowerProfile &profile : profiles) {
        ui->powerProfileDropdown->addItem(profile.name, QVariant::fromValue(profile));
    }
}

void CpuTab::selectPowerProfile(PowerProfile &profile, bool selectIndex) {
    if (selectIndex) {
        int i = ui->powerProfileDropdown->findText(profile.name);
        if (i != -1) {
            ui->powerProfileDropdown->setCurrentIndex(i);
        }
    }
    fillPowerProfileData(profile);
}

void CpuTab::setSelected(bool selected) {
    if (selected) {
        qTimer->start();
    } else {
        qTimer->stop();
    }
}
