#ifndef POWERPROFILESTAB_H
#define POWERPROFILESTAB_H

#include "G14AppState/appstate.h"
#include "ui/powerProfileTab/arCrate/armourycratewidget.h"
#include "ui/powerProfileTab/cpu/cpuwidget.h"
#include "ui/powerProfileTab/fans/fanswidget.h"
#include <QWidget>

namespace Ui {
class PowerProfileTab;
}

class PowerProfileTab : public QWidget
{
    Q_OBJECT

public:
    explicit PowerProfileTab(QWidget *parent = nullptr);
    ~PowerProfileTab();

    void setAppState(const AppState &state);
private:
    Ui::PowerProfileTab *ui;

    ArmouryCrateWidget *m_arCrateWidget = nullptr;
    CpuWidget *m_cpuWidget = nullptr;
    FansWidget *m_fansWidget = nullptr;

    void addLayoutToBox(QWidget *widget, const QString &name);

};

#endif // POWERPROFILESTAB_H
