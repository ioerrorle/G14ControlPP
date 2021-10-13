#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->powerPlansTab = new PowerPlansTab();
    this->cpuTab = new CpuTab();
    this->fansTab = new FansTab();
    this->batteryTab = new BatteryTab();

    ui->centralwidget->addTab(powerPlansTab, tr("Power plans"));
    ui->centralwidget->addTab(cpuTab, tr("CPU"));
    ui->centralwidget->addTab(fansTab, tr("Fans"));
    ui->centralwidget->addTab(batteryTab, tr("Battery"));

    connect(ui->centralwidget, &QTabWidget::currentChanged, this, &MainWindow::onIndexChanged);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("G14ControlPP");
    /* After that create a context menu of two items */
    auto *menu = new QMenu(this);
    auto *viewWindow = new QAction("Maximize", this);
    auto *quitAction = new QAction("Quit", this);

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
        onIndexChanged(-1);
        this->hide();
        //QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

        /*trayIcon->showMessage("Tray Program",
                              trUtf8("Приложение свернуто в трей. Для того чтобы, "
                                     "развернуть окно приложения, щелкните по иконке приложения в трее"),
                              icon,
                              2000);*/
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    onIndexChanged(ui->centralwidget->currentIndex());
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

void MainWindow::closeTrayAction() {
    this->closeActionTriggered = true;
    this->close();
}

void MainWindow::onIndexChanged(int index) {
    for (int i = 0; i < ui->centralwidget->count(); i++) {
        auto tab = dynamic_cast<BaseTab *>(ui->centralwidget->widget(i));
        tab->setSelected(i == index);
    }
}
