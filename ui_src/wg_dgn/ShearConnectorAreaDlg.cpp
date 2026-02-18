// CShearConnectorAreaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
//#include "wg_cmd2.h"
#include "ShearConnectorAreaDlg.h"

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
// CShearConnectorAreaDlg dialog


CShearConnectorAreaDlg::CShearConnectorAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CShearConnectorAreaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearConnectorAreaDlg)	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}

CShearConnectorAreaDlg::~CShearConnectorAreaDlg()
{
}

void CShearConnectorAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearConnectorAreaDlg)
	//DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_CODE_CMB,       m_cmbCode);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_DIA_CMB,        m_cmbDia);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_LAT_NUM_EDT,    m_edtLatNum);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_LONG_NUM_EDT,   m_edtLongNum);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_AREA_EDT,       m_edtArea);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_UNT,            m_untArea);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShearConnectorAreaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//ON_CBN_SELCHANGE(IDC_DGN_DESIGN_CONNECTOR_AREA_CODE_CMB,     OnShearConnectorAreaCodeCmb)
	ON_CBN_SELCHANGE(IDC_DGN_DESIGN_CONNECTOR_AREA_DIA_CMB,      OnShearConnectorAreaDiaCmb)
	ON_EN_CHANGE    (IDC_DGN_DESIGN_CONNECTOR_AREA_LAT_NUM_EDT,  OnShearConnectorAreaLatNumEdt)
	ON_EN_CHANGE    (IDC_DGN_DESIGN_CONNECTOR_AREA_LONG_NUM_EDT,  OnShearConnectorAreaLongNumEdt)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CShearConnectorAreaDlg::InitUnit()
{
	m_untArea.SetUnitType(D_UNITSYS_BASE_AREA);
}

void CShearConnectorAreaDlg::InitCombo()
{
	// Rebar Code
//   CArray<CString, CString&> arDesignCode;
//   CString strMatlType = _T("C");
//   m_pDoc->m_pMatlDB->GetDesignCodeList(strMatlType, arDesignCode);
// 
//   for(int i=0; i<arDesignCode.GetSize(); i++)
//   {
//     m_cmbCode.AddString(arDesignCode[i]);
//   }
// 
//   if(i>0) m_cmbCode.SetCurSel(0);
//   else ASSERT(0);
// 
//   CString strRebarCode = _T("");
//   m_cmbCode.GetLBText(m_cmbCode.GetCurSel(), strRebarCode);

	// Rebar Name
	CStringArray aRBName;
	CDBLib::GetRebarNameListByCode(aRBName, TRUE); //m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, aRBName); // Grade

	m_cmbDia.ResetContent();

	int nSize = aRBName.GetSize();
	for(int i=0; i<nSize; ++i) m_cmbDia.AddString(aRBName[i]);

	if(nSize>0) m_cmbDia.SetCurSel(0);
	else ASSERT(0);
}

// void CShearConnectorAreaDlg::InitDiaCombo()
// {
//   CStringArray aRBName;
//   CDBLib::GetRebarNameListByCode(aRBName, TRUE);
// 
//   // Point Type
//   m_cmbDia.ResetContent();
// 
//   int nSize = aRBName.GetSize();
//   for(int i=0; i<nSize; ++i) m_cmbDia.AddString(aRBName[i]);
// 
//   if(i>0) m_cmbDia.SetCurSel(0);
// }

void CShearConnectorAreaDlg::CalcArea()
{
	UpdateData(TRUE);

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	m_pDoc->m_pMatlDB->Create_RebarData(UnitIndex.nBase_Length, UnitIndex.nBase_Force, TRUE);	// Concrete=TRUE.

	// Data Check
	CString strRebarName = _T("");
	m_cmbDia.GetLBText(m_cmbDia.GetCurSel(), strRebarName);

	double dNum     = m_edtLatNum.GetEditValue();
	double dNumLong = m_edtLongNum.GetEditValue();

	if(dNum*dNumLong <= 0.0)
	{
		m_edtArea.SetEditUnit(0.0);
	}
	else
	{
		double dArea = m_pDoc->m_pMatlDB->Get_RebarArea(strRebarName);
		dArea *= (dNum*dNumLong);

		m_edtArea.SetEditUnit(dArea);
	}
}

void CShearConnectorAreaDlg::GetValue(double &dArea)
{
	dArea = m_edtArea.GetEditValue();
}

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorAreaDlg message handlers

BOOL CShearConnectorAreaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	CalcArea();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShearConnectorAreaDlg::OnOK() 
{
	CDialogMove::OnOK();
}

// void CShearConnectorAreaDlg::OnShearConnectorAreaCodeCmb()
// {
// //   // Rebar Name
// //   CString strRebarCode = _T("");
// //   m_cmbCode.GetLBText(m_cmbCode.GetCurSel(), strRebarCode);
// // 
// //   CArray<CString,CString&> aRBName;
// //   m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, aRBName);
// 
//   CStringArray aRBName;
//   CDBLib::GetRebarNameListByCode(aRBName, TRUE); 
//   m_cmbDia.ResetContent();
// 
//   int nSize = aRBName.GetSize();
//   for(int i=0; i<nSize; ++i) m_cmbDia.AddString(aRBName[i]);
// 
//   if(i>0) m_cmbDia.SetCurSel(0);
//   else ASSERT(0);
// 
//   CalcArea();
// }

void CShearConnectorAreaDlg::OnShearConnectorAreaDiaCmb()
{
	CalcArea();
}

void CShearConnectorAreaDlg::OnShearConnectorAreaLatNumEdt()
{
	CalcArea();
}

void CShearConnectorAreaDlg::OnShearConnectorAreaLongNumEdt()
{
	CalcArea();
}


