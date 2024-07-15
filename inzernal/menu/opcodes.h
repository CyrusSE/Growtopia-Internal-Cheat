#pragma once
#include <iostream>
#include <Windows.h>

/*

Growtopia V3.81

*/


uintptr_t baseAdd;

const char je[] = "\x74";
const char jmp[] = "\xEB";

const char bAntiBounce[] = "\x0F\x84\x03\x01\x00\x00";			// to "\xE9\x04\x01\x00\x00\x90"
//const char bAntiCheckpoint[] = "\x74";							// to jmp
//const char bAntiGlue[] = "\x74";								// to jmp
//const char bAntiWater[] = "\x74";								// to jmp
//const char bAntiRespawn[] = "\x4F\x6E";						// nop
//const char bGhost[] = "\x74";									// to jmp
const char bGrowz[] = "\xF3\x0F\x5C\xD1";						// nop
const char bModFly[] = "\x74\x5D";								// nop
const char bNoClip[] = "\x75\x0B";								// nop

uintptr_t offset_banbypass = 0x3ECBB4;
uintptr_t offset_antibounce = 0x54DCCA;
uintptr_t offset_anticheckpoint = 0x4F100C;
uintptr_t offset_antiglue = 0x4ED7A5;
uintptr_t offset_antirespawn = 0x93CC40;
uintptr_t offset_antiwater = 0x4ED702;
uintptr_t offset_ghost = 0x4ECE62;
uintptr_t offset_growz = 0x4ED6F1;
uintptr_t offset_modfly = 0x54CCB7;
uintptr_t offset_noclip = 0x53CDA5;

// uintptr_t offset_mod_noclip = 0x4EE22B;
// const char bModNoClip[] = "\xF6\x87\x88\x01\x00\x00\x01"; // to "\xC6\x87\x88\x01\x00\x00\x01"