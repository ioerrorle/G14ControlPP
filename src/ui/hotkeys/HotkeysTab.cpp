#include "HotkeysTab.h"
#include "ui_HotkeysTab.h"

HotkeysTab::HotkeysTab(QWidget *parent) : QWidget(parent), ui(new Ui::HotkeysTab) {
    ui->setupUi(this);

    ui->deleteProfile->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->saveProfile->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));

    QList<QComboBox *> dropdowns{
            ui->mute,
            ui->rog,
            ui->fan,
            ui->lcdBrD,
            ui->lcdBrU,
            ui->touchpad,
            ui->sleep,
            ui->plane,
            ui->cButton,
            ui->vButton,
            ui->kbdBrD,
            ui->kbdBrU,
            ui->left,
            ui->right
    };

    for (QComboBox *dropdown : dropdowns) {
        for (uchar key : HOTKEY_ACTION_LIST.keys()) {
            dropdown->addItem(HOTKEY_ACTION_LIST[key], key);
        }
        connect(dropdown, QOverload<int>::of(&QComboBox::activated), this,
                [=](int index) { dropdownSelected(dropdown, index); });
    }

    connect(ui->kbdProfile, QOverload<int>::of(&QComboBox::activated), this, &HotkeysTab::onProfileSelected);
    connect(ui->saveProfile, &QPushButton::clicked, this, &HotkeysTab::onSaveProfileClicked);
    connect(ui->deleteProfile, &QPushButton::clicked, this, &HotkeysTab::onDeleteProfileClicked);
    connect(ui->apply, &QPushButton::clicked, this, &HotkeysTab::onApplyClicked);

    loadProfiles();
}

void HotkeysTab::loadProfiles(bool selectCurrentProfile) {
    ui->kbdProfile->clear();
    auto hotkeyProfiles = SETT.getHotkeysProfiles();
    for (HotkeysProfile profile : hotkeyProfiles) {
        ui->kbdProfile->addItem(profile.name, QVariant::fromValue(profile));
    }
    auto currentProfile = SETT.getCurrentHotkeysProfile();
    auto currentProfileVariant = QVariant::fromValue(currentProfile);
    int index = ui->kbdProfile->findData(currentProfileVariant);
    ui->kbdProfile->setCurrentIndex(index);
    if (selectCurrentProfile) {
        loadProfile(ui->kbdProfile->currentData().value<HotkeysProfile>());
    }
}

void HotkeysTab::loadProfile(const HotkeysProfile &hotkeysProfile) {
    setDropdown(ui->mute, hotkeysProfile.mute);
    setDropdown(ui->rog, hotkeysProfile.rog);
    setDropdown(ui->fan, hotkeysProfile.fan);
    setDropdown(ui->lcdBrD, hotkeysProfile.lcdBrD);
    setDropdown(ui->lcdBrU, hotkeysProfile.lcdBrU);
    setDropdown(ui->touchpad, hotkeysProfile.touchpad);
    setDropdown(ui->sleep, hotkeysProfile.sleep);
    setDropdown(ui->plane, hotkeysProfile.plane);
    setDropdown(ui->cButton, hotkeysProfile.cButton);
    setDropdown(ui->vButton, hotkeysProfile.vButton);
    setDropdown(ui->kbdBrD, hotkeysProfile.kbdBrD);
    setDropdown(ui->kbdBrU, hotkeysProfile.kbdBrU);
    setDropdown(ui->left, hotkeysProfile.leftButton);
    setDropdown(ui->right, hotkeysProfile.rightButton);
}

void HotkeysTab::setDropdown(QComboBox *dropdown, const HotkeyAction &hotkeyAction, bool setIndex) {
    if (setIndex)
        dropdown->setCurrentIndex(dropdown->findData(hotkeyAction.action));
    int row, col, rowSpan, colSpan;
    int idx = ui->gridLayout->indexOf(dropdown);
    ui->gridLayout->getItemPosition(idx, &row, &col, &rowSpan, &colSpan);
    if (hotkeyAction.action != HOTKEY_ACTION_BUTTON) {
        //check if there is a label
        auto item = ui->gridLayout->itemAtPosition(row, col + 1);
        ui->gridLayout->removeItem(item);
        if (item != nullptr) {
            delete item->widget();
        }
        delete item;
    } else {
        QLabel *label;
        auto item = ui->gridLayout->itemAtPosition(row, col + 1);
        if (item == nullptr) {
            label = new QLabel();
            ui->gridLayout->addWidget(label, row, col + 1);
        } else {
            label = dynamic_cast<QLabel *>(item->widget());
        }
        dropdown->setItemData(dropdown->currentIndex(), hotkeyAction.data, Qt::UserRole + 1);
        dropdown->setItemData(dropdown->currentIndex(), hotkeyAction.modifiers, Qt::UserRole + 2);
        int size = 50;
        CHAR buffer[size];
        int res = GetKeyNameTextA(hotkeyAction.data<<16, &buffer[0], size);
        label->setText(buffer);
    }
}

void HotkeysTab::dropdownSelected(QComboBox *dropdown, int index) {
    //get data
    auto data = dropdown->itemData(index).value<uchar>();

    //check if "button" option selected
    if (data == HOTKEY_ACTION_BUTTON) {
        //show button wait dialog
        quint32 scancode;
        quint32 modifiers;
        int result = customButtonDialog(&scancode, &modifiers);
        if (result == QDialog::Accepted) {
            HotkeyAction action = {HOTKEY_ACTION_BUTTON, scancode, modifiers};
            dropdown->setItemData(index, scancode, Qt::UserRole + 1);
            dropdown->setItemData(index, modifiers, Qt::UserRole + 2);
            setDropdown(dropdown, action, false);
        } else {
            HotkeyAction action = {HOTKEY_ACTION_NONE, 0x0, 0x0};
            setDropdown(dropdown, action);
        }
    } else {
        setDropdown(dropdown, {data, 0x0, 0x0}, false);
    }
    setHotkeysProfileChanged(true);
}

int HotkeysTab::customButtonDialog(quint32 *scancodePtr, quint32 *modifiersPtr) {
    KeyCatchMessageBox msgBox(scancodePtr, modifiersPtr);
    msgBox.setWindowIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    msgBox.setWindowTitle("Press a button");
    msgBox.setText(
            "Press a button to use for this hotkey.\n If your keyboard doesn't have desired button, use on-screen keyboard (Win+R -> type \"osk\" -> Enter).\nPress \"Shift+Esc\" to cancel");
    msgBox.setStandardButtons(QMessageBox::NoButton);
    return msgBox.exec();
}

void HotkeysTab::setHotkeysProfileChanged(bool value) {
    if (value) {
        hotkeysProfileChanged = true;
        //find "noname" profile
        int index = ui->kbdProfile->findData(0);
        if (index == -1) {
            //add "noname" profile
            index = 0;
            ui->kbdProfile->insertItem(index, "<noname>", 0);
        }
        ui->kbdProfile->setCurrentIndex(0);
    } else {
        hotkeysProfileChanged = false;
        //find "noname" profile
        int index = ui->kbdProfile->findData(0);
        //remove it
        if (index != -1) {
            ui->kbdProfile->removeItem(index);
        }
    }
}

void HotkeysTab::setSelected(bool selected) {

}

void HotkeysTab::onProfileSelected(int index) {
    if (ui->kbdProfile->currentData() != 0) {
        loadProfile(ui->kbdProfile->itemData(index).value<HotkeysProfile>());
        setHotkeysProfileChanged(false);
    }
}

KeyCatchMessageBox::KeyCatchMessageBox(quint32 *scancodePtr, quint32 *modifiersPtr, QWidget *parent) : QMessageBox(
        parent) {
    this->scancodePtr = scancodePtr;
    this->modifiersPtr = modifiersPtr;
}

void KeyCatchMessageBox::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key::Key_Escape) {
        reject();
    }
}

void KeyCatchMessageBox::keyReleaseEvent(QKeyEvent *event) {
    *this->scancodePtr = event->nativeScanCode();
    *this->modifiersPtr = event->nativeModifiers();
    accept();
}

void HotkeysTab::onSaveProfileClicked(bool selected) {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Save hotkeys"),
                                         tr("Profile name:"), QLineEdit::Normal,
                                         nullptr, &ok);
    if (ok) {
        if (!text.isEmpty() && text != "<noname>") {
            bool result = saveHotkeysProfile(text, false);
            if (!result) {
                QMessageBox msgBox;
                msgBox.setText("Hotkeys profile with this name already exists");
                msgBox.setInformativeText("Do you want to override it?");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Ok);
                int ret = msgBox.exec();

                if (ret == QMessageBox::Ok) {
                    bool result = saveHotkeysProfile(text, true);
                } else {
                    //it's either cancel or something strange
                }
            }
        } else {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error");
            msgBox.setText("Invalid profile name");
            msgBox.setInformativeText("Cannot use this name to save profile");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
}

bool HotkeysTab::saveHotkeysProfile(const QString &name, bool override) {
    HotkeysProfile profile = createProfileFromUi();
    profile.name = name;
    bool result = SETT.saveHotkeysProfile(profile, override);
    if (result) {
        SETT.setCurrentHotkeysProfileName(name);
        loadProfiles();
    }

    return result;
}

HotkeysProfile HotkeysTab::createProfileFromUi() {
    HotkeysProfile result = {
            "",
            createActionFromDropdown(ui->mute),
            createActionFromDropdown(ui->rog),
            createActionFromDropdown(ui->fan),
            createActionFromDropdown(ui->lcdBrD),
            createActionFromDropdown(ui->lcdBrU),
            createActionFromDropdown(ui->touchpad),
            createActionFromDropdown(ui->sleep),
            createActionFromDropdown(ui->plane),
            createActionFromDropdown(ui->cButton),
            createActionFromDropdown(ui->vButton),
            createActionFromDropdown(ui->kbdBrD),
            createActionFromDropdown(ui->kbdBrU),
            createActionFromDropdown(ui->left),
            createActionFromDropdown(ui->right)
    };
    return result;
}

HotkeyAction HotkeysTab::createActionFromDropdown(QComboBox *dropdown) {
    uchar action = dropdown->currentData().value<uchar>();
    if (action != HOTKEY_ACTION_BUTTON) {
        return {action, 0, 0};
    } else {
        quint32 data = dropdown->currentData(Qt::UserRole + 1).value<quint32>();
        quint32 modifiers = dropdown->currentData(Qt::UserRole + 2).value<quint32>();
        HotkeyAction act = {action, data, modifiers};
        return act;
    }
}

void HotkeysTab::onDeleteProfileClicked(bool selected) {
    if (ui->kbdProfile->currentData() != 0) {
        SETT.deleteHotkeysProfile(ui->kbdProfile->currentText());
        loadProfiles();
    }
}

void HotkeysTab::onApplyClicked(bool selected) {
    if (ui->kbdProfile->currentData() != 0) {
        SETT.setCurrentHotkeysProfileName(ui->kbdProfile->currentText());
        loadProfiles();
    } else {
        onSaveProfileClicked();
    }
}
