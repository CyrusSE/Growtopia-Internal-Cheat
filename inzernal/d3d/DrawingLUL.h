#include <WinSock2.h>
#include <menu/imgui/imgui.h>
#include <string>
#include <Windows.h>





typedef struct {
    DWORD R;
    DWORD G;
    DWORD B;
    DWORD A;
} RGBA;
RGBA red = { 255, 0, 0, 255 };

static void DrawRect(int x, int y, int w, int h, RGBA* color, int thickness) {
    ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h),
        ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0, thickness);
}
static void DrawFilledRect(int x, int y, int w, int h, RGBA* color) {
    ImGui::GetOverlayDrawList()->AddRectFilled(
        ImVec2(x, y - 1), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0, 0);
    ImGui::GetOverlayDrawList()->AddRectFilled(
        ImVec2(x, y + 1), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0, 0);
    ImGui::GetOverlayDrawList()->AddRectFilled(
        ImVec2(x - 1, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0, 0);
    ImGui::GetOverlayDrawList()->AddRectFilled(
        ImVec2(x + 1, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0, 0);
    ImGui::GetOverlayDrawList()->AddRectFilled(
        ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0);
}
static void DrawFilledRectIMCol(int x, int y, int w, int h, ImU32 col) {
    ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), col, 0, 0);
}
static void DrawCircleFilled(int x, int y, int radius, RGBA* color, int segments) {
    ImGui::GetOverlayDrawList()->AddCircleFilled(
        ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}
static void DrawCircle(int x, int y, int radius, RGBA* color, int segments) {
    ImGui::GetOverlayDrawList()->AddCircle(
        ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}
static void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color, float thickne) {
    ImGui::GetOverlayDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
        ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickne);
}
static void DrawTriangleFilled(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color) {
    ImGui::GetOverlayDrawList()->AddTriangleFilled(
        ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}
static void DrawLine(int x1, int y1, int x2, int y2, ImColor color, int thickness) {
    ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}
static void DrawCornerBox(int x, int y, int w, int h, int borderPx, RGBA* color) {
    DrawFilledRect(x + borderPx, y, w / 3, borderPx, color);                                //top
    DrawFilledRect(x + w - w / 3 + borderPx, y, w / 3, borderPx, color);                    //top
    DrawFilledRect(x, y, borderPx, h / 3, color);                                           //left
    DrawFilledRect(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);                //left
    DrawFilledRect(x + borderPx, y + h + borderPx, w / 3, borderPx, color);                 //bottom
    DrawFilledRect(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx, color);     //bottom
    DrawFilledRect(x + w + borderPx, y, borderPx, h / 3, color);                            //right
    DrawFilledRect(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color); //right
}
