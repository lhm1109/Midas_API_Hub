// CMWindItemJapan2004LoadF.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemJapan2004LoadF.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004LoadF dialog


CCMWindItemJapan2004LoadF::CCMWindItemJapan2004LoadF(T_WIND_JP2004* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMWindItemJapan2004LoadF::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMWindItemJapan2004LoadF)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pData = 0;
	ASSERT(pData);
	m_pData = pData;

	m_aCtrlWindDir.Add(IDC_CMD_WINDDIR_X_RD);
	m_aCtrlWindDir.Add(IDC_CMD_WINDDIR_Y_RD);

	m_aCtrlCombType.Add(IDC_CMD_COMBTYPE1_RD);
	m_aCtrlCombType.Add(IDC_CMD_COMBTYPE2_RD);
	m_aCtrlCombType.Add(IDC_CMD_COMBTYPE3_RD);
	m_aCtrlCombType.Add(IDC_CMD_COMBTYPE4_RD);
}


void CCMWindItemJapan2004LoadF::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemJapan2004LoadF)
	DDX_Control(pDX, IDC_CMD_FREQ_Z_EDIT, m_editFreqZ);
	DDX_Control(pDX, IDC_CMD_FREQ_Y_EDIT, m_editFreqY);
	DDX_Control(pDX, IDC_CMD_FREQ_X_EDIT, m_editFreqX);
	DDX_Control(pDX, IDC_CMD_GUST_Y_EDIT, m_editGustY);
	DDX_Control(pDX, IDC_CMD_GUST_X_EDIT, m_editGustX);
	DDX_Control(pDX, IDC_CMD_BREADTH_Y_UNIT, m_unitBreadthY);
	DDX_Control(pDX, IDC_CMD_BREADTH_Y_EDIT, m_editBreadthY);
	DDX_Control(pDX, IDC_CMD_BREADTH_X_UNIT, m_unitBreadthX);
	DDX_Control(pDX, IDC_CMD_BREADTH_X_EDIT, m_editBreadthX);
	DDX_Control(pDX, IDC_CMD_HEIGHT_UNIT, m_unitHeight);
	DDX_Control(pDX, IDC_CMD_HEIGHT_EDIT, m_editHeight);
	DDX_Control(pDX, IDC_CMD_SPEED_Y_EDIT, m_editSpeedY);
	DDX_Control(pDX, IDC_CMD_SPEED_X_EDIT, m_editSpeedX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemJapan2004LoadF, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMWindItemJapan2004LoadF)
	ON_BN_CLICKED(IDC_CMD_CALC_BTN, OnCmdCalcBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004LoadF message handlers

BOOL CCMWindItemJapan2004LoadF::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemJapan2004LoadF::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	CInternationalDlg::OnOK();
}

void CCMWindItemJapan2004LoadF::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CInternationalDlg::OnCancel();
}

void CCMWindItemJapan2004LoadF::OnCmdCalcBtn() 
{
	// TODO: Add your control notification handler code here
	CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
	ASSERT(pUnitCtrl);

	double dLoadFactorX, dLoadFactorY, dLoadFactorR;
	BOOL   bLoadFactorX, bLoadFactorY, bLoadFactorR;

	int nWindDirection, nCombType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlWindDir, nWindDirection);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCombType, nCombType);
	double dDesignWindSpeedX = m_editSpeedX.GetEditValue();
	double dDesignWindSpeedY = m_editSpeedY.GetEditValue();
	double dBuildingHeight = pUnitCtrl->ConvertUnitLengthIn(m_pData->dBuildingHeight);
	double dBreadthX = pUnitCtrl->ConvertUnitLengthIn(m_pData->dBreadthX);
	double dBreadthY = pUnitCtrl->ConvertUnitLengthIn(m_pData->dBreadthY);
	
	if(nCombType == 0 || nCombType == 1 || nCombType == 2)
	{
		if(dDesignWindSpeedX <= 0. || dDesignWindSpeedY <= 0. || 
			 dBuildingHeight <= 0. || dBreadthX <= 0. || dBreadthY <= 0. || 
			 m_pData->dGustFactorX <= 0. || m_pData->dGustFactorY <= 0. ||
			 m_pData->dFrequencyX <= 0. || m_pData->dFrequencyY <= 0. || m_pData->dFrequencyR <= 0. ||
			 m_pData->dDampingX <= 0. || m_pData->dDampingY <= 0. || m_pData->dDampingR <= 0.)
		{
			AfxMessageBox(_LS(IDS_CMD_ERROR_CANNOT_CALCULATE));
			return;
		}
	}
	else if(nCombType == 3)
	{
		if(dBuildingHeight <= 0. || dBreadthX <= 0. || dBreadthY <= 0.)
		{
			AfxMessageBox(_LS(IDS_CMD_ERROR_CANNOT_CALCULATE));
			return;
		}
	}
	else ASSERT(FALSE);

	double dCriteria = dBuildingHeight / sqrt(dBreadthX * dBreadthY);
	if(nCombType == 0 || nCombType == 1 || nCombType == 2)
	{
		if(dCriteria < 3.0) AfxMessageBox(_LS(IDS_CMD_JP2004_TYPE_MISMATCH1));  // [주의] Combination Type4가 적합한 건물 형상입니다.
	}
	else if(nCombType == 3)
	{
		if(dCriteria >= 3.0) AfxMessageBox(_LS(IDS_CMD_JP2004_TYPE_MISMATCH2)); // [주의] Combination Type4가 적합하지 않은 건물 형상입니다.
	}
	else ASSERT(FALSE);


	CLateralLoad load;
	if (!load.CalcJP2004LoadFactor(nWindDirection, nCombType, dDesignWindSpeedX, dDesignWindSpeedY,
																 dBuildingHeight, dBreadthX, dBreadthY,
																 m_pData->dGustFactorX, m_pData->dGustFactorY,
																 m_pData->dFrequencyX, m_pData->dFrequencyY, m_pData->dFrequencyR,
																 m_pData->dDampingX, m_pData->dDampingY, m_pData->dDampingR,                                 
																 bLoadFactorX, bLoadFactorY, bLoadFactorR,
																 dLoadFactorX, dLoadFactorY, dLoadFactorR))
	{
		AfxMessageBox(_LS(IDS_CMD_ERROR_CANNOT_CALCULATE));
		return;
	}

	CString strValue;
	strValue.Format(_T("%g"), dLoadFactorX);
	GetDlgItem(IDC_CMD_COMB11_EDIT)->SetWindowText(strValue);
	strValue.Format(_T("%g"), dLoadFactorY);
	GetDlgItem(IDC_CMD_COMB12_EDIT)->SetWindowText(strValue);
	strValue.Format(_T("%g"), dLoadFactorR);
	GetDlgItem(IDC_CMD_COMB13_EDIT)->SetWindowText(strValue);
	strValue = bLoadFactorX ? _LS(IDS_WG_CMD__ADDD__Consider) : _LS(IDS_WG_CMD__ADDD__Do_not_consider);
	GetDlgItem(IDC_CMD_COMB21_EDIT)->SetWindowText(strValue);
	strValue = bLoadFactorY ? _LS(IDS_WG_CMD__ADDD__Consider) : _LS(IDS_WG_CMD__ADDD__Do_not_consider);
	GetDlgItem(IDC_CMD_COMB22_EDIT)->SetWindowText(strValue);
	strValue = bLoadFactorR ? _LS(IDS_WG_CMD__ADDD__Consider) : _LS(IDS_WG_CMD__ADDD__Do_not_consider);
	GetDlgItem(IDC_CMD_COMB23_EDIT)->SetWindowText(strValue);
 
	m_pData->dLoadFactorX = dLoadFactorX;
	m_pData->dLoadFactorY = dLoadFactorY;
	m_pData->dLoadFactorR = dLoadFactorR;
}

//--------------------------------------------------------------------------------
void CCMWindItemJapan2004LoadF::InitControls()
{
	m_editFreqZ.SetUnitType(D_UNITSYS_NONE);
	m_editFreqY.SetUnitType(D_UNITSYS_NONE);
	m_editFreqX.SetUnitType(D_UNITSYS_NONE);
	m_editGustY.SetUnitType(D_UNITSYS_NONE);
	m_editGustX.SetUnitType(D_UNITSYS_NONE);
	m_unitBreadthY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editBreadthY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitBreadthX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editBreadthX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editSpeedY.SetUnitType(D_UNITSYS_NONE);
	m_editSpeedX.SetUnitType(D_UNITSYS_NONE);

	// Combination Type 표시 
	GetDlgItem(IDC_CMD_TYPE1_WDIR_EDIT)->SetWindowText(_T("1.0"));
	GetDlgItem(IDC_CMD_TYPE1_NDIR_EDIT)->SetWindowText(_T("0.4"));
	GetDlgItem(IDC_CMD_TYPE1_TDIR_EDIT)->SetWindowText(_T("0.4"));
	GetDlgItem(IDC_CMD_TYPE2_WDIR_EDIT)->SetWindowText(_T("(0.4*0.6/Gd)"));
	GetDlgItem(IDC_CMD_TYPE2_NDIR_EDIT)->SetWindowText(_T("1.0"));
	GetDlgItem(IDC_CMD_TYPE2_TDIR_EDIT)->SetWindowText(_LS(IDS_CMD_JP2004_LOADFACTOR_RHO)); //_T("Sqrt(2+2ρ)-1");
	GetDlgItem(IDC_CMD_TYPE3_WDIR_EDIT)->SetWindowText(_T("(0.4*0.6/Gd)"));
	GetDlgItem(IDC_CMD_TYPE3_NDIR_EDIT)->SetWindowText(_LS(IDS_CMD_JP2004_LOADFACTOR_RHO)); //_T("Sqrt(2+2ρ)-1");
	GetDlgItem(IDC_CMD_TYPE3_TDIR_EDIT)->SetWindowText(_T("1.0"));
	GetDlgItem(IDC_CMD_TYPE4_WDIR_EDIT)->SetWindowText(_T("1.0"));
	GetDlgItem(IDC_CMD_TYPE4_NDIR_EDIT)->SetWindowText(_T("max(0.35D/B, 0.2)"));
//  GetDlgItem(IDC_CMD_TYPE4_NDIR_EDIT)->SetWindowText(_LS(IDS_CMD_JP2004_LOADFACTOR_GAMMA)); //_T("Sqrt(2+2ρ)-1");  
}

void CCMWindItemJapan2004LoadF::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlWindDir, m_pData->nWindDirection);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCombType, m_pData->nCombType);

	CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
	ASSERT(pUnitCtrl);
	
	double dBuildingHeight = pUnitCtrl->ConvertUnitLengthIn(m_pData->dBuildingHeight);
	double dBreadthX = pUnitCtrl->ConvertUnitLengthIn(m_pData->dBreadthX);
	double dBreadthY = pUnitCtrl->ConvertUnitLengthIn(m_pData->dBreadthY);
	double dHillHeightX = pUnitCtrl->ConvertUnitLengthIn(m_pData->dHillHeightX);
	double dHillHeightY = pUnitCtrl->ConvertUnitLengthIn(m_pData->dHillHeightY);
	double dHillLengthX = pUnitCtrl->ConvertUnitLengthIn(m_pData->dHillLengthX);
	double dHillLengthY = pUnitCtrl->ConvertUnitLengthIn(m_pData->dHillLengthY);
	double dBldgDistanceX = pUnitCtrl->ConvertUnitLengthIn(m_pData->dBldgDistanceX);
	double dBldgDistanceY = pUnitCtrl->ConvertUnitLengthIn(m_pData->dBldgDistanceY);

	CLateralLoad load;
	double dUhx = load.CalcJP2004DesignWindSpeed(m_pData->dBasicWindSpeed, m_pData->dDirectionFactorX, m_pData->dMeanWindSpeed, 
																							 dBuildingHeight, m_pData->nExposureCategory, m_pData->bTopographyX, 
																							 m_pData->nHillShapeX, dHillHeightX, dHillLengthX, dBldgDistanceX);
	double dUhy = load.CalcJP2004DesignWindSpeed(m_pData->dBasicWindSpeed, m_pData->dDirectionFactorY, m_pData->dMeanWindSpeed, 
																							 dBuildingHeight, m_pData->nExposureCategory, m_pData->bTopographyY, 
																							 m_pData->nHillShapeY, dHillHeightY, dHillLengthY, dBldgDistanceY);

	m_editSpeedX.SetEditUnit(dUhx);
	m_editSpeedY.SetEditUnit(dUhy);
	m_editHeight.SetEditUnit(m_pData->dBuildingHeight);
	m_editBreadthX.SetEditUnit(m_pData->dBreadthX);
	m_editBreadthY.SetEditUnit(m_pData->dBreadthY);
	m_editGustX.SetEditUnit(m_pData->dGustFactorX);
	m_editGustY.SetEditUnit(m_pData->dGustFactorY);
	m_editFreqX.SetEditUnit(m_pData->dFrequencyX);
	m_editFreqY.SetEditUnit(m_pData->dFrequencyY);
	m_editFreqZ.SetEditUnit(m_pData->dFrequencyR);
}

BOOL CCMWindItemJapan2004LoadF::Dlg2Data()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlWindDir, m_pData->nWindDirection);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCombType, m_pData->nCombType);
	return TRUE;
}
