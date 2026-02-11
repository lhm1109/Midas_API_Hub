#ifndef __WG_CMDALL_H__
#define __WG_CMDALL_H__

#ifdef _AFXEXT
#define  __INTER_DLL__  // DLL 에서 다른 DLL의 Member에 대한 참조가 발생할 때 
#endif

// 현재 DLL 에서 다른 DLL을 참조 할때 아래 예와 같이 참조하는  DLL의  
// Interface Header File을 Include 한다.  "WGen DLL 템플릿 생성"  App Wizard에서 
// 생성된 DLL의 Interface Header File은 suffix "All"을 Header File 이름에 갖는다. 

//#if !defined(__WG_BASEALL_H__)
//	#include "..\wg_base\wg_baseall.h"
//#endif

#if !defined(__WG_CMDVER_H__)
	#include "wg_cmdver.h"
#endif

#if !defined(__WG_CMD_H__)
	#include "wg_cmd.h"
#endif

// wg_cmd.dll에 추가되는 class 에 대한 header file을 추가한다. 
//제시된 예는 추가된 Class Header를 Include 하는 모습이다. 
//Export가 필요없는 Class 또는 Function은 Include할 필요 없다. 
//자세한 내용은 AddClass.txt를 참고 할것 
//#if !defined(__ADDEDCLASS_H__)
//#include "AddedClass.h"
//#endif

#if !defined(__EXPORTFUNC_H__)
#include "ExportFunc.h"
#endif

#if !defined(__CMPROPERTIESDLG_H__)
#include "CMPropertiesDlg.h"
#endif

#if !defined(__CMLOADCASEDLG_H__)
#include "CMLoadCaseDlg.h"
#endif

#if !defined(__CMLOADCOMBDLG_H__)
#include "CMLoadCombDlg.h"
#endif

#if !defined(__CMLOADCOMBDEFDLG_H__)
#include "CMLoadCombDefDlg.h"
#endif

#if !defined(__CMDLGBASE_H__)
#include "CMDlgBase.h"
#endif

#if !defined(__CMSELECTDLG_H__)
#include "CMSelectDlg.h"
#endif

#if !defined(__MACTIVEIDENTITYDLG_H__)
#include "MActiveIdentityDlg.h"
#endif

#if !defined(__MLOADPNLDDLG_H__)
#include "MLoadPNLDDlg.h"
#endif

#ifdef _AFXEXT
#undef  __INTER_DLL__
#endif

#endif 
