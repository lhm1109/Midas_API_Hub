// CMNlacDlg.cpp : implementation file
//
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNlacDlg_MEC.h"

#include "../wg_base/wg_base_DlgUtil.h"

#include "../wg_db/wg_db_AttrCtrl.h"
#include "../wg_db/wg_db_DataCtrl.h"
#include "../wg_db/wg_db_ViewBuff.h"
#include "../wg_db/EditData.h"

#include "../wg_main/wg_mainRes2.h"

#include "CMNXNlctLoadStepDlg.h"
#include "CMNlctNSAdvNonlinearParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_TOP_MARGIN 50 

/////////////////////////////////////////////////////////////////////////////
// CCMNlacDlg_MEC dialog

CCMNlacDlg_MEC::CCMNlacDlg_MEC(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMNlacDlg_MEC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMNlacDlg_MEC)
	m_nMethod = 0;
	m_nLoadStepType = 0;
	
	m_DataGUI.Initialize();
	
	m_nPrevIncStep_ForceDisp = 1;
	m_nPrevIncStep_Arc = 100;
	//}}AFX_DATA_INIT  
}

void CCMNlacDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMNlacDlg_MEC)  
	DDX_Control(pDX, IDC_CMD_NLAC_SEL_LCASE_CBX, m_LoadCase);
	DDX_Radio(pDX, IDC_CMD_NLAC_ITER_METHOD_RDO, m_nMethod);
	DDX_Radio(pDX, IDC_CMD_NLAC_NUMBER_OF_INC_LOAD_RDO, m_nLoadStepType);
	DDX_Control(pDX, IDC_CMD_NLAC_EDIT_NUM_INCR_LOAD, m_edtNumberOfIncrements);
	DDX_Control(pDX, IDC_CMD_NLAC_CBX_INC_METHOD_LOAD, m_cmbIntermediateOutputRequest);
	DDX_Control(pDX, IDC_CMD_NLAC_DISP_CHK, m_chkDisp);
	DDX_Control(pDX, IDC_CMD_NLAC_LOAD_CHK, m_chkLoad);
	DDX_Control(pDX, IDC_CMD_NLAC_WORK_CHK, m_chkWork);
	DDX_Control(pDX, IDC_CMD_NLAC_DISP_ED, m_edtDisp);
	DDX_Control(pDX, IDC_CMD_NLAC_LOAD_ED, m_edtLoad);
	DDX_Control(pDX, IDC_CMD_NLAC_WORK_ED, m_edtWork);

	DDX_Control(pDX, IDC_CMD_NONL_MIN_ARC_RATIO_EDT, m_edtMinArcRatio);
	DDX_Control(pDX, IDC_CMD_NONL_MAX_ARC_RATIO_EDT, m_edtMaxArcRatio);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXINCR_ED, m_edtMaxincr);

	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MASTER_NODE_ED, m_edtDispCtrlMasterNode);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXDISP_ED, m_edtDispCtrlMaxDisp);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXDISP_UNIT, m_untDispCtrlMaxDisp);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_DIRECTION_CMB, m_cmbDispCtrlDirection);
	DDX_Control(pDX, IDC_CMD_NLAC_RELATIVE_NODE_CHK, m_chkRelativeNode);
	DDX_Control(pDX, IDC_CMD_NLAC_RELATIVE_NODE_ED, m_edtRelativeNode);
	DDX_Control(pDX, IDC_CMD_NLAC_NONLINEAR_TYPE_CHK, m_chkGeom);
	DDX_Control(pDX, IDC_CMD_NLAC_NONLINEAR_TYPE_CHK2, m_chkMatl);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMNlacDlg_MEC, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMNlacDlg_MEC)  
	ON_BN_CLICKED(IDC_CMD_NLAC_ALL_LCASE_RDO, OnCmdNlctAllLcaseRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_SEL_LCASE_RDO, OnCmdNlctSelLcaseRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_ITER_METHOD_RDO, OnCmdNlctAcasMethodRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_ITER_METHOD_RDO2, OnCmdNlctAcasMethodRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_ITER_METHOD_RDO3, OnCmdNlctAcasMethodRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_NUMBER_OF_INC_LOAD_RDO, OnCmdNlctAcasLoadStepTypeRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_MANUAL_USER_STEP_LOAD_RDO, OnCmdNlctAcasLoadStepTypeRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_DISP_CHK, OnCmdNlctAcasDispChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_LOAD_CHK, OnCmdNlctAcasLoadChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_WORK_CHK, OnCmdNlctAcasWorkChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_RELATIVE_NODE_CHK, OnCmdNlctAcasRelativeNodeChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_BTN_LOAD_STEP_LOAD, OnCmdNlctAcasLoadStepsBtn)
	ON_BN_CLICKED(IDC_CMD_NLAC_BTN_ADV_NON_SETTING, OnCmdNlctAcasAdvancedBtn)
	ON_BN_CLICKED(IDC_CMD_NLAC_NONLINEAR_TYPE_CHK, OnCmdBtnGeom)
	ON_BN_CLICKED(IDC_CMD_NLAC_NONLINEAR_TYPE_CHK2, OnCmdBtnMatl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD_MOD, OnCmdBtnAddMod)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDel)
	ON_BN_CLICKED(IDC_CMD_NLAC_REMOVE, OnCmdBtnRemoveCtrlData)
	ON_BN_CLICKED(IDC_CMD_NLAC_SEL_LCASE_BTN, OnClickLoadCaseBtn)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMNlacDlg_MEC message handlers

BOOL CCMNlacDlg_MEC::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	
	SetDataByDB();
	InitUnit();
	InitCombo();
	InitControl();
	MakeItemEx(); 	
	SetFirstState();
	Data2Dlg();

	AlignControls();
	ControlsShowHide();

	OnCmdNlctAcasMethodRdo();
	OnCmdNlctAcasDispChk();
	OnCmdNlctAcasLoadChk();
	OnCmdNlctAcasWorkChk();	
	
	m_List.Invalidate();
	m_List.UpdateWindow();

	// 타이머 설정, list 에 redraw 문제 때문에 창이 완벽하게 뜨면 OnTimer 함수에서 redraw 를 
	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMNlacDlg_MEC::SetFirstState()
{
	if (m_Data.nGlobalOrLcase == 1)
	{
		// Lcase 로 세팅된 상태
		CDlgUtil::CtrlRadioSetCheck(this, m_rdoGlobalOrStld, 1);
		m_DataGUI = m_Data.aLCaseData[0].NlctData;
		m_bIsLastStateGlobal = FALSE;
	}
	else
	{
		// Global 로 세팅된 상태
		CDlgUtil::CtrlRadioSetCheck(this, m_rdoGlobalOrStld, 0);
		m_DataGUI = m_Data.GlobalData;
		m_bIsLastStateGlobal = TRUE;
	}
}

void CCMNlacDlg_MEC::SetDataByDB()
{
	if (!m_pDoc->m_pAttrCtrl->GetNlct(m_Data)) m_Data.Initialize();
}

void CCMNlacDlg_MEC::OnOK() 
{
	int nGlobalOrStld = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_rdoGlobalOrStld, nGlobalOrStld);

	m_Data.nGlobalOrLcase = nGlobalOrStld;

	if (nGlobalOrStld == 0)
	{
		if (!Dlg2Data()) return;
		m_Data.GlobalData = m_DataGUI;
	}

	if (!m_pDoc->m_pDataCtrl->AddNlct(m_Data)) return;

	CCMDlgBase::DestroyWindow();
}

void CCMNlacDlg_MEC::OnCancel()
{
	CCMDlgBase::DestroyWindow();
}

void CCMNlacDlg_MEC::SetListCtrlHeader()
{
	const CString aTitle[] = { _LS(IDS_CMD_NLAC_NAME), _LS(IDS_CMD_NLAC_TYPE)};
	CRect rtList;
	m_List.GetWindowRect(rtList);
	const double dWidth = (rtList.Width() * 1.0) - globalUtils.GetSystemMetricsForDPI(&m_List, SM_CXVSCROLL) - 1.0;
	const int nColWidth[] = { static_cast<int>(dWidth * 0.30), static_cast<int>(dWidth * 0.7) };
	LV_COLUMN lvColumn;

	// set full row select mode
	DWORD dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	constexpr int nColNum = 2;
	for (int i = 0; i < nColNum; i++)
	{
		CString title = aTitle[i];
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_CENTER;

		lvColumn.iSubItem = i;
		lvColumn.cx = nColWidth[i];
		lvColumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvColumn);
		m_List.SetColumn(i, &lvColumn);
		title.ReleaseBuffer();
	}
}

void CCMNlacDlg_MEC::InitUnit()
{
	m_rdoGlobalOrStld.RemoveAll();
	m_rdoGlobalOrStld.Add(IDC_CMD_NLAC_ALL_LCASE_RDO);
	m_rdoGlobalOrStld.Add(IDC_CMD_NLAC_SEL_LCASE_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, m_rdoGlobalOrStld, 0);

	m_edtNumberOfIncrements.SetUnitType(D_UNITSYS_NONE);
	m_edtDisp.SetUnitType(D_UNITSYS_NONE);
	m_edtLoad.SetUnitType(D_UNITSYS_NONE);
	m_edtWork.SetUnitType(D_UNITSYS_NONE);

	m_edtMinArcRatio.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxArcRatio.SetUnitType(D_UNITSYS_NONE);

	m_edtDispCtrlMaxDisp.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDispCtrlMaxDisp.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMNlacDlg_MEC::InitCombo()
{
	m_LoadCase.SetIgnoreStldCSLoad(TRUE);
	m_LoadCase.SetLoadType(D_LOADCASE_STATIC);

	m_cmbIntermediateOutputRequest.ResetContent();
	m_cmbIntermediateOutputRequest.AddString(_LS(IDS_CMD_COMBO_OUTPUT_REQUEST_TYPE1));
	m_cmbIntermediateOutputRequest.AddString(_LS(IDS_CMD_COMBO_OUTPUT_REQUEST_TYPE2));
	m_cmbIntermediateOutputRequest.SetCurSel(0);

	m_cmbDispCtrlDirection.ResetContent();
	m_cmbDispCtrlDirection.AddString(_LS(IDS_CMD_NLAC_DX));
	m_cmbDispCtrlDirection.AddString(_LS(IDS_CMD_NLAC_DY));
	m_cmbDispCtrlDirection.AddString(_LS(IDS_CMD_NLAC_DZ));
	m_cmbDispCtrlDirection.SetCurSel(0);
}

void CCMNlacDlg_MEC::InitControl()
{
	// Iteration Method	
	m_DataGUI.nIterationMethod = 0;

	// Spin
	m_edtMaxincr.SetRange(1, 100);

	// 
	m_aCtrlArcLeng.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlArcLeng, IDC_CMD_NLAC_ARCLENG_FRM, TRUE);

	m_aCtrlDispCtrl.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDispCtrl, IDC_CMD_NLAC_DISPCTRL_FRM, TRUE);

	m_edtDispCtrlMasterNode.SetAttNodeList();
	m_edtDispCtrlMasterNode.SetLButtonDownNotifyWindow(this);
	m_edtDispCtrlMasterNode.SetEnterNotifyWindow(this);
	m_edtDispCtrlMasterNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_edtDispCtrlMasterNode.SetMaxNodeKeyNum(1);

	m_edtRelativeNode.SetLButtonDownNotifyWindow(this);
	m_edtRelativeNode.SetEnterNotifyWindow(this);
	m_edtRelativeNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_edtRelativeNode.SetAttNodeList();
	m_edtRelativeNode.SetMaxNodeKeyNum(1);
	
	SetListCtrlHeader();
}

BOOL CCMNlacDlg_MEC::GetSelectedItem(int &nIndex) const
{
	const int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMNlacDlg_MEC::MakeItemEx()
{
	m_List.DeleteAllItems();	
	if (m_Data.aLCaseData.GetSize() == 0) return;

	m_List.SetRedraw(FALSE);
	for (int i = 0; i < m_Data.aLCaseData.GetSize(); i++)
	{
		m_List.InsertItem(i, _T(""));
		SetItem(i, m_Data.aLCaseData[i]);
	}
	m_List.SetRedraw(TRUE);
	m_List.SetItemState(0,  LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED);
}

void CCMNlacDlg_MEC::SetItem(int nIndex, T_NLCT_LCASE_D&data)
{
	LVITEM lvItem;
	for (int i = 0; i < 2; i++)
	{
		lvItem.iItem = nIndex;
		lvItem.iSubItem = i;
		CString str = DataToStr(i, data);
		lvItem.pszText = str.GetBuffer(0);
		lvItem.mask = LVIF_TEXT;

		m_List.SetItem(&lvItem);
		str.ReleaseBuffer();
	}
}

CString CCMNlacDlg_MEC::DataToStr(int i, const T_NLCT_LCASE_D& data) const
{
	CString aLoadType[] = { _LS(IDS_CMD_BUCK_Variable), _LS(IDS_CMD_BUCK_Constant) };
	CString value;
	if (i == 0)
	{
		T_STLD_K key = data.StldK;
		T_STLD_D data;
		m_pDoc->m_pAttrCtrl->GetStld(key, data);
		value = data.LoadCaseName;
	}
	else if (i == 1)
	{
		if (data.NlctData.nNonlinearType == 0)
		{
			value = _LS(IDS_CMD_NLAC_GEOM);
		}
		else if (data.NlctData.nNonlinearType == 1)
		{
			value = _LS(IDS_CMD_NLAC_MATL);
		}
		else if (data.NlctData.nNonlinearType == 2)
		{
			value = _LS(IDS_CMD_NLAC_GEOM_MATL);
		}
	}
	return value;
}

void CCMNlacDlg_MEC::AlignControls()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;

	// Displacement Control
	GetDlgItem(IDC_CMD_NLAC_ARCLENG_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_NLAC_DISPCTRL_FRM)->GetWindowRect(rToMove);

	const int nDistX = rRef.left - rToMove.left;
	const int nDistY = rRef.top - rToMove.top;

	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlDispCtrl, nDistX, nDistY);
}

void CCMNlacDlg_MEC::ControlsShowHide()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aCtrlArcLeng, m_nMethod == 1 || m_nMethod == 0); // Force-Control || Arc-Length
	CDlgUtil::CtrlShowHide(this, m_aCtrlDispCtrl, m_nMethod == 2); // Displacement-Control
}

int CCMNlacDlg_MEC::GetCurNlType() const
{
	int nNlType;
	const BOOL bGeom = m_chkGeom.GetCheck();
	const BOOL bMatl = m_chkMatl.GetCheck();
	if (bGeom && bMatl) nNlType = 2;
	else if (bGeom) nNlType = 0;
	else nNlType = 1;

	return nNlType;
}

void CCMNlacDlg_MEC::EnableDisableControls()
{
	UpdateData(TRUE);

	int nGlobalOrStld;
	CDlgUtil::CtrlRadioGetCheck(this, m_rdoGlobalOrStld, nGlobalOrStld);
	GetDlgItem(IDC_CMD_NLAC_SEL_LCASE_STC)->EnableWindow(nGlobalOrStld == 1);
	GetDlgItem(IDC_CMD_NLAC_SEL_LCASE_CBX)->EnableWindow(nGlobalOrStld == 1);
	GetDlgItem(IDC_CMD_NLAC_SEL_LCASE_BTN)->EnableWindow(nGlobalOrStld == 1);

	GetDlgItem(IDC_CMD_LIST)->EnableWindow(nGlobalOrStld == 1);
	GetDlgItem(IDC_CMD_BTN_ADD_MOD)->EnableWindow(nGlobalOrStld == 1);
	GetDlgItem(IDC_CMD_BTN_DELETE)->EnableWindow(nGlobalOrStld == 1);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlArcLeng, m_nMethod == 1);

	if (m_nMethod == 1 && m_nLoadStepType != 0)
	{
		m_nLoadStepType = 0;
		UpdateData(FALSE);
	}

	GetDlgItem(IDC_CMD_NLAC_MANUAL_USER_STEP_LOAD_RDO)->EnableWindow(m_nMethod != 1);

	GetDlgItem(IDC_CMD_NLAC_EDIT_NUM_INCR_LOAD)->EnableWindow(m_nLoadStepType == 0);
	GetDlgItem(IDC_CMD_NLAC_STATIC_INTERMEDIATE_OUTPUT_LOAD)->EnableWindow(m_nLoadStepType == 0);
	GetDlgItem(IDC_CMD_NLAC_CBX_INC_METHOD_LOAD)->EnableWindow(m_nLoadStepType == 0);

	GetDlgItem(IDC_CMD_NLAC_BTN_LOAD_STEP_LOAD)->EnableWindow(m_nLoadStepType == 1);

	GetDlgItem(IDC_CMD_NLAC_RELATIVE_NODE_ED)->EnableWindow(m_chkRelativeNode.GetCheck());

	// Disp, Load, Work 는 각 함수에서 처리 	
}

void CCMNlacDlg_MEC::Data2Dlg()
{
	if (m_DataGUI.nNonlinearType == 0)
	{
		m_chkGeom.SetCheck(1);
		m_chkMatl.SetCheck(0);
	}
	else if (m_DataGUI.nNonlinearType == 1)
	{
		m_chkGeom.SetCheck(0);
		m_chkMatl.SetCheck(1);
	}
	else
	{
		m_chkGeom.SetCheck(1);
		m_chkMatl.SetCheck(1);
	}

	m_nMethod = m_DataGUI.nIterationMethod;
	m_nLoadStepType = m_DataGUI.nLoadStepType;
	m_edtNumberOfIncrements.SetEditUnit(m_DataGUI.nNumberOfIncrement);
	m_cmbIntermediateOutputRequest.SetCurSel(m_DataGUI.nIntermediateOutputRequest);

	m_chkDisp.SetCheck(m_DataGUI.bConvFlag[0]);
	m_chkLoad.SetCheck(m_DataGUI.bConvFlag[1]);
	m_chkWork.SetCheck(m_DataGUI.bConvFlag[2]);
	m_edtDisp.SetEditUnit(m_DataGUI.dConvValue[0]);
	m_edtLoad.SetEditUnit(m_DataGUI.dConvValue[1]);
	m_edtWork.SetEditUnit(m_DataGUI.dConvValue[2]);

	m_edtMinArcRatio.SetEditUnit(m_DataGUI.dMinArcRatio);
	m_edtMaxArcRatio.SetEditUnit(m_DataGUI.dMaxArcRatio);

	CString str;
	str.Format(_T("%d"), m_DataGUI.nMaxIncStepArclen);
	m_edtMaxincr.SetWindowText(str);

	m_edtDispCtrlMasterNode.SetKeyPressedFlag(FALSE);
	m_edtDispCtrlMasterNode.AddNodeKey(m_DataGUI.MasterNodeKeyDispct);
	m_edtDispCtrlMaxDisp.SetEditUnit(m_DataGUI.dMaxDisplacementDispct);
	m_cmbDispCtrlDirection.SetCurSel(m_DataGUI.nDirectionDispct);
	m_chkRelativeNode.SetCheck(m_DataGUI.bRelativeNode);
	m_edtRelativeNode.SetKeyPressedFlag(FALSE);
	m_edtRelativeNode.AddNodeKey(m_DataGUI.RelativeNodeKey);

	if (m_nMethod == 0 || m_nMethod == 2)
		m_nPrevIncStep_ForceDisp = m_DataGUI.nNumberOfIncrement;
	else
		m_nPrevIncStep_Arc = m_DataGUI.nNumberOfIncrement;
	 
	UpdateData(FALSE);
	EnableDisableControls();	
}

BOOL CCMNlacDlg_MEC::Dlg2Data()
{
	UpdateData(TRUE);

	m_DataGUI.nNonlinearType = GetCurNlType();

	m_DataGUI.nIterationMethod = m_nMethod;
	m_DataGUI.nLoadStepType = m_nLoadStepType;
	m_DataGUI.nNumberOfIncrement = m_edtNumberOfIncrements.GetEditValue();
	m_DataGUI.nIntermediateOutputRequest = m_cmbIntermediateOutputRequest.GetCurSel();

	m_DataGUI.bConvFlag[0] = m_chkDisp.GetCheck();
	m_DataGUI.bConvFlag[1] = m_chkLoad.GetCheck();
	m_DataGUI.bConvFlag[2] = m_chkWork.GetCheck();
	m_DataGUI.dConvValue[0] = m_edtDisp.GetEditValue();
	m_DataGUI.dConvValue[1] = m_edtLoad.GetEditValue();
	m_DataGUI.dConvValue[2] = m_edtWork.GetEditValue();

	m_DataGUI.dMinArcRatio = m_edtMinArcRatio.GetEditValue();
	m_DataGUI.dMaxArcRatio = m_edtMaxArcRatio.GetEditValue();
	m_DataGUI.nMaxIncStepArclen = m_edtMaxincr.GetEditValue();

	m_DataGUI.dMaxDisplacementDispct = m_edtDispCtrlMaxDisp.GetEditValue();
	m_DataGUI.nDirectionDispct = m_cmbDispCtrlDirection.GetCurSel();
	m_DataGUI.bRelativeNode = m_chkRelativeNode.GetCheck();

	int nMasterNodeKey = 0;
	int nRelativeNodeKey = 0;
	if(m_DataGUI.nIterationMethod == 2) // Displacement-Control
	{
		CFormulaEdit::GetEditValue(&m_edtDispCtrlMasterNode, nMasterNodeKey);
		if(m_DataGUI.bRelativeNode)
		{
			CFormulaEdit::GetEditValue(&m_edtRelativeNode, nRelativeNodeKey);
		}
	}
	m_DataGUI.MasterNodeKeyDispct = nMasterNodeKey;
	m_DataGUI.RelativeNodeKey = nRelativeNodeKey;

	return TRUE;
}

void CCMNlacDlg_MEC::OnCmdNlctAllLcaseRdo()
{
	if (!m_bIsLastStateGlobal)
	{
		// Lcase 에서 Global 로 바꾸면 Global 입력 상태로 ui 를 갱신한다.
		m_DataGUI = m_Data.GlobalData;
		Data2Dlg();

		m_List.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
	}
	m_bIsLastStateGlobal = TRUE;
	ControlsShowHide();
	EnableDisableControls();
}

void CCMNlacDlg_MEC::OnCmdNlctSelLcaseRdo()
{
	if (m_bIsLastStateGlobal)
	{
		// Global 에서 Lcase 로 바꾸면 입력 상태를 저장해 둔다.
		Dlg2Data();
		m_Data.GlobalData = m_DataGUI;
	}
	m_bIsLastStateGlobal = FALSE;
	ControlsShowHide();
	EnableDisableControls();
}

void CCMNlacDlg_MEC::OnCmdNlctAcasMethodRdo()
{
	int nOldMethod = m_nMethod;
	if (m_nMethod == 0 || m_nMethod == 2)
		m_nPrevIncStep_ForceDisp = m_edtNumberOfIncrements.GetEditValue();
	else
		m_nPrevIncStep_Arc = m_edtNumberOfIncrements.GetEditValue();

	UpdateData(TRUE);

	AlignControls();
	ControlsShowHide();
	EnableDisableControls();

	// Arc-Length 일 때는 Number of Increment 초기값을 100으로 설정
	if (nOldMethod != m_nMethod)
	{
		m_edtNumberOfIncrements.SetEditUnit(m_nMethod == 1 ? m_nPrevIncStep_Arc : m_nPrevIncStep_ForceDisp);
	}
}

void CCMNlacDlg_MEC::OnCmdNlctAcasLoadStepTypeRdo()
{
	EnableDisableControls();
}

void CCMNlacDlg_MEC::OnCmdNlctAcasDispChk()
{
	if (!m_chkDisp.GetCheck() && !m_chkLoad.GetCheck() && !m_chkWork.GetCheck())
	{
		m_chkDisp.SetCheck(TRUE);
	}

	GetDlgItem(IDC_CMD_NLAC_DISP_ED)->EnableWindow(m_chkDisp.GetCheck());
}

void CCMNlacDlg_MEC::OnCmdNlctAcasLoadChk()
{
	if (!m_chkDisp.GetCheck() && !m_chkLoad.GetCheck() && !m_chkWork.GetCheck())
	{
		m_chkLoad.SetCheck(TRUE);
	}

	GetDlgItem(IDC_CMD_NLAC_LOAD_ED)->EnableWindow(m_chkLoad.GetCheck());
}

void CCMNlacDlg_MEC::OnCmdNlctAcasWorkChk()
{
	if (!m_chkDisp.GetCheck() && !m_chkLoad.GetCheck() && !m_chkWork.GetCheck())
	{
		m_chkWork.SetCheck(TRUE);
	}

	GetDlgItem(IDC_CMD_NLAC_WORK_ED)->EnableWindow(m_chkWork.GetCheck());
}

void CCMNlacDlg_MEC::OnCmdNlctAcasRelativeNodeChk()
{
	EnableDisableControls();
}

void CCMNlacDlg_MEC::OnCmdNlctAcasLoadStepsBtn()
{
	CMNXNlctLoadStepDlg dlg;
	dlg.SetData(m_DataGUI, m_nMethod);
	dlg.SetInitPos(D_INIT_POS_RT);

	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_DataGUI);
	}
}

void CCMNlacDlg_MEC::OnCmdNlctAcasAdvancedBtn()
{
	CMNlctNSAdvNonlinearParam dlg(0);
	dlg.SetData(m_DataGUI.AdvancedParam);
	dlg.SetInitPos(D_INIT_POS_RT);

	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_DataGUI.AdvancedParam);
	}
}

void CCMNlacDlg_MEC::OnCmdBtnGeom()
{
	ControlsShowHide();
	EnableDisableControls();
	
	BOOL bMatl;
	bMatl = m_chkMatl.GetCheck();
	if (bMatl) return;
	m_chkMatl.SetCheck(1);
}

void CCMNlacDlg_MEC::OnCmdBtnMatl()
{
	ControlsShowHide();
	EnableDisableControls();
	
	BOOL bGeom;
	bGeom = m_chkGeom.GetCheck();
	if (bGeom) return;
	m_chkGeom.SetCheck(1);
}

void CCMNlacDlg_MEC::OnCmdBtnAddMod()
{
	Dlg2Data();

	T_STLD_K StldK;
	UINT nLoadCaseType;
	m_LoadCase.GetSelectedLoad(nLoadCaseType, StldK);
	
	int nIdxFind = -1;
	for (int i = 0; i < m_Data.aLCaseData.GetSize(); i++)
	{
		if (StldK == m_Data.aLCaseData[i].StldK)
		{
			nIdxFind = i;
		}
	}

	if (nIdxFind == -1)
	{
		T_NLCT_LCASE_D NlctAdd;
		NlctAdd.StldK = StldK;
		NlctAdd.NlctData = m_DataGUI;
		m_Data.aLCaseData.Add(NlctAdd);
	}
	else
	{
		ASSERT(m_Data.aLCaseData[nIdxFind].StldK == StldK);
		m_Data.aLCaseData[nIdxFind].NlctData = m_DataGUI;
	}

	MakeItemEx();

	if (nIdxFind == -1)
	{
		SelectListCtrlRow(m_List, m_List.GetItemCount() - 1);
	}
	else
	{
		SelectListCtrlRow(m_List, nIdxFind);
	}
}

void CCMNlacDlg_MEC::SelectListCtrlRow(CListCtrl& listCtrl, int nRow) const
{
	listCtrl.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
	listCtrl.SetItemState(nRow, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	listCtrl.EnsureVisible(nRow, FALSE);
}

void CCMNlacDlg_MEC::OnCmdBtnRemoveCtrlData()
{
	if(m_pDoc->m_pDataCtrl->DelNlct())
	{
		CCMDlgBase::DestroyWindow();
	}
}

void CCMNlacDlg_MEC::OnCmdBtnDel()
{
	int nIdx = 0;
	if (GetSelectedItem(nIdx))
	{
		m_Data.aLCaseData.RemoveAt(nIdx);
		MakeItemEx();
	}
}

void CCMNlacDlg_MEC::OnClickLoadCaseBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));
}

void CCMNlacDlg_MEC::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex;
	if (GetSelectedItem(nIndex))
	{
		m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Data.aLCaseData[nIndex].StldK);
		m_DataGUI = m_Data.aLCaseData[nIndex].NlctData;
		Data2Dlg();
		ControlsShowHide();
		EnableDisableControls();
	}

	*pResult = 0;
}

void CCMNlacDlg_MEC::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMNlacDlg_MEC::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key;
	T_STLD_D Data, DataBak;

	int i;

	BOOL bUpdate = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_STLD_ADD):
			// do nothing
			break;
		case(UR_STLD_DEL):
			pViewBuff->GetStld(nKey, Key, Data);
			for (i = m_Data.aLCaseData.GetSize() - 1; i >= 0; i--)
			{
				if (m_Data.aLCaseData[i].StldK == Key)
				{
					m_Data.aLCaseData.RemoveAt(i);
				}
			}
			bUpdate = TRUE;
			break;
		case(UR_STLD_MFD):			
			bUpdate = TRUE;
			break;
		case(UR_STLD_MFS):
			bUpdate = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if(bUpdate)
		MakeItemEx();
}

void CCMNlacDlg_MEC::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_List.GetSafeHwnd() != nullptr)
	{
		// List를 잘 안그려서 onpaint 에 redraw 코드를 넣음
		m_List.Invalidate();
		m_List.UpdateWindow();
	}

	CCMDlgBase::OnPaint();
}

void CCMNlacDlg_MEC::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		while (m_List.GetSafeHwnd() == nullptr)
		{			
			Sleep(100);
		}

		m_List.Invalidate();
		m_List.UpdateWindow();

		// 타이머 중지
		KillTimer(1);		
	}

	CCMDlgBase::OnTimer(nIDEvent);
}