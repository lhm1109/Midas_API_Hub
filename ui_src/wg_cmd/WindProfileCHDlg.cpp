// WindProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindProfileCHDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\DynamicReportTemplateDlg.h"

#include "..\wg_common\wg_common_TBGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindProfileCHDlg dialog


//code : 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982

#define KS_W_92 1
#define JP_W_87 2
#define UBC_W_97 3
#define ANSI_W_82 4
#define EURO_W_92 7
#define BS_W_97 8
#define CH_W_2002 9
#define CH_W_2012 19


CWindProfileCHDlg::CWindProfileCHDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindProfileCHDlg::IDD, pParent), m_LateralLoad(0, TRUE)
{
	//{{AFX_DATA_INIT(CWindProfileCHDlg)
	m_nWindType = 0;
	m_nComponent = 0;
	m_nProfileType = 0;
	m_strFileName = _T("");
	//}}AFX_DATA_INIT
	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;

}


void CWindProfileCHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindProfileCHDlg)
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Radio(pDX, IDC_WINDPF_COMP_WDk, m_nWindType);
	DDX_Radio(pDX, IDC_WINDPF_COMP_X, m_nComponent);
	DDX_Radio(pDX, IDC_WINDPF_PROFILE_FORCE, m_nProfileType);
	DDX_Text(pDX, IDC_WINDPF_EDT_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_WINDPF_TABLE, m_WindPfGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindProfileCHDlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindProfileCHDlg)
	ON_BN_CLICKED(IDC_WINDPF_COMP_WDk, OnChangeWindType)
	ON_BN_CLICKED(IDC_WINDPF_COMP_WLk, OnChangeWindType)
	ON_BN_CLICKED(IDC_WINDPF_COMP_WTk, OnChangeWindType)
	ON_BN_CLICKED(IDC_WINDPF_COMP_X, OnChangeCompont)
	ON_BN_CLICKED(IDC_WINDPF_PROFILE_FORCE, OnChangeProfileType)
	ON_BN_CLICKED(IDC_WINDPF_ViewCalcSheet, OnWINDPFViewCalcSheet)
	ON_BN_CLICKED(IDC_WINDPF_COMP_Y, OnChangeCompont)
	ON_BN_CLICKED(IDC_WINDPF_COMP_XY, OnChangeCompont)
	ON_BN_CLICKED(IDC_WINDPF_COMP_SRSS, OnChangeCompont)
	ON_BN_CLICKED(IDC_WINDPF_PROFILE_SHEAR, OnChangeProfileType)
	ON_BN_CLICKED(IDC_WINDPF_PROFILE_MNT, OnChangeProfileType)
	ON_BN_CLICKED(IDC_WINDPF_BROWSE, OnWindPFBrowse)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindProfileCHDlg message handlers


void CWindProfileCHDlg::InitGrid(int nCode, BOOL bInitial)
{
	//////////////////////////////////////////////////////////////////////////////////
	// Grid Initialize 
	//////////////////////////////////////////////////////////////////////////////////
	if (bInitial)
	{
		m_WindPfGrid.Initialize();
	}

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

	if (m_nWindType == 1)
	{
		m_WindPfGrid.SetColCount(8);
	}
	if (m_nWindType == 2)
	{
		m_WindPfGrid.SetColCount(2);
	}
	else
	{
		m_WindPfGrid.SetColCount(9);
	}

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

	{
		m_WindPfGrid.SetStyleRange(CGXRange(0, 0), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Story_n_Name)));
		if (m_nWindType == 1)
		{
			m_WindPfGrid.SetStyleRange(CGXRange(0, 1), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Elev_)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 2), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Pressure)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 3), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Loaded_n__H)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 4), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Loaded_n__B)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 5), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Wind_Force)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 6), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Story_n_Force)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 7), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Story_n_Shear)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 8), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Overturn_g_nMoment)));
		}
		if (m_nWindType == 2)
		{
			m_WindPfGrid.SetStyleRange(CGXRange(0, 1), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Elev_)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 2), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Wind_Force)));
		}
		else
		{
			m_WindPfGrid.SetStyleRange(CGXRange(0, 1), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Elev_)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 2), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Pressure)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 3), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Loaded_n__H)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 4), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Loaded_n__B)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 5), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Wind_Force)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 6), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Added_n_Force)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 7), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Story_n_Force)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 8), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Story_n_Shear)));
			m_WindPfGrid.SetStyleRange(CGXRange(0, 9), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Overturn_g_nMoment)));
		}
	}

	if (m_nWindType == 1)
	{
		m_WindPfGrid.SetStyleRange(CGXRange().SetCols(0, 8), CGXStyle()
			.SetWrapText(TRUE)
			.SetEnabled(FALSE));
	}
	if (m_nWindType == 2)
	{
		m_WindPfGrid.SetStyleRange(CGXRange().SetCols(0, 2), CGXStyle()
			.SetWrapText(TRUE)
			.SetEnabled(FALSE));
	}
	else
	{
		m_WindPfGrid.SetStyleRange(CGXRange().SetCols(0, 9), CGXStyle()
			.SetWrapText(TRUE)
			.SetEnabled(FALSE));
	}

	CGXGridParam* param;
	param = m_WindPfGrid.GetParam();
	param->EnableSelection(GX_SELFULL | GX_SELMULTIPLE);
	//  param->EnableTrackColWidth(TRUE);
	param->EnableTrackRowHeight(FALSE);

	UpdateGrid();
}

BOOL CWindProfileCHDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	if (m_pDataWind->ScaleFactor_X == 0.0) m_nComponent = 1;
	InitGrid(m_pDataWind->nCodeType, TRUE);
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

	UpdateData(FALSE);

	delete[] lpBuffer;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CWindProfileCHDlg::PreTranslateMessage(MSG* pMsg)
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

void CWindProfileCHDlg::InitChart()
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

	//pDisplay->GetStyle()->SetShowXGrid(m_GridXFlag);
	//pDisplay->GetStyle()->SetShowYGrid(m_GridYFlag);

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

void CWindProfileCHDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup * 2, x);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);

	m_Graph.SetValue(nIndex, nGroup * 2 + 1, y);
}

void CWindProfileCHDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel* pLabel = (SRGraphLabel*)m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CWindProfileCHDlg::InitWindProfile(T_WIND_K nKey, T_WIND_D& rDataWind)
{
	m_Key = nKey;
	T_WIND_D data = rDataWind;
	if (data.nCodeType == CH_W_2021)
	{
		data.CodeParam.CH2019.bCrossVibration = FALSE;
		data.CodeParam.CH2019.bCross = FALSE;
		data.CodeParam.CH2019.bTorsion = FALSE;
	}
	else if (data.nCodeType == CH_W_2012)
	{
		data.CodeParam.CH2012.bCrossVibration = FALSE;
		data.CodeParam.CH2012.bCross = FALSE;
		data.CodeParam.CH2012.bTorsion = FALSE;
	}
	else ASSERT(0);
	m_LateralLoad.MakeWindLoad(data, FALSE, _T(""), TRUE);
	m_pDataWind = &rDataWind;
}

void CWindProfileCHDlg::UpdateWindType()
{
	T_WIND_D rDataWind;
	if (m_nWindType == 0)
	{
		rDataWind = *m_pDataWind;
		rDataWind.CodeParam.CH2012.bCrossVibration = FALSE;
		rDataWind.CodeParam.CH2012.bTorsion = FALSE;
		rDataWind.CodeParam.CH2012.bCross = FALSE;
		if (rDataWind.nCodeType == 23)
		{
			rDataWind.CodeParam.CH2019.bCrossVibration = FALSE;
			rDataWind.CodeParam.CH2019.bTorsion = FALSE;
			rDataWind.CodeParam.CH2019.bCross = FALSE;
		}
		m_LateralLoad.MakeWindLoad(rDataWind, FALSE, _T(""), TRUE);
	}
	else if (m_nWindType == 1)
	{
		rDataWind = *m_pDataWind;
		rDataWind.CodeParam.CH2012.bAlongVibration = FALSE;
		rDataWind.CodeParam.CH2012.nAlongType = 0;
		rDataWind.CodeParam.CH2012.bTorsion = FALSE;
		rDataWind.CodeParam.CH2012.bCross = TRUE;
		if (rDataWind.nCodeType == 23)
		{
			rDataWind.CodeParam.CH2019.bAlongVibration = FALSE;
			rDataWind.CodeParam.CH2019.nAlongType = 0;
			rDataWind.CodeParam.CH2019.bTorsion = FALSE;
			rDataWind.CodeParam.CH2019.bCross = TRUE;
		}
		m_LateralLoad.MakeWindLoad(rDataWind, FALSE, _T(""), TRUE);
	}
	else if (m_nWindType == 2)
	{
		rDataWind = *m_pDataWind;
		rDataWind.CodeParam.CH2012.bAlongVibration = FALSE;
		rDataWind.CodeParam.CH2012.nAlongType = 0;
		rDataWind.CodeParam.CH2012.bCross = FALSE;
		rDataWind.CodeParam.CH2012.bTorsion = TRUE;
		if (rDataWind.nCodeType == 23)
		{
			rDataWind.CodeParam.CH2019.bAlongVibration = FALSE;
			rDataWind.CodeParam.CH2019.nAlongType = 0;
			rDataWind.CodeParam.CH2019.bTorsion = FALSE;
			rDataWind.CodeParam.CH2019.bCross = TRUE;
		}
		m_LateralLoad.MakeWindLoad(rDataWind, FALSE, _T(""), TRUE);
	}
	UpdateData(TRUE);
	InitGrid(m_pDataWind->nCodeType, FALSE);
	m_WindPfGrid.Redraw();
}

void CWindProfileCHDlg::OnChangeWindType()
{
	UpdateData(TRUE);
	UpdateWindType();
	UpdateGrid();
	UpdateChart();
}

void CWindProfileCHDlg::OnChangeCompont()
{
	UpdateData(TRUE);
	UpdateGrid();
	UpdateChart();
}

void CWindProfileCHDlg::OnChangeProfileType()
{
	UpdateData(TRUE);
	UpdateGrid();
	UpdateChart();
}

void CWindProfileCHDlg::UpdateGrid()
{
	stWindLoad windload, windload_x, windload_y, windload_t;
	CString sLevel, sPressure, sHeight, sBreadth, sForce, sAddedFrc, sWindLoad, sStoryShear, sOTMoment;

	for (int i = 0; i < m_LateralLoad.m_aWindLoad_X.GetSize(); i++)
	{
		windload_x = m_LateralLoad.m_aWindLoad_X[i];
		windload_y = m_LateralLoad.m_aWindLoad_Y[i];
		windload_t = m_LateralLoad.m_aWindLoad_RZ[i];

		switch (m_nComponent)
		{
		case 0: // X
			windload = windload_x;
			if (m_nWindType == 2) windload = windload_t;
			break;
		case 1: // Y
			windload = windload_y;
			if (m_nWindType == 2) windload = windload_t;
			break;
		case 2: // XY
			windload = windload_x;
			windload.dBreadth = -1;
			windload.dForce = -1;
			windload.dAddedFrc = -1;
			windload.dOTMoment = -1;
			windload.dWindLoad = -1;
			windload.dStoryShear = -1;
			break;
		case 3: //SRSS
			windload = windload_x;
			windload.dBreadth = -1;
			windload.dAddedFrc = sqrt(windload_x.dAddedFrc * windload_x.dAddedFrc + windload_y.dAddedFrc * windload_y.dAddedFrc);
			windload.dForce = sqrt(windload_x.dForce * windload_x.dForce + windload_y.dForce * windload_y.dForce);
			windload.dOTMoment = sqrt(windload_x.dOTMoment * windload_x.dOTMoment + windload_y.dOTMoment * windload_y.dOTMoment);
			windload.dWindLoad = sqrt(windload_x.dWindLoad * windload_x.dWindLoad + windload_y.dWindLoad * windload_y.dWindLoad);
			windload.dStoryShear = sqrt(windload_x.dStoryShear * windload_x.dStoryShear + windload_y.dStoryShear * windload_y.dStoryShear);
			break;
		}

		sLevel = ConvertValToFmtStr(windload.dLevel, 9);
		sPressure = ConvertValToFmtStr(windload.dPressure, 10);
		sHeight = ConvertValToFmtStr(windload.dHeight, 10);

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
			break;
		case 2: // XY
			sBreadth = _T("  --  ");
			sForce = _T("   --   ");
			sAddedFrc = _T("   --   ");
			sWindLoad = _T("   --   ");
			sStoryShear = _T("   --   ");
			sOTMoment = _T("   --   ");
			break;
		case 3: // SRSS
			sBreadth = _T("  --  ");
			sForce = ConvertValToFmtStr(windload.dForce, 10);
			sAddedFrc = ConvertValToFmtStr(windload.dAddedFrc, 10);
			sWindLoad = ConvertValToFmtStr(windload.dWindLoad, 10);
			sStoryShear = ConvertValToFmtStr(windload.dStoryShear, 10);
			sOTMoment = ConvertValToFmtStr(windload.dOTMoment, 10);
			break;
		}

		m_WindPfGrid.SetStyleRange(CGXRange(0).SetRows(0, 0), CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
		m_WindPfGrid.ColHeaderStyle().SetFont(CGXFont_GC().SetBold(FALSE));
		m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 0), CGXStyle().SetValue(windload.sStoryName));
		if (m_nWindType == 1)
		{
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sPressure));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(sHeight));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(sBreadth));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(sForce));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(sWindLoad));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(sStoryShear));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(sOTMoment));
		}
		if (m_nWindType == 2)
		{
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sForce));
		}
		else
		{
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sPressure));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(sHeight));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(sBreadth));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(sForce));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(sAddedFrc));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(sWindLoad));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(sStoryShear));
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 9), CGXStyle().SetValue(sOTMoment));
		}
		if (i == m_LateralLoad.m_aWindLoad_X.GetSize() - 1)
		{
			m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 0), CGXStyle().SetValue(windload.sStoryName));
			if (m_nWindType == 1)
			{
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sPressure));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(sHeight));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(sBreadth));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(sForce));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(_T("   --   ")));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(sStoryShear));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(sOTMoment));
			}
			if (m_nWindType == 2)
			{
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sForce));
			}
			else
			{
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sLevel));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sPressure));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(sHeight));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(sBreadth));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(sForce));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(sAddedFrc));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(_T("   --   ")));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(sStoryShear));
				m_WindPfGrid.SetStyleRange(CGXRange(i + 1, 9), CGXStyle().SetValue(sOTMoment));
			}
		}
	}
}

void CWindProfileCHDlg::UpdateChart()
{
	stWindLoad windload, windload_x, windload_y, windload_t;
	m_Graph.KillData();
	double dValue;
	int index;
	for (int i = 0; i < m_LateralLoad.m_aWindLoad_X.GetSize(); i++)
	{
		index = m_LateralLoad.m_aWindLoad_X.GetSize() - 1 - i;
		windload_x = m_LateralLoad.m_aWindLoad_X[i];
		windload_y = m_LateralLoad.m_aWindLoad_Y[i];
		windload_t = m_LateralLoad.m_aWindLoad_RZ[i];

		m_Graph.GetGroup(0)->GetIndex(index)->SetAnnotation(windload_x.sStoryName);

		switch (m_nComponent)
		{
		case 0: // X
			windload = (m_nWindType == 2) ? windload_t : windload_x;
			break;
		case 1: // Y
			windload = (m_nWindType == 2) ? windload_t : windload_y;
			break;
		case 2: // XY
			windload = (m_nWindType == 2) ? windload_t : windload_x;
			switch (m_nProfileType)
			{
			case 0: // Force;
				dValue = windload_y.dWindLoad;
				break;
			case 1: // Shear;
				dValue = (m_nWindType == 2) ? .0 : windload_y.dStoryShear;
				break;
			case 2: // moment;
				dValue = (m_nWindType == 2) ? .0 : windload_y.dOTMoment;
			}
			m_Graph.SetValue(index, 1, dValue);
			break;
		case 3: //SRSS
			windload = (m_nWindType == 2) ? windload_t : windload_x;
			windload.dOTMoment = sqrt(windload_x.dOTMoment * windload_x.dOTMoment + windload_y.dOTMoment * windload_y.dOTMoment);
			windload.dWindLoad = sqrt(windload_x.dWindLoad * windload_x.dWindLoad + windload_y.dWindLoad * windload_y.dWindLoad);
			windload.dStoryShear = sqrt(windload_x.dStoryShear * windload_x.dStoryShear + windload_y.dStoryShear * windload_y.dStoryShear);
			break;
		}

		switch (m_nProfileType)
		{
		case 0: // Force;
			dValue = windload.dWindLoad;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADD2__Wind_Force));
			break;
		case 1: // Shear;
			dValue = (m_nWindType == 2) ? .0 : windload.dStoryShear;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Story_Shear));
			break;
		case 2: // moment;
			dValue = (m_nWindType == 2) ? .0 : windload.dOTMoment;
			SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Overturn_g_Moment));
		}

		m_Graph.SetValue(index, 0, dValue);
	}

	m_GraphView.Invalidate();

}

void CWindProfileCHDlg::OnBtnDyGen()
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

CString CWindProfileCHDlg::ConvertValToFmtStr(double Val, int nLimit)
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

void CWindProfileCHDlg::OnWINDPFViewCalcSheet()
{
	UpdateData(TRUE);
	T_WIND_D rDataWind = *m_pDataWind;
	if (m_nWindType == 0)//Along
	{
		if (rDataWind.nCodeType == CH_W_2021)
		{
			rDataWind.CodeParam.CH2019.bCross = FALSE;
			rDataWind.CodeParam.CH2019.bTorsion = FALSE;
		}
		else if (rDataWind.nCodeType == CH_W_2012)
		{
			rDataWind.CodeParam.CH2012.bCross = FALSE;
			rDataWind.CodeParam.CH2012.bTorsion = FALSE;
		}
	}
	else if (m_nWindType == 1)//cross
	{
		if (rDataWind.nCodeType == CH_W_2021)
		{
			rDataWind.CodeParam.CH2019.bAlongVibration = FALSE;
			rDataWind.CodeParam.CH2019.nAlongType = 0;
			rDataWind.CodeParam.CH2019.bTorsion = FALSE;
		}
		else if (rDataWind.nCodeType == CH_W_2012)
		{
			rDataWind.CodeParam.CH2012.bAlongVibration = FALSE;
			rDataWind.CodeParam.CH2012.nAlongType = 0;
			rDataWind.CodeParam.CH2012.bTorsion = FALSE;
		}
	}
	else if (m_nWindType == 2)//Torsion
	{
		if (rDataWind.nCodeType == CH_W_2021)
		{
			rDataWind.CodeParam.CH2019.bAlongVibration = FALSE;
			rDataWind.CodeParam.CH2019.nAlongType = 0;
			rDataWind.CodeParam.CH2019.bTorsion = TRUE;
			rDataWind.CodeParam.CH2019.bCross = FALSE;
		}
		else if (rDataWind.nCodeType == CH_W_2012)
		{
			rDataWind.CodeParam.CH2012.bAlongVibration = FALSE;
			rDataWind.CodeParam.CH2012.nAlongType = 0;
			rDataWind.CodeParam.CH2012.bTorsion = TRUE;
			rDataWind.CodeParam.CH2012.bCross = FALSE;
		}
	}
	m_LateralLoad.MakeWindLoad(rDataWind, TRUE, m_strFileName, TRUE, FALSE, FALSE, m_nComponent);

	///////////////////////////////////////////////////////////////////////////////
	// Launching the Text Editor;
	///////////////////////////////////////////////////////////////////////////////
	CString strTitle = _LS(IDS_WG_CMD__ADD2__WIND_LOAD_CALC_);
	CDBLib::RunTextEditor(m_strFileName, strTitle);
}

void CWindProfileCHDlg::OnWindPFBrowse()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, _LS(IDS_WG_CMD__ADDD__All_Files__________));
	if (dlg.DoModal() == IDOK)
		m_strFileName = dlg.GetPathName();
	UpdateData(FALSE);
}
