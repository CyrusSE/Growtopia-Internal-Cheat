#pragma once
#include <sdk/sdk.h>

GameLogic* sdk::GetGameLogic() {
    static auto address = (GameLogic * (__cdecl*)()) sigs::get(sig::gamelogic);
    return address();
}

ENetClient* sdk::GetClient() {
    static auto client = (ENetClient * (__cdecl*)()) sigs::get(sig::enetclient);
    return client();
}

ENetPeer* sdk::GetPeer() {
    return GetClient()->peer;
}

App* sdk::GetApp() {
    return nullptr; //nullptr for now
}
