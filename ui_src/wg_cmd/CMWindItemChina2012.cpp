// CMWindItemChina2012.cpp : implementation file
//
// 2003. 09. 23  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemChina2012.h"
#include "CMWindItemDlg.h"

#include "CMWindItemChina2012Tool.h"
#include "CMWindItemChina2012CrossDlg.h"
#include "CMWindItemChina2012PartDlg.h"
#include "CMWindItemChina2012ShieldDlg.h"
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
// CCMWindItemChina2012 dialog


CCMWindItemChina2012::CCMWindItemChina2012(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemChina2012::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemChina2012)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlStruct.Add(IDC_CMD_STRUCT_TAB_RD);
	m_aCtrlStruct.Add(IDC_CMD_STRUCT_CAL_RD);

	m_aCtrlAlongStructure.Add(IDC_CMD_WIND_ALONG_RADIO1);
	m_aCtrlAlongStructure.Add(IDC_CMD_WIND_ALONG_RADIO2);
	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
}

void CCMWindItemChina2012::SetData2Dlg(T_WIND_CH2012& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemChina2012::SetDlg2Data(T_WIND_CH2012& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemChina2012::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemChina2012)
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editBasicWindPressure);
	DDX_Control(pDX, IDC_CMD_TOPOCOEF_EDIT, m_editModifyFactor);
	DDX_Control(pDX, IDC_CMD_DAMPING_EDIT, m_editDamping);
	DDX_Control(pDX, IDC_CMD_WINDWARD_X_EDIT, m_editWindwardCoefX);
	DDX_Control(pDX, IDC_CMD_LEEWARD_X_EDIT, m_editLeewardCoefX);
	DDX_Control(pDX, IDC_CMD_WINDWARD_Y_EDIT, m_editWindwardCoefY);
	DDX_Control(pDX, IDC_CMD_LEEWARD_Y_EDIT, m_editLeewardCoefY);
	DDX_Control(pDX, IDC_CMD_PERIOD_Y_EDIT, m_editPeriody);
	DDX_Control(pDX, IDC_CMD_PERIOD_X_EDIT, m_editPeriodx);
	DDX_Control(pDX, IDC_CMD_SPEED_CFT_EDIT, m_editCheckWindPressure);
	DDX_Control(pDX, IDC_CMD_DAMPING_CFT_EDIT, m_editCheckDamping);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemChina2012, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemChina2012)
	ON_CBN_SELCHANGE(IDC_CMD_CATEGORY_COMBO, OnSelchangeCategoryCombo)
	
	ON_BN_CLICKED(IDC_CMD_WIND_PARTITION_BUTTON, OnCmdPartitionButton)
	ON_BN_CLICKED(IDC_CMD_WINDSHIELD_BUTTON, OnCmdShieldButton)
	ON_BN_CLICKED(IDC_CMD_PERIOD_BUTTON, OnCmdPeriodButton)
	ON_BN_CLICKED(IDC_CMD_WIND_CROSS_BUTTON, OnCmdCrossButton)

	ON_BN_CLICKED(IDC_CMD_WIND_PARTITION_CHECK, OnCmdPartitionCheck)
	ON_BN_CLICKED(IDC_CMD_WINDSHIELD_CHECK, OnCmdShieldCheck)
	ON_BN_CLICKED(IDC_CMD_WIND_CROSS_CHECK, OnCmdCrossCheck)
	ON_BN_CLICKED(IDC_CMD_WIND_CFT_CHECK, OnCmdComfortCheck)
	ON_BN_CLICKED(IDC_CMD_WIND_ALONG_CHECK, OnCmdWindAlongCheck)

	ON_EN_CHANGE(IDC_CMD_SPEED_EDIT, OnChangeCmdSpeedEdit)
	ON_EN_CHANGE(IDC_CMD_PERIOD_X_EDIT, OnChangeCmdPeriodXEdit)
	ON_EN_CHANGE(IDC_CMD_PERIOD_Y_EDIT, OnChangeCmdPeriodYEdit)
	ON_EN_CHANGE(IDC_CMD_DAMPING_EDIT, OnChangeCmdDampingEdit)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemChina2012 message handlers

BOOL CCMWindItemChina2012::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
		m_grpWindParam.InitControl(this, CCMWindItemChina2012::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_editBasicWindPressure.SetUnitType(D_UNITSYS_NONE);
	m_editModifyFactor.SetUnitType(D_UNITSYS_NONE);
	m_editDamping.SetUnitType(D_UNITSYS_NONE);
	m_editWindwardCoefX.SetUnitType(D_UNITSYS_NONE);
	m_editLeewardCoefX.SetUnitType(D_UNITSYS_NONE);
	m_editWindwardCoefY.SetUnitType(D_UNITSYS_NONE);
	m_editLeewardCoefY.SetUnitType(D_UNITSYS_NONE);
	m_editPeriody.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodx.SetUnitType(D_UNITSYS_NONE);
	m_editCheckWindPressure.SetUnitType(D_UNITSYS_NONE);
	m_editCheckDamping.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultValue();
		m_bInit = TRUE;
	}

	InitCategoryCombo();

	Data2Dlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindItemChina2012::DestroyWindow() 
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

void CCMWindItemChina2012::OnChangeCmdPeriodXEdit() 
{
	CalcPulseMagnifyFactor();		
}

void CCMWindItemChina2012::OnChangeCmdPeriodYEdit() 
{
	CalcPulseMagnifyFactor();	
}

void CCMWindItemChina2012::OnChangeCmdDampingEdit() 
{
	CalcPulseMagnifyFactor();		
}

void CCMWindItemChina2012::OnSelchangeCategoryCombo() 
{
	CalcPulseMagnifyFactor();		
}

void CCMWindItemChina2012::OnChangeCmdSpeedEdit() 
{
	CalcPulseMagnifyFactor();		
}

void CCMWindItemChina2012::OnCmdPeriodButton() 
{
	// TODO: Add your control notification handler code here	
	int nUnitChange = 0;   // 주기계산 대화상자에서 wind load시에는 단위변환하고, 
	                       //                       seismic load시에는 단위변환않음			
	CSeisChinaCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nUnitChange = nUnitChange;
	dlg.m_dHX			 = m_dHXCH2012;
	dlg.m_dHY			 = m_dHYCH2012;
	dlg.m_dBX			 = m_dBXCH2012;
	dlg.m_dBY			 = m_dBYCH2012;	
	dlg.m_nNX			 = m_nNXCH2012;
	dlg.m_nNY			 = m_nNYCH2012;	
	dlg.m_nXMethod = m_nMtdXCH2012;
	dlg.m_nYMethod = m_nMtdYCH2012;
	
	if(dlg.DoModal()== IDOK)
	{
		CString cx,cy;
		cx.Format(_T("%g"),dlg.m_dPeriod_X);
		cy.Format(_T("%g"),dlg.m_dPeriod_Y);
		m_editPeriodx.SetWindowText(cx);
		m_editPeriody.SetWindowText(cy);		

		m_dHXCH2012		= dlg.m_dHX;
		m_dHYCH2012		= dlg.m_dHY;
		m_dBXCH2012		= dlg.m_dBX;
		m_dBYCH2012		=	dlg.m_dBY;
	  m_nNXCH2012		= dlg.m_nNX;
	  m_nNYCH2012		=	dlg.m_nNY;
		m_nMtdXCH2012 = dlg.m_nXMethod;
		m_nMtdYCH2012 = dlg.m_nYMethod;
	}		
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCMWindItemChina2012::InitDefaultValue()
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
	m_nNXCH2012 = nStorNum - 1;		
	m_nNYCH2012 = nStorNum - 1;		
	m_dHXCH2012 = StorD.dStoryLevel - dBaseLevel;	
	m_dHYCH2012 = StorD.dStoryLevel - dBaseLevel;
	m_dBXCH2012	 = dBx;
	m_dBYCH2012  = dBy;
	m_nMtdXCH2012 = 0;
	m_nMtdYCH2012 = 0;
}

void CCMWindItemChina2012::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum ; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemChina2012::CalcPulseMagnifyFactor()
{
//  int nMethod;
//  CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCalc, nMethod);
//  if (nMethod != 0) return;

	T_WIND_CH2012 data;
	data.Initialize();
	data.nRoughCategory     = m_cobxCategory.GetCurSel() + 1;  
	data.dDampingRatio      = m_editDamping.GetEditValue();
	data.dBasicWindPressure = m_editBasicWindPressure.GetEditValue();
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
	CCMWindItemChina2012Tool::CalWind_PulseMagnifyFactor(data);
}

void CCMWindItemChina2012::Data2Dlg()
{
	m_cobxCategory.SetCurSel(m_Data.nRoughCategory-1);

	m_editBasicWindPressure.SetEditUnit(m_Data.dBasicWindPressure);
	m_editModifyFactor.SetEditUnit(m_Data.dModifyFactor);
	m_editDamping.SetEditUnit(m_Data.dDampingRatio);
 
	m_editWindwardCoefX.SetEditUnit(m_Data.dWindwardCoefX);
	m_editLeewardCoefX.SetEditUnit(m_Data.dLeewardCoefX);
	m_editWindwardCoefY.SetEditUnit(m_Data.dWindwardCoefY);
	m_editLeewardCoefY.SetEditUnit(m_Data.dLeewardCoefY);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlStruct, m_Data.nStructMethod);
//  CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCalc, m_Data.nPulseMagnifyMethod);
	m_editPeriodx.SetEditUnit(m_Data.dPeriod_X);
	m_editPeriody.SetEditUnit(m_Data.dPeriod_Y);

	((CButton*)GetDlgItem(IDC_CMD_WIND_PARTITION_CHECK))->SetCheck(m_Data.bPartitionCoef);	
	OnCmdPartitionCheck();
	((CButton*)GetDlgItem(IDC_CMD_WINDSHIELD_CHECK))->SetCheck(m_Data.bWindShield);	
	OnCmdShieldCheck();
	((CButton*)GetDlgItem(IDC_CMD_WIND_ALONG_CHECK))->SetCheck(m_Data.bAlongVibration);	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlAlongStructure, m_Data.nAlongType);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_ALONG_RADIO1, m_Data.bAlongVibration == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_ALONG_RADIO2, m_Data.bAlongVibration == 1);

	((CButton*)GetDlgItem(IDC_CMD_WIND_CROSS_CHECK))->SetCheck(m_Data.bCrossVibration);	
	OnCmdCrossCheck();
	((CButton*)GetDlgItem(IDC_CMD_WIND_CFT_CHECK))->SetCheck(m_Data.bWindComfort);	
	OnCmdComfortCheck();
	
	m_editCheckWindPressure.SetEditUnit(m_Data.dCheckWindPressure);
	m_editCheckDamping.SetEditUnit(m_Data.dCheckDampingRatio);
}

BOOL CCMWindItemChina2012::Dlg2Data()
{
//  m_Data.Initialize();
	m_Data.nRoughCategory = m_cobxCategory.GetCurSel();
	m_Data.nRoughCategory += 1;

	m_Data.dBasicWindPressure = m_editBasicWindPressure.GetEditValue();
	m_Data.dModifyFactor = m_editModifyFactor.GetEditValue();
	m_Data.dDampingRatio = m_editDamping.GetEditValue();
	m_Data.dWindwardCoefX = m_editWindwardCoefX.GetEditValue();
	m_Data.dLeewardCoefX  = m_editLeewardCoefX.GetEditValue();
	m_Data.dWindwardCoefY = m_editWindwardCoefY.GetEditValue();
	m_Data.dLeewardCoefY  = m_editLeewardCoefY.GetEditValue();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStruct, m_Data.nStructMethod);
//  CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCalc, m_Data.nPulseMagnifyMethod);
	m_Data.dPeriod_X = m_editPeriodx.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriody.GetEditValue();

	m_Data.bPartitionCoef = ((CButton*)GetDlgItem(IDC_CMD_WIND_PARTITION_CHECK))->GetCheck();	
	m_Data.bWindShield = ((CButton*)GetDlgItem(IDC_CMD_WINDSHIELD_CHECK))->GetCheck();	
	m_Data.bAlongVibration = ((CButton*)GetDlgItem(IDC_CMD_WIND_ALONG_CHECK))->GetCheck();	
	m_Data.bCrossVibration = ((CButton*)GetDlgItem(IDC_CMD_WIND_CROSS_CHECK))->GetCheck();	
	m_Data.bWindComfort = ((CButton*)GetDlgItem(IDC_CMD_WIND_CFT_CHECK))->GetCheck();	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAlongStructure, m_Data.nAlongType);

	m_Data.dCheckWindPressure = m_editCheckWindPressure.GetEditValue();
	m_Data.dCheckDampingRatio = m_editCheckDamping.GetEditValue();
	
	return TRUE;
}

void CCMWindItemChina2012::OnCmdPartitionButton()
{
	CMWindItemChina2012PartDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	if(dlg.DoModal()== IDOK)
	{
	}
}
void CCMWindItemChina2012::OnCmdShieldButton()
{
	CMWindItemChina2012ShieldDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	if(dlg.DoModal()== IDOK)
	{
	}
}
void CCMWindItemChina2012::OnCmdCrossButton()
{
	CMWindItemChina2012CrossDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetData(m_Data);	
	if(dlg.DoModal()== IDOK)
	{
		m_Data.bCross = dlg.m_nCrossCheck;
		m_Data.bTorsion = dlg.m_nTorsionCheck;
		m_Data.nSectType = dlg.m_nRoundOrRectRidio;
		m_Data.dCrossPeriod_X = dlg.m_dCrossPeriodX;
		m_Data.dCrossPeriod_Y = dlg.m_dCrossPeriodY;
		m_Data.dTorsionPeriod = dlg.m_dPeriodTorsion;
	}		
}
void CCMWindItemChina2012::OnCmdPartitionCheck()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_CMD_WIND_PARTITION_CHECK))->GetCheck();	
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_PARTITION_BUTTON, nCheck == 1);
}
void CCMWindItemChina2012::OnCmdShieldCheck()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_CMD_WINDSHIELD_CHECK))->GetCheck();	
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WINDSHIELD_BUTTON, nCheck == 1);
}
void CCMWindItemChina2012::OnCmdCrossCheck()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_CMD_WIND_CROSS_CHECK))->GetCheck();	
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_CROSS_BUTTON, nCheck == 1);
}
void CCMWindItemChina2012::OnCmdComfortCheck()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_CMD_WIND_CFT_CHECK))->GetCheck();	
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SPEED_CFT_EDIT, nCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_DAMPING_CFT_EDIT, nCheck == 1);
}

LRESULT CCMWindItemChina2012::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}

void CCMWindItemChina2012::OnCmdWindAlongCheck()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_CMD_WIND_ALONG_CHECK))->GetCheck();
	m_Data.bAlongVibration = nCheck;
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_ALONG_RADIO1, nCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_ALONG_RADIO2, nCheck == 1);
}
