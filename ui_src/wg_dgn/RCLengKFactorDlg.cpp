// RCLengKFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCLengKFactorDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_LENG
#include "DgnDataCtrl.h"

#include "DgnGenKTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCLengKFactorDlg dialog


CRCLengKFactorDlg::CRCLengKFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CRCLengKFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCLengKFactorDlg)
	m_nOption = 0;
	m_BLy = 0.0;
	m_BLz = 0.0;
	m_BKy = 0.0;
	m_BKz = 0.0;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CRCLengKFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCLengKFactorDlg)
	DDX_Control(pDX, IDC_DGN_RC_LENGTH_BLZ, m_BLzCtrl);
	DDX_Control(pDX, IDC_DGN_RC_LENGTH_BLY, m_BLyCtrl);
	DDX_Control(pDX, IDC_DGN_RC_LENGTH_BLZUNIT, m_BLzUnit);
	DDX_Control(pDX, IDC_DGN_RC_LENGTH_BLYUNIT, m_BLyUnit);
	DDX_Radio(pDX, IDC_DGN_RC_LENGTH_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_RC_LENGTH_BLY, m_BLy);
	DDX_Text(pDX, IDC_DGN_RC_LENGTH_BLZ, m_BLz);
	DDX_Text(pDX, IDC_DGN_RC_KFACTOR_BKY, m_BKy);
	DDX_Text(pDX, IDC_DGN_RC_KFACTOR_BKZ, m_BKz);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCLengKFactorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CRCLengKFactorDlg)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_RC_LENGTH_AR, OnDgnLengthAr)
	ON_BN_CLICKED(IDC_DGN_RC_LENGTH_DEL, OnDgnLengthDel)
	ON_BN_CLICKED(IDC_DGN_RC_KFACTOR_KYINPUT, OnDgnKfactorKyinput)
	ON_BN_CLICKED(IDC_DGN_RC_KFACTOR_KZINPUT, OnDgnKfactorKzinput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCLengKFactorDlg message handlers

BOOL CRCLengKFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	if(m_pDoc==NULL)	return TRUE;
	// Default 값 초기화
	Initial_Data();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRCLengKFactorDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_BLy < 0.)		bCheck = FALSE;
	if(m_BLz < 0.)		bCheck = FALSE;
	if(m_BKy <= 0.)		bCheck = FALSE;
	if(m_BKz <= 0.)		bCheck = FALSE;
	
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_UNBRACED_LEN),MB_OK);

	return bCheck;
}

void CRCLengKFactorDlg::Initial_Data()
{
	m_BLyCtrl.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_BLyCtrl.SetAttWcsDistance();
	m_BLyCtrl.SetDistValueMode(TRUE);
	
	m_BLzCtrl.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_BLzCtrl.SetAttWcsDistance();
	m_BLzCtrl.SetDistValueMode(TRUE);

	//BOOL GetCurMousePosDistVal(double &Distance);// 나중에 값을 받아올 때...
		
	m_nOption  = 0;	// add/replace

	m_BLy = 0.0;
	m_BLz = 0.0;
	m_BKy  = 1.0;
	m_BKz  = 1.0;
		
	UpdateData(FALSE);
}

void CRCLengKFactorDlg::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			// Beam, Truss.
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rSelKey.Add(key);
		}
		T_LENG_D rLengData;
		rLengData.Initialize();
		T_KFAC_D rKfatorData;
		rKfatorData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rLengData.BLy = m_BLy;
			rLengData.BLz = m_BLz;
			rKfatorData.BKy = m_BKy;
			rKfatorData.BKz = m_BKz;
				  	
			if(rSelKey.GetSize() > 0)
			{
		    // Initialize selected Element.
			  if(m_pDoc->m_pDataCtrl->AddLengKfac(rSelKey, rLengData, rKfatorData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
	  }
		else	// Delete
	  {
			// Initialize selected Element.
		  if(m_pDoc->m_pDataCtrl->DelLengKfac(rSelKey))	Initial_SelectItem();
	  }
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CRCLengKFactorDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CRCLengKFactorDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
}

void CRCLengKFactorDlg::OnDgnLengthAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CRCLengKFactorDlg::OnDgnLengthDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CRCLengKFactorDlg::OnDgnKfactorKyinput() 
{
	UpdateData(TRUE);

	CDgnGenKTypeDlg dlg;
	dlg.m_K = m_BKy;
	if(dlg.DoModal() == IDOK)
	{
		m_BKy = dlg.m_K;
		UpdateData(FALSE);
	}
}

void CRCLengKFactorDlg::OnDgnKfactorKzinput() 
{
	UpdateData(TRUE);

	CDgnGenKTypeDlg dlg;
	dlg.m_K = m_BKz;
	if(dlg.DoModal() == IDOK)
	{
		m_BKz = dlg.m_K;
		UpdateData(FALSE);
	}
}

void CRCLengKFactorDlg::OnDgnKfactorAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CRCLengKFactorDlg::OnDgnKfactorDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}


void CRCLengKFactorDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CRCLengKFactorDlg::Update_InitDataByCode()
{
	CString strInitKy = _T("Ky"), strInitKz = _T("Kz");
	CString strKy = m_pDoc->GetDesignParamNa(strInitKy);
	CString strKz = m_pDoc->GetDesignParamNa(strInitKz);
	GetDlgItem(IDC_DGN_RC_KY_STATIC)->SetWindowText(strKy);
	GetDlgItem(IDC_DGN_RC_KZ_STATIC)->SetWindowText(strKz);
}
