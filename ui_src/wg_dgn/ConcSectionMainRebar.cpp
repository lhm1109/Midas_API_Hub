#include "stdafx.h"
#include "wg_dgn.h"

#include "../wg_db/DBCodeDef.h"
#include "ConcSectionMainRebar.h"

#include "ConcSectionCreateRebar.h"
#include "ConcSectionCreateRebar_J.h"

#include "oncSectionRebarViewDlg.h"

#include "DgnTabCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ConcSectionMainRebar dialog


CConcSectionMainRebar::CConcSectionMainRebar(CWnd* pParent /*=NULL*/)
	: CChildDialog(CConcSectionMainRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcSectionMainRebar)
	
	//}}AFX_DATA_INIT
	//ASSERT(pSectView);
	//m_pSectView = pSectView;
	ASSERT(pParent);
	m_pParent = (ConcSectionRebarViewDlg*)pParent;

	m_pMainRebarI = NULL;
	m_pMainRebarJ = NULL;

	bSameIJ = TRUE;

	//m_strDgnCode	= _T("");
	m_strDgnCode	= m_pParent->m_strDgnCode;


	//m_Data.Initialize();
}


void CConcSectionMainRebar::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionMainRebar)
	DDX_Control(pDX, IDC_DGN_CON_MAIN_REBAR_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_DGN_CON_MAIN_REBAR_IJBOTH, m_chkSameIJ);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConcSectionMainRebar, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionMainRebar)
	ON_BN_CLICKED(IDC_DGN_CON_MAIN_REBAR_IJBOTH, OnCmdSameCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_CON_MAIN_REBAR_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface



/////////////////////////////////////////////////////////////////////////////
// CConcSectionMainRebar message handlers

BOOL CConcSectionMainRebar::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->LayoutChildDlg();

	//Data2Dlg();  
	LoadDlgData(m_pParent->bMainSameIJ);

	GetDlgItem(IDC_DGN_CON_MAIN_REBAR_IJBOTH)->ShowWindow(SW_SHOW);

	const CString& strCode = m_pParent->m_strDgnCode;
	if(strCode == CONCODE_EC2_2_05 || strCode == CONCODE_BS5400_90 || 
	   strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_TMH07_89 ||
	   strCode == CONCODE_AASHTO_LRFD07 || strCode == CONCODE_AASHTO_LRFD16 ||
	   strCode == CONCODE_AASHTO_LRFD17 || strCode == CONCODE_AASHTO_LRFD20 ||
	   strCode == CONCODE_AASHTO_LRFD24)
	{
		m_chkSameIJ.SetCheck(TRUE);
		GetDlgItem(IDC_DGN_CON_MAIN_REBAR_IJBOTH)->ShowWindow(SW_HIDE);
	}

	// 20110520. by JhKim. Skin tab 보이지 않는 문제 수정
	m_ctrlTab.SendMessage(WM_PAINT, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CConcSectionMainRebar::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->bMainSameIJ = bSameIJ;
	  
	return CChildDialog::DestroyWindow();
}

void CConcSectionMainRebar::OnCmdSameCheck() 
{
	bSameIJ = (m_chkSameIJ.GetCheck() == 0) ? FALSE : TRUE;

	if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pMainRebarJ->AllControlEnableWindow(!bSameIJ);
	}
}

void CConcSectionMainRebar::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pParent->DrawSectionRebarTabChanged();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CConcSectionMainRebar::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_MAINREBAR_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	CString strTabTitle = _T("");
	BOOL bCheckJ = TRUE;
	if (m_strDgnCode == CONCODE_EC2_2_05 || m_strDgnCode == CONCODE_BS5400_90 ||
		m_strDgnCode == CONCODE_AASHTO_LRFD12 || m_strDgnCode == CONCODE_TMH07_89 ||
		m_strDgnCode == CONCODE_AASHTO_LRFD07 || m_strDgnCode == CONCODE_AASHTO_LRFD16 ||
		m_strDgnCode == CONCODE_AASHTO_LRFD17 || m_strDgnCode == CONCODE_AASHTO_LRFD20 ||
		m_strDgnCode == CONCODE_AASHTO_LRFD24)
	{
		bCheckJ = FALSE;
	}	
	strTabTitle = (!bCheckJ) ? _LS(IDS_DGN_SET_DATA) : _LS(IDS_DGN_RPSC_I);

	// TabWinddow Create & Setting
	m_pMainRebarI = new CConcSectionCreateRebar(this);
	m_pMainRebarI->SetDlgViewPanel(this->m_pIDVP);
	m_ctrlTab.AddTab(m_pMainRebarI, strTabTitle, CConcSectionCreateRebar::IDD, TRUE);


	if(!bCheckJ)
	{
		// 동작하지 않음.
	}
	else
	{
		m_pMainRebarJ = new CConcSectionCreateRebar_J(this);
		m_pMainRebarJ->SetDlgViewPanel(this->m_pIDVP);
		m_ctrlTab.AddTab(m_pMainRebarJ, _LS(IDS_DGN_RPSC_J), CConcSectionCreateRebar_J::IDD, TRUE);
	}

	m_ctrlTab.ShowTab(0);
}

void CConcSectionMainRebar::Data2Dlg()
{
	m_chkSameIJ.SetCheck(bSameIJ);
	
}

BOOL CConcSectionMainRebar::Dlg2Data()
{
	bSameIJ = (m_chkSameIJ.GetCheck() == 0) ? FALSE : TRUE;
		
	return TRUE;
}

void CConcSectionMainRebar::RedrawView()
{
	//ASSERT(m_pSectView);
	//Dlg2Data(&m_Data, FALSE);
	//m_pSectView->Invalidate();
}

void CConcSectionMainRebar::SetDlgViewPanel(I_DlgViewPanel* pIDVP)
{
	this->m_pIDVP = pIDVP;
	//m_pMainRebarI->SetDlgViewPanel(this->m_pIDVP);
	//m_pMainRebarJ->SetDlgViewPanel(this->m_pIDVP);
}

void CConcSectionMainRebar::AddRebarData(REBAR_KEY RebarKey, T_REBT_MABR MaReberData, BOOL bI_End)
{
	m_pParent->AddRebarData(RebarKey, MaReberData, bI_End);		
}

void CConcSectionMainRebar::DeleteRebarData(REBAR_KEY RebarKey, BOOL bI_End)
{
	m_pParent->DeleteRebarData(RebarKey, bI_End);
}

int CConcSectionMainRebar::GetCurSel()
{
	return m_ctrlTab.GetCurSel();
}

void CConcSectionMainRebar::RemoveMainRebarDataListI(REBAR_KEY Key, T_REBT_MABR Data)
{
	m_pMainRebarI->RemoveMainRebarDataList(Key, Data);
}

void CConcSectionMainRebar::RemoveMainRebarDataListJ(REBAR_KEY Key, T_REBT_MABR Data)
{
	m_pMainRebarJ->RemoveMainRebarDataList(Key, Data);
	
}

void CConcSectionMainRebar::SetInitDataI(CArray<T_REBT_MABR,T_REBT_MABR&>& MainRebarDataList)
{
	m_pMainRebarI->SetInitData(MainRebarDataList);	
}

void CConcSectionMainRebar::SetInitDataJ(CArray<T_REBT_MABR,T_REBT_MABR&>& MainRebarDataList)
{
	m_pMainRebarJ->SetInitData(MainRebarDataList);	
}

void CConcSectionMainRebar::SetRebarListDataI(CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR>& m_MabrDataList)
{
	m_pMainRebarI->SetRebarListData(m_MabrDataList);	
}

void CConcSectionMainRebar::SetRebarListDataJ(CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR>& m_MabrDataList)
{
	m_pMainRebarJ->SetRebarListData(m_MabrDataList);	
}

void CConcSectionMainRebar::LoadDlgData(BOOL bSameij)
{
	bSameIJ = bSameij;
	Data2Dlg();
}

void CConcSectionMainRebar::SaveDlgData()
{	
	Dlg2Data();
}

void CConcSectionMainRebar::InitDialog()
{	
	OnInitDialog();
}

void CConcSectionMainRebar::SetAsRebarArea()
{	
	if(m_ctrlTab.GetCurSel() == 0)
		m_pMainRebarI->SetAsRebarArea();
	else
		m_pMainRebarJ->SetAsRebarArea();
}