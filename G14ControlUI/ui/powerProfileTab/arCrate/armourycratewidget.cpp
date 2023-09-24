#include "armourycratewidget.h"
#include "ui_armourycratewidget.h"

const QMap<G14ArmouryCratePlan, QString> ArmouryCrateWidget::NAMED_ARMOURY_CRATE_PLANS = {
    {G14ArmouryCratePlan::WindowsPerfomance, tr("Windows/Performance")},
    {G14ArmouryCratePlan::TurboManual, tr("Turbo/Manual")},
    {G14ArmouryCratePlan::Silent, tr("Silent")},
};

ArmouryCrateWidget::ArmouryCrateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArmouryCrateWidget)
{
    ui->setupUi(this);

    connect(ui->applyPlan, &QToolButton::clicked, this, [=]{
        G14ArmouryCratePlan plan = G14ArmouryCratePlan(ui->plansDropdown->currentData().value<uint64_t>());
        emit changeArCratePlanRequested(plan);
    });

    //filling the dropdown
    for (const G14ArmouryCratePlan &plan : NAMED_ARMOURY_CRATE_PLANS.keys()) {
        ui->plansDropdown->addItem(NAMED_ARMOURY_CRATE_PLANS[plan], (uint64_t)plan);
    }
}

ArmouryCrateWidget::~ArmouryCrateWidget()
{
    delete ui;
}

void ArmouryCrateWidget::setCurrentArCratePlan(const G14ArmouryCratePlan plan)
{
    if (NAMED_ARMOURY_CRATE_PLANS.contains(plan))
        ui->currentPlanLb->setText(NAMED_ARMOURY_CRATE_PLANS[plan]);
}
