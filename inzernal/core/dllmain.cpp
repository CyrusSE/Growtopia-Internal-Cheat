#pragma once
#include <core/globals.h>
#include <core/gt.h>
#include <core/minhook/hook.h>
#include <core/utils.h>
#include <hooks\hooks.h>
#include <process.h>
#include <proton\MiscUtils.h>
#include <core/sigs.hpp>


bool unload_done = false;
HMODULE self = nullptr;
void on_exit() {
    if (unload_done) //since this function can be called by DLL_PROCESS_DETACH and unload loop, we only want to do this once.
        return;

    printf("Cleaning up internal changes....\n");
    hooks::destroy();
    Sleep(200);

    printf("Cleaning done\tDetaching console and exiting process.\n");
    Sleep(600);
    utils::detach_console();

    unload_done = true;
    FreeLibraryAndExitThread(self, 1);
}

void on_inject() {
    MH_Initialize();
    
    utils::attach_console();

    SetConsoleTitleA("[INZERNAL] by Cyrus#8000");
    utils::printc("96", "INZERNAL - internal framework and enhancement suite by Cyrus#8000");
    printf("Base address: 0x%llx\n", global::gt);
    utils::seed_random();
    sigs::init();

    if (!gt::patch_banbypass())
        on_exit();
    if (!gt::patch_mutex())
        printf("Failed in patching mutex checks, your choice if you want to still keep running.\n");

    hooks::init();


    while (!global::unload)
        Sleep(10);

    on_exit();
}

//for patcher to work
void dll() {
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        self = HMODULE(dll);
        global::gt = (uintptr_t)GetModuleHandleW(nullptr);
        DisableThreadLibraryCalls(HMODULE(dll));
        CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(on_inject), nullptr, 0, nullptr);
        return 1;
    }
    return 0;
}
