#ifndef G14CONTROLPP_HOTKEYSTAB_H
#define G14CONTROLPP_HOTKEYSTAB_H

#include <QDebug>
#include <QInputDialog>
#include <QKeyEvent>
#include <QWidget>
#include <QComboBox>
#include <QMessageBox>
#include "src/settings/Settings.h"
#include "src/ui/base/BaseTab.h"
#include "HotkeysProfile.h"

namespace Ui {
    class HotkeysTab;
}

class HotkeysTab : public QWidget, public BaseTab {
Q_OBJECT

public:
    explicit HotkeysTab(QWidget *parent = nullptr);

    void setSelected(bool selected) override;

private:

    Ui::HotkeysTab *ui;
    QVector<QComboBox *> *comboBoxes;

    bool hotkeysProfileChanged = false;

    void loadProfile(const HotkeysProfile &hotkeysProfile);

    void setDropdown(QComboBox *dropdown, const HotkeyAction &hotkeyAction, bool setIndex = true);

    void dropdownSelected(QComboBox *dropdown, int index);

    void loadProfiles(bool selectCurrentProfile = true);

    void setHotkeysProfileChanged(bool value);

    int customButtonDialog(quint32 *scancodePtr, quint32 *modifiersPtr);

    HotkeysProfile createProfileFromUi();

    HotkeyAction createActionFromDropdown(QComboBox *dropdown);

    bool saveHotkeysProfile(const QString &name, bool override);

private slots:

    void onProfileSelected(int index);

    void onSaveProfileClicked(bool selected = false);

    void onDeleteProfileClicked(bool selected = false);

    void onApplyClicked(bool selected = false);
};

class KeyCatchMessageBox : public QMessageBox {
public:
    KeyCatchMessageBox(quint32 *scancodePtr, quint32 *modifiersPtr, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private:
    quint32 *scancodePtr;
    quint32 *modifiersPtr;
};


#endif //G14CONTROLPP_HOTKEYSTAB_H
