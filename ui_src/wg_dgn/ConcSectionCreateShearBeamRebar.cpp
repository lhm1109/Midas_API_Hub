// ConcSectionCreateRebar.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionCreateShearBeamRebar.h"
#include "ConcSectionShearBeamRebar.h"

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
// CConcSectionCreateShearBeamRebar dialog


CConcSectionCreateShearBeamRebar::CConcSectionCreateShearBeamRebar(CWnd* pParent /*=NULL*/, int nCurTab /*= 0*/)
	: CChildDialog(CConcSectionCreateShearBeamRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcSectionCreateShearBeamRebar)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = (CConcSectionShearBeamRebar*)pParent;

	m_ShbrData.Initialize();
	m_nCurTab = nCurTab;
}

CConcSectionCreateShearBeamRebar::~CConcSectionCreateShearBeamRebar()
{
}

void CConcSectionCreateShearBeamRebar::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionCreateShearBeamRebar)
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_SIZE_CMB,    m_cmbSize);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_SPACING_EDT, m_edtSpacing);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_SPACING_UNT, m_untSpacing);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_NUMBER_EDT,  m_edtNum);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_ANGLE_EDT,   m_edtAngle);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_ANGLE_UNT,   m_untAngle);
	//}}AFX_DATA_MAP
}

void CConcSectionCreateShearBeamRebar::InitCombo()
{
	T_UNIT_INDEX CurUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(CurUnit.nBase_Length,CurUnit.nBase_Force,TRUE);
	
	CDBLib::GetRebarNameAtComboBox(&m_cmbSize, TRUE);
}

void CConcSectionCreateShearBeamRebar::InitUnit()
{
	m_edtSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNum    .SetUnitType(D_UNITSYS_NONE);
	m_edtAngle  .SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untAngle  .SetUnitType(D_UNITSYS_BASE_DEGREE);
}

void CConcSectionCreateShearBeamRebar::EnableDisableControls(BOOL bSameIMJ)
{
	if((m_nCurTab==1 || m_nCurTab==2) && bSameIMJ)
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_SHEAR_REBAR_GRP, FALSE, TRUE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_SHEAR_REBAR_GRP, TRUE, TRUE);
	}
}

void CConcSectionCreateShearBeamRebar::Data2Dlg()
{
	m_cmbSize.SetCurSel(GetComboRebarIndex(m_ShbrData.strRebarName));
	m_edtSpacing.SetEditUnit(m_ShbrData.dSpace);
	m_edtNum    .SetEditUnit(m_ShbrData.dNum);
	m_edtAngle  .SetEditUnit(m_ShbrData.dAngle);
	
	UpdateData(FALSE);
}

BOOL CConcSectionCreateShearBeamRebar::Dlg2Data()
{	
	UpdateData(TRUE);
	
	m_ShbrData.Initialize();
	
	int Index = m_cmbSize.GetCurSel();
	if(Index!=-1) m_cmbSize.GetLBText(Index, m_ShbrData.strRebarName);
	m_ShbrData.dSpace = m_edtSpacing.GetEditValue();
	m_ShbrData.dNum   = m_edtNum.GetEditValue();
	m_ShbrData.dAngle = m_edtAngle.GetEditValue();
	
	return TRUE;
}

int CConcSectionCreateShearBeamRebar::GetComboRebarIndex(CString strRebarname)
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

BEGIN_MESSAGE_MAP(CConcSectionCreateShearBeamRebar, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionCreateShearBeamRebar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearBeamRebar message handlers
BOOL CConcSectionCreateShearBeamRebar::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	InitCombo();
	InitUnit();
	EnableDisableControls(m_pParent->m_bSameIMJ);
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConcSectionCreateShearBeamRebar::DestroyWindow() 
{
	Dlg2Data();
	
	if     (m_nCurTab==0) m_pParent->m_DataI = m_ShbrData;
	else if(m_nCurTab==1) m_pParent->m_DataM = m_ShbrData;
	else                  m_pParent->m_DataJ = m_ShbrData;

	return CChildDialog::DestroyWindow();
}




