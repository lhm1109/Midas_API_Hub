// CMRebarPscMoment.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionShearBeamRebar.h"

#include "ConcSectionBeamRebarViewDlg.h"
#include "ConcSectionCreateShearBeamRebar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ConcSectionMainRebar dialog


CConcSectionShearBeamRebar::CConcSectionShearBeamRebar(CWnd* pParent /*=NULL*/)
	: CChildDialog(CConcSectionShearBeamRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcSectionShearBeamRebar)
	//}}AFX_DATA_INIT
	ASSERT(pParent);
	m_pParent = (CConcSectionBeamRebarViewDlg*)pParent;

	m_pShearRebarI = NULL;
	m_pShearRebarM = NULL;
	m_pShearRebarJ = NULL;

	m_DataI.Initialize();
	m_DataM.Initialize();
	m_DataJ.Initialize();
}

void CConcSectionShearBeamRebar::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionShearBeamRebar)
	DDX_Control(pDX, IDC_DGN_CON_SHEAR_REBAR_TAB,    m_ctrlTab);
	DDX_Check  (pDX, IDC_DGN_CON_SHEAR_REBAR_IJBOTH, m_bSameIMJ);	
	//}}AFX_DATA_MAP
}


void CConcSectionShearBeamRebar::Data2Dlg()
{
	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pShearRebarI->m_ShbrData = m_DataI;
		m_pShearRebarI->Data2Dlg();
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pShearRebarM->m_ShbrData = m_DataM;
		m_pShearRebarM->Data2Dlg();
	}
	else
	{
		m_pShearRebarJ->m_ShbrData = m_DataJ;
		m_pShearRebarJ->Data2Dlg();
	}
	
	UpdateData(FALSE);	
}

BOOL CConcSectionShearBeamRebar::Dlg2Data()
{
	UpdateData(TRUE);
	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pShearRebarI->Dlg2Data();
		m_DataI = m_pShearRebarI->m_ShbrData;
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pShearRebarM->Dlg2Data();
		m_DataM = m_pShearRebarM->m_ShbrData;
	}
	else
	{
		m_pShearRebarJ->Dlg2Data();
		m_DataJ = m_pShearRebarJ->m_ShbrData;
	}

	if(m_bSameIMJ)
	{
		m_DataM = m_DataI;
		m_DataJ = m_DataI;
	}

	m_pParent->m_ShearRebarDataI = m_DataI;
	m_pParent->m_ShearRebarDataM = m_DataM;
	m_pParent->m_ShearRebarDataJ = m_DataJ;
	
	return TRUE;
}

int CConcSectionShearBeamRebar::GetCurSel()
{
	return m_ctrlTab.GetCurSel();
}


BEGIN_MESSAGE_MAP(CConcSectionShearBeamRebar, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionShearBeamRebar)
	ON_BN_CLICKED(IDC_DGN_CON_SHEAR_REBAR_IJBOTH, OnCmdSameCheck)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_CON_SHEAR_REBAR_TAB, OnSelchangeingCmdTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_CON_SHEAR_REBAR_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionShearBeamRebar message handlers

BOOL CConcSectionShearBeamRebar::OnInitDialog() 
{
	CChildDialog::OnInitDialog();	
	
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SHEARREBAR_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	// TabWinddow Create & Setting 
	m_pShearRebarI = new CConcSectionCreateShearBeamRebar(this, 0);
	m_ctrlTab.AddTab(m_pShearRebarI, _T("I"), CConcSectionCreateShearBeamRebar::IDD, TRUE);
	
	m_pShearRebarM = new CConcSectionCreateShearBeamRebar(this, 1);
	m_ctrlTab.AddTab(m_pShearRebarM, _T("M"), CConcSectionCreateShearBeamRebar::IDD, TRUE);
	
	m_pShearRebarJ = new CConcSectionCreateShearBeamRebar(this, 2);
	m_ctrlTab.AddTab(m_pShearRebarJ, _T("J"), CConcSectionCreateShearBeamRebar::IDD, TRUE);
	
	m_ctrlTab.ShowTab(0);

	m_bSameIMJ = m_pParent->m_bShearSameIMJ;
	m_DataI = m_pParent->m_ShearRebarDataI;
	m_DataM = m_pParent->m_ShearRebarDataM;
	m_DataJ = m_pParent->m_ShearRebarDataJ;

	Data2Dlg();

	// 20110520. by JhKim. Skin tab 보이지 않는 문제 수정
	m_ctrlTab.SendMessage(WM_PAINT, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConcSectionShearBeamRebar::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_bShearSameIMJ = m_bSameIMJ;
	m_pParent->m_ShearRebarDataI = m_DataI;
	m_pParent->m_ShearRebarDataM = m_DataM;
	m_pParent->m_ShearRebarDataJ = m_DataJ;

	return CChildDialog::DestroyWindow();
}

void CConcSectionShearBeamRebar::OnCmdSameCheck() 
{
	UpdateData(TRUE);

	Dlg2Data();
	m_DataM = m_DataI;
	m_DataJ = m_DataI;
	Data2Dlg();
	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pShearRebarI->EnableDisableControls(m_bSameIMJ);
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pShearRebarM->EnableDisableControls(m_bSameIMJ);
	}
	else
	{
		m_pShearRebarJ->EnableDisableControls(m_bSameIMJ);
	}
}

void CConcSectionShearBeamRebar::OnSelchangeingCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	Dlg2Data();
}

void CConcSectionShearBeamRebar::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pParent->DrawSectionRebarTabChanged();
}