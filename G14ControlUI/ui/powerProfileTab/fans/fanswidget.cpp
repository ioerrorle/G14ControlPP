#include "fanswidget.h"
#include "ui_fanswidget.h"

FansWidget::FansWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FansWidget)
{
    ui->setupUi(this);
}

FansWidget::~FansWidget()
{
    delete ui;
}
