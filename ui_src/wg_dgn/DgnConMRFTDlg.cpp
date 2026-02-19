// DgnConMRFTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConMRFTDlg.h"

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
// CDgnConMRFTDlg dialog


CDgnConMRFTDlg::CDgnConMRFTDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConMRFTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMRFTDlg)
	m_nOption = -1;
	m_dMrftFactor = 1.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

}


void CDgnConMRFTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMRFTDlg)
	DDX_Radio(pDX, IDC_DGN_CON_MRFT_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CON_MRFT_FACTOR, m_dMrftFactor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConMRFTDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConMRFTDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_MRFT_AR, OnDgnMRFTAr)
	ON_BN_CLICKED(IDC_DGN_CON_MRFT_DEL, OnDgnMRFTDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMRFTDlg message handlers
void CDgnConMRFTDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConMRFTDlg::OnDgnExecute() 
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
			// Beam
			if(m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_BEAM) rSelKey.Add(key);
		}

		T_MRFT_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			UpdateData(TRUE);
			rData.dFactor = m_dMrftFactor;
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddMrft(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_BEAM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelMrft(rSelKey))	Initial_SelectItem();
		}
		
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnConMRFTDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnConMRFTDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConMRFTDlg::Initial_Data()
{
	m_nOption	= 0;      // add/replace
	m_dMrftFactor = 1.0;

	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnConMRFTDlg::OnDgnMRFTAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConMRFTDlg::OnDgnMRFTDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConMRFTDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CDgnConMRFTDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCon(ID_DGN_CON_MRFT, strTitle, bShow, m_pDoc->IsPostMode());
	if(bShow)
	{
		GetDlgItem(IDC_DGN_CON_MRFT_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_MRFT_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_MRFT_FACTOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_MRFT_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_MRFT_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_MRFT_FACTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}
}
