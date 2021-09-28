#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->mainTab = new MainTab();
    this->fansTab = new FansTab();

    ui->centralwidget->addTab(mainTab, tr("Main"));
    ui->centralwidget->addTab(fansTab, tr("Fans"));

    QTimer *timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &MainWindow::refreshMainTab);
    timer->start();

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("Tray Program" "\n"
                         "Работа со сворачиванием программы трей");
    /* After that create a context menu of two items */
    QMenu *menu = new QMenu(this);
    QAction *viewWindow = new QAction(trUtf8("Развернуть окно"), this);
    QAction *quitAction = new QAction(trUtf8("Выход"), this);

    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

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
    if (this->isVisible()) {
        event->ignore();
        this->hide();
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

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
            } else {
                this->hide();
            }
            break;
        default:
            break;
    }
}

void MainWindow::refreshMainTab() {
    if (((QTabWidget*)centralWidget())->currentWidget() == mainTab && this->isVisible()) {
        mainTab->refresh();
    }
    if (((QTabWidget*)centralWidget())->currentWidget() == fansTab && this->isVisible()) {
        fansTab->refresh();
    }
}
