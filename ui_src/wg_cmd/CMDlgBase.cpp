// CMDlgBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDlgBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_ModelessDlgCtrl.h"

#include "..\mit_frx\global_Opt4Perf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDlgBase dialog
#define CDialog CModelessMoveDlg


CCMDlgBase::CCMDlgBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CModelessMoveDlg(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CCMDlgBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = 0;
}

CCMDlgBase::~CCMDlgBase()
{

}


void CCMDlgBase::DoDataExchange(CDataExchange* pDX)
{
	CModelessMoveDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDlgBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMDlgBase, CDialog)
	//{{AFX_MSG_MAP(CCMDlgBase)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UPDATE_MODELESS_DLG, OnUpdateDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDlgBase message handlers

// Create에서 document와 연결
BOOL CCMDlgBase::Create(UINT nIDTemplate, CWnd* pParentWnd, int nOption/* = 0*/)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pDoc == NULL)
	{
		m_pDoc = CDBDoc::GetDocPoint();
		ASSERT(m_pDoc);
	}

	BOOL bResult = CModelessMoveDlg::Create(nIDTemplate, pParentWnd);

	// Document에 등록한다.
	if (bResult)
		m_pDoc->m_pDlgCtrl->Add(nIDTemplate, (CDialog*)this, nOption);
	return bResult;
}

// PostNcDestroy에서 document와 연결 해제 및 메모리 해제(self delete)한다.
// 명시적인 delete를 해서는 안된다.
void CCMDlgBase::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc->m_pDlgCtrl->Remove((CDialog*)this);
	delete this;	
	//CDialog::PostNcDestroy();
}


void CCMDlgBase::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nID == SC_CLOSE)
	{
		DestroyWindow();
		return;
	}
	CDialog::OnSysCommand(nID, lParam);
}

LRESULT CCMDlgBase::OnUpdateDlg(WPARAM wParam, LPARAM lParam)
{
	SDlgCtrlUpdateArg* pArg;

	pArg = (SDlgCtrlUpdateArg*)lParam;

	// call virtual function : OnUpdate (as like CView does)
	OnUpdate(pArg->pSender, pArg->lHint, pArg->pHint);
	return 0L;
}

// override this member function
void CCMDlgBase::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	// not used
	pSender; lHint; pHint;
}

BOOL CCMDlgBase::PreTranslateMessage(MSG* pMsg) 
{
	if(false == global_Opt4Perf::getInstance().m_skipUpdateDialogControlsInCCMDlgBase)
		UpdateDialogControls(this, false);
	
	return CModelessMoveDlg::PreTranslateMessage(pMsg);
}
