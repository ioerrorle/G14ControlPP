#include "MainTab.h"

#define qnm(...) QString::number(__VA_ARGS__)

MainTab::MainTab(QWidget *parent)
        : QWidget(parent) {
    stapmLimitLabel = new QLabel();
    fastLimitLabel = new QLabel();
    slowLimitLabel = new QLabel();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stapmLimitLabel);
    mainLayout->addWidget(fastLimitLabel);
    mainLayout->addWidget(slowLimitLabel);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

void MainTab::refresh() {
        RY.refreshTable();
        stapmLimitLabel->setText(
                tr("STAPM Limit: ") + qnm(RY.getStapmLimit()) + tr(" (") + qnm(RY.getStapmValue()) + tr(")"));
        fastLimitLabel->setText(
                tr("Fast Limit: ") + qnm(RY.getFastLimit()) + tr(" (") + qnm(RY.getFastValue()) + tr(")"));
        slowLimitLabel->setText(
                tr("Slow limit: ") + qnm(RY.getSlowLimit()) + tr(" (") + qnm(RY.getSlowValue()) + tr(")"));
}

#undef qnm