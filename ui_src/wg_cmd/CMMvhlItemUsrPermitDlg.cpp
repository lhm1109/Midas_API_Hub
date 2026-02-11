// CMMvhlItemUsrPermitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrPermitDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\wg_db\VehlDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_MAX_WHEEL_LINE  10

/////////////////////////////////////////////////////////////////////////////
// CCMAxleTypeGrid dialog
CCMAxleTypeGrid::CCMAxleTypeGrid()
{
}
CCMAxleTypeGrid::~CCMAxleTypeGrid()
{
}

void CCMAxleTypeGrid::Initialize()
{
	CGXGridWnd::Initialize();
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	CGXGridParam* pParam = GetParam();
	pParam->EnableUndo(FALSE);
	EnableScrollTips();
	EnableHints();
	SetDrawingTechnique(gxDrawUsingMemDC);

	LockUpdate(TRUE);

	pParam->EnableTrackRowHeight(FALSE); 
	pParam->EnableMoveRows(FALSE);                  // rows cannot be dragged
	pParam->EnableMoveCols(TRUE);                   // columns can be dragged
	pParam->EnableThumbTrack(FALSE);                // thumb-tracking is disabled
	pParam->SetNumberedRowHeaders(FALSE);
	pParam->SetNumberedColHeaders(FALSE);

	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ColHeaderStyle()
			  .SetReadOnly(FALSE)
			  .SetEnabled(FALSE); // Current cell cannot be moved to header

	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetFaceName(_LS(IDS_CMD_FONT_Ms_Sans_Serif)).SetSize(9))
			.SetVerticalAlignment(DT_VCENTER)
			.SetAllowEnter(FALSE));

	// Wheel Mouse 지원
	EnableMouseWheel();

	// Show Horizontal Scroll Bar Always
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	// 컬럼 갯수 설정
	SetColCount(2*D_MAX_WHEEL_LINE);
	SetColWidth(0, 0, globalUtils.ScaleByDPI(20));
	SetColWidth(1, GetColCount(), globalUtils.ScaleByDPI(40));
	SetRowCount(1);
	SetHeaderTitle();

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetCurrentCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	LockUpdate(FALSE);
}

void CCMAxleTypeGrid::SetHeaderTitle()
{
	CString csP, csD;
	for (int i = 0; i < D_MAX_WHEEL_LINE; i++)
	{
		csP.Format(_T("P%d"), i+1);
		csD.Format(_T("D%d"), i+1);
		SetValueRange(CGXRange(0, 2*i+1), csP);
		SetValueRange(CGXRange(0, 2*i+1+1), csD);
	}
}

BOOL CCMAxleTypeGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bReadOnly;
	BOOL bHeaderCol;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	BOOL bReadOnlyMode = FALSE;
	BOOL bCurrentRow = TRUE;
	bCurrentCell = (nRow == ncRow && nCol == ncCol);

	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
	// ComposeStyleRowCol은 호출해서는 안된다.
	// 그래서 여기서는 직접 조사한다.
	// 단 Cell Style, Column Style만 조사한다.
	if (style.GetIncludeEnabled())  // 셀에 Enable정보 포함 여부 알아냄
	{
		bEnabled = style.GetEnabled();
	}
	else // Column style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	if (style.GetIncludeReadOnly()) // 셀에 readonly정보 포함 여부 알아냄
	{
		bReadOnly = style.GetReadOnly();
	}
	else // Column style에서 readonly 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bReadOnly = (styleEx.GetIncludeReadOnly() && styleEx.GetReadOnly());
	}

	bHeaderCol = IsFrozenCol(nCol);

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bReadOnly && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		if (bReadOnlyMode) style
			.SetInterior(CTBGrid::m_Color.ReadOnlyModeBg)
			.SetTextColor(CTBGrid::m_Color.ReadOnlyModeFg);
		else style
			.SetInterior(CTBGrid::m_Color.NormalModeBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.DisabledFg);

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentCellBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentCellFg);
	else if (bCurrentRow) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentRowBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentRowFg);
	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}

BOOL CCMAxleTypeGrid::OnPasteFromClipboard(const CGXRange &range)
{
	CGXGridWnd x;
	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), GetParent(), 0);
	x.Initialize();
	x.GetParam()->EnableUndo(FALSE);
	x.LockUpdate(TRUE);
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	ROWCOL i = 1;
	for (i = 1; i <= x.GetColCount(); i++)
		if (!ValidateField(x.GetValueRowCol(1, i), i)) break;
	if (i == x.GetColCount()+1)
	{
		int ix = 1;
		for (ROWCOL i = ncCol; i <= GetColCount() && ix <= x.GetColCount(); i++)
		{
			SetValueRange(CGXRange(1, i), x.GetValueRowCol(1, ix++));
		}
	}
	else
	{
		DisplayWarningText();
	}
	x.DestroyWindow();
	return TRUE;
}

BOOL CCMAxleTypeGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString value;
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (nRow == ncRow && nCol == ncCol)
	{
		CGXControl* pControl = GetControl(nRow, nCol);
		if (pControl->IsActive()) pControl->GetCurrentText(value);
		else value = GetValueRowCol(nRow, nCol);
	}
	else value = GetValueRowCol(nRow, nCol);

	if (!ValidateField(value, nCol)) return FALSE;
	return TRUE;
}

BOOL CCMAxleTypeGrid::ValidateField(CString value, int nColID)
{
	double dVal;
	if (!CFormulaEdit::GetFormulaValue(value, dVal))
	{
		SetWarningText(_LS(IDS_CMD_ERROR_NOT_NUMERIC_VALUE));
		return FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrPermitDlg dialog


CCMMvhlItemUsrPermitDlg::CCMMvhlItemUsrPermitDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrPermitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrPermitDlg)
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_csOldName = _T("");
	m_nNextAction = CLOSE_DLG;
	m_nType = 0;
	m_nBSType = 0;
	m_nEuroBSType = 0;
	m_nSouthAfricaType = 0;
	m_nIndiaType = 0;
	m_nCodeType = 0;
	m_nASType = 0;
	
	m_aCtrlLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_TRK_RD);
	m_aCtrlLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LEGAL_RD);
	m_aCtrlLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_TRN_RD);
	m_aCtrlLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_CRD_RD);  

	m_aCtrlBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO1);
	m_aCtrlBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO2);
	m_aCtrlBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO3);
	m_aCtrlBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO4);
	m_aCtrlBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO5);
	m_aCtrlBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO6);
	m_aCtrlBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO7);

	m_aCtrlEuroBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO);
	m_aCtrlEuroBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LM2_RDO);
	m_aCtrlEuroBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_RDO);
	m_aCtrlEuroBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_RDO);
	m_aCtrlEuroBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_RDO);
	m_aCtrlEuroBSLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_EURO_BS_RDO);

	m_aCtrlSouthAfricaLoadType.Add(IDC_CMD_MVHL_NA_RDO);
	m_aCtrlSouthAfricaLoadType.Add(IDC_CMD_MVHL_NB_RDO);
	m_aCtrlSouthAfricaLoadType.Add(IDC_CMD_MVHL_NC_RDO);
	//m_aCtrlSouthAfricaLoadType.Add(IDC_CMD_MVHL_NBNC_RDO);
	m_aCtrlSouthAfricaLoadType.Add(IDC_CMD_MVHL_PERMIT_RDO);

	m_aCtrlIndiaType.Add(IDC_CMD_MVHL_WHEELED_TRACKED_RDO);
	m_aCtrlIndiaType.Add(IDC_CMD_MVHL_WHEELED_RDO);
	m_aCtrlIndiaType.Add(IDC_CMD_MVHL_INDIA_PERMIT_RDO);

	m_aCtrlASLoadType.Add(IDC_CMD_USER_TRK_RDO);
	m_aCtrlASLoadType.Add(IDC_CMD_USER_TRN_RDO);
	m_aCtrlASLoadType.Add(IDC_CMD_USER_150LA_RDO);
	m_aCtrlASLoadType.Add(IDC_CMD_USER_PERMIT_RDO);

}


int CCMMvhlItemUsrPermitDlg::GetType()
{
	if(m_nType == 0) return 0;
	else if(m_nType == 1) return 3;
	else if(m_nType == 2) return 1;
	else if(m_nType == 3) return 2;
	
	ASSERT(0);
	return 0;
}

void CCMMvhlItemUsrPermitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrPermitDlg)
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB, m_wndCodeName);
	DDX_Control(pDX, IDC_CMD_AXLE_NAME_EDT, m_edtAxleName);
	DDX_Control(pDX, IDC_CMD_AXLE_NAME_LST, m_lstAxleName);
	DDX_Control(pDX, IDC_CMD_AXLE_SYMM_CHK, m_chkAxleSymm);
	DDX_Control(pDX, IDC_CMD_AXLE_EVEN_CHK, m_chkAxleEven);
	DDX_Control(pDX, IDC_CMD_VEHICLE_NAME, m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_IMPACTFACTOR_EDT, m_edtImpactFactor);
	DDX_Control(pDX, IDC_CMD_MVHL_PERMITSTDNAME_CMB, m_cmbPermitStdName);
	DDX_Control(pDX, IDC_CMD_MVHL_PERMITLOADNAME_CMB, m_cmbPermitLoadName);

	DDX_Control(pDX, IDC_CMD_AXLE_TYPE_GRD, m_grdAxleType);
	DDX_Control(pDX, IDC_CMD_AXLE_INFO_GRD, m_grdPermitLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_PIC1, m_wndPicture);
	DDX_Control(pDX, IDC_CMD_MVHL_PIC2, m_wndPicture2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUsrPermitDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrPermitDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB, OnSelchangeCmdMvhlCodeCmb)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_TRK_RD,  OnCmdMlMvhlItemuTrkRd)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LEGAL_RD,  OnCmdMlMvhlItemuTrkRd)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_TRN_RD,  OnCmdMlMvhlItemuTrkRd)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_CRD_RD,  OnCmdMlMvhlItemuTrkRd)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_BS_RDO1, OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_BS_RDO2, OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_BS_RDO3, OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_BS_RDO4, OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_BS_RDO5, OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_BS_RDO6, OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_BS_RDO7, OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO, OnCmdMlMvhlItemuEuroBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_RDO, OnCmdMlMvhlItemuEuroBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_RDO, OnCmdMlMvhlItemuEuroBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_RDO, OnCmdMlMvhlItemuEuroBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_FLM3_RDO, OnCmdMlMvhlItemuEuroBSRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_EURO_BS_RDO, OnCmdMlMvhlItemuEuroBSRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_NA_RDO,     OnCmdMlMvhlItemuSouthAfricaRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_NB_RDO,     OnCmdMlMvhlItemuSouthAfricaRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_NC_RDO,     OnCmdMlMvhlItemuSouthAfricaRdo)
	//ON_BN_CLICKED(IDC_CMD_MVHL_NBNC_RDO,   OnCmdMlMvhlItemuSouthAfricaRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_PERMIT_RDO, OnCmdMlMvhlItemuSouthAfricaRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_WHEELED_TRACKED_RDO, OnCmdMlMvhlItemuIndiaRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_WHEELED_RDO,         OnCmdMlMvhlItemuIndiaRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_INDIA_PERMIT_RDO,    OnCmdMlMvhlItemuIndiaRdo)

	ON_BN_CLICKED(IDC_CMD_USER_TRK_RDO,    OnCmdMlMvhlItemuASRdo)
	ON_BN_CLICKED(IDC_CMD_USER_TRN_RDO,    OnCmdMlMvhlItemuASRdo)
	ON_BN_CLICKED(IDC_CMD_USER_150LA_RDO,  OnCmdMlMvhlItemuASRdo)
	ON_BN_CLICKED(IDC_CMD_USER_PERMIT_RDO, OnCmdMlMvhlItemuASRdo)

	ON_BN_CLICKED(IDC_CMD_AXLE_EVEN_CHK, OnCmdAxleEvenChk)
	ON_BN_CLICKED(IDC_CMD_AXLE_SYMM_CHK, OnCmdAxleSymmChk)
	ON_BN_CLICKED(IDC_CMD_ADD_BTN, OnCmdAddBtn)
	ON_BN_CLICKED(IDC_CMD_MODIFY_BTN, OnCmdModifyBtn)
	ON_BN_CLICKED(IDC_CMD_DELETE_BTN, OnCmdDeleteBtn)
	ON_LBN_SELCHANGE(IDC_CMD_AXLE_NAME_LST, OnSelchangeCmdAxleNameLst)
	ON_LBN_SELCHANGE(IDC_CMD_MVHL_PERMITSTDNAME_CMB, OnSelChangeCmbPermitStdName)
	ON_LBN_SELCHANGE(IDC_CMD_MVHL_PERMITLOADNAME_CMB, OnSelChangeCmbPermitLoadName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrPermitDlg message handlers

BOOL CCMMvhlItemUsrPermitDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\mvhl_permit1.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\mvhl_permit2.svg"));

	if(m_nMovingType == D_MOVE_CODE_EURO_BS)
	{
		CString strCodeName[] = { _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE), _LS(IDS_CMD_MVHL_EURO_BS_TRAIL_LOAD) };

		int nCodeSeq[2];
		nCodeSeq[0] = D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE;
		nCodeSeq[1] = D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD;

		for(int i = 0; i < 2; i++) 
		{
			CDlgUtil::CobxAddItem(m_wndCodeName, strCodeName[nCodeSeq[i]], nCodeSeq[i]);
		}
		m_wndCodeName.SetCurSel(0);
	}
	else if(m_nMovingType == D_MOVE_CODE_INDIA)
	{
		CString strCodeName[] = { _LS(IDS_CMD_MVHL_IRC6_2000), _LS(IDS_CMD_MVHL_IRS_BRIDGE_RULES) };

		int nCodeSeq[2];
		nCodeSeq[0] = D_MVHL_INDIA_IRC6_2000;
		nCodeSeq[1] = D_MVHL_INDIA_IRS_BRIDGE_RULES;

		for(int i = 0; i < 2; i++) 
		{
			CDlgUtil::CobxAddItem(m_wndCodeName, strCodeName[nCodeSeq[i]], nCodeSeq[i]);
		}
		m_wndCodeName.SetCurSel(0);
	}  

	// TODO: Add extra initialization here
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->SetWindowText(m_paRadioText[0]);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->SetWindowText(m_paRadioText[1]);
	if (m_nMovingType == D_MOVE_CODE_NEWZEALAND || m_nMovingType == D_MOVE_CODE_BRAZIL)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLoadType, 2);
	}
	else
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_CRD_RD)->SetWindowText(m_paRadioText[2]);
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLoadType, 3);
	}


	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlBSLoadType,          6);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEuroBSLoadType,      5);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlSouthAfricaLoadType, 3);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIndiaType,           2);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlASLoadType,          3);

	m_aCtrlVehiProp.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlVehiProp, IDC_CMD_MVHL_ITEMU_VEHI_PROP_GRP, TRUE);

	InitGrid();
	AlignControl();

	Data2Dlg();
	ShowWindow(SW_SHOW);
	GetParent()->ShowWindow(SW_HIDE);

	ShowHideControls();

	if(m_nMovingType == D_MOVE_CODE_CANADA)
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->EnableWindow(FALSE);
	}

	if(m_nMovingType == D_MOVE_CODE_INDIA)
	{
		GetDlgItem(IDC_CMD_MVHL_IMPACTFACTOR_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_MVHL_IMPACTFACTOR_EDT)->EnableWindow(FALSE);
	}

	InitCombo();

	ResizeDialog();

	GetDlgItem(IDC_CMD_MVHL_NBNC_RDO)->ShowWindow(SW_HIDE); // 지원하지 않음
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrPermitDlg::InitGrid()
{
	// grid 초기화 
	m_grdAxleType.Initialize();
	
	m_grdPermitLoad.Initialize(m_pData);
	m_grdPermitLoad.SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);
	m_grdPermitLoad.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
}

void CCMMvhlItemUsrPermitDlg::InitCombo()
{
	m_cmbPermitStdName.ResetContent();
	m_cmbPermitStdName.AddString(_LS(IDS_WG_CMD__ADDD__User_Defined));
	m_cmbPermitStdName.AddString(_LS(IDS_CMD_MVHL_CALTRANS_2017));
	m_cmbPermitStdName.AddString(_LS(IDS_CMD_MVHL_CALTRANS));
	m_cmbPermitStdName.AddString(_LS(IDS_CMD_MVHL_IRC6_2000));
	m_cmbPermitStdName.AddString(_LS(IDS_CMD_MVHL_IADOT_LOAD));
	m_cmbPermitStdName.AddString(_LS(IDS_CMD_MVHL_ALASKA_LOAD));
	m_cmbPermitStdName.AddString(_LS(IDS_CMD_MVHL_KANSAS_LOAD));
	m_cmbPermitStdName.AddString(_LS(IDS_CMD_MVHL_MODOT_LOAD));
	m_cmbPermitStdName.SetCurSel(0);

	m_cmbPermitLoadName.ResetContent();
	m_cmbPermitLoadName.AddString(_LS(IDS_WG_CMD__ADDD__User_Defined));
	
	m_cmbPermitLoadName.SetCurSel(0);

	AdjustComboListBox(m_cmbPermitStdName);
	AdjustComboListBox(m_cmbPermitLoadName);
}

void CCMMvhlItemUsrPermitDlg::ChangeCombo()
{
	m_cmbPermitLoadName.ResetContent();

	int nStd = m_cmbPermitStdName.GetCurSel();

	if(nStd == 0)
	{
		m_cmbPermitLoadName.AddString(_LS(IDS_WG_CMD__ADDD__User_Defined));
	}
	else if(nStd == 1 || nStd == 2)
	{
		m_cmbPermitLoadName.AddString(_T("P5-BONUS"));
		m_cmbPermitLoadName.AddString(_T("P7-BONUS"));
		m_cmbPermitLoadName.AddString(_T("P9-BONUS"));
		m_cmbPermitLoadName.AddString(_T("P11-BONUS"));
		m_cmbPermitLoadName.AddString(_T("P13-BONUS"));
		m_cmbPermitLoadName.AddString(_T("P15-BONUS"));
		m_cmbPermitLoadName.AddString(_T("P5-BONUS-VS"));
		m_cmbPermitLoadName.AddString(_T("P7-BONUS-VS"));
		m_cmbPermitLoadName.AddString(_T("P9-BONUS-VS"));
		m_cmbPermitLoadName.AddString(_T("P11-BONUS-VS"));
		m_cmbPermitLoadName.AddString(_T("P13-BONUS-VS"));
		m_cmbPermitLoadName.AddString(_T("P15-BONUS-VS"));
	}
	else if(nStd == 3)
	{
		m_cmbPermitLoadName.AddString(_T("IRC Class Special Vehicle (SV)"));
	}
	else if(nStd == 4)
	{
		m_cmbPermitLoadName.AddString(_T("IADOT-SPV-Type 4"));
		m_cmbPermitLoadName.AddString(_T("IADOT-SPV-Type 353A"));
		m_cmbPermitLoadName.AddString(_T("IADOT-SPV-Type 3-3"));
		m_cmbPermitLoadName.AddString(_T("IADOT-SPV-Type 353B"));
		m_cmbPermitLoadName.AddString(_T("IADOT-SPV-Type 453"));
	}
	else if (nStd == 5)
	{
		m_cmbPermitLoadName.AddString(_T("Barnhart"));
		m_cmbPermitLoadName.AddString(_T("Mammoet"));
	}
	else if (nStd == 6)
	{
		m_cmbPermitLoadName.AddString(_LS(IDS_WG_CMD_Heavy_Equipment_Transporter_EMPTY_LOAD));
		m_cmbPermitLoadName.AddString(_LS(IDS_WG_CMD_Heavy_Equipment_Transporter_64_5_ton));
	}
	else if (nStd == 7)
	{
		m_cmbPermitLoadName.AddString(_T("Trunion 17 Axles"));
	}
	
	m_cmbPermitLoadName.SetCurSel(0);

	AdjustComboListBox(m_cmbPermitStdName);
	AdjustComboListBox(m_cmbPermitLoadName);
}

void CCMMvhlItemUsrPermitDlg::AlignControl() 
{
	CRect rRef;
	CRect rMove;
	int nDistX = 0;
	int nDistY = 0;

	CArray<UINT, UINT> aControls;

	/////////////////////////////////////////////////////////////////
	if(m_nMovingType!=D_MOVE_CODE_EURO_BS && m_nMovingType!=D_MOVE_CODE_INDIA)
	{
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_MVHL_ITEMU_LOAD_TYPE_GRP);
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_TRK_RD);
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_LEGAL_RD);    
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_TRN_RD);
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_CRD_RD);
		
		GetDlgItem(IDC_CMD_MVHL_CODE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_ITEMU_LOAD_TYPE_GRP)->GetWindowRect(rMove);
		nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}

	if(m_nMovingType!=D_MOVE_CODE_AASHTO_LRFD)
	{
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_TRN_RD);
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_CRD_RD);

		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_RD)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->GetWindowRect(rMove);
		nDistX = rRef.left - rMove.left;
		CDlgUtil::CtrlMoveDistX(this, aControls, nDistX);
	}


	/////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO1)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlBSLoadType, nDistX, nDistY);

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlEuroBSLoadType, nDistX, nDistY);

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_NA_RDO         )->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlSouthAfricaLoadType, nDistX, nDistY);

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD    )->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_WHEELED_TRACKED_RDO)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlIndiaType, nDistX, nDistY);

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_USER_TRK_RDO)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlASLoadType, nDistX, nDistY);
}

void CCMMvhlItemUsrPermitDlg::ShowHideControls() 
{
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->ShowWindow(m_nMovingType!=D_MOVE_CODE_BS&&m_nMovingType!=D_MOVE_CODE_EURO_BS&&m_nMovingType!=D_MOVE_CODE_SOUTH_AFRICA&&m_nMovingType!=D_MOVE_CODE_INDIA&&m_nMovingType!=D_MOVE_CODE_AUSTRALIA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_RD)->ShowWindow(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->ShowWindow(m_nMovingType!=D_MOVE_CODE_BS&&m_nMovingType!=D_MOVE_CODE_EURO_BS&&m_nMovingType!=D_MOVE_CODE_SOUTH_AFRICA&&m_nMovingType!=D_MOVE_CODE_INDIA&&m_nMovingType!=D_MOVE_CODE_AUSTRALIA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_CRD_RD)->ShowWindow(m_nMovingType!=D_MOVE_CODE_BS&&m_nMovingType!=D_MOVE_CODE_EURO_BS&&m_nMovingType!=D_MOVE_CODE_SOUTH_AFRICA&&m_nMovingType!=D_MOVE_CODE_INDIA&&
																		m_nMovingType!=D_MOVE_CODE_AUSTRALIA&&m_nMovingType!=D_MOVE_CODE_NEWZEALAND&&m_nMovingType!= D_MOVE_CODE_BRAZIL);

	GetDlgItem(IDC_CMD_MVHL_CODE_GRP)->ShowWindow(m_nMovingType==D_MOVE_CODE_EURO_BS||m_nMovingType==D_MOVE_CODE_INDIA);
	GetDlgItem(IDC_CMD_MVHL_CODE_CMB)->ShowWindow(m_nMovingType==D_MOVE_CODE_EURO_BS||m_nMovingType==D_MOVE_CODE_INDIA);

	CDlgUtil::CtrlShowHide(this, m_aCtrlBSLoadType,          m_nMovingType==D_MOVE_CODE_BS);
	CDlgUtil::CtrlShowHide(this, m_aCtrlEuroBSLoadType,      m_nMovingType==D_MOVE_CODE_EURO_BS);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSouthAfricaLoadType, m_nMovingType==D_MOVE_CODE_SOUTH_AFRICA);
	CDlgUtil::CtrlShowHide(this, m_aCtrlIndiaType,           m_nMovingType==D_MOVE_CODE_INDIA);
	CDlgUtil::CtrlShowHide(this, m_aCtrlASLoadType,          m_nMovingType==D_MOVE_CODE_AUSTRALIA);

}

void CCMMvhlItemUsrPermitDlg::ResizeDialog()
{
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;

	// Load Type Group Box
	if(m_nMovingType==D_MOVE_CODE_BS)
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO7)->GetWindowRect(rRef);
	}
	else if(m_nMovingType==D_MOVE_CODE_EURO_BS)
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_EURO_BS_RDO)->GetWindowRect(rRef);
	}
	else if(m_nMovingType==D_MOVE_CODE_SOUTH_AFRICA)
	{
		GetDlgItem(IDC_CMD_MVHL_PERMIT_RDO)->GetWindowRect(rRef);
	}
	else if(m_nMovingType==D_MOVE_CODE_INDIA)
	{
		GetDlgItem(IDC_CMD_MVHL_INDIA_PERMIT_RDO)->GetWindowRect(rRef);
	}
	else if(m_nMovingType==D_MOVE_CODE_AUSTRALIA)
	{
		GetDlgItem(IDC_CMD_USER_PERMIT_RDO)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->GetWindowRect(rRef);
	}
	
	GetDlgItem(IDC_CMD_MVHL_ITEMU_LOAD_TYPE_GRP)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rToMove);
	GetDlgItem(IDC_CMD_MVHL_ITEMU_LOAD_TYPE_GRP)->MoveWindow(rToMove);

	// Vehicular Load Properties
	GetDlgItem(IDC_CMD_MVHL_ITEMU_LOAD_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_ITEMU_VEHI_PROP_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlVehiProp, nDistY);

	// etc
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_PERMITSTDNAME_CMB);
	aControls.Add(IDC_CMD_MVHL_PERMITLOADNAME_CMB);
	aControls.Add(IDOK);
	aControls.Add(IDCANCEL);
	aControls.Add(IDC_CMD_APPLY);
	
	GetDlgItem(IDC_CMD_MVHL_ITEMU_VEHI_PROP_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(44);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	Invalidate();
}

void CCMMvhlItemUsrPermitDlg::AdjustComboListBox(CComboBox& Cbx)
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = Cbx.GetDC();
	for (int i=0;i < Cbx.GetCount();i++)
	{
		Cbx.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	Cbx.ReleaseDC(pDC);

	if (Cbx.GetDroppedWidth() < dx)
	{
		Cbx.SetDroppedWidth(dx);
		ASSERT(Cbx.GetDroppedWidth() == dx);
	}
}


void CCMMvhlItemUsrPermitDlg::OnOK()
{
	// TODO: Add extra validation here
	if (!Apply()) return;
	CDialogMove::OnOK();
}

void CCMMvhlItemUsrPermitDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	Apply();
}

void CCMMvhlItemUsrPermitDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMMvhlItemUsrPermitDlg::OnCmdMlMvhlItemuTrkRd()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLoadType, m_nType);
	if (m_nType == 0 || m_nType == 1 || m_nType == 2)
	{
		m_nNextAction = CHANGE_TYPE;
		CDialogMove::OnOK();
		GetWindowRect(m_WindowPos);
		m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
	}
}

void CCMMvhlItemUsrPermitDlg::OnCmdMlMvhlItemuBSRdo()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBSLoadType, m_nBSType);
	if (m_nBSType != 6)
	{
		m_nNextAction = CHANGE_TYPE;
		CDialogMove::OnOK();
		GetWindowRect(m_WindowPos);
		m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
	}
}

void CCMMvhlItemUsrPermitDlg::OnCmdMlMvhlItemuEuroBSRdo()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEuroBSLoadType, m_nEuroBSType);
	if (m_nEuroBSType != 5)
	{
		m_nNextAction = CHANGE_TYPE;
		CDialogMove::OnOK();
		GetWindowRect(m_WindowPos);
		m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
	}
}


void CCMMvhlItemUsrPermitDlg::OnCmdMlMvhlItemuSouthAfricaRdo()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSouthAfricaLoadType, m_nSouthAfricaType);
	if (m_nSouthAfricaType != 3)
	{
		m_nNextAction = CHANGE_TYPE;
		CDialogMove::OnOK();
		GetWindowRect(m_WindowPos);
		m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
	}
}

void CCMMvhlItemUsrPermitDlg::OnCmdMlMvhlItemuIndiaRdo()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIndiaType, m_nIndiaType);
	if (m_nIndiaType != 2)
	{
		m_nNextAction = CHANGE_TYPE;
		CDialogMove::OnOK();
		GetWindowRect(m_WindowPos);
		m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
	}
}

void CCMMvhlItemUsrPermitDlg::OnCmdMlMvhlItemuASRdo()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlASLoadType, m_nASType);
	if (m_nASType != 3)
	{
		m_nNextAction = CHANGE_TYPE;
		CDialogMove::OnOK();
		GetWindowRect(m_WindowPos);
		m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
	}
}

void CCMMvhlItemUsrPermitDlg::OnSelchangeCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel());

	if(m_nMovingType == D_MOVE_CODE_EURO_BS)
	{
		if(nCodeType!=D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE)
		{
			m_nCodeType = D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD;
			m_nNextAction = CHANGE_TYPE;
			CDialogMove::OnOK();
			GetWindowRect(m_WindowPos);
			m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
		}
	}
	else if(m_nMovingType == D_MOVE_CODE_INDIA)
	{
		if(nCodeType!=D_MVHL_INDIA_IRC6_2000)
		{
			m_nCodeType = D_MVHL_INDIA_IRS_BRIDGE_RULES;
			m_nNextAction = CHANGE_TYPE;
			CDialogMove::OnOK();
			GetWindowRect(m_WindowPos);
			m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
		}
	}
}

void CCMMvhlItemUsrPermitDlg::Data2Dlg()
{
	m_edtVehicleName.SetWindowText(m_pData->VehicleLoadName); 
	m_edtImpactFactor.SetEditUnit(m_pData->PermitLoad.dImpactFactor);
		
	m_lstAxleName.ResetContent();
	for (int i = 0; i < m_pData->PermitLoad.aAxleType.GetSize(); i++)
	{
		m_lstAxleName.AddString(m_pData->PermitLoad.aAxleType[i].strAxleType);
	}
	m_grdPermitLoad.MakeItemEx();

	m_chkAxleEven.SetCheck(FALSE);
	m_chkAxleSymm.SetCheck(TRUE);
	if (m_lstAxleName.GetCount() > 0) 
	{
		m_lstAxleName.SetCurSel(0);
		AxleType2Dlg(&m_pData->PermitLoad.aAxleType[0]);
	}
}

BOOL CCMMvhlItemUsrPermitDlg::Dlg2Data()
{
	if (!m_grdPermitLoad.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;
	T_MVHL_PERMIT_LOAD PermitLoad = m_pData->PermitLoad;
	m_pData->Initialize();
	m_pData->PermitLoad = PermitLoad;

	if     (m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)  m_pData->nStandardCode = D_MVHL_AASHTO_LRFD;
	else if(m_nMovingType == D_MOVE_CODE_CANADA)       m_pData->nStandardCode = D_MVHL_CANCSA_S600;
	else if(m_nMovingType == D_MOVE_CODE_AUSTRALIA)    m_pData->nStandardCode = D_MVHL_AUSTRALIA_AS_5100_2;
	else if(m_nMovingType == D_MOVE_CODE_BS)           m_pData->nStandardCode = D_MVHL_BS_BD3701;
	else if(m_nMovingType == D_MOVE_CODE_EURO_BS)      m_pData->nStandardCode = D_MVHL_EURO_EN1991_2_2003_ROAD_BRG;
	else if(m_nMovingType == D_MOVE_CODE_INDIA)        m_pData->nStandardCode = D_MVHL_IRC6_2000;
	else if(m_nMovingType == D_MOVE_CODE_POLAND)       m_pData->nStandardCode = D_MVHL_PN_85S_10030_ROAD_BRG;
	else if(m_nMovingType == D_MOVE_CODE_SOUTH_AFRICA) m_pData->nStandardCode = D_MVHL_SOUTH_AFRICA_TMH7_1981;
	else if(m_nMovingType == D_MOVE_CODE_NEWZEALAND)   m_pData->nStandardCode = D_MVHL_TRAFFIC_LOAD_NZ;
	else if(m_nMovingType == D_MOVE_CODE_BRAZIL)       m_pData->nStandardCode = D_MVHL_NBR7188_FOOTWAY;
	else ASSERT(0);

	m_pData->nLoadType = 2;       // Permit Load
	if     (m_nMovingType == D_MOVE_CODE_BS          ) m_pData->nSubType = 6;
	else if(m_nMovingType == D_MOVE_CODE_EURO_BS     ) m_pData->nSubType = 5;
	else if(m_nMovingType == D_MOVE_CODE_SOUTH_AFRICA) m_pData->nSubType = 3;
	m_pData->bStandard = FALSE;
	m_edtVehicleName.GetWindowText(m_pData->VehicleLoadName);
	m_pData->PermitLoad.dImpactFactor = m_edtImpactFactor.GetEditValue();
	return TRUE;
}

BOOL CCMMvhlItemUsrPermitDlg::Apply()
{
	if (!Dlg2Data()) return FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess = FALSE;
	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, *m_pData);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(*m_pData);
	if (!bSuccess) return FALSE;
	return TRUE;
}

void CCMMvhlItemUsrPermitDlg::OnCmdAxleEvenChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bEnable = TRUE;
	if (m_chkAxleEven.GetCheck() != 0) bEnable = FALSE;
	CString csValue;
	for (int i = 1; i < D_MAX_WHEEL_LINE; i++)
	{
		if (!bEnable) 
		{
			csValue = _T("");
			m_grdAxleType.SetValueRange(CGXRange(1, i*2+1), csValue);
			m_grdAxleType.SetStyleRange(CGXRange().SetCols(i*2+1), CGXStyle().SetEnabled(bEnable)); 
		}
		else
		{
			if (m_grdAxleType.GetValueRowCol(1, i*2+2) == _T("")) csValue = _T("");
			else csValue = m_grdAxleType.GetValueRowCol(1,1);
			m_grdAxleType.SetStyleRange(CGXRange().SetCols(i*2+1), CGXStyle().SetEnabled(bEnable));
			m_grdAxleType.SetValueRange(CGXRange(1, i*2+1), csValue);
		}
	}
}

void CCMMvhlItemUsrPermitDlg::OnCmdAxleSymmChk() 
{
	// TODO: Add your control notification handler code here
}

void CCMMvhlItemUsrPermitDlg::OnCmdAddBtn() 
{
	// TODO: Add your control notification handler code here
	T_MVHL_AXLE_TYPE AxleType;
	if (!Dlg2AxleType(&AxleType)) return;
	for (int i = 0; i < m_pData->PermitLoad.aAxleType.GetSize(); i++)
	{
		if (m_pData->PermitLoad.aAxleType[i].strAxleType.CompareNoCase(AxleType.strAxleType) == 0)
		{
			AfxMessageBox(_LS(IDS_CMD_SAME_NAME_EXIST));
			return;
		}
	}
	m_lstAxleName.AddString(AxleType.strAxleType);
	m_lstAxleName.SetCurSel(m_lstAxleName.GetCount()-1);
	m_pData->PermitLoad.aAxleType.Add(AxleType);
	m_grdPermitLoad.UpdateAxleTypeColumn();
}

void CCMMvhlItemUsrPermitDlg::OnCmdModifyBtn() 
{
	// TODO: Add your control notification handler code here
	int ix = m_lstAxleName.GetCurSel();
	if (ix == LB_ERR) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	// Permit Load 먼저 처리
	if (m_grdPermitLoad.IsEdit())
	{
		if (!m_grdPermitLoad.OnTerminate(D_TB_SAVE_ALWAYS))
			m_grdPermitLoad.CancelEdit();
	}

	T_MVHL_AXLE_TYPE AxleType;
	if (!Dlg2AxleType(&AxleType)) return;

	for (int i = 0; i < m_pData->PermitLoad.aAxleType.GetSize(); i++)
	{
		if (i == ix) continue;
		if (m_pData->PermitLoad.aAxleType[i].strAxleType.CompareNoCase(AxleType.strAxleType) == 0)
		{
			AfxMessageBox(_LS(IDS_CMD_SAME_NAME_EXIST));
			return;
		}
	}

	m_lstAxleName.InsertString(ix, AxleType.strAxleType);
	m_lstAxleName.DeleteString(ix+1);
	m_lstAxleName.SetCurSel(ix);

	m_pData->PermitLoad.aAxleType[ix] = AxleType;

	// Permit Load Table Update
	m_grdPermitLoad.LockUpdate(TRUE);
	m_grdPermitLoad.UpdateAxleTypeColumn();
	m_grdPermitLoad.MakeItemEx(0, -2, TRUE);
	m_grdPermitLoad.LockUpdate(FALSE);
	m_grdPermitLoad.Redraw();
}

void CCMMvhlItemUsrPermitDlg::OnCmdDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	int ix = m_lstAxleName.GetCurSel();
	if (ix == LB_ERR)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	// Permit Load 먼저 처리
	if (m_grdPermitLoad.IsEdit())
	{
		if (!m_grdPermitLoad.OnTerminate(D_TB_SAVE_ALWAYS))
			m_grdPermitLoad.CancelEdit();
	}

	m_pData->PermitLoad.aAxleType.RemoveAt(ix);

	int nNewIx = ix;
	m_lstAxleName.DeleteString(ix);
	int nCount = m_lstAxleName.GetCount();
	if (nCount > 0)
	{
		if (ix >= nCount) nNewIx = ix-1;
		m_lstAxleName.SetCurSel(nNewIx);
		AxleType2Dlg(&m_pData->PermitLoad.aAxleType[nNewIx]);
	}
	

	CArray<int, int> aKey;
	for (int i= 0; i < m_pData->PermitLoad.aPermitLoad.GetSize(); i++)
		if (m_pData->PermitLoad.aPermitLoad[i].nAxleType == ix)
			aKey.Add(i);
	m_grdPermitLoad.LockUpdate(TRUE);
	if (aKey.GetSize() > 0) m_grdPermitLoad.DelData(aKey, TRUE);
	m_grdPermitLoad.UpdateAxleTypeColumn();
	m_grdPermitLoad.LockUpdate(FALSE);
	m_grdPermitLoad.Redraw();
}

void CCMMvhlItemUsrPermitDlg::OnSelchangeCmdAxleNameLst() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_lstAxleName.GetCurSel();
	AxleType2Dlg(&m_pData->PermitLoad.aAxleType[nSel]);
}

void CCMMvhlItemUsrPermitDlg::AxleType2Dlg(T_MVHL_AXLE_TYPE* pData)
{
	m_edtAxleName.SetWindowText(pData->strAxleType);
	m_chkAxleEven.SetCheck(pData->bEvenlyDistLoad ? 1 : 0);
	m_chkAxleSymm.SetCheck(pData->bSymmetricVehicle ? 1 : 0);
	OnCmdAxleEvenChk();
	CString csP, csD;
	m_grdAxleType.LockUpdate(TRUE);
	for (int i = 0; i < D_MAX_WHEEL_LINE; i++)
	{
		if (i < pData->aLoad.GetSize())
		{
			csP.Format(_T("%g"), pData->aLoad[i]);
			csD.Format(_T("%g"), pData->aDist[i]);
		}
		else csP = csD = _T("");
		m_grdAxleType.SetValueRange(CGXRange(1, i*2+1), csP);
		m_grdAxleType.SetValueRange(CGXRange(1, i*2+2), csD);
	}
	m_grdAxleType.LockUpdate(FALSE);
	m_grdAxleType.Redraw();
}

BOOL CCMMvhlItemUsrPermitDlg::Dlg2AxleType(T_MVHL_AXLE_TYPE* pData)
{
	pData->Initialize();
	m_edtAxleName.GetWindowText(pData->strAxleType);
	pData->strAxleType.TrimLeft();
	pData->strAxleType.TrimRight();
	if (pData->strAxleType == _T("")) 
	{
		CString csMsg;
		csMsg.Format(_LS(IDS_CMD_ERROR_NOT_VALID), _LS(IDS_CMD_NAME));
		AfxMessageBox(csMsg);
		return FALSE;
	}
	pData->bEvenlyDistLoad = m_chkAxleEven.GetCheck() != 0 ? TRUE : FALSE;
	pData->bSymmetricVehicle = m_chkAxleSymm.GetCheck() != 0 ? TRUE : FALSE;

	CString csP, csD;
	for (int i = 0; i < D_MAX_WHEEL_LINE; i++)
	{
		if (pData->bEvenlyDistLoad) csP = m_grdAxleType.GetValueRowCol(1, 1);
		else csP = m_grdAxleType.GetValueRowCol(1, i*2+1);
		csD = m_grdAxleType.GetValueRowCol(1, i*2+2);
		csD.TrimLeft(); csD.TrimRight();
		if (csD == _T("")) break;
		if (pData->bSymmetricVehicle && _tstof(csD) <= 0.0)
		{
			CString csMsg;
			csMsg.Format(_LS(IDS_CMD_MVHL_ERROR_POSITIVE_VALUE), i+1);
			AfxMessageBox(csMsg);
			return FALSE;
		}
		pData->aLoad.Add(_tstof(csP));
		pData->aDist.Add(_tstof(csD));
	}
	return TRUE;
}

void CCMMvhlItemUsrPermitDlg::OnSelChangeCmbPermitStdName()
{
	ChangeCombo();
	OnSelChangeCmbPermitLoadName();
}

void CCMMvhlItemUsrPermitDlg::OnSelChangeCmbPermitLoadName()
{
	int nStd = m_cmbPermitStdName.GetCurSel();
	int nLoad =m_cmbPermitLoadName.GetCurSel();
	
	////////
	T_MVHL_PERMIT_LOAD PermitLoad;
	InitWithBonusVehicle(nStd, nLoad, PermitLoad);
	m_pData->PermitLoad = PermitLoad;

	// Redraw Grid
	m_grdPermitLoad.UpdateAxleTypeColumn();
	Data2Dlg();
 
	return;
}

// nIdx - BONUS:0-5, BONUS VS: 6-11
void CCMMvhlItemUsrPermitDlg::InitWithBonusVehicle(int nStd, int nLoad, T_MVHL_PERMIT_LOAD& PermitLoad)
{
	PermitLoad.Initialize();
	int ind = 0, inf = 0;

	if(nStd == 0)
	{
		PermitLoad.aPermitLoad.RemoveAll();
	}
	else if(nStd == 1 || nStd == 2)
	{
		// a1, a2, a3 만들어서 채우기
		T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

		Axle.strAxleType = _T("a1");
		Axle.bEvenlyDistLoad = FALSE;
		Axle.bSymmetricVehicle = TRUE;
		Axle.aLoad.RemoveAll();
		Axle.aLoad.Add(13.);
		Axle.aDist.RemoveAll();
		Axle.aDist.Add(3.);
		PermitLoad.aAxleType.Add(Axle); // add

		Axle.strAxleType = _T("a2");
		Axle.aLoad.RemoveAll();
		if(nStd==1) Axle.aLoad.Add(27.);
		else        Axle.aLoad.Add(24.);
		Axle.aDist.RemoveAll();
		Axle.aDist.Add(3.);
		PermitLoad.aAxleType.Add(Axle); // add

		Axle.strAxleType = _T("a3");
		Axle.aLoad.RemoveAll();
		Axle.aLoad.Add(7.5); Axle.aLoad.Add(7.5); Axle.aLoad.Add(7.5); Axle.aLoad.Add(7.5);
		Axle.aDist.RemoveAll();
		Axle.aDist.Add(0.64); Axle.aDist.Add(1.93); Axle.aDist.Add(3.21); Axle.aDist.Add(4.5);
		PermitLoad.aAxleType.Add(Axle); // add

		// aPermitLoad 채우기
		int nVSIdx=0;
		T_MVHL_PERMIT_ITEM a1, a2, a3;
		a1.nAxleType=0; a1.dSpacing=18.;
		a2.nAxleType=1; a2.dSpacing=18.;
		a3.nAxleType=2; a3.dSpacing=18.;

		PermitLoad.aPermitLoad.Add(a1);
		PermitLoad.aPermitLoad.Add(a2);
		PermitLoad.aPermitLoad.Add(a3);

		if(nLoad==0 || nLoad==6)
		{
			nVSIdx = 2;
		}
		if(nLoad==1 || nLoad==7)
		{
			nVSIdx = 2;
			PermitLoad.aPermitLoad.Add(a3);
		}
		if(nLoad==2 || nLoad==8)
		{
			nVSIdx = 3;
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
		}
		if(nLoad==3 || nLoad==9)
		{
			nVSIdx = 4;
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
		}
		if(nLoad==4 || nLoad==10)
		{
			nVSIdx = 4;
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
		}
		if(nLoad==5 || nLoad==11)
		{
			nVSIdx = 4;
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a3);
		}

		// VS
		if(nLoad >= 6)
		{
			if(nStd==1) PermitLoad.aPermitLoad[nVSIdx].dSpacing = 60.;
			else        PermitLoad.aPermitLoad[nVSIdx].dSpacing = 40.;
			PermitLoad.nVS_Idx = nVSIdx+1;
		}
		ind=D_UNITSYS_LENGTH_INDEX_FT;
		inf=D_UNITSYS_FORCE_INDEX_KIP;
	}
	else if(nStd == 3)
	{
		// a1, a2, a3 만들어서 채우기
		T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

		Axle.strAxleType = _T("a1");
		Axle.bEvenlyDistLoad = FALSE;
		Axle.bSymmetricVehicle = TRUE;
		Axle.aLoad.RemoveAll();
		Axle.aLoad.Add(3.0);
		Axle.aDist.RemoveAll();
		Axle.aDist.Add(900.0);
		PermitLoad.aAxleType.Add(Axle); // add

		Axle.strAxleType = _T("a2");
		Axle.aLoad.RemoveAll();
		Axle.aLoad.Add(4.75);
		Axle.aDist.RemoveAll();
		Axle.aDist.Add(900.0);
		PermitLoad.aAxleType.Add(Axle); // add

		Axle.strAxleType = _T("a3");
		Axle.aLoad.RemoveAll();
		Axle.aLoad.Add(2.25); Axle.aLoad.Add(2.25); Axle.aLoad.Add(2.25); Axle.aLoad.Add(2.25);
		Axle.aDist.RemoveAll();
		Axle.aDist.Add(403.0); Axle.aDist.Add(647.0); Axle.aDist.Add(1153.0); Axle.aDist.Add(1397.0);
		PermitLoad.aAxleType.Add(Axle); // add

		// aPermitLoad 채우기
		int nVSIdx=0;
		T_MVHL_PERMIT_ITEM a1, a2, a3, a4;
		a1.nAxleType=0; a1.dSpacing=3200.0;
		a2.nAxleType=1; a2.dSpacing=1370.0;
		a3.nAxleType=1; a3.dSpacing=5389.0;
		a4.nAxleType=2; a4.dSpacing=1500.0;

		PermitLoad.aPermitLoad.Add(a1);
		PermitLoad.aPermitLoad.Add(a2);
		PermitLoad.aPermitLoad.Add(a3);

		for(int i=0; i<20; ++i)
		{
			PermitLoad.aPermitLoad.Add(a4);
		}	

		ind=D_UNITSYS_LENGTH_INDEX_MM;
		inf=D_UNITSYS_FORCE_INDEX_TON;
	}
	else if(nStd == 4)
	{
		if(nLoad == 0)
		{
			// a1, a2, a3 만들어서 채우기
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.strAxleType = _T("a1");
			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(12.5);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.125);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a2");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(14.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.5);
			PermitLoad.aAxleType.Add(Axle); // add

			// aPermitLoad 채우기
			int nVSIdx=0;
			T_MVHL_PERMIT_ITEM a1, a2;
			a1.nAxleType=0; a1.dSpacing=11.0;
			a2.nAxleType=1; a2.dSpacing= 4.0;

			PermitLoad.aPermitLoad.Add(a1);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a2);

			ind=D_UNITSYS_LENGTH_INDEX_FT;
			inf=D_UNITSYS_FORCE_INDEX_KIP;
		}
		else if(nLoad == 1)
		{
			// a1, a2, a3 만들어서 채우기
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.strAxleType = _T("a1");
			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(12);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.0);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a2");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(13.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.25);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a3");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(14.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.5);
			PermitLoad.aAxleType.Add(Axle); // add

			// aPermitLoad 채우기
			int nVSIdx=0;
			T_MVHL_PERMIT_ITEM a1, a2, a3, a4;
			a1.nAxleType=0; a1.dSpacing=11.0;
			a2.nAxleType=1; a2.dSpacing= 4.0;
			a3.nAxleType=1; a3.dSpacing=20.0;
			a4.nAxleType=2; a4.dSpacing= 4.0;

			PermitLoad.aPermitLoad.Add(a1);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a4);
			PermitLoad.aPermitLoad.Add(a4);
			PermitLoad.aPermitLoad.Add(a4);

			ind=D_UNITSYS_LENGTH_INDEX_FT;
			inf=D_UNITSYS_FORCE_INDEX_KIP;
		}
		else if(nLoad == 2)
		{
			// a1, a2, a3 만들어서 채우기
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.strAxleType = _T("a1");
			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(14.5);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.625);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a2");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(12.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.0);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a3");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(13.5);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.375);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a4");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(14.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.5);
			PermitLoad.aAxleType.Add(Axle); // add

			// aPermitLoad 채우기
			int nVSIdx=0;
			T_MVHL_PERMIT_ITEM a1, a2, a3, a4, a5;
			a1.nAxleType=0; a1.dSpacing=15.0;
			a2.nAxleType=1; a2.dSpacing= 4.0;
			a3.nAxleType=1; a3.dSpacing=10.0;
			a4.nAxleType=2; a4.dSpacing=10.0;
			a5.nAxleType=3; a5.dSpacing= 4.0;

			PermitLoad.aPermitLoad.Add(a1);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a4);
			PermitLoad.aPermitLoad.Add(a5);
			PermitLoad.aPermitLoad.Add(a5);

			ind=D_UNITSYS_LENGTH_INDEX_FT;
			inf=D_UNITSYS_FORCE_INDEX_KIP;
		}
		else if(nLoad == 3)
		{
			// a1, a2, a3 만들어서 채우기
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.strAxleType = _T("a1");
			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(12.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.0);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a2");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(17.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(4.25);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a3");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(10.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(2.5);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a4");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(17.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(4.25);
			PermitLoad.aAxleType.Add(Axle); // add

			// aPermitLoad 채우기
			int nVSIdx=0;
			T_MVHL_PERMIT_ITEM a1, a2, a3, a4, a5;
			a1.nAxleType=0; a1.dSpacing=12.0;
			a2.nAxleType=1; a2.dSpacing= 4.0;
			a3.nAxleType=1; a3.dSpacing=33.0;
			a4.nAxleType=2; a4.dSpacing= 7.0;
			a5.nAxleType=3; a5.dSpacing= 4.0;

			PermitLoad.aPermitLoad.Add(a1);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a4);
			PermitLoad.aPermitLoad.Add(a5);
			PermitLoad.aPermitLoad.Add(a5);

			ind=D_UNITSYS_LENGTH_INDEX_FT;
			inf=D_UNITSYS_FORCE_INDEX_KIP;
		}
		else if(nLoad == 4)
		{
			// a1, a2, a3 만들어서 채우기
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.strAxleType = _T("a1");
			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(12.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.0);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a2");
			Axle.aLoad.RemoveAll();
			Axle.aLoad.Add(14.0);
			Axle.aDist.RemoveAll();
			Axle.aDist.Add(3.5);
			PermitLoad.aAxleType.Add(Axle); // add

			// aPermitLoad 채우기
			int nVSIdx=0;
			T_MVHL_PERMIT_ITEM a1, a2, a3;
			a1.nAxleType=0; a1.dSpacing=12.0;
			a2.nAxleType=1; a2.dSpacing= 4.0;
			a3.nAxleType=1; a3.dSpacing=34.0;

			PermitLoad.aPermitLoad.Add(a1);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a2);

			ind=D_UNITSYS_LENGTH_INDEX_FT;
			inf=D_UNITSYS_FORCE_INDEX_KIP;
		}
	}

	else if (nStd == 5)
	{
		if (nLoad == 0)
		{
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.strAxleType = _T("a1");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(30.18 * 0.25);
			Axle.aDist.Add(41.0*0.5);

			Axle.aLoad.Add(30.18 * 0.25);
			Axle.aDist.Add(118.0 * 0.5 - 8.5);

			Axle.aLoad.Add(30.18 * 0.25);
			Axle.aDist.Add(118.0 * 0.5 - 8.5);
			PermitLoad.aAxleType.Add(Axle); // add

			T_MVHL_PERMIT_ITEM a1;
			a1.nAxleType = 0; a1.dSpacing = 59.0;

			for (int i = 0; i < 18; ++i)
			{
				PermitLoad.aPermitLoad.Add(a1);
			}

			ind = D_UNITSYS_LENGTH_INDEX_IN;
			inf = D_UNITSYS_FORCE_INDEX_KIP;
		}
		else if (nLoad == 1)
		{
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			double dTmp;

			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.strAxleType = _T("a1");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(18.0 * 0.5);
			dTmp = 0.817 * 0.5 + 0.248 + 0.487 * 0.5;
			dTmp = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dTmp);
			Axle.aDist.Add(dTmp);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a2");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(23.0 * 0.5);
			Axle.aDist.Add(dTmp);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a3");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(38.47 * 0.25);
			dTmp = 0.817 * 0.5 + 0.248 * 0.5;
			dTmp = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dTmp);
			Axle.aDist.Add(dTmp);

			Axle.aLoad.Add(38.47 * 0.25);
			dTmp = 3.0 * 0.5 - 0.216;
			dTmp = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dTmp);
			Axle.aDist.Add(dTmp);

			PermitLoad.aAxleType.Add(Axle); // add

			T_MVHL_PERMIT_ITEM a1;
			a1.nAxleType = 0; a1.dSpacing = 17.75;
			PermitLoad.aPermitLoad.Add(a1);

			a1.nAxleType = 1; a1.dSpacing = 5.0;
			PermitLoad.aPermitLoad.Add(a1);

			a1.nAxleType = 1; a1.dSpacing = 18.17;
			PermitLoad.aPermitLoad.Add(a1);


			a1.nAxleType = 2; a1.dSpacing = 4.92;
			for (int i = 0; i < 15; ++i)
			{
				PermitLoad.aPermitLoad.Add(a1);
			}

			a1.nAxleType = 2; a1.dSpacing = 19.29;
			PermitLoad.aPermitLoad.Add(a1);

			a1.nAxleType = 0; a1.dSpacing = 17.75;
			PermitLoad.aPermitLoad.Add(a1);

			a1.nAxleType = 1; a1.dSpacing = 5.0;
			PermitLoad.aPermitLoad.Add(a1);
			PermitLoad.aPermitLoad.Add(a1);

			ind = D_UNITSYS_LENGTH_INDEX_FT;
			inf = D_UNITSYS_FORCE_INDEX_KIP;
		}
		else
		{
			ASSERT(0);
		}
	}
	else if (nStd == 6)
	{
		CString Tractor = _LS(IDS_WG_CMD_Tractor);
		CString SemiTrailer = _LS(IDS_WG_CMD_SemiTrailer);
		if (nLoad == 0)
		{
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.strAxleType = Tractor + _T("1");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(19.42 * 0.5);
			Axle.aDist.Add(102.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = Tractor + _T("2");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(10.75 * 0.5);
			Axle.aDist.Add(102.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = Tractor + _T("3");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(10.175 * 0.5);
			Axle.aDist.Add(102.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = Tractor + _T("4");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(10.075 * 0.5);
			Axle.aDist.Add(102.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add


			Axle.strAxleType = SemiTrailer + _T("1");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(6.45 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(6.45 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(6.45 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(6.45 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = SemiTrailer + _T("2");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(6.55 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(6.55 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(6.55 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(6.55 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = SemiTrailer + _T("3");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(6.25 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(6.25 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(6.25 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(6.25 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = SemiTrailer + _T("4");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(10.65 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(10.65 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(10.65 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(10.65 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = SemiTrailer + _T("5");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(10.45 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(10.45 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(10.45 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(10.45 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			T_MVHL_PERMIT_ITEM a1, a2, a3, a4, a5, a6, a7, a8;
			a1.nAxleType = 0; a1.dSpacing = 155.00;
			a2.nAxleType = 1; a2.dSpacing = 60.00;
			a3.nAxleType = 2; a3.dSpacing = 50.00;
			a4.nAxleType = 3; a4.dSpacing = 181.25;
			a5.nAxleType = 4; a5.dSpacing = 71.25;
			a6.nAxleType = 5; a6.dSpacing = 71.25;
			a7.nAxleType = 6; a7.dSpacing = 71.25;
			a8.nAxleType = 7; a8.dSpacing = 71.25;

			PermitLoad.aPermitLoad.Add(a1);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a4);
			PermitLoad.aPermitLoad.Add(a5);
			PermitLoad.aPermitLoad.Add(a6);
			PermitLoad.aPermitLoad.Add(a7);
			PermitLoad.aPermitLoad.Add(a8);
			PermitLoad.aPermitLoad.Add(a8);

			ind = D_UNITSYS_LENGTH_INDEX_FT;
			inf = D_UNITSYS_FORCE_INDEX_KIP;
		}
		else if (nLoad == 1)
		{
			CString Tractor = _LS(IDS_WG_CMD_Tractor);
			CString SemiTrailer = _LS(IDS_WG_CMD_SemiTrailer);

			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();

			Axle.strAxleType = Tractor + _T("1");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(21.477 * 0.5);
			Axle.aDist.Add(102.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = Tractor + _T("2");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(21.346 * 0.5);
			Axle.aDist.Add(102.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = Tractor + _T("3");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(21.163 * 0.5);
			Axle.aDist.Add(102.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = Tractor + _T("4");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(19.128 * 0.5);
			Axle.aDist.Add(102.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add


			Axle.strAxleType = SemiTrailer + _T("1");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(25.385 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(25.385 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(25.385 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(25.385 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = SemiTrailer + _T("2");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(27.835 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(27.835 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(27.835 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(27.835 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = SemiTrailer + _T("3");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(26.291 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(26.291 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(26.291 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(26.291 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = SemiTrailer + _T("4");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(27.420 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(27.420 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(27.420 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(27.420 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = SemiTrailer + _T("5");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(29.754 * 0.125);
			Axle.aDist.Add(144.0 / 14.0);

			Axle.aLoad.Add(29.754 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 1. / 7.);

			Axle.aLoad.Add(29.754 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 2. / 7.);

			Axle.aLoad.Add(29.754 * 0.125);
			Axle.aDist.Add(144.0 / 14.0 + 144.0 * 3. / 7.);
			PermitLoad.aAxleType.Add(Axle); // add

			T_MVHL_PERMIT_ITEM a1, a2, a3, a4, a5, a6, a7, a8;
			a1.nAxleType = 0; a1.dSpacing = 155.00;
			a2.nAxleType = 1; a2.dSpacing = 60.00;
			a3.nAxleType = 2; a3.dSpacing = 50.00;
			a4.nAxleType = 3; a4.dSpacing = 181.25;
			a5.nAxleType = 4; a5.dSpacing = 71.25;
			a6.nAxleType = 5; a6.dSpacing = 71.25;
			a7.nAxleType = 6; a7.dSpacing = 71.25;
			a8.nAxleType = 7; a8.dSpacing = 71.25;

			PermitLoad.aPermitLoad.Add(a1);
			PermitLoad.aPermitLoad.Add(a2);
			PermitLoad.aPermitLoad.Add(a3);
			PermitLoad.aPermitLoad.Add(a4);
			PermitLoad.aPermitLoad.Add(a5);
			PermitLoad.aPermitLoad.Add(a6);
			PermitLoad.aPermitLoad.Add(a7);
			PermitLoad.aPermitLoad.Add(a8);
			PermitLoad.aPermitLoad.Add(a8);

			ind = D_UNITSYS_LENGTH_INDEX_FT;
			inf = D_UNITSYS_FORCE_INDEX_KIP;
		}
		else
		{
			ASSERT(0);
		}
		ind = D_UNITSYS_LENGTH_INDEX_FT;
		inf = D_UNITSYS_FORCE_INDEX_KIP;
	}
	else if (nStd == 7)
	{
		if (nLoad == 0)
		{
			T_MVHL_AXLE_TYPE Axle; Axle.Initialize();

			Axle.bEvenlyDistLoad = FALSE;
			Axle.bSymmetricVehicle = TRUE;
			Axle.strAxleType = _T("a1");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(20.0 * 0.5);
			Axle.aDist.Add(72.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a2");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(38.0 * 0.25);
			Axle.aDist.Add(48.0 * 0.5);
			Axle.aLoad.Add(38.0 * 0.25);
			Axle.aDist.Add(48.0 * 0.5 + 36.0);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a3");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(39.0 * 0.25);
			Axle.aDist.Add(48.0 * 0.5);
			Axle.aLoad.Add(39.0 * 0.25);
			Axle.aDist.Add(49.0 * 0.5 + 36.0);
			PermitLoad.aAxleType.Add(Axle); // add

			Axle.strAxleType = _T("a4");
			Axle.aLoad.RemoveAll();
			Axle.aDist.RemoveAll();
			Axle.aLoad.Add(17.0 * 0.5);
			Axle.aDist.Add(72.0 * 0.5);
			PermitLoad.aAxleType.Add(Axle); // add


			T_MVHL_PERMIT_ITEM a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17;
			 a1.nAxleType = 0;  a1.dSpacing = 196.0;
			 a2.nAxleType = 0;  a2.dSpacing =  54.0;
			 a3.nAxleType = 0;  a3.dSpacing =  54.0;
			 a4.nAxleType = 0;  a4.dSpacing = 179.0;
			 a5.nAxleType = 1;  a5.dSpacing =  72.0;
			 a6.nAxleType = 1;  a6.dSpacing = 176.0;
			 a7.nAxleType = 1;  a7.dSpacing =  72.0;
			 a8.nAxleType = 1;  a8.dSpacing = 658.0;
			 a9.nAxleType = 2;  a9.dSpacing =  72.0;
			a10.nAxleType = 1; a10.dSpacing = 174.0;
			a11.nAxleType = 1; a11.dSpacing =  72.0;
			a12.nAxleType = 1; a12.dSpacing = 174.0;
			a13.nAxleType = 1; a13.dSpacing =  72.0;
			a14.nAxleType = 1; a14.dSpacing = 181.0;
			a15.nAxleType = 0; a15.dSpacing = 196.0;
			a16.nAxleType = 3; a16.dSpacing =  54.0;
			a17.nAxleType = 3; a17.dSpacing =  54.0;

			PermitLoad.aPermitLoad.Add( a1);
			PermitLoad.aPermitLoad.Add( a2);
			PermitLoad.aPermitLoad.Add( a3);
			PermitLoad.aPermitLoad.Add( a4);
			PermitLoad.aPermitLoad.Add( a5);
			PermitLoad.aPermitLoad.Add( a6);
			PermitLoad.aPermitLoad.Add( a7);
			PermitLoad.aPermitLoad.Add( a8);
			PermitLoad.aPermitLoad.Add( a9);
			PermitLoad.aPermitLoad.Add(a10);
			PermitLoad.aPermitLoad.Add(a11);
			PermitLoad.aPermitLoad.Add(a12);
			PermitLoad.aPermitLoad.Add(a13);
			PermitLoad.aPermitLoad.Add(a14);
			PermitLoad.aPermitLoad.Add(a15);
			PermitLoad.aPermitLoad.Add(a16);
			PermitLoad.aPermitLoad.Add(a17);

			ind = D_UNITSYS_LENGTH_INDEX_IN;
			inf = D_UNITSYS_FORCE_INDEX_KIP;
		}
		else
		{
			ASSERT(0);
		}
		}
	else
	{
		ASSERT(0);
	}

	T_MVHL_D DataMvhl;
	DataMvhl.Initialize();
	
	DataMvhl.PermitLoad = PermitLoad;
	
	T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	UnitIndexNew=UnitIndexOld;
	UnitIndexNew.nBase_Length=ind;
	UnitIndexNew.nBase_Force=inf;
	m_pDoc->m_pUnitCtrl->SaveUnitChangeFactor();
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew);
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld);
	
	m_pDoc->m_pUnitCtrl->ConvertUnitMvhlPrevious(DataMvhl);
	
	m_pDoc->m_pUnitCtrl->RestoreUnitChangeFactor();
	
	PermitLoad = DataMvhl.PermitLoad;
}
