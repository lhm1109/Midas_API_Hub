// CMWindItemIS1987.cpp : implementation file
//
// 2003. 09. 24  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemIS875_2015.h"
#include "CMWindItemDlg.h"
#include "WindGustIS875_15Dlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIS875_2015 dialog


CCMWindItemIS875_2015::CCMWindItemIS875_2015(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemIS875_2015::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemIS875_2015)
	//}}AFX_DATA_INIT
	m_aCtrlMethod.Add(IDC_CMD_STANDARD_RD1);
	m_aCtrlMethod.Add(IDC_CMD_GUSTF_RD2);

	m_aCtrlAutoUser.Add(IDC_CMD_AUTO_RD);
	m_aCtrlAutoUser.Add(IDC_CMD_USER_RD);

	m_aCtrlStandard.Add(IDC_CMD_BCLASS_TITLE);
	m_aCtrlStandard.Add(IDC_CMD_BCLASS_COMBO);
	m_aCtrlStandard.Add(IDC_CMD_FRICTION_TITLE);
	m_aCtrlStandard.Add(IDC_CMD_FRICTION_COMBO);
	m_aCtrlStandard.Add(IDC_CMD_LOADEVAL_CHK);
	m_aCtrlStandard.Add(IDC_CMD_ANAL_FRAME2);
	m_aCtrlStandard.Add(IDC_CMD_ANAL_FCOEF_TITLE);
	m_aCtrlStandard.Add(IDC_CMD_FCOEF_EDIT);
	m_aCtrlStandard.Add(IDC_CMD_GUST_FACT_BTN);

	m_aCtrlGustFactor.Add(IDC_CMD_FRICTION_TITLE);
	m_aCtrlGustFactor.Add(IDC_CMD_FRICTION_COMBO);
	m_aCtrlGustFactor.Add(IDC_CMD_ANAL_FCOEF_TITLE2);
	m_aCtrlGustFactor.Add(IDC_CMD_WINDDIRFACT_KD_EDIT2);
	m_aCtrlGustFactor.Add(IDC_CMD_ANAL_COMBFACT_TITLE3);
	m_aCtrlGustFactor.Add(IDC_CMD_COMBFACT_Kc_EDIT3);
	m_aCtrlGustFactor.Add(IDC_CMD_LOADEVAL_CHK);
	m_aCtrlGustFactor.Add(IDC_CMD_ANAL_FRAME2);
	m_aCtrlGustFactor.Add(IDC_CMD_ANAL_FCOEF_TITLE);
	m_aCtrlGustFactor.Add(IDC_CMD_FCOEF_EDIT);
	

	m_aCtrlFriction.Add(IDC_CMD_FRICTION_TITLE);
	m_aCtrlFriction.Add(IDC_CMD_FRICTION_COMBO);

	m_aCtrlGustFactorParamBtn.Add(IDC_CMD_GUST_FACT_BTN);

	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();

	m_dBeta= 0.0;
	m_dAlongWind= 0.0;
	m_dAcrossWind= 0.0;
	m_iFactk=0;
	m_dCfs= 0.0;
	m_dMc= 0.0;
	m_iStrKind = 0;
	m_bIsUSerDefGustFact = FALSE;
}

void CCMWindItemIS875_2015::SetData2Dlg(T_WIND_IS875_2015& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
	if (m_Data.nProcedure == 2 && bModify) 
	{
		CWindGustIS875_15Dlg dlg;
		dlg.iStrKind	= m_Data.GustFact.nStrKind	;
		dlg.dBeta		= m_Data.GustFact.dBeta;
		dlg.dAlongWind	= m_Data.GustFact.dAlongWindPeriod;
		dlg.dAcrossWind	= m_Data.GustFact.dAAcrossWindPeriod;
		dlg.iFactk		= m_Data.GustFact.m_ifact;;
		dlg.dCfs		= m_Data.GustFact.dCfs;
		dlg.dMc			= m_Data.GustFact.nMc;
		dlg.bIsUSerDefGustFact = m_Data.GustFact.nProcedure	;
		
		m_iStrKind		     =  dlg.iStrKind;
		m_dBeta			     =  dlg.dBeta;
		m_dAlongWind	     =  dlg.dAlongWind;
		m_dAcrossWind	     =  dlg.dAcrossWind;
		m_dGustFactor        =  m_Data.GustFact.dModeShapeFact;
		m_iFactk		     =  dlg.iFactk;
		m_dCfs			     =  dlg.dCfs;
		m_dMc			     =  dlg.dMc;
		m_bIsUSerDefGustFact = dlg.bIsUSerDefGustFact;
	}
	
}

BOOL CCMWindItemIS875_2015::SetDlg2Data(T_WIND_IS875_2015& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemIS875_2015::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemIS875_2015)
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_FCOEF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_VRANGE_UNIT, m_unitVRange);
	DDX_Control(pDX, IDC_CMD_VRANGE_EDIT, m_editVRange);
	DDX_Control(pDX, IDC_CMD_TOPOFACTOR_EDIT, m_editTopoFactor);
	DDX_Control(pDX, IDC_CMD_TOPO_CHK, m_chkTopographic);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SCLASS_COMBO, m_cobxStructClass);
	DDX_Control(pDX, IDC_CMD_RISKCOEF_EDIT, m_editRiskCoef);
	DDX_Control(pDX, IDC_CMD_LOADEVAL_CHK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_GUSTF_EDIT, m_editGustFactor);
	DDX_Control(pDX, IDC_CMD_FRICTION_COMBO, m_cobxFriction);
	DDX_Control(pDX, IDC_CMD_FORCECOEF_EDIT, m_editForceCoef2);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_BCLASS_COMBO, m_cobxBuildingClass);
	//For Cyclonic Region Effects
	DDX_Control(pDX, IDC_CMD_CYCLO_CHK1, m_chkCycloRegEffects);
	DDX_Control(pDX, IDC_CMD_CYCLOCLASS_COMBO2, m_cobxImportanceClass);
	DDX_Control(pDX, IDC_CMD_IMPFACT_K4_EDIT,      m_editk4Factor);
	DDX_Control(pDX, IDC_CMD_WINDDIRFACT_KD_EDIT2, m_editkdFactor);
	DDX_Control(pDX, IDC_CMD_COMBFACT_Kc_EDIT3,    m_editkcFactor);


		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemIS875_2015, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemIS875_2015)
	ON_BN_CLICKED(IDC_CMD_STANDARD_RD1, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_USER_RD, OnCmdAutoUserRadio)
	ON_CBN_SELCHANGE(IDC_CMD_SCLASS_COMBO, OnSelchangeCmdSclassCombo)
	ON_BN_CLICKED(IDC_CMD_LOADEVAL_CHK, OnCmdLoadevalChk)
	ON_BN_CLICKED(IDC_CMD_TOPO_CHK, OnCmdTopoChk)
	ON_BN_CLICKED(IDC_CMD_GUSTF_RD2, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_AUTO_RD, OnCmdAutoUserRadio)
	ON_EN_CHANGE(IDC_CMD_SPEED_EDIT, OnChangeCmdSpeedEdit)
	ON_BN_CLICKED(IDC_CMD_CYCLO_CHK1, OnCmdCyclonRadio)
	ON_CBN_SELCHANGE(IDC_CMD_CYCLOCLASS_COMBO2, OnSelchangeImportanceCombo)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CMD_GUST_FACT_BTN, OnBnClickedCmdGustFactBtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIS875_2015 message handlers

BOOL CCMWindItemIS875_2015::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_editForceCoef.SetUnitType(D_UNITSYS_NONE);
	m_unitVRange.SetUnitType(CUnitCtrl::m_WIND_UNIT.IS1987_dTopographyRange);
	m_editVRange.SetUnitType(CUnitCtrl::m_WIND_UNIT.IS1987_dTopographyRange);
	m_editTopoFactor.SetUnitType(D_UNITSYS_NONE);
	m_editRiskCoef.SetUnitType(D_UNITSYS_NONE);
	m_editGustFactor.SetUnitType(D_UNITSYS_NONE);
	m_editForceCoef2.SetUnitType(D_UNITSYS_NONE);
	m_editSpeed.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	//AlignControls();
	InitCategoryCombo();
	InitBuildingClassCombo();
	InitFrictionCombo();
	InitStructureClassCombo();
	InitCycloRegCombo();

	m_grpWindParam.InitControl(this, CCMWindItemIS875_2015::IDD, IDC_CMD_FRAME, TRUE, TRUE); // AlignControls 다음에
	m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다


	//CDlgUtil::CtrlEnableDisable(this,IDC_CMD_GUSTF_RD2,FALSE);
	//if(m_Data.nProcedure != 1) m_Data.nProcedure = 1;

	
	Data2Dlg();
	
	
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemIS875_2015::DestroyWindow() 
{
	Dlg2Data();

	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpWindParam.SetFoldState(FALSE, FALSE);
		}
	}
	
	return CChildDialog::DestroyWindow();
}

void CCMWindItemIS875_2015::OnCmdMethodRadio() 
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	CDlgUtil::CtrlShowHide(this, m_aCtrlStandard, TRUE);
	

	if (nMethod == 0)
	{
		int nCheck = m_chkLoadEval.GetCheck();
		//CDlgUtil::CtrlShowHide(this, m_aCtrlFriction, nCheck != 1);
		CDlgUtil::CtrlEnableDisable(this,m_aCtrlFriction,TRUE);
		CDlgUtil::CtrlEnableDisable(this,m_aCtrlGustFactor,TRUE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this,m_aCtrlGustFactor,FALSE);
	}
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlGustFactorParamBtn, nMethod == 1);
}

void CCMWindItemIS875_2015::OnCmdAutoUserRadio() 
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, nMethod);
	m_cobxStructClass.EnableWindow(nMethod == 0);
	m_editRiskCoef.SetReadOnly(nMethod == 0);
	if (nMethod == 0) CalculateRiskCoef();
}

void CCMWindItemIS875_2015::OnCmdLoadevalChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	m_editForceCoef.EnableWindow(nCheck == 1);

	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	if (nMethod == 0)
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlFriction, nCheck != 1);
		
}

void CCMWindItemIS875_2015::OnCmdTopoChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic.GetCheck();
	m_editTopoFactor.EnableWindow(nCheck == 1);
	m_editVRange.EnableWindow(nCheck == 1);
}

void CCMWindItemIS875_2015::OnSelchangeCmdSclassCombo() 
{
	// TODO: Add your control notification handler code here
	CalculateRiskCoef();
}

void CCMWindItemIS875_2015::OnSelchangeImportanceCombo() 
{


	int nIndex = m_cobxImportanceClass.GetCurSel();
	if (nIndex == CB_ERR) return;
	int nStructClass = m_cobxImportanceClass.GetItemData(nIndex);

	double dRiskCoef;
	dRiskCoef = GetK4Factor(nStructClass);
	m_editk4Factor.SetEditUnit(dRiskCoef);
}

void CCMWindItemIS875_2015::OnChangeCmdSpeedEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CalculateRiskCoef();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemIS875_2015::InitDefaultData()
{
	if (m_bModify) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 층개수와 Base Level 구하기 
	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	T_STOR_D StorD, StorD2;
	CArray<T_STOR_K,T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);  
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0)	pDoc->m_pAttrCtrl->GetStor(aStorK[nStorNum-1], StorD);
	else StorD.Initialize();
	
	m_Data.dTopographyRange = StorD.dStoryLevel - dBaseLevel;				
}

void CCMWindItemIS875_2015::AlignControls()
{
	CArray<UINT, UINT> aControls;
	aControls.Add(IDC_CMD_GUSTF_TITLE);
	aControls.Add(IDC_CMD_GUSTF_EDIT);
	aControls.Add(IDC_CMD_FORCECOEF_TITLE);
	aControls.Add(IDC_CMD_FORCECOEF_EDIT);

	CRect ToRect, FromRect;
	m_cobxBuildingClass.GetWindowRect(ToRect);
	m_editGustFactor.GetWindowRect(FromRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
}

void CCMWindItemIS875_2015::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("I"), _T("II"), _T("III"), _T("IV")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemIS875_2015::InitBuildingClassCombo()
{
	m_cobxBuildingClass.ResetContent();

	CString aItem[] = {_T("A")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxBuildingClass.AddString(aItem[i]);
	m_cobxBuildingClass.SetCurSel(0);
}

void CCMWindItemIS875_2015::InitFrictionCombo()
{
	m_cobxFriction.ResetContent();

	CString aItem[] = {_T("0.01"), _T("0.02"), _T("0.04")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxFriction.AddString(aItem[i]);
	m_cobxFriction.SetCurSel(0); 
}

void CCMWindItemIS875_2015::InitStructureClassCombo()
{
	m_cobxStructClass.ResetContent();

	CString aItem[] = {_LS(IDS_CMD_WIND_IS1987_StrClassA), _LS(IDS_CMD_WIND_IS1987_StrClassB), 
		_LS(IDS_CMD_WIND_IS1987_StrClassC), _LS(IDS_CMD_WIND_IS1987_StrClassD)};
	
	int nIndex;
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxStructClass.AddString(aItem[i]);
		m_cobxStructClass.SetItemData(nIndex, i);
	}
	m_cobxStructClass.SetCurSel(0); 
}

void CCMWindItemIS875_2015::InitCycloRegCombo()
{
	m_cobxImportanceClass.ResetContent();

	CString aItem[] = {_LS(IDS_CMD_WIND_IS875_2015_StrPostCyclo), _LS(IDS_CMD_WIND_IS875_2015_StrIndustrial), 
		_LS(IDS_CMD_WIND_IS875_2015_StrOther)};

	int nIndex;
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxImportanceClass.AddString(aItem[i]);
		m_cobxImportanceClass.SetItemData(nIndex, i);
	}
	m_cobxImportanceClass.SetCurSel(nNum-1); 
}


void CCMWindItemIS875_2015::CalculateRiskCoef()
{
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, nMethod);
	if (nMethod != 0) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) { ASSERT(0); return; }

	int nIndex = m_cobxStructClass.GetCurSel();
	if (nIndex == CB_ERR) return;
	int nStructClass = m_cobxStructClass.GetItemData(nIndex);

	CString csVal;
	m_editSpeed.GetWindowText(csVal);

	double dRiskCoef;
	dRiskCoef = pDoc->m_pEditData->GetIS1987RiskCoef(nStructClass, _tstof(csVal));
	m_editRiskCoef.SetEditUnit(dRiskCoef);
}

void CCMWindItemIS875_2015::Data2Dlg()
{
	CString csVal;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nProcedure-1);
	OnCmdMethodRadio();

	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	OnChangeCmdSpeedEdit();
	m_cobxCategory.SetCurSel(m_Data.nTerrainCategory);
	m_cobxBuildingClass.SetCurSel(m_Data.nBuildingClass);
	if (m_Data.dFrictionCoef == 0.01 || m_Data.dFrictionCoef == 0.02 ||
			m_Data.dFrictionCoef == 0.04)
	{
		csVal.Format(_T("%g"), m_Data.dFrictionCoef);
		m_cobxFriction.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dFrictionCoef);
		m_cobxFriction.SetWindowText(csVal);
	}
	m_editGustFactor.SetEditUnit(m_Data.dForceCoef);
	m_editForceCoef2.SetEditUnit(m_Data.dGustFactor);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlAutoUser, m_Data.nAutoCalcRiskCoef);
	OnCmdAutoUserRadio();
	m_cobxStructClass.SetCurSel(m_Data.nStructureClass);
	OnSelchangeCmdSclassCombo();
	m_editRiskCoef.SetEditUnit(m_Data.dRiskCoef);

	m_chkTopographic.SetCheck(m_Data.bTopography);
	OnCmdTopoChk();
	m_editTopoFactor.SetEditUnit(m_Data.dTopographicFactor);
	m_editVRange.SetEditUnit(m_Data.dTopographyRange);

	m_chkCycloRegEffects.SetCheck(m_Data.bInclCyclonicRegEffects);
	OnCmdCyclonRadio();
	OnSelchangeImportanceCombo();
	m_editk4Factor.SetEditUnit(m_Data.dk4Fact);
	m_editkdFactor.SetEditUnit(m_Data.dkdfact);
	m_editkcFactor.SetEditUnit(m_Data.dkcfact);
	if (m_Data.dk4Fact > 1.0)
	{//In case importing from MCT/MGB
		if (m_Data.dk4Fact==1.3) m_cobxImportanceClass.SetCurSel(0); 
		else                     m_cobxImportanceClass.SetCurSel(1); 
	}



	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	OnCmdLoadevalChk();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoef);
}

BOOL CCMWindItemIS875_2015::Dlg2Data()
{
	CString csVal;
	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nProcedure);
	m_Data.nProcedure += 1;

	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.nTerrainCategory = m_cobxCategory.GetCurSel();
	if (m_Data.nProcedure == 1)
	{
		m_Data.nBuildingClass = m_cobxBuildingClass.GetCurSel();
		m_cobxFriction.GetWindowText(csVal);
		m_Data.dFrictionCoef = _tstof(csVal);
	}
	else 
	{
		CWindGustIS875_15Dlg dlg;
		m_Data.dForceCoef = m_editGustFactor.GetEditValue();
		m_Data.dGustFactor = m_editForceCoef2.GetEditValue();
		
		m_Data.GustFact.nStrKind			=  m_iStrKind		 ;
		m_Data.GustFact.dBeta				=	m_dBeta		 ;
		m_Data.GustFact.dAlongWindPeriod	=	m_dAlongWind ;
		m_Data.GustFact.dAAcrossWindPeriod  =	m_dAcrossWind ;
		m_Data.GustFact.dModeShapeFact = m_dGustFactor;
		m_Data.GustFact.dCfs				=	m_dCfs		 ;
		m_Data.GustFact.nMc					=	m_dMc		 ;
		m_Data.GustFact.nProcedure			=	m_bIsUSerDefGustFact ;
		m_Data.GustFact.m_ifact = m_iFactk;
	}														 

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, m_Data.nAutoCalcRiskCoef);
	if (m_Data.nAutoCalcRiskCoef == 0)
		m_Data.nStructureClass = m_cobxStructClass.GetCurSel();
	m_Data.dRiskCoef = m_editRiskCoef.GetEditValue();

	m_Data.bTopography = m_chkTopographic.GetCheck();
	if (m_Data.bTopography)
	{
		m_Data.dTopographicFactor = m_editTopoFactor.GetEditValue();
		m_Data.dTopographyRange = m_editVRange.GetEditValue();
	}


	m_Data.bInclCyclonicRegEffects = m_chkCycloRegEffects.GetCheck();
	if (m_Data.bInclCyclonicRegEffects)
	{
		m_Data.dk4Fact =  m_editk4Factor.GetEditValue();
		m_Data.dkdfact =  m_editkdFactor.GetEditValue();
		m_Data.dkcfact =  m_editkcFactor.GetEditValue();
	}


	if (m_Data.nProcedure == 1)
	{
		m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
		if (m_Data.bUseForceCoef)
		m_Data.dForceCoef = m_editForceCoef.GetEditValue();
	}

	return TRUE;
}

LRESULT CCMWindItemIS875_2015::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		BOOL bFolded = m_grpWindParam.GetFoldState();
		if (!bFolded)
		{
				OnCmdMethodRadio();
				OnCmdLoadevalChk();
		}

		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}

void CCMWindItemIS875_2015::OnCmdCyclonRadio() 
{
	// TODO: Add your control notification handler code here
//   int nMethod;
//   CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, nMethod);
//   m_cobxStructClass.EnableWindow(nMethod == 0);
//   m_editRiskCoef.SetReadOnly(nMethod == 0);
//   if (nMethod == 0) CalculateRiskCoef();
	int nCheck = m_chkCycloRegEffects.GetCheck();
//   m_editTopoFactor.EnableWindow(nCheck == 1);
//   m_editVRange.EnableWindow(nCheck == 1);
	m_cobxImportanceClass.EnableWindow(nCheck == 1);
	m_editk4Factor.EnableWindow(FALSE);
//     m_editkdFactor.EnableWindow(nCheck == 1);
//     m_editkcFactor.EnableWindow(nCheck == 1);

}

double CCMWindItemIS875_2015::GetK4Factor(int impfactor) 
{
	double dk4fact=0.0;
	if (impfactor==0) dk4fact = 1.3;
	else if (impfactor==1)dk4fact = 1.15;
	else dk4fact = 1.0;

	return dk4fact;

}

void CCMWindItemIS875_2015::OnBnClickedCmdGustFactBtn()
{
	// TODO: Add your control notification handler code here
	CWindGustIS875_15Dlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	//dlg.SetData2Dlg(m_Data,TRUE);
	dlg.iStrKind	= m_Data.GustFact.nStrKind	;
	dlg.dBeta		= m_Data.GustFact.dBeta;
	dlg.dAlongWind	= m_Data.GustFact.dAlongWindPeriod;
	dlg.dAcrossWind	= m_Data.GustFact.dAAcrossWindPeriod;
	dlg.iFactk = m_Data.GustFact.m_ifact;
	dlg.dCfs		= m_Data.GustFact.dCfs;
	dlg.dMc			= m_Data.GustFact.nMc;
	dlg.bIsUSerDefGustFact =  m_Data.GustFact.nProcedure;

	if(dlg.DoModal()== IDOK)
	{
		//dlg.SetDlg2Data(m_Data);
		m_Data.GustFact.nStrKind			=  dlg.iStrKind		;
		m_Data.GustFact.dBeta				=  dlg.dBeta		;
		m_Data.GustFact.dAlongWindPeriod	=  dlg.dAlongWind	;
		m_Data.GustFact.dAAcrossWindPeriod  =  dlg.dAcrossWind	; 
		m_Data.GustFact.dModeShapeFact		= _wtof(dlg.m_GustFactor);
		m_Data.GustFact.dCfs				=  dlg.dCfs			;
		m_Data.GustFact.nMc					=  dlg.dMc			;
		m_Data.GustFact.nProcedure			=  dlg.bIsUSerDefGustFact;
		m_Data.GustFact.m_ifact = dlg.iFactk;
		m_iStrKind		=  dlg.iStrKind		;
		m_dBeta			=  dlg.dBeta		;
		m_dAlongWind	=  dlg.dAlongWind	;
		m_dAcrossWind	=  dlg.dAcrossWind	; 
		m_iFactk		=  dlg.iFactk		;
		m_dCfs			=  dlg.dCfs			;
		m_dMc			=  dlg.dMc			;
		m_bIsUSerDefGustFact = dlg.bIsUSerDefGustFact;
		m_dGustFactor = _wtof(dlg.m_GustFactor);
	}
}
