#ifndef G14CONTROLPP_RYZENADJTYPES_H
#define G14CONTROLPP_RYZENADJTYPES_H

#include <QDataStream>
#include <QString>

typedef uchar Setpoint;
static Setpoint SP_PERFORMANCE = 0;
static Setpoint SP_POWER_SAVING = 1;
static Setpoint SP_DEFAULT = 2;

struct _PowerProfile {
    QString name;
    float stapmLimit;
    float stapmTime;
    float slowLimit;
    float slowTime;
    float fastLimit;
    Setpoint mode;
};

typedef _PowerProfile PowerProfile;

static PowerProfile STOCK_PROFILE {
    "Stock", 35, 200, 42, 5, 60, SP_DEFAULT
};

QDataStream &operator<<(QDataStream &out, const _PowerProfile &v);
QDataStream &operator>>(QDataStream &in, _PowerProfile &v);

Q_DECLARE_METATYPE(PowerProfile);

struct _RyzenAccess;
typedef struct _RyzenAccess *RyzenAccess;
typedef RyzenAccess *(*init_ryzenadj)();
typedef void (*cleanup_ryzenadj)(RyzenAccess * ry);

typedef int (*init_table)(RyzenAccess * ry);
typedef uint32_t (*get_table_ver)(RyzenAccess * ry);
typedef size_t (*get_table_size)(RyzenAccess * ry);
typedef float* (*get_table_values)(RyzenAccess * ry);
typedef int (*refresh_table)(RyzenAccess * ry);

typedef int (*set_stapm_limit)(RyzenAccess *, uint32_t value);
typedef int (*set_fast_limit)(RyzenAccess *, uint32_t value);
typedef int (*set_slow_limit)(RyzenAccess *, uint32_t value);
typedef int (*set_slow_time)(RyzenAccess *, uint32_t value);
typedef int (*set_stapm_time)(RyzenAccess *, uint32_t value);
typedef int (*set_tctl_temp)(RyzenAccess *, uint32_t value);
typedef int (*set_vrm_current)(RyzenAccess *, uint32_t value);
typedef int (*set_vrmsoc_current)(RyzenAccess *, uint32_t value);
typedef int (*set_vrmmax_current)(RyzenAccess *, uint32_t value);
typedef int (*set_vrmsocmax_current)(RyzenAccess *, uint32_t value);
typedef int (*set_psi0_current)(RyzenAccess *, uint32_t value);
typedef int (*set_psi0soc_current)(RyzenAccess *, uint32_t value);
typedef int (*set_max_gfxclk_freq)(RyzenAccess *, uint32_t value);
typedef int (*set_min_gfxclk_freq)(RyzenAccess *, uint32_t value);
typedef int (*set_max_socclk_freq)(RyzenAccess *, uint32_t value);
typedef int (*set_min_socclk_freq)(RyzenAccess *, uint32_t value);
typedef int (*set_max_fclk_freq)(RyzenAccess *, uint32_t value);
typedef int (*set_min_fclk_freq)(RyzenAccess *, uint32_t value);
typedef int (*set_max_vcn)(RyzenAccess *, uint32_t value);
typedef int (*set_min_vcn)(RyzenAccess *, uint32_t value);
typedef int (*set_max_lclk)(RyzenAccess *, uint32_t value);
typedef int (*set_min_lclk)(RyzenAccess *, uint32_t value);
typedef int (*set_prochot_deassertion_ramp)(RyzenAccess * ry, uint32_t value);
typedef int (*set_apu_skin_temp_limit)(RyzenAccess *, uint32_t value);
typedef int (*set_dgpu_skin_temp_limit)(RyzenAccess *, uint32_t value);
typedef int (*set_apu_slow_limit)(RyzenAccess *, uint32_t value);
typedef int (*set_skin_temp_power_limit)(RyzenAccess * ry, uint32_t value);
typedef int (*set_power_saving)(RyzenAccess * ry);
typedef int (*set_max_performance)(RyzenAccess * ry);

typedef float (*get_stapm_limit)(RyzenAccess * ry);
typedef float (*get_stapm_value)(RyzenAccess * ry);
typedef float (*get_fast_limit)(RyzenAccess * ry);
typedef float (*get_fast_value)(RyzenAccess * ry);
typedef float (*get_slow_limit)(RyzenAccess * ry);
typedef float (*get_slow_value)(RyzenAccess * ry);
typedef float (*get_apu_slow_limit)(RyzenAccess * ry);
typedef float (*get_apu_slow_value)(RyzenAccess * ry);
typedef float (*get_vrm_current)(RyzenAccess * ry);
typedef float (*get_vrm_current_value)(RyzenAccess * ry);
typedef float (*get_vrmsoc_current)(RyzenAccess * ry);
typedef float (*get_vrmsoc_current_value)(RyzenAccess * ry);
typedef float (*get_vrmmax_current)(RyzenAccess * ry);
typedef float (*get_vrmmax_current_value)(RyzenAccess * ry);
typedef float (*get_vrmsocmax_current)(RyzenAccess * ry);
typedef float (*get_vrmsocmax_current_value)(RyzenAccess * ry);
typedef float (*get_tctl_temp)(RyzenAccess * ry);
typedef float (*get_tctl_temp_value)(RyzenAccess * ry);
typedef float (*get_apu_skin_temp_limit)(RyzenAccess * ry);
typedef float (*get_apu_skin_temp_value)(RyzenAccess * ry);
typedef float (*get_dgpu_skin_temp_limit)(RyzenAccess * ry);
typedef float (*get_dgpu_skin_temp_value)(RyzenAccess * ry);
typedef float (*get_psi0_current)(RyzenAccess * ry);
typedef float (*get_psi0soc_current)(RyzenAccess * ry);
typedef float (*get_stapm_time)(RyzenAccess * ry);
typedef float (*get_slow_time)(RyzenAccess * ry);
typedef float (*get_cclk_setpoint)(RyzenAccess * ry);
typedef float (*get_cclk_busy_value)(RyzenAccess * ry);

#endif //G14CONTROLPP_RYZENADJTYPES_H
