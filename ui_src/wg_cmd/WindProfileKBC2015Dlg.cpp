// WindProfileKBC2015Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindProfileKBC2015Dlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\DynamicReportTemplateDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_common\wg_common_TBGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindProfileKBC2015Dlg dialog


CWindProfileKBC2015Dlg::CWindProfileKBC2015Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindProfileKBC2015Dlg::IDD, pParent), m_LateralLoad(0, TRUE)
{
	//{{AFX_DATA_INIT(CWindProfileKBC2015Dlg)
	m_nDir = 0;
	m_nComponent = 0;
	m_nProfileType = 0;
	m_strFileName = _T("");
	m_KeyWAorWT = NULL;
	//}}AFX_DATA_INIT
	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;

	m_aCtrlProfileSub.RemoveAll();
	m_aCtrlProfileSub.Add(IDC_WINDPF_PROFILE_SHEAR);
	m_aCtrlProfileSub.Add(IDC_WINDPF_PROFILE_MNT);
	m_aCtrlProfileSub.FreeExtra();

	m_aCtrlProfileSub2.RemoveAll();
	m_aCtrlProfileSub2.Add(IDC_WINDPF_PROFILE_MAX_DISP);
	m_aCtrlProfileSub2.Add(IDC_WINDPF_PROFILE_MAX_ACC);
	m_aCtrlProfileSub2.FreeExtra();

	m_pDataWind = NULL;
	m_pWnatD = NULL;
}


void CWindProfileKBC2015Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindProfileKBC2015Dlg)
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Radio(pDX, IDC_WINDPF_DIR_ALONG, m_nDir);
	DDX_Radio(pDX, IDC_WINDPF_COMP_X, m_nComponent);
	DDX_Radio(pDX, IDC_WINDPF_PROFILE_FORCE, m_nProfileType);
	DDX_Text(pDX, IDC_WINDPF_EDT_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_WINDPF_TABLE, m_WindPfGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindProfileKBC2015Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindProfileKBC2015Dlg)
	ON_BN_CLICKED(IDC_WINDPF_DIR_ALONG, OnChangeDir)
	ON_BN_CLICKED(IDC_WINDPF_DIR_ACROSS, OnChangeDir)
	ON_BN_CLICKED(IDC_WINDPF_DIR_TORSIONAL, OnChangeDir)
	ON_BN_CLICKED(IDC_WINDPF_COMP_X, OnChangeCompont)
	ON_BN_CLICKED(IDC_WINDPF_COMP_Y, OnChangeCompont)
	ON_BN_CLICKED(IDC_WINDPF_COMP_XY, OnChangeCompont)
	ON_BN_CLICKED(IDC_WINDPF_COMP_SRSS, OnChangeCompont)
	ON_BN_CLICKED(IDC_WINDPF_PROFILE_FORCE, OnChangeProfileType)
	ON_BN_CLICKED(IDC_WINDPF_PROFILE_SHEAR, OnChangeProfileType)
	ON_BN_CLICKED(IDC_WINDPF_PROFILE_MNT, OnChangeProfileType)
	ON_BN_CLICKED(IDC_WINDPF_PROFILE_MAX_DISP, OnChangeProfileType)
	ON_BN_CLICKED(IDC_WINDPF_PROFILE_MAX_ACC, OnChangeProfileType)
	ON_BN_CLICKED(IDC_WINDPF_ViewCalcSheet, OnWINDPFViewCalcSheet)
	ON_BN_CLICKED(IDC_WINDPF_BROWSE, OnWindPFBrowse)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindProfileKBC2015Dlg message handlers


void CWindProfileKBC2015Dlg::InitGrid(int nCode)
{

	//////////////////////////////////////////////////////////////////////////////////
	// Grid Initialize 
	//////////////////////////////////////////////////////////////////////////////////


	m_WindPfGrid.Initialize();

	// cell 바깥영역 배경색 지정 
	m_WindPfGrid.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	m_WindPfGrid.ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	m_WindPfGrid.ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	m_WindPfGrid.ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	// Sample setup for the grid
	m_WindPfGrid.GetParam()->EnableUndo(FALSE);

	m_WindPfGrid.SetRowCount(m_LateralLoad.m_aWindLoad_X.GetSize());
	m_WindPfGrid.SetColCount(11);

	CRect rect;
	m_WindPfGrid.GetClientRect(&rect);
	int cx;
	cx = rect.Width() * 2;
	int  scrollbar = GetSystemMetrics(SM_CXVSCROLL);

	m_WindPfGrid.SetColWidth(0, 0, cx * 0.1); // Name
	m_WindPfGrid.SetFrozenCols(0, 0);
	m_WindPfGrid.SetColWidth(1, 4, cx * 0.1);
	m_WindPfGrid.SetColWidth(5, 8, cx * 0.12);
	m_WindPfGrid.SetRowHeight(0, 0, globalUtils.ScaleByDPI(30), NULL);
	m_WindPfGrid.SetScrollBarMode(SB_VERT, gxnEnabled);
	m_WindPfGrid.SetScrollBarMode(SB_HORZ, gxnEnabled);

	m_WindPfGrid.SetStyleRange(CGXRange(0, 0), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Story_n_Name)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 1), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Elev_)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 2), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Pressure)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 3), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Loaded_n__H)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 4), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Loaded_n__B)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 5), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Wind_Force)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 6), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Added_n_Force)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 7), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Story_n_Force)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 8), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Story_n_Shear)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 9), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Overturn_g_nMoment)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 10), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Max_Disp)));
	m_WindPfGrid.SetStyleRange(CGXRange(0, 11), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Max_Accl)));

	m_WindPfGrid.SetStyleRange(CGXRange().SetCols(0, 11), CGXStyle()
		.SetWrapText(TRUE)
		.SetEnabled(FALSE));

	CGXGridParam* param;
	param = m_WindPfGrid.GetParam();
	param->EnableSelection(GX_SELFULL | GX_SELMULTIPLE);
	param->EnableTrackRowHeight(FALSE);

	UpdateGrid();
}

BOOL CWindProfileKBC2015Dlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	if (m_pDataWind->ScaleFactor_X == 0.0) m_nComponent = 1;

	CDlgUtil::CtrlEnableDisable(this, IDC_WINDPF_DIR_ACROSS, IsEnableAcross(m_pDataWind));
	CDlgUtil::CtrlEnableDisable(this, IDC_WINDPF_DIR_TORSIONAL, IsEnableTorsional(m_pDataWind));
	CDlgUtil::CtrlShowHide(this, m_aCtrlProfileSub2, SW_HIDE);
	if (m_KeyWAorWT != NULL)
	{
		if (GetKeyWA(m_pDataWind) == m_KeyWAorWT) m_nDir = 1;
		else m_nDir = 2;
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlProfileSub, m_nDir != 2);
	}

	GetDlgItem(IDC_WINDPF_COMP_X)->SetWindowText(m_nDir == 2 ? _LS(IDS_WG_CMD_DIR_RZ_X) : _LS(IDS_WG_CMD_DIR_X));
	GetDlgItem(IDC_WINDPF_COMP_Y)->SetWindowText(m_nDir == 2 ? _LS(IDS_WG_CMD_DIR_RZ_Y) : _LS(IDS_WG_CMD_DIR_Y));
	GetDlgItem(IDC_WINDPF_COMP_XY)->SetWindowText(m_nDir == 2 ? _LS(IDS_WG_CMD_DIR_RZ_XY) : _LS(IDS_WG_CMD_DIR_XY));
	GetDlgItem(IDC_WINDPF_COMP_SRSS)->SetWindowText(m_nDir == 2 ? _LS(IDS_WG_CMD_DIR_RZ_SRSS) : _LS(IDS_WG_CMD_DIR_SRSS));

	InitGrid(m_pDataWind->nCodeType);
	InitChart();

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	if (!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	///////////////////////////////////////////////////
	// FileName;
	TCHAR* lpBuffer;
	lpBuffer = new TCHAR[255];

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();

	m_strFileName = pDoc->GetPathName();
	if (m_strFileName == _T(""))
	{
		GetCurrentDirectory(255, lpBuffer);
		m_strFileName = lpBuffer;
		m_strFileName += _T("\\Untitled.wpf");
	}
	else
	{
		TCHAR path_buffer[_MAX_PATH];
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];

		m_strFileName.GetBuffer(255);
		_tcscpy(path_buffer, m_strFileName);
		m_strFileName.ReleaseBuffer();

		_tsplitpath(path_buffer, drive, dir, fname, ext);
		_tmakepath(path_buffer, drive, dir, fname, _T("wpf"));
		m_strFileName = path_buffer;
	}

#if defined (_MGEN_RUS)
	GetDlgItem(IDC_WINDPF_ViewCalcSheet)->ShowWindow(SW_HIDE);
#endif

	UpdateData(FALSE);

	delete[] lpBuffer;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CWindProfileKBC2015Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			if (pMsg->wParam == VK_RETURN)
				if (pMsg->wParam == VK_ESCAPE)

					::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
			// DO NOT process further
		}
		if ((GetKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 'C')
		{
			if (GetFocus() == &m_WindPfGrid)
			{
				CGXRangeList* pSelList = m_WindPfGrid.GetParam()->GetRangeList();
				if (pSelList && pSelList->GetCount() > 0)
				{
					POSITION pos = pSelList->GetHeadPosition();
					if (pos)
					{
						CGXRange range = pSelList->GetNext(pos);
						CGXRange headerRange(0, range.left, 0, range.right);
						m_WindPfGrid.SelectRange(headerRange);
						m_WindPfGrid.Copy();

						CString strData;
						if (::OpenClipboard(m_hWnd))
						{
							HANDLE hData = ::GetClipboardData(CF_UNICODETEXT);
							if (hData)
							{
								LPTSTR pData = (LPTSTR)::GlobalLock(hData);
								if (pData)
								{
									strData = pData;
									::GlobalUnlock(hData);
								}
							}
							::CloseClipboard();
						}
						m_WindPfGrid.SelectRange(range);
					}
				}
				return TRUE;
			}
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}

void CWindProfileKBC2015Dlg::InitChart()
{
	////////////////////////////////////////////////////////////////
	// Initialize m_Graph

	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component
	//
	SRGraphBackground* pB = new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent*)pB);

	// Add Display Component
	//

	SRGraphDisplay* pDisplay = new SRGraphDisplay;
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_HBAR);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_AUTOMATIC);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetRect(0.2, 0, 100, 95.0);
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));

	// Logarithmic Axis Scaling
	//
	pDisplay->GetStyle()->SetLogX(m_LogFlagX);
	pDisplay->GetStyle()->SetLog(m_LogFlagY);

	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis
	//
	SRGraphLabel* pLabel = new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::TopCenter);
	pLabel->SetTextPosition(50.0, 92.5);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	pLabel->SetFontSize(8);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Force));

	// Add Label Component for Y-Axis
	//
	pLabel = new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(2.0, 50.0);
	pLabel->SetFontSize(8);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	//pLabel->GetStyle()->SetColor(CXCLR_BACKGRND);
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(1, _LS(IDS_WG_CMD__ADDD__Story));

	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview

	CWnd* wnd = GetDlgItem(IDC_WINDPF_PLACEHOLDER);
	CRect rc;

	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left += 2;
	rc.top += 2;
	rc.right -= 2;
	rc.bottom -= 2;

	m_GraphView.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rc, this, IDC_CMD_PLACEHOLDER);
	m_GraphView.m_pGraph = &m_Graph;

	UpdateChart();

	m_GraphView.Invalidate();
}

void CWindProfileKBC2015Dlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup * 2, x);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);

	m_Graph.SetValue(nIndex, nGroup * 2 + 1, y);
}

void CWindProfileKBC2015Dlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel* pLabel = (SRGraphLabel*)m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CWindProfileKBC2015Dlg::InitWindProfile(T_WIND_K nKey, T_WIND_D& rDataWind, T_KEY KeyWAorWT, T_WNAT_D* pWnatD/*=NULL*/)
{
	m_Key = nKey;
	m_LateralLoad.MakeWindLoad(rDataWind, FALSE, _T(""), TRUE, FALSE, FALSE, 3, pWnatD);
	m_pDataWind = &rDataWind;
	m_KeyWAorWT = KeyWAorWT;
	m_pWnatD = pWnatD;
}

void CWindProfileKBC2015Dlg::OnChangeDir()
{
	UpdateData(TRUE);

	if (m_nDir == 2 && (m_nProfileType == 1 || m_nProfileType == 2))
	{
		m_nProfileType = 0;
		UpdateData(FALSE);
	}

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlProfileSub, m_nDir != 2);

	int nComponent = m_nComponent;
	if (m_nDir == 1) nComponent = 4 + m_nComponent;
	else if (m_nDir == 2) nComponent = 2 * 4;

	GetDlgItem(IDC_WINDPF_COMP_X)->SetWindowText(m_nDir == 2 ? _LS(IDS_WG_CMD_DIR_RZ_X) : _LS(IDS_WG_CMD_DIR_X));
	GetDlgItem(IDC_WINDPF_COMP_Y)->SetWindowText(m_nDir == 2 ? _LS(IDS_WG_CMD_DIR_RZ_Y) : _LS(IDS_WG_CMD_DIR_Y));
	GetDlgItem(IDC_WINDPF_COMP_XY)->SetWindowText(m_nDir == 2 ? _LS(IDS_WG_CMD_DIR_RZ_XY) : _LS(IDS_WG_CMD_DIR_XY));
	GetDlgItem(IDC_WINDPF_COMP_SRSS)->SetWindowText(m_nDir == 2 ? _LS(IDS_WG_CMD_DIR_RZ_SRSS) : _LS(IDS_WG_CMD_DIR_SRSS));

	m_LateralLoad.MakeWindLoad(*m_pDataWind, FALSE, _T(""), TRUE, FALSE, FALSE, nComponent, m_pWnatD);

	UpdateGrid();
	UpdateChart();
}

void CWindProfileKBC2015Dlg::OnChangeCompont()
{
	UpdateData(TRUE);
	UpdateGrid();
	UpdateChart();
}

void CWindProfileKBC2015Dlg::OnChangeProfileType()
{
	UpdateData(TRUE);
	UpdateGrid();
	UpdateChart();
}

void CWindProfileKBC2015Dlg::UpdateGrid()
{
	stWindLoad windload, windload_x, windload_y;
	CString sLevel = _T(""), sPressure = _T(""), sHeight = _T(""), sBreadth = _T(""), sForce, sAddedFrc = _T(""), sWindLoad = _T(""), sStoryShear = _T(""), sOTMoment = _T(""), strMaxDisp = _T(""), strMaxAccl = _T("");

	m_WindPfGrid.HideCols(2, 2, IsHideColPresssure(m_nDir, m_pDataWind));
	m_WindPfGrid.HideCols(8, 9, m_nDir == 2);
	m_WindPfGrid.HideCols(10, 11, IsHideColMaxDispAccl(m_nDir, m_pDataWind));

	for (int i = 0; i < m_LateralLoad.m_aWindLoad_X.GetSize(); i++)
	{
		if (m_nDir == 2)
		{
			windload_x = m_LateralLoad.m_aWindLoad_RZ[i];
			windload_y = m_LateralLoad.m_aWindLoad_RZ2[i];
		}
		else if (m_nDir == 1)
		{
			windload_x = m_LateralLoad.m_aWindLoad_X2[i];
			windload_y = m_LateralLoad.m_aWindLoad_Y2[i];
		}
		else
		{
			windload_x = m_LateralLoad.m_aWindLoad_X[i];
			windload_y = m_LateralLoad.m_aWindLoad_Y[i];
		}

		switch (m_nComponent)
		{
		case 0: // X
			windload = windload_x;
			break;
		case 1: // Y
			windload = windload_y;
			break;
		case 2: // XY
			windload = windload_x;
			windload.dBreadth = -1;
			windload.dForce = -1;
			windload.dAddedFrc = -1;
			windload.dOTMoment = -1;
			windload.dWindLoad = -1;
			windload.dStoryShear = -1;
			windload.dMaxDisp = -1;
			windload.dMaxAccl = -1;
			break;
		case 3: //SRSS
			windload = windload_x;
			windload.dBreadth = -1;
			windload.dAddedFrc = hypot(windload_x.dAddedFrc, windload_y.dAddedFrc);
			windload.dForce = hypot(windload_x.dForce, windload_y.dForce);
			windload.dOTMoment = hypot(windload_x.dOTMoment, windload_y.dOTMoment);
			windload.dWindLoad = hypot(windload_x.dWindLoad, windload_y.dWindLoad);
			windload.dStoryShear = hypot(windload_x.dStoryShear, windload_y.dStoryShear);
			windload.dMaxDisp = hypot(windload_x.dMaxDisp, windload_y.dMaxDisp);
			windload.dMaxAccl = hypot(windload_x.dMaxAccl, windload_y.dMaxAccl);
			break;
		}

		sLevel = ConvertValToFmtStr(windload.dLevel, 9);
		sPressure = ConvertValToFmtStr(windload.dPressure, 10);
		sHeight = ConvertValToFmtStr(windload.dHeight, 10);

		if (m_nDir == 2)
		{
			if (m_nComponent == 2)
			{
				sBreadth = _T("  --  ");
				sForce = _T("   --   ");
				sAddedFrc = _T("   --   ");
				sWindLoad = _T("   --   ");
				sStoryShear = _T("   --   ");
				sOTMoment = _T("   --   ");
				strMaxDisp = _T("   --   ");
				strMaxAccl = _T("   --   ");
			}
			else
			{
				sBreadth = m_nComponent == 3 ? _T("  --  ") : ConvertValToFmtStr(windload.dBreadth, 10);
				sForce = ConvertValToFmtStr(windload.dForce, 10);
				sAddedFrc = ConvertValToFmtStr(windload.dAddedFrc, 10);
				sWindLoad = ConvertValToFmtStr(windload.dWindLoad, 10);
				sStoryShear = _T("   --   ");
				sOTMoment = _T("   --   ");
				strMaxDisp = fabs(windload.dMaxDisp) < 1.0e-6 ? _T("   --   ") : ConvertValToFmtStr(windload.dMaxDisp, 10);
				strMaxAccl = fabs(windload.dMaxAccl) < 1.0e-6 ? _T("   --   ") : ConvertValToFmtStr(windload.dMaxAccl, 10);
			}
		}
		else
		{
			switch (m_nComponent)
			{
			case 0: // X
			case 1: // Y
				sBreadth = ConvertValToFmtStr(windload.dBreadth, 10);
				sForce = ConvertValToFmtStr(windload.dForce, 10);
				sAddedFrc = ConvertValToFmtStr(windload.dAddedFrc, 10);
				sWindLoad = ConvertValToFmtStr(windload.dWindLoad, 10);
				sStoryShear = ConvertValToFmtStr(windload.dStoryShear, 10);
				sOTMoment = ConvertValToFmtStr(windload.dOTMoment, 10);
				strMaxDisp = fabs(windload.dMaxDisp) < 1.0e-6 ? _T("   --   ") : ConvertValToFmtStr(windload.dMaxDisp, 10);
				strMaxAccl = fabs(windload.dMaxAccl) < 1.0e-6 ? _T("   --   ") : ConvertValToFmtStr(windload.dMaxAccl, 10);
				break;
			case 2: // XY
				sBreadth = _T("  --  ");
				sForce = _T("   --   ");
				sAddedFrc = _T("   --   ");
				sWindLoad = _T("   --   ");
				sStoryShear = _T("   --   ");
				sOTMoment = _T("   --   ");
				strMaxDisp = _T("   --   ");
				strMaxAccl = _T("   --   ");
				break;
			case 3: // SRSS
				sBreadth = _T("  --  ");
				sForce = ConvertValToFmtStr(windload.dForce, 10);
				sAddedFrc = ConvertValToFmtStr(windload.dAddedFrc, 10);
				sWindLoad = ConvertValToFmtStr(windload.dWindLoad, 10);
				sStoryShear = ConvertValToFmtStr(windload.dStoryShear, 10);
				sOTMoment = ConvertValToFmtStr(windload.dOTMoment, 10);
				strMaxDisp = fabs(windload.dMaxDisp) < 1.0e-6 ? _T("   --   ") : ConvertValToFmtStr(windload.dMaxDisp, 10);
				strMaxAccl = fabs(windload.dMaxAccl) < 1.0e-6 ? _T("   --   ") : ConvertValToFmtStr(windload.dMaxAccl, 10);
				break;
			}
		}

#if defined(_JP) || defined(_CH)
		m_WindPfGrid.SetStyleRange(CGXRange(0).SetRows(0, 0), CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
		m_WindPfGrid.ColHeaderStyle().SetFont(CGXFont_GC().SetBold(FALSE));
#endif
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 0), CGXStyle().SetValue(windload.sStoryName));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sPressure));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(sHeight));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(sBreadth));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(sForce));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(sAddedFrc));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(sWindLoad));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(sStoryShear));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 9), CGXStyle().SetValue(sOTMoment));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 10), CGXStyle().SetValue(strMaxDisp));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 11), CGXStyle().SetValue(strMaxAccl));

		if (i == m_LateralLoad.m_aWindLoad_X.GetSize() - 1)
		{
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 0), CGXStyle().SetValue(windload.sStoryName));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sPressure));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(sHeight));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(sBreadth));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(sForce));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(sAddedFrc));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(_T("   --   ")));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(sStoryShear));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 9), CGXStyle().SetValue(sOTMoment));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 10), CGXStyle().SetValue(strMaxDisp));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 11), CGXStyle().SetValue(strMaxAccl));
		}
	}
}

void CWindProfileKBC2015Dlg::UpdateChart()
{
	stWindLoad windload, windload_x, windload_y;
	m_Graph.KillData();
	double dValue;
	int index;
	for (int i = 0; i < m_LateralLoad.m_aWindLoad_X.GetSize(); i++)
	{
		index = m_LateralLoad.m_aWindLoad_X.GetSize() - 1 - i;
		if (m_nDir == 2)
		{
			windload_x = m_LateralLoad.m_aWindLoad_RZ[i];
			windload_y = m_LateralLoad.m_aWindLoad_RZ2[i];
		}
		else if (m_nDir == 1)
		{
			windload_x = m_LateralLoad.m_aWindLoad_X2[i];
			windload_y = m_LateralLoad.m_aWindLoad_Y2[i];
		}
		else
		{
			windload_x = m_LateralLoad.m_aWindLoad_X[i];
			windload_y = m_LateralLoad.m_aWindLoad_Y[i];
		}

		m_Graph.GetGroup(0)->GetIndex(index)->SetAnnotation(windload_x.sStoryName);

		switch (m_nComponent)
		{
		case 0: // X
			windload = windload_x;
			break;
		case 1: // Y
			windload = windload_y;
			break;
		case 2: // XY
			windload = windload_x;
			switch (m_nProfileType)
			{
			case 0: // Force;
				dValue = windload_y.dWindLoad;
				break;
			case 1: // Shear;
				dValue = windload_y.dStoryShear;
				break;
			case 2: // moment;
				dValue = windload_y.dOTMoment;
				break;
			case 3: // Max. Disp.
				dValue = windload.dMaxDisp;
				break;
			case 4: // Max. Accl.
				dValue = windload.dMaxAccl;
				break;
			}
			m_Graph.SetValue(index, 1, dValue);
			break;
		case 3: //SRSS
			windload = windload_x;
			windload.dOTMoment = sqrt(windload_x.dOTMoment * windload_x.dOTMoment + windload_y.dOTMoment * windload_y.dOTMoment);
			windload.dWindLoad = sqrt(windload_x.dWindLoad * windload_x.dWindLoad + windload_y.dWindLoad * windload_y.dWindLoad);
			windload.dStoryShear = sqrt(windload_x.dStoryShear * windload_x.dStoryShear + windload_y.dStoryShear * windload_y.dStoryShear);
			windload.dMaxDisp = sqrt(windload_x.dMaxDisp * windload_x.dMaxDisp + windload_y.dMaxDisp * windload_y.dMaxDisp);
			windload.dMaxAccl = sqrt(windload_x.dMaxAccl * windload_x.dMaxAccl + windload_y.dMaxAccl * windload_y.dMaxAccl);
			break;
		}

		switch (m_nProfileType)
		{
		case 0: // Force;
			dValue = windload.dWindLoad;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADD2__Wind_Force));
			break;
		case 1: // Shear;
			dValue = windload.dStoryShear;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Story_Shear));
			break;
		case 2: // moment;
			dValue = windload.dOTMoment;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Overturn_g_Moment));
			break;
		case 3: // Max. Disp.
			dValue = windload.dMaxDisp;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Max_Disp));
			break;
		case 4: // Max. Accl.
			dValue = windload.dMaxAccl;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Max_Accl));
			break;
		}

		m_Graph.SetValue(index, 0, dValue);
	}

	m_GraphView.Invalidate();

}

void CWindProfileKBC2015Dlg::OnBtnDyGen()
{
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_WD_PROF;   // Wind Load Profile
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = m_Key;
	UchtD.Data.Grph.nGraphType = m_nComponent;
	UchtD.Data.Grph.nOctBandOption = m_nProfileType;
	UchtD.bLogX = m_LogFlagX;
	UchtD.bLogY = m_LogFlagY;

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();

	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_WLPROF_NAME); //_T("Wind Load Profile");
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_WLPROF_NAME), nCount + 1);

	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if (dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;
		if (!pDoc->m_pDataCtrl->AddUcht(UchtD)) { ASSERT(0); return; }
	}

}

CString CWindProfileKBC2015Dlg::ConvertValToFmtStr(double Val, int nLimit)
{
	// nLimit = the Total Width of Output Value including deximal point and +/-Sign;
					 //   So Real Number of Digit above the point = nLimit -2;
	double RoundVal = 0.0;
	int    Sign = 0;
	CString Out, Fmt;
	int nPrecision = 0;

	if (fabs(Val) <= 1e-6) { Out = _T("0.0"); return Out; }

	int nDigits = 0;
	if (Val != 0) 	nDigits = log10(fabs(Val));

	if (nDigits >= 0)
	{
		nDigits++;
		if (nLimit - 2 - nDigits > 0) 		nPrecision = nLimit - 2 - nDigits;
		else	nPrecision = 1;
	}
	else if (nDigits < 0)
	{
		nDigits = 1;
		nPrecision = nLimit - 2 - nDigits;
	}

	if (Val >= 0) Sign = 1;
	else Sign = -1;

	RoundVal = Val + 5.0 * pow(10, -(nPrecision + 1)) * Sign; // 반올림효과 고려

	if (fabs(RoundVal) >= pow(10, (nLimit - 2) - nPrecision))   // Angle값은 +-90이다. 따라서 E포멧으로 가지 않는다.
	{
		if ((nLimit - 3 - 5) >= 1) Fmt.Format(_T("%%%d.%de"), nLimit, nLimit - 3 - 5);  //   exam. +1.245e-001
		else Fmt.Format(_T("*****"));                        // f로 안되면서 e로도 안되는 값/ ERROR!
	}

	else Fmt.Format(_T("%%%d.%df"), nLimit, nPrecision);

	Out.Format(Fmt, Val);

	/////////////////////////////////////////////
	// removing meaningless zeros...

	int nTemp = Out.GetLength();

	for (int i = nTemp - 1; i > 3; i--)
	{
		if (Out[i] == '0' && Out[i - 1] != '.')	Out = Out.Left(i);
		else break;
	}
	return Out;
}

void CWindProfileKBC2015Dlg::OnWINDPFViewCalcSheet()
{
	UpdateData(TRUE);

	int nComponent = m_nComponent;
	if (m_nDir == 1) nComponent = 4 + m_nComponent;
	else if (m_nDir == 2) nComponent = 2 * 4;

	m_LateralLoad.MakeWindLoad(*m_pDataWind, TRUE, m_strFileName, TRUE, FALSE, FALSE, nComponent, m_pWnatD);

	///////////////////////////////////////////////////////////////////////////////
	// Launching the Text Editor;
	///////////////////////////////////////////////////////////////////////////////
	CString strTitle = _LS(IDS_WG_CMD__ADD2__WIND_LOAD_CALC_);
	CDBLib::RunTextEditor(m_strFileName, strTitle);
}

void CWindProfileKBC2015Dlg::OnWindPFBrowse()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, _LS(IDS_WG_CMD__ADDD__All_Files__________));
	if (dlg.DoModal() == IDOK)
		m_strFileName = dlg.GetPathName();
	UpdateData(FALSE);
}

BOOL CWindProfileKBC2015Dlg::IsEnableAcross(const T_WIND_D* pWindD)
{
	auto L_CheckAcross = [](BOOL bUseForceCoeff, int nMethod, int nBuildingType, BOOL bAcrossWind) -> BOOL
		{
			if (bUseForceCoeff) return FALSE;
			if (nMethod != 1) return FALSE;
			if (nBuildingType != 0 && !bAcrossWind) return FALSE;
			return TRUE;
		};

	switch (pWindD->nCodeType)
	{
	case KBC_W_2016:
	{
		const T_WIND_KBC2015& KBC2015 = pWindD->CodeParam.KBC2015;
		return L_CheckAcross(KBC2015.bUseForceCoef, KBC2015.nMethod, KBC2015.nBuildingType, KBC2015.bAcrossWind);
	}
	case KDS_W_2019:
	{
		const T_WIND_KBC2015& KDS2019 = pWindD->CodeParam.KDS2019;
		return L_CheckAcross(KDS2019.bUseForceCoef, KDS2019.nMethod, KDS2019.nBuildingType, KDS2019.bAcrossWind);
	}
	case KDS_W_2022:
	{
		const T_WIND_KDS2021& KDS2021 = pWindD->CodeParam.KDS2021;
		return L_CheckAcross(KDS2021.bUseForceCoef, KDS2021.nMethod, KDS2021.nBuildingType, KDS2021.bAcrossWind);
	}
	default:
		ASSERT(0);
		return FALSE;
	}
}

BOOL CWindProfileKBC2015Dlg::IsEnableTorsional(const T_WIND_D* pWindD)
{
	auto L_CheckTorsional = [](BOOL bUseForceCoeff, int nMethod, BOOL bTorsionalWind) -> BOOL
		{
			if (bUseForceCoeff) return FALSE;
			if (nMethod != 1) return FALSE;
			if (!bTorsionalWind) return FALSE;
			return TRUE;
		};

	switch (pWindD->nCodeType)
	{
	case KBC_W_2016:
	{
		const T_WIND_KBC2015& KBC2015 = pWindD->CodeParam.KBC2015;
		return L_CheckTorsional(KBC2015.bUseForceCoef, KBC2015.nMethod, KBC2015.bTorsionalWind);
	}
	case KDS_W_2019:
	{
		const T_WIND_KBC2015& KDS2019 = pWindD->CodeParam.KDS2019;
		return L_CheckTorsional(KDS2019.bUseForceCoef, KDS2019.nMethod, KDS2019.bTorsionalWind);
	}
	case KDS_W_2022:
	{
		const T_WIND_KDS2021& KDS2021 = pWindD->CodeParam.KDS2021;
		return L_CheckTorsional(KDS2021.bUseForceCoef, KDS2021.nMethod, KDS2021.bTorsionalWind);
	}
	default:
		ASSERT(0);
		return FALSE;
	}
}

T_KEY CWindProfileKBC2015Dlg::GetKeyWA(const T_WIND_D* pWindD)
{
	switch (pWindD->nCodeType)
	{
	case KBC_W_2016: return pWindD->CodeParam.KBC2015.keyWA;
	case KDS_W_2019: return pWindD->CodeParam.KDS2019.keyWA;
	case KDS_W_2022: return pWindD->CodeParam.KDS2021.keyWA;
	default: ASSERT(0); return 0;
	}
}

BOOL CWindProfileKBC2015Dlg::IsHideColPresssure(int nDir, const T_WIND_D* pWindD)
{
	if (nDir != 0) { return TRUE; }

	auto L_HidePressure = [](int nMethod, BOOL bAutoCalc, int nWnatStructType) -> BOOL
		{
			if (nMethod == 0) { return TRUE; }
			if (bAutoCalc && nWnatStructType != ENM_T_CIRCLE_PLAN) { return TRUE; }
			return FALSE;
		};

	switch (pWindD->nCodeType)
	{
	case KBC_W_2016:
	{
		const T_WIND_KBC2015& KBC2015 = pWindD->CodeParam.KBC2015;
		return L_HidePressure(KBC2015.nMethod, KBC2015.bAutoCalc, KBC2015.nWnatStructType);
	}
	case KDS_W_2019:
	{
		const T_WIND_KBC2015& KDS2019 = pWindD->CodeParam.KDS2019;
		return L_HidePressure(KDS2019.nMethod, KDS2019.bAutoCalc, KDS2019.nWnatStructType);
	}
	case KDS_W_2022:
	{
		const T_WIND_KDS2021& KDS2021 = pWindD->CodeParam.KDS2021;
		return L_HidePressure(KDS2021.nMethod, KDS2021.bAutoCalc, KDS2021.nWnatStructType);
	}
	default:
		ASSERT(0);
		return FALSE;
	}
}

BOOL CWindProfileKBC2015Dlg::IsHideColMaxDispAccl(int nDir, const T_WIND_D* pWindD)
{
	auto L_CheckHide = [nDir](int nMethod, BOOL bUseForceCoef, BOOL bWindResponse, int nBuildingType) -> BOOL
		{
			if (nMethod == 0) { return TRUE; }
			if (nMethod == 1 && bUseForceCoef) { return TRUE; }
			if (!bWindResponse) { return TRUE; }
			if (nBuildingType == 0 && nDir == 1) { return TRUE; }
			return FALSE;
		};

	switch (pWindD->nCodeType)
	{
	case KBC_W_2016:
	{
		const T_WIND_KBC2015& KBC2015 = pWindD->CodeParam.KBC2015;
		return L_CheckHide(KBC2015.nMethod, KBC2015.bUseForceCoef, KBC2015.bWindResponse, KBC2015.nBuildingType);
	}
	case KDS_W_2019:
	{
		const T_WIND_KBC2015& KDS2019 = pWindD->CodeParam.KDS2019;
		return L_CheckHide(KDS2019.nMethod, KDS2019.bUseForceCoef, KDS2019.bWindResponse, KDS2019.nBuildingType);
	}
	case KDS_W_2022:
	{
		const T_WIND_KDS2021& KDS2021 = pWindD->CodeParam.KDS2021;
		return L_CheckHide(KDS2021.nMethod, KDS2021.bUseForceCoef, KDS2021.bWindResponse, KDS2021.nBuildingType);
	}
	default:
		ASSERT(0);
		return FALSE;
	}
}