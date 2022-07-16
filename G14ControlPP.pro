QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#        src/controller/acpi/acpicontroller.cpp \
#        src/controller/acpi/acpilistenerthread.cpp \
#        src/controller/nativeEvent/nativeeventcontroller.cpp \
        src/controller/servicecontroller.cpp \
#        src/controller/keyboard/keyboardcontroller.cpp \
        src/controller/cpu/cpucontroller.cpp \
        src/main.cpp \
#        src/ryzenadj/RyzenAdjSingleton.cpp \
    src/model/collections.cpp \
        src/storage/settingsstorage.cpp \
        src/ui/MainWindow.cpp \
        src/ui/cpuFansTab/cpufanstab.cpp \
#        src/ui/cpuTab/CpuTab.cpp \
#        src/ui/fansTab/FansTab.cpp \
#        src/controller/keyboard/kbdlistenerthread.cpp \
#        src/settings/Settings.cpp \
#        src/events/GlobalEventDispatcher.cpp \
#        src/audio/AudioUtils.cpp \
#        src/settings/Settings.cpp \
#        src/ui/batteryTab/BatteryTab.cpp \
#        src/ui/hotkeys/HotkeysTab.cpp \
#        src/ui/powerPlans/PowerPlansTab.cpp \
#        src/settings/PowerPlan.cpp \
#        src/settings/PowerPlanSet.cpp \
        src/ui/notificationHelper/NotificationHelper.cpp \
#        src/ui/hotkeys/HotkeyAction.cpp \
#        src/ui/hotkeys/HotkeysProfile.cpp \
        src/ui/cpuFansTab/fanCurveView/FanCurveView.cpp \
        src/ui/cpuFansTab/fanCurveView/Node.cpp \
        src/ui/cpuFansTab/fanCurveView/Edge.cpp

HEADERS += \
#        src/controller/acpi/acpicontroller.h \
#        src/controller/acpi/acpilistenerthread.h \
#        src/controller/nativeEvent/nativeeventcontroller.h \
#        src/controller/servicecontroller.h \
#        src/controller/keyboard/keyboardcontroller.h \
#        src/controller/keyboard/kbdlistenerthread.h \
        src/controller/cpu/cpucontroller.h \
        src/controller/cpu/RyzenAdjTypes.h \
        src/storage/settingsstorage.h \
        src/ui/MainWindow.h \
        src/ui/cpuFansTab/cpufanstab.h \
#        src/ui/cpuTab/CpuTab.h \
#        src/ui/fansTab/FansTab.h \
#        src/settings/Settings.h \
#        src/events/GlobalEventDispatcher.h \
#        src/audio/AudioUtils.h \
#        src/settings/Settings.h \
#        src/ui/batteryTab/BatteryTab.h \
#        src/ui/hotkeys/HotkeysTab.h \
#        src/ui/powerPlans/PowerPlansTab.h \
        src/model/CpuProfile.h \
#        src/settings/PowerPlan.h \
#        src/settings/PowerPlanSet.h \
        src/ui/notificationHelper/NotificationHelper.h \
#        src/ui/hotkeys/HotkeyAction.h \
#        src/ui/hotkeys/HotkeysProfile.h \
#        src/ui/base/BaseTab.h \
        src/ui/cpuFansTab/fanCurveView/FanCurveView.h \
        src/ui/cpuFansTab/fanCurveView/Node.h \
        src/ui/cpuFansTab/fanCurveView/Edge.h \
        src/model/HwProfile.h \
        src/model/PowerProfile.h \
        src/model/FanCurve.h \
        src/model/ArmouryCratePlan.h \
        src/model/FansProfile.h
FORMS += \
        src/ui/MainWindow.ui \
        src/ui/cpuFansTab/cpufanstab.ui
#        src/ui/fansTab/FansTab.ui \
#        src/ui/cpuTab/CpuTab.ui \
#        src/ui/batteryTab/BatteryTab.ui \
#        src/ui/powerPlans/PowerPlansTab.ui \
#        src/ui/hotkeys/HotkeysTab.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
