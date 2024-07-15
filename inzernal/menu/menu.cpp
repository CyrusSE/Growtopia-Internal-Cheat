#pragma once
#include <core/utils.h>
#include <menu/menu.h>
#include "core/globals.h"
#include "proton/Math/rtRect.h"
#include <sdk/sdk.h>
#include <hooks/hooks.h>
#include <hooks/SendPacketRaw.h>
#include <hooks/SendPacket.h>
#include <thread>

int rmode = 1;
int firs = 255;
int seco = 0;
int thir = 0;
ImColor ESPColor;
void menu::animate() {
    static bool direction = false;

    //most imgui controls have a custom added fading effect
    global::fade += direction ? -0.003f : 0.003f;
    if (global::fade <= 0.0f) {
        direction = !direction;
        global::fade = 0.0f;
    }
    else if (global::fade >= 0.3f) {
        direction = !direction;
        global::fade = 0.3f;
    }
}



void autocollect() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        auto draw = ImGui::GetBackgroundDrawList();
        auto logic = sdk::GetGameLogic();
        auto tilemap = logic->GetTileMap();
        auto pos2f = local->GetPos();
        auto objmap = logic->GetObjectMap();
        auto rend = sdk::GetGameLogic()->renderer;

        CL_Vec2f localposfirst;
        localposfirst.x = local->GetPos().x + opt::cheat::range_val * 32;
        localposfirst.y = local->GetPos().y + opt::cheat::range_val * 32;
        auto LocalScreenFirst = rend->GetCamera()->WorldToScreen(localposfirst);
        CL_Vec2f localpossec;
        localpossec.x = local->GetPos().x - opt::cheat::range_val * 32;
        localpossec.y = local->GetPos().y - opt::cheat::range_val * 32;
        auto LocalScreenSec = rend->GetCamera()->WorldToScreen(localpossec);
        draw->AddRect(ImVec2(LocalScreenFirst.x, LocalScreenFirst.y), ImVec2(LocalScreenSec.x, LocalScreenSec.y), ImColor(rand() % 255, rand() % 255, rand() % 255), 1000.0f,
            0, 2.0f);
        float tilex = pos2f.x / 32;
        float tiley = pos2f.y / 32;
        for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it) {
            if (utils::isInside(it->pos.x, it->pos.y, opt::cheat::range_val * 32, pos2f.x, pos2f.y)) {
                static types::time times = std::chrono::system_clock::now();
                auto mf = it->pos;
                float mf1 = mf.x;
                float mf2 = mf.y;
                //Tile* tile = tilemap->GetTileSafe(mf1 / 32, mf2 / 32);
                if (opt::cheat::filterautocollect) {
                    if (it->item_id == opt::cheat::itemfilter) {
                        GameUpdatePacket packet{ 0 };
                        packet.pos_x = it->pos.x;
                        packet.pos_y = it->pos.y;
                        packet.type = 11;
                        packet.netid = -1;
                        packet.object_id = it->object_id;

                        SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                    }
                }
                else {
                    GameUpdatePacket packet{ 0 };
                    packet.pos_x = it->pos.x;
                    packet.pos_y = it->pos.y;
                    packet.type = 11;
                    packet.netid = -1;
                    packet.object_id = it->object_id;

                    SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                }
            }
        }
    }
}
void tptoitem() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        auto logic = sdk::GetGameLogic();
        auto tilemap = logic->GetTileMap();
        auto pos2f = local->GetPos();
        auto objmap = logic->GetObjectMap();
        auto rend = sdk::GetGameLogic()->renderer;

        float tilex = pos2f.x / 32;
        float tiley = pos2f.y / 32;
        for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it) {
            if (utils::isInside(it->pos.x, it->pos.y, opt::cheat::range_val * 32, pos2f.x, pos2f.y)) {
                static types::time times = std::chrono::system_clock::now();
                auto mf = it->pos;
                float mf1 = mf.x;
                float mf2 = mf.y;
                Tile* tile = tilemap->GetTileSafe(mf1 / 32, mf2 / 32);
                if (tile) {
                    if (tile->foreground == 0) {
                        GameUpdatePacket packet{ 0 };
                        packet.pos_x = it->pos.x;
                        packet.pos_y = it->pos.y;
                        packet.type = 11;
                        packet.netid = -1;
                        packet.object_id = it->object_id;
                        SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        local->SetPos(it->pos.x, it->pos.y);
                    } 
                    if (tile->foreground != 0) {
                        if (tile->flags != 0) {
                            float mf1s = mf.x + 32;
                            float mf2s = mf.y;
                            Tile* tiles = tilemap->GetTileSafe(mf1s / 32, mf2s / 32);
                            if (tiles->flags != 0) {
                                GameUpdatePacket packet{ 0 };
                                packet.pos_x = it->pos.x;
                                packet.pos_y = it->pos.y;
                                packet.type = 11;
                                packet.netid = -1;
                                packet.object_id = it->object_id;
                                SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                            }
                        }
                        if (tile->flags != 256 && tile->flags != 0) {
                            GameUpdatePacket packet{ 0 };
                            packet.pos_x = it->pos.x;
                            packet.pos_y = it->pos.y;
                            packet.type = 11;
                            packet.netid = -1;
                            packet.object_id = it->object_id;
                            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        }
                        if (tile->flags != 18 && tile->flags != 0) {
                            GameUpdatePacket packet{ 0 };
                            packet.pos_x = it->pos.x;
                            packet.pos_y = it->pos.y;
                            packet.type = 11;
                            packet.netid = -1;
                            packet.object_id = it->object_id;
                            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        }
                        if (tile->flags != 50 && tile->flags != 0) {
                            GameUpdatePacket packet{ 0 };
                            packet.pos_x = it->pos.x;
                            packet.pos_y = it->pos.y;
                            packet.type = 11;
                            packet.netid = -1;
                            packet.object_id = it->object_id;
                            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        }
                        if (tile->flags != 24 && tile->flags != 0) {
                            GameUpdatePacket packet{ 0 };
                            packet.pos_x = it->pos.x;
                            packet.pos_y = it->pos.y;
                            packet.type = 11;
                            packet.netid = -1;
                            packet.object_id = it->object_id;
                            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        }
                        if (tile->flags != 128 && tile->flags != 0) {
                            GameUpdatePacket packet{ 0 };
                            packet.pos_x = it->pos.x;
                            packet.pos_y = it->pos.y;
                            packet.type = 11;
                            packet.netid = -1;
                            packet.object_id = it->object_id;
                            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        }
                        if (tile->flags != 2 && tile->flags != 0) {
                            GameUpdatePacket packet{ 0 };
                            packet.pos_x = it->pos.x;
                            packet.pos_y = it->pos.y;
                            packet.type = 11;
                            packet.netid = -1;
                            packet.object_id = it->object_id;
                            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        }
                    }
                }
            }
        }
    }
}
void doAutofarm() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        auto logic = sdk::GetGameLogic();
        auto tilemap = logic->GetTileMap();
        bool isLeft = false;
        if (local->flags = local->facing_left) {
            isLeft = true;
        }
        auto tile = local->GetPos() / 32;
        static types::time timerlol = std::chrono::system_clock::now();
        static int mfoffsets = isLeft ? -1 : 1;
        if (utils::run_at_interval(timerlol, 0.30)) {
            mfoffsets += isLeft ? -1 : 1;
            if (mfoffsets > 2 || mfoffsets < -2)
                mfoffsets = isLeft ? -1 : 1;
        }
        Tile* tilepog = tilemap->GetTileSafe(tile.x + mfoffsets, tile.y);


        GameUpdatePacket legitpacket{ 0 };
        legitpacket.type = PACKET_STATE;
        legitpacket.vec_x = local->pos.x;
        legitpacket.vec_y = local->pos.y;

        if (tilepog->background == 0 && tilepog->foreground == 0) {
            GameUpdatePacket placepacket{ 0 };
            placepacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
            placepacket.int_data = opt::cheat::itemid_val; // magplant remote id
            placepacket.vec_x = local->GetPos().x;
            placepacket.vec_y = local->GetPos().y;
            placepacket.int_x = tilepog->position_x;
            placepacket.int_y = tilepog->position_y;

            legitpacket.int_data = opt::cheat::itemid_val;
            legitpacket.int_x = tilepog->position_x;
            legitpacket.int_y = tilepog->position_y;
            legitpacket.flags = (1 << 5) | (1 << 10) | (1 << 11);

            static types::time time = std::chrono::system_clock::now();
            if (utils::run_at_interval(time, 0.20)) {
                gt::send(&legitpacket);
                SendPacketRawHook::Execute(4, &placepacket, 84, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
            }
        }
        else {
            GameUpdatePacket punchpacket{ 0 };
            punchpacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
            punchpacket.int_data = 18;                     // punch id
            punchpacket.vec_x = local->GetPos().x;
            punchpacket.vec_y = local->GetPos().y;
            punchpacket.int_x = tilepog->position_x;
            punchpacket.int_y = tilepog->position_y;

            legitpacket.int_data = 18;
            legitpacket.int_x = tilepog->position_x;
            legitpacket.int_y = tilepog->position_y;
            legitpacket.flags = 2560;

            static types::time timepunch = std::chrono::system_clock::now();
            if (utils::run_at_interval(timepunch, 0.20)) {
                gt::send(&legitpacket);
                SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
            }
        }
        auto draw = ImGui::GetBackgroundDrawList();
        auto rend = sdk::GetGameLogic()->renderer;
        CL_Vec2f blocktop;
        blocktop.x = tilepog->position_x * 32;
        blocktop.y = tilepog->position_y * 32;
        auto screenBlockTOP = rend->GetCamera()->WorldToScreen(blocktop);
        CL_Vec2f blockbottom;
        blockbottom.x = tilepog->position_x * 32;
        blockbottom.y = tilepog->position_y * 32;
        blockbottom.x = blockbottom.x + 32;
        blockbottom.y = blockbottom.y + 32;
        auto screenBlockBottom = rend->GetCamera()->WorldToScreen(blockbottom);
        draw->AddRectFilled(ImVec2(screenBlockTOP.x, screenBlockTOP.y), ImVec2(screenBlockBottom.x, screenBlockBottom.y), ImColor(255, 0, 0, 100));
    }
    else {
        opt::cheat::autofarm = false;
    }
}
void doNoname() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local && opt::cheat::noname) {
        //gt::send(3, "action|quit_to_exit");
        local->name = "````" + opt::cheat::name;
    }

}
void playeresp() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        auto draw = ImGui::GetBackgroundDrawList();

        auto locals = sdk::GetGameLogic()->GetNetObjMgr();
        auto logic = sdk::GetGameLogic();
        auto pos2f = local->GetPos();
        auto objmap = logic->GetObjectMap();
        auto tilemap = logic->GetTileMap();
        auto rend = sdk::GetGameLogic()->renderer;

        float tilex = pos2f.x / 32;
        float tiley = pos2f.y / 32;
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
#pragma region shit lot
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            const char* namechar = name2.c_str();
            CL_Vec2f player;
            player.x = pair.second->pos.x - 8;
            player.y = pair.second->pos.y - 8;
            auto ScreenPlayer = rend->GetCamera()->WorldToScreen(player);
            CL_Vec2f playerline;
            playerline.x = pair.second->pos.x + 10;
            playerline.y = pair.second->pos.y - 8;
            auto ScreenPlayerLine = rend->GetCamera()->WorldToScreen(playerline);
            CL_Vec2f playername;
            playername.x = pair.second->pos.x + 5;
            playername.y = pair.second->pos.y + 30;
            auto PlayerNameScreen = rend->GetCamera()->WorldToScreen(playername);
            CL_Vec2f LocalPlayer;
            LocalPlayer.x = local->GetPos().x + 10;
            LocalPlayer.y = local->GetPos().y - (local->GetSize().y / 2);
            auto ScreenLocal = rend->GetCamera()->WorldToScreen(LocalPlayer);
            CL_Vec2f foot;
            foot.x = pair.second->pos.x + pair.second->GetSize().x + 5;
            foot.y = pair.second->pos.y + pair.second->GetSize().y + 5;
            auto footcam = rend->GetCamera()->WorldToScreen(foot);
#pragma endregion
            if (pair.second != local) {
                if (utils::isInside(pair.second->pos.x, pair.second->pos.y, 3 * 32, pos2f.x, pos2f.y)) {
                    draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ScreenPlayerLine.x, ScreenPlayerLine.y), ESPColor, 2.0f);
                    draw->AddRect(ImVec2(ScreenPlayer.x, ScreenPlayer.y), ImVec2(footcam.x, footcam.y), ImColor(25, 0, 0), 5.5f, 15, 2.0f);
                }
                else {
                    draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ScreenPlayerLine.x, ScreenPlayerLine.y), ESPColor, 1.0f);
                    draw->AddRect(ImVec2(ScreenPlayer.x, ScreenPlayer.y), ImVec2(footcam.x, footcam.y), ImColor(0, 255, 0), 5.5f, 15, 2.0f);
                }
            }
            draw->AddText(ImVec2(PlayerNameScreen.x, PlayerNameScreen.y), ESPColor, namechar);
        }
    }
}
void itemesp() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        auto draw = ImGui::GetBackgroundDrawList();

        auto locals = sdk::GetGameLogic()->GetNetObjMgr();
        auto logic = sdk::GetGameLogic();
        auto pos2f = local->GetPos();
        auto objmap = logic->GetObjectMap();
        auto tilemap = logic->GetTileMap();
        auto rend = sdk::GetGameLogic()->renderer;

        float tilex = pos2f.x / 32;
        float tiley = pos2f.y / 32;
        for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it) {
            CL_Vec2f item;
            item.x = it->pos.x - 3;
            item.y = it->pos.y - 3;
            CL_Vec2f itemy;
            itemy.x = it->pos.x + 19;
            itemy.y = it->pos.y + 19;
            auto ItemXYScreen = rend->GetCamera()->WorldToScreen(item);
            auto ItemWHScreen = rend->GetCamera()->WorldToScreen(itemy);
            CL_Vec2f LocalPlayer;
            LocalPlayer.x = logic->local->GetPos().x + 8;
            LocalPlayer.y = logic->local->GetPos().y;
            auto ScreenLocal = rend->GetCamera()->WorldToScreen(LocalPlayer);
            if (it->item_id != 112) {
                if (it->item_id == 242 || it->item_id == 202 || it->item_id == 204 || it->item_id == 206) {
                    draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImColor(555, 555, 555), 2.0f);
                    draw->AddRect(ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImVec2(ItemWHScreen.x, ItemWHScreen.y), ImColor(555, 555, 555), 5.5f, 15, 1.5f);
                }
                if (utils::isInside(it->pos.x, it->pos.y, opt::cheat::range_val * 32 + 10, pos2f.x, pos2f.y)) {
                    draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImColor(555, 555, 555), 1.5f);
                    draw->AddRect(ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImVec2(ItemWHScreen.x, ItemWHScreen.y), ImColor(555, 555, 555), 5.5f, 15, 1.5f);
                }
                else {
                    draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImColor(555, 555, 555), 1.0f);
                    draw->AddRect(ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImVec2(ItemWHScreen.x, ItemWHScreen.y), ImColor(555, 555, 555), 5.5f, 15, 1.5f);
                }
                if (it->item_id == 1796 || it->item_id == 2408) {
                    draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImColor(555, 555, 555), 3.0f);
                    draw->AddRect(ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImVec2(ItemWHScreen.x, ItemWHScreen.y), ImColor(555, 555, 555), 5.5f, 15, 1.5f);
                }
                std::string id = "Item ID: " + std::to_string(it->item_id);
                const char* idchar = id.c_str();
                draw->AddText(ImVec2(ItemXYScreen.x, ItemXYScreen.y - 15), ESPColor, idchar);
            }
        }
    }
}
void doSpamWater() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time times = std::chrono::system_clock::now();
        static types::time timer = std::chrono::system_clock::now();
        auto locals = sdk::GetGameLogic()->GetNetObjMgr();
        auto it = sdk::GetGameLogic()->GetNetObjMgr()->players.begin();
        std::advance(it, rand() % sdk::GetGameLogic()->GetNetObjMgr()->players.size());
        GameUpdatePacket packet{ 0 };
        auto pos = local->GetPos();
        int x = local->GetPos().x / 32;
        int y = local->GetPos().y / 32;
        packet.type = PACKET_STATE;
        std::vector<int> A;
        A = { 2242, 2244, 2246, 2248, 2250 };
        if (opt::cheat::crystalmode && !opt::cheat::randommodepacketstate) {
            packet.int_data = A.at(utils::random(0, 4));
        }
        if (opt::cheat::randommodepacketstate && !opt::cheat::crystalmode) {
            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            packet.int_data = 1 + rand() % 255;
        }
        if (!opt::cheat::randommodepacketstate && !opt::cheat::crystalmode)
        {
            packet.int_data = opt::cheat::watervalue;
        }
        static int poggers;
        packet.int_x = (it->second->pos.x / 32) + utils::random(-1, 1);
        packet.int_y = (it->second->pos.y / 32) + utils::random(-1, 1);
        std::vector<int> flags;
        flags = { 32, 48 };
        poggers = flags.at(rand() % 2);
        //if (utils::run_at_interval(timer, 0.5)) {
        //    poggers = flags.at(rand() % 2);
        //}
        packet.flags = poggers | (1 << 10) | (1 << 11); //no enum rn so using raw values

        //Punch: 2560
        //Respawn: 2308   show

        packet.pos_x = pos.x;
        packet.pos_y = pos.y;
        SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        Sleep(20);
        //if (utils::run_at_interval(times, 0.026)) {
        //    SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        //}

    }
}
void doCHECKTILES() {
    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto tilemap = logic->GetTileMap();
    auto w2s = logic->GetWorldRenderer()->camera;
    if (opt::cheat::testmftiles) {
        if (local) {
            auto draw = ImGui::GetBackgroundDrawList();
            auto LOCALSCREEN = w2s.WorldToScreen(local->GetPos() + (local->GetSize() / 2.f));
            auto tilepos = local->GetPos() / 32;
            static int goalx = 32;
            static int goaly = 37;
            static types::time timerlol = std::chrono::system_clock::now();
            for (auto& tile : tilemap->tiles) {
                CL_Vec2f screenfirst;
                CL_Vec2f screensecond;
                screenfirst.x = tile.position_x * 32;
                screenfirst.y = tile.position_y * 32;
                screensecond.x = tile.position_x * 32 + 32;
                screensecond.y = tile.position_y * 32 + 32;
                auto screen = w2s.WorldToScreen(screenfirst);
                auto screen2 = w2s.WorldToScreen(screensecond);
                if (tile.foreground) {
                    std::string id = "ID " + std::to_string(tile.foreground);
                    const char* idchar = id.c_str();
                    std::string id2 = "X " + std::to_string(tile.position_x);
                    const char* idchar2 = id2.c_str();
                    std::string id3 = "Y " + std::to_string(tile.position_y);
                    const char* idchar3 = id3.c_str();
                    draw->AddText(ImVec2(screen.x + 3, screen.y + 0), ImColor(0, 0, 0), idchar);
                    draw->AddText(ImVec2(screen.x + 3, screen.y + 15), ImColor(0, 0, 0), idchar2);
                    draw->AddText(ImVec2(screen.x + 3, screen.y + 30), ImColor(0, 0, 0), idchar3);
                    draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 0, 0));
                    if (tile.foreground != 6 && tile.foreground != 410 && tile.foreground != 20 && tile.foreground != 26 && tile.foreground != 608 &&
                        tile.foreground != 780 && tile.foreground != 102 && tile.foreground != 1222 && tile.foreground != 4422 && tile.foreground != 2978 &&
                        tile.foreground != 3524 && tile.foreground != 762 && tile.foreground != 4632 && tile.foreground != 1308 && tile.foreground != 2862 &&
                        tile.foreground != 4798 && tile.foreground != 9740 && tile.foreground != 4240 && tile.foreground != 846 && tile.foreground != 5616 &&
                        tile.foreground != 9198 && tile.foreground != 1528 && tile.foreground != 3002 && tile.foreground != 430 && tile.foreground != 5036 &&
                        tile.foreground != 7444 && tile.foreground != 482 && tile.foreground != 12 && tile.foreground != 22 && tile.foreground != 16 &&
                        tile.foreground != 954 && tile.foreground != 3898 && tile.foreground != 1042 && tile.foreground != 6854 && tile.foreground != 8260 &&
                        tile.foreground != 5666 && tile.foreground != 340 && tile.foreground != 4722 && tile.foreground != 598 && tile.foreground != 2810 &&
                        tile.foreground != 554 && tile.foreground != 60 && tile.foreground != 1450 && tile.foreground != 8682 && tile.foreground != 5054 &&
                        tile.foreground != 4202 && tile.foreground != 3804 && tile.foreground != 4482 && tile.foreground != 1104 && tile.foreground != 4782 &&
                        tile.foreground != 3806 && tile.foreground != 2948 && tile.foreground != 8676 && tile.foreground != 858 && tile.foreground != 4740 &&
                        tile.foreground != 3796 && tile.foreground != 1700 && tile.foreground != 3232 && tile.foreground != 3572 && tile.foreground != 3566 &&
                        tile.foreground != 484 && tile.foreground != 5034 && tile.foreground != 546 && tile.foreground != 5032 && tile.foreground != 1446 &&
                        tile.foreground != 1604 && tile.foreground != 7164 && tile.foreground != 5040 && tile.foreground != 1684 && tile.foreground != 1702 &&
                        tile.foreground != 4704 && tile.foreground != 4706 && tile.foreground != 2072 && tile.foreground != 1162 && tile.foreground != 1240 &&
                        tile.foreground != 1770 && tile.foreground != 1420 && tile.foreground != 2586 && tile.foreground != 1422 && tile.foreground != 3522 &&
                        tile.foreground != 658 && tile.foreground != 428 && tile.foreground != 224 && tile.foreground != 4720 && tile.foreground != 998 &&
                        tile.foreground != 1326 && tile.foreground != 1752 && tile.foreground != 1324 && tile.foreground != 1682 && tile.foreground != 30 &&
                        tile.foreground != 4794 && tile.foreground != 4546 && tile.foreground != 3808 && tile.foreground != 10076 && tile.foreground != 10078 &&
                        tile.foreground != 2252 && tile.foreground != 2274 && tile.foreground != 62 && tile.foreground != 480 && tile.foreground != 24 &&
                        tile.foreground != 5660 && tile.foreground != 8020 && tile.foreground != 2244 && tile.foreground != 2242 && tile.foreground != 2246 &&
                        tile.foreground != 2248 && tile.foreground != 190 && tile.foreground != 192 && tile.foreground != 188 && tile.foreground != 758 &&
                        tile.foreground != 1256 && tile.foreground != 5468 && tile.foreground != 3072 && tile.foreground != 4352 && tile.foreground != 2272 &&
                        tile.foreground != 8994 && tile.foreground != 1482 && tile.foreground != 379 && tile.foreground != 10776 && tile.foreground != 9614 &&
                        tile.foreground != 886 && tile.foreground != 486 && tile.foreground != 1048 && tile.foreground != 3286 && tile.foreground != 3496 &&
                        tile.foreground != 3832 && tile.foreground != 756 && tile.foreground != 1436 && tile.foreground != 9268 && tile.foreground != 8634 &&
                        tile.foreground != 7362 && tile.foreground != 550 && tile.foreground != 286 && tile.foreground != 444 && tile.foreground != 2964 &&
                        tile.foreground != 1288 && tile.foreground != 854 && tile.foreground != 9330 && tile.foreground != 7984 && tile.foreground != 7888 &&
                        tile.foreground != 652 && tile.foreground != 1108 && tile.foreground != 1594 && tile.foreground != 398 && tile.foreground != 1434 &&
                        tile.foreground != 1304 && tile.foreground != 926 && tile.foreground != 656 && tile.foreground != 544 && tile.foreground != 1698 &&
                        tile.foreground != 974 && tile.foreground != 1694 && tile.foreground != 2646 && tile.foreground != 2068 && tile.foreground != 688 &&
                        tile.foreground != 1432 && tile.foreground != 9080 && tile.foreground != 686 && tile.foreground != 3126 && tile.foreground != 4698 &&
                        tile.foreground != 986 && tile.foreground != 4712 && tile.foreground != 5792 && tile.foreground != 6146 && tile.foreground != 5798 &&
                        tile.foreground != 3794 && tile.foreground != 6772 && tile.foreground != 1696 && tile.foreground != 4718 && tile.foreground != 596) {
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 0, 255));
                    }
                    if (tile.foreground == 596 && tile.flags == 32) { //Chest
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 596 && tile.flags == 0) { //Chest
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 4712 && tile.flags == 64) { //Gorrila
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 3126 && tile.flags == 1) { //Dark Magic Barrier
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 3126 && tile.flags == 0) { //Dark Magic Barrier
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 3126 && tile.flags == 32) { //Dark Magic Barrier
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 686 && tile.flags == 1) { //Jail Door
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 686 && tile.flags == 0) { //Jail Door
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 686 && tile.flags == 32) { //Jail Door
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 3286 && tile.flags == 1024) { //Steam Door
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 3286 && tile.flags == 1056) { //Steam Door
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 3798 && tile.flags == 1) { //Vip Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 3798 && tile.flags == 0) { //Vip Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 3798 && tile.flags == 32) { //Vip Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 4352 && tile.flags == 0) { //Wolf Gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 4352 && tile.flags == 32) { //Wolf Gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 224 && tile.flags == 0) { //House Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 224 && tile.flags == 32) { //House Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 224 && tile.flags == 48) { //House Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 224 && tile.flags == 24) { //House Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 224 && tile.flags == 288) { //House Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 4240 && tile.flags == 0) { //Jade Portcullis
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 4240 && tile.flags == 32) { //Jade Portcullis
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 4240 && tile.flags == 24) { //Jade Portcullis
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 1162 && tile.flags == 0) { //Forcefield
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 1162 && tile.flags == 32) { //Forcefield
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 1162 && tile.flags == 24) { //Forcefield
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 4706 && tile.flags == 0) { //Adventure Barrier
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 7164 && tile.flags == 0) { //Red Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                        std::string id = "blocked";
                        const char* idchar = id.c_str();
                        draw->AddText(ImVec2(screen.x + 15, screen.y + 5), ESPColor, idchar);
                    }
                    if (tile.foreground == 7164 && tile.flags == 32) { //Red Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                        std::string id = "blocked";
                        const char* idchar = id.c_str();
                        draw->AddText(ImVec2(screen.x + 15, screen.y + 5), ESPColor, idchar);
                    }
                    if (tile.foreground == 7164 && tile.flags == 24) { //Red Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                        std::string id = "blocked";
                        const char* idchar = id.c_str();
                        draw->AddText(ImVec2(screen.x + 15, screen.y + 5), ESPColor, idchar);
                    }
                    if (tile.foreground == 7164 && tile.flags == 16) { //Red Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                        std::string id = "blocked";
                        const char* idchar = id.c_str();
                        draw->AddText(ImVec2(screen.x + 15, screen.y + 5), ESPColor, idchar);
                    }
                    if (tile.foreground == 7164 && tile.flags > 1000) { //Red Entrance
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5032 && tile.flags == 32) { //Dirt Gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5032 && tile.flags == 0) { //Dirt Gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5032 && tile.flags == 24) { //Dirt Gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5032 && tile.flags == 16) { //Dirt Gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 60 && tile.flags == 0) { //portuculis
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 60 && tile.flags == 32) { //portuculis
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 60 && tile.flags == 288) { //portuculis
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 2810 && tile.flags == 0) { //air vent
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 2810 && tile.flags == 32) { //air vent
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 2810 && tile.flags == 24) { //air vent
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 598 && tile.flags == 0) { //dragon gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 598 && tile.flags == 32) { //dragon gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 598 && tile.flags == 16) { //dragon gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 598 && tile.flags == 128) { //dragon gate
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5036 && tile.flags == 0) { //hidden door 3 VV
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5036 && tile.flags == 32) { //hidden door 3 VV
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5036 && tile.flags == 24) { //hidden door 3 VV
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5036 && tile.flags == 50) { //hidden door 3 VV
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.foreground == 5036 && tile.flags > 1000) { //hidden door 3 VV
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(255, 0, 0));
                    }
                    if (tile.flags == 25) {
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.flags == 17) {
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.flags == 19) {
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.foreground == 1042 && tile.flags == 1) { //sumthing lol ahaha
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.foreground == 6854 && tile.flags == 1) { //sumthing lol ahaha
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.foreground == 20 && tile.flags == 1) { //sumthing lol ahaha
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.foreground == 428 && tile.flags == 1) { //sumthing lol ahaha
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.foreground == 430 && tile.flags == 1) { //sumthing lol ahaha
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.foreground == 546 && tile.flags == 1) { //sumthing lol ahaha
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.foreground == 544 && tile.flags == 1) { //sumthing lol ahaha
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    if (tile.foreground == 986 && tile.flags == 1) { //sumthing lol ahaha
                        draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                    else {
                        // draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ImColor(0, 255, 0));
                    }
                }
            }
        }
    }
}

void doNazi() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time times = std::chrono::system_clock::now();
        static types::time timer = std::chrono::system_clock::now();
        static int i = 0;
        if (utils::run_at_interval(times, 0.020)) {
            i += 1;
            if (i > 16)
                i = 0;
        }
        auto pos = local->GetPos();
        static int x, y;
        x = pos.x / 32, y = pos.y / 32;
        if (i == 0) //UrPos
            x += 0, y += 0;
        //-----------------
        if (i == 1) //Right
            x += 1, y += 0;
        if (i == 2) //Right
            x += 2, y += 0;
        if (i == 3) //Up
            x += 2, y -= 1;
        //Right Done
        //------------------
        if (i == 4) //Up
            x += 0, y -= 1;
        if (i == 5) //Up
            x += 0, y -= 2;
        if (i == 6) //Left
            x -= 1, y -= 2;
        //Up Done
        //-----------------
        if (i == 7) //Left
            x -= 1, y += 0;
        if (i == 8) //Left
            x -= 2, y += 0;
        if (i == 9) //Down
            x -= 2, y += 1;
        //Down Done
        //-----------------
        if (i == 10) //down
            x += 0, y += 1;
        if (i == 11) //down
            x += 0, y += 2;
        if (i == 12) //Left
            x += 1, y += 2;
        //ALL DONE
        //---------------------------------- CORNERS
        if (i == 13) //right up
            x += 2, y -= 2;
        if (i == 14) //left up
            x -= 2, y -= 2;
        if (i == 15) //Right down
            x += 2, y += 2;
        if (i == 16) //Left
            x -= 2, y += 2;


        //ImGui::Text("%d, %d", x, y);
        GameUpdatePacket packet{ 0 };
        packet.type = PACKET_STATE;
        packet.int_data = 2034;
        packet.int_x = x;
        packet.int_y = y;
        packet.pos_x = pos.x;
        packet.pos_y = pos.y;
        packet.flags = 32 | (1 << 10) | (1 << 11);
        if (utils::run_at_interval(timer, 0.020)) {
            gt::send(&packet);
            SendPacketRawHook::Execute(4, &packet, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        }
    }
}

void spamxp() { //test
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        if (local) {
            bool isLeft = false;
            if (local->flags = local->facing_left) {
                isLeft = true;
            }
            static types::time times = std::chrono::system_clock::now();
            static types::time timer = std::chrono::system_clock::now();
            static int i = 0;
            auto pos = local->GetPos();
            static int x, y;
            x = pos.x / 32, y = pos.y / 32;
            GameUpdatePacket packet{ 0 };
            packet.type = PACKET_TILE_CHANGE_REQUEST;
            packet.int_data = 1488;
            packet.int_x = x;
            packet.int_y = y;
            packet.pos_x = pos.x;
            packet.pos_y = pos.y;
            packet.flags = 2560;
            packet.flags = 32 | (1 << 10) | (1 << 11);
            if (utils::run_at_interval(timer, 0.080)) {
                gt::send(&packet);
                SendPacketRawHook::Execute(4, &packet, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
            }
    }
}

void autofish() { //test
    if (opt::autofish2) {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        bool isLeft = false;
        if (local->flags = local->facing_left) {
            isLeft = true;
        }
        static int i = 0;
        auto pos = local->GetPos();
        static int x, y;
        x = pos.x / 32, y = pos.y / 32;
        if (isLeft) {
            x -= 1, y += 1;
        }
        else {
            x += 1, y += 1;
        }
        GameUpdatePacket packet{ 0 };
        packet.type = PACKET_TILE_CHANGE_REQUEST;
        packet.int_data = opt::cheat::itemid_fish;
        packet.int_x = x;
        packet.int_y = y;
        packet.pos_x = pos.x;
        packet.pos_y = pos.y;
        packet.flags = 2560;
        packet.flags = 32 | (1 << 10) | (1 << 11);
        gt::send(&packet);
        SendPacketRawHook::Execute(4, &packet, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        opt::autofish2 = false;
    }
    }
}

void autoclear() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto logic = sdk::GetGameLogic();
    auto tilemap = logic->GetTileMap();
    auto abab = logic->GetTileMap();
    static int x = 0, y = 0;
    static Tile* supermario = tilemap->GetTileSafe(x, y);
    if (local) {
        for (auto& tile : abab->tiles) {
            if (tile.foreground != 8) {
                //if (tile.foreground == 2 || tile.background == 14 || tile.foreground == 10 || tile.foreground == 4) {
                if (tile.foreground == opt::cheat::blockid) {
                    GameUpdatePacket legitpacket{ 0 };
                    legitpacket.type = PACKET_STATE;
                    legitpacket.vec_x = local->pos.x;
                    legitpacket.vec_y = local->pos.y;

                    GameUpdatePacket punchpacket{ 0 };
                    punchpacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
                    punchpacket.int_data = 18;                     // punch id
                    punchpacket.vec_x = local->GetPos().x;
                    punchpacket.vec_y = local->GetPos().y;
                    punchpacket.int_x = tile.position_x;
                    punchpacket.int_y = tile.position_y;
                    legitpacket.int_data = 18;
                    legitpacket.int_x = tile.position_x;
                    legitpacket.int_y = tile.position_y;
                    legitpacket.flags = 2560;
                    static types::time timepunch = std::chrono::system_clock::now();
                    if (utils::run_at_interval(timepunch, 0.20)) {
                        gt::send(&legitpacket);
                        SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        //if (opt::tpclear == true) {
                        //    int yfloa = (tile.position_y - 1) * 32;
                        //    int xfloa = tile.position_x * 32;
                        //    local->SetPos(xfloa, yfloa);
                        //}
                    }
                }
            }
        }
    }
}

//void autosurg() {
//    auto local = sdk::GetGameLogic()->GetLocalPlayer();
//    auto logic = sdk::GetGameLogic();
//    auto tilemap = logic->GetTileMap();
//    auto abab = logic->GetTileMap();
//    static int x = 0, y = 0;
//    static Tile* supermario = tilemap->GetTileSafe(x, y);
//    if (local) {
//     if (opt::autosurgreal2)
//        for (auto& tile : abab->tiles) {
//            if (tile.foreground != 8) {
//                //if (tile.foreground == 2 || tile.background == 14 || tile.foreground == 10 || tile.foreground == 4) {
//                if (tile.foreground == 4296) {
//                //if (tile.foreground == opt::cheat::blockid) {
//                    GameUpdatePacket legitpacket{ 0 };
//                    legitpacket.type = PACKET_STATE;
//                    legitpacket.vec_x = local->pos.x;
//                    legitpacket.vec_y = local->pos.y;
//
//                    GameUpdatePacket punchpacket{ 0 };
//                    punchpacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
//                    punchpacket.int_data = 32;                     // punch id
//                    punchpacket.vec_x = local->GetPos().x;
//                    punchpacket.vec_y = local->GetPos().y;
//                    punchpacket.int_x = tile.position_x;
//                    punchpacket.int_y = tile.position_y;
//                    legitpacket.int_data = 32;
//                    legitpacket.int_x = tile.position_x;
//                    legitpacket.int_y = tile.position_y;
//                    legitpacket.flags = 2560;
//                    SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
//                    opt::autosurgreal2 = false;
//                    static types::time timepunch = std::chrono::system_clock::now();
//                    if (utils::run_at_interval(timepunch, 1)) {
//                        SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
//                    }
//                    //    gt::send(&legitpacket);
//                        //SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
//                        //if (opt::tpclear == true) {
//                        //    int yfloa = (tile.position_y - 1) * 32;
//                        //    int xfloa = tile.position_x * 32;
//                        //    local->SetPos(xfloa, yfloa);
//                        //}
//                    //}
//                }
//            }
//        }
//    }
//}

void doBlinkcolor() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time times = std::chrono::system_clock::now();
        std::vector<uint32_t> Skinlist;
        Skinlist = { 1348237567, 1685231359, 2022356223, 2190853119, 2527912447, 2864971775, 3033464831, 3370516479 };
        if (utils::run_at_interval(times, 0.2)) {
            string color = "action|setSkin\ncolor|" + std::to_string(Skinlist.at(utils::random(0, 7)));
            SendPacketHook::Execute(2, color, sdk::GetPeer());
            local->skin_color = Skinlist.at(utils::random(0, 7));
            //printf("changed");
        }
    }
}


void smokeplayer() {
    if (opt::smokeplayer) {
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        if (local) {
            static types::time timerlol = std::chrono::system_clock::now();
            static types::time times = std::chrono::system_clock::now();
            static types::time timer = std::chrono::system_clock::now();
            auto locals = sdk::GetGameLogic()->GetNetObjMgr();
            //for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it)
            auto it = sdk::GetGameLogic()->GetNetObjMgr()->players.begin();
            std::advance(it, rand() % sdk::GetGameLogic()->GetNetObjMgr()->players.size());

            if (it->second != local && utils::isInside(it->second->pos.x, it->second->pos.y, 200 * 32, local->pos.x, local->pos.y)) {
                GameUpdatePacket packet{ 0 };
                auto pos = local->GetPos();
                int x = local->GetPos().x / 32;
                int y = local->GetPos().y / 32;
                packet.type = PACKET_TILE_CHANGE_REQUEST;
                //packet.type = PACKET_STATE;
                packet.int_data = opt::cheat::watervalue;
                packet.int_x = it->second->pos.x / 32;
                packet.int_y = it->second->pos.y / 32;
                packet.flags = 32 | (1 << 10) | (1 << 11);
                packet.pos_x = pos.x;
                packet.pos_y = pos.y;
                //if (utils::run_at_interval(timerlol, opt::cheat::interval2)) {
               // gt::send(&packet);
                SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                Sleep(25);
                //}
            }
        }
    }
}


void dospam() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time timerlol = std::chrono::system_clock::now();
        if (opt::spam_text2 && opt::cheat::spamcolor) {
            if (utils::run_at_interval(timerlol, opt::cheat::interval2)) {
                gt::send(2, "action|input\n|text|`" + std::to_string(utils::random(1, 9)) + opt::cheat::spam2);
            }
        }
        else {
            if (opt::spam_text2) {
                if (utils::run_at_interval(timerlol, opt::cheat::interval2)) {
                    gt::send(2, "action|input\n|text|" + opt::cheat::spam2);
                }
            }
        }
    }
}

void sayit() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        gt::sendlog(opt::words);
        opt::sayit = false;
    }
}


void dospampacket() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time timerlol = std::chrono::system_clock::now();
        if (opt::spampacket) {
            if (utils::run_at_interval(timerlol, opt::cheat::interval2)) {
                while (utils::replace(opt::cheat::thepacket, ";;", "\n"));
                gt::send(opt::cheat::type, opt::cheat::thepacket);
                if (opt::duapacket) {
                    while (utils::replace(opt::cheat::thepacket2, ";;", "\n"));
                    gt::send(opt::cheat::type2, opt::cheat::thepacket2);
                }
            }
        }
    }
}

void dospamdrop() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time timerlol = std::chrono::system_clock::now();
        if (opt::spam_drop2) {
            if (utils::run_at_interval(timerlol, opt::cheat::interval2)) {
                gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + opt::cheat::itemid_nya2 + "|\ncount|" + "1" + "\n");
            }
        }
    }
    }

void doLagBack() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    menu::TouchdrawBox();
    if (local) {
        auto pos = local->GetPos();
        static types::time times = std::chrono::system_clock::now();
        GameUpdatePacket packet{ 0 };
        packet.type = PACKET_STATE;
        //packet.int_data = 822;
        packet.int_x = pos.x / 32;
        packet.int_y = pos.y / 32;
        packet.pos_x = pos.x;
        packet.pos_y = pos.y;
        packet.vec_x = pos.x;
        packet.vec_y = pos.y;
        packet.vec2_x = 2000;
        packet.vec2_y = 600;
        packet.flags = 32 | (1 << 10) | (1 << 11);
        if (utils::run_at_interval(times, 0.017)) {
            gt::send(&packet);
        }
    }
}


//colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.15f, 0.95f);
//colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
//colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
//colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
//colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
//colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
//colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
//colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
//colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
//colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
//colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
//colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
//colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
//colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
//colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
//colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
//colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
//colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
//colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
//colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
//colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
//colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
//colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
//colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
//colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
//colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
//colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
//colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
//colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
//colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
//colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
//colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
//colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
//colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
//colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
//colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
//colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
//colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
//colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
//colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
//colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
//colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
//colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
//style->FramePadding = ImVec2(4, 2);
//style->ItemSpacing = ImVec2(10, 2);
//style->IndentSpacing = 12;
//style->ScrollbarSize = 10;
//style->WindowRounding = 4;
//style->FrameRounding = 4;
//style->PopupRounding = 4;
//style->ScrollbarRounding = 6;
//style->TabRounding = 4;
//style->DisplaySafeAreaPadding = ImVec2(4, 4);

void theme() {
#pragma region theme
    ImGuiIO io = ImGui::GetIO();
    //io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14);
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 15.0f);
    ImGui::GetStyle().FrameRounding = 4.0f;
    ImGui::GetStyle().GrabRounding = 4.0f;
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.15f, 0.95f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
    colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
    colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
    style->FramePadding = ImVec2(4, 2);
    style->ItemSpacing = ImVec2(10, 2);
    style->IndentSpacing = 12;
    style->ScrollbarSize = 10;
    style->WindowRounding = 4;
    style->FrameRounding = 4;
    style->PopupRounding = 4;
    style->ScrollbarRounding = 6;
    style->TabRounding = 4;
    style->DisplaySafeAreaPadding = ImVec2(4, 4);

#pragma endregion
}
void loopRainbow() {
    switch (rmode) {
    case 1: {
        seco += 3;
        if (seco == 255) {
            rmode = 2;
        }
    } break;

    case 2: {
        firs -= 3;
        if (firs == 0) {
            rmode = 3;
        }
    } break;

    case 3: {
        thir += 3;
        if (thir == 255) {
            rmode = 4;
        }
    } break;

    case 4: {
        seco -= 3;
        if (seco == 0) {
            rmode = 5;
        }
    } break;

    case 5: {
        firs += 3;
        if (firs == 255) {
            rmode = 6;
        }
    } break;

    case 6: {
        thir -= 3;
        if (thir == 0) {
            rmode = 1;
        }
    } break;
    }
    ESPColor = ImColor(firs, seco, thir);
}
void UpdateMap() {
    auto logic = sdk::GetGameLogic();
    auto tilemap = logic->GetTileMap();
    if (logic) {
        for (auto& tile : tilemap->tiles) {
            if (tile.foreground != 0) {
                if (tile.foreground != 6 && tile.foreground != 410 && tile.foreground != 20 && tile.foreground != 26 && tile.foreground != 608 &&
                    tile.foreground != 780 && tile.foreground != 102 && tile.foreground != 1222 && tile.foreground != 4422 && tile.foreground != 2978 &&
                    tile.foreground != 3524 && tile.foreground != 762 && tile.foreground != 4632 && tile.foreground != 1308 && tile.foreground != 2862 &&
                    tile.foreground != 4798 && tile.foreground != 9740 && tile.foreground != 4240 && tile.foreground != 846 && tile.foreground != 5616 &&
                    tile.foreground != 9198 && tile.foreground != 1528 && tile.foreground != 3002 && tile.foreground != 430 && tile.foreground != 5036 &&
                    tile.foreground != 7444 && tile.foreground != 482 && tile.foreground != 12 && tile.foreground != 22 && tile.foreground != 16 &&
                    tile.foreground != 954 && tile.foreground != 3898 && tile.foreground != 1042 && tile.foreground != 6854 && tile.foreground != 8260 &&
                    tile.foreground != 5666 && tile.foreground != 340 && tile.foreground != 4722 && tile.foreground != 598 && tile.foreground != 2810 &&
                    tile.foreground != 554 && tile.foreground != 60 && tile.foreground != 1450 && tile.foreground != 8682 && tile.foreground != 5054 &&
                    tile.foreground != 4202 && tile.foreground != 3804 && tile.foreground != 4482 && tile.foreground != 1104 && tile.foreground != 4782 &&
                    tile.foreground != 3806 && tile.foreground != 2948 && tile.foreground != 8676 && tile.foreground != 858 && tile.foreground != 4740 &&
                    tile.foreground != 3796 && tile.foreground != 1700 && tile.foreground != 3232 && tile.foreground != 3572 && tile.foreground != 3566 &&
                    tile.foreground != 484 && tile.foreground != 5034 && tile.foreground != 546 && tile.foreground != 5032 && tile.foreground != 1446 &&
                    tile.foreground != 1604 && tile.foreground != 7164 && tile.foreground != 5040 && tile.foreground != 1684 && tile.foreground != 1702 &&
                    tile.foreground != 4704 && tile.foreground != 4706 && tile.foreground != 2072 && tile.foreground != 1162 && tile.foreground != 1240 &&
                    tile.foreground != 1770 && tile.foreground != 1420 && tile.foreground != 2586 && tile.foreground != 1422 && tile.foreground != 3522 &&
                    tile.foreground != 658 && tile.foreground != 428 && tile.foreground != 224 && tile.foreground != 4720 && tile.foreground != 998 &&
                    tile.foreground != 1326 && tile.foreground != 1752 && tile.foreground != 1324 && tile.foreground != 1682 && tile.foreground != 30 &&
                    tile.foreground != 4794 && tile.foreground != 4546 && tile.foreground != 3808 && tile.foreground != 10076 && tile.foreground != 10078 &&
                    tile.foreground != 2252 && tile.foreground != 2274 && tile.foreground != 62 && tile.foreground != 480 && tile.foreground != 24 &&
                    tile.foreground != 5660 && tile.foreground != 8020 && tile.foreground != 2244 && tile.foreground != 2242 && tile.foreground != 2246 &&
                    tile.foreground != 2248 && tile.foreground != 190 && tile.foreground != 192 && tile.foreground != 188 && tile.foreground != 758 &&
                    tile.foreground != 1256 && tile.foreground != 5468 && tile.foreground != 3072 && tile.foreground != 4352 && tile.foreground != 2272 &&
                    tile.foreground != 8994 && tile.foreground != 1482 && tile.foreground != 379 && tile.foreground != 10776 && tile.foreground != 9614 &&
                    tile.foreground != 886 && tile.foreground != 486 && tile.foreground != 1048 && tile.foreground != 3286 && tile.foreground != 3496 &&
                    tile.foreground != 3832 && tile.foreground != 756 && tile.foreground != 1436 && tile.foreground != 9268 && tile.foreground != 8634 &&
                    tile.foreground != 7362 && tile.foreground != 550 && tile.foreground != 286 && tile.foreground != 444 && tile.foreground != 2964 &&
                    tile.foreground != 1288 && tile.foreground != 854 && tile.foreground != 9330 && tile.foreground != 7984 && tile.foreground != 7888 &&
                    tile.foreground != 652 && tile.foreground != 1108 && tile.foreground != 1594 && tile.foreground != 398 && tile.foreground != 1434 &&
                    tile.foreground != 1304 && tile.foreground != 926 && tile.foreground != 656 && tile.foreground != 544 && tile.foreground != 1698 &&
                    tile.foreground != 974 && tile.foreground != 1694 && tile.foreground != 2646 && tile.foreground != 2068 && tile.foreground != 688 &&
                    tile.foreground != 1432 && tile.foreground != 9080 && tile.foreground != 686 && tile.foreground != 3126 && tile.foreground != 4698 &&
                    tile.foreground != 986 && tile.foreground != 4712 && tile.foreground != 5792 && tile.foreground != 6146 && tile.foreground != 5798 &&
                    tile.foreground != 3794 && tile.foreground != 6772 && tile.foreground != 1696 && tile.foreground != 4718 && tile.foreground != 596 && 
                    tile.foreground != 1162 && tile.foreground != 3426 && tile.foreground != 6160 && tile.foreground != 382 && tile.foreground != 4558 && tile.foreground != 4758 && tile.foreground != 4584) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                else {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                    //return;
                }
                if (tile.foreground == 596 && tile.flags == 32) { //Chest
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 596 && tile.flags == 0) { //Chest
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4712 && tile.flags == 64) { //Gorilla
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3126 && tile.flags == 1) { //Dark Magic Barrier
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3126 && tile.flags == 0) { //Dark Magic Barrier
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3126 && tile.flags == 32) { //Dark Magic Barrier
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 686 && tile.flags == 1) { //Jail Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 686 && tile.flags == 0) { //Jail Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 686 && tile.flags == 32) { //Jail Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3286 && tile.flags == 1024) { //Steam Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3286 && tile.flags == 1056) { //Steam Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3798 && tile.flags == 1) { //Vip Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3798 && tile.flags == 0) { //Vip Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3798 && tile.flags == 32) { //Vip Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4352 && tile.flags == 0) { //Wolf Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4352 && tile.flags == 32) { //Wolf Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 0) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 32) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 48) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 24) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 288) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4240 && tile.flags == 0) { //Jade Portcullis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4240 && tile.flags == 32) { //Jade Portcullis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4240 && tile.flags == 24) { //Jade Portcullis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 1162 && tile.flags == 0) { //Forcefield
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 1162 && tile.flags == 32) { //Forcefield
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 1162 && tile.flags == 24) { //Forcefield
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4706 && tile.flags == 0) { //Adventure Barrier
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags == 0) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags == 32) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags == 24) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags == 16) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags > 1000) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5032 && tile.flags == 32) { //Dirt Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5032 && tile.flags == 0) { //Dirt Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5032 && tile.flags == 24) { //Dirt Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5032 && tile.flags == 16) { //Dirt Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 60 && tile.flags == 0) { //portuculis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 60 && tile.flags == 32) { //portuculis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 60 && tile.flags == 288) { //portuculis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 2810 && tile.flags == 0) { //air vent
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 2810 && tile.flags == 32) { //air vent
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 2810 && tile.flags == 24) { //air vent
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 598 && tile.flags == 0) { //dragon gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 598 && tile.flags == 32) { //dragon gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 598 && tile.flags == 16) { //dragon gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 598 && tile.flags == 128) { //dragon gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags == 0) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags == 32) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags == 24) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags == 50) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags > 1000) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }

                if (tile.flags == 25) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.flags == 17) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.flags == 19) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.flags == 27) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 1042 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 6854 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 20 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 428 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 430 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 546 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 544 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 986 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                //986
            }
            if (tile.foreground == 0) {
                opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
            }
        }
    }
}

void menu::TouchdrawBox() {
    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto tilemap = logic->GetTileMap();
    auto w2s = logic->GetWorldRenderer()->camera;
    typedef pair<int, int> Pair;
    if (local) {
        auto draw = ImGui::GetBackgroundDrawList();
        auto LOCALSCREEN = w2s.WorldToScreen(local->GetPos() + (local->GetSize() / 2.f));
        Pair src((int)logic->GetLocalPlayer()->pos.x / 32, (int)logic->GetLocalPlayer()->pos.y / 32);
        Pair dest((int)opt::cheat::TouchPos.x, (int)opt::cheat::TouchPos.y);
        //Create new pathfinder
        Pathfinder* pathfinder = new Pathfinder(100, 60);

        //Loop through the map array and insert all coordinates into pathfinder's STL map
        for (int i = 0; i < pathfinder->width; i++) {
            for (int j = 0; j < pathfinder->height; j++) {
                pathfinder->nodes->insert(std::pair<Pathfinder::Point, int>(Pathfinder::Point(i, j), opt::cheat::gtmap[j][i]));
            }
        }
        std::vector<Pathfinder::Point> path = pathfinder->findPath(Pathfinder::Point(src.first, src.second), Pathfinder::Point(dest.first, dest.second));
        for (int i = 0; i < path.size(); i++) {
            CL_Vec2f goalfirst;
            goalfirst.x = (int)path[i].x * 32;
            goalfirst.y = (int)path[i].y * 32;
            CL_Vec2f goalsecond;
            goalsecond.x = (int)path[i].x * 32 + 32;
            goalsecond.y = (int)path[i].y * 32 + 32;
            auto SCG = w2s.WorldToScreen(goalfirst);
            auto SCG2 = w2s.WorldToScreen(goalsecond);
            //draw->AddRect(ImVec2(SCG.x, SCG.y), ImVec2(SCG2.x, SCG2.y), ESPColor);
            //draw->AddLine(ImVec2(SCG.x, SCG.y), ImVec2(SCG2.x, SCG2.y), ImColor(100, 255, 255), 3.f);
        }


    }
}


void menu::EndScene(IDirect3DDevice9* device, bool active) {
    if (!global::load) { //init imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplDX9_Init(device);
        ImGui::StyleColorsDark();
        //load_config();
        utils::printc("93", "dx9 and imgui init done");
        theme();
        global::load = true;
    }
    if (global::load) {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        static bool showgui = false;
        if (GetAsyncKeyState(VK_F2) & 1)
            showgui = !showgui;

        ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2{ 800, 450 }, ImGuiCond_Once);
        if (showgui && ImGui::Begin("Cyrus#8000", &showgui, ImGuiWindowFlags_NoCollapse)) {
            animate();
            static char* tab_names[] = { (char*)"Main", (char*)"Cheats", (char*)"Modification", (char*)"Information", (char*)"World" };
            static int active_tab = 0;

            auto& style = ImGui::GetStyle();
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, style.WindowPadding.y });

            float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
            imwrap::horizontal_tabs(tab_names, active_tab, group_w / _countof(tab_names), 33.0f);
            ImGui::PopStyleVar(3);

            switch (active_tab) {
            case 0: enhancements_tab(); break;
            case 1: cheats_tab(); break;
            case 2: cheats_tab2(); break;
            case 3: framework_tab(); break;
            case 4: playerpog(); break;
            }
            ImGui::End();
        }
        auto logic = sdk::GetGameLogic();
        auto world = logic->GetWorld();
        //auto local = sdk::GetGameLogic()->GetLocalPlayer();
        if (world) {
            if (opt::cheat::itemesp)
                itemesp();
            if (opt::cheat::playeresp)
                playeresp();
            if (opt::cheat::spamwater)
                doSpamWater();
            if (opt::cheat::autocollect)
                autocollect();
            if (opt::cheat::autofarm)
                doAutofarm();
            if (opt::cheat::noname)
                doNoname();
            if (opt::cheat::blinkcolor)
                doBlinkcolor();
            if (opt::cheat::testmftiles)
                doCHECKTILES();
            if (opt::cheat::nazismoke)
                doNazi();
            if (opt::cheat::PathFindinglol)
                doLagBack();
            if (opt::spam_text2)
                dospam();
            if (opt::spampacket)
                dospampacket();
            if (opt::spam_drop2)
                dospamdrop();
            if (opt::smokeplayer)
                smokeplayer();
            if (opt::autofish)
                autofish();
            if (opt::autoclear)
                autoclear();
            if (opt::spamxp) {
                spamxp();
            }
            if (opt::gg)
                tptoitem();
            //if (opt::autosurgreal2)
            //    autosurg();
            if (opt::sayit) {
                sayit();
            }
            UpdateMap();
        }
        loopRainbow();
        auto draw = ImGui::GetBackgroundDrawList();
        std::string hah = "Cyrus#8000";
        const char* hahx = hah.c_str();
        draw->AddText(NULL, 30.0f, ImVec2(10, 10), ESPColor, hahx);
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    }
}
WNDPROC OriginalWndProcFunction;
bool menu::WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    static bool init = false;
    if (!global::d9init)
        return false;

    if (global::load && !init) {
        ImGui_ImplWin32_Init(wnd);
        init = true;
    }
    //global::draw = true;
    if (msg == WM_KEYUP && wparam == VK_DELETE)
        global::draw = !global::draw;
    //= !global::draw;

    /*  if (msg == WM_KEYUP && wparam == VK_F4)
        load_config();*/
    ImGui_ImplWin32_WndProcHandler(wnd, msg, wparam, lparam);

    return CallWindowProcW(OriginalWndProcFunction, wnd, msg, wparam, lparam);
}

