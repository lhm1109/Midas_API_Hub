// CMWindItemKBC2009.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureNodalDlg_CH2012.h"
#include "StldWindPressureAreaDlg_CH_Period.h"
#include "StldWindPressureAreaDlg_CH2012_CrossTorsion.h"

#include "..\wg_base\wg_base_DlgUtil.h"
// #include "..\wg_base\wg_base_MsgDll.h"
// 

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_CH2012 dialog

CStldWindPressureNodalDlg_CH2012::CStldWindPressureNodalDlg_CH2012(CWnd* pParent /*=NULL*/)
: CCMWindPressureBaseChildDlg(CStldWindPressureNodalDlg_CH2012::IDD, pParent)
{
	m_pData = NULL;
	m_nPeriodType = 1;
}

void CStldWindPressureNodalDlg_CH2012::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_DAMPING_EDIT,                  m_edtDampingRatio);
	DDX_Control(pDX, IDC_TM_FUND_PERIOD,                   m_edtBasePeriod);
	DDX_Control(pDX, IDC_TM_FUND_PERIOD2,                  m_edtBasePeriod2);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT,              m_edtWindwardCf);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT3,             m_edtLeewardCf);
	DDX_Control(pDX, IDC_TM_WIND_CONSIDER_PARTITION_EDIT,  m_edtPartitionPresCf);
	DDX_Control(pDX, IDC_TM_WIND_CONSIDER_WINDSHILED_EDIT, m_edtWindshieldCf);
//   DDX_Control(pDX, IDC_TM_SPEED_CFT_EDIT,                m_edtCftWinPressure);
//   DDX_Control(pDX, IDC_TM_WIND_Damping_EDT,             m_edtCftDampingRatio);
}


BEGIN_MESSAGE_MAP(CStldWindPressureNodalDlg_CH2012, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_WIND_CONSIDER_PARTITION,  OnChkConsiderPartition)
	ON_BN_CLICKED(IDC_TM_WIND_CONSIDER_WINDSHIELD, OnChkConsiderWindShield)
	ON_BN_CLICKED(IDC_TM_WIND_CROSS_CHECK,         OnChkConsiderCrossWind)
	ON_BN_CLICKED(IDC_TM_PERIOD_BTN,               OnBtnPeriod)
	ON_BN_CLICKED(IDC_TM_PERIOD_BTN2,               OnBtnPeriod2)
	ON_BN_CLICKED(IDC_TM_WIND_CROSS_BUTTON,        OnBtnCrossWind)  
	ON_BN_CLICKED(IDC_TM_WIND_ALONG_CHECK,			OnCmdWindAlongCheck)
	//ON_BN_CLICKED(IDC_TM_WIND_CFT_CHECK,           OnChkConsiderComfort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_CH2012 message handlers

BOOL CStldWindPressureNodalDlg_CH2012::OnInitDialog() 
{
	CCMWindPressureBaseChildDlg::OnInitDialog();
	
	InitControl();

	if(m_pData)
		Data2Dlg( (void*)m_pData);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CStldWindPressureNodalDlg_CH2012::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data((void*)m_pData);
	
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}

BOOL CStldWindPressureNodalDlg_CH2012::Dlg2Data( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;
	
	if(((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->GetCheck())
		m_pData->CodeParam.CH2012.nStructMethod = 2;
	else
		m_pData->CodeParam.CH2012.nStructMethod = 1;

	m_pData->CodeParam.CH2012.dDampingRatio = m_edtDampingRatio.GetEditValue();
	m_pData->CodeParam.CH2012.dBasicPeriod = m_edtBasePeriod.GetEditValue();
	m_pData->CodeParam.CH2012.dBasicPeriod_Cross = m_edtBasePeriod2.GetEditValue();
	if(bWarning, m_pData->CodeParam.CH2012.dBasicPeriod <= 1E-5 || m_pData->CodeParam.CH2012.dBasicPeriod_Cross <= 1E-5)
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_PERIOD));
		return FALSE;
	}
	m_pData->CodeParam.CH2012.dWindwardCoef = m_edtWindwardCf.GetEditValue();
	m_pData->CodeParam.CH2012.dLeewardCoef = m_edtLeewardCf.GetEditValue();
	
	if(((CButton*)GetDlgItem(IDC_TM_WIND_CONSIDER_PARTITION))->GetCheck())
	{
		m_pData->CodeParam.CH2012.bConsiderPartition = TRUE;
		m_pData->CodeParam.CH2012.dPartitionCoef = m_edtPartitionPresCf.GetEditValue();
	}
	else
		m_pData->CodeParam.CH2012.bConsiderPartition = FALSE;
	
	if(((CButton*)GetDlgItem(IDC_TM_WIND_CONSIDER_WINDSHIELD))->GetCheck())
	{
		m_pData->CodeParam.CH2012.bConsiderWindShield = TRUE;
		m_pData->CodeParam.CH2012.dWindShieldCoef = m_edtWindshieldCf.GetEditValue();
	}
	else
		m_pData->CodeParam.CH2012.bConsiderWindShield = FALSE;

	if(((CButton*)GetDlgItem(IDC_TM_WIND_ALONG_CHECK))->GetCheck())
	{
		m_pData->CodeParam.CH2012.bAlongVibration = TRUE;
		m_pData->CodeParam.CH2012.nAlongType = ((CButton*)GetDlgItem(IDC_TM_WIND_ALONG_RADIO1))->GetCheck() ? 0 : 1;
	}
	else
		m_pData->CodeParam.CH2012.bAlongVibration = FALSE;
	
	if(((CButton*)GetDlgItem(IDC_TM_WIND_CROSS_CHECK))->GetCheck())
		m_pData->CodeParam.CH2012.bCrossVibration = TRUE;
	else
		m_pData->CodeParam.CH2012.bCrossVibration = FALSE;
	
//   if(((CButton*)GetDlgItem(IDC_TM_WIND_CFT_CHECK))->GetCheck())
//   {
//     m_pData->CH2012.bWindComfort = TRUE;
//     m_pData->CH2012.dCftWindPressure = m_edtCftWinPressure.GetEditValue();
//     m_pData->CH2012.dCftDampingRatio = m_edtCftDampingRatio.GetEditValue();
//   }
//   else
//     m_pData->CH2012.bWindComfort = FALSE;

	return TRUE; 
}

void CStldWindPressureNodalDlg_CH2012::Data2Dlg( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;

	if(m_pData->CodeParam.CH2012.nStructMethod == 2)
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(FALSE);
	}
	m_edtDampingRatio.SetEditUnit(m_pData->CodeParam.CH2012.dDampingRatio);
	m_edtBasePeriod.SetEditUnit(m_pData->CodeParam.CH2012.dBasicPeriod);
	m_edtBasePeriod2.SetEditUnit(m_pData->CodeParam.CH2012.dBasicPeriod_Cross);
	m_edtWindwardCf.SetEditUnit(m_pData->CodeParam.CH2012.dWindwardCoef);
	m_edtLeewardCf.SetEditUnit(m_pData->CodeParam.CH2012.dLeewardCoef);

	if(m_pData->CodeParam.CH2012.bConsiderPartition)
		((CButton*)GetDlgItem(IDC_TM_WIND_CONSIDER_PARTITION))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_TM_WIND_CONSIDER_PARTITION))->SetCheck(FALSE);
	OnChkConsiderPartition();
	m_edtPartitionPresCf.SetEditUnit(m_pData->CodeParam.CH2012.dPartitionCoef);
	
	if(m_pData->CodeParam.CH2012.bConsiderWindShield)
		((CButton*)GetDlgItem(IDC_TM_WIND_CONSIDER_WINDSHIELD))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_TM_WIND_CONSIDER_WINDSHIELD))->SetCheck(FALSE);
	OnChkConsiderWindShield();
	m_edtWindshieldCf.SetEditUnit(m_pData->CodeParam.CH2012.dWindShieldCoef);
	
	if(m_pData->CodeParam.CH2012.bAlongVibration)
	{
		((CButton*)GetDlgItem(IDC_TM_WIND_ALONG_CHECK))->SetCheck(TRUE);
		GetDlgItem(IDC_TM_WIND_ALONG_RADIO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_WIND_ALONG_RADIO2)->EnableWindow(TRUE);
		if (m_pData->CodeParam.CH2012.nAlongType == 0)
			((CButton*)GetDlgItem(IDC_TM_WIND_ALONG_RADIO1))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_TM_WIND_ALONG_RADIO2))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_TM_WIND_ALONG_CHECK))->SetCheck(FALSE);
		GetDlgItem(IDC_TM_WIND_ALONG_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_WIND_ALONG_RADIO2)->EnableWindow(FALSE);
	}

	if(m_pData->CodeParam.CH2012.bCrossVibration)
		((CButton*)GetDlgItem(IDC_TM_WIND_CROSS_CHECK))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_TM_WIND_CROSS_CHECK))->SetCheck(FALSE);
	OnChkConsiderCrossWind();
	
//   if(m_pData->CH2012.bWindComfort)
//     ((CButton*)GetDlgItem(IDC_TM_WIND_CFT_CHECK))->SetCheck(TRUE);
//   else
//     ((CButton*)GetDlgItem(IDC_TM_WIND_CFT_CHECK))->SetCheck(FALSE);
//   m_edtCftWinPressure.SetEditUnit(m_pData->CH2012.dCftWindPressure);
//   m_edtCftDampingRatio.SetEditUnit(m_pData->CH2012.dCftDampingRatio);
//   OnChkConsiderComfort();
}

int CStldWindPressureNodalDlg_CH2012::GetType()
{
	int nType = 1;
	if(((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->GetCheck())
		nType = 1;
	else if(((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->GetCheck())
		nType = 2;
	else 
	{
		nType = 1;
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(FALSE);
	}
	return nType;  
}

void CStldWindPressureNodalDlg_CH2012::InitControl()
{
	m_edtDampingRatio.SetUnitType(D_UNITSYS_NONE);
	m_edtBasePeriod.SetUnitType(D_UNITSYS_NONE);
	m_edtBasePeriod2.SetUnitType(D_UNITSYS_NONE);
	m_edtWindwardCf.SetUnitType(D_UNITSYS_NONE);
	m_edtLeewardCf.SetUnitType(D_UNITSYS_NONE);
	m_edtPartitionPresCf.SetUnitType(D_UNITSYS_NONE);
	m_edtWindshieldCf.SetUnitType(D_UNITSYS_NONE);
//   m_edtCftWinPressure.SetUnitType(D_UNITSYS_BASE_PRESSURE);
//   m_edtCftDampingRatio.SetUnitType(D_UNITSYS_NONE);
}

void CStldWindPressureNodalDlg_CH2012::OnChkConsiderPartition()
{
	BOOL bEnable;
	if(((CButton*)GetDlgItem(IDC_TM_WIND_CONSIDER_PARTITION))->GetCheck())
		bEnable = TRUE;
	else
		bEnable = FALSE;
	
	GetDlgItem(IDC_TM_WIND_CONSIDER_PARTITION_EDIT)->EnableWindow(bEnable);
}

void CStldWindPressureNodalDlg_CH2012::OnChkConsiderWindShield()
{
	BOOL bEnable;
	if(((CButton*)GetDlgItem(IDC_TM_WIND_CONSIDER_WINDSHIELD))->GetCheck())
		bEnable = TRUE;
	else
		bEnable = FALSE;
	
	GetDlgItem(IDC_TM_WIND_CONSIDER_WINDSHILED_EDIT)->EnableWindow(bEnable);
}

void CStldWindPressureNodalDlg_CH2012::OnChkConsiderCrossWind()
{
	BOOL bEnable;
	if(((CButton*)GetDlgItem(IDC_TM_WIND_CROSS_CHECK))->GetCheck())
		bEnable = TRUE;
	else
		bEnable = FALSE;
	
	GetDlgItem(IDC_TM_WIND_CROSS_BUTTON)->EnableWindow(bEnable);
}

// void CStldWindPressureNodalDlg_CH2012::OnChkConsiderComfort()
// {
//   BOOL bEnable;
//   if(((CButton*)GetDlgItem(IDC_TM_WIND_CFT_CHECK))->GetCheck())
//     bEnable = TRUE;
//   else
//     bEnable = FALSE;
//   
//   GetDlgItem(IDC_TM_SPEED_CFT_EDIT)->EnableWindow(bEnable);
//   GetDlgItem(IDC_TM_WIND_Damping_EDT)->EnableWindow(bEnable);  
// }

void CStldWindPressureNodalDlg_CH2012::OnBtnPeriod()
{
	int nStore;
	double dHeight, dWidth;  
	if(!CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(m_pData, dHeight, nStore, dWidth))
	{
	  dHeight = 0.0;
	  nStore = 0;
	  dWidth = 0.0;
	}

	//m단위계로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	dHeight = pDoc->m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dHeight);
	dWidth = pDoc->m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dWidth);

	CStldWindPressureAreaDlg_CH_Period dlg;
	dlg.SetDlgType(m_nPeriodType);
	dlg.SetInit(dWidth, dHeight, nStore);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetDlgPeriod(m_dPeriod);
		m_nPeriodType = dlg.GetDlgType();
		m_edtBasePeriod.SetEditUnit(m_dPeriod);
	}
}

void CStldWindPressureNodalDlg_CH2012::OnBtnCrossWind()
{
	CStldWindPressureAreaDlg_CH2012_CrossTorsion dlg;
	dlg.SetData(m_pData->CodeParam.CH2012);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_pData->CodeParam.CH2012);
	}
}

void CStldWindPressureNodalDlg_CH2012::OnBtnPeriod2()
{
	int nStore;
	double dHeight, dWidth;  
	T_WDPR_D WdprD = *m_pData;
	if(WdprD.nDirection == 1)
	{
		WdprD.dAngle += 90;
	}
	if(!CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(m_pData, dHeight, nStore, dWidth))
	{
	  dHeight = 0.0;
	  nStore = 0;
	  dWidth = 0.0;
	}

	//m단위계로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	dHeight = pDoc->m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dHeight);
	dWidth = pDoc->m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dWidth);
	
	CStldWindPressureAreaDlg_CH_Period dlg;
	dlg.SetDlgType(m_nPeriodType);
	dlg.SetInit(dWidth, dHeight, nStore);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetDlgPeriod(m_dPeriod2);
		m_nPeriodType = dlg.GetDlgType();
		m_edtBasePeriod2.SetEditUnit(m_dPeriod2);
	}
}
void CStldWindPressureNodalDlg_CH2012::OnCmdWindAlongCheck()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_TM_WIND_ALONG_CHECK))->GetCheck();
	m_pData->CodeParam.CH2012.bAlongVibration = nCheck;
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_WIND_ALONG_RADIO1, nCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_WIND_ALONG_RADIO2, nCheck == 1);
}