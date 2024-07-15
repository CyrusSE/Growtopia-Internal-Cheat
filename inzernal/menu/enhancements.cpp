#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
#include <hooks/SendPacket.h>

inline void ReplaceAll(string& str, const string& from, const string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void menu::enhancements_tab() {
    if (ImGui::CollapsingHeader("Login info spoofing")) {
        if (ImGui::BeginChild("###loginspoof", ImVec2(ImGui::GetWindowWidth() * 0.90f, 138.f), true)) {
            imwrap::checkbox("Custom MAC", opt::custom_mac_login, "Customize your MAC");
            ImGui::SameLine();
            imwrap::inputstring("Enter MAC", &opt::cheat::custom_mac);
            imwrap::checkbox("Custom RID", opt::custom_rid_login, "Customize your RID");
            ImGui::SameLine();
            imwrap::inputstring("Enter RID", &opt::cheat::custom_rid);
            imwrap::checkbox("Custom Guest", opt::custom_guest_login, "Customize your Guest Name");
            ImGui::SameLine();
            imwrap::inputstring("Enter Name", &opt::cheat::custom_guest);
            imwrap::checkbox("Custom Flag", opt::custom_flag_login, "Customize your Flag");
            ImGui::SameLine();
            imwrap::inputstring("Enter Flag ID", &opt::cheat::custom_flag);
            imwrap::checkbox("Custom Version", opt::gt_version_spoof, "Customize your GT Version");
            ImGui::SameLine();
            imwrap::inputstring("Enter Version", &opt::gt_version);
            if (ImGui::Button("RGT Mode")) {
                opt::custom_flag_login = false;
                opt::custom_guest_login = false;
                opt::custom_mac_login = false;
                opt::custom_rid_login = false;
                opt::cheat::dev_zoom = false;
                opt::cheat::mod_zoom = true;
                opt::rgtmode = true;
                opt::gt_version_spoof = false;
            }
            ImGui::EndChild();
        }
    }
    if (ImGui::CollapsingHeader("AAP Management")) {
        if (ImGui::BeginChild("###androidaap", ImVec2(ImGui::GetWindowWidth() * 0.90f, 97.f), true)) {
            ImGui::Checkbox("Grow ID", &opt::custom_login);
            ImGui::SameLine();
            imwrap::inputstring("Enter Growid", &opt::cheat::custom_id);
            ImGui::Checkbox("Password", &opt::custom_pass);
            ImGui::SameLine();
            imwrap::inputstring("Enter Password", &opt::cheat::custom_pass);
            imwrap::checkbox("Android AAP", opt::android_login, "Bypass AAP for Android.");
            ImGui::SameLine();
            imwrap::inputstring("Enter RID", &opt::cheat::spoof_rid_dek);
            imwrap::checkbox("PC AAP", opt::pc_login, "Bypass AAP for PC.");
            ImGui::SameLine();
            imwrap::inputstring("Enter MAC", &opt::cheat::spoof_mac_dek);
            ImGui::EndChild();
        }
    }
    if (ImGui::CollapsingHeader("Utility")) {
        if (ImGui::BeginChild("###utility", ImVec2(ImGui::GetWindowWidth() * 0.90f, 140.f), true)) {
            imwrap::prep_columns(6);
            imwrap::checkbox("Fast drop", opt::fast_drop, "Drop items without dialog");
            ImGui::NextColumn();
            imwrap::checkbox("Fast trash", opt::fast_trash, "Trash items without dialog");
            ImGui::NextColumn();
            imwrap::checkbox("Fast kick", opt::fast_kick, "Kick user without dialog");
            ImGui::NextColumn();
            imwrap::checkbox("Fast pull", opt::fast_pull, "Pull user without dialog");
            ImGui::NextColumn();
            imwrap::checkbox("Fast ban", opt::fast_ban, "Ban user without dialog");
            ImGui::Columns(1, nullptr, false);
            ImGui::Spacing();
            imwrap::prep_columns(3);
            imwrap::checkbox("Fast take vend", opt::fast_vend, "Take items from vend without dialog");
            ImGui::NextColumn();
            imwrap::checkbox("Fast restock vend", opt::fast_vend2, "Restock items to vend without dialog");
            ImGui::NextColumn();
            imwrap::checkbox("Fast take and drop vend", opt::fast_vend3, "Take and drop items from vend without dialog");
            ImGui::Columns(1, nullptr, false);
            imwrap::prep_columns(3);
            imwrap::checkbox("Fast take GAUT", opt::fast_gaia, "Take items from gaia without dialog");
            ImGui::NextColumn();
            //imwrap::checkbox("Fast restock magplant", opt::fast_gaia2, "Restock items to magplant without dialog");
            //ImGui::NextColumn();
            imwrap::checkbox("Fast take and drop GAUT", opt::fast_gaia2, "Take and drop items from without dialog");
            ImGui::Columns(1, nullptr, false);
            imwrap::prep_columns(3);
            imwrap::checkbox("Fast take magplant", opt::fast_mag, "Take items from magplant without dialog");
            ImGui::NextColumn();
            imwrap::checkbox("Fast restock magplant", opt::fast_mag2, "Restock items to magplant without dialog");
            ImGui::NextColumn();
            imwrap::checkbox("Fast take and drop magplant", opt::fast_mag3, "Take and drop items from without dialog");
            ImGui::Columns(1, nullptr, false);
            imwrap::checkbox("Fast buy vend", opt::fast_buy, "Buy items from vend without dialog (Items per World lock)");
            ImGui::SameLine();
            imwrap::inputstring("(items per WL)", &opt::cheat::vendcount);
            imwrap::checkbox("Fast buy vend 2", opt::fast_buy2, "Buy items from vend without dialog (World locks per Item)");
            ImGui::SameLine();
            imwrap::inputstring("(WLs per item)", &opt::cheat::vendcount2);
            ImGui::EndChild();
        }
    }
    if (ImGui::CollapsingHeader("Auto Ban System")) {
        if (ImGui::BeginChild("###autoban", ImVec2(ImGui::GetWindowWidth() * 0.90f, 134.f), true)) {
            ImGui::Text("(Turn off the auto ban first before writing the text for auto ban)");
            imwrap::checkbox("Auto Ban", opt::auto_ban, "Turn off the auto ban first before writing the text for auto ban");
            imwrap::inputstring("Enter Text to find in SB for auto ban", &opt::cheat::ban_text);
            imwrap::inputstring("Enter reason for auto ban", &opt::cheat::reason);
            imwrap::prep_columns(5);
            imwrap::checkbox("729 Days", opt::perma, "729 Days auto ban");
            ImGui::NextColumn();
            imwrap::checkbox("IP Ban", opt::ip_perma, "Auto IP ban");
            ImGui::Columns(1, nullptr, false);
            imwrap::checkbox("Fast Ban", opt::fast_ban_game, "Fast banning people");
            ImGui::SameLine();
            imwrap::inputstring("Enter reason", &opt::cheat::reason2);
            ImGui::EndChild();
        }
    }
        if (ImGui::CollapsingHeader("Packet sender")) {
            if (!opt::variantpacket) {
                ImGui::Checkbox("Normal Packet", &opt::normalpacket);
            }
            //ImGui::SameLine();
            if (!opt::normalpacket) {
                if (!opt::variantpacket) {
                    ImGui::SameLine();
                }
                ImGui::Checkbox("Variantlist Packet", &opt::variantpacket);
            }
            if (opt::normalpacket) {
                //if (ImGui::BeginChild("###packetsender", ImVec2(ImGui::GetWindowWidth() * 0.83f, 60.f), true)) {
                static std::string meme = "action|";
                static int type = 2;
                imwrap::inputstring("Packet", &meme, ImGuiInputTextFlags_Multiline);
                if (opt::duapacket) {
                    ImGui::InputInt("Packet type", &type);
                }
                static std::string meme2 = "action|";
                static int type2 = 2;
                if (opt::duapacket) {
                    imwrap::inputstring("Packet v2", &meme2, ImGuiInputTextFlags_Multiline);
                    ImGui::InputInt("Packet type v2", &type2);
                }
                if (!opt::duapacket) {
                    ImGui::InputInt("Packet type", &type);
                }
                ImGui::Checkbox("Spam Packet", &opt::spampacket);
                ImGui::SameLine();
                ImGui::Checkbox("2 Packets", &opt::duapacket);
                ImGui::Text("Cooldown");
                ImGui::SameLine();
                ImGui::InputFloat("Interval", &opt::cheat::interval2, 0.f, 10.f);
                opt::cheat::thepacket = meme;
                opt::cheat::type = type;
                opt::cheat::thepacket2 = meme2;
                opt::cheat::type2 = type2;
                if (ImGui::Button("Send packet")) {
                    std::string copy = meme;
                    std::string copy2 = meme2;
                    while (utils::replace(copy, ";;", "\n"));
                    gt::send(type, copy, sdk::GetPeer());
                    if (opt::duapacket) {
                        while (utils::replace(copy2, ";;", "\n"));
                        gt::send(type2, copy2, sdk::GetPeer());
                    }
                }
                ImGui::EndChild();
            }
            if (opt::variantpacket) {
                static std::string meme = "";
                ImGui::SameLine();
                ImGui::Checkbox("Param 2", &opt::param2);
                ImGui::SameLine();
                ImGui::Checkbox("Param 3", &opt::param3);
                ImGui::SameLine();
                ImGui::Checkbox("Param 4", &opt::param4);
                if (opt::param2 || opt::param3 || opt::param4) {
                    ImGui::SameLine();
                    if (ImGui::Button("Reset Param")) {
                        opt::param2 = false;
                        opt::param3 = false;
                        opt::param4 = false;
                    }
                }
                imwrap::inputstring("Variantlist", &opt::variantlist);
                if (!opt::param2 && !opt::param3 && !opt::param4) {
                    imwrap::inputstring("Packet", &meme, ImGuiInputTextFlags_Multiline);
                }
                if (opt::param2) {
                    imwrap::inputstring("Param 1", &opt::param1text);
                    imwrap::inputstring("Param 2", &opt::param2text);
                }
                if (opt::param3) {
                    imwrap::inputstring("Param 3", &opt::param3text);
                }
                if (opt::param4) {
                    imwrap::inputstring("Param 4", &opt::param4text);
                }
                if (ImGui::Button("Send packet")) {
                    std::string copy = meme;
                    while (utils::replace(copy, ";;", "\n"));
                    variantlist_t va{ opt::variantlist };
                    if (!opt::param2 || !opt::param3 || !opt::param4) {
                        va[1] = copy;
                    }
                    else {
                        va[1] = opt::param1text;
                    }
                    if (opt::param2) {
                        va[2] = opt::param2text;
                    }
                    if (opt::param3) {
                        va[3] = opt::param3text;
                    }
                    if (opt::param4) {
                        va[4] = opt::param4text;
                    }
                    gt::send_varlist_self(va);
                    //if (opt::duapacket) {
                    //    while (utils::replace(copy2, ";;", "\n"));
                    //    gt::send(type2, copy2, sdk::GetPeer());
                    //}
                    ImGui::EndChild();
                }
            }
            //static std::string meme2 = "action|";
            //static int type2 = 2;
            //if (opt::duapacket) {
            //    imwrap::inputstring("Packet v2", &meme2, ImGuiInputTextFlags_Multiline);
            //    ImGui::InputInt("Packet type v2", &type2);
            //}
            //}
        }
    }