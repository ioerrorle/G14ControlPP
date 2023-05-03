#include "ryzencontroller.h"

RyzenController::RyzenController(QObject *parent)
    : QObject{parent}
{
    m_timer = new QTimer(this);
    m_timer->setInterval(TABLE_REFRESH_INTERVAL_MS);
    connect(m_timer, &QTimer::timeout, this, &RyzenController::onTimerTimeout);
}

RyzenController::~RyzenController()
{
    cleanup_ryzenadj(m_ryzenAccess);
}

bool RyzenController::init(QString &error)
{
    m_ryzenAccess = init_ryzenadj();
    if (!m_ryzenAccess) {
        error = tr("Unable to init RyzenAdj");
        return false;
    }

    int err = init_table(m_ryzenAccess);
    if (err) {
        error = tr("Unable to init power metric table: %1").arg(err);
        return false;
    }

    err = refresh_table(m_ryzenAccess);
    if (err) {
        error = tr("Unable to refresh power metric table: %1").arg(err);
        return false;
    }

    m_init = true;
    return true;
}

void RyzenController::startWatching()
{
    m_timer->start();
}

void RyzenController::stopWatching()
{
    m_timer->stop();
}

bool RyzenController::getCpuState(CpuState &cpuState, QString &error)
{
    if (!m_init) {
        error = tr("RyzenAdj is not initialized");
        return false;
    }

    int err = refresh_table(m_ryzenAccess);

    if (err) {
        error = tr("Unable to refresh power metric table: table: %1").arg(err);
        return false;
    }

    cpuState.stapmLimit = get_stapm_limit(m_ryzenAccess);
    cpuState.stapmTime = get_stapm_time(m_ryzenAccess);
    cpuState.slowLimit = get_slow_limit(m_ryzenAccess);
    cpuState.slowTime = get_slow_time(m_ryzenAccess);
    cpuState.fastLimit = get_fast_limit(m_ryzenAccess);
    cpuState.mode = get_cclk_setpoint(m_ryzenAccess) == 95.0 ? CpuMode::PowerSaving : CpuMode::Performance;
    cpuState.coreTempLimit = getCoreTempLimit(m_ryzenAccess);
    cpuState.coreTemp = getCoreTemp(m_ryzenAccess);
    cpuState.apuTempLimit = getApuTempLimit(m_ryzenAccess);
    cpuState.apuTemp = getApuTemp(m_ryzenAccess);
    cpuState.dGpuTempLimit = getDGpuTempLimit(m_ryzenAccess);
    cpuState.dGpuTemp = getDGpuTemp(m_ryzenAccess);

    return true;
}

bool RyzenController::setCpuProfile(const CpuProfile &cpuProfile, QString &error)
{
    if (!m_init) {
        error = tr("RyzenAdj is not initialized");
        return false;
    }

    int err = 0;

    if ((err = set_stapm_limit(m_ryzenAccess, (uint32_t)(1000 * cpuProfile.stapmLimit)))) {
        error = tr("Unable to set CPU parameter STAPM limit: %1").arg(err);
        return false;
    }
    if ((err = set_stapm_time(m_ryzenAccess, (uint32_t)cpuProfile.stapmTime))) {
        error = tr("Unable to set CPU parameter STAPM time: %1").arg(err);
        return false;
    }
    if ((err = set_slow_limit(m_ryzenAccess, (uint32_t)(1000 * cpuProfile.slowLimit)))) {
        error = tr("Unable to set CPU parameter Slow limit: %1").arg(err);
        return false;
    }
    if ((err = set_slow_time(m_ryzenAccess, (uint32_t)cpuProfile.slowTime))) {
        error = tr("Unable to set CPU parameter Slow time: %1").arg(err);
        return false;
    }
    if ((err = set_fast_limit(m_ryzenAccess, (uint32_t)(1000 * cpuProfile.fastLimit)))) {
        error = tr("Unable to set CPU parameter Fast limit: %1").arg(err);
        return false;
    }

    if (cpuProfile.mode == CpuMode::Performance)
        if ((err = set_max_performance(m_ryzenAccess))) {
            error = tr("Unable to set CPU parameter Setpoint: %1").arg(err);
            return false;
        }
    if (cpuProfile.mode == CpuMode::PowerSaving)
        if ((err = set_power_saving(m_ryzenAccess))) {
            error = tr("Unable to set CPU parameter Setpoint: %1").arg(err);
            return false;
        }

    return true;
}

void RyzenController::onTimerTimeout()
{
    CpuState cpuState;
    QString err;
    if (!getCpuState(cpuState, err)) {
        emit onCpuStateError(tr("Unable to get CPU state: %1").arg(err));
        return;
    }
    emit onCpuStateChanged(cpuState);
}

float RyzenController::getValueByOffset(ryzen_access ry, ushort offset) {
    return get_table_values(ry)[offset >> 2];
}

float RyzenController::getCoreTempLimit(ryzen_access ry) {
    return getValueByOffset(ry, 0x40);
}

float RyzenController::getApuTempLimit(ryzen_access ry) {
    return getValueByOffset(ry, 0x48);
}

float RyzenController::getDGpuTempLimit(ryzen_access ry) {
    return getValueByOffset(ry, 0x50);
}

float RyzenController::getCoreTemp(ryzen_access ry) {
    return getValueByOffset(ry, 0x44);
}

float RyzenController::getApuTemp(ryzen_access ry) {
    return getValueByOffset(ry, 0x4C);
}

float RyzenController::getDGpuTemp(ryzen_access ry) {
    return getValueByOffset(ry, 0x54);
}
