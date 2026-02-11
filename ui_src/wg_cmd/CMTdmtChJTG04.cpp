// CMTdmtChJTG04.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtChJTG04.h"

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
// CCMTdmtChJTG04 dialog


CCMTdmtChJTG04::CCMTdmtChJTG04(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtChJTG04::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtChJTG04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMTdmtChJTG04::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtChJTG04)
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_CEMENTTYPE, m_wndCementTypeCoef);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_AGE_UNIT, m_wndConcreteAgeUnit);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_NOTA_UNIT, m_wndNotationSizeUnit);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_COMP_UNIT, m_wndCompStrengthUnit);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_AGE, m_wndConcreteAge);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_NOTA, m_wndNotationSize);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_RELA, m_wndRelativeHumidity);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_COMP, m_wndCompStrength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtChJTG04, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtChJTG04)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtChJTG04 message handlers

void CCMTdmtChJTG04::SetUnitType()
{  
	m_wndNotationSizeUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_wndNotationSize.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_wndCompStrengthUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_wndCompStrength.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_wndConcreteAgeUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
}

void CCMTdmtChJTG04::SetData2Dlg()
{ 
	m_wndCompStrength.SetEditUnit(m_pData->CHJTG04.dCompStrength);
	m_wndRelativeHumidity.SetValue(m_pData->CHJTG04.dRelativeHumidity);
	m_wndConcreteAge.SetValue(m_pData->CHJTG04.dConcreteAge);
	m_wndNotationSize.SetEditUnit(m_pData->CHJTG04.dMemberSize);
	m_wndCementTypeCoef.SetEditUnit(m_pData->CHJTG04.dCementTypeCoef);	
	UpdateData(FALSE);
}

void CCMTdmtChJTG04::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->CHJTG04.dCompStrength     = m_wndCompStrength.GetEditValue();
	m_pData->CHJTG04.dRelativeHumidity = m_wndRelativeHumidity.GetEditValue();
	m_pData->CHJTG04.dConcreteAge      = m_wndConcreteAge.GetEditValue();
	m_pData->CHJTG04.dMemberSize       = m_wndNotationSize.GetEditValue();
	m_pData->CHJTG04.dCementTypeCoef   = m_wndCementTypeCoef.GetEditValue();
	
}

BOOL CCMTdmtChJTG04::OnInitDialog() 
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
