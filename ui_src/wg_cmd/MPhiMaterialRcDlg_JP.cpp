// MPhiMaterialRcDlg_JP.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiMaterialRcDlg_JP.h"
#include "MPhiMaterialDlg_JP.h"

#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_MATL.h"
#include "..\wg_db\DB_ST_DT_MATD_REIN.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "MPhiParameterDef.h"

//#include "..\wg_base\wg_base_DlgUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMBO_SHEET_TYPE_CARBON_FIBER _LS(IDS_SHEATH_CARBON_MATE_SEAT_01)
#define COMBO_SHEET_TYPE_AFRP _LS(IDS_SHEATH_CARBON_MATE_SEAT_02)

#define COMBO_SHEET_CF_High_Strength_Type1 _LS(IDS_SHEATH_CARBON_MATE_SEAT1_01)
#define COMBO_SHEET_CF_High_Strength_Type2 _LS(IDS_SHEATH_CARBON_MATE_SEAT1_02)
#define COMBO_SHEET_CF_Medium_Elasticity_Type _LS(IDS_SHEATH_CARBON_MATE_SEAT1_03)
#define COMBO_SHEET_CF_High_Elasticity_Type _LS(IDS_SHEATH_CARBON_MATE_SEAT1_04)

#define COMBO_SHEET_AFRT_aramid1 _LS(IDS_SHEATH_CARBON_MATE_SEAT2_01)
#define COMBO_SHEET_AFRT_aramid2_E78400 _LS(IDS_SHEATH_CARBON_MATE_SEAT2_02)
#define COMBO_SHEET_AFRT_aramid2_E78000 _LS(IDS_SHEATH_CARBON_MATE_SEAT2_03)

#define COMBO_VOLUME_200  _T("200")
#define COMBO_VOLUME_235  _T("235")
#define COMBO_VOLUME_280  _T("280")
#define COMBO_VOLUME_300  _T("300")
#define COMBO_VOLUME_300_1  _T("300-1")
#define COMBO_VOLUME_300_2  _T("300-2")
#define COMBO_VOLUME_350  _T("350")
#define COMBO_VOLUME_400  _T("400")
#define COMBO_VOLUME_415  _T("415")
#define COMBO_VOLUME_450  _T("450")
#define COMBO_VOLUME_525  _T("525")
#define COMBO_VOLUME_600  _T("600")
#define COMBO_VOLUME_623  _T("623")
#define COMBO_VOLUME_700  _T("700")
#define COMBO_VOLUME_830  _T("830")

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialRcDlg_JP dialog


CMPhiMaterialRcDlg_JP::CMPhiMaterialRcDlg_JP(MPhiDataAll* pMPhiDataAll, CMPhiMaterialDlg_JP* pParent /*=NULL*/, int nType /*=0*/)
	: CDlgChild(CMPhiMaterialRcDlg_JP::IDD, pParent)
{
	m_pParent = pParent;
	m_pDoc = CDBDoc::GetDocPoint();
	m_Fc = 0.0;
	m_MatName2 = _T("");
	m_SubBarName2 = _T("");
	m_SheathRcName2 = _T("");
	m_SheathRc_SNameMain2 = _T("");
	m_SheathRc_SNameSub2 = _T("");
	m_SheathSteelName2 = _T("");

	m_kMatl = 0;
	m_Fy = 0.0;
	m_Fys = 0.0;
	m_Ec = 0.0;
	m_Ack = 0.0;
	m_SheathFy = 0.0;
	m_SheathFys = 0.0;
	m_SteelSurrend = 0.0;
	m_CarbonHeight = 0.0;
	m_CarbonFiber = 0.0;
	m_CarbonTens = 0.0;

	m_strCurrentItem = _T("");

	// no use JP
	m_bTransfer = FALSE; 	
	m_bLambda = FALSE;
	m_bChkTransfer = FALSE;

	m_pMPhiDataAll = pMPhiDataAll;
}

void CMPhiMaterialRcDlg_JP::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_CONCMAT_LIST, m_ListCtrl);
	DDX_Check(pDX, IDC_CMD_SHEATH_GRP_CBX, m_bSheath);

	DDX_Control(pDX, IDC_CMD_CONC_REBARCODE, m_RebarCode);
	DDX_Control(pDX, IDC_CMD_CONC_SUBNAME, m_SubBarName);
	DDX_Control(pDX, IDC_CMD_CONC_MAINNAME, m_MainBarName);
	DDX_Control(pDX, IDC_CMD_CONCMAT_CODE, m_Code);
	DDX_Control(pDX, IDC_CMD_CONC_MAT, m_MatName);
	DDX_Control(pDX, IDC_SHEATH_RC_CON_CODE_COB, m_SheathRcCode);
	DDX_Control(pDX, IDC_SHEATH_RC_CON_MAIN_COB, m_SheathRcName);
	DDX_Control(pDX, IDC_SHEATH_RC_S_CODE_COB, m_SheathRc_SCode);
	DDX_Control(pDX, IDC_SHEATH_RC_S_MAIN_REBAR_COB, m_SheathRc_SNameMain);
	DDX_Control(pDX, IDC_SHEATH_RC_S_SUB_REBAR_COB, m_SheathRc_SNameSub);
	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_CODE_COB, m_SheathSteelCode);
	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_NAME_COB, m_SheathSteelName);
	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_REBAR_CODE_CBO, m_SteelRebarCode);
	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_REBAR_MAIN_CBO, m_SteelRebarName);

	DDX_Control(pDX, IDC_CMD_SHEATH_CARBON_SEAT_COB, m_SheathCarbonSeat);
	DDX_Control(pDX, IDC_CMD_SHEATH_CARBON_TYPE_COB, m_SheathCarbonType);
	DDX_Control(pDX, IDC_CMD_SHEATH_CARBON_VOLUME_COB, m_SheathCarbonVolume);

	DDX_Control(pDX, IDC_CMD_CONC_MATFYUNIT, m_FyUnit);
	DDX_Control(pDX, IDC_CMD_CONC_MATFYSUNIT, m_FysUnit);
	DDX_Control(pDX, IDC_CMD_CONC_MATFCUNIT, m_FcUnit);
	DDX_Control(pDX, IDC_SHEATH_RC_S_FY_UNIT, m_SheathFyUnit);
	DDX_Control(pDX, IDC_SHEATH_RC_CON_EC_UNIT, m_EcUnit);
	DDX_Control(pDX, IDC_SHEATH_RC_CON_ACK_UNIT, m_AckUnit);
	DDX_Control(pDX, IDC_SHEATH_RC_S_FYS_UNIT, m_SheathFysUnit);
	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_SURRENDER_UNIT, m_SteelSurrendUnit);
	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_REBAR_FY_UNIT, m_SteelRebarFyUnit);
	DDX_Control(pDX, IDC_CMD_SHEATH_CARBON_MATE_HEIGHT_UNIT, m_CarbonHeightUnit);
	DDX_Control(pDX, IDC_CMD_SHEATH_CARBON_MATE_FIBER_UNIT, m_CarbonFiberUnit);
	DDX_Control(pDX, IDC_CMD_SHEATH_CARBON_MATE_TENS_UNIT, m_CarbonTensUnit);

	DDX_Text(pDX, IDC_CMD_CONC_MATFC, m_Fc);
	DDX_Text(pDX, IDC_CMD_CONC_MAT2, m_MatName2);
	DDX_Text(pDX, IDC_CMD_CONC_SUBNAME2, m_SubBarName2);
	DDX_Text(pDX, IDC_CMD_CONC_MAINNAME2, m_MainBarName2);
	DDX_Text(pDX, IDC_SHEATH_RC_CON_MAIN_EDT, m_SheathRcName2);
	DDX_Text(pDX, IDC_SHEATH_RC_S_MAIN_REBAR_EDT, m_SheathRc_SNameMain2);
	DDX_Text(pDX, IDC_SHEATH_RC_S_SUB_REBAR_EDT, m_SheathRc_SNameSub2);
	DDX_Text(pDX, IDC_CMD_SHEATH_STELL_NAME_EDT, m_SheathSteelName2);
	DDX_Text(pDX, IDC_CMD_SHEATH_STELL_REBAR_MAIN_EDT, m_SteelRebarName2);

	DDX_Text(pDX, IDC_CMD_CONC_MATFY, m_Fy);
	DDX_Text(pDX, IDC_CMD_CONC_MATFYS, m_Fys);
	DDX_Text(pDX, IDC_SHEATH_RC_CON_EC_EDT, m_Ec);
	DDX_Text(pDX, IDC_SHEATH_RC_CON_ACK_EDT, m_Ack);
	DDX_Text(pDX, IDC_SHEATH_RC_S_FY_EDT, m_SheathFy);
	DDX_Text(pDX, IDC_SHEATH_RC_S_FYS_EDT, m_SheathFys);
	DDX_Text(pDX, IDC_CMD_SHEATH_STELL_SURRENDER_EDT, m_SteelSurrend);
	DDX_Text(pDX, IDC_CMD_SHEATH_STELL_REBAR_FY_EDT, m_SteelRebarFy);
	DDX_Text(pDX, IDC_CMD_SHEATH_CARBON_MATE_HEIGHT_EDT, m_CarbonHeight);
	DDX_Text(pDX, IDC_CMD_SHEATH_CARBON_MATE_FIBER_EDT, m_CarbonFiber);
	DDX_Text(pDX, IDC_CMD_SHEATH_CARBON_MATE_TENS_EDT, m_CarbonTens);
	DDX_Control(pDX, IDC_CMD_SHEATH_RC_REIN_FOLD, m_FoldGrp);
}

BEGIN_MESSAGE_MAP(CMPhiMaterialRcDlg_JP, CDlgChild)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()

	ON_NOTIFY(NM_CLICK, IDC_CMD_CONCMAT_LIST, OnClickDgnConcmatList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_CONCMAT_LIST, OnItemchangedDgnConcmatList)
	ON_CBN_SELCHANGE(IDC_CMD_CONC_MAT, OnSelchangeDgnConcMat)
	ON_CBN_SELCHANGE(IDC_CMD_CONCMAT_CODE, OnSelchangeDgnConcmatCode)
	ON_CBN_SELCHANGE(IDC_CMD_CONC_MAINNAME, OnSelchangeDgnConcMainname)
	ON_CBN_SELCHANGE(IDC_CMD_CONC_SUBNAME, OnSelchangeDgnConcSubname)
	//ON_BN_CLICKED(IDC_CMD_LAMBDA_CHK, OnClickLambdaChk)
	//ON_BN_CLICKED(IDC_CMD_TRANSFER_CHK, OnClickTransferChk)
	//ON_BN_CLICKED(IDC_CMD_CONCMAT_CLOSE, OnDgnConcmatClose)
	ON_CBN_SELCHANGE(IDC_CMD_CONC_REBARCODE, OnSelchangeDgnConcRebarcode)
	//ON_BN_CLICKED(IDC_CMD_CONC_EL, OnDgnConcElasticity)

	ON_BN_CLICKED(IDC_CMD_SHEATH_GRP_CBX, OnClickbSheath)
	ON_CBN_SELCHANGE(IDC_SHEATH_RC_CON_CODE_COB, OnChangeSheathRcCode)
	ON_CBN_SELCHANGE(IDC_SHEATH_RC_CON_MAIN_COB, OnChangeSheathRcName)
	ON_CBN_SELCHANGE(IDC_SHEATH_RC_S_CODE_COB, OnChangeSheathRc_SCode)
	ON_CBN_SELCHANGE(IDC_SHEATH_RC_S_MAIN_REBAR_COB, OnChangeSheathRc_SMainName)
	ON_CBN_SELCHANGE(IDC_SHEATH_RC_S_SUB_REBAR_COB, OnChangeSheathRc_SSubName)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_STELL_CODE_COB, OnChangeSheathSteelCode)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_STELL_NAME_COB, OnChangeSheathSteelName)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_STELL_REBAR_CODE_CBO, OnChangeSheathSteelRebarCode)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_STELL_REBAR_MAIN_CBO, OnChangeSheathSteelRebarName)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_CARBON_SEAT_COB, OnChangeSheathSeatCode)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_CARBON_TYPE_COB, OnChangeSheathType)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_CARBON_VOLUME_COB, OnChangeSheathVolume)
	ON_BN_CLICKED(IDC_BTN_MPHI_MATL_RC_ADD_MOD, OnAddModBtn)
	ON_BN_CLICKED(IDC_BTN_MPHI_MATL_RC_DELETE, OnDeleteBtn)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMPhiMaterialRcDlg_JP::OnInitDialog()
{
	CDlgChild::OnInitDialog();

	// 창이 완전히 그려진 후 스크롤 적용
	SetTimer(1001, 10, nullptr);

	//AlignControl();
	// Initialize Grid, Unit, Matl Code.
	Init_GridData();
	Init_Unit();
	Init_MatlCode();
	Init_Items();

	// Sheath 
	m_bSheath = FALSE;
	Init_SheathCase();
	SheathShowInit();

	UpdateData(FALSE);

	m_FoldGrp.InitControl(this, CMPhiMaterialRcDlg_JP::IDD, IDC_CMD_SHEATH_RC_REIN_FOLD, FALSE, FALSE);
	if(m_FoldGrp.GetFoldState()) m_FoldGrp.SetFoldState(FALSE, FALSE); // 접힌 상태로 시작
	m_FoldGrp.SetFoldState(TRUE, TRUE); // 접힌 상태로 시작
	AlignBtnMove();

	return TRUE;
}

void CMPhiMaterialRcDlg_JP::AlignBtnMove()
{
	CArray <UINT, UINT> aControls;
	aControls.Add(IDC_BTN_MPHI_MATL_RC_ADD_MOD);
	aControls.Add(IDC_BTN_MPHI_MATL_RC_DELETE);

	CRect rRect, rBtnRect;
	GetDlgItem(IDC_CMD_SHEATH_RC_REIN_FOLD)->GetWindowRect(rRect);
	GetDlgItem(IDC_BTN_MPHI_MATL_RC_ADD_MOD)->GetWindowRect(rBtnRect);
	int dYDist = (rRect.bottom - rBtnRect.top) + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, dYDist);
}

void CMPhiMaterialRcDlg_JP::FoldResetVisible()
{
	if (m_FoldGrp.GetFoldState())
	{
		// 접혔을 때
		m_FoldGrp.SetFoldState(FALSE, FALSE);
		m_FoldGrp.SetFoldState(TRUE, TRUE);
	}
	else
	{
		m_FoldGrp.SetFoldState(FALSE);
	}
	AlignBtnMove();
}

void CMPhiMaterialRcDlg_JP::Init_Scroll()
{
	m_nScrollPos = 0;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = 410;  // 컨텐츠 크기 설정
	si.nPage = 82; // 한 번에 표시되는 크기
	SetScrollInfo(SB_VERT, &si, TRUE);
	ShowScrollBar(SB_VERT, TRUE);
}

void CMPhiMaterialRcDlg_JP::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1001)
	{
		KillTimer(1001);

		Init_Scroll();
	}

	CDialog::OnTimer(nIDEvent);
}

void CMPhiMaterialRcDlg_JP::AlignControl()
{
	//CRect rRef;
	//CRect rToMove;
	//int nDistY;
	//CArray<UINT, UINT> aControls;

	//// control move
	//aControls.RemoveAll();
	////aControls.Add(IDC_CMD_LAMBDA_EDT);
	////aControls.Add(IDC_CMD_LAMBDA_CHK);

	//GetDlgItem(IDC_CMD_CONC_MATFC)->GetWindowRect(rRef);
	////if (m_bTransfer)
	////{
	////	GetDlgItem(IDC_CMD_TRANSFER_EDT)->GetWindowRect(rToMove);
	////	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	////}
	////else
	////{
	////	GetDlgItem(IDC_CMD_LAMBDA_EDT)->GetWindowRect(rToMove);
	////	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	////}
	//CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	//// Concrete Selection Group resize
	//CRect rectResize;
	//GetDlgItem(IDC_CMD_CONC_GRP)->GetWindowRect(rectResize);
	////GetDlgItem(IDC_CMD_LAMBDA_EDT)->GetWindowRect(rRef);
	//rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);

	//ScreenToClient(rectResize);
	//GetDlgItem(IDC_CMD_CONC_GRP)->MoveWindow(rectResize);

	//// Rebar Selection Group(GR) move
	//aControls.RemoveAll();
	//CDlgUtil::GetCtrlIDByIncRect(this, aControls, IDC_CMD_REBAR_GRP, TRUE);

	//GetDlgItem(IDC_CMD_CONC_GRP)->GetWindowRect(rRef);
	//GetDlgItem(IDC_CMD_REBAR_GRP)->GetWindowRect(rToMove);
	//nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	//CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	//// control move
	//aControls.RemoveAll();
	//aControls.Add(IDC_CMD_CONCMAT_MODIFY);
	//aControls.Add(IDC_CMD_CONCMAT_CLOSE);

	//GetDlgItem(IDC_CMD_REBAR_GRP)->GetWindowRect(rRef);
	//GetDlgItem(IDC_CMD_CONCMAT_MODIFY)->GetWindowRect(rToMove);
	//nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	//CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	//// ResizeDialog
	//UINT nLastCtrlID = IDC_CMD_CONCMAT_MODIFY;
	//CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	//CRect rectLast;
	//pWndLast->GetWindowRect(rectLast);
	//CRect r;
	//GetWindowRect(r);
	//r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	//MoveWindow(r);
}

#define COLUMN_SIZE 6
void CMPhiMaterialRcDlg_JP::Init_GridData()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(), dwStyle);

	int nColSize = 6; // Column Size
	LV_COLUMN lvcolumn;

	TCHAR* list[COLUMN_SIZE] = { _T("ID"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME), _T("fc"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_MAIN_BAR), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_SUB_BAR), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_MATD_REIN) };
	int width[COLUMN_SIZE] = { 40,140,140,120,140,100 };
	for (int i = 0; i < COLUMN_SIZE; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i, &lvcolumn);
	}
	// Set Design Material Data using Analysis Material Data.
	const CArray<T_MATL_K, T_MATL_K>& arKey = m_pMPhiDataAll->aMatdK;
	// Arrange Data for initializing Material Data.
	T_MATD_D mData;
	int iListNo = 0;
	for (int i = 0; i < arKey.GetSize(); i++)
	{
		int iID = arKey.GetAt(i);
		mData.Initialize();
		if(!m_pMPhiDataAll->GetMatd(iID, mData)) continue;
		if (mData.Type != _T("C")) continue; // Concrete Type

		CString strID = _T(""), strFc = _T("");
		strID.Format(_T("%7d"), iID);
		strFc.Format(_T("%g"), mData.Data1.Design.C_fc);
		CString strNa = mData.Name;
		CString strMbar = mData.MainRebar_RebarName;
		CString strSbar = mData.SubRebar_RebarName;
		BOOL bMatdRein = m_pMPhiDataAll->ExistMatdRein(iID);

		Write_ListBox(iListNo, strID, strNa, strFc,  strMbar, strSbar, bMatdRein);
		iListNo++;
	}

	GetDlgItem(IDC_CMD_CONC_MAT2)->ShowWindow(SW_HIDE);
}

void CMPhiMaterialRcDlg_JP::Write_ListBox(int iIndex, CString strID, CString strNa, CString strFc,  CString strMbar, CString strSbar, BOOL bMatdRein)
{
	LV_ITEM lvitem;
	LPTSTR szText[COLUMN_SIZE];
	szText[0] = (LPTSTR)(LPCTSTR)strID;
	szText[1] = (LPTSTR)(LPCTSTR)strNa;
	szText[2] = (LPTSTR)(LPCTSTR)strFc;
	szText[3] = (LPTSTR)(LPCTSTR)strMbar;
	szText[4] = (LPTSTR)(LPCTSTR)strSbar;
	szText[5] = bMatdRein ? _T("O") : _T("X");
	// Input ListBox.
	for (int i = 0; i < 7; i++)
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iIndex;
		lvitem.iSubItem = i;
		lvitem.pszText = szText[i];
		if (i == 0)	m_ListCtrl.InsertItem(&lvitem);
		else		m_ListCtrl.SetItem(&lvitem);
	}
}

void CMPhiMaterialRcDlg_JP::Init_Unit()
{
	m_FcUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FyUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FysUnit.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_EcUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_AckUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_SheathFyUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_SheathFysUnit.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_SteelSurrendUnit.SetUnitType(D_UNITSYS_BASE_ELAST); // kN / m2 
	m_SteelRebarFyUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_CarbonHeightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH); // mm
	m_CarbonFiberUnit.SetUnitType(D_UNITSYS_BASE_STRESS); // N / mm2
	m_CarbonTensUnit.SetUnitType(D_UNITSYS_BASE_STRESS); // N / mm2

	// no use JP
	m_edtLambda.SetUnitType(D_UNITSYS_NONE);
	m_edtTransfer.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_TransferUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
}

void CMPhiMaterialRcDlg_JP::Init_MatlCode()
{
	// Reset Contents.
	m_Code.ResetContent();
	m_RebarCode.ResetContent();

	CArray<CString, CString&> arDesignCode;
	CString strMatlType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strMatlType, arDesignCode);
	// Initialize MatlCode, RebarCode.
	m_Code.AddString(_T("None"));
	m_RebarCode.AddString(_T("None"));
	for (int i = 0; i < arDesignCode.GetSize(); i++)
	{
		if (arDesignCode[i] == MATLCODE_CON_JIS || arDesignCode[i] == MATLCODE_CON_JIS_CIVIL)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arDesignCode[i]);
#endif
			m_Code.AddString(arDesignCode[i]);
			m_RebarCode.AddString(arDesignCode[i]);
		}
	}
	for (int i = 0; i < arDesignCode.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arDesignCode[i]);
#endif
		m_RebarCode.AddString(arDesignCode[i]);
	}

	EnableEditBox(FALSE);
	EnableRebarEditBox(FALSE);
}

void CMPhiMaterialRcDlg_JP::EnableEditBox(BOOL bCheck)
{
	if (bCheck)	// None.
	{
		GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_NAME));
		GetDlgItem(IDC_CMD_CONC_MAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_CONC_MAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_CONC_MATFC)->EnableWindow(TRUE);
	}
	else	// DB.
	{
		GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_RCPC_DLG_IDC_STATIC_MATNAME));
		GetDlgItem(IDC_CMD_CONC_MAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_CONC_MAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_CONC_MATFC)->EnableWindow(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::EnableRebarEditBox(BOOL bCheck)
{
	if (bCheck)	// None.
	{
		GetDlgItem(IDC_CMD_STATIC_MATEFY)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_STEEL_MAIN_NAME));
		GetDlgItem(IDC_CMD_STATIC_MATAFV)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_STEEL_SUB_NAME));
		GetDlgItem(IDC_CMD_CONC_MAINNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_CONC_MAINNAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_CONC_MAINNAME2)->SetWindowText(_T(""));
		GetDlgItem(IDC_CMD_CONC_MATFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_CONC_MATFY)->SetWindowText(_T("0"));
		GetDlgItem(IDC_CMD_CONC_SUBNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_CONC_SUBNAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_CONC_SUBNAME2)->SetWindowText(_T(""));
		GetDlgItem(IDC_CMD_CONC_MATFYS)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_CONC_MATFYS)->SetWindowText(_T("0"));
	}
	else	// DB.
	{
		GetDlgItem(IDC_CMD_STATIC_MATEFY)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_RCPC_DLG_IDC_CMD_STATIC_MATEFY));
		GetDlgItem(IDC_CMD_STATIC_MATAFV)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_RCPC_DLG_IDC_CMD_STATIC_MATAFV));
		GetDlgItem(IDC_CMD_CONC_MAINNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_CONC_MAINNAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_CONC_MATFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_CONC_SUBNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_CONC_SUBNAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_CONC_MATFYS)->EnableWindow(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::Init_Items()
{
	//GetDlgItem(IDC_CMD_TRANSFER_CHK)->ShowWindow(m_bTransfer);
	//GetDlgItem(IDC_CMD_TRANSFER_EDT)->ShowWindow(m_bTransfer);
	//GetDlgItem(IDC_CMD_TRANSFER_UNIT)->ShowWindow(m_bTransfer);

	if (!(CDBLib::GetConCodeName() == _T("Eurocode2") || CDBLib::GetConCodeName() == _T("Eurocode2:04") ||
		CDBLib::GetConCodeName() == _T("Eurocode2-2:05"))) // Coded by Seungjun ('20070613) MNet:No.2313.
	{
		GetDlgItem(IDC_CMD_CONC_EL)->ShowWindow(SW_HIDE);
		return;
	}

	GetDlgItem(IDC_CMD_CONC_EL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CMD_CONC_EL)->EnableWindow(FALSE);
}

void CMPhiMaterialRcDlg_JP::Init_SheathCase()
{
	m_SheathRcName2 = _T("");
	m_SheathRc_SNameMain2 = _T("");
	m_SheathRc_SNameSub2 = _T("");
	m_SheathSteelName2 = _T("");
	m_SteelRebarName2 = _T("");
	m_Ec = 0.0;
	m_Ack = 0.0;
	m_SheathFy = 0.0;
	m_SheathFys = 0.0;
	m_SteelSurrend = 0.0;
	m_SteelRebarFy = 0.0;
	m_CarbonHeight = 0.0;
	m_CarbonFiber = 0.0;
	m_CarbonTens = 0.0;

	m_SheathRcCode.EnableWindow(m_bSheath);
	m_SheathRcName.EnableWindow(m_bSheath);
	m_SheathRcName.EnableWindow(m_bSheath);
	m_SheathRc_SCode.EnableWindow(m_bSheath);
	m_SheathRc_SNameMain.EnableWindow(m_bSheath);
	m_SheathRc_SNameSub.EnableWindow(m_bSheath);
	m_SheathSteelCode.EnableWindow(m_bSheath);
	m_SheathSteelName.EnableWindow(m_bSheath);
	m_SteelRebarCode.EnableWindow(m_bSheath);
	m_SteelRebarName.EnableWindow(m_bSheath);
	m_SheathCarbonSeat.EnableWindow(m_bSheath);
	m_SheathCarbonType.EnableWindow(m_bSheath);
	m_SheathCarbonVolume.EnableWindow(m_bSheath);

	EnableSheathEditBox(m_bSheath, eSheathType::kAll);
	if (m_bSheath)
	{
		Init_SheathComboBox();
		SheathShowInit();
	}
}

void CMPhiMaterialRcDlg_JP::Set_SheathCase(const T_MATD_REIN_D& reinD)
{
	m_SheathRcCode.EnableWindow(m_bSheath);
	m_SheathRcName.EnableWindow(m_bSheath);
	m_SheathRcName.EnableWindow(m_bSheath);
	m_SheathRc_SCode.EnableWindow(m_bSheath);
	m_SheathRc_SNameMain.EnableWindow(m_bSheath);
	m_SheathRc_SNameSub.EnableWindow(m_bSheath);
	m_SheathSteelCode.EnableWindow(m_bSheath);
	m_SheathSteelName.EnableWindow(m_bSheath);
	m_SteelRebarCode.EnableWindow(m_bSheath);
	m_SteelRebarName.EnableWindow(m_bSheath);
	m_SheathCarbonSeat.EnableWindow(m_bSheath);
	m_SheathCarbonType.EnableWindow(m_bSheath);
	m_SheathCarbonVolume.EnableWindow(m_bSheath);

	Init_SheathComboBox();

	BOOL bUserRC = (reinD.CoverRC.strC_CodeName == _T("None") || reinD.CoverRC.strC_CodeName == _T(""));
	EnableSheathEditBox(bUserRC, eSheathType::kRcCode);
	GetDlgItem(IDC_SHEATH_RC_CON_MAIN_COB)->ShowWindow(bUserRC ? FALSE : TRUE);
	GetDlgItem(IDC_SHEATH_RC_CON_MAIN_EDT)->ShowWindow(bUserRC);

	BOOL bUserRebarMain = (reinD.CoverRC.strR_CodeName == _T("None") || reinD.CoverRC.strR_CodeName == _T(""));
	EnableSheathEditBox(bUserRebarMain, eSheathType::kRc_SCode);
	GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_COB)->ShowWindow(bUserRebarMain ? FALSE : TRUE);
	GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_COB)->ShowWindow(bUserRebarMain ? FALSE : TRUE);
	GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_EDT)->ShowWindow(bUserRebarMain);
	GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_EDT)->ShowWindow(bUserRebarMain);

	BOOL bUserSPName = (reinD.CoverSP.strCodeName == _T("None") || reinD.CoverSP.strCodeName == _T(""));
	EnableSheathEditBox(bUserSPName, eSheathType::kSteelCode);
	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(bUserSPName ? FALSE : TRUE);
	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(bUserSPName);

	BOOL bUserSPCode = (reinD.CoverSP.strR_CodeName == _T("None") || reinD.CoverSP.strR_CodeName == _T(""));
	EnableSheathEditBox(bUserSPCode, eSheathType::kSteelRebarCode);
	GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_CBO)->ShowWindow(bUserSPCode ? FALSE : TRUE);
	GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_EDT)->ShowWindow(bUserSPCode);

	BOOL bUserCf = (reinD.CoverCF.nMainType == T_MATD_REIN_CF::kUserInput);
	EnableSheathEditBox(bUserCf, eSheathType::kSeat);
	m_SheathCarbonType.EnableWindow(bUserCf ? FALSE : TRUE);
	m_SheathCarbonVolume.EnableWindow(bUserCf ? FALSE : TRUE);

	//철근콘크리트(Rebar Concrete) 피복
	SetCtrlAtReinRC(&reinD.CoverRC);
	//강판(Steel Plate) 피복
	SetCtrlAtReinSP(&reinD.CoverSP);
	//탄소섬유(Carbon fiber) 피복
	SetCtrlAtReinCF(&reinD.CoverCF);
}

void CMPhiMaterialRcDlg_JP::SetCtrlAtReinRC(const T_MATD_REIN_RC* pRC)
{
	int iIndex = 0;
	CString strCode = _T("");

	iIndex = m_SheathRcCode.FindStringExact(-1, pRC->strC_CodeName);
	if (iIndex >= 0) m_SheathRcCode.SetCurSel(iIndex);
	else			 m_SheathRcCode.SetCurSel(0);

	iIndex = m_SheathRcCode.GetCurSel();
	strCode = _T("");
	if (iIndex != -1) {
		m_SheathRcCode.GetLBText(iIndex, strCode);

		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arName);

		m_SheathRcName.ResetContent();
		for (int i = 0; i < arName.GetSize(); i++)	m_SheathRcName.AddString(arName[i]);
	}

	iIndex = m_SheathRcName.FindStringExact(-1, pRC->strC_CodeMatlName);
	if (iIndex >= 0) m_SheathRcName.SetCurSel(iIndex);
	else			 m_SheathRcName.SetCurSel(0);

	m_Ack = pRC->dC_Fc;
	m_Ec = pRC->dC_Elast;

	iIndex = m_SheathRc_SCode.FindStringExact(-1, pRC->strR_CodeName);
	if (iIndex >= 0) m_SheathRc_SCode.SetCurSel(iIndex);
	else			 m_SheathRc_SCode.SetCurSel(0);

	iIndex = m_SheathRc_SCode.GetCurSel();
	strCode = _T("");
	if (iIndex != -1) {
		m_SheathRc_SCode.GetLBText(iIndex, strCode);

		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetRebarNameList(strCode, arName);

		m_SheathRc_SNameMain.ResetContent();
		m_SheathRc_SNameSub.ResetContent();

		for (int i = 0; i < arName.GetSize(); i++)
		{
			m_SheathRc_SNameMain.AddString(arName[i]);
			m_SheathRc_SNameSub.AddString(arName[i]);
		}
	}

	iIndex = m_SheathRc_SNameMain.FindStringExact(-1, pRC->strMR_RebarName);
	if (iIndex >= 0) m_SheathRc_SNameMain.SetCurSel(iIndex);
	else			 m_SheathRc_SNameMain.SetCurSel(0);

	iIndex = m_SheathRc_SNameSub.FindStringExact(-1, pRC->strSR_RebarName);
	if (iIndex >= 0) m_SheathRc_SNameSub.SetCurSel(iIndex);
	else			 m_SheathRc_SNameSub.SetCurSel(0);

	m_SheathRc_SNameMain2 = pRC->strMR_RebarName;
	m_SheathRc_SNameSub2 = pRC->strSR_RebarName;

	m_SheathFy = pRC->dMR_Fy;
	m_SheathFys = pRC->dSR_Fy;
}

void CMPhiMaterialRcDlg_JP::SetCtrlAtReinSP(const T_MATD_REIN_SP* pSP)
{
	int iIndex = 0;
	iIndex = m_SheathSteelCode.FindStringExact(-1, pSP->strCodeName);
	if (iIndex >= 0) m_SheathSteelCode.SetCurSel(iIndex);
	else			 m_SheathSteelCode.SetCurSel(0);

	iIndex = m_SheathSteelCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1) {
		m_SheathSteelCode.GetLBText(iIndex, strCode);

		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetSteelNameList(strCode, arName);

		m_SheathSteelName.ResetContent();
		for (int i = 0; i < arName.GetSize(); i++) m_SheathSteelName.AddString(arName[i]);
	}

	iIndex = m_SheathSteelName.FindStringExact(-1, pSP->strCodeMatlName);
	if (iIndex >= 0) m_SheathSteelName.SetCurSel(iIndex);
	else			 m_SheathSteelName.SetCurSel(0);

	m_SheathSteelName2 = pSP->strCodeMatlName;
	m_SteelSurrend = pSP->dFy;

	// Rebar
	iIndex = m_SteelRebarCode.FindStringExact(-1, pSP->strR_CodeName);
	if (iIndex >= 0) m_SteelRebarCode.SetCurSel(iIndex);
	else			 m_SteelRebarCode.SetCurSel(0);

	iIndex = m_SteelRebarCode.GetCurSel();
	strCode = _T("");
	if (iIndex != -1) 
	{
		m_SteelRebarCode.GetLBText(iIndex, strCode);

		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetRebarNameList(strCode, arName);

		m_SteelRebarName.ResetContent();
		for (int i = 0; i < arName.GetSize(); i++)
		{
			m_SteelRebarName.AddString(arName[i]);
		}
	}	

	iIndex = m_SteelRebarName.FindStringExact(-1, pSP->strAnchor_RebarName);
	if (iIndex >= 0) m_SteelRebarName.SetCurSel(iIndex);
	else			 m_SteelRebarName.SetCurSel(0);

	m_SteelRebarName2 = pSP->strAnchor_RebarName;

	m_SteelRebarFy = pSP->dAnchor_Fy;
}

void CMPhiMaterialRcDlg_JP::SetCtrlAtReinCF(const T_MATD_REIN_CF* pCF)
{
	int iIndex = 0;
	CString strValue;
	switch (pCF->nMainType)
	{
	case T_MATD_REIN_CF::kUserInput:
		strValue = _T("None");
		break;
	case T_MATD_REIN_CF::kCarbonFiber:
		strValue = COMBO_SHEET_TYPE_CARBON_FIBER;
		break;
	case T_MATD_REIN_CF::kAFRP:
		strValue = COMBO_SHEET_TYPE_AFRP;
		break;
	default:
		strValue = _T("");
		break;
	}

	iIndex = m_SheathCarbonSeat.FindStringExact(-1, strValue);
	if (iIndex >= 0) m_SheathCarbonSeat.SetCurSel(iIndex);
	else			 m_SheathCarbonSeat.SetCurSel(0);

	iIndex = m_SheathCarbonSeat.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1) {
		m_SheathCarbonSeat.GetLBText(iIndex, strCode);

		std::vector<CString> arFiber;
		if (strCode == COMBO_SHEET_TYPE_CARBON_FIBER) {
			arFiber = { COMBO_SHEET_CF_High_Strength_Type1, COMBO_SHEET_CF_High_Strength_Type2,
				COMBO_SHEET_CF_Medium_Elasticity_Type, COMBO_SHEET_CF_High_Elasticity_Type };
		}
		else if(strCode == COMBO_SHEET_TYPE_AFRP){
			arFiber = { COMBO_SHEET_AFRT_aramid1, COMBO_SHEET_AFRT_aramid2_E78400, COMBO_SHEET_AFRT_aramid2_E78000 };
		}

		m_SheathCarbonType.ResetContent();
		for (int i = 0; i < arFiber.size(); i++)
			m_SheathCarbonType.AddString(arFiber[i]);
	}

	switch (pCF->nSubType)
	{
	case T_MATD_REIN_CF::kHighStrengthType1:
		strValue = COMBO_SHEET_CF_High_Strength_Type1;
		break;
	case T_MATD_REIN_CF::kHighStrengthType2:
		strValue = COMBO_SHEET_CF_High_Strength_Type2;
		break;
	case T_MATD_REIN_CF::kMediumElasticityType:
		strValue = COMBO_SHEET_CF_Medium_Elasticity_Type;
		break;
	case T_MATD_REIN_CF::kHighElasticityType:
		strValue = COMBO_SHEET_CF_High_Elasticity_Type;
		break;
	case T_MATD_REIN_CF::kAramid1:
		strValue = COMBO_SHEET_AFRT_aramid1;
		break;
	case T_MATD_REIN_CF::kAramid2_E78400:
		strValue = COMBO_SHEET_AFRT_aramid2_E78400;
		break;
	case T_MATD_REIN_CF::kAramid2_E78000:
		strValue = COMBO_SHEET_AFRT_aramid2_E78000;
		break;
	default:
		strValue = _T("");
		break;
	}

	iIndex = m_SheathCarbonType.FindStringExact(-1, strValue);
	if (iIndex >= 0) m_SheathCarbonType.SetCurSel(iIndex);
	else			 m_SheathCarbonType.SetCurSel(0);

	iIndex = m_SheathCarbonType.GetCurSel();
	CString strType = _T("");
	if (iIndex != -1) {
		m_SheathCarbonType.GetLBText(iIndex, strType);

		std::vector<CString> arVolume;
		if (strType == COMBO_SHEET_CF_High_Strength_Type1 || strType == COMBO_SHEET_CF_High_Strength_Type2) {
			arVolume = { COMBO_VOLUME_200, COMBO_VOLUME_300, COMBO_VOLUME_400, COMBO_VOLUME_450, COMBO_VOLUME_600 };
		}
		else if (strType == COMBO_SHEET_CF_Medium_Elasticity_Type) {
			arVolume = { COMBO_VOLUME_300_1, COMBO_VOLUME_300_2 };
		}
		else if (strType == COMBO_SHEET_CF_High_Elasticity_Type) {
			arVolume = { COMBO_VOLUME_300, COMBO_VOLUME_400 };
		}
		else if (strType == COMBO_SHEET_AFRT_aramid1) {
			arVolume = { COMBO_VOLUME_280, COMBO_VOLUME_415, COMBO_VOLUME_623 };
		}
		else if (strType == COMBO_SHEET_AFRT_aramid2_E78400) {
			arVolume = { COMBO_VOLUME_235, COMBO_VOLUME_350, COMBO_VOLUME_525, COMBO_VOLUME_700 };
		}
		else if (strType == COMBO_SHEET_AFRT_aramid2_E78000) {
			arVolume = { COMBO_VOLUME_235, COMBO_VOLUME_350, COMBO_VOLUME_525, COMBO_VOLUME_700 };
		}

		m_SheathCarbonVolume.ResetContent();
		for (int i = 0; i < arVolume.size(); i++) m_SheathCarbonVolume.AddString(arVolume[i]);
	}

	switch (pCF->nVolume)
	{
	case T_MATD_REIN_CF::k200:
		strValue = COMBO_VOLUME_200;
		break;
	case T_MATD_REIN_CF::k235:
		strValue = COMBO_VOLUME_235;
		break;
	case T_MATD_REIN_CF::k280:
		strValue = COMBO_VOLUME_280;
		break;
	case T_MATD_REIN_CF::k300:
		strValue = COMBO_VOLUME_300;
		break;
	case T_MATD_REIN_CF::k300_1:
		strValue = COMBO_VOLUME_300_1;
		break;
	case T_MATD_REIN_CF::k300_2:
		strValue = COMBO_VOLUME_300_2;
		break;
	case T_MATD_REIN_CF::k350:
		strValue = COMBO_VOLUME_350;
		break;
	case T_MATD_REIN_CF::k400:
		strValue = COMBO_VOLUME_400;
		break;
	case T_MATD_REIN_CF::k415:
		strValue = COMBO_VOLUME_415;
		break;
	case T_MATD_REIN_CF::k450:
		strValue = COMBO_VOLUME_450;
		break;
	case T_MATD_REIN_CF::k525:
		strValue = COMBO_VOLUME_525;
		break;
	case T_MATD_REIN_CF::k600:
		strValue = COMBO_VOLUME_600;
		break;
	case T_MATD_REIN_CF::k623:
		strValue = COMBO_VOLUME_623;
		break;
	case T_MATD_REIN_CF::k700:
		strValue = COMBO_VOLUME_700;
		break;
	case T_MATD_REIN_CF::k830:
		strValue = COMBO_VOLUME_830;
		break;
	default:
		strValue = _T("");
		break;
	}

	iIndex = m_SheathCarbonVolume.FindStringExact(-1, strValue);
	if (iIndex >= 0) m_SheathCarbonVolume.SetCurSel(iIndex);
	else			 m_SheathCarbonVolume.SetCurSel(0);

	m_CarbonHeight = pCF->dThickness;
	m_CarbonFiber = pCF->dElast;
	m_CarbonTens = pCF->dFu;
}

void CMPhiMaterialRcDlg_JP::SheathShowInit()
{
	GetDlgItem(IDC_SHEATH_RC_CON_MAIN_COB)->ShowWindow(FALSE);
	GetDlgItem(IDC_SHEATH_RC_CON_MAIN_EDT)->ShowWindow(TRUE);

	GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_COB)->ShowWindow(FALSE);
	GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_EDT)->ShowWindow(TRUE);

	GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_COB)->ShowWindow(FALSE);
	GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_EDT)->ShowWindow(TRUE);

	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(TRUE);

	GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_CBO)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_EDT)->ShowWindow(TRUE);
}

void CMPhiMaterialRcDlg_JP::Init_SheathComboBox()
{
	// Reset Contents.
	m_SheathRcCode.ResetContent();
	m_SheathRc_SCode.ResetContent(); 
	m_SheathSteelCode.ResetContent(); 
	m_SteelRebarCode.ResetContent();

	m_SheathCarbonSeat.ResetContent();
	m_SheathCarbonType.ResetContent();
	m_SheathCarbonVolume.ResetContent();

	CArray<CString, CString&> arRCCode;
	CString strMatlType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strMatlType, arRCCode);
	// Initialize MatlCode, RebarCode.
	m_SheathRcCode.AddString(_T("None"));
	m_SheathRc_SCode.AddString(_T("None"));
	m_SheathSteelCode.AddString(_T("None"));
	m_SteelRebarCode.AddString(_T("None"));
	for (int i = 0; i < arRCCode.GetSize(); i++)
	{
		if (arRCCode[i] == MATLCODE_CON_JIS || arRCCode[i] == MATLCODE_CON_JIS_CIVIL)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arRCCode[i]);
#endif
			m_SheathRcCode.AddString(arRCCode[i]);
			m_SheathRc_SCode.AddString(arRCCode[i]);
			m_SteelRebarCode.AddString(arRCCode[i]);
		}
	}

	CArray<CString, CString&> arSCodeList;
	CString strType = _T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, arSCodeList);
	for (int i = 0; i < arSCodeList.GetSize(); i++)
	{
		if (arSCodeList[i] == MATLCODE_STL_JIS || arSCodeList[i] == MATLCODE_STL_JIS_CIVIL)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arSCodeList[i]);
#endif
			m_SheathSteelCode.AddString((arSCodeList[i]));
		}
	}

	std::vector<CString> arSeatCode = {_T("None"), COMBO_SHEET_TYPE_CARBON_FIBER, COMBO_SHEET_TYPE_AFRP };
	for (int i = 0; i < arSeatCode.size(); i++)
		m_SheathCarbonSeat.AddString(arSeatCode[i]);
}

void CMPhiMaterialRcDlg_JP::EnableSheathEditBox(BOOL bCheck, int sheathType)
{
	switch (sheathType)
	{
	case 0:
		GetDlgItem(IDC_SHEATH_RC_CON_EC_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_CON_ACK_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_S_FY_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_S_FYS_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_STELL_SURRENDER_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_CARBON_MATE_HEIGHT_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_CARBON_MATE_FIBER_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_CARBON_MATE_TENS_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_CON_MAIN_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_FY_EDT)->EnableWindow(bCheck);
		break;
	case 1:
		GetDlgItem(IDC_SHEATH_RC_CON_EC_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_CON_ACK_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_CON_MAIN_EDT)->EnableWindow(bCheck);
		break;
	case 2:
		GetDlgItem(IDC_SHEATH_RC_S_FY_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_S_FYS_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_EDT)->EnableWindow(bCheck);
		break;
	case 3:
		GetDlgItem(IDC_CMD_SHEATH_STELL_SURRENDER_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->EnableWindow(bCheck);
		break;
	case 4:
		GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_FY_EDT)->EnableWindow(bCheck);
		break;
	case 5:
		GetDlgItem(IDC_CMD_SHEATH_CARBON_MATE_HEIGHT_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_CARBON_MATE_FIBER_EDT)->EnableWindow(bCheck);
		GetDlgItem(IDC_CMD_SHEATH_CARBON_MATE_TENS_EDT)->EnableWindow(bCheck);
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialRcDlg_JP message handlers

void CMPhiMaterialRcDlg_JP::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nCurPos = GetScrollPos(SB_VERT);
	switch (nSBCode)
	{
	case SB_LINEUP:    nCurPos -= 82; break;
	case SB_LINEDOWN:  nCurPos += 82; break;
	case SB_PAGEUP:    nCurPos -= 82; break;
	case SB_PAGEDOWN:  nCurPos += 82; break;
	case SB_THUMBTRACK: nCurPos =  nPos; break;
	}

	nCurPos = max(0, min(nCurPos, 410));
	SetScrollPos(SB_VERT, nCurPos);
	ScrollWindow(0, m_nScrollPos - nCurPos);
	m_nScrollPos = nCurPos;
}

BOOL CMPhiMaterialRcDlg_JP::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nWheel = GetScrollPos(SB_VERT);
	if (zDelta > 0)
		nWheel -= 82;
	else if (zDelta < 0)
		nWheel += 82;

	nWheel = max(0, min(nWheel, 410));
	SetScrollPos(SB_VERT, nWheel);
	ScrollWindow(0, m_nScrollPos - nWheel);
	m_nScrollPos = nWheel;

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CMPhiMaterialRcDlg_JP::OnClickDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	OnItemchangedDgnConcmatList(pNMHDR, pResult);
	*pResult = 0;
}

void CMPhiMaterialRcDlg_JP::OnItemchangedDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem, 0);

	T_MATL_K kMatl = _ttoi(strID);
	T_MATD_D mData;
	mData.Initialize();
	if (!m_pMPhiDataAll->GetMatd(kMatl, mData)) return;
	m_kMatl = kMatl;

	// Coded by Seungjun MNet:No.2393 ('20061110)
	m_strCurrentItem = strID;
	GetDlgItem(IDC_CMD_CONC_EL)->EnableWindow(TRUE);

	// If Code is None, Material Name is mData.Name.
	m_MatName2 = mData.Name;
	m_Fc = mData.Data1.Design.C_fc;
	m_bLambda = mData.Data1.Design.bLambda;
	// MQC_ab_5526-2 Civil의 경우 light weight concrete 고려 하지 않음.
#ifdef _CIVIL
	m_bLambda = FALSE;
#endif // _CIVIL

	m_bChkTransfer = mData.Data1.Design.bTransfer;
	m_edtTransfer.SetEditUnit(mData.Data1.Design.dTransferFci);
	if (!m_bTransfer) m_bChkTransfer = FALSE;

	m_edtLambda.SetEditUnit(mData.Data1.Design.dLambda);
	m_MainBarName2 = mData.MainRebar_RebarName;
	m_Fy = mData.MainRebarData.B_fy;
	m_SubBarName2 = mData.SubRebar_RebarName;
	m_Fys = mData.SubRebarData.B_fy;

	CString strMatlCode = _T("");
	if (mData.Data1.CodeName == _T("None") || mData.Data1.CodeName == _T(""))	// None.
	{
		strMatlCode = _T("None");
		EnableEditBox(TRUE);
	}
	else	// DB.
	{
		strMatlCode = mData.Data1.CodeName;
		EnableEditBox(FALSE);
	}

	CString strRebarCode = _T("");
	if (mData.Rebar_CodeName == _T("None") || mData.Rebar_CodeName == _T(""))	// None.
	{
		strRebarCode = _T("None");
		EnableRebarEditBox(TRUE);
	}
	else	// DB.
	{
		strRebarCode = mData.Rebar_CodeName;
		EnableRebarEditBox(FALSE);
	}

	CString CodeName;
	CodeName = strRebarCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	int iIndex = m_RebarCode.FindStringExact(-1, CodeName);
	m_RebarCode.SetCurSel(iIndex);

	CodeName = strMatlCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	iIndex = m_Code.FindStringExact(-1, CodeName);
	m_Code.SetCurSel(iIndex);
	// Initialize Material Name ComboBox by Material Code.
	Init_MatlNameCombo(strMatlCode, mData.Data1.CodeMatlName);
	// Initialize RebarCode and Name ComboBox.
	Init_RebarCombo(strRebarCode, mData);

	T_MATD_REIN_D reinD;
	if (m_pMPhiDataAll->GetMatdRein(kMatl, reinD) == FALSE) {
		m_bSheath = FALSE;
		Init_SheathCase();
	}
	else{
		if (reinD.kMatl == 0) {
			m_bSheath = FALSE;
			Init_SheathCase();
		}
		else {
			m_bSheath = TRUE;
			Set_SheathCase(reinD);
		}
	}

	ChangeDlgItemInfo();
	FoldResetVisible();
	/////////////////////////////////////////////////////////////////
	// Sheath 
	/////////////////////////////////////////////////////////////////
	//
	// 
	//
	UpdateData(FALSE);

	*pResult = 0;
}

void CMPhiMaterialRcDlg_JP::Init_MatlNameCombo(CString strCode, CString strNa)
{
	if (strCode == _T("None") || strCode == _T(""))	return;	// Skip if None.

	m_MatName.ResetContent();
	CArray<CString, CString&> arName;
	m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arName);
	for (int i = 0; i < arName.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arName[i]);
#endif
		m_MatName.AddString(arName[i]);
	}
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, strNa);
#endif
	int Index = m_MatName.FindStringExact(-1, strNa);
	if (Index != -1)	m_MatName.SetCurSel(Index);
}

void CMPhiMaterialRcDlg_JP::Init_RebarCombo(CString strRebarCode, T_MATD_D rData)
{
	m_MainBarName.ResetContent();
	m_SubBarName.ResetContent();

	T_MATL_REBAR RebarData;
	if (strRebarCode != _T("None") && strRebarCode != _T(""))
	{
		// Initialize Rebar Name ComboBox by Code.
		CArray<CString, CString&> arRebar;
		m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, arRebar);
		for (int i = 0; i < arRebar.GetSize(); i++)
		{
			m_MainBarName.AddString(arRebar[i]);
			m_SubBarName.AddString(arRebar[i]);
		}
		int Index = m_MainBarName.FindStringExact(-1, rData.MainRebar_RebarName);
		if (Index != -1)
		{
			m_MainBarName.SetCurSel(Index);
			if (rData.MainRebarData.B_fy == 0.0)
			{
				RebarData.Initialize();
				m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, rData.MainRebar_RebarName, RebarData);
				m_Fy = RebarData.B_fy;
			}
			else	m_Fy = rData.MainRebarData.B_fy;
		}
		else	m_Fy = 0.0;

		Index = m_SubBarName.FindStringExact(-1, rData.SubRebar_RebarName);
		if (Index != -1)
		{
			m_SubBarName.SetCurSel(Index);
			if (rData.SubRebarData.B_fy == 0.0)
			{
				RebarData.Initialize();
				m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, rData.SubRebar_RebarName, RebarData);
				m_Fys = RebarData.B_fy;
			}
			else	m_Fys = rData.SubRebarData.B_fy;
		}
		else	m_Fys = 0.0;
	}
	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::ChangeDlgItemInfo()
{
	CString ConcCode = _T("");
	CString RBarCode = _T("");
	int ConcIdx = m_Code.GetCurSel();
	if (ConcIdx >= 0)
	{
		m_Code.GetLBText(ConcIdx, ConcCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(ConcCode);
#endif
	}

//	BOOL bChinaEN04 = FALSE;
//#ifdef _MGEN_CH
//	bChinaEN04 = TRUE;
//#endif

// 	if (ConcCode == MATLCODE_CON_GB_CIVIL)
// 		GetDlgItem(IDC_DGN_STATIC_FC)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_JTJ));
// 	else if (!bChinaEN04 && ConcCode == MATLCODE_CON_EN04)
// 		GetDlgItem(IDC_DGN_STATIC_FC)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_EN04));
// 	else
// 		GetDlgItem(IDC_DGN_STATIC_FC)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_ETC));
// 
// 	int RBarIdx = m_RebarCode.GetCurSel();
// 	if (RBarIdx >= 0)
// 		m_RebarCode.GetLBText(RBarIdx, RBarCode);
// 	if (RBarCode == MATLCODE_CON_GB_CIVIL)
// 	{
// 		GetDlgItem(IDC_DGN_STATIC_FY)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_JTJ));
// 		GetDlgItem(IDC_DGN_STATIC_FYS)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_JTJ));
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_DGN_STATIC_FY)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_ETC));
// 		GetDlgItem(IDC_DGN_STATIC_FYS)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_ETC));
// 	}

// 	GetDlgItem(IDC_CMD_STATIC_FC)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_ETC));
// 	GetDlgItem(IDC_CMD_STATIC_FY)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_ETC));
// 	GetDlgItem(IDC_CMD_STATIC_FYS)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_ETC));
	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnSelchangeDgnConcMat()
{
	int Index = m_Code.GetCurSel();
	CString strCode = _T("");
	if (Index != -1) m_Code.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	Index = m_MatName.GetCurSel();
	CString strName = _T("");
	if (Index != -1) m_MatName.GetLBText(Index, strName);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	T_MATL_CONCRETE cData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode, strName, cData);
	if (bCheck)
	{
		m_Fc = cData.C_fc;
		UpdateData(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::OnSelchangeDgnConcmatCode()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int iIndex = m_Code.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_Code.GetLBText(iIndex, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	if (strCode == _T("None") || strCode == _T(""))	EnableEditBox(TRUE);
	else
	{
		EnableEditBox(FALSE);
		// Change Material Name by Code.
		m_MatName.ResetContent();
		CArray<CString, CString&> arMatlName;
		m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arMatlName);
		for (int i = 0; i < arMatlName.GetSize(); i++)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arMatlName[i]);
#endif
			m_MatName.AddString(arMatlName[i]);
		}
	}
	if (strCode != _T("None"))	m_Fc = 0.0;
	m_MatName2 = _T("");

	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnSelchangeDgnConcMainname()
{
	CString strRebarName = _T(""), strRebarCode = _T("");
	int Index = m_MainBarName.GetCurSel();
	if (Index != -1)
	{
		UpdateData(TRUE);
		m_MainBarName.GetLBText(Index, strRebarName);

		Index = m_RebarCode.GetCurSel();
		if (Index != -1)
		{
			m_RebarCode.GetLBText(Index, strRebarCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif
		}
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE), MB_OK);

		T_MATL_REBAR rData;
		rData.Initialize();
		if (m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strRebarName, rData))
		{
			m_Fy = rData.B_fy;
		}
		UpdateData(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::OnSelchangeDgnConcSubname()
{
	CString strRebarName = _T(""), strRebarCode = _T("");
	int Index = m_SubBarName.GetCurSel();
	if (Index != -1)
	{
		UpdateData(TRUE);
		m_SubBarName.GetLBText(Index, strRebarName);

		Index = m_RebarCode.GetCurSel();
		if (Index != -1)
		{
			m_RebarCode.GetLBText(Index, strRebarCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif
		}
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE), MB_OK);

		T_MATL_REBAR rData;
		rData.Initialize();
		if (m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strRebarName, rData))
		{
			m_Fys = rData.B_fy;
		}
		UpdateData(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::ModifyMaterial(int& Index,  CString& strCode, CString& strMatName, CString& strFc, CString& strChk, CString& strLambda, T_MATD_D& mData)
{
	if (Index != 0)	// if Code exists.
	{
		Index = m_MatName.GetCurSel();
		if (Index != -1) m_MatName.GetLBText(Index, strMatName);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strMatName);
#endif
		mData.Data1.CodeMatlName = strMatName;	// Get Material Code Name.

		//strName	= strMatName; //MQC4581-SALEE
		//mData.Name = strName;
		// Get Analysis Data & Design Data.
		T_MATL_CONCRETE cData;
		BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode, strMatName, cData);
		if (bCheck)
		{
			mData.Data1.Design.C_fc = m_Fc;
			mData.Data1.Design.C_fci = 0.7 * m_Fc;	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
			strFc.Format(_T("%g"), mData.Data1.Design.C_fc);

			if (m_bTransfer)
			{
				mData.Data1.Design.bTransfer = m_bChkTransfer;
				mData.Data1.Design.dTransferFci = (m_bChkTransfer) ? m_edtTransfer.GetEditValue() : 0.0;
			}
			else
			{
				mData.Data1.Design.bTransfer = FALSE;
				mData.Data1.Design.dTransferFci = 0.0;
			}

			//mData.Data1.Design.bLambda = m_bLambda;
			//mData.Data1.Design.dLambda = m_edtLambda.GetEditValue();

			if (m_bLambda) strChk = _T("O");
			else          strChk = _T("X");

			strLambda.Format(_T("%g"), mData.Data1.Design.dLambda);
		}
	}
	else	// if Code not exists.
	{
		mData.Data1.CodeMatlName = strCode;

		//strName = m_MatName2; //MQC4581-SALEE
		//mData.Name = strName;

		mData.Data1.Design.C_fc = m_Fc;
		mData.Data1.Design.C_fci = 0.7 * m_Fc;	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
		strFc.Format(_T("%g"), mData.Data1.Design.C_fc);

		if (m_bTransfer)
		{
			mData.Data1.Design.bTransfer = m_bChkTransfer;
			mData.Data1.Design.dTransferFci = (m_bChkTransfer) ? m_edtTransfer.GetEditValue() : 0.0;
		}
		else
		{
			mData.Data1.Design.bTransfer = FALSE;
			mData.Data1.Design.dTransferFci = 0.0;
		}

		//mData.Data1.Design.bLambda = m_bLambda;
		//mData.Data1.Design.dLambda = m_edtLambda.GetEditValue();

		if (m_bLambda) strChk = _T("O");
		else          strChk = _T("X");

		strLambda.Format(_T("%g"), mData.Data1.Design.dLambda);
	}
}

void CMPhiMaterialRcDlg_JP::ModifyRebar(int& Index, int& reIndex, BOOL& bCheck, CString& strRebarCode, CString& strMain, CString& strSub, T_MATD_D& mData)
{
	if (reIndex != 0)	// if Code exists.
	{
		// Rebar Data를 얻는다.
		T_MATL_REBAR MainData, SubData;
		Index = m_MainBarName.GetCurSel();
		if (Index != -1) m_MainBarName.GetLBText(Index, strMain);
		mData.MainRebar_RebarName = strMain;
		MainData.Initialize();
		bCheck = m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strMain, MainData);
		mData.MainRebarData.B_Density = MainData.B_Density;
		mData.MainRebarData.B_Elast = MainData.B_Elast;
		mData.MainRebarData.B_fy = MainData.B_fy;

		Index = m_SubBarName.GetCurSel();
		if (Index != -1) m_SubBarName.GetLBText(Index, strSub);
		mData.SubRebar_RebarName = strSub;
		SubData.Initialize();
		bCheck = m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strSub, SubData);
		mData.SubRebarData.B_Density = SubData.B_Density;
		mData.SubRebarData.B_Elast = SubData.B_Elast;
		mData.SubRebarData.B_fy = SubData.B_fy;
	}
	else	// if Code not exists.
	{
		mData.Rebar_CodeName = strRebarCode;  //  Rebar Code를 얻는다.

		strMain = m_MainBarName2;
		strSub = m_SubBarName2;

		mData.MainRebar_RebarName = m_MainBarName2;
		mData.MainRebarData.B_Density = 0.0;
		mData.MainRebarData.B_Elast = 0.0;
		mData.MainRebarData.B_fy = m_Fy;

		mData.SubRebar_RebarName = m_SubBarName2;
		mData.SubRebarData.B_Density = 0.0;
		mData.SubRebarData.B_Elast = 0.0;
		mData.SubRebarData.B_fy = m_Fys;
	}
}

void CMPhiMaterialRcDlg_JP::ModifyMaterialRein(const T_MATL_K& key, OUT T_MATD_REIN_D& rRein)
{
	rRein.Initialize();
	
	if (m_bSheath == FALSE) { //보강재질 삭제
		m_pMPhiDataAll->DeleteMatdRein(key);
		return;
	}
	// 보강재질 추가/수정
	m_pMPhiDataAll->GetMatdRein(key, rRein);

	rRein.kMatl = key;
	//철근콘크리트(Rebar Concrete) 피복
	ModifyMaterialReinRC(&rRein.CoverRC);
	//강판(Steel Plate) 피복
	ModifyMaterialReinSP(&rRein.CoverSP);
	//탄소섬유(Carbon fiber) 피복
	ModifyMaterialReinCF(&rRein.CoverCF);
}

void CMPhiMaterialRcDlg_JP::ModifyMaterialReinRC(OUT T_MATD_REIN_RC* pRC)
{
	int nIndex = 0;
	CString strVal;

	nIndex = m_SheathRcCode.GetCurSel();
	if (nIndex != -1) m_SheathRcCode.GetLBText(nIndex, pRC->strC_CodeName);

	if(pRC->strC_CodeName == _T("None") || pRC->strC_CodeName == _T("")) {	// None.
		m_SheathRcName.GetWindowText(pRC->strC_CodeMatlName);
	}
	else {
		nIndex = m_SheathRcName.GetCurSel();
		if (nIndex != -1) m_SheathRcName.GetLBText(nIndex, pRC->strC_CodeMatlName);
	}
	pRC->dC_Fc = m_Ack;
	pRC->dC_Elast = m_Ec;

	nIndex = m_SheathRc_SCode.GetCurSel();
	if (nIndex != -1) m_SheathRc_SCode.GetLBText(nIndex, pRC->strR_CodeName);

	if(pRC->strR_CodeName == _T("None") || pRC->strR_CodeName == _T("")) {	// None.
		pRC->strMR_RebarName = m_SheathRc_SNameMain2;
		pRC->strSR_RebarName = m_SheathRc_SNameSub2;
	}
	else {
		nIndex = m_SheathRc_SNameMain.GetCurSel();
		if (nIndex != -1) m_SheathRc_SNameMain.GetLBText(nIndex, pRC->strMR_RebarName);
		nIndex = m_SheathRc_SNameSub.GetCurSel();
		if (nIndex != -1) m_SheathRc_SNameSub.GetLBText(nIndex, pRC->strSR_RebarName);
	}
	
	pRC->dMR_Fy = m_SheathFy;
	pRC->dSR_Fy = m_SheathFys;
}

void CMPhiMaterialRcDlg_JP::ModifyMaterialReinSP(OUT T_MATD_REIN_SP* pSP)
{
	int nIndex = 0;
	CString strVal;

	nIndex = m_SheathSteelCode.GetCurSel();
	if (nIndex != -1) m_SheathSteelCode.GetLBText(nIndex, pSP->strCodeName);
	if(pSP->strCodeName == _T("None") || pSP->strCodeName == _T("")) {	// None.
		pSP->strCodeMatlName = m_SheathSteelName2;
	}
	else {
		nIndex = m_SheathSteelName.GetCurSel();
		if (nIndex != -1) m_SheathSteelName.GetLBText(nIndex, pSP->strCodeMatlName);
	}

	pSP->dFy = m_SteelSurrend;

	// Rebar
	nIndex = m_SteelRebarCode.GetCurSel();
	if (nIndex != -1) m_SteelRebarCode.GetLBText(nIndex, pSP->strR_CodeName);
	if (pSP->strR_CodeName == _T("None") || pSP->strR_CodeName == _T("")) {	// None.
		pSP->strAnchor_RebarName = m_SteelRebarName2;
	}
	else {
		nIndex = m_SteelRebarName.GetCurSel();
		if (nIndex != -1) m_SteelRebarName.GetLBText(nIndex, pSP->strAnchor_RebarName);
	}
	pSP->dAnchor_Fy = m_SteelRebarFy;
}

void CMPhiMaterialRcDlg_JP::ModifyMaterialReinCF(OUT T_MATD_REIN_CF* pCF)
{
	int nIndex = 0;
	CString strVal;

	m_SheathCarbonSeat.GetWindowText(strVal);
	if (strVal == _T("None")) pCF->nMainType = T_MATD_REIN_CF::kUserInput;
	else if (strVal == COMBO_SHEET_TYPE_CARBON_FIBER) pCF->nMainType = T_MATD_REIN_CF::kCarbonFiber;
	else if (strVal == COMBO_SHEET_TYPE_AFRP) pCF->nMainType = T_MATD_REIN_CF::kAFRP;
	else pCF->nMainType = T_MATD_REIN_CF::kUnKnownMain;

	m_SheathCarbonType.GetWindowText(strVal);
	if (strVal == COMBO_SHEET_CF_High_Strength_Type1) pCF->nSubType = T_MATD_REIN_CF::kHighStrengthType1;
	else if (strVal == COMBO_SHEET_CF_High_Strength_Type2) pCF->nSubType = T_MATD_REIN_CF::kHighStrengthType2;
	else if (strVal == COMBO_SHEET_CF_Medium_Elasticity_Type) pCF->nSubType = T_MATD_REIN_CF::kMediumElasticityType;
	else if (strVal == COMBO_SHEET_CF_High_Elasticity_Type) pCF->nSubType = T_MATD_REIN_CF::kHighElasticityType;
	else if (strVal == COMBO_SHEET_AFRT_aramid1) pCF->nSubType = T_MATD_REIN_CF::kAramid1;
	else if (strVal == COMBO_SHEET_AFRT_aramid2_E78400) pCF->nSubType = T_MATD_REIN_CF::kAramid2_E78400;
	else if (strVal == COMBO_SHEET_AFRT_aramid2_E78000) pCF->nSubType = T_MATD_REIN_CF::kAramid2_E78000;
	else pCF->nSubType = T_MATD_REIN_CF::kUnKnownSub;
	
	m_SheathCarbonVolume.GetWindowText(strVal);
	if (strVal == COMBO_VOLUME_200) pCF->nVolume = T_MATD_REIN_CF::k200;
	else if (strVal == COMBO_VOLUME_235) pCF->nVolume = T_MATD_REIN_CF::k235;
	else if (strVal == COMBO_VOLUME_280) pCF->nVolume = T_MATD_REIN_CF::k280;
	else if (strVal == COMBO_VOLUME_300) pCF->nVolume = T_MATD_REIN_CF::k300;
	else if (strVal == COMBO_VOLUME_300_1) pCF->nVolume = T_MATD_REIN_CF::k300_1;
	else if (strVal == COMBO_VOLUME_300_2) pCF->nVolume = T_MATD_REIN_CF::k300_2;
	else if (strVal == COMBO_VOLUME_350) pCF->nVolume = T_MATD_REIN_CF::k350;
	else if (strVal == COMBO_VOLUME_400) pCF->nVolume = T_MATD_REIN_CF::k400;
	else if (strVal == COMBO_VOLUME_415) pCF->nVolume = T_MATD_REIN_CF::k415;
	else if (strVal == COMBO_VOLUME_450) pCF->nVolume = T_MATD_REIN_CF::k450;
	else if (strVal == COMBO_VOLUME_525) pCF->nVolume = T_MATD_REIN_CF::k525;
	else if (strVal == COMBO_VOLUME_600) pCF->nVolume = T_MATD_REIN_CF::k600;
	else if (strVal == COMBO_VOLUME_623) pCF->nVolume = T_MATD_REIN_CF::k623;
	else if (strVal == COMBO_VOLUME_700) pCF->nVolume = T_MATD_REIN_CF::k700;
	else if (strVal == COMBO_VOLUME_830) pCF->nVolume = T_MATD_REIN_CF::k830;
	else pCF->nVolume = T_MATD_REIN_CF::kUnKnown;

	pCF->dThickness = m_CarbonHeight;
	pCF->dElast = m_CarbonFiber;
	pCF->dFu = m_CarbonTens;
}

BOOL CMPhiMaterialRcDlg_JP::ErrorCheck()
{
	BOOL bCheck = TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	if (m_Code.GetCurSel() == 0)
	{
		if (m_MatName2 == _T("")) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_CON_MATL_NAME_ERROR); }
	}
	if (m_RebarCode.GetCurSel() == 0)
	{
		if (m_MainBarName2 == _T("") || m_SubBarName2 == _T(""))
		{
			bCheck = FALSE;
			strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);
		}
	}

	//double dLambda = m_edtLambda.GetEditValue();
	//if (m_bLambda)
	//{
	//	if (dLambda < 0.75 || dLambda > 1.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_LAMBDA_ERROR); }
	//}

	if (m_Fc <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FC_ERROR); }
	if (m_Fy <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR); }
	if (m_Fys <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FYS_ERROR); }

	//double dTransferFci = m_edtTransfer.GetEditValue();
	//if (dTransferFci < 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FCI_ERROR); }

	if (!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

void CMPhiMaterialRcDlg_JP::OnSelchangeDgnConcRebarcode()
{
	CString strCode = _T("");
	int Index = m_RebarCode.GetCurSel();
	if (Index != -1)
	{
		m_RebarCode.GetLBText(Index, strCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	}

	if (strCode == _T("None") || strCode == _T(""))	EnableRebarEditBox(TRUE);
	else
	{
		EnableRebarEditBox(FALSE);
		m_MainBarName.ResetContent();
		m_SubBarName.ResetContent();

		CArray<CString, CString&> arRebar;
		m_pDoc->m_pMatlDB->GetRebarNameList(strCode, arRebar);
		for (int i = 0; i < arRebar.GetSize(); i++)
		{
			m_MainBarName.AddString(arRebar[i]);
			m_SubBarName.AddString(arRebar[i]);
		}
	}

	if (strCode != _T("None"))
	{
		m_Fy = 0.0;
		m_Fys = 0.0;
	}
	m_MainBarName2 = _T("");
	m_SubBarName2 = _T("");

	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnClickbSheath()
{
	m_bSheath = m_bSheath == FALSE ? TRUE : FALSE;

	Init_SheathCase();

	UpdateData(FALSE);
}

LRESULT CMPhiMaterialRcDlg_JP::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	AlignBtnMove();
	if (m_FoldGrp.GetFoldState())
	{
		m_FoldGrp.SetWindowTextW(_LS(IDS_CMD_MPHI_MATL_SHEATH_UNFOLD));
	}
	else
	{
		m_FoldGrp.SetWindowTextW(_LS(IDS_CMD_MPHI_MATL_SHEATH_FOLD));
		Invalidate();
		UpdateWindow();
		m_pParent->Invalidate();
		m_pParent->UpdateWindow();
	}

	return 0L;
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathRcCode()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int iIndex = m_SheathRcCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SheathRcCode.GetLBText(iIndex, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	m_SheathRcName.ResetContent();
	if (strCode == _T("None") || strCode == _T(""))
	{
		EnableSheathEditBox(TRUE, eSheathType::kRcCode);
		GetDlgItem(IDC_SHEATH_RC_CON_MAIN_COB)->ShowWindow(FALSE);
		GetDlgItem(IDC_SHEATH_RC_CON_MAIN_EDT)->ShowWindow(TRUE);
		GetDlgItem(IDC_SHEATH_RC_CON_MAIN_NAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_NAME));
	}
	else
	{
		EnableSheathEditBox(FALSE, eSheathType::kRcCode);
		GetDlgItem(IDC_SHEATH_RC_CON_MAIN_COB)->ShowWindow(TRUE);
		GetDlgItem(IDC_SHEATH_RC_CON_MAIN_EDT)->ShowWindow(FALSE);
		GetDlgItem(IDC_SHEATH_RC_CON_MAIN_NAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_RCPC_DLG_IDC_STATIC_MATNAME));

		// Change Material Name by Code.
		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arName);
		for (int i = 0; i < arName.GetSize(); i++)	m_SheathRcName.AddString(arName[i]);
	}
	m_Ec = 0.0;
	m_Ack = 0.0;
	m_SheathRcName2 = _T("");

	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathRcName()
{
	CString strRcName = _T(""), strRcCode = _T("");
	int Index = m_SheathRcName.GetCurSel();
	if (Index != -1)
	{
		UpdateData(TRUE);
		m_SheathRcName.GetLBText(Index, strRcName);

		Index = m_SheathRcCode.GetCurSel();
		if (Index != -1) m_SheathRcCode.GetLBText(Index, strRcCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRcCode);
#endif
		T_MATL_CONCRETE cData;
		cData.Initialize();
		if (m_pDoc->m_pMatlDB->GetConcreteData(strRcCode, strRcName, cData))
		{
			m_Ec = cData.Elast;
			m_Ack = cData.C_fc;
		}
		UpdateData(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathRc_SCode()
{
	int iIndex = m_SheathRc_SCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SheathRc_SCode.GetLBText(iIndex, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	m_SheathRc_SNameMain.ResetContent();
	m_SheathRc_SNameSub.ResetContent();
	if (strCode == _T("None") || strCode == _T(""))
	{
		EnableSheathEditBox(TRUE, eSheathType::kRc_SCode);
		GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_COB)->ShowWindow(FALSE);
		GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_EDT)->ShowWindow(TRUE);
		GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_COB)->ShowWindow(FALSE);
		GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_EDT)->ShowWindow(TRUE);
		GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_STC)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_STEEL_MAIN_NAME));
		GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_STC)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_STEEL_SUB_NAME));
	}
	else
	{
		EnableSheathEditBox(FALSE, eSheathType::kRc_SCode);
		GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_COB)->ShowWindow(TRUE);
		GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_EDT)->ShowWindow(FALSE);
		GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_COB)->ShowWindow(TRUE);
		GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_EDT)->ShowWindow(FALSE);
		GetDlgItem(IDC_SHEATH_RC_S_MAIN_REBAR_STC)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_RCPC_DLG_IDC_CMD_STATIC_MATEFY));
		GetDlgItem(IDC_SHEATH_RC_S_SUB_REBAR_STC)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_RCPC_DLG_IDC_CMD_STATIC_MATAFV));

		// Change Material Name by Code.
		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetRebarNameList(strCode, arName);
		for (int i = 0; i < arName.GetSize(); i++)
		{
			m_SheathRc_SNameMain.AddString(arName[i]);
			m_SheathRc_SNameSub.AddString(arName[i]);
		}
	}
	m_SheathFy = 0.0;
	m_SheathFys = 0.0;
	m_SheathRc_SNameMain2 = _T("");
	m_SheathRc_SNameSub2 = _T("");

	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathRc_SMainName()
{
	CString strRebarName = _T(""), strRebarCode = _T("");
	int Index = m_SheathRc_SNameMain.GetCurSel();
	if (Index != -1)
	{
		UpdateData(TRUE);
		m_SheathRc_SNameMain.GetLBText(Index, strRebarName);

		Index = m_SheathRc_SCode.GetCurSel();
		if (Index != -1) m_SheathRc_SCode.GetLBText(Index, strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE), MB_OK);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

		T_MATL_REBAR rData;
		rData.Initialize();
		if (m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strRebarName, rData))
		{
			m_SheathFy = rData.B_fy;
		}
		UpdateData(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathRc_SSubName()
{
	CString strRebarName = _T(""), strRebarCode = _T("");
	int Index = m_SheathRc_SNameSub.GetCurSel();
	if (Index != -1)
	{
		UpdateData(TRUE);
		m_SheathRc_SNameSub.GetLBText(Index, strRebarName);

		Index = m_SheathRc_SCode.GetCurSel();
		if (Index != -1) m_SheathRc_SCode.GetLBText(Index, strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE), MB_OK);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

		T_MATL_REBAR rData;
		rData.Initialize();
		if (m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strRebarName, rData))
		{
			m_SheathFys = rData.B_fy;
		}
		UpdateData(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathSteelCode()
{
	int iIndex = m_SheathSteelCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SheathSteelCode.GetLBText(iIndex, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	m_SheathSteelName.ResetContent();
	if (strCode == _T("None") || strCode == _T(""))
	{
		EnableSheathEditBox(TRUE, eSheathType::kSteelCode);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_STC)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_NAME));
	}
	else
	{
		EnableSheathEditBox(FALSE, eSheathType::kSteelCode);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_STC)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_RCPC_DLG_IDC_CMD_SHEATH_STELL_NAME_STC));

		// Change Material Name by Code.
		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetSteelNameList(strCode, arName);
		for (int i = 0; i < arName.GetSize(); i++)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arName[i]);
#endif
			m_SheathSteelName.AddString(arName[i]);
		}
	}
	m_SteelSurrend = 0.0;
	m_SheathSteelName2 = _T("");

	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathSteelName()
{
	int iIndex = m_SheathSteelCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SheathSteelCode.GetLBText(iIndex, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	int nameIdx= m_SheathSteelName.GetCurSel();
	CString strName = _T("");
	if (nameIdx != -1)	m_SheathSteelName.GetLBText(nameIdx, strName);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode, strName, sData);
	if (bCheck)	m_SteelSurrend = sData.S_Fy1;
	else
	{
		int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if (nItem != -1)
		{
			CString strID = m_ListCtrl.GetItemText(nItem, 0);
			int nID = _ttoi(strID);

			T_MATD_D mData;
			mData.Initialize();
			BOOL bCheck = m_pMPhiDataAll->GetMatd(nID, mData);
			if (bCheck)	m_SteelSurrend = mData.Data1.Design.S_Fy1;
			else				m_SteelSurrend = 0.0;
		}
	}
	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathSteelRebarCode()
{
	int iIndex = m_SteelRebarCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SteelRebarCode.GetLBText(iIndex, strCode);

	m_SteelRebarName.ResetContent();
	if (strCode == _T("None") || strCode == _T(""))
	{
		EnableSheathEditBox(TRUE, eSheathType::kSteelRebarCode);
		GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_CBO)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_EDT)->ShowWindow(TRUE);
	}
	else
	{
		EnableSheathEditBox(FALSE, eSheathType::kSteelRebarCode);
		GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_CBO)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_REBAR_MAIN_EDT)->ShowWindow(FALSE);

		// Change Material Name by Code.
		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetRebarNameList(strCode, arName);
		for (int i = 0; i < arName.GetSize(); i++)
		{
			m_SteelRebarName.AddString(arName[i]);
		}
	}
	m_SteelRebarFy = 0.0;
	m_SteelRebarName2 = _T("");
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathSteelRebarName()
{
	CString strRebarName = _T(""), strRebarCode = _T("");
	int Index = m_SteelRebarName.GetCurSel();
	if (Index != -1)
	{
		UpdateData(TRUE);
		m_SteelRebarName.GetLBText(Index, strRebarName);

		Index = m_SteelRebarCode.GetCurSel();
		if (Index != -1) m_SteelRebarCode.GetLBText(Index, strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE), MB_OK);

		T_MATL_REBAR rData;
		rData.Initialize();
		if (m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strRebarName, rData))
		{
			m_SteelRebarFy = rData.B_fy;
		}
		UpdateData(FALSE);
	}
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathSeatCode()
{
	int iIndex = m_SheathCarbonSeat.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SheathCarbonSeat.GetLBText(iIndex, strCode);

	m_SheathCarbonType.ResetContent();
	m_SheathCarbonVolume.ResetContent();
	if (strCode == _T("None") || strCode == _T(""))
	{
		EnableSheathEditBox(TRUE, eSheathType::kSeat);
		m_SheathCarbonType.EnableWindow(FALSE);
		m_SheathCarbonVolume.EnableWindow(FALSE);
	}
	else
	{
		EnableSheathEditBox(FALSE, eSheathType::kSeat);
		m_SheathCarbonType.EnableWindow(TRUE);
		m_SheathCarbonVolume.EnableWindow(TRUE);

		// Change Material Name by Code.

		// m_SheathCarbonType
		std::vector<CString> arFiber;
		switch (iIndex)
		{
		case 1:
			arFiber = { COMBO_SHEET_CF_High_Strength_Type1, COMBO_SHEET_CF_High_Strength_Type2,
				COMBO_SHEET_CF_Medium_Elasticity_Type, COMBO_SHEET_CF_High_Elasticity_Type };
			break;
		case 2:
			arFiber = { COMBO_SHEET_AFRT_aramid1, COMBO_SHEET_AFRT_aramid2_E78400, COMBO_SHEET_AFRT_aramid2_E78000 };
			break;
		default:
			break;
		}
		for (int i = 0; i < arFiber.size(); i++)
			m_SheathCarbonType.AddString(arFiber[i]);
	}
	m_CarbonHeight = 0.0;
	m_CarbonFiber = 0.0;
	m_CarbonTens = 0.0;

	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathType()
{
	int iIndex = m_SheathCarbonSeat.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SheathCarbonSeat.GetLBText(iIndex, strCode);

	int typeIdx = m_SheathCarbonType.GetCurSel();
	CString strType = _T("");
	if (typeIdx != -1)	m_SheathCarbonType.GetLBText(typeIdx, strType);

	std::vector<CString> arVolume;
	if (iIndex == 1) // Carbon
	{
		switch (typeIdx)
		{
		case 0:
		case 1:
			arVolume = { COMBO_VOLUME_200, COMBO_VOLUME_300, COMBO_VOLUME_400, COMBO_VOLUME_450, COMBO_VOLUME_600 };
			break;
		case 2:
			arVolume = { COMBO_VOLUME_300_1, COMBO_VOLUME_300_2 };
			break;
		case 3:
			arVolume = { COMBO_VOLUME_300, COMBO_VOLUME_400 };
			break;
		default:
			break;
		}
	}
	else // AFRT
	{
		switch (typeIdx)
		{
		case 0:
			arVolume = { COMBO_VOLUME_280, COMBO_VOLUME_415, COMBO_VOLUME_623, COMBO_VOLUME_830 };
			break;
		case 1:
		case 2:
			arVolume = { COMBO_VOLUME_235, COMBO_VOLUME_350, COMBO_VOLUME_525, COMBO_VOLUME_700 };
			break;
		default:
			break;
		}
	}

	m_SheathCarbonVolume.ResetContent();
	for (int i = 0; i < arVolume.size(); i++)
	{
		m_SheathCarbonVolume.AddString(arVolume[i]);
	}

	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnChangeSheathVolume()
{
	int typeIdx = m_SheathCarbonType.GetCurSel();
	CString strType = _T("");
	if (typeIdx != -1)	m_SheathCarbonType.GetLBText(typeIdx, strType);

	int volumeIdx = m_SheathCarbonVolume.GetCurSel();
	CString strVolume = _T("");
	if (volumeIdx != -1)	m_SheathCarbonVolume.GetLBText(volumeIdx, strVolume);

	std::tuple<double, double, double> data;
	m_pDoc->m_pAttrCtrl2->GetQMatdRein()->GetReinCfDoubleData(strType, strVolume, data);

	m_CarbonHeight = std::get<0>(data);
	m_CarbonFiber = std::get<1>(data);
	m_CarbonTens = std::get<2>(data);
	UpdateData(FALSE);
}

void CMPhiMaterialRcDlg_JP::OnAddModBtn()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}

	T_MATL_K key;
	T_MATD_D matdD; matdD.Initialize();
	T_MATD_REIN_D reinD; reinD.Initialize();
	m_pMPhiDataAll->GetMatd(key, matdD);
	m_pMPhiDataAll->GetMatdRein(key, reinD);
	if (!Dlg2Data(key, matdD, reinD)) return;

	// 일단 CMPhiParameterDlg::m_pMPhiDataAll에 쌓아 놓음
	m_pMPhiDataAll->SetMatd(key, matdD);
	if (reinD.kMatl != 0)
		m_pMPhiDataAll->SetMatdRein(key, reinD);

	ModifyItem(nItem, key, matdD);

	SetFocus();
	RedrawWindow();
}

void CMPhiMaterialRcDlg_JP::OnDeleteBtn()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}

	CString strID = m_ListCtrl.GetItemText(nItem, 0);
	
	T_MATL_K key = _ttoi(strID);
	T_MATD_D data; data.Initialize();
	if (m_pMPhiDataAll->GetMatd(key, data) == FALSE) {
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	}

	data.Rebar_CodeName = _T("");
	data.MainRebar_RebarName = _T("");
	data.MainRebarData.Initialize();
	data.SubRebar_RebarName = _T("");
	data.SubRebarData.Initialize();

	m_pMPhiDataAll->SetMatd(key, data);
	m_pMPhiDataAll->DeleteMatdRein(key);

	ModifyItem(nItem, key, data);
	InitRebarCode();
	if(m_bSheath == TRUE)
		OnClickbSheath();

	SetFocus();
	RedrawWindow();
}

BOOL CMPhiMaterialRcDlg_JP::Dlg2Data(OUT T_MATL_K& rKey, OUT T_MATD_D& rMatd, OUT T_MATD_REIN_D& rRein)
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return FALSE;
	}

	UpdateData(TRUE);
	if (! ErrorCheck()) return FALSE;

	CString strID = _T(""), strName = _T(""), strCode = _T(""), strRebarCode = _T("");
	CString strFc = _T(""), strChk = _T(""), strLambda = _T(""), strMain = _T("");
	CString strSub = _T(""), strMatName = _T("");
	
	strID = m_ListCtrl.GetItemText(nItem, 0);
	// Modify Material ID
	rKey = _ttoi(strID);
	rMatd.Initialize();
	BOOL bCheck = m_pMPhiDataAll->GetMatd(rKey, rMatd);

	// Get changed Code Name.
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index, strCode);							// Get Material Code.
	rMatd.Data1.CodeName = strCode;
	int reIndex = m_RebarCode.GetCurSel();
	m_RebarCode.GetLBText(reIndex, strRebarCode);	// Get Rebar Code.
	rMatd.Rebar_CodeName = strRebarCode;

	ModifyMaterial(Index, strCode, strMatName, strFc, strChk, strLambda, rMatd);

	ModifyRebar(Index, reIndex, bCheck, strRebarCode, strMain, strSub, rMatd);

	ModifyMaterialRein(rKey, rRein);

	return TRUE;
}

void CMPhiMaterialRcDlg_JP::ModifyItem(const int& nLineNo, const T_MATL_K& Key, const T_MATD_D& mData)
{
	LV_ITEM lvitem;
	LPTSTR szText[COLUMN_SIZE];

	CString strID; strID.Format(_T("%7d"), Key);
	CString strFc; strFc.Format(_T("%g"), mData.Data1.Design.C_fc);
	szText[0] = (LPTSTR)(LPCTSTR)strID;
	szText[1] = (LPTSTR)(LPCTSTR)mData.Name;
	szText[2] = (LPTSTR)(LPCTSTR)strFc;
	szText[3] = (LPTSTR)(LPCTSTR)mData.MainRebar_RebarName;
	szText[4] = (LPTSTR)(LPCTSTR)mData.SubRebar_RebarName;
	BOOL bMatdRein = m_pMPhiDataAll->ExistMatdRein(Key);
	szText[5] = bMatdRein ? _T("O") : _T("X");
	
	// Input ListBox.
	for (int i = 0; i < 7; i++)
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = nLineNo;
		lvitem.iSubItem = i;
		lvitem.pszText = szText[i];
		m_ListCtrl.SetItem(&lvitem);
	}
}

void CMPhiMaterialRcDlg_JP::InitRebarCode()
{
	CString strRebarCode = _T("None");

	int iIndex = m_RebarCode.FindStringExact(-1, strRebarCode);
	m_RebarCode.SetCurSel(iIndex);
	EnableRebarEditBox(TRUE);
}