#pragma once
#include <core/globals.h>
#include <hooks/Update.h>
#include <sdk/sdk.h>
#include <string>

class SendPacketRawHook {
   public:
    static void Execute(int type, GameUpdatePacket* packet, int size, void* packetsender, ENetPeer* peer, int flag) {
        static auto orig = decltype(&hooks::SendPacketRaw)(hookmgr->orig(sig::sendpacketraw));

        if (opt::cheat::block_sendpacketraw)
            return;
        //std::cout << "Packet found " << packet << std::endl;
        if (packet->type == PACKET_APP_INTEGRITY_FAIL) {
            utils::printc("95", "Was going to send a packet about app integrity failure, but we blocked it.");
            return;
        }
        if (packet->type == PACKET_STATE && opt::cheat::dancemove && !opt::cheat::spamwater && !opt::cheat::ghost) {
            packet->int_data = 0;
            packet->animation_type = 0;
            packet->flags = 32 | 48;
            packet->vec2_x = 0;
            packet->vec2_y = 0;
        }
        if (opt::skipitem) {
            if (packet->type == 24 || packet->type == 16) {
                gt::sendlog("Skipping update items");
                gt::send(2, "action|enter_game\n");
                return;
            }
        }
        if (packet->type == PACKET_STATE && !opt::cheat::dancemove && !opt::cheat::spamwater && opt::cheat::ghost) {
            return;
        }
        if (packet->type == PACKET_TILE_CHANGE_REQUEST && opt::cheat::superpunchbreak) {
            if (packet->int_data == 18) {
                static types::time timerlol = std::chrono::system_clock::now();
                static int mfoffsets = -1;
                if (utils::run_at_interval(timerlol, 0.05)) {
                    mfoffsets += 1;
                    if (mfoffsets > 2)
                        mfoffsets = -1;
                }
                for (auto i = 0; i < 4; i++) {
                    int x = packet->int_x;
                    int y = packet->int_y;
                    if (i == 1)
                        x -= 1;
                    if (i == 2)
                        x -= 2;
                    //-----------------
                    if (i == 3)
                        x += 1;
                    if (i == 4)
                        x += 2;
                    GameUpdatePacket newpacket{};
                    newpacket.type = PACKET_TILE_CHANGE_REQUEST;
                    newpacket.int_data = 18;
                    newpacket.int_x = x;
                    newpacket.int_y = packet->int_y + mfoffsets;
                    newpacket.pos_x = packet->pos_x;
                    newpacket.pos_y = packet->pos_y;
                    newpacket.flags = 16;
                    void* PacketSender = nullptr;
                    orig(NET_MESSAGE_GAME_PACKET, &newpacket, 56, PacketSender, peer, flag);
                }
            }
            else {
                static types::time timerlol = std::chrono::system_clock::now();
                static int mfoffsets = -1;
                if (utils::run_at_interval(timerlol, 0.30)) {
                    mfoffsets += 1;
                    if (mfoffsets > 2)
                        mfoffsets = -1;
                }
                for (auto i = 0; i < 6; i++) {
                    int x = packet->int_x;
                    int y = packet->int_y;
                    if (i == 1)
                        x -= 1;
                    if (i == 2)
                        x -= 2;
                    if (i == 3)
                        x -= 3;
                    //-----------------
                    if (i == 4)
                        x += 1;
                    if (i == 5)
                        x += 2;
                    if (i == 6)
                        x += 3;
                    GameUpdatePacket newpacket{};
                    newpacket.type = PACKET_TILE_CHANGE_REQUEST;
                    newpacket.int_data = packet->int_data;
                    newpacket.int_x = x;
                    newpacket.int_y = packet->int_y;
                    newpacket.pos_x = packet->pos_x;
                    newpacket.pos_y = packet->pos_y;
                    newpacket.flags = 16;
                    void* PacketSender = nullptr;
                    orig(NET_MESSAGE_GAME_PACKET, &newpacket, 56, PacketSender, peer, flag);
                }
            }
        }
        if (packet->type == PACKET_GOT_PUNCHED) {
            return;
        }
        if (packet->type == PACKET_SEND_PARTICLE_EFFECT) {
            printf("NetID: %d\n", (int)packet->netid);
            printf("PosX: %0f\n", (float)packet->vec_x);
            printf("PosY: %0f\n", (float)packet->vec_y);
            printf("XSpeed: %d\n", (int)packet->vec2_x);
            printf("YSpeed: %d\n", (int)packet->vec2_y);
            printf("MainValue: %d\n", (int)packet->int_data);
        }
        //printf("sending raw packet: %d [%s]\n", packet->type, gt::get_type_string(packet->type).c_str());

        if (packet->type == 0 && packet->flags & 4) {
            auto player = sdk::GetGameLogic()->GetLocalPlayer();
            if (player)
                UpdateManager::OnJoinedWorld(player);
            for (auto otherplayerorme : sdk::GetGameLogic()->GetNetObjMgr()->players) {
                if (otherplayerorme.second != player) {
                    if (otherplayerorme.second->is_invis == true) {
                        //otherplayerorme.second->is_invis = false;
                        otherplayerorme.second->name = otherplayerorme.second->name + " `4[INVIS]";
                    }
                    if (otherplayerorme.second->is_invis == false) {
                        otherplayerorme.second->name = otherplayerorme.second->name;
                    }
                    //otherplayerorme.second->is_invis = false;
                    //otherplayerorme.second->is_supermod = false;
                    //otherplayerorme.second->is_mod = false;
                    if (otherplayerorme.second->name == "````") {
                        otherplayerorme.second->name = otherplayerorme.second->name + " `4[INVIS]";
                    }
                }
            }
        }

        if (packet->velocity_x == 1000.f || packet->type == PACKET_PING_REPLY) {
            GameUpdatePacket pk{ 0 };
            pk.type = PACKET_PING_REPLY;
            pk.ping_hash = packet->ping_hash; // HashString of ping request int data field
            pk.int_data = packet->int_data;   // tick count (speedhack)
            pk.pos_x = 64.f;                  // punch range in pixels
            pk.pos_y = 64.f;                  // build range in pixels
            pk.velocity_x = 1000.f;           //gravity
            pk.velocity_y = 250.f;            // movement speed

            void* PacketSender = nullptr;
            orig(NET_MESSAGE_GAME_PACKET, &pk, 56, PacketSender, peer, flag);
            return;
        }
        //if (packet->type != 0) {
        //    gt::sendlog("Packet type " + std::to_string(packet->type));
        //}
        orig(type, packet, size, packetsender, peer, flag);
    }
};