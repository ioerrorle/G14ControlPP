#ifndef FANSWIDGET_H
#define FANSWIDGET_H

#include <QWidget>

namespace Ui {
class FansWidget;
}

class FansWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FansWidget(QWidget *parent = nullptr);
    ~FansWidget();

private:
    Ui::FansWidget *ui;
};

#endif // FANSWIDGET_H
