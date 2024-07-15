#pragma once
#include <proton/variant2.hpp>
#include <core/utils.h>

struct TileExtra;

#pragma pack(push, 1)
GTClass Tile{
   public:
    int unk;
    __int16 foreground;
    __int16 background;
    __int16 flags;
    __int8 position_x;
    __int8 position_y;
    short world_pos; //basically if pos is 8, 17 then this value is 817
    short texture_pos;
    short texture_pos_bg;
    char pad0[6];
    rect_t collision_rect; //see: Tile::BuildCache and Tile::DoesWorldRectCollide
    TileExtra* tile_extra;
    char health_or_damage;
    __int16 unk6;
    int last_tick_punched;
    int unk7;
    int some_tick_2;
    float opacity_maybe;
    char pad[6];
    __int16 backup_flags_maybe;
    char pad1[85]; //to get to 128 as size (see WorldTileMap::Clear) was 69

    //NOTE: just pushed commit for now, dont use any of these funcs or variables since rn they are outdated and brokken
    
  /*  void set_tile_foreground(short id) {
        static auto address = detail::get_call<settileforeground_t>("cd e8 ? ? ? ? 41 bf 0f", 1);
        address(this, id);
    }

    void set_text(textmanager_t * manager, const std::string& str, unsigned int background, unsigned int border, CL_Vec2f pos) {
        auto text = manager->add_text(this, str);
        text->position = pos;
        text->set_color(background, border);
    }
    void toggle_flag(short flag) {
        static auto func = flag_generic_t(utils::find_func_start("66 31 51 08"));
        func(this, flag);
    }
    void enable_flag(short flag) {
        static auto func = flag_generic_t(utils::find_func_start("66 09 51 08"));
        func(this, flag);
    }
    void remove_flag(short flag) {
        static auto func = flag_generic_t(utils::find_func_start("66 21 51 08"));
        func(this, flag);
    }
    bool is_collideable(unsigned int uid, world_t* world) {
        static auto func = is_collideable_t(utils::find_func_start("02 74 ? 83 f8 07 74 ? 8b"));
        if (!world || !this)
            return true;
        return func(this, uid, world, true);
    }*/
    CL_Vec2f GetPosAtWorld() {
        return CL_Vec2f{ (float)this->position_x * 32.f, (float)this->position_y * 32.f };
    }
    CL_Vec2i GetPos() {
        return CL_Vec2i{ this->position_x, this->position_y };
    }
};
#pragma pack(pop)

//extremely important because we have raw std::vector in tilemap for tiles.
static_assert(sizeof(Tile) == 160, "wrong tile size!");
constexpr auto tilelen = sizeof(Tile);

#pragma pack(push, 1)
GTClass tamagochi_t {
    uint32_t food;
    uint32_t water;
    uint32_t unk0;
    uint32_t sick;
};
#pragma pack(pop)

#pragma pack(push, 1)
GTClass TileExtra {
   public:
    void* vftable;
    uint8_t type;
    uint8_t some_flag;
    uint8_t pad0[2];
    int32_t owner; //for heart monitor, lock etc
    std::string unk1;
    std::string label;
    std::string unk3;
    uint32_t last_updated;
    uint32_t growth;     //gets incremented every second for trees, silkworms etc
    uint8_t fruit_count; //1 for heart monitor, 2 for door, fruit_count for seeds
    char pad1[3];
    uint32_t unk5;    //definitely some time too
    uint8_t alt_type; //has values like 128, 256 but in the windows bin it also gets set to 0 for TeamEntrance1, 1 for TeamEntrance2 etc
    char pad2[23];
    std::vector<uint32_t> access_list;
    void* avatar_data;
    char unk7a[8];
    tamagochi_t* tamagochi; //silkworm shit
    char unk7b[16];
    std::vector<void* /*CommandData::CommandData -> mac bin*/> command_data;
    uint32_t unk8;
    uint32_t unk9;
    uint32_t unk10;
    uint32_t unk11;
    uint32_t unk12;
    uint32_t unk13;
    uint32_t unk14;
    uint32_t unk15;
    uint32_t unk16;
    uint32_t unk17;
    uint32_t unk18;
    uint32_t unk19;
    uint32_t unk20;
    uint32_t unk21;
    uint32_t unk22;
    uint32_t unk23;
    uint32_t unk24;
    uint32_t unk25;
    uint32_t unk26;
    uint32_t unk27;
    uint32_t unk28;
    uint32_t unk29;
    uint32_t unk30;
    uint32_t unk31;
};
#pragma pack(pop)