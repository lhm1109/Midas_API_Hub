// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#ifdef __STDAFX_H__
	#error wg_main.h를 Include 해야 합니다. 
	#error stdafx.h File이 두번 Include 되어 있습니다. 
#endif

#if !defined (__STDAFX_H__)
#define       __STDAFX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS
#define __STDC_LIMIT_MACROS
#include <SDKDDKVer.h>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		    // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "ProjInfo.h"

// GDI+ for GdiplusDrawerStatic
#pragma comment(lib, "gdiplus.lib")

//--- 컴파일 속도 개선(Pre-Compile Header 사용) -------------------
#include "..\wg_db\DB_ST_DT.h"
//-----------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__STDAFX_H__)
