#include "RyzenAdjSingleton.h"

RyzenAdjSingleton &RyzenAdjSingleton::getInstance() {
    static RyzenAdjSingleton instance;
    return instance;
}

RyzenAdjSingleton::RyzenAdjSingleton() {
}

bool RyzenAdjSingleton::init(QString &error) {
    QLibrary lib("libryzenadj.dll");
    if (lib.load()) {

        if (!(init_ryzenadj_ptr = (init_ryzenadj) lib.resolve("init_ryzenadj"))) return false;
        if (!(cleanup_ryzenadj_ptr = (cleanup_ryzenadj) lib.resolve("cleanup_ryzenadj"))) return false;
        if (!(init_table_ptr = (init_table) lib.resolve("init_table"))) return false;
        if (!(get_table_ver_ptr = (get_table_ver) lib.resolve("get_table_ver"))) return false;
        if (!(get_table_size_ptr = (get_table_size) lib.resolve("get_table_size"))) return false;
        if (!(get_table_values_ptr = (get_table_values) lib.resolve("get_table_values"))) return false;
        if (!(refresh_table_ptr = (refresh_table) lib.resolve("refresh_table"))) return false;

        if (!(set_stapm_limit_ptr = (set_stapm_limit) lib.resolve("set_stapm_limit"))) return false;
        if (!(set_fast_limit_ptr = (set_fast_limit) lib.resolve("set_fast_limit"))) return false;
        if (!(set_slow_limit_ptr = (set_slow_limit) lib.resolve("set_slow_limit"))) return false;
        if (!(set_slow_time_ptr = (set_slow_time) lib.resolve("set_slow_time"))) return false;
        if (!(set_stapm_time_ptr = (set_stapm_time) lib.resolve("set_stapm_time"))) return false;
        if (!(set_tctl_temp_ptr = (set_tctl_temp) lib.resolve("set_tctl_temp"))) return false;
        if (!(set_vrm_current_ptr = (set_vrm_current) lib.resolve("set_vrm_current"))) return false;
        if (!(set_vrmsoc_current_ptr = (set_vrmsoc_current) lib.resolve("set_vrmsoc_current"))) return false;
        if (!(set_vrmmax_current_ptr = (set_vrmmax_current) lib.resolve("set_vrmmax_current"))) return false;
        if (!(set_vrmsocmax_current_ptr = (set_vrmsocmax_current) lib.resolve("set_vrmsocmax_current"))) return false;
        if (!(set_psi0_current_ptr = (set_psi0_current) lib.resolve("set_psi0_current"))) return false;
        if (!(set_psi0soc_current_ptr = (set_psi0soc_current) lib.resolve("set_psi0soc_current"))) return false;
        if (!(set_max_gfxclk_freq_ptr = (set_max_gfxclk_freq) lib.resolve("set_max_gfxclk_freq"))) return false;
        if (!(set_min_gfxclk_freq_ptr = (set_min_gfxclk_freq) lib.resolve("set_min_gfxclk_freq"))) return false;
        if (!(set_max_socclk_freq_ptr = (set_max_socclk_freq) lib.resolve("set_max_socclk_freq"))) return false;
        if (!(set_min_socclk_freq_ptr = (set_min_socclk_freq) lib.resolve("set_min_socclk_freq"))) return false;
        if (!(set_max_fclk_freq_ptr = (set_max_fclk_freq) lib.resolve("set_max_fclk_freq"))) return false;
        if (!(set_min_fclk_freq_ptr = (set_min_fclk_freq) lib.resolve("set_min_fclk_freq"))) return false;
        if (!(set_max_vcn_ptr = (set_max_vcn) lib.resolve("set_max_vcn"))) return false;
        if (!(set_min_vcn_ptr = (set_min_vcn) lib.resolve("set_min_vcn"))) return false;
        if (!(set_max_lclk_ptr = (set_max_lclk) lib.resolve("set_max_lclk"))) return false;
        if (!(set_min_lclk_ptr = (set_min_lclk) lib.resolve("set_min_lclk"))) return false;
        if (!(set_prochot_deassertion_ramp_ptr = (set_prochot_deassertion_ramp) lib.resolve(
                "set_prochot_deassertion_ramp")))
            return false;
        if (!(set_apu_skin_temp_limit_ptr = (set_apu_skin_temp_limit) lib.resolve("set_apu_skin_temp_limit")))
            return false;
        if (!(set_dgpu_skin_temp_limit_ptr = (set_dgpu_skin_temp_limit) lib.resolve("set_dgpu_skin_temp_limit")))
            return false;
        if (!(set_apu_slow_limit_ptr = (set_apu_slow_limit) lib.resolve("set_apu_slow_limit"))) return false;
        if (!(set_skin_temp_power_limit_ptr = (set_skin_temp_power_limit) lib.resolve(
                "set_skin_temp_power_limit")))
            return false;
        if (!(set_power_saving_ptr = (set_power_saving) lib.resolve("set_power_saving"))) return false;
        if (!(set_max_performance_ptr = (set_max_performance) lib.resolve("set_max_performance"))) return false;

        if (!(get_stapm_limit_ptr = (get_stapm_limit) lib.resolve("get_stapm_limit"))) return false;
        if (!(get_stapm_value_ptr = (get_stapm_value) lib.resolve("get_stapm_value"))) return false;
        if (!(get_fast_limit_ptr = (get_fast_limit) lib.resolve("get_fast_limit"))) return false;
        if (!(get_fast_value_ptr = (get_fast_value) lib.resolve("get_fast_value"))) return false;
        if (!(get_slow_limit_ptr = (get_slow_limit) lib.resolve("get_slow_limit"))) return false;
        if (!(get_slow_value_ptr = (get_slow_value) lib.resolve("get_slow_value"))) return false;
        if (!(get_apu_slow_limit_ptr = (get_apu_slow_limit) lib.resolve("get_apu_slow_limit"))) return false;
        if (!(get_apu_slow_value_ptr = (get_apu_slow_value) lib.resolve("get_apu_slow_value"))) return false;
        if (!(get_vrm_current_ptr = (get_vrm_current) lib.resolve("get_vrm_current"))) return false;
        if (!(get_vrm_current_value_ptr = (get_vrm_current_value) lib.resolve("get_vrm_current_value"))) return false;
        if (!(get_vrmsoc_current_ptr = (get_vrmsoc_current) lib.resolve("get_vrmsoc_current"))) return false;
        if (!(get_vrmsoc_current_value_ptr = (get_vrmsoc_current_value) lib.resolve("get_vrmsoc_current_value")))
            return false;
        if (!(get_vrmmax_current_ptr = (get_vrmmax_current) lib.resolve("get_vrmmax_current"))) return false;
        if (!(get_vrmmax_current_value_ptr = (get_vrmmax_current_value) lib.resolve("get_vrmmax_current_value")))
            return false;
        if (!(get_vrmsocmax_current_ptr = (get_vrmsocmax_current) lib.resolve("get_vrmsocmax_current"))) return false;
        if (!(get_vrmsocmax_current_value_ptr = (get_vrmsocmax_current_value) lib.resolve(
                "get_vrmsocmax_current_value")))
            return false;
        if (!(get_tctl_temp_ptr = (get_tctl_temp) lib.resolve("get_tctl_temp"))) return false;
        if (!(get_tctl_temp_value_ptr = (get_tctl_temp_value) lib.resolve("get_tctl_temp_value"))) return false;
        if (!(get_apu_skin_temp_limit_ptr = (get_apu_skin_temp_limit) lib.resolve("get_apu_skin_temp_limit")))
            return false;
        if (!(get_apu_skin_temp_value_ptr = (get_apu_skin_temp_value) lib.resolve("get_apu_skin_temp_value")))
            return false;
        if (!(get_dgpu_skin_temp_limit_ptr = (get_dgpu_skin_temp_limit) lib.resolve("get_dgpu_skin_temp_limit")))
            return false;
        if (!(get_dgpu_skin_temp_value_ptr = (get_dgpu_skin_temp_value) lib.resolve("get_dgpu_skin_temp_value")))
            return false;
        if (!(get_psi0_current_ptr = (get_psi0_current) lib.resolve("get_psi0_current"))) return false;
        if (!(get_psi0soc_current_ptr = (get_psi0soc_current) lib.resolve("get_psi0soc_current"))) return false;
        if (!(get_stapm_time_ptr = (get_stapm_time) lib.resolve("get_stapm_time"))) return false;
        if (!(get_slow_time_ptr = (get_slow_time) lib.resolve("get_slow_time"))) return false;
        if (!(get_cclk_setpoint_ptr = (get_cclk_setpoint) lib.resolve("get_cclk_setpoint"))) return false;
        if (!(get_cclk_busy_value_ptr = (get_cclk_busy_value) lib.resolve("get_cclk_busy_value"))) return false;

        this->ry = init_ryzenadj_ptr();
        initTable();

        return true;
    } else {

    }
    return false;
}

void RyzenAdjSingleton::cleanupRyzenadj() { return this->cleanup_ryzenadj_ptr(this->ry); }

int RyzenAdjSingleton::initTable() { return this->init_table_ptr(this->ry); }

uint32_t RyzenAdjSingleton::getTableVer() { return this->get_table_ver_ptr(this->ry); }

size_t RyzenAdjSingleton::getTableSize() { return this->get_table_size_ptr(this->ry); }

float *RyzenAdjSingleton::getTableValues() { return this->get_table_values_ptr(this->ry); }

int RyzenAdjSingleton::refreshTable() { return this->refresh_table_ptr(this->ry); }

int RyzenAdjSingleton::setStapmLimit(uint32_t value) { return this->set_stapm_limit_ptr(this->ry, value); }

int RyzenAdjSingleton::setFastLimit(uint32_t value) { return this->set_fast_limit_ptr(this->ry, value); }

int RyzenAdjSingleton::setSlowLimit(uint32_t value) { return this->set_slow_limit_ptr(this->ry, value); }

int RyzenAdjSingleton::setSlowTime(uint32_t value) { return this->set_slow_time_ptr(this->ry, value); }

int RyzenAdjSingleton::setStapmTime(uint32_t value) { return this->set_stapm_time_ptr(this->ry, value); }

int RyzenAdjSingleton::setTctlTemp(uint32_t value) { return this->set_tctl_temp_ptr(this->ry, value); }

int RyzenAdjSingleton::setVrmCurrent(uint32_t value) { return this->set_vrm_current_ptr(this->ry, value); }

int RyzenAdjSingleton::setVrmsocCurrent(uint32_t value) { return this->set_vrmsoc_current_ptr(this->ry, value); }

int RyzenAdjSingleton::setVrmmaxCurrent(uint32_t value) { return this->set_vrmmax_current_ptr(this->ry, value); }

int RyzenAdjSingleton::setVrmsocmaxCurrent(uint32_t value) { return this->set_vrmsocmax_current_ptr(this->ry, value); }

int RyzenAdjSingleton::setPsi0Current(uint32_t value) { return this->set_psi0_current_ptr(this->ry, value); }

int RyzenAdjSingleton::setPsi0socCurrent(uint32_t value) { return this->set_psi0soc_current_ptr(this->ry, value); }

int RyzenAdjSingleton::setMaxGfxclkFreq(uint32_t value) { return this->set_max_gfxclk_freq_ptr(this->ry, value); }

int RyzenAdjSingleton::setMinGfxclkFreq(uint32_t value) { return this->set_min_gfxclk_freq_ptr(this->ry, value); }

int RyzenAdjSingleton::setMaxSocclkFreq(uint32_t value) { return this->set_max_socclk_freq_ptr(this->ry, value); }

int RyzenAdjSingleton::setMinSocclkFreq(uint32_t value) { return this->set_min_socclk_freq_ptr(this->ry, value); }

int RyzenAdjSingleton::setMaxFclkFreq(uint32_t value) { return this->set_max_fclk_freq_ptr(this->ry, value); }

int RyzenAdjSingleton::setMinFclkFreq(uint32_t value) { return this->set_min_fclk_freq_ptr(this->ry, value); }

int RyzenAdjSingleton::setMaxVcn(uint32_t value) { return this->set_max_vcn_ptr(this->ry, value); }

int RyzenAdjSingleton::setMinVcn(uint32_t value) { return this->set_min_vcn_ptr(this->ry, value); }

int RyzenAdjSingleton::setMaxLclk(uint32_t value) { return this->set_max_lclk_ptr(this->ry, value); }

int RyzenAdjSingleton::setMinLclk(uint32_t value) { return this->set_min_lclk_ptr(this->ry, value); }

int RyzenAdjSingleton::setProchotDeassertionRamp(uint32_t value) {
    return this->set_prochot_deassertion_ramp_ptr(this->ry, value);
}

int RyzenAdjSingleton::setApuSkinTempLimit(uint32_t value) {
    return this->set_apu_skin_temp_limit_ptr(this->ry, value);
}

int RyzenAdjSingleton::setDgpuSkinTempLimit(uint32_t value) {
    return this->set_dgpu_skin_temp_limit_ptr(this->ry, value);
}

int RyzenAdjSingleton::setApuSlowLimit(uint32_t value) { return this->set_apu_slow_limit_ptr(this->ry, value); }

int RyzenAdjSingleton::setSkinTempPowerLimit(uint32_t value) {
    return this->set_skin_temp_power_limit_ptr(this->ry, value);
}

int RyzenAdjSingleton::setPowerSaving() { return this->set_power_saving_ptr(this->ry); }

int RyzenAdjSingleton::setMaxPerformance() { return this->set_max_performance_ptr(this->ry); }

float RyzenAdjSingleton::getStapmLimit() { return this->get_stapm_limit_ptr(this->ry); }

float RyzenAdjSingleton::getStapmValue() { return this->get_stapm_value_ptr(this->ry); }

float RyzenAdjSingleton::getFastLimit() { return this->get_fast_limit_ptr(this->ry); }

float RyzenAdjSingleton::getFastValue() { return this->get_fast_value_ptr(this->ry); }

float RyzenAdjSingleton::getSlowLimit() { return this->get_slow_limit_ptr(this->ry); }

float RyzenAdjSingleton::getSlowValue() { return this->get_slow_value_ptr(this->ry); }

float RyzenAdjSingleton::getApuSlowLimit() { return this->get_apu_slow_limit_ptr(this->ry); }

float RyzenAdjSingleton::getApuSlowValue() { return this->get_apu_slow_value_ptr(this->ry); }

float RyzenAdjSingleton::getVrmCurrent() { return this->get_vrm_current_ptr(this->ry); }

float RyzenAdjSingleton::getVrmCurrentValue() { return this->get_vrm_current_value_ptr(this->ry); }

float RyzenAdjSingleton::getVrmsocCurrent() { return this->get_vrmsoc_current_ptr(this->ry); }

float RyzenAdjSingleton::getVrmsocCurrentValue() { return this->get_vrmsoc_current_value_ptr(this->ry); }

float RyzenAdjSingleton::getVrmmaxCurrent() { return this->get_vrmmax_current_ptr(this->ry); }

float RyzenAdjSingleton::getVrmmaxCurrentValue() { return this->get_vrmmax_current_value_ptr(this->ry); }

float RyzenAdjSingleton::getVrmsocmaxCurrent() { return this->get_vrmsocmax_current_ptr(this->ry); }

float RyzenAdjSingleton::getVrmsocmaxCurrentValue() { return this->get_vrmsocmax_current_value_ptr(this->ry); }

float RyzenAdjSingleton::getTctlTemp() { return this->get_tctl_temp_ptr(this->ry); }

float RyzenAdjSingleton::getTctlTempValue() { return this->get_tctl_temp_value_ptr(this->ry); }

float RyzenAdjSingleton::getApuSkinTempLimit() { return this->get_apu_skin_temp_limit_ptr(this->ry); }

float RyzenAdjSingleton::getApuSkinTempValue() { return this->get_apu_skin_temp_value_ptr(this->ry); }

float RyzenAdjSingleton::getDgpuSkinTempLimit() { return this->get_dgpu_skin_temp_limit_ptr(this->ry); }

float RyzenAdjSingleton::getDgpuSkinTempValue() { return this->get_dgpu_skin_temp_value_ptr(this->ry); }

float RyzenAdjSingleton::getPsi0Current() { return this->get_psi0_current_ptr(this->ry); }

float RyzenAdjSingleton::getPsi0socCurrent() { return this->get_psi0soc_current_ptr(this->ry); }

float RyzenAdjSingleton::getStapmTime() { return this->get_stapm_time_ptr(this->ry); }

float RyzenAdjSingleton::getSlowTime() { return this->get_slow_time_ptr(this->ry); }

float RyzenAdjSingleton::getCclkSetpoint() { return this->get_cclk_setpoint_ptr(this->ry); }

float RyzenAdjSingleton::getCclkBusyValue() { return this->get_cclk_busy_value_ptr(this->ry); }

float RyzenAdjSingleton::getValueByOffset(ushort offset) {
    return getTableValues()[offset / 4];
}

float RyzenAdjSingleton::getCoreTempLimit() {
    return getValueByOffset(0x40);
}

float RyzenAdjSingleton::getApuTempLimit() {
    return getValueByOffset(0x48);
}

float RyzenAdjSingleton::getDGpuTempLimit() {
    return getValueByOffset(0x50);
}

float RyzenAdjSingleton::getCoreTemp() {
    return getValueByOffset(0x44);
}

float RyzenAdjSingleton::getApuTemp() {
    return getValueByOffset(0x4C);
}

float RyzenAdjSingleton::getDGpuTemp() {
    return getValueByOffset(0x54);
}

void RyzenAdjSingleton::setPowerProfile(const CpuProfile &powerProfile) {
    setStapmLimit((uint32_t) (powerProfile.getStapmLimit() * 1000));
    setStapmTime((uint32_t) powerProfile.getStapmTime());
    setSlowLimit((uint32_t) (powerProfile.getSlowLimit() * 1000));
    setSlowTime((uint32_t) powerProfile.getSlowTime());
    setFastLimit((uint32_t) (powerProfile.getFastLimit() * 1000));
    if (powerProfile.getMode() == SP_PERFORMANCE) {
        setMaxPerformance();
    }
    if (powerProfile.getMode() == SP_POWER_SAVING) {
        setPowerSaving();
    }
}


