// CMTdmtRUS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtRUS.h"

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
// CCMTdmtRUS dialog


CCMTdmtRUS::CCMTdmtRUS(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtRUS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtRUS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nCuringMeth = 0;
	m_nCementType = 0;
	m_bCreep = FALSE;
	m_nConcType = 0;
}

void CCMTdmtRUS::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtRUS)
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_CONC_B_EDT,     m_edtConcB);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_CONC_B_UNT,     m_untConcB);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_RELA_EDT,       m_edtRelaHumi);
	//DDX_Control(pDX, IDC_CMD_HCRP_RUS_RELA_UNT,       m_untRelaHumi);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_M_EDT,          m_edtM);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_M_UNT,          m_untM);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_AGE_EDT,        m_edtAge);
	//DDX_Control(pDX, IDC_CMD_HCRP_RUS_AGE_UNT,        m_untAge);
	DDX_Radio  (pDX, IDC_CMD_HCRP_RUS_CMETH_RDO1,     m_nCuringMeth);
	DDX_Radio  (pDX, IDC_CMD_HCRP_RUS_CEMENT_RDO1,    m_nCementType);
	DDX_Check  (pDX, IDC_CMD_HCRP_RUS_CREEP_CHK,      m_bCreep);
	DDX_Radio  (pDX, IDC_CMD_HCRP_RUS_CONC_TYPE_RDO1, m_nConcType);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_WATER_EDT,      m_edtWater);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_WATER_UNT,      m_untWater);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_MAX_SIZE_EDT,   m_edtMaxSize);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_MAX_SIZE_UNT,   m_untMaxSize);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_AIR_EDT,        m_edtAir);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_AIR_UNT,        m_untAir);
	DDX_Control(pDX, IDC_CMD_HCRP_RUS_PZ_EDT,         m_edtPz);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtRUS, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtRUS)
 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_RUS_RELA_SPN, OnCmdHcrpRUSRelaSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_RUS_AGE_SPN,  OnCmdHcrpRUSAgeSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtRUS message handlers

void CCMTdmtRUS::InitUnit()
{  
	m_edtConcB.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_untConcB.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_edtM.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dExposeSurfM);
	m_untM.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dExposeSurfM);
	m_edtWater.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dWater);
	m_untWater.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dWater);
	m_edtMaxSize.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMaxSize);
	m_untMaxSize.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMaxSize);
	m_edtAir.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dAir);
	m_untAir.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dAir);
	m_edtPz.SetUnitType(D_UNITSYS_NONE);	
}

void CCMTdmtRUS::SetData2Dlg()
{ 
	m_edtConcB   .SetEditUnit(m_pData->COMMON.dCompStrength);
	m_edtRelaHumi.SetValue(m_pData->COMMON.dRelativeHumidity);
	m_edtM       .SetEditUnit(m_pData->RUSSIA.dExposeSurfM);
	m_edtAge     .SetValue(m_pData->COMMON.dConcreteAge);
	m_nCuringMeth = m_pData->RUSSIA.nCuringMeth;
	m_nCementType = m_pData->RUSSIA.nCementType;
	m_bCreep      = m_pData->RUSSIA.bCreep;
	m_nConcType   = m_pData->RUSSIA.nConcType;
	m_edtWater   .SetEditUnit(m_pData->RUSSIA.dWater);
	m_edtMaxSize .SetEditUnit(m_pData->RUSSIA.dMaxSize);
	m_edtAir     .SetEditUnit(m_pData->RUSSIA.dAir);
	m_edtPz      .SetEditUnit(m_pData->RUSSIA.dPz);

	UpdateData(FALSE);
}

void CCMTdmtRUS::SetDlg2Data()
{
	UpdateData(TRUE);
	
	m_pData->COMMON.dCompStrength     = m_edtConcB.GetEditValue();
	m_pData->COMMON.dRelativeHumidity = m_edtRelaHumi.GetEditValue();
	m_pData->RUSSIA.dExposeSurfM      = m_edtM.GetEditValue();
	m_pData->COMMON.dConcreteAge      = m_edtAge.GetEditValue();
	m_pData->RUSSIA.nCuringMeth       = m_nCuringMeth;
	m_pData->RUSSIA.nCementType       = m_nCementType;
	m_pData->RUSSIA.bCreep            = m_bCreep;
	m_pData->RUSSIA.nConcType         = m_nConcType;
	m_pData->RUSSIA.dWater            = m_edtWater.GetEditValue();
	m_pData->RUSSIA.dMaxSize          = m_edtMaxSize.GetEditValue();
	m_pData->RUSSIA.dAir              = m_edtAir.GetEditValue();
	m_pData->RUSSIA.dPz               = m_edtPz.GetEditValue();
}

BOOL CCMTdmtRUS::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
		
	ASSERT(m_pData != 0); // Please Set m_pData;

	InitUnit();
	
	m_edtRelaHumi.SetRange(0, 100);
	m_edtRelaHumi.SetValue(70);

	m_edtAge.SetRange(0, 10000);
	m_edtAge.SetValue(3);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTdmtRUS::OnCmdHcrpRUSRelaSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_RUS_RELA_EDT,0,100,pNMHDR,pResult);
	
	*pResult = 0;
}
	
void CCMTdmtRUS::OnCmdHcrpRUSAgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_RUS_AGE_EDT,0,10000,pNMHDR,pResult);
	
	*pResult = 0;
}
