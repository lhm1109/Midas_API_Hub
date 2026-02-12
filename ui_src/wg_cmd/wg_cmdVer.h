#ifndef __WG_CMDVER_H__
#define __WG_CMDVER_H__

#if !defined(_AFXDLL)
	#error MFC Lib를 Shared Lib로 지정해야 합니다 !
#endif

#if defined(_AFXEXT)
#define WGCMD_LIBPATH "../../lib/Build"
#else
#define WGCMD_LIBPATH "../lib/Build"
#endif 


#if defined(_DEBUG)
#if defined(_IGEN)
#define WGCMD_DEBUGORRELEASE "/iGenDebug"
#elif defined(_MGEN)	
#if defined(_MEC)
#define WGCMD_DEBUGORRELEASE "/MecDebug"
#else
#define WGCMD_DEBUGORRELEASE "/Debug"
#endif
#else
#if defined(_MEC)
#define WGCMD_DEBUGORRELEASE "/MecCivilDebug"
#else
#define WGCMD_DEBUGORRELEASE "/CivilDebug"
#endif
#endif
#else
#if defined(_IGEN)
#define WGCMD_DEBUGORRELEASE "/iGenRelease"
#elif defined(_MGEN)
#if defined(_MEC)
#define WGCMD_DEBUGORRELEASE "/MecRelease"
#else
#define WGCMD_DEBUGORRELEASE "/Release"
#endif
#else
#if defined(_MEC)
#define WGCMD_DEBUGORRELEASE "/MecCivilRelease"
#else
#define WGCMD_DEBUGORRELEASE "/CivilRelease"
#endif
#endif
#endif

#if !defined(WGCMD_DEBUGORRELEASE)
	ASSERT(0);
	#if defined(_MEC)
	#define WGCMD_DEBUGORRELEASE "/MecRelease"
	#else
	#define WGCMD_DEBUGORRELEASE "/Release"
	#endif
#endif

#if defined(_EDU)
	#define WGCMD_SUBMDL "_EDU"
#else
	#define WGCMD_SUBMDL ""
#endif

#if defined(_CH)
	#define WGCMD_COUNTRY "_CH"
#elif defined(_US)
	#define WGCMD_COUNTRY "_US"
#elif defined(_JP)
	#define WGCMD_COUNTRY "_JP"
#elif defined(_RUS)
	#define WGCMD_COUNTRY "_RUS"
#else
	#define WGCMD_COUNTRY ""
#endif

#if !defined(_X64)
	#define WGCMD_PLATFORM "/x86"
#else
	#define WGCMD_PLATFORM "/x64"
#endif

#if defined(_DEBUG)
	#define WGCMD_LIBNAME "/wg_cmdd.lib"
#else
	#define WGCMD_LIBNAME "/wg_cmd.lib"
#endif

// WGCMD_LIBPATH WGCMD_DEBUGORRELEASE WGCMD_COUNTRY WGCMD_PLATFORM WGCMD_LIBNAME
// ex) "../../lib/Debug/x86/MIT_lib.lib"
// ex) "../lib/Release_CH/x64/MIT_lib.lib"
// ex) "../../lib/vc90/release/x64/MIT_lib.lib"
//#pragma message( "automatically link with (" WGCMD_LIBPATH WGCMD_DEBUGORRELEASE WGCMD_PLATFORM WGCMD_LIBNAME ")")
#define AUTOLIBNAME   WGCMD_LIBPATH WGCMD_DEBUGORRELEASE WGCMD_SUBMDL WGCMD_COUNTRY WGCMD_PLATFORM WGCMD_LIBNAME

// Perform autolink here:
#pragma message( "automatically link with (" AUTOLIBNAME ")")
#pragma comment(lib, AUTOLIBNAME)
#undef AUTOLIBNAME
#endif // End of __WG_CMDVER_H__
