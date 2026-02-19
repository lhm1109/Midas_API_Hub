// SODPosiDgnOutDlg.cpp: implementation of the CSODPosiDgnOutDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "SODPosiDgnOutDlg.h"

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


CSODPosiDgnOutDlg::CSODPosiDgnOutDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSODPosiDgnOutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSODPosiDgnOutDlg)
	m_nOption = 0;
	m_iPosi = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}


void CSODPosiDgnOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSODPosiDgnOutDlg)
	DDX_Radio(pDX, IDC_DGN_SOD_OPT_ADD_RDO,    m_nOption);
	DDX_Radio(pDX, IDC_DGN_SOD_POSI_DGN_I_RDO, m_iPosi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSODPosiDgnOutDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSODPosiDgnOutDlg)
// 	ON_BN_CLICKED(IDC_DGN_SOD_POSI_DGN_CLOSE_BTN,  OnDgnClose)
// 	ON_BN_CLICKED(IDC_DGN_SOD_POSI_DGN_APPLY_BTN,  OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_SOD_OPT_ADD_RDO,         OnDgnSODPosiDgnAdd)
	ON_BN_CLICKED(IDC_DGN_SOD_OPT_DEL_RDO,         OnDgnSODPosiDgnDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CSODPosiDgnOutDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	GetDlgItem(IDC_DGN_SOD_POSI_DGN_CLOSE_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SOD_POSI_DGN_APPLY_BTN)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// void CSODPosiDgnOutDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CSODPosiDgnOutDlg::Execute()
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
			if(m_pDoc->m_pDataCtrl->AddSddo(aSelKey, m_Data))	Initial_SelectItem();
		}
	  else	// Delete
		{
	  	if(m_pDoc->m_pDataCtrl->DelSddo(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_SOD_BEAM_NON_MEMBER));
}

void CSODPosiDgnOutDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

BOOL CSODPosiDgnOutDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.iDgnPosi = m_iPosi;
	return TRUE;
}

void CSODPosiDgnOutDlg::OnDgnSODPosiDgnAdd() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CSODPosiDgnOutDlg::OnDgnSODPosiDgnDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}
