#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisProfieDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"
#include "..\wg_common\wg_common_TBGrid.h"

#define KS_E_92  1
#define UBC_E_91 2
#define UBC_E_97 3
#define ATC306_E 4
#define KS_E_2000 5
#define EURO_E_1996 8
#define CH_E_2002 9

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSeisProfileDlg::CSeisProfileDlg(CWnd* pParent)
	: CDialogMove(CSeisProfileDlg::IDD, pParent), m_LateralLoad(0, TRUE)
{
	m_nComponent = 0;
	m_nProfileType = 0;
	m_strFileName = _T("");
	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;
}

void CSeisProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Radio(pDX, IDC_SEISPF_COMP_X, m_nComponent);
	DDX_Radio(pDX, IDC_SEISPF_PROFILE_FORCE, m_nProfileType);
	DDX_Text(pDX, IDC_SEISPF_EDT_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_SEISPF_TABLE, m_SeisPfGrid);
}

BEGIN_MESSAGE_MAP(CSeisProfileDlg, CDialogMove)
	ON_BN_CLICKED(IDC_SEISPF_COMP_X, OnChangeCompont)
	ON_BN_CLICKED(IDC_SEISPF_PROFILE_FORCE, OnChangeProfileType)
	ON_BN_CLICKED(IDC_SEISPF_ViewCalcSheet, OnSEISPFViewCalcSheet)
	ON_BN_CLICKED(IDC_SEISPF_COMP_Y, OnChangeCompont)
	ON_BN_CLICKED(IDC_SEISPF_COMP_XY, OnChangeCompont)
	ON_BN_CLICKED(IDC_SEISPF_COMP_SRSS, OnChangeCompont)
	ON_BN_CLICKED(IDC_SEISPF_PROFILE_SHEAR, OnChangeProfileType)
	ON_BN_CLICKED(IDC_SEISPF_PROFILE_MNT, OnChangeProfileType)
	ON_BN_CLICKED(IDC_SEISPF_BROWSE, OnSeispfBrowse)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
END_MESSAGE_MAP()

void CSeisProfileDlg::InitGrid(int nCode)
{
	// Grid Initialize
	m_SeisPfGrid.Initialize();

	// cell 바깥영역 배경색 지정
	m_SeisPfGrid.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정
	m_SeisPfGrid.ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	m_SeisPfGrid.ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	m_SeisPfGrid.ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	// Sample setup for the grid
	m_SeisPfGrid.GetParam()->EnableUndo(FALSE);

	m_SeisPfGrid.SetRowCount(m_LateralLoad.m_aSeisLoad_X.GetSize());
	m_SeisPfGrid.SetColCount(11);

	CRect rect;
	m_SeisPfGrid.GetClientRect(&rect);
	int cx = rect.Width() * 2;
	m_SeisPfGrid.SetColWidth(0, 0, cx*0.1); // Name
	m_SeisPfGrid.SetFrozenCols(0, 0);
	m_SeisPfGrid.SetColWidth(1, 4, cx*0.1);
	m_SeisPfGrid.SetColWidth(5, 8, cx*0.12);
	m_SeisPfGrid.SetRowHeight(0, 0, globalUtils.ScaleByDPI(30), NULL);

	m_SeisPfGrid.SetScrollBarMode(SB_VERT, gxnEnabled);
	m_SeisPfGrid.SetScrollBarMode(SB_HORZ, gxnEnabled);

	{
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 0), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Story_n_Name)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 1), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Weight)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 2), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Elev_)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 3), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Seismic_n_Force)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 4), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Added_n_Force)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 5), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Story_n_Force)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 6), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADDD__Story_n_Shear)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 7), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Overturn_g_nMoment)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 8), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Eccent_)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 9), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__Accidental_nTorsion)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 10), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__INHERENT_TORSION)));
		m_SeisPfGrid.SetStyleRange(CGXRange(0, 11), CGXStyle().SetValue(_LS(IDS_WG_CMD__ADD2__TOTAL_TORSION)));
	}

	m_SeisPfGrid.SetStyleRange(CGXRange().SetCols(0, 11), CGXStyle()
		.SetWrapText(TRUE)
		.SetEnabled(FALSE));

	CGXGridParam* param = m_SeisPfGrid.GetParam();
	param->EnableSelection(GX_SELFULL | GX_SELMULTIPLE);
	param->EnableTrackRowHeight(FALSE);

	UpdateGrid();
}

BOOL CSeisProfileDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if ( m_pDataSeis->ScaleFactor_X == 0.0 ) m_nComponent = 1;
	InitGrid(m_pDataSeis->nCodeType);
	InitChart();

	if ( m_LateralLoad.m_aSeisLoad_X.GetSize() < 1 )
	{
		m_GraphView.EnableWindow(FALSE);
	}

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	if ( !CDBDoc::EnableDynaReport() )
	{
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영
	}

	// FileName;
	CDBDoc* pDoc = ( CDBDoc* ) CDBDoc::GetDocPoint();
	m_strFileName = pDoc->GetPathName();
	if ( m_strFileName.IsEmpty() )
	{
		TCHAR* lpBuffer = new TCHAR[255];
		GetCurrentDirectory(255, lpBuffer);
		m_strFileName = lpBuffer;
		m_strFileName += _T("\\Untitled.spf");
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
		_tmakepath(path_buffer, drive, dir, fname, _T("spf"));
		m_strFileName = path_buffer;
	}

	UpdateData(FALSE);
	return TRUE;
}

BOOL CSeisProfileDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
			if ( pMsg->wParam == VK_RETURN )
			{
				if ( pMsg->wParam == VK_ESCAPE )
				{
					::TranslateMessage(pMsg);
				}
			}
			::DispatchMessage(pMsg);
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 'C')
		{
			if (GetFocus() == &m_SeisPfGrid)
			{
				CGXRangeList* pSelList = m_SeisPfGrid.GetParam()->GetRangeList();
				if (pSelList && pSelList->GetCount() > 0)
				{
					POSITION pos = pSelList->GetHeadPosition();
					if (pos)
					{
						CGXRange range = pSelList->GetNext(pos);
						CGXRange headerRange(0, range.left, 0, range.right);
						m_SeisPfGrid.SelectRange(headerRange);
						m_SeisPfGrid.Copy();

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
						m_SeisPfGrid.SelectRange(range);
					}
				}
				return TRUE;
			}
		}

	}
	return CDialogMove::PreTranslateMessage(pMsg);
}

void CSeisProfileDlg::InitChart()
{
	// Initialize m_Graph
	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component

	SRGraphBackground* pB = new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent(( SRGraphComponent * ) pB);

	// Add Display Component
	SRGraphDisplay *pDisplay = new SRGraphDisplay;
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

	//pDisplay->GetStyle()->SetShowXGrid(m_GridXFlag);
	//pDisplay->GetStyle()->SetShowYGrid(m_GridYFlag);

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

	/////////////////////////////////////////////////////////////
	// Attach graph view to dialog's placeholder and connect graph to graph view

	CWnd* wnd = GetDlgItem(IDC_SEISPF_PLACEHOLDER);
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

void CSeisProfileDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup * 2, x);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);

	m_Graph.SetValue(nIndex, nGroup * 2 + 1, y);
}

void CSeisProfileDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel = ( SRGraphLabel * ) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CSeisProfileDlg::InitSeisProfile(T_SEIS_K nKey, T_SEIS_D &rDataSeis)
{
	m_LateralLoad.MakeSeismicLoad(rDataSeis, FALSE, _T(""), TRUE);
	m_pDataSeis = &rDataSeis;
	m_Key = nKey;
}

void CSeisProfileDlg::OnChangeCompont()
{
	UpdateData(TRUE);
	UpdateGrid();
	UpdateChart();
}

void CSeisProfileDlg::OnChangeProfileType()
{
	UpdateData(TRUE);
	UpdateGrid();
	UpdateChart();
}

void CSeisProfileDlg::UpdateGrid()
{
	stSeisLoad seisload, seisload_x, seisload_y;
	CString sLevel, sWeight, sForce, sAddedFrc, sSeisLoad, sStoryShear, sOTMoment, sEccent, sTorsion, sInTorsion, sAllTorsion;
	for ( int i = 0; i < m_LateralLoad.m_aSeisLoad_X.GetSize(); i++ )
	{
		seisload_x = m_LateralLoad.m_aSeisLoad_X[i];
		seisload_y = m_LateralLoad.m_aSeisLoad_Y[i];

		switch ( m_nComponent )
		{
		case 0: // X
			{
				seisload = seisload_x;
			}
			break;
		case 1: // Y
			{
				seisload = seisload_y;
			}
			break;
		case 2: // XY
			{
				seisload = seisload_x;
				seisload.dEccent     = -1;
				seisload.dForce      = -1;
				seisload.dAddedFrc   = -1;
				seisload.dOTMoment   = -1;
				seisload.dSeisLoad   = -1;
				seisload.dStoryShear = -1;
				seisload.dTorsion    = seisload_x.dTorsion + seisload_y.dTorsion;
				seisload.dInTorsion  = seisload_x.dInTorsion + seisload_y.dInTorsion;
				seisload.dAllTorsion = seisload_x.dAllTorsion + seisload_y.dAllTorsion;
			}
			break;
		case 3: //SRSS
			{
				seisload = seisload_x;
				seisload.dEccent     = -1;
				seisload.dAddedFrc   = sqrt(seisload_x.dAddedFrc*seisload_x.dAddedFrc + seisload_y.dAddedFrc*seisload_y.dAddedFrc);
				seisload.dForce      = sqrt(seisload_x.dForce*seisload_x.dForce + seisload_y.dForce*seisload_y.dForce);
				seisload.dOTMoment   = sqrt(seisload_x.dOTMoment*seisload_x.dOTMoment + seisload_y.dOTMoment*seisload_y.dOTMoment);
				seisload.dSeisLoad   = sqrt(seisload_x.dSeisLoad*seisload_x.dSeisLoad + seisload_y.dSeisLoad*seisload_y.dSeisLoad);
				seisload.dStoryShear = sqrt(seisload_x.dStoryShear*seisload_x.dStoryShear + seisload_y.dStoryShear*seisload_y.dStoryShear);
				seisload.dTorsion    = seisload_x.dTorsion + seisload_y.dTorsion;
				seisload.dInTorsion  = seisload_x.dInTorsion + seisload_y.dInTorsion;
				seisload.dAllTorsion = seisload_x.dAllTorsion + seisload_y.dAllTorsion;
			}
			break;
		}

		switch ( m_nComponent )
		{
		case 0:  // X
		case 1:  // Y
			{
				sLevel      = ConvertValToFmtStr(seisload.dLevel, 9);
				sWeight     = ConvertValToFmtStr(seisload.dWeight, 10);
				sForce      = ConvertValToFmtStr(seisload.dForce, 10);
				sAddedFrc   = ConvertValToFmtStr(seisload.dAddedFrc, 10);
				sSeisLoad   = ConvertValToFmtStr(seisload.dSeisLoad, 10);
				sStoryShear = ConvertValToFmtStr(seisload.dStoryShear, 10);
				sOTMoment   = ConvertValToFmtStr(seisload.dOTMoment, 10);
				sEccent     = ConvertValToFmtStr(seisload.dEccent, 10);
				sTorsion    = ConvertValToFmtStr(seisload.dTorsion, 10);
				sInTorsion  = ConvertValToFmtStr(seisload.dInTorsion, 10);
				sAllTorsion = ConvertValToFmtStr(seisload.dAllTorsion, 10);
			}
			break;
		case 2:  // X & Y
			{
				sLevel      = ConvertValToFmtStr(seisload.dLevel, 9);
				sWeight     = ConvertValToFmtStr(seisload.dWeight, 10);
				sForce      = _T("   --   ");
				sAddedFrc   = _T("   --   ");
				sSeisLoad   = _T("   --   ");
				sStoryShear = _T("   --   ");
				sOTMoment   = _T("   --   ");
				sEccent     = _T("   --   ");
				sTorsion    = ConvertValToFmtStr(seisload.dTorsion, 10);
				sInTorsion  = ConvertValToFmtStr(seisload.dInTorsion, 10);
				sAllTorsion = ConvertValToFmtStr(seisload.dAllTorsion, 10);
			}
			break;
		case 3:  // SRSS
			{
				sLevel      = ConvertValToFmtStr(seisload.dLevel, 9);
				sWeight     = ConvertValToFmtStr(seisload.dWeight, 10);
				sForce      = ConvertValToFmtStr(seisload.dForce, 10);
				sAddedFrc   = ConvertValToFmtStr(seisload.dAddedFrc, 10);
				sSeisLoad   = ConvertValToFmtStr(seisload.dSeisLoad, 10);
				sStoryShear = ConvertValToFmtStr(seisload.dStoryShear, 10);
				sOTMoment   = ConvertValToFmtStr(seisload.dOTMoment, 10);
				sEccent     = _T("   --   ");
				sTorsion    = ConvertValToFmtStr(seisload.dTorsion, 10);
				sInTorsion  = ConvertValToFmtStr(seisload.dInTorsion, 10);
				sAllTorsion = ConvertValToFmtStr(seisload.dAllTorsion, 10);
			}
			break;
		}

	#if defined(_JP) || defined(_CH)
		m_SeisPfGrid.SetStyleRange(CGXRange(0).SetRows(0, 0), CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
		m_SeisPfGrid.ColHeaderStyle().SetFont(CGXFont_GC().SetBold(FALSE));
	#endif
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 0), CGXStyle().SetValue(seisload.sStoryName));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(sWeight));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sLevel));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(sForce));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(sAddedFrc));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(sSeisLoad));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(sStoryShear));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(sOTMoment));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(sEccent));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 9), CGXStyle().SetValue(sTorsion));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 10), CGXStyle().SetValue(sInTorsion));
		m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 11), CGXStyle().SetValue(sAllTorsion));

		if ( i == m_LateralLoad.m_aSeisLoad_X.GetSize() - 1 )
		{
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 0), CGXStyle().SetValue(seisload.sStoryName));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 1), CGXStyle().SetValue(_T("   --   ")));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 2), CGXStyle().SetValue(sLevel));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 3), CGXStyle().SetValue(_T("   --   ")));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 4), CGXStyle().SetValue(_T("   --   ")));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 5), CGXStyle().SetValue(_T("   --   ")));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 6), CGXStyle().SetValue(sStoryShear));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 7), CGXStyle().SetValue(sOTMoment));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 8), CGXStyle().SetValue(_T("   --   ")));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 9), CGXStyle().SetValue(_T("   --   ")));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 10), CGXStyle().SetValue(_T("   --   ")));
			m_SeisPfGrid.SetStyleRange(CGXRange(i + 1, 11), CGXStyle().SetValue(_T("   --   ")));
		}
	}
}

void CSeisProfileDlg::UpdateChart()
{
	stSeisLoad seisload, seisload_x, seisload_y;
	m_Graph.KillData();
	double dValue;
	int index;
	for ( int i = 0; i < m_LateralLoad.m_aSeisLoad_X.GetSize(); i++ )
	{
		index = m_LateralLoad.m_aSeisLoad_X.GetSize() - 1 - i;
		seisload_x = m_LateralLoad.m_aSeisLoad_X[i];
		seisload_y = m_LateralLoad.m_aSeisLoad_Y[i];

		m_Graph.GetGroup(0)->GetIndex(index)->SetAnnotation(seisload_x.sStoryName);

		switch ( m_nComponent )
		{
		case 0: // X
			{
				seisload = seisload_x;
			}
			break;
		case 1: // Y
			{
				seisload = seisload_y;
			}
			break;
		case 2: // XY
			{
				seisload = seisload_x;
				switch ( m_nProfileType )
				{
				case 0: // Force;
					{
						dValue = seisload_y.dSeisLoad;
					}
					break;
				case 1: // Shear;
					{
						dValue = seisload_y.dStoryShear;
					}
					break;
				case 2: // moment;
					{
						dValue = seisload_y.dOTMoment;
					}
				}
				m_Graph.SetValue(index, 1, dValue);
			}
			break;
		case 3: //SRSS
			{
				seisload = seisload_x;
				seisload.dOTMoment   = sqrt(seisload_x.dOTMoment*seisload_x.dOTMoment + seisload_y.dOTMoment*seisload_y.dOTMoment);
				seisload.dSeisLoad   = sqrt(seisload_x.dSeisLoad*seisload_x.dSeisLoad + seisload_y.dSeisLoad*seisload_y.dSeisLoad);
				seisload.dStoryShear = sqrt(seisload_x.dStoryShear*seisload_x.dStoryShear + seisload_y.dStoryShear*seisload_y.dStoryShear);
			}
			break;
		}

		switch ( m_nProfileType )
		{
		case 0: // Force;
			{
				dValue = seisload.dSeisLoad;
				SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Seismic_Force));
			}
			break;
		case 1: // Shear;
			{
				dValue = seisload.dStoryShear;
				SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Story_Shear));
			}
			break;
		case 2: // moment;
			{
				dValue = seisload.dOTMoment;
				SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Overturn_g_Moment));
			}
		}

		m_Graph.SetValue(index, 0, dValue);
	}

	m_GraphView.Invalidate();
}

void CSeisProfileDlg::OnBtnDyGen()
{
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_SM_PROF;   // Wind Load Profile
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = m_Key;
	UchtD.Data.Grph.nGraphType = m_nComponent;
	UchtD.Data.Grph.nOctBandOption = m_nProfileType;
	UchtD.bLogX = m_LogFlagX;
	UchtD.bLogY = m_LogFlagY;

	CDBDoc* pDoc = ( CDBDoc* ) CDBDoc::GetDocPoint();

	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_SLPROF_NAME); //_T("Wind Load Profile");
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_SLPROF_NAME), nCount + 1);

	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if ( dlg.DoModal() == IDOK )
	{
		UchtD.strName = csName;
		if ( !pDoc->m_pDataCtrl->AddUcht(UchtD) ) { ASSERT(0); return; }
	}
}

CString CSeisProfileDlg::ConvertValToFmtStr(double Val, int nLimit)
{
	// nLimit = the Total Width of Output Value including deximal point and +/-Sign;
	//   So Real Number of Digit above the point = nLimit -2;
	double RoundVal = 0.0;
	int    Sign = 0;
	CString Out, Fmt;
	int nPrecision = 0;

	if ( fabs(Val) <= 1e-6 ) { Out = _T("0.0"); return Out; }

	int nDigits = 0;
	if ( Val != 0 )
	{
		nDigits = log10(fabs(Val));
	}

	if ( nDigits >= 0 )
	{
		nDigits++;
		if ( nLimit - 2 - nDigits > 0 )
		{
			nPrecision = nLimit - 2 - nDigits;
		}
		else
		{
			nPrecision = 1;
		}
	}
	else if ( nDigits < 0 )
	{
		nDigits = 1;
		nPrecision = nLimit - 2 - nDigits;
	}

	if ( Val >= 0 )
	{
		Sign = 1;
	}
	else
	{
		Sign = -1;
	}

	RoundVal = Val + 5.0*pow(10, -( nPrecision + 1 ))* Sign; // 반올림효과 고려

	if ( fabs(RoundVal) >= pow(10, ( nLimit - 2 ) - nPrecision) )   // Angle값은 +-90이다. 따라서 E포멧으로 가지 않는다.
	{
		if ( ( nLimit - 3 - 5 ) >= 1 )
		{
			Fmt.Format(_T("%%%d.%de"), nLimit, nLimit - 3 - 5);  //   exam. +1.245e-001
		}
		else
		{
			Fmt.Format(_T("*****"));                        // f로 안되면서 e로도 안되는 값/ ERROR!
		}
	}
	else
	{
		Fmt.Format(_T("%%%d.%df"), nLimit, nPrecision);
	}

	Out.Format(Fmt, Val);

	/////////////////////////////////////////////
	// removing meaningless zeros...
	int nTemp = Out.GetLength();
	for ( int i = nTemp - 1; i > 3; i-- )
	{
		if ( Out[i] == '0' && Out[i - 1] != '.' )
		{
			Out = Out.Left(i);
		}
		else break;
	}

	return Out;
}

void CSeisProfileDlg::OnSEISPFViewCalcSheet()
{
	UpdateData(TRUE);
	if ( !m_LateralLoad.MakeSeismicLoad(*m_pDataSeis, TRUE, m_strFileName, TRUE) )
	{
		if ( m_LateralLoad.GetStoryCount() < 1 )
		{
			CString csMsg = _LS(IDS_WG_CMD__ADDD__NO_Seismic_Load_Profile);
			AfxMessageBox(csMsg, MB_ICONINFORMATION);
		}
		return;
	}
	CString strTitle = _LS(IDS_WG_CMD__ADD2__SEIS_LOAD_CALC_);
	CDBLib::RunTextEditor(m_strFileName, strTitle);
}

void CSeisProfileDlg::OnSeispfBrowse()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, _LS(IDS_WG_CMD__ADDD__All_Files__________));
	if ( dlg.DoModal() == IDOK )
	{
		m_strFileName = dlg.GetPathName();
	}
	UpdateData(FALSE);
}