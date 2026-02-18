// DgnGenKFacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlPhibFacDlg.h"

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
// CDgnStlPhibFacDlg dialog


CDgnStlPhibFacDlg::CDgnStlPhibFacDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlPhibFacDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlPhibFacDlg)
	m_nOption = -1;
	m_dPhiby = 0.0;
	m_dPhibz = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlPhibFacDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlPhibFacDlg)
	DDX_Radio(pDX, IDC_DGN_KFACTOR_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_KFACTOR_BKY, m_dPhiby);
	DDX_Text(pDX, IDC_DGN_KFACTOR_BKZ, m_dPhibz);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlPhibFacDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlPhibFacDlg)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_AR, OnDgnPhibAr)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_DEL, OnDgnPhibDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlPhibFacDlg message handlers

BOOL CDgnStlPhibFacDlg::OnInitDialog() 
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

void CDgnStlPhibFacDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlPhibFacDlg::OnDgnExecute() 
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
		T_PHIB_D rData;
		rData.Initialize();
		//add by xuezc(2018/2/8)GB50017-17	Amend DataCtrl Add Phib
		if(rBeamSelKey.GetSize() > 0)
		{
			if(m_nOption == 0)//add/replace
			{
				UpdateData(TRUE);
				rData.Phiby = m_dPhiby;
				rData.Phibz = m_dPhibz;
				if(m_pDoc->m_pDataCtrl->AddPhib(rBeamSelKey,rData)) Initial_SelectItem();
			}
			else
				if(m_pDoc->m_pDataCtrl->DelPhib(rBeamSelKey)) Initial_SelectItem();
		}
		else
			AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_NO_DEF_PHIB),MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlPhibFacDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnStlPhibFacDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_dPhiby <= 0.)		bCheck = FALSE;
	if(m_dPhibz <= 0.)		bCheck = FALSE;

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_PHIB_FACTOR),MB_OK);

	return bCheck;
}

void CDgnStlPhibFacDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	m_dPhiby  = 1.0;
	m_dPhibz  = 1.0;
	// Add by ZINU.('02.3.21).
	Update_InitDataByCode();
	UpdateData(FALSE);
}


void CDgnStlPhibFacDlg::OnDgnPhibAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlPhibFacDlg::OnDgnPhibDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('02.3.21).
void CDgnStlPhibFacDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnStlPhibFacDlg::Update_InitDataByCode()
{
	CString strInitKy = _T("┵by"), strInitKz = _T("┵bz");
	CString strKy = m_pDoc->GetDesignParamNa(strInitKy);
	CString strKz = m_pDoc->GetDesignParamNa(strInitKz);
	GetDlgItem(IDC_DGN_KY_STATIC)->SetWindowText(strKy);
	GetDlgItem(IDC_DGN_KZ_STATIC)->SetWindowText(strKz);
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_STL_PHIB, strTitle, bShow);

	EnableItems(bShow);
}
//Add by xuezc For Modify Code -Only GB50017-17 Enable this Dlg

void CDgnStlPhibFacDlg::EnableItems(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_KFACTOR_AR)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_KFACTOR_DEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_KFACTOR_BKY)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_KFACTOR_BKZ)->EnableWindow(bEnable);
}