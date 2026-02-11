// CMTdmt_ABNT_NBR_6118.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmt_ABNT_NBR_6118.h"

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
// CCMTdmt_ABNT_NBR_6118 dialog


CCMTdmt_ABNT_NBR_6118::CCMTdmt_ABNT_NBR_6118(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmt_ABNT_NBR_6118::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmt_ABNT_NBR_6118)
	m_nCementPerformance = 0;
	m_nDeflectionRange = 0;
	m_nRH = 0;
	//}}AFX_DATA_INIT
}


void CCMTdmt_ABNT_NBR_6118::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmt_ABNT_NBR_6118)
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_FCK_EDT, m_edtfck);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_FCK_UNT, m_untfck);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_RH_EDT, m_edtRH);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_THK_EDT, m_edtThk);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_THK_UNT, m_untThk);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_CTYPE_CMB, m_cbmCtype);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_AGG_TYPE_CMB, m_cbmAtype);
	DDX_Radio(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_HUMIDITY_RDO, m_nRH);
	DDX_Radio(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_C20_C45_RDO, m_nCementPerformance);
	DDX_Radio(pDX, IDC_CMD_HCRP_TDMT_ABNT_NBR_6118_0CM_4CM_RDO, m_nDeflectionRange);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_AGE_EDT, m_edtAge);
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_AGE_UNT, m_untAge);
	//}}AFX_DATA_MAP
}


void CCMTdmt_ABNT_NBR_6118::InitUnit()
{
	m_edtfck.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_untfck.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_edtRH.SetValue(D_UNITSYS_NONE);
	m_edtThk.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dThickness);
	m_untThk.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dThickness);
	m_edtAge.SetValue(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_untAge.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
}



void CCMTdmt_ABNT_NBR_6118::SetData2Dlg()
{
	m_edtfck.SetEditUnit(m_pData->COMMON.dCompStrength);
	m_edtRH.SetValue(m_pData->COMMON.dRelativeHumidity);

	if (m_pData->BRAZIL.bInWater)
	{
		m_nRH = 1;
		GetDlgItem(IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_RH_EDT)->EnableWindow(FALSE);
	}
	else
	{
		m_nRH = 0;
		GetDlgItem(IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_RH_EDT)->EnableWindow(TRUE);
	}
	m_edtThk.SetEditUnit(m_pData->BRAZIL.dThickness);

	m_cbmCtype.SetCurSel(m_pData->BRAZIL.nCementType);
	m_cbmAtype.SetCurSel(m_pData->BRAZIL.nAggtype);

  m_nCementPerformance = m_pData->BRAZIL.nCementPerformanceType;
  m_nDeflectionRange = m_pData->BRAZIL.nDeflectionRange;

	m_edtAge.SetValue(m_pData->COMMON.dConcreteAge);

	UpdateData(FALSE);
}

void CCMTdmt_ABNT_NBR_6118::SetDlg2Data()
{
	UpdateData(TRUE);
	m_pData->BRAZIL.Initialize();

	m_pData->COMMON.dCompStrength = m_edtfck.GetEditValue();
	m_pData->COMMON.dRelativeHumidity = m_edtRH.GetEditValue();

	if(m_nRH == 1)
	{
		m_pData->BRAZIL.bInWater = TRUE;
	}
	else
	{
		m_pData->BRAZIL.bInWater = FALSE;
  }

	m_pData->BRAZIL.dThickness = m_edtThk.GetEditValue();
	m_pData->BRAZIL.nCementType = m_cbmCtype.GetCurSel();
	m_pData->BRAZIL.nAggtype = m_cbmAtype.GetCurSel();
	m_pData->BRAZIL.nCementPerformanceType = m_nCementPerformance;
	m_pData->BRAZIL.nDeflectionRange = m_nDeflectionRange;
	m_pData->COMMON.dConcreteAge = m_edtAge.GetEditValue();
}


BEGIN_MESSAGE_MAP(CCMTdmt_ABNT_NBR_6118, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmt_ABNT_NBR_6118)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_RH_SPN, OnCmdTdmtNBRRHSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_AGE_SPN, OnCmdTdmtNBRAgeSpin)
	ON_BN_CLICKED(IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_HUMIDITY_RDO, OnClickRHRdo)
	ON_BN_CLICKED(IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_WATER_RDO, OnClickRHRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMTdmt_ABNT_NBR_6118 message handlers

BOOL CCMTdmt_ABNT_NBR_6118::OnInitDialog()
{
	CDlgChild::OnInitDialog();

	InitUnit();
	m_edtRH.SetRange(40, 90);
	m_edtRH.SetValue(70);

	m_edtAge.SetRange(0, 100);
	m_edtAge.SetValue(3);


	m_cbmCtype.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_CTYPE_CMB1));
	m_cbmCtype.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_CTYPE_CMB2));
	m_cbmCtype.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_CTYPE_CMB3));

	CDlgUtil::CobxAdjustListBoxWidth(m_cbmCtype);

	m_cbmAtype.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_ATYPE_CMB1));
	m_cbmAtype.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_ATYPE_CMB2));
	m_cbmAtype.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_ATYPE_CMB3));
	m_cbmAtype.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_ATYPE_CMB4));

	CDlgUtil::CobxAdjustListBoxWidth(m_cbmAtype);

	GetDlgItem(IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_RH_EDT)->EnableWindow((m_nRH == 0));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTdmt_ABNT_NBR_6118::OnCmdTdmtNBRRHSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_RH_EDT, 40, 90, pNMHDR, pResult);

	*pResult = 0;
}

void CCMTdmt_ABNT_NBR_6118::OnCmdTdmtNBRAgeSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_ETC_HCRP_TDMT_KDS2016_AGE_EDT, 0, 100, pNMHDR, pResult);

	*pResult = 0;
}

void CCMTdmt_ABNT_NBR_6118::OnClickRHRdo()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ETC_HCRP_TDMT_ABNT_NBR_6118_RH_EDT)->EnableWindow((m_nRH == 0));
}