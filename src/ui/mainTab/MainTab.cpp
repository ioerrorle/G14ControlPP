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
        stapmLimitLabel->setText(QString::asprintf("STAPM Limit: %.1f (%.1f)", RY.getStapmLimit(), RY.getStapmValue()));
                //tr("STAPM Limit: ") + qnm(RY.getStapmLimit()) + tr(" (") + qnm(RY.getStapmValue()) + tr(")"));
        fastLimitLabel->setText(QString::asprintf("Fast Limit: %.1f (%.1f)", RY.getFastLimit(), RY.getFastValue()));
                //tr("Fast Limit: ") + qnm(RY.getFastLimit()) + tr(" (") + qnm(RY.getFastValue()) + tr(")"));
        slowLimitLabel->setText(QString::asprintf("Slow Limit: %.1f (%.1f)", RY.getSlowLimit(), RY.getSlowValue()));
                //tr("Slow limit: ") + qnm(RY.getSlowLimit()) + tr(" (") + qnm(RY.getSlowValue()) + tr(")"));
}

#undef qnm