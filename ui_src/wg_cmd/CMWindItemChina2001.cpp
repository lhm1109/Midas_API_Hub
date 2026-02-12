// CMWindItemChina2001.cpp : implementation file
//
// 2003. 09. 23  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemChina2001.h"
#include "CMWindItemDlg.h"

#include "SeisChinaCalcDlg.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemChina2001 dialog


CCMWindItemChina2001::CCMWindItemChina2001(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemChina2001::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemChina2001)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlCalc.Add(IDC_CMD_AUTO_CALC_RD);
	m_aCtrlCalc.Add(IDC_CMD_USER_DEF_RD);

	m_aCtrlAuto.Add(IDC_CMD_MATL_COMBO);
	m_aCtrlAuto.Add(IDC_CMD_DAMPING_EDIT);
	m_aCtrlAuto.Add(IDC_CMD_PERIOD_X_EDIT);
	m_aCtrlAuto.Add(IDC_CMD_PERIOD_Y_EDIT);
	m_aCtrlAuto.Add(IDC_CMD_PERIOD_BUTTON);

	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
}

void CCMWindItemChina2001::SetData2Dlg(T_WIND_CH2002& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemChina2001::SetDlg2Data(T_WIND_CH2002& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemChina2001::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemChina2001)
	DDX_Control(pDX, IDC_CMD_WINDWARD_EDIT, m_editWindward);
	DDX_Control(pDX, IDC_CMD_TOPOCOEF_EDIT, m_editTopoCoef);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_PERIOD_Y_EDIT, m_editPeriody);
	DDX_Control(pDX, IDC_CMD_PERIOD_X_EDIT, m_editPeriodx);
	DDX_Control(pDX, IDC_CMD_MATL_COMBO, m_cobxMaterial);
	DDX_Control(pDX, IDC_CMD_MAGFACTOR_Y_EDIT, m_editMagFy);
	DDX_Control(pDX, IDC_CMD_MAGFACTOR_X_EDIT, m_editMagFx);
	DDX_Control(pDX, IDC_CMD_LEEWARD_EDIT, m_editLeeward);
	DDX_Control(pDX, IDC_CMD_DAMPING_EDIT, m_editDamping);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_SPEED_CFT_EDIT2, m_editCheckWindPressure);
	DDX_Control(pDX, IDC_CMD_DAMPING_CFT_EDIT, m_editCheckDamping);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemChina2001, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemChina2001)
	ON_CBN_SELCHANGE(IDC_CMD_CATEGORY_COMBO, OnSelchangeCategoryCombo)
	ON_EN_CHANGE(IDC_CMD_SPEED_EDIT, OnChangeCmdSpeedEdit)
	ON_BN_CLICKED(IDC_CMD_AUTO_CALC_RD, OnCmdAutoUserRadio)
	ON_BN_CLICKED(IDC_CMD_PERIOD_BUTTON, OnCmdPeriodButton)
	ON_CBN_SELCHANGE(IDC_CMD_MATL_COMBO, OnSelchangeCmdMatlCombo)
	ON_BN_CLICKED(IDC_CMD_USER_DEF_RD, OnCmdAutoUserRadio)
	ON_EN_CHANGE(IDC_CMD_PERIOD_X_EDIT, OnChangeCmdPeriodXEdit)
	ON_EN_CHANGE(IDC_CMD_PERIOD_Y_EDIT, OnChangeCmdPeriodYEdit)
	ON_EN_CHANGE(IDC_CMD_DAMPING_EDIT, OnChangeCmdDampingEdit)
	ON_BN_CLICKED(IDC_CMD_WIND_CFT_CHECK, OnCmdComfortCheck)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemChina2001 message handlers

BOOL CCMWindItemChina2001::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemChina2001::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_editWindward.SetUnitType(D_UNITSYS_NONE);
	m_editTopoCoef.SetUnitType(D_UNITSYS_NONE);
	m_editSpeed.SetUnitType(D_UNITSYS_NONE);
	m_editPeriody.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodx.SetUnitType(D_UNITSYS_NONE);
	m_editMagFy.SetUnitType(D_UNITSYS_NONE);
	m_editMagFx.SetUnitType(D_UNITSYS_NONE);
	m_editLeeward.SetUnitType(D_UNITSYS_NONE);
	m_editDamping.SetUnitType(D_UNITSYS_NONE);
	m_editCheckWindPressure.SetUnitType(D_UNITSYS_NONE);
	m_editCheckDamping.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultValue();
		m_bInit = TRUE;
	}

	InitCategoryCombo();
	InitMaterialCombo();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindItemChina2001::DestroyWindow() 
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

void CCMWindItemChina2001::OnCmdAutoUserRadio() 
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCalc, nMethod);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlAuto, nMethod == 0);
	m_editMagFx.SetReadOnly(nMethod == 0);
	m_editMagFy.SetReadOnly(nMethod == 0);
	CalcPulseMagnifyFactor();
}


void CCMWindItemChina2001::OnSelchangeCmdMatlCombo() 
{
	// TODO: Add your control notification handler code here
	int nMaterial = m_cobxMaterial.GetCurSel();
	double dDamping[] = { 0.01, 0.02, 0.05 };
	m_editDamping.SetEditUnit(dDamping[nMaterial]);  
	CalcPulseMagnifyFactor();	
}

void CCMWindItemChina2001::OnChangeCmdPeriodXEdit() 
{
	CalcPulseMagnifyFactor();		
}

void CCMWindItemChina2001::OnChangeCmdPeriodYEdit() 
{
	CalcPulseMagnifyFactor();	
}

void CCMWindItemChina2001::OnChangeCmdDampingEdit() 
{
	CalcPulseMagnifyFactor();		
}

void CCMWindItemChina2001::OnSelchangeCategoryCombo() 
{
	CalcPulseMagnifyFactor();		
}

void CCMWindItemChina2001::OnChangeCmdSpeedEdit() 
{
	CalcPulseMagnifyFactor();		
}

void CCMWindItemChina2001::OnCmdPeriodButton() 
{
	// TODO: Add your control notification handler code here	
	int nUnitChange = 0;   // 주기계산 대화상자에서 wind load시에는 단위변환하고, 
	                       //                       seismic load시에는 단위변환않음			
	CSeisChinaCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nUnitChange = nUnitChange;
	dlg.m_dHX			 = m_dHXCH2001;
	dlg.m_dHY			 = m_dHYCH2001;
	dlg.m_dBX			 = m_dBXCH2001;
	dlg.m_dBY			 = m_dBYCH2001;	
	dlg.m_nNX			 = m_nNXCH2001;
	dlg.m_nNY			 = m_nNYCH2001;	
	dlg.m_nXMethod = m_nMtdXCH2001;
	dlg.m_nYMethod = m_nMtdYCH2001;
	
	if(dlg.DoModal()== IDOK)
	{
		CString cx,cy;
		cx.Format(_T("%g"),dlg.m_dPeriod_X);
		cy.Format(_T("%g"),dlg.m_dPeriod_Y);
		m_editPeriodx.SetWindowText(cx);
		m_editPeriody.SetWindowText(cy);		

		m_dHXCH2001		= dlg.m_dHX;
		m_dHYCH2001		= dlg.m_dHY;
		m_dBXCH2001		= dlg.m_dBX;
		m_dBYCH2001		=	dlg.m_dBY;
	  m_nNXCH2001		= dlg.m_nNX;
	  m_nNYCH2001		=	dlg.m_nNY;
		m_nMtdXCH2001 = dlg.m_nXMethod;
		m_nMtdYCH2001 = dlg.m_nYMethod;
	}		
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemChina2001::InitDefaultValue()
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
	m_nNXCH2001 = nStorNum - 1;		
	m_nNYCH2001 = nStorNum - 1;		
	m_dHXCH2001 = StorD.dStoryLevel - dBaseLevel;	
	m_dHYCH2001 = StorD.dStoryLevel - dBaseLevel;
	m_dBXCH2001	 = dBx;
	m_dBYCH2001  = dBy;
	m_nMtdXCH2001 = 0;
	m_nMtdYCH2001 = 0;
}

void CCMWindItemChina2001::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum ; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemChina2001::InitMaterialCombo()
{
	m_cobxMaterial.ResetContent();
	
	CString aItem[] = {_LS(IDS_WG_CMD__ADDD__Steel), _LS(IDS_CMD_STEEL_INFILLED_WALL), _LS(IDS_CMD_RC)};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum ; i++)
		m_cobxMaterial.AddString(aItem[i]);
	m_cobxMaterial.SetCurSel(0);
}

void CCMWindItemChina2001::CalcPulseMagnifyFactor()
{
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCalc, nMethod);
	if (nMethod != 0) return;

	T_WIND_CH2002 data;
	data.Initialize();
	data.nRoughCategory     = m_cobxCategory.GetCurSel() + 1;  
	data.dDampingRatio      = m_editDamping.GetEditValue();
	data.dBasicWindPressure = m_editSpeed.GetEditValue();
	data.dPeriod_X          = m_editPeriodx.GetEditValue();
	data.dPeriod_Y          = m_editPeriody.GetEditValue();
	
	if(data.dPeriod_X < 0 || data.dPeriod_Y < 0)
	{
		CString str;  
		CString strError = _LS(IDS_CMD_HSFC_error);  // Error
		CString strErrorR = strError + _T("! ");         // Error!	
		AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
		return;
	}
	CDBDoc::GetDocPoint()->m_pEditData->CalcWindChinaPulseMagnifyFactor(data);
	m_editMagFx.SetEditUnit(data.dPulseMagnifyFactorX);
	m_editMagFy.SetEditUnit(data.dPulseMagnifyFactorY);
}

void CCMWindItemChina2001::Data2Dlg()
{
	m_cobxCategory.SetCurSel(m_Data.nRoughCategory-1);

	m_editSpeed.SetEditUnit(m_Data.dBasicWindPressure);
	m_editTopoCoef.SetEditUnit(m_Data.dModifyFactor);
	m_editWindward.SetEditUnit(m_Data.dWindwardCoef);
	m_editLeeward.SetEditUnit(m_Data.dLeewardCoef);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCalc, m_Data.nPulseMagnifyMethod);
	OnCmdAutoUserRadio();
	m_cobxMaterial.SetCurSel(m_Data.nStructuralMat-1);
	OnSelchangeCmdMatlCombo();
	m_editDamping.SetEditUnit(m_Data.dDampingRatio);
	m_editPeriodx.SetEditUnit(m_Data.dPeriod_X);
	m_editPeriody.SetEditUnit(m_Data.dPeriod_Y);
	m_editMagFx.SetEditUnit(m_Data.dPulseMagnifyFactorX);
	m_editMagFy.SetEditUnit(m_Data.dPulseMagnifyFactorY);
	
	((CButton*)GetDlgItem(IDC_CMD_WIND_CFT_CHECK))->SetCheck(m_Data.bComfort);	
	OnCmdComfortCheck();
	
	m_editCheckWindPressure.SetEditUnit(m_Data.dCftWindPressure);
	m_editCheckDamping.SetEditUnit(m_Data.dCftDampingRatio);

}

BOOL CCMWindItemChina2001::Dlg2Data()
{
	m_Data.Initialize();

	m_Data.nRoughCategory = m_cobxCategory.GetCurSel();
	m_Data.nRoughCategory += 1;

	m_Data.dBasicWindPressure = m_editSpeed.GetEditValue();
	m_Data.dModifyFactor = m_editTopoCoef.GetEditValue();
	m_Data.dWindwardCoef = m_editWindward.GetEditValue();
	m_Data.dLeewardCoef = m_editLeeward.GetEditValue();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCalc, m_Data.nPulseMagnifyMethod);
	m_Data.nStructuralMat = m_cobxMaterial.GetCurSel();
	m_Data.nStructuralMat += 1;
	m_Data.dDampingRatio = m_editDamping.GetEditValue();
	m_Data.dPeriod_X = m_editPeriodx.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriody.GetEditValue();
	m_Data.dPulseMagnifyFactorX = m_editMagFx.GetEditValue();
	m_Data.dPulseMagnifyFactorY = m_editMagFy.GetEditValue();

	m_Data.dCftWindPressure = m_editCheckWindPressure.GetEditValue();
	m_Data.dCftDampingRatio = m_editCheckDamping.GetEditValue();

	return TRUE;
}

void CCMWindItemChina2001::OnCmdComfortCheck()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_CMD_WIND_CFT_CHECK))->GetCheck();	
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SPEED_CFT_EDIT2, nCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_DAMPING_CFT_EDIT, nCheck == 1);
}

LRESULT CCMWindItemChina2001::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}