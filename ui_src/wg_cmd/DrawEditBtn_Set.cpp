#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"
// #include "../NS_lib/IProductInfo.h"
// #include "../NS_lib/IMSGInfo.h"

#include "DrawEditBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////
void CDrawEditBtn::SetFit()
{
	m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);
	m_DrawCtrl.ConvertDrawData(TRUE);

	m_nButtonCount = 0;
	CurrRemoveDrawData();
	SnapRemoveDrawData();
	SelectRemoveDrawData();
	OverlapChecking();
	RedrawWindow(); 
}

void CDrawEditBtn::SetMode(int nMode)
{
	m_nButtonCount = 0;
	m_nMode = nMode;

	DisplayModeText();

	m_nButtonCount = 0;
	CurrRemoveDrawData();
	SnapRemoveDrawData();
	SelectRemoveDrawData();
	RedrawWindow(); 
}

void CDrawEditBtn::SetSnap(BOOL bObjectSnap, BOOL bGridSnap, int nSnapDis)
{
	m_bObjectSnap = bObjectSnap;
	m_bGridSnap   = bGridSnap;
	m_DrawCtrl.SetSnapDis(nSnapDis);
}

void CDrawEditBtn::SetSelectDrawType(int nType)
{
	m_DrawCtrl.m_SelectDrawType = nType;
}

void CDrawEditBtn::SetAxisMode(int nAxisMode, int nSize, int nWidth, BOOL bCapital)
{
	m_nAxisMode  = nAxisMode;
	m_nAxisSize  = nSize;
	m_nAxisWidth = nWidth;
	m_bCapital   = bCapital;

	RedrawWindow(); 
}

void CDrawEditBtn::SetAxisLocal(double dOrgPoint[2], double dAxisVector1[2], double dAxisVector2[2])
{
	m_dAxisVector1[0] = dAxisVector1[0]; 
	m_dAxisVector1[1] = dAxisVector1[1];
	m_dAxisVector2[0] = dAxisVector2[0]; 
	m_dAxisVector2[1] = dAxisVector2[1];

	CMathFunc::mathNormalize(m_dAxisVector1[0], m_dAxisVector1[1], m_dAxisVector1[0], m_dAxisVector1[1]);
	CMathFunc::mathNormalize(m_dAxisVector2[0], m_dAxisVector2[1], m_dAxisVector2[0], m_dAxisVector2[1]);

	m_AxisOrgPointR.x = dOrgPoint[0];   
	m_AxisOrgPointR.y = dOrgPoint[1];
	m_bCDC_AxisOrgPoint = FALSE;

	RedrawWindow(); 
}

void CDrawEditBtn::SetGridMode(int nGridMode, double dInterval_x, double dInterval_y)
{
	m_nGridMode        = nGridMode;
	m_dGridInterval[0] = dInterval_x;
	m_dGridInterval[1] = dInterval_y;

	RedrawWindow(); 
}

void CDrawEditBtn::SetMirrorMode(int nMirrorMode)
{
	m_DrawCtrl.m_DrawFunc.ConvertPaperFormatMirror(m_DrawCtrl.m_PaperFormat, m_Canvas, m_DrawCtrl.m_nMirrorMode, nMirrorMode);
	m_DrawCtrl.m_nMirrorMode = nMirrorMode;	

	m_DrawCtrl.ConvertDrawData(TRUE);	
	CurrConvertDrawData(TRUE);
	OverlapChecking();
	RedrawWindow(); 
}

void CDrawEditBtn::SetWndCoordText(CBCGPStatic* pText)
{
	m_pCoordinateStatic = pText;
}

void CDrawEditBtn::SetWndModeText(CBCGPStatic* pText)
{
	m_pModeStatic = pText;
}

void CDrawEditBtn::SetWndPointEdit(CEdit* pEdit)
{
	m_pPointEdit = pEdit;
}

void CDrawEditBtn::SetWndLineEdit(CEdit* pStartEdit, CEdit* pEndEdit)
{
	m_pLineStartPEdit = pStartEdit;
	m_pLineEndPEdit   = pEndEdit;
}

void CDrawEditBtn::SetWndArcEdit(CEdit* pStartEdit, CEdit* pPassEdit, CEdit* pEndEdit, CEdit* pBulgeEdit, CEdit* pRadiusEdit, CButton* pInsideCheck)
{
	m_pArcStartPEdit = pStartEdit;
	m_pArcPassPEdit  = pPassEdit;
	m_pArcEndPEdit   = pEndEdit;
	m_pArcBulgeEdit  = pBulgeEdit;
	m_pArcRadiusEdit = pRadiusEdit;
	m_pArcInsideCheck= pInsideCheck;
}

void CDrawEditBtn::SetWndCircleEdit(CEdit* pStartEdit, CEdit* pEndEdit, CEdit* pRadiusEdit)
{
	m_pCircleStartPEdit = pStartEdit;
	m_pCircleEndPEdit   = pEndEdit;
	m_pCircleRadiusEdit = pRadiusEdit;
}

void CDrawEditBtn::SetWndPolylineEdit(CEdit* pStartEdit, CEdit* pPassEdit, CEdit* pEndEdit)
{
	m_pPolylineStartPEdit = pStartEdit;
	m_pPolylinePassPEdit  = pPassEdit;
	m_pPolylineEndPEdit   = pEndEdit;
}
void CDrawEditBtn::SetWndRectEdit(CEdit* pStartEdit, CEdit* pPassEdit, CEdit* pEndEdit)
{
	m_pRectCorner1PEdit = pStartEdit;
	m_pRectCorner2PEdit  = pEndEdit;
}
////////////////////////////////////////////////////////////////
void CDrawEditBtn::SetEntityFormat()
{	
	//m_DrawFunc.SetFontAddSize_RZero(3);
	// Axis
	m_AxisDrawFormat.Init();
	m_AxisDrawFormat.lineColor      = RGB(150, 150,   0);
	m_AxisDrawFormat.startArrowType = EN_ARROW_TYPE_OPEN;
	m_AxisDrawFormat.endArrowType   = EN_ARROW_TYPE_NONE;
	m_AxisDrawFormat.Width          = 1;
	m_AxisDrawFormat.dRWidth        = 3.0;
	m_AxisDrawFormat.dRHeight       = 3.0;	

	// Grid
	m_GridDrawFormat.Init();
	m_GridDrawFormat.lineColor      = RGB(250, 100, 100);
	m_GridDrawFormat.Width          = 1;

	// Snap
	m_SnapDrawFormat.Init();
	m_SnapDrawFormat.lineColor = RGB(255, 180,  20);
	m_SnapDrawFormat.Width     = 1;
	m_SnapDrawFormat.bFill     = FALSE;

	// Highlight	
	m_HighlightDrawFormat.Init();
	m_HighlightDrawFormat.lineColor      = RGB(255, 180,  20);
	m_HighlightDrawFormat.Width          = 1;
	m_HighlightDrawFormat.bFill          = FALSE;
	m_HighlightDrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	m_HighlightDrawFormat.endArrowType   = EN_ARROW_TYPE_NONE;

	// Current
	m_CurrDrawFormat_Point.Init();
	m_CurrDrawFormat_Point.lineColor= RGB(  0,   0,   0);
	m_CurrDrawFormat_Point.Width    = 3;
	m_CurrDrawFormat.Init();
	m_CurrDrawFormat.lineColor      = RGB(  0,   0,   0);
	m_CurrDrawFormat.Width          = 1;
	m_CurrDrawFormat.bFill          = FALSE;
	m_CurrDrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	m_CurrDrawFormat.endArrowType   = EN_ARROW_TYPE_NONE;

	m_CurrTextFormat.Init();
	m_CurrDimFormat.Init();

	// Overlap
	m_OverlapDimFormat.Init();
	m_OverlapDimFormat.fontColor = RGB(255,   0,   0);
	m_OverlapDimFormat.Width     = 1;
	m_OverlapDimFormat.dRTextSpace = 0.1;
	m_OverlapDimFormat.fontSize  = 12;

	// Default
	m_DefaultDrawFormatKey = 1;
	m_DefaultTextFormatKey = 1;
	m_DefaultDimFormatKey  = 1;

	T_DRAW_FORMAT      defaultDrawFormat;
	defaultDrawFormat.Init();
	defaultDrawFormat.lineColor      = RGB(200,   0,   0);
	defaultDrawFormat.fillColor      = RGB(  0, 200,   0);
	defaultDrawFormat.Width          = 2;
	defaultDrawFormat.bFill          = TRUE;
	defaultDrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	defaultDrawFormat.endArrowType   = EN_ARROW_TYPE_NONE;
	m_DrawCtrl.m_arDrawFormat[m_DefaultDrawFormatKey] = defaultDrawFormat;

	//IProductInfo* pProdInfo = IProductInfo::Instance();

	T_TEXT_FORMAT      defaultTextFormat;
	//defaultTextFormat.Init(pProdInfo->GetLanguageID());
	defaultTextFormat.Init(0x01);
	m_DrawCtrl.m_arTextFormat[m_DefaultTextFormatKey] = defaultTextFormat;

	T_DIM_FORMAT defaultDimFormat ;
	//defaultDimFormat.Init(pProdInfo->GetLanguageID());
	defaultDimFormat.Init(0x01);
	m_DrawCtrl.m_arDimFormat[m_DefaultDimFormatKey] = defaultDimFormat;

	// Select Line
	m_SelectAreaDrawFormat.Init();
	m_SelectAreaDrawFormat.lineColor = RGB(0, 0, 0);
	m_SelectAreaDrawFormat.Width     = 1;
	m_SelectAreaDrawFormat.bFill     = FALSE;
	m_SelectAreaDrawFormat.nPenStyle = PS_DOT;
	// Select Move
	m_CurrDrawFormat_Point_SelectMove.Init();
	m_CurrDrawFormat_Point_SelectMove.lineColor = RGB(0, 0, 0);
	m_CurrDrawFormat_Point_SelectMove.Width = 3;
	m_CurrDrawFormat_Point_SelectMove.bFill = FALSE;
	m_CurrDrawFormat_Point_SelectMove.nPenStyle = PS_DOT;	
	m_CurrDrawFormat_SelectMove.Init();
	m_CurrDrawFormat_SelectMove.lineColor = RGB(0, 0, 0);
	m_CurrDrawFormat_SelectMove.Width = 1;
	m_CurrDrawFormat_SelectMove.bFill = FALSE;
	m_CurrDrawFormat_SelectMove.nPenStyle = PS_DOT;	
	m_CurrTextFormat_SelectMove.Init();
	m_CurrTextFormat_SelectMove.fontColor = RGB(0, 0, 0);		
	m_CurrTextFormat_SelectMove.bFill = FALSE;	
	m_CurrDimFormat_SelectMove.Init();
	m_CurrDimFormat_SelectMove.fontColor = RGB(0, 0, 0);
	m_CurrDimFormat_SelectMove.lineColor = RGB(0, 0, 0);
	m_CurrDimFormat_SelectMove.Width = 1;
	//m_CurrDimFormat_SelectMove.nPenStyle = PS_DASH;	
	// Selected
	m_DrawCtrl.m_SelectDrawFormat.lineColor = RGB(255, 140,  00);
	m_DrawCtrl.m_SelectDrawFormat.fillColor = RGB(255, 230, 100);
	m_DrawCtrl.m_SelectDrawFormat.Width     = 1;
	m_DrawCtrl.m_SelectTextFormat.fontColor = RGB(255, 140,  00);
	m_DrawCtrl.m_SelectTextFormat.fillColor = RGB(255, 230, 100);
	m_DrawCtrl.m_SelectDimFormat.lineColor  = RGB(255, 140,  00);
	m_DrawCtrl.m_SelectDimFormat.fontColor  = RGB(255, 140,  00);
	m_DrawCtrl.m_SelectDimFormat.Width      = 1;
}
