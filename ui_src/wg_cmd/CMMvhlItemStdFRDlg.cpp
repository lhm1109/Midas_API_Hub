// CMMvhlItemStdFRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdFRDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdFRDlg dialog

#define COLCOUNT 3

CCMMvhlItemStdFRDlg::CCMMvhlItemStdFRDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdFRDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;

	m_aPicture.RemoveAll();
	m_aPicture.Add(IDC_CMD_MVHL_PICTURE);

	m_aList.RemoveAll();
	m_aList.Add(IDC_CMD_MVHL_PTLIST);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);

	//
	m_aDist.RemoveAll();
	m_aDist.Add(IDC_CMD_MVHL_FR_DIST_GRP);
	m_aDist.Add(IDC_CMD_MVHL_FR_DIST_MIN_STC);
	m_aDist.Add(IDC_CMD_MVHL_FR_DIST_MAX_STC);

	m_aCoeff.RemoveAll();
	m_aCoeff.Add(IDC_CMD_MVHL_FR_COEFFICIENT_STC);
	m_aCoeff.Add(IDC_CMD_MVHL_FR_COEFFICIENT_GRID);
	
	m_aCoeff2.RemoveAll();
	m_aCoeff2.Add(IDC_CMD_MVHL_FR_COEFFICIENT_A2_STC);
	m_aCoeff2.Add(IDC_CMD_MVHL_FR_COEFFICIENT_A2_GRID);

	m_aMilitaryTail.RemoveAll();
	m_aMilitaryTail.Add(IDC_CMD_MVHL_FR_TAIL_DISTANCE_STC);
	m_aMilitaryTail.Add(IDC_CMD_MVHL_FR_TAIL_DISTANCE_EDT);
	m_aMilitaryTail.Add(IDC_CMD_MVHL_FR_TAIL_DISTANCE_UNT);
	m_aMilitaryP.RemoveAll();
	m_aMilitaryP.Add(IDC_CMD_MVHL_FR_MILITARY_P_STC);
	m_aMilitaryP.Add(IDC_CMD_MVHL_FR_MILITARY_P_EDT);
	m_aMilitaryP.Add(IDC_CMD_MVHL_FR_MILITARY_P_UNT);
	m_aMilitaryD.RemoveAll();
	m_aMilitaryD.Add(IDC_CMD_MVHL_FR_MILITARY_D_STC);
	m_aMilitaryD.Add(IDC_CMD_MVHL_FR_MILITARY_D_EDT);
	m_aMilitaryD.Add(IDC_CMD_MVHL_FR_MILITARY_D_UNT);
	m_aMilitaryD1.RemoveAll();
	m_aMilitaryD1.Add(IDC_CMD_MVHL_FR_MILITARY_D1_STC);
	m_aMilitaryD1.Add(IDC_CMD_MVHL_FR_MILITARY_D1_EDT);
	m_aMilitaryD1.Add(IDC_CMD_MVHL_FR_MILITARY_D1_UNT);

	m_aMilitary.RemoveAll();
	m_aMilitary.Append(m_aMilitaryP);
	m_aMilitary.Append(m_aMilitaryD);
	m_aMilitary.Append(m_aMilitaryD1);

	m_aSidewalk.RemoveAll();
	m_aSidewalk.Add(IDC_CMD_MVHL_FR_SIDEWALK_W_STC);
	m_aSidewalk.Add(IDC_CMD_MVHL_FR_SIDEWALK_W_EDT);
	m_aSidewalk.Add(IDC_CMD_MVHL_FR_SIDEWALK_W_UNT);

	m_aDyFactor.RemoveAll();
	m_aDyFactor.Add(IDC_CMD_MVHL_DYNAMIC_CHK);
}

CCMMvhlItemStdFRDlg::~CCMMvhlItemStdFRDlg()
{
}

void CCMMvhlItemStdFRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdFRDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,               m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT,          m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB,          m_cmbVehicleType);
	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST,                 m_List);

	DDX_Control(pDX, IDC_CMD_MVHL_FR_TAIL_DISTANCE_EDT, m_edtTailDist);
	DDX_Control(pDX, IDC_CMD_MVHL_FR_TAIL_DISTANCE_UNT, m_untTailDist);
	
	DDX_Control(pDX, IDC_CMD_MVHL_FR_MILITARY_P_EDT,  m_edtP);
	DDX_Control(pDX, IDC_CMD_MVHL_FR_MILITARY_P_UNT,  m_untP);
	DDX_Control(pDX, IDC_CMD_MVHL_FR_MILITARY_D_EDT,  m_edtD);
	DDX_Control(pDX, IDC_CMD_MVHL_FR_MILITARY_D_UNT,  m_untD);
	DDX_Control(pDX, IDC_CMD_MVHL_FR_MILITARY_D1_EDT, m_edtD1);
	DDX_Control(pDX, IDC_CMD_MVHL_FR_MILITARY_D1_UNT, m_untD1);

	DDX_Control(pDX, IDC_CMD_MVHL_FR_SIDEWALK_W_EDT, m_edtW);
	DDX_Control(pDX, IDC_CMD_MVHL_FR_SIDEWALK_W_UNT, m_untW);

	DDX_Check(pDX, IDC_CMD_MVHL_DYNAMIC_CHK, m_bDynamicFactor);
	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdFRDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdFRDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,        OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB,   OnCmdMvhlTypeCmb)
	ON_BN_CLICKED   (IDC_CMD_APPLY,                OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemStdFRDlg::InitUnit()
{
	m_edtTailDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTailDist.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtP.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untP.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD1.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtW.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_untW.SetUnitType(D_UNITSYS_BASE_PRESSURE);
}

void CCMMvhlItemStdFRDlg::InitCombo()
{
	int nCodeSeq[4];
	nCodeSeq[0] = D_MVHL_FRANCE_SYSTEM_A;
	nCodeSeq[1] = D_MVHL_FRANCE_SYSTEM_B;
	nCodeSeq[2] = D_MVHL_FRANCE_MILITARY_LOAD;
	nCodeSeq[3] = D_MVHL_FRANCE_SIDEWALK;
	
	for(int i=0; i<4; i++) 
	{
		CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}
}

void CCMMvhlItemStdFRDlg::AlignControl()
{
	UpdateData(TRUE);
	
	CString strVehicleType = _T("");
	strVehicleType = GetVehicleTypeStr();
	
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;	

	CRect rectTarget;
	CRect rectShort;
	BOOL bDyFactor = FALSE;

	CArray<UINT, UINT> aClrls; aClrls.RemoveAll();
	aClrls.Add(IDC_CMD_MVHL_PTLIST);

	GetDlgItem(IDC_CMD_MVHL_PICTURE)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aClrls , nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aDist, nDistY);

	if (m_Data.VehicleTypeName == _T("Load System A"))
	{
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_STC)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCoeff, nDistY);

		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_GRID)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_A2_STC)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCoeff2, nDistY);

		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_A2_GRID)->GetWindowRect(rectTarget);
	}
	else if (m_Data.VehicleTypeName == _T("Load System Bc") || m_Data.VehicleTypeName == _T("Load System Bt"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_STC)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCoeff, nDistY);

		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_GRID)->GetWindowRect(rectTarget);
	}
	else if (m_Data.VehicleTypeName == _T("System Mc 80") || m_Data.VehicleTypeName == _T("System Mc 120"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_FR_TAIL_DISTANCE_STC)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aMilitaryTail, nDistY);

		GetDlgItem(IDC_CMD_MVHL_FR_TAIL_DISTANCE_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_FR_MILITARY_P_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(5);
		CDlgUtil::CtrlMoveDistY(this, m_aMilitary, nDistY);

		GetDlgItem(IDC_CMD_MVHL_FR_MILITARY_D_EDT)->GetWindowRect(rectTarget);
	}
	else if (m_Data.VehicleTypeName == _T("System Me 80") || m_Data.VehicleTypeName == _T("System Me 120"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_DYNAMIC_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aDyFactor, nDistY);

		GetDlgItem(IDC_CMD_MVHL_DYNAMIC_CHK)->GetWindowRect(rectTarget);
	}
	else if (m_Data.VehicleTypeName == _T("Convoy Type D") || m_Data.VehicleTypeName == _T("Convoy Type E"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_FR_MILITARY_P_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aMilitary, nDistY);

		GetDlgItem(IDC_CMD_MVHL_FR_MILITARY_D1_EDT)->GetWindowRect(rectTarget);
	}
	else if (m_Data.VehicleTypeName == _T("Sidewalk") ||     
			 m_Data.VehicleTypeName == _T("Pedestrian"))
	{
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_FR_SIDEWALK_W_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aSidewalk, nDistY);

		GetDlgItem(IDC_CMD_MVHL_FR_SIDEWALK_W_EDT)->GetWindowRect(rectTarget);
	}
	else
		ASSERT(0);

	// Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rectShort);
	rectShort.bottom = rectTarget.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->MoveWindow(rectShort);

	if(bDyFactor)
	{
		GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_DYNAMIC_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aDyFactor, nDistY);

		GetDlgItem(IDC_CMD_MVHL_DYNAMIC_CHK)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rRef);
	}

	// OK, Cancel, Apply
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
	Invalidate();
}

void CCMMvhlItemStdFRDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_FR_SYSTEMA_GRP )->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_MVHL_FR_MILITARY_GRP)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_MVHL_FR_SIDEWALK_GRP)->ShowWindow(FALSE);

	CString strVehicleType = _T("");
	strVehicleType = GetVehicleTypeStr();

	BOOL bDyFactor = FALSE;
	if (m_Data.VehicleTypeName == _T("Load System A"))
	{
		GetDlgItem(IDC_CMD_MVHL_PTLIST  )->ShowWindow(FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDist        , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff       , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff2      , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryTail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryP   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD1  , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSidewalk    , FALSE);
	}
	else if (m_Data.VehicleTypeName == _T("Load System Bc"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST  )->ShowWindow(TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDist        , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff       , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff2      , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryTail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryP   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD1  , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSidewalk    , FALSE);
	}
	else if (m_Data.VehicleTypeName == _T("Load System Bt"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST  )->ShowWindow(TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDist        , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff       , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff2      , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryTail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryP   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD1  , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSidewalk    , FALSE);
	}
	else if (m_Data.VehicleTypeName == _T("System Mc 80") || m_Data.VehicleTypeName == _T("System Mc 120"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST  )->ShowWindow(FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDist        , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff       , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff2      , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryTail, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryP   , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD   , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD1  , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSidewalk    , FALSE);
	}
	else if (m_Data.VehicleTypeName == _T("System Me 80") || m_Data.VehicleTypeName == _T("System Me 120"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST  )->ShowWindow(FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDist        , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff       , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff2      , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryTail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryP   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD1  , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSidewalk    , FALSE);
	}
	else if (m_Data.VehicleTypeName == _T("Convoy Type D") || m_Data.VehicleTypeName == _T("Convoy Type E"))
	{
		bDyFactor = TRUE;

		GetDlgItem(IDC_CMD_MVHL_PTLIST  )->ShowWindow(FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDist        , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff       , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff2      , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryTail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryP   , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD   , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD1  , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aSidewalk    , FALSE);
	}
	else if (m_Data.VehicleTypeName == _T("Sidewalk"))
	{
		GetDlgItem(IDC_CMD_MVHL_PTLIST  )->ShowWindow(FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDist        , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff       , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff2      , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryTail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryP   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD1  , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSidewalk    , TRUE);
	}
	else if (m_Data.VehicleTypeName == _T("Pedestrian"))
	{
		GetDlgItem(IDC_CMD_MVHL_PTLIST  )->ShowWindow(FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDist        , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff       , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCoeff2      , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryTail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryP   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD   , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMilitaryD1  , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSidewalk    , FALSE);
	}
	else
		ASSERT(0);

	GetDlgItem(IDC_CMD_MVHL_DYNAMIC_CHK)->ShowWindow(bDyFactor);
}

void CCMMvhlItemStdFRDlg::SetHeaderTitle()
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
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 90;
	
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

BOOL CCMMvhlItemStdFRDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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
			double dDistTmp = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_TON, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dDist);

			double dTol = 1.e-10;
			if      (fabs(dDistTmp)       < dTol) str = _LS(IDS_WG_CMD__ADDD__end);
			else if (fabs(dDistTmp + 1.0) < dTol) str = _LS(IDS_WG_CMD_INFINITE);
			else                                  str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if(i == 0)
			m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

void CCMMvhlItemStdFRDlg::MakeItemEx()
{
	m_List.DeleteAllItems();    
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	if (nLoadCount > 0)
	{
		int i = 0;
		for (i = 0; i < nDistCount; i++)
			InsertItem(i, i + 1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

		if (nLoadCount > nDistCount)
			InsertItem(i, i + 1, m_Data.dPointLoad[i], 0.0);
	}
	
	UpdateData(FALSE);
}

int CCMMvhlItemStdFRDlg::GetLoadCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdFRDlg::GetDistCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdFRDlg::ChangeBitmap(int nIndex, int nBitmap)
{
	CString aBitmapSystemA[]    = { _T("cmd_mvhl_FR_System_A.svg")};
	CString aBitmapSystemB[]    = { _T("cmd_mvhl_FR_System_Bc.svg"), _T("cmd_mvhl_FR_System_Bt.svg")};
	CString aBitmapMilitary[]   = { _T("cmd_mvhl_FR_System_Mc.svg"), _T("cmd_mvhl_FR_System_Mc_120.svg"), _T("cmd_mvhl_FR_System_Me_80.svg"), 
									_T("cmd_mvhl_FR_System_Me_120.svg"), _T("cmd_mvhl_FR_System_Convoy.svg"), _T("cmd_mvhl_FR_System_Convoy.svg") };
	CString aBitmapSideGlobal[] = { _T("cmd_mvhl_FR_Sidewalk.svg"), _T("cmd_mvhl_FR_Sidewalk_Length.svg") };
		
	CString strSVG; 
	int nSelBitmap = 0;
	switch(nIndex)
	{
		case D_MVHL_FRANCE_SYSTEM_A:          strSVG = aBitmapSystemA[nBitmap];  break;
		case D_MVHL_FRANCE_SYSTEM_B:          strSVG = aBitmapSystemB[nBitmap];  break;
		case D_MVHL_FRANCE_MILITARY_LOAD:     strSVG = aBitmapMilitary[nBitmap];  break;
		case D_MVHL_FRANCE_SIDEWALK:          strSVG = aBitmapSideGlobal[nBitmap]; break;
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdFRDlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode;
	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(ConvVehicleTypeStrRaw2Trans(m_Data.VehicleTypeName));
	else
		m_edtVehicleName.SetWindowText(ConvVehicleTypeStrRaw2Trans(m_Data.VehicleLoadName));
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode, sIndex);

	CVehlDB db(m_pDoc);
	if (db.GetStandardVehicleLoadValue(m_Data))	MakeItemEx();

	m_edtTailDist.SetEditUnit(m_Data.dTailDist);
	m_edtP.SetEditUnit(m_Data.dMilitaryP);
	m_edtD.SetEditUnit(m_Data.dMilitaryD);
	m_edtD1.SetEditUnit(m_Data.dMilitaryD1);
	m_edtW.SetEditUnit(m_Data.dSidewalkW);

	BOOL bBtType = FALSE;
	if (/*m_bStd && */m_Data.VehicleTypeName == _T("Load System Bt"))
	{
		bBtType = TRUE;
	}
	m_wndCoeffGrid.ShowData(bBtType);
	m_wndNominGrid.ShowData();

	m_bDynamicFactor = m_Data.bDynamicFactor;

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdFRDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
	m_Data.bStandard = TRUE;
	m_Data.VehicleTypeName = GetVehicleTypeStr();

	m_Data.nStandardCode = m_nStndCode;

	m_Data.dSidewalkW = m_edtW.GetEditValue();
	m_Data.dMilitaryP  = m_edtP.GetEditValue();
	m_Data.dMilitaryD  = m_edtD.GetEditValue();
	m_Data.dMilitaryD1 = m_edtD1.GetEditValue();

	m_wndCoeffGrid.SaveData();
	m_wndNominGrid.SaveData();

	m_Data.bDynamicFactor = m_bDynamicFactor;

	return TRUE;
}

BOOL CCMMvhlItemStdFRDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

BOOL CCMMvhlItemStdFRDlg::ChangeData()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_Data.nStandardCode = m_nStndCode;
	
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode, sIndex);

	if (m_Data.VehicleTypeName == _T("Sidewalk"))
	{
		m_Data.dSidewalkW = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_PRESSURE, 4.5);
	}
	if (m_Data.VehicleTypeName == _T("Load System Bt"))
	{
		m_Data.dCoefficient01[0] = 1.2;
		m_Data.dCoefficient02[0] = 1.0;
	}
	Data2Dlg();

	m_edtVehicleName.SetWindowText(ConvVehicleTypeStrRaw2Trans(m_Data.VehicleTypeName));

	if (m_Data.VehicleTypeName == _T("Load System A"))
	{
		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_STC)->SetWindowText(_LS(IDS_WG_CMD__ADDD__FR_MVHL_Coefficent_a1));
	}
	else if (m_Data.VehicleTypeName == _T("Load System Bc"))
	{
		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_STC)->SetWindowText(_LS(IDS_WG_CMD__ADDD__FR_MVHL_Coefficent_bc));
	}
	else if (m_Data.VehicleTypeName == _T("Load System Bt"))
	{
		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_STC)->SetWindowText(_LS(IDS_WG_CMD__ADDD__FR_MVHL_Coefficent_bt));
	}

	return TRUE;
}

void CCMMvhlItemStdFRDlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;
	
	m_cmbVehicleType.ResetContent();
	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
	{
		const auto& strTranslation =
			ConvVehicleTypeStrRaw2Trans(CVehlDB::GetTypeListAr()[nIndex][i++]);
		m_cmbVehicleType.AddString(strTranslation);
	}
	m_cmbVehicleType.SetCurSel(0);
}

void CCMMvhlItemStdFRDlg::GetIndex(CString &csName, int *sitem)
{  
	for (int i = 0; i < D_MAX_COUNT; i++)
	{
		if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == csName)
		{
			if(sitem) *sitem = i;
			return ;
		}
	}
	
	ASSERT(0);
	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdFRDlg message handlers

BOOL CCMMvhlItemStdFRDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = _T("Load System A");
		m_nStndCode          = D_MVHL_FRANCE_SYSTEM_A;
		m_Data.nStandardCode = D_MVHL_FRANCE_SYSTEM_A;
	}
	GetIndex(m_Data.VehicleTypeName);
	int nIndex = m_Data.nStandardCode;
	if (nIndex < 0) return TRUE;
	
	// InitCtrl
	m_wndCoeffGrid.SubclassDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_GRID, this);
	m_wndCoeffGrid.InitGrid();
	m_wndCoeffGrid.SetDataSource(&m_Data, this, TRUE);

	m_wndNominGrid.SubclassDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_A2_GRID, this);
	m_wndNominGrid.InitGrid();
	m_wndNominGrid.SetDataSource(&m_Data, this, TRUE);

	InitUnit();
	InitCombo();
	SetHeaderTitle();
	SetLoadTypeList(nIndex);
	
	Data2Dlg();
	
	AlignControl();
	ControlsShowHide();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdFRDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdFRDlg::OnCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);

	int nIndex = m_cmbCodeName.GetCurSel();
	m_nStndCode = m_cmbCodeName.GetItemData(nIndex);

	ChangeData();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdFRDlg::OnCmdMvhlTypeCmb() 
{
	ChangeData();  

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdFRDlg::OnCmdApply() 
{	
	ApplyOrOK();
}

typedef std::pair<CString, CString> pair_cstring;
template <std::size_t N> using array_pair_cstring = std::array<pair_cstring, N>;

template <std::size_t N>
CString ConvStrRaw2Trans(const array_pair_cstring<N>& aPairStr,
	const CString& strRaw)
{
	for (const auto& strPair : aPairStr)
	{
		const auto& strTarget = strPair.first;
		if (strRaw == strTarget)
		{
			return strPair.second;
		}
	}

	return strRaw;
}

template <std::size_t N>
CString ConvStrTrans2Raw(const array_pair_cstring<N>& aPairStr,
	const CString& strTrans)
{
	for (const auto& strPair : aPairStr)
	{
		const auto& strTarget = strPair.second;
		if (strTrans == strTarget)
		{
			return strPair.first;
		}
	}

	return strTrans;
}

#define __static_caVehicleLoadType static const array_pair_cstring<11> caVehicleLoadType = \
{ \
    std::make_pair(_T("Load System A"),    _LS(IDS_WG_CMD_FR_Load_System_A)), \
    std::make_pair(_T("Load System Bc"),   _LS(IDS_WG_CMD_FR_Load_System_Bc)), \
    std::make_pair(_T("Load System Bt"),   _LS(IDS_WG_CMD_FR_Load_System_Bt)), \
    std::make_pair(_T("System Mc 80"),     _LS(IDS_WG_CMD_FR_System_Mc_80)), \
    std::make_pair(_T("System Mc 120"),    _LS(IDS_WG_CMD_FR_System_Mc_120)), \
    std::make_pair(_T("System Me 80"),     _LS(IDS_WG_CMD_FR_System_Me_80)), \
    std::make_pair(_T("System Me 120"),    _LS(IDS_WG_CMD_FR_System_Me_120)), \
    std::make_pair(_T("Convoy Type D"),    _LS(IDS_WG_CMD_FR_Convoy_Type_D)), \
    std::make_pair(_T("Convoy Type E"),    _LS(IDS_WG_CMD_FR_Convoy_Type_E)), \
    std::make_pair(_T("Sidewalk"),         _LS(IDS_WG_CMD_FR_Sidewalk)), \
    std::make_pair(_T("Pedestrian"),       _LS(IDS_WG_CMD_FR_Pedestrian)), \
}; \

CString CCMMvhlItemStdFRDlg::ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const
{
	__static_caVehicleLoadType;
	return ConvStrRaw2Trans(caVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdFRDlg::ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const
{
	__static_caVehicleLoadType;
	return ConvStrTrans2Raw(caVehicleLoadType, strTrans);
}

#undef __static_caVehicleLoadType

/*
 * 지금 콤보박스를 사용하는 곳이 String으로 비교를 하고 있어
 * 일괄적으로 적용하기에는 리스크가 존재해서..
 * 번역이 필요한 String만 따로 추출해서 변환시켜주는 역할ㅠㅠ
 */

CString CCMMvhlItemStdFRDlg::GetVehicleTypeStr() const
{
	CString CurTransStr;
	m_cmbVehicleType.GetWindowText(CurTransStr);

	return ConvVehicleTypeStrTrans2Raw(CurTransStr);
}