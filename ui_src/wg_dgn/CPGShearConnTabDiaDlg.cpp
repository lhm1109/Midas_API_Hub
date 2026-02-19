// CCPGShearConnTabDiaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGShearConnTabDiaDlg.h"

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
// CCPGShearConnTabDiaDlg dialog


CCPGShearConnTabDiaDlg::CCPGShearConnTabDiaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCPGShearConnTabDiaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGShearConnTabDiaDlg)	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}

CCPGShearConnTabDiaDlg::~CCPGShearConnTabDiaDlg()
{
}

void CCPGShearConnTabDiaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGShearConnTabDiaDlg)
	//DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_CODE_CMB,       m_cmbCode);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_CMB,       m_cmbDia);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_EDT,       m_edtDia);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_UNT,       m_untDia);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCPGShearConnTabDiaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_CBN_SELCHANGE(IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_CMB,  OnShearConnectorDiaCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCPGShearConnTabDiaDlg::InitUnit()
{
	m_untDia.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCPGShearConnTabDiaDlg::InitCombo()
{
	// Rebar Name
	CStringArray aRBName;
	CDBLib::GetRebarNameListByCode(aRBName, TRUE); //m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, aRBName); // Grade

	m_cmbDia.ResetContent();

	int nSize = aRBName.GetSize();
    for ( int i=0; i<nSize; ++i )
    {
        m_cmbDia.AddString(aRBName[i]);
    }

	if(nSize>0) m_cmbDia.SetCurSel(0);
	else ASSERT(0);
}


void CCPGShearConnTabDiaDlg::GetDia()
{
	UpdateData(TRUE);

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	m_pDoc->m_pMatlDB->Create_RebarData(UnitIndex.nBase_Length, UnitIndex.nBase_Force, TRUE);	// Concrete=TRUE.

	// Data Check
	CString strRebarName = _T("");
	m_cmbDia.GetLBText(m_cmbDia.GetCurSel(), strRebarName);

	double dDia = m_pDoc->m_pMatlDB->Get_RebarOutDia(strRebarName);
	m_edtDia.SetEditUnit(dDia);

}

void CCPGShearConnTabDiaDlg::GetValue(double &dDia)
{
	dDia = m_edtDia.GetEditValue();
}

/////////////////////////////////////////////////////////////////////////////
// CCPGShearConnTabDiaDlg message handlers

BOOL CCPGShearConnTabDiaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	GetDia();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCPGShearConnTabDiaDlg::OnOK() 
{
	CDialogMove::OnOK();
}

void CCPGShearConnTabDiaDlg::OnShearConnectorDiaCmb()
{
	GetDia();
}

