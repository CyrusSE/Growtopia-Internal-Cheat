#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
#include <hooks/SendPacket.h>
#include <hooks/SendPacketRaw.h>

inline void ReplaceAll(string& str, const string& from, const string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}


void menu::playerpog() {
    if (ImGui::Button("Ban All")) {
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            utils::replace(name2, "@", "");
            utils::replace(name2, "`1", "");
            utils::replace(name2, "`2", "");
            utils::replace(name2, "`3", "");
            utils::replace(name2, "`4", "");
            utils::replace(name2, "`5", "");
            utils::replace(name2, "`6", "");
            utils::replace(name2, "`7", "");
            utils::replace(name2, "`8", "");
            utils::replace(name2, "`9", "");
            std::string packet = "action|input\n|text|/ban " + name2;
            SendPacketHook::Execute(2, packet, sdk::GetPeer());
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Pull All")) {
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            utils::replace(name2, "@", "");
            utils::replace(name2, "`1", "");
            utils::replace(name2, "`2", "");
            utils::replace(name2, "`3", "");
            utils::replace(name2, "`4", "");
            utils::replace(name2, "`5", "");
            utils::replace(name2, "`6", "");
            utils::replace(name2, "`7", "");
            utils::replace(name2, "`8", "");
            utils::replace(name2, "`9", "");
            std::string packet = "action|input\n|text|/pull " + name2;
            SendPacketHook::Execute(2, packet, sdk::GetPeer());
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Kick All")) {
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string packet = "action|wrench\n|netid|" + std::to_string(pair.first);
            SendPacketHook::Execute(2, packet, sdk::GetPeer());
                //std::string packet2 = "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(pair.first) + "|\nbuttonClicked|kick\n\n";
                //SendPacketHook::Execute(2, packet2, sdk::GetPeer());
            gt::sendlog("Kicked " + pair.second->name);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::string packet2 = "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(pair.first) + "|\nbuttonClicked|kick\n\n";
            SendPacketHook::Execute(2, packet2, sdk::GetPeer());
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Trade all")) {
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            utils::replace(name2, "@", "");
            ReplaceAll(name2, "`1", "");
            ReplaceAll(name2, "`2", "");
            ReplaceAll(name2, "`3", "");
            ReplaceAll(name2, "`4", "");
            ReplaceAll(name2, "`5", "");
            ReplaceAll(name2, "`6", "");
            ReplaceAll(name2, "`7", "");
            ReplaceAll(name2, "`8", "");
            ReplaceAll(name2, "`9", "");
            ReplaceAll(name2, "Dr.", "");
            std::string packet = "action|input\n|text|/trade " + name2;
            SendPacketHook::Execute(2, packet, sdk::GetPeer());
        }
    }
    ImGui::Text("Player List");
    if (sdk::GetGameLogic()->GetNetObjMgr()) {
        ImGui::Columns(6, "Player"); // 4-ways, with border
        static auto width = ImGui::GetColumnWidth(0);

        ImGui::SetColumnWidth(0, width * 0.65f);
        ImGui::SetColumnWidth(1, width * 0.65f);
        ImGui::SetColumnWidth(2, width * 1.20f);
        ImGui::SetColumnWidth(3, width * 0.65f);
        ImGui::SetColumnWidth(4, width * 0.65f);
        ImGui::SetColumnWidth(5, width * 1.55f);
        ImGui::Separator();
        ImGui::Text("NetID");
        ImGui::NextColumn();
        ImGui::Text("UserID");
        ImGui::NextColumn();
        ImGui::Text("Name");
        ImGui::NextColumn();
        ImGui::Text("Mod");
        ImGui::NextColumn();
        ImGui::Text("Dev");
        ImGui::NextColumn();
        ImGui::Text("Position");
        ImGui::NextColumn();
        ImGui::Separator();
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            ImGui::Text("%d", pair.first);
            ImGui::NextColumn();
            ImGui::Text("%d", pair.second->userid);
            ImGui::NextColumn();
            ReplaceAll(name2, "`1", "");
            ReplaceAll(name2, "`2", "");
            ReplaceAll(name2, "`3", "");
            ReplaceAll(name2, "`4", "");
            ReplaceAll(name2, "`5", "");
            ReplaceAll(name2, "`6", "");
            ReplaceAll(name2, "`7", "");
            ReplaceAll(name2, "`8", "");
            ReplaceAll(name2, "`9", "");
            ImGui::Text(std::string(name2).c_str());
            ImGui::NextColumn();
            if (pair.second->is_mod)
                ImGui::Text("Yes");
            else
                ImGui::Text("No");
            //ImGui::Text("No", pair.second->is_mod);
            ImGui::NextColumn();
            if (pair.second->is_supermod)
                ImGui::Text("Yes");
            else
                ImGui::Text("No");
            //ImGui::Text("Yes", pair.second->is_supermod);
            ImGui::NextColumn();
            ImGui::Text("%.0f, %.0f", pair.second->pos.x, pair.second->pos.y);
            ImGui::NextColumn(); 
            //ImGui::Text("NetID: %d, Name: %s\n", pair.first, pair.second->name.c_str());
        }
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::Columns(2, "Commands");
        ImGui::Text("Name");
        ImGui::NextColumn();
        ImGui::Text("Command");
        ImGui::NextColumn();
        ImGui::Separator();
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            ReplaceAll(name2, "`1", "");
            ReplaceAll(name2, "`2", "");
            ReplaceAll(name2, "`3", "");
            ReplaceAll(name2, "`4", "");
            ReplaceAll(name2, "`5", "");
            ReplaceAll(name2, "`6", "");
            ReplaceAll(name2, "`7", "");
            ReplaceAll(name2, "`8", "");
            ReplaceAll(name2, "`9", "");
            /////////////////////////////////////////
            ImGui::Text(std::string(name2).c_str());
            /////////////////////////////////////////
            ImGui::SameLine();
            ImGui::NextColumn();
            if (ImGui::Button((std::string("Kick") + std::string("##") + name2).c_str())) {
                utils::replace(name2, "``", "");
                utils::replace(name2, "@", "");
                ReplaceAll(name2, "`1", "");
                ReplaceAll(name2, "`2", "");
                ReplaceAll(name2, "`3", "");
                ReplaceAll(name2, "`4", "");
                ReplaceAll(name2, "`5", "");
                ReplaceAll(name2, "`6", "");
                ReplaceAll(name2, "`7", "");
                ReplaceAll(name2, "`8", "");
                ReplaceAll(name2, "`9", "");
                std::string packet = "action|input\n|text|/kick " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
            ImGui::SameLine();
            if (ImGui::Button((std::string("Ban") + std::string("##") + name2).c_str())) {
                utils::replace(name2, "``", "");
                utils::replace(name2, "@", "");
                ReplaceAll(name2, "`1", "");
                ReplaceAll(name2, "`2", "");
                ReplaceAll(name2, "`3", "");
                ReplaceAll(name2, "`4", "");
                ReplaceAll(name2, "`5", "");
                ReplaceAll(name2, "`6", "");
                ReplaceAll(name2, "`7", "");
                ReplaceAll(name2, "`8", "");
                ReplaceAll(name2, "`9", "");
                std::string packet = "action|input\n|text|/ban " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
            ImGui::SameLine();
            if (ImGui::Button((std::string("Pull") + std::string("##") + name2).c_str())) {
                utils::replace(name2, "``", "");
                utils::replace(name2, "@", "");
                ReplaceAll(name2, "`1", "");
                ReplaceAll(name2, "`2", "");
                ReplaceAll(name2, "`3", "");
                ReplaceAll(name2, "`4", "");
                ReplaceAll(name2, "`5", "");
                ReplaceAll(name2, "`6", "");
                ReplaceAll(name2, "`7", "");
                ReplaceAll(name2, "`8", "");
                ReplaceAll(name2, "`9", "");
                std::string packet = "action|input\n|text|/pull " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
            ImGui::SameLine();
            if (ImGui::Button((std::string("Teleport") + std::string("##") + name2).c_str())) {
                auto local = sdk::GetGameLogic()->GetLocalPlayer();
                local->SetPos(pair.second->pos.x, pair.second->pos.y);
            }
            ImGui::SameLine();
            if (ImGui::Button((std::string("Wrench") + std::string("##") + name2).c_str())) {
                std::string packet = "action|wrench\n|netid|" + std::to_string(pair.first);
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
            ImGui::NextColumn();

        }
       
    }
}