// CMWindItemBS1997.cpp : implementation file
//
// 2003. 09. 23  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemBS1997.h"
#include "CMWindItemDlg.h"

#include "WindBSFactor.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemBS1997 dialog


CCMWindItemBS1997::CCMWindItemBS1997(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemBS1997::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemBS1997)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlMethod.Add(IDC_CMD_STANDARD_RD1);
	m_aCtrlMethod.Add(IDC_CMD_DIRECTION_RD1);

	m_aCtrlDirection.Add(IDC_CMD_TOWNDIST_TITLE);
	m_aCtrlDirection.Add(IDC_CMD_TOWNDIST_EDIT);
	m_aCtrlDirection.Add(IDC_CMD_TOWNDIST_UNIT);

	m_Data.Initialize();
}

BOOL CCMWindItemBS1997::DestroyWindow() 
{
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpWindParam.SetFoldState(FALSE, FALSE);
		}
	}

	return CChildDialog::DestroyWindow();
}

void CCMWindItemBS1997::SetData2Dlg(T_WIND_BS6399& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemBS1997::SetDlg2Data(T_WIND_BS6399& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemBS1997::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemBS1997)
	DDX_Control(pDX, IDC_CMD_TOWNDIST_UNIT, m_unitTownDist);
	DDX_Control(pDX, IDC_CMD_TOWNDIST_EDIT, m_editTownDist);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_SEPARATION_UNIT, m_unitSeparation);
	DDX_Control(pDX, IDC_CMD_SEPARATION_EDOT, m_editSeparation);
	DDX_Control(pDX, IDC_CMD_SEADIST_UNIT, m_unitSeaDist);
	DDX_Control(pDX, IDC_CMD_SEADIST_EDIT, m_editSeaDist);
	DDX_Control(pDX, IDC_CMD_ROOFH_UNIT, m_unitRoofH);
	DDX_Control(pDX, IDC_CMD_ROOFH_EDIT, m_editRoofH);
	DDX_Control(pDX, IDC_CMD_FRICTION_EDIT, m_editFriction);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_BTYPE_COMBO, m_cobxBDType);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemBS1997, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemBS1997)
	ON_BN_CLICKED(IDC_CMD_DIRECTION_RD1, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_STANDARD_RD1, OnCmdMethodRadio)
	ON_CBN_SELCHANGE(IDC_CMD_CATEGORY_COMBO, OnSelchangeCmdCategoryCombo)
	ON_BN_CLICKED(IDC_CMD_SPEEDFACTOR_BTN, OnCmdSpeedfactorBtn)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemBS1997 message handlers

BOOL CCMWindItemBS1997::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemBS1997::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_unitSeparation.SetUnitType(CUnitCtrl::m_WIND_UNIT.BS6399_dBldgSeparation);
	m_editSeparation.SetUnitType(CUnitCtrl::m_WIND_UNIT.BS6399_dBldgSeparation);
	m_unitRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.BS6399_dMeanRoofHeight);
	m_editRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.BS6399_dMeanRoofHeight);
	m_editTownDist.SetUnitType(D_UNITSYS_NONE);
	m_editSpeed.SetUnitType(D_UNITSYS_NONE);
	m_editSeaDist.SetUnitType(D_UNITSYS_NONE);
	m_editFriction.SetUnitType(D_UNITSYS_NONE);

	InitCategoryCombo();
	InitBuildingTypeCombo();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemBS1997::OnCmdMethodRadio() 
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDirection, nMethod == 1);
}

void CCMWindItemBS1997::OnSelchangeCmdCategoryCombo() 
{
	// TODO: Add your control notification handler code here
	int nCategory = m_cobxCategory.GetCurSel();
	m_editTownDist.EnableWindow(nCategory == 1);
}

void CCMWindItemBS1997::OnCmdSpeedfactorBtn() 
{
	// TODO: Add your control notification handler code here	
	CWindBSFactor dlg;
	dlg.SetInitPos(D_INIT_POS_RT);	
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, dlg.nProcedure);
	dlg.m_Data.CodeParam.BS6399.dAltitudeFactor    =	m_Data.dAltitudeFactor   ; 
	dlg.m_Data.CodeParam.BS6399.dDirectionalFactor =	m_Data.dDirectionalFactor; 
	dlg.m_Data.CodeParam.BS6399.dSeasonalFactor    =	m_Data.dSeasonalFactor	 ; 
	dlg.m_Data.CodeParam.BS6399.dProbabilityFactor =	m_Data.dProbabilityFactor; 
	dlg.m_Data.CodeParam.BS6399.dTopographicFactor =	m_Data.dTopographicFactor; 
	
	if(dlg.DoModal() == IDOK)
	{	
		m_Data.dAltitudeFactor    = dlg.m_Data.CodeParam.BS6399.dAltitudeFactor   ;   
		m_Data.dDirectionalFactor = dlg.m_Data.CodeParam.BS6399.dDirectionalFactor;    
		m_Data.dSeasonalFactor	  = dlg.m_Data.CodeParam.BS6399.dSeasonalFactor   ;
		m_Data.dProbabilityFactor = dlg.m_Data.CodeParam.BS6399.dProbabilityFactor;
		m_Data.dTopographicFactor = dlg.m_Data.CodeParam.BS6399.dTopographicFactor;		
	}		
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemBS1997::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("country"), _T("town")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemBS1997::InitBuildingTypeCombo()
{
	m_cobxBDType.ResetContent();

	CString aItem[] = {_T("8"), _T("4"), _T("2"), _T("1"), _T("0.5")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxBDType.AddString(aItem[i]);
	m_cobxBDType.SetCurSel(0);
}

void CCMWindItemBS1997::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nProcedure-1);
	OnCmdMethodRadio();
	m_cobxCategory.SetCurSel(m_Data.nSiteCategory-1);
	OnSelchangeCmdCategoryCombo();
	m_cobxBDType.SetCurSel(m_Data.nBldgTypeFactor-1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);

	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editRoofH.SetEditUnit(m_Data.dMeanRoofHeight);
	m_editSeparation.SetEditUnit(m_Data.dBldgSeparation);
	m_editFriction.SetEditUnit(m_Data.dFrictionCoef);
	m_editSeaDist.SetEditUnit(m_Data.dSeaDistance);
	m_editTownDist.SetEditUnit(m_Data.dTownDistance);
}

BOOL CCMWindItemBS1997::Dlg2Data()
{
	double dAltitudeFactor     = m_Data.dAltitudeFactor;   
	double dDirectionalFactor  = m_Data.dDirectionalFactor;
	double dSeasonalFactor     = m_Data.dSeasonalFactor;   
	double dProbabilityFactor  = m_Data.dProbabilityFactor;
	double dTopographicFactor  = m_Data.dTopographicFactor;

	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nProcedure);
	m_Data.nProcedure += 1;
	m_Data.nSiteCategory = m_cobxCategory.GetCurSel();
	m_Data.nSiteCategory += 1;
	m_Data.nBldgTypeFactor = m_cobxBDType.GetCurSel();
	m_Data.nBldgTypeFactor += 1;
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();

	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.dMeanRoofHeight = m_editRoofH.GetEditValue();
	m_Data.dBldgSeparation = m_editSeparation.GetEditValue();
	m_Data.dFrictionCoef = m_editFriction.GetEditValue();
	m_Data.dSeaDistance = m_editSeaDist.GetEditValue();
	if (m_Data.nProcedure == 2 && m_Data.nSiteCategory == 2)
		m_Data.dTownDistance = m_editTownDist.GetEditValue();

	m_Data.dAltitudeFactor = dAltitudeFactor;
	m_Data.dDirectionalFactor = dDirectionalFactor;
	m_Data.dSeasonalFactor = dSeasonalFactor;
	m_Data.dProbabilityFactor = dProbabilityFactor;
	m_Data.dTopographicFactor = dTopographicFactor;

	return TRUE;
}

LRESULT CCMWindItemBS1997::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		BOOL bFolded = m_grpWindParam.GetFoldState();
		if (!bFolded)
		{
				OnCmdMethodRadio();
		}

		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}