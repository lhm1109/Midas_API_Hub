// CMRebarPscMoment.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionMainBeamRebar.h"

#include "ConcSectionBeamRebarViewDlg.h"
#include "ConcSectionCreateBeamRebar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ConcSectionMainBeamRebar dialog

CConcSectionMainBeamRebar::CConcSectionMainBeamRebar(CWnd* pParent /*=NULL*/)
	: CChildDialog(CConcSectionMainBeamRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcSectionMainBeamRebar)
	//}}AFX_DATA_INIT
	ASSERT(pParent);
	m_pParent = (CConcSectionBeamRebarViewDlg*)pParent;

	m_pMainRebarI = NULL;
	m_pMainRebarM = NULL;
	m_pMainRebarJ = NULL;

	m_bSameIMJ = TRUE;
}

void CConcSectionMainBeamRebar::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionMainBeamRebar)
	DDX_Control(pDX, IDC_DGN_CON_MAIN_REBAR_TAB,    m_ctrlTab);
	DDX_Control(pDX, IDC_DGN_CON_MAIN_REBAR_IJBOTH, m_chkSameIJ);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConcSectionMainBeamRebar, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionMainBeamRebar)
	ON_BN_CLICKED(IDC_DGN_CON_MAIN_REBAR_IJBOTH, OnCmdSameCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_CON_MAIN_REBAR_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionMainBeamRebar message handlers

BOOL CConcSectionMainBeamRebar::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_MAINREBAR_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	m_pMainRebarI = new CConcSectionCreateBeamRebar(this, 0);
	m_pMainRebarI->SetDlgViewPanel(this->m_pIDVP);
	m_ctrlTab.AddTab(m_pMainRebarI, _T("I"), CConcSectionCreateBeamRebar::IDD, TRUE);
	
	m_pMainRebarM = new CConcSectionCreateBeamRebar(this, 1);
	m_pMainRebarM->SetDlgViewPanel(this->m_pIDVP);
	m_ctrlTab.AddTab(m_pMainRebarM, _T("M"), CConcSectionCreateBeamRebar::IDD, TRUE);
	
	m_pMainRebarJ = new CConcSectionCreateBeamRebar(this, 2);
	m_pMainRebarJ->SetDlgViewPanel(this->m_pIDVP);
	m_ctrlTab.AddTab(m_pMainRebarJ, _T("J"), CConcSectionCreateBeamRebar::IDD, TRUE);
	
	m_ctrlTab.ShowTab(0);

	//Data2Dlg();  
	LoadDlgData(m_pParent->m_bMainSameIMJ);                       

	// 20110520. by JhKim. Skin tab 보이지 않는 문제 수정
	m_ctrlTab.SendMessage(WM_PAINT, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConcSectionMainBeamRebar::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_bMainSameIMJ = m_bSameIMJ;
	  
	return CChildDialog::DestroyWindow();
}

void CConcSectionMainBeamRebar::OnCmdSameCheck() 
{
	m_bSameIMJ = (m_chkSameIJ.GetCheck() == 0) ? FALSE : TRUE;

	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pMainRebarI->EnableDisableControls(m_bSameIMJ);
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pMainRebarM->EnableDisableControls(m_bSameIMJ);
	}
	else
	{
		m_pMainRebarJ->EnableDisableControls(m_bSameIMJ);
	}
}

void CConcSectionMainBeamRebar::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pParent->DrawSectionRebarTabChanged();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CConcSectionMainBeamRebar::Data2Dlg()
{
	m_chkSameIJ.SetCheck(m_bSameIMJ);
}

BOOL CConcSectionMainBeamRebar::Dlg2Data()
{
	m_bSameIMJ = (m_chkSameIJ.GetCheck() == 0) ? FALSE : TRUE;
		
	return TRUE;
}

void CConcSectionMainBeamRebar::SetDlgViewPanel(I_DlgViewPanel* pIDVP)
{
	this->m_pIDVP = pIDVP;
	//m_pMainRebarI->SetDlgViewPanel(this->m_pIDVP);
	//m_pMainRebarJ->SetDlgViewPanel(this->m_pIDVP);
}

void CConcSectionMainBeamRebar::AddRebarData(REBAR_KEY RebarKey, T_RBGC_MABR MaReberData, int nPosition)
{
	m_pParent->AddRebarData(RebarKey, MaReberData, nPosition);		
}

void CConcSectionMainBeamRebar::DeleteRebarData(REBAR_KEY RebarKey, int nPosition)
{
	m_pParent->DeleteRebarData(RebarKey, nPosition);
}

int CConcSectionMainBeamRebar::GetCurSel()
{
	return m_ctrlTab.GetCurSel();
}

void CConcSectionMainBeamRebar::RemoveMainRebarDataListI(REBAR_KEY Key, T_RBGC_MABR Data)
{
	m_pMainRebarI->RemoveMainRebarDataList(Key, Data);
}

void CConcSectionMainBeamRebar::RemoveMainRebarDataListM(REBAR_KEY Key, T_RBGC_MABR Data)
{
	m_pMainRebarM->RemoveMainRebarDataList(Key, Data);
}

void CConcSectionMainBeamRebar::RemoveMainRebarDataListJ(REBAR_KEY Key, T_RBGC_MABR Data)
{
	m_pMainRebarJ->RemoveMainRebarDataList(Key, Data);
}

void CConcSectionMainBeamRebar::SetInitDataI(CArray<T_RBGC_MABR,T_RBGC_MABR&>& MainRebarDataList)
{
	m_pMainRebarI->SetInitData(MainRebarDataList);	
}

void CConcSectionMainBeamRebar::SetInitDataM(CArray<T_RBGC_MABR,T_RBGC_MABR&>& MainRebarDataList)
{
	m_pMainRebarM->SetInitData(MainRebarDataList);	
}

void CConcSectionMainBeamRebar::SetInitDataJ(CArray<T_RBGC_MABR,T_RBGC_MABR&>& MainRebarDataList)
{
	m_pMainRebarJ->SetInitData(MainRebarDataList);	
}

void CConcSectionMainBeamRebar::SetRebarListDataI(CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR>& m_MabrDataList)
{
	m_pMainRebarI->SetRebarListData(m_MabrDataList);	
}

void CConcSectionMainBeamRebar::SetRebarListDataM(CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR>& m_MabrDataList)
{
	m_pMainRebarM->SetRebarListData(m_MabrDataList);	
}

void CConcSectionMainBeamRebar::SetRebarListDataJ(CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR>& m_MabrDataList)
{
	m_pMainRebarJ->SetRebarListData(m_MabrDataList);	
}

void CConcSectionMainBeamRebar::LoadDlgData(BOOL bSameIMJ)
{
	m_bSameIMJ = bSameIMJ;
	Data2Dlg();
}

void CConcSectionMainBeamRebar::SaveDlgData()
{	
	Dlg2Data();
}

void CConcSectionMainBeamRebar::SetAsRebarArea()
{	
	if(m_ctrlTab.GetCurSel() == 0)
		m_pMainRebarI->SetAsRebarArea();
	else if(m_ctrlTab.GetCurSel() == 1)
		m_pMainRebarM->SetAsRebarArea();
	else
		m_pMainRebarJ->SetAsRebarArea();
}