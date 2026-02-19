// DgnGenKFacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSectionTypeDlg.h"

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
#include "..\wg_main\wg_mainres.h"	


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSectionTypeDlg dialog


CDgnStlSectionTypeDlg::CDgnStlSectionTypeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlSectionTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlSectionTypeDlg)
	m_nOption = -1;
	m_nTypeY = 1;
	m_nTypeZ = 1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlSectionTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlSectionTypeDlg)
	DDX_Radio(pDX, IDC_DGN_KFACTOR_AR, m_nOption);
	DDX_Control(pDX, IDC_DGN_SECTTYPE_Y_COMBO, m_cmbTypeY);
	DDX_Control(pDX, IDC_DGN_SECTTYPE_Z_COMBO, m_cmbTypeZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlSectionTypeDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlSectionTypeDlg)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_AR, OnDgnSectionTypeAr)
	ON_BN_CLICKED(IDC_DGN_KFACTOR_DEL, OnDgnSectionTypeDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSectionTypeDlg message handlers

BOOL CDgnStlSectionTypeDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlSectionTypeDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlSectionTypeDlg::OnDgnExecute() 
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
		T_SECTCLASSIFY_D rData;
		rData.Initialize();
		
		if(rElemSelKey.GetSize() > 0)
		{
			if(m_nOption == 0)//add/replace
			{
				UpdateData(TRUE);
				rData.nSectClassifyY = m_cmbTypeY.GetCurSel();
				rData.nSectClassifyZ = m_cmbTypeZ.GetCurSel();
				if(m_pDoc->m_pDataCtrl->AddSectClassify(rElemSelKey,rData)) Initial_SelectItem();
			}
			else
				if(m_pDoc->m_pDataCtrl->DelSectClassify(rElemSelKey)) Initial_SelectItem();
		}
		else
			AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_NO_DEF_SECTCLASSIFY),MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlSectionTypeDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	//Initial_Data();
}

BOOL CDgnStlSectionTypeDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	//if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_PHIB_FACTOR),MB_OK);

	return bCheck;
}

void CDgnStlSectionTypeDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	Inital_SectTypeCombo_Y();
	Inital_SectTypeCombo_Z();
	Update_InitDataByCode();
	UpdateData(FALSE);
}

void CDgnStlSectionTypeDlg::Inital_SectTypeCombo_Y()
{
	m_cmbTypeY.ResetContent();
	m_cmbTypeY.AddString(_T("a"));
	m_cmbTypeY.AddString(_T("b"));
	m_cmbTypeY.AddString(_T("c"));
	m_cmbTypeY.AddString(_T("d"));
	m_cmbTypeY.SetCurSel(1);
}

void CDgnStlSectionTypeDlg::Inital_SectTypeCombo_Z()
{
	m_cmbTypeZ.ResetContent();
	m_cmbTypeZ.AddString(_T("a"));
	m_cmbTypeZ.AddString(_T("b"));
	m_cmbTypeZ.AddString(_T("c"));
	m_cmbTypeZ.AddString(_T("d"));
	m_cmbTypeZ.SetCurSel(1);
}

void CDgnStlSectionTypeDlg::OnDgnSectionTypeAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlSectionTypeDlg::OnDgnSectionTypeDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('02.3.21).
void CDgnStlSectionTypeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnStlSectionTypeDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_STL_SECTION_TYPE, strTitle, bShow);

	EnableItems(bShow);
}
//Add by xuezc For Modify Code -Only GB50017-17 Enable this Dlg

void CDgnStlSectionTypeDlg::EnableItems(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_KFACTOR_AR)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_KFACTOR_DEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_SECTTYPE_Y_COMBO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_SECTTYPE_Z_COMBO)->EnableWindow(bEnable);
}
