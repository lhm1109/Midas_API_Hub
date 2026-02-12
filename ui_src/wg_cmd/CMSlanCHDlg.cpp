// CMSlanCHDlg.cpp : implementation file
// 2002. 8. 12, by TAE
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSlanCHDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\DB_ST_DT_UNIT.h"

extern double GetInitialWheelSpacing(int nLaneType);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSlanDlg dialog


CCMSlanCHDlg::CCMSlanCHDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMSlanCHDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSlanCHDlg)
	m_nMovDir = -1;
	m_nSelectionMethod = 0;
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bListMode = TRUE;

	m_aCtrlList.Add(IDC_CMD_LIST);
	m_aCtrlList.Add(IDC_CMD_BTN_ADD);
	m_aCtrlList.Add(IDC_CMD_BTN_MODIFY);
	m_aCtrlList.Add(IDC_CMD_BTN_DELETE);
	m_aCtrlList.Add(IDC_CMD_BTN_COPY);
	m_aCtrlList.Add(IDC_CMD_BTN_CLOSE);

	m_aCtrlData.Add(IDC_CMD_LANE_NAME_TITLE);
	m_aCtrlData.Add(IDC_CMD_LANE_NAME_EDIT);
	m_aCtrlData.Add(IDC_CMD_FRAME1);
	m_aCtrlData.Add(IDC_CMD_OFFSET_TITLE1);
	m_aCtrlData.Add(IDC_CMD_OFFSET_TITLE2);
	m_aCtrlData.Add(IDC_CMD_OFFSET_EDIT);
	m_aCtrlData.Add(IDC_CMD_OFFSET_UNIT);
	m_aCtrlData.Add(IDC_CMD_LWIDTH_TITLE);
	m_aCtrlData.Add(IDC_CMD_LWIDTH_EDIT);
	m_aCtrlData.Add(IDC_CMD_LWIDTH_UNIT);
	m_aCtrlData.Add(IDC_CMD_FACTOR_TITLE);
	m_aCtrlData.Add(IDC_CMD_FACTOR_EDIT);
	m_aCtrlData.Add(IDC_CMD_FACTOR_UNIT);
	m_aCtrlData.Add(IDC_CMD_FRAME7);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT2);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT3);

	m_aCtrlData.Add(IDC_CMD_2POINTS);
	m_aCtrlData.Add(IDC_CMD_2POINTS_EDIT1);
	m_aCtrlData.Add(IDC_CMD_2POINTS_EDIT2);
	m_aCtrlData.Add(IDC_CMD_PICKING);
	m_aCtrlData.Add(IDC_CMD_PICKING_EDIT);
	m_aCtrlData.Add(IDC_CMD_FRAME2);
	m_aCtrlData.Add(IDC_CMD_OP_ADD);
	m_aCtrlData.Add(IDC_CMD_OP_INSERT);
	m_aCtrlData.Add(IDC_CMD_OP_DELETE);
	m_aCtrlData.Add(IDC_CMD_PLACEHOLDER);
	m_aCtrlData.Add(IDC_CMD_BTN_OK);
	m_aCtrlData.Add(IDC_CMD_BTN_CANCEL);
	m_aCtrlData.Add(IDC_CMD_BTN_APPLY);
	m_aCtrlData.Add(IDC_CMD_NODESELECT);
	m_aCtrlData.Add(IDC_CMD_NODESELECT_EDIT);
	m_aCtrlData.Add(IDC_CMD_STATIC_G   );
	m_aCtrlData.Add(IDC_CMD_START_TITLE);
	m_aCtrlData.Add(IDC_CMD_END_TITLE  );
	m_aCtrlData.Add(IDC_CMD_SKEW_START_EDIT);
	m_aCtrlData.Add(IDC_CMD_SKEW_END_EDIT);
	m_aCtrlData.Add(IDC_CMD_START_UNIT);
	m_aCtrlData.Add(IDC_CMD_END_UNIT);
	m_aCtrlData.Add(IDC_CMD_BMP_STATIC);
	m_aCtrlData.Add(IDC_CMD_BMP_STATIC_GRP);
	m_aCtrlData.Add(IDC_CMD_ST_MOVINGDIR);
	m_aCtrlData.Add(IDC_CMD_MOV_FORWARD );
	m_aCtrlData.Add(IDC_CMD_MOV_BACKWARD);
	m_aCtrlData.Add(IDC_CMD_MOV_BOTH    );
	
	// Wheel Spacing
	m_aCtrlData.Add(IDC_CMD_WHEEL_TXT);
	m_aCtrlData.Add(IDC_CMD_WHEEL_EDT);
	m_aCtrlData.Add(IDC_CMD_WHEEL_UNT);

	// Lane Optimization
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_CHK);
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_GRP);
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_STC);
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT);
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT);

	m_aCtrlSelect.Add(IDC_CMD_2POINTS);
	m_aCtrlSelect.Add(IDC_CMD_PICKING);
	m_aCtrlSelect.Add(IDC_CMD_NODESELECT);
}


void CCMSlanCHDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSlanCHDlg)
	DDX_Control(pDX, IDC_CMD_WHEEL_UNT, m_untWheelSpace);
	DDX_Control(pDX, IDC_CMD_WHEEL_EDT, m_edtWheelSpace);
	DDX_Control(pDX, IDC_CMD_FACTOR_UNIT, m_unitSpace);
	DDX_Control(pDX, IDC_CMD_FACTOR_EDIT, m_editSpace);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT3, m_2PointUnit2);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT2, m_2PointUnit1);
	DDX_Control(pDX, IDC_CMD_SKEW_START_EDIT, m_edtSkewStart);
	DDX_Control(pDX, IDC_CMD_SKEW_END_EDIT, m_edtSkewEnd);
	DDX_Control(pDX, IDC_CMD_NODESELECT_EDIT, m_wndNodeSelectEdit);
	DDX_Control(pDX, IDC_CMD_PICKING_EDIT, m_wndPickingEdit);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT2, m_wnd2PointsEdit2);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT1, m_wnd2PointsEdit1);
	DDX_Control(pDX, IDC_CMD_OFFSET_EDIT, m_wndOffsetEdit);
	DDX_Control(pDX, IDC_CMD_OFFSET_UNIT, m_wndOffsetUnit);
	DDX_Control(pDX, IDC_CMD_LWIDTH_EDIT, m_wndLWidthEdit);
	DDX_Control(pDX, IDC_CMD_LWIDTH_UNIT, m_wndLWidthUnit);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_LANE_NAME_EDIT, m_wndLaneName);
	DDX_Radio(pDX, IDC_CMD_MOV_FORWARD, m_nMovDir);
	DDX_Radio(pDX, IDC_CMD_2POINTS, m_nSelectionMethod);
	DDX_Control(pDX, IDC_CMD_LANE_OPTIM_CHK, m_chkLaneOptim);
	DDX_Control(pDX, IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT, m_edtAllowWidth);
	DDX_Control(pDX, IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT, m_untAllowWidth);

	DDX_Control(pDX, IDC_CMD_BMP_STATIC_GRP,           m_chkBmp);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndGrid);
	DDX_Control(pDX, IDC_CMD_BMP_STATIC, m_ImgViewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSlanCHDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMSlanCHDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CMD_2POINTS, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_OP_ADD, OnCmdOpAdd)
	ON_BN_CLICKED(IDC_CMD_OP_INSERT, OnCmdOpInsert)
	ON_BN_CLICKED(IDC_CMD_OP_DELETE, OnCmdOpDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnCmdBtnOk)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY, OnCmdBtnApply)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_PICKING, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_NODESELECT, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY, OnCmdBtnCopy)
	ON_BN_CLICKED(IDC_CMD_LANE_OPTIM_CHK, OnCmdChangeOptimChk)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSlanCHDlg message handlers
#define COLCOUNT 1

void CCMSlanCHDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(IDC_CMD_LIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LANE_NAME_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlData, nDistY);
	
	//
	AlignControl_Fold();
	//

	ResizeDialog();
}

void CCMSlanCHDlg::AlignControl_Fold()
{

	CRect rRef;
	CRect rToMove;
	int nDistY;
	CArray<UINT, UINT> aControls;

	// Traffic Lane Properties 안에 그림을 제외한 컨트롤 이동 (Fold Group Box Auto Effect 꺼서 들어간 코드...)
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_LWIDTH_TITLE);
	aControls.Add(IDC_CMD_LWIDTH_EDIT);
	aControls.Add(IDC_CMD_LWIDTH_UNIT);
	aControls.Add(IDC_CMD_WHEEL_TXT);
	aControls.Add(IDC_CMD_WHEEL_EDT);
	aControls.Add(IDC_CMD_WHEEL_UNT);
	aControls.Add(IDC_CMD_OFFSET_TITLE1);
	aControls.Add(IDC_CMD_OFFSET_TITLE2);
	aControls.Add(IDC_CMD_OFFSET_EDIT);
	aControls.Add(IDC_CMD_OFFSET_UNIT);
	aControls.Add(IDC_CMD_FACTOR_TITLE);
	aControls.Add(IDC_CMD_FACTOR_EDIT);		
	aControls.Add(IDC_CMD_FACTOR_UNIT);		
	aControls.Add(IDC_CMD_STATIC_G   );
	aControls.Add(IDC_CMD_START_TITLE);
	aControls.Add(IDC_CMD_END_TITLE  );
	aControls.Add(IDC_CMD_SKEW_START_EDIT);
	aControls.Add(IDC_CMD_SKEW_END_EDIT);
	aControls.Add(IDC_CMD_START_UNIT);
	aControls.Add(IDC_CMD_END_UNIT);

	GetDlgItem(IDC_CMD_BMP_STATIC_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LWIDTH_EDIT)->GetWindowRect(rToMove);
	if(m_chkBmp.GetFoldState()) nDistY = rRef.bottom - rToMove.top;
	else                        nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Traffic Lane Properties
	CRect rectResize;
	GetDlgItem(IDC_CMD_FRAME1)->GetWindowRect(rectResize);
	GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rRef);
	rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);

	ScreenToClient(rectResize);
	GetDlgItem(IDC_CMD_FRAME1)->MoveWindow(rectResize);

	// Transverse Lane Optimization
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_LANE_OPTIM_CHK);
	aControls.Add(IDC_CMD_LANE_OPTIM_GRP);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_STC);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT);

	GetDlgItem(IDC_CMD_FRAME1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LANE_OPTIM_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Moving Direction
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_ST_MOVINGDIR);
	aControls.Add(IDC_CMD_MOV_FORWARD);
	aControls.Add(IDC_CMD_MOV_BACKWARD);
	aControls.Add(IDC_CMD_MOV_BOTH);

	GetDlgItem(IDC_CMD_LANE_OPTIM_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ST_MOVINGDIR)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);


	// Selection by
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME7);
	aControls.Add(IDC_CMD_2POINTS);
	aControls.Add(IDC_CMD_PICKING);
	aControls.Add(IDC_CMD_NODESELECT);
	aControls.Add(IDC_CMD_2POINTS_EDIT1);
	aControls.Add(IDC_CMD_2POINT_UNIT2);
	aControls.Add(IDC_CMD_2POINTS_EDIT2);
	aControls.Add(IDC_CMD_2POINT_UNIT3);

	GetDlgItem(IDC_CMD_ST_MOVINGDIR)->GetWindowRect(rRef); 
	GetDlgItem(IDC_CMD_FRAME7)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);


	// Operations
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME2);
	aControls.Add(IDC_CMD_OP_ADD);
	aControls.Add(IDC_CMD_OP_INSERT);
	aControls.Add(IDC_CMD_OP_DELETE);

	GetDlgItem(IDC_CMD_FRAME7)->GetWindowRect(rRef); 
	GetDlgItem(IDC_CMD_FRAME2)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Grid
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_PLACEHOLDER);

	GetDlgItem(IDC_CMD_FRAME2)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_PLACEHOLDER)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// OK, Cancel, Apply
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_BTN_OK);
	aControls.Add(IDC_CMD_BTN_CANCEL);
	aControls.Add(IDC_CMD_BTN_APPLY);

	GetDlgItem(IDC_CMD_PLACEHOLDER)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_BTN_CANCEL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
}

void CCMSlanCHDlg::ResizeDialog()
{
	UINT nLastCtrlID;
	if (m_bListMode) nLastCtrlID = IDC_CMD_BTN_CLOSE;
	else nLastCtrlID = IDC_CMD_BTN_CANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CCMSlanCHDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_CMD_MVLD_lane_name) };
//18japan  CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Lane_Name)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 140;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
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

void CCMSlanCHDlg::InitGrid()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	CGXGridParam* pParam = m_wndGrid.GetParam();
	ASSERT_VALID(pParam);

	CGXProperties* pProp = pParam->GetProperties();
	ASSERT_VALID(pProp);

	pParam->EnableUndo(FALSE);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	// Row and column headers
	m_wndGrid.ColHeaderStyle().SetReadOnly(FALSE).SetEnabled(FALSE)
														.SetFont(CGXFont_GC().SetBold(FALSE));
	m_wndGrid.RowHeaderStyle().SetEnabled(FALSE);
	m_wndGrid.ChangeStandardStyle(CGXStyle().SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetAllowEnter(FALSE));

	// Special grid settings
	pParam->EnableMoveRows(TRUE);                  // rows cannot be dragged
	pParam->EnableMoveCols(FALSE);                 // columns can be dragged
	//pParam->EnableTrackColWidth(FALSE);             // disable resizing
	pParam->EnableTrackRowHeight(FALSE);
	pParam->EnableThumbTrack(FALSE);               // thumb-tracking is disabled
	pParam->SetNumberedColHeaders(FALSE);
	// Excel과 같은 셀 선택 : DoLButtonDblClk 참조
	pParam->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	pParam->SetHideCurrentCell(GX_HIDE_NEVER);

	// removing rows is not undoable for this kind of grid
	pParam->SetRemoveColsFlags(FALSE);
	pParam->SetRemoveRowsFlags(FALSE);

	pProp->SetMarkColHeader(FALSE);    // Turn off pressed button effect for column headers
	pProp->SetPrintRowHeaders(FALSE);  // Don't print column headers

	m_wndGrid.SetRowHeightByDPI(0, 0, 32);
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	//m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_wndGrid.SetColCount(4);
	SetGridTitle();
	m_wndGrid.SetColWidthByDPI(0, 0, 40);
	m_wndGrid.SetColWidthByDPI(1, 1, 40);
	m_wndGrid.SetColWidthByDPI(2, 2, 55);
	m_wndGrid.SetColWidthByDPI(3, 3, 55);
	m_wndGrid.SetColWidthByDPI(4, 4, 40);
	// Check Box Column의 스타일 변경
	m_wndGrid.SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetValueType(GX_VT_NUMERIC)
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
			//.SetValue(_T("0"))
			);
	//m_wndGrid.EnableOleDataSource(GX_DNDDISABLED); // copy & paste 금지

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMSlanCHDlg::SetGridTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_SLANch_UNIT.Offset, csUnit); //나중에 물어봐야지.. 
																			 // Offset 단위를 기본으로 잡아도 되는지 	
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_CMD_SLAN_no));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD_SLAN_node));
	m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_LS(IDS_CMD_SLAN_unit1))+csUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 3), CString(_LS(IDS_CMD_SLAN_unit2))+csUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 4), _LS(IDS_CMD_SLAN_SPAN_START));
	m_wndGrid.LockUpdate(bOldLock);

	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMSlanCHDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	T_SLANch_D SlanD;
	CArray<T_SLANch_K, T_SLANch_K> aSlanK;
	m_pDoc->m_pAttrCtrl->GetSlanchKeyList(aSlanK);

	for (int i = 0; i < aSlanK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetSlanch(aSlanK[i], SlanD)) continue;

		InsertItem(aSlanK[i], SlanD);
	}
}

void CCMSlanCHDlg::InitItemDataVar()
{
	m_wndOffsetEdit.SetWindowText(_T("0.0"));
	m_editSpace.SetUnitType(CUnitCtrl::m_SLANch_UNIT.dSpan);
	m_unitSpace.SetUnitType(CUnitCtrl::m_SLANch_UNIT.dSpan);
	m_editSpace.SetWindowText(_T("0.0"));
	m_wnd2PointsEdit1.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wnd2PointsEdit2.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wndPickingEdit.SetWindowText(_T(""));
}

BOOL CCMSlanCHDlg::InsertItem(T_SLANch_K Key, T_SLANch_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Data.LaneName);
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

CString CCMSlanCHDlg::DataToStr(int i, T_SLANch_K Key, T_SLANch_D &Data)
{
	CString str;
	if (i == 0) str = Data.LaneName;
	else str = _T("");

	return str;
}

void CCMSlanCHDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		//DestroyWindow();
		MakeItemEx();
		ButtonCtrl();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMSlanCHDlg::ButtonCtrl()
{
	BOOL bEnable = FALSE;
	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag())
		bEnable = TRUE;

	GetDlgItem(IDC_CMD_BTN_OK)->EnableWindow(bEnable);
}

int CCMSlanCHDlg::FindInsertionPos(CString& strName)
{
	T_SLANch_K key;
	T_SLANch_D data;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetSlanch(key, data);

		if (strName.Compare(data.LaneName) < 0) break;
	}
	return i;    
}

void CCMSlanCHDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SLANch_K Key, KeyBak;
	T_SLANch_D Data, DataBak;

	BOOL bMFD = FALSE;
	BOOL bMvcdChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
	  switch(nCmd)
	  {
		case(UR_SLANch_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetSlanch(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_SLANch_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetSlanch(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_SLANch_MFD):
			{
				pViewBuff->GetSlanch(nKey, KeyBak, DataBak);
			}
			bMFD = TRUE;
			break;
		case(UR_SLANch_MFS):
			{
				pViewBuff->GetSlanch(nKey, Key, Data);
				DeleteItem(Key, Data);
				InsertItem(KeyBak, DataBak);
			}
			bMFD = FALSE;
			break;
		case(UR_MVCD_ADD):
		case(UR_MVCD_DEL):
			bMvcdChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	// 코드가 바뀌면 무조건 닫는다.
	if (bMvcdChanged)
	{
		DestroyWindow();
	}
}

/*
BOOL CCMSlanCHDlg::IsRelevantCode(int nCode)
{
	if (nCode == D_MOVE_CODE_CHINA) return TRUE;
	return FALSE;
}
*/

void CCMSlanCHDlg::ShowHideByCode()
{
	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_STC);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT);

	int nCheck = m_chkLaneOptim.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, aControls, nCheck==1);
}

void CCMSlanCHDlg::UpdateUnit() 
{
	SetGridTitle();
	if (m_bListMode) return;  // unit change할 게 없다.
	
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	int nUnitTypeOffset = CUnitCtrl::m_SLANch_UNIT.Offset;  
	int nUnitTypeSpan = CUnitCtrl::m_SLANch_UNIT.dSpan;  

	double dblOffset;                                 
	double dblSpan;                                 
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		dblOffset = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
		dblOffset = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitTypeOffset, dblOffset);
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), dblOffset);

		dblSpan = _tstof(m_wndGrid.GetValueRowCol(i+1, 3));
		dblSpan = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitTypeSpan, dblSpan);
		m_wndGrid.SetValueRange(CGXRange(i+1, 3), dblSpan);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMSlanCHDlg::DeleteItem(T_SLANch_K Key, T_SLANch_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMSlanCHDlg::Dlg2Data()
{
	UpdateData(TRUE);
	T_SLANch_BASE item;
	m_wndLaneName.GetWindowText(m_Data.LaneName);
	CFormulaEdit::GetEditValue(&m_wndLWidthEdit, m_Data.Width);
	int nCount = m_wndGrid.GetRowCount();
	m_Data.arLaneData.SetSize(nCount);
	for (int i = 0; i < nCount; i++)
	{
		item.KeyNode = _tstol(m_wndGrid.GetValueRowCol(i+1, 1));
		item.Offset = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
		item.dSpan = _tstof(m_wndGrid.GetValueRowCol(i+1, 3));
		item.bSpanStart = (m_wndGrid.GetValueRowCol(i+1, 4)==_T("1")) ? TRUE : FALSE;
		m_Data.arLaneData.SetAt(i, item);
	}
	
	m_edtSkewStart.GetEditValue(m_Data.dSkewStart);
	m_edtSkewEnd.GetEditValue(m_Data.dSkewEnd);
	m_Data.dWheelSpace = m_edtWheelSpace.GetEditValue();
	m_Data.nMovingDir = m_nMovDir;

	int nCheck = m_chkLaneOptim.GetCheck();
	m_Data.bAutoOptimize = (nCheck == 1)? TRUE : FALSE;
	m_Data.dAllowWidth = m_edtAllowWidth.GetEditValue();
	
	return TRUE;
}

void CCMSlanCHDlg::Data2Dlg()
{
	m_wndLaneName.SetWindowText(m_Data.LaneName);
	CString csVal;
	csVal.Format(_T("%g"), m_Data.Width);
	m_wndLWidthEdit.SetWindowText(csVal);
	InitItemDataVar();

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	 
	int nSize = m_Data.arLaneData.GetSize();

	m_wndGrid.SetRowCount(nSize);
	for (int i = 0; i < nSize; i++)
	{ 
		m_wndGrid.SetValueRange(CGXRange(i+1, 1), (long)m_Data.arLaneData[i].KeyNode);  
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), m_Data.arLaneData[i].Offset); 
		m_wndGrid.SetValueRange(CGXRange(i+1, 3), m_Data.arLaneData[i].dSpan); 
		CString str = (m_Data.arLaneData[i].bSpanStart) ? _T("1") : _T("0");
		m_wndGrid.SetValueRange(CGXRange(i+1, 4), str);
	}
	if (nSize > 0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	CString str;

	str.Format(_T("%g"), m_Data.dSkewStart);
	m_edtSkewStart.SetWindowText(str);

	str.Format(_T("%g"), m_Data.dSkewEnd);
	m_edtSkewEnd.SetWindowText(str);

	m_edtWheelSpace.SetEditUnit(m_Data.dWheelSpace);

	int nCheck = (m_Data.bAutoOptimize)? 1 : 0;
	m_chkLaneOptim.SetCheck(nCheck);
	m_edtAllowWidth.SetEditUnit(m_Data.dAllowWidth);

	m_nMovDir = m_Data.nMovingDir;
	
	UpdateData(FALSE);

	SelectNode();
}

BOOL CCMSlanCHDlg::Grid_IsExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		if (nKey == _tstol(m_wndGrid.GetValueRowCol(i+1, 1))) return TRUE;
	}
	return FALSE;
}

void CCMSlanCHDlg::Grid_InsertItem(int nPos, T_SLANch_BASE &item)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	m_wndGrid.InsertRows(nRow+1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), (long)item.KeyNode); 
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), item.Offset); 
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 3), item.dSpan); 
	CString str = _T("0");
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 4), str);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMSlanCHDlg::InsertBaseItem(int nPos, int nType)
{
	T_SLANch_BASE item;

	CFormulaEdit::GetEditValue(&m_wndOffsetEdit,item.Offset);
	CFormulaEdit::GetEditValue(&m_editSpace, item.dSpan);

	if (nType == 0)  // Select By 2 Points
	{
		CArray<T_NODE_K, T_NODE_K> aNodeKey;  
		double ux, uy, uz;
		double WX1, WY1, WZ1;
		double WX2, WY2, WZ2;

		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit1, ux, uy, uz))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___1st_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit1.GetWcsPos(ux, uy, uz, WX1, WY1, WZ1);
		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit2, ux, uy, uz))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___2nd_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit2.GetWcsPos(ux, uy, uz, WX2, WY2, WZ2);

	// Get Node Key List
	// 2000.6.5 Add by scchoi
	double dbTolerance = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
	m_pDoc->m_pDataCtrl->GetNodeBy2Point(WX1, WY1, WZ1, WX2, WY2, WZ2, dbTolerance, aNodeKey, TRUE);

		if (aNodeKey.GetSize() == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Node_on_line_));
			return;
		}  

	int nNodeKey;
		int nCount = aNodeKey.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			nNodeKey = aNodeKey[i];
			if (Grid_IsExistKey(nNodeKey))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
				AfxMessageBox(msg);
				return;
			}
	  item.KeyNode = (unsigned int)nNodeKey;  
			Grid_InsertItem(nPos, item);
			if (nPos >= 0) nPos++;
		}

	 SelectNode();
	 return;
	}
	else if(nType == 1) // Select By Picking
	{
		int nNodeKey;
		CFormulaEdit::GetEditValue(&m_wndPickingEdit, nNodeKey);
		if (nNodeKey <= 0)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_not_exist_), nNodeKey);
			AfxMessageBox(msg);
			return;
		}
		if (Grid_IsExistKey(nNodeKey))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
			AfxMessageBox(msg);
			return;
		}
		T_NODE_D Data;

	m_pDoc->m_pAttrCtrl->GetNode(nNodeKey, Data);

	item.KeyNode = (unsigned int)nNodeKey;  
		Grid_InsertItem(nPos, item);
	}
	else if(nType == 2) // Select By Select Node
	{
		CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
		CArray<long, long> arNodeList;

		CString strNodeNumber;
	  m_wndNodeSelectEdit.GetWindowText(strNodeNumber);

		m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

		CNumericOptimizer optimizer;

		optimizer.SelectNumByStrBase((long*)arNodeKeyList.GetData(), (long)arNodeKeyList.GetSize(),
			                            strNodeNumber, arNodeList);

		if(arNodeList.GetSize() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_node_));
			return;
		}

		for(int i=0; i<arNodeList.GetSize(); i++)
			if (Grid_IsExistKey((UINT)arNodeList[i]))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No__d__is_already_ex), (UINT)arNodeList[i]);
				AfxMessageBox(msg);
				return;
			} 
		int nInsertPos = nPos;
		for(int i=0; i<arNodeList.GetSize(); i++)
		{
			T_NODE_D Data;

			m_pDoc->m_pAttrCtrl->GetNode((UINT)arNodeList[i], Data);

			item.KeyNode = (UINT)arNodeList[i];
			Grid_InsertItem(nPos, item);
			if(nInsertPos >= 0) nPos++;
		}
	}


	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	if (!bCurrent) ncCol = 1;

	if (nPos == -1) ncRow = m_wndGrid.GetRowCount();
	else ncRow = nPos+1;

	m_wndGrid.SetCurrentCell(ncRow, ncCol);

	SelectNode();
}

// add by pig
void CCMSlanCHDlg::SelectNode() 
{
	int nCount = m_wndGrid.GetRowCount();
	if(nCount >= 0)
	{
	  CArray<T_NODE_K, T_NODE_K>arSelNode;
	  for (int i = 0; i < nCount; i++)arSelNode.Add(_tstol(m_wndGrid.GetValueRowCol(i+1, 1)));
	  m_pDoc->m_pViewCtrl->SelectNode(NULL, arSelNode, TRUE, TRUE);
	}
}

BOOL CCMSlanCHDlg::OnInitDialog() 
{ // This
	CCMDlgBase::OnInitDialog();
	
	// Auto Effect 끄자 (Auto Effect 가 왜 안먹을까...)
	m_chkBmp.InitControl(this, CCMSlanCHDlg::IDD, IDC_CMD_BMP_STATIC_GRP,FALSE,TRUE);
	m_chkBmp.SetFoldState(FALSE);

	// TODO: Add extra initialization here
	AlignControl();
	SetHeaderTitle();
	MakeItemEx();
	ButtonCtrl();

	m_edtSkewStart.SetRange(SHRT_MIN, SHRT_MAX);
	m_edtSkewEnd.SetRange(SHRT_MIN, SHRT_MAX);

	CString strImgPath = _T("SVG\\illustration\\Dialog\\cmd_tsur 2.svg");

	m_ImgViewer.SetImage(strImgPath);

	m_wndOffsetUnit.SetUnitType(CUnitCtrl::m_SLANch_UNIT.Offset);
	m_wndLWidthUnit.SetUnitType(CUnitCtrl::m_SLANch_UNIT.Width );  
	m_edtSkewStart .SetUnitType(CUnitCtrl::m_SLANch_UNIT.dSkewStart);
	m_edtSkewEnd   .SetUnitType(CUnitCtrl::m_SLANch_UNIT.dSkewEnd  );
	// this  
	m_2PointUnit1.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_2PointUnit2.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_untAllowWidth.SetUnitType(CUnitCtrl::m_LLAN_UNIT.dAllowWidth);

	m_wndOffsetEdit.SetAttUcsDistance();
	m_wndOffsetEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndOffsetEdit.SetDistValueMode(TRUE);
	m_wndLWidthEdit.SetAttUcsDistance();
	m_wndLWidthEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndLWidthEdit.SetDistValueMode(TRUE);

	m_wnd2PointsEdit1.SetAttUcsPos();
	m_wnd2PointsEdit1.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_wnd2PointsEdit2.SetAttUcsPos();
	m_wnd2PointsEdit2.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_wnd2PointsEdit2.SetLButtonDownNotifyWindow(this);
	m_wnd2PointsEdit2.SetEnterNotifyWindow(this);
	m_wnd2PointsEdit1.SetNextLink(&m_wnd2PointsEdit2);

	m_wndPickingEdit.SetAttNodeList();
	m_wndPickingEdit.SetLButtonDownNotifyWindow(this);
	m_wndPickingEdit.SetEnterNotifyWindow(this);
	m_wndPickingEdit.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndPickingEdit.SetMaxNodeKeyNum(1);

	m_edtWheelSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untWheelSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);

	InitGrid();

	InitItemDataVar();

	if(m_bModify)
	{
		SetInitModify();
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
		CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	}

	SelectMethodMan();

	ShowHideByCode();
	
	return TRUE;   // return TRUE unless you set the focus to a control
								// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSlanCHDlg::SetParamData(T_SLANch_D& data)
{
	m_bModify = TRUE;
	m_Data = data;
}

void CCMSlanCHDlg::SetInitModify()
{
	m_csOldLaneName = m_Data.LaneName;
	
	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	
	m_wndLaneName.SetFocus();

	ShowHideByCode();
}

void CCMSlanCHDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	m_bModify = FALSE;
	m_Data.Initialize();

	//-------------------------------------------------------------
	// MQC 4897-13 단위가 ft인 경우 Lane Width Default 값을 10ft로.
	T_UNIT_INDEX CurrentUnitIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurrentUnitIndex);
	if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT ||
		CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)
	{
		m_Data.Width = M_InitValueCurUnit(10.0, LBF, FT, CUnitCtrl::m_SLANch_UNIT.Width);
		m_Data.dAllowWidth = M_InitValueCurUnit(10.0, LBF, FT, CUnitCtrl::m_LLAN_UNIT.dAllowWidth);
	}
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M ||
		CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM )
	{
		m_Data.Width = M_InitValueCurUnit(3.0, KN, M, CUnitCtrl::m_SLANch_UNIT.Width);
		m_Data.dAllowWidth = M_InitValueCurUnit(3.0, KN, M, CUnitCtrl::m_LLAN_UNIT.dAllowWidth);
	}
	//-------------------------------------------------------------

	m_Data.dWheelSpace = GetInitialWheelSpacing(0);
	Data2Dlg();

	m_bListMode = FALSE;
	m_chkBmp.SetFoldState(FALSE);
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();SelectMethodMan();
	ShowHideByCode();
}

void CCMSlanCHDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMSlanCHDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();	
	*pResult = 0;
}

void CCMSlanCHDlg::OnCmdBtnCancel() 
{
	// TODO: Add your control notification handler code here
	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END); //현재 작업을 종료한다. 이전 상태로 복귀한다. 
	m_List.SetFocus();
}

void CCMSlanCHDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	BOOL bOldLock = m_wndGrid.LockUpdate(FALSE);
	if (bOldLock) m_wndGrid.Redraw();
	m_wndGrid.LockUpdate(bOldLock);	
	// Do not call CCMDlgBase::OnPaint() for painting messages
}

void CCMSlanCHDlg::OnCmdChangeSelectBy() 
{
	UpdateData(TRUE);
	SelectMethodMan();
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	if (m_nSelectionMethod == 0) m_wnd2PointsEdit1.SetFocus();
	else if(m_nSelectionMethod == 1) m_wndPickingEdit.SetFocus();
	else m_wndNodeSelectEdit.SetFocus();
}

void CCMSlanCHDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_SLANch_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
 
	Key = (T_SLANch_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetSlanch(Key, m_Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Moving_load_lane_key__d_d), Key);
		AfxMessageBox(msg);
		return;
	}

	m_csOldLaneName = m_Data.LaneName;
	
	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;	
	m_chkBmp.SetFoldState(FALSE);
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	
	m_wndLaneName.SetFocus();SelectMethodMan();
	ShowHideByCode();
}

void CCMSlanCHDlg::OnCmdBtnCopy() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
	CString csSLaneName = m_List.GetItemText(iItem, 0);

	CArray<CString,CString&> SLaneNames;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetSlanchList(SLaneNames);

	CString StrName;
	if(!CStrParser::MakeUniqueName(SLaneNames,csSLaneName,StrName))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Unable_to_make_lane_Name___));
		return;
	}

	
	T_SLANch_D LaneD, SourceLaneD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetSlanch(csSLaneName,SourceLaneD))
	{
		CString StrTe;
		StrTe.Format(_LS(IDS_CMD0417__Not_Exist_Lane__s),csSLaneName);
		AfxMessageBox(StrTe);
		return;
	}

	LaneD = SourceLaneD;
	LaneD.LaneName = StrName;
		
	CDBDoc::GetDocPoint()->m_pDataCtrl->AddSlanch(LaneD);
}

void CCMSlanCHDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}

	CString csLaneName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelSlanch(csLaneName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

LRESULT CCMSlanCHDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (!m_bListMode)
	{
		int nType;
		if (wParam == m_wnd2PointsEdit2.GetDlgCtrlID()) nType = 0;
		else if (wParam == m_wndPickingEdit.GetDlgCtrlID()) nType = 1;
		else if (wParam == m_wndNodeSelectEdit.GetDlgCtrlID()) nType = 2;
		else ASSERT(0);

		InsertBaseItem(-1, nType);
	}

	return 0L;
}

void CCMSlanCHDlg::OnCmdOpAdd() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(-1, nCheck);	
}

void CCMSlanCHDlg::OnCmdOpInsert() 
{
	// TODO: Add your control notification handler code here
	ROWCOL nRow, nCol;
	if (m_wndGrid.GetCurrentCell(nRow, nCol))
	{
		nRow = nRow-1;
	}
	else nRow = -1;
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(nRow, nCheck);	
}

void CCMSlanCHDlg::OnCmdOpDelete() 
{
	// TODO: Add your control notification handler code here
	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	CRowColArray awRows;
	m_wndGrid.GetSelectedRows(awRows, TRUE, TRUE);
	if (awRows.GetSize() < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_item_to_delete_));
		return;
	}
	ROWCOL nRow;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = awRows.GetSize()-1; i >= 0; i--)
	{
		nRow = awRows.GetAt(i);
		if (nRow == 0) continue;
		m_wndGrid.RemoveRows(nRow, nRow);
	}

	int nRowCount = m_wndGrid.GetRowCount();
	if (bCurrent && nRowCount > 0)
	{
		if (nRowCount < ncRow) ncRow = nRowCount;
		m_wndGrid.SetCurrentCell(ncRow, ncCol);
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();	

	SelectNode();
}

void CCMSlanCHDlg::OnCmdBtnOk() 
{
	// TODO: Add your control notification handler code here
	BOOL CheckSlan=FALSE;
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);

	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) CheckSlan = m_pDoc->m_pDataCtrl->ModifySlanch(m_csOldLaneName, m_Data);
	else CheckSlan = m_pDoc->m_pDataCtrl->AddSlanch(m_Data); 

	if (!CheckSlan)   // 입력 데이타에 오류가 있을때 Window를 Close하지 않게 한다.
		return; 

	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);	
	m_List.SetFocus();
}

void CCMSlanCHDlg::OnCmdBtnApply() 
{
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);

	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) m_pDoc->m_pDataCtrl->ModifySlanch(m_csOldLaneName, m_Data);
	else m_pDoc->m_pDataCtrl->AddSlanch(m_Data); 
}

BOOL CCMSlanCHDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = GetFocus();
		if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
			pMsg->wParam == VK_DELETE && pWnd->GetDlgCtrlID() == IDC_CMD_PLACEHOLDER)
		{
			OnCmdOpDelete();
			return TRUE;
		}
	}	
	return CCMDlgBase::PreTranslateMessage(pMsg);
}

void CCMSlanCHDlg::SelectMethodMan()
{
	CArray<UINT,UINT> PointArr;
	CArray<UINT,UINT> PickingArr;
	CArray<UINT,UINT> ElemSelArr;
	
	PointArr.Add(IDC_CMD_2POINTS_EDIT1    );
	PointArr.Add(IDC_CMD_2POINTS_EDIT2    );
	PointArr.Add(IDC_CMD_2POINT_UNIT2     );
	PointArr.Add(IDC_CMD_2POINT_UNIT3     );
	PickingArr.Add(IDC_CMD_PICKING_EDIT   );
	ElemSelArr.Add(IDC_CMD_NODESELECT_EDIT);


	if(m_nSelectionMethod == 0)
	{
		CDlgUtil::CtrlShowHide(this,PointArr  ,TRUE);
		CDlgUtil::CtrlShowHide(this,PickingArr,FALSE);
		CDlgUtil::CtrlShowHide(this,ElemSelArr,FALSE);
	}
	else if( m_nSelectionMethod == 1)
	{
		CRect DRect,SRect;
		
		m_wnd2PointsEdit1.GetWindowRect(&DRect);
		m_wndPickingEdit.GetWindowRect(&SRect);

		int Dx = DRect.left - SRect.left;
		int Dy = DRect.top - SRect.top;

		CDlgUtil::CtrlShowHide(this,PointArr  ,FALSE);
		CDlgUtil::CtrlMoveDistXY(this,PickingArr,Dx, Dy);
		CDlgUtil::CtrlShowHide(this,PickingArr,TRUE);
		CDlgUtil::CtrlShowHide(this,ElemSelArr,FALSE); 
	}
	else
	{
		CRect DRect, SRect;
		m_wnd2PointsEdit1 .GetWindowRect(&DRect);
		m_wndNodeSelectEdit.GetWindowRect(&SRect);

		int Dx = DRect.left - SRect.left;
		int Dy = DRect.top - SRect.top;

		CDlgUtil::CtrlShowHide(this,PointArr  ,FALSE);
		CDlgUtil::CtrlShowHide(this,PickingArr,FALSE);
		CDlgUtil::CtrlMoveDistXY(this,ElemSelArr,Dx, Dy);
		CDlgUtil::CtrlShowHide(this,ElemSelArr,TRUE); 
	}
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
}


////////////////////////////////////////////////////////////////////////////
// 외부에서 Data를 Setting 하는 함수  
// 단, Dialog(Modaless)는 이미 Create&Activartion 되어있는 것을 전제로 한다.
// 이 함수를 Call 해주면 입력모드(List Mode가 아님) 상태에서 
// 인자로 넘어온 Key, Data를 Setting 하여 보여준다.
// by TAE
BOOL CCMSlanCHDlg::ExternalSetting(T_SLANch_K SlanKey)
{
	BOOL bFlag = TRUE;
	bFlag = ::IsWindow(this->GetSafeHwnd());                 // 윈도우가 생성되지 않았으면
	bFlag = m_pDoc->m_pAttrCtrl->ExistSlanch(SlanKey);       // 키값이 존재하지 않으면
	bFlag = m_pDoc->m_pAttrCtrl->GetSlanch(SlanKey, m_Data); // Data를 가져올 수 없으면
	if(!bFlag) return bFlag;

	if(IsWindow(m_chkBmp.GetSafeHwnd()))
	{
		m_chkBmp.SetFoldState(FALSE);
	}  

	m_csOldLaneName = m_Data.LaneName;
	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	ShowHideByCode();

	return TRUE;
}

void CCMSlanCHDlg::OnCmdChangeOptimChk() 
{
	ShowHideByCode();
}


LRESULT CCMSlanCHDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	AlignControl();
	SelectMethodMan();
	return 0L;
}
