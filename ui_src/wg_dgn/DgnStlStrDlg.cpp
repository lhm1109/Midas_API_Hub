// DgnStlStrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlStrDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_STL_DALW

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlStrDlg dialog


CDgnStlStrDlg::CDgnStlStrDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlStrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlStrDlg)
	m_nOption = -1;
	m_Fa = 0.0;
	m_Fby = 0.0;
	m_Fbz = 0.0;
	m_Ft = 0.0;
	m_Fv = 0.0;
	m_StLtRatio = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnStlStrDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlStrDlg)
	DDX_Radio(pDX, IDC_DGN_DFNSTRESS_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_DFNSTRESS_FA, m_Fa);
	DDX_Text(pDX, IDC_DGN_DFNSTRESS_FBY, m_Fby);
	DDX_Text(pDX, IDC_DGN_DFNSTRESS_FBZ, m_Fbz);
	DDX_Text(pDX, IDC_DGN_DFNSTRESS_FT, m_Ft);
	DDX_Text(pDX, IDC_DGN_DFNSTRESS_FV, m_Fv);
	//add by cylee 05.06.25
	DDX_Control(pDX, IDC_DGN_STL_COMP_USE_CHECK, m_wndUser_Fa);
	DDX_Control(pDX, IDC_DGN_STL_TENS_USE_CHECK, m_wndUser_Ft);
	DDX_Control(pDX, IDC_DGN_STL_BENDY_USE_CHECK, m_wndUser_Fby);
	DDX_Control(pDX, IDC_DGN_STL_BENDZ_USE_CHECK, m_wndUser_Fbz);
	DDX_Control(pDX, IDC_DGN_STL_SHEAR_USE_CHECK, m_wndUser_Fv);
	DDX_Text(pDX, IDC_DGN_STL_STLT_RATIO_EDIT, m_StLtRatio);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnStlStrDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlStrDlg)
	ON_BN_CLICKED(IDC_DGN_DFNSTRESS_AR, OnDgnDfnstressAr)
	ON_BN_CLICKED(IDC_DGN_DFNSTRESS_DEL, OnDgnDfnstressDel)
	ON_BN_CLICKED(IDC_DGN_STL_COMP_USE_CHECK, OnDgnUserInputStress)
	ON_BN_CLICKED(IDC_DGN_STL_TENS_USE_CHECK, OnDgnUserInputStress)
	ON_BN_CLICKED(IDC_DGN_STL_BENDY_USE_CHECK, OnDgnUserInputStress)
	ON_BN_CLICKED(IDC_DGN_STL_BENDZ_USE_CHECK, OnDgnUserInputStress)
	ON_BN_CLICKED(IDC_DGN_STL_SHEAR_USE_CHECK, OnDgnUserInputStress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlStrDlg message handlers

void CDgnStlStrDlg::Execute() 
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
		T_DALW_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.Fa	= m_Fa;
			rData.Fby	= m_Fby;
	  	rData.Fbz	= m_Fbz;
		  rData.Ft	= m_Ft;
			rData.Fv	= m_Fv;
			//add by cylee 05.06.25
			rData.bFa	 = m_wndUser_Fa.GetCheck();
			rData.bFby = m_wndUser_Fby.GetCheck();
	  	rData.bFbz = m_wndUser_Fbz.GetCheck();
		  rData.bFt	 = m_wndUser_Ft.GetCheck();
			rData.bFv	 = m_wndUser_Fv.GetCheck();
			rData.dShortTermRatio	 = m_StLtRatio;
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddDalw(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelDalw(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlStrDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnStlStrDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// Default 값 초기화	
	Initial_Data();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlStrDlg::Initial_Data()
{
	CString strDgnCode=_T("");
	T_DSTL_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDstl(rData);
	strDgnCode = rData.DesignCode;
	
	m_nOption = 0;      //  add/replace
	m_Fa  = 0.5;
	m_Fby = 0.6;
	m_Fbz = 0.6;
	m_Ft  = 0.6;
	m_Fv  = 0.4;
	//add by cylee 05.06.25
	m_wndUser_Fa.SetCheck(TRUE);
	m_wndUser_Fby.SetCheck(TRUE);
	m_wndUser_Fbz.SetCheck(TRUE);
	m_wndUser_Ft.SetCheck(TRUE);
	m_wndUser_Fv.SetCheck(TRUE);
	m_StLtRatio = 1.5;
	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
	
	if(strDgnCode!=_T("AIJ-ASD02")) 
	{
		GetDlgItem(IDC_DGN_TERM_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_STLT_RATIO_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_TERM_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_STLT_RATIO_EDIT)->EnableWindow(TRUE);
	}
}

void CDgnStlStrDlg::OnDgnDfnstressAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlStrDlg::OnDgnDfnstressDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('01.1.3).
void CDgnStlStrDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnStlStrDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_STL_DALW, strTitle, bShow);
	if(bShow)
	{
		GetDlgItem(IDC_DGN_DFNSTRESS_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DFNSTRESS_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FA)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FBY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FBZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FV)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
		//add by cylee 05.06.26
		m_wndUser_Fa.SetCheck(TRUE);
		m_wndUser_Fby.SetCheck(TRUE);
	  m_wndUser_Fbz.SetCheck(TRUE);
	  m_wndUser_Ft.SetCheck(TRUE);
	  m_wndUser_Fv.SetCheck(TRUE);
		GetDlgItem(IDC_DGN_STL_COMP_USE_CHECK) ->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_TENS_USE_CHECK) ->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_BENDY_USE_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_BENDZ_USE_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_SHEAR_USE_CHECK) ->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_DFNSTRESS_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DFNSTRESS_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FA)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FBY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FBZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DFNSTRESS_FV)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
		//add by cylee 05.06.26
		m_wndUser_Fa.SetCheck(FALSE);
		m_wndUser_Fby.SetCheck(FALSE);
	  m_wndUser_Fbz.SetCheck(FALSE);
	  m_wndUser_Ft.SetCheck(FALSE);
	  m_wndUser_Fv.SetCheck(FALSE);
		GetDlgItem(IDC_DGN_STL_COMP_USE_CHECK) ->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_TENS_USE_CHECK) ->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_BENDY_USE_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_BENDZ_USE_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_SHEAR_USE_CHECK) ->EnableWindow(FALSE);
	}
	CString strDgnCode=_T("");
	T_DSTL_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDstl(rData);
	strDgnCode = rData.DesignCode;
	// Coded by Seungjun MNet:No.2354 ('20060803)
	if(strDgnCode!=_T("AIJ-ASD02")) 
	{
		GetDlgItem(IDC_DGN_TERM_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_STLT_RATIO_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_TERM_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_STLT_RATIO_EDIT)->EnableWindow(TRUE);
	}

	if(strDgnCode==_T("KSSC-ASD03"))  GetDlgItem(IDC_AxialComp_Title)->SetWindowText(_LS(IDS_TREE_TEXT_DGN_AXIAL_COMPRESSIVE_STRESS_Fc));
	else                          GetDlgItem(IDC_AxialComp_Title)->SetWindowText(_LS(IDS_TREE_TEXT_DGN_AXIAL_COMPRESSIVE_STRESS_Fa));
}

void CDgnStlStrDlg::OnDgnUserInputStress()
{
	GetDlgItem(IDC_DGN_DFNSTRESS_FA) ->EnableWindow(m_wndUser_Fa.GetCheck());
	GetDlgItem(IDC_DGN_DFNSTRESS_FT) ->EnableWindow(m_wndUser_Ft.GetCheck());
	GetDlgItem(IDC_DGN_DFNSTRESS_FBY)->EnableWindow(m_wndUser_Fby.GetCheck());
	GetDlgItem(IDC_DGN_DFNSTRESS_FBZ)->EnableWindow(m_wndUser_Fbz.GetCheck());
	GetDlgItem(IDC_DGN_DFNSTRESS_FV) ->EnableWindow(m_wndUser_Fv.GetCheck());
}
