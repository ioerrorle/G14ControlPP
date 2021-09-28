#include "QMainWindow"
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QTimer>
#include <QStyle>
#include <src/ui/mainTab/MainTab.h>
#include <src/ui/fansTab/FansTab.h>

#ifndef G14CONTROLPP_MAINWINDOW_H
#define G14CONTROLPP_MAINWINDOW_H

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent * event) override;

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon         * trayIcon;
    MainTab *mainTab;
    FansTab *fansTab;

    void refreshMainTab();

private slots:
    /* The slot that will accept the signal from the event
     * Click on the application icon in the system tray
     */
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

};


#endif //G14CONTROLPP_MAINWINDOW_H
