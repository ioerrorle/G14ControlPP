#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include <QObject>
#include <QSettings>


class SettingsStorage : public QObject
{
    Q_OBJECT
public:
    explicit SettingsStorage(QObject *parent = nullptr);


private:
    static const QString DEFAULT_SETTINGS_PATH;

    QSettings *m_settings = nullptr;


signals:

};

#endif // SETTINGSSTORAGE_H
