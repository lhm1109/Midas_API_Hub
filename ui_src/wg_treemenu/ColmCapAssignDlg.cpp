// ColmCapAssignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ColmCapAssignDlg.h"
#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColmCapAssignDlg dialog

CColmCapAssignDlg::CColmCapAssignDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CColmCapAssignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColmCapAssignDlg)
	m_nAddDel = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CColmCapAssignDlg::~CColmCapAssignDlg()
{

}

void CColmCapAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColmCapAssignDlg)
	DDX_Control(pDX, IDC_TM_COLM_CAP_ASSIGN_NAME_CMB, m_cmbCcpt);
	DDX_Radio(pDX, IDC_TM_COLM_CAP_ASSIGN_ADDREP_RDO, m_nAddDel);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CColmCapAssignDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CColmCapAssignDlg)
	ON_BN_CLICKED(IDC_TM_COLM_CAP_ASSIGN_NAME_BTN, OnTmCcpaButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColmCapAssignDlg message handlers

void CColmCapAssignDlg::Execute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
 
	T_CCPA_D CcpaD;
	CcpaD.Initialize();
	CcpaD.CcptK = m_cmbCcpt.GetItemData(m_cmbCcpt.GetCurSel());

	if(m_nAddDel == 0) m_pDoc->m_pDataCtrl->AddCcpa(aElemK, CcpaD);
	else               m_pDoc->m_pDataCtrl->DelCcpa(aElemK);

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BOOL CColmCapAssignDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitColmCapListCmb();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColmCapAssignDlg::InitColmCapListCmb()
{
	CArray<T_CCPT_K,T_CCPT_K> aCcptK;
	m_pDoc->m_pAttrCtrl2->GetCcptKeyList(aCcptK);
	T_CCPT_D CcptD;

	m_cmbCcpt.ResetContent();
	for(int i=0; i<aCcptK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl2->GetCcpt(aCcptK[i], CcptD);
		m_cmbCcpt.AddString(CcptD.strName);
		m_cmbCcpt.SetItemData(i,aCcptK[i]);
	}
	if(m_cmbCcpt.GetCount() > 0) m_cmbCcpt.SetCurSel(0);
}
///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CColmCapAssignDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CColmCapAssignDlg::UpdateBuffer()
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
		case(UR_CCPT_ADD): 
		case(UR_CCPT_DEL): 
			InitColmCapListCmb();
			break;
		default:
			break;
		}
	} // end of while
}

void CColmCapAssignDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}
void CColmCapAssignDlg::OnTmCcpaButton()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_MAIN_DEFINE_COLM_CAPITAL,0));
}