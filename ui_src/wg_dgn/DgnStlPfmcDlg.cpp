// DgnGenKFacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlPfmcDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\DgnData4Report.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "DgnGenKTypeDlg.h"
#include "..\wg_main\wg_mainres2.h"	


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlPfmcDlg dialog


CDgnStlPfmcDlg::CDgnStlPfmcDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlPfmcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlPfmcDlg)
	m_nOption = -1;
	m_dPfmc = 0.3;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlPfmcDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlPfmcDlg)
	DDX_Radio(pDX, IDC_DGN_KFACTOR_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_PFMC_VALUE, m_dPfmc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlPfmcDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlPfmcDlg)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_AR, OnDgnPfmcAr)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_DEL, OnDgnPfmcDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlPfmcDlg message handlers

BOOL CDgnStlPfmcDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;
	Initial_Data();

	return TRUE;
}

void CDgnStlPfmcDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlPfmcDlg::OnDgnExecute() 
{
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rBeamSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
			T_MATD_D MatlD;MatlD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(eData.elmat,MatlD))continue;
			if(MatlD.Type != _T("S")) continue;
			if(m_pDoc->m_pAttrCtrl->IsBeam(eData.eltyp))	rBeamSelKey.Add(key);
			/*if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rBeamSelKey.Add(key);*/
		}
		T_PFMC_D rData;
		rData.Initialize();
		rData.Pfmc = m_dPfmc;
		if(rBeamSelKey.GetSize() > 0)
		{
			UpdateData(TRUE);
			rData.Pfmc = m_dPfmc;
			if(m_pDoc->m_pDataCtrl->AddPfmc(rBeamSelKey,rData)) Initial_SelectItem();
		}
		else
			AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_NO_DEF_PHIB),MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlPfmcDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnStlPfmcDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_PHIB_FACTOR),MB_OK);

	return bCheck;
}

void CDgnStlPfmcDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	m_dPfmc = 0.3;
	// Add by ZINU.('02.3.21).
	Update_InitDataByCode();
	UpdateData(FALSE);
}


void CDgnStlPfmcDlg::OnDgnPfmcAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlPfmcDlg::OnDgnPfmcDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('02.3.21).
void CDgnStlPfmcDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnStlPfmcDlg::Update_InitDataByCode()
{
	/*CString strInitKy = _T("┵by");
	CString strKy = m_pDoc->GetDesignParamNa(strInitKy);
	CString strKz = m_pDoc->GetDesignParamNa(strInitKz);
	GetDlgItem(IDC_DGN_KY_STATIC)->SetWindowText(strKy);
	GetDlgItem(IDC_DGN_KZ_STATIC)->SetWindowText(strKz);*/
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_QUERY_DGN_PERFOR_MIN_COEFFICEN, strTitle, bShow);

	EnableItems(bShow);
}

void CDgnStlPfmcDlg::EnableItems(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_KFACTOR_AR)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_KFACTOR_DEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_PFMC_VALUE)->EnableWindow(bEnable);
}