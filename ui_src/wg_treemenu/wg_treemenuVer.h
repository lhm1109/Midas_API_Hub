#ifndef __WG_TREEMENUVER_H__
#define __WG_TREEMENUVER_H__

#if !defined(_AFXDLL)
	#error MFC Lib를 Shared Lib로 지정해야 합니다 !
#endif

#define WGTREEMENU_LIBPATH "../lib/Build"
#if defined(_AFXEXT)
#define WGTREEMENU_LIBPATH "../../lib/Build"
#endif 

#if defined(_MEC)
#define WGTREEMENU_DEBUGORRELEASE "/MecRelease"
#else
#define WGTREEMENU_DEBUGORRELEASE "/Release"
#endif
#if defined(_DEBUG)
#if defined(_IGEN)
#define WGTREEMENU_DEBUGORRELEASE "/iGenDebug"
#elif defined(_MGEN)	
#if defined(_MEC)
#define WGTREEMENU_DEBUGORRELEASE "/MecDebug"
#else
#define WGTREEMENU_DEBUGORRELEASE "/Debug"
#endif
#else
#if defined(_MEC)
#define WGTREEMENU_DEBUGORRELEASE "/MecCivilDebug"
#else
#define WGTREEMENU_DEBUGORRELEASE "/CivilDebug"
#endif
#endif
#else
#if defined(_IGEN)
#define WGTREEMENU_DEBUGORRELEASE "/iGenRelease"
#elif defined(_MGEN)
#if defined(_MEC)
#define WGTREEMENU_DEBUGORRELEASE "/MecRelease"
#else
#define WGTREEMENU_DEBUGORRELEASE "/Release"
#endif
#else
#if defined(_MEC)
#define WGTREEMENU_DEBUGORRELEASE "/MecCivilRelease"
#else
#define WGTREEMENU_DEBUGORRELEASE "/CivilRelease"
#endif
#endif
#endif

#define WGTREEMENU_SUBMDL ""
#if defined(_EDU)
	#define WGTREEMENU_SUBMDL "_EDU"
#endif

#define WGTREEMENU_COUNTRY ""
#if defined(_CH)
#define WGTREEMENU_COUNTRY "_CH"
#elif defined(_US)
#define WGTREEMENU_COUNTRY "_US"
#elif defined(_JP)
#define WGTREEMENU_COUNTRY "_JP"
#elif defined(_RUS)
#define WGTREEMENU_COUNTRY "_RUS"
#endif

#define WGTREEMENU_PLATFORM "/x64"
#if !defined(_X64)
#define WGTREEMENU_PLATFORM "/x86"
#endif

#define WGTREEMENU_LIBNAME "/wg_treemenu.lib"
#if defined(_DEBUG)
#define WGTREEMENU_LIBNAME "/wg_treemenud.lib"
#endif

// WGTREEMENU_LIBPATH WGTREEMENU_DEBUGORRELEASE WGTREEMENU_COUNTRY WGTREEMENU_PLATFORM WGTREEMENU_LIBNAME
// ex) "../../lib/Debug/x86/MIT_lib.lib"
// ex) "../lib/Release_CH/x64/MIT_lib.lib"
// ex) "../../lib/vc90/release/x64/MIT_lib.lib"
//#pragma message( "automatically link with (" WGTREEMENU_LIBPATH WGTREEMENU_DEBUGORRELEASE WGTREEMENU_PLATFORM WGTREEMENU_LIBNAME ")")
#define AUTOLIBNAME   WGTREEMENU_LIBPATH WGTREEMENU_DEBUGORRELEASE WGTREEMENU_SUBMDL WGTREEMENU_COUNTRY WGTREEMENU_PLATFORM WGTREEMENU_LIBNAME

// Perform autolink here:
#pragma message( "automatically link with (" AUTOLIBNAME ")")
#pragma comment(lib, AUTOLIBNAME)
#undef AUTOLIBNAME
#endif // End of __WG_TREEMENUVER_H__
