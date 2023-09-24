#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QWidget>

namespace Ui {
class CpuWidget;
}

class CpuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CpuWidget(QWidget *parent = nullptr);
    ~CpuWidget();

private:
    Ui::CpuWidget *ui;
};

#endif // CPUWIDGET_H
