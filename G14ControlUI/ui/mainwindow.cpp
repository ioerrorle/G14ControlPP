#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mainController(new MainController(this))
{
    ui->setupUi(this);

    m_powerProfileTab = new PowerProfileTab(this);
    ui->tabWidget->addTab(m_powerProfileTab, tr("Power profile"));

    connect(m_mainController, &MainController::appStateUpdated, this, &MainWindow::onAppStateUpdate);

    m_mainController->requestAppState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAppStateUpdate(const AppState &appState)
{
    m_powerProfileTab->setAppState(appState);
}

