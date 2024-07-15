#pragma once
#include <string>
#include <vector>

namespace sig {
    enum : int {
        banbypass = 0,
        gamelogic = 1,
        world2screen,
        enetclient,
        baseapp_setfpslimit,
        logmsg,
        canmessaget4,
        sendpacketraw,
        handletouch,
        worldcamera_onupdate,
        sendpacket,
        processtankupdatepacket,
        canseeghosts,
        nethttp_update,
        app_update,
        tileextra_serialize,
        onpunched,
        getfruitbloompercent,
        dialogisopened,
        setextracharactermods,
        getsurfaceanim,
        mutexbypass1,
        mutexbypass2,
        s_renderd3d9,
        consts_offset,
        counter_offset
    };

    namespace type {
        enum : int {
            direct, //utils::find_pattern
            fstart, //utils::find_func_start
            call,   //detail::get_call
            direct_calc
            //leaving out some unused methods (like utils::find_func_end) of getting these for now
        };
    }

} // namespace sig

namespace sigs {
    extern std::vector<uintptr_t> database;
    uintptr_t add_pattern(std::string name, std::string pattern, int type, int offset = 0, bool ignore = false);
    void init();
    __forceinline uintptr_t get(int num) {
        return database[num];
    }
} // namespace sigs