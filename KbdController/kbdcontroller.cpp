#include "kbdcontroller.h"
#include "Win32Utils/win32error.h"
#include <QDebug>

KbdController::KbdController(QObject *parent)
    : QObject{parent}
{

}

KbdController::~KbdController()
{
    if (m_kbdListenerThread) {
        m_kbdListenerThread->quit();
    }
    if (m_kbdHandle)
        CloseHandle(m_kbdHandle);
}

bool KbdController::init(QString &error)
{
    auto libHandle = LoadLibrary(L"hid.dll");
    if (!libHandle) {
        QString errorDesc;
        DWORD lastError = GetLastError();
        Win32Utils::GetLastErrorAsString(lastError, errorDesc);
        error = QString("[KbdCtrlr] Unable to load hid.dll library: %1").arg(errorDesc);
        return false;
    }

    auto HidD_SetFeatureAddress = GetProcAddress(libHandle, "HidD_SetFeature");
    if (!HidD_SetFeatureAddress) {
        QString errorDesc;
        DWORD lastError = GetLastError();
        Win32Utils::GetLastErrorAsString(lastError, errorDesc);
        error = QString("[KbdCtrlr] Unable to resolve HidD_SetFeature proc address: %1").arg(errorDesc);
        return false;
    }
    HidD_SetFeature = (HidD_SetFeature_)HidD_SetFeatureAddress;

    unsigned long deviceInterfaceListLength = 0;

    CONFIGRET configReturnCode = CM_Get_Device_Interface_List_Size(&deviceInterfaceListLength, (LPGUID) &GUID_DEVINTERFACE_HID, NULL, 0);
    if (configReturnCode != CR_SUCCESS) {
        error = QString("Cannot exec CM_Get_Device_Interface_List_Size function, error code %1").arg(configReturnCode, 8, 16);
        return false;
    }

    wchar_t deviceInterfaceList[deviceInterfaceListLength * sizeof(WCHAR)];

    configReturnCode = CM_Get_Device_Interface_List((LPGUID) &GUID_DEVINTERFACE_HID,
                                          NULL,
                                          deviceInterfaceList,
                                          deviceInterfaceListLength,
                                          CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

    if (configReturnCode != CR_SUCCESS) {
        error = QString("Cannot exec CM_Get_Device_Interface_List function, error code %1").arg(configReturnCode, 8, 16);
        return false;
    }

    QStringList devices = QString::fromWCharArray(deviceInterfaceList, deviceInterfaceListLength - 1).split(
            QChar(0x00));

    for (int i = 0; i < devices.size(); i++) {
        if (devices.at(i).contains("VID_0B05")
            && devices.at(i).contains("PID_1866")
            && devices.at(i).contains("Col01")) {
            QString devicePath = devices.at(i);
            m_kbdHandle = CreateFile(
                        (const wchar_t *)devicePath.utf16(),
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_OVERLAPPED,
                        NULL
                );
            if (m_kbdHandle == INVALID_HANDLE_VALUE) {
                error = "Error creating kbd handle";
                return false;
            }
            break;
        }
    }

    if (m_kbdHandle == NULL) {
        error = "Couldn't find matching device";
        return false;
    }

    m_kbdListenerThread = new KbdListenerThread();
    if (!m_kbdListenerThread->init(m_kbdHandle, error)) {
        return false;
    }

    connect(m_kbdListenerThread, &KbdListenerThread::keyPressed, this, &KbdController::handleKbdFnPress);
    connect(m_kbdListenerThread, &KbdListenerThread::kbdError, this, &KbdController::handleKbdError);
    connect(m_kbdListenerThread, &QThread::finished, m_kbdListenerThread, &QThread::deleteLater);
    connect(m_kbdListenerThread, &QThread::finished, this, [=]() {
        m_kbdListenerThread = nullptr;
    });

    m_kbdListenerThread->start();
}

void KbdController::toggleTouchpad()
{
    QString error;
    if (!sendHidControl(HidControlCode::TOUCHPAD_TOGGLE, 0x0, error)) {
        qDebug() << error;
    }
}

void KbdController::setKbdBrightness(uchar brightness)
{
    QString error;

    if (brightness > 3) {
        brightness = 3;
    }

    if (!sendHidControl(HidControlCode::KBD_BKLT, brightness, error)) {
        qDebug() << error;
    }
}

void KbdController::handleKbdFnPress(const uchar &keycode)
{
    qDebug() << "Key pressed" << keycode;
}

void KbdController::handleKbdError(const QString &error)
{
    qDebug() << "Kbd error" << error;
}

bool KbdController::sendHidControl(const uint32_t command, const uint32_t arg, QString &error)
{
    if (!m_kbdHandle) {
        error = "Keyboard handle is not initialized";
        return false;
    }

    auto data = QByteArray::fromRawData((const char *)&command, sizeof(command));
    data.append(QByteArray::fromRawData((const char *)&arg, sizeof(arg)));
    data.append(REQUIRED_DATA_SIZE - data.size(), 0);

    WINBOOL result = HidD_SetFeature(m_kbdHandle, (PVOID)data.constData(), data.size());
    if (!result) {
        QString errorDesc = "Unknown";
        DWORD lastError = GetLastError();
        Win32Utils::GetLastErrorAsString(lastError, errorDesc);
        error = QString("[KbdCtrlr] Unable to set HID device feature: %1").arg(errorDesc);
        return false;
    }
}
