// EtcSpctDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtcSpctDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtcSpctDlg dialog

CEtcSpctDlg::CEtcSpctDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtcSpctDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEtcSpctDlg)
	m_nSignType = 0;	
	//}}AFX_DATA_INIT
	m_aCtrlRadio.Add(IDC_CMD_SPEC_TYPE_SRSS);
	m_aCtrlRadio.Add(IDC_CMD_SPEC_TYPE_CQC);
	m_aCtrlRadio.Add(IDC_CMD_SPEC_TYPE_ABS);

	m_aCtrlDamping.Add(IDC_CMD_SPEC_DAMP_STATIC);
	m_aCtrlDamping.Add(IDC_CMD_SPEC_DAMP_RATIO);

	m_arCtrlSignType.Add(IDC_CMD_SPEC_RESULT_DIRECTION);
	m_arCtrlSignType.Add(IDC_CMD_SPEC_RESULT_ABSOLUTE);  
}


void CEtcSpctDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtcSpctDlg)	
	DDX_Control(pDX, IDC_CMD_SPEC_TYPE_RESULTSIGN, m_chkResultSign);
	DDX_Control(pDX, IDC_CMD_SPEC_DAMP_RATIO, m_wndDampEdit);
	DDX_Radio(pDX, IDC_CMD_SPEC_RESULT_DIRECTION, m_nSignType);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEtcSpctDlg, CDialogMove)
	//{{AFX_MSG_MAP(CEtcSpctDlg)
	ON_BN_CLICKED(IDC_CMD_SPEC_TYPE_SRSS, OnClickRadio)
	ON_BN_CLICKED(IDC_CMD_SPEC_TYPE_RESULTSIGN, OnCmdSpecTypeResultsign)
	ON_BN_CLICKED(IDC_CMD_SPEC_TYPE_CQC, OnClickRadio)
	ON_BN_CLICKED(IDC_CMD_SPEC_TYPE_ABS, OnClickRadio)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcSpctDlg message handlers

BOOL CEtcSpctDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndDampEdit.SetUnitType(CUnitCtrl::m_SPCT_UNIT.dblDamping);

	T_SPCT_D data;
	if (!m_pDoc->m_pAttrCtrl->GetSpct(data))
	{
//    data.dblDamping = 0.0;
//    data.nSpecCombType = 1;
		data.Initialize();
	}
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadio, data.nSpecCombType-1);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDamping, data.nSpecCombType == 2);
	m_chkResultSign.SetCheck(data.bResultSign);
	CDlgUtil::CtrlEnableDisable(this, m_arCtrlSignType, data.bResultSign == TRUE);
	m_nSignType = data.nSignType;
	CString csVal;
	csVal.Format(_T("%g"), data.dblDamping);
	m_wndDampEdit.SetWindowText(csVal);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtcSpctDlg::OnClickRadio() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, nCheck);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDamping, nCheck == 1);
	if (nCheck != 1) m_wndDampEdit.SetWindowText(_T("0"));
}

void CEtcSpctDlg::OnOK() 
{
	UpdateData(TRUE);

	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, nCheck);
	T_SPCT_D data;
	data.Initialize();
	data.nSpecCombType = nCheck+1;
	if(nCheck == 1) data.dblDamping = m_wndDampEdit.GetEditValue();

	data.bResultSign = m_chkResultSign.GetCheck();
	data.nSignType = m_nSignType;
	
	if (m_pDoc->m_pDataCtrl->AddSpct(data))	CDialogMove::OnOK();
}

void CEtcSpctDlg::OnCmdSpecTypeResultsign() 
{
	CDlgUtil::CtrlEnableDisable(this, m_arCtrlSignType, m_chkResultSign.GetCheck() == TRUE);
}
