#include "powerprofiletab.h"
#include "ui_powerprofiletab.h"

#include <QGroupBox>

PowerProfileTab::PowerProfileTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PowerProfileTab)
{
    ui->setupUi(this);

    m_arCrateWidget = new ArmouryCrateWidget();
    m_cpuWidget = new CpuWidget();
    m_fansWidget = new FansWidget();

    addLayoutToBox(m_arCrateWidget, tr("Armoury Crate profile"));
    addLayoutToBox(m_cpuWidget, tr("CPU profile"));
    addLayoutToBox(m_fansWidget, tr("Fans profile"));
}

PowerProfileTab::~PowerProfileTab()
{
    delete ui;
}

void PowerProfileTab::setAppState(const AppState &state)
{
    m_arCrateWidget->setCurrentArCratePlan(state.arCratePlan);
}

void PowerProfileTab::addLayoutToBox(QWidget *widget, const QString &name)
{
    QGroupBox *groupBox = new QGroupBox();
    groupBox->setTitle(name);
    QBoxLayout *groupBoxLayout = new QBoxLayout(QBoxLayout::BottomToTop);
    groupBox->setLayout(groupBoxLayout);
    groupBoxLayout->addWidget(widget);
    ui->verticalLayout->addWidget(groupBox);
}
