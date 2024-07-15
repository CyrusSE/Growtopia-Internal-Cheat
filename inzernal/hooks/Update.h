#pragma once
#include <core/globals.h>
#include <core/gt.h>
#include <hooks/hooks.h>
#include <sdk/Consts.h>

static bool NoMoreUpdating;
class UpdateManager {
   public:
    //for code that needs to be handled on injection as well
    static void OnInject(App* app) {
        static auto orig_fps = decltype(&hooks::BaseApp_SetFPSLimit)(hookmgr->orig(sig::baseapp_setfpslimit));
        orig_fps((BaseApp*)app, opt::fps_limit);
        utils::printc("93", "Modified FPS limit!");
        global::app = app;
        utils::unprotect_process();

        consts::set_defaults();

        if (opt::cheat::punch_cooldown_on)
            consts::set_float(C_PUNCH_RELOAD_TIME, opt::cheat::punch_cooldown_val);

        auto gamelogic = sdk::GetGameLogic();
        if (!gamelogic)
            return;
        auto player = gamelogic->GetLocalPlayer();
        if (!player)
            return;

        global::state.copy_inject(player, true);
        player->SetModStatus(opt::cheat::mod_zoom, opt::cheat::dev_zoom);
        player->SetCharacterMods(opt::cheat::dash, opt::cheat::jump_charge, opt::cheat::jump_cancel);
    }
    static void OnDestroy() {
        NoMoreUpdating = true;
        auto app = global::app;

        consts::reset_all(); //reset all consts

        auto gamelogic = sdk::GetGameLogic();
        if (!gamelogic)
            return;
        auto local = gamelogic->GetLocalPlayer();
        if (!local)
            return;

        local->SetModStatus(false, false);
        local->SetCharacterMods(false, false, false);

        if (opt::cheat::gravity_on)
            local->gravity.set(global::state.gravity);
        if (opt::cheat::accel_on)
            local->accel.set(global::state.accel);
        if (opt::cheat::speed_on)
            local->speed.set(global::state.speed);
        if (opt::cheat::waterspeed_on)
            local->water_speed = global::state.water_speed;
    }
    static void OnJoinedWorld(NetAvatar* local) {
        local->SetModStatus(opt::cheat::mod_zoom, opt::cheat::dev_zoom);
        local->SetCharacterMods(opt::cheat::dash, opt::cheat::jump_charge, opt::cheat::jump_cancel);
    }
    static void OnUpdateInWorld(App* app, NetAvatar* local) {
        if (opt::cheat::gravity_on) {
            if (opt::cheat::gravity_val != local->gravity.get())
                local->gravity.set(opt::cheat::gravity_val);
        }
        if (opt::cheat::accel_on) {
            if (opt::cheat::accel_val != local->accel.get())
                local->accel.set(opt::cheat::accel_val);
        }
        if (opt::cheat::speed_on) {
            if (opt::cheat::speed_val != local->speed.get())
                local->speed.set(opt::cheat::speed_val);
        }
        if (opt::cheat::waterspeed_on) {
            if (opt::cheat::waterspeed_val != local->water_speed)
                local->water_speed = opt::cheat::waterspeed_val;
        }

    }
    //any kind of continuous code can be ran here
    static void Execute(App* app) {
        static auto orig = decltype(&hooks::App_Update)(hookmgr->orig(sig::app_update));
        if (!global::app && app)
            OnInject(app);

        if (!global::d9init)
            hooks::init_endscene();

        auto gamelogic = sdk::GetGameLogic();
        if (gamelogic && !NoMoreUpdating) {
            auto player = gamelogic->GetLocalPlayer();
            if (player)
                OnUpdateInWorld(app, player);

        }
        
        orig(app);
    }
};