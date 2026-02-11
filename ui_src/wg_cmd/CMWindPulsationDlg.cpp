// CMSpecModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindPulsationDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "SP20_WindPulsation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPulsationDlg dialog

CCMWindPulsationDlg::CCMWindPulsationDlg(CDBDoc* pDoc, T_KEY KeyWind, CWnd* pParent)
	:m_pDoc{ pDoc }, m_KeyWind{ KeyWind }, CDialogMove(CCMWindPulsationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMWindPulsationDlg)

	//}}AFX_DATA_INIT
	m_arCtrlHide.Add(IDC_CMD_SPEC_MODE_GRUP);
	m_arCtrlHide.Add(IDC_CMD_SPEC_MODE_GRID);
	m_arCtrlHide.Add(IDC_CMD_SPEC_ALL_BTN);
	m_arCtrlHide.Add(IDC_CMD_SPEC_NONE_BTN);
	m_arCtrlMove.Add(IDOK);
	m_arCtrlMove.Add(IDCANCEL);

}


void CCMWindPulsationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindPulsationDlg)	
	 // Connect ComboBoxes
	DDX_Control(pDX, IDC_CMD_WIND_PUL_CONS_TYPE_CMB, m_cmbConsType);
	DDX_Control(pDX, IDC_CMD_WIND_PUL_TERR_TYPE_CMB, m_cmbTerrType);

	// Connect Edit Controls
	DDX_Control(pDX, IDC_CMD_WIND_PUL_OSCI_DECR_EDT, m_edtOsciDecrement);
	DDX_Control(pDX, IDC_CMD_WIND_PUL_WIND_PRES_EDT, m_edtWindPressure);
	DDX_Control(pDX, IDC_CMD_WIND_DIMS_PLAN_X_EDT, m_edtDimPlanX);
	DDX_Control(pDX, IDC_CMD_WIND_DIMS_PLAN_Y_EDT, m_edtDimPlanY);
	DDX_Control(pDX, IDC_CMD_WIND_DIMS_WIND_X_EDT, m_edtDimWindX);
	DDX_Control(pDX, IDC_CMD_WIND_DIMS_WIND_Y_EDT, m_edtDimWindY);

	// Connect Static Controls (for Unit Labels)
	DDX_Control(pDX, IDC_CMD_WIND_DIMS_PLAN_X_UNT, m_untDimPlanX);
	DDX_Control(pDX, IDC_CMD_WIND_DIMS_PLAN_Y_UNT, m_untDimPlanY);
	DDX_Control(pDX, IDC_CMD_WIND_DIMS_WIND_X_UNT, m_untDimWindX);
	DDX_Control(pDX, IDC_CMD_WIND_DIMS_WIND_Y_UNT, m_untDimWindY);

	// Connect Direction Factor Edit Controls
	DDX_Control(pDX, IDC_CMD_WIND_PUL_DIRE_FACT_X_EDT, m_edtDirectionFactorX);
	DDX_Control(pDX, IDC_CMD_WIND_PUL_DIRE_FACT_Y_EDT, m_edtDirectionFactorY);

	// Connect Mode Selection Checkbox
	DDX_Control(pDX, IDC_CMD_WIND_SELECT_MODE_CHK, m_chkAutoSelectMode);

	// Connect Sum of Modal Mass Edit Controls
	DDX_Control(pDX, IDC_CMD_PUL_DIRE_SUM_X_EDT, m_edtSumModalMassX);
	DDX_Control(pDX, IDC_CMD_PUL_DIRE_SUM_Y_EDT, m_edtSumModalMassY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindPulsationDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMWindPulsationDlg)
	ON_BN_CLICKED(IDC_CMD_WIND_SELECT_MODE_CHK, OnCmdSpecSelectModeChk)
	ON_BN_CLICKED(IDC_CMD_WIND_ALL_BTN, OnCmdSpecAllBtn)
	ON_BN_CLICKED(IDC_CMD_WIND_NONE_BTN, OnCmdSpecNoneBtn)
	ON_BN_CLICKED(IDC_CMD_WIND_ALL_Y_BTN , OnCmdSpecAll_Y_Btn)
	ON_BN_CLICKED(IDC_CMD_WIND_NONE_Y_BTN, OnCmdSpecNone_Y_Btn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPulsationDlg message handlers

BOOL CCMWindPulsationDlg::OnInitDialog() 
{
 	CDialogMove::OnInitDialog();
	InitControls();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindPulsationDlg::Dlg2Data()
{
	m_pData->enConstructionType = (EN_WNPS_CONSTRUCTION_TYPE)CDlgUtil::CobxGetCurSelItemData(m_cmbConsType, m_cmbConsType.GetCurSel());
	m_pData->enTerrainType = (EN_WNPS_TERRAIN_TYPE)CDlgUtil::CobxGetCurSelItemData(m_cmbTerrType, m_cmbTerrType.GetCurSel());
	m_pData->dLogDecrement = m_edtOsciDecrement.GetEditValue();
	m_pData->dWindPressure = m_edtWindPressure.GetEditValue();
	m_pData->dDimXPlan = m_edtDimPlanX.GetEditValue();
	m_pData->dDimYPlan = m_edtDimPlanY.GetEditValue();
	m_pData->dDimXWind = m_edtDimWindX.GetEditValue();
	m_pData->dDimYWind = m_edtDimWindY.GetEditValue();
	m_pData->dFactorX = m_edtDirectionFactorX.GetEditValue();
	m_pData->dFactorY = m_edtDirectionFactorY.GetEditValue();
	m_pData->dModalMassX = m_edtSumModalMassX.GetEditValue();
	m_pData->dModalMassY = m_edtSumModalMassY.GetEditValue();
	m_pData->bCheckModeShape = !m_chkAutoSelectMode.GetCheck();

	m_wndGridX.SaveData();
	m_wndGridY.SaveData();
}

BOOL CCMWindPulsationDlg::Data2Dlg()
{
	m_cmbConsType.SetCurSel(CDlgUtil::CobxSetCurSelItemData(m_cmbConsType, m_pData->enConstructionType));
	m_cmbTerrType.SetCurSel(CDlgUtil::CobxSetCurSelItemData(m_cmbTerrType, m_pData->enTerrainType));
	m_edtOsciDecrement.SetEditUnit(m_pData->dLogDecrement);
	m_edtWindPressure.SetEditUnit(m_pData->dWindPressure);
	m_edtDimPlanX.SetEditUnit(m_pData->dDimXPlan);
	m_edtDimPlanY.SetEditUnit(m_pData->dDimYPlan);
	m_edtDimWindX.SetEditUnit(m_pData->dDimXWind);
	m_edtDimWindY.SetEditUnit(m_pData->dDimYWind);
	m_edtDirectionFactorX.SetEditUnit(m_pData->dFactorX);
	m_edtDirectionFactorY.SetEditUnit(m_pData->dFactorY);
	m_edtSumModalMassX.SetEditUnit(m_pData->dModalMassX);
	m_edtSumModalMassY.SetEditUnit(m_pData->dModalMassY);
	m_chkAutoSelectMode.SetCheck(!m_pData->bCheckModeShape);

	m_wndGridX.SetSelect(m_pData->bCheckModeShape);
	m_wndGridY.SetSelect(m_pData->bCheckModeShape);

	m_wndGridX.ShowData();
	m_wndGridY.ShowData();

	return TRUE;
}

void CCMWindPulsationDlg::OnOK() 
{
	UpdateData(TRUE);

	Dlg2Data();
	
	CDialogMove::OnOK();

	ASSERT(m_pDoc && m_KeyWind && m_pData);
	const auto params = ru_sp20::Params{ *m_pData };
	const auto ok = ru_sp20::MakePulsation(*m_pDoc, m_KeyWind, *m_pData);
	ASSERT(ok);
}
void CCMWindPulsationDlg::UpdatePercent(int nDirection, double dPercent)
{
	if(nDirection== EN_WIND_DIRECTION_X)
	{
		m_edtSumModalMassX.SetEditUnit(dPercent);
	}
	else if(nDirection==EN_WIND_DIRECTION_Y)
	{
		m_edtSumModalMassY.SetEditUnit(dPercent);
	}
}

void CCMWindPulsationDlg::SetDataSource(T_WNPS_D* pData, CArray<double, double>& arModePercentX, CArray<double, double>& arModePercentY)
{
	m_pData=pData;
	m_arModePercentX.Copy(arModePercentX);
	m_arModePercentY.Copy(arModePercentY);
}
void CCMWindPulsationDlg::InitControls()
{
	// 먼저 콤보박스의 기존 내용을 초기화합니다.
	m_cmbConsType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbConsType, _T("Towers, masts"), EN_WNPS_TYPE_TOWERS);
	CDlgUtil::CobxAddItem(m_cmbConsType, _T("Other structures"), EN_WNPS_TYPE_OTHERS);
	CDlgUtil::CobxSetCurSelItemData(m_cmbConsType,  EN_WNPS_TYPE_TOWERS);

	m_cmbTerrType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbTerrType, _T("A"), EN_WNPS_TERRAIN_A);
	CDlgUtil::CobxAddItem(m_cmbTerrType, _T("B"), EN_WNPS_TERRAIN_B);
	CDlgUtil::CobxAddItem(m_cmbTerrType, _T("C"), EN_WNPS_TERRAIN_C);
	CDlgUtil::CobxSetCurSelItemData(m_cmbTerrType, EN_WNPS_TERRAIN_A);

	// Edit Controls
	m_edtOsciDecrement.SetUnitType(D_UNITSYS_NONE);
	m_edtWindPressure.SetUnitType(D_UNITSYS_NONE);
	m_edtDimPlanX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtDimPlanY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtDimWindX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtDimWindY.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_untDimPlanX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDimPlanY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDimWindX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDimWindY.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtDirectionFactorX.SetUnitType(D_UNITSYS_NONE);
	m_edtDirectionFactorY.SetUnitType(D_UNITSYS_NONE);

	m_edtSumModalMassX.SetUnitType(D_UNITSYS_NONE);
	m_edtSumModalMassY.SetUnitType(D_UNITSYS_NONE);

	m_wndGridX.SubclassDlgItem(IDC_CMD_WIND_PUL_MODE_X_GRID, this);
	m_wndGridX.InitGrid(this, EN_WIND_DIRECTION_X);
	m_wndGridX.SetDataSource(&m_pData->arModeShapesX, m_arModePercentX, m_pData->bCheckModeShape);

	m_wndGridY.SubclassDlgItem(IDC_CMD_WIND_PUL_MODE_Y_GRID, this);
	m_wndGridY.InitGrid(this, EN_WIND_DIRECTION_Y);
	m_wndGridY.SetDataSource(&m_pData->arModeShapesY, m_arModePercentY, m_pData->bCheckModeShape);

	GetDlgItem(IDC_CMD_WIND_CREATE_REPORT_BTN)->ShowWindow(SW_HIDE);
}



void CCMWindPulsationDlg::OnCmdSpecSelectModeChk() 
{
	BOOL bDoHide = m_chkAutoSelectMode.GetCheck();
	m_pData->bCheckModeShape = !bDoHide;
	
	m_wndGridX.SetSelect(!bDoHide);
	m_wndGridY.SetSelect(!bDoHide);
	
	m_wndGridX.ShowData();
	m_wndGridY.ShowData();
}

void CCMWindPulsationDlg::OnCmdSpecAllBtn() 
{
	for(int i=0;i<m_pData->arModeShapesX.GetSize();i++)
	{
		m_pData->arModeShapesX[i]=TRUE;
	}
	m_wndGridX.ShowData();
}

void CCMWindPulsationDlg::OnCmdSpecNoneBtn() 
{
	for(int i=0;i<m_pData->arModeShapesX.GetSize();i++)
	{
		m_pData->arModeShapesX[i]=FALSE;
	}
	m_wndGridX.ShowData();
}

void CCMWindPulsationDlg::OnCmdSpecAll_Y_Btn()
{
	for (int i = 0; i < m_pData->arModeShapesY.GetSize(); i++)
	{
		m_pData->arModeShapesY[i] = TRUE;
	}
	m_wndGridY.ShowData();
}
void CCMWindPulsationDlg::OnCmdSpecNone_Y_Btn()
{
	for (int i = 0; i < m_pData->arModeShapesY.GetSize(); i++)
	{
		m_pData->arModeShapesY[i] = FALSE;
	}
	m_wndGridY.ShowData();
}