// CPGUnbraLengDlg.cpp: implementation of the CCPGUnbraLengDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGUnbraLengDlg.h"
#include "CPGUnbraLengTabIDlg.h"
#include "CPGUnbraLengTabJDlg.h"

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


CCPGUnbraLengDlg::CCPGUnbraLengDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CCPGUnbraLengDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGTranStifDlg)
	m_nOption = 0;
	m_bBothIJ = TRUE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();	

	m_I_Data.Initialize();
	m_J_Data.Initialize();

	m_bJTabEnable = FALSE;

	m_Data.Initialize();
}

void CCPGUnbraLengDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CCPGUnbraLengDlg)
	DDX_Radio  (pDX, IDC_DGN_CPG_OPT_ADD_RDO,      m_nOption);
	DDX_Check  (pDX, IDC_DGN_CPG_CPUL_IJBOTH_CHK,  m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_TAB,         m_Tab);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_IJBOTH_CHK,  m_Ctrl_BothIJ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGUnbraLengDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CCPGUnbraLengDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_CPUL_CLOSE,       OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_CPG_CPUL_APPLY,       OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO,      OnDgnCPGLoadAppAdd)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO,      OnDgnCPGLoadAppDel)
	ON_BN_CLICKED(IDC_DGN_CPG_CPUL_IJBOTH_CHK,  OnBothIJChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGUnbraLengDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	m_Tab.DeleteAllItems();

	// I
	m_pSubDlg_I = new CCPGUnbraLengTabIDlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGUnbraLengTabIDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CCPGUnbraLengTabJDlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGUnbraLengTabJDlg::IDD, TRUE);		

	m_Tab.ShowTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPGUnbraLengDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CCPGUnbraLengDlg::OnDgnExecute() 
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
			if(m_pDoc->m_pDataCtrl->AddCpul(aSelKey, m_Data))	Initial_SelectItem();
		}
	  else	// Delete
		{
	  	if(m_pDoc->m_pDataCtrl->DelCpul(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_CPG_BEAM_NON_MEMBER));
}

void CCPGUnbraLengDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGUnbraLengDlg::OnBothIJChk() 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->SetAllControlEnable(m_bJTabEnable);		
}

BOOL CCPGUnbraLengDlg::Dlg2Data()
{
	m_Data.bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if(m_Tab.GetCurSel() == 0)
	{
		if(m_Data.bBothIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetCpulData();
			m_Data.TfType[0] = m_I_Data;
			m_Data.TfType[1] = m_Data.TfType[0];			
		}
		else
		{
			m_pSubDlg_I->GetCpulData();
			m_Data.TfType[0] = m_I_Data;
			m_Data.TfType[1] = m_J_Data;
		}
	}
	else
	{
		if(m_Data.bBothIJ)
		{
			m_Data.TfType[0] = m_I_Data;
			m_Data.TfType[1] = m_Data.TfType[0];
		}
		else
		{
			m_Data.TfType[0] = m_I_Data;
			m_pSubDlg_J->GetCpulData();
			m_Data.TfType[1] = m_J_Data;
		}
	}	

	return TRUE;
}

void CCPGUnbraLengDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;
}

void CCPGUnbraLengDlg::OnDgnCPGLoadAppAdd() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CCPGUnbraLengDlg::OnDgnCPGLoadAppDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}
