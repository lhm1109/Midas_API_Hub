// DgnStlCbDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnAluElemParaDlg.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_main\wg_mainres2.h"	
#include "..\wg_db\AluDgnTool.h"
#include "..\wg_common\TBUtilFunc.h"
#include "..\wg_db\DB_ST_DN.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnAluParaDlg dialog
CDgnAluParaDlg::CDgnAluParaDlg(UINT nID, CWnd* pParent)
	: CMenuBarChildDlg(nID, pParent)
{
}


void CDgnAluParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnAluParaDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnAluParaDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnAluParaDlg)
	//ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	//ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnAluParaDlg message handlers

void CDgnAluParaDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnAluParaDlg::Execute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if(aSelKey.GetSize() > 0)
	{
		IAluDgnTool::FiltAluKeysByMatl(aSelKey, rSelKey);
		if(CDlgUtilEx::CtrlIsCheck(this, IDC_DGN_ALU_ADD))	// add/replace
		{
			if(rSelKey.GetSize() > 0)
			{
					if(OnAddData(pDoc, rSelKey))
						Initial_SelectItem();
			}
			else	
				AfxMessageBox(_LS(IDS_DGN_CHK_BEAM_TYPE_ALU),MB_OK);
		}
		else	// Delete
		{
				if(OnDelData(pDoc, rSelKey))
					Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnAluParaDlg::Initial_SelectItem() 
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	// Unselected All.
	if(pDoc==NULL)	
		return;
	if(pDoc->m_pViewCtrl->SelectedElemCount())
		pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}
	 
BOOL CDgnAluParaDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	//
	//
	CDlgUtilEx::CtrlCheck(this, IDC_DGN_ALU_ADD, TRUE);
	Initial_Data();
	Update_InitDataByCode();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnAluParaDlg::Initial_Data()
{	
}

// Add by ZINU.('01.1.3).
void CDgnAluParaDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				// Add by ZINU.('01.1.3).
				Update_InitDataByCode();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

// Add by ZINU.('01.1.3).
void CDgnAluParaDlg::Update_InitDataByCode()
{
}

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDgnAluAribDlg, CDgnAluParaDlg)
	//{{AFX_MSG_MAP(CDgnAluParaDlg)
	ON_BN_CLICKED(IDC_RADIO_ALU_ARIB_MODE1, OnCheckMode)
	ON_BN_CLICKED(IDC_RADIO_ALU_ARIB_MODE2, OnCheckMode)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
CDgnAluAribDlg::CDgnAluAribDlg(CWnd* pParent):CDgnAluParaDlg(IDD_DGN_ALU_ARIB, pParent)
{
}
void CDgnAluAribDlg::Initial_Data()
{
	CDlgUtilEx::CtrlCheck(this, IDC_RADIO_ALU_ARIB_MODE1, TRUE); 
	CDlgUtilEx::CtrlCheck(this, IDC_RADIO_ALU_ARIB_MODE2, FALSE); 
	CDlgUtilEx::SetWindowVal(this, IDC_EDIT_ALU_ARIB_RATIO, 1.2);
}
void CDgnAluAribDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnAluParaDlg::DoDataExchange(pDX);
}
BOOL CDgnAluAribDlg::OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	T_ARIB_D rData;
	rData.Initialize();
	rData.m_nType = CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_ALU_ARIB_MODE1) ? 0 : 1;
	rData.m_dAdjRatio = CDlgUtilEx::GetWindowVal(this, IDC_EDIT_ALU_ARIB_RATIO);
	if(pDoc->m_pDataCtrl->AddArib(lst,rData))
		return TRUE;
	return FALSE;
}
BOOL CDgnAluAribDlg::OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	if(pDoc->m_pDataCtrl->DelArib(lst))
		return TRUE;
	return FALSE;
}
void CDgnAluAribDlg::OnCheckMode()
{
	BOOL bMode1 = CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_ALU_ARIB_MODE1);
	GetDlgItem(IDC_STATIC_ALU_ARIB_RATIO)->EnableWindow(bMode1);
	GetDlgItem(IDC_EDIT_ALU_ARIB_RATIO)->EnableWindow(bMode1);    
}
//////////////////////////////////////////////////////////////////////////
CDgnAluAhtrDlg::CDgnAluAhtrDlg(CWnd* pParent):CDgnAluParaDlg(IDD_DGN_ALU_AHTR, pParent)
{
}
void CDgnAluAhtrDlg::Initial_Data()
{
	m_Val.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Val.SetEditUnit(0);
}
void CDgnAluAhtrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnAluParaDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DGN_ALU_AHTR_VAL    , m_Val);
	DDX_Control(pDX, IDC_STATIC_ALU_AHTR_UNIT, m_Unit);  
		
}
BOOL CDgnAluAhtrDlg::OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	T_AHTR_D rData;
	rData.Initialize();
	rData.m_dWidth = m_Val.GetEditValue();
	if(pDoc->m_pDataCtrl->AddAhtr(lst,rData))
		return TRUE;
	return FALSE;
}
BOOL CDgnAluAhtrDlg::OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	if(pDoc->m_pDataCtrl->DelAhtr(lst))
		return TRUE;
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
CDgnAluAwedDlg::CDgnAluAwedDlg(CWnd* pParent):CDgnAluParaDlg(IDD_DGN_ALU_AWED, pParent)
{
}
void CDgnAluAwedDlg::Initial_Data()
{
	T_AWED_D data;data.Initialize();
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_ALU_AWED_RATIO, data.m_dRatio);
}
void CDgnAluAwedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnAluParaDlg::DoDataExchange(pDX);
}
BOOL CDgnAluAwedDlg::OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	T_AWED_D rData;
	rData.Initialize();
	rData.m_dRatio = CDlgUtilEx::GetWindowVal(this, IDC_DGN_ALU_AWED_RATIO);
	if(pDoc->m_pDataCtrl->AddAwed(lst,rData))
		return TRUE;
	return FALSE;
}
BOOL CDgnAluAwedDlg::OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	if(pDoc->m_pDataCtrl->DelAwed(lst))
		return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
CDgnAluAPhiDlg::CDgnAluAPhiDlg(CWnd* pParent):CDgnAluParaDlg(IDD_DGN_ALU_APHI, pParent)
{
}
void CDgnAluAPhiDlg::Initial_Data()
{
	T_APHI_D data;
	data.Initialize();
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_KFACTOR_BKY, data.m_dPhiy);
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_KFACTOR_BKZ, data.m_dPhiz);
}
void CDgnAluAPhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnAluParaDlg::DoDataExchange(pDX);
}
BOOL CDgnAluAPhiDlg::OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	T_APHI_D rData;
	rData.Initialize();
	rData.m_dPhiy = CDlgUtilEx::GetWindowVal(this, IDC_DGN_KFACTOR_BKY);
	rData.m_dPhiz = CDlgUtilEx::GetWindowVal(this, IDC_DGN_KFACTOR_BKZ);
	if(pDoc->m_pDataCtrl->AddAphi(lst, rData))
		return TRUE;
	return FALSE;
}
BOOL CDgnAluAPhiDlg::OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	if(pDoc->m_pDataCtrl->DelAphi(lst))
		return TRUE;
	return FALSE;
}
