
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdPolandDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdPolandDlg dialog

#define COLCOUNT 3

CCMMvhlItemStdPolandDlg::CCMMvhlItemStdPolandDlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CCMMvhlItemStdPolandDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;
}

CCMMvhlItemStdPolandDlg::~CCMMvhlItemStdPolandDlg()
{
}

void CCMMvhlItemStdPolandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,             m_cmbCodeName);  
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT,        m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB,        m_cmbVehicleType);		  
	DDX_Control(pDX, IDC_CMD_MVHL_SEL_VEHICLE_CMB,      m_cmbSelVihicle);  
	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST,               m_List);	
	DDX_Control(pDX, IDC_CMD_MVHL_Q_EDT,                m_edtQ);	
	DDX_Control(pDX, IDC_CMD_MVHL_Q_UNT,                m_untQ);	
	DDX_Control(pDX, IDC_CMD_MVHL_A_EDT,                m_edtA);	
	DDX_Control(pDX, IDC_CMD_MVHL_A_UNT,                m_untA);	
	DDX_Control(pDX, IDC_CMD_MVHL_DYNAMIC_CHK,          m_chkDynamic);	
	DDX_Control(pDX, IDC_CMD_MVHL_PHI_EDT,              m_edtPhi);	
	DDX_Control(pDX, IDC_CMD_MVHL_WHEEL_SPACING_EDT1,   m_edtWheelSpacing1);	
	DDX_Control(pDX, IDC_CMD_MVHL_WHEEL_SPACING_UNT1,   m_untWheelSpacing1);	
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT1,  m_edtNoseTailDist1);	
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT1,  m_untNoseTailDist1);	
	DDX_Control(pDX, IDC_CMD_MVHL_NUM_VEHICLE_EDT1,     m_edtNumVehicle1);	
	DDX_Control(pDX, IDC_CMD_MVHL_TOTAL_LOAD_EDT,       m_edtTotalLoad);	
	DDX_Control(pDX, IDC_CMD_MVHL_TOTAL_LOAD_UNT,       m_untTotalLoad);	
	DDX_Control(pDX, IDC_CMD_MVHL_TRACKED_LENGTH_EDT,   m_edtTrackedLength);	
	DDX_Control(pDX, IDC_CMD_MVHL_TRACKED_LENGTH_UNT,   m_untTrackedLength);
	DDX_Control(pDX, IDC_CMD_MVHL_WHEEL_SPACING_EDT2,   m_edtWheelSpacing2);	
	DDX_Control(pDX, IDC_CMD_MVHL_WHEEL_SPACING_UNT2,   m_untWheelSpacing2);	
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT2,  m_edtNoseTailDist2);	
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT2,  m_untNoseTailDist2);	
	DDX_Control(pDX, IDC_CMD_MVHL_NUM_VEHICLE_EDT2,     m_edtNumVehicle2);
	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE,				m_wndPicture);
}

void CCMMvhlItemStdPolandDlg::InitUnit() 
{
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

void CCMMvhlItemStdPolandDlg::InitCombo()
{
	int nCodeSeq;
	nCodeSeq = D_MVHL_PN_85S_10030_ROAD_BRG;
	CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq], nCodeSeq);
	nCodeSeq = D_MVHL_PN_85S_10030_WALKWALY_BRG;
	CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq], nCodeSeq);  
	nCodeSeq = D_MVHL_MILITARY_LOAD;
	CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq], nCodeSeq);  
	m_cmbCodeName.SetCurSel(0);

	SetSelVehicleList();
}

void CCMMvhlItemStdPolandDlg::SetSelVehicleList()
{
	UpdateData(TRUE);

	int nStndCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	
	CString str;

	m_cmbSelVihicle.ResetContent();
	if(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG)
	{
		str = _T("Class A");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class B");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class C");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class D");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class E");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
	}
	else if(nStndCode == D_MVHL_PN_85S_10030_WALKWALY_BRG)
	{ 
		m_cmbSelVihicle.AddString(_T(""));
	}
	else if(nStndCode == D_MVHL_MILITARY_LOAD)
	{
		// Tracked Vehicle 과 Wheeled Vehicle 이 같음
		str = _T("Class 4");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 8");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 12");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 16");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 20");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 24");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 30");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 40");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 50");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 60");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 70");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 80");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 90");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 100");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 120");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = _T("Class 150");
		m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(str));
	}
	else
	{
		m_cmbSelVihicle.AddString(_T(""));
	}
	m_cmbSelVihicle.SetCurSel(0);
}

void CCMMvhlItemStdPolandDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	int nDistY = 0;	

	int nStndCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());

	// GroupBox  
	if(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG || nStndCode == D_MVHL_MILITARY_LOAD)
	{    
		GetDlgItem(IDC_CMD_MVHL_PHI_GRUP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rToMove);
		rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(12);
		ScreenToClient(rToMove);
		GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->MoveWindow(rToMove);
	}
	else if(nStndCode == D_MVHL_PN_85S_10030_WALKWALY_BRG)
	{
		GetDlgItem(IDC_CMD_MVHL_PICTURE)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rToMove);
		rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
		ScreenToClient(rToMove);
		GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->MoveWindow(rToMove);
	}
	else
	{
		ASSERT(0);
		return;
	}

	// a
	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_A_STC);
	aControls.Add(IDC_CMD_MVHL_A_EDT);
	aControls.Add(IDC_CMD_MVHL_A_UNT);

	GetDlgItem(IDC_CMD_MVHL_Q_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_A_STC)->GetWindowRect(rToMove);
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

	GetDlgItem(IDC_CMD_MVHL_Q_STC)->GetWindowRect(rRef);
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
	
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_TOTAL_LOAD_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Ok, Cancel, Apply
	aControls.RemoveAll();
	aControls.Add(IDOK);
	aControls.Add(IDCANCEL);
	aControls.Add(IDC_CMD_APPLY);

	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
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

void CCMMvhlItemStdPolandDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	int nStndCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	int nVehicleType = m_cmbVehicleType.GetCurSel();

	GetDlgItem(IDC_CMD_MVHL_SEL_VEHICLE_STC)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG || nStndCode == D_MVHL_MILITARY_LOAD);
	GetDlgItem(IDC_CMD_MVHL_SEL_VEHICLE_CMB)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG || nStndCode == D_MVHL_MILITARY_LOAD);
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG || (nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1));
	GetDlgItem(IDC_CMD_MVHL_Q_STC)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG && nVehicleType != 2);
	GetDlgItem(IDC_CMD_MVHL_Q_EDT)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG && nVehicleType != 2);
	GetDlgItem(IDC_CMD_MVHL_Q_UNT)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG && nVehicleType != 2);
	GetDlgItem(IDC_CMD_MVHL_A_STC)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG && nVehicleType == 2);
	GetDlgItem(IDC_CMD_MVHL_A_EDT)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG && nVehicleType == 2);
	GetDlgItem(IDC_CMD_MVHL_A_UNT)->ShowWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG && nVehicleType == 2);

	// Wheeled Vehicle
	GetDlgItem(IDC_CMD_MVHL_WHEEL_SPACING_STC1)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1);
	GetDlgItem(IDC_CMD_MVHL_WHEEL_SPACING_EDT1)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1);
	GetDlgItem(IDC_CMD_MVHL_WHEEL_SPACING_UNT1)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1);
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_STC1)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1);
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT1)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1);
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT1)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1);
	GetDlgItem(IDC_CMD_MVHL_NUM_VEHICLE_STC1)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1);
	GetDlgItem(IDC_CMD_MVHL_NUM_VEHICLE_EDT1)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 1);
	
	// Tracked Vehicle
	GetDlgItem(IDC_CMD_MVHL_TOTAL_LOAD_STC)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_TOTAL_LOAD_EDT)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_TOTAL_LOAD_UNT)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_TRACKED_LENGTH_STC)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_TRACKED_LENGTH_EDT)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_TRACKED_LENGTH_UNT)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_WHEEL_SPACING_STC2)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_WHEEL_SPACING_EDT2)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_WHEEL_SPACING_UNT2)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_STC2)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT2)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT2)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_NUM_VEHICLE_STC2)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
	GetDlgItem(IDC_CMD_MVHL_NUM_VEHICLE_EDT2)->ShowWindow(nStndCode == D_MVHL_MILITARY_LOAD && nVehicleType == 0);
}

void CCMMvhlItemStdPolandDlg::EnableDisableCtrl() 
{
	int nStndCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	GetDlgItem(IDC_CMD_MVHL_SEL_VEHICLE_STC)->EnableWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG || nStndCode == D_MVHL_MILITARY_LOAD);
	GetDlgItem(IDC_CMD_MVHL_SEL_VEHICLE_CMB)->EnableWindow(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG || nStndCode == D_MVHL_MILITARY_LOAD);

	CArray<UINT,UINT> aRadio;
	int nAuto = 0;
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this, aRadio, nAuto);
	BOOL bDynamic = m_chkDynamic.GetCheck();
	GetDlgItem(IDC_CMD_MVHL_AUTO_RDO)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_USER_RDO)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->EnableWindow(bDynamic && (nAuto==1));
	GetDlgItem(IDC_CMD_MVHL_PHI_EDT)->EnableWindow(bDynamic && (nAuto==1));
}

void CCMMvhlItemStdPolandDlg::SetHeaderTitle()
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

BOOL CCMMvhlItemStdPolandDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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

		if(i == 0)
			m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

void CCMMvhlItemStdPolandDlg::MakeItemEx()
{
	m_List.DeleteAllItems();    
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();

	if (nLoadCount > 0)
	{
		int i = 0;
		for (i = 0; i < nDistCount; i++)
			InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

		if (nLoadCount > nDistCount)
			InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
	}

	UpdateData(FALSE);
}

int CCMMvhlItemStdPolandDlg::GetLoadCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdPolandDlg::GetDistCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdPolandDlg::ChangeBitmap()
{
	CString bitmapID;

	int nStndCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	int nVehicleType = m_cmbVehicleType.GetCurSel();
	int nSelVehicle = m_cmbSelVihicle.GetCurSel();

	if(nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG)
	{    
		if(nVehicleType == 0) // Vehicle K
		{
			if(nSelVehicle == 0)      bitmapID = _T("cmd_mvhl_poland_1.svg"); // Class A
			else if(nSelVehicle == 1) bitmapID = _T("cmd_mvhl_poland_1.svg"); // Class B
			else if(nSelVehicle == 2) bitmapID = _T("cmd_mvhl_poland_1.svg"); // Class C
			else if(nSelVehicle == 3) bitmapID = _T("cmd_mvhl_poland_1.svg"); // Class D
			else if(nSelVehicle == 4) bitmapID = _T("cmd_mvhl_poland_1.svg"); // Class E
		}
		else if(nVehicleType == 1) // Vehicle S
		{
			if(nSelVehicle == 0)      bitmapID = _T("cmd_mvhl_poland_2.svg");
			else if(nSelVehicle == 1) bitmapID = _T("cmd_mvhl_poland_2.svg");
			else if(nSelVehicle == 2) bitmapID = _T("cmd_mvhl_poland_2.svg");
			else if(nSelVehicle == 3) bitmapID = _T("cmd_mvhl_poland_3.svg");
			else if(nSelVehicle == 4) bitmapID = _T("cmd_mvhl_poland_3.svg");
		}
		else if(nVehicleType == 2) // Vehicle 2S
		{
			if(nSelVehicle == 0)      bitmapID = _T("cmd_mvhl_poland_7.svg");
			else if(nSelVehicle == 1) bitmapID = _T("cmd_mvhl_poland_7.svg");
			else if(nSelVehicle == 2) bitmapID = _T("cmd_mvhl_poland_7.svg");
			else if(nSelVehicle == 3) bitmapID = _T("cmd_mvhl_poland_8.svg");
			else if(nSelVehicle == 4) bitmapID = _T("cmd_mvhl_poland_8.svg");
		}
		else
		{
			ASSERT(0);
		}
	}
	else if(nStndCode == D_MVHL_PN_85S_10030_WALKWALY_BRG)
	{
		if(nVehicleType == 0) // Sidewalks, stairs and walkways
		{
			bitmapID = _T("cmd_mvhl_poland_4.svg");
		}
		else if(nVehicleType == 1) // Main girders and supports
		{
			bitmapID = _T("cmd_mvhl_poland_5.svg");
		}
		else if(nVehicleType == 2) // Service walkways and working platforms
		{
			bitmapID = _T("cmd_mvhl_poland_6.svg");
		}
		else
		{
			ASSERT(0);
		}
	}
	else if(nStndCode == D_MVHL_MILITARY_LOAD)
	{
		if(nVehicleType == 0) // Tracked Vehicle
		{
			bitmapID = _T("cmd_mvhl_poland_9.svg");
		}
		else if(nVehicleType == 1) // Wheeled Vehicle
		{
			if (nSelVehicle == 0)      bitmapID = _T("cmd_mvhl_poland_10.svg");
			else if (nSelVehicle == 1) bitmapID = _T("cmd_mvhl_poland_10.svg");
			else if (nSelVehicle == 2) bitmapID = _T("cmd_mvhl_poland_11.svg");
			else if (nSelVehicle == 3) bitmapID = _T("cmd_mvhl_poland_11.svg");
			else if (nSelVehicle == 4) bitmapID = _T("cmd_mvhl_poland_11.svg");
			else if (nSelVehicle == 5) bitmapID = _T("cmd_mvhl_poland_11.svg");
			else if (nSelVehicle == 6) bitmapID = _T("cmd_mvhl_poland_11.svg");
			else if (nSelVehicle == 7) bitmapID = _T("cmd_mvhl_poland_11.svg");
			else if (nSelVehicle == 8) bitmapID = _T("cmd_mvhl_poland_11.svg");
			else if (nSelVehicle == 9) bitmapID = _T("cmd_mvhl_poland_12.svg");
			else if(nSelVehicle == 10) bitmapID = _T("cmd_mvhl_poland_12.svg");
			else if(nSelVehicle == 11) bitmapID = _T("cmd_mvhl_poland_12.svg");
			else if(nSelVehicle == 12) bitmapID = _T("cmd_mvhl_poland_12.svg");
			else if(nSelVehicle == 13) bitmapID = _T("cmd_mvhl_poland_12.svg");
			else if(nSelVehicle == 14) bitmapID = _T("cmd_mvhl_poland_12.svg");
			else if(nSelVehicle == 15) bitmapID = _T("cmd_mvhl_poland_12.svg");			
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		ASSERT(0);
		nStndCode = 0;
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + bitmapID);
}

void CCMMvhlItemStdPolandDlg::Data2Dlg()
{
	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleTypeName));
	else
		m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleLoadName));

	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_Data.nStandardCode);
	SetSelVehicleList();

	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	SetCbxCurSelByStr(m_cmbSelVihicle, m_Data.SelVehicle);
	
	ChangeBitmap();

	double dNoseTailDist = 0.0;
	int    nNumVehicle = 0;
	
	if(m_bModify)
	{
		dNoseTailDist = m_Data.dNoseTailDist;
		nNumVehicle = m_Data.nNumVehicle;
	}

	CVehlDB db(m_pDoc);  
	if (db.GetStandardVehicleLoadValue(m_Data))	MakeItemEx();

	if(m_bModify)
	{
		m_Data.dNoseTailDist = dNoseTailDist;
		m_Data.nNumVehicle = nNumVehicle;
	}

	m_edtQ.SetEditUnit(m_Data.dPressureLoad);
	m_edtA.SetEditUnit(m_Data.dSpace);
	
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

BOOL CCMMvhlItemStdPolandDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_Data.SelVehicle = GetSelVihicleStr();

	m_Data.nStandardCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());

	CVehlDB db(m_pDoc);
	if(!db.GetStandardVehicleLoadValue(m_Data))  return FALSE;  

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
	
	if(m_Data.VehicleTypeName == _T("Tracked Vehicle"))
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

BOOL CCMMvhlItemStdPolandDlg::ChangeData()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_Data.SelVehicle = GetSelVihicleStr();
	m_Data.nStandardCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());;

	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleTypeName));
	else
		m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleLoadName));

	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_Data.nStandardCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);

	ChangeBitmap();

	CVehlDB db(m_pDoc);
	if(db.GetStandardVehicleLoadValue(m_Data)) MakeItemEx();    

	m_edtQ.SetEditUnit(m_Data.dPressureLoad);
	m_edtA.SetEditUnit(m_Data.dSpace);

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

	return TRUE;
}

BOOL CCMMvhlItemStdPolandDlg::ApplyOrOK()
{
	//if(!CheckData()) return FALSE;
	if(!Dlg2Data()) return FALSE;

	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;

	return TRUE;
}

void CCMMvhlItemStdPolandDlg::SetLoadTypeList(int nIndex)
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

void CCMMvhlItemStdPolandDlg::GetIndex(CString &csName, int *sitem)
{  
	for (int i = 0; i < D_MAX_COUNT; i++)
	{
		CString strDebug = CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i];
		if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == csName)
		{
			if(sitem) *sitem = i;
			return ;
		}
	}

	ASSERT(0);
	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdPolandDlg, CDialogMove)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,           OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB,      OnCmdMvhlTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_SEL_VEHICLE_CMB,    OnCmdSelVehicleCmb)  
	ON_BN_CLICKED   (IDC_CMD_APPLY,                   OnCmdApply)
	ON_BN_CLICKED   (IDC_CMD_MVHL_DYNAMIC_CHK,        OnCmdDynamicChk)
	ON_BN_CLICKED   (IDC_CMD_MVHL_AUTO_RDO,           OnCmdAutoUserRdo)
	ON_BN_CLICKED   (IDC_CMD_MVHL_USER_RDO,           OnCmdAutoUserRdo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdPolandDlg message handlers

BOOL CCMMvhlItemStdPolandDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitUnit();
	InitCombo();  

	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = _T("Vehicle K");
		m_Data.SelVehicle = _T("Class A");
		m_Data.nStandardCode = D_MVHL_PN_85S_10030_ROAD_BRG;
	}
	GetIndex(m_Data.VehicleTypeName);
	int nIndex = m_Data.nStandardCode;
	if (nIndex < 0) return TRUE;

	SetHeaderTitle();
	SetLoadTypeList(nIndex);
	
	Data2Dlg();
	
	AlignControl();
	ControlsShowHide();
	EnableDisableCtrl();

 	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->SetWindowText(_LS(IDS_WG_CMD_MVHL_POLAND_PHI_EQUATION));
 	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->SetWindowText(_LS(IDS_WG_CMD_MVHL_POLAND_PHI));	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdPolandDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdPolandDlg::OnCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);

	int nIndex = m_cmbCodeName.GetCurSel();

	SetSelVehicleList();
	ChangeData();

	AlignControl();
	ControlsShowHide();
	EnableDisableCtrl();
}

void CCMMvhlItemStdPolandDlg::OnCmdMvhlTypeCmb() 
{
	SetSelVehicleList();
	ChangeData();  

	AlignControl();
	ControlsShowHide();
	EnableDisableCtrl();
}

void CCMMvhlItemStdPolandDlg::OnCmdSelVehicleCmb()
{
	ChangeData();  

	AlignControl();
	ControlsShowHide();
	EnableDisableCtrl();
}

void CCMMvhlItemStdPolandDlg::OnCmdDynamicChk() 
{
	EnableDisableCtrl();
}

void CCMMvhlItemStdPolandDlg::OnCmdAutoUserRdo() 
{
	EnableDisableCtrl();
}

void CCMMvhlItemStdPolandDlg::OnCmdApply() 
{	
	ApplyOrOK();
}

void CCMMvhlItemStdPolandDlg::SetCbxCurSelByStr(CComboBox& Cbx, CString str)
{
	CString strTemp;
	for (int i = 0; i < Cbx.GetCount(); i++)
	{
		Cbx.GetLBText(i, strTemp);
		if(strTemp == str)
		{
			Cbx.SetCurSel(i);
			return;
		}
	}
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

#define __static_caVehicleLoadType static const array_pair_cstring<8> caVehicleLoadType = \
{ \
    std::make_pair(_T("Vehicle K"),                                _LS(IDS_WG_CMD_Vehicle_K)), \
    std::make_pair(_T("Vehicle S"),                                _LS(IDS_WG_CMD_Vehicle_S)), \
    std::make_pair(_T("Vehicle 2S"),                               _LS(IDS_WG_CMD_Vehicle_2S)), \
    std::make_pair(_T("Sidewalks, stairs and walkways"),           _LS(IDS_WG_CMD_Sidewalks_stairs_and_walkways)), \
    std::make_pair(_T("Main girders and supports"),                _LS(IDS_WG_CMD_Main_girders_and_supports)), \
    std::make_pair(_T("Service walkways and working platforms"),   _LS(IDS_WG_CMD_Service_walkways_and_working_platforms)), \
    std::make_pair(_T("Tracked Vehicle"),                          _LS(IDS_WG_CMD_Tracked_Vehicle)), \
    std::make_pair(_T("Wheeled Vehicle"),                          _LS(IDS_WG_CMD_Wheeled_Vehicle)), \
}; \

CString CCMMvhlItemStdPolandDlg::ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const
{
	__static_caVehicleLoadType;
	return ConvStrRaw2Trans(caVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdPolandDlg::ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const
{
	__static_caVehicleLoadType;
	return ConvStrTrans2Raw(caVehicleLoadType, strTrans);
}

#undef __static_caVehicleLoadType


#define __static_caSelVehicleLoadType static const array_pair_cstring<21> caSelVehicleLoadType = \
{ \
    std::make_pair(_T("Class A"),    _LS(IDS_WG_CMD_Class_A)), \
    std::make_pair(_T("Class B"),    _LS(IDS_WG_CMD_Class_B)), \
    std::make_pair(_T("Class C"),    _LS(IDS_WG_CMD_Class_C)), \
    std::make_pair(_T("Class D"),    _LS(IDS_WG_CMD_Class_D)), \
    std::make_pair(_T("Class E"),    _LS(IDS_WG_CMD_Class_E)), \
    std::make_pair(_T("Class 4"),    _LS(IDS_WG_CMD_Class_4)), \
    std::make_pair(_T("Class 8"),    _LS(IDS_WG_CMD_Class_8)), \
    std::make_pair(_T("Class 12"),   _LS(IDS_WG_CMD_Class_12)), \
    std::make_pair(_T("Class 16"),   _LS(IDS_WG_CMD_Class_16)), \
    std::make_pair(_T("Class 20"),   _LS(IDS_WG_CMD_Class_20)), \
    std::make_pair(_T("Class 24"),   _LS(IDS_WG_CMD_Class_24)), \
    std::make_pair(_T("Class 30"),   _LS(IDS_WG_CMD_Class_30)), \
    std::make_pair(_T("Class 40"),   _LS(IDS_WG_CMD_Class_40)), \
    std::make_pair(_T("Class 50"),   _LS(IDS_WG_CMD_Class_50)), \
    std::make_pair(_T("Class 60"),   _LS(IDS_WG_CMD_Class_60)), \
    std::make_pair(_T("Class 70"),   _LS(IDS_WG_CMD_Class_70)), \
    std::make_pair(_T("Class 80"),   _LS(IDS_WG_CMD_Class_80)), \
    std::make_pair(_T("Class 90"),   _LS(IDS_WG_CMD_Class_90)), \
    std::make_pair(_T("Class 100"),  _LS(IDS_WG_CMD_Class_100)), \
    std::make_pair(_T("Class 120"),  _LS(IDS_WG_CMD_Class_120)), \
    std::make_pair(_T("Class 150"),  _LS(IDS_WG_CMD_Class_150)), \
}; \

CString CCMMvhlItemStdPolandDlg::ConvSelVihicleStrRaw2Trans(const CString& strRaw) const
{
	__static_caSelVehicleLoadType;
	return ConvStrRaw2Trans(caSelVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdPolandDlg::ConvSelVihicleStrTrans2Raw(const CString& strTrans) const
{
	__static_caSelVehicleLoadType;
	return ConvStrTrans2Raw(caSelVehicleLoadType, strTrans);
}

#undef __static_caSelVehicleLoadType

#define __static_caVehicleLoadName static const array_pair_cstring<29> caVehicleLoadName = \
{ \
    std::make_pair(_T("Vehicle K"),                                _LS(IDS_WG_CMD_Vehicle_K)), \
    std::make_pair(_T("Vehicle S"),                                _LS(IDS_WG_CMD_Vehicle_S)), \
    std::make_pair(_T("Vehicle 2S"),                               _LS(IDS_WG_CMD_Vehicle_2S)), \
    std::make_pair(_T("Sidewalks, stairs and walkways"),           _LS(IDS_WG_CMD_Sidewalks_stairs_and_walkways)), \
    std::make_pair(_T("Main girders and supports"),                _LS(IDS_WG_CMD_Main_girders_and_supports)), \
    std::make_pair(_T("Service walkways and working platforms"),   _LS(IDS_WG_CMD_Service_walkways_and_working_platforms)), \
    std::make_pair(_T("Tracked Vehicle"),                          _LS(IDS_WG_CMD_Tracked_Vehicle)), \
    std::make_pair(_T("Wheeled Vehicle"),                          _LS(IDS_WG_CMD_Wheeled_Vehicle)), \
    std::make_pair(_T("Class A"),    _LS(IDS_WG_CMD_Class_A)), \
    std::make_pair(_T("Class B"),    _LS(IDS_WG_CMD_Class_B)), \
    std::make_pair(_T("Class C"),    _LS(IDS_WG_CMD_Class_C)), \
    std::make_pair(_T("Class D"),    _LS(IDS_WG_CMD_Class_D)), \
    std::make_pair(_T("Class E"),    _LS(IDS_WG_CMD_Class_E)), \
    std::make_pair(_T("Class 4"),    _LS(IDS_WG_CMD_Class_4)), \
    std::make_pair(_T("Class 8"),    _LS(IDS_WG_CMD_Class_8)), \
    std::make_pair(_T("Class 12"),   _LS(IDS_WG_CMD_Class_12)), \
    std::make_pair(_T("Class 16"),   _LS(IDS_WG_CMD_Class_16)), \
    std::make_pair(_T("Class 20"),   _LS(IDS_WG_CMD_Class_20)), \
    std::make_pair(_T("Class 24"),   _LS(IDS_WG_CMD_Class_24)), \
    std::make_pair(_T("Class 30"),   _LS(IDS_WG_CMD_Class_30)), \
    std::make_pair(_T("Class 40"),   _LS(IDS_WG_CMD_Class_40)), \
    std::make_pair(_T("Class 50"),   _LS(IDS_WG_CMD_Class_50)), \
    std::make_pair(_T("Class 60"),   _LS(IDS_WG_CMD_Class_60)), \
    std::make_pair(_T("Class 70"),   _LS(IDS_WG_CMD_Class_70)), \
    std::make_pair(_T("Class 80"),   _LS(IDS_WG_CMD_Class_80)), \
    std::make_pair(_T("Class 90"),   _LS(IDS_WG_CMD_Class_90)), \
    std::make_pair(_T("Class 100"),  _LS(IDS_WG_CMD_Class_100)), \
    std::make_pair(_T("Class 120"),  _LS(IDS_WG_CMD_Class_120)), \
    std::make_pair(_T("Class 150"),  _LS(IDS_WG_CMD_Class_150)), \
}; \

CString CCMMvhlItemStdPolandDlg::ConvVehicleNameStrRaw2Trans(const CString& strRaw) const
{
	__static_caVehicleLoadName;
	return ConvStrRaw2Trans(caVehicleLoadName, strRaw);
}

CString CCMMvhlItemStdPolandDlg::ConvVehicleNameStrTrans2Raw(const CString& strTrans) const
{
	__static_caVehicleLoadName;
	return ConvStrTrans2Raw(caVehicleLoadName, strTrans);
}

#undef __static_caVehicleLoadName

CString CCMMvhlItemStdPolandDlg::GetVehicleTypeStr() const
{
	CString CurTransStr;
	m_cmbVehicleType.GetWindowText(CurTransStr);
	return ConvVehicleTypeStrTrans2Raw(CurTransStr);
}

CString CCMMvhlItemStdPolandDlg::GetSelVihicleStr() const
{
	CString CurTransStr;
	m_cmbSelVihicle.GetWindowText(CurTransStr);
	return ConvSelVihicleStrTrans2Raw(CurTransStr);
}