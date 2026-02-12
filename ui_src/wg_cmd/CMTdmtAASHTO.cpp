// CMTdmtAASHTO.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtAASHTO.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAASHTO dialog


CCMTdmtAASHTO::CCMTdmtAASHTO(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtAASHTO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtAASHTO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMTdmtAASHTO::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtAASHTO)
	DDX_Control(pDX, IDC_CMD_EXPOSE_CHK, m_chkExpose);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_RELA, m_AASHTOrela);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_COMP_UNIT, m_AASHTOcompunit);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_COMP, m_AASHTOcomp);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_AGE_UNIT, m_AASHTOageunit);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_AGE, m_AASHTOage);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_VOLU, m_edtVolumeSurfaceRatio);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_VOLU_UNIT, m_untVolumeSurfaceRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtAASHTO, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtAASHTO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAASHTO message handlers

BOOL CCMTdmtAASHTO::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	// TODO: Add extra initialization here
	ASSERT(m_pData != 0); // Please Set m_pData;

	SetUnitType();

	m_AASHTOrela.SetRange(30, 998);
	m_AASHTOrela.SetValue(70);

	m_AASHTOage.SetRange(0, 10000);
	m_AASHTOage.SetValue(3);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//--------------------------------------------------------------------
//
void CCMTdmtAASHTO::SetUnitType()
{
	//----------------------------------------------------------------
	// Japanese Standard
	//----------------------------------------------------------------
	m_AASHTOcompunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_AASHTOcomp.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_AASHTOageunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_edtVolumeSurfaceRatio.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_untVolumeSurfaceRatio.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
}

void CCMTdmtAASHTO::SetData2Dlg()
{ 
	m_AASHTOcomp.SetEditUnit(m_pData->COMMON.dCompStrength);
	m_AASHTOrela.SetValue(m_pData->COMMON.dRelativeHumidity);
	m_AASHTOage.SetValue(m_pData->COMMON.dConcreteAge);
	m_edtVolumeSurfaceRatio.SetEditUnit(m_pData->AASHTO.dVolumeSurfaceRatio);
	m_chkExpose.SetCheck(m_pData->AASHTO.bExpose ? 1 : 0);

	UpdateData(FALSE);
}

void CCMTdmtAASHTO::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->COMMON.dCompStrength     = m_AASHTOcomp.GetEditValue();
	m_pData->COMMON.dRelativeHumidity = m_AASHTOrela.GetEditValue();
	m_pData->COMMON.dConcreteAge      = m_AASHTOage.GetEditValue();
	m_pData->AASHTO.dVolumeSurfaceRatio = m_edtVolumeSurfaceRatio.GetEditValue();
	m_pData->AASHTO.bExpose           = m_chkExpose.GetCheck() != 0 ? TRUE : FALSE;
}
