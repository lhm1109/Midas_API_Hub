// DgnCfStabCoefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfParaDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	
#include "..\wg_db\cfsDgnTool.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\wg_common\TBUtilFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCfParaDlg dialog
CDgnCfParaDlg::CDgnCfParaDlg(UINT nID, CWnd* pParent)
	: CMenuBarChildDlg(nID, pParent)
{
}


void CDgnCfParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCfParaDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCfParaDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnCfParaDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCfParaDlg message handlers
void CDgnCfParaDlg::Execute()
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if(aSelKey.GetSize() > 0)
	{
		ICFSteelDgnTool::FiltCFSteelKeysByMatl(aSelKey, rSelKey);
		if(CDlgUtilEx::CtrlIsCheck(this, IDC_DGN_CF_COEF_ADD))	// add/replace
		{
			if(rSelKey.GetSize() > 0)
			{
				if(OnAddData(pDoc, rSelKey))
					Initial_SelectItem();
			}
			else	
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_CFS),MB_OK);
		}
		else	// Delete
		{
			if(OnDelData(pDoc, rSelKey))
				Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnCfParaDlg::Initial_SelectItem() 
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	// Unselected All.
	if(pDoc==NULL)	
		return;
	if(pDoc->m_pViewCtrl->SelectedElemCount())
		pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

BOOL CDgnCfParaDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	//
	CDlgUtilEx::CtrlCheck(this, IDC_DGN_CF_COEF_ADD, TRUE);
	Initial_Data();
	Update_InitDataByCode();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCfParaDlg::Initial_Data()
{	
}

void CDgnCfParaDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnCfParaDlg::Update_InitDataByCode()
{
}


/////////////////////////////////////////////////////////////////////////////
// CDgnCfStabCoefDlg dialog
CDgnCfStabCoefDlg::CDgnCfStabCoefDlg(CWnd* pParent):CDgnCfParaDlg(IDD_DGN_CF_STABCOEF_DLG, pParent)
{
}

void CDgnCfStabCoefDlg::Initial_Data()
{
	T_CSMS_D data;
	data.Initialize();
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_COLDSTYLE_STABCOEF_BKY, data.m_dPhiby);
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_COLDSTYLE_STABCOEF_BKZ, data.m_dPhibz);
}

void CDgnCfStabCoefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnCfParaDlg::DoDataExchange(pDX);
}

BOOL CDgnCfStabCoefDlg::OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	T_CSMS_D rData;
	rData.Initialize();
	rData.m_dPhiby = CDlgUtilEx::GetWindowVal(this, IDC_DGN_COLDSTYLE_STABCOEF_BKY);
	rData.m_dPhibz = CDlgUtilEx::GetWindowVal(this, IDC_DGN_COLDSTYLE_STABCOEF_BKZ);
	if(pDoc->m_pDataCtrl->AddCsms(lst, rData))
		return TRUE;
	return FALSE;
}
BOOL CDgnCfStabCoefDlg::OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	if(pDoc->m_pDataCtrl->DelCsms(lst))
		return TRUE;
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CDgnCfOpenSectCoefDlg dialog
CDgnCfOpenSectCoefDlg::CDgnCfOpenSectCoefDlg(CWnd* pParent):CDgnCfParaDlg(IDD_DGN_CF_OPENSECT_COEF_DLG, pParent)
{
}

void CDgnCfOpenSectCoefDlg::Initial_Data()
{
	T_CSOS_D data;
	data.Initialize();
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_CF_OPENSECT_COEF_ALPHA, data.m_dAlpha);
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_CF_OPENSECT_COEF_BETA, data.m_dBeta);
}

void CDgnCfOpenSectCoefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnCfParaDlg::DoDataExchange(pDX);
}

BOOL CDgnCfOpenSectCoefDlg::OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	T_CSOS_D rData;
	rData.Initialize();
	rData.m_dAlpha = CDlgUtilEx::GetWindowVal(this, IDC_DGN_CF_OPENSECT_COEF_ALPHA);
	rData.m_dBeta = CDlgUtilEx::GetWindowVal(this, IDC_DGN_CF_OPENSECT_COEF_BETA);
	if(pDoc->m_pDataCtrl->AddCsos(lst, rData))
		return TRUE;
	return FALSE;
}
BOOL CDgnCfOpenSectCoefDlg::OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst)
{
	if(pDoc->m_pDataCtrl->DelCsos(lst))
		return TRUE;
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CDgnCfPurlinDlg dialog
CDgnCfPurlinDlg::CDgnCfPurlinDlg(CWnd* pParent) :CDgnCfParaDlg(IDD_DGN_CF_PURLIN_DLG, pParent)
{
}

void CDgnCfPurlinDlg::Initial_Data()
{
	CDlgUtilEx::CtrlCheck(this, IDC_DGN_CF_PURLIN_RADIO1, TRUE);
	T_PLIN_D data;
	data.Initialize();
}

void CDgnCfPurlinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnCfParaDlg::DoDataExchange(pDX);
}

BOOL CDgnCfPurlinDlg::OnAddData(CDBDoc* pDoc, T_KEY_LIST& lst)
{
	T_PLIN_D rData;
	rData.Initialize();
	if (CDlgUtilEx::CtrlIsCheck(this, IDC_DGN_CF_PURLIN_RADIO1))
		rData.bPurlin = TRUE;
	if (pDoc->m_pDataCtrl->AddPlin(lst, rData))
		return TRUE;
	return FALSE;
}
BOOL CDgnCfPurlinDlg::OnDelData(CDBDoc* pDoc, T_KEY_LIST& lst)
{
	if (pDoc->m_pDataCtrl->DelPlin(lst))
		return TRUE;
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnCfPhiCoefDlg dialog
CDgnCfPhiCoefDlg::CDgnCfPhiCoefDlg(CWnd* pParent) :CDgnCfParaDlg(IDD_DGN_CF_PHI_COEF_DLG, pParent)
{
}

void CDgnCfPhiCoefDlg::Initial_Data()
{
	T_SPHI_D data;
	data.Initialize();
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_CF_PHI_Y, data.dPhiy);
	CDlgUtilEx::SetWindowVal(this, IDC_DGN_CF_PHI_Z, data.dPhiz);
}

void CDgnCfPhiCoefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnCfParaDlg::DoDataExchange(pDX);
}

BOOL CDgnCfPhiCoefDlg::OnAddData(CDBDoc* pDoc, T_KEY_LIST &lst)
{
	T_SPHI_D rData;
	rData.Initialize();
	rData.dPhiy = CDlgUtilEx::GetWindowVal(this, IDC_DGN_CF_PHI_Y);
	rData.dPhiz = CDlgUtilEx::GetWindowVal(this, IDC_DGN_CF_PHI_Z);
	rData.bAutoPhi = TRUE;
	if (pDoc->m_pDataCtrl->AddSPhi(lst, rData))
		return TRUE;
	return FALSE;
}

BOOL CDgnCfPhiCoefDlg::OnDelData(CDBDoc* pDoc, T_KEY_LIST &lst)
{
	if (pDoc->m_pDataCtrl->DelSPhi(lst))
		return TRUE;
	return FALSE;
}