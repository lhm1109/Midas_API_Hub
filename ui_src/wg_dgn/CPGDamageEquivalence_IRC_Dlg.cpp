// CPGDamageEquivalence_IRC_Dlg.cpp: implementation of the CCPGDamageEquivalence_IRC_Dlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGDamageEquivalence_IRC_Dlg.h"
#include "CPGDamageEquivalenceTab_IRC_Dlg.h"
#include "CPGDamageEquivalenceTabJ_IRC_Dlg.h"

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


CCPGDamageEquivalence_IRC_Dlg::CCPGDamageEquivalence_IRC_Dlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CCPGDamageEquivalence_IRC_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGLateraTorsionDlg)
	m_nOption = 0;
	m_bBothIJ = TRUE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_I_Data.Initialize();
	m_J_Data.Initialize();

	m_bJTabEnable = FALSE;

	m_Data.Initialize();  
}

void CCPGDamageEquivalence_IRC_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CCPGDamageEquivalence_IRC_Dlg)
	DDX_Radio  (pDX, IDC_DGN_CPG_OPT_ADD_RDO,      m_nOption);
	DDX_Check  (pDX, IDC_DGN_CPG_IJBOTH_CHK,       m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_CPG_TAB,              m_Tab);
	DDX_Control(pDX, IDC_DGN_CPG_IJBOTH_CHK,       m_Ctrl_BothIJ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGDamageEquivalence_IRC_Dlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CCPGDamageEquivalence_IRC_Dlg)
// 	ON_BN_CLICKED(IDC_DGN_CPG_CLOSE,            OnDgnClose)
// 	ON_BN_CLICKED(IDC_DGN_CPG_APPLY,            OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO,      OnDgnCPGDamageAdd)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO,      OnDgnCPGDamageDel)
	ON_BN_CLICKED(IDC_DGN_CPG_IJBOTH_CHK,       OnBothIJChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGDamageEquivalence_IRC_Dlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	m_Tab.DeleteAllItems();

	// I
	m_pSubDlg_I = new CCPGDamageEquivalenceTab_IRC_Dlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGDamageEquivalenceTab_IRC_Dlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CCPGDamageEquivalenceTabJ_IRC_Dlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGDamageEquivalenceTabJ_IRC_Dlg::IDD, TRUE);		

	m_Tab.ShowTab(0);

	GetDlgItem(IDC_DGN_CPG_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CPG_APPLY)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// void CCPGDamageEquivalence_IRC_Dlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CCPGDamageEquivalence_IRC_Dlg::Execute()
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
			if(m_pDoc->m_pDataCtrl->AddCgfr(aSelKey, m_Data))	Initial_SelectItem();
		}
	  else	// Delete
		{
	  	if(m_pDoc->m_pDataCtrl->DelCgfr(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_CPG_BEAM_NON_MEMBER));
}

void CCPGDamageEquivalence_IRC_Dlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGDamageEquivalence_IRC_Dlg::OnBothIJChk() 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->SetAllControlEnable(m_bJTabEnable);		
}

BOOL CCPGDamageEquivalence_IRC_Dlg::Dlg2Data()
{
	m_Data.bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if(m_Tab.GetCurSel() == 0)
	{
		if(m_Data.bBothIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetCgfrData();
			m_Data.FRdata[0] = m_I_Data;
			m_Data.FRdata[1] = m_Data.FRdata[0];			
		}
		else
		{
			m_pSubDlg_I->GetCgfrData();
			m_Data.FRdata[0] = m_I_Data;
			m_Data.FRdata[1] = m_J_Data;
		}
	}
	else
	{
		if(m_Data.bBothIJ)
		{
			m_Data.FRdata[0] = m_I_Data;
			m_Data.FRdata[1] = m_Data.FRdata[0];
		}
		else
		{
			m_Data.FRdata[0] = m_I_Data;
			m_pSubDlg_J->GetCgfrData();
			m_Data.FRdata[1] = m_J_Data;
		}
	}	

	return TRUE;
}

void CCPGDamageEquivalence_IRC_Dlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;
}

void CCPGDamageEquivalence_IRC_Dlg::OnDgnCPGDamageAdd() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CCPGDamageEquivalence_IRC_Dlg::OnDgnCPGDamageDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}
