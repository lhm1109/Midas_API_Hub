#include "stdafx.h"
#include "wg_cmd.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "wg_cmdRes2.h"

#include "CMPostTensionTendonStressingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMPostTensionTendonStressingDlg dialog

CCMPostTensionTendonStressingDlg::CCMPostTensionTendonStressingDlg(CWnd* pParent)
	: CDialogMove(CCMPostTensionTendonStressingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMPostTensionTendonStressingDlg)
	m_strName = _T("");
	m_strFirstJackingStage = _T("0.80");
	m_strFirstTransferStage = _T("0.70");
	m_strFirstServiceStage = _T("0.65");
	m_strLastJackingStage = _T("0.80");
	m_strLastTransferStage = _T("0.70");
	m_strLastServiceStage = _T("0.65");
	m_strMaxRatio = _T("0.8");
	m_strMinRatio = _T("0.5");
	m_strMaxPreComp = _T("2.0");
	m_strMinPreComp = _T("1.0");
	m_bApplySame = TRUE;
	m_bFirstAutoTransfer = TRUE;
	m_bLastAutoTransfer = TRUE;
	m_nFirstMethod = 0;
	m_nLastMethod = 0;
	m_nSlabFilter = 0;
	//}}AFX_DATA_INIT
	
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}

CCMPostTensionTendonStressingDlg::~CCMPostTensionTendonStressingDlg()
{

}

int CCMPostTensionTendonStressingDlg::GetSelectedStressingKey()
{
	int nSelectedIndex = m_listStressing.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
		return -1;

	CString strID = m_listStressing.GetItemText(nSelectedIndex, 0);

	return _ttoi(strID);
}

void CCMPostTensionTendonStressingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMPostTensionTendonStressingDlg)
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_TENDON_LIST, m_listStressing);
	if (GetDlgItem(IDC_CMD_PT_TDN_STRESSING_NAME_EDT))
	{
		DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_NAME_EDT, m_editName);
		DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_NAME_EDT, m_strName);
	}
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_APPLY_SAME_CHECK, m_checkApplySame);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_METHOD_CMB, m_comboFirstMethod);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_JACKING_EDIT, m_editFirstJackingStage);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_TRANSFER_EDIT, m_editFirstTransferStage);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_SERVICE_EDIT, m_editFirstServiceStage);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_AUTO_TRANSFER_CHECK, m_checkFirstAutoTransfer);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_METHOD_CMB, m_comboLastMethod);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_JACKING_EDIT, m_editLastJackingStage);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_TRANSFER_EDIT, m_editLastTransferStage);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_SERVICE_EDIT, m_editLastServiceStage);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_AUTO_TRANSFER_CHECK, m_checkLastAutoTransfer);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_MAX_RATIO_EDIT, m_editMaxRatio);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_MIN_RATIO_EDIT, m_editMinRatio);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_MAX_PRECOMP_EDIT, m_editMaxPreComp);
	DDX_Control(pDX, IDC_CMD_PT_TDN_STRESSING_MIN_PRECOMP_EDIT, m_editMinPreComp);
	DDX_Control(pDX, IDC_CMD_CLOSE, m_btnClose);
	
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_JACKING_EDIT, m_strFirstJackingStage);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_TRANSFER_EDIT, m_strFirstTransferStage);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_SERVICE_EDIT, m_strFirstServiceStage);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_JACKING_EDIT, m_strLastJackingStage);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_TRANSFER_EDIT, m_strLastTransferStage);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_SERVICE_EDIT, m_strLastServiceStage);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_MAX_RATIO_EDIT, m_strMaxRatio);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_MIN_RATIO_EDIT, m_strMinRatio);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_MAX_PRECOMP_EDIT, m_strMaxPreComp);
	DDX_Text(pDX, IDC_CMD_PT_TDN_STRESSING_MIN_PRECOMP_EDIT, m_strMinPreComp);
	DDX_Check(pDX, IDC_CMD_PT_TDN_STRESSING_APPLY_SAME_CHECK, m_bApplySame);
	DDX_Check(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_AUTO_TRANSFER_CHECK, m_bFirstAutoTransfer);
	DDX_Check(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_AUTO_TRANSFER_CHECK, m_bLastAutoTransfer);
	DDX_CBIndex(pDX, IDC_CMD_PT_TDN_STRESSING_FIRST_METHOD_CMB, m_nFirstMethod);
	DDX_CBIndex(pDX, IDC_CMD_PT_TDN_STRESSING_LAST_METHOD_CMB, m_nLastMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMPostTensionTendonStressingDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMPostTensionTendonStressingDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_CMD_ADD_BTN, OnAddBtn)
	ON_BN_CLICKED(IDC_CMD_MOD_BTN, OnModifyBtn)
	ON_BN_CLICKED(IDC_CMD_DEL_BTN, OnDeleteBtn)
	ON_BN_CLICKED(IDC_CMD_PT_TDN_STRESSING_APPLY_SAME_CHECK, OnCheckApplySame)
	ON_CBN_SELCHANGE(IDC_CMD_PT_TDN_STRESSING_FIRST_METHOD_CMB, OnSelChangeFirstMethod)
	ON_CBN_SELCHANGE(IDC_CMD_PT_TDN_STRESSING_LAST_METHOD_CMB, OnSelChangeLastMethod)
	ON_BN_CLICKED(IDC_CMD_PT_TDN_STRESSING_FIRST_AUTO_TRANSFER_CHECK, OnCheckFirstAutoTransfer)
	ON_BN_CLICKED(IDC_CMD_PT_TDN_STRESSING_LAST_AUTO_TRANSFER_CHECK, OnCheckLastAutoTransfer)
	ON_NOTIFY(NM_CLICK, IDC_CMD_PT_TDN_STRESSING_TENDON_LIST, OnClickStressingList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMPostTensionTendonStressingDlg message handlers

BOOL CCMPostTensionTendonStressingDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// Setup method combos
	m_comboFirstMethod.AddString(_T("Jacking Stress"));
	m_comboFirstMethod.AddString(_T("Jacking Stress/fpu"));
	m_comboFirstMethod.AddString(_T("Jacking Stress/fpy"));
	m_comboFirstMethod.SetCurSel(0);
	
	m_comboLastMethod.AddString(_T("Jacking Stress"));
	m_comboLastMethod.AddString(_T("Jacking Stress/fpu"));
	m_comboLastMethod.AddString(_T("Jacking Stress/fpy"));
	m_comboLastMethod.SetCurSel(0);
	
	// Initialize the list control
	MakeListHeader();
	
	// Set initial dialog data
	SetData2Dlg();
	
	// Enable/disable controls based on "Apply the same to both ends" checkbox
	EnableLastSectionControls(!m_bApplySame);
	
	return TRUE;
}

void CCMPostTensionTendonStressingDlg::MakeListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	
	HTitles.Add(_T("ID"));      HRatio.Add(0.35f);
	HTitles.Add(_T("Name"));    HRatio.Add(0.65f);
	
	CDlgUtil::_SetListCtrlHeader(&m_listStressing, HTitles, &HRatio, NULL);
	
	// 체크박스 제거하고 Custom Draw 활성화
	//m_wndTendonList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CCMPostTensionTendonStressingDlg::SetData2Dlg()
{
	UpdateData(FALSE);
	UpdateStressingList();
}

BOOL CCMPostTensionTendonStressingDlg::ApplyDlg()
{
	UpdateData(TRUE);
	
	// Validate input data
	double dValue;
	
	// Validate jacking stage values
	dValue = _tstof(m_strFirstJackingStage);
	if (dValue <= 0.0 || dValue > 1.0)
	{
		AfxMessageBox(_T("Jacking Stage value must be between 0 and 1."));
		return FALSE;
	}
	
	// Additional validation logic would go here
	
	// Apply the tendon stressing data to the model
	// Implementation would depend on the specific data structures used
	
	return TRUE;
}

void CCMPostTensionTendonStressingDlg::EnableLastSectionControls(BOOL bEnable)
{
	m_comboLastMethod.EnableWindow(bEnable);
	m_editLastJackingStage.EnableWindow(bEnable);
	m_editLastTransferStage.EnableWindow(bEnable);
	m_editLastServiceStage.EnableWindow(bEnable);
	m_checkLastAutoTransfer.EnableWindow(bEnable);
	if(bEnable)
	{
		m_editLastTransferStage.EnableWindow(!m_bLastAutoTransfer);
		m_editLastServiceStage.EnableWindow(!m_bLastAutoTransfer);
	}
	
	if (!bEnable)
	{
		// Copy values from first section to last section
		m_strLastJackingStage = m_strFirstJackingStage;
		m_strLastTransferStage = m_strFirstTransferStage;
		m_strLastServiceStage = m_strFirstServiceStage;
		m_bLastAutoTransfer = m_bFirstAutoTransfer;
		m_nLastMethod = m_nFirstMethod;
		UpdateData(FALSE);
	}
}

void CCMPostTensionTendonStressingDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// Handle updates from the document
}

void CCMPostTensionTendonStressingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogMove::OnSize(nType, cx, cy);
}

void CCMPostTensionTendonStressingDlg::OnClose() 
{
	CDialogMove::OnCancel();
}

void CCMPostTensionTendonStressingDlg::OnAddBtn()
{
	Dlg2Data();

	if (!m_pDoc->m_pDataCtrl->AddTdst(m_Data))
	{
		ASSERT(0); return;
	}

	Data2Dlg();
}

void CCMPostTensionTendonStressingDlg::OnModifyBtn()
{
	auto nSelectedID = GetSelectedStressingKey();
	if (nSelectedID < 0)
	{
		ASSERT(0); return;
	}

	T_TDST_K TdstK = nSelectedID;

	Dlg2Data();

	if (!m_pDoc->m_pDataCtrl->ModifyTdst(TdstK, TdstK, m_Data))
	{
		ASSERT(0); return;
	}

	Data2Dlg();
}

void CCMPostTensionTendonStressingDlg::OnDeleteBtn()
{
	auto nSelectedID = GetSelectedStressingKey();
	if (nSelectedID < 0)
	{
		ASSERT(0); return;
	}

	T_TDST_K TdstK = nSelectedID;

	if (!m_pDoc->m_pDataCtrl->DelTdst(TdstK))
	{
		ASSERT(0); return;
	}
	Data2Dlg();
}

void CCMPostTensionTendonStressingDlg::OnCheckApplySame() 
{
	UpdateData(TRUE);
	EnableLastSectionControls(!m_bApplySame);
}

void CCMPostTensionTendonStressingDlg::OnSelChangeFirstMethod() 
{
	UpdateData(TRUE);
	if (m_bApplySame)
	{
		m_nLastMethod = m_nFirstMethod;
		UpdateData(FALSE);
	}
}

void CCMPostTensionTendonStressingDlg::OnSelChangeLastMethod() 
{
	UpdateData(TRUE);
}

void CCMPostTensionTendonStressingDlg::OnCheckFirstAutoTransfer() 
{
	UpdateData(TRUE);
	m_editFirstTransferStage.EnableWindow(!m_bFirstAutoTransfer);
	m_editFirstServiceStage.EnableWindow(!m_bFirstAutoTransfer);

	if (m_bApplySame)
	{
		m_bLastAutoTransfer = m_bFirstAutoTransfer;
	}
	UpdateData(FALSE);
}

void CCMPostTensionTendonStressingDlg::OnCheckLastAutoTransfer() 
{
	UpdateData(TRUE);

	m_editLastTransferStage.EnableWindow(!m_bLastAutoTransfer);
	m_editLastServiceStage.EnableWindow(!m_bLastAutoTransfer);

	UpdateData(FALSE);
}

void CCMPostTensionTendonStressingDlg::OnClickStressingList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->iItem >= 0 && m_editName.GetSafeHwnd())
	{
		CString strName = m_listStressing.GetItemText(pNMLV->iItem, 1);
		m_editName.SetWindowTextW(strName);

		UpdateStressingData();
	}

	*pResult = 0;
}

void CCMPostTensionTendonStressingDlg::UpdateStressingList()
{
	InitTendonStressingList();

	CArray<T_TDST_K, T_TDST_K> aTdstK;
	m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstK);

	T_TDST_D TdstD;
	for (auto i = 0; i < aTdstK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetTdst(aTdstK[i], TdstD))
		{
			continue;
		}

		AddTendonStressingList(aTdstK[i], TdstD.strName);
	}
}

void CCMPostTensionTendonStressingDlg::UpdateStressingData()
{
	T_TDST_D TdstD;

	int nID = GetSelectedStressingKey();
	if (nID < 1) return;

	if (!m_pDoc->m_pAttrCtrl2->GetTdst(nID, TdstD))
	{
		ASSERT(0); return;
	}

	// General Info.
	m_editName.SetWindowText(TdstD.strName);
	m_checkApplySame.SetCheck(TdstD.bApplySame ? BST_CHECKED : BST_UNCHECKED);

	// First
	m_comboFirstMethod.SetCurSel(TdstD.nFirstMethod);

	CString strValue;
	strValue.Format(_T("%.2f"), TdstD.dFirstJacking);
	m_editFirstJackingStage.SetWindowText(strValue);
	strValue.Format(_T("%.2f"), TdstD.dFirstTransfer);
	m_editFirstTransferStage.SetWindowText(strValue);
	strValue.Format(_T("%.2f"), TdstD.dFirstService);
	m_editFirstServiceStage.SetWindowText(strValue);

	m_checkFirstAutoTransfer.SetCheck(TdstD.bFirstAuto ? BST_CHECKED : BST_UNCHECKED);

	// Last
	m_comboLastMethod.SetCurSel(TdstD.nLastMethod);

	strValue.Format(_T("%.2f"), TdstD.dLastJacking);
	m_editLastJackingStage.SetWindowText(strValue);
	strValue.Format(_T("%.2f"), TdstD.dLastTransfer);
	m_editLastTransferStage.SetWindowText(strValue);
	strValue.Format(_T("%.2f"), TdstD.dLastService);
	m_editLastServiceStage.SetWindowText(strValue);

	m_checkLastAutoTransfer.SetCheck(TdstD.bLastAuto ? BST_CHECKED : BST_UNCHECKED);

	// Dead Load Balancing Ratio
	strValue.Format(_T("%.2f"), TdstD.dDLBalancingRatioMax);
	m_editMaxRatio.SetWindowText(strValue);
	strValue.Format(_T("%.2f"), TdstD.dDLBalancingRatioMin);
	m_editMinRatio.SetWindowText(strValue);

	// Pre-Compression Level
	strValue.Format(_T("%.2f"), TdstD.dPreCompLevelMax);
	m_editMaxPreComp.SetWindowText(strValue);
	strValue.Format(_T("%.2f"), TdstD.dPreCompLevelMin);
	m_editMinPreComp.SetWindowText(strValue);
}

void CCMPostTensionTendonStressingDlg::InitTendonStressingList()
{
	if (m_listStressing.GetSafeHwnd())
	{
		m_listStressing.DeleteAllItems();
	}
}

void CCMPostTensionTendonStressingDlg::AddTendonStressingList(const UINT& nID, const CString& strName)
{
	if (!m_listStressing.GetSafeHwnd()) return;

	int nIndex = m_listStressing.GetItemCount();

	CString strID;
	strID.Format(_T("%d"), nID);
	nIndex = m_listStressing.InsertItem(nIndex, strID);
	m_listStressing.SetItemText(nIndex, 1, strName);
}

void CCMPostTensionTendonStressingDlg::Data2Dlg()
{
	UpdateStressingList();
	UpdateStressingData();

	UpdateData(FALSE);
}

void CCMPostTensionTendonStressingDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.strName = m_strName;
	m_Data.bApplySame = (m_bApplySame != FALSE);
	m_Data.nFirstMethod = m_nFirstMethod;
	m_Data.dFirstJacking = _tstof(m_strFirstJackingStage);
	m_Data.dFirstTransfer = _tstof(m_strFirstTransferStage);
	m_Data.dFirstService = _tstof(m_strFirstServiceStage);
	m_Data.bFirstAuto = (m_bFirstAutoTransfer != FALSE);
	m_Data.nLastMethod = m_nLastMethod;
	m_Data.dLastJacking = _tstof(m_strLastJackingStage);
	m_Data.dLastTransfer = _tstof(m_strLastTransferStage);
	m_Data.dLastService = _tstof(m_strLastServiceStage);
	m_Data.bLastAuto = (m_bLastAutoTransfer != FALSE);
	m_Data.dDLBalancingRatioMax = _tstof(m_strMaxRatio);
	m_Data.dDLBalancingRatioMin = _tstof(m_strMinRatio);
	m_Data.dPreCompLevelMax = _tstof(m_strMaxPreComp);
	m_Data.dPreCompLevelMin = _tstof(m_strMinPreComp);
}