// CMWindItemJapan2004GustF.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemJapan2004GustF.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004GustF dialog


CCMWindItemJapan2004GustF::CCMWindItemJapan2004GustF(T_WIND_JP2004* pData, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMWindItemJapan2004GustF::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMWindItemJapan2004GustF)
	//}}AFX_DATA_INIT
	m_pData = 0;
	ASSERT(pData);
	m_pData = pData;
}


void CCMWindItemJapan2004GustF::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemJapan2004GustF)
	DDX_Control(pDX, IDC_CMD_MODE_Y_EDIT, m_editModeY);
	DDX_Control(pDX, IDC_CMD_MODE_X_EDIT, m_editModeX);
	DDX_Control(pDX, IDC_CMD_GUST_Y_EDIT, m_editGustY);
	DDX_Control(pDX, IDC_CMD_GUST_X_EDIT, m_editGustX);
	DDX_Control(pDX, IDC_CMD_FREQ_Y_EDIT, m_editFreqY);
	DDX_Control(pDX, IDC_CMD_FREQ_X_EDIT, m_editFreqX);
	DDX_Control(pDX, IDC_CMD_DAMP_Y_EDIT, m_editDampY);
	DDX_Control(pDX, IDC_CMD_DAMP_X_EDIT, m_editDampX);
	DDX_Control(pDX, IDC_CMD_BREADTH_Y_UNIT, m_unitBreadthY);
	DDX_Control(pDX, IDC_CMD_BREADTH_Y_EDIT, m_editBreadthY);
	DDX_Control(pDX, IDC_CMD_BREADTH_X_UNIT, m_unitBreadthX);
	DDX_Control(pDX, IDC_CMD_BREADTH_X_EDIT, m_editBreadthX);
	DDX_Control(pDX, IDC_CMD_TOPO_Y_EDIT, m_editTopoY);
	DDX_Control(pDX, IDC_CMD_TOPO_X_EDIT, m_editTopoX);
	DDX_Control(pDX, IDC_CMD_HEIGHT_UNIT, m_unitHeight);
	DDX_Control(pDX, IDC_CMD_HEIGHT_EDIT, m_editHeight);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COBX, m_editCategory);
	DDX_Control(pDX, IDC_CMD_SPEED_Y_EDIT, m_editSpeedY);
	DDX_Control(pDX, IDC_CMD_SPEED_X_EDIT, m_editSpeedX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemJapan2004GustF, CDialogMove)
	//{{AFX_MSG_MAP(CCMWindItemJapan2004GustF)
	ON_BN_CLICKED(IDC_CMD_CALC_BTN, OnCmdCalcBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004GustF message handlers

BOOL CCMWindItemJapan2004GustF::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemJapan2004GustF::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}

void CCMWindItemJapan2004GustF::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CCMWindItemJapan2004GustF::OnCmdCalcBtn() 
{
	// TODO: Add your control notification handler code here
	CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
	ASSERT(pUnitCtrl);

	double dDesignWindSpeedX = m_editSpeedX.GetEditValue();
	double dDesignWindSpeedY = m_editSpeedY.GetEditValue();

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
	if (!load.CalcJP2004GustFactor(dDesignWindSpeedX, dDesignWindSpeedY, m_pData->nExposureCategory,
																 dBuildingHeight, dBreadthX, dBreadthY, 
																 m_pData->dFrequencyX, m_pData->dFrequencyY, 
																 m_pData->dDampingX, m_pData->dDampingY,
																 m_pData->dModeFactorX, m_pData->dModeFactorY, 
																 m_pData->bTopographyX, m_pData->bTopographyY,
																 m_pData->nHillShapeX, m_pData->nHillShapeY, 
																 dHillHeightX, dHillHeightY,
																 dHillLengthX, dHillLengthY, 
																 dBldgDistanceX, dBldgDistanceY,
																 m_pData->dGustFactorX, m_pData->dGustFactorY)) 
	{
		AfxMessageBox(_LS(IDS_CMD_ERROR_CANNOT_CALCULATE));
		return;
	}

	m_editGustX.SetEditUnit(m_pData->dGustFactorX);
	m_editGustY.SetEditUnit(m_pData->dGustFactorY);
}

//---------------------------------------------------------------------------
void CCMWindItemJapan2004GustF::InitControls()
{
	m_editModeY.SetUnitType(D_UNITSYS_NONE);
	m_editModeX.SetUnitType(D_UNITSYS_NONE);
	m_editGustY.SetUnitType(D_UNITSYS_NONE);
	m_editGustX.SetUnitType(D_UNITSYS_NONE);
	m_editFreqY.SetUnitType(D_UNITSYS_NONE);
	m_editFreqX.SetUnitType(D_UNITSYS_NONE);
	m_editDampY.SetUnitType(D_UNITSYS_NONE);
	m_editDampX.SetUnitType(D_UNITSYS_NONE);
	m_unitBreadthY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editBreadthY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitBreadthX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editBreadthX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editSpeedY.SetUnitType(D_UNITSYS_NONE);
	m_editSpeedX.SetUnitType(D_UNITSYS_NONE);
}

void CCMWindItemJapan2004GustF::Data2Dlg()
{
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

	CString aCategory[] = { _T("I"), _T("II"), _T("III"), _T("IV"), _T("V") };
	CString aTopoEffect[] = { _LS(IDS_CMD_WIND_NBC95_NOT_INCLUDED), _LS(IDS_CMD_WIND_NBC95_INCLUDED) };

	m_editSpeedX.SetEditUnit(dUhx);
	m_editSpeedY.SetEditUnit(dUhy);
	m_editCategory.SetWindowText(aCategory[m_pData->nExposureCategory]);
	m_editHeight.SetEditUnit(m_pData->dBuildingHeight);
	int ix = (m_pData->bTopographyX) ? 1 : 0;
	m_editTopoX.SetWindowText(aTopoEffect[ix]);
	ix = (m_pData->bTopographyY) ? 1 : 0;
	m_editTopoY.SetWindowText(aTopoEffect[ix]);
	m_editBreadthX.SetEditUnit(m_pData->dBreadthX);
	m_editBreadthY.SetEditUnit(m_pData->dBreadthY);
	m_editDampX.SetEditUnit(m_pData->dDampingX);
	m_editDampY.SetEditUnit(m_pData->dDampingY);
	m_editFreqX.SetEditUnit(m_pData->dFrequencyX);
	m_editFreqY.SetEditUnit(m_pData->dFrequencyY);
	m_editModeX.SetEditUnit(m_pData->dModeFactorX);
	m_editModeY.SetEditUnit(m_pData->dModeFactorY);
}

BOOL CCMWindItemJapan2004GustF::Dlg2Data()
{
	m_pData->dGustFactorX = m_editGustX.GetEditValue();
	m_pData->dGustFactorY = m_editGustY.GetEditValue();
	return TRUE;
}

