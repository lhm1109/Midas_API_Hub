// DgnConJSFTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConJSFTDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConJSFTDlg dialog


CDgnConJSFTDlg::CDgnConJSFTDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConJSFTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConJSFTDlg)
	m_nOption = -1;
	m_dJackFactor = 0.7;
	m_bAuto = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConJSFTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConJSFTDlg)
	DDX_Radio(pDX, IDC_DGN_CON_JSFT_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CON_JSFT_FACTOR, m_dJackFactor);
	DDX_Check(pDX, IDC_DGN_CON_JSFT_CAL_PROGRAM, m_bAuto);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConJSFTDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConJSFTDlg)
	ON_BN_CLICKED(IDC_DGN_CON_JSFT_AR, OnDgnJSFTAr)
	ON_BN_CLICKED(IDC_DGN_CON_JSFT_DEL, OnDgnJSFTDel)
	ON_BN_CLICKED(IDC_DGN_CON_JSFT_CAL_PROGRAM, OnDgnJSFTcalProgram)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConJSFTDlg message handlers

void CDgnConJSFTDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConJSFTDlg::OnDgnExecute() 
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

		T_JSFT_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			UpdateData(TRUE);
			rData.bAuto = m_bAuto;
			rData.dJackFactor = m_dJackFactor;
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddJsft(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelJsft(rSelKey))	Initial_SelectItem();
		}
		
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnConJSFTDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnConJSFTDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConJSFTDlg::Initial_Data()
{
	Update_InitDataByCode();

	m_nOption	= 0;      // add/replace
	m_bAuto = TRUE;  // Automeation
	m_dJackFactor = 0.;
	GetDlgItem(IDC_DGN_CON_JSFT_FACTOR)->EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CDgnConJSFTDlg::OnDgnJSFTAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConJSFTDlg::OnDgnJSFTDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConJSFTDlg::OnDgnJSFTcalProgram() 
{
	UpdateData(TRUE);

	if(m_bAuto)   // Automatic Calculation
	{
		m_dJackFactor = 0.;
	  GetDlgItem(IDC_DGN_CON_JSFT_FACTOR)->EnableWindow(FALSE);
	}
	else          // Not Automatic Calculation
	{
		GetDlgItem(IDC_DGN_CON_JSFT_FACTOR)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDgnConJSFTDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnConJSFTDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCon(ID_DGN_CON_JSFT, strTitle, bShow, m_pDoc->IsPostMode());
	if(bShow)
	{
		GetDlgItem(IDC_DGN_CON_JSFT_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_JSFT_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_JSFT_FACTOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_JSFT_CAL_PROGRAM)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_JSFT_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_JSFT_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_JSFT_FACTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_JSFT_CAL_PROGRAM)->EnableWindow(FALSE);
	}

}
