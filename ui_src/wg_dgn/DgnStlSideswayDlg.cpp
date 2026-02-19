// DgnGenKFacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSideswayDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\DgnData4Report.h"

#include "DgnGenKTypeDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSideswayDlg dialog


CDgnStlSideswayDlg::CDgnStlSideswayDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlSideswayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlPhibFacDlg)
	m_nOption = -1;
	m_nSideswayy = 0;
	m_nSideswayz = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlSideswayDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlPhibFacDlg)
	DDX_Radio(pDX, IDC_DGN_FACTOR_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_Y_YES, m_nSideswayy);
	DDX_Radio(pDX, IDC_DGN_Z_YES, m_nSideswayz);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlSideswayDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlPhibFacDlg)
	ON_BN_CLICKED(IDC_DGN_FACTOR_AR, OnDgnAr)
	ON_BN_CLICKED(IDC_DGN_FACTOR_DEL, OnDgnDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSideswayDlg message handlers

BOOL CDgnStlSideswayDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Default 값 초기화
	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlSideswayDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlSideswayDlg::OnDgnExecute() 
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
		T_SDSW_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.bSideSway_Y = (m_nSideswayy==0);
		  rData.bSideSway_Z = (m_nSideswayz==0);
			if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddSdsw(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelSdsw(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlSideswayDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnStlSideswayDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
//	if(m_dPhiby <= 0.)		bCheck = FALSE;
//if(m_dPhibz <= 0.)		bCheck = FALSE;
//if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_PHIB_FACTOR),MB_OK);
	return bCheck;
}

void CDgnStlSideswayDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	m_nSideswayy  = 0;
	m_nSideswayz  = 0;
	// Add by ZINU.('02.3.21).
	Update_InitDataByCode();
	UpdateData(FALSE);
}


void CDgnStlSideswayDlg::OnDgnAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlSideswayDlg::OnDgnDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('02.3.21).
void CDgnStlSideswayDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnStlSideswayDlg::Update_InitDataByCode()
{
	/*
	CString strInitKy = _T("┵by"), strInitKz = _T("┵bz");
	CString strKy = m_pDoc->GetDesignParamNa(strInitKy);
	CString strKz = m_pDoc->GetDesignParamNa(strInitKz);
	GetDlgItem(IDC_DGN_KY_STATIC)->SetWindowText(strKy);
	GetDlgItem(IDC_DGN_KZ_STATIC)->SetWindowText(strKz);
	*/
}