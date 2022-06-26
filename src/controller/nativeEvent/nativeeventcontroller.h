#ifndef NATIVEEVENTCONTROLLER_H
#define NATIVEEVENTCONTROLLER_H

#include <QAbstractNativeEventFilter>
#include <QObject>
#include <windows.h>

class NativeEventController : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    NativeEventController(QObject *parent = nullptr);
    bool init(HWND mainWindowHandle, QString &error);

    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *) Q_DECL_OVERRIDE;

private:
    const static GUID EventGUIDs[];
    void handlePowerCfgChange(POWERBROADCAST_SETTING *settings);
    void handleWakeUp();

signals:
    void lidStateChanged(UCHAR state);
    void powerSourceChanged(UCHAR powerSource);
    void wakeUpReceived();

};

#endif // NATIVEEVENTCONTROLLER_H
