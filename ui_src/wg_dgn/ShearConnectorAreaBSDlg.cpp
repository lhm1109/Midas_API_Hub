// CShearConnectorAreaBSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ShearConnectorAreaBSDlg.h"

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
// CShearConnectorAreaBSDlg dialog


CShearConnectorAreaBSDlg::CShearConnectorAreaBSDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CShearConnectorAreaBSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearConnectorAreaBSDlg)	
	//}}AFX_DATA_INIT
}

CShearConnectorAreaBSDlg::~CShearConnectorAreaBSDlg()
{
}

void CShearConnectorAreaBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearConnectorAreaBSDlg)
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_DIA_CMB,     m_cmbDia);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_NUM_EDT,     m_edtNum);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_SPACING_EDT, m_edtSpacing);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_SPACING_UNT, m_untSpacing);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_AE_EDT,      m_edtAe);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_AE_UNT,      m_untAe);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShearConnectorAreaBSDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_CBN_SELCHANGE(IDC_DGN_DESIGN_CONNECTOR_AREA_DIA_CMB,     OnShearConnectorAreaDiaCmb)
	ON_EN_CHANGE    (IDC_DGN_DESIGN_CONNECTOR_AREA_NUM_EDT,     OnShearConnectorAreaNumEdt)
	ON_EN_CHANGE    (IDC_DGN_DESIGN_CONNECTOR_AREA_SPACING_EDT, OnShearConnectorAreSpacingEdt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CShearConnectorAreaBSDlg::InitUnit()
{
	m_edtNum.SetUnitType(D_UNITSYS_NONE);
	m_edtSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtAe.SetUnitType(D_UNITSYS_BASE_UNIT_AREA);
	m_untAe.SetUnitType(D_UNITSYS_BASE_UNIT_AREA);
}

void CShearConnectorAreaBSDlg::InitCombo()
{
	CStringArray aRBName;
	CDBLib::GetRebarNameListByCode(aRBName, TRUE);

	m_cmbDia.ResetContent();

	int nSize = aRBName.GetSize();
	for(int i=0; i<nSize; ++i) m_cmbDia.AddString(aRBName[i]);

	if(nSize>0) m_cmbDia.SetCurSel(0);
	else ASSERT(0);
}

void CShearConnectorAreaBSDlg::CalcArea()
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_UNIT_INDEX UnitIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	pDoc->m_pMatlDB->Create_RebarData(UnitIndex.nBase_Length, UnitIndex.nBase_Force, TRUE);

	CString strRebarName = _T("");
	m_cmbDia.GetLBText(m_cmbDia.GetCurSel(), strRebarName);
	
	double dNum     = m_edtNum.GetEditValue();
	double dSpacing = m_edtSpacing.GetEditValue();

	if( (dNum*dSpacing) <= 0.0)
	{
		m_edtAe.SetEditUnit(0.0);
	}
	else
	{
		double dArea = pDoc->m_pMatlDB->Get_RebarArea(strRebarName);
		double dAe = (dArea * dNum) / dSpacing;

		m_edtAe.SetEditUnit(dAe);
	}
}

void CShearConnectorAreaBSDlg::GetValue(double &dArea)
{
	dArea = m_edtAe.GetEditValue();
}

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorAreaBSDlg message handlers

BOOL CShearConnectorAreaBSDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	CalcArea();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShearConnectorAreaBSDlg::OnOK()
{
	CDialogMove::OnOK();
}

void CShearConnectorAreaBSDlg::OnShearConnectorAreaDiaCmb()
{
	CalcArea();
}

void CShearConnectorAreaBSDlg::OnShearConnectorAreaNumEdt()
{
	CalcArea();
}

void CShearConnectorAreaBSDlg::OnShearConnectorAreSpacingEdt()
{
	CalcArea();
}