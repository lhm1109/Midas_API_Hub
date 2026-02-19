// ConcSectionCreateRebar.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionCreateTorsionBeamRebar.h"
#include "ConcSectionTorsionBeamRebar.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateTorsionBeamRebar dialog


CConcSectionCreateTorsionBeamRebar::CConcSectionCreateTorsionBeamRebar(CWnd* pParent /*=NULL*/, int nCurTab /*= 0*/)
	: CChildDialog(CConcSectionCreateTorsionBeamRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcSectionCreateTorsionBeamRebar)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = (CConcSectionTorsionBeamRebar*)pParent;

	m_TorbrData.Initialize();
	m_nCurTab = nCurTab;
}

CConcSectionCreateTorsionBeamRebar::~CConcSectionCreateTorsionBeamRebar()
{
}

void CConcSectionCreateTorsionBeamRebar::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionCreateTorsionBeamRebar)
	DDX_Check  (pDX, IDC_DGN_TORSION_REBAR_TOR_CHK,                  m_bTorsionalBar);
	DDX_Control(pDX, IDC_DGN_TORSION_REBAR_TOR_STIRRUP_SIZE_CMB,     m_cmbStirrupSize);
	DDX_Control(pDX, IDC_DGN_TORSION_REBAR_TOR_STIRRUP_SPACING_EDT,  m_edtStirrupSpacing);
	DDX_Control(pDX, IDC_DGN_TORSION_REBAR_TOR_STIRRUP_SPACING_UNT,  m_untStirrupSpacing);
	DDX_Check  (pDX, IDC_DGN_TORSION_REBAR_TOR_BUNDLED_CHK,          m_bBundled);
	DDX_Control(pDX, IDC_DGN_TORSION_REBAR_TOR_BUNDLED_NUM_EDT,      m_edtBundledNum);
	DDX_Control(pDX, IDC_DGN_TORSION_REBAR_TOR_LONGI_SIZE_CMB,       m_cmbLongiSize);
	DDX_Control(pDX, IDC_DGN_TORSION_REBAR_TOR_LONGI_NUM_EDT,				 m_edtLongiNum);
	//}}AFX_DATA_MAP
}

void CConcSectionCreateTorsionBeamRebar::InitCombo()
{
	T_UNIT_INDEX CurUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(CurUnit.nBase_Length,CurUnit.nBase_Force,TRUE);
	
	CDBLib::GetRebarNameAtComboBox(&m_cmbStirrupSize, TRUE);
	CDBLib::GetRebarNameAtComboBox(&m_cmbLongiSize,   TRUE);
}

void CConcSectionCreateTorsionBeamRebar::InitUnit()
{
	m_edtStirrupSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStirrupSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtBundledNum    .SetUnitType(D_UNITSYS_NONE);
	m_edtLongiNum      .SetUnitType(D_UNITSYS_NONE);
}

void CConcSectionCreateTorsionBeamRebar::EnableDisableControls(BOOL bSameIMJ)
{
	if((m_nCurTab==1 || m_nCurTab==2) && bSameIMJ)
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_TORSION_REBAR_TOR_STIRRUP_GRP, FALSE, TRUE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_TORSION_REBAR_TOR_LONGI_GRP,   FALSE, TRUE);

		GetDlgItem(IDC_DGN_TORSION_REBAR_TOR_CHK)->EnableWindow(FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_TORSION_REBAR_TOR_STIRRUP_GRP, m_bTorsionalBar, TRUE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_TORSION_REBAR_TOR_LONGI_GRP,   m_bTorsionalBar, TRUE);

		GetDlgItem(IDC_DGN_TORSION_REBAR_TOR_CHK)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_DGN_TORSION_REBAR_TOR_BUNDLED_CHK)->EnableWindow(m_bTorsionalBar);
		GetDlgItem(IDC_DGN_TORSION_REBAR_TOR_BUNDLED_NUM_STC)->EnableWindow(m_bTorsionalBar && m_bBundled);
		GetDlgItem(IDC_DGN_TORSION_REBAR_TOR_BUNDLED_NUM_EDT)->EnableWindow(m_bTorsionalBar && m_bBundled);
	}
}

void CConcSectionCreateTorsionBeamRebar::Data2Dlg()
{
	m_bTorsionalBar = m_TorbrData.bTorsionalBar;
	m_cmbStirrupSize.SetCurSel(GetComboRebarIndex(m_TorbrData.strTorBarNa));
	m_edtStirrupSpacing.SetEditUnit(m_TorbrData.dTorBarSpacing);
	m_bBundled = m_TorbrData.bBundledBar;
	m_edtBundledNum.SetEditUnit(m_TorbrData.dTorBarNum);
	m_cmbLongiSize.SetCurSel(GetComboRebarIndex(m_TorbrData.strLongiBarNa));
	m_edtLongiNum.SetEditUnit(m_TorbrData.dLongiBarNum);

	EnableDisableControls(m_pParent->m_bSameIMJ);
	
	UpdateData(FALSE);
}

BOOL CConcSectionCreateTorsionBeamRebar::Dlg2Data()
{	
	UpdateData(TRUE);
	
	m_TorbrData.Initialize();

	m_TorbrData.bTorsionalBar =	m_bTorsionalBar;
	int Index = m_cmbStirrupSize.GetCurSel();
	if(Index!=-1) m_cmbStirrupSize.GetLBText(Index, m_TorbrData.strTorBarNa);
	m_TorbrData.dTorBarSpacing = m_edtStirrupSpacing.GetEditValue();
	m_TorbrData.bBundledBar =	m_bBundled;
	m_TorbrData.dTorBarNum = m_edtBundledNum.GetEditValue();
	Index = m_cmbLongiSize.GetCurSel();
	if(Index!=-1) m_cmbLongiSize.GetLBText(Index, m_TorbrData.strLongiBarNa);
	m_TorbrData.dLongiBarNum = m_edtLongiNum.GetEditValue();

	return TRUE;
}

int CConcSectionCreateTorsionBeamRebar::GetComboRebarIndex(CString strRebarname)
{
	CStringArray arRebarNa;
	CDBLib::GetRebarNameListByCode(arRebarNa, TRUE);
	
	int i = 0;
	for(i=0 ; i<arRebarNa.GetSize() ; i++)
	{
		if(strRebarname == arRebarNa[i])
			break;
	}
	
	return i;
}

BEGIN_MESSAGE_MAP(CConcSectionCreateTorsionBeamRebar, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionCreateTorsionBeamRebar)
	ON_BN_CLICKED(IDC_DGN_TORSION_REBAR_TOR_CHK, OnClickTorsionalBarBtn)
	ON_BN_CLICKED(IDC_DGN_TORSION_REBAR_TOR_BUNDLED_CHK, OnClickBundledBtn)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateTorsionBeamRebar message handlers
BOOL CConcSectionCreateTorsionBeamRebar::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	InitCombo();
	InitUnit();
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConcSectionCreateTorsionBeamRebar::DestroyWindow() 
{
	Dlg2Data();
	
	if     (m_nCurTab==0) m_pParent->m_DataI = m_TorbrData;
	else if(m_nCurTab==1) m_pParent->m_DataM = m_TorbrData;
	else                  m_pParent->m_DataJ = m_TorbrData;

	return CChildDialog::DestroyWindow();
}

void CConcSectionCreateTorsionBeamRebar::OnClickTorsionalBarBtn()
{
	UpdateData(TRUE);

	EnableDisableControls(m_pParent->m_bSameIMJ);
}

void CConcSectionCreateTorsionBeamRebar::OnClickBundledBtn()
{
	UpdateData(TRUE);

	EnableDisableControls(m_pParent->m_bSameIMJ);
}


