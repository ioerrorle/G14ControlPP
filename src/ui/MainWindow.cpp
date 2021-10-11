#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->cpuTab = new CpuTab();
    this->fansTab = new FansTab();
    this->batteryTab = new BatteryTab();

    ui->centralwidget->addTab(cpuTab, tr("CPU"));
    ui->centralwidget->addTab(fansTab, tr("Fans"));
    ui->centralwidget->addTab(batteryTab, tr("Battery"));

    QTimer *timer = new QTimer(this);
    timer->setInterval(250);
    connect(timer, &QTimer::timeout, this, &MainWindow::refreshMainTab);
    timer->start();

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("G14ControlPP");
    /* After that create a context menu of two items */
    QMenu *menu = new QMenu(this);
    QAction *viewWindow = new QAction(trUtf8("Maximize"), this);
    QAction *quitAction = new QAction(trUtf8("Quit"), this);

    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(closeTrayAction()));

    menu->addAction(viewWindow);
    menu->addAction(quitAction);

    trayIcon->setContextMenu(menu);
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (this->isVisible() && !closeActionTriggered) {
        event->ignore();
        this->hide();
        //QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

        /*trayIcon->showMessage("Tray Program",
                              trUtf8("Приложение свернуто в трей. Для того чтобы, "
                                     "развернуть окно приложения, щелкните по иконке приложения в трее"),
                              icon,
                              2000);*/
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::Trigger:

            if (!this->isVisible()) {
                this->show();
            }
            if (!this->isActiveWindow()) {
                this->activateWindow();
            }
            break;
        default:
            break;
    }
}

void MainWindow::refreshMainTab() {
    if(this->isVisible()) {
        if (((QTabWidget *) centralWidget())->currentWidget() == cpuTab && this->isVisible()) {
            cpuTab->refresh();
        }
        if (((QTabWidget *) centralWidget())->currentWidget() == fansTab && this->isVisible()) {
            fansTab->refresh();
        }
    }
}

void MainWindow::closeTrayAction() {
    this->closeActionTriggered = true;
    this->close();
}
