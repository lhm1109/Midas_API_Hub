// CMRebarPscMoment.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionTorsionBeamRebar.h"

#include "ConcSectionBeamRebarViewDlg.h"
#include "ConcSectionCreateTorsionBeamRebar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ConcSectionMainRebar dialog


CConcSectionTorsionBeamRebar::CConcSectionTorsionBeamRebar(CWnd* pParent /*=NULL*/)
	: CChildDialog(CConcSectionTorsionBeamRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcSectionTorsionBeamRebar)
	//}}AFX_DATA_INIT
	ASSERT(pParent);
	m_pParent = (CConcSectionBeamRebarViewDlg*)pParent;

	m_pTorsionRebarI = NULL;
	m_pTorsionRebarM = NULL;
	m_pTorsionRebarJ = NULL;

	m_DataI.Initialize();
	m_DataM.Initialize();
	m_DataJ.Initialize();
}

void CConcSectionTorsionBeamRebar::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionTorsionBeamRebar)
	DDX_Control(pDX, IDC_DGN_CON_TORSION_REBAR_TAB,    m_ctrlTab);
	DDX_Check  (pDX, IDC_DGN_CON_TORSION_REBAR_IJBOTH, m_bSameIMJ);	
	//}}AFX_DATA_MAP
}


void CConcSectionTorsionBeamRebar::Data2Dlg()
{
	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pTorsionRebarI->m_TorbrData = m_DataI;
		m_pTorsionRebarI->Data2Dlg();
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pTorsionRebarM->m_TorbrData = m_DataM;
		m_pTorsionRebarM->Data2Dlg();
	}
	else
	{
		m_pTorsionRebarJ->m_TorbrData = m_DataJ;
		m_pTorsionRebarJ->Data2Dlg();
	}
	
	UpdateData(FALSE);	
}

BOOL CConcSectionTorsionBeamRebar::Dlg2Data()
{
	UpdateData(TRUE);
	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pTorsionRebarI->Dlg2Data();
		m_DataI = m_pTorsionRebarI->m_TorbrData;
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pTorsionRebarM->Dlg2Data();
		m_DataM = m_pTorsionRebarM->m_TorbrData;
	}
	else
	{
		m_pTorsionRebarJ->Dlg2Data();
		m_DataJ = m_pTorsionRebarJ->m_TorbrData;
	}
	
	if(m_bSameIMJ)
	{
		m_DataM = m_DataI;
		m_DataJ = m_DataI;
	}
	
	m_pParent->m_TorsionRebarDataI = m_DataI;
	m_pParent->m_TorsionRebarDataM = m_DataM;
	m_pParent->m_TorsionRebarDataJ = m_DataJ;

	return TRUE;
}

int CConcSectionTorsionBeamRebar::GetCurSel()
{
	return m_ctrlTab.GetCurSel();
}


BEGIN_MESSAGE_MAP(CConcSectionTorsionBeamRebar, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionTorsionBeamRebar)
	ON_BN_CLICKED(IDC_DGN_CON_TORSION_REBAR_IJBOTH, OnCmdSameCheck)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_CON_TORSION_REBAR_TAB, OnSelchangeingCmdTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_CON_TORSION_REBAR_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionTorsionBeamRebar message handlers

BOOL CConcSectionTorsionBeamRebar::OnInitDialog() 
{
	CChildDialog::OnInitDialog();	
	
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_TORSIONREBAR_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	// TabWinddow Create & Setting 
	m_pTorsionRebarI = new CConcSectionCreateTorsionBeamRebar(this, 0);
	m_ctrlTab.AddTab(m_pTorsionRebarI, _T("I"), CConcSectionCreateTorsionBeamRebar::IDD, TRUE);
	
	m_pTorsionRebarM = new CConcSectionCreateTorsionBeamRebar(this, 1);
	m_ctrlTab.AddTab(m_pTorsionRebarM, _T("M"), CConcSectionCreateTorsionBeamRebar::IDD, TRUE);
	
	m_pTorsionRebarJ = new CConcSectionCreateTorsionBeamRebar(this, 2);
	m_ctrlTab.AddTab(m_pTorsionRebarJ, _T("J"), CConcSectionCreateTorsionBeamRebar::IDD, TRUE);
	
	m_ctrlTab.ShowTab(0);

	m_bSameIMJ = m_pParent->m_bTorsionSameIMJ;
	m_DataI = m_pParent->m_TorsionRebarDataI;
	m_DataM = m_pParent->m_TorsionRebarDataM;
	m_DataJ = m_pParent->m_TorsionRebarDataJ;

	Data2Dlg();

	// 20110520. by JhKim. Skin tab 보이지 않는 문제 수정
	m_ctrlTab.SendMessage(WM_PAINT, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConcSectionTorsionBeamRebar::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_bTorsionSameIMJ = m_bSameIMJ;
	m_pParent->m_TorsionRebarDataI = m_DataI;
	m_pParent->m_TorsionRebarDataM = m_DataM;
	m_pParent->m_TorsionRebarDataJ = m_DataJ;

	return CChildDialog::DestroyWindow();
}

void CConcSectionTorsionBeamRebar::OnCmdSameCheck() 
{
	UpdateData(TRUE);

	Dlg2Data();
	m_DataM = m_DataI;
	m_DataJ = m_DataI;
	Data2Dlg();

	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pTorsionRebarI->EnableDisableControls(m_bSameIMJ);
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pTorsionRebarM->EnableDisableControls(m_bSameIMJ);
	}
	else
	{
		m_pTorsionRebarJ->EnableDisableControls(m_bSameIMJ);
	}
}

void CConcSectionTorsionBeamRebar::OnSelchangeingCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	Dlg2Data();
}

void CConcSectionTorsionBeamRebar::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pParent->DrawSectionRebarTabChanged();
}