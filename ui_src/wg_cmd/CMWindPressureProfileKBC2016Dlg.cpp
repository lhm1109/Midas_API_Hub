// WindProfileKBC2016Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindPressureProfileKBC2016Dlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureProfileKBC2016Dlg dialog

CCMWindPressureProfileKBC2016Dlg::CCMWindPressureProfileKBC2016Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMWindPressureProfileKBC2016Dlg::IDD, pParent), m_LateralLoad(0, TRUE)
{
	m_nDir = 0;
	m_strFileName = _T("");
	m_bGridInit = FALSE;
}


void CCMWindPressureProfileKBC2016Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_WINDPF_DIR_ALONG, m_nDir);
	DDX_Text(pDX, IDC_WINDP_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_WINDP_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_WINDP_TABLE, m_wndGrid);
}


BEGIN_MESSAGE_MAP(CCMWindPressureProfileKBC2016Dlg, CDialogMove)
	ON_BN_CLICKED(IDC_WINDPF_DIR_ALONG      , OnChangeDir)
	ON_BN_CLICKED(IDC_WINDPF_DIR_ACROSS     , OnChangeDir)
	ON_BN_CLICKED(IDC_WINDPF_DIR_TORSIONAL  , OnChangeDir)
	ON_BN_CLICKED(IDC_WINDP_MAKE_SHEET, OnWINDPFViewCalcSheet)
	ON_BN_CLICKED(IDC_WINDP_BROWSE,     OnWindPFBrowse)
	ON_BN_CLICKED(IDC_WINDP_DYGEN,      OnBtnDyGen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureProfileKBC2016Dlg message handlers

BOOL CCMWindPressureProfileKBC2016Dlg::OnInitDialog() 
{  
	CDialogMove::OnInitDialog();

    const BOOL bEnableAlong = [](const T_WDPR_D& WdprD) -> BOOL
    {
        switch ( WdprD.nCodeType )
        {
        case KBC_W_2016:
            return WdprD.CodeParam.KBC2016.bAlongWind;
        case KDS_W_2019:
            return WdprD.CodeParam.KDS2019.bAlongWind;
        case KDS_W_2022:
            return WdprD.CodeParam.KDS2021.bAlongWind;
        default:
            return FALSE;
        }
    }(*m_pWdprD);

    const BOOL bEnableAcross = [] (const T_WDPR_D& WdprD) -> BOOL
    {
        switch ( WdprD.nCodeType )
        {
        case KBC_W_2016:
            {
                const T_WDPR_KBC2016& CurWdpr = WdprD.CodeParam.KBC2016;
                return !CurWdpr.bAutoForceCoef && (CurWdpr.nBuildingType==0 || CurWdpr.bAcrossWind);
            }
        case KDS_W_2019:
            {
                const T_WDPR_KBC2016& CurWdpr = WdprD.CodeParam.KDS2019;
                return !CurWdpr.bAutoForceCoef && (CurWdpr.nBuildingType==0 || CurWdpr.bAcrossWind);
            }
        case KDS_W_2022:
            {
                const T_WDPR_KDS2021& CurWdpr = WdprD.CodeParam.KDS2021;
                return !CurWdpr.bAutoForceCoef && (CurWdpr.nBuildingType==0 || CurWdpr.bAcrossWind);
            }
        default:
            return FALSE;
        }
    }(*m_pWdprD);

    const BOOL bEnableTorsional = [] (const T_WDPR_D& WdprD) -> BOOL
    {
        switch ( WdprD.nCodeType )
        {
        case KBC_W_2016:
            {
                const T_WDPR_KBC2016& CurWdpr = WdprD.CodeParam.KBC2016;
                return CurWdpr.bAutoForceCoef && CurWdpr.bTorsionalWind;
            }
        case KDS_W_2019:
            {
                const T_WDPR_KBC2016& CurWdpr = WdprD.CodeParam.KDS2019;
                return CurWdpr.bAutoForceCoef && CurWdpr.bTorsionalWind;
            }
        case KDS_W_2022:
            {
                const T_WDPR_KDS2021& CurWdpr = WdprD.CodeParam.KDS2021;
                return CurWdpr.bAutoForceCoef && CurWdpr.bTorsionalWind;
            }
        default:
            return FALSE;
        }
    }(*m_pWdprD);

	CDlgUtil::CtrlEnableDisable(this, IDC_WINDPF_DIR_ALONG, bEnableAlong);
	CDlgUtil::CtrlEnableDisable(this, IDC_WINDPF_DIR_ACROSS, bEnableAcross);
	CDlgUtil::CtrlEnableDisable(this, IDC_WINDPF_DIR_TORSIONAL, bEnableTorsional);

    m_nDir = [](const T_WDPR_D& WdprD) -> int
    {
        switch ( WdprD.nCodeType )
        {
        case KBC_W_2016:
            {
                const T_WDPR_KBC2016& CurWdpr = WdprD.CodeParam.KBC2016;                
                if ( !CurWdpr.bAlongWind )
                {
                    return (CurWdpr.bAcrossWind) ? 1 : 2;
                }
                return 0;
            }
        case KDS_W_2019:
            {
                const T_WDPR_KBC2016& CurWdpr = WdprD.CodeParam.KDS2019;                
                if ( !CurWdpr.bAlongWind )
                {
                    return (CurWdpr.bAcrossWind) ? 1 : 2;
                }
                return 0;
            }
        case KDS_W_2022:
            {
                const T_WDPR_KDS2021& CurWdpr = WdprD.CodeParam.KDS2021;
                if ( !CurWdpr.bAlongWind )
                {
                    return (CurWdpr.bAcrossWind) ? 1 : 2;                    
                }
                return 0;
            }
        default:
            return FALSE;
        }
    }(*m_pWdprD);
    	
	//그래프 초기화 및 업데이트
	InitChart();
	UpdateChart();
	
	//그리드 초기화 및 업데이트
	InitGrid();
	m_wndGrid.SetWindDir(m_nDir);
	m_wndGrid.MakeItemEx();

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));
	m_btnDyGen.ShowWindow(SW_HIDE);//일단 숨기자.....

	if(!CDBDoc::EnableDynaReport())
	m_btnDyGen.ShowWindow(SW_HIDE);

	///////////////////////////////////////////////////
	// FileName;
	TCHAR *lpBuffer;
	lpBuffer = new TCHAR[255];

	CDBDoc* pDoc  = (CDBDoc*)CDBDoc::GetDocPoint();
 
	m_strFileName = pDoc->GetPathName();
	if(m_strFileName == _T(""))
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
	 _tcscpy(path_buffer,m_strFileName);
	 m_strFileName.ReleaseBuffer();
		 
	 _tsplitpath( path_buffer, drive, dir, fname, ext );
	 _tmakepath( path_buffer, drive, dir,fname,_T("wpf") );
	 m_strFileName = path_buffer; 
	}
	
	UpdateData(FALSE);
	
	delete[] lpBuffer;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// BOOL CCMWindPressureProfileKBC2016Dlg::PreTranslateMessage(MSG* pMsg) 
// {
// 	// TODO: Add your specialized code here and/or call the base class
// 	if( pMsg->message == WM_KEYDOWN )
//   {
// 	  if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
//     {
// 		  if(pMsg->wParam == VK_RETURN)
//       if(pMsg->wParam == VK_ESCAPE)
//       
//       ::TranslateMessage(pMsg);
//       ::DispatchMessage(pMsg);
// 			return TRUE;        
//       // DO NOT process further
//     }
//   }
// 	return CDialogMove::PreTranslateMessage(pMsg);
// }

void CCMWindPressureProfileKBC2016Dlg::InitChart()
{
	////////////////////////////////////////////////////////////////
	// Initialize m_Graph
	
	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component
	//
	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *) pB);
	
	// Add Display Component
	//
	
	SRGraphDisplay *pDisplay=new SRGraphDisplay;	
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
	SRGraphLabel	*pLabel=new SRGraphLabel;
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

	if(pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_PRESSURE, strUnit))
	{
		strTemp.Format(_T("%s (%s)"), _LS(IDS_CMD_WINDP_WINDPRESSURE), strUnit);
		SetAxisLabelText(0, strTemp);
	}
	else
	{
		SetAxisLabelText(0, _LS(IDS_CMD_WINDP_WINDPRESSURE));
	}

	// Add Label Component for Y-Axis
	//
	pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(4.0, 50.0);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	//pLabel->GetStyle()->SetColor(CXCLR_BACKGRND);
	m_Graph.AddComponent(pLabel);
	if(pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit))
	{
		strTemp.Format(_T("%s (%s)"), _LS(IDS_WG_CMD__ADDD__Height), strUnit);
		SetAxisLabelText(1, strTemp);
	}
	else
	{
		SetAxisLabelText(1,_LS(IDS_WG_CMD__ADDD__Height));
	}

	
	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview
	
	CWnd* wnd = GetDlgItem(IDC_WINDP_PLACEHOLDER);
	if(wnd == NULL || ::IsWindow(wnd->GetSafeHwnd()) == FALSE)
	{
		ASSERT(0);
		return;
	}

	CRect rc;
	
	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left +=2;
	rc.top +=2;
	rc.right -= 2;
	rc.bottom -= 2;
	
	m_GraphView.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_WINDP_PLACEHOLDER);
	m_GraphView.m_pGraph = &m_Graph;
	
	m_GraphView.Invalidate();
}

void CCMWindPressureProfileKBC2016Dlg::SetValue(int nIndex, int nGroup, double x,double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);

// 	m_Graph.SetValue(nIndex, nGroup*2, x);
// 	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);	
// 	m_Graph.SetValue(nIndex, nGroup*2+1, y);	
}

void CCMWindPressureProfileKBC2016Dlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}
/*
void CCMWindPressureProfileKBC2016Dlg::OnETCSPFCCHARTXLog() 
{
		
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLogX(m_btnXLog.GetCheck());
	
	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);   
	  pD->SetMinRangeX(0.001);   
	}
	else 
	 	pD->GetStyle()->SetScaleLimit(FALSE);
	
	m_GraphView.Invalidate();		
}

void CCMWindPressureProfileKBC2016Dlg::OnETCSPFCCHARTYLog() 
{
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLog(m_btnYLog.GetCheck());

	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);
		pD->SetMinRangeX(0.001);
	}
	else 
	 	pD->GetStyle()->SetScaleLimit(FALSE);

	m_GraphView.Invalidate();		
}
*/
void CCMWindPressureProfileKBC2016Dlg::InitWindProfile(T_WDPR_D &rDataWind, double* pWindDir)
{
	m_LateralLoad.MakeWindPressure(rDataWind, FALSE, _T(""), TRUE, FALSE, pWindDir);
	m_pWdprD = &rDataWind;
}

void CCMWindPressureProfileKBC2016Dlg::OnBtnDyGen()
{  
//   T_UCHT_D UchtD;
//   UchtD.nType = D_TYPE_WD_PROF;   // Wind Load Profile
//   UchtD.SetType(UchtD.nType);
//   UchtD.nKey = m_Key;
//   UchtD.Data.Grph.nGraphType = m_nComponent;
//   UchtD.Data.Grph.nOctBandOption = m_nProfileType;
//   UchtD.bLogX = m_LogFlagX;
//   UchtD.bLogY = m_LogFlagY;
//   
//   CDBDoc* pDoc  = (CDBDoc*)CDBDoc::GetDocPoint();
// 
//   CString csName; csName.Empty();
//   int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
//   if(nCount==0) csName = _ULS('WLProf');//_T("Wind Load Profile");
//   else csName.Format(_ULS('WLProf%d'), nCount+1);
//   
//   CDynamicReportTemplateDlg dlg;
//   dlg.SetNamePtr(&csName);
//   if(dlg.DoModal() == IDOK)
//   {
//     UchtD.strName = csName;
//     if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) { ASSERT(0); return; }
//   }
	
}

CString CCMWindPressureProfileKBC2016Dlg::ConvertValToFmtStr(double Val,int nLimit)
{
	 
		// nLimit = the Total Width of Output Value including deximal point and +/-Sign;
						 //   So Real Number of Digit above the point = nLimit -2;
		double RoundVal=0.0;
		int    Sign=0;
		CString Out,Fmt;
		int nPrecision=0;

		if(fabs(Val) <= 1e-6) { Out = _T("0.0"); return Out;}

		int nDigits= 0;
		if(Val !=0) 	nDigits = log10(fabs(Val));

		if(nDigits >= 0)
		{
			nDigits ++;
			if(nLimit-2 - nDigits > 0) 		nPrecision = nLimit-2 - nDigits ;
			else	nPrecision = 1;
		}
		else if(nDigits < 0)
		{
			nDigits = 1;
			nPrecision = nLimit-2 - nDigits ;			
		}
		
		if(Val >= 0) Sign = 1;
		else Sign = -1;
			
		RoundVal = Val + 5.0*pow(10, -(nPrecision+1) )* Sign; // 반올림효과 고려
	
		if( fabs(RoundVal) >= pow(10, (nLimit-2)-nPrecision ) )   // Angle값은 +-90이다. 따라서 E포멧으로 가지 않는다.
		{
				if((nLimit-3-5) >= 1) Fmt.Format(_T("%%%d.%de"),nLimit,nLimit-3-5);  //   exam. +1.245e-001
				else Fmt.Format(_T("*****"));                        // f로 안되면서 e로도 안되는 값/ ERROR!
		}

		else Fmt.Format(_T("%%%d.%df"),nLimit,nPrecision);

		Out.Format(Fmt,Val);

		/////////////////////////////////////////////
		// removing meaningless zeros...

		int nTemp = Out.GetLength();

		for(int i=nTemp-1; i>3; i--)
		{
			if(Out[i] == '0' && Out[i-1] != '.')	Out = Out.Left(i);
			else break;
		}
		return Out;  
}

void CCMWindPressureProfileKBC2016Dlg::OnChangeDir() 
{
	UpdateData(TRUE);

	m_pWdprD->arPresProfile.RemoveAll();    
	m_LateralLoad.MakeWindPressure(*m_pWdprD,FALSE,_T(""),TRUE);
	UpdateChart();
	m_wndGrid.SetWindDir(m_nDir);
	m_wndGrid.MakeItemEx();
}

void CCMWindPressureProfileKBC2016Dlg::OnWINDPFViewCalcSheet() 
{
	UpdateData(TRUE);
	m_LateralLoad.MakeWindPressure(*m_pWdprD,TRUE,m_strFileName,TRUE, FALSE);
	
	///////////////////////////////////////////////////////////////////////////////
	// Launching the Text Editor;
	///////////////////////////////////////////////////////////////////////////////
	CString strTitle = _LS(IDS_WG_CMD__ADD2__WIND_LOAD_CALC_);
	CDBLib::RunTextEditor(m_strFileName, strTitle);
}
	
void CCMWindPressureProfileKBC2016Dlg::OnWindPFBrowse() 
{
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY,_LS(IDS_WG_CMD__ADDD__All_Files__________));
	if(dlg.DoModal()==IDOK)
		m_strFileName=dlg.GetPathName();
	UpdateData(FALSE);
}

void CCMWindPressureProfileKBC2016Dlg::UpdateChart()
{
	int i, nSize;
	nSize = m_pWdprD->arPresProfile.GetSize();
	m_Graph.KillData();

	if(nSize <= 1)
		return;

	double dMinLeve = m_pWdprD->arPresProfile[0].dLevel;
	double dMaxLeve = m_pWdprD->arPresProfile[0].dLevel;
	double dMinPres = m_pWdprD->arPresProfile[0].dPressure;
	double dMaxPres = m_pWdprD->arPresProfile[0].dPressure;
	
	if (m_nDir==1)
	{
		dMinPres = m_pWdprD->arPresProfile[0].dPressureA;
		dMaxPres = m_pWdprD->arPresProfile[0].dPressureA;
	}
	else if (m_nDir==2)
	{
		dMinPres = m_pWdprD->arPresProfile[0].dPressureT;
		dMaxPres = m_pWdprD->arPresProfile[0].dPressureT;
	}


	for (i = 0; i < nSize; i++)
	{
		double dLevel = m_pWdprD->arPresProfile[i].dLevel;
		double dPressure = 0.0;
		
		if      (m_nDir==0) dPressure =  m_pWdprD->arPresProfile[i].dPressure  +  m_pWdprD->arPresProfile[i].dAdditionalPressure;
		else if (m_nDir==1) dPressure =  m_pWdprD->arPresProfile[i].dPressureA +  m_pWdprD->arPresProfile[i].dAdditionalPressureA;
		else if (m_nDir==2) dPressure =  m_pWdprD->arPresProfile[i].dPressureT +  m_pWdprD->arPresProfile[i].dAdditionalPressureT;
		else ASSERT(0);

		SetValue(i, 0, dPressure, dLevel);

		if (dMinLeve > dLevel) dMinLeve = dLevel;
		if (dMaxLeve < dLevel) dMaxLeve = dLevel;
		if (dMinPres > dPressure) dMinPres = dPressure;
		if (dMaxPres < dPressure) dMaxPres = dPressure;
	}

	if(dMinPres > 1E-5) dMinPres = 0.0;

	if(m_pDisplay && fabs(dMinLeve - dMaxLeve) >= 1E-5)
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

void CCMWindPressureProfileKBC2016Dlg::InitGrid()
{
	m_wndGrid.Initialize(m_pWdprD, this);
	
	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	
	m_bGridInit = TRUE;
	
}

void CCMWindPressureProfileKBC2016Dlg::OnDataChanged()
{
	if(m_pWdprD->arPresProfile.GetSize() > 1)
	{
		m_LateralLoad.MakeWindPressure(*m_pWdprD,FALSE,_T(""),TRUE);
		UpdateChart();
		m_wndGrid.MakeItemEx();
	}
	else
	{
		m_pWdprD->arPresProfile.RemoveAll();    
		m_LateralLoad.MakeWindPressure(*m_pWdprD,FALSE,_T(""),TRUE);
		UpdateChart();
		m_wndGrid.MakeItemEx();
	}
}
void CCMWindPressureProfileKBC2016Dlg::UpdateProfileData()
{
	UpdateChart();
}