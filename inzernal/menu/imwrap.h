#pragma once
#include <menu/menu.h>
#include <string>
#include <menu\imgui\imgui_internal.h>


struct InputTextCallback_UserData {
    std::string* Str;
    ImGuiInputTextCallback ChainCallback;
    void* ChainCallbackUserData;
};

#define BLOCK_SIZE 30.0f
#define AUTOSIZE(x) ImVec2(ImGui::GetWindowWidth() * 0.93f, BLOCK_SIZE * x)
#define AUTOSIZEC(x) ImVec2(ImGui::GetColumnWidth() - 10.f, BLOCK_SIZE * x)
#define RGBA(r, g, b, a) (((unsigned char)(a)) + ((unsigned char)(r) << 8) + ((unsigned char)(g) << 16) + ((unsigned char)(b) << 24))

namespace imwrap {

    //from private internal, might be used later on, so bringing it for now
    namespace col {
        struct im {
            float red;
            float green;
            float blue;
            float alpha;
        };

        //GT argb
        int im_to_gt(im& c);
        im gt_to_im(int gt);
    } 


    static auto vector_getter = [](void* vec, int idx, const char** out_text) {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) {
            return false;
        }
        *out_text = vector.at(idx).c_str();
        return true;
    };

    bool combo(const char* label, int* currIndex, std::vector<std::string>& values);
    bool listbox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1);
    void const_slider(const char* name, float min, float max, const char* format, int index, const char* tooltip = nullptr);
    void tooltip(const char* tip = nullptr);
    bool inputstring(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, const char* tip = nullptr);
    bool fchanger(const char* label, float& variable, bool& toggle, float min, float max);

    void prep_columns(int count);

     template <size_t N>
    void horizontal_tabs(char* (&names)[N], int& activetab, float w, float h) {
        bool values[N] = { false };
        values[activetab] = true;
        for (int i = 0; i < N; ++i) {
            if (ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h }, 1))
                activetab = i;

            if (i < N - 1)
                ImGui::SameLine();
        }
    }

    template <typename t>
    bool checkbox(const char* name, const char* label, t& variable, const char* tip = nullptr) {
        bool ret = false;
        if (ImGui::Checkbox(label, &variable)) {
            // save_changes(name, variable);
            ret = true;
        }
        tooltip(tip);
        return ret;
    }

	template <typename t>
    bool checkbox(const char* name, t& variable, const char* tip) {
        bool ret = checkbox(name, name, variable, tip);
        return ret;
    }


    static int InputTextCallback(ImGuiInputTextCallbackData* data) {
        InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            // Resize string callback
            // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
            std::string* str = user_data->Str;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        }
        else if (user_data->ChainCallback) {
            // Forward to user callback, if any
            data->UserData = user_data->ChainCallbackUserData;
            return user_data->ChainCallback(data);
        }
        return 0;
    }

} // namespace imwrap
