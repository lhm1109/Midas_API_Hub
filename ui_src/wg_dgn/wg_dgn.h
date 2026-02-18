#ifndef  __WG_DGN_H__
#define  __WG_DGN_H__

// wg_dgn.dll ÀÇ Resource Symbol File header
#include "wg_dgnRes2.h"

/////////////////////////////////
#include "HeaderPre.h"	///////////
/////////////////////////////////

class CDBDoc;
extern "C" __MY_EXT_API__ void WINAPI Init_wg_dgndll();
extern __declspec(dllexport) void* DgnCreateOrActivateDlg(CDBDoc* pDoc,UINT nIDTemplate,CWnd* pParent=NULL);

/////////////////////////////////
#include "HeaderPost.h"	///////////
/////////////////////////////////
#endif
