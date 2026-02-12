// CMTdmtIndia.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtIndia.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtIndia dialog


CCMTdmtIndia::CCMTdmtIndia(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtIndia::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtIndia)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMTdmtIndia::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtIndia)
	// INDIA(IRC:18-2000)
	DDX_Control(pDX, IDC_CMD_HCRP_INDIA_COMP, m_INDIAComp);
	DDX_Control(pDX, IDC_CMD_HCRP_INDIA_RELA, m_INDIARela);
	DDX_Control(pDX, IDC_CMD_HCRP_INDIA_NOTA, m_INDIANota);
	DDX_Control(pDX, IDC_CMD_HCRP_INDIA_AGE,  m_INDIAAge);
	DDX_Control(pDX, IDC_CMD_HCRP_INDIA_COMP_UNIT1, m_INDIACompUnit);
	DDX_Control(pDX, IDC_CMD_HCRP_INDIA_NOTA_UNIT3, m_INDIANotaUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtIndia, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtIndia)


	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtIndia message handlers

BOOL CCMTdmtIndia::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	// TODO: Add extra initialization here
	ASSERT(m_pData != 0); // Please Set m_pData;

	SetUnitType();
	
	m_INDIARela.SetRange(40, 99);
	m_INDIARela.SetValue(70);

	m_INDIAAge.SetRange(1, 10000);
	m_INDIAAge.SetValue(3);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//--------------------------------------------------------------------
//
void CCMTdmtIndia::SetUnitType()
{
	//----------------------------------------------------------------
	// Japanese Standard
	//----------------------------------------------------------------
	m_INDIACompUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_INDIANotaUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
}

void CCMTdmtIndia::SetData2Dlg()
{ 
	m_INDIAComp.SetEditUnit(m_pData->COMMON.dCompStrength);
	m_INDIARela.SetValue(m_pData->COMMON.dRelativeHumidity);
	m_INDIANota.SetEditUnit(m_pData->CEB.dMemberSize);
	m_INDIAAge.SetValue(m_pData->COMMON.dConcreteAge);

	UpdateData(FALSE);
}

void CCMTdmtIndia::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->COMMON.dCompStrength     = m_INDIAComp.GetEditValue();
	m_pData->COMMON.dRelativeHumidity = m_INDIARela.GetEditValue();
	m_pData->CEB.dMemberSize          = m_INDIANota.GetEditValue();
	m_pData->COMMON.dConcreteAge      = m_INDIAAge.GetEditValue();
}
