// CMMvldItemBSDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemBSDlg.h"
#include "CMMvldSubItemBSDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\VehlDef.h"
#include "..\wg_db\VehlDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT_ASSIGNVEHL 2

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemBSDlg dialog

union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key : 31;
	} typekey;
};

CCMMvldItemBSDlg::CCMMvldItemBSDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemBSDlg::IDD, pParent)
{
	m_pDoc = NULL;
	m_bModify = FALSE;
	//{{AFX_DATA_INIT(CCMMvldItemBSDlg)
	m_nCombinationRa2 = 0;
	//}}AFX_DATA_INIT

	m_bSurface = FALSE;

	m_aLaneList.RemoveAll();
	m_aSelFlag.RemoveAll();
	m_aSelLlan.RemoveAll();

	m_bAutoOptim = FALSE;
	m_nCombOption2 = 1;
	m_bOptimRemainArea = FALSE;

	InitRcAr();
}


void CCMMvldItemBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemBSDlg)
	DDX_Control(pDX, IDC_CMD_DESCRIPTION,                   m_wndDesc);
	DDX_Control(pDX, IDC_CMD_LIST,                          m_List);
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME,                 m_wndLoadCase);
	DDX_Radio  (pDX, IDC_CMD_COMBINATION1_RADIO2,           m_nCombinationRa2);
	DDX_Control(pDX, IDC_CMD_AUTO_LIVE_CHK,                 m_chkAutoLoadComb);
	DDX_Radio  (pDX, IDC_CMD_LOADING_EFFECT_COMBINED_RDO,   m_nLoadingEffect);

	DDX_Control(pDX, IDC_ST_LOAD_CBX,                       m_cbxStandardLoadVehi);
	DDX_Control(pDX, IDC_SPECIAL_LOAD_CBX,                  m_cbxSpecialLoadVehi);

	DDX_Control(pDX, IDC_CMD_UNSEL_LIST,                    m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST,                      m_wndSelList);
	DDX_Control(pDX, IDC_CMD_STRAD_LANE_LST,                m_lstStrad);
	DDX_Control(pDX, IDC_CMD_STRAD_LANE_LST2,               m_lstStradLM1);
	DDX_Control(pDX, IDC_CMD_AREA_LANE_LST,                 m_lstRemain);
	DDX_Control(pDX, IDC_CMD_OPTIM_UNSEL_LST,               m_lstOptimUnsel);
	DDX_Control(pDX, IDC_CMD_OPTIM_REMAIN_LST,              m_lstOptimRemain);

	DDX_Check  (pDX, IDC_CMD_MVLD_OPTIM_CHK,                m_bAutoOptim);

	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_EDT,        m_edtMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_UNT,        m_untMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB,                 m_cmbOptimLane);
	DDX_Control(pDX, IDC_CMD_MVLD_NUM_LOADED_LANE_EDT,      m_edtNumLoadLanes);
	DDX_Radio  (pDX, IDC_CMD_RADIO_COMBINED2,               m_nCombOption2);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB,             m_cmbSelVehl);  
	DDX_Control(pDX, IDC_CMD_ASSIGN_VEHL_LIST,              m_AssignVehlList);
	DDX_Control(pDX, IDC_CMD_MVLD_SCALE_FACTOR_EDT,         m_edtScaleFactorOptim);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB2,                m_cmbOptimLane2);
	DDX_Control(pDX, IDC_CMD_MVLD_NUM_LOADED_LANE_EDT2,     m_edtNumLoadLanes2);

	DDX_Control(pDX, IDC_CMD_MVLD_SACLE_FACT1_EDT2, m_edtTrackFactor1);
	DDX_Control(pDX, IDC_CMD_MVLD_SACLE_FACT2_EDT2, m_edtTrackFactor2);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvldItemBSDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemBSDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_AUTO_LIVE_CHK, OnClickAutoLiveChk)
	ON_BN_CLICKED(IDC_STANDARD_LOAD_RDO, OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_SPECIAL_LOAD_RDO,  OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_CS454_LOAD_RDO,    OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_CS454_LOAD2_RDO,   OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_NR_GN_CIV_025_RDO, OnSelLoadModelRdo)

	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST,       OnDblclkCmdSubSelList)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST,     OnDblclkCmdSubUnselList)  
	ON_LBN_DBLCLK(IDC_CMD_STRAD_LANE_LST, OnDblclkCmdSubStradList)
	ON_LBN_DBLCLK(IDC_CMD_STRAD_LANE_LST2,OnDblclkCmdSubStradList)  // LM1
	ON_LBN_DBLCLK(IDC_CMD_AREA_LANE_LST,  OnDblclkCmdSubRemainArea) // LM1
	ON_LBN_DBLCLK(IDC_CMD_OPTIM_REMAIN_LST, OnDblclkCmdSubOptimRemainList)

	ON_BN_CLICKED(IDC_CMD_SUB_BTN_ADD,        OnCmdSubBtnAdd)
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_DEL,        OnCmdSubBtnDelete)
	ON_BN_CLICKED(IDC_CMD_SUB_STRAD_ADD_BTN,  OnCmdSubStradAddBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_STRAD_DEL_BTN,  OnCmdSubStradDelBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_STRAD_ADD_BTN2, OnCmdSubStradAddBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_STRAD_DEL_BTN2, OnCmdSubStradDelBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_REMAIN_ADD_BTN, OnCmdSubRemainAreaAddBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_REMAIN_DEL_BTN, OnCmdSubRemainAreaDelBtn)
	ON_BN_CLICKED(IDC_CMD_OPTIM_REMAIN_ADD_BTN,  OnCmdSubOptimRemainAddBtn)
	ON_BN_CLICKED(IDC_CMD_OPTIM_REMAIN_DEL_BTN,  OnCmdSubOptimRemainDelBtn)

	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK, OnCmdOptmChk)  
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_ADD_BTN, OnCmdBtnAdd_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_MOD_BTN, OnCmdBtnModify_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_DEL_BTN, OnCmdBtnDelete_AssignVehl)

// 	ON_CBN_SELCHANGE(IDC_ST_LOAD_CBX, OnSelChangeVClass)
// 	ON_CBN_SELCHANGE(IDC_SPECIAL_LOAD_CBX, OnSelChangeVClass)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemBSDlg message handlers
BOOL CCMMvldItemBSDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	if (!m_bModify) 
	{
		m_Data.Initialize();
		m_Data.dMinVehlDist = M_InitValueCurUnit(1.0, KN, M, CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	}

	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0 || m_pDoc->m_pAttrCtrl->GetCountSlanop() > 0)
	{
		POSITION pos;
		T_SLAN_K Key;
		T_SLAN_D Data;
		pos = m_pDoc->m_pAttrCtrl->GetStartSlan();
		while (pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextSlan(pos, Key, Data);
			if (Data.nLaneType == 1) continue;
			m_aLaneList.Add(Key);
		}
		m_bSurface = TRUE;
	}
	else
	{
		POSITION pos;
		T_LLAN_K Key;
		T_LLAN_D Data;
		pos = m_pDoc->m_pAttrCtrl->GetStartLlan();
		while (pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextLlan(pos, Key, Data);
			if (Data.nLaneType == 1) continue;
			m_aLaneList.Add(Key);
		}
		m_bSurface = FALSE;
	}

	InitUnit();
	InitCombo();
	SetHeaderTitle();
	SetHeaderTitle_AssignVehl();
	Data2Dlg();

#if defined(_MGEN)
	GetDlgItem(IDC_SPECIAL_LOAD_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_CS454_LOAD_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_CS454_LOAD2_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_NR_GN_CIV_025_RDO)->EnableWindow(FALSE);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldItemBSDlg::Data2Dlg()
{
	CString csVal;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLoadModelRdo, m_Data.nLoadModel);
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadio, m_Data.nDgnCombFactorType);
	m_nCombinationRa2 = m_Data.nCombinationMethod;
	m_chkAutoLoadComb.SetCheck(m_Data.bAutoLiveLoadComb ? 1 : 0);
	m_nLoadingEffect = m_Data.nLoadingEffect;

	if (m_cbxStandardLoadVehi.GetCount() > 0)
	{
		T_MVLD_TYPE_KEY TypeKey;

		TypeKey.typekey.type = m_Data.Vehicle.nVehicleType - 1;
		TypeKey.typekey.key = m_Data.Vehicle.VehicleKey;
		if (m_Data.nLoadModel == 4) TypeKey.value = 0;
		CDlgUtil::CobxSetCurSelItemData(m_cbxStandardLoadVehi, TypeKey.value);
	}
		
	if(m_cbxSpecialLoadVehi.GetCount() > 0)
		CDlgUtil::CobxSetCurSelItemData(m_cbxSpecialLoadVehi, (DWORD&)m_Data.VehiSpecialK);
// 	if(m_cbxLoadModel1.GetCount() > 0)
// 		CDlgUtil::CobxSetCurSelItemData(m_cbxLoadModel1, (DWORD&)m_Data.Vehicle.VehicleKey);
// 	if(m_cbxSpecialLoadModel1.GetCount() > 0)
// 		CDlgUtil::CobxSetCurSelItemData(m_cbxSpecialLoadModel1, (DWORD&)m_Data.VehiSpecialK);

	Data2Dlg_LaneListByVehicleIndex();

	m_bAutoOptim = m_Data.bAutoOptimize;
	m_edtMinVehlDist.SetEditUnit(m_Data.dMinVehlDist);

	m_edtTrackFactor1.SetEditUnit(m_Data.dRailTrackFactor[0]);
	m_edtTrackFactor2.SetEditUnit(m_Data.dRailTrackFactor[1]);
	//m_bOptimRemainArea = m_Data.bOptimRemainArea;

	T_LLANop_K OptimK = 0;
	if(m_Data.bAutoLiveLoadComb) OptimK = m_Data.OptimizeLane2;
	else                         OptimK = m_Data.OptimizeLane;
	int i = 0;
	for(i=0; i<m_cmbOptimLane.GetCount(); i++)
	{
		if(OptimK != m_cmbOptimLane.GetItemData(i)) continue;
		break;
	}
	m_cmbOptimLane.SetCurSel(i);

	for(i=0; i<m_cmbOptimLane2.GetCount(); i++)
	{
		if(OptimK != m_cmbOptimLane2.GetItemData(i)) continue;
		break;
	}
	m_cmbOptimLane2.SetCurSel(i);

	m_nCombOption2 = m_Data.nLoadingEffect;
	
	if (m_Data.nLoadModel == 0 || m_Data.nLoadModel == 4)
	{
		m_edtNumLoadLanes.SetEditUnit(m_Data.nNumLoadedLanes);
	}
	else
	{
		m_edtNumLoadLanes2.SetEditUnit(m_Data.nNumLoadedLanes);
	}
	
	// Default Value
	m_edtScaleFactorOptim.SetWindowText(_T("1.0"));

	MakeItemEx();  
	MakeItemEx_AssignVehl();

	UpdateData(FALSE);

	AlignControl();
	EnableDisableCtrl();  
	SetCtrlShowHide();

	
}

BOOL CCMMvldItemBSDlg::Dlg2Data()
{
	UpdateData();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadModelRdo, m_Data.nLoadModel);
	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, m_Data.nDgnCombFactorType);
	m_Data.nCombinationMethod = m_nCombinationRa2;
	m_Data.bAutoLiveLoadComb = m_chkAutoLoadComb.GetCheck() != 0;
	m_Data.nLoadingEffect = m_nLoadingEffect;

	// 이 아래부터 퍼온건데 정리 한다음에 함수로 빼자
	int nSize = m_aLaneList.GetSize();
	int nCount = 0;
	int nCountFoot = 0;
	int nCountArea = 0;
	
	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cbxStandardLoadVehi.GetItemData(m_cbxStandardLoadVehi.GetCurSel());

	m_Data.Vehicle.nVehicleType = TypeKey.typekey.type + 1;
	m_Data.Vehicle.VehicleKey = TypeKey.typekey.key;

	if(!CDlgUtil::CobxGetItemDataByCurSel(m_cbxSpecialLoadVehi,  (DWORD&)m_Data.VehiSpecialK)) m_Data.VehiSpecialK = 0;
	if(m_Data.nLoadModel==2 && !m_Data.bAutoLiveLoadComb) m_Data.VehiSpecialK = 0;// Load Model 1

	m_Data.bAutoOptimize = m_bAutoOptim;
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();

	m_Data.dRailTrackFactor[0] = m_edtTrackFactor1.GetEditValue();
	m_Data.dRailTrackFactor[1] = m_edtTrackFactor2.GetEditValue();

	m_Data.bOptimRemainArea = FALSE; //m_bOptimRemainArea;  사용안함.

	if(m_Data.bAutoLiveLoadComb)
	{
		if (m_Data.nLoadModel == 0 || m_Data.nLoadModel == 4)
		{
			m_Data.OptimizeLane2 = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
		}
		else
		{
			m_Data.OptimizeLane2 = m_cmbOptimLane2.GetItemData(m_cmbOptimLane2.GetCurSel());
		}
	}
	else
	{
		if (m_Data.nLoadModel == 0 || m_Data.nLoadModel == 4)
		{
			m_Data.OptimizeLane = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
		}
		else
		{
			m_Data.OptimizeLane = m_cmbOptimLane2.GetItemData(m_cmbOptimLane2.GetCurSel());
		}
	}
	
	if (m_Data.nLoadModel == 0 || m_Data.nLoadModel == 4)
	{
		m_Data.nNumLoadedLanes = m_edtNumLoadLanes.GetEditValue();
	}
	else
	{
		m_Data.nNumLoadedLanes = m_edtNumLoadLanes2.GetEditValue();
	}
	
	if(m_bAutoOptim) m_Data.nLoadingEffect = m_nCombOption2;

	nSize = m_aLaneList.GetSize();  
	// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
	int nMaxLane = 20;
	// 초기화
	m_Data.aSelectedLanes.RemoveAll();
	for (int i=0; i<nSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			if (nCount >= nMaxLane)
			{
				CString csMsg;
				csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
				AfxMessageBox(csMsg);
				return FALSE;
			}
			m_Data.aSelectedLanes.Add(m_aLaneList[i]);
			nCount++;
		}
	}

	T_LLAN_EURO_NEW_KEY key;
	// 초기화
	m_Data.aTwoLanes.RemoveAll();
	m_Data.aTwoLanes.SetSize(m_aSelLlan.GetSize());

	for (int i=0; i<m_aSelLlan.GetSize(); i++)
	{
		key = m_aSelLlan[i];
		m_Data.aTwoLanes[i].Lane1 = key.key.key1;
		m_Data.aTwoLanes[i].Lane2 = key.key.key2;
	}

	m_Data.aSelectedRemainingArea.RemoveAll();
	if(m_Data.nLoadModel == 2) // All Model1
	{
		for (int i=0; i<nSize; i++)
		{
			if (m_aAreaFlag[i]) 
			{
				if (nCountArea >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX_AREA), nMaxLane); 
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aSelectedRemainingArea.Add(m_aLaneList[i]);
				nCountArea++;
			}
		}
	}

	return TRUE;
}

BOOL CCMMvldItemBSDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	
	BOOL bSuccess;
	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvldbs(m_csOldName, m_Data);
	else bSuccess = m_pDoc->m_pDataCtrl->AddMvldbs(m_Data);
	if (!bSuccess) return FALSE;
	return TRUE;
}

#define COLCOUNT 12
/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
void CCMMvldItemBSDlg::SetHeaderTitle()
{
	CString aTitle[] = { _T("Vehicle"), _T("Scale"), _T("Line1"), _T("Line2"), _T("Line3"), _T("Line4"), _T("Line5"),
											 _T("Line6"), _T("Line7"), _T("Line8"), _T("Line9"), _T("Line10")}; 
	aTitle[0] = _LS(IDS_CMD_MVLD_BS_VEHICLE);
	aTitle[1] = _LS(IDS_CMD_MVLD_BS_SCALE);
	for (int n1 = 1; n1 <= 10; n1++)
		aTitle[1+n1].Format(_LS(IDS_CMD_MVLD_BS_LINE_d), n1);

	int nColWidth[COLCOUNT] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	CString title;
	LV_COLUMN lvcolumn;
	for(int i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemBSDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.arSubLoadData.GetSize();
	if(nItemCount == 0) return;
	for(int nCount = 0 ; nCount < nItemCount ; nCount++)
		InsertItem(nCount, m_Data.arSubLoadData[nCount]);
}

BOOL CCMMvldItemBSDlg::InsertItem(int nIndex, T_MVLDbs_BASE &Data)
{
	int nItem;
	CString str;
	LVITEM lvitem;

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		if (i==0) nItem = m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	return TRUE;
}

BOOL CCMMvldItemBSDlg::DeleteItem(int nIndex, T_MVLDbs_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_List.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemBSDlg::ModifyItem(int nIndex, T_MVLDbs_BASE &Data)
{
	int nItem = nIndex;
	if (nItem == -1) return TRUE;
	
	CString str;
	for(int i = 0; i < COLCOUNT; i++)
	{
		str = DataToStr(i, Data);
		m_List.SetItemText(nItem, i, str);
	}
	return TRUE;
}

CString CCMMvldItemBSDlg::GetMvhlName(UINT nMvhlKey)
{
	T_MVHL_D mvhl;
	m_pDoc->m_pAttrCtrl->GetMvhl(nMvhlKey, mvhl);
	return mvhl.VehicleLoadName;
}

CString CCMMvldItemBSDlg::DataToStr(int i, T_MVLDbs_BASE &Data)
{
	CString str;
	if (i == 0) str = GetMvhlName(Data.VehicleKey);
	else if (i==1) str.Format(_T("%g"), Data.dScaleFactor);
	else if (i>=2 && i <= 11)
	{
		if(Data.aSelectedLanes.GetSize() > i-2)
		{
			if (Data.aSelectedLanes[i-2] == 0) str = _T("");
			else str = GetLaneName(Data.aSelectedLanes[i-2]); 
		}
		else str = _T("");
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);
	return str;
}

CString CCMMvldItemBSDlg::GetLaneName(UINT nLaneKey)
{
	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0)
	{
		T_SLAN_D slan;
		m_pDoc->m_pAttrCtrl->GetSlan(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_LLAN_D llan;
		m_pDoc->m_pAttrCtrl->GetLlan(nLaneKey, llan);
		return llan.LineLaneName;
	}
}

void CCMMvldItemBSDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldItemBSDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();	
}

void CCMMvldItemBSDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMMvldSubItemBSDlg dlg(this);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemBSDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
 
	CCMMvldSubItemBSDlg dlg(this);
	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

BOOL CCMMvldItemBSDlg::OnApplySubDlg(BOOL bModify, T_MVLDbs_BASE& data, int nPos)
{
	if (!bModify)
	{
		int nCount = m_Data.arSubLoadData.GetSize();
		m_Data.arSubLoadData.Add(data);
		InsertItem(nCount, data);
	}
	else
	{
		m_Data.arSubLoadData.SetAt(nPos, data);
		ModifyItem(nPos, data);
	}
	EnableDisableCtrl();

	return TRUE;
}

void CCMMvldItemBSDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
	m_Data.arSubLoadData.RemoveAt(iItem);
	m_List.DeleteItem(iItem);

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

	EnableDisableCtrl();
}

void CCMMvldItemBSDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();	
	*pResult = 0;
}

void CCMMvldItemBSDlg::EnableDisableCtrl()
{
	BOOL bExist_HA_HB_AUTO = FALSE;
	BOOL bExist_OTHER = FALSE;  // HA & HB (BS5400), HA & HB(Auto)와 Pedestrian 제외한 것 있을 때

	T_MVHL_D MvhlD;

	int nLoadModel = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadModelRdo, nLoadModel); // nLoadModel : 0=Standard Load(BD 37/01, BS 5400), 1=Special Load(BD 86/11),  2=All Model1  3=All Model2

	if(nLoadModel == 0)
	{
		int nSize = 0;
		UINT VehicleKey = 0;


		if(m_bAutoOptim) nSize = m_Data.aOptimize.GetSize();
		else             nSize = m_Data.arSubLoadData.GetSize(); 
		
		for (int i = 0; i < nSize; i++)
		{
			if(m_bAutoOptim) VehicleKey = m_Data.aOptimize[i].VehicleK;
			else             VehicleKey = m_Data.arSubLoadData[i].VehicleKey; 

			if (!m_pDoc->m_pAttrCtrl->GetMvhl(VehicleKey, MvhlD)) continue;;
			if (MvhlD.bStandard)
			{
				MvhlD.VehicleTypeName.MakeUpper();
				if (MvhlD.VehicleTypeName == _T("HA & HB (BS5400)") || MvhlD.VehicleTypeName == _T("HA & HB(AUTO)"))
					bExist_HA_HB_AUTO = TRUE;
				else if (MvhlD.VehicleTypeName != _T("PEDESTRIAN"))
					bExist_OTHER = TRUE;
			}
			else  // User
			{
				if (MvhlD.nSubType == 0 || MvhlD.nSubType == 4) bExist_HA_HB_AUTO = TRUE;
				else if (MvhlD.nSubType != 2) bExist_OTHER = TRUE;
			}
		}
	}
	else if (nLoadModel == 4)
	{
		bExist_HA_HB_AUTO = FALSE;
		bExist_OTHER = TRUE;
	}
	else
	{
		// Default Setting
		bExist_HA_HB_AUTO = FALSE;
		bExist_OTHER = FALSE;
	}  

	ASSERT(!(bExist_HA_HB_AUTO && bExist_OTHER));
	if (bExist_HA_HB_AUTO)
	{
		m_chkAutoLoadComb.SetCheck(1);
		m_chkAutoLoadComb.EnableWindow(FALSE);
	}
	else if (bExist_OTHER)
	{
		m_chkAutoLoadComb.SetCheck(0);
		m_chkAutoLoadComb.EnableWindow(FALSE);
	}
	else
	{
		m_chkAutoLoadComb.EnableWindow(TRUE);
	}


	BOOL bSpecialLoad = (nLoadModel == 1 || nLoadModel == 3)? TRUE : FALSE;
	m_chkAutoLoadComb.EnableWindow((!bExist_HA_HB_AUTO && !bExist_OTHER) || bSpecialLoad || nLoadModel == 4);

	int nCheck = 0;
	//CDlgUtil::CtrlEnableDisable(this, m_aCtrlLM1Hide, TRUE);
	nCheck = m_chkAutoLoadComb.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlRadio, nCheck != 0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCombRdo, nCheck != 0);

	if(nLoadModel == 2) // All Model 1
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLM1SpecialHide, nCheck != 0);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLM1SpecialHideStradd, nCheck != 0);
		
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLM1SpecialHide, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLM1SpecialHideStradd, TRUE);
	}


	BOOL bBS3701 = FALSE;
	//if(MvhlD.nSubType == 1 || MvhlD.nSubType == 2) bBS3701 = TRUE;
	if(MvhlD.nSubType == 1 || MvhlD.nSubType == 2 || MvhlD.nSubType == 3 || MvhlD.nSubType == 4 || MvhlD.nSubType == 5 || MvhlD.nSubType == 6) bBS3701 = TRUE;

	if((nCheck == 0 && bBS3701) || nLoadModel == 4)
	{
		GetDlgItem(IDC_CMD_LOADING_EFFECT_COMBINED_RDO   )->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_LOADING_EFFECT_INDEPENDENT_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_RADIO_COMBINED2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_RADIO_INDEPENDENT2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_LOADING_EFFECT_COMBINED_RDO   )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_LOADING_EFFECT_INDEPENDENT_RDO)->EnableWindow(FALSE); 
		GetDlgItem(IDC_CMD_RADIO_COMBINED2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_RADIO_INDEPENDENT2)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_CMD_MVLD_OPTIM_CHK)->EnableWindow((nLoadModel != 4));
	GetDlgItem(IDC_NR_GN_CIV_025_RDO)->EnableWindow(!m_bAutoOptim);

	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(!m_bModify);

#if defined(_MGEN)
	GetDlgItem(IDC_CMD_MVLD_OPTIM_CHK)->EnableWindow(FALSE);
#endif

	//OnSelChangeVClass();
}

BOOL CCMMvldItemBSDlg::IsAutoLiveLoadComb()
{
	if (m_chkAutoLoadComb.GetCheck() != 0) return TRUE;

	return FALSE;
}

BOOL CCMMvldItemBSDlg::IsNetworkRail()
{
	int nSelLoadModelRdo = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadModelRdo, nSelLoadModelRdo);
	return (nSelLoadModelRdo == 4);
}

void CCMMvldItemBSDlg::OnClickAutoLiveChk()
{
	int nSelLoadModelRdo = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadModelRdo, nSelLoadModelRdo);
	m_Data.nLoadModel = nSelLoadModelRdo;  // InitCombo()에서 현재 CheckBox를 봐야 할 때도 있고, Data를 봐야 할 때도 있어서 실시간으로 Dlg2Data를 하도록 처리 했음

	m_Data.bAutoLiveLoadComb = m_chkAutoLoadComb.GetCheck() != 0; // InitCombo()에서 현재 CheckBox를 봐야 할 때도 있고, Data를 봐야 할 때도 있어서 실시간으로 Dlg2Data를 하도록 처리 했음
	SetAutoLiveState(nSelLoadModelRdo);
	InitCombo();
	LoadModel_LaneListByVehicleIndex();
	EnableDisableCtrl();
}

void CCMMvldItemBSDlg::InitRcAr()
{
	// Rdo
	m_aCtrlLoadModelRdo.RemoveAll();
	m_aCtrlLoadModelRdo.Add(IDC_STANDARD_LOAD_RDO);
	m_aCtrlLoadModelRdo.Add(IDC_SPECIAL_LOAD_RDO);
	m_aCtrlLoadModelRdo.Add(IDC_CS454_LOAD_RDO);
	m_aCtrlLoadModelRdo.Add(IDC_CS454_LOAD2_RDO);
	m_aCtrlLoadModelRdo.Add(IDC_NR_GN_CIV_025_RDO);

	m_aCtrlRadio.RemoveAll();
	m_aCtrlRadio.Add(IDC_CMD_ULTIMATE_RADIO);
	m_aCtrlRadio.Add(IDC_CMD_SERVICE_RADIO);

	m_aCtrlCombRdo.RemoveAll();
	m_aCtrlCombRdo.Add(IDC_CMD_COMBINATION1_RADIO2);
	m_aCtrlCombRdo.Add(IDC_CMD_COMBINATION23_RADIO2);

// 	m_aCtrlLM1Hide.RemoveAll();
// 	m_aCtrlLM1Hide.Add(IDC_CMD_AUTO_LIVE_CHK);
// 	m_aCtrlLM1Hide.Add(IDC_WG_CMD_STATIC2);
// 	m_aCtrlLM1Hide.Add(IDC_CMD_ULTIMATE_RADIO);
// 	m_aCtrlLM1Hide.Add(IDC_CMD_SERVICE_RADIO);
// 	m_aCtrlLM1Hide.Add(IDC_WG_CMD_STATIC3);
// 	m_aCtrlLM1Hide.Add(IDC_CMD_COMBINATION1_RADIO2);
// 	m_aCtrlLM1Hide.Add(IDC_CMD_COMBINATION23_RADIO2);

	m_aCtrlLM1SpecialHide.RemoveAll();
	m_aCtrlLM1SpecialHide.Add(IDC_SPECIAL_LOAD_TXT);
	m_aCtrlLM1SpecialHide.Add(IDC_SPECIAL_LOAD_CBX);

	m_aCtrlLM1SpecialHideStradd.RemoveAll();
	m_aCtrlLM1SpecialHideStradd.Add(IDC_CMD_STRAD_LANE_STC2);
	m_aCtrlLM1SpecialHideStradd.Add(IDC_CMD_STRAD_LANE_LST2);
	m_aCtrlLM1SpecialHideStradd.Add(IDC_CMD_SUB_STRAD_ADD_BTN2);
	m_aCtrlLM1SpecialHideStradd.Add(IDC_CMD_SUB_STRAD_DEL_BTN2);


	// Hide/Enable용
	m_aCtrlStandard.RemoveAll();
	m_aCtrlStandard.Add(IDC_SUB_LOAD_CASES_GRP);
	m_aCtrlStandard.Add(IDC_CMD_LOADING_EFFECT_GRP);
	m_aCtrlStandard.Add(IDC_CMD_LOADING_EFFECT_COMBINED_RDO);
	m_aCtrlStandard.Add(IDC_CMD_LOADING_EFFECT_INDEPENDENT_RDO);
	m_aCtrlStandard.Add(IDC_CMD_LIST);
	m_aCtrlStandard.Add(IDC_CMD_BTN_ADD);
	m_aCtrlStandard.Add(IDC_CMD_BTN_MODIFY);
	m_aCtrlStandard.Add(IDC_CMD_BTN_DELETE);

	m_aCtrlLoadCase.RemoveAll();
	m_aCtrlLoadCase.Add(IDC_CMD_LOAD_CASE_TXT);
	m_aCtrlLoadCase.Add(IDC_ST_LOAD_TXT);
	m_aCtrlLoadCase.Add(IDC_ST_LOAD_CBX);
	m_aCtrlLoadCase.Add(IDC_SPECIAL_LOAD_TXT);
	m_aCtrlLoadCase.Add(IDC_SPECIAL_LOAD_CBX);

//	m_aCtrlLoadLM1Case.RemoveAll();
//	m_aCtrlLoadLM1Case.Add(IDC_CMD_LOAD_LM1_CASE_TXT);
//	m_aCtrlLoadLM1Case.Add(IDC_ST_LOAD_LM1_TXT);
//	m_aCtrlLoadLM1Case.Add(IDC_ST_LOAD_LM1_CBX);
//	m_aCtrlLoadLM1Case.Add(IDC_SPECIAL_LOAD_LM1_TXT);
//	m_aCtrlLoadLM1Case.Add(IDC_SPECIAL_LOAD_LM1_CBX);

	m_aCtrlAreaLane.RemoveAll();
	m_aCtrlAreaLane.Add(IDC_CMD_STRAD_LANE_STC2);
	m_aCtrlAreaLane.Add(IDC_CMD_STRAD_LANE_LST2);
	m_aCtrlAreaLane.Add(IDC_CMD_SUB_STRAD_ADD_BTN2);
	m_aCtrlAreaLane.Add(IDC_CMD_SUB_STRAD_DEL_BTN2);
	m_aCtrlAreaLane.Add(IDC_CMD_AREA_LANE_STC);
	m_aCtrlAreaLane.Add(IDC_CMD_AREA_LANE_LST);
	m_aCtrlAreaLane.Add(IDC_CMD_SUB_REMAIN_ADD_BTN);
	m_aCtrlAreaLane.Add(IDC_CMD_SUB_REMAIN_DEL_BTN);
 	
	m_aCtrlSpecial.RemoveAll();
	m_aCtrlSpecial.Add(IDC_CMD_LANES_GRUP);
	m_aCtrlSpecial.Add(IDC_CMD_LIST_TXT);
	m_aCtrlSpecial.Add(IDC_CMD_SELECTED_TXT);
	m_aCtrlSpecial.Add(IDC_CMD_STRAD_LANE_STC);
	m_aCtrlSpecial.Add(IDC_CMD_UNSEL_LIST);
	m_aCtrlSpecial.Add(IDC_CMD_SUB_BTN_ADD);
	m_aCtrlSpecial.Add(IDC_CMD_SUB_BTN_DEL);
	m_aCtrlSpecial.Add(IDC_CMD_SEL_LIST);
	m_aCtrlSpecial.Add(IDC_CMD_SUB_STRAD_ADD_BTN);
	m_aCtrlSpecial.Add(IDC_CMD_SUB_STRAD_DEL_BTN);
	m_aCtrlSpecial.Add(IDC_CMD_STRAD_LANE_LST);  
	m_aCtrlSpecial.Append(m_aCtrlAreaLane);

	m_aCtrlSpecialSub.RemoveAll();
	m_aCtrlSpecialSub.Add(IDC_CMD_STRAD_LANE_STC);
	m_aCtrlSpecialSub.Add(IDC_CMD_STRAD_LANE_LST);
	m_aCtrlSpecialSub.Add(IDC_CMD_SUB_STRAD_ADD_BTN);
	m_aCtrlSpecialSub.Add(IDC_CMD_SUB_STRAD_DEL_BTN);


	m_aOKCancelApply.RemoveAll();
	m_aOKCancelApply.Add(IDOK);
	m_aOKCancelApply.Add(IDCANCEL);
	m_aOKCancelApply.Add(IDC_CMD_APPLY);

	m_aMinDist.RemoveAll();
	m_aMinDist.Add(IDC_CMD_MVLD_OPTIM_GRP);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_STC);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_EDT);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_UNT);

	m_aOptimCtrl.RemoveAll();
	m_aOptimCtrl.Add(IDC_CMD_MVLD_LCASE_GRP);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_LANE_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_LANE_CMB);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_EDT);
	m_aOptimCtrl.Add(IDC_CMD_LOAD_EFFECT_STC2);
	m_aOptimCtrl.Add(IDC_CMD_RADIO_COMBINED2);
	m_aOptimCtrl.Add(IDC_CMD_RADIO_INDEPENDENT2);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_ASSIGN_VEHL_GRP);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_SEL_VEHL_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_SEL_VEHL_CMB);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_SCALE_FACTOR_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_SCALE_FACTOR_EDT);
	m_aOptimCtrl.Add(IDC_CMD_ASSIGN_VEHL_LIST);
	m_aOptimCtrl.Add(IDC_CMD_ASSIGN_VEHL_ADD_BTN);
	m_aOptimCtrl.Add(IDC_CMD_ASSIGN_VEHL_MOD_BTN);
	m_aOptimCtrl.Add(IDC_CMD_ASSIGN_VEHL_DEL_BTN);

	m_aOptimAssignLaneSub.RemoveAll();
	m_aOptimAssignLaneSub.Add(IDC_CMD_OPTIM_UNSEL_STC);
	m_aOptimAssignLaneSub.Add(IDC_CMD_OPTIM_UNSEL_LST);
	m_aOptimAssignLaneSub.Add(IDC_CMD_OPTIM_REMAIN_ADD_BTN);
	m_aOptimAssignLaneSub.Add(IDC_CMD_OPTIM_REMAIN_DEL_BTN);
	m_aOptimAssignLaneSub.Add(IDC_CMD_OPTIM_REMAIN_STC);
	m_aOptimAssignLaneSub.Add(IDC_CMD_OPTIM_REMAIN_LST);

	m_aOptimAssignLane.RemoveAll();
	m_aOptimAssignLane.Add(IDC_CMD_LANES_GRUP2);
	m_aOptimAssignLane.Add(IDC_CMD_MVLD_LANE_STC2);
	m_aOptimAssignLane.Add(IDC_CMD_MVLD_LANE_CMB2);
	m_aOptimAssignLane.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_STC2);
	m_aOptimAssignLane.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_EDT2);
	m_aOptimAssignLane.Append(m_aOptimAssignLaneSub);


	m_m_aCtrNetworkRailHide.RemoveAll();
	m_m_aCtrNetworkRailHide.Add(IDC_CMD_AUTO_LIVE_CHK);
	m_m_aCtrNetworkRailHide.Add(IDC_WG_CMD_STATIC2);
	m_m_aCtrNetworkRailHide.Add(IDC_CMD_ULTIMATE_RADIO);
	m_m_aCtrNetworkRailHide.Add(IDC_CMD_SERVICE_RADIO);
	m_m_aCtrNetworkRailHide.Add(IDC_WG_CMD_STATIC3);
	m_m_aCtrNetworkRailHide.Add(IDC_CMD_COMBINATION1_RADIO2);
	m_m_aCtrNetworkRailHide.Add(IDC_CMD_COMBINATION23_RADIO2);

	m_m_aCtrTrackFactor.RemoveAll();
	m_m_aCtrTrackFactor.Add(IDC_CMD_TRACK_FACTOR_GRP);
	m_m_aCtrTrackFactor.Add(IDC_CMD_MVLD_NUM_LANE_STC);
	m_m_aCtrTrackFactor.Add(IDC_CMD_MVLD_SACLE_FACT_STC);
	m_m_aCtrTrackFactor.Add(IDC_CMD_MVLD_NUM_LANE1_STC);
	m_m_aCtrTrackFactor.Add(IDC_CMD_MVLD_NUM_LANE2_STC);
	m_m_aCtrTrackFactor.Add(IDC_CMD_MVLD_SACLE_FACT1_EDT2);
	m_m_aCtrTrackFactor.Add(IDC_CMD_MVLD_SACLE_FACT2_EDT2);
	//m_aOptimAssignLane.Add(IDC_CMD_REMAIN_AREA_CHK);

}

void CCMMvldItemBSDlg::OnSelLoadModelRdo()
{
	m_Data.arSubLoadData.RemoveAll();
	MakeItemEx();

	int nSelLoadModelRdo = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadModelRdo, nSelLoadModelRdo);
	m_Data.nLoadModel = nSelLoadModelRdo;  // InitCombo()에서 현재 CheckBox를 봐야 할 때도 있고, Data를 봐야 할 때도 있어서 실시간으로 Dlg2Data를 하도록 처리 했음
	SetAutoLiveState(nSelLoadModelRdo);
	InitCombo();

	LoadModel_LaneListByVehicleIndex();

	AlignControl();
	SetCtrlShowHide();
	EnableDisableCtrl();

	RedrawWindow();
}

void CCMMvldItemBSDlg::AlignControlDynamic()
{
	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;

	int nSelLoadModelRdo;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadModelRdo, nSelLoadModelRdo);

	if(!m_bAutoOptim)
	{
		if (nSelLoadModelRdo == 0 || nSelLoadModelRdo == 4)
		{
			GetDlgItem(IDC_SUB_LOAD_CASES_GRP)->GetWindowRect(rTo);
		}
		else
		{
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rTo);
		}
	}
	else
	{
		if (nSelLoadModelRdo == 0 || nSelLoadModelRdo == 4)
		{
			GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rTo);
		}
		else
		{
			GetDlgItem(IDC_CMD_LANES_GRUP2)->GetWindowRect(rTo);
		}
	}

	GetDlgItem(IDOK)->GetWindowRect(rMove);
	nDistY = rTo.bottom-rMove.top+ globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aOKCancelApply, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right = rTo.right + globalUtils.ScaleByDPI(8);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMMvldItemBSDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;

	int nSelLoadModelRdo;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadModelRdo, nSelLoadModelRdo);

	if(m_bAutoOptim)
	{
		if(nSelLoadModelRdo == 0)
		{
			GetDlgItem(IDC_CMD_LOAD_CASE_TXT )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.top-rMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);

			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.bottom-rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimCtrl, nDistX, nDistY);
		}
		else if (nSelLoadModelRdo == 4)
		{
			GetDlgItem(IDC_SEL_LOAD_MODEL_TXT)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_TRACK_FACTOR_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left - rMove.left;
			nDistY = rTo.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_m_aCtrTrackFactor, nDistX, nDistY);

			GetDlgItem(IDC_CMD_TRACK_FACTOR_GRP)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left - rMove.left;
			nDistY = rTo.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);

			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left - rMove.left;
			nDistY = rTo.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimCtrl, nDistX, nDistY);
		}
		else
		{
			// LM1 Load case
// 			GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rTo);
// 			GetDlgItem(IDC_CMD_LOAD_LM1_CASE_TXT)->GetWindowRect(rMove);
// 			nDistX = rTo.left-rMove.left;
// 			nDistY = rTo.top-rMove.top;
// 			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlLoadLM1Case, nDistX, nDistY);
// 
// 			if(nSelLoadModelRdo == 2) // All Model 1
// 				GetDlgItem(IDC_CMD_LOAD_LM1_CASE_TXT )->GetWindowRect(rTo);
// 			else
// 				GetDlgItem(IDC_CMD_LOAD_CASE_TXT )->GetWindowRect(rTo);

			GetDlgItem(IDC_CMD_LOAD_CASE_TXT )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.bottom-rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);

			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_LANES_GRUP2   )->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.bottom-rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimAssignLane, nDistX, nDistY);

			if(nSelLoadModelRdo == 2) GetDlgItem(IDC_CMD_OPTIM_REMAIN_LST          )->GetWindowRect(rTo);  // All Model 1
			else                      GetDlgItem(IDC_CMD_MVLD_NUM_LOADED_LANE_EDT2 )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_LANES_GRUP2 )->GetWindowRect(rMove);
			double dHeight = rTo.bottom - rMove.top + globalUtils.ScaleByDPI(10);

			double dWidth  = rMove.Width();
			GetDlgItem(IDC_CMD_LANES_GRUP2)->SetWindowPos(NULL, 0/*rMove.left*/, 0/*rMove.top*/, dWidth/*rMove.Width()*/, dHeight, SWP_NOZORDER | SWP_NOMOVE);
		}
	}
	else
	{
		if (nSelLoadModelRdo == 4)
		{
			GetDlgItem(IDC_SEL_LOAD_MODEL_TXT)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_TRACK_FACTOR_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left - rMove.left;
			nDistY = rTo.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_m_aCtrTrackFactor, nDistX, nDistY);

			GetDlgItem(IDC_CMD_TRACK_FACTOR_GRP)->GetWindowRect(rTo);
			GetDlgItem(IDC_SUB_LOAD_CASES_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left - rMove.left;
			nDistY = rTo.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlStandard, nDistX, nDistY);
		}
		else
		{
			GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rTo);
			GetDlgItem(IDC_SUB_LOAD_CASES_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.top-rMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlStandard, nDistX, nDistY);

	// 		// LM1 Load case
	// 		GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rTo);
	// 		GetDlgItem(IDC_CMD_LOAD_LM1_CASE_TXT)->GetWindowRect(rMove);
	// 		nDistX = rTo.left-rMove.left;
	// 		nDistY = rTo.top-rMove.top;
	// 		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlLoadLM1Case, nDistX, nDistY);

			// m_aCtrlSpecial 이동
	// 		// List Control 이동하면서 잔상이 남아서 여기서는 강제로 HIde 시킴 
	// 		CDlgUtil::CtrlShowHide(this, m_aCtrlSpecial, FALSE); 
	// 		if(nSelLoadModelRdo == 2) // All Model 1
	// 			GetDlgItem(IDC_CMD_LOAD_LM1_CASE_TXT)->GetWindowRect(rTo);
	// 		else
	// 			GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rTo);

			GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_LANES_GRUP   )->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.bottom-rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlSpecial, nDistX, nDistY);

			// Remaining Area + straddling(ALL ML1) lane List 이동
			GetDlgItem(IDC_CMD_STRAD_LANE_LST)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_STRAD_LANE_LST2)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.top-rMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlAreaLane, nDistX, nDistY);
		}

	} 

	AlignControlDynamic();

	//SetAutoLiveState(nSelLoadModelRdo);
}

void CCMMvldItemBSDlg::SetAutoLiveState(int nLoadModel)
{
	return;

// 	// All Modle 1일경우만 고려
// 	if(nLoadModel != 2) return;
// 
// 	m_chkAutoLoadComb.SetCheck(FALSE);
// 	m_Data.bAutoLiveLoadComb = FALSE; // 실시간 동작 필요
}

void CCMMvldItemBSDlg::SetCtrlShowHide()
{
	int nSelLoadModelRdo;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadModelRdo, nSelLoadModelRdo);
	
	BOOL bSpecialLoad = (nSelLoadModelRdo == 1 || nSelLoadModelRdo == 2 || nSelLoadModelRdo == 3/*2*/)? TRUE : FALSE;
	BOOL bLM1         = (nSelLoadModelRdo == 2)? TRUE : FALSE;
	BOOL bNetworkRail = (nSelLoadModelRdo == 4)? TRUE : FALSE;

	CDlgUtil::CtrlShowHide(this, m_aCtrlStandard, !m_bAutoOptim && (nSelLoadModelRdo == 0 || bNetworkRail));
	CDlgUtil::CtrlShowHide(this, m_aCtrlLoadCase, bSpecialLoad);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSpecial,  !m_bAutoOptim && (bSpecialLoad || bLM1));
	CDlgUtil::CtrlShowHide(this, m_aMinDist,         m_bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aOptimCtrl,       m_bAutoOptim && (nSelLoadModelRdo == 0 || bNetworkRail));
	CDlgUtil::CtrlShowHide(this, m_aOptimAssignLane, m_bAutoOptim && (bSpecialLoad || bLM1));
	CDlgUtil::CtrlShowHide(this, m_m_aCtrNetworkRailHide, !bNetworkRail);
	CDlgUtil::CtrlShowHide(this, m_m_aCtrTrackFactor, bNetworkRail);
	if(m_bAutoOptim)
	{
		//CDlgUtil::CtrlShowHide(this, m_aCtrlLoadLM1Case, FALSE/*bLM1*/);
		CDlgUtil::CtrlShowHide(this, m_aCtrlAreaLane,    FALSE);
 		if(nSelLoadModelRdo==0 || bNetworkRail)
			CDlgUtil::CtrlShowHide(this, m_aOptimAssignLaneSub, FALSE); //GetDlgItem(IDC_CMD_REMAIN_AREA_CHK)->ShowWindow(FALSE);
 		else
 			CDlgUtil::CtrlShowHide(this, m_aOptimAssignLaneSub, bLM1); //GetDlgItem(IDC_CMD_REMAIN_AREA_CHK)->ShowWindow(bLM1);
	}
	else
	{
        CDlgUtil::CtrlShowHide(this, m_aOptimAssignLaneSub, FALSE);   //GetDlgItem(IDC_CMD_REMAIN_AREA_CHK)->ShowWindow(FALSE);

		//CDlgUtil::CtrlShowHide(this, m_aCtrlLoadLM1Case, FALSE/*bLM1*/);
		CDlgUtil::CtrlShowHide(this, m_aCtrlAreaLane,    bLM1);
		if(nSelLoadModelRdo==0 || bNetworkRail)
			CDlgUtil::CtrlShowHide(this, m_aCtrlSpecialSub, FALSE);
		else 
			CDlgUtil::CtrlShowHide(this, m_aCtrlSpecialSub, !bLM1);

	}
}

void CCMMvldItemBSDlg::InitUnit()
{
	m_edtMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_untMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_edtNumLoadLanes .SetUnitType(D_UNITSYS_NONE);
	m_edtNumLoadLanes2.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactorOptim.SetUnitType(D_UNITSYS_NONE);
	m_edtTrackFactor1.SetUnitType(D_UNITSYS_NONE);
	m_edtTrackFactor2.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvldItemBSDlg::InitCombo()
{  
	m_cbxStandardLoadVehi.ResetContent();
	m_cbxSpecialLoadVehi.ResetContent();
// 	m_cbxLoadModel1.ResetContent();
// 	m_cbxSpecialLoadModel1.ResetContent();
	
	// Item 추가
	T_MVLD_TYPE_KEY TypeKey;
	BOOL bAutoLoadComb;
	
	// 조건문에서 현재 CheckBox를 봐야 할 때도 있고, Data를 봐야 할 때도 있는데.. InitCombo()함수는 Data2Dlg()보다 먼저 불려야 한다.
	// 이 문제를 해결 하기 위해 실시간으로 Dlg2Data를 하도록 처리(OnClickAutoLiveChk()등을 참고)한 다음 Data로 조건문을 거는 방식을 택함
	bAutoLoadComb = m_Data.bAutoLiveLoadComb; 

	int nSelLoadModelRdo = m_Data.nLoadModel;

	// None 추가하는 상황 처리
	BOOL bAddNone = FALSE;
	if(!bAutoLoadComb)
	{
		if(nSelLoadModelRdo != 2) 
		{
			bAddNone = TRUE;
		}
	}
	else if (nSelLoadModelRdo == 4)
	{
		bAddNone = TRUE;
	}

	if (bAddNone)
	{
		TypeKey.value = 0;
		CDlgUtil::CobxAddItem(m_cbxStandardLoadVehi, _T("None"), TypeKey.value);
		CDlgUtil::CobxAddItem(m_cbxSpecialLoadVehi, _T("None"), TypeKey.value);
	}

	CString strVehi;

	CArray<T_MVHC_K, T_MVHC_K> aMvhcK;
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK;
	m_pDoc->m_pAttrCtrl->GetMvhcKeyList(aMvhcK);
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVHC_D MvhcD;	
	T_MVHL_D MvhlD;	

	if (nSelLoadModelRdo == 2)  // ALL Model 1
	{
		for (int i = 0; i < aMvhcK.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetMvhc(aMvhcK[i], MvhcD)) continue;

			strVehi = _T("VC:") + MvhcD.VehicleClassName;

			TypeKey.typekey.type = 0;
			TypeKey.typekey.key = aMvhcK[i];

			CDlgUtil::CobxAddItem(m_cbxStandardLoadVehi, strVehi, TypeKey.value);
		}

		for (int i = 0; i < aMvhlK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD);

			if (CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::CS454_LM1))
			{
				strVehi = _T("VL:") + MvhlD.VehicleLoadName;

				TypeKey.typekey.type = 1;
				TypeKey.typekey.key = aMvhlK[i];

				CDlgUtil::CobxAddItem(m_cbxStandardLoadVehi, strVehi, TypeKey.value);
			}
			else if (CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::SV) ||
				     CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::SOV) ||
				     CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::CS454_HB_ONLY))
			{
				CDlgUtil::CobxAddItem(m_cbxSpecialLoadVehi, MvhlD.VehicleLoadName, aMvhlK[i]);
			}
		}
	}
	else
	{
		for (int i = 0; i < aMvhlK.GetSize(); i++)
		{
			// 조건 추가 시 CVehlDB::IsBelongToVType함수에다가 추가해서 부르는 식으로 코딩하세요.
			m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD);

			if (nSelLoadModelRdo == 3/*2*/)  // ALL Model 2
			{
				if (CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::CS454_LM2))
				{
					TypeKey.typekey.type = 1;
					TypeKey.typekey.key = aMvhlK[i];

					CDlgUtil::CobxAddItem(m_cbxStandardLoadVehi, MvhlD.VehicleLoadName, TypeKey.value);
				}
				else if (CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::SV) ||
					CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::SOV) ||
					CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::CS454_HB_ONLY))
				{
					CDlgUtil::CobxAddItem(m_cbxSpecialLoadVehi, MvhlD.VehicleLoadName, aMvhlK[i]);
				}
			}
			else if (nSelLoadModelRdo == 4)  // Railway
			{
				//if (CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::RAILWAY))
				//{
				//	TypeKey.typekey.type = 1;
				//	TypeKey.typekey.key = aMvhlK[i];

				//	CDlgUtil::CobxAddItem(m_cbxStandardLoadVehi, MvhlD.VehicleLoadName, TypeKey.value);
				//}
			}
			else
			{
				if (CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::HA_ONLY))
				{
					TypeKey.typekey.type = 1;
					TypeKey.typekey.key = aMvhlK[i];

					CDlgUtil::CobxAddItem(m_cbxStandardLoadVehi, MvhlD.VehicleLoadName, TypeKey.value);
				}
				else if (CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::SV) ||
					CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::SOV))
				{
					CDlgUtil::CobxAddItem(m_cbxSpecialLoadVehi, MvhlD.VehicleLoadName, aMvhlK[i]);
				}
			}
		}
	}	

	CString strTemp;

	if (m_cbxStandardLoadVehi.GetCount() > 0) m_cbxStandardLoadVehi.SetCurSel(0);
	if (m_cbxSpecialLoadVehi.GetCount() > 0) m_cbxSpecialLoadVehi.SetCurSel(0);
// 	if (m_cbxLoadModel1.GetCount() > 0) m_cbxLoadModel1.SetCurSel(0);
// 	if (m_cbxSpecialLoadModel1.GetCount() > 0) m_cbxSpecialLoadModel1.SetCurSel(0);


	// Loaded Lane
	m_cmbOptimLane.ResetContent();
	m_cmbOptimLane2.ResetContent();

	if(m_bSurface)
	{
		T_SLANop_D SlanD; SlanD.Initialize();
		CArray<T_SLANop_K, T_SLANop_K> aSlanK; aSlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSlanopKeyList(aSlanK);

		for(int i=0; i<aSlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSlanop(aSlanK[i], SlanD)) continue;

			if(bAutoLoadComb)
			{
				if(SlanD.bStradd)
				{
					m_cmbOptimLane .SetItemData(m_cmbOptimLane .AddString(SlanD.LaneName), aSlanK[i]);
					m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(SlanD.LaneName), aSlanK[i]);
				}
			}
			else
			{
				if(!SlanD.bStradd)
				{
					m_cmbOptimLane .SetItemData(m_cmbOptimLane .AddString(SlanD.LaneName), aSlanK[i]);
					m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(SlanD.LaneName), aSlanK[i]);
				}
			}
		}
	}
	else
	{
		T_LLANop_D LlanD; LlanD.Initialize();
		CArray<T_LLANop_K, T_LLANop_K> aLlanK; aLlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetLlanopKeyList(aLlanK);

		for(int i=0; i<aLlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetLlanop(aLlanK[i], LlanD)) continue;

			if(bAutoLoadComb)
			{
				if(LlanD.bStradd)
				{
					m_cmbOptimLane .SetItemData(m_cmbOptimLane .AddString(LlanD.LineLaneName), aLlanK[i]);
					m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(LlanD.LineLaneName), aLlanK[i]);
				}
			}
			else
			{
				if(!LlanD.bStradd)
				{
					m_cmbOptimLane .SetItemData(m_cmbOptimLane .AddString(LlanD.LineLaneName), aLlanK[i]);
					m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(LlanD.LineLaneName), aLlanK[i]);
				}
			}
		}
	}

	m_cmbOptimLane .SetCurSel(0);
	m_cmbOptimLane2.SetCurSel(0);

	// Vehicle
	m_cmbSelVehl.ResetContent();

	int nIndex = 0;

	for (int i=0; i<aMvhlK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;
		if (nSelLoadModelRdo == 4 && MvhlD.nStandardCode != D_MVHL_BS_NETWORK_RAIL_STANDARDS) continue;
		if (nSelLoadModelRdo != 4 && MvhlD.nStandardCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS) continue;
		if (MvhlD.bStandard)
		{
			CString csTypeName = MvhlD.VehicleTypeName;
			csTypeName.MakeUpper();
			if (bAutoLoadComb)  // HA && HB(Auto) or Pedestrian
			{
				if (csTypeName != _T("HA & HB (BS5400)") && csTypeName != _T("HA & HB(AUTO)") && csTypeName != _T("PEDESTRIAN")) continue;
			}
			else
			{
				if (csTypeName == _T("HA & HB (BS5400)") || csTypeName == _T("HA & HB(AUTO)")) continue;
			}

			if(csTypeName == _T("SV 80") || csTypeName == _T("SV 100") || csTypeName == _T("SV 150") || csTypeName == _T("SV 196") || csTypeName == _T("SV-Train") || csTypeName == _T("SV TT") ||
			   csTypeName == _T("SOV 250") || csTypeName == _T("SOV 350") || csTypeName == _T("SOV 450") || csTypeName == _T("SOV 600")) continue;
		}
		else  // User
		{
			if (bAutoLoadComb)  // HA && HB(Auto) or Pedestrian
			{
				if (MvhlD.nSubType != 0 && MvhlD.nSubType != 4 && MvhlD.nSubType != 5 && MvhlD.nSubType != 6) continue;
				//if (mvhl.nSubType != 0 && mvhl.nSubType != 1 && mvhl.nSubType != 2) continue;
			}
			else
			{
				if (MvhlD.nSubType == 0 || MvhlD.nSubType == 4) continue;
				//if (mvhl.nSubType == 0 || mvhl.nSubType == 1) continue;
			}
		}
		nIndex = m_cmbSelVehl.AddString(MvhlD.VehicleLoadName);
		m_cmbSelVehl.SetItemData(nIndex, aMvhlK[i]);
	}
	m_cmbSelVehl.SetCurSel(0);
}

void CCMMvldItemBSDlg::Data2Dlg_LaneListByVehicleIndex()
{
	int nAllSize = m_aLaneList.GetSize();
	int nSelSize = 0;
	for (nSelSize = 0; nSelSize < m_Data.aSelectedLanes.GetSize(); nSelSize++)
	{
		if (m_Data.aSelectedLanes[nSelSize] == 0) break;
	}

	int nAreaSize = 0;
	for (nAreaSize = 0; nAreaSize < m_Data.aSelectedRemainingArea.GetSize(); nAreaSize++)
	{
		if (m_Data.aSelectedRemainingArea[nAreaSize] == 0) break;
	}

	int nStrdSize = 0;
	for (nStrdSize = 0; nStrdSize < m_Data.aTwoLanes.GetSize(); nStrdSize++)
	{
		if (m_Data.aTwoLanes[nStrdSize].Lane1 == 0 || m_Data.aTwoLanes[nStrdSize].Lane2 == 0) break;
	}

	m_aSelFlag.SetSize(nAllSize);
	m_aAreaFlag.SetSize(nAllSize);

	CArray<BOOL, BOOL> aStrdFlag;
	aStrdFlag.SetSize(nAllSize);

	for (int i=0; i<nAllSize; i++)
	{
		BOOL bFoundInSel = FALSE;
		BOOL bFoundInFoot = FALSE;
		BOOL bFoundInArea = FALSE;
		BOOL bFoundInStrd = FALSE;

		for (int j=0; j<nSelSize; j++)
		{
			if (m_aLaneList[i] == m_Data.aSelectedLanes[j]) 
			{
				bFoundInSel = TRUE; break;
			}
		}
		for (int j=0 ; j<nStrdSize ; ++j)
		{
			if (m_aLaneList[i] == m_Data.aTwoLanes[j].Lane1 || m_aLaneList[i] == m_Data.aTwoLanes[j].Lane2)
			{
				bFoundInStrd = TRUE; break;
			}
		}
		for (int j=0 ; j<nAreaSize ; ++j)
		{
			if (m_aLaneList[i] == m_Data.aSelectedRemainingArea[j])
			{
				bFoundInArea = TRUE; break;
			}
		}

		m_aSelFlag[i] = bFoundInSel;
		aStrdFlag[i] = bFoundInStrd;
		m_aAreaFlag[i] = bFoundInArea;
	}

	int nIndex, nIndex2;

	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();
	m_lstOptimUnsel.ResetContent();
	m_lstStrad.ResetContent();
	m_lstStradLM1.ResetContent();
	m_lstRemain.ResetContent();
	m_lstOptimRemain.ResetContent();

	// Add String
	CString LaneName;
	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(LaneName);
			m_wndSelList.SetItemData(nIndex, i);

			// Optimize 에는 Select Lane 이 없다.
			nIndex = m_lstOptimUnsel.AddString(LaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);

		}
		else if (m_aAreaFlag[i]) 
		{
			nIndex = m_lstRemain.AddString(LaneName);
			m_lstRemain.SetItemData(nIndex, i);

			nIndex = m_lstOptimRemain.AddString(LaneName);
			m_lstOptimRemain.SetItemData(nIndex, i);
		}
		else if (aStrdFlag[i])
		{
			continue;
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);

			nIndex = m_lstOptimUnsel.AddString(LaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);
		}
	}

	T_LLAN_EURO_NEW_KEY key;
	for (int i=0; i<m_Data.aTwoLanes.GetSize(); i++)
	{
		if (m_Data.aTwoLanes[i].Lane1 == 0 || m_Data.aTwoLanes[i].Lane2 == 0) break;

		key.key.key1 = m_Data.aTwoLanes[i].Lane1;
		key.key.key2 = m_Data.aTwoLanes[i].Lane2;

		LaneName = GetLaneName(key.key.key1);
		LaneName += _T(" : ");
		LaneName += GetLaneName(key.key.key2);
		nIndex = m_lstStrad.AddString(LaneName);
		m_lstStrad.SetItemData(nIndex, key.keymap);
		nIndex2 = m_lstStradLM1.AddString(LaneName);
		m_lstStradLM1.SetItemData(nIndex2, key.keymap);

		m_aSelLlan.Add(key);
	}
}


void CCMMvldItemBSDlg::LoadModel_LaneListByVehicleIndex()
{

	int nAllSize = m_aLaneList.GetSize();
	int nStrdSize = 0;
	for (nStrdSize = 0; nStrdSize < m_aSelLlan.GetSize(); nStrdSize++)
	{
		if (m_aSelLlan[nStrdSize].key.key1 == 0 || m_aSelLlan[nStrdSize].key.key2 == 0) break;
	}

	CArray<BOOL, BOOL> aStrdFlag;
	aStrdFlag.SetSize(nAllSize);

	for (int i=0; i<nAllSize; i++)
	{
		BOOL bFoundInStrd = FALSE;

		for (int j=0 ; j<nStrdSize ; ++j) 
		{
			if (m_aLaneList[i] == m_aSelLlan[j].key.key1 || m_aLaneList[i] == m_aSelLlan[j].key.key2)
			{
				bFoundInStrd = TRUE; break;
			}
		}
		aStrdFlag[i] = bFoundInStrd;
	}

	int nSelLoadModelRdo = m_Data.nLoadModel;

	int nIndex, nIndex2;

	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();
	m_lstOptimUnsel.ResetContent();
	m_lstStrad.ResetContent();
	m_lstStradLM1.ResetContent();
	m_lstRemain.ResetContent();
	m_lstOptimRemain.ResetContent();

	// Add String
	CString LaneName;
	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);

		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(LaneName);
			m_wndSelList.SetItemData(nIndex, i);

			// Optimize 에는 Select Lane 이 없다.
			nIndex = m_lstOptimUnsel.AddString(LaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);

		}
		else if (m_aAreaFlag[i]) 
		{
			if(nSelLoadModelRdo == 2) // ALL Model 1
			{
				nIndex = m_lstRemain.AddString(LaneName);
				m_lstRemain.SetItemData(nIndex, i);

				nIndex = m_lstOptimRemain.AddString(LaneName);
				m_lstOptimRemain.SetItemData(nIndex, i);
			}
			else
			{
				m_aAreaFlag[i] = FALSE;
				m_aSelFlag[i]  = TRUE;

				nIndex = m_wndSelList.AddString(LaneName);
				m_wndSelList.SetItemData(nIndex, i);

				// Optimize 에는 Select Lane 이 없다.
				nIndex = m_lstOptimUnsel.AddString(LaneName);
				m_lstOptimUnsel.SetItemData(nIndex, i);
			}
		}
		else if (aStrdFlag[i])
		{
			continue;
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);

			nIndex = m_lstOptimUnsel.AddString(LaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);
		}
	}

	if(nSelLoadModelRdo == 0 || (nSelLoadModelRdo == 2 && !m_Data.bAutoLiveLoadComb))
	{
		m_aSelLlan.RemoveAll();
		return;
	}

	T_LLAN_EURO_NEW_KEY key;
	for (int i=0; i<m_aSelLlan.GetSize(); i++)
	{
		if (m_aSelLlan[i].key.key1 == 0 || m_aSelLlan[i].key.key2 == 0) break;

		key.key.key1 = m_aSelLlan[i].key.key1;
		key.key.key2 = m_aSelLlan[i].key.key2;

		LaneName = GetLaneName(key.key.key1);
		LaneName += _T(" : ");
		LaneName += GetLaneName(key.key.key2);
		nIndex = m_lstStrad.AddString(LaneName);
		m_lstStrad.SetItemData(nIndex, key.keymap);
		nIndex2 = m_lstStradLM1.AddString(LaneName);
		m_lstStradLM1.SetItemData(nIndex2, key.keymap);

		//m_aSelLlan.Add(key);
	}
}

void CCMMvldItemBSDlg::CmdSubBtnDeleteSpecial()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;

	// Error Check
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);

		int StraddK = m_aLaneList[nIndex];

		for(int j=0; j<m_aSelLlan.GetSize(); j++)
		{
			if(StraddK == m_aSelLlan[j].key.key1) { AfxMessageBox(_LS(IDS_CMD_MVLD_EURO_STRADD_LANE_ERROR)); return; }
			if(StraddK == m_aSelLlan[j].key.key2) { AfxMessageBox(_LS(IDS_CMD_MVLD_EURO_STRADD_LANE_ERROR)); return; }
		}
	}

	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}
void CCMMvldItemBSDlg::CmdSubBtnDeleteLM1()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;


	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i] && !m_aAreaFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}

}

void CCMMvldItemBSDlg::OnDblclkCmdSubUnselList() 
{
	OnCmdSubBtnAdd();
}

void CCMMvldItemBSDlg::OnDblclkCmdSubSelList()
{
	OnCmdSubRemainAreaAddBtn();
}

void CCMMvldItemBSDlg::OnDblclkCmdSubStradList() 
{
	OnCmdSubStradDelBtn();
}

void CCMMvldItemBSDlg::OnDblclkCmdSubRemainArea()
{
	OnCmdSubRemainAreaDelBtn();
}

void CCMMvldItemBSDlg::OnDblclkCmdSubOptimRemainList()
{
	OnCmdSubOptimRemainDelBtn();
}

void CCMMvldItemBSDlg::OnCmdSubBtnAdd() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndUnselList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_wndUnselList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndSelList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemBSDlg::OnCmdSubBtnDelete() 
{
	if(m_Data.nLoadModel==1 || m_Data.nLoadModel==3) 
	{
		CmdSubBtnDeleteSpecial();
	}
	else if(m_Data.nLoadModel==2)
	{
		CmdSubBtnDeleteLM1(); //CmdSubBtnDeleteLM1();
	}
	else ASSERT(0);
}

void CCMMvldItemBSDlg::OnCmdSubStradAddBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);

	if(m_Data.nLoadModel==1 || m_Data.nLoadModel==2 || m_Data.nLoadModel==3) 
	{
		int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
		if (nCount != 2)
		{
			AfxMessageBox(_LS(IDS_WG_CMD_ERR_Selected_Lane_Number2));
			return;
		}

		T_LLAN_EURO_NEW_KEY key;
		nIndex = m_wndSelList.GetItemData(aIndex[0]);
		key.key.key1 = m_aLaneList[nIndex];
		nIndex = m_wndSelList.GetItemData(aIndex[1]);
		key.key.key2 = m_aLaneList[nIndex];

		if (IsDuplicatedItem(key)) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD_ERR_EXIST_SAME_ITEM));
			return;
		}

		if (!m_bSurface)
		{
			T_LLAN_D LlanD1, LlanD2;
			m_pDoc->m_pAttrCtrl->GetLlan(key.key.key1, LlanD1);
			m_pDoc->m_pAttrCtrl->GetLlan(key.key.key2, LlanD2);

			//if (CTestEnvMgr::IsTestEnvST(_T("Test_STRADD"), _T("yes")))
			{
				BOOL bChk = TRUE;
				if(LlanD1.nLoadDistribution == 1)
				{
					if(LlanD2.nLoadDistribution != 1)        bChk = FALSE;
				}
				if(LlanD1.nLoadDistribution == 2)
				{
					if(LlanD2.nLoadDistribution != 2)        bChk = FALSE;
					if(LlanD1.ElemGroup != LlanD2.ElemGroup) bChk = FALSE;
				}
				if(!bChk)
				{
					//[오류] Heavy Load Lanes 또는 WA Rating Vehicle Group2 에 사용된 두 차선의 Vehicular Load Distribution 정보는 서로 같아야 합니다. ( Lane Name = %s, %s )
					GSaveHistoryFormatNF(_LS(IDS_DB_MOVLANE__ERROR_AS_STRADDLING_LOADDISTRNEW), LlanD1.LineLaneName, LlanD2.LineLaneName);
					return;
				}
			}
// 			else
// 			{
// 				if (LlanD1.nLoadDistribution != 2 ||
// 					LlanD2.nLoadDistribution != 2 ) // Cross Beam 아닌 경우
// 				{
// 					AfxMessageBox(_LS(IDS_WG_CMD_MVLD_BS_LANE_TYPE_ERROR));
// 					return;
// 				}
// 			}
		}

		CString csLaneName;
		csLaneName = GetLaneName(key.key.key1);
		csLaneName += _T(" : ");
		csLaneName += GetLaneName(key.key.key2);
		if(m_Data.nLoadModel==1  || m_Data.nLoadModel==2 || m_Data.nLoadModel==3) 
		{
			nIndex = m_lstStrad.AddString(csLaneName);
			m_lstStrad.SetItemData(nIndex, key.keymap);

			nIndex = m_lstStradLM1.AddString(csLaneName);
			m_lstStradLM1.SetItemData(nIndex, key.keymap);
		}
		else ASSERT(0);
		m_aSelLlan.Add(key);
	}
}

void CCMMvldItemBSDlg::OnCmdSubRemainAreaAddBtn()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);

	if(m_Data.nLoadModel==2) // All Model 1 
	{
		int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
		if (nCount == 0) return;
		for (int i = nCount-1; i >= 0; i--)
		{
			nIndex = m_wndSelList.GetItemData(aIndex[i]);
			T_LLAN_K LaneK = m_aLaneList[nIndex];
			if (IsStraddlingLane(LaneK)) 
			{
				AfxMessageBox(_LS(IDS_CMD_MVLD_BS_RAMAIANING_LANE_ERROR));
				return;
			}

			m_aAreaFlag[nIndex] = TRUE;
			m_aSelFlag[nIndex] = FALSE;
			m_wndSelList.DeleteString(aIndex[i]);
		}

		CString csLaneName;
		m_lstRemain.ResetContent();
		int nAllSize = m_aLaneList.GetSize();
		for (int i = 0; i < nAllSize; i++)
		{
			csLaneName = GetLaneName(m_aLaneList[i]);
			if (m_aAreaFlag[i]) 
			{
				nIndex = m_lstRemain.AddString(csLaneName);
				m_lstRemain.SetItemData(nIndex, i);
			}
		}
	}

}

void CCMMvldItemBSDlg::OnCmdSubStradDelBtn() 
{
	int nIndex = 0;
	if(m_Data.nLoadModel==1 || m_Data.nLoadModel==2 || m_Data.nLoadModel==3) 
	{
		UINT idata;
		int nMax = m_aSelLlan.GetSize();
		if (nMax <= 0) return;
		CArray<int, int> aIndex;
		aIndex.SetSize(nMax);

		int nCount = 0; 
		if   (m_Data.nLoadModel==2) nCount = m_lstStradLM1.GetSelItems(nMax, aIndex.GetData());
		else						nCount = m_lstStrad.GetSelItems(nMax, aIndex.GetData());

		if (nCount == 0) return;
		for (int i = nCount-1; i >= 0; i--)
		{
			if   (m_Data.nLoadModel==2) idata = m_lstStradLM1.GetItemData(aIndex[i]);
			else                        idata = m_lstStrad.GetItemData(aIndex[i]);
			if (m_lstStrad.DeleteString(aIndex[i]) == LB_ERR) continue;
			if (m_lstStradLM1.DeleteString(aIndex[i]) == LB_ERR) continue;
			int k = 0;
			for (k=0; k<nMax; k++)
				if (idata == m_aSelLlan[k].keymap) break;
			if (k == nMax) { ASSERT(0); continue; }
			m_aSelLlan.RemoveAt(k);
		}
	}

}

void CCMMvldItemBSDlg::OnCmdSubRemainAreaDelBtn()
{
	if(m_Data.nLoadModel==2) // All Model 1 
	{
		int nIndex;
		int nMax = m_aLaneList.GetSize();
		if (nMax <= 0) return;
		CArray<int, int> aIndex;
		aIndex.SetSize(nMax);

		int nCount = m_lstRemain.GetSelItems(nMax, aIndex.GetData());
		if (nCount == 0) return;
		for (int i = nCount-1; i >= 0; i--)
		{
			nIndex = m_lstRemain.GetItemData(aIndex[i]);
			m_aAreaFlag[nIndex] = FALSE;
			m_aSelFlag[nIndex] = TRUE;
			m_lstRemain.DeleteString(aIndex[i]);
		}

		CString csLaneName;
		m_wndSelList.ResetContent();
		int nAllSize = m_aLaneList.GetSize();
		for (int i = 0; i < nAllSize; i++)
		{
			csLaneName = GetLaneName(m_aLaneList[i]);
			if (m_aSelFlag[i]) 
			{
				nIndex = m_wndSelList.AddString(csLaneName);
				m_wndSelList.SetItemData(nIndex, i);
			}
		}

	}

}


void CCMMvldItemBSDlg::OnCmdSubOptimRemainAddBtn()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstOptimUnsel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstOptimUnsel.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = TRUE;
		m_lstOptimUnsel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstOptimRemain.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aAreaFlag[i]) 
		{
			nIndex = m_lstOptimRemain.AddString(csLaneName);
			m_lstOptimRemain.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemBSDlg::OnCmdSubOptimRemainDelBtn()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstOptimRemain.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstOptimRemain.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = FALSE;
		m_lstOptimRemain.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstOptimUnsel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aAreaFlag[i])
		{
			nIndex = m_lstOptimUnsel.AddString(csLaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);
		}
	}
}


BOOL CCMMvldItemBSDlg::IsDuplicatedItem(T_LLAN_EURO_NEW_KEY& key)
{
	int nNum = m_aSelLlan.GetSize();
	for (int i=0; i<nNum; i++)
	{
		if ((m_aSelLlan[i].key.key1 == key.key.key1 && m_aSelLlan[i].key.key2 == key.key.key2) ||
			(m_aSelLlan[i].key.key1 == key.key.key2 && m_aSelLlan[i].key.key2 == key.key.key1)) 
			return TRUE;
	}
	return FALSE;
}

BOOL CCMMvldItemBSDlg::IsStraddlingLane(T_LLAN_K LaneK)
{
	int nNum = m_aSelLlan.GetSize();
	for (int i=0; i<nNum; i++)
	{
		if (m_aSelLlan[i].key.key1 == LaneK || m_aSelLlan[i].key.key2 == LaneK)
			return TRUE;
	}

	return FALSE;
}

void CCMMvldItemBSDlg::OnCmdOptmChk() 
{
	LoadModel_LaneListByVehicleIndex();

	AlignControl();
	SetCtrlShowHide();
	EnableDisableCtrl();

	RedrawWindow();
}

void CCMMvldItemBSDlg::SetHeaderTitle_AssignVehl()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact)};
	int nColWidth[COLCOUNT_ASSIGNVEHL] = {140, 140};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_AssignVehlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_AssignVehlList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT_ASSIGNVEHL; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_AssignVehlList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemBSDlg::MakeItemEx_AssignVehl()
{
	m_AssignVehlList.DeleteAllItems();

	int nItemCount = m_Data.aOptimize.GetSize();
	if(nItemCount == 0) return;

	if(nItemCount)
	{
		for(int nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			InsertItem_AssignVehl(nCount, m_Data.aOptimize[nCount]);
		}
	}
}

BOOL CCMMvldItemBSDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr_AssignVehl(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			nItem = m_AssignVehlList.InsertItem(&lvitem);
		}
		else m_AssignVehlList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemBSDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_AssignVehlList.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemBSDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr_AssignVehl(i, Data);
			m_AssignVehlList.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemBSDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data)
{
	CString str;

	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if(i==0) 
	{
		if (Data.nVehicleType == 1)
		{
			m_pDoc->m_pAttrCtrl->GetMvhc(Data.VehicleK, mvhc);
			str = _T("VC:")+mvhc.VehicleClassName;
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleK, mvhl);
			str = _T("VL:")+mvhl.VehicleLoadName;
		}
	}
	else if(i==1) str.Format(_T("%g"), Data.dScaleFactor);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

void CCMMvldItemBSDlg::OnCmdBtnAdd_AssignVehl() 
{
	UpdateData(TRUE);

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVHL_K MvhlK = m_cmbSelVehl.GetItemData(m_cmbSelVehl.GetCurSel());

	Data.nVehicleType = 2;
	Data.VehicleK = MvhlK;
	Data.dScaleFactor = m_edtScaleFactorOptim.GetEditValue();

	if(Data.nVehicleType == 1)
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhc(Data.VehicleK)) return;
	}
	else
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhl(Data.VehicleK)) return;
	}

	int nCount = m_Data.aOptimize.GetSize();
	m_Data.aOptimize.Add(Data);
	InsertItem_AssignVehl(nCount, Data);

	EnableDisableCtrl();
}

void CCMMvldItemBSDlg::OnCmdBtnModify_AssignVehl() 
{
	UpdateData(TRUE);

	int iItem = m_AssignVehlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVHL_K MvhlK = m_cmbSelVehl.GetItemData(m_cmbSelVehl.GetCurSel());

	Data.nVehicleType = 2;
	Data.VehicleK = MvhlK;
	Data.dScaleFactor = m_edtScaleFactorOptim.GetEditValue();

	if(Data.nVehicleType == 1)
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhc(Data.VehicleK)) return;
	}
	else
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhl(Data.VehicleK)) return;
	}

	m_Data.aOptimize.SetAt(iItem, Data);
	ModifyItem_AssignVehl(iItem, Data);

	EnableDisableCtrl();
}

void CCMMvldItemBSDlg::OnCmdBtnDelete_AssignVehl() 
{
	int iItem = m_AssignVehlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	m_Data.aOptimize.RemoveAt(iItem);
	m_AssignVehlList.DeleteItem(iItem);

	int nCount = m_AssignVehlList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_AssignVehlList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

	EnableDisableCtrl();
}

