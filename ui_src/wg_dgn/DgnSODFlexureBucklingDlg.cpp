// SODPosiDgnOutDlg.cpp: implementation of the CDgnSODFlexureBucklingDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSODFlexureBucklingDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CDgnSODFlexureBucklingDlg::CDgnSODFlexureBucklingDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnSODFlexureBucklingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSODFlexureBucklingDlg)
	m_nOption = 0;
	m_nMembType = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}


void CDgnSODFlexureBucklingDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDgnSODFlexureBucklingDlg)
	DDX_Radio(pDX, IDC_DGN_SOD_OPT_ADD_RDO,			m_nOption);
	DDX_Radio(pDX, IDC_DGN_SOD_FLEX_BUCK_TYPE1_RDO, m_nMembType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSODFlexureBucklingDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnSODFlexureBucklingDlg)
// 	ON_BN_CLICKED(IDC_DGN_SOD_POSI_DGN_CLOSE_BTN,  OnDgnClose)
// 	ON_BN_CLICKED(IDC_DGN_SOD_POSI_DGN_APPLY_BTN,  OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_SOD_OPT_ADD_RDO,         OnDgnSODPosiDgnAdd)
	ON_BN_CLICKED(IDC_DGN_SOD_OPT_DEL_RDO,         OnDgnSODPosiDgnDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnSODFlexureBucklingDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	GetDlgItem(IDC_DGN_SOD_POSI_DGN_CLOSE_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SOD_POSI_DGN_APPLY_BTN)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// void CDgnSODFlexureBucklingDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnSODFlexureBucklingDlg::Execute()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		if(m_nOption==0)	// add/replace
		{
			Dlg2Data();
			if(m_pDoc->m_pDataCtrl->AddFlbk(aSelKey, m_Data))	Initial_SelectItem();
		}
		else	// Delete
		{
			if(m_pDoc->m_pDataCtrl->DelFlbk(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_SOD_BEAM_NON_MEMBER));
}

void CDgnSODFlexureBucklingDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

BOOL CDgnSODFlexureBucklingDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.nMembType = m_nMembType;
	return TRUE;
}

void CDgnSODFlexureBucklingDlg::OnDgnSODPosiDgnAdd() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnSODFlexureBucklingDlg::OnDgnSODPosiDgnDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}
