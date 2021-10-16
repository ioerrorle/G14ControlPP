#include "NotificationHelper.h"

NotificationHelper &NotificationHelper::getInstance() {
    static NotificationHelper instance;
    return instance;
}

NotificationHelper::NotificationHelper() {

}

void NotificationHelper::init(QSystemTrayIcon *trayIcon) {
    this->trayIcon = trayIcon;
}

void NotificationHelper::showMessage(QString &title, QString &text, QSystemTrayIcon::MessageIcon icon) {
    trayIcon->showMessage(title,
                          text,
                          icon,
                          2000);
}
