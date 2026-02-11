// SeisCommonSerdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonSerdDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonSerdDlg dialog

CSeisCommonSerdDlg::CSeisCommonSerdDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCommonSerdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCommonSerdDlg)
	m_nAddDel = 0;
	m_nReinforcement = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CSeisCommonSerdDlg::~CSeisCommonSerdDlg()
{

}

void CSeisCommonSerdDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCommonSerdDlg)
	DDX_Radio(pDX, IDC_TM_ADDREP_RDO, m_nAddDel);
	DDX_Radio(pDX, IDC_TM_RD1_RDO, m_nReinforcement);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CSeisCommonSerdDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSeisCommonSerdDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonSerdDlg message handlers

void CSeisCommonSerdDlg::Execute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);

	T_SERD_D SerdD;
	SerdD.Initialize();
	SerdD.nReinforcement = m_nReinforcement;

	if(m_nAddDel == 0) m_pDoc->m_pDataCtrl->AddSerd(aElemK, SerdD);
	else               m_pDoc->m_pDataCtrl->DelSerd(aElemK);

	if (m_nAddDel == 0)
	{
		IUsageCounter::Use(_T("SPE_SERD"));
	}

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	// ÃÊ±âÈ­
	m_nReinforcement=0;
	UpdateData(FALSE);
}

BOOL CSeisCommonSerdDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CSeisCommonSerdDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CSeisCommonSerdDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SERD_ADD): 
		case(UR_SERD_DEL): 
			break;
		default:
			break;
		}
	} // end of while
}

void CSeisCommonSerdDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}