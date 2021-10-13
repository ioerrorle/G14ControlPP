#include "PowerPlansTab.h"
#include "ui_PowerPlansTab.h"

PowerPlansTab::PowerPlansTab(QWidget *parent) : QWidget(parent), ui(new Ui::PowerPlansTab) {
    ui->setupUi(this);

    loadProfiles();
    loadSettings();

    connect(ui->save, &QPushButton::clicked, this, &PowerPlansTab::saveSettings);
}

void PowerPlansTab::loadProfiles() {
    ui->dcPowerProfile->clear();
    ui->dcFanProfile->clear();
    ui->dcPlan->clear();
    ui->acPowerProfile->clear();
    ui->acFanProfile->clear();
    ui->acPlan->clear();
    ui->usbPowerProfile->clear();
    ui->acFanProfile->clear();
    ui->acPlan->clear();

    ui->dcPowerProfile->addItem("Default", 0);
    ui->acPowerProfile->addItem("Default", 0);
    ui->usbPowerProfile->addItem("Default", 0);
    auto powerProfiles = SETT.getPowerProfiles();
    for (PowerProfile &profile : powerProfiles) {
        ui->dcPowerProfile->addItem(profile.name, QVariant::fromValue(profile));
        ui->acPowerProfile->addItem(profile.name, QVariant::fromValue(profile));
        ui->usbPowerProfile->addItem(profile.name, QVariant::fromValue(profile));
    }

    ui->dcFanProfile->addItem("Default", 0);
    ui->acFanProfile->addItem("Default", 0);
    ui->usbFanProfile->addItem("Default", 0);
    auto fanProfiles = SETT.getFansProfiles();
    for (FansProfile &profile : fanProfiles) {
        ui->dcFanProfile->addItem(profile.name, QVariant::fromValue(profile));
        ui->acFanProfile->addItem(profile.name, QVariant::fromValue(profile));
        ui->usbFanProfile->addItem(profile.name, QVariant::fromValue(profile));
    }

    for (ArmouryCratePlan &plan : ARMOURY_CRATE_PLANS) {
        ui->dcPlan->addItem(plan.name, plan.id);
        ui->acPlan->addItem(plan.name, plan.id);
        ui->usbPlan->addItem(plan.name, plan.id);
    }
}

void PowerPlansTab::loadSettings(PowerSourceType powerSourceType) {
    QComboBox *arcPlan;
    QComboBox *fanProfile;
    QComboBox *powerProfile;

    switch (powerSourceType) {
        case POWER_SOURCE_BATTERY:
            arcPlan = ui->dcPlan;
            fanProfile = ui->dcFanProfile;
            powerProfile = ui->dcPowerProfile;
            break;
        case POWER_SOURCE_180W:
            arcPlan = ui->acPlan;
            fanProfile = ui->acFanProfile;
            powerProfile = ui->acPowerProfile;
            break;
        case POWER_SOURCE_USB:
            arcPlan = ui->usbPlan;
            fanProfile = ui->usbFanProfile;
            powerProfile = ui->usbPowerProfile;
            break;
    }

    auto powerPlan = SETT.loadPowerPlanForPowerSource(powerSourceType);
    int index = arcPlan->findData(powerPlan.armouryCratePlanId);
    if (index != -1) {
        arcPlan->setCurrentIndex(index);
    }

    if (powerPlan.fansProfile.name.isEmpty()) {
        fanProfile->setCurrentIndex(fanProfile->findData(0));
    } else {
        index = fanProfile->findData(QVariant::fromValue(powerPlan.fansProfile));
        if (index != -1) {
            fanProfile->setCurrentIndex(index);
        }
    }

    if (powerPlan.powerProfile.name.isEmpty()) {
        powerProfile->setCurrentIndex(fanProfile->findData(0));
    } else {
        index = powerProfile->findData(QVariant::fromValue(powerPlan.powerProfile));
        if (index != -1) {
            powerProfile->setCurrentIndex(index);
        }
    }
}

void PowerPlansTab::loadSettings() {
    loadSettings(POWER_SOURCE_BATTERY);
    loadSettings(POWER_SOURCE_180W);
    loadSettings(POWER_SOURCE_USB);
}

PowerPlan PowerPlansTab::createPowerPlan(PowerSourceType powerSourceType) {
    QComboBox *arcPlan;
    QComboBox *fanProfile;
    QComboBox *powerProfile;

    switch (powerSourceType) {
        case POWER_SOURCE_BATTERY:
            arcPlan = ui->dcPlan;
            fanProfile = ui->dcFanProfile;
            powerProfile = ui->dcPowerProfile;
            break;
        case POWER_SOURCE_180W:
            arcPlan = ui->acPlan;
            fanProfile = ui->acFanProfile;
            powerProfile = ui->acPowerProfile;
            break;
        case POWER_SOURCE_USB:
            arcPlan = ui->usbPlan;
            fanProfile = ui->usbFanProfile;
            powerProfile = ui->usbPowerProfile;
            break;
    }

    PowerPlan plan = {};
    plan.armouryCratePlanId = arcPlan->currentData().value<uchar>();
    if (fanProfile->currentData() == 0) {
        plan.fansProfile = {};
    } else {
        plan.fansProfile = fanProfile->currentData().value<FansProfile>();
    }

    if (powerProfile->currentData() == 0) {
        plan.powerProfile = {};
    } else {
        plan.powerProfile = powerProfile->currentData().value<PowerProfile>();
    }

    return plan;
}

void PowerPlansTab::saveSettings(bool checked) {
    auto dc = createPowerPlan(POWER_SOURCE_BATTERY);
    auto ac = createPowerPlan(POWER_SOURCE_180W);
    auto usb = createPowerPlan(POWER_SOURCE_USB);

    SETT.savePowerPlans(dc, ac, usb);
}

void PowerPlansTab::setSelected(bool selected) {
    if (selected) {
        loadProfiles();
        loadSettings();
    }
}
