#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#pragma pack(push, 1)
struct GameUpdatePacket {
    uint8_t type{}; //0
    union {
        uint8_t objtype{}; //1
        uint8_t punchid;
        uint8_t npctype;
    };
    union {
        uint8_t count1{}; //2
        uint8_t jump_count;
        uint8_t build_range;
        uint8_t npc_id;
        uint8_t lost_item_count;
    };
    union {
        uint8_t count2{}; //3
        uint8_t animation_type;
        uint8_t punch_range;
        uint8_t npc_action;
        uint8_t particle_id;
        uint8_t gained_item_count;
        uint8_t dice_result;
        uint8_t fruit_count;
    };
    union {
        int32_t netid{}; //4 NetID
        int32_t effect_flags_check;
        int32_t object_change_type;
        int32_t particle_emitter_id;
    };
    union {
        int32_t item{}; //8 
        int32_t ping_hash;
        int32_t item_netid;
        int32_t pupil_color;
        int32_t tiles_length;
    };
    int32_t flags{}; //12 - 0xC //characterState
    union {
        float float_var{}; //16 - 0x10
        float water_speed;
        float obj_alt_count;
    };
    union {
        int32_t int_data{}; //20 - 0x14 MainValue or PlantingTree
        int32_t ping_item;
        int32_t elapsed_ms;
        int32_t delay;
        int32_t tile_damage;
        int32_t item_id;
        int32_t item_speed;
        int32_t effect_flags;
        int32_t object_id;
        int32_t hash;
        int32_t verify_pos;
        int32_t client_hack_type;
    };
    union {
        float vec_x{}; //24 - 0x18 PosX
        float pos_x;
        float accel;
        float punch_range_in;
    };
    union {
        float vec_y{}; //28 - 0x1C PosY
        float pos_y;
        float build_range_in;
        float punch_strength;
    };
    union {
        float vec2_x{}; //32 - 0x20 XSpeed
        float dest_x;
        float gravity_in;
        float speed_out;
        float velocity_x;
        float particle_variable;
        float pos2_x;
        int hack_type;
    };
    union {
        float vec2_y{}; //36 - 0x24  YSpeed
        float dest_y;
        float speed_in;
        float gravity_out;
        float velocity_y;
        float particle_alt_id;
        float pos2_y;
        int hack_type2;
    };
    union {
        float particle_rotation{}; //40 - 0x28
        float npc_variable;
    };
    union {
        uint32_t int_x{}; //44 - 0x2C PunchX
        uint32_t item_id_alt;
        uint32_t eye_shade_color;
    };
    union {
        uint32_t int_y{}; //48 - 0x30 PunchY
        uint32_t item_count;
        uint32_t eyecolor;
        uint32_t npc_speed;
        uint32_t particle_size_alt;
    };
    uint32_t data_size;
    uint32_t data; //not officially a part of gameupdatepacket, thus sizeof(GameUpdatePacket) is 60 and not 56

    void debug_print(const char* name) {
        printf("---- start debug print ----\n");
        printf("\tplayer: %s\n", name);
        printf("type: %d\tobjtype: %d\tcount1: %d\tcount2: %d\n", type, objtype, count1, count2);
        printf("netid: %d\titem: %d\tflags: %d\tfloat1: %0.2f\n", netid, item, flags, float_var);
        printf("idata: %d\tvecx: %0.2f\tvecy: %0.2f\n", int_data, vec_x, vec_y);
        printf("vec2x: %0.2f\tvec2y: %0.2f\tpart: %0.2f\n", vec2_x, vec2_y, particle_rotation);
        printf("int_x: %d\tint_y: %d\tdata_size: %d\n", int_x, int_y, data_size);
        if (fabsf(0.f - float_var) < 0.01f && float_var != 0.f)
            printf("float var (small): %f (%d)\n", float_var, *(int*)&float_var);
        if (fabsf(0.f - vec_x) < 0.01f && vec_x != 0.f)
            printf("vecx (small): %f (%d)\n", vec_x, *(int*)&vec_x);
        if (fabsf(0.f - vec_y) < 0.01f && vec_y != 0.f)
            printf("vecy (small): %f (%d)\n", vec_y, *(int*)&vec_y);
        if (fabsf(0.f - vec2_x) < 0.01f && vec2_x != 0.f)
            printf("vec2x (small): %f (%d)\n", vec2_x, *(int*)&vec2_x);
        if (fabsf(0.f - vec2_y) < 0.01f && vec2_y != 0.f)
            printf("vec2y (small): %f (%d)\n", vec2_y, *(int*)&vec2_y);
        if (fabsf(0.f - particle_rotation) < 0.01f && particle_rotation != 0.f)
            printf("part (small): %f (%d)\n", particle_rotation, *(int*)&particle_rotation);
        printf("---- end debug print ----\n");
    }
    
};
typedef struct gametankpacket_t {
    int32_t m_type;
    char m_data;
} gametextpacket_t;
#pragma pack(pop)
enum {
    PACKET_STATE = 0,
    PACKET_CALL_FUNCTION,
    PACKET_UPDATE_STATUS,
    PACKET_TILE_CHANGE_REQUEST,
    PACKET_SEND_MAP_DATA,
    PACKET_SEND_TILE_UPDATE_DATA,
    PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE,
    PACKET_TILE_ACTIVATE_REQUEST,
    PACKET_TILE_APPLY_DAMAGE,
    PACKET_SEND_INVENTORY_STATE,
    PACKET_ITEM_ACTIVATE_REQUEST,
    PACKET_ITEM_ACTIVATE_OBJECT_REQUEST,
    PACKET_SEND_TILE_TREE_STATE,
    PACKET_MODIFY_ITEM_INVENTORY,
    PACKET_ITEM_CHANGE_OBJECT,
    PACKET_SEND_LOCK,
    PACKET_SEND_ITEM_DATABASE_DATA,
    PACKET_SEND_PARTICLE_EFFECT,
    PACKET_SET_ICON_STATE,
    PACKET_ITEM_EFFECT,
    PACKET_SET_CHARACTER_STATE,
    PACKET_PING_REPLY,
    PACKET_PING_REQUEST,
    PACKET_GOT_PUNCHED,
    PACKET_APP_CHECK_RESPONSE,
    PACKET_APP_INTEGRITY_FAIL,
    PACKET_DISCONNECT,
    PACKET_BATTLE_JOIN,
    PACKET_BATTLE_EVENT,
    PACKET_USE_DOOR,
    PACKET_SEND_PARENTAL,
    PACKET_GONE_FISHIN,
    PACKET_STEAM,
    PACKET_PET_BATTLE,
    PACKET_NPC,
    PACKET_SPECIAL,
    PACKET_SEND_PARTICLE_EFFECT_V2,
    PACKET_ACTIVE_ARROW_TO_ITEM,
    PACKET_SELECT_TILE_INDEX,
    PACKET_SEND_PLAYER_TRIBUTE_DATA, //39
    PACKET_PVE_UNK1, //no names for these 5, so i named them unk. 
    PACKET_PVE_UNK2,
    PACKET_PVE_UNK3,
    PACKET_PVE_UNK4,
    PACKET_PVE_UNK5,
    PACKET_SET_EXTRA_MODS,           
    PACKET_ON_STEP_ON_TILE_MOD,     
    PACKET_MAXVAL                    //not real, just for string overflow
};
enum {
    NET_MESSAGE_UNKNOWN = 0,
    NET_MESSAGE_SERVER_HELLO,
    NET_MESSAGE_GENERIC_TEXT,
    NET_MESSAGE_GAME_MESSAGE,
    NET_MESSAGE_GAME_PACKET,
    NET_MESSAGE_ERROR,
    NET_MESSAGE_TRACK,
    NET_MESSAGE_CLIENT_LOG_REQUEST,
    NET_MESSAGE_CLIENT_LOG_RESPONSE,
};
enum TileCollisionType {
    TILE_COLLISION_NONE,
    TILE_COLLISION_ALWAYS, // actually unnamed in GT.
    TILE_COLLISION_JUMP_THROUGH,
    TILE_COLLISION_GATEWAY,
    TILE_COLLISION_IF_OFF,
    TILE_COLLISION_ONE_WAY,
    TILE_COLLISION_VIP,
    TILE_COLLISION_JUMP_DOWN,
    TILE_COLLISION_ADVENTURE,
    TILE_COLLISION_IF_ON,
    TILE_COLLISION_FACTION,
    TILE_COLLISION_GUILD,
    TILE_COLLISION_CLOUD,
    TILE_COLLISION_FRIENDS_ENTRANCE
};

#pragma pack(push, 1)
struct GameUpdatePacketSimple {
    uint8_t type;    //0
    uint8_t objtype; //1
    uint8_t count1;
    uint8_t count2;
    int32_t netid;
    int32_t item;
    int32_t flags; //12 - 0xC
    float float1;
    int32_t int_data;
    float vec_x;
    float vec_y;
    float vec2_x;
    float vec2_y;
    float float2;
    int32_t int_x;
    int32_t int_y;
    uint32_t data_size;
};
#pragma pack(pop)