// DgnGenKFacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlBendModulusRtoDlg.h"

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
// CDgnStlBendModulusRtoDlg dialog


CDgnStlBendModulusRtoDlg::CDgnStlBendModulusRtoDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlBendModulusRtoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlBendModulusRtoDlg)
	m_nOption = -1;
	m_dC= 0.85;
	m_dBendModulusY = 1.0;
	m_dBendModulusZ = 1.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlBendModulusRtoDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlBendModulusRtoDlg)
	DDX_Radio(pDX, IDC_DGN_KFACTOR_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_NETGROSS_RATIO_EDIT, m_dC);
	DDX_Text(pDX, IDC_DGN_BENDMODULUS_RATIO_Y_EDIT, m_dBendModulusY);
	DDX_Text(pDX, IDC_DGN_BENDMODULUS_RATIO_Z_EDIT, m_dBendModulusZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlBendModulusRtoDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlBendModulusRtoDlg)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_AR, OnDgnBendModulusRtoAr)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_DEL, OnDgnBendModulusRtoDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlBendModulusRtoDlg message handlers

BOOL CDgnStlBendModulusRtoDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlBendModulusRtoDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlBendModulusRtoDlg::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rElemSelKey;
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
			rElemSelKey.Add(key);
			
		}
		T_BENDMODULUS_D rData;
		rData.Initialize();
		
		if(rElemSelKey.GetSize() > 0)
		{
			if(m_nOption == 0)//add/replace
			{
				UpdateData(TRUE);
				rData.dNetGross = m_dC;
				rData.dBendModulusY = m_dBendModulusY;
				rData.dBendModulusZ = m_dBendModulusZ;
				if(m_pDoc->m_pDataCtrl->AddBendModulus(rElemSelKey,rData)) Initial_SelectItem();
			}
			else
				if(m_pDoc->m_pDataCtrl->DelBendModulus(rElemSelKey)) Initial_SelectItem();
		}
		else
			AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_NO_DEF_BENDMODULUS),MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlBendModulusRtoDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	//Initial_Data();
}

BOOL CDgnStlBendModulusRtoDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_dC <= 0.)					bCheck = FALSE;
	if(m_dBendModulusY <= 0.)		bCheck = FALSE;
	if(m_dBendModulusZ <= 0.)		bCheck = FALSE;

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_BEND_MODULUS_RATIO),MB_OK);

	return bCheck;
}

void CDgnStlBendModulusRtoDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	m_dC = 0.85;
	m_dBendModulusY = 1.0;
	 m_dBendModulusZ = 1.0;
	Update_InitDataByCode();
	UpdateData(FALSE);
}

void CDgnStlBendModulusRtoDlg::OnDgnBendModulusRtoAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlBendModulusRtoDlg::OnDgnBendModulusRtoDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('02.3.21).
void CDgnStlBendModulusRtoDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnStlBendModulusRtoDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_STL_BEND_MODULUS_RTO, strTitle, bShow);
}

