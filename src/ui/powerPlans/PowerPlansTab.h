#ifndef G14CONTROLPP_POWERPLANSTAB_H
#define G14CONTROLPP_POWERPLANSTAB_H

#include <QWidget>
#include "src/settings/Settings.h"


namespace Ui {
    class PowerPlansTab;
}

class PowerPlansTab : public QWidget {
    Q_OBJECT

public:
    explicit PowerPlansTab(QWidget *parent = nullptr);

private:
    Ui::PowerPlansTab *ui;

    void loadProfiles();

    void loadSettings(PowerSourceType powerSourceType);
    void loadSettings();

    PowerPlan createPowerPlan(PowerSourceType powerSourceType);

public slots:
    void saveSettings(bool checked = false);
};


#endif //G14CONTROLPP_POWERPLANSTAB_H
