// DgnPscCrackWidthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnPscCrackWidthDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnPscCrackWidthDlg dialog


CDgnPscCrackWidthDlg::CDgnPscCrackWidthDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnPscCrackWidthDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnPscCrackWidthDlg)
	m_nOption = -1;
	m_dC1 = 0.0;
	m_dC2 = 0.0;
	m_dC3 = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnPscCrackWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnPscCrackWidthDlg)
	DDX_Radio(pDX, IDC_DGN_PSC_CRACK_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_PSC_CRACK_C1, m_dC1);
	DDX_Text(pDX, IDC_DGN_PSC_CRACK_C2, m_dC2);
	DDX_Text(pDX, IDC_DGN_PSC_CRACK_C3, m_dC3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnPscCrackWidthDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnPscCrackWidthDlg)
	ON_BN_CLICKED(IDC_DGN_PSC_CRACK_AR, OnDgnCrackAr)
	ON_BN_CLICKED(IDC_DGN_PSC_CRACK_DEL, OnDgnCrackDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnPscCrackWidthDlg message handlers

void CDgnPscCrackWidthDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnPscCrackWidthDlg::OnDgnExecute() 
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
			// Beam.
			if(m_pDoc->m_pAttrCtrl->IsBeam(eData.eltyp))	rSelKey.Add(key);
		}
		
		T_PSCW_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.dC1 = m_dC1;
			rData.dC2 = m_dC2;
			rData.dC3 = m_dC3;
			if(rSelKey.GetSize() > 0)
			{
		    // Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddPscw(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	//if(m_pDoc->m_pDataCtrl->DelPscw(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}


void CDgnPscCrackWidthDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}


BOOL CDgnPscCrackWidthDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Default 값 초기화
	Initial_Data();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnPscCrackWidthDlg::Initial_Data()
{
	m_nOption = 0;				//	add/replace
	m_dC1 = 1.0;	        	
	m_dC2 = 1.0;
	m_dC3 = 1.0;

	UpdateData(FALSE);
}

BOOL CDgnPscCrackWidthDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_dC1< 1.0)	bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_CTC_FACTOR),MB_OK);

	return bCheck;
}

void CDgnPscCrackWidthDlg::OnDgnCrackAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnPscCrackWidthDlg::OnDgnCrackDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}
