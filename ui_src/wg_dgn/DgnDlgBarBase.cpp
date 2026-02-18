// DgnDlgBarBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnDlgBarBase.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDlgBarBase
IMPLEMENT_DYNCREATE(CDgnDlgBarBase, CFormViewInBar)

CDgnDlgBarBase::CDgnDlgBarBase()
	: CFormViewInBar((UINT)0)
{
}

CDgnDlgBarBase::CDgnDlgBarBase(UINT nID)
	: CFormViewInBar(nID)
{
	//{{AFX_DATA_INIT(CCMDlgBarBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDgnDlgBarBase::~CDgnDlgBarBase()
{
}

BOOL CDgnDlgBarBase::GetSelectedNodeKeyList(CArray<UINT, UINT> &raNodeKeyList)
{
	BOOL bResult = GetIGenModelBase()->GetSelectedNodeKeyList(raNodeKeyList);
	if (!bResult) return bResult;
	qsort((void*)raNodeKeyList.GetData(), raNodeKeyList.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

	return bResult;
}

BOOL CDgnDlgBarBase::GetSelectedElemKeyList(CArray<UINT, UINT> &raElemKeyList)
{
	BOOL bResult = GetIGenModelBase()->GetSelectedElemKeyList(raElemKeyList);
	if (!bResult) return bResult;
	qsort((void*)raElemKeyList.GetData(), raElemKeyList.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

	return bResult;
}

I_GENModelBase* CDgnDlgBarBase::GetIGenModelBase()
{
	CWnd* pWnd = GetParent();   // get tab window
	VERIFY(pWnd != 0 && pWnd->GetSafeHwnd() != 0);

	CTreeMenuBarBase* pTreeMenuBarBase = (CTreeMenuBarBase*)pWnd->GetParent();
	VERIFY(pTreeMenuBarBase != 0 && pTreeMenuBarBase->GetSafeHwnd() != 0);

	I_GENModelBase* pIGenModelBase =  pTreeMenuBarBase->GetI_GENModelST();
	VERIFY(pIGenModelBase != 0);

	return pIGenModelBase;
}
