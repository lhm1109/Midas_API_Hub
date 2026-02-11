// CMWindItemIBC2012.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindItemIBC2012.h"
#include "WindAutoCalcDlg.h"
#include "CMWindItemIBC2012Topography.h"

#include "WindGustFIBC2000Dlg.h"
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
// CCMWindItemIBC2012 dialog


CCMWindItemIBC2012::CCMWindItemIBC2012(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemIBC2012::IDD, pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemIBC2012)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aCtrlPrecedure.Add(IDC_CMD_SIMPLE_RD1);
	m_aCtrlPrecedure.Add(IDC_CMD_ANAL_RD2);

	m_aCtrlStructType.Add(IDC_CMD_ANAL_RIGID_RD);
	m_aCtrlStructType.Add(IDC_CMD_ANAL_FLEX_RD);

	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_CNET_FRAME);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_WINDWARD_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_WINDWARD_EDIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_LEEWARD_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_LEEWARD_EDIT);

	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_DFACTOR_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_KDX_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_DFACTOR_X_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_KDY_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_DFACTOR_Y_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_RIGID_RD);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FLEX_RD);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GX_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GX_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GY_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GY_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_BUTTON);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_LOADEVAL_FRAME);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_LOADEVAL_CHK);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FCOEF_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FCOEF_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_FORCE_COEF_USER_RDO);
	m_aCtrlAnalytic.Add(IDC_CMD_FORCE_COEF_AUTO_RDO);
	m_aCtrlAnalytic.Add(IDC_CMD_FORCE_COEF_AUTO_BTN);

	m_aRdoForceCoefCalcType.RemoveAll();
	m_aRdoForceCoefCalcType.Add(IDC_CMD_FORCE_COEF_USER_RDO);
	m_aRdoForceCoefCalcType.Add(IDC_CMD_FORCE_COEF_AUTO_RDO);
	m_aRdoForceCoefCalcType.FreeExtra();

	m_aCtrlForceCoef_user.RemoveAll();
	m_aCtrlForceCoef_user.Add(IDC_CMD_ANAL_FCOEF_TITLE);
	m_aCtrlForceCoef_user.Add(IDC_CMD_ANAL_FCOEF_EDIT);
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

	m_aCtrlExRadio.Add(IDC_CMD_XDIR_POS);
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_NEG);
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_NONE);

	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_POS);
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NEG);
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NONE);

	m_bInit = FALSE;
	m_bModify = FALSE;

	m_Data.Initialize();
	m_WnatD.Initialize();

	m_nType = 17; // IBC2012
}

void CCMWindItemIBC2012::SetData2Dlg(T_WIND_IBC2012& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl2->GetWnat(m_Data.keyWnat, m_WnatD)) m_WnatD.Initialize();
}

BOOL CCMWindItemIBC2012::SetDlg2Data(T_WIND_IBC2012& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemIBC2012::SetIBCType(int nType)
{
	m_nType = nType;
	SetCtrlByIBCType();
}

void CCMWindItemIBC2012::SetCtrlByIBCType()
{
	BOOL bShow = FALSE;
	if (m_nType == 16)
	{
		bShow = TRUE;
		SetDlgItemText(IDC_CMD_ANAL_RD2, _LS(IDS_WG_CMD_ANAL_PROCEDURE));
	}
	else
		SetDlgItemText(IDC_CMD_ANAL_RD2, _LS(IDS_WG_CMD_DIR_PROCEDURE));

	GetDlgItem(IDC_CMD_ANAL_IFACTOR_TITLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_ANAL_IFACTOR_COMBO)->ShowWindow(bShow);
}

void CCMWindItemIBC2012::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemIBC2012)
	DDX_Control(pDX, IDC_CMD_ANAL_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_ANAL_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ANAL_ROOFH_UNIT, m_unitRoofH);
	DDX_Control(pDX, IDC_CMD_ANAL_ROOFH_EDIT, m_editRoofH);
	DDX_Control(pDX, IDC_CMD_ANAL_LOADEVAL_CHK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_ANAL_IFACTOR_COMBO, m_cobxIFactor);
	DDX_Control(pDX, IDC_CMD_ANAL_GUST_GY_EDIT, m_editGustGy);
	DDX_Control(pDX, IDC_CMD_ANAL_GUST_GX_EDIT, m_editGustGx);
	DDX_Control(pDX, IDC_CMD_ANAL_FCOEF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_ANAL_DFACTOR_X_EDIT, m_editDFactorX);
	DDX_Control(pDX, IDC_CMD_ANAL_DFACTOR_Y_EDIT, m_editDFactorY);
	DDX_Control(pDX, IDC_CMD_ANAL_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_SIMPLE_WINDWARD_EDIT, m_editWindward);
	DDX_Control(pDX, IDC_CMD_SIMPLE_LEEWARD_EDIT, m_editLeeward);
	DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemIBC2012, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemIBC2012)
	ON_BN_CLICKED(IDC_CMD_SIMPLE_RD1, OnCmdSimpleAnalRadio)
	ON_BN_CLICKED(IDC_CMD_ANAL_RD2, OnCmdSimpleAnalRadio)
	ON_BN_CLICKED(IDC_CMD_ANAL_FLEX_RD, OnCmdAnalRigidFlexRd)
	ON_BN_CLICKED(IDC_CMD_ANAL_RIGID_RD, OnCmdAnalRigidFlexRd)
	ON_BN_CLICKED(IDC_CMD_ANAL_GUST_BUTTON, OnCmdAnalGustButton)
	ON_BN_CLICKED(IDC_CMD_ANAL_LOADEVAL_CHK, OnCmdAnalLoadevalChk)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_USER_RDO, OnCmdChangeRdo_ForceCoefCalcType)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_RDO, OnCmdChangeRdo_ForceCoefCalcType)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_BTN, OnBtnAutoCalc)
	ON_BN_CLICKED(IDC_CMD_TOPOGRAPHY_BUTTON, OnBtnTopography)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIBC2012 message handlers

BOOL CCMWindItemIBC2012::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitControls();
	AlignControls();

	// After AlignControls
	m_grpWindParam.InitControl(this, CCMWindItemIBC2012::IDD, IDC_CMD_FRAME, TRUE, TRUE);
	m_grpWindParam.SetFoldState(FALSE); // open

	Data2Dlg();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemIBC2012::DestroyWindow()
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

void CCMWindItemIBC2012::OnCmdSimpleAnalRadio()
{
	// TODO: Add your control notification handler code here
	int nPrecedure;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPrecedure, nPrecedure);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSimple, nPrecedure == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlAnalytic, nPrecedure == 1);
}

void CCMWindItemIBC2012::OnCmdAnalRigidFlexRd()
{
	// TODO: Add your control notification handler code here
	int nRigidFlex;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructType, nRigidFlex);
	if (nRigidFlex == 0)
	{
		GetDlgItem(IDC_CMD_ANAL_GUST_GX_TITLE)->SetWindowText(_T("Gx"));
		GetDlgItem(IDC_CMD_ANAL_GUST_GY_TITLE)->SetWindowText(_T("Gy"));
	}
	else
	{
		GetDlgItem(IDC_CMD_ANAL_GUST_GX_TITLE)->SetWindowText(_T("Gfx"));
		GetDlgItem(IDC_CMD_ANAL_GUST_GY_TITLE)->SetWindowText(_T("Gfy"));
	}
}

void CCMWindItemIBC2012::OnCmdAnalGustButton()
{
	// TODO: Add your control notification handler code here
	int nRigidFlex;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructType, nRigidFlex);

	if (nRigidFlex == 0)
	{
		CWindGustFIBC2000Dlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.m_nExposure = m_cobxCategory.GetCurSel() + 1;
		dlg.m_dheight = m_editRoofH.GetEditValue();
		dlg.m_dBX = m_dBYIBC2012;
		dlg.m_dBY = m_dBXIBC2012;

		if (dlg.DoModal() == IDOK)
		{
			m_editGustGx.SetEditUnit(dlg.m_dXFactor);
			m_editGustGy.SetEditUnit(dlg.m_dYFactor);
			m_dBXIBC2012 = dlg.m_dBY;
			m_dBYIBC2012 = dlg.m_dBX;
		}
	}
	else
	{
		CWindGustF2IBC2000Dlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.m_dBasicWind = m_editSpeed.GetEditValue();
		dlg.m_nExposure = m_cobxCategory.GetCurSel() + 1;
		dlg.m_dheight = m_editRoofH.GetEditValue();
		dlg.m_dBX = m_dBYIBC2012;
		dlg.m_dBY = m_dBXIBC2012;
		dlg.m_dXNatural = m_dXNaturalIBC2012;
		dlg.m_dYNatural = m_dYNaturalIBC2012;
		dlg.m_dDamping = m_dDampingIBC2012;

		if (dlg.DoModal() == IDOK)
		{
			m_editGustGx.SetEditUnit(dlg.m_dXFactor);
			m_editGustGy.SetEditUnit(dlg.m_dYFactor);
			m_dBXIBC2012 = dlg.m_dBY;
			m_dBYIBC2012 = dlg.m_dBX;
			m_dXNaturalIBC2012 = dlg.m_dXNatural;
			m_dYNaturalIBC2012 = dlg.m_dYNatural;
			m_dDampingIBC2012 = dlg.m_dDamping;
		}
	}
}

void CCMWindItemIBC2012::OnCmdAnalLoadevalChk()
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	BOOL bEnable = (nCheck == 1) ? TRUE : FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef, bEnable);

	if (bEnable) OnCmdChangeRdo_ForceCoefCalcType();
}

void CCMWindItemIBC2012::OnCmdChangeRdo_ForceCoefCalcType()
{
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheck);

	BOOL bEnableUser = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER) ? TRUE : FALSE;
	BOOL bEnableAuto = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO) ? TRUE : FALSE;

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_user, bEnableUser);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_auto, bEnableAuto);
}

void CCMWindItemIBC2012::OnBtnAutoCalc()
{
	T_WIND_D WindD;
	m_pParent->GetWindData(WindD);
	CWindAutoCalcDlg dlg(NULL, WindD);
	dlg.SetData(m_WnatD);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_WnatD);
	}
}

void CCMWindItemIBC2012::OnBtnTopography()
{
	CCMWindItemIBC2012Topography dlg(&m_Data);
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemIBC2012::InitDefaultData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 층개수와 Base Level 구하기 
	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	T_STOR_D StorD, StorD2;
	CArray<T_STOR_K,T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0) pDoc->m_pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	else StorD.Initialize();

	// 폭 구하기 
	double dBx = 0.0;
	double dBy = 0.0;
	if (nStorNum > 0)
	{
		for (int i = 0; i < nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
			if (StorD2.dStoryLevel > dBaseLevel) break;
		}
		if (StorD2.dStoryLevel > dBaseLevel)
		{
			dBx = StorD2.WindWidthX;
			dBy = StorD2.WindWidthY;
		}
		else
		{
			dBx = 0.0;
			dBy = 0.0;
		}
	}
	// 초기화
	m_dBXIBC2012 = dBx;
	m_dBYIBC2012 = dBy;
	m_dXNaturalIBC2012 = 0.0;
	m_dYNaturalIBC2012 = 0.0;
	m_dDampingIBC2012 = 0.0;

	if (!m_bModify)
		m_Data.dHRoof = StorD.dStoryLevel - dBaseLevel;
}

void CCMWindItemIBC2012::InitControls()
{
	m_unitRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHRoof);
	m_editRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHRoof);

	InitIFactorCombo();
	InitCategoryCombo();

	SetCtrlByIBCType();
}

void CCMWindItemIBC2012::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();

	CString aItem[] = {_T("0.77"), _T("0.87"), _T("1.00"), _T("1.15")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i = 0; i < nNum; i++)
		m_cobxIFactor.AddString(aItem[i]);
	m_cobxIFactor.SetCurSel(2);
}

void CCMWindItemIBC2012::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i = 0; i < nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(1);
}

void CCMWindItemIBC2012::AlignControls()
{
	CRect ToRect, FromRect;
	GetDlgItem(IDC_CMD_SIMPLE_CNET_FRAME)->GetWindowRect(FromRect);
	GetDlgItem(IDC_CMD_ANAL_DFACTOR_TITLE)->GetWindowRect(ToRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSimple, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_FRAME);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + 4;
	MoveWindow(r);
}

void CCMWindItemIBC2012::Data2Dlg()
{
	CString csVal;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPrecedure, m_Data.nProcedure - 1);
	OnCmdSimpleAnalRadio();
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory - 2);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editDFactorX.SetEditUnit(m_Data.dDirectionFactorX);
	m_editDFactorY.SetEditUnit(m_Data.dDirectionFactorY);
	m_editRoofH.SetEditUnit(m_Data.dHRoof);
	m_editWindward.SetEditUnit(m_Data.dCnetWindward);
	m_editLeeward.SetEditUnit(m_Data.dCnetLeeward);

	// IBC2009
	csVal.Format(_T("%g"), m_Data.dImportanceFactor);
	if (CB_ERR == m_cobxIFactor.FindStringExact(-1, csVal))
	{
		m_cobxIFactor.SetWindowText(csVal);
	}
	else
	{
		m_cobxIFactor.SelectString(-1, csVal);
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlStructType, m_Data.nRigidity);
	OnCmdAnalRigidFlexRd();
	m_editGustGx.SetEditUnit(m_Data.dGustFactorX);
	m_editGustGy.SetEditUnit(m_Data.dGustFactorY);

	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	int nCheckForceCoefType = (m_Data.bAutoCalc)
		                          ? D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO
		                          : D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
	OnCmdAnalLoadevalChk();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoefficient);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);
}

BOOL CCMWindItemIBC2012::Dlg2Data()
{
	T_WNAT_K WnatK = m_Data.keyWnat;

	if (m_bModify) m_Data.keyWnat = WnatK;
	else m_Data.keyWnat = 0;

	CString csVal;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPrecedure, m_Data.nProcedure);
	m_Data.nProcedure += 1;
	m_Data.nExposureCategory = m_cobxCategory.GetCurSel() + 2;
	m_cobxIFactor.GetWindowText(csVal);
	m_Data.dImportanceFactor = _tstof(csVal);
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.dHRoof = m_editRoofH.GetEditValue();

	if (m_Data.nProcedure == 1)
	{
		m_Data.dCnetWindward = m_editWindward.GetEditValue();
		m_Data.dCnetLeeward = m_editLeeward.GetEditValue();
	}
	else
	{
		m_Data.dDirectionFactorX = m_editDFactorX.GetEditValue();
		m_Data.dDirectionFactorY = m_editDFactorY.GetEditValue();

		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructType, m_Data.nRigidity);
		m_Data.dGustFactorX = m_editGustGx.GetEditValue();
		m_Data.dGustFactorY = m_editGustGy.GetEditValue();

		m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
		if (m_Data.bUseForceCoef)
			m_Data.dForceCoefficient = m_editForceCoef.GetEditValue();

		int nCheckForceCoefType = D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
		m_Data.bAutoCalc = (nCheckForceCoefType == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO) ? TRUE : FALSE;
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);

	return TRUE;
}

LRESULT CCMWindItemIBC2012::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	BOOL bFolded = m_grpWindParam.GetFoldState();
	if (!bFolded)
	{
		SetCtrlByIBCType();
		OnCmdSimpleAnalRadio();
	}

	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}

#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER
#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO
