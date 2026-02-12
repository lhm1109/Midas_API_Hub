// CMMvhlItemUsrPolandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrPolandDlg.h"
#include "CMMvhlItemUsrPermitDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
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
// CCMMvhlItemUsrPolandDlg dialog

#define COLCOUNT 3

CCMMvhlItemUsrPolandDlg::CCMMvhlItemUsrPolandDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrPolandDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrPolandDlg)
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_nLoadType = 0;

	m_Data.Initialize();

	m_bPermitLoadCall = FALSE;
}

void CCMMvhlItemUsrPolandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrPolandDlg)  
	DDX_Radio  (pDX, IDC_CMD_USER_TRUCK_RDO,               m_nLoadType);
	DDX_Control(pDX, IDC_CMD_USER_TRUCK_CMB,               m_cmbTruck);
	DDX_Control(pDX, IDC_CMD_USER_MILITARY_CMB,            m_cmbMilitary);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_NAME_EDT,           m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_P_EDT,		   m_edtTrkP);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_D_EDT,	       m_edtTrkD);
	DDX_Control(pDX, IDC_CMD_LIST,				           m_List);	
	DDX_Control(pDX, IDC_CMD_USER_MVHL_Q_EDT,              m_edtQ);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_Q_UNT,              m_untQ);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_A_EDT,              m_edtA);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_A_UNT,              m_untA);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNAMIC_CHK,             m_chkDynamic);	
	DDX_Control(pDX, IDC_CMD_MVHL_PHI_EDT,                 m_edtPhi);	
	DDX_Control(pDX, IDC_CMD_MVHL_WHEEL_SPACING_EDT1,      m_edtWheelSpacing1);	
	DDX_Control(pDX, IDC_CMD_MVHL_WHEEL_SPACING_UNT1,      m_untWheelSpacing1);	
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT1,     m_edtNoseTailDist1);	
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT1,     m_untNoseTailDist1);	
	DDX_Control(pDX, IDC_CMD_MVHL_NUM_VEHICLE_EDT1,        m_edtNumVehicle1);	
	DDX_Control(pDX, IDC_CMD_MVHL_TOTAL_LOAD_EDT,          m_edtTotalLoad);	
	DDX_Control(pDX, IDC_CMD_MVHL_TOTAL_LOAD_UNT,          m_untTotalLoad);	
	DDX_Control(pDX, IDC_CMD_MVHL_TRACKED_LENGTH_EDT,      m_edtTrackedLength);	
	DDX_Control(pDX, IDC_CMD_MVHL_TRACKED_LENGTH_UNT,      m_untTrackedLength);
	DDX_Control(pDX, IDC_CMD_MVHL_WHEEL_SPACING_EDT2,      m_edtWheelSpacing2);	
	DDX_Control(pDX, IDC_CMD_MVHL_WHEEL_SPACING_UNT2,      m_untWheelSpacing2);	
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT2,     m_edtNoseTailDist2);	
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT2,     m_untNoseTailDist2);	
	DDX_Control(pDX, IDC_CMD_MVHL_NUM_VEHICLE_EDT2,        m_edtNumVehicle2);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_PICTURE,			   m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrPolandDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrPolandDlg)
	ON_BN_CLICKED(IDC_CMD_USER_TRUCK_RDO,        OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_MILITARY_RDO,     OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_PERMIT_RDO,       OnCmdMvhlLoadTypeRdo)
	ON_CBN_SELCHANGE(IDC_CMD_USER_TRUCK_CMB,     OnCmdMvhlTruckCmb)
	ON_CBN_SELCHANGE(IDC_CMD_USER_MILITARY_CMB,  OnCmdMvhlMilitaryCmb)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,               OnCmdMvhlAddBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT,            OnCmdMvhlInsBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY,            OnCmdMvhlModBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,            OnCmdMvhlDelBtn)
	ON_BN_CLICKED(IDC_CMD_APPLY,                 OnCmdMvhlApplyBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST,     OnItemchangedCmdList)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNAMIC_CHK,      OnCmdDynamic)
	ON_BN_CLICKED(IDC_CMD_MVHL_AUTO_RDO,         OnCmdDynamic)
	ON_BN_CLICKED(IDC_CMD_MVHL_USER_RDO,         OnCmdDynamic)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrPolandDlg::InitUnit()
{
	m_edtTrkP.SetUnitType(D_UNITSYS_NONE);
	m_edtTrkD.SetUnitType(D_UNITSYS_NONE);
	m_edtQ.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPressureLoad);
	m_untQ.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPressureLoad);
	m_edtA.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dSpace);
	m_untA.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dSpace);
	m_edtPhi.SetUnitType(D_UNITSYS_NONE);
	m_edtWheelSpacing1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWheelSpacing);
	m_untWheelSpacing1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWheelSpacing);
	m_edtNoseTailDist1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untNoseTailDist1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_edtNumVehicle1.SetUnitType(D_UNITSYS_NONE);
	m_edtTotalLoad.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTotalLoad);
	m_untTotalLoad.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTotalLoad);
	m_edtTrackedLength.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrackedLength);
	m_untTrackedLength.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrackedLength);
	m_edtWheelSpacing2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWheelSpacing);
	m_untWheelSpacing2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWheelSpacing);
	m_edtNoseTailDist2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untNoseTailDist2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_edtNumVehicle2.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvhlItemUsrPolandDlg::InitCombo()
{
	// 이 부분은 번역이 필요 없을것 같은데....음...
	m_cmbTruck.ResetContent();
	m_cmbTruck.AddString(_LS(IDS_WG_CMD_Vehicle_K));
	m_cmbTruck.AddString(_LS(IDS_WG_CMD_Vehicle_S));
	m_cmbTruck.AddString(_LS(IDS_WG_CMD_Vehicle_2S));
	m_cmbTruck.AddString(_LS(IDS_WG_CMD_Walkway));
	m_cmbTruck.SetCurSel(0);

	m_cmbMilitary.ResetContent();
	m_cmbMilitary.AddString(_LS(IDS_WG_CMD_Tracked_Vehicle));
	m_cmbMilitary.AddString(_LS(IDS_WG_CMD_Wheeled_Vehicle));
	m_cmbMilitary.SetCurSel(0);
}

void CCMMvhlItemUsrPolandDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	int nDistY = 0;	

	// a
	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_MVHL_A_STC);
	aControls.Add(IDC_CMD_USER_MVHL_A_EDT);
	aControls.Add(IDC_CMD_USER_MVHL_A_UNT);

	GetDlgItem(IDC_CMD_USER_MVHL_Q_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_USER_MVHL_A_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Wheeled Vehicle
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_STC1);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_EDT1);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_UNT1);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_STC1);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT1);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT1);
	aControls.Add(IDC_CMD_MVHL_NUM_VEHICLE_STC1);
	aControls.Add(IDC_CMD_MVHL_NUM_VEHICLE_EDT1);

	GetDlgItem(IDC_CMD_USER_MVHL_Q_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_WHEEL_SPACING_STC1)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Tracked Vehicle
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_TOTAL_LOAD_STC);
	aControls.Add(IDC_CMD_MVHL_TOTAL_LOAD_EDT);
	aControls.Add(IDC_CMD_MVHL_TOTAL_LOAD_UNT);
	aControls.Add(IDC_CMD_MVHL_TRACKED_LENGTH_STC);
	aControls.Add(IDC_CMD_MVHL_TRACKED_LENGTH_EDT);
	aControls.Add(IDC_CMD_MVHL_TRACKED_LENGTH_UNT);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_STC2);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_EDT2);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_UNT2);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_STC2);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT2);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT2);
	aControls.Add(IDC_CMD_MVHL_NUM_VEHICLE_STC2);
	aControls.Add(IDC_CMD_MVHL_NUM_VEHICLE_EDT2);

	GetDlgItem(IDC_CMD_USER_VEHI_TRUCK_P_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_TOTAL_LOAD_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);  
}

void CCMMvhlItemUsrPolandDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	int nSubType = 0;
	if     (m_nLoadType == 0) nSubType = m_cmbTruck.GetCurSel();
	else if(m_nLoadType == 1) nSubType = m_cmbMilitary.GetCurSel();
	else                      nSubType = 0;

	// List
	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_GRP);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_P_STC);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_P_EDT);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_D_STC);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_D_EDT);
	aControls.Add(IDC_CMD_LIST);
	aControls.Add(IDC_CMD_BTN_ADD);
	aControls.Add(IDC_CMD_BTN_INSERT);
	aControls.Add(IDC_CMD_BTN_MODIFY);
	aControls.Add(IDC_CMD_BTN_DELETE);
	CDlgUtil::CtrlShowHide(this, aControls, m_nLoadType == 0 || (m_nLoadType == 1 && nSubType == 1));

	// Q
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_MVHL_Q_STC);
	aControls.Add(IDC_CMD_USER_MVHL_Q_EDT);
	aControls.Add(IDC_CMD_USER_MVHL_Q_UNT);
	CDlgUtil::CtrlShowHide(this, aControls, (m_nLoadType == 0 && (nSubType == 0 || nSubType == 1 || nSubType == 3)));

	// A
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_MVHL_A_STC);
	aControls.Add(IDC_CMD_USER_MVHL_A_EDT);
	aControls.Add(IDC_CMD_USER_MVHL_A_UNT);
	CDlgUtil::CtrlShowHide(this, aControls, m_nLoadType == 0 && nSubType == 2);
	
	// Wheeled Vehicle
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_STC1);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_EDT1);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_UNT1);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_STC1);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT1);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT1);
	aControls.Add(IDC_CMD_MVHL_NUM_VEHICLE_STC1);
	aControls.Add(IDC_CMD_MVHL_NUM_VEHICLE_EDT1);
	CDlgUtil::CtrlShowHide(this, aControls, m_nLoadType == 1 && nSubType == 1);
	
	// Tracked Vehicle
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_TOTAL_LOAD_STC);
	aControls.Add(IDC_CMD_MVHL_TOTAL_LOAD_EDT);
	aControls.Add(IDC_CMD_MVHL_TOTAL_LOAD_UNT);
	aControls.Add(IDC_CMD_MVHL_TRACKED_LENGTH_STC);
	aControls.Add(IDC_CMD_MVHL_TRACKED_LENGTH_EDT);
	aControls.Add(IDC_CMD_MVHL_TRACKED_LENGTH_UNT);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_STC2);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_EDT2);
	aControls.Add(IDC_CMD_MVHL_WHEEL_SPACING_UNT2);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_STC2);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT2);
	aControls.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT2);
	aControls.Add(IDC_CMD_MVHL_NUM_VEHICLE_STC2);
	aControls.Add(IDC_CMD_MVHL_NUM_VEHICLE_EDT2);
	CDlgUtil::CtrlShowHide(this, aControls, m_nLoadType == 1 && nSubType == 0);
}

void CCMMvhlItemUsrPolandDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	int nSubType = 0;
	if     (m_nLoadType == 0) nSubType = m_cmbTruck.GetCurSel();
	else if(m_nLoadType == 1) nSubType = m_cmbMilitary.GetCurSel();
	else                      nSubType = 0;

	GetDlgItem(IDC_CMD_USER_TRUCK_CMB)->EnableWindow(m_nLoadType == 0);
	GetDlgItem(IDC_CMD_USER_MILITARY_CMB)->EnableWindow(m_nLoadType == 1);

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_P_STC);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_P_EDT);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_D_STC);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_D_EDT);
	aControls.Add(IDC_CMD_LIST);
	aControls.Add(IDC_CMD_BTN_ADD);
	aControls.Add(IDC_CMD_BTN_INSERT);
	aControls.Add(IDC_CMD_BTN_MODIFY);
	aControls.Add(IDC_CMD_BTN_DELETE);
	aControls.Add(IDC_CMD_MVHL_DYNAMIC_CHK);
	aControls.Add(IDC_CMD_MVHL_AUTO_RDO);
	aControls.Add(IDC_CMD_MVHL_USER_RDO);
	aControls.Add(IDC_CMD_MVHL_PHI_EQUATION_TXT);
	aControls.Add(IDC_CMD_MVHL_PHI_TXT);
	aControls.Add(IDC_CMD_MVHL_PHI_EDT);
	CDlgUtil::CtrlEnableDisable(this, aControls, !(m_nLoadType == 0 && nSubType == 3));

	CArray<UINT,UINT> aRadio;
	int nAuto = 0;
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this, aRadio, nAuto);

	BOOL bDynamic = m_chkDynamic.GetCheck();
	GetDlgItem(IDC_CMD_MVHL_AUTO_RDO)->EnableWindow(!(m_nLoadType == 0 && nSubType == 3) && bDynamic);
	GetDlgItem(IDC_CMD_MVHL_USER_RDO)->EnableWindow(!(m_nLoadType == 0 && nSubType == 3) && bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->EnableWindow(!(m_nLoadType == 0 && nSubType == 3) && bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->EnableWindow(!(m_nLoadType == 0 && nSubType == 3) && bDynamic && (nAuto==1));
	GetDlgItem(IDC_CMD_MVHL_PHI_EDT)->EnableWindow(!(m_nLoadType == 0 && nSubType == 3) && bDynamic && (nAuto==1));	
}

void CCMMvhlItemUsrPolandDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing)};
	
	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	
	int nColWidth[COLCOUNT];
	
	CString title;
	int i;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 80;
	
	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemUsrPolandDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	if (nLoadCount == 0) return;
	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);
	
	if (nLoadCount > nDistCount)
		InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
}

BOOL CCMMvhlItemUsrPolandDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;
	
	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if(i == 0) m_List.InsertItem(&lvitem);
		else       m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMMvhlItemUsrPolandDlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUsrPolandDlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
{
	int nItem = nPos;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			if (i == 0) str.Format(_T("%d"), nNo);
			else if (i == 1) str.Format(_T("%g"), dLoad);
			else if (i == 2)
			{
				if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
				else str.Format(_T("%g"), dDist);
			}
			else str = _LS(IDS_WG_CMD__ADDD__Error);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

void CCMMvhlItemUsrPolandDlg::ChangeBitMap()
{	
	UpdateData(TRUE);

	int nSubType = 0;
	if     (m_nLoadType == 0) nSubType = m_cmbTruck.GetCurSel();
	else if(m_nLoadType == 1) nSubType = m_cmbMilitary.GetCurSel();
	else                      nSubType = 0;

	// 아직은 Bitmap이 한개임
	CString bitmapID;

	if(m_nLoadType == 0)
	{
		if     (nSubType == 0) bitmapID = _T("cmd_mvhl_poland_13.svg");
		else if(nSubType == 1) bitmapID = _T("cmd_mvhl_poland_13.svg");
		else if(nSubType == 2) bitmapID = _T("cmd_mvhl_poland_14.svg");
		else if(nSubType == 3) bitmapID = _T("cmd_mvhl_poland_15.svg");
		else ASSERT(0);
	}
	else if(m_nLoadType == 1)
	{
		if     (nSubType == 0) bitmapID = _T("cmd_mvhl_poland_9.svg");
		else if(nSubType == 1) bitmapID = _T("cmd_mvhl_poland_14.svg");
		else ASSERT(0);
	}
	else ASSERT(0);

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + bitmapID);
}

void CCMMvhlItemUsrPolandDlg::Data2Dlg()
{
	m_nLoadType = m_Data.nLoadType;
	if     (m_nLoadType == 0) m_cmbTruck.SetCurSel(m_Data.nSubType);
	else if(m_nLoadType == 1) m_cmbMilitary.SetCurSel(m_Data.nSubType);
	
	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	
	m_edtQ .SetEditUnit(m_Data.dPressureLoad);
	m_edtA.SetEditUnit(m_Data.dSpace);
	
	MakeItemEx();
	
	CArray<UINT,UINT> aRadio;
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	m_chkDynamic.SetCheck(m_Data.bDynamicFactor);
	int nRdo = (m_Data.bUserInput)?1:0;
	CDlgUtil::CtrlRadioSetCheck(this,aRadio,nRdo);
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data.dAmplification);
	m_edtPhi.SetWindowText(strTemp);

	m_edtWheelSpacing1.SetEditUnit(m_Data.dWheelSpacing);
	m_edtNoseTailDist1.SetEditUnit(m_Data.dNoseTailDist);
	m_edtNumVehicle1.SetEditUnit(m_Data.nNumVehicle);
	m_edtTotalLoad.SetEditUnit(m_Data.dTotalLoad);
	m_edtTrackedLength.SetEditUnit(m_Data.dTrackedLength);
	m_edtWheelSpacing2.SetEditUnit(m_Data.dWheelSpacing);
	m_edtNoseTailDist2.SetEditUnit(m_Data.dNoseTailDist);
	m_edtNumVehicle2.SetEditUnit(m_Data.nNumVehicle);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrPolandDlg::Dlg2Data()
{	
	UpdateData(TRUE);  
	m_Data.bStandard = FALSE;    
	m_Data.nLoadType = m_nLoadType;

	if     (m_nLoadType == 0) m_Data.nSubType = m_cmbTruck.GetCurSel();
	else if(m_nLoadType == 1) m_Data.nSubType = m_cmbMilitary.GetCurSel();
	else                      m_Data.nSubType = 0;

	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);	
	
	m_Data.dPressureLoad = m_edtQ.GetEditValue();
	m_Data.dSpace = m_edtA.GetEditValue();
	
	CArray<UINT,UINT> aRadio;  
	CString strTemp;
	BOOL bDynamic;
	bDynamic = m_chkDynamic.GetCheck();
	m_Data.bDynamicFactor = bDynamic;
	int nRdo;
	aRadio.RemoveAll();
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,aRadio,nRdo);
	m_Data.bUserInput = nRdo;
	m_edtPhi.GetWindowText(strTemp);
	m_Data.dAmplification = _tstof(strTemp);

	m_Data.dTotalLoad = m_edtTotalLoad.GetEditValue();
	m_Data.dTrackedLength = m_edtTrackedLength.GetEditValue();

	if(m_nLoadType == 1 && m_Data.nSubType == 0)
	{
		m_Data.dTotalLoad = m_edtTotalLoad.GetEditValue();
		m_Data.dTrackedLength = m_edtTrackedLength.GetEditValue();
		m_Data.dWheelSpacing = m_edtWheelSpacing2.GetEditValue();
		m_Data.dNoseTailDist = m_edtNoseTailDist2.GetEditValue();
		m_Data.nNumVehicle = m_edtNumVehicle2.GetEditValue();
	}
	else
	{
		m_Data.dWheelSpacing = m_edtWheelSpacing1.GetEditValue();
		m_Data.dNoseTailDist = m_edtNoseTailDist1.GetEditValue();
		m_Data.nNumVehicle = m_edtNumVehicle1.GetEditValue();
	}

	return TRUE;
}

int CCMMvhlItemUsrPolandDlg::GetLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUsrPolandDlg::GetDistCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CCMMvhlItemUsrPolandDlg::GetLoadAndDistValue(double &dLoad, double &dDist)
{
	if (!CFormulaEdit::GetEditValue(&m_edtTrkP, dLoad))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
		return FALSE;
	}
	if (!CFormulaEdit::GetEditValue(&m_edtTrkD, dDist))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
		return FALSE;
	}
	
	if (dLoad == 0.0) // Load 0은 허용 안함
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_value_must_not_be_ze));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CCMMvhlItemUsrPolandDlg::ApplyOrOK()
{
	BOOL bSuccess = FALSE;
	
	if (!Dlg2Data()) return FALSE;

	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);

	if (!bSuccess) return FALSE;
	
	return TRUE;
}

void CCMMvhlItemUsrPolandDlg::DelData_byWalkway()
{
	UpdateData(TRUE);

	int nSubType = 0;
	if     (m_nLoadType == 0) nSubType = m_cmbTruck.GetCurSel();
	else if(m_nLoadType == 1) nSubType = m_cmbMilitary.GetCurSel();
	else                      nSubType = 0;

	if(m_nLoadType == 0 && nSubType == 3)
	{
		m_List.DeleteAllItems();

		for(int i=0; i<D_MVHL_NUMLOAD; i++)
		{
			m_Data.dPointLoad[i] = 0.0;
			m_Data.dPointDistance[i] = 0.0;
		}

		CArray<UINT,UINT> aRadio;
		aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
		aRadio.Add(IDC_CMD_MVHL_USER_RDO);
		m_chkDynamic.SetCheck(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrPolandDlg message handlers

BOOL CCMMvhlItemUsrPolandDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	SetHeaderTitle();
		
	Data2Dlg();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
		
	UpdateData(FALSE);

	ChangeBitMap();

	OnCmdMvhlLoadTypeRdo();

	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->SetWindowText(_LS(IDS_WG_CMD_MVHL_POLAND_PHI_EQUATION));
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->SetWindowText(_LS(IDS_WG_CMD_MVHL_POLAND_PHI));
 
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrPolandDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvhlItemUsrPolandDlg::OnCmdMvhlLoadTypeRdo() 
{
	UpdateData(TRUE);

	DelData_byWalkway();
		
	if(m_nLoadType == 2)
	{
		if (m_bPermitLoadCall)
		{
			m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
			m_bPermitLoadCall = FALSE;
			CCMMvhlItemUsrPermitDlg dlg;
			dlg.SetInitPos(D_INIT_POS_LT);
			CString csRadioText[3];
			GetDlgItem(IDC_CMD_USER_TRUCK_RDO)->GetWindowText(csRadioText[0]);
			GetDlgItem(IDC_CMD_USER_MILITARY_RDO)->GetWindowText(csRadioText[1]);
			GetDlgItem(IDC_CMD_USER_PERMIT_RDO)->GetWindowText(csRadioText[2]);
	
			T_MVCD_D MvcdD; MvcdD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return;

			dlg.SetModifyData(m_bModify, m_csOldName, &m_Data, m_pDoc, csRadioText, MvcdD.nCodeType);
			dlg.DoModal();
			//Type Change이면
			int nLastEvent = dlg.GetNextAction();
			if (nLastEvent == CCMMvhlItemUsrPermitDlg::CHANGE_TYPE)
			{
				CRect r = dlg.GetLastWindowPos();
				CRect rOrg;
				GetWindowRect(rOrg);
				r.right = r.left + rOrg.Width();
				r.bottom = r.top + rOrg.Height();
				MoveWindow(r);
				ShowWindow(SW_SHOW);
				m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

				m_nLoadType = dlg.GetType();
				UpdateData(FALSE);

				ChangeBitMap();
				AlignControl();
				ControlsShowHide();
				EnableDisableControls();
			}
			else
			{
				CDialogMove::OnOK();
			}
		}
		else
		{
			m_bPermitLoadCall = TRUE;
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_USER_PERMIT_RDO, BN_CLICKED), 0);
		}
	}
	else
	{
		ChangeBitMap();
		AlignControl();
		ControlsShowHide();
		EnableDisableControls();
	}
}

void CCMMvhlItemUsrPolandDlg::OnCmdMvhlTruckCmb() 
{
	DelData_byWalkway();

	ChangeBitMap();
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
}

void CCMMvhlItemUsrPolandDlg::OnCmdMvhlMilitaryCmb() 
{
	ChangeBitMap();
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
}

void CCMMvhlItemUsrPolandDlg::OnCmdMvhlAddBtn() 
{
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	if (nLoadCount > nDistCount) // Last 입력된 경우 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Delete_last_item_and_try_));
		return;
	}
	
	if (dDist == 0.0) // Last
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
	}
	else
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
		m_Data.dPointDistance[nLoadCount] = dDist;
	}
	
	int no = nLoadCount+1;
	InsertItem(nLoadCount, no, dLoad, dDist);
	
	int nCount = m_List.GetItemCount();
	m_List.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrPolandDlg::OnCmdMvhlInsBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	
	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}
	
	// 데이타 이동하기
	for (int i = D_MVHL_NUMLOAD-1; i > iItem; i--)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i-1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i-1];
	}
	
	// 데이타 삽입한다.
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	
	// List에 Item 삽입
	InsertItem(iItem, iItem+1, dLoad, dDist);
	
	// List 번호 변경
	int nCount = m_List.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrPolandDlg::OnCmdMvhlModBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}
	
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	
	ModifyItem(iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUsrPolandDlg::OnCmdMvhlDelBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	int i = 0;
	for (i = iItem; i < D_MVHL_NUMLOAD-1; i++)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i+1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i+1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;
	
	m_List.DeleteItem(iItem);
	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	
	// List 번호 변경
	CString csVal;
	for (int i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUsrPolandDlg::OnCmdMvhlApplyBtn() 
{
	ApplyOrOK();
}

void CCMMvhlItemUsrPolandDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		m_edtTrkD.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance[iItem]);
		m_edtTrkD.SetWindowText(csVal);
	}
	
	*pResult = 0;
}

void CCMMvhlItemUsrPolandDlg::OnCmdDynamic() 
{
	EnableDisableControls();
}