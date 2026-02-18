// DgnStlRatingShearConnTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingShearConnTabJDlg.h"
#include "DgnStlRatingShearConnDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingShearConnTabJDlg dialog


CDgnStlRatingShearConnTabJDlg::CDgnStlRatingShearConnTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnStlRatingShearConnTabJDlg::IDD, pParent)
{
	ASSERT(pParent);
	m_pParent = (CDgnStlRatingShearConnDlg*)pParent;

	m_Data.Initialize();

	m_Data.dPim = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_FORCE, m_Data.dPim);
	m_Data.dPr = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_FORCE, m_Data.dPr);
}


void CDgnStlRatingShearConnTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DGN_NUM_STUDS_WEB_EDT, m_iNumStudWeb);
	DDX_Control(pDX, IDC_DGN_L_SPACE_EDT, m_dLSpacing);
	DDX_Control(pDX, IDC_DGN_PIM_EDT, m_dPim);
	DDX_Control(pDX, IDC_DGN_PR_EDT, m_dPr);
	DDX_Control(pDX, IDC_DGN_MAX_RATIO_EDT, m_dMaxRatio);
	DDX_Control(pDX, IDC_DGN_NA_EDT, m_dNa);

	DDX_Control(pDX, IDC_DGN_L_SPACE_UNT, m_dLSpacingUnit);
	DDX_Control(pDX, IDC_DGN_PIM_UNT, m_dPimUnit);
	DDX_Control(pDX, IDC_DGN_PR_UNT, m_dPrUnit);
}


BEGIN_MESSAGE_MAP(CDgnStlRatingShearConnTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnStlRatingShearConnTabJDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingShearConnTabJDlg message handlers


BOOL CDgnStlRatingShearConnTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();


	SetInitUnit();
	SetText();

	InitialDlgData();
	SetAllControlEnable(m_pParent->m_bJTabEnable);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnStlRatingShearConnTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CDgnStlRatingShearConnTabJDlg::SetInitUnit()
{
	m_iNumStudWeb.SetUnitType(D_UNITSYS_NONE);
	m_dLSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dPim.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_dPr.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_dMaxRatio.SetUnitType(D_UNITSYS_NONE);
	m_dNa.SetUnitType(D_UNITSYS_NONE);

	m_dLSpacingUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dPimUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_dPrUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
}

void CDgnStlRatingShearConnTabJDlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CDgnStlRatingShearConnTabJDlg::Data2Dlg()
{
	m_iNumStudWeb.SetEditUnit(m_Data.nNumStudWeb);
	m_dLSpacing.SetEditUnit(m_Data.dLSpacing);
	m_dPim.SetEditUnit(m_Data.dPim);
	m_dPr.SetEditUnit(m_Data.dPr);
	m_dMaxRatio.SetEditUnit(m_Data.dMaxRatio);
	m_dNa.SetEditUnit(m_Data.dNa);

	UpdateData(FALSE);

	SetAllControlEnable(m_pParent->m_bJTabEnable);

	return TRUE;
}

BOOL CDgnStlRatingShearConnTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.nNumStudWeb = m_iNumStudWeb.GetEditValue();
	m_Data.dLSpacing = m_dLSpacing.GetEditValue();
	m_Data.dPim = m_dPim.GetEditValue();
	m_Data.dPr = m_dPr.GetEditValue();
	m_Data.dMaxRatio = m_dMaxRatio.GetEditValue();
	m_Data.dNa = m_dNa.GetEditValue();

	return TRUE;
}

void CDgnStlRatingShearConnTabJDlg::SetText()
{
	GetDlgItem(IDC_DGN_NUM_STUDS_WEB_STC)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_NUM_STUDS_PER_WEB));
	GetDlgItem(IDC_DGN_L_SPACE_STC)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_L_SPACING));
	GetDlgItem(IDC_DGN_PIM_STC)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_PIM));
	GetDlgItem(IDC_DGN_PR_STC)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_PR));
	GetDlgItem(IDC_DGN_MAX_RATIO_STC)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_MAX_RATIO));
	GetDlgItem(IDC_DGN_NA_STC)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_NA));
}

void CDgnStlRatingShearConnTabJDlg::GetSrscData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CDgnStlRatingShearConnTabJDlg::SetSrscData()
{
	Data2Dlg();
}

void CDgnStlRatingShearConnTabJDlg::SetAllControlEnable(bool enable)
{
	GetDlgItem(IDC_DGN_NUM_STUDS_WEB_STC)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_NUM_STUDS_WEB_EDT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_L_SPACE_STC)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_L_SPACE_EDT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_L_SPACE_UNT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_PIM_STC)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_PIM_EDT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_PIM_UNT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_PR_STC)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_PR_EDT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_PR_UNT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_MAX_RATIO_STC)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_MAX_RATIO_EDT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_NA_STC)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_NA_EDT)->EnableWindow(enable);
}

