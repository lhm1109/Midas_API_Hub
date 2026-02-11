#include "stdafx.h"
#include "wg_cmd.h"
#include "WindProfileDPTDlg.h"
#include "..\wg_db\LateralWindDPT.h"
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

enum EN_WIND_DIR
{
	EN_WIND_DIR_ALONG = 0,
	EN_WIND_DIR_ACROS,
	EN_WIND_DIR_TORSN,
};

enum EN_WIND_COMPONENT
{
	EN_WIND_COMPO_XX = 0,
	EN_WIND_COMPO_YY,
	EN_WIND_COMPO_XY,
	EN_WIND_COMPO_SR,
};

enum EN_WIND_PROFILE
{
	EN_WIND_PROFILE_FORCE = 0,
	EN_WIND_PROFILE_SHEAR,
	EN_WIND_PROFILE_MOMEN, // Moment
	EN_WIND_PROFILE_MDISP, // Max Displacement
	EN_WIND_PROFILE_MACCL, // Max Acceleration
};

CWindProfileDPTDlg::CWindProfileDPTDlg(CWnd* pParent)
	: CDialogMove(CWindProfileDPTDlg::IDD, pParent), m_LateralLoad(0, TRUE)
{
	m_nDir = EN_WIND_DIR_ALONG;
	m_nComponent = EN_WIND_COMPO_XX;
	m_nProfileType = EN_WIND_PROFILE_FORCE;
	m_strFileName.Empty();
	m_KeyWAorWT = NULL;

	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;

	m_aCtrlProfileSub.RemoveAll();
	m_aCtrlProfileSub.Add(IDC_WINDPF_PROFILE_SHEAR);
	m_aCtrlProfileSub.Add(IDC_WINDPF_PROFILE_MNT);
	m_aCtrlProfileSub.FreeExtra();

	m_pDataWind = nullptr;
	m_pWnatD = nullptr;
}

void CWindProfileDPTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Radio(pDX, IDC_WINDPF_DIR_ALONG, m_nDir);
	DDX_Radio(pDX, IDC_WINDPF_COMP_X, m_nComponent);
	DDX_Radio(pDX, IDC_WINDPF_PROFILE_FORCE, m_nProfileType);
	DDX_Text(pDX, IDC_WINDPF_EDT_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_WINDPF_TABLE, m_WindPfGrid);
}

BEGIN_MESSAGE_MAP(CWindProfileDPTDlg, CDialogMove)
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
END_MESSAGE_MAP()

BOOL CWindProfileDPTDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::DispatchMessage(pMsg);
			return TRUE;
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

BOOL CWindProfileDPTDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	if (m_pDataWind->ScaleFactor_X == 0.0)
	{
		m_nComponent = EN_WIND_COMPO_YY;
	}

	CDlgUtil::CtrlEnableDisable(this, IDC_WINDPF_DIR_ACROSS, IsEnableAcross(m_pDataWind));
	CDlgUtil::CtrlEnableDisable(this, IDC_WINDPF_DIR_TORSIONAL, IsEnableTorsional(m_pDataWind));

	if (m_KeyWAorWT != NULL)
	{
		if (GetKeyWA(m_pDataWind) == m_KeyWAorWT)
		{
			m_nDir = EN_WIND_DIR_ACROS;
		}
		else
		{
			m_nDir = EN_WIND_DIR_TORSN;
		}
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlProfileSub, m_nDir != EN_WIND_DIR_TORSN);
	}

	SetComponentText(m_nDir);
	InitGrid(m_pDataWind->nCodeType);
	InitChart();

	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	if (!CDBDoc::EnableDynaReport())
	{
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE);
	}

	InitFileName();

	UpdateData(FALSE);
	return TRUE;
}

void CWindProfileDPTDlg::InitGrid(int nCode)
{
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
	int cx = rect.Width() * 2;
	int scrollbar = GetSystemMetrics(SM_CXVSCROLL);

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

void CWindProfileDPTDlg::InitChart()
{
	m_GraphView.SetDragable(FALSE); // set data-drag unable

	SRGraphBackground* pB = new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent*)pB);

	// Add Display Component
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
	pDisplay->GetStyle()->SetLogX(m_LogFlagX);
	pDisplay->GetStyle()->SetLog(m_LogFlagY);

	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis
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
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(1, _LS(IDS_WG_CMD__ADDD__Story));

	CWnd* pWnd = GetDlgItem(IDC_WINDPF_PLACEHOLDER);
	CRect rc;
	pWnd->GetWindowRect(&rc);
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

void CWindProfileDPTDlg::InitFileName()
{
	m_strFileName.Empty();
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	if (pDoc != nullptr)
	{
		m_strFileName = pDoc->GetPathName();
	}

	if (m_strFileName.IsEmpty())
	{
		TCHAR* lpBuffer = new TCHAR[255];
		GetCurrentDirectory(255, lpBuffer);
		m_strFileName = lpBuffer;
		m_strFileName += "\\Untitled.wpf";
		delete[] lpBuffer;
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
}

void CWindProfileDPTDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup * 2, x);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);

	m_Graph.SetValue(nIndex, nGroup * 2 + 1, y);
}

void CWindProfileDPTDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel* pLabel = (SRGraphLabel*)m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CWindProfileDPTDlg::InitWindProfile(T_WIND_K nKey, T_WIND_D& rDataWind, T_KEY KeyWAorWT, T_WNAT_D* pWnatD/*=NULL*/)
{
	m_Key = nKey;
	m_LateralLoad.MakeWindLoad(rDataWind, FALSE, "", TRUE, FALSE, FALSE, 3, pWnatD);
	m_pDataWind = &rDataWind;
	m_KeyWAorWT = KeyWAorWT;
	m_pWnatD = pWnatD;
}

void CWindProfileDPTDlg::OnChangeDir()
{
	UpdateData(TRUE);

	if (m_nDir == EN_WIND_DIR_TORSN)
	{
		switch (m_nProfileType)
		{
		case EN_WIND_PROFILE_SHEAR:
		case EN_WIND_PROFILE_MOMEN:
		{
			m_nProfileType = EN_WIND_PROFILE_FORCE;
			UpdateData(FALSE);
		}
		break;
		}
	}

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlProfileSub, m_nDir != EN_WIND_DIR_TORSN);

	SetComponentText(m_nDir);

	int nComponent = GetComponent();
	m_LateralLoad.MakeWindLoad(*m_pDataWind, FALSE, "", TRUE, FALSE, FALSE, nComponent, m_pWnatD);
	UpdateGrid();
	UpdateChart();
}

void CWindProfileDPTDlg::OnChangeCompont()
{
	UpdateData(TRUE);
	UpdateGrid();
	UpdateChart();
}

void CWindProfileDPTDlg::OnChangeProfileType()
{
	UpdateData(TRUE);
	UpdateGrid();
	UpdateChart();
}

void CWindProfileDPTDlg::UpdateGrid()
{
	m_WindPfGrid.HideCols(2, 2, IsHideColPresssure(m_nDir, m_pDataWind));
	m_WindPfGrid.HideCols(8, 9, m_nDir == EN_WIND_DIR_TORSN);
	m_WindPfGrid.HideCols(10, 11, IsHideColMaxDispAccl(m_nDir, m_pDataWind));

	for (int i = 0; i < m_LateralLoad.m_aWindLoad_X.GetSize(); i++)
	{
		stWindLoad windload_x, windload_y;
		switch (m_nDir)
		{
		case EN_WIND_DIR_ACROS:
		{
			windload_x = m_LateralLoad.m_aWindLoad_X2[i];
			windload_y = m_LateralLoad.m_aWindLoad_Y2[i];
		}
		break;
		case EN_WIND_DIR_TORSN:
		{
			windload_x = m_LateralLoad.m_aWindLoad_RZ[i];
			windload_y = m_LateralLoad.m_aWindLoad_RZ2[i];
		}
		break;
		default:
		{
			windload_x = m_LateralLoad.m_aWindLoad_X[i];
			windload_y = m_LateralLoad.m_aWindLoad_Y[i];
		}
		break;
		}

		stWindLoad windload;
		switch (m_nComponent)
		{
		case EN_WIND_COMPO_XX:
		{
			windload = windload_x;
		}
		break;
		case EN_WIND_COMPO_YY:
		{
			windload = windload_y;
		}
		break;
		case EN_WIND_COMPO_XY:
		{
			windload = windload_x;
			windload.dBreadth = -1;
			windload.dForce = -1;
			windload.dAddedFrc = -1;
			windload.dOTMoment = -1;
			windload.dWindLoad = -1;
			windload.dStoryShear = -1;
			windload.dMaxDisp = -1;
			windload.dMaxAccl = -1;
		}
		break;
		case EN_WIND_COMPO_SR:
		{
			windload = windload_x;
			windload.dBreadth = -1;
			windload.dAddedFrc = hypot(windload_x.dAddedFrc, windload_y.dAddedFrc);
			windload.dForce = hypot(windload_x.dForce, windload_y.dForce);
			windload.dOTMoment = hypot(windload_x.dOTMoment, windload_y.dOTMoment);
			windload.dWindLoad = hypot(windload_x.dWindLoad, windload_y.dWindLoad);
			windload.dStoryShear = hypot(windload_x.dStoryShear, windload_y.dStoryShear);
			windload.dMaxDisp = hypot(windload_x.dMaxDisp, windload_y.dMaxDisp);
			windload.dMaxAccl = hypot(windload_x.dMaxAccl, windload_y.dMaxAccl);
		}
		break;
		}

		CString sLevel = ConvertValToFmtStr(windload.dLevel, 9);
		CString sPressure = ConvertValToFmtStr(windload.dPressure, 10);
		CString sHeight = ConvertValToFmtStr(windload.dHeight, 10);
		CString sBreadth = _T("  --  ");
		CString sForce = _T("   --   ");
		CString sAddedFrc = _T("   --   ");
		CString sWindLoad = _T("   --   ");
		CString sStoryShear = _T("   --   ");
		CString sOTMoment = _T("   --   ");
		CString strMaxDisp = _T("   --   ");
		CString strMaxAccl = _T("   --   ");

		if (m_nDir == EN_WIND_DIR_TORSN)
		{
			if (m_nComponent != EN_WIND_COMPO_XY)
			{
				sBreadth = (m_nComponent == EN_WIND_COMPO_SR) ? _T("  --  ") : ConvertValToFmtStr(windload.dBreadth, 10);
				sForce = ConvertValToFmtStr(windload.dForce, 10);
				sAddedFrc = ConvertValToFmtStr(windload.dAddedFrc, 10);
				sWindLoad = ConvertValToFmtStr(windload.dWindLoad, 10);
				sStoryShear = _T("   --   ");
				sOTMoment = _T("   --   ");
			}
		}
		else
		{
			switch (m_nComponent)
			{
			case EN_WIND_COMPO_XX:
			case EN_WIND_COMPO_YY:
			{
				sBreadth = ConvertValToFmtStr(windload.dBreadth, 10);
				sForce = ConvertValToFmtStr(windload.dForce, 10);
				sAddedFrc = ConvertValToFmtStr(windload.dAddedFrc, 10);
				sWindLoad = ConvertValToFmtStr(windload.dWindLoad, 10);
				sStoryShear = ConvertValToFmtStr(windload.dStoryShear, 10);
				sOTMoment = ConvertValToFmtStr(windload.dOTMoment, 10);
			}
			break;
			case EN_WIND_COMPO_XY:
				break;
			case EN_WIND_COMPO_SR:
			{
				sBreadth = _T("  --  ");
				sForce = ConvertValToFmtStr(windload.dForce, 10);
				sAddedFrc = ConvertValToFmtStr(windload.dAddedFrc, 10);
				sWindLoad = ConvertValToFmtStr(windload.dWindLoad, 10);
				sStoryShear = ConvertValToFmtStr(windload.dStoryShear, 10);
				sOTMoment = ConvertValToFmtStr(windload.dOTMoment, 10);
			}
			break;
			}
		}

		bool bLast = (i == m_LateralLoad.m_aWindLoad_X.GetSize() - 1) ? true : false;
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 0), CGXStyle().SetValue(windload.sStoryName));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sPressure));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(sHeight));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(sBreadth));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(sForce));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(sAddedFrc));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(bLast ? _T("   --   ") : sWindLoad));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(sStoryShear));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 9), CGXStyle().SetValue(sOTMoment));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 10), CGXStyle().SetValue(strMaxDisp));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 11), CGXStyle().SetValue(strMaxAccl));
	}
}

void CWindProfileDPTDlg::UpdateChart()
{
	m_Graph.KillData();

	// stWindLoad windload, windload_x, windload_y;
	// double dValue;
	// int index;
	for (int i = 0; i < m_LateralLoad.m_aWindLoad_X.GetSize(); i++)
	{
		stWindLoad windload_x, windload_y;
		switch (m_nDir)
		{
		case EN_WIND_DIR_ACROS:
		{
			windload_x = m_LateralLoad.m_aWindLoad_X2[i];
			windload_y = m_LateralLoad.m_aWindLoad_Y2[i];
		}
		break;
		case EN_WIND_DIR_TORSN:
		{
			windload_x = m_LateralLoad.m_aWindLoad_RZ[i];
			windload_y = m_LateralLoad.m_aWindLoad_RZ2[i];
		}
		break;
		default:
		{
			windload_x = m_LateralLoad.m_aWindLoad_X[i];
			windload_y = m_LateralLoad.m_aWindLoad_Y[i];
		}
		break;
		}

		int index = m_LateralLoad.m_aWindLoad_X.GetSize() - 1 - i;
		m_Graph.GetGroup(0)->GetIndex(index)->SetAnnotation(windload_x.sStoryName);

		stWindLoad windload;
		double dValue = 0.0;
		switch (m_nComponent)
		{
		case EN_WIND_COMPO_XX:
		{
			windload = windload_x;
		}
		break;
		case EN_WIND_COMPO_YY:
		{
			windload = windload_y;
		}
		break;
		case EN_WIND_COMPO_XY:
		{
			windload = windload_x;
			switch (m_nProfileType)
			{
			case EN_WIND_PROFILE_FORCE:
			{
				dValue = windload_y.dWindLoad;
			}
			break;
			case EN_WIND_PROFILE_SHEAR:
			{
				dValue = windload_y.dStoryShear;
			}
			break;
			case EN_WIND_PROFILE_MOMEN:
			{
				dValue = windload_y.dOTMoment;
			}
			break;
			case EN_WIND_PROFILE_MDISP:
			{
				dValue = windload.dMaxDisp;
			}
			break;
			case EN_WIND_PROFILE_MACCL:
			{
				dValue = windload.dMaxAccl;
			}
			break;
			}
			m_Graph.SetValue(index, 1, dValue);
		}
		break;
		case EN_WIND_COMPO_SR:
		{
			windload = windload_x;
			windload.dOTMoment = sqrt(windload_x.dOTMoment * windload_x.dOTMoment + windload_y.dOTMoment * windload_y.dOTMoment);
			windload.dWindLoad = sqrt(windload_x.dWindLoad * windload_x.dWindLoad + windload_y.dWindLoad * windload_y.dWindLoad);
			windload.dStoryShear = sqrt(windload_x.dStoryShear * windload_x.dStoryShear + windload_y.dStoryShear * windload_y.dStoryShear);
			windload.dMaxDisp = sqrt(windload_x.dMaxDisp * windload_x.dMaxDisp + windload_y.dMaxDisp * windload_y.dMaxDisp);
			windload.dMaxAccl = sqrt(windload_x.dMaxAccl * windload_x.dMaxAccl + windload_y.dMaxAccl * windload_y.dMaxAccl);
		}
		break;
		}

		switch (m_nProfileType)
		{
		case EN_WIND_PROFILE_FORCE:
		{
			dValue = windload.dWindLoad;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADD2__Wind_Force));
		}
		break;
		case EN_WIND_PROFILE_SHEAR:
		{
			dValue = windload.dStoryShear;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Story_Shear));
		}
		break;
		case EN_WIND_PROFILE_MOMEN:
		{
			dValue = windload.dOTMoment;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Overturn_g_Moment));
		}
		break;
		case EN_WIND_PROFILE_MDISP:
		{
			dValue = windload.dMaxDisp;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Max_Disp));
		}
		break;
		case EN_WIND_PROFILE_MACCL:
		{
			dValue = windload.dMaxAccl;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Max_Accl));
		}
		break;
		}

		m_Graph.SetValue(index, 0, dValue);
	}

	m_GraphView.Invalidate();
}

void CWindProfileDPTDlg::OnBtnDyGen()
{
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_WD_PROF; // Wind Load Profile
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = m_Key;
	UchtD.Data.Grph.nGraphType = m_nComponent;
	UchtD.Data.Grph.nOctBandOption = m_nProfileType;
	UchtD.bLogX = m_LogFlagX;
	UchtD.bLogY = m_LogFlagY;

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);

	CString csName;
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

CString CWindProfileDPTDlg::ConvertValToFmtStr(double dVal, int nLimit)
{
	if (dgn::EQ0(dVal)) { return _T("0.0"); }

	int nDigits = 0;
	if (dVal != 0)
	{
		nDigits = log10(fabs(dVal));
	}

	int nPrecision = 0;
	if (nDigits >= 0)
	{
		nDigits++;
		if (nLimit - 2 - nDigits > 0)
		{
			nPrecision = nLimit - 2 - nDigits;
		}
		else
		{
			nPrecision = 1;
		}
	}
	else if (nDigits < 0)
	{
		nDigits = 1;
		nPrecision = nLimit - 2 - nDigits;
	}

	int nSign = -1;
	if (dVal >= 0)
	{
		nSign = 1;
	}

	double dRoundVal = dVal + 5.0 * pow(10, -(nPrecision + 1)) * nSign; // 반올림효과 고려

	CString csFormat;
	if (fabs(dRoundVal) >= pow(10, (nLimit - 2) - nPrecision))
	{
		if ((nLimit - 3 - 5) >= 1)
		{
			csFormat.Format(_T("%%%d.%de"), nLimit, nLimit - 3 - 5);
		}
		else
		{
			csFormat.Format(_T("*****"));
		}
	}
	else
	{
		csFormat.Format(_T("%%%d.%df"), nLimit, nPrecision);
	}

	CString csOut;
	csOut.Format(csFormat, dVal);

	/////////////////////////////////////////////
	// removing meaningless zeros...
	int nTemp = csOut.GetLength();
	for (int i = nTemp - 1; i > 3; i--)
	{
		if (csOut[i] == '0' && csOut[i - 1] != '.')
		{
			csOut = csOut.Left(i);
		}
		else
		{
			break;
		}
	}
	return csOut;
}

void CWindProfileDPTDlg::OnWINDPFViewCalcSheet()
{
	UpdateData(TRUE);

	int nComponent = GetComponent();
	m_LateralLoad.MakeWindLoad(*m_pDataWind, TRUE, m_strFileName, TRUE, FALSE, FALSE, nComponent, m_pWnatD);

	CString strTitle = _LS(IDS_WG_CMD__ADD2__WIND_LOAD_CALC_);
	CDBLib::RunTextEditor(m_strFileName, strTitle);
}

int CWindProfileDPTDlg::GetComponent()
{
	int nComponent = m_nComponent;
	switch (m_nDir)
	{
	case EN_WIND_DIR_ACROS:
	{
		nComponent = 4 + m_nComponent;
	}
	break;
	case EN_WIND_DIR_TORSN:
	{
		nComponent = 2 * 4;
	}
	break;
	}
	return nComponent;
}

void CWindProfileDPTDlg::OnWindPFBrowse()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, _LS(IDS_WG_CMD__ADDD__All_Files__________));
	if (dlg.DoModal() == IDOK)
	{
		m_strFileName = dlg.GetPathName();
	}
	UpdateData(FALSE);
}

BOOL CWindProfileDPTDlg::IsEnableAcross(const T_WIND_D* pWindD)
{
	switch (pWindD->nCodeType)
	{
	case DPT_W_2007:
	{
		CLateralWindDPT windDPT(CDBDoc::GetDocPoint());
		BOOL bAcross = FALSE, bTorsion = FALSE, bResponse = FALSE;
		windDPT.GetAdditionalWindParams(pWindD->CodeParam.DPT2007, bAcross, bTorsion, bResponse);
		return bAcross;
	}
	}
	ASSERT(0); return FALSE;
}

BOOL CWindProfileDPTDlg::IsEnableTorsional(const T_WIND_D* pWindD)
{
	switch (pWindD->nCodeType)
	{
	case DPT_W_2007:
	{
		CLateralWindDPT windDPT(CDBDoc::GetDocPoint());
		BOOL bAcross = FALSE, bTorsion = FALSE, bResponse = FALSE;
		windDPT.GetAdditionalWindParams(pWindD->CodeParam.DPT2007, bAcross, bTorsion, bResponse);
		return bTorsion;
	}
	}
	ASSERT(0); return FALSE;
}

UINT CWindProfileDPTDlg::GetKeyWA(const T_WIND_D* pWindD)
{
	switch (pWindD->nCodeType)
	{
	case DPT_W_2007: return pWindD->CodeParam.DPT2007.keyWA;
	}
	ASSERT(0); return 0;
}

BOOL CWindProfileDPTDlg::IsHideColPresssure(int nDir, const T_WIND_D* pWindD)
{
	return TRUE;
}

BOOL CWindProfileDPTDlg::IsHideColMaxDispAccl(int nDir, const T_WIND_D* pWindD)
{
	return TRUE;
}

void CWindProfileDPTDlg::SetComponentText(UINT nDir)
{
	auto L_SetWindowText = [this](UINT nCtrlID, const CString& csText)
		{
			CWnd* pWnd = GetDlgItem(nCtrlID);
			if (pWnd == nullptr) { ASSERT(0); return; }
			pWnd->SetWindowText(csText);
		};

	switch (nDir)
	{
	case EN_WIND_DIR_TORSN:
	{
		L_SetWindowText(IDC_WINDPF_COMP_X, _LS(IDS_WG_CMD_DIR_RZ_X));
		L_SetWindowText(IDC_WINDPF_COMP_Y, _LS(IDS_WG_CMD_DIR_RZ_Y));
		L_SetWindowText(IDC_WINDPF_COMP_XY, _LS(IDS_WG_CMD_DIR_RZ_XY));
		L_SetWindowText(IDC_WINDPF_COMP_SRSS, _LS(IDS_WG_CMD_DIR_RZ_SRSS));
	}
	break;
	default:
	{
		L_SetWindowText(IDC_WINDPF_COMP_X, _LS(IDS_WG_CMD_DIR_X));
		L_SetWindowText(IDC_WINDPF_COMP_Y, _LS(IDS_WG_CMD_DIR_Y));
		L_SetWindowText(IDC_WINDPF_COMP_XY, _LS(IDS_WG_CMD_DIR_XY));
		L_SetWindowText(IDC_WINDPF_COMP_SRSS, _LS(IDS_WG_CMD_DIR_SRSS));
	}
	break;
	}
}