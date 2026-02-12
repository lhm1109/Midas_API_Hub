// CMDlgBarBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMChildBarBase.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMChildBarBase
//IMPLEMENT_DYNCREATE(CCMChildBarBase, CMenuBarChildDlg)

CCMChildBarBase::CCMChildBarBase()
	: CMenuBarChildDlg((UINT)0)
{

}

CCMChildBarBase::CCMChildBarBase(UINT nID)
	: CMenuBarChildDlg(nID)
{
	//{{AFX_DATA_INIT(CCMChildBarBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMChildBarBase::~CCMChildBarBase()
{
}

BOOL CCMChildBarBase::GetSelectedNodeKeyList(CArray<UINT, UINT> &raNodeKeyList)
{
	BOOL bResult = GetIGenModelBase()->GetSelectedNodeKeyList(raNodeKeyList);
	if (!bResult) return bResult;
	qsort((void*)raNodeKeyList.GetData(), raNodeKeyList.GetSize(), sizeof(UINT), 
		CCompFunc::UINTAsc);
	return bResult;
}

BOOL CCMChildBarBase::GetSelectedElemKeyList(CArray<UINT, UINT> &raElemKeyList)
{
	BOOL bResult = GetIGenModelBase()->GetSelectedElemKeyList(raElemKeyList);
	if (!bResult) return bResult;
	qsort((void*)raElemKeyList.GetData(), raElemKeyList.GetSize(), sizeof(UINT), 
		CCompFunc::UINTAsc);
	return bResult;
}

I_GENModelBase* CCMChildBarBase::GetIGenModelBase()
{
	CWnd* pWnd = GetParent();   // get tab window
	VERIFY(pWnd != 0 && pWnd->GetSafeHwnd() != 0);

	CTreeMenuBarBase* pTreeMenuBarBase = (CTreeMenuBarBase*)pWnd->GetParent();
	VERIFY(pTreeMenuBarBase != 0 && pTreeMenuBarBase->GetSafeHwnd() != 0);

	I_GENModelBase* pIGenModelBase =  pTreeMenuBarBase->GetI_GENModelST();
	VERIFY(pIGenModelBase != 0);
	return pIGenModelBase;
}

