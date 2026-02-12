#ifndef __WG_CMD_CMSECTVIEWWND_H___
#define __WG_CMD_CMSECTVIEWWND_H___

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

#include "CMSecViewWnd.h"

#ifdef _AFXEXT
#undef  __INTER_DLL__
#endif

#endif 
