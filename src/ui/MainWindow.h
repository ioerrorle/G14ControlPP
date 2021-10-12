#ifndef G14CONTROLPP_MAINWINDOW_H
#define G14CONTROLPP_MAINWINDOW_H

#include "QMainWindow"
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QTimer>
#include <QStyle>
#include <src/ui/powerPlans/PowerPlansTab.h>
#include <src/ui/cpuTab/CpuTab.h>
#include <src/ui/fansTab/FansTab.h>
#include <src/ui/batteryTab/BatteryTab.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    PowerPlansTab *powerPlansTab;
    CpuTab *cpuTab;
    FansTab *fansTab;
    BatteryTab *batteryTab;

    bool closeActionTriggered = false;

    void refreshMainTab();

private slots:

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void closeTrayAction();
};


#endif //G14CONTROLPP_MAINWINDOW_H
