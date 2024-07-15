#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
#include <proton\VariantDB.h>

inline void ReplaceAll(string& str, const string& from, const string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}


void menu::framework_tab() {
    imwrap::prep_columns(3);

    auto client = sdk::GetClient();
    auto logic = sdk::GetGameLogic();
    auto local = logic->GetLocalPlayer();
    auto world = logic->GetWorld();
    auto tilemap = logic->GetTileMap();
    bool local_div = ImGui::BeginChild("Player", ImVec2(ImGui::GetWindowWidth() / 3.15f, 250.f), true, ImGuiWindowFlags_MenuBar);
    if (local_div && local) {
        ImGui::BeginMenuBar();
        ImGui::Text("Player");
        ImGui::EndMenuBar();
        std::string name = local->name.c_str();
        std::string name2 = name.substr(2);
        ReplaceAll(name2, "`1", "");
        ReplaceAll(name2, "`2", "");
        ReplaceAll(name2, "`3", "");
        ReplaceAll(name2, "`4", "");
        ReplaceAll(name2, "`5", "");
        ReplaceAll(name2, "`6", "");
        ReplaceAll(name2, "`7", "");
        ReplaceAll(name2, "`8", "");
        ReplaceAll(name2, "`9", "");
        ImGui::Text("Name: %s", name2);
        ImGui::Text("Netid: %d", local->netid);
        ImGui::Text("Address: %llx", local);
        ImGui::Text("Userid: %d", local->userid);
        ImGui::Separator();
        ImGui::Text("NetControllerLocal: %llx", local->NetControllerLocal);
        ImGui::Text("AvatarPacketReceiver: %llx", local->AvatarPacketReceiver);
        ImGui::Text("AvatarPacketSender: %llx", local->AvatarPacketSender);
        ImGui::Separator();
        ImGui::Text("Size: %.0f,%.0f", local->size.x, local->size.y);
        auto pos2f = local->GetPos();
        
        ImGui::Text("Position: %.0f,%.0f", pos2f.x, pos2f.y);
        ImGui::Text("Pos Tile: %.0f,%.0f", pos2f.x / 32, pos2f.y / 32);

        static float xpos = 0.f, ypos = 0.f;
        ImGui::PushItemWidth(100.f);
        ImGui::InputFloat("X", &xpos, 10.f, 10.f, 0);
        ImGui::SameLine();
        ImGui::InputFloat("Y", &ypos, 10.f, 10.f, 0);
        ImGui::PopItemWidth();
        if (ImGui::Button("Teleport"))
            local->SetPos(xpos, ypos);
        ImGui::EndChild();
    }
    else if (local_div && !local) {
        ImGui::BeginMenuBar();
        ImGui::Text("Player");
        ImGui::EndMenuBar();
        ImGui::TextDisabled("Not in a world.");
        ImGui::EndChild();
    }

    ImGui::NextColumn();

    bool world_div = ImGui::BeginChild("World", ImVec2(ImGui::GetWindowWidth() / 3.15f, 250.f), true, ImGuiWindowFlags_MenuBar);
    if (world_div && world) {
        ImGui::BeginMenuBar();
        ImGui::Text("World");
        ImGui::EndMenuBar();

        ImGui::Text("Name: %s", world->name.c_str());
        ImGui::Text("Address: %llx", world);
        ImGui::Separator();
        ImGui::Text("WorldRenderer: %llx", logic->GetWorldRenderer());
        ImGui::Text("TileMap: %llx", tilemap);
        auto objmap = logic->GetObjectMap();
        ImGui::Text("WorldObjectMap: %llx", objmap);
        ImGui::Separator();

        static int x = 0, y = 0;
        static Tile* tile = tilemap->GetTileSafe(x, y);

        if (ImGui::CollapsingHeader("Tile information")) {
            ImGui::PushItemWidth(150.f);
            if (ImGui::InputInt("X", &x, 1, 10)) {
                if (x > 99)
                    x = 99;
                else if (x < 0)
                    x = 0;
                tile = tilemap->GetTileSafe(x, y);
            }

            if (ImGui::InputInt("Y", &y, 1, 10)) {
                if (y > 59)
                    y = 59;
                else if (y < 0)
                    y = 0;
                tile = tilemap->GetTileSafe(x, y);
            }
            ImGui::PopItemWidth();
            ImGui::Separator();
            if (tile) {
                ImGui::Text("Foreground: %d", tile->foreground);
                ImGui::Text("Background: %d", tile->background);
                ImGui::Text("Flags: %d", tile->flags);
                ImGui::Text("FG texture pos: %d,%d", (int)(tile->texture_pos % 8), (int)(tile->texture_pos / 8));
                ImGui::Text("BG texture pos: %d,%d", (int)(tile->texture_pos_bg % 8), (int)(tile->texture_pos_bg / 8));
            }
            else
                ImGui::TextColored(ImVec4(231.0f / 255.0f, 76.0f / 255.0f, 60.0f / 255.0f, 1.0f), "Invalid tile");
        }
        if (objmap && ImGui::CollapsingHeader("Dropped items")) {

            ImGui::Columns(4, "dropped"); // 4-ways, with border
            static auto width = ImGui::GetColumnWidth(0);

            ImGui::SetColumnWidth(0, width * 0.65f);
            ImGui::SetColumnWidth(1, width * 1.05f);
            ImGui::SetColumnWidth(2, width * 0.80f);
            
            ImGui::Separator();
            ImGui::Text("OID");
            ImGui::NextColumn();
            ImGui::Text("Item ID");
            ImGui::NextColumn();
            ImGui::Text("Count");
            ImGui::NextColumn();
            ImGui::Text("Position");
            ImGui::NextColumn();
            ImGui::Separator();
            for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it) {
                ImGui::Text("%d", it->object_id);
                ImGui::NextColumn();
                ImGui::Text("%d", it->item_id);
                ImGui::NextColumn();
                ImGui::Text("%d", it->amount);
                ImGui::NextColumn();
                ImGui::Text("%.0f,%.0f", it->pos.x, it->pos.y);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Separator();
        }
        ImGui::EndChild();
    }
    else if (world_div && !world) {
        ImGui::BeginMenuBar();
        ImGui::Text("World");
        ImGui::EndMenuBar();
        ImGui::TextDisabled("Not in a world.");
        ImGui::EndChild();
    }

    ImGui::NextColumn();

    bool client_div = ImGui::BeginChild("ENetClient", ImVec2(ImGui::GetWindowWidth() / 3.15f, 250.f), true, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);
    if (client_div && client) {
        ImGui::BeginMenuBar();
        ImGui::Text("ENetClient");
        ImGui::EndMenuBar();

        ImGui::Text("Address: %llx", client);
        ImGui::Separator();
        ImGui::Text("Host: %llx", client->host);
        ImGui::Text("Peer: %llx", client->peer);
        ImGui::Text("Address: %s:%d", client->address.c_str(), client->port);
        std::string gg = client->address.c_str();
        if (gg.find("213.179.209.") != -1) {
            ImGui::Text("(in RGT server)");
            opt::custom_server_on = true;
        }
        else {
            ImGui::Text("(Not in RGT server)");
        }
        //ImGui::Text("Meta: %u", client->meta);
        ImGui::Text("User: %u", client->user);
        ImGui::Text("Token: %u", client->token);
        ImGui::Separator();
        ImGui::PushItemWidth(150.0f);
        ImGui::InputInt("track_tick", &client->tracking_tick);
        ImGui::InputInt("net_msg_type", &client->net_msg_type);
        ImGui::InputInt("unk_timer", &client->another_timer);
        ImGui::InputInt("conn_timer", &client->connection_timer);
        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
    else if (client_div && !client) {
        ImGui::BeginMenuBar();
        ImGui::Text("ENetClient");
        ImGui::EndMenuBar();
        ImGui::TextDisabled("Not connected.");
        ImGui::EndChild();
    }

    ImGui::Columns(1, nullptr, false);
    ImGui::PopStyleVar();

    if (ImGui::Button("Get fz/zf of fz.exe")) {
        uint32_t size{};
        auto file = utils::read_file("fz.exe", size);

        if (!file)
            printf("File fz.exe does not exist!\n");
        else {
            auto fz = size;
            auto zf = HashString((const char*)file, size);

            printf("fz: %d, zf: %d\n", fz, zf);
            free(file);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Decrypt registry values"))
        gt::decrypt_reg_vals();
    
    imwrap::tooltip("Does not do validity checks for checking if you actually have the keys, so can crash.");
    ImGui::SameLine();
    if (ImGui::Button("Print save.dat variantdb")) {
      
     auto vardb = (VariantDB*)((uintptr_t)global::app + 3976);
        vardb->Print();
    }
   

    ImGui::Text("GameLogic: %llx", logic);
}

