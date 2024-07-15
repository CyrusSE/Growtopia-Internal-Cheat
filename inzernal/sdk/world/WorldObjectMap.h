#pragma once
#include <core/utils.h>
#include <sdk/world/WorldObject.h>

#pragma pack(push, 1)
GTClass WorldObjectMap {
   public:
    void* vftable;
    uint32_t object_counter;
    uint32_t unk;
   public:
    std::list<WorldObject> objects;
};

#pragma pack(pop)