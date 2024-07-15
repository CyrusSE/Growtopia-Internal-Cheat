#ifndef PlatformSetup_h__
#define PlatformSetup_h__

//#include "PlatformEnums.h"

#ifdef WIN32
//#include "win/PlatformSetupWin.h"
#endif

//RAND_MAX is different on android? whatevs, we'll use our own
#ifdef PLATFORM_ANDROID

#define RT_RAND_MAX (0x7fffffff) // 0x7FFF

#else

#define RT_RAND_MAX RAND_MAX
#endif

#ifdef __APPLE__

#include "TargetConditionals.h"

	#if TARGET_OS_IPHONE == 1
		#include "iOS/PlatformSetupIOS.h"
	#else
		#include "OSX/PlatformSetupOSX.h"
	#endif
#endif
#if defined(RTLINUX) || defined(PLATFORM_LINUX)
#include "linux/PlatformSetupLinux.h"

#endif

#ifdef PLATFORM_HTML5
	#include "html5/PlatformSetupHTML5.h"
#endif

#ifdef RT_WEBOS_ARM
#include "WebOS/PlatformSetupWebOS.h"
#endif

#ifdef ANDROID_NDK
#include "android/PlatformSetupAndroid.h"
#endif

#ifdef PLATFORM_BBX
#include "bbx/PlatformSetupBBX.h"
#endif

#ifdef PLATFORM_FLASH
#include "flash/PlatformSetupFlash.h"
#endif

#if defined(__cplusplus) || defined(__OBJC__)
	#include <cstdio>
	#include <string>
	#include <vector>
	#include <cmath>
	#include <deque>
	#include <cassert>
	#include <map>
	#include <deque>
	#include <stdlib.h>
	#include <iostream>
	#include <sstream>

//hack for making irrBullet compile without needing irr::core prefix on its
//list.  It's because I include this file in the irrlicht main config stuff.
	
#ifdef _CONSOLE
#include <list>
using namespace std;
#endif

const uint16_t C_JPG_HEADER_MARKER = 55551; 

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)      { if(p) { delete (p); (p)=NULL; } }
#endif

#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#ifndef SAFE_FREE
#define SAFE_FREE(p)      { if(p) { free (p); (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) {(p)->Release(); (p)=NULL; } }
#endif

#define MAKE_RGB(r, g, b)      ( ((uint32_t)(r) << 8) + ((uint32_t)(g) << 16) + ((uint32_t)(b) << 24) )
#define MAKE_RGBA(r, g, b, a) ( ((uint32_t)(r) << 8) + ((uint32_t)(g) << 16) + ((uint32_t)(b) << 24) + ((uint32_t)(a)))
const uint32_t PURE_WHITE = MAKE_RGBA(255, 255, 255, 255);

#define GET_BLUE(p)        ( (p)               >> 24)
#define GET_GREEN(p)          (((p) & 0x00FF0000) >> 16)
#define GET_RED(p)        (((p) & 0x0000FF00) >>  8)
#define GET_ALPHA(p)         ( (p) & 0x000000FF       )

#define DEG2RAD(x) (M_PI * (x) / 180.0)
#define RAD2DEG(x) (x * (180/M_PI))

#ifndef UINT_MAX
//fix problem for webOS compiles
#define UINT_MAX      0xffffffff
#endif


#endif

#endif // PlatformSetup_h__
