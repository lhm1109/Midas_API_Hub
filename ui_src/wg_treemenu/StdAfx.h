// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#ifdef __STDAFX_H__
	#error wg_treemenu.h를 Include 해야 합니다. 
	#error stdafx.h File이 두번 Include 되어 있습니다. 
#endif

#if !defined __STDAFX_H__
#define __STDAFX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#define __STDC_LIMIT_MACROS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <SDKDDKVer.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>     // MFC OLE dialog classes
#include <afxdisp.h>       // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "FormViewInBar.h"

#include "gsxbutton.h"

#include "ProjInfo.h"

//--- 컴파일 속도 개선(Pre-Compile Header 사용) -------------------
#include "..\wg_db\DB_ST_DT.h"
#include <afxcontrolbars.h>
//-----------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif// !defined __AFX_STDAFX_H__


