// CSeisCvlJpSetInvestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestDlg.h"
#include "SeisCvlJpSetInvestSimdDlg.h"
#include "SeisCvlJpSetInvestSirdDlg.h"
#include "SeisCvlJpSetInvestSibdDlg.h"
#include "SeisCvlJpSetInvestSidpDlg.h"
#include "SeisCvlJpSetInvestSisrDlg.h"
#include "SeisCvlJpSetInvestSietDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"
#include "..\wg_main\wg_mainRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestDlg

IMPLEMENT_DYNCREATE(CSeisCvlJpSetInvestDlg, MBaseDockingView)

CSeisCvlJpSetInvestDlg::CSeisCvlJpSetInvestDlg()
	: MBaseDockingView(CSeisCvlJpSetInvestDlg::IDD)
{
	// 클래스 만들때 Menu 추가 해야함
	AddChildDlgs();
	m_nCurChildDlg = -1;
}

CSeisCvlJpSetInvestDlg::~CSeisCvlJpSetInvestDlg()
{
	m_nCurChildDlg = -1;
}

void CSeisCvlJpSetInvestDlg::DoDataExchange(CDataExchange* pDX)
{
	MBaseDockingView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlJpSetInvestDlg)
	DDX_Control(pDX, IDC_TM_SEIS_CVL_JP_MENU_CMB, m_cmbMenu);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_JP_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetInvestDlg, MBaseDockingView)
	//{{AFX_MSG_MAP(CSeisCvlJpSetInvestDlg)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_JP_MENU_CMB, OnSelChangeMenuCmb)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_JP_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestDlg diagnostics

void CSeisCvlJpSetInvestDlg::AddChildDlgs()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) return;

	T_SIDC_D tSidc; tSidc.Initialize();
	pDoc->m_pAttrCtrl2->GetQSidc()->Get(tSidc);

	m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIMD__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_INVESTIGATE_SIMD));
	m_aChildDlg.Add(new CSeisCvlJpSetInvestSimdDlg);
	m_aTableID.Add(ID_MAIN_RIBBON_SEIS_CVL_JP_SETTING_INVESTIGATE_SIMD_TB);

	m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIRD__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_INVESTIGATE_SIRD));
	m_aChildDlg.Add(new CSeisCvlJpSetInvestSirdDlg);
	m_aTableID.Add(ID_MAIN_RIBBON_SEIS_CVL_JP_SETTING_INVESTIGATE_SIRD_TB);

	m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIBD__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_INVESTIGATE_SIBD));
	m_aChildDlg.Add(new CSeisCvlJpSetInvestSibdDlg);
	m_aTableID.Add(ID_MAIN_RIBBON_SEIS_CVL_JP_SETTING_INVESTIGATE_SIBD_TB);

	if (tSidc.nDesignCode != BrdgSE_CodeJP::JROAD5_H29)
	{
		m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIDP__);
		m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_INVESTIGATE_SIDP));
		m_aChildDlg.Add(new CSeisCvlJpSetInvestSidpDlg);
		m_aTableID.Add(ID_MAIN_RIBBON_SEIS_CVL_JP_SETTING_INVESTIGATE_SIDP_TB);
	}

	m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SISR__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_INVESTIGATE_SISR));
	m_aChildDlg.Add(new CSeisCvlJpSetInvestSisrDlg);
	m_aTableID.Add(ID_MAIN_RIBBON_SEIS_CVL_JP_SETTING_INVESTIGATE_SISR_TB);

	m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIET__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_INVESTIGATE_SIET));
	m_aChildDlg.Add(new CSeisCvlJpSetInvestSietDlg);
	m_aTableID.Add(ID_MAIN_RIBBON_SEIS_CVL_JP_SETTING_INVESTIGATE_SIET_TB);

	CBrdgSeisEvalResult_JP* pResult = pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (pResult) {
		if (pResult->GetSizeYieldStepForSeismic() < 1 && pResult->GetSizeUltimateStepForSeismic() < 1)
			pResult->FindYieldStep();
	}
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
BOOL CSeisCvlJpSetInvestDlg::ShowChildDlgByID(int nID)
{
	int nDlgIndex = -1;
	for (int i = 0; i < m_aDlgID.GetSize(); ++i)
	{
		if (m_aDlgID[i] == nID)	{
			nDlgIndex = i;
			break;
		}
	}
	if (nDlgIndex < 0 || nDlgIndex >= m_aChildDlg.GetSize())
		return FALSE;

	ShowCurChildDlg(nDlgIndex);
	m_cmbMenu.SetCurSel(nDlgIndex);
	return TRUE;
}

void CSeisCvlJpSetInvestDlg::OnInitialUpdate()
{
	MBaseDockingView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestDlg message handlers

int CSeisCvlJpSetInvestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MBaseDockingView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}