#pragma once
#include "sigs.hpp"
#include <core/utils.h>
#include <hooks/hooks.h>
#include <sdk/player/NetAvatar.h>

std::vector<uintptr_t> sigs::database{};

uintptr_t sigs::add_pattern(std::string name, std::string pattern, int type, int offset, bool ignore) {
    uintptr_t address = 0;
    switch (type) {
        case sig::type::direct: address = utils::find_pattern(pattern.c_str(), false, offset); break;
        case sig::type::fstart: address = utils::find_func_start(pattern.c_str()) + offset; break;
        case sig::type::call: address = detail::get_call(pattern.c_str(), offset); break;
        case sig::type::direct_calc: {
            address = utils::find_pattern(pattern.c_str(), false, offset);

            if (address) {
                //                    Curr address offset     +               Offset in instruction
                auto offset = (address - global::gt) + (utils::read<uint32_t>(address, 0) + sizeof(uint32_t));

                address = (global::gt + offset); 
                //dont instantly dereference since s_renderD3D9 is not initialized for everyone upon patching
            }

            break;
        }
    }

    if (address == 0 || address <= offset || address >= 0xffffffffffffefff) {
        if (ignore)
            return 0;
        utils::printc("91", "pattern of %s not found!", name.c_str());
        address = 0;
    }

    database.push_back(address);
    return address;
}
void add_pattern2() {
}
void sigs::init() {
    auto bp = add_pattern("gt::patch_banbypass", "00 3B C1 75 ? 85 C9", sig::type::direct, 3, true);
    if (!bp) //will be invalid if ban bypass already patched, so we are doing this.
        add_pattern("gt::patch_banbypass", "00 3B C1 90 90 85 C9", sig::type::direct, 3);

    add_pattern("sdk::GetGameLogic", "E8 ? ? ? ? 8B 17 48 8D ? ? ? ? ? E8 ? ? ? ? 48 8B", sig::type::call);
    add_pattern("WorldCamera::WorldToScreen", "00 e8 ? ? ? ? 49 8b ? ? 41 ? 00 04", sig::type::call, 1);
    add_pattern("sdk::GetClient", "E8 ? ? ? ? 45 ? ? 89 ? ? ? 48 8D ? ? 48 8B", sig::type::call);

    //all hooks.cpp
    hookmgr->add_hook("BaseApp::SetFPSLimit", "00 00 0F 57 C0 0F 2F C8 72", sig::type::fstart, hooks::BaseApp_SetFPSLimit);
    hookmgr->add_hook("LogMsg", "00 28 00 00 45", sig::type::fstart, hooks::LogMsg);
    hookmgr->add_hook("NetAvatar::CanMessageT4", "48 8b ce e8 ? ? ? ? 84 c0 74 ? e8", sig::type::call, hooks::NetAvatar_CanMessageT4, 3);
    hookmgr->add_hook("SendPacketRaw", "00 81 FF 40 42 0F 00 7E 13", sig::type::fstart, hooks::SendPacketRaw);
    hookmgr->add_hook("HandleTouch", "83 B8 ?? ?? ?? ?? 12 75", sig::type::fstart, hooks::HandleTouch);
    hookmgr->add_hook("WorldCamera_OnUpdate", "89 43 10 0f 2f", sig::type::fstart, hooks::WorldCamera_OnUpdate);
    hookmgr->add_hook("SendPacket", "02 00 00 00 e8 ? ? ? ? 90 48 8d 4c 24 50", sig::type::call, hooks::SendPacket, 4);
    hookmgr->add_hook("ProcessTankUpdatePacket", "83 78 04 71 75 ? 49", sig::type::fstart, hooks::ProcessTankUpdatePacket);
    hookmgr->add_hook("CanSeeGhosts", "04 00 00 00 e8 ? ? ? ? 8b c8 e8", sig::type::call, hooks::CanSeeGhosts, 11);
    hookmgr->add_hook("NetHTTP::Update", "05 4D 01 00 00 89 ?? ?? 00", sig::type::fstart, hooks::NetHTTP_Update);
    hookmgr->add_hook("App::Update", "00 E8 ? ? ? ? C6 ?? ? ? 00 00 01 B9 ? ? 00 00", sig::type::fstart, hooks::App_Update);
    hookmgr->add_hook("TileExtra::Serialize", "04 ? C0 0F ? C0 88 83 ? ? 00", sig::type::fstart, hooks::TileExtra_Serialize);
    hookmgr->add_hook("NetAvatar::OnPunched", "4C 8B C3 E8 ? ? ? ? 33", sig::type::call, hooks::OnPunched, 3);
    hookmgr->add_hook("Tile::GetFruitBloomProgressPercent", "04 66 ?? ?? ?? FD FF 00 00", sig::type::fstart, hooks::GetFruitBloomProgressPercent);
    hookmgr->add_hook("GameLogicComponent::DialogIsOpened", "c8 e8 ? ? ? ? 84 c0 74 ? 48", sig::type::call, hooks::DialogIsOpened, 1);
    add_pattern("NetAvatar::SetCharacterMods", "E8 ? ? ? ? e9 ? ? ? ? 4C 8d ?? ?? ?? ?? ?? 4C", sig::type::call);
    add_pattern("GetSurfaceAnim", "48 ? ? 20 41 ? 01 e8", sig::type::call, 7);
    add_pattern("Mutex Bypass 1", "EB ? 33 ? 48 ? ? E8 ? ? ? ? 48 ? ? D0 01", sig::type::direct, 24);
    add_pattern("Mutex Bypass 2", "B9 01 00 1f 00 FF", sig::type::direct);
    add_pattern("s_renderD3D9", "C0 48 ?? 80 22 00 00 48", sig::type::direct_calc, 18);
    add_pattern("consts offset", "B9 10 1F 00 00 66 ? ? 75 ? F3", sig::type::direct_calc, 38);
    add_pattern("counter offset", "F3 ? ? ? ? ? 00 00 C7 ? ? ? 00 00 FF FF FF FF", sig::type::direct, 4);

    size_t invalid = 0;
    for (auto sig : database) {
        if (!sig)
            invalid++;
    }

    if (invalid == 0)
        utils::printc("92", "%zd signatures in db, all valid\n", database.size());
}
