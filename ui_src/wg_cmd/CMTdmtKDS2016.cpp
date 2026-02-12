// CMTdmtKDS2016.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtKDS2016.h"
#include "CMRefWD_KDS2016.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_main\wg_mainres2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtKDS2016 dialog


CCMTdmtKDS2016::CCMTdmtKDS2016(CWnd* pParent /*=NULL*/)
: CDlgChild(CCMTdmtKDS2016::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtKDS2016)
	m_nCementType = 0;
	//}}AFX_DATA_INIT
}


void CCMTdmtKDS2016::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtKDS2016)
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_FCK_EDT,       m_edtfck);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_FCK_UNT,       m_untfck);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_RH_EDT,        m_edtRH);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_SIZE_EDT,      m_edtSize);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_SIZE_UNT,      m_untSize);
	DDX_Radio  (pDX, IDC_ETC_HCRP_TDMT_KDS2016_CEMENT_RS_RDO, m_nCementType);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_DENSITY_EDT,   m_edtDensity);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_AGE_EDT,       m_edtAge);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_AGE_UNT,       m_untAge);
	//}}AFX_DATA_MAP
}


void CCMTdmtKDS2016::InitUnit()
{
	m_edtfck.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_untfck.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_edtRH.SetValue(D_UNITSYS_NONE);
	m_edtSize.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_untSize.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_edtDensity.SetUnitType(D_UNITSYS_NONE);
	m_edtAge.SetValue(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_untAge.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
}



void CCMTdmtKDS2016::SetData2Dlg()
{ 
	m_edtfck.SetEditUnit(m_pData->COMMON.dCompStrength);
	m_edtRH.SetValue(m_pData->COMMON.dRelativeHumidity);
	m_edtSize.SetEditUnit(m_pData->CEB.dMemberSize);
	m_nCementType = m_pData->CEB.nCementType -1;
	if(m_nModify == 1) m_edtDensity.SetEditUnit(m_pData->KDS2016.dDensity);	
	m_edtAge.SetValue(m_pData->COMMON.dConcreteAge);
	
	UpdateData(FALSE);
}

void CCMTdmtKDS2016::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->COMMON.dCompStrength = m_edtfck.GetEditValue();
	m_pData->COMMON.dRelativeHumidity = m_edtRH.GetEditValue();
	m_pData->CEB.dMemberSize = m_edtSize.GetEditValue();
	m_pData->CEB.nCementType = m_nCementType + 1;
	m_pData->KDS2016.dDensity = m_edtDensity.GetEditValue();
	m_pData->COMMON.dConcreteAge = m_edtAge.GetEditValue();
}


BEGIN_MESSAGE_MAP(CCMTdmtKDS2016, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtKDS2016)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ETC_HCRP_TDMT_KDS2016_RH_SPN, OnCmdTdmtKSD2016RHSpin)	
	ON_BN_CLICKED(IDC_ETC_HCRP_TDMT_KDS2016_WD_BTN, OnCmdTdmtKSD2016WDBtn)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ETC_HCRP_TDMT_KDS2016_AGE_SPN, OnCmdTdmtKSD2016AgeSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMTdmtKDS2016 message handlers

BOOL CCMTdmtKDS2016::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	InitUnit();
	m_edtRH.SetRange(40, 99);
	m_edtRH.SetValue(70);

	m_edtAge.SetRange(0, 100);
	m_edtAge.SetValue(3);
	/*if(m_nModify == 0) OnCmdTdmtKSD2016MatlCmb();*/
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTdmtKDS2016::OnCmdTdmtKSD2016RHSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_ETC_HCRP_TDMT_KDS2016_RH_EDT,40,99,pNMHDR,pResult);

	*pResult = 0;
}



void CCMTdmtKDS2016::OnCmdTdmtKSD2016WDBtn()
{
	T_MATL_K MatlKey = 0;
	

	CCMDRefwdKDS2016Dlg Dlg;
	if (IDOK == Dlg.DoModal())
	{
		m_edtDensity.SetEditUnit(Dlg.m_dDensity);
	}

}

void CCMTdmtKDS2016::OnCmdTdmtKSD2016AgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_ETC_HCRP_TDMT_KDS2016_AGE_EDT,0,100, pNMHDR, pResult);	

	*pResult = 0;
}