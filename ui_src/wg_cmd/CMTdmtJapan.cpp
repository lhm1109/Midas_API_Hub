// CMTdmtJapan.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtJapan.h"

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
// CCMTdmtJapan dialog


CCMTdmtJapan::CCMTdmtJapan(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtJapan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtJapan)
	m_jpancemen = 0;
	m_jpanhumid = 1;
	m_nCalcMethod = -1;
	//}}AFX_DATA_INIT
}


void CCMTdmtJapan::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtJapan)
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_RELA, m_jpanrela);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_NOTA_UNIT, m_jpannotaunit);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_NOTA, m_jpannota);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_COMP_UNIT, m_jpancompunit);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_COMP, m_jpancomp);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_AGE_UNIT, m_jpanageunit);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_AGE, m_jpanage);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_LAMBDA, m_jpanLambda);
	DDX_Radio(pDX, IDC_CMD_HCRP_JAPAN_RAPID, m_jpancemen);
	DDX_Radio(pDX, IDC_CMD_HCRP_JAPAN_REL_RD1, m_jpanhumid);
	DDX_Radio(pDX, IDC_CMD_RADIO_JSCE, m_nCalcMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtJapan, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtJapan)
	ON_BN_CLICKED(IDC_CMD_HCRP_JAPAN_REL_RD1, OnCmdHcrpJapanRelRd1)
	ON_BN_CLICKED(IDC_CMD_HCRP_JAPAN_REL_RD2, OnCmdHcrpJapanRelRd2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtJapan message handlers
BOOL CCMTdmtJapan::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	ASSERT(m_pData != 0); // Please Set m_pData;

	SetUnitType();

	m_jpanrela.SetRange(40, 90);
	m_jpanrela.SetValue(70);

	m_jpanage.SetRange(0, 10000);
	m_jpanage.SetValue(3);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTdmtJapan::OnCmdHcrpJapanRelRd1() 
{
	EnableJapanRelaCtrl(FALSE);	
}

void CCMTdmtJapan::OnCmdHcrpJapanRelRd2() 
{
	EnableJapanRelaCtrl(TRUE);		
}

//--------------------------------------------------------------------
//
void CCMTdmtJapan::SetUnitType()
{
	//----------------------------------------------------------------
	// Japanese Standard
	//----------------------------------------------------------------
	m_jpanrela.SetValue(0);
	m_jpannotaunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_jpannota.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_jpancompunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_jpancomp.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_jpanageunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_jpanage.SetValue(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_jpanLambda.SetUnitType(0);
}

void CCMTdmtJapan::SetData2Dlg()
{ 
	//----------------------------------------------------------------
	// JAPAN
	//----------------------------------------------------------------
	m_jpancomp.SetEditUnit(m_pData->COMMON.dCompStrength);
	m_jpanrela.SetValue(m_pData->COMMON.dRelativeHumidity);
	m_jpanage.SetValue(m_pData->COMMON.dConcreteAge);
	m_jpannota.SetEditUnit(m_pData->JAPAN.dMemberSize);
	m_jpanLambda.SetEditUnit(m_pData->JAPAN.dLambda);
	m_jpancemen = m_pData->JAPAN.nCementType - 1;        // (JAPAN) 1:Rapid Hardening cement, 2:Normal cement
	m_jpanhumid = m_pData->JAPAN.nRelativeHumidity - 1;  // (JAPAN) 1:Curing Underwater, 2:Relative Humidity
	m_nCalcMethod = m_pData->JAPAN.nCalcMethodForE;
	EnableJapanRelaCtrl(m_jpanhumid);	

	UpdateData(FALSE);
}

void CCMTdmtJapan::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->COMMON.dCompStrength     = m_jpancomp.GetEditValue();
	m_pData->COMMON.dRelativeHumidity = m_jpanrela.GetEditValue();
	m_pData->COMMON.dConcreteAge      = m_jpanage.GetEditValue();
	m_pData->JAPAN.dMemberSize        = m_jpannota.GetEditValue();
	m_pData->JAPAN.dLambda            = m_jpanLambda.GetEditValue();
	m_pData->JAPAN.nCementType        = m_jpancemen + 1;    // (JAPAN) 1:Rapid Hardening cement, 2:Normal cement
	m_pData->JAPAN.nRelativeHumidity  = m_jpanhumid + 1;    // (JAPAN) 1:Curing Underwater, 2:Relative Humidity
	m_pData->JAPAN.nCalcMethodForE    = m_nCalcMethod;
}


void CCMTdmtJapan::EnableJapanRelaCtrl(BOOL bEnable)
{
	CWnd* pWnd[1];
	pWnd[0] = GetDlgItem(IDC_CMD_HCRP_JAPAN_RELA);      ASSERT(pWnd[0]);

	for(int i=0; i<1; i++) pWnd[i]->EnableWindow(bEnable);
}

