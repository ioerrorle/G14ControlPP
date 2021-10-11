#include "CpuTab.h"
#include "ui_CpuTab.h"

CpuTab::CpuTab(QWidget *parent)
        : QWidget(parent), ui(new Ui::CpuTab) {
    ui->setupUi(this);

    ui->deleteProfile->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));

    connect(ui->powerProfileDropdown, QOverload<int>::of(&QComboBox::activated), this, &CpuTab::onPowerProfileSelected);
    connect(ui->deleteProfile, &QPushButton::clicked, this, &CpuTab::onDeleteProfileClicked);

    loadPowerProfiles();
    loadCurrentPowerProfile();
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

void CpuTab::onDeleteProfileClicked(bool checked) {

}

void CpuTab::onSaveClicked(bool checked) {

}

void CpuTab::onApplyClicked(bool checked) {

}
