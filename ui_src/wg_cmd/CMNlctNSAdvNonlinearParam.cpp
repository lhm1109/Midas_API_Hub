#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNlctNSAdvNonlinearParam.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMNlctNSAdvNonlinearParam, CDialogMove)

CMNlctNSAdvNonlinearParam::CMNlctNSAdvNonlinearParam(CWnd* pParent /*= NULL*/)
	:CDialogMove(CMNlctNSAdvNonlinearParam::IDD, pParent)
{
	m_Data.Initialize();
}

CMNlctNSAdvNonlinearParam::~CMNlctNSAdvNonlinearParam()
{

}
BEGIN_MESSAGE_MAP(CMNlctNSAdvNonlinearParam, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_USE_DEFAULT_SETTING_CHK, OnCmdCheckDefault)
	ON_BN_CLICKED(IDC_CMD_ENABLE_LINE_SEARCH_CHK, OnCmdCheckLineSearch)
	ON_CBN_SELCHANGE(IDC_CMD_STIFF_UPDATE_SCHEME_CBX, OnCmdChangeSchemeCbx)
	ON_CBN_SELCHANGE(IDC_LINE_SEARCH_OPT_CMB, OnCmdChangeLineSearchOptCbx)
END_MESSAGE_MAP()

void CMNlctNSAdvNonlinearParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CMD_USE_DEFAULT_SETTING_CHK, m_bDefaultSetting);
	DDX_Check(pDX, IDC_CMD_TERMINATE_ON_FAIL_CONV_CHK, m_bTerminateAnal);
	DDX_Control(pDX, IDC_CMD_STIFF_UPDATE_SCHEME_CBX, m_cbxStiffScheme);
	DDX_Control(pDX, IDC_CMD_NUM_OF_ITER_BEFORE_UPDATE_EDT, m_edtNumofIterStiff);
	DDX_Control(pDX, IDC_CMD_MAX_NUM_OF_ITER_PER_INCRE_EDT, m_edtMaxNumofIter);
	DDX_Control(pDX, IDC_CMD_MAX_BISECT_LVL_EDT, m_edtMaxBisectLevel);
	DDX_Control(pDX, IDC_CMD_SMART_BISECTION_CHK, m_chkSmartBisection);
	DDX_Control(pDX, IDC_CMD_DIVERGENCE_THRESHOLD_EDT, m_edtDivThreshold);
	DDX_Control(pDX, IDC_CMD_ENABLE_LINE_SEARCH_CHK, m_chkEnableLineSearch);
	DDX_Control(pDX, IDC_LINE_SEARCH_OPT_CMB, m_cbxLineSearchOpt);
	DDX_Control(pDX, IDC_CMD_MAX_LINE_SEARCH_ITER_EDT, m_edtMaxLineSearch);
	DDX_Control(pDX, IDC_CMD_LINE_SEARCH_TOL_EDT, m_edtLineSearchTol);
}


void CMNlctNSAdvNonlinearParam::SetData(T_NLCT_NLCTRL_PARM& data)
{
	m_Data = data;
}

void CMNlctNSAdvNonlinearParam::GetData(T_NLCT_NLCTRL_PARM& data)
{
	data = m_Data;
}

BOOL CMNlctNSAdvNonlinearParam::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	SetStiffUpdateSchemeCbx();
	SetLineSearchOptCmb();

	Data2Dlg();

	OnCmdCheckDefault();

	return TRUE;
}

void CMNlctNSAdvNonlinearParam::OnOK()
{
	if (!DoApply())return;
	CDialogMove::OnOK();
}

void CMNlctNSAdvNonlinearParam::Data2Dlg()
{
	m_bDefaultSetting = m_Data.bUseDefaultNLParm;
	m_bTerminateAnal = m_Data.bTerminateAnalOnFailConv;
	m_cbxStiffScheme.SetCurSel(m_Data.nStiffUpdateScheme);
	m_edtNumofIterStiff.SetEditUnit(m_Data.nIterStiffUpdate);
	m_edtMaxNumofIter.SetEditUnit(m_Data.nMaxIterPerIncre);
	m_edtMaxBisectLevel.SetEditUnit(m_Data.nMaxBisectLevel);
	m_chkSmartBisection.SetCheck(m_Data.bSmartBisection);
	m_edtDivThreshold.SetEditUnit(m_Data.nDiverThreshold);
	m_chkEnableLineSearch.SetCheck(m_Data.bEnableLineSearch);
	CDlgUtil::CobxSetCurSelItemData(m_cbxLineSearchOpt, m_Data.nLineSearchOption);
	m_edtMaxLineSearch.SetEditUnit(m_Data.nMaxLineSearch);
	m_edtLineSearchTol.SetEditUnit(m_Data.dLineSearchTol);

	UpdateData(FALSE);

}

BOOL CMNlctNSAdvNonlinearParam::DoApply()
{
	UpdateData(TRUE);
	m_Data.bUseDefaultNLParm = m_bDefaultSetting;
	m_Data.bTerminateAnalOnFailConv = m_bTerminateAnal;
	m_Data.bEnableLineSearch = m_chkEnableLineSearch.GetCheck();
	m_Data.nStiffUpdateScheme = m_cbxStiffScheme.GetCurSel();
	m_Data.nIterStiffUpdate = m_edtNumofIterStiff.GetEditValue();
	m_Data.nMaxIterPerIncre = m_edtMaxNumofIter.GetEditValue();
	m_Data.nMaxBisectLevel = m_edtMaxBisectLevel.GetEditValue();
	m_Data.bSmartBisection = m_chkSmartBisection.GetCheck();
	m_Data.nDiverThreshold = m_edtDivThreshold.GetEditValue();
	CDlgUtil::CobxGetItemDataByCurSel(m_cbxLineSearchOpt, reinterpret_cast<DWORD&>(m_Data.nLineSearchOption));
	m_Data.nMaxLineSearch = m_edtMaxLineSearch.GetEditValue();
	m_Data.dLineSearchTol = m_edtLineSearchTol.GetEditValue();

	return TRUE;
}

void CMNlctNSAdvNonlinearParam::SetStiffUpdateSchemeCbx()
{
	m_cbxStiffScheme.ResetContent();
	m_cbxStiffScheme.AddString(_LS(IDS_CMD_NLCTRL_PARAM_CUSTOM));
	m_cbxStiffScheme.AddString(_LS(IDS_CMD_NLCTRL_PARAM_FULL_NEWTON_RAPSHON));
	m_cbxStiffScheme.AddString(_LS(IDS_CMD_NLCTRL_PARAM_INIT_STIFFNESS));
	m_cbxStiffScheme.SetCurSel(0);

	CString str;
	CSize sz;
	int dx = 0;
	CDC* pDC = m_cbxStiffScheme.GetDC();
	for (int i = 0; i < m_cbxStiffScheme.GetCount(); i++)
	{
		m_cbxStiffScheme.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);
		if (sz.cx > dx)dx = sz.cx;
	}
	m_cbxStiffScheme.SetDroppedWidth(dx);
	m_cbxStiffScheme.ReleaseDC(pDC);
}

void CMNlctNSAdvNonlinearParam::SetLineSearchOptCmb()
{
	const std::vector<std::pair<CString, DWORD>> vData = {
		{_T("Auto Line Search"), NLCT_NLCTRL_PARM::Auto},
		{_T("User Defined"), NLCT_NLCTRL_PARM::UserDefined}
	};
	CDlgUtil::InitComboBox(m_cbxLineSearchOpt, vData);
}

void CMNlctNSAdvNonlinearParam::EnableLineSearch(bool bEnable)
{
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_LINE_SEARCH_GRP, bEnable, TRUE);
	if (bEnable) OnCmdChangeLineSearchOptCbx();
}

void CMNlctNSAdvNonlinearParam::OnCmdCheckDefault()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_NL_PARAM_GRP, !m_bDefaultSetting, TRUE);
	GetDlgItem(IDC_CMD_USE_DEFAULT_SETTING_CHK)->EnableWindow(TRUE);
	if (m_bDefaultSetting) return;

	GetDlgItem(IDC_CMD_NUM_OF_ITER_BEFORE_UPDATE_EDT)->EnableWindow(m_cbxStiffScheme.GetCurSel() == 0);	

	EnableLineSearch(m_chkEnableLineSearch.GetCheck());
}

void CMNlctNSAdvNonlinearParam::OnCmdCheckLineSearch()
{
	const bool bEnable = m_chkEnableLineSearch.GetCheck();
	EnableLineSearch(bEnable);
}

void CMNlctNSAdvNonlinearParam::OnCmdChangeSchemeCbx()
{
	UpdateData(TRUE);

	if (m_cbxStiffScheme.GetCurSel() == 0)m_edtNumofIterStiff.SetEditUnit(5);
	if (m_cbxStiffScheme.GetCurSel() == 1)m_edtNumofIterStiff.SetEditUnit(1);
	if (m_cbxStiffScheme.GetCurSel() == 2)m_edtNumofIterStiff.SetEditUnit(51);
	if (m_cbxStiffScheme.GetCurSel() == 3)m_edtNumofIterStiff.SetEditUnit(50);
	if (m_cbxStiffScheme.GetCurSel() == 4)m_edtNumofIterStiff.SetEditUnit(50);
	
	GetDlgItem(IDC_CMD_NUM_OF_ITER_BEFORE_UPDATE_EDT)->EnableWindow(m_cbxStiffScheme.GetCurSel() == 0);

	UpdateData(FALSE);
}

void CMNlctNSAdvNonlinearParam::OnCmdChangeLineSearchOptCbx()
{
    CArray<UINT, UINT> IDCtrls;
	IDCtrls.Add(IDC_CMD_MAX_LINE_SEARCH_ITER_EDT);
	IDCtrls.Add(IDC_CMD_LINE_SEARCH_TOL_EDT);

	int nLineSearchOption;
	CDlgUtil::CobxGetItemDataByCurSel(m_cbxLineSearchOpt, reinterpret_cast<DWORD&>(nLineSearchOption));

	BOOL bEnable = nLineSearchOption == NTIS_BOUNDARY::UserDefined;

	CDlgUtil::CtrlEnableDisable (this, IDCtrls, bEnable);
}