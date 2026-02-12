#ifndef __WG_CMD_PUSHOVEREC8_H__
#define __WG_CMD_PUSHOVEREC8_H__

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

#include "wg_cmdRes2.h"
#include "MyChildDialog.h"
#include "ETC_GDS_EURO1996.h"
#include "ETC_GDS_EURO1996_ELA.h"
#include "ETC_GDS_EURO2003.h"
#include "ETC_GDS_USER.h"



#ifdef _AFXEXT
#undef  __INTER_DLL__
#endif

#endif 
