#pragma once
#include <core/utils.h>

#pragma pack(push, 1)
GTClass WorldObject {
   public:
    void* vtable;
    vector2_t pos;
    uint16_t item_id;
    uint8_t amount;
    uint8_t flags;
    uint32_t object_id;
    char padding[24]; //TODO: figure this out
};

#pragma pack(pop)