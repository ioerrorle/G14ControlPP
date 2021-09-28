#ifndef G14CONTROLPP_FANSTAB_H
#define G14CONTROLPP_FANSTAB_H

#include <QtCore/QArgument>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <src/atkacpi/FanControlSingleton.h>
#include <src/ryzenadj/RyzenAdjSingleton.h>

class FansTab : public QWidget
{
    Q_OBJECT

public:
    explicit FansTab(QWidget *parent = nullptr);
    void refresh();

private:
    QLabel *gpuRPM;
    QLabel *cpuRPM;
};


#endif //G14CONTROLPP_FANSTAB_H
