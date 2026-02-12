#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonSetLongTermStressLossDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CChildDialogMove 

CCMTendonSetLongTermStressLossDlg::CCMTendonSetLongTermStressLossDlg(CWnd* pParent)
	: CDialog(CCMTendonSetLongTermStressLossDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_LtslData.Initialize();

//	m_nLongTermLossType = 0;
	m_nStrandType = 0;

//	m_dEstForce = 0.0;
	m_dConcAge = 15.0;
	m_dConcElasticity = 0.0;
	m_dRelativeHumidity = 0.0;
	m_dVSRatio = 0.0;
	m_dAvgPreComp = 0.0;
	m_dUltCreepCoef = 2.0;
	m_dTensFactor = 0.5;
	m_dShrinkageConst = 0.0;
	m_dRelaxConst = 0.0;
	m_dRelaxReduct = 0.0;

	m_bAuto = FALSE;
}

void CCMTendonSetLongTermStressLossDlg::SetData(T_TDNT_LTSL& Data)
{
	m_LtslData = Data;
}

void CCMTendonSetLongTermStressLossDlg::GetData(T_TDNT_LTSL& Data)
{
	Data.Initialize();
	Data = m_LtslData;
}

void CCMTendonSetLongTermStressLossDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//DDX_Radio(pDX, IDC_CMD_TENDON_SLTSL_ESTIMATE_RDO, m_nLongTermLossType);
	DDX_Radio(pDX, IDC_CMD_TENDON_SLTSL_LOWLAX_RDO, m_nStrandType);

	//DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_ESTIMATE_FORCE_EDT, m_dEstForce);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_AGE_EDT, m_dConcAge);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_CONC_ELAST_EDT, m_dConcElasticity);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_RELATIVE_HUMIDITY_EDT, m_dRelativeHumidity);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_VS_RATIO_EDT, m_dVSRatio);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_AVG_PRECOMP_EDT, m_dAvgPreComp);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_ULT_CREEP_COEF_EDT, m_dUltCreepCoef);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_TENS_FACT_EDT, m_dTensFactor);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_SHRNK_CONST_EDT, m_dShrinkageConst);

	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_RELAX_CONST_EDT, m_dRelaxConst);
	DDX_Text(pDX, IDC_CMD_TENDON_SLTSL_RELAX_REDUCT_EDT, m_dRelaxReduct);

//	DDX_Control(pDX, IDC_CMD_TENDON_SLTSL_ESTIMATE_UNIT_STC, m_unitEstForce);
	DDX_Control(pDX, IDC_CMD_TENDON_SLTSL_CONC_ELAST_UNIT_STC, m_unitConcElasticity);
	DDX_Control(pDX, IDC_CMD_TENDON_SLTSL_VS_RATIO_UNIT_STC, m_unitVS);
	DDX_Control(pDX, IDC_CMD_TENDON_SLTSL_AVG_PRECOMP_UNIT_STC, m_unitAvgPreComp);
	DDX_Control(pDX, IDC_CMD_TENDON_SLTSL_RELAX_CONST_UNT, m_unitRelaxConst);
	

	DDX_Check(pDX, IDC_CMD_TENDON_SLTSL_AVG_PRECOMP_CHK, m_bAuto);
}

void CCMTendonSetLongTermStressLossDlg::SetControl()
{
	SetUnitType();
}

void CCMTendonSetLongTermStressLossDlg::SetUnitType()
{
//	m_unitEstForce.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_unitConcElasticity.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_unitVS.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitAvgPreComp.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_unitRelaxConst.SetUnitType(D_UNITSYS_BASE_STRESS);
}

void CCMTendonSetLongTermStressLossDlg::Data2Dlg()
{
//	m_nLongTermLossType = m_LtslData.nLongTermLossType;
	m_nStrandType = m_LtslData.nStrandType;
//	m_dEstForce = m_LtslData.dEstForce;
	m_dConcAge = m_LtslData.dConcAge;
	m_dConcElasticity = m_LtslData.dConcElasticity;
	m_dRelativeHumidity = m_LtslData.dRelativeHumidity;
	m_dVSRatio = m_LtslData.dVSRatio;
	m_dAvgPreComp = m_LtslData.dAvgPreComp;
	m_dUltCreepCoef = m_LtslData.dUltCreepCoef;
	m_dTensFactor = m_LtslData.dTensFactor;
	m_dShrinkageConst = m_LtslData.dShrinkageConst;
	m_dRelaxConst     = m_LtslData.dRelaxConst;
	m_dRelaxReduct    = m_LtslData.dRelaxReduct;
	m_bAuto = m_LtslData.bAuto;

	SetUnitType();

	UpdateData(FALSE);
}

bool CCMTendonSetLongTermStressLossDlg::Dlg2Data()
{
	if (!UpdateData(TRUE)) return false;

//	m_LtslData.nLongTermLossType = m_nLongTermLossType;
	m_LtslData.nStrandType = m_nStrandType;
//	m_LtslData.dEstForce = m_dEstForce;
	m_LtslData.dConcAge = m_dConcAge;
	m_LtslData.dConcElasticity = m_dConcElasticity;
	m_LtslData.dRelativeHumidity = m_dRelativeHumidity;
	m_LtslData.dVSRatio = m_dVSRatio;
	m_LtslData.dAvgPreComp = m_dAvgPreComp;
	m_LtslData.dUltCreepCoef = m_dUltCreepCoef;
	m_LtslData.dTensFactor = m_dTensFactor;
	m_LtslData.dShrinkageConst = m_dShrinkageConst;
 	m_LtslData.dRelaxConst     = m_dRelaxConst;
	m_LtslData.dRelaxReduct    = m_dRelaxReduct;

	m_LtslData.bAuto = m_bAuto;

	return true;
}

BOOL CCMTendonSetLongTermStressLossDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetControl();

	Data2Dlg();

	return TRUE;
}

void CCMTendonSetLongTermStressLossDlg::OnOK()
{
	if (Dlg2Data())
	{
		CDialog::OnOK();
	}
	else
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_TDNT_ITEM_Parameter_Error__));
	}

}

void CCMTendonSetLongTermStressLossDlg::OnApply()
{
	OnOK();
}

void CCMTendonSetLongTermStressLossDlg::OnStrandType()
{
	UpdateData(TRUE);

	if (m_nStrandType == 0)
	{
		m_dRelaxConst  = M_InitValueCurUnit(30000.0, KN, M, D_UNITSYS_BASE_STRESS);
		m_dRelaxReduct = 0.35;
	}
	else if (m_nStrandType == 1)
	{
		m_dRelaxConst  = M_InitValueCurUnit(120000.0, KN, M, D_UNITSYS_BASE_STRESS);
		m_dRelaxReduct = 0.13;
	}
	else
		ASSERT(0);

	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CCMTendonSetLongTermStressLossDlg, CDialog)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(IDC_CMD_TENDON_SLTSL_LOWLAX_RDO,        OnStrandType)
	ON_BN_CLICKED(IDC_CMD_TENDON_SLTSL_STRESSRELIVED_RDO, OnStrandType)
	
END_MESSAGE_MAP()