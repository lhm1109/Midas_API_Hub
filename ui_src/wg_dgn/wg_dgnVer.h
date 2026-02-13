#ifndef __WG_DGNVER_H__
#define __WG_DGNVER_H__

#if !defined(_AFXDLL)
	#error MFC Lib를 Shared Lib로 지정해야 합니다 !
#endif


#if defined(_AFXEXT)
	#define WGDGN_LIBPATH "../../lib/Build"
#else
	#define WGDGN_LIBPATH "../lib/Build"
#endif 

#if defined(_DEBUG)
	#if defined(_IGEN)
		#define WGDGN_DEBUGORRELEASE "/iGenDebug"
	#elif defined(_MGEN)	
		#if defined(_MEC)
			#define WGDGN_DEBUGORRELEASE "/MecDebug"
		#else
			#define WGDGN_DEBUGORRELEASE "/Debug"
		#endif
	#else
		#if defined(_MEC)
			#define WGDGN_DEBUGORRELEASE "/MecCivilDebug"
		#else
			#define WGDGN_DEBUGORRELEASE "/CivilDebug"
		#endif
	#endif
#else
	#if defined(_IGEN)
		#define WGDGN_DEBUGORRELEASE "/iGenRelease"
	#elif defined(_MGEN)
		#if defined(_MEC)
			#define WGDGN_DEBUGORRELEASE "/MecRelease"
		#else
			#define WGDGN_DEBUGORRELEASE "/Release"
		#endif
	#else
		#if defined(_MEC)
			#define WGDGN_DEBUGORRELEASE "/MecCivilRelease"
		#else
			#define WGDGN_DEBUGORRELEASE "/CivilRelease"
		#endif
	#endif
#endif

#define WGDGN_SUBMDL ""
#if defined(_EDU)
	#define WGDGN_SUBMDL "_EDU"
#endif

#define WGDGN_COUNTRY ""
#if defined(_CH)
#define WGDGN_COUNTRY "_CH"
#elif defined(_US)
#define WGDGN_COUNTRY "_US"
#elif defined(_JP)
#define WGDGN_COUNTRY "_JP"
#elif defined(_RUS)
#define WGDGN_COUNTRY "_RUS"
#endif

#if defined(_X64)
	#define WGDGN_PLATFORM "/x64"
#else
	#define WGDGN_PLATFORM "/x86"
#endif

#if defined(_DEBUG)
	#define WGDGN_LIBNAME "/wg_dgnd.lib"
#else
	#define WGDGN_LIBNAME "/wg_dgn.lib"
#endif

// WGDGN_LIBPATH WGDGN_DEBUGORRELEASE WGDGN_COUNTRY WGDGN_PLATFORM WGDGN_LIBNAME
// ex) "../../lib/Debug/x86/MIT_lib.lib"
// ex) "../lib/Release_CH/x64/MIT_lib.lib"
// ex) "../../lib/vc90/release/x64/MIT_lib.lib"
//#pragma message( "automatically link with (" WGDGN_LIBPATH WGDGN_DEBUGORRELEASE WGDGN_PLATFORM WGDGN_LIBNAME ")")
#define AUTOLIBNAME   WGDGN_LIBPATH WGDGN_DEBUGORRELEASE WGDGN_SUBMDL WGDGN_COUNTRY WGDGN_PLATFORM WGDGN_LIBNAME


// Perform autolink here:
#pragma message( "automatically link with (" AUTOLIBNAME ")")
#pragma comment(lib, AUTOLIBNAME)
#undef AUTOLIBNAME
#endif // End of __WG_DGNVER_H__
