#ifndef ARMOURYCRATEWIDGET_H
#define ARMOURYCRATEWIDGET_H

#include <QWidget>

#include "G14ArmouryCrate/g14armourycrate.h"

namespace Ui {
class ArmouryCrateWidget;
}

class ArmouryCrateWidget : public QWidget
{
    Q_OBJECT

public:
    static const QMap<G14ArmouryCratePlan, QString> NAMED_ARMOURY_CRATE_PLANS;

    explicit ArmouryCrateWidget(QWidget *parent = nullptr);
    ~ArmouryCrateWidget();

    void setCurrentArCratePlan(const G14ArmouryCratePlan plan);

private:
    Ui::ArmouryCrateWidget *ui;

signals:
    void changeArCratePlanRequested(const G14ArmouryCratePlan plan);
};

#endif // ARMOURYCRATEWIDGET_H
