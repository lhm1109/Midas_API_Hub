// DgnGenKFacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlPfcmDlg.h"

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
// CDgnStlPfcmDlg dialog


CDgnStlPfcmDlg::CDgnStlPfcmDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlPfcmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlPfcmDlg)
	m_nOption = -1;
	m_dPerforMinCoe = 0.35;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlPfcmDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlPfcmDlg)
	DDX_Radio(pDX, IDC_DGN_KFACTOR_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_PFCM_VALUE, m_dPerforMinCoe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlPfcmDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlPfcmDlg)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_AR, OnDgnPfcmAr)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_DEL, OnDgnPfcmDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlPfcmDlg message handlers

BOOL CDgnStlPfcmDlg::OnInitDialog() 
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

void CDgnStlPfcmDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlPfcmDlg::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
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
			//modify by xuezc(2018/1/31)GB50017-17
			if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
			T_MATD_D MatlD;MatlD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(eData.elmat,MatlD))continue;
			if(MatlD.Type != _T("S")) continue;
			if(m_pDoc->m_pAttrCtrl->IsBeam(eData.eltyp))	rBeamSelKey.Add(key);
			/*if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rBeamSelKey.Add(key);*/
		}
		T_PFCM_D rData;
		rData.Initialize();
		if(rBeamSelKey.GetSize() > 0)
		{
			if(m_nOption == 0)//add/replace
			{
				UpdateData(TRUE);
				rData.Pfcm = m_dPerforMinCoe;
				if(m_pDoc->m_pDataCtrl->AddPfcm(rBeamSelKey,rData)) Initial_SelectItem();
			}
			else
				if(m_pDoc->m_pDataCtrl->DelPfcm(rBeamSelKey)) Initial_SelectItem();
		}
		else
			AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_NO_DEF_PHIB),MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlPfcmDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnStlPfcmDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_dPerforMinCoe <= 0.)		bCheck = FALSE;

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_PHIB_FACTOR),MB_OK);

	return bCheck;
}

void CDgnStlPfcmDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	m_dPerforMinCoe  = .35;
	// Add by ZINU.('02.3.21).
	Update_InitDataByCode();
	UpdateData(FALSE);
}


void CDgnStlPfcmDlg::OnDgnPfcmAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlPfcmDlg::OnDgnPfcmDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('02.3.21).
void CDgnStlPfcmDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnStlPfcmDlg::Update_InitDataByCode()
{
	CString strInitKy = _T("┯min");
	//CString strKy = m_pDoc->GetDesignParamNa(strInitKy);
	GetDlgItem(IDC_DGN_KY_STATIC)->SetWindowText(strInitKy);

	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_PERFOR_MIN_COEFFICEN, strTitle, bShow);

	EnableItems(bShow);
}


void CDgnStlPfcmDlg::EnableItems(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_KFACTOR_AR)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_KFACTOR_DEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_PFCM_VALUE)->EnableWindow(bEnable);
}