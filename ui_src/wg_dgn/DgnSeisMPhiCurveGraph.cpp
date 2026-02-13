// DgnSeisMPhiCurveGraph.cpp: implementation of the CDgnSeisMPhiCurveGraph class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"

#include "DgnSeisMPhiCurveGraph.h"
#include "DgnSeisMPhiCurveDlg.h"

#include "..\wg_db\MathFunc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\MyGraphDisplaySeisMPhiCurve.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////
/////////     Construction/Destruction           //////////
///////////////////////////////////////////////////////////
// IMPLEMENT_DYNCREATE(CDgnSeisMPhiCurveGraph, CView)

CDgnSeisMPhiCurveGraph::CDgnSeisMPhiCurveGraph()
{
}

CDgnSeisMPhiCurveGraph::~CDgnSeisMPhiCurveGraph()
{
}

BEGIN_MESSAGE_MAP(CDgnSeisMPhiCurveGraph, SREGraphView)
//{{AFX_MSG_MAP(CView)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDgnSeisMPhiCurveGraph::OnInitialUpdate()
{
	SREGraphView::OnInitialUpdate();
	m_pGraph = &m_Graph;
	

	InitGrph();
	DrawGraph();
}

void CDgnSeisMPhiCurveGraph::ShowFeedback(SRGraphFeedback *pFeedback)
{
	if(m_pCurveOptD->nCurveType != 0) return;

	CPoint tPoint=m_MousePoint;	

	int nIndex = pFeedback->m_nIndex+1;
	int nGroup = pFeedback->m_nGroup;

	double dX = GetGraph()->GetData(pFeedback->m_nIndex, pFeedback->m_nGroup)->GetValue();
	double dY = GetGraph()->GetData(pFeedback->m_nIndex, pFeedback->m_nGroup + 1)->GetValue();

	CString strText;
	if(nGroup == 6)   // Point로 그린 경우
	{
		switch (nIndex-1)
		{
		case 0: strText.Format(_T(" a (%g, %g) "),  dX*0.001, dY); break;
		case 1: strText.Format(_T(" b (%g, %g) "),  dX*0.001, dY); break;
		case 2: strText.Format(_T(" c (%g, %g) "),  dX*0.001, dY); break;
		case 3: strText.Format(_T(" d (%g, %g) "),  dX*0.001, dY); break;
		case 4: strText.Format(_T(" e (%g, %g) "),  dX*0.001, dY); break;
		case 5: 
			if(m_pCurveOptD->bIdealize)
			{
				strText.Format(_T(" f (%g, %g) "),  dX*0.001, dY); 
			}
			break;
		default: strText.Empty();
		}
	}
	else  strText.Format(_T(" %d (%g, %g) "), nIndex, dX*0.001, dY);
	ClientToScreen(&tPoint);

	m_GraphTip.ShowTip(strText,tPoint,this);
}

void CDgnSeisMPhiCurveGraph::MakeBmp(CString strFilePath)
{
	SREGraphView::SaveToFileAsDib(strFilePath);
}

void CDgnSeisMPhiCurveGraph::OnDraw(CDC* pDC)
{
	SREGraphView::OnDraw(pDC);
}

void CDgnSeisMPhiCurveGraph::PostNcDestroy() 
{
	SREGraphView::PostNcDestroy();
}

#ifdef _DEBUG
void CDgnSeisMPhiCurveGraph::AssertValid() const
{
	SREGraphView::AssertValid();
}

void CDgnSeisMPhiCurveGraph::Dump(CDumpContext& dc) const
{
	SREGraphView::Dump(dc);
}
#endif //_DEBUG


void CDgnSeisMPhiCurveGraph::DrawGraph()
{
	m_Graph.KillComponentList();
	m_Graph.KillData();

	if(m_pGraph != NULL) InitGrph();

	double dValX = 0.0, dValY=0.0;
	int nDataCnt = m_MphiResD[m_pCurveOptD->nAngleDir].arMPhiCruve.GetSize();

	if(nDataCnt == 0)
	{
		return;
	}

	double dMaxY = -DBL_MAX;

	for (int i=0; i<nDataCnt; i++)
	{
		T_BRDGEVAL_MPHI_UNIT& MPhiUnit = m_MphiResD[m_pCurveOptD->nAngleDir].arMPhiCruve.GetAt(i);

		dValX = MPhiUnit.dCurvature*1000.;

		if     (m_pCurveOptD->nCurveType == 0) dValY = MPhiUnit.dMoment;
		else if(m_pCurveOptD->nCurveType == 1) dValY = MPhiUnit.dXn;
		else if(m_pCurveOptD->nCurveType == 2) dValY = MPhiUnit.dConcStrain;
		else if(m_pCurveOptD->nCurveType == 3) dValY = MPhiUnit.dStlStrain;
		else if(m_pCurveOptD->nCurveType == 4) dValY = MPhiUnit.dConcComp;
		else if(m_pCurveOptD->nCurveType == 5) dValY = MPhiUnit.dStlComp;
		else if(m_pCurveOptD->nCurveType == 6) dValY = MPhiUnit.dStlTens;
		else                                   dValY = MPhiUnit.dMoment;

		SetValue(i, 1, dValX, dValY);
		dMaxY = max(dMaxY, dValY);
	}
	
	if(m_pCurveOptD->nCurveType == 0)
	{
		BOOL bDrawIdeal = FALSE;

		T_BRDGEVAL_MPHI_STATE& StateUnit = m_MphiResD[m_pCurveOptD->nAngleDir].StateD;
		double dMaxCurvature = 0.0;

		if(m_pCurveOptD->bIdealize)
		{
			dValX = StateUnit.dCurvature[4]*1000; // Yield(ideal)
			dValY = StateUnit.dMoment[4]; // Yield(ideal)

			dMaxCurvature = StateUnit.dCurvature[5]*1000.;  // Ultimate
			double dUserCurvature = m_pCurveOptD->dCurvature*1000;

			// Yield(ideal) or Ultimate(ideal) 이 0 이면 그리지 않음
			if((fabs(dValX)>CMathFunc::m_NormalZero || fabs(dValY)>CMathFunc::m_NormalZero) && dMaxCurvature>CMathFunc::m_NormalZero)
			{
				// 원점, Yield(ideal) 점, 끝점
				SetValue(0, 2, 0.0,    0.0);
				SetValue(1, 2, dValX,  dValY);
				dMaxY = max(dMaxY, dValY);

				if(m_pCurveOptD->bCurvature)
				{
					if(dUserCurvature < dMaxCurvature) dMaxCurvature = dUserCurvature;
				}

				dValX = dMaxCurvature;
				dValY = StateUnit.dMoment[5];
				SetValue(2, 2, dValX, dValY);
				dMaxY = max(dMaxY, dValY);

				bDrawIdeal = TRUE;
			}
		}

		if(bDrawIdeal)
		{
			for (int i=0; i<6; i++)
			{
				if(i==5) continue;

				dValX = StateUnit.dCurvature[i]*1000.;
				dValY = StateUnit.dMoment[i];
				SetValueDot(i, 3, dValX, dValY);
				dMaxY = max(dMaxY, dValY);
			}
		}
	  
		if(m_pCurveOptD->bIdealize)
		{
			double dScale = 1.05;
			SetValue(0, 4, 0.0, dMaxY * dScale); // pDC 로 그린 문자가 짤릴경우도 있어서 Y 축 Scale 을 이런식으로 좀 더 늘린다 ( 이 값은 Data가 1개뿐이여서 출력되진 않음 )
		}
	}	

	CString strUnit, strTitle;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_FORCE, strUnit);
	
	if     (m_pCurveOptD->nCurveType == 0) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_M_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 1) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_NA_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 2) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_CS_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 3) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_SS_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 4) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_CC_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 5) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_SC_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 6) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_ST_C), m_pCurveOptD->dAxialForce, strUnit);
	else                                   strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_M_C), m_pCurveOptD->dAxialForce, strUnit);
		
	m_Graph.SetGraphTitle(strTitle);
}

void CDgnSeisMPhiCurveGraph::InitGrph()
{
	SetDragable(FALSE);

	AddBackGrndComponent();
	AddTitleComponent();
	AddDisplayComponent();
	AddXAxisLabelComponent();
	AddYAxisLabelComponent();
}

void CDgnSeisMPhiCurveGraph::SetValue(int nIndex, int nGroup, double x,double y)
{ 
	m_Graph.SetValue(nIndex, nGroup*2, x);  //x
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(1);
	m_Graph.SetValue(nIndex, nGroup*2+1, y); //y
}

void CDgnSeisMPhiCurveGraph::SetValueDot(int nIndex, int nGroup, double x,double y)
{ 
	m_Graph.SetValue(nIndex, nGroup*2, x);  //x
	m_Graph.SetValue(nIndex, nGroup*2+1, y); //y

	SRGraphData* pData = m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex);    ASSERT(pData);

	SRGraphStyle* pStyle = pData->GetStyle();
	pStyle->SetObjectStyle(CX_OBJECT_POINT);
	pStyle->SetFrameWidth(1);
	pStyle->SetUseMaxSize(TRUE);
	pStyle->SetMaxObjectHeight(0.8);    // 0.8*10 = 8 pixels high
	pStyle->SetMaxObjectWidth(0.8);     // 0.8*10 = 8 pixels high
}

void CDgnSeisMPhiCurveGraph::AddBackGrndComponent()
{
	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *) pB);
}

void CDgnSeisMPhiCurveGraph::AddTitleComponent()
{
	SRGraphTitle *pTitle = new SRGraphTitle;
	pTitle->SetMeasurement(CX_PERCENT);
	pTitle->SetFontSize(10);
	
	pTitle->SetFaceName(_LSX(Courier New));
	pTitle->SetRect(0,0,100,15);
	pTitle->GetStyle()->SetColor(CXCLR_WHITE);
	pTitle->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pTitle->SetTextColor(CXCLR_BLACK);
	
	pTitle->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	
	CString strUnit, strTitle;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_FORCE, strUnit);

	if     (m_pCurveOptD->nCurveType == 0) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_M_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 1) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_NA_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 2) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_CS_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 3) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_SS_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 4) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_CC_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 5) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_SC_C), m_pCurveOptD->dAxialForce, strUnit);
	else if(m_pCurveOptD->nCurveType == 6) strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_ST_C), m_pCurveOptD->dAxialForce, strUnit);
	else                                   strTitle.Format(_LS(IDS_DGN_SEIS_MPCC_CURVE_GRAPH_M_C), m_pCurveOptD->dAxialForce, strUnit);
	  
	m_Graph.AddComponent((SRGraphComponent *) pTitle);		
	m_Graph.SetGraphTitle(strTitle);
}

void CDgnSeisMPhiCurveGraph::AddDisplayComponent()
{
	MyGraphDisplaySeisMPhiCurve *pDisplay=new MyGraphDisplaySeisMPhiCurve;	
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX); // default
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetFontSize(8);
	pDisplay->SetRect(0, 12, 100, 95);
	pDisplay->SetFaceName(_LSX(Courier New));
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(FALSE);
	
	// Logarithmic Axis Scaling
	pDisplay->GetStyle()->SetLogX(FALSE);
	pDisplay->GetStyle()->SetLog(FALSE);
	pDisplay->GetStyle()->SetShowXGrid(TRUE);
	pDisplay->GetStyle()->SetShowYGrid(TRUE);

	// Subclass Method
	pDisplay->SetCamberDiagramMode(FALSE);

	m_Graph.AddComponent(pDisplay);
}

void CDgnSeisMPhiCurveGraph::AddXAxisLabelComponent()
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_pGraph->GetComponent(0, IDS_SRG_LABELTYPE);
	if(!pLabel)
	{
		pLabel=new SRGraphLabel;
		pLabel->SetMeasurement(SRGraphComponent::PERCENT);
		pLabel->SetLocationPoint(SRGraphLabel::TopCenter);
		pLabel->SetTextPosition(50.0, 94.0);
		pLabel->SetBlockMember(FALSE);
		pLabel->SetParent(&m_Graph);
		pLabel->SetLabelStandard(SRGraphLabel::Normal);
		pLabel->GetStyle()->SetColor(CXCLR_WHITE);
		pLabel->SetFontSize(9);
		pLabel->SetFaceName(_LSX(Courier New));

		CString strLableX, strUnit;
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit);
		strLableX.Format(_T("Curvature*0.001(1/%s)"), strUnit);
		m_Graph.AddComponent(pLabel);
	  SetAxisLabelText(0,strLableX);
	}
}

void CDgnSeisMPhiCurveGraph::AddYAxisLabelComponent()
{
	SRGraphLabel	*pLabel;
	pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(2.5, 50.0);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LSX(Courier New));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);

	CString strLableY, strUnit;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_MOMENT, strUnit);
	strLableY.Format(_T("Moment Mn(%s)"), strUnit);
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(1,strLableY);
}

void CDgnSeisMPhiCurveGraph::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_pGraph->GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}