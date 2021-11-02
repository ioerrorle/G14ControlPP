#include "BatteryTab.h"
#include "ui_BatteryTab.h"

BatteryTab::BatteryTab(QWidget *parent) : QWidget(parent), ui(new Ui::BatteryTab) {
    ui->setupUi(this);
    onSliderMoved(ui->batterySlider->value());

    connect(ui->batterySlider, &QSlider::sliderMoved, this, &BatteryTab::onSliderMoved);
    connect(ui->apply, &QPushButton::clicked, this, &BatteryTab::onApplyClicked);

    loadSettings();
}

void BatteryTab::onSliderMoved(int value) {
    ui->percentValue->setText(QString::asprintf("%d%%", value));
}

void BatteryTab::onApplyClicked(bool checked) {
    Settings::getInstance().putMaxBatteryCharge(ui->batterySlider->value());
    AcpiControlSingleton::getInstance().setMaxBatteryPercentage(ui->batterySlider->value());
}

void BatteryTab::loadSettings() {
    uchar maxBatteryCharge = Settings::getInstance().getMaxBatteryCharge();
    ui->batterySlider->setValue(maxBatteryCharge);
    AcpiControlSingleton::getInstance().setMaxBatteryPercentage(ui->batterySlider->value());
}

void BatteryTab::setSelected(bool selected) {

}