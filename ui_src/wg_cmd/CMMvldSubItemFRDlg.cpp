// CMMvldSubItemFRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldSubItemFRDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\VehlDef.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "CMMvhcListDlg.h"
#include "CMMvldItemFRDlg.h"

#include "..\wg_base\TestEnvMgr.h"


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

#define SYSTEM_A   0
#define SYSTEM_Bc  1
#define SYSTEM_Bt  2
#define MILITARY   3
#define PEDESTRIAN 4

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemFRDlg dialog
CCMMvldSubItemFRDlg::CCMMvldSubItemFRDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldSubItemFRDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;
	m_nLoadModel = 0;
	m_nPos = 0;
	ASSERT(pParent);
	T_MVCD_D mvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData)) ASSERT(0);
	if (mvcdData.nCodeType == D_MOVE_CODE_FRANCE)
		m_pMvldItemFRDlg = (CCMMvldItemFRDlg*)pParent;
	else
		ASSERT(0);

	m_aVehLaneList.RemoveAll();
	m_aVehLaneList2.RemoveAll();

	m_aCtrlCentEff.RemoveAll();
 	m_aCtrlCentEff.Add(IDC_CMD_FR_CENT_EFF_GRP);
 	m_aCtrlCentEff.Add(IDC_CMD_FR_CENT_EFF_01_RDO);
 	m_aCtrlCentEff.Add(IDC_CMD_FR_CENT_EFF_02_RDO);
	m_aCtrlCentEff.Add(IDC_CMD_FR_CENT_EFF_03_RDO);

	m_aCtrlVehicle.RemoveAll();
	m_aCtrlVehicle.Add(IDC_CMD_VEHICLE_GRP);
	m_aCtrlVehicle.Add(IDC_CMD_VEHICLE_STC);
	m_aCtrlVehicle.Add(IDC_CMD_VEHICLE_CMB);
	m_aCtrlVehicle.Add(IDC_CMD_UNSEL_STC);
	m_aCtrlVehicle.Add(IDC_CMD_UNSEL_LIST);
	m_aCtrlVehicle.Add(IDC_CMD_SEL_STC);
	m_aCtrlVehicle.Add(IDC_CMD_SEL_LIST);
	m_aCtrlVehicle.Add(IDC_CMD_BTN_ADD);
	m_aCtrlVehicle.Add(IDC_CMD_BTN_DELETE);
	m_aCtrlVehicle.Append(m_aCtrlCentEff);

	m_aCtrlSidewalk.RemoveAll();
	m_aCtrlSidewalk.Add(IDC_CMD_SIDEWORK_GRP);
	m_aCtrlSidewalk.Add(IDC_CMD_SIDEWALK_STC);
	m_aCtrlSidewalk.Add(IDC_CMD_SIDEWALK_CMB);
	m_aCtrlSidewalk.Add(IDC_CMD_UNSEL2_STC);
	m_aCtrlSidewalk.Add(IDC_CMD_UNSEL2_LIST);
	m_aCtrlSidewalk.Add(IDC_CMD_BTN2_ADD);
	m_aCtrlSidewalk.Add(IDC_CMD_BTN2_DELETE);
	m_aCtrlSidewalk.Add(IDC_CMD_SEL2_STC);
	m_aCtrlSidewalk.Add(IDC_CMD_SEL2_LIST);

	m_aCtrlOkNg.RemoveAll();
	m_aCtrlOkNg.Add(IDOK);
	m_aCtrlOkNg.Add(IDCANCEL);

	//
	m_aCenfEffType.RemoveAll();
	m_aCenfEffType.Add(IDC_CMD_FR_CENT_EFF_01_RDO);
	m_aCenfEffType.Add(IDC_CMD_FR_CENT_EFF_02_RDO);
	m_aCenfEffType.Add(IDC_CMD_FR_CENT_EFF_03_RDO);
}


void CCMMvldSubItemFRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldSubItemFRDlg)
	DDX_Control(pDX, IDC_CMD_UNSEL_LIST , m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST   , m_wndSelList);
	DDX_Control(pDX, IDC_CMD_VEHICLE_CMB, m_wndVeh);

	DDX_Control(pDX, IDC_CMD_UNSEL2_LIST , m_wndUnselList2);
	DDX_Control(pDX, IDC_CMD_SEL2_LIST   , m_wndSelList2);
	DDX_Control(pDX, IDC_CMD_SIDEWALK_CMB, m_wndVeh2);

	//DDX_Control(pDX, IDC_CMD_VEHICLE_CENF_CMB, m_wndVehBc);

	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldSubItemFRDlg::Data2Dlg()
{
	T_MVHL_D mvhl;
	int nIndex;
	int nAllSize, nAllSize2;
	int nSelSize, nSideSize;
	int nMaxCount, nMaxCount2;
	CString LaneName;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCenfEffType, m_Data.nLoadEffType);

	//////////////////////////////////////////////////////////////////////////
	// Vehicle
	m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey, mvhl);
	m_wndVeh.SelectString(-1, mvhl.VehicleLoadName);

	// Select 된 것, 아닌 것 구분
	nAllSize = m_aVehLaneList.GetSize();
	nMaxCount = m_Data.aSelectedLanes.GetSize();

	for (nSelSize = 0; nSelSize < nMaxCount; nSelSize++)
		if (m_Data.aSelectedLanes[nSelSize] == 0) break;

	m_aSelFlag.SetSize(nAllSize);
	m_aUnSelFlag.SetSize(nAllSize);
	for (int i = 0; i < nAllSize; i++)
	{
		int j = 0;
		for (j = 0; j < nSelSize; j++)
			if (m_aVehLaneList[i] == m_Data.aSelectedLanes[j]) break;
		if (j == nSelSize) 
		{
			m_aSelFlag[i]   = FALSE; 
			m_aUnSelFlag[i] = TRUE;
		}
		else 
		{
			m_aSelFlag[i]   = TRUE;
			m_aUnSelFlag[i] = FALSE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Sidewalk
	m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.SidewalKey, mvhl);
	m_wndVeh2.SelectString(-1, mvhl.VehicleLoadName);

	// Select 된 것, 아닌 것 구분
	nAllSize2 = m_aVehLaneList2.GetSize();
	nMaxCount2 = m_Data.aSidewalkLanes.GetSize();

	for (nSideSize = 0; nSideSize < nMaxCount2; nSideSize++)
		if (m_Data.aSidewalkLanes[nSideSize] == 0) break;

	m_aSelFlag2.SetSize(nAllSize2);
	m_aUnSelFlag2.SetSize(nAllSize2);
	for (int i = 0; i < nAllSize2; i++)
	{
		int j = 0;
		for (j = 0; j < nSideSize; j++)
			if (m_aVehLaneList2[i] == m_Data.aSidewalkLanes[j]) break;
		if (j == nSideSize)
		{
			m_aSelFlag2[i] = FALSE;
			m_aUnSelFlag2[i] = TRUE;
		}
		else 
		{
			m_aSelFlag2[i] = TRUE;
			m_aUnSelFlag2[i] = FALSE;
		}
	}

	//-----
	if (m_nLoadModel == MILITARY)
	{
		ASSERT(nAllSize == nAllSize2);

		for (int i = 0; i < nAllSize; i++)
		{
			if(m_aSelFlag[i] == TRUE) m_aUnSelFlag2[i] = FALSE;

			if(m_aSelFlag2[i] == TRUE) m_aUnSelFlag[i] = FALSE;
		}
	}
	//-----

	// Add String - Vehicle
	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();

	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aVehLaneList[i]);
		if (m_aSelFlag[i])
		{
			nIndex = m_wndSelList.AddString(LaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
		if (m_aUnSelFlag[i])
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}

	// Add String - Sidewalk
	m_wndSelList2.ResetContent();
	m_wndUnselList2.ResetContent();

	for (int i = 0; i < nAllSize2; i++)
	{
		LaneName = GetLaneName(m_aVehLaneList2[i]);
		if (m_aSelFlag2[i])
		{
			nIndex = m_wndSelList2.AddString(LaneName);
			m_wndSelList2.SetItemData(nIndex, i);
		}
		if (m_aUnSelFlag2[i])
		{
			nIndex = m_wndUnselList2.AddString(LaneName);
			m_wndUnselList2.SetItemData(nIndex, i);
		}
	}
}

BOOL CCMMvldSubItemFRDlg::Dlg2Data()
{
	m_Data.Initialize();
	int nCurSel;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCenfEffType, m_Data.nLoadEffType);

	//////////////////////////////////////////////////////////////////////////
	// Vehicle
	nCurSel = m_wndVeh.GetCurSel();
	if (nCurSel < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected__nChoose_one_));
		return FALSE;
	}
	m_Data.VehicleKey = m_wndVeh.GetItemData(nCurSel);

	//////////////////////////////////////////////////////////////////////////
	// Sidewalk
	nCurSel = m_wndVeh2.GetCurSel();
	if (nCurSel < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected__nChoose_one_));
		return FALSE;
	}
	m_Data.SidewalKey = m_wndVeh2.GetItemData(nCurSel);

	if(!ErrorCheck()) return FALSE;

	return TRUE;
}

BOOL CCMMvldSubItemFRDlg::ErrorCheck()
{
	// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
	int nMaxLane = 20;

	int nSize;
	int nCount;
	CString csMsg;

	//////////////////////////////////////////////////////////////////////////
	// Vehicle
	nSize = m_aVehLaneList.GetSize();
	nCount = 0;
	for (int i=0; i<nSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			if (nCount >= 1)
			{
				csMsg.Format(_LS(IDS_CMD_MVLD_FR_VEHICLE_LANE_ONLY_ONE));
				AfxMessageBox(csMsg);
				return FALSE;
			}
			m_Data.aSelectedLanes.Add(m_aVehLaneList[i]);
			nCount++;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Sidewalk
	nSize = m_aVehLaneList2.GetSize();
	nCount = 0;
	for (int i=0; i<nSize; i++)
	{
		if (m_aSelFlag2[i]) 
		{
			if (nCount >= nMaxLane)
			{
				csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
				AfxMessageBox(csMsg);
				return FALSE;
			}
			m_Data.aSidewalkLanes.Add(m_aVehLaneList2[i]);
			nCount++;
		}
	}
	
	if (m_nLoadModel != PEDESTRIAN)
	{
		if ((m_Data.VehicleKey == 0 && m_Data.SidewalKey == 0)              ||
			(m_Data.VehicleKey == 0 && m_Data.aSelectedLanes.GetSize() >  0)||
			(m_Data.VehicleKey  > 0 && m_Data.aSelectedLanes.GetSize() != 1))
		{
			AfxMessageBox(_LS(IDS_CMD_MVLD_FR_VEHICLE_LANE_NOT)); return FALSE;
		}
	}

	if ((m_Data.SidewalKey == 0 && m_Data.aSidewalkLanes.GetSize() > 0) ||
		(m_Data.SidewalKey > 0  && m_Data.aSidewalkLanes.GetSize() == 0))
	{
		if (m_nLoadModel == PEDESTRIAN)
		{
			AfxMessageBox(_LS(IDS_CMD_MVLD_FR_PEDESTRIAN_LANE_NOT)); return FALSE;
		}
		else
		{
			AfxMessageBox(_LS(IDS_CMD_MVLD_FR_SIDEWALK_LANE_NOT)); return FALSE;
		}

	}

	if (m_nLoadModel == PEDESTRIAN)
	{
		if (m_Data.SidewalKey == 0 && m_Data.aSidewalkLanes.GetSize() == 0)
		{
			AfxMessageBox(_LS(IDS_CMD_MVLD_FR_PEDESTRIAN_LANE_NOT)); return FALSE;
		}
	}

	return TRUE;
}

BOOL CCMMvldSubItemFRDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	T_MVCD_D mvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData))ASSERT(0);
	if(mvcdData.nCodeType == D_MOVE_CODE_FRANCE)
		return m_pMvldItemFRDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else
	{
		ASSERT(0);
		return FALSE;
	}
	
}

void CCMMvldSubItemFRDlg::InitCombo()
{
	//////////////////////////////////////////////////////////////////////////
	// Vehicle
	m_wndVeh.ResetContent();

	CDlgUtil::CobxAddItem(m_wndVeh, _LS(IDS_WG_CMD_NONE), 0);

	CArray<T_MVHL_K, T_MVHL_K> aMvhlK;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVHL_D MvhlD;
	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		MvhlD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if(m_nLoadModel == SYSTEM_A)
		{
			if (MvhlD.nStandardCode != D_MVHL_FRANCE_SYSTEM_A) continue;
		}
		else if (m_nLoadModel == SYSTEM_Bc)
		{
			if(MvhlD.VehicleTypeName != _T("Load System Bc")) continue;
		}
		else if (m_nLoadModel == SYSTEM_Bt)
		{
			if (MvhlD.VehicleTypeName != _T("Load System Bt")) continue;
		}
		else if (m_nLoadModel == MILITARY)
		{
			if (MvhlD.nStandardCode != D_MVHL_FRANCE_MILITARY_LOAD) continue;
		}

		CDlgUtil::CobxAddItem(m_wndVeh, MvhlD.VehicleLoadName, aMvhlK[i]);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_wndVeh);
	m_wndVeh.SetCurSel(0);


	//////////////////////////////////////////////////////////////////////////
	// Sidewalk
	m_wndVeh2.ResetContent();

	CDlgUtil::CobxAddItem(m_wndVeh2, _LS(IDS_WG_CMD_NONE), 0);

	aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		MvhlD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if (MvhlD.nStandardCode != D_MVHL_FRANCE_SIDEWALK) continue;  // Load on Sidewalk

		if (m_nLoadModel == PEDESTRIAN)
		{
			if (MvhlD.VehicleTypeName != _T("Pedestrian")) continue;
		}
		else
		{
			if (MvhlD.VehicleTypeName != _T("Sidewalk")) continue;
		}

		CDlgUtil::CobxAddItem(m_wndVeh2, MvhlD.VehicleLoadName, aMvhlK[i]);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_wndVeh2);
	m_wndVeh2.SetCurSel(0);

}

CString CCMMvldSubItemFRDlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLANfr_D slan;
		m_pDoc->m_pAttrCtrl->GetSlanfr(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_LLANfr_D llan;
		m_pDoc->m_pAttrCtrl->GetLlanfr(nLaneKey, llan);
		return llan.LineLaneName;
	}
}

int  CCMMvldSubItemFRDlg::GetLaneType(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLANfr_D slan;
		m_pDoc->m_pAttrCtrl->GetSlanfr(nLaneKey, slan);
		return slan.nLineType;
	}
	else
	{
		T_LLANfr_D llan;
		m_pDoc->m_pAttrCtrl->GetLlanfr(nLaneKey, llan);
		return llan.nLineType;
	}
}

void CCMMvldSubItemFRDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;
	int nDistX = 0;

	if (m_nLoadModel == SYSTEM_Bc) // Bc
		return;

	if (m_nLoadModel == PEDESTRIAN) //Pedestrian
	{
		// Side walk lane move
		GetDlgItem(IDC_CMD_VEHICLE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SIDEWORK_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlSidewalk, nDistY);
	}
	else if(m_nLoadModel == SYSTEM_A || m_nLoadModel == SYSTEM_Bt || m_nLoadModel == MILITARY) //A, Bt, Miliatary
	{
		// Vehicle Lane resize
		CRect rectResize;
		GetDlgItem(IDC_CMD_VEHICLE_GRP)->GetWindowRect(rectResize);
		GetDlgItem(IDC_CMD_UNSEL_LIST)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(6);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_CMD_VEHICLE_GRP)->MoveWindow(rectResize);

		// Side walk lane
		GetDlgItem(IDC_CMD_VEHICLE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SIDEWORK_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(6);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlSidewalk, nDistY);

	}
	else ASSERT(0);

	// OK NG move
	GetDlgItem(IDC_CMD_SIDEWORK_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(6);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkNg, nDistY);

	//
	ResizeDialog();
}

void CCMMvldSubItemFRDlg::ResizeDialog()
{
	UINT nLastCtrlID = IDCANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + 16;

	MoveWindow(r);
}

void CCMMvldSubItemFRDlg::ShowHIdeCtrl()
{
	CDlgUtil::CtrlShowHide(this, m_aCtrlVehicle, m_nLoadModel != PEDESTRIAN);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCentEff, m_nLoadModel == SYSTEM_Bc);
}

BEGIN_MESSAGE_MAP(CCMMvldSubItemFRDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldSubItemFRDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD   , OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST  , OnDblclkCmdSelList)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST, OnDblclkCmdUnselList)
	ON_BN_CLICKED(IDC_CMD_VEHICLE_CMB, OnCmbVeh)

	ON_BN_CLICKED(IDC_CMD_BTN2_ADD   , OnCmdBtnAdd2)
	ON_BN_CLICKED(IDC_CMD_BTN2_DELETE, OnCmdBtnDelete2)
	ON_LBN_DBLCLK(IDC_CMD_SEL2_LIST  , OnDblclkCmdSelList2)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL2_LIST, OnDblclkCmdUnselList2)
	ON_BN_CLICKED(IDC_CMD_SIDEWALK_CMB, OnCmbVeh2)

// 	ON_BN_CLICKED(IDC_CMD_FR_CENT_EFF_01_RDO, OnCmdLoadEffRdo)
// 	ON_BN_CLICKED(IDC_CMD_FR_CENT_EFF_02_RDO, OnCmdLoadEffRdo)
// 	ON_BN_CLICKED(IDC_CMD_FR_CENT_EFF_03_RDO, OnCmdLoadEffRdo)

	//ON_BN_CLICKED(IDC_CMD_VEHICLE_CENF_CMB, OnCmbVehBc)


	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemFRDlg message handlers
BOOL CCMMvldSubItemFRDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if (m_pDoc->m_pAttrCtrl->GetCountSlanfr() > 0) m_bSurface = TRUE;
	else                                           m_bSurface = FALSE;

	if (!m_bModify)
	{
		m_Data.Initialize();
	}
	
	CArray<UINT, UINT> aLaneList;
	if (m_bSurface) 
	{
		m_pDoc->m_pAttrCtrl->GetSlanfrKeyList(aLaneList);
	}
	else            
	{
		m_pDoc->m_pAttrCtrl->GetLlanfrKeyList(aLaneList);
	}

	int nAllSize = aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		int nLaneType = GetLaneType(aLaneList[i]);
		if(m_nLoadModel== MILITARY)
		{
			if (nLaneType == 1)
			{
				m_aVehLaneList.Add(aLaneList[i]);
			}
		}
		else
		{
			if (nLaneType == 0)
			{
				m_aVehLaneList.Add(aLaneList[i]);
			}
		}

		if (nLaneType==1)
		{
			m_aVehLaneList2.Add(aLaneList[i]);
		}
	}

	CString str1, str2;
	if (m_nLoadModel == PEDESTRIAN)
	{
		str1 = _LS(IDS_CMD_MVLD_PEDESTRIAN_LANE_GROUP);
		str2 = _LS(IDS_CMD_MVLD_PEDESTRIAN_VEHICLE);
	}
	else
	{
		str1 = _LS(IDS_CMD_MVLD_SIDEWALK_LANE_GROUP);
		str2 = _LS(IDS_CMD_MVLD_SIDEWALK_VEHICLE);
	}
	GetDlgItem(IDC_CMD_SIDEWORK_GRP)->SetWindowText(str1);
	GetDlgItem(IDC_CMD_SIDEWALK_STC)->SetWindowText(str2);


	AlignControl();
	ShowHIdeCtrl();

	InitCombo();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldSubItemFRDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldSubItemFRDlg::OnCmdBtnAdd() 
{
	int nIndex, nIndex2;
	int nMax;
	int nCount, nCount2;
	int nAllSize, nAllSize2;
	CString strName, strName2;

	//////////////////////////////////////////////////////////////////////////
	// Vehicle
	nMax = m_aVehLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	nCount = m_wndUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;

	// check
	int nNum = 0;
	for (int i = 0; i < m_aSelFlag.GetSize(); i++)
	{
		if(m_aSelFlag[i]==TRUE) nNum++;
	}
	if(nCount != 1 || nNum>=1)
	{
		AfxMessageBox(_LS(IDS_CMD_MVLD_FR_VEHICLE_LANE_ONLY_ONE));
		return;
	}
	//

	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndUnselList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_aUnSelFlag[nIndex] = FALSE;
		m_wndUnselList.GetText(aIndex[i], strName);
		m_wndUnselList.DeleteString(aIndex[i]);

		//-----	
		if (m_nLoadModel == MILITARY)
		{
			nCount2 = m_wndUnselList2.GetCount();

			for (int j = nCount2-1; j >= 0; j--)
			{
				nIndex2 = m_wndUnselList2.GetItemData(j);

				m_wndUnselList2.GetText(j, strName2);

				if (strName == strName2)
				{
					m_aUnSelFlag2[nIndex2] = FALSE;
				}
			}
		}
		//-----
	}
	
	CString csLaneName;
	m_wndSelList.ResetContent();
	nAllSize = m_aVehLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aVehLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}

	//-----
	if (m_nLoadModel == MILITARY)
	{
		m_wndUnselList2.ResetContent();
		nAllSize2 = m_aVehLaneList2.GetSize();
		for (int i = 0; i < nAllSize2; i++)
		{
			csLaneName = GetLaneName(m_aVehLaneList2[i]);
			if (m_aUnSelFlag2[i])
			{
				nIndex = m_wndUnselList2.AddString(csLaneName);
				m_wndUnselList2.SetItemData(nIndex, i);
			}
		}
	}
	//-----
}

void CCMMvldSubItemFRDlg::OnCmdBtnAdd2()
{
	int nIndex, nIndex2;
	int  nMax2;
	int nCount, nCount2;
	int nAllSize, nAllSize2;
	CString strName, strName2;

	//////////////////////////////////////////////////////////////////////////
	// Sidewalk
	nMax2 = m_aVehLaneList2.GetSize();
	if (nMax2 <= 0) return;
	CArray<int, int> aIndex2;
	aIndex2.SetSize(nMax2);
	nCount2 = m_wndUnselList2.GetSelItems(nMax2, aIndex2.GetData());
	if (nCount2 == 0) return;

	for (int i = nCount2 - 1; i >= 0; i--)
	{
		nIndex2 = m_wndUnselList2.GetItemData(aIndex2[i]);
		m_aSelFlag2[nIndex2] = TRUE;
		m_aUnSelFlag2[nIndex2] = FALSE;
		m_wndUnselList2.GetText(aIndex2[i], strName2);
		m_wndUnselList2.DeleteString(aIndex2[i]);

		//-----	
		if (m_nLoadModel == MILITARY)
		{
			nCount = m_wndUnselList.GetCount();

			for (int j = nCount - 1; j >= 0; j--)
			{
				nIndex = m_wndUnselList.GetItemData(j);

				m_wndUnselList.GetText(j, strName);

				if (strName == strName2)
				{
					m_aUnSelFlag[nIndex] = FALSE;
				}
			}
		}
		//-----
	}

	CString csLaneName;
	m_wndSelList2.ResetContent();
	nAllSize2 = m_aVehLaneList2.GetSize();
	for (int i = 0; i < nAllSize2; i++)
	{
		csLaneName = GetLaneName(m_aVehLaneList2[i]);
		if (m_aSelFlag2[i])
		{
			nIndex2 = m_wndSelList2.AddString(csLaneName);
			m_wndSelList2.SetItemData(nIndex2, i);
		}
	}

	//-----
	if (m_nLoadModel == MILITARY)
	{
		m_wndUnselList.ResetContent();
		nAllSize = m_aVehLaneList.GetSize();
		for (int i = 0; i < nAllSize; i++)
		{
			csLaneName = GetLaneName(m_aVehLaneList[i]);
			if (m_aUnSelFlag[i])
			{
				nIndex = m_wndUnselList.AddString(csLaneName);
				m_wndUnselList.SetItemData(nIndex, i);
			}
		}
	}
	//-----
}

void CCMMvldSubItemFRDlg::OnCmdBtnDelete() 
{
	int nIndex, nIndex2;
	int nMax;
	int nCount;
	int nAllSize, nAllSize2;
	CString strName, strName2;

	//////////////////////////////////////////////////////////////////////////
	// Vehicle
	nMax = m_aVehLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_aUnSelFlag[nIndex] = TRUE;
		m_wndSelList.DeleteString(aIndex[i]);
		//-----
		if (m_nLoadModel == MILITARY)
			m_aUnSelFlag2[nIndex] = TRUE; 
		//-----
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	nAllSize = m_aVehLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aVehLaneList[i]);
		if (m_aUnSelFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}

	//-----
	if (m_nLoadModel == MILITARY)
	{
		m_wndUnselList2.ResetContent();
		nAllSize2 = m_aVehLaneList2.GetSize();
		for (int i = 0; i < nAllSize2; i++)
		{
			csLaneName = GetLaneName(m_aVehLaneList2[i]);
			if (m_aUnSelFlag2[i])
			{
				nIndex2 = m_wndUnselList2.AddString(csLaneName);
				m_wndUnselList2.SetItemData(nIndex2, i);
			}
		}
	}
	//-----
}

void CCMMvldSubItemFRDlg::OnCmdBtnDelete2()
{
	int nIndex, nIndex2;
	int nMax2;
	int nCount2;
	int nAllSize, nAllSize2;

	//////////////////////////////////////////////////////////////////////////
	// Sidewalk
	nMax2 = m_aVehLaneList2.GetSize();
	if (nMax2 <= 0) return;
	CArray<int, int> aIndex2;
	aIndex2.SetSize(nMax2);
	nCount2 = m_wndSelList2.GetSelItems(nMax2, aIndex2.GetData());
	if (nCount2 == 0) return;
	for (int i = nCount2 - 1; i >= 0; i--)
	{
		nIndex2 = m_wndSelList2.GetItemData(aIndex2[i]);
		m_aSelFlag2[nIndex2] = FALSE;
		m_aUnSelFlag2[nIndex2] = TRUE; 
		m_wndSelList2.DeleteString(aIndex2[i]);
		//-----
		if (m_nLoadModel == MILITARY)
			m_aUnSelFlag[nIndex2] = TRUE;
		//-----
	}

	CString csLaneName;
	m_wndUnselList2.ResetContent();
	nAllSize2 = m_aVehLaneList2.GetSize();
	for (int i = 0; i < nAllSize2; i++)
	{
		csLaneName = GetLaneName(m_aVehLaneList2[i]);
		if (m_aUnSelFlag2[i])
		{
			nIndex2 = m_wndUnselList2.AddString(csLaneName);
			m_wndUnselList2.SetItemData(nIndex2, i);
		}
	}

	//-----
	if (m_nLoadModel == MILITARY)
	{
		m_wndUnselList.ResetContent();
		nAllSize = m_aVehLaneList.GetSize();
		for (int i = 0; i < nAllSize; i++)
		{
			csLaneName = GetLaneName(m_aVehLaneList[i]);
			if (m_aUnSelFlag[i])
			{
				nIndex = m_wndUnselList.AddString(csLaneName);
				m_wndUnselList.SetItemData(nIndex, i);
			}
		}
	}
	//-----
}

void CCMMvldSubItemFRDlg::OnDblclkCmdSelList() 
{
	OnCmdBtnDelete();
}

void CCMMvldSubItemFRDlg::OnDblclkCmdSelList2()
{
	OnCmdBtnDelete2();
}

void CCMMvldSubItemFRDlg::OnDblclkCmdUnselList() 
{
	OnCmdBtnAdd();
}

void CCMMvldSubItemFRDlg::OnDblclkCmdUnselList2()
{
	OnCmdBtnAdd2();
}

void CCMMvldSubItemFRDlg::OnCmbVeh()
{
	//////////////////////////////////////////////////////////////////////////
	// Vehicle
	T_MVHL_K MvhlKey;
	int nCurSel = m_wndVeh.GetCurSel();
	if (nCurSel >= 0) 
	MvhlKey = m_wndVeh.GetItemData(nCurSel);
	InitCombo();
	CString csName;
	if (nCurSel >= 0) // 예전 선택 복구
	{
		T_MVHL_D mvhl;
		if (m_pDoc->m_pAttrCtrl->GetMvhl(MvhlKey, mvhl))
			csName = /*_T("VL:") +*/ mvhl.VehicleLoadName;
		else csName = _T("");
	}
	else csName = _T("");
	if (csName == _T("")) m_wndVeh.SetCurSel(0);
	else m_wndVeh.SelectString(-1, csName);
}

void CCMMvldSubItemFRDlg::OnCmbVeh2()
{
	//////////////////////////////////////////////////////////////////////////
	// Sidewalk
	T_MVLD_TYPE_KEY TypeKey;
	int nCurSel = m_wndVeh2.GetCurSel();
	if (nCurSel >= 0)
		TypeKey.value = m_wndVeh2.GetItemData(nCurSel);
	InitCombo();
	CString csName;
	if (nCurSel >= 0) // 예전 선택 복구
	{
		T_MVHL_D mvhl;
		if (m_pDoc->m_pAttrCtrl->GetMvhl(TypeKey.typekey.key, mvhl))
			csName = /*_T("VL:") +*/ mvhl.VehicleLoadName;
		else csName = _T("");
	}
	else csName = _T("");
	if (csName == _T("")) m_wndVeh2.SetCurSel(0);
	else m_wndVeh2.SelectString(-1, csName);
}

// void CCMMvldSubItemFRDlg::OnCmdLoadEffRdo()
// {
// 	CDlgUtil::CtrlRadioGetCheck(this, m_aCenfEffType, m_Data.nLoadEffType);
// }