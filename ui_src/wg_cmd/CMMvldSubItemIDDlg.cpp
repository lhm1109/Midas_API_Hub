// CMMvldSubItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldSubItemIDDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\VehlDef.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "CMMvhlListDlg.h"
#include "CMMvldItemIDDlg.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key  : 31;
	} typekey;
};

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemIDDlg dialog


CCMMvldSubItemIDDlg::CCMMvldSubItemIDDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldSubItemIDDlg::IDD, pParent)
{
	m_pDoc    = 0;
	m_bModify = FALSE;
	m_nPos    = 0;

	ASSERT(pParent);
	m_pMvldItemDlg = (CCMMvldItemIDDlg*)pParent; 

	m_bListMode = TRUE;
	m_bAuto     = TRUE;
	m_bSurface  = FALSE;

	//{{AFX_DATA_INIT(CCMMvldSubItemIDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// Auto Load Case - Sub Load Case 1
	m_aAutoLoadCaseData.Add(IDC_CMD_LOAD_CASE_DATA_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_SCALE_FACTOR_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_SCALE_FACTOR);
	m_aAutoLoadCaseData.Add(IDC_CMD_NUM_LOAD_LANE_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_LANE_NUM);
	m_aAutoLoadCaseData.Add(IDC_CMD_VEHICLE_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_CLASS_1_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_VCLASS);
	m_aAutoLoadCaseData.Add(IDC_CMD_BTN_VCLASS);
	m_aAutoLoadCaseData.Add(IDC_CMD_CLASS_2_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_VCLASS2);
	m_aAutoLoadCaseData.Add(IDC_CMD_BTN_VCLASS2);
	m_aAutoLoadCaseData.Add(IDC_CMD_FOOTWAYKEY_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_FOOTWAYKEY_CMB);
	m_aAutoLoadCaseData.Add(IDC_CMD_BTN_FOOTWAYKEY);
	m_aAutoLoadCaseData.Add(IDC_CMD_CARRIAGEWAY_WIDTH_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_CARRIAGEWAY_WIDTH_EDT);
	m_aAutoLoadCaseData.Add(IDC_CMD_CARRIAGEWAY_WIDTH_UNT);
	m_aAutoLoadCaseData.Add(IDC_CMD_FOOTWAY_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_DLOAD_EDIT);
	m_aAutoLoadCaseData.Add(IDC_CMD_DLOAD_UNIT);
	m_aAutoLoadCaseData.Add(IDC_CMD_ASSIGN_LANES_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_LIST_LANES_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_UNSEL_LIST);
	m_aAutoLoadCaseData.Add(IDC_CMD_SEL_LANES_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_SEL_LIST);
	m_aAutoLoadCaseData.Add(IDC_CMD_SEL_FOOTWAY_STC);
	m_aAutoLoadCaseData.Add(IDC_CMD_SEL_FOOTWAY_LST);
	m_aAutoLoadCaseData.Add(IDC_CMD_BTN_ADD);
	m_aAutoLoadCaseData.Add(IDC_CMD_BTN_DELETE);
	m_aAutoLoadCaseData.Add(IDC_CMD_FOOTWAY_ADD_BTN);
	m_aAutoLoadCaseData.Add(IDC_CMD_FOOTWAY_DEL_BTN);

	// User Load Case - Sub Load Case 2
	m_aUserLoadCaseData.Add(IDC_CMD_LOAD_CASE_DATA_STC2);
	m_aUserLoadCaseData.Add(IDC_CMD_SCALE_FACTOR_STC2);
	m_aUserLoadCaseData.Add(IDC_CMD_SCALE_FACTOR2);
	m_aUserLoadCaseData.Add(IDC_CMD_NUM_LOAD_LANE_STC2);
	m_aUserLoadCaseData.Add(IDC_CMD_LANE_NUM2);
	m_aUserLoadCaseData.Add(IDC_CMD_NUM_LOAD_LANE_STC3);
	m_aUserLoadCaseData.Add(IDC_CMD_LANE_NUM3);
	m_aUserLoadCaseData.Add(IDC_CMD_VEHICLE_2_STC);
	m_aUserLoadCaseData.Add(IDC_CMD_VCLASS3);
	m_aUserLoadCaseData.Add(IDC_CMD_BTN_VCLASS3);
	m_aUserLoadCaseData.Add(IDC_CMD_ASSIGN_LANES_STC2);
	m_aUserLoadCaseData.Add(IDC_CMD_LIST_LANES_STC2);
	m_aUserLoadCaseData.Add(IDC_CMD_UNSEL_LIST2);
	m_aUserLoadCaseData.Add(IDC_CMD_SEL_LANES_STC2);
	m_aUserLoadCaseData.Add(IDC_CMD_SEL_LIST3);
	m_aUserLoadCaseData.Add(IDC_CMD_BTN_ADD3);
	m_aUserLoadCaseData.Add(IDC_CMD_BTN_DELETE2);

	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);
}

void CCMMvldSubItemIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldSubItemIDDlg)

	// Auto Load Case - Sub Load Case 1
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR,           m_wndScaleFactor);    // Scale Factor
	DDX_Control(pDX, IDC_CMD_LANE_NUM,               m_wndLaneNum);        // Number of Design Lanes
	DDX_Control(pDX, IDC_CMD_VCLASS,                 m_wndVClass);         // Vehicle Class 1
	DDX_Control(pDX, IDC_CMD_VCLASS2,                m_wndVClass2);        // Vehicle Class 2
	DDX_Control(pDX, IDC_CMD_FOOTWAYKEY_CMB,         m_wndFootway);        // Footway
	DDX_Control(pDX, IDC_CMD_CARRIAGEWAY_WIDTH_EDT,  m_wndWidthEdit);      // Carriageway Width
	DDX_Control(pDX, IDC_CMD_CARRIAGEWAY_WIDTH_UNT,  m_wndWidthUnit);      // Carriageway Width Unit
	DDX_Control(pDX, IDC_CMD_DLOAD_EDIT,             m_wndDLoadEdit);      // Carriageway Loading 
	DDX_Control(pDX, IDC_CMD_DLOAD_UNIT,             m_wndDLoadUnit);      // Carriageway Loading Unit
	DDX_Control(pDX, IDC_CMD_UNSEL_LIST,             m_wndUnselList);      // List of Lanes
	DDX_Control(pDX, IDC_CMD_SEL_LIST,               m_wndSelList);        // Selected Lanes for Carriageway
	DDX_Control(pDX, IDC_CMD_SEL_FOOTWAY_LST,        m_wndSelFootwayList); // Selected Lanes for footway

	// User Load Case - Sub Load Case 2
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR2,  m_edtScaleFactor);      // Scale Factor
	DDX_Control(pDX, IDC_CMD_LANE_NUM2,      m_edtMinLaneNum);       // Min
	DDX_Control(pDX, IDC_CMD_LANE_NUM3,      m_edtMaxLaneNum);       // Max
	DDX_Control(pDX, IDC_CMD_VCLASS3,        m_cmbVehicle);          // Vehicle
	DDX_Control(pDX, IDC_CMD_UNSEL_LIST2,    m_wndUnselList2);       // List of Lanes
	DDX_Control(pDX, IDC_CMD_SEL_LIST3,      m_wndSelList2);         // Selected Lanes
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldSubItemIDDlg::Data2Dlg()
{
	// Sub-Load Case1
	if(m_bAuto)
	{
		// Scale Factor
		CString csVal;
		csVal.Format(_T("%g"), m_Data.dScaleFactor);
		m_wndScaleFactor.SetWindowText(csVal);

		// Number of Loaded Lanes
		int nLaneNum = m_pMvldItemDlg->GetLaneNum();
		csVal.Format(_T("%d"), nLaneNum);
		m_wndLaneNum.SetWindowText(csVal);
		m_wndLaneNum.SetReadOnly(TRUE);    // Read only

		// Vehicle Class 1
		T_MVHL_D mvhl;
		m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey1, mvhl);
		m_wndVClass.SelectString(-1, mvhl.VehicleLoadName);

		// Footway
		m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.FootwayKey, mvhl);
		m_wndFootway.SelectString(-1, mvhl.VehicleLoadName);
	
		if (nLaneNum == 1) // 한개일 때 
		{
			// Footway Loading -> Carriageway Loading
			m_wndDLoadEdit.SetEditUnit(m_Data.dFootwayLoad);

			// Carriageway Width
			m_wndWidthEdit.SetEditUnit(m_Data.dCarriagewayWidth);
		}    
		else    
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey2, mvhl);
			m_wndVClass2.SelectString(-1, mvhl.VehicleLoadName);
		}  
		// Vehicle Class 2
		m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey2, mvhl);
		m_wndVClass2.SelectString(-1, mvhl.VehicleLoadName);

		// Enable, Disable
		m_wndDLoadEdit.EnableWindow(nLaneNum == 1);
		m_wndWidthEdit.EnableWindow(nLaneNum == 1);
		m_wndVClass2.EnableWindow(nLaneNum != 1);

		// Selected Lanes for Carriageway
		int nAllSize = m_aLaneList.GetSize();
		int nSelSize = 0;
		int nMaxCount = m_Data.aSelectedLanes.GetSize();
		for (nSelSize = 0; nSelSize < nMaxCount; nSelSize++)
		{
			if (m_Data.aSelectedLanes[nSelSize] == 0) break;
		}

		m_aSelFlag.SetSize(nAllSize);
		for (int i = 0; i < nAllSize; i++)
		{
			int j = 0;
			for (j = 0; j < nSelSize; j++)
				if (m_aLaneList[i] == m_Data.aSelectedLanes[j]) break;
			if (j == nSelSize) m_aSelFlag[i] = FALSE;
			else m_aSelFlag[i] = TRUE;
		}

		// Selected Lanes for footway
		nSelSize = 0;
		nMaxCount = m_Data.aSelectedFootwayLanes.GetSize();
		for (nSelSize = 0; nSelSize < nMaxCount; nSelSize++)
		{
			if (m_Data.aSelectedFootwayLanes[nSelSize] == 0) break;
		}

		m_aSelFootwayFlag.SetSize(nAllSize);
		for (int i = 0; i < nAllSize; i++)
		{
			int j = 0;
			for (j = 0; j < nSelSize; j++)
				if (m_aLaneList[i] == m_Data.aSelectedFootwayLanes[j]) break;
			if (j == nSelSize) m_aSelFootwayFlag[i] = FALSE;
			else m_aSelFootwayFlag[i] = TRUE;
		}

		int nIndex = 0;

		m_wndSelList.ResetContent();
		m_wndUnselList.ResetContent();
		m_wndSelFootwayList.ResetContent();

		// Selected Lanes for Carriageway, Selected Lanes for footway - Add String
		CString LaneName;
		for (int i = 0; i < nAllSize; i++)
		{
			LaneName = GetLaneName(m_aLaneList[i]);
			if (m_aSelFlag[i]) 
			{
				nIndex = m_wndSelList.AddString(LaneName);
				m_wndSelList.SetItemData(nIndex, i);
			}
			else if (m_aSelFootwayFlag[i])
			{
				nIndex = m_wndSelFootwayList.AddString(LaneName);
				m_wndSelFootwayList.SetItemData(nIndex, i);
			}
			else
			{
				nIndex = m_wndUnselList.AddString(LaneName);
				m_wndUnselList.SetItemData(nIndex, i);
			}
		}

	}
	// Sub-Load Case 2
	else 
	{
		// Scale Factor
		m_edtScaleFactor.SetEditUnit(m_Data.dScaleFactor);

		// Number of Loaded Lanes, Spin
		m_edtMinLaneNum.SetRange(0, 6);
		m_edtMinLaneNum.SetValue(m_Data.nMinNumLoadedLanes);
		m_edtMinLaneNum.SetInteger(TRUE);
		m_edtMaxLaneNum.SetRange(1, 6);
		m_edtMaxLaneNum.SetValue(m_Data.nMaxNumLoadedLanes);
		m_edtMaxLaneNum.SetInteger(TRUE);

		// Vehicle
		T_MVHL_D mvhl;
		m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey1, mvhl);
		m_cmbVehicle.SelectString(-1, mvhl.VehicleLoadName);

		// Select 된 것, 아닌 것 구분
		int nAllSize = m_aLaneList.GetSize();
		int nSelSize;
		int nMaxCount = m_Data.aSelectedLanes.GetSize();
		for (nSelSize = 0; nSelSize < nMaxCount; nSelSize++)
			if (m_Data.aSelectedLanes[nSelSize] == 0) break;

		m_aSelFlag.SetSize(nAllSize);
		for (int i = 0; i < nAllSize; i++)
		{
			int j = 0;
			for (j = 0; j < nSelSize; j++)
				if (m_aLaneList[i] == m_Data.aSelectedLanes[j]) break;
			if (j == nSelSize) m_aSelFlag[i] = FALSE;
			else m_aSelFlag[i] = TRUE;
		}

		int nIndex;

		m_wndSelList2.ResetContent();
		m_wndUnselList2.ResetContent();

		// Add String
		CString LaneName;
		for (int i = 0; i < nAllSize; i++)
		{
			LaneName = GetLaneName(m_aLaneList[i]);
			if (m_aSelFlag[i]) 
			{
				nIndex = m_wndSelList2.AddString(LaneName);
				m_wndSelList2.SetItemData(nIndex, i);
			}
			else
			{
				nIndex = m_wndUnselList2.AddString(LaneName);
				m_wndUnselList2.SetItemData(nIndex, i);
			}
		}
	}
}

BOOL CCMMvldSubItemIDDlg::Dlg2Data()
{
	m_Data.Initialize();

	// Sub-Load Case 1
	if (m_bAuto)
	{
		// Scale Factor
		CFormulaEdit::GetEditValue(&m_wndScaleFactor, m_Data.dScaleFactor);

		// Number of Loaded Lanes
		int nLaneNum;
		CFormulaEdit::GetEditValue(&m_wndLaneNum, nLaneNum);

		// Vehicle Class 1
		int nCurSel = m_wndVClass.GetCurSel();
		if (nCurSel < 0) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_Class_is_not_sele));
			return FALSE;
		}
		m_Data.VehicleKey1 = m_wndVClass.GetItemData(nCurSel);

		// Vehicle Class 2
		if (nLaneNum != 1)
		{
			nCurSel = m_wndVClass2.GetCurSel();
			if (nCurSel < 0) 
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_Class_is_not_sele));
				return FALSE;
			}
			m_Data.VehicleKey2 = m_wndVClass2.GetItemData(nCurSel);
		}
		else
		{
			//Footway Loading -> Carriageway Loading 
			m_Data.dFootwayLoad = m_wndDLoadEdit.GetEditValue();

			// Carriageway Width 
			m_Data.dCarriagewayWidth = m_wndWidthEdit.GetEditValue();
		}
	 
		// 공통 Test 
		nLaneNum = 0;
		CFormulaEdit::GetEditValue(&m_wndLaneNum, nLaneNum);

		int nCount = 0;
		int nFootwayCount = 0;

		// v795 부터 aSelectedLanes, aSelectedFootwayLanes Array 로 바뀌었다.
		int nMax = 6;
		int nFootwayMax = 10;
		int nSize = m_aLaneList.GetSize();
		for (int i = 0; i < nSize; i++)
		{
			if (m_aSelFlag[i]) 
			{
				if (nCount >= nMax)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMax);
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aSelectedLanes.Add(m_aLaneList[i]);
				nCount++;
			}

			if(m_aSelFootwayFlag[i])
			{
				// Footway
				if (nFootwayCount >= nFootwayMax)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nFootwayMax);
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aSelectedFootwayLanes.Add(m_aLaneList[i]);
				nFootwayCount++;

				nCurSel = m_wndFootway.GetCurSel();
				if (nCurSel < 0)
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_Class_is_not_sele));
					return FALSE;
				}
				m_Data.FootwayKey = m_wndFootway.GetItemData(nCurSel);
			}
		}
		if (nLaneNum != nCount)
		{
			CString strMsg;
			strMsg.Format(_LS(IDS_WG_CMD_ERR_Selected_Lane_Number), nLaneNum);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	// Sub-Load Case 2
	else
	{
		// Scale Factor
		m_Data.dScaleFactor = m_edtScaleFactor.GetEditValue();

		// Number of Loaded Lanes
		m_Data.nMinNumLoadedLanes = m_edtMinLaneNum.GetEditValueInt();
		m_Data.nMaxNumLoadedLanes = m_edtMaxLaneNum.GetEditValueInt();

		if(m_Data.nMinNumLoadedLanes > m_Data.nMaxNumLoadedLanes)
		{
			AfxMessageBox(_LS(IDS_CMD_MOVING_INDIA_NUMBER_OF_LOADED_LANES_ERROR));
			return FALSE; 
		}

		// Vehicle
		int nCurSel = m_cmbVehicle.GetCurSel();
		if (nCurSel < 0) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_Class_is_not_sele));
			return FALSE;
		}
		m_Data.VehicleKey1 = m_cmbVehicle.GetItemData(nCurSel);

		
		int nLaneNum = 0;
		nLaneNum = m_Data.nMaxNumLoadedLanes;

		int nCount = 0;
		int nMax = sizeof(m_Data.aSelectedLanes)/sizeof(int);
		int nSize = m_aLaneList.GetSize();
		for (int i = 0; i < nSize; i++)
		{
			if (m_aSelFlag[i]) 
			{
				if (nCount >= nMax)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMax);
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aSelectedLanes.Add(m_aLaneList[i]);
				nCount++;
			}
		}
		if (nLaneNum > nCount)
		{
			CString strMsg;
			strMsg.Format(_LS(IDS_WG_CMD_ERR_Selected_Lane_Number), nLaneNum);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CCMMvldSubItemIDDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	return m_pMvldItemDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
}

void CCMMvldSubItemIDDlg::FillVClassList()
{
	m_wndVClass.ResetContent();
	m_wndVClass2.ResetContent();
	m_cmbVehicle.ResetContent();
	m_wndFootway.ResetContent();

	int nIndex;
	T_MVHL_D mvhl;
	CArray<T_MVHL_K, T_MVHL_K> aKeyListMvhl;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aKeyListMvhl);
	int nSize = aKeyListMvhl.GetSize();

	for (int i = 0; i < nSize; i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(aKeyListMvhl[i], mvhl)) continue;

		if(m_bAuto)
		{
			if(m_pDoc->m_pAttrCtrl->IsPermitVehicle(mvhl)) continue;
			if(mvhl.nStandardCode == D_MVHL_IRS_BRIDGE_RULES) continue;

			if (mvhl.nLoadType == 6) // FATIGUE VEHICLE
			{
				// 포함 안되도록 함.
			}
			else if (mvhl.nLoadType && mvhl.nLoadType != 5) // Vehicle Class 1 (Auto Load Case)
			{
				nIndex = m_wndVClass.AddString(mvhl.VehicleLoadName);
				m_wndVClass.SetItemData(nIndex, aKeyListMvhl[i]);
			}
			else if(mvhl.nLoadType == 5) // Footway (Auto Load Case)
			{
				nIndex = m_wndFootway.AddString(mvhl.VehicleLoadName);
				m_wndFootway.SetItemData(nIndex, aKeyListMvhl[i]);
			}
			else // Vehicle Class 2 (Auto Load Case)
			{
				nIndex = m_wndVClass2.AddString(mvhl.VehicleLoadName);
				m_wndVClass2.SetItemData(nIndex, aKeyListMvhl[i]);
			}
		}
		else
		{
			// Vehicle (User Load Case)
			if (CTestEnvMgr::GetTestEnvST(_LSX(India Permit)) == _LSX(yes)) 
			{
				nIndex = m_cmbVehicle.AddString(mvhl.VehicleLoadName);
				m_cmbVehicle.SetItemData(nIndex, aKeyListMvhl[i]);
			}
			else
			{
				if(m_pDoc->m_pAttrCtrl->IsPermitVehicle(mvhl)) continue;
				
				nIndex = m_cmbVehicle.AddString(mvhl.VehicleLoadName);
				m_cmbVehicle.SetItemData(nIndex, aKeyListMvhl[i]);
			}   
		}
	}
	m_wndVClass.SetCurSel(0);
	m_wndVClass2.SetCurSel(0);
	m_cmbVehicle.SetCurSel(0);
	m_wndFootway.SetCurSel(0);
}

CString CCMMvldSubItemIDDlg::GetLaneName(UINT nLaneKey)
{
	if(m_bSurface)
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

void CCMMvldSubItemIDDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;

	double dDistY = 0.;

	if(m_bAuto) // Auto Live Load Combination Check
	{
		CDlgUtil::CtrlShowHide(this, m_aAutoLoadCaseData, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aUserLoadCaseData, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aEtc, TRUE);
	}
	else // Auto Live Load Combination Uncheck
	{
		CDlgUtil::CtrlShowHide(this, m_aAutoLoadCaseData, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aUserLoadCaseData, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aEtc, TRUE);

		// Load Case Data Move
		GetDlgItem(IDC_CMD_LOAD_CASE_DATA_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LOAD_CASE_DATA_STC2)->GetWindowRect(rToMove);

		dDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aUserLoadCaseData, dDistY);

		// Etc Move
		GetDlgItem(IDC_CMD_ASSIGN_LANES_STC2)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rToMove);

		dDistY = rRef.bottom - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aEtc, dDistY+13);
	}
	
	// Resize Window
	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + 12;
	MoveWindow(rToMove);
}

BEGIN_MESSAGE_MAP(CCMMvldSubItemIDDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldSubItemIDDlg)

	// Auto Load Case - Sub Load Case 1  
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS,      OnCmdBtnVclass)
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS2,     OnCmdBtnVclass)
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS3,     OnCmdBtnVclass)
	ON_BN_CLICKED(IDC_CMD_BTN_FOOTWAYKEY,  OnCmdBtnVclass)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,         OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,      OnCmdBtnDel)
	ON_BN_CLICKED(IDC_CMD_FOOTWAY_ADD_BTN, OnCmdBtnFootwayAdd)
	ON_BN_CLICKED(IDC_CMD_FOOTWAY_DEL_BTN, OnCmdBtnFootwayDel)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST,      OnDblclkCmdUnselList)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST,        OnDblclkCmdSelList)
	ON_LBN_DBLCLK(IDC_CMD_SEL_FOOTWAY_LST, OnDblclkCmdSelFootwayList)

	// User Load Case - Sub Load Case 2
	ON_BN_CLICKED(IDC_CMD_BTN_ADD3,     OnCmdBtnAdd2)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE2,  OnCmdBtnDel2)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST2,  OnDblclkCmdUnselList2)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST3,    OnDblclkCmdSelList2)

	ON_BN_CLICKED(IDC_CMD_APPLY,       OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemIDDlg message handlers

BOOL CCMMvldSubItemIDDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0) m_bSurface = TRUE;
	else m_bSurface = FALSE;
	
	// TODO: Add extra initialization here
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dScaleFactor = 1.0;
		m_Data.dFootwayLoad = GetInitFootwayLoad();
	}
	//m_pDoc->m_pAttrCtrl->GetLlanidKeyList(m_aLaneList);
	if (m_bSurface) m_pDoc->m_pAttrCtrl->GetSlanKeyList(m_aLaneList);
	else m_pDoc->m_pAttrCtrl->GetLlanidKeyList(m_aLaneList);

	m_wndDLoadEdit.SetUnitType(CUnitCtrl::m_MVLDid_UNIT.dFootwayLoad);
	m_wndDLoadUnit.SetUnitType(CUnitCtrl::m_MVLDid_UNIT.dFootwayLoad);
	m_wndWidthEdit.SetUnitType(CUnitCtrl::m_MVLDid_UNIT.dCarriagewayWidth);
	m_wndWidthUnit.SetUnitType(CUnitCtrl::m_MVLDid_UNIT.dCarriagewayWidth);
	
	FillVClassList();

	Data2Dlg();

	AlignControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldSubItemIDDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

double CCMMvldSubItemIDDlg::GetInitFootwayLoad()
{   
	T_UNIT_INDEX UnitIndex; 
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	int nLengthUnit, nForceUnit;
	nLengthUnit = UnitIndex.nBase_Length;
	nForceUnit  = UnitIndex.nBase_Force;
/*
#define D_UNITSYS_LENGTH_INDEX_MM           0
#define D_UNITSYS_LENGTH_INDEX_CM           1
#define D_UNITSYS_LENGTH_INDEX_M            2
#define D_UNITSYS_LENGTH_INDEX_IN           3
#define D_UNITSYS_LENGTH_INDEX_FT           4

#define D_UNITSYS_FORCE_INDEX_KG            0
#define D_UNITSYS_FORCE_INDEX_TON           1
#define D_UNITSYS_FORCE_INDEX_N             2
#define D_UNITSYS_FORCE_INDEX_KN            3
#define D_UNITSYS_FORCE_INDEX_LBF           4
#define D_UNITSYS_FORCE_INDEX_KIP           5
*/

	double DimFAC = 1.0, FrcFAC = 1.0;

	if     (nLengthUnit == 0) DimFAC=1000.0 ;           // mm TO m
	else if(nLengthUnit == 1) DimFAC=100.0  ;           // cm TO m
	else if(nLengthUnit == 2) DimFAC=1.0    ;           //  m TO m
	else if(nLengthUnit == 3) DimFAC=100.0/2.540;       // in TO m
	else if(nLengthUnit == 4) DimFAC=100.0/2.540/12.0 ; // ft TO m

	if     (nForceUnit == 0) FrcFAC=1.0  ;             //  kgf TO kgf 
	else if(nForceUnit == 1) FrcFAC=0.001;             //  ton TO kgf 
	else if(nForceUnit == 4) FrcFAC=2.20459;           //  lbf TO kgf 
	else if(nForceUnit == 5) FrcFAC=2.20459*0.001;     //  kip TO kgf 
	else if(nForceUnit == 2) FrcFAC=9.80665;           //  N   TO kgf 
	else if(nForceUnit == 3) FrcFAC=9.80665*0.001;     //  KN  TO kgf 
	
	double dFootwayLoad =  500.0 * FrcFAC / (DimFAC * DimFAC);   // 500kgf/m^2

	return dFootwayLoad;
}

void CCMMvldSubItemIDDlg::OnCmdBtnVclass() 
{
	// TODO: Add your control notification handler code here
	CCMMvhlListDlg dlg;
//  dlg.SetModeAddOnly();
	dlg.DoModal();

	T_MVHL_K MvhlK1, MvhlK2, FootwayK;
	int nCurSel1 = m_wndVClass.GetCurSel();
	if (nCurSel1 >= 0) MvhlK1 = m_wndVClass.GetItemData(nCurSel1);
	int nCurSel2 = m_wndVClass2.GetCurSel();
	if (nCurSel2 >= 0) MvhlK2 = m_wndVClass2.GetItemData(nCurSel2);
	int nCurSel3 = m_wndFootway.GetCurSel();
	if (nCurSel3 >= 0) FootwayK = m_wndFootway.GetItemData(nCurSel3);

	FillVClassList();
	
	CString csName;
	if (nCurSel1 >= 0) // 예전 선택 복구
	{
		T_MVHL_D mvhl;
		if (m_pDoc->m_pAttrCtrl->GetMvhl(MvhlK1, mvhl))
			csName = mvhl.VehicleLoadName;
		else csName = _T("");
	}
	else csName = _T("");
	if (csName == _T("")) m_wndVClass.SetCurSel(0);
	else m_wndVClass.SelectString(-1, csName);
	
	if (nCurSel2 >= 0) // 예전 선택 복구
	{
		T_MVHL_D mvhl;
		if (m_pDoc->m_pAttrCtrl->GetMvhl(MvhlK2, mvhl))
			csName = mvhl.VehicleLoadName;
		else csName = _T("");
	}
	else csName = _T("");
	if (csName == _T("")) m_wndVClass2.SetCurSel(0);
	else m_wndVClass2.SelectString(-1, csName);

	if (nCurSel3 >= 0) // 예전 선택 복구
	{
		T_MVHL_D mvhl;
		if (m_pDoc->m_pAttrCtrl->GetMvhl(FootwayK, mvhl))
			csName = mvhl.VehicleLoadName;
		else csName = _T("");
	}
}


void CCMMvldSubItemIDDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
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

void CCMMvldSubItemIDDlg::OnCmdBtnDel() 
{
	// TODO: Add your control notification handler code here
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
		if (!m_aSelFlag[i] && !m_aSelFootwayFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldSubItemIDDlg::OnCmdBtnFootwayAdd() 
{
	// TODO: Add your control notification handler code here
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
		m_aSelFootwayFlag[nIndex] = TRUE;
		m_wndUnselList.DeleteString(aIndex[i]);
	}
	
	CString csLaneName;
	m_wndSelFootwayList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFootwayFlag[i]) 
		{
			nIndex = m_wndSelFootwayList.AddString(csLaneName);
			m_wndSelFootwayList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldSubItemIDDlg::OnCmdBtnFootwayDel() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelFootwayList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelFootwayList.GetItemData(aIndex[i]);
		m_aSelFootwayFlag[nIndex] = FALSE;
		m_wndSelFootwayList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFootwayFlag[i] && !m_aSelFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldSubItemIDDlg::OnDblclkCmdUnselList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnAdd();
}

void CCMMvldSubItemIDDlg::OnDblclkCmdSelList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDel();
}

void CCMMvldSubItemIDDlg::OnDblclkCmdSelFootwayList()
{
	OnCmdBtnFootwayDel();
}

void CCMMvldSubItemIDDlg::OnCmdBtnAdd2() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndUnselList2.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndUnselList2.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_wndUnselList2.DeleteString(aIndex[i]);
	}
	
	CString csLaneName;
	m_wndSelList2.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList2.AddString(csLaneName);
			m_wndSelList2.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldSubItemIDDlg::OnCmdBtnDel2() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList2.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList2.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList2.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndUnselList2.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i]) 
		{
			nIndex = m_wndUnselList2.AddString(csLaneName);
			m_wndUnselList2.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldSubItemIDDlg::OnDblclkCmdUnselList2() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnAdd2();
}

void CCMMvldSubItemIDDlg::OnDblclkCmdSelList2() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDel2();
}

void CCMMvldSubItemIDDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();
}