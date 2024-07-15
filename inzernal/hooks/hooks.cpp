#pragma once
#include <core/gt.h>
#include <core/utils.h>
#include <hooks/ProcessTankUpdatePacket.h>
#include <hooks/SendPacket.h>
#include <hooks/SendPacketRaw.h>
#include <hooks/Update.h>
#include <hooks/hooks.h>
#include <intrin.h>
#include <menu\menu.h>
#include <sdk/sdk.h>
#include <stdio.h>
#include <iomanip>
#include <thread>
#include <set>
#include "PathFinder/PathFinder.h"
//#include "PathFinder/AStar.h"

WNDPROC hooks::wndproc; //special cases which dont follow normal pattern
LPVOID hooks::endscene;

hookmanager* hookmgr = new hookmanager();
bool canrender = false;

static int map5[60][100];

static void updatemap() {
    
}
static void testomegalul() {
    auto logic = sdk::GetGameLogic();
    auto tilemap = logic->GetTileMap();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    //int w = 0;
    if (logic) {
        typedef pair<int, int> Pair;
        Pair src((int)logic->GetLocalPlayer()->pos.x / 32, (int)logic->GetLocalPlayer()->pos.y / 32);
        Pair dest((int)opt::cheat::TouchPos.x, (int)opt::cheat::TouchPos.y);
        //Create new pathfinder
        Pathfinder* pathfinder = new Pathfinder(100, 60);

        //Loop through the map array and insert all coordinates into pathfinder's STL map
        for (int i = 0; i < pathfinder->width; i++) {
            for (int j = 0; j < pathfinder->height; j++) {
                pathfinder->nodes->insert(std::pair<Pathfinder::Point, int>(Pathfinder::Point(i, j), opt::cheat::gtmap[j][i]));
                //w = w + 1;
            }
        }
        std::vector<Pathfinder::Point> path = pathfinder->findPath(Pathfinder::Point(src.first, src.second), Pathfinder::Point(dest.first, dest.second));
        //menu::pathLine = pathfinder->findPath(Pathfinder::Point(src.first, src.second), Pathfinder::Point(dest.first, dest.second));
       //int w = 0;
        if (local) {
            for (int i = 0; i < path.size(); i++) {
                //w = w + 1;
                GameUpdatePacket packet{ 0 };
                auto pos = logic->GetLocalPlayer()->GetPos();
                packet.type = PACKET_STATE;
                //packet.int_data = 822;
                packet.int_x = path[i].x;
                packet.int_y = path[i].y;
                packet.pos_x = path[i].x * 32;
                packet.pos_y = path[i].y * 32;
                packet.flags = (1 << 5) | (1 << 10) | (1 << 11); //no enum rn so using raw values
                //gt::send(&packet);
                pos.x = path[i].x * 32;
                pos.y = path[i].y * 32;
                Sleep(7);
                logic->GetLocalPlayer()->SetPosAtTile(path[i].x, path[i].y);
            }
        }
        //else {
        //    for (int i = 0; i < path.size(); i++) {
        //        //w = w + 1;
        //        GameUpdatePacket packet{ 0 };
        //        auto pos = logic->GetLocalPlayer()->GetPos();
        //        packet.type = PACKET_STATE;
        //        //packet.int_data = 822;
        //        packet.int_x = path[i].x;
        //        packet.int_y = path[i].y;
        //        packet.pos_x = path[i].x * 32;
        //        packet.pos_y = path[i].y * 32;
        //        packet.flags = (1 << 5) | (1 << 10) | (1 << 11); //no enum rn so using raw values
        //        //gt::send(&packet);
        //        pos.x = path[i].x * 32;
        //        pos.y = path[i].y * 32;
        //        Sleep(8);
        //        logic->GetLocalPlayer()->SetPosAtTile(path[i].x, path[i].y);
        //    }
        //}
        opt::cheat::PathFindinglol = false;
        //if (path.size() != 0 && path.size() != 1) {
        //    variantlist_t va{ "OnTextOverlay" };
        //    va[1] = "`9Traveling " + to_string(path.size()) + " blocks.";
        //    gt::send_varlist_self(va);
        //}
        //opt::words = "`9Traveling " + to_string(path.size()) + " blocks.";
        //opt::sayit = true;
    }
}

void hooks::init_endscene() {   
    if (global::d9init)
        return;

    auto s_renderD3D9 = *(uintptr_t*)sigs::get(sig::s_renderd3d9);

    if (!s_renderD3D9)
        return;

    //source for s_RenderD3D9: https://github.com/bkaradzic/bgfx/blob/master/src/renderer_d3d9.cpp
    //the dx9 rendering device that GT (bgfx) uses as well

    auto m_d3d9ex = *(IDirect3D9Ex**)(s_renderD3D9 + 312);
    void** vtable = nullptr;
    if (m_d3d9ex) { //bgfx proefers m_deviceEx if m_d3d9ex exists
        auto m_deviceEx = *(IDirect3DDevice9Ex**)(s_renderD3D9 + 320);
        if (!m_deviceEx) {
            auto m_device = *(IDirect3DDevice9**)(s_renderD3D9 + 336);
            if (!m_device)
                return;
            vtable = *reinterpret_cast<void***>(m_device);
        }
        vtable = *reinterpret_cast<void***>(m_deviceEx);
    }
    else {
        auto m_device = *(IDirect3DDevice9**)(s_renderD3D9 + 336);
        if (!m_device)
            return;
        vtable = *reinterpret_cast<void***>(m_device);
    }
    if (!vtable)
        return;
    global::d9init = true;
    MH_CreateHook(LPVOID(vtable[42]), EndScene, (void**)(&hooks::endscene));
    MH_EnableHook(LPVOID(vtable[42]));
    utils::printc("93", "Hooked endscene");
}

void hooks::init() {
    global::hwnd = FindWindowA(nullptr, "Growtopia");

    init_endscene();

    //hooks can now be found in sigs.cpp, they are directly set up there
    for (auto hk : hookmgr->hooks)
        MH_CreateHook(hk->address, hk->hooked, &hk->orig);

    if (!global::hwnd)
        printf("TESTING: HWND not found\n");

    wndproc = WNDPROC(SetWindowLongPtrW(global::hwnd, -4, LONG_PTR(WndProc)));

    MH_EnableHook(MH_ALL_HOOKS);
    utils::printc("93", "Hooks have been setup!");
    //DrawString("The quick brown fox jumped over the lazy dog", 16, 20, 0, 0, 0, 1);
}

void hooks::destroy() {
    UpdateManager::OnDestroy();

    //I mean, I can't be bothered to deal with this, so let's just restore this sig in case we re-inject
    //it does not matter anyways cuz this is in App::Init and won't be called again.
    auto mutex2 = sigs::get(sig::mutexbypass2);
    utils::patch_bytes<6>(mutex2, "\xB9\x01\x00\x1F\x00\xFF");
    SetWindowLongPtr(global::hwnd, -4, LONG_PTR(wndproc));
RETRY:
    if (MH_OK != MH_DisableHook(MH_ALL_HOOKS)) {
        auto res = MessageBox(GetForegroundWindow(), L"Hooks could not be reverted", L"Internal", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TOPMOST);
        switch (res) {
            case IDABORT:
                while (!TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS))
                    ;
            case IDRETRY: goto RETRY;
            case IDIGNORE: // just ignore
                break;
        }
    }
}

void __cdecl hooks::BaseApp_SetFPSLimit(BaseApp* ba, float fps) {
    static auto orig = decltype(&hooks::BaseApp_SetFPSLimit)(hookmgr->orig(sig::baseapp_setfpslimit));
    orig(ba, opt::fps_limit);
}

int __cdecl hooks::LogMsg(const char* msg, ...) {
    static auto orig = decltype(&hooks::LogMsg)(hookmgr->orig(sig::logmsg));
    char buf[0x1000u];
    int len = -1;
    va_list va;
    va_start(va, msg);
    if (0 > (len = _vsnprintf_s(buf, sizeof(buf), msg, va))) {
        va_end(va);
        return 0;
    }
    va_end(va);

    if (len < 90 && len > 2) {
        if (logging::enabled && logging::console & logging::logmsg)
            utils::printc("88", "LogMsg\t%s", buf);
    }
    return orig(buf);
}

bool __cdecl hooks::NetAvatar_CanMessageT4(NetAvatar* player) {
    utils::printc("1;40;31", "CanMessageT4 called, there is a chance of ban, so automatically left the world.");
    if (opt::custom_server_on) {
        gt::send(3, "action|quit_to_exit");
    }
    return false;
}

bool active = false;
LRESULT __stdcall hooks::WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    active = GetActiveWindow() == global::hwnd;

    //TODO: with imgui
    if (menu::WndProc(wnd, msg, wparam, lparam))
        return true;

    if (msg == WM_KEYDOWN && (wparam == VK_CONTROL || wparam == VK_LCONTROL || wparam == VK_RCONTROL))
        return true;
    if (global::load == true) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse || io.WantCaptureKeyboard && (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP))
            return TRUE;
    }
    /*if (msg == WM_KEYUP && wparam == VK_F3) //TODO: customization to these keybinds
        global::unload = true;*/

    //if (GetAsyncKeyState(VK_F6)) { //Convenient way for me when checking accs, to go to their worlds from cli
    //    auto cboard = utils::get_clipboard();
    //    if (cboard.find("error") != -1)
    //        printf("Couldn't get clipboard data.\n");
    //    else {
    //        std::string packet = "action|join_request\nname|" + cboard + "\ninvitedWorld|0";
    //        gt::sendlog("Warping to " + cboard);
    //        gt::send(NET_MESSAGE_GAME_MESSAGE, packet, false);
    //    }
    //}
    //if (GetAsyncKeyState(VK_F5)) {
    //    std::string packet = "action|input\n|text|/go";
    //    gt::send(2, packet, true);
    //}
    //if (GetAsyncKeyState(VK_F7)) {
    //    std::string packet = "action|respawn\n";
    //    gt::send(2, packet, true);
    //}
    //if (GetAsyncKeyState(VK_F8)) {
    //    std::string packet = "action|enter_game\n";
    //    gt::send(2, packet, true);
    //}
    if (GetAsyncKeyState(VK_F5) & 1) {
        if (opt::fast_trash == false) {
            opt::fast_trash = true;
            variantlist_t va{ "OnTextOverlay" };
            va[1] = "`2Enabled fast trash.";
            gt::send_varlist_self(va);
        }
        else {
            opt::fast_trash = false;
            variantlist_t va{ "OnTextOverlay" };
            va[1] = "`4Disabled fast trash.";
            gt::send_varlist_self(va);
        }
    }
    if (GetAsyncKeyState(VK_F6) & 1) {
        if (opt::fast_drop == false) {
            opt::fast_drop = true;
            variantlist_t va{ "OnTextOverlay" };
            va[1] = "`2Enabled fast drop.";
            gt::send_varlist_self(va);
        }
        else {
            opt::fast_drop = false;
            variantlist_t va{ "OnTextOverlay" };
            va[1] = "`4Disabled fast drop.";
            gt::send_varlist_self(va);
        }
    }
    if (GetAsyncKeyState(VK_F7) & 1) {
        if (opt::fast_ban) {
            opt::fast_ban = false;
            gt::sendlog("`4Disabled fast ban.");
        }
        if (opt::fast_pull) {
            opt::fast_pull = false;
            gt::sendlog("`4Disabled fast pull.");
        }
        if (opt::fast_kick == false) {
            opt::fast_kick = true;
            gt::sendlog("`2Enabled fast kick.");
        }
        else {
            opt::fast_kick = false;
            gt::sendlog("`4Disabled fast kick.");
        }
    }
    if (GetAsyncKeyState(VK_F8) & 1) {
        if (opt::fast_ban) {
            opt::fast_ban = false;
            gt::sendlog("`4Disabled fast ban.");
        }
        if (opt::fast_kick) {
            opt::fast_kick = false;
            gt::sendlog("`4Disabled fast kick.");
        }
        if (opt::fast_pull == false) {
            opt::fast_pull = true;
            gt::sendlog("`2Enabled fast pull.");
        }
        else {
            opt::fast_pull = false;
            gt::sendlog("`4Disabled fast pull.");
        }
    }
    if (GetAsyncKeyState(VK_F9) & 1) {
        if (opt::fast_pull) {
            opt::fast_pull = false;
            gt::sendlog("`4Disabled fast pull.");
        }
        if (opt::fast_kick) {
            opt::fast_kick = false;
            gt::sendlog("`4Disabled fast kick.");
        }
        if (opt::fast_ban == false) {
            opt::fast_ban = true;
            gt::sendlog("`2Enabled fast ban.");
        }
        else {
            opt::fast_ban = false;
            gt::sendlog("`4Disabled fast ban.");
        }
    }
    //kick pull ban
    if (GetAsyncKeyState(VK_F10) & 1) {
        if (opt::cheat::autocollect == false) {
            opt::cheat::autocollect = true;
            variantlist_t va{ "OnTextOverlay" };
            va[1] = "`2Enabled auto collect.";
            gt::send_varlist_self(va);
        }
        else {
            opt::cheat::autocollect = false;
            variantlist_t va{ "OnTextOverlay" };
            va[1] = "`4Disabled auto collect.";
            gt::send_varlist_self(va);
        }
    }
    //if (GetAsyncKeyState(VK_F11)) {
    //    if (opt::gg== false) {
    //        opt::gg = true;
    //        variantlist_t va{ "OnTextOverlay" };
    //        va[1] = "`2Teleporting to items.";
    //        gt::send_varlist_self(va);
    //    }
    //    else {
    //        opt::gg = false;
    //        variantlist_t va{ "OnTextOverlay" };
    //        va[1] = "`4Disabled teleports.";
    //        gt::send_varlist_self(va);
    //    }
    //}
    return CallWindowProcW(hooks::wndproc, wnd, msg, wparam, lparam);
}

void __cdecl hooks::SendPacketRaw(int type, GameUpdatePacket* packet, int size, void* packetsender, ENetPeer* peer, int flag) {
    SendPacketRawHook::Execute(type, packet, size, packetsender, peer, flag);
}

void __cdecl hooks::HandleTouch(LevelTouchComponent* touch, CL_Vec2f pos, bool started) {
    static auto orig = decltype(&hooks::HandleTouch)(hookmgr->orig(sig::handletouch));

    //TODO: WorldCamera::WorldToScreen to check if pos is within imgui menu when global::draw

    auto rend = sdk::GetGameLogic()->renderer;
    if (rend && global::draw && active) {
        //   auto screen = rend->GetCamera()->WorldToScreen(pos);
        //   auto screen = rend->GetCamera()->WorldToScreen(pos);

        auto screen = rend->GetCamera()->WorldToScreen(pos);

        //     auto pos = ImGui::GetWindowPos();
        //   auto size = ImGui::GetWindowSize();
        //     auto pos = ImGui::GetWindowPos();
        //   auto size = ImGui::GetWindowSize();
        //imgui cant call these during this time so just save them somewhere when not lazy

        /* if (screen.x >= pos.x && screen.x <= (pos.x + size.x) && screen.y >= pos.y && screen.y <= (pos.y + size.y)) {
        /* if (screen.x >= pos.x && screen.x <= (pos.x + size.x) && screen.y >= pos.y && screen.y <= (pos.y + size.y)) {
            printf("good for us?\n");
        }*/
        //  return;
        //  return;
    }
    auto logic = sdk::GetGameLogic();
    auto tilemap = logic->GetTileMap();
    opt::cheat::TouchPos.x = pos.x / 32;
    opt::cheat::TouchPos.y = pos.y / 32;
    if (GetAsyncKeyState(VK_SHIFT) && logic->GetLocalPlayer()) {
        if (!opt::cheat::TouchPos.x == 0 && !opt::cheat::TouchPos.y == 0) {
            std::thread t(&testomegalul);
            opt::cheat::PathFindinglol = true;
            t.detach();
        }
    }
    auto draw = ImGui::GetBackgroundDrawList();
    if (opt::cheat::tp_click && GetAsyncKeyState(VK_CONTROL)) {
        //localplayer is guaranteed to be a valid pointer here according to xrefs
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        auto new_pos = pos - (local->GetSize() / 2.f);
        local->SetPos(new_pos);
        pos = new_pos;
    }
    if (GetAsyncKeyState(VK_CONTROL)) {
        return;
    }
    if (!(GetAsyncKeyState(VK_CONTROL) && opt::cheat::autopunchplayer)) {
        orig(touch, pos, started);
    }
    if (GetAsyncKeyState(VK_F1)) {
        static bool mod_fly = false;
        if (mod_fly) { // aob ("73 19 80 63 13 FE C7 43 1C 00 00 00 00 F3 0F 10 05 ?? ?? ?? 00 48 83 C4 30 5B C3 F6 43 13 01")
            uintptr_t ad_mod_fly = 0x8C69A7; // address 
            if (utils::patch_bytes<2>(global::gt + ad_mod_fly, "\x90\x90"));// enable
        }
        if (!mod_fly) {
            uintptr_t ad_mod_fly = 0x8C2A02;
            if (utils::patch_bytes<2>(global::gt + ad_mod_fly, "\x74\x5D")); // disable
        }
    }
        //orig(touch, pos, started);
}

void __cdecl hooks::WorldCamera_OnUpdate(WorldCamera* camera, CL_Vec2f unk, CL_Vec2f unk2) {
    static auto orig = decltype(&hooks::WorldCamera_OnUpdate)(hookmgr->orig(sig::worldcamera_onupdate));
    if (opt::cheat::tp_click && GetAsyncKeyState(VK_CONTROL)) //if we dont do this then there is major sliding when teleporting.
        return;
    if (GetAsyncKeyState(VK_SHIFT)) //if we dont do this then there is major sliding when teleporting.
        return;

    orig(camera, unk, unk2);
}

void __cdecl hooks::SendPacket(int type, std::string& packet, ENetPeer* peer) {
    SendPacketHook::Execute(type, packet, peer);
}



void __cdecl hooks::ProcessTankUpdatePacket(GameLogic* logic, GameUpdatePacket* packet) {
    ProcessTankUpdatePacketHook::Execute(logic, packet);
}

bool __cdecl hooks::CanSeeGhosts(int id) {
    static auto orig = decltype(&hooks::CanSeeGhosts)(hookmgr->orig(sig::canseeghosts));
    if (opt::cheat::see_ghosts)
        return true;
    return orig(id);
}

void __cdecl hooks::NetHTTP_Update(NetHTTP* http) {
    static auto orig = decltype(&hooks::NetHTTP_Update)(hookmgr->orig(sig::nethttp_update));
    //if (http->m_serverName.find("iap-mob.ubi.com") != -1) //block ubisoft iap http spam shit.
    //    return;

    //we dont know if its gt1 or gt2, so lets just do both, fuck the performance.
    if (opt::custom_server_on && http->m_serverName.find("growtopia") != -1) {
        utils::replace(http->m_serverName, "growtopia2.com", opt::custom_server_val);
        utils::replace(http->m_serverName, "growtopia1.com", opt::custom_server_val);
    }

    orig(http);
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) {
    static auto orig = decltype(&hooks::EndScene)(endscene);
    menu::EndScene(device, active); //Imgui happens here
    return orig(device);
}

//ideal hook for all kinds of continuous loops and also conveniently gets us app
void __cdecl hooks::App_Update(App* app) {
    UpdateManager::Execute(app);
}

void __cdecl hooks::TileExtra_Serialize(TileExtra* te, uint8_t* mem, uint32_t* pos, bool save1, World* world, bool save2, Tile* target, int version) { // fixed 3.64 crash
    static auto orig = decltype(&hooks::TileExtra_Serialize)(hookmgr->orig(sig::tileextra_serialize));
    orig(te, mem, pos, save1, world, save2, target, version);
    if (te->type == 3 && target->tile_extra) {
        printf("\n\n[Tile access list]\nID: %d\n", target->foreground);

        utils::printc("96", "Owner: %d", target->tile_extra->owner);
        for (auto id : target->tile_extra->access_list)
            utils::printc("92", "\t%d", id);

        printf("\n");
    }
}

void __cdecl hooks::OnPunched(NetAvatar* local, CL_Vec2f pos, NetAvatar* puncher) {
    static auto orig = decltype(&hooks::OnPunched)(hookmgr->orig(sig::onpunched));

    if (!opt::cheat::antipunch)
        orig(local, pos, puncher);
}

float __cdecl hooks::GetFruitBloomProgressPercent(Tile* tile) {
    static auto orig = decltype(&hooks::GetFruitBloomProgressPercent)(hookmgr->orig(sig::getfruitbloompercent));

    if (opt::cheat::tree)
        return opt::cheat::tree_size; //more than 1.0 is possible and makes trees bigger than normal, ig you can add it if you are interested
    else
        orig(tile);

    if (opt::cheat::see_fruits)
        return 1.0f; //more than 1.0 is possible and makes trees bigger than normal, ig you can add it if you are interested
    else
        return orig(tile);
}

bool __cdecl hooks::DialogIsOpened(GameLogic* logic) {
    static auto orig = decltype(&hooks::DialogIsOpened)(hookmgr->orig(sig::dialogisopened));

    if (opt::cheat::dialog_cheat)
        return false;
    else
        return orig(logic);
}
