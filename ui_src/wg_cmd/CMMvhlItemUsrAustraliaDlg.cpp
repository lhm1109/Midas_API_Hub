// CMMvhlItemUsrAustraliaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrAustraliaDlg.h"
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
// CCMMvhlItemUsrAustraliaDlg dialog

#define COLCOUNT 3

CCMMvhlItemUsrAustraliaDlg::CCMMvhlItemUsrAustraliaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrAustraliaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrAustraliaDlg)
	m_bFatigue = FALSE;
	m_bIgnoreLoco = FALSE;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_nLoadType = 0;
	m_bVariable = FALSE;

	m_Data.Initialize();

	m_bPermitLoadCall = FALSE;
}

void CCMMvhlItemUsrAustraliaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrAustraliaDlg)  
	DDX_Radio  (pDX, IDC_CMD_USER_TRK_RDO,                 m_nLoadType);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_NAME_EDT,           m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_P_EDT,		     m_edtTrkP);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_D_EDT,	       m_edtTrkD);
	DDX_Control(pDX, IDC_CMD_LIST,				                 m_List);
	DDX_Check  (pDX, IDC_CMD_USER_VEHI_TRUCK_VARIABLE_CHK, m_bVariable);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W_EDT,              m_edtLaneW);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W_UNT,              m_untLaneW);
	DDX_Check  (pDX, IDC_CMD_USER_MVHL_FATIGUE_CHK,        m_bFatigue);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W1_EDT,             m_edtLaneW1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W1_UNT,             m_untLaneW1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_D1_EDT,             m_edtLaneD1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_D1_UNT,             m_untLaneD1);	
	DDX_Control(pDX, IDC_CMD_MVHL_RAIL_INC_EDT,            m_edtRailInc);
	DDX_Control(pDX, IDC_CMD_MVHL_RAIL_INC_UNT,            m_untRailInc);
	DDX_Control(pDX, IDC_CMD_MVHL_RAIL_GRP_EDT,            m_edtGrpNum);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_BEND_EDT,     m_edtDynaAllow_Bend);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_EDT,          m_edtDynaAllow_AllOther);
	
	DDX_Check  (pDX, IDC_CMD_USER_IGNORE_LOCO_CHK,         m_bIgnoreLoco);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_PICTURE, m_wndPicture);
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUsrAustraliaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrAustraliaDlg)
	ON_BN_CLICKED(IDC_CMD_USER_TRK_RDO,          OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_TRN_RDO,          OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_150LA_RDO,        OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_PERMIT_RDO,       OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,               OnCmdMvhlAddBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT,            OnCmdMvhlInsBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY,            OnCmdMvhlModBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,            OnCmdMvhlDelBtn)
	ON_BN_CLICKED(IDC_CMD_USER_MVHL_FATIGUE_CHK, OnCmdMvhlFatigueChk)
	ON_BN_CLICKED(IDC_CMD_APPLY,                 OnCmdMvhlApplyBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST,     OnItemchangedCmdList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrAustraliaDlg::InitUnit()
{
	m_edtLaneW .SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untLaneW .SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtLaneW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untLaneW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtLaneD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLaneD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtRailInc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untRailInc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtGrpNum.SetUnitType(D_UNITSYS_NONE);
	m_edtDynaAllow_Bend.SetUnitType(D_UNITSYS_NONE);
	m_edtDynaAllow_AllOther.SetUnitType(D_UNITSYS_NONE);
	
}

void CCMMvhlItemUsrAustraliaDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;	
	int nDistY = 0;	

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_MVHL_W1_STC);
	aControls.Add(IDC_CMD_USER_MVHL_W1_EDT);
	aControls.Add(IDC_CMD_USER_MVHL_W1_UNT);
	aControls.Add(IDC_CMD_USER_MVHL_D1_STC);
	aControls.Add(IDC_CMD_USER_MVHL_D1_EDT);
	aControls.Add(IDC_CMD_USER_MVHL_D1_UNT);
	
	GetDlgItem(IDC_CMD_USER_MVHL_W_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_USER_MVHL_W1_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);


	CArray<UINT, UINT> aRailGroup;
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_GRP);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_MIN_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_MAX_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_INC_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_INC_EDT);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_INC_UNT);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_D1_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_D1_STC2);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_D1_STC3);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_GRP_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_GRP_EDT);

	GetDlgItem(IDC_CMD_USER_VEHI_LANE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_RAIL_GRP)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aRailGroup, nDistX, nDistY);

	CArray<UINT, UINT> aLoco;
	aLoco.Add(IDC_CMD_USER_IGNORE_LOCO_CHK);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_STC);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_BEND_STC);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_BEND_EDT);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_OTHER_STC);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_EDT);

	GetDlgItem(IDC_CMD_USER_VEHI_TRUCK_VARIABLE_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_USER_IGNORE_LOCO_CHK)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aLoco, nDistX, nDistY);

}

void CCMMvhlItemUsrAustraliaDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	//
	CArray<UINT, UINT> aControlPre;
	aControlPre.RemoveAll();
	aControlPre.Add(IDC_CMD_USER_VEHI_TRUCK_VARIABLE_CHK);
	aControlPre.Add(IDC_CMD_USER_VEHI_TRUCK_VARIABLE_STC);
	aControlPre.Add(IDC_CMD_USER_MVHL_FATIGUE_CHK);

	CArray<UINT, UINT> aLoco;
	aLoco.Add(IDC_CMD_USER_IGNORE_LOCO_CHK);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_STC);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_BEND_STC);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_BEND_EDT);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_OTHER_STC);
	aLoco.Add(IDC_CMD_MVHL_DYNA_ALLOW_EDT);

	CDlgUtil::CtrlShowHide(this, aControlPre, m_nLoadType==0 || m_nLoadType==1);
	CDlgUtil::CtrlShowHide(this, aLoco,       m_nLoadType==2);

	//
	CArray<UINT, UINT> aControls1; aControls1.RemoveAll();
	aControls1.Add(IDC_CMD_USER_MVHL_W_STC);
	aControls1.Add(IDC_CMD_USER_MVHL_W_EDT);
	aControls1.Add(IDC_CMD_USER_MVHL_W_UNT);
	aControls1.Add(IDC_CMD_USER_MVHL_FATIGUE_CHK);

	CArray<UINT, UINT> aControls2; aControls2.RemoveAll();
	aControls2.Add(IDC_CMD_USER_MVHL_W1_STC);
	aControls2.Add(IDC_CMD_USER_MVHL_W1_EDT);
	aControls2.Add(IDC_CMD_USER_MVHL_W1_UNT);
	aControls2.Add(IDC_CMD_USER_MVHL_D1_STC);
	aControls2.Add(IDC_CMD_USER_MVHL_D1_EDT);
	aControls2.Add(IDC_CMD_USER_MVHL_D1_UNT);

	CArray<UINT, UINT> aControls;
	aControls.Add(IDC_CMD_USER_VEHI_LANE_GRP);
	aControls.Append(aControls1);
	aControls.Append(aControls2);

	//
	CArray<UINT, UINT> aRailGroup; aRailGroup.RemoveAll();
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_GRP);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_MIN_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_MAX_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_INC_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_INC_EDT);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_INC_UNT);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_D1_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_D1_STC2);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_D1_STC3);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_GRP_STC);
	aRailGroup.Add(IDC_CMD_MVHL_RAIL_GRP_EDT);

	CDlgUtil::CtrlShowHide(this, aControls,  m_nLoadType==0 || m_nLoadType==1);
	CDlgUtil::CtrlShowHide(this, aRailGroup, m_nLoadType==2);

	CDlgUtil::CtrlShowHide(this, aControls1, m_nLoadType==0);
	CDlgUtil::CtrlShowHide(this, aControls2, m_nLoadType==1);

}

void CCMMvhlItemUsrAustraliaDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CArray<UINT, UINT> aControls, aControls1; 
	aControls.RemoveAll();
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

	aControls1.RemoveAll();
	aControls1.Add(IDC_CMD_USER_VEHI_TRUCK_VARIABLE_CHK);
	aControls1.Add(IDC_CMD_USER_VEHI_TRUCK_VARIABLE_STC);

	CDlgUtil::CtrlEnableDisable(this, aControls, m_nLoadType==0 || m_nLoadType==2);
	CDlgUtil::CtrlEnableDisable(this, aControls1, m_nLoadType==0);

	aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_MVHL_W_STC);
	aControls.Add(IDC_CMD_USER_MVHL_W_EDT);
	aControls.Add(IDC_CMD_USER_MVHL_W_UNT);

	CDlgUtil::CtrlEnableDisable(this, aControls, !m_bFatigue);

}

void CCMMvhlItemUsrAustraliaDlg::SetHeaderTitle()
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

void CCMMvhlItemUsrAustraliaDlg::MakeItemEx()
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

BOOL CCMMvhlItemUsrAustraliaDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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

BOOL CCMMvhlItemUsrAustraliaDlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUsrAustraliaDlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
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


void CCMMvhlItemUsrAustraliaDlg::ChangeData()
{
	UpdateData(FALSE);

	ChangeBitMap();
	//AlignControl();
	ControlsShowHide();
	EnableDisableControls();

}

void CCMMvhlItemUsrAustraliaDlg::ChangeBitMap()
{	
	UpdateData(TRUE);

	CString strSVG = _T("cmd_mvhl_113.svg");
	if     (m_nLoadType == 0) strSVG = _T("cmd_mvhl_113.svg");
	else if(m_nLoadType == 1) strSVG = _T("cmd_mvhl_130.svg");
	else if(m_nLoadType == 2) strSVG = _T("cmd_mvhl_AS_300LA_User.svg");
	
	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

//m_nLoadType      : 0=truck  1:Train  2:Rail    3:Permit
//m_Data.nLoadType : 0=truck  1:Train  2:Permit  3:Rail
void CCMMvhlItemUsrAustraliaDlg::Data2Dlg()
{
	if     (m_Data.nLoadType == 2) m_nLoadType = 3;
	else if(m_Data.nLoadType == 3) m_nLoadType = 2;
	else                           m_nLoadType = m_Data.nLoadType;

	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

	if(m_bModify) 
	{
		m_bVariable = m_Data.bVarSpacing;
	}
	else          
		m_bVariable = TRUE; // Default Check On
	
	m_edtLaneW .SetEditUnit(m_Data.dUniformLoad);
	m_edtLaneW1.SetEditUnit(m_Data.dW1);
	m_edtLaneD1.SetEditUnit(m_Data.dD1);
	m_bFatigue = m_Data.bFatigue;
	m_edtRailInc.SetEditUnit(m_Data.dIncreLength);
	m_bIgnoreLoco    = m_Data.bIgnoreLoco;
	m_edtGrpNum .SetEditUnit(m_Data.nGroupNum);
	m_edtDynaAllow_Bend.SetEditUnit(m_Data.dDynLoadAllowance2);
	m_edtDynaAllow_AllOther.SetEditUnit(m_Data.dDynLoadAllowance);
	
	MakeItemEx();
	
	UpdateData(FALSE);
}

//m_nLoadType      : 0=truck  1:Train  2:Rail    3:Permit
//m_Data.nLoadType : 0=truck  1:Train  2:Permit  3:Rail
BOOL CCMMvhlItemUsrAustraliaDlg::Dlg2Data()
{	
	UpdateData(TRUE);  
	m_Data.bStandard = FALSE;    

	if     (m_nLoadType == 2) m_Data.nLoadType = 3;
	else if(m_nLoadType == 3) m_Data.nLoadType = 2;
	else                      m_Data.nLoadType = m_nLoadType;

	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);	

	m_Data.bVarSpacing = m_bVariable;

	m_Data.dUniformLoad = m_edtLaneW.GetEditValue();
	m_Data.dW1 = m_edtLaneW1.GetEditValue();
	m_Data.dD1 = m_edtLaneD1.GetEditValue();
	m_Data.bFatigue = m_bFatigue;
	m_Data.dIncreLength = m_edtRailInc.GetEditValue();
	m_Data.nGroupNum    = m_edtGrpNum.GetEditValue();
	m_Data.bIgnoreLoco  = m_bIgnoreLoco; 

	m_Data.dDynLoadAllowance2 = m_edtDynaAllow_Bend.GetEditValue();
	m_Data.dDynLoadAllowance = m_edtDynaAllow_AllOther.GetEditValue();

	  
	return TRUE;
}

int CCMMvhlItemUsrAustraliaDlg::GetLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUsrAustraliaDlg::GetDistCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CCMMvhlItemUsrAustraliaDlg::GetLoadAndDistValue(double &dLoad, double &dDist)
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

BOOL CCMMvhlItemUsrAustraliaDlg::ApplyOrOK()
{
	BOOL bSuccess = FALSE;
	
	if (!Dlg2Data()) return FALSE;

	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);

	if (!bSuccess) return FALSE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrAustraliaDlg message handlers

BOOL CCMMvhlItemUsrAustraliaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	SetHeaderTitle();
		
	Data2Dlg();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
		
	UpdateData(FALSE);

	ChangeBitMap();

	OnCmdMvhlLoadTypeRdo();
 
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrAustraliaDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvhlItemUsrAustraliaDlg::OnCmdMvhlLoadTypeRdo() 
{
	UpdateData(TRUE);

	if(m_nLoadType == 3) // permit
	{
		if (m_bPermitLoadCall)
		{
			m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
			m_bPermitLoadCall = FALSE;
			CCMMvhlItemUsrPermitDlg dlg;
			dlg.SetInitPos(D_INIT_POS_LT);
			CString csRadioText[4];
			GetDlgItem(IDC_CMD_USER_TRK_RDO)->GetWindowText(csRadioText[0]);
			GetDlgItem(IDC_CMD_USER_TRN_RDO)->GetWindowText(csRadioText[1]);
			GetDlgItem(IDC_CMD_USER_150LA_RDO)->GetWindowText(csRadioText[2]);
			GetDlgItem(IDC_CMD_USER_PERMIT_RDO)->GetWindowText(csRadioText[3]);
	
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
				int nID = dlg.GetASType();
				m_nLoadType = nID;

				ChangeData();
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
		//AlignControl();
		ControlsShowHide();
		EnableDisableControls();
	}
}

void CCMMvhlItemUsrAustraliaDlg::OnCmdMvhlAddBtn() 
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

void CCMMvhlItemUsrAustraliaDlg::OnCmdMvhlInsBtn() 
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

void CCMMvhlItemUsrAustraliaDlg::OnCmdMvhlModBtn() 
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

void CCMMvhlItemUsrAustraliaDlg::OnCmdMvhlDelBtn() 
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
	int i = iItem;
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

void CCMMvhlItemUsrAustraliaDlg::OnCmdMvhlFatigueChk() 
{
	EnableDisableControls();
}

void CCMMvhlItemUsrAustraliaDlg::OnCmdMvhlApplyBtn() 
{
	ApplyOrOK();
}

void CCMMvhlItemUsrAustraliaDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
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








