#include "cpufanstab.h"
#include "ui_cpufanstab.h"

#include <QInputDialog>
#include <QMessageBox>

CpuFansTab::CpuFansTab(const ServiceController *serviceController,
                       const SettingsStorage *settingsStorage,
                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CpuFansTab)
    , m_serviceController(serviceController)
    , m_settingsStorage(settingsStorage) {
    ui->setupUi(this);

    //adding fan curve widgets
    cpuFanEditor = new FanCurveView();
    gpuFanEditor = new FanCurveView();
    dynamic_cast<QGridLayout *>(ui->fansGroupBox->layout())->addWidget(cpuFanEditor, 1, 0);
    dynamic_cast<QGridLayout *>(ui->fansGroupBox->layout())->addWidget(gpuFanEditor, 1, 1);

    loadArCratePlans();

    ui->cpuProfileDelete->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->cpuProfileSave->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(ui->cpuProfileSave, &QPushButton::clicked, this, &CpuFansTab::onSaveCpuProfileClicked);
    connect(ui->cpuProfileDelete, &QPushButton::clicked, this, &CpuFansTab::onDeleteCpuProfileClicked);

    connect(ui->cpuStapmLimitInput, &QSpinBox::valueChanged, this, &CpuFansTab::onCpuSettingChanged);
    connect(ui->cpuStapmTimeInput, &QSpinBox::valueChanged, this, &CpuFansTab::onCpuSettingChanged);
    connect(ui->cpuSlowLimitInput, &QSpinBox::valueChanged, this, &CpuFansTab::onCpuSettingChanged);
    connect(ui->cpuSlowTimeInput, &QSpinBox::valueChanged, this, &CpuFansTab::onCpuSettingChanged);
    connect(ui->cpuFastLimitInput, &QSpinBox::valueChanged, this, &CpuFansTab::onCpuSettingChanged);
    loadCpuModes();
    connect(ui->cpuModeDropdown,
            &QComboBox::currentIndexChanged,
            this,
            &CpuFansTab::onCpuSettingChanged);
    connect(ui->cpuProfileDropdown,
            &QComboBox::activated,
            this,
            &CpuFansTab::onCpuProfileDropdownSelected);
    loadCpuProfiles();

    loadFansProfiles();
}

void CpuFansTab::setSelected(bool selected) {}

void CpuFansTab::cleanCpuProfileChangesIfAny() {
    if (currentCpuProfileChanged && changedCpuDropdownIndex != -1) {
        auto changedCpuDropdownText = ui->cpuProfileDropdown->itemText(changedCpuDropdownIndex);
        changedCpuDropdownText.truncate(changedCpuDropdownText.size() - 1); //removing asterisk
        ui->cpuProfileDropdown->setItemText(changedCpuDropdownIndex, changedCpuDropdownText);
    }
    currentCpuProfileChanged = false;
}

void CpuFansTab::loadCpuModes() {
    for (const Ryzen::Mode &cpuMode : Ryzen::Mode::Modes) {
        ui->cpuModeDropdown->addItem(cpuMode.name(), (int)cpuMode);
    }
}

void CpuFansTab::loadArCratePlans() {
    for (const ArCrate::Plan &plan : ArCrate::Plan::List) {
        ui->arCrateDropdown->addItem(plan.name(), (int)plan);
    }
}

void CpuFansTab::loadCpuProfiles() {
    ui->cpuProfileDropdown->clear();

    ui->cpuProfileDropdown->addItem(Ryzen::Profile::Current.name(), (int)Ryzen::Profile::Current.type());
    ui->cpuProfileDropdown->addItem(Ryzen::Profile::Default.name(), (int)Ryzen::Profile::Default.type());
    auto cpuProfiles = m_settingsStorage->getCpuProfiles();
    for (const Ryzen::Profile &cpuProfile : cpuProfiles) {
        ui->cpuProfileDropdown->addItem(cpuProfile.name(), QVariant::fromValue(cpuProfile));
    }
}

void CpuFansTab::setCpuSettingsEnabled(bool enabled) {
    ui->cpuStapmLimitInput->setEnabled(enabled);
    ui->cpuStapmTimeInput->setEnabled(enabled);
    ui->cpuSlowLimitInput->setEnabled(enabled);
    ui->cpuSlowTimeInput->setEnabled(enabled);
    ui->cpuFastLimitInput->setEnabled(enabled);
    ui->cpuModeDropdown->setEnabled(enabled);

    if (!enabled) {
        ui->cpuFastLimitInput->clear();
        ui->cpuSlowTimeInput->clear();
        ui->cpuSlowLimitInput->clear();
        ui->cpuStapmTimeInput->clear();
        ui->cpuStapmLimitInput->clear();
        ui->cpuModeDropdown->setCurrentIndex(-1);
    }
}

void CpuFansTab::setCpuSettingsFromCurrentValues() {
    Ryzen::Status currentCpuStatus = m_serviceController->getCpuStatus();
    setCpuSettingsFromCpuProfile(currentCpuStatus);
}

void CpuFansTab::setCpuSettingsFromCpuProfile(const Ryzen::Profile &profile) {
    ui->cpuStapmLimitInput->setValue(profile.stapmLimit);
    ui->cpuStapmTimeInput->setValue(profile.stapmTime);
    ui->cpuSlowLimitInput->setValue(profile.slowLimit);
    ui->cpuSlowTimeInput->setValue(profile.slowTime);
    ui->cpuFastLimitInput->setValue(profile.fastLimit);
    ui->cpuModeDropdown->setCurrentIndex((int)profile.mode());
}

Ryzen::Profile CpuFansTab::saveCurrentCpuProfile(const QString &name) const
{
    Ryzen::Profile currentCpuProfile(name,
                                 ui->cpuStapmLimitInput->value(),
                                 ui->cpuStapmTimeInput->value(),
                                 ui->cpuSlowLimitInput->value(),
                                 ui->cpuSlowTimeInput->value(),
                                 ui->cpuFastLimitInput->value(),
                                 ui->cpuModeDropdown->currentData().value<int>());

    m_settingsStorage->saveCpuProfile(currentCpuProfile);
    return currentCpuProfile;
}

void CpuFansTab::onCpuProfileDropdownSelected(int index) {
    //nothing selected
    if (index == -1) {
        changedCpuDropdownIndex = index;
        return;
    }
    auto currentData = ui->cpuProfileDropdown->currentData();
    if (currentData.canConvert<int>()) {
        ui->cpuProfileDelete->setEnabled(false);
        //means it's either "current" or "default"
        if (currentData.value<int>() == (int)Ryzen::Profile::Type::Default) {
            ui->cpuProfileSave->setEnabled(false);
            setCpuSettingsEnabled(false);
        } else if (currentData.value<int>() == (int)Ryzen::Profile::Type::Current) {
            ui->cpuProfileSave->setEnabled(true);
            setCpuSettingsEnabled(true);
            setCpuSettingsFromCurrentValues();
        }
    } else {
        //it's user defined, getting cpu profile from qvariant
        ui->cpuProfileDelete->setEnabled(true);
        ui->cpuProfileSave->setEnabled(true);
        const Ryzen::Profile &selectedCpuProfile = currentData.value<Ryzen::Profile>();
        setCpuSettingsEnabled(true);
        setCpuSettingsFromCpuProfile(selectedCpuProfile);
    }
    cleanCpuProfileChangesIfAny();
}

void CpuFansTab::onCpuSettingChanged(int indexOrValue) {
    Q_UNUSED(indexOrValue)
    if (!currentCpuProfileChanged && ui->cpuProfileDropdown->currentIndex() != -1) {
        changedCpuDropdownIndex = ui->cpuProfileDropdown->currentIndex();
        auto currentCpuDropdownText = ui->cpuProfileDropdown->currentText();
        ui->cpuProfileDropdown->setItemText(changedCpuDropdownIndex, currentCpuDropdownText + "*");
    }
    currentCpuProfileChanged = true;
}

void CpuFansTab::onSaveCpuProfileClicked(bool)
{
    bool cpuProfileNameOk;
    QString cpuProfileName = QInputDialog::getText(this,
                                                tr("Save CPU profile"),
                                                tr("Profile name:"),
                                                QLineEdit::Normal,
                                                nullptr,
                                                &cpuProfileNameOk);
    if (!cpuProfileNameOk || cpuProfileName.isEmpty())
        return;

    bool profileExists = m_settingsStorage->cpuProfileExists(cpuProfileName);


    bool overrideProfile = QMessageBox::Yes == (profileExists ? QMessageBox::question(this,
                          tr("CPU profile with this name already exists"),
                          tr("Do you want to override it?")) : QMessageBox::Yes);

    if (!overrideProfile)
        return;

    cleanCpuProfileChangesIfAny();
    auto savedProfile = saveCurrentCpuProfile(cpuProfileName);
    if (profileExists) {
        //update data
        int profileIndex = ui->cpuProfileDropdown->findText(savedProfile.name());
        Q_ASSERT(profileIndex != -1);
        ui->cpuProfileDropdown->setItemData(profileIndex, QVariant::fromValue(savedProfile));
    } else
        //add it to the dropdown
        ui->cpuProfileDropdown->addItem(savedProfile.name(), QVariant::fromValue(savedProfile));
    //select it from the dropdown
    ui->cpuProfileDropdown->blockSignals(true);
    ui->cpuProfileDropdown->setCurrentText(savedProfile.name());
    ui->cpuProfileDropdown->blockSignals(false);
}

void CpuFansTab::onDeleteCpuProfileClicked(bool)
{
    int currentIndex = ui->cpuProfileDropdown->currentIndex();
    Ryzen::Profile currentProfile = ui->cpuProfileDropdown->currentData().value<Ryzen::Profile>();
    Q_ASSERT(currentProfile.type() == Ryzen::Profile::Type::UserDefined);
    bool confirmDelete = QMessageBox::Yes == (QMessageBox::question(this,
                          tr("Confirm deleting CPU profile"),
                          tr("Do you want to delete profile %1%2?")
                                .arg(currentProfile.name())
                                .arg(currentCpuProfileChanged ? tr("(unsaved)") : "")));
    if (!confirmDelete)
        return;

    m_settingsStorage->deleteCpuProfile(currentProfile.name());
    int defaultProfileIndex = ui->cpuProfileDropdown->findData((int)Ryzen::Profile::Type::Default);
    ui->cpuProfileDropdown->setCurrentIndex(defaultProfileIndex);
    onCpuProfileDropdownSelected(defaultProfileIndex);
    ui->cpuProfileDropdown->removeItem(currentIndex);
}

void CpuFansTab::loadFansProfiles()
{

}
