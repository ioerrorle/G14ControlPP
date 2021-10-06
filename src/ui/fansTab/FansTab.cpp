#include "FansTab.h"

FansTab::FansTab(QWidget *parent) {

    //COMBO BOXES
    auto *comboBoxesLayout = new QHBoxLayout;

    auto *arCrateProfileLabel = new QLabel("Armoury Crate profile");
    this->arCrateProfileSelector = new QComboBox();
    this->defaultFanCurves = new QCheckBox("Use default fan curves");
    auto *fanCurveLabel = new QLabel("Fan curve");
    this->fanCurveComboBox = new QComboBox();

    comboBoxesLayout->addWidget(arCrateProfileLabel);
    comboBoxesLayout->addWidget(arCrateProfileSelector);
    comboBoxesLayout->addWidget(defaultFanCurves);
    comboBoxesLayout->addWidget(fanCurveLabel);
    comboBoxesLayout->addWidget(fanCurveComboBox);
    comboBoxesLayout->addStretch(1);

    auto *comboBoxes = new QWidget();
    comboBoxes->setLayout(comboBoxesLayout);

    //fill combo boxes
    for (ArmouryCratePowerPlan &plan : POWER_PLANS) {
        arCrateProfileSelector->addItem(plan.name, plan.powerPlan);
    }

    connect(defaultFanCurves, &QCheckBox::stateChanged, this, &FansTab::defaultFanCurvesChange);
    connect(fanCurveComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FansTab::onFanCurveIndexChanged);

    //CPU SECTION
    auto *cpuSectionLayout = new QHBoxLayout;

    for (int i = 0; i < 8; i++) {
        auto *sliderBox = new QWidget;
        auto *sliderBoxLayout = new QVBoxLayout;
        auto *tempValue = new QLabel(QString::number(35 + 10 * i) + tr("°C"));
        tempValue->setStyleSheet("QLabel { width : 4em }");
        auto *qSlider = new QSlider(Qt::Orientation::Vertical);
        qSlider->setMaximum(100);
        cpuSliders[i] = qSlider;
        sliderBoxLayout->addWidget(tempValue);
        sliderBoxLayout->addWidget(qSlider);
        sliderBoxLayout->addStretch(1);
        sliderBox->setLayout(sliderBoxLayout);
        cpuSectionLayout->addWidget(sliderBox);
    }
    cpuRPM = new QLabel(/*"CPU RPM: 2000"*/);
    cpuSectionLayout->addWidget(cpuRPM);
    cpuSectionLayout->addStretch(1);
    auto *cpuSection = new QGroupBox(tr("CPU"));
    cpuSection->setLayout(cpuSectionLayout);

    //GPU SECTION
    auto *gpuSectionLayout = new QHBoxLayout;

    for (int i = 0; i < 8; i++) {
        auto *sliderBox = new QWidget;
        auto *sliderBoxLayout = new QVBoxLayout;
        auto *tempValue = new QLabel(QString::number(35 + 10 * i) + tr("°C"));
        tempValue->setStyleSheet("QLabel { width : 4em }");
        auto *qSlider = new QSlider(Qt::Orientation::Vertical);
        qSlider->setMaximum(100);
        gpuSliders[i] = qSlider;
        sliderBoxLayout->addWidget(tempValue);
        sliderBoxLayout->addWidget(qSlider);
        sliderBoxLayout->addStretch(1);
        sliderBox->setLayout(sliderBoxLayout);
        gpuSectionLayout->addWidget(sliderBox);
    }

    gpuRPM = new QLabel(/*"GPU RPM: 2000"*/);

    gpuSectionLayout->addWidget(gpuRPM);

    gpuSectionLayout->addStretch(1);

    auto *gpuSection = new QGroupBox(tr("GPU"));
    gpuSection->setLayout(gpuSectionLayout);

    //BUTTONS SECTION
    auto *buttonsLayout = new QHBoxLayout;

    auto *saveButton = new QPushButton("Save");
    auto *applyButton = new QPushButton("Apply");

    connect(saveButton, &QPushButton::clicked, this, &FansTab::onSaveFanCurvesClicked);
    connect(applyButton, &QPushButton::clicked, this, &FansTab::onApplyClicked);

    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(applyButton);

    auto *buttonsSection = new QWidget();
    buttonsSection->setLayout(buttonsLayout);

    //ADD ALL

    auto *container = new QVBoxLayout;
    container->addWidget(comboBoxes);
    container->addWidget(cpuSection);
    container->addWidget(gpuSection);
    container->addStretch(1);
    container->addWidget(buttonsSection);

    setLayout(container);

    //
}

void FansTab::refresh() {
    RY.refreshTable();

    cpuRPM->setText(QString::asprintf("%.2f°C, %ld RPM",
                                      RY.getCpuTemp(),
                                      AcpiControlSingleton::getInstance().getCpuFanSpeed()));
    gpuRPM->setText(QString::number(AcpiControlSingleton::getInstance().getGpuFanSpeed()) + " RPM");
}

void FansTab::defaultFanCurvesChange(int state) {
    if (state) {
        this->fanCurveComboBox->clear();
        this->fanCurveComboBox->setEnabled(false);
    } else {
        this->fanCurveComboBox->setEnabled(true);
        reloadFanCurves();
    }
}

void FansTab::onSaveFanCurvesClicked(bool checked) {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Save curves"),
                                         tr("Curve name:"), QLineEdit::Normal,
                                         nullptr, &ok);
    if (ok && !text.isEmpty()) {
        FansProfile profile = {};
        profile.name = text;
        FanCurve cpuCurve = {};
        FanCurve gpuCurve = {};
        for (int i = 0; i < 8; i++) {
            cpuCurve.temp[i] = gpuCurve.temp[i] = 35 + i*10;
            cpuCurve.speed[i] = cpuSliders[i]->value();
            gpuCurve.speed[i] = gpuSliders[i]->value();
        }
        profile.cpu = cpuCurve;
        profile.gpu = gpuCurve;

        SETT.saveFansProfile(profile);
        reloadFanCurves();
        selectFanProfile(profile);
    }
}

void FansTab::reloadFanCurves() {
    if (fanCurveComboBox->isEnabled()) {
        this->fanCurveComboBox->clear();
        this->fanCurveComboBox->setEnabled(true);
        auto profiles = SETT.getFansProfiles();
        for(FansProfile &profile : profiles) {
            this->fanCurveComboBox->addItem(profile.name, QVariant::fromValue(profile));
        }
    }
}

void FansTab::selectFanProfile(FansProfile &profile) {
    this->fanCurveComboBox->setCurrentText(profile.name);
    for (int i = 0; i < 8; i++) {
        cpuSliders[i]->setValue(profile.cpu.speed[i]);
        gpuSliders[i]->setValue(profile.gpu.speed[i]);
    }
}

void FansTab::onFanCurveIndexChanged(int index) {
    if (fanCurveComboBox->isEnabled()) {
        auto profile = fanCurveComboBox->itemData(index).value<FansProfile>();
        selectFanProfile(profile);
    }
}

void FansTab::onApplyClicked(bool checked) {
    auto powerPlan = arCrateProfileSelector->itemData(arCrateProfileSelector->currentIndex()).value<ArmouryCratePowerPlan>();
    AcpiControlSingleton::getInstance().setPowerPlan(powerPlan.powerPlan);
    if (!defaultFanCurves->isChecked()) {
        FansProfile profile = {};
        profile.name = fanCurveComboBox->currentText();
        FanCurve cpuCurve = {};
        FanCurve gpuCurve = {};
        for (int i = 0; i < 8; i++) {
            cpuCurve.temp[i] = gpuCurve.temp[i] = 35 + i*10;
            cpuCurve.speed[i] = cpuSliders[i]->value();
            gpuCurve.speed[i] = gpuSliders[i]->value();
        }
        AcpiControlSingleton::fixFanCurve(FAN_CPU, cpuCurve);
        profile.cpu = cpuCurve;
        AcpiControlSingleton::fixFanCurve(FAN_GPU, gpuCurve);
        profile.gpu = gpuCurve;
        selectFanProfile(profile);
        AcpiControlSingleton::getInstance().setFanProfile(profile);
    }
}
