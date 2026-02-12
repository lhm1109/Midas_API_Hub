// StaticEarthProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStaticEarthPressureProfileDlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\EarthPressureLoad.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStaticEarthPressureProfileDlg dialog

CCMStaticEarthPressureProfileDlg::CCMStaticEarthPressureProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMStaticEarthPressureProfileDlg::IDD, pParent), m_LateralLoad(0, TRUE)
{
	m_strFileName = _T("");
	m_bGridInit = FALSE;
}


void CCMStaticEarthPressureProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EARTHP_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_EARTHP_TABLE, m_wndGrid);
}


BEGIN_MESSAGE_MAP(CCMStaticEarthPressureProfileDlg, CDialogMove)
	ON_BN_CLICKED(IDC_EARTHP_MAKE_SHEET, OnPFViewCalcSheet)
	ON_BN_CLICKED(IDC_EARTHP_BROWSE, OnPFBrowse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStaticEarthPressureProfileDlg message handlers

BOOL CCMStaticEarthPressureProfileDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	//그래프 초기화 및 업데이트
	InitChart();
	UpdateChart();

	//그리드 초기화 및 업데이트
	InitGrid();
	m_wndGrid.MakeItemEx();

	///////////////////////////////////////////////////
	// FileName;
	TCHAR *lpBuffer;
	lpBuffer = new TCHAR[255];

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();

	m_strFileName = pDoc->GetPathName();
	if (m_strFileName == _T(""))
	{
		GetCurrentDirectory(255, lpBuffer);
		m_strFileName = lpBuffer;
		m_strFileName += _T("\\Untitled.epf");
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
		_tmakepath(path_buffer, drive, dir, fname, _T("epf"));
		m_strFileName = path_buffer;
	}

	UpdateData(FALSE);

	delete[] lpBuffer;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStaticEarthPressureProfileDlg::InitChart()
{
	////////////////////////////////////////////////////////////////
	// Initialize m_Graph

	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component
	//
	SRGraphBackground	*pB = new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *)pB);

	// Add Display Component
	//

	SRGraphDisplay *pDisplay = new SRGraphDisplay;
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetRect(3, 0, 100, 97.0);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));

	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);
	pDisplay->GetStyle()->SetUseMaxScale(TRUE);

	m_pDisplay = pDisplay;
	m_Graph.AddComponent(pDisplay);

	CString strTemp;
	CString strUnit;
	CUnitCtrl* pUnitCtrl = ((CDBDoc*)CDBDoc::GetDocPoint())->m_pUnitCtrl;
	// Add Label Component for X-Axis
	//
	SRGraphLabel	*pLabel = new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::TopCenter);
	pLabel->SetTextPosition(50.0, 93.5);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	m_Graph.AddComponent(pLabel);

	if (pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_PRESSURE, strUnit))
	{
		strTemp.Format(_T("%s (%s)"), _LS(IDS_CMD_EARTHP_PROFILE_EARTH_PRESSURE), strUnit);
		SetAxisLabelText(0, strTemp);
	}
	else
	{
		SetAxisLabelText(0, _LS(IDS_CMD_EARTHP_PROFILE_EARTH_PRESSURE));
	}

	// Add Label Component for Y-Axis
	//
	pLabel = new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(4.0, 50.0);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent(pLabel);
	if (pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit))
	{
		strTemp.Format(_T("%s (%s)"), _LS(IDS_WG_CMD__ADDD__Level), strUnit);
		SetAxisLabelText(1, strTemp);
	}
	else
	{
		SetAxisLabelText(1, _LS(IDS_WG_CMD__ADDD__Level));
	}


	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview

	CWnd* wnd = GetDlgItem(IDC_EARTH_PLACEHOLDER);
	if (wnd == NULL || ::IsWindow(wnd->GetSafeHwnd()) == FALSE)
	{
		ASSERT(0);
		return;
	}

	CRect rc;

	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left += 2;
	rc.top += 2;
	rc.right -= 2;
	rc.bottom -= 2;

	m_GraphView.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rc, this, IDC_EARTH_PLACEHOLDER);
	m_GraphView.m_pGraph = &m_Graph;

	m_GraphView.Invalidate();
}

void CCMStaticEarthPressureProfileDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup * 2, x);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup * 2 + 1, y);

}

void CCMStaticEarthPressureProfileDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel = (SRGraphLabel *)m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CCMStaticEarthPressureProfileDlg::InitProfile(T_EPST_D &rData, BOOL bInitData, double* pDir)
{
    m_LateralLoad.MakeStaticEarthPressure(rData, bInitData);

	m_pEpstD = &rData;
}

CString CCMStaticEarthPressureProfileDlg::ConvertValToFmtStr(double Val, int nLimit)
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

	RoundVal = Val + 5.0*pow(10, -(nPrecision + 1))* Sign; // 반올림효과 고려

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

void CCMStaticEarthPressureProfileDlg::OnPFViewCalcSheet()
{
	UpdateData(TRUE);
	
    m_LateralLoad.MakeStaticEarthPressure(*m_pEpstD, FALSE, TRUE, m_strFileName, TRUE, FALSE);

	///////////////////////////////////////////////////////////////////////////////
	// Launching the Text Editor;
	///////////////////////////////////////////////////////////////////////////////
	CString strTitle = _LS(IDS_WG_CMD__ADD2__EARTH_PRESSURE_CALC_);
	CDBLib::RunTextEditor(m_strFileName, strTitle);
}

void CCMStaticEarthPressureProfileDlg::OnPFBrowse()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, _LS(IDS_WG_CMD__ADDD__All_Files__________));
	if (dlg.DoModal() == IDOK)
		m_strFileName = dlg.GetPathName();
	UpdateData(FALSE);
}

void CCMStaticEarthPressureProfileDlg::UpdateChart()
{
	int i, nSize;
	nSize = m_pEpstD->arPresProfile.GetSize();
	m_Graph.KillData();

	if (nSize <= 1)
		return;

	double dMinLeve = m_pEpstD->arPresProfile[0].dLevel;
	double dMaxLeve = m_pEpstD->arPresProfile[0].dLevel;
	double dMinPres = m_pEpstD->arPresProfile[0].dSoilPressure + m_pEpstD->arPresProfile[0].dAdditionalPressure;
	double dMaxPres = m_pEpstD->arPresProfile[0].dSoilPressure + m_pEpstD->arPresProfile[0].dAdditionalPressure;

	double dPressure = 0.0, dLevel = 0.0;
	for (i = 0; i < nSize; i++)
	{
		dLevel = m_pEpstD->arPresProfile[i].dLevel;
		dPressure = m_pEpstD->arPresProfile[i].dSoilPressure + m_pEpstD->arPresProfile[i].dAdditionalPressure;
		SetValue(i, 0, dPressure, dLevel);

		if (dMinLeve > dLevel) dMinLeve = dLevel;
		if (dMaxLeve < dLevel) dMaxLeve = dLevel;
		if (dMinPres > dPressure) dMinPres = dPressure;
		if (dMaxPres < dPressure) dMaxPres = dPressure;
	}

	if (dMinPres > 1E-5) dMinPres = 0.0;

	if (m_pDisplay && fabs(dMinLeve - dMaxLeve) >= 1E-5)
	{
		double dMargin = (dMaxPres - dMinPres) * 0.1;
		m_pDisplay->SetMinRangeY(dMinLeve);
		m_pDisplay->SetMaxRangeY(dMaxLeve);
		m_pDisplay->SetMinRangeX(dMinPres);
		m_pDisplay->SetMaxRangeX(dMaxPres + dMargin);
	}
	else
	{
		m_pDisplay->SetMinRangeY(0.0);
		m_pDisplay->SetMaxRangeY(1.0);
		m_pDisplay->SetMinRangeX(0.0);
		m_pDisplay->SetMaxRangeX(1.0);
	}
	m_GraphView.Invalidate();
}

void CCMStaticEarthPressureProfileDlg::InitGrid()
{
	m_wndGrid.Initialize(m_pEpstD, this);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;

}
void CCMStaticEarthPressureProfileDlg::UpdateProfileData()
{
	UpdateChart();
}