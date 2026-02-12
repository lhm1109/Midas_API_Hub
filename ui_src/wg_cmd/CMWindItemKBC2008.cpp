// CMWindItemKBC2009.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindItemKBC2008.h"
#include "WindGustKBC2008Dlg.h"
#include "WindAutoCalcDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER  0
#define D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO  1

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKBC2009 dialog


CCMWindItemKBC2009::CCMWindItemKBC2009(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindItemKBC2009::IDD, pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_bModify = FALSE;
	m_pParent = (CCMWindItemDlg*)pParent;
	//{{AFX_DATA_INIT(CCMWindItemKBC2009)
	//}}AFX_DATA_INIT
	m_aCtrlType.Add(IDC_CMD_RIGID_RADIO1);
	m_aCtrlType.Add(IDC_CMD_FLEX_RADIO2);
	
	m_aRdoForceCoefCalcType.RemoveAll();
	m_aRdoForceCoefCalcType.Add(IDC_CMD_FORCE_COEF_USER_RDO);
	m_aRdoForceCoefCalcType.Add(IDC_CMD_FORCE_COEF_AUTO_RDO);
	m_aRdoForceCoefCalcType.FreeExtra();
	
	m_aCtrlForceCoef_user.RemoveAll();
	m_aCtrlForceCoef_user.Add(IDC_CMD_FORCE_COEFF_STC );
	m_aCtrlForceCoef_user.Add(IDC_CMD_FORCE_COEFF_EDIT);
	m_aCtrlForceCoef_user.FreeExtra();
	
	m_aCtrlForceCoef_auto.RemoveAll();
	m_aCtrlForceCoef_auto.Add(IDC_CMD_FORCE_COEF_AUTO_BTN);
	m_aCtrlForceCoef_auto.FreeExtra();
	
	m_aCtrlForceCoef.RemoveAll();
	m_aCtrlForceCoef.Add(IDC_CMD_FORCE_COEF_USER_RDO);
	m_aCtrlForceCoef.Append(m_aCtrlForceCoef_user);
	m_aCtrlForceCoef.Add(IDC_CMD_FORCE_COEF_AUTO_RDO);
	m_aCtrlForceCoef.Append(m_aCtrlForceCoef_auto);
	m_aCtrlForceCoef.FreeExtra();
	
	m_bInit = FALSE;
	m_Data.Initialize();
	m_WnatD.Initialize();
}

void CCMWindItemKBC2009::SetData2Dlg(T_WIND_KBC2009& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl2->GetWnat(m_Data.keyWnat, m_WnatD)) m_WnatD.Initialize();
}

BOOL CCMWindItemKBC2009::SetDlg2Data(T_WIND_KBC2009& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemKBC2009::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemKBC2009)
	DDX_Control(pDX, IDC_CMD_TOPO_EFFECT_CHECK, m_chkTopoEffect);
	DDX_Control(pDX, IDC_CMD_LOAD_EVAL_CHECK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_UNIT, m_unitRoofHeight);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_EDIT, m_editRoofHeight);
	DDX_Control(pDX, IDC_CMD_KZT_EDIT, m_editKZT);
	DDX_Control(pDX, IDC_CMD_GUST_FACTOR_EDIT, m_editGustFactorX);
	DDX_Control(pDX, IDC_CMD_GUST_FACTOR_EDIT_Y, m_editGustFactorY);
	DDX_Control(pDX, IDC_CMD_FORCE_COEFF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_COMBO, m_cobxImportFactor);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemKBC2009, CChildDialog)
//{{AFX_MSG_MAP(CCMWindItemKBC2009)
ON_BN_CLICKED(IDC_CMD_GUST_FACTOR_BTN, OnCmdGustFactorBtn)
ON_BN_CLICKED(IDC_CMD_LOAD_EVAL_CHECK, OnCmdLoadEvalCheck)
ON_BN_CLICKED(IDC_CMD_TOPO_EFFECT_CHECK, OnCmdTopoEffectCheck)
ON_BN_CLICKED(IDC_CMD_FORCE_COEF_USER_RDO, OnCmdChangeRdo_ForceCoefCalcType)
ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_RDO, OnCmdChangeRdo_ForceCoefCalcType)
ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_BTN, OnBtnAutoCalc)
ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKBC2009 message handlers

BOOL CCMWindItemKBC2009::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemKBC2009::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_editSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dBasicWindSpeed);
	
	m_unitRoofHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	
	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	
	InitCategoryCombo();
	InitImportFactorCombo();
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindItemKBC2009::DestroyWindow() 
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

void CCMWindItemKBC2009::OnCmdGustFactorBtn() 
{
	// TODO: Add your control notification handler code here	
	CWindGustKBC2009Dlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	CString str = _T("");
	m_cobxImportFactor.GetWindowText(str);
	
	dlg.m_dHeight			= m_editRoofHeight.GetEditValue();
	dlg.m_dImportance = _tstof(str);
	dlg.m_nExposure		= m_cobxCategory.GetCurSel();
	dlg.m_dSpeed			= m_editSpeed.GetEditValue();
	dlg.m_dKzt				= m_editKZT.GetEditValue();
	dlg.m_dHzt        = 0.0;
	dlg.m_bTopography = m_chkTopoEffect.GetCheck();
	dlg.m_dWidth			= m_dBYKBC2009;
	dlg.m_dBreadth		= m_dBXKBC2009;
	dlg.m_dFrequencyX = m_dFreqXKBC2009;
	dlg.m_dFrequencyY = m_dFreqYKBC2009;
	dlg.m_dDamping	  = m_dDampingKBC2009;  
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, dlg.m_nRigidity); 
	
	if (dlg.DoModal() == IDOK)
	{
		m_editGustFactorX.SetEditUnit(dlg.m_dGustFactorX, 4);
		m_editGustFactorY.SetEditUnit(dlg.m_dGustFactorY, 4);
		
		m_dBYKBC2009			= dlg.m_dWidth;
		m_dBXKBC2009			= dlg.m_dBreadth;
		m_dFreqXKBC2009		= dlg.m_dFrequencyX;
		m_dFreqYKBC2009		= dlg.m_dFrequencyY;
		m_dDampingKBC2009 = dlg.m_dDamping;		
	}	
}

void CCMWindItemKBC2009::OnCmdLoadEvalCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	BOOL bEnable = (nCheck == 1) ? TRUE : FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef, bEnable);
	
	if (bEnable)  OnCmdChangeRdo_ForceCoefCalcType();
}

void CCMWindItemKBC2009::OnCmdTopoEffectCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopoEffect.GetCheck();
	m_editKZT.EnableWindow(nCheck == 1);
}

void CCMWindItemKBC2009::OnCmdChangeRdo_ForceCoefCalcType()
{
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheck);
	
	BOOL bEnableUser = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER) ? TRUE : FALSE;
	BOOL bEnableAuto = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO) ? TRUE : FALSE;
	
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_user, bEnableUser);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_auto, bEnableAuto);
}

void CCMWindItemKBC2009::OnBtnAutoCalc()
{
	T_WIND_D WindD;
	m_pParent->GetWindData(WindD);
	CWindAutoCalcDlg dlg(NULL,WindD);
	dlg.SetData(m_WnatD);
	if(dlg.DoModal() == IDOK)
	{
		m_WnatD.arWnatStor.RemoveAll();
		dlg.GetData(m_WnatD);
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemKBC2009::InitDefaultData()
{
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
	
	// 폭 구하기 
	double dBx = 0.0;
	double dBy = 0.0;
	if (nStorNum > 0)
	{	
		for (int i=0; i<nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
			if (StorD2.dStoryLevel > dBaseLevel) break;
		}
		if(StorD2.dStoryLevel > dBaseLevel) 
		{
			dBx = StorD2.WindWidthX;
			dBy = StorD2.WindWidthY;
		}
		else
		{
			dBx = 0.0; dBy = 0.0;
		}
	}
	// 초기화
	m_dBXKBC2009 = dBx;	
	m_dBYKBC2009 = dBy;	
	m_dFreqXKBC2009 = 0.0;	
	m_dFreqYKBC2009 = 0.0;	
	m_dDampingKBC2009 = 0.0;
	
	if (!m_bModify) 
	{
		m_Data.dRoofHeight	= StorD.dStoryLevel - dBaseLevel;	
		m_editGustFactorX.SetWindowText(_T("2.2")); 
		m_editGustFactorY.SetWindowText(_T("2.2"));
	}
}

void CCMWindItemKBC2009::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	
	int nIndex;
	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxCategory.AddString(aItem[i]);
		m_cobxCategory.SetItemData(nIndex , i+1);
	}
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemKBC2009::InitImportFactorCombo()
{
	m_cobxImportFactor.ResetContent();
	
	CString aItem[] = {_T("0.9"), _T("0.95"), _T("1.0"), _T("1.1")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxImportFactor.AddString(aItem[i]);
	m_cobxImportFactor.SetCurSel(0); 
}

void CCMWindItemKBC2009::Data2Dlg()
{
	CString csVal;
	
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory-1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	
	if (m_Data.dImportanceFactor == 0.9  || m_Data.dImportanceFactor == 0.95 ||
		m_Data.dImportanceFactor == 1.0  || m_Data.dImportanceFactor == 1.1)
	{
		csVal.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxImportFactor.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxImportFactor.SetWindowText(csVal);
	}
	
	m_editRoofHeight.SetEditUnit(m_Data.dRoofHeight);
	m_chkTopoEffect.SetCheck(m_Data.bTopographicEffect);
	OnCmdTopoEffectCheck();
	m_editKZT.SetEditUnit(m_Data.dKzt);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlType, m_Data.nRigidity);
	m_editGustFactorX.SetEditUnit(m_Data.dGustFactorX, 4);
	m_editGustFactorY.SetEditUnit(m_Data.dGustFactorY, 4);
	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	int nCheckForceCoefType = (m_Data.bAutoCalc)? D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO : D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
	OnCmdLoadEvalCheck();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoefficient);
}

BOOL CCMWindItemKBC2009::Dlg2Data()
{
	CString csVal;
	
	T_WNAT_K WnatK = m_Data.keyWnat;

	m_Data.Initialize();

	if(m_bModify) m_Data.keyWnat = WnatK;
	
	m_Data.nExposureCategory = m_cobxCategory.GetCurSel() + 1;
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	
	m_cobxImportFactor.GetWindowText(csVal);
	m_Data.dImportanceFactor = _tstof(csVal);
	
	m_Data.dRoofHeight = m_editRoofHeight.GetEditValue();
	m_Data.bTopographicEffect = m_chkTopoEffect.GetCheck();
	if (m_Data.bTopographicEffect)
	{
		m_Data.dKzt = m_editKZT.GetEditValue();
		m_Data.dVzt = 0.0;
	}
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, m_Data.nRigidity);
	m_Data.dGustFactorX = m_editGustFactorX.GetEditValue();
	m_Data.dGustFactorY = m_editGustFactorY.GetEditValue();
	m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
	if (m_Data.bUseForceCoef)
		m_Data.dForceCoefficient = m_editForceCoef.GetEditValue();
	
	int nCheckForceCoefType = D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
	m_Data.bAutoCalc = (nCheckForceCoefType == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO)? TRUE : FALSE;
	
	return TRUE;
}

LRESULT CCMWindItemKBC2009::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}

#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER
#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO