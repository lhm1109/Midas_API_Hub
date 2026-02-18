// DgnDlgBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnDlgBase.h"

#include "..\wg_base\wg_base_ModelessDlgCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnDlgBase

CDgnDlgBase::CDgnDlgBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CChildDialogMove(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CDgnDlgBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = 0;
}


void CDgnDlgBase::DoDataExchange(CDataExchange* pDX)
{
	CChildDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnDlgBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

#define CDialog CChildDialogMove
BEGIN_MESSAGE_MAP(CDgnDlgBase, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnDlgBase)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
ON_MESSAGE(WM_UPDATE_MODELESS_DLG, OnUpdateDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnDlgBase message handlers

// Create에서 document와 연결
BOOL CDgnDlgBase::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT(m_pDoc != 0);
	BOOL bResult = CChildDialogMove::Create(nIDTemplate, pParentWnd);

	// Document에 등록한다.
	if (bResult)
		m_pDoc->m_pDlgCtrl->Add(nIDTemplate, (CDialog*)this);
	return bResult;
}

// PostNcDestroy에서 document와 연결 해제 및 메모리 해제(self delete)한다.
// 명시적인 delete를 해서는 안된다.
void CDgnDlgBase::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc->m_pDlgCtrl->Remove((CDialog*)this);
	delete this;	
	//CDialog::PostNcDestroy();
}

void CDgnDlgBase::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nID == SC_CLOSE)
	{
		DestroyWindow();
		return;
	}
	CDialog::OnSysCommand(nID, lParam);
}

LRESULT CDgnDlgBase::OnUpdateDlg(WPARAM wParam, LPARAM lParam)
{
	SDlgCtrlUpdateArg* pArg;

	pArg = (SDlgCtrlUpdateArg*)lParam;

	// call virtual function : OnUpdate (as like CView does)
	OnUpdate(pArg->pSender, pArg->lHint, pArg->pHint);
	return 0L;
}

// override this member function
void CDgnDlgBase::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	// not used
	pSender; lHint; pHint;
}
