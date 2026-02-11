// EtcPzefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtcPzefDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtcPzefDlg dialog


CEtcPzefDlg::CEtcPzefDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtcPzefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEtcPzefDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlCalc.Add(IDC_ETC_BLDC_AUTO_CALC);
	m_aCtrlCalc.Add(IDC_ETC_BLDC_NOT_AUTO_CALC);

	m_aCtrlOffType.Add(IDC_ETC_BLDC_OFFSET_T_PZ);
	m_aCtrlOffType.Add(IDC_ETC_BLDC_OFFSET_T_OP);

	m_aCtrlCalcSub.Add(IDC_ETC_BLDC_OFFSET_F);
	m_aCtrlCalcSub.Add(IDC_ETC_BLDC_OFFSET_T_PZ);
	m_aCtrlCalcSub.Add(IDC_ETC_BLDC_OFFSET_T_OP);
}


void CEtcPzefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtcPzefDlg)
	DDX_Control(pDX, IDC_ETC_BLDC_OFFSET_F, m_wndOffsetFactor);
	//}}AFX_DATA_MAP
}

void CEtcPzefDlg::Data2DlgVar()
{
	int nAutoCalc = m_Data.bOffset?0:1;
	int nOffType = (m_Data.nOffsType==1)?0:1;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCalc, nAutoCalc);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOffType, nOffType);
	m_wndOffsetFactor.SetEditUnit(m_Data.dOffsFact);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCalcSub, nAutoCalc==0);	
}

void CEtcPzefDlg::DlgVar2Data()
{
	m_Data.Initialize();
	int nAutoCalc;
	int nOffType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCalc, nAutoCalc);
	if (nAutoCalc == 0)
	{
		m_Data.bOffset = 1;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOffType, nOffType);
		m_Data.nOffsType = nOffType+1;
		m_Data.dOffsFact = m_wndOffsetFactor.GetEditValue();
	}
	else
	{
		m_Data.bOffset = 0;
	}
}


BEGIN_MESSAGE_MAP(CEtcPzefDlg, CDialogMove)
	//{{AFX_MSG_MAP(CEtcPzefDlg)
	ON_BN_CLICKED(IDC_ETC_BLDC_AUTO_CALC, OnChangeAutoCalc)
	ON_BN_CLICKED(IDC_ETC_BLDC_NOT_AUTO_CALC, OnChangeAutoCalc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcPzefDlg message handlers

BOOL CEtcPzefDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndOffsetFactor.SetUnitType(CUnitCtrl::m_PZEF_UNIT.dOffsFact);

	if (!m_pDoc->m_pAttrCtrl->GetPzef(m_Data)) m_Data.Initialize();
	Data2DlgVar();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtcPzefDlg::OnOK() 
{
	// TODO: Add extra validation here
	DlgVar2Data();
	if (!m_pDoc->m_pDataCtrl->AddPzef(m_Data))
	{  
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_add_replace_panel_z));
		return;
	}
		
	CDialogMove::OnOK();
}

void CEtcPzefDlg::OnChangeAutoCalc() 
{
	// TODO: Add your control notification handler code here
	int nAutoCalc;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCalc, nAutoCalc);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCalcSub, nAutoCalc==0);
}
