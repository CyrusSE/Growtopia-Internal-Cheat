#pragma once
#include <core/utils.h>

#pragma pack(push, 1)
GTClass NetObjectManager {
public :
    void* vtable;
    std::map<int, NetAvatar*> players; //key is netid
    //theres more after but we dont need them for now.
};
#pragma pack(pop)
