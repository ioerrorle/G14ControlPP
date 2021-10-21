#ifndef G14CONTROLPP_POWERPLANSTAB_H
#define G14CONTROLPP_POWERPLANSTAB_H

#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include "src/settings/Settings.h"
#include "src/ui/base/BaseTab.h"
#include "src/ryzenadj/RyzenAdjSingleton.h"
#include "src/events/GlobalEventDispatcher.h"

namespace Ui {
    class PowerPlansTab;
}

class PowerPlansTab : public QWidget, public BaseTab {
Q_OBJECT

public:
    explicit PowerPlansTab(QWidget *parent = nullptr);

    void setSelected(bool selected) override;

private:
    Ui::PowerPlansTab *ui;
    bool powerPlanSetChanged;

    void loadProfiles();

    void loadSettings(PowerSourceType powerSourceType, PowerPlan &powerPlan);
//
//    void loadSettings();

    PowerPlan createPowerPlan(PowerSourceType powerSourceType);

    void selectSavedPowerPlan(QString &text);

    void loadPowerPlans();

    bool savePowerPlanSet(QString &name, bool override);

public slots:

    void onSavePowerPlanSetClicked(bool checked = false);
    void onApplyAndSaveClicked(bool checked = false);
    void powerPlanSetSelected(int index);
    void powerPlanSetActivated(int index);
    void deleteSelectedPlanSet(bool checked = false);
    void addPowerPlanToUsed(bool checked = false);
    void removePowerPlanFromUsed(bool checked = false);
};


#endif //G14CONTROLPP_POWERPLANSTAB_H
