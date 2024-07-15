#pragma once
#define WIN32_LEAN_AND_MEAN
#include <aclapi.h>
#include <core/globals.h>
#include <core/gt.h>
#include <core/utils.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <iostream>

HANDLE _out = NULL, _old_out = NULL;
HANDLE _err = NULL, _old_err = NULL;
HANDLE _in = NULL, _old_in = NULL;
FILE* _out_b = nullptr;
int _out_h = 0;

typedef std::mt19937 rng_type;
rng_type rng;

void utils::attach_console() {
    //set up utils::print to work using WriteConsoleA
    _old_out = GetStdHandle(STD_OUTPUT_HANDLE);
    _old_err = GetStdHandle(STD_ERROR_HANDLE);
    _old_in = GetStdHandle(STD_INPUT_HANDLE);
    ::AllocConsole() && ::AttachConsole(GetCurrentProcessId());
    _out = GetStdHandle(STD_OUTPUT_HANDLE);
    _err = GetStdHandle(STD_ERROR_HANDLE);
    _in = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(_out, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleMode(_in, ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);

    //setup printf, cout, fwrite to stdout to work
    freopen_s(&_out_b, "nul", "w", stdout);
    auto outfd = _open_osfhandle((intptr_t)_out, _O_TEXT);
    auto outf = _fdopen(outfd, "w");
    if (outf) {
        _out_h = _fileno(stdout);
        _dup2(_fileno(outf), _out_h);
        setvbuf(stdout, NULL, _IONBF, 0);
    }
}



void utils::detach_console() {
    if (_out && _err && _in) {
        FreeConsole();
        if (_out_h != -1 && _out_h != 0)
            _dup2(_fileno(stdout), _out_h);

        if (_old_out)
            SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
        if (_old_err)
            SetStdHandle(STD_ERROR_HANDLE, _old_err);
        if (_old_in)
            SetStdHandle(STD_INPUT_HANDLE, _old_in);
    }
}

std::string utils::format(const char* msg, ...) {
    char fmt[1024] = { 0 };
    va_list va;
    va_start(va, msg);
    vsnprintf(fmt, 1024, msg, va);
    va_end(va);
    return std::string(fmt);
}
char utils::read_key() {
    if (!_in)
        return false;
    char key = 0;
    DWORD keysread = 0;

    ReadConsoleA(_in, &key, 1, &keysread, nullptr);
    return key;
}

int utils::random(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(rng);
}
void utils::printc(std::string color, const char* fmt, ...) {
    if (!_out)
        return;

    char buf[2048];
    int len = -1;
    va_list va;
    va_start(va, fmt);
    if (0 > (len = _vsnprintf_s(buf, sizeof(buf), fmt, va))) {
        va_end(va);
        return;
    }
    va_end(va);
    printf("\x1B[%sm%s\n\033[0m", color.c_str(), buf);
}
void utils::seed_random() {
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed((uint32_t)seed);
    printf("set random seed: %lld\n", seed);
}

uint8_t* utils::read_file(const std::string& file, uint32_t& size) {
    auto f = fopen(file.data(), "rb");
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    auto data = (uint8_t*)malloc(size);
    fseek(f, 0, SEEK_SET);
    fread(data, 1, size, f);
    fclose(f);
    return data;
}

const char hexmap_s[17] = "0123456789abcdef";
std::string utils::hex_str(unsigned char data) {
    std::string s(2, ' ');
    s[0] = hexmap_s[(data & 0xF0) >> 4];
    s[1] = hexmap_s[data & 0x0F];
    return s;
}

std::string utils::between(const std::string& s, const std::string& start_delim, const std::string& stop_delim) {
    size_t first_delim_pos = s.find(start_delim);
    size_t end_pos_of_first_delim = first_delim_pos + start_delim.length();
    size_t last_delim_pos = s.find_first_of(stop_delim, end_pos_of_first_delim);
    return s.substr(end_pos_of_first_delim, last_delim_pos - end_pos_of_first_delim);
}

bool utils::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string utils::rnd(size_t length) {
    static auto randchar = []() -> char {
        const char charset[] =
            "0123456789"
            "qwertyuiopasdfghjklzxcvbnm"
            "QWERTYUIOPASDFGHJKLZXCVBNM";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[utils::random(INT16_MAX, INT32_MAX) % max_index];
    };

    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

float utils::random_float(float a, float b) {
    std::uniform_real_distribution<float> distribution(a, b);
    return distribution(rng);
}

bool utils::run_at_interval(types::time& timer, double interval, bool sway, float swayd) {
    types::time now = std::chrono::system_clock::now();
    types::elapsed elapsed_sec = now - timer;

    if (sway)
        interval += random_float(-swayd, swayd);

    if (elapsed_sec.count() >= interval) {
        timer = now;
        return true;
    }
    return false;
}

std::string utils::get_clipboard() {
    // Try opening the clipboard
    if (!OpenClipboard(nullptr))
        return "error";

    // Get handle of clipboard object for ANSI text
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr)
        return "error";

    // Lock the handle to get the actual text pointer
    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr)
        return "error";

    // Save text in a string class instance
    std::string text(pszText);

    // Release the lock
    GlobalUnlock(hData);

    // Release the clipboard
    CloseClipboard();

    return text;
}

//unprotection coded by atipls
void utils::
    unprotect_process() //i tried returning a bool but its just so t h i c k code with 80 variables needed to clean up its just not worth it currently, if it fails then theres bigger problems at some other place for sure
{
    void* token = NULL;
    void* token_info = NULL;
    PSID psid_everyone = NULL;
    PSID psid_system = NULL;
    PSID psid_admins = NULL;
    PACL dacl = NULL;
    PSECURITY_DESCRIPTOR descriptor = NULL;
    unsigned long size = 0;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &token))
        return;
    GetTokenInformation(token, TokenUser, NULL, 0, &size);
    if (size)
        token_info = HeapAlloc(GetProcessHeap(), 0, size);
    GetTokenInformation(token, TokenUser, token_info, size, &size);
    PSID psid_curuser = ((TOKEN_USER*)token_info)->User.Sid;
    SID_IDENTIFIER_AUTHORITY sidEveryone = SECURITY_WORLD_SID_AUTHORITY;
    SID_IDENTIFIER_AUTHORITY sidSystem = SECURITY_NT_AUTHORITY;
    SID_IDENTIFIER_AUTHORITY sidAdministrators = SECURITY_NT_AUTHORITY;
    AllocateAndInitializeSid(&sidEveryone, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &psid_everyone);
    AllocateAndInitializeSid(&sidSystem, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &psid_system);
    AllocateAndInitializeSid(&sidAdministrators, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &psid_admins);
    const PSID sid_array[] = {
        psid_everyone,
        psid_curuser,
        psid_system,
        psid_admins,
    };
    size = sizeof(ACL);
    size += GetLengthSid(sid_array[0]);
    for (UINT i = 0; i < _countof(sid_array); i++) {
        size += GetLengthSid(sid_array[i]);
        size += sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD);
    }
    dacl = PACL(HeapAlloc(GetProcessHeap(), 0, size));
    InitializeAcl(dacl, size, ACL_REVISION);
    AddAccessAllowedAce(dacl, ACL_REVISION, PROCESS_ALL_ACCESS, sid_array[0]);
    AddAccessAllowedAce(dacl, ACL_REVISION, PROCESS_ALL_ACCESS, sid_array[1]);
    AddAccessAllowedAce(dacl, ACL_REVISION, PROCESS_ALL_ACCESS, sid_array[2]);
    AddAccessAllowedAce(dacl, ACL_REVISION, PROCESS_ALL_ACCESS, sid_array[3]);
    descriptor = (PSECURITY_DESCRIPTOR)HeapAlloc(GetProcessHeap(), 0, SECURITY_DESCRIPTOR_MIN_LENGTH);
    InitializeSecurityDescriptor(descriptor, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(descriptor, TRUE, dacl, FALSE);
    SetSecurityInfo(GetCurrentProcess(), SE_KERNEL_OBJECT, OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, psid_curuser, NULL, dacl, NULL);
    if (descriptor)
        HeapFree(GetProcessHeap(), 0, descriptor);
    if (dacl)
        HeapFree(GetProcessHeap(), 0, dacl);
    if (psid_admins)
        FreeSid(psid_admins);
    if (psid_system)
        FreeSid(psid_system);
    if (psid_everyone)
        FreeSid(psid_everyone);
    if (token_info)
        HeapFree(GetProcessHeap(), 0, token_info);
    if (token)
        CloseHandle(token);
 
    printf("restored full security permissions\n");
}

//advanced pattern scanning functions that I have been honing for a long time for Growtopia specifically
//When used properly you can guarantee at least 30 versions lasting signature to something
#pragma region advanced
uintptr_t utils::find_func_start(uintptr_t addr) {
    if (addr < 15) {
        printf("[failsafe] find_func_start got too small address.\n");
        return uintptr_t();
    }

    int count = 0;
    int times = 0;
    bool lastwas = false;
    bool anywas = false;
    while (true) {
        if (times >= 3 || (times >= 2 && !anywas))
            return addr + count + times + 1;
        if (read<uint8_t>(addr, count) == 0xCC) {
            lastwas = false;
            times++;
        }
        else if (times > 1 && !lastwas && read<uint8_t>(addr, count) == 0xC3) {
            lastwas = true;
            anywas = true;
            times++;
        }
        else {
            lastwas = false;
            times = 0;
        }

        count--;
    }
    return uintptr_t();
}
uintptr_t utils::find_func_start(const char* pattern) {
    return utils::find_func_start(utils::find_pattern(pattern));
}
uintptr_t utils::find_func_end(uintptr_t addr) {
    if (addr < 15) {
        printf("[failsafe] find_func_end got too small address.\n");
        return uintptr_t();
    }
    int count = 0;
    int times = 0;
    bool lastwasret = false;
    bool lastwascc = false;
    auto mod = global::gt;
    auto doshdr = PIMAGE_DOS_HEADER(mod);
    auto nthdrs = PIMAGE_NT_HEADERS(uintptr_t(mod) + doshdr->e_lfanew);
    auto opthdr = &nthdrs->OptionalHeader;
    auto start = uintptr_t(mod) + opthdr->BaseOfCode;
    auto end = opthdr->SizeOfCode;
    auto last = start + end;
    while (true) {
        auto ret = addr + count - times + 1;
        if (ret > last)
            return 0x0;

        if (times >= 2 && lastwascc)
            return ret;

        bool set = false;
        if (read<uint8_t>(addr, count) == 0xC3) {
            times++;
            lastwasret = true;
            lastwascc = false;
            goto end;
        }
        else
            set = true;

        if (lastwasret || lastwascc) {
            if (read<uint8_t>(addr, count) == 0xCC) {
                lastwascc = true;
                lastwasret = false;
                times++;
            }
            else {
                lastwascc = false;
                times = 0;
            }
        }
        if (set)
            lastwasret = false;
    end:
        count++;
    }
    return uintptr_t();
}
uintptr_t utils::find_pattern_in_area(uintptr_t start, uintptr_t end, const char* pat) {
    static auto pat2byt = [](const char* pat) {
        std::vector<int> bytes{};
        auto start = (char*)pat;
        auto end = start + strlen(pat);

        for (auto curr = start; curr < end; ++curr) {
            if (*curr == '?') {
                ++curr;
                if (*curr == '?')
                    ++curr;
                bytes.push_back(-1);
            }
            else
                bytes.push_back(strtoul(curr, &curr, 16));
        }
        return bytes;
    };
    auto bytes = pat2byt(pat);
    auto data = bytes.data();
    auto size = bytes.size();
    if (size < 1)
        return 0x0;

    bool found = false;
    for (size_t i = start; i <= end; i++) {
        for (size_t t = 0; t < size; t++) {
            if (*((PBYTE)i + t) == data[t] || data[t] == -1)
                found = true;
            else {
                found = false;
                break;
            }
        }
        if (found)
            return uintptr_t(i);
    }
    return 0;
}
uintptr_t utils::find_func_has(std::vector<const char*> to_be_found, uint32_t max_length) {
    auto mod = global::gt;
    std::vector<uintptr_t> candidates = {};

    auto doshdr = PIMAGE_DOS_HEADER(mod);
    auto nthdrs = PIMAGE_NT_HEADERS(uintptr_t(mod) + doshdr->e_lfanew);
    auto opthdr = &nthdrs->OptionalHeader;
    auto start = uintptr_t(mod) + opthdr->BaseOfCode;
    auto end = opthdr->SizeOfCode;

    if (!start || !end)
        return 0x0;

    start += 0x5000; //rig so it wont go thru the shit that we dont want to search anyways, nothing is ever at the start

    static auto pat2byt = [](const char* pat) {
        std::vector<int> bytes{};
        auto start = (char*)pat;
        auto end = start + strlen(pat);

        for (auto curr = start; curr < end; ++curr) {
            if (*curr == '?') {
                ++curr;
                if (*curr == '?')
                    ++curr;
                bytes.push_back(-1);
            }
            else
                bytes.push_back(strtoul(curr, &curr, 16));
        }
        return bytes;
    };
    auto bytes = pat2byt(to_be_found.at(0));
    auto data = bytes.data();
    auto size = bytes.size();
    if (size < 1)
        return 0x0;

    bool found = false;
    for (size_t i = start; i <= start + end; i++) {
        for (size_t t = 0; t < size; t++) {
            if (*((PBYTE)i + t) == data[t] || data[t] == -1)
                found = true;
            else {
                found = false;
                break;
            }
        }
        if (found) {
            candidates.push_back(i);
            found = false;
        }
    }
    for (auto candidate : candidates) {
        auto start_ = find_func_start(candidate);
        auto end_ = find_func_end(candidate);
        auto len = end_ - start_;
        if (max_length != 0 && len > max_length) //max function length. useful if you need small funcs.
            continue;

        bool all_found_in_func = false;
        for (auto pattern : to_be_found) {
            if (find_pattern_in_area(start_, end_, pattern) >= start_)
                all_found_in_func = true;
            else {
                all_found_in_func = false;
                break;
            }
        }
        if (all_found_in_func)
            return start_;
    }

    return 0x0;
}

bool utils::is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin() + (*s.data() == '-' ? 1 : 0), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool utils::read_reg_value(LPCSTR subkey, LPCSTR value, LPBYTE data, LPDWORD data_len) {
    *data_len = 1024;
    DWORD type = 3;
    HKEY hkey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, subkey, 0, 1, &hkey))
        return false;

    int res = RegQueryValueExA(hkey, value, 0, &type, data, data_len);
    RegCloseKey(hkey);
    return res == 0;
}

#pragma endregion