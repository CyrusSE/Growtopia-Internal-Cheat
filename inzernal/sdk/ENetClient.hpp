#pragma once
#include <core/utils.h>
#include <enet/include/enet.h>

#pragma pack(push, 1)
GTClass ENetClient {
   public:
    char unk0[160];
    ENetHost* host;
    ENetPeer* peer;
    int connection_timer;
    int tracking_tick;
    int net_msg_type;
    int another_timer;
    std::string address;
    int port;
    int unk;
    int token;
    int user; //this is also your userid
};
#pragma pack(pop)