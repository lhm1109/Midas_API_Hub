// DgnCfCbDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfCbDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_CF_CBFT


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCfCbDlg dialog


CDgnCfCbDlg::CDgnCfCbDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnCfCbDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCfCbDlg)
	m_nOption = -1;
	m_Cb = 0.0;
	m_bAutoCb = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnCfCbDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCfCbDlg)
	DDX_Radio(pDX, IDC_DGN_CB_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CB_CB, m_Cb);
	DDX_Check(pDX, IDC_DGN_CBCAL_PROGRAM, m_bAutoCb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCfCbDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnCfCbDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CB_AR, OnDgnCbAr)
	ON_BN_CLICKED(IDC_DGN_CB_DEL, OnDgnCbDel)
	ON_BN_CLICKED(IDC_DGN_CBCAL_PROGRAM, OnDgnCbcalProgram)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCfCbDlg message handlers

void CDgnCfCbDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnCfCbDlg::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	BOOL bExistFrameType = FALSE;

	if(aSelKey.GetSize() > 0)
	{
		CString strCfsCodeName = CDBLib::GetCfsCodeName();

		T_MATD_D MatlD; MatlD.Initialize();
		T_SECT_D SectD; SectD.Initialize();
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			// Beam, Truss.
			if(!m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp)) continue;

			bExistFrameType = TRUE;

			if(!m_pDoc->m_pAttrCtrl->GetSectDesign(eData.elpro, SectD)) continue;
			if(!m_pDoc->m_pAttrCtrl->IsColdFormedSect4Code(strCfsCodeName, SectD.SectBefore.Shape, TRUE)) continue;
			
			rSelKey.Add(key);
		}
		T_CBFT_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			UpdateData(TRUE);
			rData.bAuto = m_bAutoCb;
			rData.Cb	  = m_Cb;
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddCbft(rSelKey,rData))	Initial_SelectItem();
			}
			else
			{
				if(bExistFrameType)
					AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_CFS),MB_OK);
				else
					AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
			}
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelCbft(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnCfCbDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnCfCbDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	// Default 값 초기화
	Initial_Data();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCfCbDlg::Initial_Data()
{
	m_nOption	= 0;      // add/replace
	m_bAutoCb = FALSE;  // Not Automeation
	m_Cb = 1.0;
	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnCfCbDlg::OnDgnCbAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnCfCbDlg::OnDgnCbDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnCfCbDlg::OnDgnCbcalProgram() 
{
	UpdateData(TRUE);

	if(m_bAutoCb)   // Automatic Calculation
	{
		m_Cb = 0.;
	  GetDlgItem(IDC_DGN_CB_CB)->EnableWindow(FALSE);
	}
	else            // Not Automatic Calculation
	{
		GetDlgItem(IDC_DGN_CB_CB)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDgnCfCbDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			ASSERT(TRUE);
	}
}

void CDgnCfCbDlg::Update_InitDataByCode()
{
	CString strInitCb = _T("Cb");
	CString strCb=_T(""); strCb.Format(_T("%6s"), CDBDoc::GetDocPoint()->GetDesignParamNa(strInitCb, TRUE));
	GetDlgItem(IDC_DGN_CB_STATIC)->SetWindowText(strCb);
	if(strInitCb==_T("CmLT"))  	GetDlgItem(IDC_DGN_CB_BC_STATIC)->SetWindowText(_LS(IDS_TREE_TEXT_STL_BEND_COEFF_EC));
	else										GetDlgItem(IDC_DGN_CB_BC_STATIC)->SetWindowText(_LS(IDS_TREE_TEXT_STL_BEND_COEFF));

	CString strTitle=_T("");
	BOOL bShow = FALSE;
	CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_CBFT, strTitle, bShow);
	if(bShow)
	{
		GetDlgItem(IDC_DGN_CB_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CB_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CB_CB)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CBCAL_PROGRAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CB_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CB_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CB_CB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CBCAL_PROGRAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}
}