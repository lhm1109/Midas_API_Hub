// DgnSeisBearingPropEditBtn.cpp: implementation of the CDgnEditBtn_TndnTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"

#include "DgnSeisBearingPropEditBtn.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\MatlDB.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\Db_DllManager_Base.h"
#include "..\wg_db\InitCtrl.h"

#include <utility>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LINKKEY_ORIGIN 9999

/////////////////////////////////////////////////////////////////////////////
// CDgnEditBtn_TndnTemplate dialog
CDgnSeisBearingPropEditBtn::CDgnSeisBearingPropEditBtn()
	: CDrawEditBtn()
{
// 	Reset_Variables(FALSE);
 	SetDrawFormat();

 	m_DrawCtrl.m_SelectDrawType = 4;
	m_DrawCtrl.m_SelectDrawFormat.Width = 3;
	m_DrawCtrl.m_SelectDrawFormat.lineColor = RGB(250, 115, 75);
	m_DrawCtrl.m_SelectDrawFormat.pointType = EN_POINT_TYPE_CIRCLE;

// 	m_nOverlapType = EN_DGN_OVERLAP_OBJECT;
	m_bUseMove = FALSE; //일단 이게 있어야 선택 사각형을 그린다.....
	m_bMoveOrigin = FALSE;
 	m_DrawCtrl.SetSelectDis(2);
		
	//m_BkGroudColor = RGB(227, 227, 227);
	m_BkGroudColor = RGB(255, 255, 255);
	m_ColorBearingBody = RGB(254, 159, 107);
	m_ColorBearingLine = RGB(209, 109, 55);
	m_ColorBolt = RGB(180, 208, 221);
	m_ColorDimensionLine = RGB(79, 79, 79);

	m_nLastFormatID = -1;


	m_dFitMargin = 0.06;

	m_dBearingWidth = 0.0;
	m_dBearingHeight = 0.0;
}

CDgnSeisBearingPropEditBtn::~CDgnSeisBearingPropEditBtn()
{
}

void CDgnSeisBearingPropEditBtn::DoDataExchange(CDataExchange* pDX)
{
	CDrawEditBtn::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDgnSeisBearingPropEditBtn, CDrawEditBtn)
END_MESSAGE_MAP()

void CDgnSeisBearingPropEditBtn::MakeDrawUnit( BOOL bFit /*= TRUE*/ )
{
	m_DrawCtrl.RemoveDrawData(TRUE, TRUE, FALSE); // Format들을 삭제 함

	m_nLastFormatID = -1;    
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	if(fabs(m_dBearingWidth) < dZero || fabs(m_dBearingHeight) < dZero)
	{
		return ;
	}

	if(!m_ShapeMgr.CheckBrpr(m_Data))
	{
		Redraw(bFit);
		return;
	}

	MakeDrawBearing();  
	MakeDrawAnchor();
	FullyRedraw(bFit); // 고정Size 수치들을 세팅 하기 전에 Redraw를 해줘야 함
	MakeDrawDimensionLine();
	
	FullyRedraw(bFit);
}

void CDgnSeisBearingPropEditBtn::FullyRedraw(BOOL bFit)
{
	if(bFit)
	{
		m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);
	}
	m_DrawCtrl.ConvertDrawData(TRUE);

	OverlapChecking();
	Redraw(bFit);
}

BOOL CDgnSeisBearingPropEditBtn::SetData(T_BRPR_D& Data, BOOL bFit)
{
	//m_pTemplateCtrl->GetLengthCtrl()->GetHeightWidthByElem(tElemKey, m_dTotalHeight, m_dTotalWidth, m_dGapY, m_dGapZ, dPos);
	//m_pTemplateCtrl->GetPosByElem(tElemKey, dPos, m_dPos);
//  m_dTotalHeight = 10;
//  m_dTotalWidth = 10;
	
	m_Data = Data;

	m_dBearingWidth = m_Data.dSx * 2.0;
	int nSize;
	nSize = m_Data.nN1 - 1;
	for (int i = 0; i < nSize ; i++)
	{
		m_dBearingWidth += m_Data.dS1;
	}

	m_dBearingHeight = m_Data.dSy * 2.0;	
	nSize = m_Data.nN2 - 1;
	for (int i = 0; i < nSize ; i++)
	{
		m_dBearingHeight += m_Data.dS2;
	}

	ResetSnapPos();
	MakeDrawUnit(bFit);

	return TRUE;
}

void CDgnSeisBearingPropEditBtn::MakeDrawAnchor()
{
	std::vector<T_POINT2D> aAnchorPos;	
	GetAnchorPos(aAnchorPos);
	if(aAnchorPos.size() == 0)
	{
		return;
	}

	T_CIRCLE_R_GROUP tCircleGroup;
	tCircleGroup.List.SetSize(aAnchorPos.size());	

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.bFill  = TRUE;	
	DrawFormat.Width  = 1;     
	DrawFormat.lineColor = RGB(0, 0, 0);
	DrawFormat.fillColor = m_ColorBolt;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	int nSize = (int)aAnchorPos.size();
	for (int i = 0; i < nSize ; i++)
	{
		tCircleGroup.List[i].Radius = m_Data.dda * 0.5;
		tCircleGroup.List[i].CenterP.x = aAnchorPos[i].x;
		tCircleGroup.List[i].CenterP.y = aAnchorPos[i].y;	
	}
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tCircleGroup);
}

void CDgnSeisBearingPropEditBtn::GetAnchorPos(std::vector<T_POINT2D> &aAnchorPos)
{
	double dX, dY;

	dX = m_Data.dSx;
	dY = m_Data.dSy;

	int nSize, nSizeJ;
	nSize = m_Data.nN2;
	for (int i = 0; i < nSize; i++)
	{
		nSizeJ = m_Data.nN1;
		for (int j = 0; j < nSizeJ; j++)
		{
			aAnchorPos.push_back(T_POINT2D(dX, dY));
			if(j < m_Data.nN1 - 1)
			{
				dX += m_Data.dS1;
			}
		}

		dX = m_Data.dSx;
		if(i < m_Data.nN2 - 1)
		{
			dY += m_Data.dS2;
		}
	}
}

double CDgnSeisBearingPropEditBtn::ConvWndPosToCoord(int nWndPos)
{
	double dZeroX, dZeroY;
	double dX, dY;
	GetGenuineCoord(CPoint(0, 0), dZeroX, dZeroY);
	GetGenuineCoord(CPoint(nWndPos, nWndPos), dX, dY);

	return dX - dZeroX;
}

void CDgnSeisBearingPropEditBtn::MakeDrawDimensionLine()
{
	int nFormatIDLine, nFormatIDArrow, nFormatIDTxt, nFormatIDTxtY;

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	//DrawFormat.bFill  = FALSE;		  
	DrawFormat.lineColor = m_ColorDimensionLine;
	//DrawFormat.fillColor = RGB(118,   0,   118);
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width  = 1;     
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDLine = m_nLastFormatID;
	
	DrawFormat.startArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.endArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.dRWidth        = 3;
	DrawFormat.dRHeight       = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDArrow = m_nLastFormatID;
	
	T_TEXT_FORMAT	TextFormat;    
	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	TextFormat.Size = 14;  
	TextFormat.fontColor = RGB(0, 0, 0);
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	nFormatIDTxt = m_nLastFormatID;

	TextFormat.Size = 13;  
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;
	TextFormat.nEscapement = 900;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	nFormatIDTxtY = m_nLastFormatID;

	std::vector<T_POINT2D> aAnchorPos;	
	GetAnchorPos(aAnchorPos);
	if(aAnchorPos.size() == 0)
	{
		return;
	}

	m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin); // 전체 Size가 잡힌 다음 ConvWndPosToCoord 를 부르기전에 한번은 불러 줘야 함

	double dLenS1, dLenS2, dLenBa, dLenHa, dGap, dGapTxt, dDimGap;	
	dLenS1 = ConvWndPosToCoord(20);
	dLenS2 = ConvWndPosToCoord(20);
	dLenBa = ConvWndPosToCoord(36);
	dLenHa = ConvWndPosToCoord(36);
	dGap = ConvWndPosToCoord(5);
	dGapTxt = ConvWndPosToCoord(7);
	dDimGap = ConvWndPosToCoord(5);

	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);  
	T_LINE_R tLine;
	
	// X 방향(Sx, S1, Ba)
	
	// Ba
	tLine.P1.x = 0;
	tLine.P1.y = m_dBearingHeight;
	tLine.P2.x = 0;
	tLine.P2.y = m_dBearingHeight + dLenBa;
	tLine.P1.y += dDimGap;
	tLine.P2.y += dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);

	tLine.P1.x = m_dBearingWidth;
	tLine.P1.y = m_dBearingHeight;
	tLine.P2.x = m_dBearingWidth;
	tLine.P2.y = m_dBearingHeight + dLenBa;
	tLine.P1.y += dDimGap;
	tLine.P2.y += dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);

	tLine.P1.x = 0;
	tLine.P1.y = m_dBearingHeight + dLenBa - dGap;
	tLine.P2.x = m_dBearingWidth;
	tLine.P2.y = m_dBearingHeight + dLenBa - dGap;
	tLine.P1.y += dDimGap;
	tLine.P2.y += dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

	tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
	tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + dGapTxt;
	tTextGroup.List[0].szText = _LSX(Ba);
	m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);

	// Sx
	tLine.P1.x = 0;
	tLine.P1.y = m_dBearingHeight + dLenS1 - dGap;
	tLine.P2.x = aAnchorPos[0].x;
	tLine.P2.y = m_dBearingHeight + dLenS1 - dGap;
	tLine.P1.y += dDimGap;
	tLine.P2.y += dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);
	
	tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
	tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + dGapTxt;
	tTextGroup.List[0].szText = _LSX(Sx);
	m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);

	// S1
	int nSize;
	nSize = m_Data.nN1;
	for (int i = 0; i < nSize ; i++)
	{	
		tLine.P1.x = aAnchorPos[i].x;
		tLine.P1.y = m_dBearingHeight;
		tLine.P2.x = aAnchorPos[i].x;
		tLine.P2.y = m_dBearingHeight + dLenS1;
		tLine.P1.y += dDimGap;
		tLine.P2.y += dDimGap;
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);

		if(i < nSize - 1)
		{
			tLine.P1.x = aAnchorPos[i].x;
			tLine.P1.y = m_dBearingHeight + dLenS1 - dGap;
			tLine.P2.x = aAnchorPos[i + 1].x;
			tLine.P2.y = m_dBearingHeight + dLenS1 - dGap;
			tLine.P1.y += dDimGap;
			tLine.P2.y += dDimGap;
			tArrowGroup.List[0] = tLine;
			m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

			tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
			tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + dGapTxt;
			tTextGroup.List[0].szText = _LSX(S1);
			m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);
		}
	}	

	// Sx
	if(m_Data.nN1 - 1 >= 0)
	{
		tLine.P1.x = aAnchorPos[m_Data.nN1 - 1].x;
		tLine.P1.y = m_dBearingHeight + dLenS1 - dGap;
		tLine.P2.x = m_dBearingWidth;
		tLine.P2.y = m_dBearingHeight + dLenS1 - dGap;
		tLine.P1.y += dDimGap;
		tLine.P2.y += dDimGap;
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + dGapTxt;
		tTextGroup.List[0].szText = _LSX(Sx);
		m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);
	}

	// Y 방향(Sy, S2, Ha)
	// Ha
	tLine.P1.x = 0.0;
	tLine.P1.y = 0.0;
	tLine.P2.x = -dLenHa;
	tLine.P2.y = 0.0;
	tLine.P1.x -= dDimGap;
	tLine.P2.x -= dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);

	tLine.P1.x = 0.0;
	tLine.P1.y = m_dBearingHeight;
	tLine.P2.x = -dLenHa;
	tLine.P2.y = m_dBearingHeight;
	tLine.P1.x -= dDimGap;
	tLine.P2.x -= dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);

	tLine.P1.x = -dLenHa + dGap;
	tLine.P1.y = 0;
	tLine.P2.x = -dLenHa + dGap;
	tLine.P2.y = m_dBearingHeight;
	tLine.P1.x -= dDimGap;
	tLine.P2.x -= dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

	tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - dGapTxt;
	tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	tTextGroup.List[0].szText = _LSX(Ha);
	m_DrawCtrl.AddDrawUnit(nFormatIDTxtY, tTextGroup);

	// Sy
	tLine.P1.x = -dLenS2 + dGap;
	tLine.P1.y = 0.0;
	tLine.P2.x = -dLenS2 + dGap;
	tLine.P2.y = aAnchorPos[0].y;
	tLine.P1.x -= dDimGap;
	tLine.P2.x -= dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

	tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - dGapTxt;
	tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	tTextGroup.List[0].szText = _LSX(Sy);
	m_DrawCtrl.AddDrawUnit(nFormatIDTxtY, tTextGroup);

	// S2
	nSize = m_Data.nN2;
	for (int i = 0; i < nSize ; i++)
	{	
		tLine.P1.x = 0.0;
		tLine.P1.y = aAnchorPos[i * (m_Data.nN1)].y;
		tLine.P2.x = -dLenS2;
		tLine.P2.y = tLine.P1.y;
		tLine.P1.x -= dDimGap;
		tLine.P2.x -= dDimGap;
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);

		if(i < nSize - 1)
		{
			tLine.P1.x = -dLenS2 + dGap;
			tLine.P1.y = aAnchorPos[i * (m_Data.nN1)].y;
			tLine.P2.x = -dLenS2 + dGap;
			tLine.P2.y = aAnchorPos[(i + 1) * (m_Data.nN1)].y;
			tLine.P1.x -= dDimGap;
			tLine.P2.x -= dDimGap;
			tArrowGroup.List[0] = tLine;
			m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

			tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - dGapTxt;
			tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
			tTextGroup.List[0].szText = _LSX(S2);						
			m_DrawCtrl.AddDrawUnit(nFormatIDTxtY, tTextGroup);
		}
	}	

	// Sy
	tLine.P1.x = -dLenS2 + dGap;
	tLine.P1.y = m_dBearingHeight;
	tLine.P2.x = -dLenS2 + dGap;
	tLine.P2.y = aAnchorPos[aAnchorPos.size() - 1].y;
	tLine.P1.x -= dDimGap;
	tLine.P2.x -= dDimGap;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

	tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - dGapTxt;
	tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	tTextGroup.List[0].szText = _LSX(Sy);
	m_DrawCtrl.AddDrawUnit(nFormatIDTxtY, tTextGroup);
}

void CDgnSeisBearingPropEditBtn::MakeDrawBearing()
{
	m_dBoundBox_Bottom = 0.0;
	m_dBoundBox_Top = 0.0;
	m_dBoundBox_Left = 0.0;
	m_dBoundBox_Right = 0.0;

	double dOnePixel;	
	//dOnePixel = ConvWndPosToCoord(2);
	//dOnePixel *= 0.70;
	dOnePixel = 0.0;

	CalcBoundingBox(0, -dOnePixel);	
	CalcBoundingBox(m_dBearingWidth + dOnePixel * 0.8, m_dBearingHeight);
	
	T_RECT_R_GROUP tRectGroup;
	tRectGroup.List.SetSize(1);
	tRectGroup.List[0].dThik = 1;
	
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.bFill  = TRUE;	
	DrawFormat.Width  = 1;
	DrawFormat.lineColor = m_ColorBearingLine;
	DrawFormat.fillColor = m_ColorBearingBody;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	
	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);
	tRectGroup.List[0].P1.x = 0.0;
	tRectGroup.List[0].P1.y = -dOnePixel;
	tRectGroup.List[0].P2.x = m_dBearingWidth + dOnePixel * 0.8;
	tRectGroup.List[0].P2.y = m_dBearingHeight;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tRectGroup);
}

/*
void CDgnSeisBearingPropEditBtn::MakeDrawValue()
{
	T_TEXT_FORMAT	TextFormat;
	TextFormat.fontColor = RGB(0,0,0);
	TextFormat.Size = 13;
	TextFormat.positionType = EN_POSITION_TYPE_RIGHT_BOTTOM;  
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);

	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	tTextGroup.List[0].PPosition.x = (FitToFixSize(m_dMaxY, m_aY[0], m_dContourWidth) + 5) * -1;
	tTextGroup.List[0].PPosition.y = 0 + 15;
	CString strTmp;
	strTmp.Format(_T("%g"), m_aY[0]);
	tTextGroup.List[0].szText = strTmp;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

	tTextGroup.List[0].PPosition.x = (FitToFixSize(m_dMaxY, m_aY[m_aY.size() - 1], m_dContourWidth) + 5) * -1;
	tTextGroup.List[0].PPosition.y = m_dContourHeight + 15; 
	strTmp.Format(_T("%g"), m_aY[m_aY.size() - 1]);
	tTextGroup.List[0].szText = strTmp;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);  
}
*/

void CDgnSeisBearingPropEditBtn::SetDrawFormat()
{
	/*
	T_DRAW_FORMAT	DrawFormat;
	T_TEXT_FORMAT	TextFormat;
	T_DIM_FORMAT	DimFormat;

	DimFormat.Init();
	DimFormat.fontColor = RGB(0,0,0);
	DimFormat.bMainLine = TRUE;
	DimFormat.dRTextSpace = 0.05;
	DimFormat.dRDimSpace = 0.1;
	
	DrawFormat.bLine          = TRUE;	
	DrawFormat.bFill          = TRUE;		
	//DrawFormat.fillColor      = RGB(220, 220, 220);
	DrawFormat.fillColor      = RGB(220, 0, 0);
	DrawFormat.lineColor      = RGB(0,   0,   0); 	
	m_DrawCtrl.m_arDrawFormat.SetAt(0, DrawFormat);
	*/
	m_CurrDrawFormat.nPenStyle = PS_DOT;
}

void CDgnSeisBearingPropEditBtn::Redraw( BOOL bFit )
{
	if(bFit)
	{
		m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);
	}

	if(this)
	{
		if(IsWindow(this->m_hWnd))
		{
			RedrawWindow();
		}
	}	
}

void CDgnSeisBearingPropEditBtn::CalcBoundingBox( double dX, double dY, double* dLeft, double* dRight, double* dTop, double* dBottom )
{
	if(dRight  == NULL) dRight  = &m_dBoundBox_Right;
	if(dLeft   == NULL) dLeft   = &m_dBoundBox_Left;
	if(dTop    == NULL) dTop    = &m_dBoundBox_Top;
	if(dBottom == NULL) dBottom = &m_dBoundBox_Bottom;

	if(dX > *dRight)  *dRight  = dX;
	if(dX < *dLeft)   *dLeft   = dX;
	if(dY > *dTop)    *dTop    = dY;
	if(dY < *dBottom) *dBottom = dY;
}

void CDgnSeisBearingPropEditBtn::StartCurrUnitSelect( CPoint CurrP )
{
	CDrawEditBtn::StartCurrUnitSelect( CurrP );
}

void CDgnSeisBearingPropEditBtn::InProgressCurrUnitSelect( CPoint CurrP )
{
	/*
	if(m_bMoveOrigin)
	{
		
		CurrRemoveDrawData();
		SelectRemoveDrawData();

		int nSnapX = -1;
		int nSnapY = -1;
		double dCoord[2];
		GetPosByCPoint(CurrP, dCoord[0], dCoord[1], nSnapX, nSnapY, TRUE);
		GetDisplayCoord(dCoord[0], dCoord[1], CurrP);

		//     MakeCurrUnitSelectMove(m_StartPoint, CurrP);    
		//     CurrConvertDrawData(FALSE);

		MakeCurrLinePreview(dCoord[0], dCoord[1], nSnapX, nSnapY);
		
		RedrawWindow();
	}
	else
	{
	*/
		CDrawEditBtn::InProgressCurrUnitSelect( CurrP );  
	//}
}

void CDgnSeisBearingPropEditBtn::CompleteCurrUnitSelect( CPoint CurrP )
{
	/*
	BOOL bShift = GetAsyncKeyState(VK_LSHIFT) ? TRUE: FALSE;

	nrUINT nrUintPre;
	int nSizePre = 0;
	
	if(bShift) nSizePre = GetSelectedLinkKeyList(nrUintPre);

	CDrawEditBtn::CompleteCurrUnitSelect( CurrP );

	if(m_bHasList)
	{
		vecTndnKey keyList;
		nrUINT nrUint, nrUintTarget;
		int nSize = GetSelectedLinkKeyList(nrUint);
		int i, j;

		for(i = 0 ; i < nSize; i++)
		{
			nrUintTarget.Add(nrUint[i]);
		}

		if(bShift && nSizePre > 0)
		{
			BOOL bExist;
			for(i = 0 ; i < nSizePre; i++)
			{
				bExist = FALSE;

				for(j = 0 ; j < nSize; j++)
				{
					if(nrUintPre[i] == nrUint[j]) bExist = TRUE;
				}

				if(!bExist) nrUintTarget.Add(nrUintPre[i]);
			}
		}
		nSize = nrUintTarget.GetSize();
		keyList.resize(nSize);
		int idxOrigin = -1;
		int nIdx = 0;
		for(i = 0 ; i < nSize; i++)
		{
			if(nrUintTarget[i] == LINKKEY_ORIGIN) idxOrigin = i;
			else if(nrUintTarget[i] > 0)          keyList[nIdx++] = nrUintTarget[i] - 1;
			else                                  ASSERT(0);
		}

		keyList.resize(nIdx);
		if(idxOrigin >= 0) nrUintTarget.RemoveAt(idxOrigin);

		//SetParentSelectedKey(nIdx, keyList, nrUintTarget, idxOrigin);

	}
	*/
}

void CDgnSeisBearingPropEditBtn::OnLButtonUp( UINT nFlags, CPoint point )
{
	/*
	if(m_bMoveOrigin)
	{
		m_bUseMove = FALSE;
		m_bMoveOrigin = FALSE;  

		nrUINT arKey;
		m_DrawCtrl.GetSelectedKeyList(arKey);
		UINT nKey; 
		BOOL bMovePoint = FALSE;
		if(arKey.GetSize() == 1)
		{
			CDrawPoint* pDrawPt = (CDrawPoint*)m_DrawCtrl.GetDrawClass(arKey[0]);
			if(pDrawPt->m_LinkKey == LINKKEY_ORIGIN)
			{
				bMovePoint = TRUE;
				nKey = arKey[0];
			}
		}

		//이전의 상태를 저장해놓는다.
		int nModePre = m_nMode;
		m_nMode = EN_DRAW_WINMODE_NONE;
		CDrawEditBtn::OnLButtonUp(nFlags, point);
		m_nMode = nModePre;

		//움직인 점을 이동한다.
		if(bMovePoint)
		{
			int nSnapX, nSnapY;
			double dPos[2];
			GetPosByCPoint(point, dPos[0], dPos[1], nSnapX, nSnapY, TRUE);
			dPos[0] = dPos[0];
			dPos[1] = dPos[1];
			SetDrawOrigin(dPos, FALSE, TRUE);
		}    
	}
	else
	{*/
		//CDrawEditBtn::OnLButtonUp(nFlags, point);
	//}
}

void CDgnSeisBearingPropEditBtn::OnLButtonDown( UINT nFlags, CPoint point )
{
//	CDrawEditBtn::OnLButtonDown(nFlags, point);
	/*
	UINT nKey;
	if(m_DrawCtrl.GetSelectDrawUnitKey(point, nKey))
	{
		CDrawPoint* pDrawPt = (CDrawPoint*)m_DrawCtrl.GetDrawClass(nKey);
		if(pDrawPt->m_LinkKey == LINKKEY_ORIGIN)
		{
			m_bUseMove = TRUE;
			m_bMoveOrigin = TRUE;
		}
	}
	*/
}

void CDgnSeisBearingPropEditBtn::GetPosByCPoint( CPoint& cPt, double& dPtX, double& dPtY, int& nSnapX, int& nSnapY, BOOL bSnap )
{
	/*
	GetGenuineCoord(cPt, dPtX, dPtY);

	if(bSnap && !(GetAsyncKeyState(VK_LSHIFT)))
	{
		nSnapX = -1;
		nSnapY = -1;
		double dGap = (m_dTotalHeight + m_dTotalWidth) * 0.015;
		double dSnapLenX, dSnapLenY;
		double dTempX, dTempY;
		int i, nSize;
		nSize = m_aSnapVertex.GetSize();

		//가장 가까운 X스냅 좌표를 찾는다.
		for(i = 0 ; i < nSize; i++)
		{
			dTempX = fabs(dPtX - m_aSnapVertex[i].x);
			dTempY = fabs(dPtY - m_aSnapVertex[i].y);

			if(dTempX < dGap)
			{
				if(nSnapX < 0 || dSnapLenX > dTempX)
				{
					dSnapLenX = dTempX;
					dSnapLenY = dTempY;
					nSnapX = i;
				}
				else if(fabs(dSnapLenX - dTempX) <= 1E-5 && dSnapLenY > dTempY)
				{
					dSnapLenX = dTempX;
					dSnapLenY = dTempY;
					nSnapX = i;
				}
			}
		}

		//가장 가까운 Y스냅 좌표를 찾는다.
		for(i = 0 ; i < nSize; i++)
		{
			dTempX = fabs(dPtX - m_aSnapVertex[i].x);
			dTempY = fabs(dPtY - m_aSnapVertex[i].y);

			if(dTempY < dGap)
			{
				if(nSnapY < 0 || dSnapLenY > dTempY)
				{
					dSnapLenX = dTempX;
					dSnapLenY = dTempY;
					nSnapY = i;
				}
				else if(fabs(dSnapLenY - dTempY) <= 1E-5 && dSnapLenX > dTempX)
				{
					dSnapLenX = dTempX;
					dSnapLenY = dTempY;
					nSnapY = i;
				}
			}
		}

		if(nSnapX >= 0) dPtX = m_aSnapVertex[nSnapX].x;
		if(nSnapY >= 0) dPtY = m_aSnapVertex[nSnapY].y;
	}
	*/
}

void CDgnSeisBearingPropEditBtn::ResetSnapPos()
{
	m_aSnapVertex.RemoveAll();
	/*
	for(int i = 0 ; i < m_aPnt1.size(); i++)
	{
		m_aSnapVertex.Add(m_aPnt1[i]);
	}
	
	for(int i = 0 ; i < m_aPnt2.size(); i++)
	{
		m_aSnapVertex.Add(m_aPnt2[i]);
	}
	*/
}

void CDgnSeisBearingPropEditBtn::MakeCurrLinePreview( double dPtX, double dPtY, int nSnapX, int nSnapY )
{
	T_POINT tPt;
	tPt.x = dPtX;
	tPt.y = dPtY;
	m_CurrDrawClass_Point.m_DataR.List.Add(tPt);
	if(nSnapX >= 0)
	{
		T_LINE_R tLine;
		tLine.P1.x = dPtX;
		tLine.P1.y = dPtY;
		tLine.P2.x = m_aSnapVertex[nSnapX].x;
		tLine.P2.y = m_aSnapVertex[nSnapX].y;
		m_CurrDrawClass_Line.m_DataR.List.Add(tLine);
	}
	if(nSnapY >= 0)
	{
		T_LINE_R tLine;
		tLine.P1.x = dPtX;
		tLine.P1.y = dPtY;
		tLine.P2.x = m_aSnapVertex[nSnapY].x;
		tLine.P2.y = m_aSnapVertex[nSnapY].y;
		m_CurrDrawClass_Line.m_DataR.List.Add(tLine);
	}

	m_CurrDrawClass_Point.Convert(TRUE, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Line.Convert(TRUE, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
}

/*
// Diff는 Org-Cmp
BOOL CDgnSeisBearingPropEditBtn::IsSamePolygon( const DGN_GSEC_POLYGON_LIST& arOrg, const DGN_GSEC_POLYGON_LIST& arCmp, double& dDiffY, double& dDiffZ )
{
	dDiffY = 0.0;  
	dDiffZ = 0.0;

	int i, j;
	int nSizeOrg = arOrg.GetSize();
	int nSizeCmp = arCmp.GetSize();
	if(nSizeCmp != nSizeCmp)
		return FALSE;

	double dDiffYTemp;
	double dDiffZTemp;
	BOOL bDiffInit = TRUE;
	int nSizeVertexOrg;
	int nSizeVertexCmp;
	for(i = 0 ; i < nSizeOrg; i++)
	{
		nSizeVertexOrg = arOrg[i].aVertex.GetSize();
		nSizeVertexCmp = arCmp[i].aVertex.GetSize();
		if(nSizeVertexCmp != nSizeVertexOrg)
			return FALSE;

		for(j = 0; j < nSizeVertexOrg; j++)
		{
			if(bDiffInit)
			{
				dDiffY = arOrg[i].aVertex[j].x - arCmp[i].aVertex[j].x;
				dDiffZ = arOrg[i].aVertex[j].y - arCmp[i].aVertex[j].y;
			}
			else
			{
				dDiffYTemp = arOrg[i].aVertex[j].x - arCmp[i].aVertex[j].x;
				dDiffZTemp = arOrg[i].aVertex[j].y - arCmp[i].aVertex[j].y;

				if(fabs(dDiffY - dDiffYTemp) > 1E-5)
					return FALSE;
				if(fabs(dDiffZ - dDiffZTemp) > 1E-5)
					return FALSE;
			}
		}
	}
	return TRUE;
}
*/



BOOL CDgnSeisBearingPropEditBtn::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 휠 확대 축소 막음

	/*
	if(GetAsyncKeyState(VK_LSHIFT))
	{
		if(zDelta > 0)
		{
			//m_dTendonZoomFactor += 0.01;
//			if(m_dTendonZoomFactor > 0.05) m_dTendonZoomFactor = 0.05; //최대
		}
		else if(zDelta < 0)
		{
			//m_dTendonZoomFactor -= 0.01;
			//if(m_dTendonZoomFactor < 0.01) m_dTendonZoomFactor = 0.01; //최소
		}

		MakeDrawUnit(FALSE);
		return TRUE;
	}
	else
		return CDrawEditBtn::OnMouseWheel(nFlags, zDelta, pt);
		*/
	return TRUE;
}

void CDgnSeisBearingPropEditBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	// 화면 이동 막음
	return;
}

void CDgnSeisBearingPropEditBtn::MakeDrawSubClass()
{
	// 자식 Class에서 그림
}
/*
void CDgnSeisBearingPropEditBtn::SetParentSelectedKey( int nIdx, vecTndnKey& keyList, nrUINT& nrUintTarget, int idxOrigin )
{
	CCMDTendonTemplateDlg* pDlg = (CCMDTendonTemplateDlg*)GetParent();

	if(nIdx > 0)
	{
		pDlg->SetSelectedKeyList(keyList);
		SetSelectedLinkKeyList(nrUintTarget, TRUE);

	}
	else if(nIdx == 0 && idxOrigin < 0)
	{
		nrUintTarget.SetSize(0);
		SetSelectedKeyList(nrUintTarget, TRUE);
		pDlg->UnSelectAllKey();
	}
}
*/


