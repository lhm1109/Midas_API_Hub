// BndrSospUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrIntegralPileDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_SpecialChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralPileDlg dialog


CBndrIntegralPileDlg::CBndrIntegralPileDlg(SpringPile* pData,CWnd* pParent /*=NULL*/)
	: CDlgChild(CBndrIntegralPileDlg::IDD, pParent)
{
	m_pData=pData;

	m_arSand.Add(IDC_TM_INANGLE_STC); m_arSand.Add(IDC_TM_INANGLE_EDT); m_arSand.Add(IDC_TM_INANGLE_UNT);
	m_arSand.Add(IDC_TM_K1_STC);      m_arSand.Add(IDC_TM_K1_CMB);      m_arSand.Add(IDC_TM_K1_EDT);       
	m_arSand.Add(IDC_TM_K1_UNT);

	m_arClay.Add(IDC_TM_TRIAXIAL_STC);  m_arClay.Add(IDC_TM_TRIAXIAL_EDT);
	m_arClay.Add(IDC_TM_CU_STC);        m_arClay.Add(IDC_TM_CU_EDT);        m_arClay.Add(IDC_TM_CU_UNT);

	//{{AFX_DATA_INIT(CBndrIntegralPileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBndrIntegralPileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrIntegralPileDlg)
	DDX_Control(pDX, IDC_TM_TYPE_CMB ,m_cmbType);
	DDX_Control(pDX, IDC_TM_K1_CMB ,m_cmbK1); 
	
	DDX_Control(pDX, IDC_TM_GLEVEL_EDT ,m_edtGroundLevel);
	DDX_Control(pDX, IDC_TM_SWEIGHT_EDT ,m_edtUnitSoilWeight);
	DDX_Control(pDX, IDC_TM_EARTH_EDT ,m_edtEarthCoeff);
	DDX_Control(pDX, IDC_TM_KH_EDT ,m_edtKH);
	DDX_Control(pDX, IDC_TM_CU_EDT ,m_edtCU);
	DDX_Control(pDX, IDC_TM_INANGLE_EDT ,m_edtInternalAngle);
	DDX_Control(pDX, IDC_TM_PILEDIA_EDT ,m_edtPileDiameter);
	DDX_Control(pDX, IDC_TM_K1_EDT ,m_edtK1);
	DDX_Control(pDX, IDC_TM_TRIAXIAL_EDT ,m_edtTriAxial);

	DDX_Control(pDX, IDC_TM_GLEVEL_UNT ,m_untGroundLevel);
	DDX_Control(pDX, IDC_TM_SWEIGHT_UNT ,m_untUnitSoilWeight);
	DDX_Control(pDX, IDC_TM_CU_UNT ,m_untCU);
	DDX_Control(pDX, IDC_TM_INANGLE_UNT ,m_untInternalAngle);
	DDX_Control(pDX, IDC_TM_PILEDIA_UNT ,m_untPileDiameter);
	DDX_Control(pDX, IDC_TM_K1_UNT ,m_untK1);
	DDX_Control(pDX, IDC_TM_KH_UNT ,m_untKH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrIntegralPileDlg, CDlgChild)
	//{{AFX_MSG_MAP(CBndrIntegralPileDlg)
	ON_CBN_SELCHANGE(IDC_TM_TYPE_CMB, OnSelchangeTypeCombo)
	ON_CBN_SELCHANGE(IDC_TM_K1_CMB, OnSelchangeK1Combo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralPileDlg message handlers

BOOL CBndrIntegralPileDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CBndrIntegralPileDlg::InitSubCtrl()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	// special character
	CString strAngle;
	GetDlgItem(IDC_TM_INANGLE_STC)->GetWindowText(strAngle);
	strAngle += _T(" (") + D_CH_PHI + _T(")");
	GetDlgItem(IDC_TM_INANGLE_STC)->SetWindowText(strAngle);

	// init unit edit
	m_edtGroundLevel.SetAttWcsPos();
	m_edtGroundLevel.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);  // Default : Global (-Z)
	m_edtGroundLevel.SetWindowText(_T("0"));
	m_edtUnitSoilWeight.SetUnitType(D_UNITSYS_BASE_DENSITY);
	double dInitSoilWeight = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M,
																																				D_UNITSYS_BASE_DENSITY, 19.0);
	m_edtUnitSoilWeight.SetEditUnit(dInitSoilWeight);
	m_edtEarthCoeff.SetUnitType(0);
	m_edtEarthCoeff.SetEditUnit(0.4);
	m_edtKH.SetUnitType(0);
	m_edtCU.SetUnitType(D_UNITSYS_BASE_STRESS);
	double dInitCU = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M,
																																				D_UNITSYS_BASE_STRESS, 5.0);
	m_edtCU.SetEditUnit(dInitCU); 
	m_edtInternalAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_edtInternalAngle.SetEditUnit(30);
	m_edtPileDiameter.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtK1.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_edtTriAxial.SetUnitType(0);
	m_edtTriAxial.SetEditUnit(0.01);
	m_edtKH.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_edtKH.SetEditUnit(0);

	m_untGroundLevel.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untUnitSoilWeight.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_untCU.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untInternalAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untPileDiameter.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untK1.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_untKH.SetUnitType(D_UNITSYS_BASE_DENSITY);
	
	// init combo
	m_cmbType.ResetContent();
	m_cmbType.AddString(_LS(IDS_TM_PILE_SAND));       m_cmbType.SetItemData(0,0);
	m_cmbType.AddString(_LS(IDS_TM_PILE_SOFTCLAY));   m_cmbType.SetItemData(1,1);
	m_cmbType.AddString(_LS(IDS_TM_PILE_STIFFCLAY));  m_cmbType.SetItemData(2,2);
	m_cmbType.SetCurSel(0);

	m_cmbK1.ResetContent();
	m_cmbK1.AddString(_LS(IDS_TM_PILE_NONE));    m_cmbK1.SetItemData(0,0);
	m_cmbK1.AddString(_LS(IDS_TM_PILE_LOOSE));   m_cmbK1.SetItemData(1,1);
	m_cmbK1.AddString(_LS(IDS_TM_PILE_MEDIUM));  m_cmbK1.SetItemData(2,2);
	m_cmbK1.AddString(_LS(IDS_TM_PILE_DENSE));   m_cmbK1.SetItemData(3,3);
	m_cmbK1.SetCurSel(3); // dense
	OnSelchangeK1Combo();

	// move and resize
	CRect rDest, rSrc;
	GetDlgItem(IDC_TM_EARTH_STC)->GetWindowRect(rDest);
	GetDlgItem(IDC_TM_TRIAXIAL_STC)->GetWindowRect(rSrc);

	CDlgUtil::CtrlMoveDistX(this, m_arClay, rDest.left-rSrc.left); // move clay group
	
	OnSelchangeTypeCombo();
}

BOOL CBndrIntegralPileDlg::GetParameter()
{
	m_pData->nSType = m_cmbType.GetCurSel();
	if (!CFormulaEdit::GetEditValue(&m_edtGroundLevel, m_pData->dGroundLevel))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_reference_level_));
		return FALSE;
	}
	CFormulaEdit::GetEditValue(&m_edtPileDiameter, m_pData->dPileDiameter);
	CFormulaEdit::GetEditValue(&m_edtUnitSoilWeight, m_pData->dUnitSoilWeight);
	CFormulaEdit::GetEditValue(&m_edtEarthCoeff, m_pData->dK0);
	CFormulaEdit::GetEditValue(&m_edtKH, m_pData->dKH);
	
	// sand
	m_pData->nKType = m_cmbK1.GetCurSel();
	CFormulaEdit::GetEditValue(&m_edtK1, m_pData->dK1);
	CFormulaEdit::GetEditValue(&m_edtInternalAngle, m_pData->dInternalAngle);
	
	// clay
	CFormulaEdit::GetEditValue(&m_edtTriAxial, m_pData->dTriAxial);
	CFormulaEdit::GetEditValue(&m_edtCU, m_pData->dCU);

	return TRUE;
}

void CBndrIntegralPileDlg::OnSelchangeTypeCombo() // sand, soft clay, stiff clay
{
	int nSel = m_cmbType.GetCurSel();
	CDlgUtil::CtrlShowHide(this, m_arSand, nSel == 0);
	CDlgUtil::CtrlShowHide(this, m_arClay, nSel != 0);
}

void CBndrIntegralPileDlg::OnSelchangeK1Combo() // loose, medium, dense
{
	int nSel = m_cmbK1.GetCurSel();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	double K1 = 0.0;
	if(nSel == 1) 
		K1 = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M,
																												D_UNITSYS_BASE_DENSITY, 5.43E06);
	else if(nSel == 2) 
		K1 = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M,
																												D_UNITSYS_BASE_DENSITY, 16.29E06);
	else if(nSel == 3)
		K1 = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M,
																												D_UNITSYS_BASE_DENSITY, 33.93E06);
	if (nSel != 0) m_edtK1.SetEditUnit(K1);

	m_edtK1.SetReadOnly(nSel != 0);
}

void CBndrIntegralPileDlg::PostNcDestroy() 
{
	CDlgChild::PostNcDestroy();
}

BOOL CBndrIntegralPileDlg::GetPosition(CString& strPos    ,double& Px ,double& Py ,double& Pz)
{
	MyParser Parser;
	if(Parser.ParsingPositionArg(strPos))
	{
		Parser.GetPositionData(Px,Py,Pz);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
