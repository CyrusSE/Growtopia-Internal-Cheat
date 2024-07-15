#pragma once
#include <core/globals.h>
#include <sdk/player/NetAvatar.h>
#include <PathFinder/PathFinder.h>

// clang-format off


//logging options
namespace logging {
		bool	enabled		= true; //master switch for all logging
	
		//logging in console
		int		console		= callfunction;

		//logging in menu (not yet implemented)
		int		max_count	= 5000;  //max events in menu, for performance concerns.
		int		menu		= sendpacket | sendpacketraw | processtank | callfunction;

		bool packetview = true;
}

//options
namespace opt {

	//enhanchements 
	float		fps_limit	= 144.0f;	//fps limit gets forced to this
	std::string	gt_version	= "4.03";	//gt version gets spoofed to this
	std::string	gt_supported    = "3.61";	//the gt version for which INZERNAL was built for. Probably wont work for other versions.

	bool		android_login = false;
	bool		pc_login = false;
	bool		custom_login = false;
	bool		custom_pass = false;
	bool		spam_text = false;
	bool		spam_text2 = false;
	bool		fast_buy = false;
	bool		fast_buy2 = false;
	bool		custom_guest_login = true;		//whether or not to spoof mac, hash2, wk, rid, etc on login. GT version spoofed even when off.
	bool		custom_rid_login = false;
	bool		custom_mac_login = false;
	bool		custom_flag_login = true;
	bool		fast_drop = false;
	bool		fast_trash = false;
	bool		fast_pull = false;
	bool		fast_ban = false;
	bool		fast_ban_game = false;
	bool		fast_kick = false;
	bool		fast_vend = false;
	bool		fast_vend2 = false;
	bool		fast_vend3 = false;
	bool		fast_mag = false;
	bool		fast_mag2 = false;
	bool		fast_mag3 = false;
	bool		fast_gaia = false;
	bool		fast_gaia2 = false;
	bool		dropitem = false;
	bool		spam_drop = false;
	bool		spam_drop2 = false;
	bool		smokeplayer = false;
	bool		auto_ban = false;
	bool		perma = true;
	bool		ip_perma = false;
	bool		showaction = false;
	bool		skipitem = false;
	bool		autofish = false;
	bool		fastidk = false;
	bool		packetpost = false;
	bool		autofish2 = false;
	bool		autosurg = false;
	bool		labkit = false;
	bool		findfix = false;
	bool		findclam = false;
	bool		foundclam = false;
	bool		foundfix = false;
	bool		ultrasound = false;
	bool		gt_version_spoof = false;
	bool		autosurg2 = false;
	bool		autoenable = false;
	bool		surgery_bug = false;
	bool		spamxp = false;
	bool		info = false;
	bool		rgtmode = false;
	bool		spampacket = false;
	bool		duapacket = false;
	bool		setchat = false;
	bool		sayit = false;
	bool		gg = false;
	bool		autoclear = false;
	bool		variantpacket = false;
	bool		normalpacket = false;
	bool		param2 = false;
	bool		param3 = false;
	bool		param4 = false;
	bool		autosurgreal = false;
	bool		autosurgreal2 = false;
	bool		freezed = false;
	bool		viewdialog = false;
	std::string words = "";
	std::string variantlist = "OnDialogRequest";
	std::string param1text = "";
	std::string param2text = "";
	std::string param3text = "";
	std::string param4text = "";
	std::string playername = "";
	std::string playernetid = "";
	std::string playernetid2 = "";
	std::string thecolor = "0";
	bool		spoof_name	= true;		//Also set the requestedName field to random. Might be unwanted for non grow-id accounts.
	WinSpoof	spoof_win	= ::wsMimic;	//Mode of zf/fz spoofing mode. See the WinSpoof enum for more details.
	FlagMode	flag_mode	= ::fmCustom; //See FlagMode enum for details.
	std::string	flag		= "us";		//set if flag_mode is set to fmCustom

	bool		custom_server_on	= false;	//when on it uses 
	std::string	custom_server_val	= "127.0.0.1"; //localhost works with proxy. custom servers and growtopia2/1 work too.

	//cheats
	namespace cheat {
		std::string spoof_rid_dek = "";
		std::string spoof_mac_dek = "";
		std::string custom_id = "";
		std::string custom_pass = "";
		std::string spam = "test";
		std::string spam2 = "test";
		std::string exploit = "testworld";
		std::string itemid_nya = "2";
		std::string itemid_nya2 = "2";
		std::string vendcount = "0";
		std::string vendcount2 = "0";
		std::string name = "`6@random";
		std::string custom_mac = "hello";
		std::string custom_rid = "hello";
		std::string ggs = "\\";
		std::string lols = "n";
		std::string custom_guest = ggs + lols + "add_smalltext|gg";
		std::string custom_flag = "us";
		std::string ban_text = "This is the text for auto ban";
		std::string reason = "Auto Banned";
		std::string reason2 = "test";
		std::string thepacket = "";
		std::string thepacket2 = "";
		int type = 2;
		int type2 = 2;
		bool    spoof_rid			= false;
		bool    spoof_mac			= false;
		bool	mod_zoom			= false;		//unlimited zoom. Cant ban you. cant punch further, only view.
		bool	dev_zoom			= true;	//	mod zoom with build/punch too. can ban when using punch/build far.
		bool	block_sendpacketraw	= false;	//	blocks any and all sendpacketraw requests. Acts as full ghost.
		bool	antighost			= false;		//	ignores ghost effect.
		bool	tp_click			= true;		//when on, holding CTRL allows you to tp to mouse location.
		bool	see_ghosts			= true;		//allows you to see ghosts so you can avoid constant slime
		bool	dash				= false;	//allows you to perform dashing when double clicking A/D
		bool	jump_charge			= false;	//switches jumping mode to charging
		bool	jump_cancel			= false;	//tapping W in air instantly stops the ascent and instantly starts bringing you down
		bool	antipunch			= true;		//basically (personal) punch jammer even when there is none
		bool	see_fruits			= false;	//See how many fruits a tree will have, before it has grown. Visually sets them to be fully grown though.
		bool	dialog_cheat		= false;	//Allows you to move when dialogs are opened, also punch and do other things.

		bool	punch_cooldown_on	= true;		//	custom punch cooldown
		float	punch_cooldown_val	= 0.15f;
		
		bool	gravity_on			= true;	//	custom gravity
		float	gravity_val			= 400.0f;	

		bool	speed_on			= true;	//	custom movement speed
		float	speed_val			= 450.f;
		
		bool	accel_on			= false;	//	custom acceleration
		float	accel_val			= 3200.0f;

		bool	waterspeed_on		= true;	//	custom water speed
		float	waterspeed_val		= 325.f;

		bool	tree = false;	
		float	tree_size = 10.f;



		bool		playeresp = false;
		bool		itemesp = false;
		bool		autocollect = false;
		bool		filterautocollect = false;
		int			itemfilter = 242;
		int			range_val = 1;
		int			interval = 0.017;

		bool		noname = false;
		bool		autopunchplayer = false;

		bool		autoleavepacket = false;
		bool		bruteforcepassdoor = false;
		bool		itemtp = false;
		bool		autofarm = false;
		bool		blinkcolor = false;
		bool		spamwater = false;
		int			watervalue = 822;
		int			itemid_val = 5640;
		int			itemid_fish = 2914;
		int			blockid = 2;
		//bool			itemid_nya = 2;
		float		interval2 = 0.180f;

		bool crystalmode = false;
		bool spamcolor = false;
		bool randommodepacketstate = false;

		bool superpunchbreak = false;
		bool nazismoke = false;


		bool testmftiles = false;
		bool PathFindinglol = false;
		int gtmap[60][100];
		//std::vector<Pathfinder::Point> pathLine;

		bool ghost = false;
		bool dancemove = false;

		CL_Vec2f TouchPos;
	}
	
}

//other variables
namespace global {
		HWND		hwnd	= NULL;		//	pointer to hwnd of Growtopia window
		App*		app		= nullptr;	//	pointer to App instance
		bool		unload	= false;	//	proces terminates when set to true
		bool		d9init	= false;	//	has s_renderD3D9 yet been init? if not, then check in App::Update for news
		uintptr_t	gt		= 0;		//	growtopia base address
		bool		load	= false;	//	menu/imgui initialization status
		bool		draw	= false;	//	menu draw status. set to true for menu to show up when you inject
		std::string	version	= "V0.6 Beta";	//	INZERNAL version
		float		fade	= 0.f;		//	does not matter, will get automatically updated
		charstate_t state{};			//	state info of player that may be modified so that we can restore it later.
}

// clang-format on

void charstate_t::copy_from_packet(GameUpdatePacket* packet) {
    //int8s
    buildrange = packet->build_range - 128;
    punchrange = packet->punch_range - 128;

    //floats
    gravity = packet->gravity_out;
    speed = packet->speed_out;
    accel = packet->accel;
    punch_strength = packet->punch_strength;
    water_speed = packet->water_speed;

    //ints
    punchid = packet->punchid;
    pupil_color = packet->pupil_color;
    eye_color = packet->eyecolor;
    eye_shade_color = packet->eye_shade_color;
    eff_flags1 = packet->effect_flags;
    eff_flags2 = packet->flags;
    if (opt::cheat::antighost) {//remove mindcontrol mod
        packet->flags &= ~(1 << 11);
        eff_flags2 &= ~(1 << 11); 
    }
}

void charstate_t::copy_inject(NetAvatar* player, bool is_local) {
    gravity = player->gravity.get(is_local);
    speed = player->speed.get(is_local);
    accel = player->accel.get(is_local);
    punch_strength = player->punch_strength.get(is_local);
    water_speed = player->water_speed;

    //cant copy more than this because we dont know all the values in NetAvatar ATM
}
