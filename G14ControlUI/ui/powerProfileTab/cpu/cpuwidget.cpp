#include "cpuwidget.h"
#include "ui_cpuwidget.h"

CpuWidget::CpuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CpuWidget)
{
    ui->setupUi(this);
}

CpuWidget::~CpuWidget()
{
    delete ui;
}
