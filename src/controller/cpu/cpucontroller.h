#include "RyzenAdjTypes.h"

#ifndef G14CONTROLPP_RYZENADJSINGLETON_H
#define G14CONTROLPP_RYZENADJSINGLETON_H

#include "src/model/CpuProfile.h"

class CpuController {
public:
    CpuController();

private:
    RyzenAccess *ry = NULL;

    init_ryzenadj init_ryzenadj_ptr = NULL;
    cleanup_ryzenadj cleanup_ryzenadj_ptr = NULL;
    init_table init_table_ptr = NULL;
    get_table_ver get_table_ver_ptr = NULL;
    get_table_size get_table_size_ptr = NULL;
    get_table_values get_table_values_ptr = NULL;
    refresh_table refresh_table_ptr = NULL;

    set_stapm_limit set_stapm_limit_ptr = NULL;
    set_fast_limit set_fast_limit_ptr = NULL;
    set_slow_limit set_slow_limit_ptr = NULL;
    set_slow_time set_slow_time_ptr = NULL;
    set_stapm_time set_stapm_time_ptr = NULL;
    set_tctl_temp set_tctl_temp_ptr = NULL;
    set_vrm_current set_vrm_current_ptr = NULL;
    set_vrmsoc_current set_vrmsoc_current_ptr = NULL;
    set_vrmmax_current set_vrmmax_current_ptr = NULL;
    set_vrmsocmax_current set_vrmsocmax_current_ptr = NULL;
    set_psi0_current set_psi0_current_ptr = NULL;
    set_psi0soc_current set_psi0soc_current_ptr = NULL;
    set_max_gfxclk_freq set_max_gfxclk_freq_ptr = NULL;
    set_min_gfxclk_freq set_min_gfxclk_freq_ptr = NULL;
    set_max_socclk_freq set_max_socclk_freq_ptr = NULL;
    set_min_socclk_freq set_min_socclk_freq_ptr = NULL;
    set_max_fclk_freq set_max_fclk_freq_ptr = NULL;
    set_min_fclk_freq set_min_fclk_freq_ptr = NULL;
    set_max_vcn set_max_vcn_ptr = NULL;
    set_min_vcn set_min_vcn_ptr = NULL;
    set_max_lclk set_max_lclk_ptr = NULL;
    set_min_lclk set_min_lclk_ptr = NULL;
    set_prochot_deassertion_ramp set_prochot_deassertion_ramp_ptr = NULL;
    set_apu_skin_temp_limit set_apu_skin_temp_limit_ptr = NULL;
    set_dgpu_skin_temp_limit set_dgpu_skin_temp_limit_ptr = NULL;
    set_apu_slow_limit set_apu_slow_limit_ptr = NULL;
    set_skin_temp_power_limit set_skin_temp_power_limit_ptr = NULL;
    set_power_saving set_power_saving_ptr = NULL;
    set_max_performance set_max_performance_ptr = NULL;

    get_stapm_limit get_stapm_limit_ptr = NULL;
    get_stapm_value get_stapm_value_ptr = NULL;
    get_fast_limit get_fast_limit_ptr = NULL;
    get_fast_value get_fast_value_ptr = NULL;
    get_slow_limit get_slow_limit_ptr = NULL;
    get_slow_value get_slow_value_ptr = NULL;
    get_apu_slow_limit get_apu_slow_limit_ptr = NULL;
    get_apu_slow_value get_apu_slow_value_ptr = NULL;
    get_vrm_current get_vrm_current_ptr = NULL;
    get_vrm_current_value get_vrm_current_value_ptr = NULL;
    get_vrmsoc_current get_vrmsoc_current_ptr = NULL;
    get_vrmsoc_current_value get_vrmsoc_current_value_ptr = NULL;
    get_vrmmax_current get_vrmmax_current_ptr = NULL;
    get_vrmmax_current_value get_vrmmax_current_value_ptr = NULL;
    get_vrmsocmax_current get_vrmsocmax_current_ptr = NULL;
    get_vrmsocmax_current_value get_vrmsocmax_current_value_ptr = NULL;
    get_tctl_temp get_tctl_temp_ptr = NULL;
    get_tctl_temp_value get_tctl_temp_value_ptr = NULL;
    get_apu_skin_temp_limit get_apu_skin_temp_limit_ptr = NULL;
    get_apu_skin_temp_value get_apu_skin_temp_value_ptr = NULL;
    get_dgpu_skin_temp_limit get_dgpu_skin_temp_limit_ptr = NULL;
    get_dgpu_skin_temp_value get_dgpu_skin_temp_value_ptr = NULL;
    get_psi0_current get_psi0_current_ptr = NULL;
    get_psi0soc_current get_psi0soc_current_ptr = NULL;
    get_stapm_time get_stapm_time_ptr = NULL;
    get_slow_time get_slow_time_ptr = NULL;
    get_cclk_setpoint get_cclk_setpoint_ptr = NULL;
    get_cclk_busy_value get_cclk_busy_value_ptr = NULL;

    float getValueByOffset(ushort offset);

public:
    bool init(QString &error);

    void cleanupRyzenadj();

    int initTable();
    uint32_t getTableVer();
    size_t getTableSize();
    float* getTableValues();
    int refreshTable();

    int setStapmLimit(uint32_t value);
    int setFastLimit(uint32_t value);
    int setSlowLimit(uint32_t value);
    int setSlowTime(uint32_t value);
    int setStapmTime(uint32_t value);
    int setTctlTemp(uint32_t value);
    int setVrmCurrent(uint32_t value);
    int setVrmsocCurrent(uint32_t value);
    int setVrmmaxCurrent(uint32_t value);
    int setVrmsocmaxCurrent(uint32_t value);
    int setPsi0Current(uint32_t value);
    int setPsi0socCurrent(uint32_t value);
    int setMaxGfxclkFreq(uint32_t value);
    int setMinGfxclkFreq(uint32_t value);
    int setMaxSocclkFreq(uint32_t value);
    int setMinSocclkFreq(uint32_t value);
    int setMaxFclkFreq(uint32_t value);
    int setMinFclkFreq(uint32_t value);
    int setMaxVcn(uint32_t value);
    int setMinVcn(uint32_t value);
    int setMaxLclk(uint32_t value);
    int setMinLclk(uint32_t value);
    int setProchotDeassertionRamp(uint32_t value);
    int setApuSkinTempLimit(uint32_t value);
    int setDgpuSkinTempLimit(uint32_t value);
    int setApuSlowLimit(uint32_t value);
    int setSkinTempPowerLimit(uint32_t value);
    int setPowerSaving();
    int setMaxPerformance();

    float getStapmLimit();
    float getStapmValue();
    float getFastLimit();
    float getFastValue();
    float getSlowLimit();
    float getSlowValue();
    float getApuSlowLimit();
    float getApuSlowValue();
    float getVrmCurrent();
    float getVrmCurrentValue();
    float getVrmsocCurrent();
    float getVrmsocCurrentValue();
    float getVrmmaxCurrent();
    float getVrmmaxCurrentValue();
    float getVrmsocmaxCurrent();
    float getVrmsocmaxCurrentValue();
    float getTctlTemp();
    float getTctlTempValue();
    float getApuSkinTempLimit();
    float getApuSkinTempValue();
    float getDgpuSkinTempLimit();
    float getDgpuSkinTempValue();
    float getPsi0Current();
    float getPsi0socCurrent();
    float getStapmTime();
    float getSlowTime();
    float getCclkSetpoint();
    float getCclkBusyValue();

    float getCoreTempLimit();
    float getApuTempLimit();
    float getDGpuTempLimit();

    float getCoreTemp();
    float getApuTemp();
    float getDGpuTemp();

    void setCpuProfile(const CpuProfile &cpuProfile);
};


#endif //G14CONTROLPP_RYZENADJSINGLETON_H
