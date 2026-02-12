// CMTdmtEuropean.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtEuropean.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_base\wg_base_StrParser.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtEuropean dialog


CCMTdmtEuropean::CCMTdmtEuropean(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtEuropean::IDD, pParent)
{
	m_nCementType = 1;
	m_nTypeofCode = 1;
	//{{AFX_DATA_INIT(CCMTdmtEuropean)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aEuropeanRadioCtrl.Add(IDC_CMD_HCRP_EUROPEAN_CLASSS);
	m_aEuropeanRadioCtrl.Add(IDC_CMD_HCRP_EUROPEAN_CLASSN);
	m_aEuropeanRadioCtrl.Add(IDC_CMD_HCRP_EUROPEAN_CLASSR);

	m_aEuropeanTypeofCode.Add(IDC_CMD_HCRP_EUROPEAN_EN1992_1);
	m_aEuropeanTypeofCode.Add(IDC_CMD_HCRP_EUROPEAN_EN1992_2);
}

void CCMTdmtEuropean::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtEuropean)
	DDX_Control(pDX, IDC_CMD_HCRP_EUROPEAN_AGE_UNIT,  m_wndConcreteAgeUnit);
	DDX_Control(pDX, IDC_CMD_HCRP_EUROPEAN_NOTA_UNIT, m_wndNotationSizeUnit);
	DDX_Control(pDX, IDC_CMD_HCRP_EUROPEAN_COMP_UNIT, m_wndCompStrengthUnit);
	DDX_Control(pDX, IDC_CMD_HCRP_EUROPEAN_AGE,  m_wndConcreteAge);
	DDX_Control(pDX, IDC_CMD_HCRP_EUROPEAN_NOTA, m_wndNotationSize);
	DDX_Control(pDX, IDC_CMD_HCRP_EUROPEAN_RELA, m_wndRelativeHumidity);
	DDX_Control(pDX, IDC_CMD_HCRP_EUROPEAN_COMP, m_wndCompStrength);

	DDX_Control(pDX, IDC_CMD_HCRP_EUROPEAN_SILICA_FUME, m_bSilicaFume);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtEuropean, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtEuropean)
	ON_BN_CLICKED(IDC_CMD_HCRP_EUROPEAN_CLASSS, OnCmdHcrpEuropeanCementType)
	ON_BN_CLICKED(IDC_CMD_HCRP_EUROPEAN_CLASSN, OnCmdHcrpEuropeanCementType)
	ON_BN_CLICKED(IDC_CMD_HCRP_EUROPEAN_CLASSR, OnCmdHcrpEuropeanCementType)
	ON_BN_CLICKED(IDC_CMD_HCRP_EUROPEAN_EN1992_1, OnCmdHcrpEuropeanTypeofCode)
	ON_BN_CLICKED(IDC_CMD_HCRP_EUROPEAN_EN1992_2, OnCmdHcrpEuropeanTypeofCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtEuropean message handlers

void CCMTdmtEuropean::SetUnitType()
{  
	m_wndNotationSizeUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_wndNotationSize.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_wndCompStrengthUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_wndCompStrength.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_wndConcreteAgeUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
}

void CCMTdmtEuropean::SetData2Dlg()
{ 
	m_wndCompStrength.SetEditUnit(m_pData->COMMON.dCompStrength);
	m_wndRelativeHumidity.SetValue(m_pData->COMMON.dRelativeHumidity);
	m_wndConcreteAge.SetValue(m_pData->COMMON.dConcreteAge);
	m_wndNotationSize.SetEditUnit(m_pData->CEB.dMemberSize);
	m_nCementType = m_pData->CEB.nCementType-1;
	CDlgUtil::CtrlRadioSetCheck(this, m_aEuropeanRadioCtrl, m_nCementType);

	m_nTypeofCode = m_pData->CEB.nTypeofCode;
	CDlgUtil::CtrlRadioSetCheck(this, m_aEuropeanTypeofCode, m_nTypeofCode);

	m_bSilicaFume.SetCheck(m_pData->CEB.bSilicaFume);

	UpdateData(FALSE);
}

void CCMTdmtEuropean::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->COMMON.dCompStrength     = m_wndCompStrength.GetEditValue();
	m_pData->COMMON.dRelativeHumidity = m_wndRelativeHumidity.GetEditValue();
	m_pData->COMMON.dConcreteAge      = m_wndConcreteAge.GetEditValue();
	m_pData->CEB.dMemberSize          = m_wndNotationSize.GetEditValue();
	m_pData->CEB.nCementType          = m_nCementType+1;

	m_pData->CEB.nTypeofCode          = m_nTypeofCode;
	m_pData->CEB.bSilicaFume          = m_bSilicaFume.GetCheck();
}

BOOL CCMTdmtEuropean::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
		
	ASSERT(m_pData != 0); // Please Set m_pData;

	SetUnitType();

	m_wndRelativeHumidity.SetRange(40, 99);
	m_wndRelativeHumidity.SetValue(70);

	m_wndConcreteAge.SetRange(0, 10000);
	m_wndConcreteAge.SetValue(3);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTdmtEuropean::OnCmdHcrpEuropeanCementType()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aEuropeanRadioCtrl, m_nCementType);
}

void CCMTdmtEuropean::OnCmdHcrpEuropeanTypeofCode() 
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aEuropeanTypeofCode, m_nTypeofCode);

	if (m_nTypeofCode == 0)
	{
		GetDlgItem(IDC_CMD_HCRP_EUROPEAN_SILICA_FUME)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_CMD_HCRP_EUROPEAN_SILICA_FUME)->EnableWindow(TRUE);
	}
}
