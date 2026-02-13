// CInterfaceShearAreaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
//#include "wg_cmd2.h"
#include "InterfaceShearAreaDlg.h"

//#include "..\wg_base\wg_base_DlgUtil.h"
//#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterfaceShearAreaDlg dialog


CInterfaceShearAreaDlg::CInterfaceShearAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CInterfaceShearAreaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInterfaceShearAreaDlg)	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}

CInterfaceShearAreaDlg::~CInterfaceShearAreaDlg()
{
}

void CInterfaceShearAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterfaceShearAreaDlg)
	//DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_CODE_CMB,       m_cmbCode);
	DDX_Control(pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_AREA_DIA_CMB,        m_cmbDia);
	DDX_Control(pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_AREA_LAT_NUM_EDT,    m_edtLatNum);
	DDX_Control(pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_SPACING_EDT,         m_edtSpacing);
	DDX_Control(pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_SPACING_UNT,         m_untSpacing);
	DDX_Control(pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_AREA_AREA__EDT,      m_edtArea);
	DDX_Control(pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_AREA_UNT,            m_untArea);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInterfaceShearAreaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//ON_CBN_SELCHANGE(IDC_DGN_DESIGN_CONNECTOR_AREA_CODE_CMB,     OnInterfaceShearAreaCodeCmb)
	ON_CBN_SELCHANGE(IDC_DGN_DESIGN_INTERFACE_SHEAR_AREA_DIA_CMB,      OnInterfaceShearAreaDiaCmb)
	ON_EN_CHANGE    (IDC_DGN_DESIGN_INTERFACE_SHEAR_AREA_LAT_NUM_EDT,  OnInterfaceShearAreaLatNumEdt)
	ON_EN_CHANGE    (IDC_DGN_DESIGN_INTERFACE_SHEAR_SPACING_EDT,       OnInterfaceShearAreaSpacingEdt)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInterfaceShearAreaDlg::InitUnit()
{
	m_untArea.SetUnitType(D_UNITSYS_BASE_UNIT_AREA);
}

void CInterfaceShearAreaDlg::InitCombo()
{
	// Rebar Name
	CStringArray aRBName;
	CDBLib::GetRebarNameListByCode(aRBName, TRUE); //m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, aRBName); // Grade

	m_cmbDia.ResetContent();

	int nSize = aRBName.GetSize();
	for(int i=0; i<nSize; ++i) m_cmbDia.AddString(aRBName[i]);

	if( nSize>0) m_cmbDia.SetCurSel(0);
	else ASSERT(0);
}

void CInterfaceShearAreaDlg::CalcArea()
{
	UpdateData(TRUE);

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	m_pDoc->m_pMatlDB->Create_RebarData(UnitIndex.nBase_Length, UnitIndex.nBase_Force, TRUE);	// Concrete=TRUE.

	// Data Check
	CString strRebarName = _T("");
	m_cmbDia.GetLBText(m_cmbDia.GetCurSel(), strRebarName);

	double dTol     = 0.000001;
	double dNum     = m_edtLatNum.GetEditValue();
	double dSpacing = m_edtSpacing.GetEditValue();
	double dArea_temp = (dSpacing < dTol)? 0.0 : dNum/dSpacing;
	//단위 인치 당 면적이므로 개수에다가 spacing(inch)을 나누어 준다. 
	if(dArea_temp <= 0.0) 
	{
		m_edtArea.SetEditUnit(0.0);
	}
	else
	{
		double dArea = m_pDoc->m_pMatlDB->Get_RebarArea(strRebarName);
		dArea *= (dArea_temp);

		m_edtArea.SetEditUnit(dArea);
	}
}

void CInterfaceShearAreaDlg::GetValue(double &dArea)
{
	dArea = m_edtArea.GetEditValue();
}

/////////////////////////////////////////////////////////////////////////////
// CInterfaceShearAreaDlg message handlers

BOOL CInterfaceShearAreaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	CalcArea();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInterfaceShearAreaDlg::OnOK() 
{
	CDialogMove::OnOK();
}


void CInterfaceShearAreaDlg::OnInterfaceShearAreaDiaCmb()
{
	CalcArea();
}

void CInterfaceShearAreaDlg::OnInterfaceShearAreaLatNumEdt()
{
	CalcArea();
}

void CInterfaceShearAreaDlg::OnInterfaceShearAreaSpacingEdt()
{
	CalcArea();
}


