#ifndef RYZENCONTROLLER_H
#define RYZENCONTROLLER_H

#include <QObject>
#include <QTimer>
#include "CpuProfile/cpuprofile.h"

#include <ryzenadj.h>


class RyzenController : public QObject
{
    Q_OBJECT
public:
    RyzenController(QObject *parent = nullptr);
    ~RyzenController();
    bool init(QString &error);
    void startWatching();
    void stopWatching();

    bool getCpuState(CpuState &cpuState, QString &error);
    bool setCpuProfile(const CpuProfile &cpuProfile, QString &error);

signals:
    void onCpuStateChanged(const CpuState &CpuState);
    void onCpuStateError(const QString &error);

private slots:
    void onTimerTimeout();

private:
    static const int TABLE_REFRESH_INTERVAL_MS = 500;

    ryzen_access m_ryzenAccess;
    bool m_init = false;

    QTimer *m_timer = nullptr;

    static float getValueByOffset(ryzen_access ry, ushort offset);
    static float getCoreTempLimit(ryzen_access ry);
    static float getApuTempLimit(ryzen_access ry);
    static float getDGpuTempLimit(ryzen_access ry);
    static float getCoreTemp(ryzen_access ry);
    static float getApuTemp(ryzen_access ry);
    static float getDGpuTemp(ryzen_access ry);
};

#endif // RYZENCONTROLLER_H
