#ifndef KBDCONTROLLER_H
#define KBDCONTROLLER_H

#include "kbdlistenerthread.h"
#include "hidcontrolcodes.h"

#include <QObject>
#include <windows.h>
#include <initguid.h>
#include <hidclass.h>
#include <cfgmgr32.h>

typedef BOOLEAN (__stdcall *HidD_SetFeature_)(HANDLE handle, PVOID data, ULONG length);
static HidD_SetFeature_ HidD_SetFeature;

class KbdController : public QObject
{
    Q_OBJECT
public:
    explicit KbdController(QObject *parent = nullptr);
    ~KbdController();
    bool init(QString &error);
    void toggleTouchpad();
    void setKbdBrightness(uchar brightness);

private slots:
    void handleKbdFnPress(const uchar &keycode);
    void handleKbdError(const QString &error);

private:
    static const uchar REQUIRED_DATA_SIZE = 0x40;

    HANDLE m_kbdHandle = NULL;
    KbdListenerThread *m_kbdListenerThread = nullptr;

    bool sendHidControl(const uint32_t command, const uint32_t arg, QString &error);
signals:

};

#endif // KBDCONTROLLER_H
