#pragma once
#include <sdk/sdk.h>

#pragma pack(push, 1)
GTClass SurfaceAnim {
   public:
    char pad[24];
    uint16_t texture_id;
    uint16_t unk;
    uint32_t width;
    uint32_t height;
};
#pragma pack(pop)