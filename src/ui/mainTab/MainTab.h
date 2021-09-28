#ifndef G14CONTROLPP_MAINTAB_H
#define G14CONTROLPP_MAINTAB_H

#include <QtCore/QArgument>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QTimer>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <src/ryzenadj/RyzenAdjSingleton.h>

class MainTab : public QWidget
{
    Q_OBJECT

public:
    explicit MainTab(QWidget *parent = nullptr);
    void refresh();
private:
    QLabel *stapmLimitLabel;
    QLabel *fastLimitLabel;
    QLabel *slowLimitLabel;
};

#endif //G14CONTROLPP_MAINTAB_H
