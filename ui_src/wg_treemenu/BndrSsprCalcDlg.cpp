// BndrSsprCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrSsprCalcDlg.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBndrSsprCalcDlg dialog
CBndrSsprCalcDlg::CBndrSsprCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CBndrSsprCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrSsprCalcDlg)
	m_nAxis = 0;
	m_soiltype = 0;
	m_supporttype=0;
	//}}AFX_DATA_INIT
	m_group1.RemoveAll();
	m_group2.RemoveAll();
	m_group3.RemoveAll();

	m_group1.Add(IDC_TM_STATIC3);
	m_group1.Add(IDC_TM_SS_STRESS);
	m_group1.Add(IDC_TM_SS_STRESS_UNIT);

	m_group2.Add(IDC_TM_SS_MODULUS2);
	m_group3.Add(IDC_TM_SS_MODULUS);
	m_bInit=FALSE;
	m_bUseAxis = FALSE;
}


void CBndrSsprCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrSsprCalcDlg)
	DDX_Radio  (pDX, IDC_TM_SS_AXIS_X_RDO, m_nAxis);
	DDX_Control(pDX, IDC_TM_SS_MODULUS2, m_modulus2);
	DDX_Control(pDX, IDC_TM_SS_STRESS_UNIT, m_stressunit);
	DDX_Control(pDX, IDC_TM_SS_STRESS, m_stress);
	DDX_Control(pDX, IDC_TM_SS_MODULUS_UNIT, m_modulusunit);
	DDX_Control(pDX, IDC_TM_SS_MODULUS, m_modulus);
	DDX_Control(pDX, IDC_TM_SS_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBndrSsprCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CBndrSsprCalcDlg)
	ON_BN_CLICKED(IDC_TM_SS_AXIS_X_RDO, OnChangeCmdSsAxis)
	ON_BN_CLICKED(IDC_TM_SS_AXIS_Y_RDO, OnChangeCmdSsAxis)
	ON_BN_CLICKED(IDC_TM_SS_AXIS_Z_RDO, OnChangeCmdSsAxis)
	ON_EN_CHANGE(IDC_TM_SS_STRESS, OnChangeCmdSsStress)
	ON_BN_CLICKED(IDC_TM_SS_CALC_BTN, OnCalcualteBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBndrSsprCalcDlg::Dlg2Data()
{
	UpdateData(TRUE);

	if(m_supporttype == 0)
		m_pData->dModulus = m_modulus2.GetEditValue();
	else
		m_pData->dModulus = m_modulus.GetEditValue();
	m_pData->dStress = m_stress.GetEditValue();

	m_pData->nSoilType = m_soiltype + 1;
	m_pData->nSupportType = m_supporttype + 1;
	m_pData->nAxis = m_nAxis;

	return TRUE;
}

BOOL CBndrSsprCalcDlg::Data2Dlg()
{
	if(m_pData->nSupportType == 1)
		m_modulus2.SetEditUnit(m_pData->dModulus);
	else
		m_modulus.SetEditUnit(m_pData->dModulus);
	m_stress.SetEditUnit(m_pData->dStress);

	m_soiltype = m_pData->nSoilType - 1;
	m_supporttype = m_pData->nSupportType - 1;
	m_nAxis = m_pData->nAxis;

	UpdateData(FALSE);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBndrSsprCalcDlg message handlers
BOOL CBndrSsprCalcDlg::OnInitDialog() 
{
	AlignControl();
	CDialogMove::OnInitDialog();

	ASSERT(m_pData);

	// TODO: Add extra initialization here
	// set unit/edit //////////////////////////////
	m_modulus.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_modulusunit.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_stress.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_stressunit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_modulus2.SetUnitType(D_UNITSYS_BASE_DENSITY);

	m_modulus2.SetTextFormat(_T("%.4g"));
	m_stress.SetTextFormat(_T("%.4g"));
	m_modulus.SetTextFormat(_T("%.4g"));

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\suppsoil.svg"));
	///////////////////////////////////////////////

	if(!m_bUseAxis) 
	{
		m_nAxis = 2;
		m_pData->nAxis = 2;
	}
	
	Data2Dlg();
	SetSoilType();
	ChangeText();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrSsprCalcDlg::AlignControl()
{
	CRect rectG1, rectG2;
	((CBCGPStatic*)GetDlgItem(IDC_TM_SS_MODULUS))->GetWindowRect(rectG1);
	((CBCGPStatic*)GetDlgItem(IDC_TM_SS_MODULUS2))->GetWindowRect(rectG2);

	int nG2DistX = rectG1.left - rectG2.left;
	int nG2DistY = rectG1.top - rectG2.top;

	CDlgUtil::CtrlMoveDistXY(this, m_group2, nG2DistX, nG2DistY);

	CDlgUtil::CtrlShowHide(this, m_group2, TRUE);
	CDlgUtil::CtrlShowHide(this, m_group3, FALSE);
}

void CBndrSsprCalcDlg::SetSoilType()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_group1, TRUE);
	CDlgUtil::CtrlShowHide(this, m_group2, TRUE);
	CDlgUtil::CtrlShowHide(this, m_group3, FALSE);

	GetDlgItem(IDC_TM_SS_AXIS_X_RDO)->EnableWindow(m_bUseAxis);
	GetDlgItem(IDC_TM_SS_AXIS_Y_RDO)->EnableWindow(m_bUseAxis);
}

void CBndrSsprCalcDlg::ChangeText()
{
	CString strSubModulus;

	if     (m_nAxis == 0) strSubModulus = _LS(IDS_WG_TREEMENU_SSPR_CALC_SUB_MODULUS_X);
	else if(m_nAxis == 1) strSubModulus = _LS(IDS_WG_TREEMENU_SSPR_CALC_SUB_MODULUS_Y);
	else                  strSubModulus = _LS(IDS_WG_TREEMENU_SSPR_CALC_SUB_MODULUS_Z);

	GetDlgItem(IDC_TM_STATIC4)->SetWindowText(strSubModulus);
}

void CBndrSsprCalcDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CBndrSsprCalcDlg::OnChangeCmdSsAxis() 
{
	UpdateData(TRUE);
	ChangeText();
}

void CBndrSsprCalcDlg::OnChangeCmdSsStress() 
{

}

void CBndrSsprCalcDlg::CalcModulus() 
{
	m_pData->dStress = m_stress.GetEditValue();
	CalcSpring(m_pData);

	CString str;
	str.Format(_T("%g"), m_pData->dModulus);
	m_modulus2.SetWindowText(str);

	m_pData->dModulus = m_modulus2.GetEditValue();
}

BOOL CBndrSsprCalcDlg::CalcSpring(T_SOSD_DATA* pData)
{
	if(pData->nSupportType != 1)return TRUE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	UnitIndexNew=UnitIndexOld;
	UnitIndexNew.nBase_Length=D_UNITSYS_LENGTH_INDEX_M;
	UnitIndexNew.nBase_Force=D_UNITSYS_FORCE_INDEX_KN;

	pDoc->m_pUnitCtrl->SaveUnitChangeFactor();
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew);

	double qa=pData->dStress;
	qa=pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_STRESS, qa);
	pData->dModulus=40.*3.0*qa;
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld);
	pData->dModulus=pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_DENSITY, pData->dModulus);

	pDoc->m_pUnitCtrl->RestoreUnitChangeFactor();

	return TRUE;
}

void CBndrSsprCalcDlg::SetSpringType(int nType)
{
	m_soiltype = nType;
}

void CBndrSsprCalcDlg::SetUseAxisOpt(BOOL bUseAxis)
{
	m_bUseAxis = bUseAxis;
}

double CBndrSsprCalcDlg::GetCalcValue()
{
	return m_pData->dModulus;
}

void CBndrSsprCalcDlg::OnCalcualteBtn() 
{
	CalcModulus();
}