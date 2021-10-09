#include "MainTab.h"
#include "ui_MainTab.h"

MainTab::MainTab(QWidget *parent)
        : QWidget(parent), ui(new Ui::MainTab) {
    ui->setupUi(this);

    loadPowerProfiles();
}

void MainTab::refresh() {
        RY.refreshTable();
        ui->stapmLimit->setText(QString::asprintf("%.1f (%.1f)", RY.getStapmLimit(), RY.getStapmValue()));
        ui->stapmTime->setText(QString::asprintf("%.0f", RY.getStapmTime()));
        ui->fastLimit->setText(QString::asprintf("%.1f (%.1f)", RY.getFastLimit(), RY.getFastValue()));
        ui->slowLimit->setText(QString::asprintf("%.1f (%.1f)", RY.getSlowLimit(), RY.getSlowValue()));
        ui->slowTime->setText(QString::asprintf("%.0f", RY.getSlowTime()));
        ui->mode->setText(RY.getCclkSetpoint() == 95.0f ? "Power saving" : "Performance");
}

void MainTab::loadPowerProfiles() {

}
