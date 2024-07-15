#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
#include <hooks/SendPacketRaw.h>

void menu::autofarmmenu() {
    
    if (ImGui::CollapsingHeader("AutoFarm")) {
        if (ImGui::BeginChild("###customserver", AUTOSIZE(2.f), true)) {
            ImGui::Text("This thing only work on bfg :)");
            
            
        }
    }
}