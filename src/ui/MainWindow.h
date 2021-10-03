#ifndef G14CONTROLPP_MAINWINDOW_H
#define G14CONTROLPP_MAINWINDOW_H

#include "QMainWindow"
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QTimer>
#include <QStyle>
#include <src/ui/mainTab/MainTab.h>
#include <src/ui/fansTab/FansTab.h>

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
    MainTab *mainTab;
    FansTab *fansTab;

    bool closeActionTriggered = false;

    void refreshMainTab();

private slots:

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void closeTrayAction();
};


#endif //G14CONTROLPP_MAINWINDOW_H
