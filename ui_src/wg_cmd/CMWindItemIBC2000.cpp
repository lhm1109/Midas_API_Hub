// CMWindItemIBC2000.cpp : implementation file
//
// 2003. 09. 22    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindItemIBC2000.h"
#include "WindAutoCalcDlg.h"

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
// CCMWindItemIBC2000 dialog


CCMWindItemIBC2000::CCMWindItemIBC2000(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindItemIBC2000::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;
	//{{AFX_DATA_INIT(CCMWindItemIBC2000)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_aCtrlPrecedure.Add(IDC_CMD_SIMPLE_RD1);
	m_aCtrlPrecedure.Add(IDC_CMD_ANAL_RD2);
	
	m_aCtrlStructType.Add(IDC_CMD_ANAL_RIGID_RD);
	m_aCtrlStructType.Add(IDC_CMD_ANAL_FLEX_RD);
	
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_EDIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_UNIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_IFACTOR_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_IFACTOR_COMBO);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_CATEGORY_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_CATEGORY_COMBO);
	
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_SPEED_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_SPEED_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_SPEED_UNIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_IFACTOR_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_IFACTOR_COMBO);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_CATEGORY_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_CATEGORY_COMBO);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_DFACTOR_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_DFACTOR_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_ROOFH_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_ROOFH_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_ROOFH_UNIT);
	
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_RIGID_RD);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FLEX_RD);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GX_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GX_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GY_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GY_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_BUTTON);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_TOPO_CHECK);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FRAME1);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_TOPO_CHECK);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_HILLSHAPE_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_HILLSHAPE_COMBO);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_BLOCATION_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_BLOCATION_COMBO);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_HHEIGHT_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_HHEIGHT_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_HHEIGHT_UNIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_HLENGTH_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_HLENGTH_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_HLENGTH_UNIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_CBDIST_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_CBDIST_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_CBDIST_UNIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FRAME2);
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
	
	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
	m_WnatD.Initialize();
}

void CCMWindItemIBC2000::SetData2Dlg(T_WIND_IBC2000& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl2->GetWnat(m_Data.keyWnat, m_WnatD)) m_WnatD.Initialize();
}

BOOL CCMWindItemIBC2000::SetDlg2Data(T_WIND_IBC2000& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemIBC2000::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemIBC2000)
	DDX_Control(pDX, IDC_CMD_SIMPLE_SPEED_UNIT, m_unitSpeed2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_SPEED_EDIT, m_editSpeed2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_IFACTOR_COMBO, m_cobxIFactor2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_CATEGORY_COMBO, m_cobxCategory2);
	DDX_Control(pDX, IDC_CMD_ANAL_TOPO_CHECK, m_chkTopographic);
	DDX_Control(pDX, IDC_CMD_ANAL_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_ANAL_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ANAL_ROOFH_UNIT, m_unitRoofH);
	DDX_Control(pDX, IDC_CMD_ANAL_ROOFH_EDIT, m_editRoofH);
	DDX_Control(pDX, IDC_CMD_ANAL_LOADEVAL_CHK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_ANAL_IFACTOR_COMBO, m_cobxIFactor);
	DDX_Control(pDX, IDC_CMD_ANAL_HLENGTH_UNIT, m_unitHillLeng);
	DDX_Control(pDX, IDC_CMD_ANAL_HLENGTH_EDIT, m_editHillLeng);
	DDX_Control(pDX, IDC_CMD_ANAL_HILLSHAPE_COMBO, m_cobxHillShape);
	DDX_Control(pDX, IDC_CMD_ANAL_HHEIGHT_UNIT, m_unitHHeight);
	DDX_Control(pDX, IDC_CMD_ANAL_HHEIGHT_EDIT, m_editHHeight);
	DDX_Control(pDX, IDC_CMD_ANAL_GUST_GY_EDIT, m_editGustGy);
	DDX_Control(pDX, IDC_CMD_ANAL_GUST_GX_EDIT, m_editGustGx);
	DDX_Control(pDX, IDC_CMD_ANAL_FCOEF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_ANAL_DFACTOR_EDIT, m_editDFactor);
	DDX_Control(pDX, IDC_CMD_ANAL_CBDIST_UNIT, m_unitCBDistance);
	DDX_Control(pDX, IDC_CMD_ANAL_CBDIST_EDIT, m_editCBDistance);
	DDX_Control(pDX, IDC_CMD_ANAL_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_ANAL_BLOCATION_COMBO, m_cobxBLocation);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemIBC2000, CChildDialog)
//{{AFX_MSG_MAP(CCMWindItemIBC2000)
	ON_BN_CLICKED(IDC_CMD_SIMPLE_RD1, OnCmdSimpleAnalRadio)
	ON_BN_CLICKED(IDC_CMD_ANAL_RD2, OnCmdSimpleAnalRadio)
	ON_CBN_SELCHANGE(IDC_CMD_ANAL_CATEGORY_COMBO, OnSelchangeCmdAnalCategoryCombo)
	ON_BN_CLICKED(IDC_CMD_ANAL_FLEX_RD, OnCmdAnalRigidFlexRd)
	ON_BN_CLICKED(IDC_CMD_ANAL_RIGID_RD, OnCmdAnalRigidFlexRd)
	ON_BN_CLICKED(IDC_CMD_ANAL_GUST_BUTTON, OnCmdAnalGustButton)
	ON_BN_CLICKED(IDC_CMD_ANAL_LOADEVAL_CHK, OnCmdAnalLoadevalChk)
	ON_BN_CLICKED(IDC_CMD_ANAL_TOPO_CHECK, OnCmdAnalTopoCheck)
	ON_CBN_SELCHANGE(IDC_CMD_ANAL_HILLSHAPE_COMBO, OnSelchangeCmdAnalHillshapeCombo)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_USER_RDO, OnCmdChangeRdo_ForceCoefCalcType)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_RDO, OnCmdChangeRdo_ForceCoefCalcType)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_RDO, OnCmdChangeRdo_ForceCoefCalcType)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_BTN, OnBtnAutoCalc)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIBC2000 message handlers

BOOL CCMWindItemIBC2000::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	
	InitControls();
	AlignControls();
		m_grpWindParam.InitControl(this, CCMWindItemIBC2000::IDD, IDC_CMD_FRAME, TRUE, TRUE); // AlignControls 다음에
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemIBC2000::DestroyWindow() 
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

void CCMWindItemIBC2000::OnCmdSimpleAnalRadio() 
{
	// TODO: Add your control notification handler code here
	int nPrecedure;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPrecedure, nPrecedure);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSimple, nPrecedure == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlAnalytic, nPrecedure == 1);
}

void CCMWindItemIBC2000::OnSelchangeCmdAnalCategoryCombo() 
{
	// TODO: Add your control notification handler code here
	int nCategory = m_cobxCategory.GetCurSel();
	//CWnd* pWnd = GetDlgItem(IDC_CMD_ANAL_TOPO_CHECK); ASSERT(pWnd);
	m_chkTopographic.EnableWindow(nCategory != 0);
	
	if (nCategory == 0)
	{
		m_chkTopographic.SetCheck(0);
		OnCmdAnalTopoCheck();
	}
}

void CCMWindItemIBC2000::OnCmdAnalRigidFlexRd() 
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

void CCMWindItemIBC2000::OnCmdAnalGustButton() 
{
	// TODO: Add your control notification handler code here
	int nRigidFlex;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructType, nRigidFlex);
	
	if (nRigidFlex == 0)
	{
		CWindGustFIBC2000Dlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.m_nExposure = m_cobxCategory.GetCurSel();
		dlg.m_dheight = m_editRoofH.GetEditValue();
		dlg.m_dBX = m_dBYIBC2000;
		dlg.m_dBY = m_dBXIBC2000;		
		
		if(dlg.DoModal() == IDOK)
		{
			m_editGustGx.SetEditUnit(dlg.m_dXFactor);
			m_editGustGy.SetEditUnit(dlg.m_dYFactor);
			m_dBXIBC2000 = dlg.m_dBY;
			m_dBYIBC2000 = dlg.m_dBX;
		}
	}
	else
	{
		CWindGustF2IBC2000Dlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.m_dBasicWind = m_editSpeed.GetEditValue();
		dlg.m_nExposure = m_cobxCategory.GetCurSel();
		dlg.m_dheight = m_editRoofH.GetEditValue();
		dlg.m_dBX = m_dBYIBC2000;
		dlg.m_dBY = m_dBXIBC2000;
		dlg.m_dXNatural = m_dXNaturalIBC2000;
		dlg.m_dYNatural = m_dYNaturalIBC2000;
		dlg.m_dDamping	= m_dDampingIBC2000;
		
		if(dlg.DoModal() == IDOK)
		{
			m_editGustGx.SetEditUnit(dlg.m_dXFactor);
			m_editGustGy.SetEditUnit(dlg.m_dYFactor);
			m_dBXIBC2000 = dlg.m_dBY;
			m_dBYIBC2000 = dlg.m_dBX;
			m_dXNaturalIBC2000 = dlg.m_dXNatural;  
			m_dYNaturalIBC2000 = dlg.m_dYNatural; 
			m_dDampingIBC2000  = dlg.m_dDamping;	
		}
	}	
}

void CCMWindItemIBC2000::OnCmdAnalLoadevalChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	BOOL bEnable = (nCheck == 1)? TRUE : FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef, bEnable);
	
	if (bEnable)  OnCmdChangeRdo_ForceCoefCalcType();
}

void CCMWindItemIBC2000::OnCmdAnalTopoCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic.GetCheck();
	m_cobxHillShape.EnableWindow(nCheck == 1);
	m_cobxBLocation.EnableWindow(nCheck == 1);
	m_editHHeight.EnableWindow(nCheck == 1);
	m_editHillLeng.EnableWindow(nCheck == 1);
	m_editCBDistance.EnableWindow(nCheck == 1);
	OnSelchangeCmdAnalHillshapeCombo();
}

void CCMWindItemIBC2000::OnSelchangeCmdAnalHillshapeCombo() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic.GetCheck();
	if (!nCheck) return;
	
	int nHillShape = m_cobxHillShape.GetCurSel();
	m_cobxBLocation.EnableWindow(nHillShape == 1);
}

void CCMWindItemIBC2000::OnCmdChangeRdo_ForceCoefCalcType()
{
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheck);
	
	BOOL bEnableUser = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER) ? TRUE : FALSE;
	BOOL bEnableAuto = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO) ? TRUE : FALSE;
	
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_user, bEnableUser);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_auto, bEnableAuto);	
}

void CCMWindItemIBC2000::OnBtnAutoCalc()
{
	T_WIND_D WindD;
	m_pParent->GetWindData(WindD);
	CWindAutoCalcDlg dlg(NULL,WindD);
	dlg.SetData(m_WnatD);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_WnatD);
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemIBC2000::InitDefaultData()
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
	m_dBXIBC2000 = dBx;	
	m_dBYIBC2000 = dBy;	
	m_dXNaturalIBC2000 = 0.0;
	m_dYNaturalIBC2000 = 0.0;
	m_dDampingIBC2000  = 0.0;
	
	if (!m_bModify) 
		m_Data.dHRoof = StorD.dStoryLevel - dBaseLevel;				
}

void CCMWindItemIBC2000::InitControls()
{
	//m_unitSpeed2.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dBasicWindSpeed);
	m_editSpeed2.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dBasicWindSpeed);
	//m_unitSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dBasicWindSpeed);
	m_editSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dBasicWindSpeed);
	m_unitRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dHRoof);
	m_editRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dHRoof);
	m_unitHillLeng.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dHillLength);
	m_editHillLeng.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dHillLength);
	m_unitHHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dHillHeight);
	m_editHHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dHillHeight);
	m_editGustGy.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dGustFactorY);
	m_editGustGx.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dGustFactorX);
	m_editDFactor.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dWindDirectionFactor);
	m_unitCBDistance.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dHillDistance);
	m_editCBDistance.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2000_dHillDistance);
	m_editForceCoef.SetUnitType(D_UNITSYS_NONE);
	
	InitIFactorCombo();
	InitCategoryCombo();
	InitHillShapeCombo();
	InitBLocationCombo();
}

void CCMWindItemIBC2000::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();
	m_cobxIFactor2.ResetContent();
	
	CString aItem[] = { _T("0.77"), _T("0.87"), _T("1.00"), _T("1.15")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxIFactor.AddString(aItem[i]);
		m_cobxIFactor2.AddString(aItem[i]);
	}
	m_cobxIFactor.SetCurSel(0);
	m_cobxIFactor2.SetCurSel(0);
}

void CCMWindItemIBC2000::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	m_cobxCategory2.ResetContent();
	
	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxCategory.AddString(aItem[i]);
		if (i != 0) m_cobxCategory2.AddString(aItem[i]);
	}
	m_cobxCategory.SetCurSel(0);
	m_cobxCategory2.SetCurSel(0);
}

void CCMWindItemIBC2000::InitHillShapeCombo()
{
	m_cobxHillShape.ResetContent();
	
	//CString aItem[] = {_T("2D-Ridge or Valley"), _T("2-D Escarpment"), _T("3-D Axisym. Hill")};
	CString aItem[] = {_LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_2DRIDGE), 
										 _LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_2DESCARP),
										 _LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_3DAXISYM)};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxHillShape.AddString(aItem[i]);
	
	m_cobxHillShape.SetCurSel(0);
}

void CCMWindItemIBC2000::InitBLocationCombo()
{
	m_cobxBLocation.ResetContent();
	//CString aItem[] = {_T("Upwind"), _T("Downwind")};
	CString aItem[] = {_LS(IDS_CMD_WIND_NBC_BLDG_LOCATION_UP), _LS(IDS_CMD_WIND_NBC_BLDG_LOCATION_DN)};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxBLocation.AddString(aItem[i]);
	
	m_cobxBLocation.SetCurSel(0);
}

void CCMWindItemIBC2000::AlignControls()
{
	CRect ToRect, FromRect;
	m_editSpeed2.GetWindowRect(FromRect);
	m_editSpeed.GetWindowRect(ToRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSimple, nDistY);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_FRAME);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+4;
	MoveWindow(r);
}

void CCMWindItemIBC2000::Data2Dlg()
{
	CString  csVal;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPrecedure, m_Data.nProcedure - 1);
	OnCmdSimpleAnalRadio();
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory - 1);
	m_cobxCategory2.SetCurSel(m_Data.nExposureCategory - 2);
	OnSelchangeCmdAnalCategoryCombo();
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editSpeed2.SetEditUnit(m_Data.dBasicWindSpeed);
	csVal.Format(_T("%g"), m_Data.dImportanceFactor);
	if(CB_ERR == m_cobxIFactor.FindStringExact(-1, csVal))
	{
		m_cobxIFactor.SetWindowText(csVal);
	}
	else
	{
		m_cobxIFactor.SelectString(-1, csVal);
	}
	//m_cobxIFactor.SelectString(-1, csVal);
	csVal.Format(_T("%g"), m_Data.dImportanceFactor);
	if(CB_ERR == m_cobxIFactor2.FindStringExact(-1, csVal))
	{
		m_cobxIFactor2.SetWindowText(csVal);
	}
	else
	{
		m_cobxIFactor2.SelectString(-1, csVal);
	}
	//m_cobxIFactor2.SelectString(-1, csVal);
	m_editDFactor.SetEditUnit(m_Data.dWindDirectionFactor);
	m_editRoofH.SetEditUnit(m_Data.dHRoof);
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlStructType, m_Data.nRigidity);
	OnCmdAnalRigidFlexRd();
	m_editGustGx.SetEditUnit(m_Data.dGustFactorX);
	m_editGustGy.SetEditUnit(m_Data.dGustFactorY);
	m_chkTopographic.SetCheck(m_Data.bTopography);
	OnCmdAnalTopoCheck();
	m_cobxHillShape.SetCurSel(m_Data.nHillShape);
	OnSelchangeCmdAnalHillshapeCombo();
	m_cobxBLocation.SetCurSel(m_Data.nBldgLocation);
	m_editHHeight.SetEditUnit(m_Data.dHillHeight);
	m_editHillLeng.SetEditUnit(m_Data.dHillLength);
	m_editCBDistance.SetEditUnit(m_Data.dHillDistance);
	
	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	int nCheckForceCoefType = (m_Data.bAutoCalc)? D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO : D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
	OnCmdAnalLoadevalChk();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoefficient);
}

BOOL CCMWindItemIBC2000::Dlg2Data()
{
	T_WNAT_K WnatK = m_Data.keyWnat;
	
	m_Data.Initialize();
		
	if(m_bModify) m_Data.keyWnat = WnatK;
	
	CString  csVal;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPrecedure, m_Data.nProcedure); m_Data.nProcedure += 1;
	if (m_Data.nProcedure == 1)
	{
		m_Data.dBasicWindSpeed = m_editSpeed2.GetEditValue();
		m_cobxIFactor2.GetWindowText(csVal);
		m_Data.dImportanceFactor = _tstof(csVal);
		m_Data.nExposureCategory = m_cobxCategory2.GetCurSel() + 2;
	}
	else 
	{
		m_Data.nExposureCategory = m_cobxCategory.GetCurSel() + 1;
		m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
		m_cobxIFactor.GetWindowText(csVal);
		m_Data.dImportanceFactor = _tstof(csVal);
		m_Data.dWindDirectionFactor = m_editDFactor.GetEditValue();
		m_Data.dHRoof = m_editRoofH.GetEditValue();
		
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructType, m_Data.nRigidity);
		m_Data.dGustFactorX = m_editGustGx.GetEditValue();
		m_Data.dGustFactorY = m_editGustGy.GetEditValue();
		
		m_Data.bTopography = m_chkTopographic.GetCheck();
		if (m_Data.bTopography)
		{
			m_Data.nHillShape = m_cobxHillShape.GetCurSel();
			m_Data.nBldgLocation = m_cobxBLocation.GetCurSel();
			m_Data.dHillHeight = m_editHHeight.GetEditValue();
			m_Data.dHillLength = m_editHillLeng.GetEditValue();
			m_Data.dHillDistance = m_editCBDistance.GetEditValue();
		}
		
		m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
		if (m_Data.bUseForceCoef)
			m_Data.dForceCoefficient = m_editForceCoef.GetEditValue();
		
		int nCheckForceCoefType = D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
		m_Data.bAutoCalc = (nCheckForceCoefType == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO)? TRUE : FALSE;
	}
	
	return TRUE;
}

LRESULT CCMWindItemIBC2000::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		BOOL bFolded = m_grpWindParam.GetFoldState();
		if (!bFolded)
		{
				OnCmdSimpleAnalRadio();
		}

		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}

#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER
#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO
