// CPGResidualStressDlg.cpp: implementation of the CCPGResidualStressDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGResidualStressDlg.h"
#include "CPGResidualStressTabDlg.h"
#include "CPGResidualStressTabJDlg.h"

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


CCPGResidualStressDlg::CCPGResidualStressDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CCPGResidualStressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGResidualStressDlg)
	m_nOption = 0;
	m_bBothIJ = TRUE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_I_Data.Initialize();
	m_J_Data.Initialize();

	m_bJTabEnable = FALSE;

	m_Data.Initialize();
}


void CCPGResidualStressDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CCPGResidualStressDlg)
	DDX_Radio  (pDX, IDC_DGN_CPG_OPT_ADD_RDO,                m_nOption);
	DDX_Check  (pDX, IDC_DGN_CPG_RESIDUAl_STRESS_IJBOTH_CHK, m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_CPG_RESIDUAl_STRESS_TAB,        m_Tab);
	DDX_Control(pDX, IDC_DGN_CPG_RESIDUAl_STRESS_IJBOTH_CHK, m_Ctrl_BothIJ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGResidualStressDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CCPGResidualStressDlg)
	//ON_BN_CLICKED(IDC_DGN_CPG_RESIDUAl_STRESS_CLOSE,      OnDgnClose)
	//ON_BN_CLICKED(IDC_DGN_CPG_RESIDUAl_STRESS_APPLY,      OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO,                OnDgnCPGShearAdd)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO,                OnDgnCPGShearDel)
	ON_BN_CLICKED(IDC_DGN_CPG_RESIDUAl_STRESS_IJBOTH_CHK, OnBothIJChk)

	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_CPG_RESIDUAl_STRESS_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGResidualStressDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	m_Tab.DeleteAllItems();

	// I
	m_pSubDlg_I = new CCPGResidualStressTabDlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGResidualStressTabDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CCPGResidualStressTabJDlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGResidualStressTabJDlg::IDD, TRUE);

	m_Tab.ShowTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPGResidualStressDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CCPGResidualStressDlg::OnDgnExecute() 
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
		   if(m_pDoc->m_pDataCtrl->AddCrst(aSelKey, m_Data))
			   Initial_SelectItem();
		}
	    else	// Delete
		{
	  	  if(m_pDoc->m_pDataCtrl->DelCrst(aSelKey))	
			  Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_CPG_BEAM_NON_MEMBER));
}

void CCPGResidualStressDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGResidualStressDlg::OnBothIJChk() 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->SetAllControlEnable(m_bJTabEnable);		
}

BOOL CCPGResidualStressDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if(m_Tab.GetCurSel() == 0)
	{
		if(m_Data.bBothIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetCgscData();
			m_Data.CrstBase[0] = m_I_Data;
			m_Data.CrstBase[1] = m_Data.CrstBase[0];
		}
		else
		{
			m_pSubDlg_I->GetCgscData();
			m_Data.CrstBase[0] = m_I_Data;
			m_Data.CrstBase[1] = m_J_Data;
		}
	}
	else
	{
		if(m_Data.bBothIJ)
		{
			m_Data.CrstBase[0] = m_I_Data;
			m_Data.CrstBase[1] = m_Data.CrstBase[0];
		}
		else
		{
			m_Data.CrstBase[0] = m_I_Data;
			m_pSubDlg_J->GetCgscData();
			m_Data.CrstBase[1] = m_J_Data;
		}
	}	

	return TRUE;
}

void CCPGResidualStressDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if (m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->SetAllControlEnable(m_bJTabEnable);

}

void CCPGResidualStressDlg::OnDgnCPGShearAdd() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CCPGResidualStressDlg::OnDgnCPGShearDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}
