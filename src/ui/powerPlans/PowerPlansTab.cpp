#include "PowerPlansTab.h"
#include "ui_PowerPlansTab.h"

PowerPlansTab::PowerPlansTab(QWidget *parent) : QWidget(parent), ui(new Ui::PowerPlansTab) {
    ui->setupUi(this);

    ui->deletePlan->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->savePlan->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    loadProfiles();
    loadPowerPlans();

    connect(ui->deletePlan, &QPushButton::clicked, this, &PowerPlansTab::deleteSelectedPlanSet);
    connect(ui->powerPlan, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetSelected);
    connect(ui->savePlan, &QPushButton::clicked, this, &PowerPlansTab::onSavePowerPlanSetClicked);
    connect(ui->save, &QPushButton::clicked, this, &PowerPlansTab::onApplyAndSaveClicked);
    connect(ui->addToSwitch, &QPushButton::clicked, this, &PowerPlansTab::addPowerPlanToUsed);
    connect(ui->removeFromSwitch, &QPushButton::clicked, this, &PowerPlansTab::removePowerPlanFromUsed);


    connect(ui->dcPlan, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
    connect(ui->dcFanProfile, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
    connect(ui->dcPowerProfile, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
    connect(ui->acPlan, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
    connect(ui->acFanProfile, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
    connect(ui->acPowerProfile, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
    connect(ui->usbPlan, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
    connect(ui->usbFanProfile, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
    connect(ui->usbPowerProfile, QOverload<int>::of(&QComboBox::activated), this, &PowerPlansTab::powerPlanSetActivated);
}

void PowerPlansTab::loadPowerPlans() {
    ui->powerPlan->clear();
    ui->usedPowerPlans->clear();
    ui->unusedPowerPlans->clear();
    auto powerPlanSets = SETT.getPowerPlanSets(true);
    auto usedPowerPlans = SETT.getUsedPowerPlans(powerPlanSets);
    for (PowerPlanSet &powerPlanSet : powerPlanSets) {
        ui->powerPlan->addItem(powerPlanSet.name, QVariant::fromValue(powerPlanSet));
        if (usedPowerPlans.contains(powerPlanSet.name)) {
            new QListWidgetItem(powerPlanSet.name, ui->usedPowerPlans);
        } else {
            new QListWidgetItem(powerPlanSet.name, ui->unusedPowerPlans);
        }
    }
}

void PowerPlansTab::loadProfiles() {
    ui->dcPowerProfile->clear();
    ui->dcFanProfile->clear();
    ui->dcPlan->clear();
    ui->acPowerProfile->clear();
    ui->acFanProfile->clear();
    ui->acPlan->clear();
    ui->usbPowerProfile->clear();
    ui->usbFanProfile->clear();
    ui->usbPlan->clear();

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

void PowerPlansTab::onSavePowerPlanSetClicked(bool checked) {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Save power plan"),
                                         tr("Power plan name:"), QLineEdit::Normal,
                                         nullptr, &ok);
    if (ok && !text.isEmpty()) {
        bool result = savePowerPlanSet(text, false);
        if (!result) {
            QMessageBox msgBox;
            msgBox.setText("Power plan with this name already exists");
            msgBox.setInformativeText("Do you want to override it?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();

            if (ret == QMessageBox::Ok) {
                result = savePowerPlanSet(text, true);
                if (result) {
                    selectSavedPowerPlan(text);
                }
            } else {
                //it's either cancel or something strange
            }
        } else {
            selectSavedPowerPlan(text);
        }
    }
}

void PowerPlansTab::selectSavedPowerPlan(QString &text) {
    loadPowerPlans();
    int index = ui->powerPlan->findText(text);
    if (index != -1) {
        if (index != ui->powerPlan->currentIndex()) {
            ui->powerPlan->setCurrentIndex(index);
        }
        powerPlanSetSelected(index);
    }
}

bool PowerPlansTab::savePowerPlanSet(QString &name, bool override) {
    auto dc = createPowerPlan(POWER_SOURCE_BATTERY);
    auto ac = createPowerPlan(POWER_SOURCE_180W);
    auto usb = createPowerPlan(POWER_SOURCE_USB);

    PowerPlanSet powerPlanSet = {
            name,
            dc,
            ac,
            usb};

    return SETT.savePowerPlanSet(powerPlanSet, override);
}

void PowerPlansTab::setSelected(bool selected) {
    if (selected) {
        loadPowerPlans();
        loadProfiles();
    }
}

void PowerPlansTab::powerPlanSetSelected(int index) {
    if (ui->powerPlan->currentData() != -1) {
        int noname = ui->powerPlan->findData(-1);
        if (noname != -1) {
            ui->powerPlan->removeItem(noname);
        }
        powerPlanSetChanged = false;
        auto powerPlanSet = ui->powerPlan->currentData().value<PowerPlanSet>();
        loadSettings(POWER_SOURCE_BATTERY, powerPlanSet.dcPowerPlan);
        loadSettings(POWER_SOURCE_180W, powerPlanSet.acPowerPlan);
        loadSettings(POWER_SOURCE_USB, powerPlanSet.usbPowerPlan);
    }
}

void PowerPlansTab::loadSettings(PowerSourceType powerSourceType, PowerPlan &powerPlan) {
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

void PowerPlansTab::deleteSelectedPlanSet(bool checked) {
    auto selectedPowerPlanSet = ui->powerPlan->currentData().value<PowerPlanSet>();
    SETT.deletePowerPlanSet(selectedPowerPlanSet);
    loadPowerPlans();
}

void PowerPlansTab::addPowerPlanToUsed(bool checked) {
    auto selectedItems = ui->unusedPowerPlans->selectedItems();
    for (QListWidgetItem *item : selectedItems) {
        int row = ui->unusedPowerPlans->row(item);
        if (row != -1) {
            auto taken = ui->unusedPowerPlans->takeItem(row);
            ui->usedPowerPlans->addItem(taken);
        }
    }
}

void PowerPlansTab::removePowerPlanFromUsed(bool checked) {
    auto selectedItems = ui->usedPowerPlans->selectedItems();
    for (QListWidgetItem *item : selectedItems) {
        int row = ui->usedPowerPlans->row(item);
        if (row != -1) {
            auto taken = ui->usedPowerPlans->takeItem(row);
            ui->unusedPowerPlans->addItem(taken);
        }
    }
}

void PowerPlansTab::onApplyAndSaveClicked(bool checked) {
    if (ui->powerPlan->currentData() != -1) {
        int count = ui->usedPowerPlans->count();
        //if (count > 0) {
        QStringList fastSwitchProfiles;
        for (int i = 0; i < count; i++) {
            fastSwitchProfiles.append(ui->usedPowerPlans->item(i)->text());
        }
        SETT.setCurrentPowerPlanSetName(ui->powerPlan->currentText());
        SETT.setUsedPowerPlans(fastSwitchProfiles);
        GlobalEventDispatcher::getInstance().applyPowerPlanFromCurrentSet();

    } else {
        QMessageBox msgBox;
        msgBox.setWindowIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
        msgBox.setWindowTitle("Power plan has not been saved");
        msgBox.setText("Save power plan first to save and apply settings.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void PowerPlansTab::powerPlanSetActivated(int index) {
    powerPlanSetChanged = true;
    if (ui->powerPlan->currentData() != -1) {
        if (ui->powerPlan->findText("<noname>") != -1) {
            ui->powerPlan->setCurrentText("<noname>");
        } else {
            ui->powerPlan->insertItem(0, "<noname>", -1);
            ui->powerPlan->setCurrentIndex(0);
        }
    }
}
