// DropPanelAssignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "DropPanelAssignDlg.h"
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
// CDropPanelAssignDlg dialog

CDropPanelAssignDlg::CDropPanelAssignDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDropPanelAssignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDropPanelAssignDlg)
	m_nAddDel = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CDropPanelAssignDlg::~CDropPanelAssignDlg()
{

}

void CDropPanelAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDropPanelAssignDlg)
 	DDX_Control(pDX, IDC_TM_DROP_PANEL_ASSIGN_NAME_CMB, m_cmbDpal);
	DDX_Radio(pDX, IDC_TM_DROP_PANEL_ASSIGN_ADDREP_RDO, m_nAddDel);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CDropPanelAssignDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CDropPanelAssignDlg)
	ON_BN_CLICKED(IDC_TM_DROP_PANEL_ASSIGN_NAME_BTN, OnTmDpaaButton)  
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropPanelAssignDlg message handlers

void CDropPanelAssignDlg::Execute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
	
	T_DPAA_D DpaaD;
	DpaaD.Initialize();
	DpaaD.DpanK = m_cmbDpal.GetItemData(m_cmbDpal.GetCurSel());
		
	if(m_nAddDel == 0) m_pDoc->m_pDataCtrl->AddDpaa(aElemK, DpaaD);
	else               m_pDoc->m_pDataCtrl->DelDpaa(aElemK);

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BOOL CDropPanelAssignDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitDropPanelListCmb();


	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CDropPanelAssignDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDropPanelAssignDlg::InitDropPanelListCmb()
{
	CArray<T_DPAN_K,T_DPAN_K> aDPanK;
	m_pDoc->m_pAttrCtrl2->GetDpanKeyList(aDPanK);
	T_DPAN_D DpanD;

	m_cmbDpal.ResetContent();
	for(int i=0; i<aDPanK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl2->GetDpan(aDPanK[i], DpanD);
		m_cmbDpal.AddString(DpanD.strName);
		m_cmbDpal.SetItemData(i,aDPanK[i]);
	}

	if(m_cmbDpal.GetCount() > 0) m_cmbDpal.SetCurSel(0);
}

void CDropPanelAssignDlg::UpdateBuffer()
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
		case(UR_DPAN_ADD): 
		case(UR_DPAN_DEL): 
			InitDropPanelListCmb();
			break;
		default:
			break;
		}
	} // end of while
}

void CDropPanelAssignDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}
void CDropPanelAssignDlg::OnTmDpaaButton()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_MAIN_DEFINE_DROP_PANEL,0));
}