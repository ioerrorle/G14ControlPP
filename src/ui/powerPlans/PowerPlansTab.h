#ifndef G14CONTROLPP_POWERPLANSTAB_H
#define G14CONTROLPP_POWERPLANSTAB_H

#include <QWidget>

namespace Ui {
    class PowerPlansTab;
}

class PowerPlansTab : public QWidget {
    Q_OBJECT

public:
    explicit PowerPlansTab(QWidget *parent = nullptr);

private:
    Ui::PowerPlansTab *ui;
};


#endif //G14CONTROLPP_POWERPLANSTAB_H
