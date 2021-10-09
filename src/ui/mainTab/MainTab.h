#ifndef G14CONTROLPP_MAINTAB_H
#define G14CONTROLPP_MAINTAB_H

#include <QtCore/QArgument>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QTimer>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <src/ryzenadj/RyzenAdjSingleton.h>

namespace Ui {
    class MainTab;
}

class MainTab : public QWidget
{
    Q_OBJECT

public:
    explicit MainTab(QWidget *parent = nullptr);
    void refresh();
private:
    Ui::MainTab *ui;
    void loadPowerProfiles();
};

#endif //G14CONTROLPP_MAINTAB_H
