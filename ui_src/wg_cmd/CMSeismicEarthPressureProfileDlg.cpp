// CMSeismicEarthPressureProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSeismicEarthPressureProfileDlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
//#include "..\wg_db\wg_db_DataCtrl.h"
// #include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\EarthPressureLoad.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_MathFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSeismicEarthPressureProfileDlg dialog

CCMSeismicEarthPressureProfileDlg::CCMSeismicEarthPressureProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSeismicEarthPressureProfileDlg::IDD, pParent), m_LateralLoad(0, TRUE)
{
	m_strFileName = _T("");
	m_bGridInit = FALSE;
	m_CtrlProfileOption.RemoveAll();
	m_CtrlProfileOption.Add(IDC_CMD_EARTH_SEIS_REACTION_RDO);
	m_CtrlProfileOption.Add(IDC_CMD_EARTH_SEIS_DISPLACE_RDO);
	m_CtrlProfileOption.Add(IDC_CMD_EARTH_SEIS_PRESSURE_RDO);

	m_CtrlProfileEqui.RemoveAll();
	m_CtrlProfileEqui.Add(IDC_CMD_EARTH_SEIS_REACTION_RDO);
	m_CtrlProfileEqui.Add(IDC_CMD_EARTH_SEIS_DISPLACE_RDO);
	m_nMethodType = EN_EPSE_METHOD_EQUIVALENT;
	m_dFootingLevel = 0.0;
}


void CCMSeismicEarthPressureProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EARTHP_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_EARTHP_TABLE, m_wndGrid);
}


BEGIN_MESSAGE_MAP(CCMSeismicEarthPressureProfileDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_EARTH_SEIS_REACTION_RDO, OnChangeProfile)
	ON_BN_CLICKED(IDC_CMD_EARTH_SEIS_DISPLACE_RDO, OnChangeProfile)
	ON_BN_CLICKED(IDC_CMD_EARTH_SEIS_PRESSURE_RDO, OnChangeProfile)
	ON_BN_CLICKED(IDC_EARTHP_MAKE_SHEET, OnPFViewCalcSheet)
	ON_BN_CLICKED(IDC_EARTHP_BROWSE, OnPFBrowse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSeismicEarthPressureProfileDlg message handlers

BOOL CCMSeismicEarthPressureProfileDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	int nOptionType = 2;
	CDlgUtil::CtrlRadioSetCheck(this, m_CtrlProfileOption, nOptionType);
	CDlgUtil::CtrlEnableDisable(this, m_CtrlProfileEqui, m_nMethodType == EN_EPSE_METHOD_RESPONSE);


	//그래프 초기화 및 업데이트
	InitChart();
	UpdateChart();

// 	m_Graph.SetHeader(1, _T("S.P"));

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

void CCMSeismicEarthPressureProfileDlg::InitChart()
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
	pDisplay->SetRect(3, 0, 100, 93.0);
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
	pLabel->SetTextPosition(50.0, 91.5);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	m_Graph.AddComponent(pLabel);

	if (pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_DENSITY, strUnit))
	{
		strTemp.Format(_T("%s (%s)"), _LS(IDS_CMD_EARTHP_PROFILE_REACTION), strUnit);
		SetAxisLabelText(0, strTemp);
	}
	else
	{
		SetAxisLabelText(0, _LS(IDS_CMD_EARTHP_PROFILE_REACTION));
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

	AddTextPannelComponent();

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

void CCMSeismicEarthPressureProfileDlg::AddTextPannelComponent()
{
	SRGTextPanel* pTextPanel = new SRGTextPanel;
	//Text Panel Layout
	pTextPanel->SetXMargin(0.5); // decrease margin
	pTextPanel->SetYMargin(0.5);
	pTextPanel->SetMeasurement(CX_PERCENT);
	pTextPanel->SetRect(3.0, 95, 100.0, 100);
	pTextPanel->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pTextPanel->GetStyle()->SetInteriorStyle(CX_INTERIOR_NONE);
	//Title text style
	pTextPanel->SetUseTitle(TRUE);
	pTextPanel->SetTitleTextColor(CXCLR_RED);
	pTextPanel->GetTitleStyle()->SetInteriorStyle(CX_INTERIOR_NONE);// SetColor(NULL); // blue
	pTextPanel->SetTitleFontJustification(2);
	pTextPanel->SetTitleFontSize(9);  // 14 points
	pTextPanel->SetTitleFontFaceName(_LS(IDS_GRAPH_FONT_ARIAL)); // Title의 Logfont는 구할 수 없다.
// 	pTextPanel->SetTitleFontStyle(CX_FONT_BOLD);
	// Body text style
	pTextPanel->SetBodyTextColor(CXCLR_RED);
	pTextPanel->SetBodyFontSize(8);
	pTextPanel->SetBodyFontFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	//Text Panel Data
	pTextPanel->SetTitleText(_LS(IDS_CMD_EARTHP_PROFILE_FOOTING_LEVEL)); //Language 바꾸지 말것!!
	pTextPanel->SetBodyFontJustification(2);
	pTextPanel->SetBodyText(_T(""));
	pTextPanel->GetTitleStyle()->SetColor(CXCLR_WHITE); // blue
	pTextPanel->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent(pTextPanel);

}
void CCMSeismicEarthPressureProfileDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup * 2, x);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup * 2 + 1, y);

}

void CCMSeismicEarthPressureProfileDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel = (SRGraphLabel *)m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CCMSeismicEarthPressureProfileDlg::ChangeLabel(int nSelType)
{
	CString strUnit, strTemp;
	CUnitCtrl* pUnitCtrl = ((CDBDoc*)CDBDoc::GetDocPoint())->m_pUnitCtrl;

	if      (nSelType == 0 && pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_DENSITY, strUnit)) strTemp.Format(_T("%s (%s)\n"), _LS(IDS_CMD_EARTHP_PROFILE_REACTION), strUnit);
	else if (nSelType == 1 && pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit))  strTemp.Format(_T("%s (%s)\n"), _LS(IDS_CMD_EARTHP_PROFILE_DISPLACE), strUnit);
	else if (nSelType == 2 && pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_PRESSURE, strUnit))strTemp.Format(_T("%s (%s)\n"), _LS(IDS_CMD_EARTHP_PROFILE_PRESSURE), strUnit);
	else { strTemp =  _LS(IDS_CMD_EARTHP_PROFILE_REACTION); }
// 	strTemp += "*Red Line : Bottom Level of Footing";
	SetAxisLabelText(0, strTemp);
	//SetAxisLabelText(2, "*Red Line : Bottom Level of Footing");
	
}
void CCMSeismicEarthPressureProfileDlg::InitProfile(T_EPSE_D &rData, BOOL bInitData/*=TRUE*/)
{
    m_LateralLoad.MakeSeismicEarthPressure(rData, bInitData);

	m_pEpseD = &rData;    

	T_POSL_D PoslD;
	CAttrCtrl* pAttrCtrl = ((CDBDoc*)CDBDoc::GetDocPoint())->m_pAttrCtrl;
	if (!pAttrCtrl->GetPosl(m_pEpseD->KeyPosl, PoslD)) { ASSERT(0); PoslD.Initialize(); }
	m_nMethodType = PoslD.KDS2019.nEPSEMethod;

	T_POSP_D PospD;
	if (!pAttrCtrl->GetPosp(m_pEpseD->KeyPosp, PospD)) { ASSERT(0); PospD.Initialize(); }
	m_dFootingLevel = PospD.dFootingLevel;
	
}

// CString CCMSeismicEarthPressureProfileDlg::ConvertValToFmtStr(double Val, int nLimit)
// {
// 
// 	// nLimit = the Total Width of Output Value including deximal point and +/-Sign;
// 					 //   So Real Number of Digit above the point = nLimit -2;
// 	double RoundVal = 0.0;
// 	int    Sign = 0;
// 	CString Out, Fmt;
// 	int nPrecision = 0;
// 
// 	if (fabs(Val) <= 1e-6) { Out = "0.0"; return Out; }
// 
// 	int nDigits = 0;
// 	if (Val != 0) 	nDigits = log10(fabs(Val));
// 
// 	if (nDigits >= 0)
// 	{
// 		nDigits++;
// 		if (nLimit - 2 - nDigits > 0) 		nPrecision = nLimit - 2 - nDigits;
// 		else	nPrecision = 1;
// 	}
// 	else if (nDigits < 0)
// 	{
// 		nDigits = 1;
// 		nPrecision = nLimit - 2 - nDigits;
// 	}
// 
// 	if (Val >= 0) Sign = 1;
// 	else Sign = -1;
// 
// 	RoundVal = Val + 5.0*pow(10, -(nPrecision + 1))* Sign; // 반올림효과 고려
// 
// 	if (fabs(RoundVal) >= pow(10, (nLimit - 2) - nPrecision))   // Angle값은 +-90이다. 따라서 E포멧으로 가지 않는다.
// 	{
// 		if ((nLimit - 3 - 5) >= 1) Fmt.Format("%%%d.%de", nLimit, nLimit - 3 - 5);  //   exam. +1.245e-001
// 		else Fmt.Format("*****");                        // f로 안되면서 e로도 안되는 값/ ERROR!
// 	}
// 
// 	else Fmt.Format("%%%d.%df", nLimit, nPrecision);
// 
// 	Out.Format(Fmt, Val);
// 
// 	/////////////////////////////////////////////
// 	// removing meaningless zeros...
// 
// 	int nTemp = Out.GetLength();
// 
// 	for (int i = nTemp - 1; i > 3; i--)
// 	{
// 		if (Out[i] == '0' && Out[i - 1] != '.')	Out = Out.Left(i);
// 		else break;
// 	}
// 	return Out;
// }

void CCMSeismicEarthPressureProfileDlg::OnPFViewCalcSheet()
{
	UpdateData(TRUE);
	
    m_LateralLoad.MakeSeismicEarthPressure(*m_pEpseD, FALSE, TRUE, m_strFileName, TRUE, FALSE);

	///////////////////////////////////////////////////////////////////////////////
	// Launching the Text Editor;
	///////////////////////////////////////////////////////////////////////////////
	CString strTitle = _LS(IDS_WG_CMD__ADD2__EARTH_PRESSURE_CALC_);
	CDBLib::RunTextEditor(m_strFileName, strTitle);
}

void CCMSeismicEarthPressureProfileDlg::OnPFBrowse()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, _LS(IDS_WG_CMD__ADDD__All_Files__________));
	if (dlg.DoModal() == IDOK)
		m_strFileName = dlg.GetPathName();
	UpdateData(FALSE);
}

void CCMSeismicEarthPressureProfileDlg::UpdateChart()
{
	int i, nSize;
	nSize = m_pEpseD->arPresProfile.GetSize();
	m_Graph.KillData();

	if (nSize <= 1)
		return;

	int nOption = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_CtrlProfileOption, nOption);
	ChangeLabel(nOption);

	double dMinLeve = m_pEpseD->arPresProfile[0].dLevel;
	double dMaxLeve = m_pEpseD->arPresProfile[0].dLevel;
	double dMinPres = m_pEpseD->arPresProfile[0].dReactionForce;
	double dMaxPres = m_pEpseD->arPresProfile[0].dReactionForce;

	if (nOption==1)
	{
		dMinPres = m_pEpseD->arPresProfile[0].dDisplacement;
		dMaxPres = m_pEpseD->arPresProfile[0].dDisplacement;
	}
	else if (nOption==2)
	{
		dMinPres = m_pEpseD->arPresProfile[0].dPressure + m_pEpseD->arPresProfile[0].dAdditionalPressure;
		dMaxPres = m_pEpseD->arPresProfile[0].dPressure + m_pEpseD->arPresProfile[0].dAdditionalPressure;

	}

	for (i = 0; i < nSize; i++)
	{
		double dXValue = m_pEpseD->arPresProfile[i].dReactionForce;
		double dYValue = m_pEpseD->arPresProfile[i].dLevel;
		
		if      (nOption==1) dXValue = m_pEpseD->arPresProfile[i].dDisplacement;
		else if (nOption==2) dXValue = m_pEpseD->arPresProfile[i].dPressure  + m_pEpseD->arPresProfile[i].dAdditionalPressure;

		SetValue(i, 1, dXValue, dYValue);

		if (dMinLeve > dYValue) dMinLeve = dYValue;
		if (dMaxLeve < dYValue) dMaxLeve = dYValue;

		if (dMinPres > dXValue) dMinPres = dXValue;
		if (dMaxPres < dXValue) dMaxPres = dXValue;
	}

	if (dMinLeve > m_dFootingLevel) dMinLeve = m_dFootingLevel;
	if (dMaxLeve < m_dFootingLevel) dMaxLeve = m_dFootingLevel;



	if (dMinPres > 1E-5)
		dMinPres = 0.0;

	double dRoundOffMinLevel = CMathFunc::mathRoundOff(dMinLeve, 1);
	double dRoundOffMaxLevel = CMathFunc::mathRoundOff(dMaxLeve, 1);

	if (dMaxLeve - dRoundOffMaxLevel <= 0.0) dMaxLeve = dRoundOffMaxLevel;
	else dMaxLeve = dRoundOffMaxLevel + 1.0;

	if (dMinLeve -  dRoundOffMinLevel< 0.0) dMinLeve = dRoundOffMinLevel - 1.0;
	else dMinLeve = dRoundOffMinLevel;

	double dDiffPres = dMaxPres - dMinPres;
	int nRound = 1;
	if (dDiffPres > 10.0) nRound=1;
	else if (dDiffPres > 1.0) nRound = 2;
	else if (dDiffPres > 0.1) nRound = 3;
	else if (dDiffPres > 0.01) nRound = 4;
	else if (dDiffPres > 0.001) nRound = 5;
	else nRound = 6;

	double dRoundOffMinPress = CMathFunc::mathRoundOff(dMinPres, nRound);
	double dRoundOffMaxPress = CMathFunc::mathRoundOff(dMaxPres, nRound);

	double dMargin1 = pow(10, nRound==1 ? 0 : -(nRound-1));
	if (dMaxPres - dRoundOffMaxPress <= 0.0) dMaxPres = dRoundOffMaxPress;
	else dMaxPres = dRoundOffMaxPress + dMargin1;

	if (dMinPres -  dRoundOffMinPress< 0.0) dMinPres = dRoundOffMinPress - dMargin1;
	else dMinPres = dRoundOffMinPress;

	double dMargin = 0.0;
	if (m_pDisplay && fabs(dMinLeve - dMaxLeve) >= 1E-5)
	{
		dMargin = (dRoundOffMaxPress - dRoundOffMinPress) * 0.1;
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

	// Footing Level을 그려줌
	SetValue(0, 0, dMinPres, m_dFootingLevel);
	SetValue(1, 0, dMaxPres+dMargin, m_dFootingLevel);

	m_Graph.SetHeader(0, _T("Bottom Level of Footing"));

	m_GraphView.Invalidate();
}

void CCMSeismicEarthPressureProfileDlg::InitGrid()
{
	m_wndGrid.Initialize(m_pEpseD, m_nMethodType, this);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;

}

void CCMSeismicEarthPressureProfileDlg::OnDataChanged()
{
	if (m_pEpseD->arPresProfile.GetSize() > 1)
	{
		m_LateralLoad.MakeSeismicEarthPressure(*m_pEpseD, FALSE, FALSE, _T(""), TRUE);
		UpdateChart();
		m_wndGrid.MakeItemEx();
	}
	else
	{
		m_pEpseD->arPresProfile.RemoveAll();
		m_LateralLoad.MakeSeismicEarthPressure(*m_pEpseD, TRUE, FALSE, _T(""), TRUE);
		UpdateChart();
		m_wndGrid.MakeItemEx();
	}
}

void CCMSeismicEarthPressureProfileDlg::OnChangeProfile()
{
	UpdateChart();
}

void CCMSeismicEarthPressureProfileDlg::UpdateProfileData()
{
	UpdateChart();
}