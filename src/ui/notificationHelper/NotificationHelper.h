#ifndef G14CONTROLPP_NOTIFICATIONHELPER_H
#define G14CONTROLPP_NOTIFICATIONHELPER_H

#include <QString>
#include <QtWidgets>

class NotificationHelper {
public:
    static NotificationHelper &getInstance();

private:
    NotificationHelper();

    QSystemTrayIcon *trayIcon;

public:
    void init(QSystemTrayIcon *trayIcon);

    void showMessage(QString &title, QString &text, QSystemTrayIcon::MessageIcon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information));
};


#endif //G14CONTROLPP_NOTIFICATIONHELPER_H
