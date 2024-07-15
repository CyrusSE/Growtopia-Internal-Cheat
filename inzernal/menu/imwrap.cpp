#pragma once
#include "imwrap.h"
#include <core/utils.h>
#include <sdk/Consts.h>

int imwrap::col::im_to_gt(im& c) {
    auto a = uint8_t(c.alpha * 255);
    auto r = uint8_t(c.red * 255);
    auto g = uint8_t(c.green * 255);
    auto b = uint8_t(c.blue * 255);
    return RGBA(r, g, b, a);
}
imwrap::col::im gt_to_im(int gt) {
    return { (float)HIBYTE(LOWORD(gt)) / 255.f, (float)LOBYTE(HIWORD(gt)) / 255.f, (float)HIBYTE(HIWORD(gt)) / 255.f, (float)LOBYTE(LOWORD(gt)) / 255.f };
}

//just ported from old csgo cheat, probably also taken from elsewhere partially
bool imwrap::combo(const char* label, int* currIndex, std::vector<std::string>& values) {
    if (values.empty())
        return false;

    return ImGui::Combo(label, currIndex, vector_getter, static_cast<void*>(&values), values.size());
}

bool imwrap::listbox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items) {
    if (values.empty())
        return false;

    return ImGui::ListBox(label, currIndex, vector_getter, static_cast<void*>(&values), values.size(), height_in_items);
}

void imwrap::const_slider(const char* name, float min, float max, const char* format, int index, const char* tooltip) {
    if (ImGui::Button(utils::format("Reset###re%d", index).c_str())) {
        consts::values[index] = consts::defs[index];
        consts::set_float(index, consts::defs[index]);
    }
    ImGui::SameLine();

    char metaformat[] = "%s: %s"; //big brain shit
    ImGui::PushItemWidth(ImGui::GetColumnWidth() - 3.f);
    if (ImGui::SliderFloat(utils::format("###cl%d", index).c_str(), &consts::values[index], min, max, utils::format(metaformat, name, format).c_str(),
            ImGuiSliderFlags_TextPositionLeft))
        consts::set_float(index, consts::values[index]);
    ImGui::PopItemWidth();
    imwrap::tooltip(tooltip);
}

void imwrap::tooltip(const char* tip) {
    if (tip) {
        if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.2f) //we do not want to hover when mouse just passes by the item therefore delay
            ImGui::SetTooltip(tip);
    }
}

//taken straight out of imgui_stdlib.cpp, since im using older imgui
bool imwrap::inputstring(const char* label, std::string* str, ImGuiInputTextFlags flags, const char* tip) {
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = nullptr;
    cb_user_data.ChainCallbackUserData = nullptr;
    auto retval = ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    tooltip(tip);
    return retval;
}

bool imwrap::fchanger(const char* label, float& variable, bool& toggle, float min, float max) {
    bool change_asap = false;

    if (ImGui::BeginChild(utils::format("###%s", label).c_str(), AUTOSIZEC(2.f), true, ImGuiWindowFlags_MenuBar)) {
        ImGui::BeginMenuBar();
        ImGui::Text(label);
        ImGui::EndMenuBar();
        if (ImGui::Checkbox("Enabled", &toggle) && !toggle)
            change_asap = true;

        ImGui::SameLine();
        ImGui::SliderFloat("", &variable, min, max, "%0.0f");
        ImGui::EndChild();
    }
    ImGui::Spacing();
    return change_asap;
}

void imwrap::prep_columns(int count) {
    auto& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
    ImGui::Columns(count, nullptr, false);
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
    float col_w = group_w / (float)count;

    for (int i = 1; i <= count; i++)
        ImGui::SetColumnOffset(i, i * col_w);
}
