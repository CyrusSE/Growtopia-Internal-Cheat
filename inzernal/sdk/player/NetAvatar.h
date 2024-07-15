#pragma once
#include <hooks/hooks.h>
#include <proton/clanlib/vec2.h>
#include <sdk/EntityComponent.h>
#include <proton/variant2.hpp>
#include <string>
#include <core/gt.h>

#pragma pack(push, 1)
GTClass EncryptedFloat {
   private:
    float enc_first;
    float decrypted; //this is the straight up value for other players, they dont have encrypted values
    float enc_second;
    float weight;

   public:
    //basically gt sets weight to random float between 3 and 29384
    //enc_half has 0.5*weight + the actual value
    //enc         has weight + the actual value
    //gt checks both enc_first and enc_second and the weight in their anti hack checks and so on

    float get(bool local = true) {
        return local ? (enc_second - weight) : decrypted;
    }
    //this is just smth i used for my testing to make sure everythings right, gt also uses this formula to deduce hacks
    float bakery() {
        return enc_second - ((enc_first - weight * 0.5f) + weight);
    }
    void set(float value, bool local = true) {
        if (local) {
            enc_second = weight + value;
            enc_first = (weight * 0.5f) + value;
        }
        else
            decrypted = value;
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
GTClass NetAvatar { //how fucking annoying it is to get align to work
   public:
    void* vtable;
    CL_Vec2f pos;
    CL_Vec2f size;
    CL_Vec2f pos_enc; //use this one for local
    CL_Vec2f size_enc;
    std::string name;
    uint32_t netid;
    uint32_t destroy;
    EntityComponent* entitycomponent;
    void* unk0;
    byte unk1;
    byte facing_left;
    char pad[2];
    void* subpixelmemory;
    char pad0a[20];
    EncryptedFloat punch_cooldown;
    uint32_t skin_color;
    char pad1a[20];
    std::string unkstr4; //didnt find anything about this
    float jump_progress;
    float unk3;
    uint32_t tile_x;
    uint32_t tile_y;
    char unk02[12];
    EncryptedFloat velocity_x; //could be fun to do all kinds of things with messing with these values
    EncryptedFloat velocity_y;
    char unk6[8];
    int emotion;
    float emotion_time;
    char unk7[4];
    void* NetControllerLocal; //280
    void* AvatarPacketReceiver;
    void* AvatarPacketSender;
    bool jump_state;
    char unk7a[3];
    uint32_t flags;
    int freeze_state;
    int userid;
    char unk7b[16];
    std::string country;
    char unk8a[8];
    bool is_invis;
    bool is_mod;
    bool is_supermod;
    bool some_unk2;
    bool some_unk3;
    int tile_id;
    char pad8b[4];
    int bubble_status;
    char unk8[59];
    EncryptedFloat gravity;
    EncryptedFloat accel;
    EncryptedFloat speed;
    float water_speed;
    int unk_smth;
    EncryptedFloat punch_strength; //the one which dragon and etc increases
    int unk50;
    std::string unkstr1; //these get set by NetAvatar::OnWepSfx, but i have no clue what that is either
    std::string unkstr2;
    std::string unkstr3;
    char unk10[16];
    int client_hack_type; //ban report type for SendMessageT4

    //TODO: somehow differentiate between local and others so you can use SetPos, GetPos for other players too
    //(since they dont use encrypted coords too)

    void SetPos(const CL_Vec2f& n) {
        SetPos(n.x, n.y);
    }
    void SetPos(float x, float y) {
        pos.x = x;
        pos.y = y;
        pos_enc.x = x / 6.5999999f;
        pos_enc.y = y / 1.3f;
    }
    void SetPosAtTile(int x, int y) {
        SetPos(x * 32.0f + 8.0f, y * 32.0f);
    }

    //was a private feature before but now that its been leaked it will be public
    void send_nazi_slime(int x, int y) {
        GameUpdatePacket packet{ 0 };
        packet.type = 46;
        packet.int_data = 3728;
        auto do_stuff = [&](auto x, auto y) {

            packet.int_x = x;
            packet.int_y = y;
            for (int i = 0; i < 5; i++) {
                gt::send(&packet);
            }
        };

        do_stuff(x, y);
        do_stuff(x, y + 1);
        do_stuff(x, y + 2);
        do_stuff(x, y + 3);
        do_stuff(x, y + 4);

        do_stuff(x + 1, y + 4);
        do_stuff(x + 2, y + 4);
        do_stuff(x + 3, y + 4);
        do_stuff(x + 4, y + 4);
        do_stuff(x + 5, y + 4);
        do_stuff(x + 6, y + 4);

        do_stuff(x + 6, y + 4);
        do_stuff(x + 6, y + 5);
        do_stuff(x + 6, y + 6);
        do_stuff(x + 6, y + 7);

        do_stuff(x + 6, y);
        do_stuff(x + 5, y);
        do_stuff(x + 4, y);
        do_stuff(x + 3, y);

        do_stuff(x + 3, y + 1);
        do_stuff(x + 3, y + 2);
        do_stuff(x + 3, y + 3);

        do_stuff(x + 3, y + 5);
        do_stuff(x + 3, y + 6);
        do_stuff(x + 3, y + 7);

        do_stuff(x + 2, y + 7);
        do_stuff(x + 1, y + 7);
        do_stuff(x, y + 7);
    }

    void send_nazi_smoke(int xx, int yy, int realx, int realy) {
        GameUpdatePacket packet{ 0 };
        packet.type = PACKET_STATE;
        packet.int_data = 2034;
        packet.flags = (1 << 5) | (1 << 10) | (1 << 11);
        packet.pos_x = realx;
        packet.pos_y = realy;
        auto do_stuff = [&](auto x, auto y) {
            packet.int_x = x;
            packet.int_y = y;
            for (int i = 0; i < 5; i++) {
                gt::send(&packet);
            }
        };
        
        do_stuff(xx, yy);
        do_stuff(xx, yy + 1);
        do_stuff(xx, yy + 2);
        do_stuff(xx, yy + 3);
        do_stuff(xx, yy + 4);

        do_stuff(xx + 1, yy + 4);
        /*
        
        do_stuff(x + 2, y + 4);
        do_stuff(x + 3, y + 4);
        do_stuff(x + 4, y + 4);
        do_stuff(x + 5, y + 4);
        do_stuff(x + 6, y + 4);

        do_stuff(x + 6, y + 4);
        do_stuff(x + 6, y + 5);
        do_stuff(x + 6, y + 6);
        do_stuff(x + 6, y + 7);

        do_stuff(x + 6, y);
        do_stuff(x + 5, y);
        do_stuff(x + 4, y);
        do_stuff(x + 3, y);

        do_stuff(x + 3, y + 1);
        do_stuff(x + 3, y + 2);
        do_stuff(x + 3, y + 3);

        do_stuff(x + 3, y + 5);
        do_stuff(x + 3, y + 6);
        do_stuff(x + 3, y + 7);

        do_stuff(x + 2, y + 7);
        do_stuff(x + 1, y + 7);
        do_stuff(x, y + 7);
        */
       
    }

    //for local only
    CL_Vec2f GetPos() {
        //returning the encrypted one cuz its 100% what the server has
        return CL_Vec2f(pos_enc.x * 6.5999999f, pos_enc.y * 1.3f);
    }
    void SetSize(const CL_Vec2f& n) {
        SetSize(n.x, n.y);
    }
    void SetSize(float x, float y) {
        size.x = x;
        size.y = y;
        size_enc.x = x / 6.5999999f;
        size_enc.y = y / 1.3f;
    }

    CL_Vec2f GetSize() {
        return size;
    }

    void SetModStatus(bool mod, bool supermod) {
        is_mod = mod;
        is_supermod = supermod;
    }
    void SetCharacterMods(bool dash, bool charge, bool cancel) {
    
        uint8_t _flags = (dash << 1) | (charge << 0) | (cancel << 2);
        gt::set_extra_character_mods(this, _flags);
    }
};
#pragma pack(pop)
