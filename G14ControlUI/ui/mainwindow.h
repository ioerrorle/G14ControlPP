#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller/maincontroller.h"
#include "ui/powerProfileTab/powerprofiletab.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    PowerProfileTab *m_powerProfileTab = nullptr;

private:
    Ui::MainWindow *ui;

    MainController *m_mainController = nullptr;

private slots:
    void onAppStateUpdate(const AppState &appState);
};
#endif // MAINWINDOW_H
