#pragma once

//include everything here so no need to include them individually in other files
#include <sdk/GameLogic.h>
#include <sdk/GameUpdatePacket.h>
#include <sdk/player/NetAvatar.h>
#include <sdk/world/WorldRenderer.h>
#include <sdk/world/WorldTileMap.h>
#include <sdk/ENetClient.hpp>
#include <core/sigs.hpp>

namespace sdk {
    GameLogic* GetGameLogic();
    ENetClient* GetClient();
    ENetPeer* GetPeer();
    App* GetApp();
} // namespace sdk