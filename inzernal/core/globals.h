#pragma once
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "shlwapi.lib")
#include <d3d9.h>
#include <enet/include/enet.h>
#include <proton/NetHTTP.h>
#include <proton/Variant.h>
#include <proton/clanlib/vec2.h>
#include <stdio.h>
#include <atomic>
#include <core/json.hpp>
#include <map>
#include <string>
#include <chrono>


// clang-format off

//TODO: move this
struct TextureD3D9 {
    union {
        IDirect3DBaseTexture9* m_ptr;
        IDirect3DTexture9* m_texture2d;
        IDirect3DVolumeTexture9* m_texture3d;
        IDirect3DCubeTexture9* m_textureCube;
    };

    IDirect3DSurface9* m_surface;

    union {
        IDirect3DBaseTexture9* m_staging;
        IDirect3DTexture9* m_staging2d;
        IDirect3DVolumeTexture9* m_staging3d;
        IDirect3DCubeTexture9* m_stagingCube;
    };

    uint64_t m_flags;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_depth;
    uint8_t m_numMips;
    uint8_t m_type;
    uint8_t m_requestedFormat;
    uint8_t m_textureFormat;
};


class BaseApp {public:};
class App : BaseApp {public:};
class variantlist_t;
struct GameUpdatePacket;
struct AvatarRenderData;
struct WorldObjectMap;
struct NetAvatar;
struct LevelTouchComponent;
struct WorldCamera;
struct GameLogic;
struct SurfaceAnim;
struct ResourceManager;

namespace types {
	//hooked
	using	SendPacketRaw				= void(__cdecl*)(int, GameUpdatePacket*, int, void*, ENetPeer*, int);
	using	SendPacket					= void(__cdecl*)(int, std::string&, ENetPeer*);
	using	ProcessTankUpdatePacket		= void(__cdecl*)(GameLogic*, GameUpdatePacket*);

	//other functions, not hooked
	using	WorldToScreen				= void(__cdecl*)(WorldCamera*, CL_Vec2f&, CL_Vec2f);
	using	SetCharacterExtraMods		= void(__cdecl*)(NetAvatar*, uint8_t*);
	using	GetSurfaceAnim				= SurfaceAnim*(__cdecl*)(ResourceManager*, const std::string&, bool);

	typedef	std::chrono::duration<double> elapsed;
	typedef std::chrono::system_clock::time_point time;
}

enum WinSpoof {
	wsOff = 0,		//will not try to spoof anything
	wsRandom = 1,	//will set random values to zf and fz as usual
	wsMimic = 2		//will set unmodified newest growtopia.exe values to avoid fake win-ver tracking. needs manual updating
};
enum FlagMode {
	fmKeep = 0,		//keep your flag
	fmRandom = 1,	//pick random flag from gt folder
	fmCustom = 2,	//use custom flag
};

namespace logging {

		enum { 
			 sendpacket = (1 << 0), //the text of the packet
			 sendpacketraw = (1 << 1), //just the type, not the full info
			 processtank = (1 << 2), //just the type, not the full info
			 callfunction = (1 << 3), //processtankupdatepacket CALL_FUNCTION serializes as variantlist
			 logmsg	= (1 << 4), //LogMsg function
			 callfuncspr = (1 << 5) //sendpacketraw CALL_FUNCTION serializes as variantlist
		 };

		extern bool	enabled;
		extern int console;
		extern int max_count;
		extern int menu;	
		extern bool packetview;
}

namespace opt {
	
	extern std::string	gt_version;
	extern std::string	gt_supported;
	extern std::string	spoof_rid_dek;
	extern float		fps_limit;

	extern bool			custom_guest_login;
	extern bool			android_login;
	extern bool			pc_login;
	extern bool			custom_login;
	extern bool			custom_pass;
	extern bool			spam_text;
	extern bool			spam_text2;
	extern bool			fast_buy;
	extern bool			fast_buy2;
	extern bool			fast_drop;
	extern bool			fast_drop2;
	extern bool			fast_trash;
	extern bool			fast_pull;
	extern bool			fast_ban;
	extern bool			fast_ban_game;
	extern bool			fast_vend;
	extern bool			fast_vend2;
	extern bool			fast_vend3;
	extern bool			fast_mag;
	extern bool			fast_mag2;
	extern bool			fast_mag3;
	extern bool			fast_gaia;
	extern bool			fast_gaia2;
	extern bool			dropitem;
	extern bool			fast_kick;
	extern bool			spam_drop;
	extern bool			spam_drop2;
	extern bool			interval;
	extern bool			smokeplayer;
	extern bool			auto_ban;
	extern bool			perma;
	extern bool			ip_perma;
	extern bool			custom_mac_login;
	extern bool			custom_rid_login;
	extern bool			custom_flag_login;
	extern bool			spoof_name;
	extern bool			showaction;
	extern bool			skipitem;
	extern bool			autofish;
	extern bool			autofish2;
	extern bool			autosurg;
	extern bool			labkit;
	extern bool			findfix;
	extern bool			foundfix;
	extern bool			findclam;
	extern bool			foundclam;
	extern bool			ultrasound;
	extern bool			gt_version_spoof;
	extern bool			autosurg2;
	extern bool			autoenable;
	extern bool			surgery_bug;
	extern bool			spamxp;
	extern bool			info;
	extern bool			rgtmode;
	extern bool			spampacket;
	extern bool			fastidk;
	extern bool			packetpost;
	extern bool			duapacket;
	extern bool			setchat;
	extern bool			sayit;
	extern bool			gg;
	extern bool			variantpacket;
	extern bool			autoclear;
	extern bool			normalpacket;
	extern bool			param2;
	extern bool			param3;
	extern bool			param4;
	extern bool			autosurgreal;
	extern bool			autosurgreal2;
	extern bool			freezed;
	extern bool			viewdialog;
	extern string			words;
	extern string			variantlist;
	extern string			param1text;
	extern string			param2text;
	extern string			param3text;
	extern string			param4text;
	extern string			playername;
	extern string			playernetid;
	extern string			playernetid2;
	extern WinSpoof		spoof_win;
	extern FlagMode		flag_mode;
	extern std::string	flag;
	extern string thecolor;

	extern bool			custom_server_on;
	extern std::string	custom_server_val;

	namespace cheat {
		extern bool		dev_zoom;
		extern bool		block_sendpacketraw;
		extern bool		antighost;
		extern bool		tp_click;
		extern bool		mod_zoom;
		extern bool		see_ghosts;
		extern bool		dash;
		extern bool		jump_charge;
		extern bool		jump_cancel;
		extern bool		antipunch;
		extern bool		see_fruits;
		extern bool		dialog_cheat;

		extern bool		punch_cooldown_on;
		extern float	punch_cooldown_val;

		extern bool		gravity_on;
		extern float	gravity_val;

		extern bool		speed_on;
		extern float	speed_val;

		extern bool		accel_on;
		extern float	accel_val;

		extern bool		waterspeed_on;
		extern float	waterspeed_val;

		extern bool		tree;
		extern float	tree_size;

		extern bool		playeresp;
		extern bool		itemesp;
		extern bool		autocollect;
		extern bool		filterautocollect;
		extern int	itemfilter;
		extern int	range_val;
		extern int	interval;
		//extern int	interval2;
		extern float		interval2;
		extern bool bruteforcepassdoor;
		extern bool autofarm;
		extern bool itemtp;
		extern bool noname;
		extern int	itemid_val;
		extern int	itemid_fish;
		extern int	blockid;
		extern string itemid_nya;
		extern string itemid_nya2;
		extern string vendcount;
		extern string vendcount2;
		extern string name;
		extern string spoof_rid_dek;
		extern string spoof_mac_dek;
		extern string custom_id;
		extern string custom_pass;
		extern string spam;
		extern string spam2;
		extern string exploit;
		extern string custom_mac;
		extern string custom_rid;
		extern string custom_guest;
		extern string custom_flag;
		extern string ban_text;
		extern string reason;
		extern string reason2;
		extern string thepacket;
		extern string thepacket2;
		extern int type;
		extern int type2;
		extern bool	spamwater;
		extern int	watervalue;
		extern bool autopunchplayer;
		extern bool blinkcolor;

		extern bool crystalmode;
		extern bool spamcolor;
		extern bool randommodepacketstate;

		extern bool testmftiles;
		extern bool PathFindinglol;
		extern int gtmap[60][100];

		extern bool ghost;
		extern bool dancemove;
		extern bool nazismoke;

		extern bool superpunchbreak;

		extern CL_Vec2f TouchPos;
	}
	
}

struct charstate_t {
    int punchid = 0;
    int8_t buildrange = 0;
    int8_t punchrange = 0;
    int pupil_color;
    int eye_color;
    int eye_shade_color;
    int eff_flags1 = 0;
    int eff_flags2 = 0;
    float gravity = 1000.0f; //just in case we inject mid world, change gravity, but never receive 
    float speed = 250.f;
    float accel = 1200.f;
    float punch_strength = 300.f;
    float water_speed  = 125.f;

    void copy_from_packet(GameUpdatePacket* packet);

	//yeah in the 0.1% chance that we inject mid world and quickly change gravity etc
	//without ever receiving set state packet, then uninjecting, getting ping reply, and that would ban without copying info on inject.
	void copy_inject(NetAvatar* player, bool is_local); 
};
namespace global {
	extern App*			app;
	extern bool			unload;
	extern bool			d9init;
	extern HWND			hwnd;
	extern uintptr_t	gt;
	extern bool			load;
	extern bool			draw;
	extern std::string	version;
	extern float		fade;
	extern charstate_t	state;
	//	extern nlohmann::json m_cfg;
}

// clang-format on

