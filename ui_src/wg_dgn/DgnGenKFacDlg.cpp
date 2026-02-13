// DgnGenKFacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenKFacDlg.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\DgnData4Report.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\DBCodeCtrl.h"

#include "DgnGenKTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenKFacDlg dialog


CDgnGenKFacDlg::CDgnGenKFacDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenKFacDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenKFacDlg)
	m_nOption = -1;
	m_BKy = 0.0;
	m_BKz = 0.0;
	m_BKt = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenKFacDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenKFacDlg)
	DDX_Radio(pDX, IDC_DGN_KFACTOR_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_KFACTOR_BKY, m_BKy);
	DDX_Text(pDX, IDC_DGN_KFACTOR_BKZ, m_BKz);
	DDX_Text(pDX, IDC_DGN_KFACTOR_BKT, m_BKt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnGenKFacDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenKFacDlg)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_KYINPUT, OnDgnKfactorKyinput)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_KZINPUT, OnDgnKfactorKzinput)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_AR, OnDgnKfactorAr)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_DEL, OnDgnKfactorDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenKFacDlg message handlers

BOOL CDgnGenKFacDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Default 값 초기화
	Initial_Data();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenKFacDlg::Execute() 
{
#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STEEL_MEMBER_DESIGN))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK7));
		AfxMessageBox(strErrMsg);
		return;
	}
#endif

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
		T_KFAC_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.BKy = m_BKy;
			rData.BKz = m_BKz;
			rData.BKt = m_BKt;
			if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddKfac(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelKfac(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenKFacDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnGenKFacDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_BKy <= 0.)		bCheck = FALSE;
	if(m_BKz <= 0.)		bCheck = FALSE;
	if(m_BKt <= 0.)   bCheck = FALSE;

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_EFFECT_LEN_FACTOR),MB_OK);

	return bCheck;
}

void CDgnGenKFacDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace

	m_BKy  = 1.0;
	m_BKz  = 1.0;
	m_BKt  = 1.0;
	// Add by ZINU.('02.3.21).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnGenKFacDlg::OnDgnKfactorKyinput() 
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

void CDgnGenKFacDlg::OnDgnKfactorKzinput() 
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

void CDgnGenKFacDlg::OnDgnKfactorAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenKFacDlg::OnDgnKfactorDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('02.3.21).
void CDgnGenKFacDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
				// Add by ZINU.('02.3.21).
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

// Add by ZINU.('02.3.21).
void CDgnGenKFacDlg::Update_InitDataByCode()
{
	CString strInitKy = _T("Ky"), strInitKz = _T("Kz");
	CString strKy = m_pDoc->GetDesignParamNa(strInitKy);
	CString strKz = m_pDoc->GetDesignParamNa(strInitKz);
	GetDlgItem(IDC_DGN_KY_STATIC)->SetWindowText(strKy);
	GetDlgItem(IDC_DGN_KZ_STATIC)->SetWindowText(strKz);

	int nShowKt = SW_SHOW;
#if defined(_CIVIL)
	nShowKt = SW_HIDE;
#else	
	nShowKt = []()
	{
		if (CDBCodeCtrl::IsStlCodeConsiderKtLt()) return SW_SHOW;
		if (CDBCodeCtrl::IsCfsCodeConsiderKtLt()) return SW_SHOW;

		return SW_HIDE;
	}();	
#endif
	GetDlgItem(IDC_DGN_KT_STATIC)->ShowWindow(nShowKt);
	GetDlgItem(IDC_DGN_KFACTOR_BKT)->ShowWindow(nShowKt);
}