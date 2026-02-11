// CMMvldItemIDDlg.cpp : implementation file
//
// 2003. 09. 15    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemIDDlg.h"
#include "CMMvldSubItemIDDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\VehlDef.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_cmd2\ExportFuncCMD2.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemIDDlg dialog


CCMMvldItemIDDlg::CCMMvldItemIDDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemIDDlg::IDD, pParent)
{
	m_pDoc = NULL;
	m_bModify = FALSE;
	m_bSurface = FALSE;
	//{{AFX_DATA_INIT(CCMMvldItemIDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aMulti.RemoveAll();
	m_aMulti.Add(IDC_CMD_FACTOR_GRP);
	m_aMulti.Add(IDC_CMD_NUM_LANE_STC);
	m_aMulti.Add(IDC_CMD_SF1_STC);
	m_aMulti.Add(IDC_CMD_SF2_STC);
	m_aMulti.Add(IDC_CMD_SF3_STC);
	m_aMulti.Add(IDC_CMD_SF4_STC);
	m_aMulti.Add(IDC_CMD_SF_STC);
	m_aMulti.Add(IDC_CMD_SF1);
	m_aMulti.Add(IDC_CMD_SF2);
	m_aMulti.Add(IDC_CMD_SF3);
	m_aMulti.Add(IDC_CMD_SF4);
	
	m_aDesignLane.RemoveAll();
	m_aDesignLane.Add(IDC_CMD_LANE_NUM_STC);
	m_aDesignLane.Add(IDC_CMD_LANE_NUM_EDIT);
	//m_aDesignLane.Add(IDC_CMD_LANE_NUM_SPIN);
	
	m_aSubLoadCase.RemoveAll();
	m_aSubLoadCase.Add(IDC_CMD_SUB_LCASE_GRP);
	m_aSubLoadCase.Add(IDC_CMD_LIST);
	m_aSubLoadCase.Add(IDC_CMD_LIST2);
	m_aSubLoadCase.Add(IDC_CMD_BTN_ADD);
	m_aSubLoadCase.Add(IDC_CMD_BTN_MODIFY);
	m_aSubLoadCase.Add(IDC_CMD_BTN_DELETE);

	m_aSubLoadCase1.RemoveAll();
	m_aSubLoadCase1.Add(IDC_CMD_LIST);

	m_aSubLoadCase2.RemoveAll();
	m_aSubLoadCase2.Add(IDC_CMD_LIST2);
	
	m_aPermit.RemoveAll();
	m_aPermit.Add(IDC_CMD_PV_FRM);
	m_aPermit.Add(IDC_CMD_PV1_TXT);
	m_aPermit.Add(IDC_CMD_VEHICLE_CMB);
	m_aPermit.Add(IDC_CMD_PV2_TXT);
	m_aPermit.Add(IDC_CMD_REF_LANE_CMB);
	m_aPermit.Add(IDC_CMD_PV3_TXT);
	m_aPermit.Add(IDC_CMD_ECCEN_EDT);
	m_aPermit.Add(IDC_CMD_ECCEN_UNT);
	m_aPermit.Add(IDC_CMD_PV4_TXT);
	m_aPermit.Add(IDC_CMD_SCALE_FACTOR_EDT);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);
}


void CCMMvldItemIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemIDDlg)
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDesc);
	DDX_Control(pDX, IDC_CMD_LIST,  m_List);
	DDX_Control(pDX, IDC_CMD_LIST2, m_List2);
	DDX_Control(pDX, IDC_CMD_SF4, m_wndSF4);
	DDX_Control(pDX, IDC_CMD_SF3, m_wndSF3);
	DDX_Control(pDX, IDC_CMD_SF2, m_wndSF2);
	DDX_Control(pDX, IDC_CMD_SF1, m_wndSF1);
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_LANE_NUM_EDIT, m_wndLaneNumEdit);
	//DDX_Control(pDX, IDC_CMD_LANE_NUM_SPIN, m_wndLaneNumSpin);
	DDX_Check  (pDX, IDC_CMD_AUTO_LIVE_LOAD_COMBINATION, m_bAutoLiveLoad);    // 761 추가
	DDX_Check  (pDX, IDC_CMD_LCPV_CHK, m_bLCPV);
	DDX_Control(pDX, IDC_CMD_VEHICLE_CMB, m_cmbVehicle);
	DDX_Control(pDX, IDC_CMD_REF_LANE_CMB, m_cmbRefLane);
	DDX_Control(pDX, IDC_CMD_ECCEN_EDT, m_edtEccen);
	DDX_Control(pDX, IDC_CMD_ECCEN_UNT, m_untEccen);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR_EDT, m_edtScaleFactor);
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldItemIDDlg::InitUnit()
{
	m_edtEccen.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untEccen.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvldItemIDDlg::InitCombo()
{
	// Vehicle : Permit Vehicle만
	m_cmbVehicle.ResetContent();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVHL_D MvhlD;
	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD);
		if (MvhlD.nStandardCode == D_MVHL_IRC6_2000 && MvhlD.nLoadType == D_MVHL_KOR_RAIL_LOAD) // Permit Load
			CDlgUtil::CobxAddItem(m_cmbVehicle, MvhlD.VehicleLoadName, aMvhlK[i]);
	}
	if (aMvhlK.GetSize() > 0) m_cmbVehicle.SetCurSel(0);

	// Ref. Lane
	m_cmbRefLane.ResetContent();
	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0)
	{
		CArray<T_SLAN_K, T_SLAN_K> aSlanK;
		m_pDoc->m_pAttrCtrl->GetSlanKeyList(aSlanK);
		T_SLAN_D SlanD;
		for (int i = 0; i < aSlanK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetSlan(aSlanK[i], SlanD);
			CDlgUtil::CobxAddItem(m_cmbRefLane, SlanD.LaneName, aSlanK[i]);
		}
		if (aSlanK.GetSize() > 0) m_cmbRefLane.SetCurSel(0);
	}
	else
	{
		CArray<T_LLANid_K, T_LLANid_K> aLlanK;
		m_pDoc->m_pAttrCtrl->GetLlanidKeyList(aLlanK);
		T_LLANid_D LlanD;
		for (int i = 0; i < aLlanK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetLlanid(aLlanK[i], LlanD);
			CDlgUtil::CobxAddItem(m_cmbRefLane, LlanD.LineLaneName, aLlanK[i]);
		}
		if (aLlanK.GetSize() > 0) m_cmbRefLane.SetCurSel(0);
	}
}

void CCMMvldItemIDDlg::Data2Dlg()
{
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);
	CString csVal;
	csVal.Format(_T("%g"), m_Data.dScaleFactor[0]);
	m_wndSF1.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[1]);
	m_wndSF2.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[2]);
	m_wndSF3.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[3]);
	m_wndSF4.SetWindowText(csVal);
	csVal.Format(_T("%d"), m_Data.nNumLoadedLanes);
	m_wndLaneNumEdit.SetWindowText(csVal);
	m_bAutoLiveLoad = m_Data.bAutoLiveLoad;

	if(m_Data.bLoadCaseForPermitLoad)
	{
		if (!CDlgUtil::CobxSetCurSelItemData(m_cmbVehicle, (DWORD&)m_Data.PermitVehicleK))
			m_Data.PermitVehicleK = 0;
		if (!CDlgUtil::CobxSetCurSelItemData(m_cmbRefLane, (DWORD&)m_Data.RefLaneK))
			m_Data.RefLaneK = 0;
	}

	m_bLCPV = m_Data.bLoadCaseForPermitLoad;
	m_edtEccen.SetEditUnit(m_Data.dEccentricity);
	m_edtScaleFactor.SetEditUnit(m_Data.dPermitScaleFactor);	
	
	UpdateData(FALSE);

	MakeItemEx();  
}

BOOL CCMMvldItemIDDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);
	CFormulaEdit::GetEditValue(&m_wndSF1, m_Data.dScaleFactor[0]);
	CFormulaEdit::GetEditValue(&m_wndSF2, m_Data.dScaleFactor[1]);
	CFormulaEdit::GetEditValue(&m_wndSF3, m_Data.dScaleFactor[2]);
	CFormulaEdit::GetEditValue(&m_wndSF4, m_Data.dScaleFactor[3]);
	CFormulaEdit::GetEditValue(&m_wndLaneNumEdit, m_Data.nNumLoadedLanes);
	m_Data.bAutoLiveLoad = m_bAutoLiveLoad;

	//m_Data.arSubLoadData.RemoveAll(); Permit 일때 삭제 해준다.
	m_Data.bLoadCaseForPermitLoad = m_bLCPV;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbVehicle, (DWORD&)m_Data.PermitVehicleK);
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbRefLane, (DWORD&)m_Data.RefLaneK);
	m_Data.dEccentricity = m_edtEccen.GetEditValue();
	m_Data.dPermitScaleFactor = m_edtScaleFactor.GetEditValue();

	return TRUE;
}

BOOL CCMMvldItemIDDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	
	BOOL bSuccess;
	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvldid(m_csOldName, m_Data);
	else bSuccess = m_pDoc->m_pDataCtrl->AddMvldid(m_Data);
	if (!bSuccess) return FALSE;
	return TRUE;
}

#define COLCOUNT 10
/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
void CCMMvldItemIDDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Vehicle1), _LS(IDS_WG_CMD__ADDD__Vehicle2), _LS(IDS_WG_CMD_India_Footway), _LS(IDS_WG_CMD__ADDD__ReducFact),
		_LS(IDS_WG_CMD__ADDD__Lane1), _LS(IDS_WG_CMD__ADDD__Lane2), _LS(IDS_WG_CMD__ADDD__Lane3), 
		_LS(IDS_WG_CMD__ADDD__Lane4), _LS(IDS_WG_CMD__ADDD__Lane5), _LS(IDS_WG_CMD__ADDD__Lane6)};
	int nColWidth[COLCOUNT] = {60, 60, 60, 50, 60, 60, 60, 60, 60, 60};

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

	CString aTitle2[] = { _LS(IDS_WG_CMD__ADDD__Vehicle), _LS(IDS_WG_CMD__ADDD__ReducFact),
		_LS(IDS_WG_CMD__ADDD__Lane1), _LS(IDS_WG_CMD__ADDD__Lane2), _LS(IDS_WG_CMD__ADDD__Lane3), 
		_LS(IDS_WG_CMD__ADDD__Lane4), _LS(IDS_WG_CMD__ADDD__Lane5), _LS(IDS_WG_CMD__ADDD__Lane6)};
	int nColWidth2[8] = {60, 50, 60, 60, 60, 60, 60, 60};

	DWORD dwStyle2;
	dwStyle2 = ListView_GetExtendedListViewStyle(m_List2.GetSafeHwnd());
	dwStyle2 |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List2.GetSafeHwnd(), dwStyle2);

	// Set Title
	CString title2;
	LV_COLUMN lvcolumn2;
	for(int i = 0; i < 8; i++)
	{
		title2 = aTitle2[i];
		lvcolumn2.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn2.fmt = LVCFMT_LEFT;
		lvcolumn2.iSubItem = i;
		lvcolumn2.cx = globalUtils.ScaleByDPI(nColWidth2[i]);
		lvcolumn2.pszText = title2.GetBuffer(0);
		m_List2.InsertColumn(i,&lvcolumn2);
		title2.ReleaseBuffer();
	}  
}

void CCMMvldItemIDDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();

	int nItemCount = m_Data.arSubLoadData.GetSize();
	if(nItemCount == 0) return;
	for(int nCount = 0 ; nCount < nItemCount ; nCount++)
		InsertItem(nCount, m_Data.arSubLoadData[nCount]);
}

BOOL CCMMvldItemIDDlg::InsertItem(int nIndex, T_MVLDid_BASE &Data)
{
	int nItem;
	CString str;
	LVITEM lvitem;

	lvitem.iItem = nIndex;

	if(m_bAutoLiveLoad)
	{
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
	}
	else
	{
		for(int i = 0; i < 8; i++)
		{
			lvitem.iSubItem=i;
			str = DataToStr(i, Data);
			lvitem.pszText=str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT;
			if (i==0) nItem = m_List2.InsertItem(&lvitem);
		  else m_List2.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
	}
	return TRUE;
}

BOOL CCMMvldItemIDDlg::DeleteItem(int nIndex, T_MVLDid_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_List.DeleteItem(nIndex);
	if (nIndex >= 0) m_List2.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemIDDlg::ModifyItem(int nIndex, T_MVLDid_BASE &Data)
{
	int nItem = nIndex;
	if (nItem == -1) return TRUE;
	
	CString str;
	
	if(m_bAutoLiveLoad)
	{
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);
		  m_List.SetItemText(nItem, i, str);
		}
	}
	else
	{
		for(int i = 0; i < 8; i++)
		{
			str = DataToStr(i, Data);
		  m_List2.SetItemText(nItem, i, str);
		}
	}

	return TRUE;
}

CString CCMMvldItemIDDlg::GetMvhlName(UINT nMvhlKey)
{
	T_MVHL_D mvhl;
	m_pDoc->m_pAttrCtrl->GetMvhl(nMvhlKey, mvhl);
	return mvhl.VehicleLoadName;
}

CString CCMMvldItemIDDlg::DataToStr(int i, T_MVLDid_BASE &Data)
{
	CString str;
	if (m_bAutoLiveLoad)
	{
		if (i == 0) str = GetMvhlName(Data.VehicleKey1);
		else if (i==1) 
		{
			int nLaneNum = GetLaneNum();
			if (nLaneNum == 1) str = _T("");
			else str = GetMvhlName(Data.VehicleKey2);
		}
		else if (i==2) str = GetMvhlName(Data.FootwayKey);
		else if (i==3) str.Format(_T("%g"), Data.dScaleFactor);
		else if (i>=4 && i<=9)
		{
			if(Data.aSelectedLanes.GetSize() > i-4)
			{
				if (Data.aSelectedLanes[i-4] == 0) str = _T("");
				else str = GetLaneName(Data.aSelectedLanes[i-4]); 
			}
			else str = _T("");
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
	}
	else
	{
		if (i == 0) str = GetMvhlName(Data.VehicleKey1);
		else if (i==1) str.Format(_T("%g"), Data.dScaleFactor);
		else if (i>=2 && i<=7)
		{
			if(Data.aSelectedLanes.GetSize() > i-2)
			{
				if (Data.aSelectedLanes[i-2] == 0) str = _T("");
				else str = GetLaneName(Data.aSelectedLanes[i-2]); 
			}
			else str = _T("");
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
	}

	return str;
}

CString CCMMvldItemIDDlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLAN_D slan;
		m_pDoc->m_pAttrCtrl->GetSlan(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_LLANid_D llan;
		m_pDoc->m_pAttrCtrl->GetLlanid(nLaneKey, llan);
		return llan.LineLaneName;
	}
	return _LS(IDS_WG_CMD__ADDD__Error);
}

void CCMMvldItemIDDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;

	int nDistX = 0;
	int nDistY = 0;

	// Sub Load Case List 2
	GetDlgItem(IDC_CMD_LIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LIST2)->GetWindowRect(rToMove);
	
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top  - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aSubLoadCase2, nDistX, nDistY);
	
	// Permit
	GetDlgItem(IDC_CMD_FACTOR_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_PV_FRM)->GetWindowRect(rToMove);
	
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top  - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aPermit, nDistX, nDistY);

	// Etc
	if(m_bLCPV) GetDlgItem(IDC_CMD_PV_FRM)->GetWindowRect(rRef);
	else        GetDlgItem(IDC_CMD_SUB_LCASE_GRP)->GetWindowRect(rRef);

	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);		

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}


BEGIN_MESSAGE_MAP(CCMMvldItemIDDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemIDDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST2, OnDblclkCmdList)
	ON_EN_CHANGE(IDC_CMD_LANE_NUM_EDIT, OnChangeCmdLaneNumEdit)
	ON_BN_CLICKED(IDC_CMD_AUTO_LIVE_LOAD_COMBINATION, OnCheckAutoLiveLoad) // v761 추가
	ON_BN_CLICKED(IDC_CMD_LCPV_CHK, OnCheckPermit) // v761 추가
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemIDDlg message handlers

BOOL CCMMvldItemIDDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0) m_bSurface = TRUE;
	else m_bSurface = FALSE;
	
	// TODO: Add extra initialization here
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dScaleFactor[0] = 1.0;
		m_Data.dScaleFactor[1] = 0.9;
		m_Data.dScaleFactor[2] = 0.8;
		m_Data.dScaleFactor[3] = 0.8;
	}

	InitUnit();
	InitCombo();
	SetHeaderTitle();
	Data2Dlg();

	m_wndLaneNumEdit.SetRange(1, 6);
	m_wndLaneNumEdit.SetInteger(TRUE);

	AlignControl();
	CtrlEnableDisable();

	// MNET:XXXX 20071225 mylee - 정진상 부장님 요청으로 ReadOnly 안하는 것으로
	//m_wndLaneNumEdit.SetReadOnly(m_Data.arSubLoadData.GetSize() > 0);
	//m_wndLaneNumSpin.EnableWindow(m_Data.arSubLoadData.GetSize() <= 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemIDDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldItemIDDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();	
}

void CCMMvldItemIDDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CCMMvldSubItemIDDlg dlg(this);
	dlg.SetAutoValue(m_bAutoLiveLoad);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemIDDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = 0;
	if(m_bAutoLiveLoad)
	{
		iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		if (iItem == -1) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
			return;
		}
	}
	else
	{
		iItem = m_List2.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		if (iItem == -1) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
			return;
		}
	}

	CCMMvldSubItemIDDlg dlg(this);
	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetAutoValue(m_bAutoLiveLoad);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

BOOL CCMMvldItemIDDlg::OnApplySubDlg(BOOL bModify, T_MVLDid_BASE& data, int nPos)
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
	// MNET:XXXX 20071225 mylee - 정진상 부장님 요청으로 ReadOnly 안하는 것으로
	//m_wndLaneNumEdit.SetReadOnly(m_Data.arSubLoadData.GetSize() > 0);
	//m_wndLaneNumSpin.EnableWindow(m_Data.arSubLoadData.GetSize() <= 0);

	return TRUE;
}

int  CCMMvldItemIDDlg::GetLaneNum()
{
	int nNumLoadedLanes;
	CFormulaEdit::GetEditValue(&m_wndLaneNumEdit, nNumLoadedLanes);
	return nNumLoadedLanes;
}

void CCMMvldItemIDDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	if(m_bAutoLiveLoad)
	{
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
	}
	else
	{
		int iItem = m_List2.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		if (iItem == -1) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
			return;
		}
		m_Data.arSubLoadData.RemoveAt(iItem);
		m_List2.DeleteItem(iItem);

		int nCount = m_List2.GetItemCount();
		if (nCount <= iItem) iItem -= 1;
		if (iItem >= 0) 
			m_List2.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
	}

	// MNET:XXXX 20071225 mylee - 정진상 부장님 요청으로 ReadOnly 안하는 것으로
	//m_wndLaneNumEdit.SetReadOnly(m_Data.arSubLoadData.GetSize() > 0);
	//m_wndLaneNumSpin.EnableWindow(m_Data.arSubLoadData.GetSize() <= 0);
}

void CCMMvldItemIDDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();	
	*pResult = 0;
}

void CCMMvldItemIDDlg::OnChangeCmdLaneNumEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	HWND hWnd = m_wndLaneNumEdit.GetSafeHwnd();
	if (!hWnd || !(::IsWindow(hWnd))) return;

	CString csVal;
	int nLaneNum;
	m_wndLaneNumEdit.GetWindowText(csVal);
	if (csVal.IsEmpty()) return;
	nLaneNum = _ttoi(csVal);

	if (nLaneNum < 1) m_wndLaneNumEdit.SetWindowText(_T("1"));
	if (nLaneNum > 6) m_wndLaneNumEdit.SetWindowText(_T("6"));
}

void CCMMvldItemIDDlg::OnCheckAutoLiveLoad()
{
	if(m_Data.arSubLoadData.GetSize())
	{
		int nAnswer = AfxMessageBox(_LS(IDS_CMD_MOVING_INDIA_CHECK_AUTO_LIVE_LOAD_COMBINATION),MB_YESNO);
		if (nAnswer == IDYES) // Yes
		{
			m_Data.arSubLoadData.RemoveAll();
			MakeItemEx();
		}
		else // No
		{
			UpdateData(FALSE);
			return;
		}
	}

	CtrlEnableDisable();
}

void CCMMvldItemIDDlg::OnCheckPermit()
{
	UpdateData(TRUE);

	AlignControl();
	CtrlEnableDisable();
}

void CCMMvldItemIDDlg::CtrlEnableDisable()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_AUTO_LIVE_LOAD_COMBINATION)->EnableWindow(!m_bLCPV);
	
	CDlgUtil::CtrlShowHide(this, m_aMulti,       !m_bLCPV);
	CDlgUtil::CtrlShowHide(this, m_aDesignLane,  !m_bLCPV);
	CDlgUtil::CtrlShowHide(this, m_aSubLoadCase, !m_bLCPV);
	CDlgUtil::CtrlShowHide(this, m_aPermit,       m_bLCPV);

	if(!m_bLCPV)
	{
		if(m_bAutoLiveLoad)
		{
			CDlgUtil::CtrlEnableDisable(this, m_aDesignLane, TRUE);
			
			CDlgUtil::CtrlShowHide(this, m_aSubLoadCase1, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aSubLoadCase2, FALSE);
		}
		else
		{
			CDlgUtil::CtrlEnableDisable(this, m_aDesignLane, FALSE);
			
			CDlgUtil::CtrlShowHide(this, m_aSubLoadCase1, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aSubLoadCase2, TRUE);
		}
	}
}