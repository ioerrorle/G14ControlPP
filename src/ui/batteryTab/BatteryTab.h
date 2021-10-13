#ifndef G14CONTROLPP_BATTERYTAB_H
#define G14CONTROLPP_BATTERYTAB_H

#include <QtWidgets/QWidget>
#include "src/settings/Settings.h"
#include "src/ui/base/BaseTab.h"

namespace Ui {
    class BatteryTab;
}

class BatteryTab : public QWidget, public BaseTab {
Q_OBJECT

public:
    explicit BatteryTab(QWidget *parent = nullptr);

    void setSelected(bool selected);

private:

    Ui::BatteryTab *ui;

    void loadSettings();

public slots:

    void onSliderMoved(int value);

    void onApplyClicked(bool checked = false);

};


#endif //G14CONTROLPP_BATTERYTAB_H
