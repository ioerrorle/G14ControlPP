#include "settingsstorage.h"

#include <QDir>
#include <QStandardPaths>

const QString SettingsStorage::DEFAULT_SETTINGS_PATH = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QDir::separator() + "settings.ini");

SettingsStorage::SettingsStorage(QObject *parent)
    : QObject{parent}
{
    m_settings = new QSettings(DEFAULT_SETTINGS_PATH, QSettings::Format::IniFormat, this);

}
