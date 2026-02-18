#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionShearRebar.h"

#include "ConcSectionCreateShearRebar_I.h"
#include "ConcSectionCreateShearRebar_J.h"

#include "oncSectionRebarViewDlg.h"

#include "DgnTabCtrl.h"

#include "../wg_db/DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ConcSectionMainRebar dialog


CConcSectionShearRebar::CConcSectionShearRebar(CWnd* pParent /*=NULL*/)
	: CChildDialog(CConcSectionShearRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcSectionShearRebar)
	
	//}}AFX_DATA_INIT
	//ASSERT(pSectView);
	//m_pSectView = pSectView;
	ASSERT(pParent);
	m_pParent = (ConcSectionRebarViewDlg*)pParent;

	m_pShearRebarI = NULL;
	m_pShearRebarJ = NULL;

	m_DataI.Initialize();
	m_DataJ.Initialize();

	//
	//m_strDgnCode	= _T("");
	m_strDgnCode	= m_pParent->m_strDgnCode;

	//m_Data.Initialize();
}


void CConcSectionShearRebar::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionShearRebar)
	DDX_Control(pDX, IDC_DGN_CON_SHEAR_REBAR_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_DGN_CON_SHEAR_REBAR_IJBOTH, m_chkSameIJ);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConcSectionShearRebar, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionShearRebar)
	ON_BN_CLICKED(IDC_DGN_CON_SHEAR_REBAR_IJBOTH, OnCmdSameCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_CON_SHEAR_REBAR_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface



/////////////////////////////////////////////////////////////////////////////
// CConcSectionShearRebar message handlers

BOOL CConcSectionShearRebar::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->LayoutChildDlg();

	//Data2Dlg();  
	LoadDlgData(m_pParent->bShearSameIJ, m_pParent->m_ShearRebarDataI, m_pParent->m_ShearRebarDataJ);

	GetDlgItem(IDC_DGN_CON_SHEAR_REBAR_IJBOTH)->ShowWindow(SW_SHOW);

	const CString& strCode = m_pParent->m_strDgnCode;
	if(strCode == CONCODE_EC2_2_05 || strCode == CONCODE_BS5400_90 ||
	   strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_TMH07_89 ||
	   strCode == CONCODE_AASHTO_LRFD07 || strCode == CONCODE_AASHTO_LRFD16 ||
	   strCode == CONCODE_AASHTO_LRFD17 || strCode == CONCODE_AASHTO_LRFD20 ||
	   strCode == CONCODE_AASHTO_LRFD24)
	{
		m_chkSameIJ.SetCheck(TRUE);
		GetDlgItem(IDC_DGN_CON_SHEAR_REBAR_IJBOTH)->ShowWindow(SW_HIDE);
	}

	// 20110520. by JhKim. Skin tab 보이지 않는 문제 수정
	m_ctrlTab.SendMessage(WM_PAINT, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CConcSectionShearRebar::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->bShearSameIJ = bSameIJ;
	m_pParent->m_ShearRebarDataI = m_DataI;
	m_pParent->m_ShearRebarDataJ = m_DataJ;

	return CChildDialog::DestroyWindow();
}

void CConcSectionShearRebar::OnCmdSameCheck() 
{
	bSameIJ = (m_chkSameIJ.GetCheck() == 0) ? FALSE : TRUE;

	if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pShearRebarJ->AllControlEnableWindow(!bSameIJ);
	}
}

void CConcSectionShearRebar::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pParent->DrawSectionRebarTabChanged();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CConcSectionShearRebar::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SHEARREBAR_PLACEHOLDER);
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
	strTabTitle = (!bCheckJ) ? _T("Set Data") : _T("I");

	// TabWinddow Create & Setting 
	m_pShearRebarI = new CConcSectionCreateShearRebar_I(this);
	m_ctrlTab.AddTab(m_pShearRebarI, strTabTitle, CConcSectionCreateShearRebar_I::IDD, TRUE);

	if(!bCheckJ)
	{
		//
	}
	else
	{
		m_pShearRebarJ = new CConcSectionCreateShearRebar_J(this);
		m_ctrlTab.AddTab(m_pShearRebarJ, _T("J"), CConcSectionCreateShearRebar_J::IDD, TRUE);
	}

	m_ctrlTab.ShowTab(0);
}

void CConcSectionShearRebar::Data2Dlg()
{
	m_chkSameIJ.SetCheck(bSameIJ);
	if(m_ctrlTab.GetCurSel() == 0)//I-End
	{
		m_pShearRebarI->SetInitData(m_DataI);
	}
	else//J-End
	{
		m_pShearRebarJ->SetInitData(m_DataJ);	
	}
	
	
}

BOOL CConcSectionShearRebar::Dlg2Data()
{
	bSameIJ = (m_chkSameIJ.GetCheck() == 0) ? FALSE : TRUE;

	if(m_ctrlTab.GetCurSel() == 0)//I-End
	{
		m_pShearRebarI->SaveDlgData();
		m_DataI = m_pShearRebarI->m_ShbrData;
	}
	else//J-End
	{
		m_pShearRebarJ->SaveDlgData();
		m_DataJ = m_pShearRebarJ->m_ShbrData;
	}
	
	return TRUE;
}

void CConcSectionShearRebar::LoadDlgData(BOOL bSameij, T_REBT_SHBR iData, T_REBT_SHBR jData)
{
	bSameIJ = bSameij;
	m_DataI = iData;
	m_DataJ = jData;

	Data2Dlg();
}

void CConcSectionShearRebar::SaveDlgData()
{	
	Dlg2Data();
}

int CConcSectionShearRebar::GetCurSel()
{
	return m_ctrlTab.GetCurSel();
}

