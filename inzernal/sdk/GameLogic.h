#pragma once
#include <core/globals.h>
#include <core/utils.h>
#include <sdk/world/World.h>
#include <core/sigs.hpp>
#include <sdk/NetObjectManager.h>

struct WorldRenderer;
struct PlayerItems;

#pragma pack(push, 1)
GTClass GameLogic {
    char pad[248];
    bool local_building;
    char pad1[15];
    World* world;
    WorldRenderer* renderer;
    char pad2[72];
    NetObjectManager netobjmgr;
    char pad3[56];
    NetAvatar* local;
    uint64_t* textmanager; //no container yet
    char pad3a[40];
    uint64_t* hudindicatormgr;
    char pad4[24];
    PlayerItems* playeritems;
    char pad5[100];
    int unk_network;
    char pad5a[8];
    uint64_t* effectmgr;
    char pad6[28];
    int32_t punch_range; //WARNING: can cause bans. 0 = your normal range (2), so with heartbow its 1, etc.
    int32_t build_range; //same as with punch, you can get banned for trying to do this, so use how you wish
    char pad7[28];
    uint8_t unk_byte;
    char pad8[7];
    uint64_t* battlestatus;
    char pad9[96];
    bool packetwatcher_on;

   public:
    //TODO: populate gamelogiccomponent

    //methods straight from gt
    float GetPunchRange() {
        return (punch_range + 2) * 32.f;
    }
    float GetBuildRange() {
        return (build_range + 2) * 32.f;
    }
    NetAvatar* GetLocalPlayer() {
        return local;
    }

    //render + tilemap offsets have not changed since 2.996, probably safe to use struct
    WorldTileMap* GetTileMap() {
        if (!world)
            return nullptr;
        return &world->tilemap;
    }
    WorldObjectMap* GetObjectMap() {
        if (!world)
            return nullptr;
        return &world->worldobjectmap;
    }
    WorldRenderer* GetWorldRenderer() {
        return renderer;
    }
    NetObjectManager* GetNetObjMgr() {
        return &netobjmgr;
    }
    World* GetWorld() {
        return world;
    }
    /* WorldCamera* GetWorldCamera() {
    }*/
};

#pragma pack(pop)
