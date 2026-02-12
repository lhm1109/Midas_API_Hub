#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdReinforceInfoDlg.h"
#include "CmdIsolatorGBStressLmtTableDlg.h"
#include "ExportFunc.h"
#include "CMLoadCombDlg.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\DampDgnMgr_CH.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "CmdDampLoadCombTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCmdReinforceInfoDlg::CCmdReinforceInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdReinforceInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdReinforceInfoDlg)
	m_nBuildingType = 0;
	//}}AFX_DATA_INIT
}

void CCmdReinforceInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdReinforceInfoDlg)
	DDX_Radio(pDX, IDC_CMD_STATIC3, m_nBuildingType);
	DDX_Control(pDX, IDC_CMD_STATIC10, m_AssessCombox);
	DDX_Control(pDX, IDC_CMD_STATIC11, m_DesignCombox);
	//IDC_CMD_STATIC3
}

BEGIN_MESSAGE_MAP(CCmdReinforceInfoDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_REINFORCE_OK_BTN, OnOK)
	ON_BN_CLICKED(IDC_CMD_REINFORCE_CANCEL_BTN, OnCancel)
END_MESSAGE_MAP()

BOOL CCmdReinforceInfoDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	//鉴定标准
	m_AssessCombox.AddString(_T("GB 50144-2019"));
	m_AssessCombox.EnableWindow(FALSE); //Combox
	m_AssessCombox.SetCurSel(0);
	//设计标准
	m_DesignCombox.AddString(_T("GB 50017-03"));
	m_DesignCombox.SetCurSel(0);

	Data2Dlg();
	return TRUE;
}

void CCmdReinforceInfoDlg::Data2Dlg()
{
	T_RCTI_D RctiD; 

	if (!m_pDoc->m_pAttrCtrl->GetRcti(RctiD))
	{
		RctiD.Initialize();
	}
	m_AssessCombox.SelectString(0, RctiD.strAssessCode);
	m_DesignCombox.SelectString(0, RctiD.strDesingCode);
	m_nBuildingType = RctiD.nBuildingType;
	UpdateData(FALSE);
}

BOOL CCmdReinforceInfoDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();
	m_AssessCombox.GetWindowText(m_Data.strAssessCode);
	m_Data.nBuildingType = m_nBuildingType;
	m_DesignCombox.GetWindowText(m_Data.strDesingCode);
	return TRUE;
}

void CCmdReinforceInfoDlg::OnOK()
{
	if(!Dlg2Data())	
		return;
	if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddRcti(m_Data)) 
		return;
	CDialogMove::OnOK();
}

void CCmdReinforceInfoDlg::OnCancel()
{
	CDialogMove::OnCancel();
}


