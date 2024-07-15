#include "Consts.h"

float* consts::values = new float[C_MAXVALUE];
float* consts::defs = new float[C_MAXVALUE];

void consts::reset_all() {
    for (int i = 0; i < C_MAXVALUE; i++) {
        auto def = defs[i];
        values[i] = def;
        set_float(i, def);
    }
}

void consts::set_defaults() {
    values[C_MAX_FALLING_SPEED] = 400.f;
    values[C_PLAYER_FLOAT_POWER] = 5000.f;
    values[C_WIND_SPEED] = 1000.f;
    values[C_WIND_SPEED_AGAINST] = 100.f;
    values[C_LOCAL_COLLISION_FOR_NET_PLAYER_MAX_DIVERGENCE] = 48.f;
    values[C_PLAYER_DRAG] = 1200.f;
    values[C_INTIAL_JUMP_BOOST] = -450.f;
    values[C_CONTROL_MOD_WHEN_HURT] = 0.15f;
    values[C_LAVA_BOUNCE_POWER_X] = 300.f;
    values[C_LAVA_BOUNCE_POWER_Y] = 800.f;
    values[C_TRAMPOLINE_BOUNCE_POWER_Y] = 800.f;
    values[C_BOUNCY_BOUNCE_POWER] = 500.f;
    values[C_PUNCHED_KNOCKBACK_DEFAULT] = 400.f;
    values[C_PARASOL_GRAV_REDUCE_MOD] = 0.3f;
    values[C_UNDER_WATER_GRAVITY_MOD] = 0.3f;
    values[C_MARS_GRAVITY_MOD] = 0.75f;
    values[C_ONE_THOUSAND] = 1000.f;
    values[C_GHOST_SPEED] = 330.f;
    values[C_WATER_LEVITATION_TIME] = 3000.f;
    values[C_PUNCH_RELOAD_TIME] = 0.2f;
    values[C_DELAY_BETWEEN_HACK_REPORTS_MS] = 15000.f;
    memcpy(&defs[0], &values[0], sizeof(float) * C_MAXVALUE);
}

void consts::set_float(int index, float val) {
    static auto offset = sigs::get(sig::consts_offset);
    static auto counter_offset = utils::read<uint32_t>(sigs::get(sig::counter_offset), 0);
    auto ptr = (EncryptedFloat*)(offset + index * sizeof(EncryptedFloat));
    float delta = val - ptr->get();
    ptr->set(val);
    if (index < C_PARASOL_GRAV_REDUCE_MOD)
        *(float*)((uintptr_t)global::app + counter_offset) += delta;
}

float consts::get_float(int index) {
    static auto offset = sigs::get(sig::consts_offset);
    auto ptr = (EncryptedFloat*)(offset + index * sizeof(EncryptedFloat));
    return ptr->get();
}
