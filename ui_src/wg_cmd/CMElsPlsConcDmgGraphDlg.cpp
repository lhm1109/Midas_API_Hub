// CMElsPlsConcDmgGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMElsPlsConcDmgGraphDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#include "..\wg_db\CMTdmtCalcCrpShk.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsConcDmgGraphDlg dialog


CCMElsPlsConcDmgGraphDlg::CCMElsPlsConcDmgGraphDlg(int nType, T_EPMT_K EpmtK, T_EPMT_CONCDMG& Data, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMElsPlsConcDmgGraphDlg::IDD, pParent), m_Grid(this)
{
	//{{AFX_DATA_INIT(CCMElsPlsConcDmgGraphDlg)	
	m_nType = nType;
	m_Data = Data;
	m_nDrawingType = 0;
	m_EpmtK = EpmtK;
	//}}AFX_DATA_INIT
}


void CCMElsPlsConcDmgGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsConcDmgGraphDlg)
	DDX_Radio(pDX,   IDC_YIELD_STRESS_RDO, m_nDrawingType);
	DDX_Control(pDX, IDC_Y_LOG_CHK, m_btnYLog);
	DDX_Control(pDX, IDC_X_LOG_CHK, m_btnXLog);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_CMD_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMElsPlsConcDmgGraphDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMElsPlsConcDmgGraphDlg)
	ON_BN_CLICKED(IDC_X_LOG_CHK,        OnXLog)
	ON_BN_CLICKED(IDC_Y_LOG_CHK,        OnYLog)
	ON_BN_CLICKED(IDC_CMD_REDRAW_BTN,       OnRedraw)
	ON_BN_CLICKED(IDC_YIELD_STRESS_RDO, OnDrawingData)
	ON_BN_CLICKED(IDC_DAMAGE_RDO,       OnDrawingData)
	ON_BN_CLICKED(IDC_CMD_DYGEN,        OnBtnDyGen)
	ON_BN_CLICKED(IDOK,                 OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsConcDmgGraphDlg message handlers


void CCMElsPlsConcDmgGraphDlg::InitGrid()
{
	m_Grid.Initialize();	
	
	m_bGridInit = TRUE;
}


BOOL CCMElsPlsConcDmgGraphDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	InitGrid();
	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;
	InitChart();
	
	CString str;
	if(m_nType == 0)
		str = _LS(IDS_CMD_EPCONC_COMPRESSIVE_BEHAVIOR);
	else if(m_nType == 1)
		str = _LS(IDS_CMD_EPCONC_TENSILE_BEHAVIOR);
	SetWindowText(str);    
		
	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	UpdateData(FALSE);

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	GetDlgItem(IDC_CMD_DYGEN)->EnableWindow(m_EpmtK); // Modify로 열었을 때 만 활성화
	
	Data2Dlg();

	RedrawGraph();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMElsPlsConcDmgGraphDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
	  if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
		  if(pMsg->wParam == VK_RETURN)
			if(pMsg->wParam == VK_ESCAPE)
			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;        
			// DO NOT process further
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}

BOOL CCMElsPlsConcDmgGraphDlg::Grid2Data()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	BOOL bReturnVal = TRUE;

	CArray<PMATL_CDMG_BEHAVIOR, PMATL_CDMG_BEHAVIOR> aData;
	m_Grid.GetData(aData);
	if(m_nType == 0)
	{
		m_Data.nCompBhNum = aData.GetSize();
		if(m_Data.nCompBhNum > 50)
		{
			pDoc->DisplayHistoryMessage(_LS(IDS_CMD_EPCONC_DATA_TOO_MUCH));
			m_Data.nCompBhNum = 50;
			bReturnVal = FALSE;
		}
		for (int i = 0; i < m_Data.nCompBhNum; i++)
		{
			m_Data.aCmpBhInelasticStrain[i] = aData[i].dInelasticStrain;
			m_Data.aCmpBhYieldStress[i] = aData[i].dYieldStress;
			m_Data.aCmpBhDamage[i] = aData[i].dDamage;
		}
	}
	else if(m_nType == 1)
	{
		m_Data.nTnsBhNum = aData.GetSize();
		if(m_Data.nTnsBhNum > 50)
		{
			pDoc->DisplayHistoryMessage(_LS(IDS_CMD_EPCONC_DATA_TOO_MUCH));;
			m_Data.nTnsBhNum = 50;
			bReturnVal = FALSE;
		}
		for (int i = 0; i < m_Data.nTnsBhNum; i++)
		{
			m_Data.aTnsBhInelasticStrain[i] = aData[i].dInelasticStrain;
			m_Data.aTnsBhYieldStress[i] = aData[i].dYieldStress;
			m_Data.aTnsBhDamage[i] = aData[i].dDamage;
		}
	}  
	else ASSERT(0);

	return bReturnVal;
}

void CCMElsPlsConcDmgGraphDlg::Data2Grid()
{
	CArray<PMATL_CDMG_BEHAVIOR, PMATL_CDMG_BEHAVIOR> aData;
	if(m_nType == 0)
	{
		aData.SetSize(m_Data.nCompBhNum);
		for (int i = 0; i < m_Data.nCompBhNum; i++)
		{
			aData[i].dInelasticStrain = m_Data.aCmpBhInelasticStrain[i];
			aData[i].dYieldStress = m_Data.aCmpBhYieldStress[i];
			aData[i].dDamage = m_Data.aCmpBhDamage[i];
		}
	}
	else if(m_nType == 1)
	{
		aData.SetSize(m_Data.nTnsBhNum);
		for (int i = 0; i < m_Data.nTnsBhNum; i++)
		{
			aData[i].dInelasticStrain = m_Data.aTnsBhInelasticStrain[i];
			aData[i].dYieldStress = m_Data.aTnsBhYieldStress[i];
			aData[i].dDamage = m_Data.aTnsBhDamage[i];
		}
	}
	m_Grid.SetData(aData);
}

void CCMElsPlsConcDmgGraphDlg::Data2Dlg()
{

	Data2Grid();
}

void CCMElsPlsConcDmgGraphDlg::OnOK()
{
	if(!Dlg2Data())
	{
		return;
	}

	CDialogMove::OnOK();
}

BOOL CCMElsPlsConcDmgGraphDlg::Dlg2Data()
{ 
	return Grid2Data();
}

void CCMElsPlsConcDmgGraphDlg::OnBtnDyGen()
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	T_UCHT_D UchtD;
	if(m_nType == 0)
	{
		UchtD.nType = D_TYPE_PM_CD_CB;
	}
	else
	{
		UchtD.nType = D_TYPE_PM_CD_TB;
	}  
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = m_EpmtK;
	
	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);

	if(m_nType == 0)
	{
		if(nCount==0) csName = _LS(IDS_CMD_EPCONC_COMPRESSIVE_BEHAVIOR);
		else csName.Format(_T("%s%d"), _LS(IDS_CMD_EPCONC_COMPRESSIVE_BEHAVIOR), nCount+1);
	}
	else
	{
		if(nCount==0) csName = _LS(IDS_CMD_EPCONC_TENSILE_BEHAVIOR);
		else csName.Format(_T("%s%d"), _LS(IDS_CMD_EPCONC_TENSILE_BEHAVIOR), nCount+1);
	}
	
	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;

		UchtD.nFuncType = m_nDrawingType;
		
		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) { ASSERT(0); return; }
	}
}

void CCMElsPlsConcDmgGraphDlg::RedrawGraph()
{
	UpdateData(TRUE);

	Grid2Data();

	int nSize;
	double* aX = NULL;
	double* aY = NULL;
	if(m_nType == 0)
	{
		nSize = m_Data.nCompBhNum;
		aX  = m_Data.aCmpBhInelasticStrain;
		if(m_nDrawingType == 0)
		{
			aY = m_Data.aCmpBhYieldStress;
		}
		else
		{
			aY = m_Data.aCmpBhDamage;
		}
	}
	else if(m_nType == 1)
	{
		nSize = m_Data.nTnsBhNum;
		aX  = m_Data.aTnsBhInelasticStrain;
		if(m_nDrawingType == 0)
		{
			aY = m_Data.aTnsBhYieldStress;
		}
		else
		{
			aY = m_Data.aTnsBhDamage;
		}
	}
	else ASSERT(0);

	m_Graph.KillData();	

	for(int i=0; i<nSize; i++)
	{
		SetValue(i, 0, aX[i], aY[i]);
	}
	m_GraphView.Invalidate();
}

void CCMElsPlsConcDmgGraphDlg::OnRedraw()
{
	RedrawGraph();
}

void CCMElsPlsConcDmgGraphDlg::SetValue(int nIndex, int nGroup, double x,double y)
{  
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}


void CCMElsPlsConcDmgGraphDlg::InitChart()
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
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
//#ifdef _JP
//	pDisplay->SetRect(0.2,0,100,95.0);
//#else 
	pDisplay->SetRect(0.2,0,100,97.0);
//#endif
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);
	
	
	// Logarithmic Axis Scaling
	//
	pDisplay->GetStyle()->SetLogX(m_LogFlagX);
	pDisplay->GetStyle()->SetLog(m_LogFlagY);

	//pDisplay->GetStyle()->SetShowXGrid(m_GridXFlag);
	//pDisplay->GetStyle()->SetShowYGrid(m_GridYFlag);
	

	m_Graph.AddComponent(pDisplay);

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
	SetAxisLabelText(0,_LS(IDS_CMD_EPCONC_INELASTIC_STRAIN));
	
	// Add Label Component for Y-Axis
	//
	pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(1.5, 50.0);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	//pLabel->GetStyle()->SetColor(CXCLR_BACKGRND);
	m_Graph.AddComponent(pLabel);

	CString YLabel;
	switch(m_nDrawingType)
	{
		case 0: SetAxisLabelText(1,_LS(IDS_CMD_EPCONC_YIELD_STRESS));break;
		case 1: SetAxisLabelText(1,_LS(IDS_CMD_EPCONC_DMG));	break;
	}
	
	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview
	
	CWnd* wnd = GetDlgItem(IDC_CMD_PLACEHOLDER);
	CRect rc;

	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left+=2;
	rc.top+=2;
	rc.right -= 2;
	rc.bottom -= 2;

	m_GraphView.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_CMD_PLACEHOLDER);
	m_GraphView.m_pGraph = &m_Graph;
	
	m_GraphView.Invalidate();

}


void CCMElsPlsConcDmgGraphDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CCMElsPlsConcDmgGraphDlg::OnXLog() 
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

void CCMElsPlsConcDmgGraphDlg::OnYLog() 
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

void CCMElsPlsConcDmgGraphDlg::NewData()
{
	m_Graph.KillData();
	m_btnXLog.SetCheck(FALSE);
	m_btnYLog.SetCheck(FALSE);
	OnYLog();
	OnXLog();
}

double CCMElsPlsConcDmgGraphDlg::UnitCvt(double dReadData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	return  pDoc->m_pUnitCtrl->ConvertUnitLengthPrevious(dReadData);
}


void CCMElsPlsConcDmgGraphDlg::OnDrawingData() 
{
	UpdateData();
	switch(m_nDrawingType)
	{
		case 0: SetAxisLabelText(1,_LS(IDS_CMD_EPCONC_YIELD_STRESS));break;
		case 1: SetAxisLabelText(1,_LS(IDS_CMD_EPCONC_DMG));	break;
	}
	
	RedrawGraph();
}