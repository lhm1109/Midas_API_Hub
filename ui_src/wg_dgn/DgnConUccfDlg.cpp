// DgnConUCCFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConUCCFDlg.h"

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
// CDgnConUCCFDlg dialog


CDgnConUCCFDlg::CDgnConUCCFDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConUCCFDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConUCCFDlg)
	m_nOption = -1;
	m_dGammard = 1.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

}


void CDgnConUCCFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConUCCFDlg)
	DDX_Radio(pDX, IDC_DGN_CON_UCCF_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CON_UCCF_FACTOR, m_dGammard);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConUCCFDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConUCCFDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_UCCF_AR, OnDgnUCCFAr)
	ON_BN_CLICKED(IDC_DGN_CON_UCCF_DEL, OnDgnUCCFDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConUCCFDlg message handlers
void CDgnConUCCFDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConUCCFDlg::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	T_ELEM_D ElemD;
	T_MATL_D MatlD;

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);      
			if(!m_pDoc->m_pAttrCtrl->GetElem(key, ElemD))         { ASSERT(0); ElemD.Initialize(); }
			if(!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) { ASSERT(0); MatlD.Initialize(); }      
			// Concrete Member.
			if(MatlD.Type==_T("C")) rSelKey.Add(key);
		}

		T_UCCF_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			UpdateData(TRUE);
			rData.dGammard = m_dGammard;
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddUccf(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_MATL_TYPE_CON),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelUccf(rSelKey))	Initial_SelectItem();
		}
		
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnConUCCFDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnConUCCFDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConUCCFDlg::Initial_Data()
{
	m_nOption	= 0;      // add/replace
	m_dGammard = 1.0;

	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnConUCCFDlg::OnDgnUCCFAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConUCCFDlg::OnDgnUCCFDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConUCCFDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnConUCCFDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCon(ID_DGN_CON_UCCF, strTitle, bShow, m_pDoc->IsPostMode());
	if(bShow)
	{
		GetDlgItem(IDC_DGN_CON_UCCF_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_UCCF_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_UCCF_FACTOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_UCCF_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_UCCF_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_UCCF_FACTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}
}
