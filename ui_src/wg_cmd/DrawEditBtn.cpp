#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"

#include "DrawEditBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawEditBtn::CDrawEditBtn() 
{
	m_bCutoffInput = FALSE;
	m_dZero = 0.0000001;
	//m_pConnector          = NULL;
	m_pModeStatic         = NULL;       
	m_pCoordinateStatic   = NULL; 	
	m_pPointEdit          = NULL;	
	m_pLineStartPEdit     = NULL;
	m_pLineEndPEdit       = NULL;	
	m_pArcStartPEdit      = NULL;
	m_pArcPassPEdit       = NULL;
	m_pArcEndPEdit        = NULL;
	m_pArcBulgeEdit       = NULL;
	m_pArcRadiusEdit      = NULL;
	m_pArcInsideCheck     = NULL;
	m_pCircleStartPEdit   = NULL;
	m_pCircleEndPEdit     = NULL;
	m_pCircleRadiusEdit   = NULL;
	m_pPolylineStartPEdit = NULL;
	m_pPolylinePassPEdit  = NULL;
	m_pPolylineEndPEdit   = NULL;
	m_pRectCorner1PEdit   = NULL;
	m_pRectCorner2PEdit   = NULL;

	m_BkGroudColor = RGB(255, 255, 255);
	m_dFitMargin = 0.05;

	m_bObjectSnap = FALSE;
	m_bGridSnap   = FALSE;
	m_dZoomRatio  = pow(2.0, 1.0 / 3.0);
	m_bUseZoom    = TRUE;
	m_bUseMove    = TRUE;
	m_bUseDel     = TRUE;
	m_bUseCopy    = TRUE;
	m_nOverlapType= EN_OVERLAP_NONE;
	m_nMode       = EN_DRAW_WINMODE_NONE;  

	m_DefaultDrawFormatKey = 0;
	m_DefaultTextFormatKey = 0;
	m_DefaultDimFormatKey  = 0;

	m_nAxisMode  = EN_DRAW_AXISMODE_NONE;
	m_nAxisSize  = 0;
	m_nAxisWidth = 0;
	m_bCapital   = TRUE;

	m_nGridMode  = EN_DRAW_GRIDMODE_NONE;
	m_dGridInterval[0] = 0.0;
	m_dGridInterval[1] = 0.0;

	m_Canvas.left = m_Canvas.top = 0;
	m_Canvas.right = m_Canvas.bottom = 300;

	m_nLButtonState = 0;
	m_nRButtonState = 0;
	m_nMButtonState = 0;
	m_nButtonCount  = 0;

	m_nShiftButtonState = 0;
	m_nCtrlButtonState  = 0;
	m_bReSelectDrawUnit = FALSE;

	m_bCDC_AxisOrgPoint = FALSE;
	m_AxisOrgPoint.x = m_AxisOrgPoint.y = 0; // Local좌표축을 사용할 경우 좌표원점
	m_AxisOrgPointR.Init();
	m_dAxisVector1[0] = 1.0;   m_dAxisVector1[1] = 0.0;// Local좌표축을 사용할 경우 첫번째 좌표축의 방향
	m_dAxisVector2[0] = 0.0;   m_dAxisVector2[1] = 1.0;// Local좌표축을 사용할 경우 두번째 좌표축의 방향

	m_bContextBitmap = FALSE;
	m_bContextEmf = FALSE;

	SetEntityFormat();
}

CDrawEditBtn::~CDrawEditBtn()
{
}

BEGIN_MESSAGE_MAP(CDrawEditBtn, CButton)
	//{{AFX_MSG_MAP(CDrawEditBtn)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//ON_WM_KEYDOWN()	// hslee : Parent(CDgnRebarGenInputDlg)의 PreTranslateMessage에서 호출함
	//ON_WM_KEYUP()		// hslee : Parent(CDgnRebarGenInputDlg)의 PreTranslateMessage에서 호출함
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_CMD_MAKE_BMP, OnMakeBMP)
	ON_COMMAND(IDM_CMD_MAKE_EMF, OnMakeEMF)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDrawEditBtn message handlers

BOOL CDrawEditBtn::Init()
{
	CRect Rect;
	GetWindowRect(Rect);
	ScreenToClient(Rect);

	m_Canvas.left = 0;
	m_Canvas.top  = 0;
	m_Canvas.right = Rect.Width();
	m_Canvas.bottom = Rect.Height();

	return TRUE;  
}

void CDrawEditBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	DrawPaint(pDC);
}

void CDrawEditBtn::DrawPaint(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	m_Bitmap.DeleteObject();
	m_Bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* oldBitmap = MemDC.SelectObject(&m_Bitmap);

	// 바탕색 : white
	CBrush  bckBrh(m_BkGroudColor);
	CBrush* pOldBrh = MemDC.SelectObject(&bckBrh);

	MemDC.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	MemDC.SelectObject(pOldBrh);
	bckBrh.DeleteObject();

	// **************** 결정된 부분(Section, Rebar등..)을 그리는 부분 *************
	m_DrawCtrl.SetHDC(MemDC.m_hDC);
	m_DrawCtrl.Draw(m_Canvas);

	// **************** Temp Data를 그리는 부분 *******************
	DrawRender();
	// *********************************************************

	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, rect.left, rect.top, SRCCOPY);
	MemDC.SelectObject(oldBitmap);
}

void CDrawEditBtn::DrawRender()  // hslee : CDgnGenSectDraw에서 재정의 해서 이부분은 불려지지 않음..
{
	HighlightDraw();
	CurrDraw();
	OverlapDraw();
	SelectDraw();
	SnapDraw();
	AxisDraw();
	GridDraw();	
}

BOOL CDrawEditBtn::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CDrawEditBtn::OnSize(UINT nType, int cx, int cy) 
{
	CButton::OnSize(nType, cx, cy);
	if(cx == 0 || cy == 0) return;
}

// Mouse ///////////////////////////////////////////////////////////////////////
void CDrawEditBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bCutoffInput = TRUE;	
	m_nLButtonState = 1;
	m_nRButtonState = 0;
	m_nMButtonState = 0;
	SetCapture();

	switch(m_nMode)
	{
	case EN_DRAW_WINMODE_VIEW:
		m_StartOrgPoint = m_DrawCtrl.m_PaperFormat.OrgPoint;
		m_StartOrgScale[0] = m_DrawCtrl.m_PaperFormat.ScaleX;
		m_StartOrgScale[1] = m_DrawCtrl.m_PaperFormat.ScaleY;
		m_StartPoint    = point;
		break;
	case EN_DRAW_WINMODE_SELECT:
		StartCurrUnitSelect(point);			
		break;
	case EN_DRAW_WINMODE_POINT:
		CompleteCurrUnitPoint(point);
		break;
	case EN_DRAW_WINMODE_LINE:
		if(m_nButtonCount == 0) StartCurrUnitLine(point);
		else                    CompleteCurrUnitLine(point);
		break;
	case EN_DRAW_WINMODE_ARC:
		if(m_nButtonCount == 0)      StartCurrUnitArc(point);
		else if(m_nButtonCount == 1) PassCurrUnitArc(point);
		else                         CompleteCurrUnitArc(point);
		break;
	case EN_DRAW_WINMODE_ARC_B:
		if(m_nButtonCount == 0)      StartCurrUnitArcB(point);
		else if(m_nButtonCount == 1) PassCurrUnitArcB(point);
		else                         CompleteCurrUnitArcB(point);
		break;
	case EN_DRAW_WINMODE_CIRCLE:
		if(m_nButtonCount == 0) StartCurrUnitCircle(point);
		else                    CompleteCurrUnitCircle(point);
		break;
	case EN_DRAW_WINMODE_POLYLINE:
		if(m_nButtonCount == 0) StartCurrUnitPolyline(point);
		else                    PassCurrUnitPolyline(point); // 시작점과 동일시 자동으로 Complete_CurrUnit_Polyline으로 변환됨
		break;
	case EN_DRAW_WINMODE_RECT:
		if(m_nButtonCount == 0) StartCurrUnitRect(point);
		else                    CompleteCurrUnitRect(point); 
		break;				
	default:
		RunUpdateCompleteObjectData(EN_DRAW_UPDATE_LBTN_DOWN_NONE, 0.0, 0.0);	
		break;	
	}
	CButton::OnLButtonDown(nFlags, point);
	m_bCutoffInput = FALSE;
}

void CDrawEditBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE;//외부입력 차단

	switch(m_nMode)
	{
	case EN_DRAW_WINMODE_SELECT:
		CompleteCurrUnitSelect(point);
		break;			
	default:
		RunUpdateCompleteObjectData(EN_DRAW_UPDATE_LBTN_UP_NONE, 0.0, 0.0);		
		break;	
	}

	CButton::OnLButtonUp(nFlags, point);

	ReleaseCapture();
	m_nLButtonState = 0;
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE;//외부입력 차단

	switch(m_nMode)
	{
	case EN_DRAW_WINMODE_POLYLINE:
		CompleteCurrUnitPolyline(point, FALSE);
		break;			
	default:
		RunUpdateCompleteObjectData(EN_DRAW_UPDATE_LBTN_DBLCLK, 0.0, 0.0);
		break;	
	}

	CButton::OnLButtonDblClk(nFlags, point);
	m_bCutoffInput = FALSE;
}

void CDrawEditBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE; //외부입력 차단
	
	switch(m_nMode)
	{
	case EN_DRAW_WINMODE_VIEW:
		if(m_nLButtonState == 1) MovePan(point);
		if(m_nRButtonState == 1) MoveZoom(point);
		break;
	case EN_DRAW_WINMODE_SELECT:
		if(m_nLButtonState == 0 && m_nMButtonState == 0 && m_nRButtonState == 0)
		{				
			double dCoord[2];
			GetGenuineCoord(point, dCoord[0], dCoord[1]);	
			ChangeSnapPoint(point, dCoord);// Snap이 적용될 경우 좌표변환
			ChangeHighlightLine(point); // Highlight 좌표 변환
			RedrawWindow();
		}
		if(m_nLButtonState == 1 && m_nButtonCount == 0) BeforeCurrUnitSelect(point);    // 해당 상황은 발생되지 않음
		if(m_nLButtonState == 1 && m_nButtonCount == 1) InProgressCurrUnitSelect(point);// 모두 이 상황에 해당됨		
		break;
	case EN_DRAW_WINMODE_POINT:
		if(m_nLButtonState == 0 && m_nButtonCount == 0) BeforeCurrUnitPoint(point);
		break;
	case EN_DRAW_WINMODE_LINE:
		if(m_nLButtonState == 0)
		{
			if(m_nButtonCount == 0)      BeforeCurrUnitLine(point);
			else if(m_nButtonCount == 1) InProgressCurrUnitLine(point);
		}
		break;
	case EN_DRAW_WINMODE_ARC:			
		if(m_nLButtonState == 0)
		{
			if(m_nButtonCount == 0)								BeforeCurrUnitArc(point);
			else if(m_nButtonCount == 1 || m_nButtonCount == 2) InProgressCurrUnitArc(point);
		}
		break;
	case EN_DRAW_WINMODE_ARC_B:			
		if(m_nLButtonState == 0)
		{
			if(m_nButtonCount == 0)								BeforeCurrUnitArcB(point);
			else if(m_nButtonCount == 1 || m_nButtonCount == 2) InProgressCurrUnitArcB(point);
		}
		break;
	case EN_DRAW_WINMODE_CIRCLE:
		if(m_nLButtonState == 0)
		{
			if(m_nButtonCount == 0)      BeforeCurrUnitCircle(point);
			else if(m_nButtonCount == 1) InProgressCurrUnitCircle(point);
		}
		break;
	case EN_DRAW_WINMODE_POLYLINE:
		if(m_nLButtonState == 0)
		{
			if(m_nButtonCount == 0)      BeforeCurrUnitPolyline(point);
			else if(m_nButtonCount >= 1) InProgressCurrUnitPolyline(point);
		}
	case EN_DRAW_WINMODE_RECT:
		if(m_nLButtonState == 0)
		{
			if(m_nButtonCount == 0)      BeforeCurrUnitRect(point);
			else if(m_nButtonCount >= 1) InProgressCurrUnitRect(point);
		}
		break;			
	default:
		RunUpdateCompleteObjectData(EN_DRAW_UPDATE_MOUSE_MOVE_NONE, 0.0, 0.0);		
		break;	
	}

	if(m_nMButtonState == 1)
	{
		MovePan(point);                                         // 중간 버튼으로 Move Pan실행
		SetFocus();
	}

	SetCursorType();

	DisplayCoordText(point);

	CButton::OnMouseMove(nFlags, point);
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

BOOL CDrawEditBtn::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{	
	CRect PHRect;
	GetWindowRect(PHRect);

	nrLine nrLineEmpty;
	MakeHighLightGroup(nrLineEmpty);

	if(PHRect.PtInRect(pt))
	{
		CPoint pt_PH;
		pt_PH.x = pt.x - PHRect.left;
		pt_PH.y = pt.y - PHRect.top;

		if(zDelta > 0)  
		{
			CDrawEditBtn::ZoomIn(pt_PH);
		}
		else
		{
			CDrawEditBtn::ZoomOut(pt_PH);
		}
	}		
		
	BOOL bChk = CButton::OnMouseWheel(nFlags, zDelta, pt);
	m_bCutoffInput = FALSE;
	return bChk;
}

void CDrawEditBtn::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE;//외부입력 차단
	m_nLButtonState = 0;
	m_nRButtonState = 0;
	m_nMButtonState = 1;
	SetCapture();

	switch(m_nMode)
	{
	case EN_DRAW_WINMODE_SELECT:
		m_StartOrgPoint    = m_DrawCtrl.m_PaperFormat.OrgPoint;
		m_StartOrgScale[0] = m_DrawCtrl.m_PaperFormat.ScaleX;
		m_StartOrgScale[1] = m_DrawCtrl.m_PaperFormat.ScaleY;
		m_StartPoint       = point;
		break;		
	default:

		// hslee : 그리는 중에도 중간버튼으로 화면 이동 가능하게 하기 위해 추가..
		m_StartOrgPoint    = m_DrawCtrl.m_PaperFormat.OrgPoint;
		m_StartOrgScale[0] = m_DrawCtrl.m_PaperFormat.ScaleX;
		m_StartOrgScale[1] = m_DrawCtrl.m_PaperFormat.ScaleY;
		m_StartPoint       = point;

		RunUpdateCompleteObjectData(EN_DRAW_UPDATE_MBTN_DOWN_NONE, 0.0, 0.0);		
		break;	
	}

	SetCursorType();

	CButton::OnMButtonDown(nFlags, point);
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE;//외부입력 차단

	RunUpdateCompleteObjectData(EN_DRAW_UPDATE_MBTN_UP_NONE, 0.0, 0.0);	

	CButton::OnMButtonUp(nFlags, point);

	ReleaseCapture();
	m_nMButtonState = 0;

	SetCursorType();

	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE;//외부입력 차단

	CButton::OnMButtonDblClk(nFlags, point);
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE;//외부입력 차단	
	m_nLButtonState = 0;
	m_nRButtonState = 1;
	m_nMButtonState = 0;
	SetCapture();

	switch(m_nMode)
	{
	case EN_DRAW_WINMODE_VIEW:
		m_StartOrgPoint    = m_DrawCtrl.m_PaperFormat.OrgPoint;
		m_StartOrgScale[0] = m_DrawCtrl.m_PaperFormat.ScaleX;
		m_StartOrgScale[1] = m_DrawCtrl.m_PaperFormat.ScaleY;
		m_StartPoint       = point;
		break;	
	default:
		break;	
	}
	CButton::OnRButtonDown(nFlags, point);
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE;//외부입력 차단

	RunUpdateCompleteObjectData(EN_DRAW_UPDATE_MBTN_UP_NONE, 0.0, 0.0);		

	CButton::OnRButtonUp(nFlags, point);

	ReleaseCapture();
	m_nRButtonState = 0;
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	m_bCutoffInput = TRUE;//외부입력 차단

	RunUpdateCompleteObjectData(EN_DRAW_UPDATE_MBTN_DBLCLK_NONE, 0.0, 0.0);		

	CButton::OnRButtonDblClk(nFlags, point);
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_bCutoffInput = TRUE;//외부입력 차단

	RunUpdateCompleteObjectData(EN_DRAW_UPDATE_RBTN_DBLCLK_NONE, 0.0, 0.0);	

	KeyDown(nChar);

	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::OnMakeEMF()
{
	CString strFileName;

	CFileDialog dlg(FALSE, _T("emf"), _T("*.emf"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Windows meta file to export (*.emf)"));

	if(dlg.DoModal() == IDOK)
	{
		strFileName = dlg.GetPathName();
	}

	CRect Rect;
	GetWindowRect(Rect);
	MakeEmfFile(strFileName, Rect.Width() * 100, Rect.Height() * 100);
}

void CDrawEditBtn::OnMakeBMP()
{
	CString strFileName;

	CFileDialog dlg(FALSE, _T("bmp"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bitmap file to export (*.bmp)"));

	if(dlg.DoModal() == IDOK)
	{
		strFileName = dlg.GetPathName();
	}

	MakeBMP(strFileName);
}

void CDrawEditBtn::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if(!m_bContextBitmap && !m_bContextEmf) return;

	CMenu pContextMenu;		
	pContextMenu.CreatePopupMenu();
	if(m_bContextBitmap)  pContextMenu.AppendMenu( MF_STRING,	IDM_CMD_MAKE_BMP,			_LS(IDS_CMD_MAKE_BMP));
	if(m_bContextEmf)  pContextMenu.AppendMenu( MF_STRING,	IDM_CMD_MAKE_EMF,			_LS(IDS_CMD_MAKE_EMF));
	pContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	pContextMenu.DestroyMenu();
	pContextMenu.DestroyMenu();
}

void CDrawEditBtn::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	m_bCutoffInput = TRUE;//외부입력 차단
	CButton::OnKeyUp(nChar, nRepCnt, nFlags);

	KeyUp(nChar);

	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::ZoomOut(CPoint CurrP)
{
	if(m_dZoomRatio == 0.0) return;
	ZoomRatio(CurrP, 1.0 /m_dZoomRatio);
}

void CDrawEditBtn::ZoomIn(CPoint CurrP)
{
	ZoomRatio(CurrP, m_dZoomRatio);
}

void CDrawEditBtn::ZoomOut()
{
	if(m_dZoomRatio == 0.0) return;
	ZoomRatio(m_DrawCtrl.m_PaperFormat.OrgPoint, 1.0 / m_dZoomRatio);
}

void CDrawEditBtn::ZoomIn()
{
	ZoomRatio(m_DrawCtrl.m_PaperFormat.OrgPoint, m_dZoomRatio);
}

void CDrawEditBtn::SetContext(BOOL bContextBitmap, BOOL bContextEmf)
{
	m_bContextBitmap = bContextBitmap;
	m_bContextEmf = bContextEmf;
}

void CDrawEditBtn::ZoomRatio(CPoint CurrP, double dZoomRatio)
{
	if(!m_bUseZoom) return;
	if(dZoomRatio == 0.0) return;
	
	double dScaleX = m_DrawCtrl.m_PaperFormat.ScaleX / dZoomRatio;
	double dScaleY = m_DrawCtrl.m_PaperFormat.ScaleY / dZoomRatio;
	double dMinScale = 0.0001;
	double dMaxScale = 100000.0;
	
	if(fabs(dScaleX) < dMinScale || fabs(dScaleX) > dMaxScale) return;
	if(fabs(dScaleY) < dMinScale || fabs(dScaleY) > dMaxScale) return;

	CPoint OrgP = m_DrawCtrl.m_PaperFormat.OrgPoint;
	int nX = OrgP.x - CurrP.x;
	int nY = OrgP.y - CurrP.y;

	m_DrawCtrl.m_PaperFormat.OrgPoint.x = CurrP.x + (int)(nX * dZoomRatio);
	m_DrawCtrl.m_PaperFormat.OrgPoint.y = CurrP.y + (int)(nY * dZoomRatio);
	m_DrawCtrl.m_PaperFormat.ScaleX = dScaleX;
	m_DrawCtrl.m_PaperFormat.ScaleY = dScaleY;

	m_DrawCtrl.ConvertDrawData(TRUE);
	CurrConvertDrawData(TRUE);
	OverlapChecking();

	RedrawWindow();
}

int CDrawEditBtn::GetSelectedKeyList(nrUINT& arKey)
{
	return m_DrawCtrl.GetSelectedKeyList(arKey);
}

int CDrawEditBtn::GetSelectedLinkKeyList(nrUINT& arLinkKey)
{
	return m_DrawCtrl.GetSelectedLinkKeyList(arLinkKey);
}

void CDrawEditBtn::SetSelectedKeyList(nrUINT& arKey, BOOL bReDraw)
{
	m_DrawCtrl.SetSelectedKeyList(arKey);
	if(bReDraw) RedrawWindow();
}

void CDrawEditBtn::SetSelectedLinkKeyList(nrUINT& arLinkKey, BOOL bReDraw)
{
	m_DrawCtrl.SetSelectedLinkKeyList(arLinkKey);
	if(bReDraw) RedrawWindow();
}

void CDrawEditBtn::RemoveAllSelectedKey()
{
	m_DrawCtrl.RemoveAllSelectedKey();
	RedrawWindow();
}

void CDrawEditBtn::AddSelectedKey(UINT key)
{
	m_DrawCtrl.AddSelectedKey(key);
	RedrawWindow();
}

void CDrawEditBtn::AddSelectedKey(nrUINT& arKey)
{
	m_DrawCtrl.AddSelectedKey(arKey);
	RedrawWindow();
}

void CDrawEditBtn::DelSelectedKey(UINT key)
{
	m_DrawCtrl.DelSelectedKey(key);
	RedrawWindow();
}

void CDrawEditBtn::DelSelectedKey(nrUINT& arKey)
{
	m_DrawCtrl.DelSelectedKey(arKey);
	RedrawWindow();
}

BOOL CDrawEditBtn::IsSelected(UINT key)
{
	return m_DrawCtrl.IsSelected(key);
}

void CDrawEditBtn::ClearCurrUnit()
{
	if(m_bCutoffInput) return; //내부동작중

	SnapRemoveDrawData();
	CurrRemoveDrawData();	
	SelectRemoveDrawData();
	m_nButtonCount = 0;
	m_bSelectMoveMode = FALSE;
	RedrawWindow();	
}

void CDrawEditBtn::SetCurrUnitPoint(double dStartP[2])
{
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_POINT) return;
	CurrRemoveDrawData();
	MakeCurrUnitPoint(dStartP);	
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::SetCurrUnitLine(int nCount)
{
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_LINE) return;
	
	double dStartP[2], dEndP[2];
	
	if(! GetCoordFromEdit(m_pLineStartPEdit, dStartP[0], dStartP[1])) return;
	if(! GetCoordFromEdit(m_pLineEndPEdit, dEndP[0], dEndP[1])) return;
	
	SetCurrUnitLine(dStartP, dEndP, nCount);
}

void CDrawEditBtn::SetCurrUnitLineA(double dStartP[2], double dEndP[2], int nCount)
{
	if(m_bCutoffInput) return; // 내부동작중
	// if(m_nMode != EN_DRAW_WINMODE_LINE) return;

	m_StartPointR[0] = dStartP[0]; 
	m_StartPointR[1] = dStartP[1];
	GetDisplayCoord(dStartP[0], dStartP[1], m_StartPoint);

	if(nCount >= 0)
	{
		nCount = max(0, min(1, nCount));
		m_nButtonCount = nCount;	
	}
	CurrRemoveDrawData();
	MakeCurrUnitLine(dStartP, dEndP);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::SetCurrUnitLine(double dStartP[2], double dEndP[2], int nCount)
{
	if(m_bCutoffInput) return; // 내부동작중
	if(m_nMode != EN_DRAW_WINMODE_LINE) return;

	m_StartPointR[0] = dStartP[0];
	m_StartPointR[1] = dStartP[1];
	GetDisplayCoord(dStartP[0], dStartP[1], m_StartPoint);

	if(nCount >= 0)
	{
		nCount = max(0, min(1, nCount));
		m_nButtonCount = nCount;	
	}
	CurrRemoveDrawData();
	MakeCurrUnitLine(dStartP, dEndP);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::SetCurrUnitLineGuide(double dStartP[2], double dEndP[2], int no, BOOL bRemove)
{
	m_StartPointR[0] = dStartP[0];
	m_StartPointR[1] = dStartP[1];
	GetDisplayCoord(dStartP[0], dStartP[1], m_StartPoint);

	m_nButtonCount = 1;	
	
	if(bRemove) CurrRemoveDrawData();
	MakeCurrUnitLine(no, dStartP, dEndP);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::SetCurrUnitArc(int nCount, int nType)
{
	if(m_bCutoffInput) return;//내부동작중

	if(m_nMode != EN_DRAW_WINMODE_ARC && m_nMode != EN_DRAW_WINMODE_ARC_B) return;

	if(nType == 1)
	{
		double dStartP[2], dPassP[2], dEndP[2];
		if(!GetCoordFromEdit(m_pArcStartPEdit, dStartP[0], dStartP[1])) return;
		if(!GetCoordFromEdit(m_pArcPassPEdit, dPassP[0], dPassP[1])) return;
		if(!GetCoordFromEdit(m_pArcEndPEdit, dEndP[0], dEndP[1])) return;
		SetCurrUnitArc(dStartP, dPassP, dEndP, nCount);
	}
	else if(nType == 2)
	{
		double dStartP[2], dEndP[2];
		double dBugle;
		if(!GetCoordFromEdit(m_pArcStartPEdit, dStartP[0], dStartP[1])) return;		
		if(!GetCoordFromEdit(m_pArcEndPEdit, dEndP[0], dEndP[1])) return;
		if(!GetValueFromEdit(m_pArcBulgeEdit, dBugle)) return;
		SetCurrUnitArcBugle(dStartP, dEndP, dBugle, nCount);
	}
	else if(nType == 3)
	{
		double dStartP[2], dEndP[2];
		double dRadius;
		BOOL   bInside;
		if(!GetCoordFromEdit(m_pArcStartPEdit, dStartP[0], dStartP[1])) return;		
		if(!GetCoordFromEdit(m_pArcEndPEdit, dEndP[0], dEndP[1])) return;
		if(!GetValueFromEdit(m_pArcRadiusEdit, dRadius)) return;
		if(!GetValueFromChkBox(m_pArcInsideCheck, bInside)) return;
		SetCurrUnitArcRadius(dStartP, dEndP, dRadius, bInside, nCount);
	}
	else ASSERT(0);
}

void CDrawEditBtn::SetCurrUnitArc(double dStartP[2], double dPassP[2], double dEndP[2], int nCount)
{
	if(m_bCutoffInput) return;//내부동작중
	if(m_nMode != EN_DRAW_WINMODE_ARC && m_nMode != EN_DRAW_WINMODE_ARC_B) return;

	m_StartPointR[0] = dStartP[0];  m_StartPointR[1] = dStartP[1];
	
	if(m_nMode == EN_DRAW_WINMODE_ARC)
	{
		m_InterPointR[0] = dPassP[0]; 
		m_InterPointR[1] = dPassP[1];
	}
	else if(m_nMode == EN_DRAW_WINMODE_ARC_B) // 두번째 점은 호의 중간점
	{
		m_InterPointR[0] = dEndP[0]; 
		m_InterPointR[1] = dEndP[1];
	}
	else ASSERT(0);  // 두번째 점은 호의 끝점

	GetDisplayCoord(dStartP[0], dStartP[1], m_StartPoint);
	GetDisplayCoord(dPassP[0],  dPassP[1],  m_InterPoint);

	if(nCount >= 0)
	{
		nCount = max(0, min(2, nCount));
		m_nButtonCount = nCount;
	}
	CurrRemoveDrawData();
	MakeCurrUnitArc(dStartP, dPassP, dEndP);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::SetCurrUnitArcBugle(double dStartP[2], double dEndP[2], double dBugle, int nCount)
{
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_ARC && m_nMode != EN_DRAW_WINMODE_ARC_B) return;

	double dMiddleP[2];
	if(CMathFunc::mathLength(dStartP[0], dStartP[1], dEndP[0], dEndP[1]) < CMathFunc::m_NormalZero)
	{
		dMiddleP[0] = dEndP[0];   
		dMiddleP[1] = dEndP[1];
	}
	else
	{
		double lineVector[2];
		CMathFunc::mathNormalize(dEndP[0]-dStartP[0], dEndP[1]-dStartP[1], lineVector[0], lineVector[1]);

		dMiddleP[0] = (dEndP[0]+dStartP[0])/2.0 + dBugle*lineVector[1];
		dMiddleP[1] = (dEndP[1]+dStartP[1])/2.0 - dBugle*lineVector[0];
	}

	SetCurrUnitArc(dStartP, dMiddleP, dEndP, nCount);
}

void CDrawEditBtn::SetCurrUnitArcRadius(double dStartP[2], double dEndP[2], double dRadius, BOOL bInside, int nCount)
{	
	if(m_bCutoffInput) return;//내부동작중
	if(m_nMode != EN_DRAW_WINMODE_ARC && m_nMode != EN_DRAW_WINMODE_ARC_B) return;

	double line[2][2];
	line[0][0] = dStartP[0];    line[0][1] = dStartP[1];
	line[1][0] = dEndP[0];      line[1][1] = dEndP[1];
	double dBulge = CMathFunc::GetBulge(line, dRadius, bInside);

	SetCurrUnitArcBugle(dStartP, dEndP, dBulge, nCount);
}

void CDrawEditBtn::SetCurrUnitCircle(int nCount, int nType)
{
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_CIRCLE) return;
	double dStartP[2], dEndP[2];
	
	if(!GetCoordFromEdit(m_pCircleStartPEdit, dStartP[0], dStartP[1])) return;
	
	if(nType == 1)
	{
		double dRadius;
		if(!GetValueFromEdit(m_pCircleRadiusEdit, dRadius)) return;
		dEndP[0] = dStartP[0] + dRadius;
		dEndP[1] = dStartP[1];
	}
	else
	{
		if(!GetCoordFromEdit(m_pCircleEndPEdit, dEndP[0], dEndP[1])) return;
	}
	SetCurrUnitCircle(dStartP, dEndP, nCount);
}

void CDrawEditBtn::SetCurrUnitCircle(double dStartP[2], double dEndP[2], int nCount)
{
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_CIRCLE) return;

	m_StartPointR[0] = dStartP[0];  m_StartPointR[1] = dStartP[1];
	GetDisplayCoord(dStartP[0], dStartP[1], m_StartPoint);

	if(nCount >= 0)
	{
		nCount = max(0, min(1, nCount));
		m_nButtonCount = nCount;
	}
	CurrRemoveDrawData();
	MakeCurrUnitCircle(dStartP, dEndP);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::SetCurrUnitPolyline(int nEditType)
{	
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_POLYLINE) return;
	
	double dStartP[2], dPassP[2], dEndP[2];
	
	if(! GetCoordFromEdit(m_pPolylineStartPEdit, dStartP[0], dStartP[1])) return;
	if(! GetCoordFromEdit(m_pPolylinePassPEdit, dPassP[0], dPassP[1])) return;
	if(! GetCoordFromEdit(m_pPolylineEndPEdit, dEndP[0], dEndP[1])) return;
	
	SetCurrUnitPolyline(dStartP, dPassP, dEndP, nEditType);
}

void CDrawEditBtn::SetCurrUnitPolyline(double dStartP[2], double dPassP[2], double dEndP[2], int nEditType)
{
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_POLYLINE) return;
	int nSize = m_CurrPolylineRD.arPoint.GetSize();
	T_POINT PointR; 

	if(nEditType < 0 || nEditType == 2)
	{
		CString strStartCoord = GetCoordText(m_StartPointR[0], m_StartPointR[1]);
		CString strEndCoord   = GetCoordText(dEndP[0], dEndP[1]);
		
		if(strStartCoord == strEndCoord)
		{
			if(nEditType < 0)
			{
				dEndP[0] = m_StartPointR[0];  
				dEndP[1] = m_StartPointR[1];
			}
			else 
			{
				dStartP[0] = dEndP[0];
				dStartP[1] = dEndP[1]; 
				nEditType = 0; 
			}
		}
	}

	if(nEditType == 0)
	{
		m_StartPointR[0] = dStartP[0]; 
		m_StartPointR[1] = dStartP[1];
		GetDisplayCoord(dStartP[0], dStartP[1], m_StartPoint);
		PointR.x = dStartP[0]; 
		PointR.y = dStartP[1];
		
		if(nSize == 0) 
		{ 
			m_CurrPolylineRD.arPoint.Add(PointR); 
			nSize++; 
			m_nButtonCount = nSize; 
		}
		else           
		{
			m_CurrPolylineRD.arPoint[0] = PointR;
		}
	}
	else if(nEditType == 1)
	{
		m_InterPointR[0]  = dPassP[0];  
		m_InterPointR[1]  = dPassP[1];
		GetDisplayCoord(dPassP[0],  dPassP[1],  m_InterPoint);
		PointR.x = dPassP[0];
		PointR.y = dPassP[1];

		if(nSize <= 1)
		{
			m_StartPointR[0] = dPassP[0]; 
			m_StartPointR[1] = dPassP[1];
			m_StartPoint = m_InterPoint;
		}

		if(nSize < 1) 
		{
			m_CurrPolylineRD.arPoint.Add(PointR);
			nSize++; 
			m_nButtonCount = nSize;
		}
		else      
		{
			m_CurrPolylineRD.arPoint[nSize - 1] = PointR;
		}
	}	

	CurrRemoveDrawData();
	MakeCurrUnitPolyline(m_CurrPolylineRD, dEndP);
	CurrConvertDrawData(TRUE);
	RedrawWindow();	
}


void CDrawEditBtn::SetCurrUnitRectangular(int nEditType)
{
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_RECT) return;

	double dCorner1P[2], dCorner2P[2];

	if(! GetCoordFromEdit(m_pRectCorner1PEdit, dCorner1P[0], dCorner1P[1])) return;
	if(! GetCoordFromEdit(m_pRectCorner2PEdit, dCorner2P[0], dCorner2P[1])) return;

	SetCurrUnitRectangular(dCorner1P, dCorner1P, nEditType);

}

void CDrawEditBtn::SetCurrUnitRectangular(double dCorner1P[2], double dCorner2P[2], int nEditType)
{
	if(m_bCutoffInput) return; //내부동작중
	if(m_nMode != EN_DRAW_WINMODE_RECT) return;
	int nSize = m_CurrPolylineRD.arPoint.GetSize();
	T_POINT PointR; 

	//작업필요
	ASSERT(0);

}

//////////////////////////////////////////////////////////////////////////

void CDrawEditBtn::OverlapChecking()
{
	if(m_nOverlapType == EN_OVERLAP_NONE) return;
	else if(m_nOverlapType == EN_OVERLAP_OBJECT)
	{
		m_DrawCtrl.OverlapChecking_Object(m_nOverlapType, m_OverlapDrawGroup, m_OverlapDimFormat);
	}
	else ASSERT(0);
}

void CDrawEditBtn::ClearOverlapUnit()
{
	OverlapRemoveDrawData();
}

void CDrawEditBtn::KeyDown(UINT nChar)
{
	m_bCutoffInput = TRUE;//외부입력 차단
	switch (nChar)
	{
	case VK_SHIFT:
	case VK_LSHIFT:
	case VK_RSHIFT:
		m_nShiftButtonState = 1;
		break; 
	case VK_CONTROL:
		m_nCtrlButtonState = 1;
		break;
	case VK_ESCAPE:
		if(m_nMode == EN_DRAW_WINMODE_POLYLINE)
		{ 
			CPoint point;
			CompleteCurrUnitPolyline(point, FALSE);
		}
		break;			
	default:
		RunUpdateCompleteObjectData(EN_DRAW_UPDATE_KEY_DOWN_NONE, 0.0, 0.0);	
		break;	
	}
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}

void CDrawEditBtn::KeyUp(UINT nChar)
{
	m_bCutoffInput = TRUE;//외부입력 차단

	switch (nChar)
	{
	case VK_SHIFT:
	case VK_LSHIFT:
	case VK_RSHIFT:
		m_nShiftButtonState = 0;
		break; 
	case VK_CONTROL:
		m_nCtrlButtonState = 0;
		break;
	case VK_ESCAPE:
		if(m_nMode == EN_DRAW_WINMODE_SELECT)
		{			
			nrUINT arKey;

			if(m_DrawCtrl.GetSelectedKeyList(arKey) > 0)
			{
				RunUpdateCompleteObjectData(EN_DRAW_UPDATESELECT_DESELECT, 0.0, 0.0);
				m_DrawCtrl.RemoveAllSelectedKey();
				//ReleaseCapture();
				RedrawWindow();
			}
		}
		break;
	case VK_DELETE:
		if(m_nMode == EN_DRAW_WINMODE_SELECT)
		{
			nrUINT arKey;

			if(m_DrawCtrl.GetSelectedKeyList(arKey) > 0)
			{
				RunUpdateCompleteObjectData(EN_DRAW_UPDATESELECT_DELETE, 0.0, 0.0);
// 
// 				if(m_DrawCtrl.GetSelectedKeyList(arKey) > 0)
// 				{
// 					m_DrawCtrl.DelDrawUnit(arKey);
// 					m_DrawCtrl.RemoveAllSelectedKey();
// 					//ReleaseCapture();
// 					RedrawWindow();
// 				}				
			}			
		}
		break;		
	default:
		RunUpdateCompleteObjectData(EN_DRAW_UPDATE_KEY_UP_NONE, 0.0, 0.0);		
		break;
	}
	m_bCutoffInput = FALSE;//외부입력 차단 해제
}
//////////////////////////////////////////////////////////////////////////

void CDrawEditBtn::MovePan(CPoint CurrP)
{	
	m_HighlightDrawGroup.Init(); // #NS_RF : 하이라이트를 같이 움직이게 해주면 좋겠는데... 시간이 없어서 그만..

	m_DrawCtrl.m_PaperFormat.OrgPoint.x = m_StartOrgPoint.x + (CurrP.x - m_StartPoint.x);
	m_DrawCtrl.m_PaperFormat.OrgPoint.y = m_StartOrgPoint.y + (CurrP.y - m_StartPoint.y);

	m_DrawCtrl.ConvertDrawData(TRUE);	
	CurrConvertDrawData(TRUE);
	OverlapChecking();
	RedrawWindow();
}

void CDrawEditBtn::MoveZoom(CPoint CurrP)
{
	double vector0[2];
	vector0[0] = vector0[1] = -1.0;
	CMathFunc::mathNormalize2D(vector0, vector0);
	double vector1[2];
	vector1[0] = CurrP.x - m_StartPoint.x;
	vector1[1] = CurrP.y - m_StartPoint.y;
	CMathFunc::mathNormalize2D(vector1, vector1);

	double dAng = CMathFunc::mathCrossAngle2DSign(vector0[0], vector0[1], vector1[0], vector1[1]);
	double dLen = sqrt((double)((CurrP.x-m_StartPoint.x)*(CurrP.x-m_StartPoint.x) + (CurrP.y-m_StartPoint.y)*(CurrP.y-m_StartPoint.y)));

	double dTrrad = 4.0 * atan(1.0) / 180.0;
	double dRatio = dLen * cos(dAng * dTrrad) / 50.0;
	if(fabs(dRatio) < m_dZero) return;	
	dRatio = pow(2.0, dRatio);
	
	// 1안 : 원점을 기준으로 Zoom동작을 실행할 경우
	dRatio = dRatio * (m_DrawCtrl.m_PaperFormat.ScaleX+m_DrawCtrl.m_PaperFormat.ScaleY) / (m_StartOrgScale[0]+m_StartOrgScale[1]);
	ZoomRatio(m_DrawCtrl.m_PaperFormat.OrgPoint, dRatio);
	/*
	// 2안 : StartPoint를 기준으로 Zoom동작을 실행할 경우
	double dScaleX = m_StartOrgScale[0] / dRatio;
	double dScaleY = m_StartOrgScale[1] / dRatio;
	double dMinScale = 0.00000001;
	double dMaxScale = 10000000.0;
	if(dScaleX<dMinScale || dScaleX>dMaxScale) return;
	if(dScaleY<dMinScale || dScaleY>dMaxScale) return;

	int nX = m_StartOrgPoint.x - m_StartPoint.x;
	int nY = m_StartOrgPoint.y - m_StartPoint.y;

	m_DrawCtrl.m_PaperFormat.OrgPoint.x = m_StartPoint.x + (int)(nX*dRatio);
	m_DrawCtrl.m_PaperFormat.OrgPoint.y = m_StartPoint.y + (int)(nY*dRatio);
	m_DrawCtrl.m_PaperFormat.ScaleX = dScaleX;
	m_DrawCtrl.m_PaperFormat.ScaleY = dScaleY;

	m_DrawCtrl.ConvertDrawData(TRUE);	
	RedrawWindow();
	*/
}

void CDrawEditBtn::ChangeHighlightLine(CPoint currPoint)
{
	HighlightRemoveDrawData();

	if(m_nMode != EN_DRAW_WINMODE_NONE && m_nMode != EN_DRAW_WINMODE_VIEW)
	{
		if(m_bSelectMoveMode)
		{
			// Check!!! 추가작업요망(2010.05.26) Select Move시에는 선택 객체들의 Snap에 우선권을 줄것 
		}
		
		nrLine aHighlightLine;
		GetOpjectHighlightLine(currPoint, aHighlightLine);
		if(aHighlightLine.GetSize() != 0)
		{
			MakeHighLightGroup(aHighlightLine);
		}
	}
}

void CDrawEditBtn::ChangeSnapPoint(CPoint& CurrPoint, double CurrPointR[2])
{
	SnapRemoveDrawData();
	
	if(m_nMode != EN_DRAW_WINMODE_NONE && m_nMode != EN_DRAW_WINMODE_VIEW)
	{
		CPoint  snapPoint;
		T_POINT snapPointR;

		if(m_bSelectMoveMode)
		{
			// Check!!! 추가작업요망(2010.05.26) Select Move시에는 선택 객체들의 Snap에 우선권을 줄것 
		}

		if(GetOpjectSnapPoint(CurrPoint, snapPoint, snapPointR))
		{
			CurrPoint = snapPoint;
			CurrPointR[0] = snapPointR.x;
			CurrPointR[1] = snapPointR.y;
			MakeSnapGroup(CurrPoint);
		}
		else if(GetGridSnapPoint(CurrPoint, snapPoint, snapPointR))
		{
			CurrPoint = snapPoint;
			CurrPointR[0] = snapPointR.x;
			CurrPointR[1] = snapPointR.y;
			MakeSnapGroup(CurrPoint);
		}		
	}
}

void CDrawEditBtn::DisplayCoordText(CPoint CurrP)
{
	if(m_pCoordinateStatic != NULL)
	{
		double dCoord[2];
		GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
		ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

		CString strCoord = GetCoordText(dCoord[0], dCoord[1]);		
		m_pCoordinateStatic->SetWindowText(strCoord);
	}
}

void CDrawEditBtn::DisplayModeText()
{
	if(m_pModeStatic != NULL)
	{
		CString strMode = _T("");

		switch(m_nMode)
		{
		case EN_DRAW_WINMODE_NONE:			strMode = _T("");				  break;
		case EN_DRAW_WINMODE_VIEW:			strMode = _T("VIEW ");			  break;
		case EN_DRAW_WINMODE_SELECT:		strMode = _T("SELECT ");		  break;
		case EN_DRAW_WINMODE_POINT:			strMode = _T("POINT ");		  break;
		case EN_DRAW_WINMODE_LINE:			strMode = _T("LINE ");			  break;
		case EN_DRAW_WINMODE_ARROWLINE:		strMode = _T("ARROWLINE ");	  break;
		case EN_DRAW_WINMODE_CIRCLE:  		strMode = _T("CIRCLE ");		  break;
		case EN_DRAW_WINMODE_DONUT:  		strMode = _T("DONUT ");		  break;
		case EN_DRAW_WINMODE_ELLIPSE:		strMode = _T("ELLIPSE ");		  break;
		case EN_DRAW_WINMODE_RECT:			strMode = _T("RECTANGULAR ");	  break;
		case EN_DRAW_WINMODE_ARC:
		case EN_DRAW_WINMODE_ARC_B: 		strMode = _T("ARC ");			  break;
		case EN_DRAW_WINMODE_POLYLINE:		strMode = _T("POLYLINE ");		  break;
		case EN_DRAW_WINMODE_POLYGON:		strMode = _T("POLYGON ");		  break;
		case EN_DRAW_WINMODE_ARROWPOLYLINE: strMode = _T("ARROW POLYLINE "); break;
		case EN_DRAW_WINMODE_TEXT:			strMode = _T("TEXT ");			  break;
		case EN_DRAW_WINMODE_DIMENSION:		strMode = _T("DIMENSION ");	  break;
		case EN_DRAW_WINMODE_DIMANGLE:		strMode = _T("DIM.ANGLE ");	  break;
		case EN_DRAW_WINMODE_DIMLEADERLINE:	strMode = _T("DIM.LEADERLINE "); break;
		}
		m_pModeStatic->SetWindowText(strMode);
	}	
}

//////////////////////////////////////////////////////
void CDrawEditBtn::GetGenuineCoord(CPoint CurrP, double& dXR, double& dYR)
{
	dXR =        (CurrP.x - m_DrawCtrl.m_PaperFormat.OrgPoint.x) * m_DrawCtrl.m_PaperFormat.ScaleX;
	dYR = (-1.0)*(CurrP.y - m_DrawCtrl.m_PaperFormat.OrgPoint.y) * m_DrawCtrl.m_PaperFormat.ScaleY;
}

void CDrawEditBtn::GetDisplayCoord(double dXR, double dYR, CPoint& CurrP)
{
	if(m_DrawCtrl.m_PaperFormat.ScaleX == 0.0) 
	{
		CurrP.x = m_DrawCtrl.m_PaperFormat.OrgPoint.x;
	}
	else
	{
		CurrP.x = math_lib::round_off(dXR / m_DrawCtrl.m_PaperFormat.ScaleX + m_DrawCtrl.m_PaperFormat.OrgPoint.x);
	}
	if(m_DrawCtrl.m_PaperFormat.ScaleY == 0.0)
	{
		CurrP.y = m_DrawCtrl.m_PaperFormat.OrgPoint.y;
	}
	else
	{
		CurrP.y = math_lib::round_off((-1.0) * dYR / m_DrawCtrl.m_PaperFormat.ScaleY + m_DrawCtrl.m_PaperFormat.OrgPoint.y);
	}
}

CString CDrawEditBtn::GetCoordText(double dX, double dY)
{
	CString strCoord, strX, strY;		
	strX = GetValueText(dX);
	strY = GetValueText(dY);
	strCoord.Format(_T("%s, %s"), strX, strY);
	return strCoord;
}

CString CDrawEditBtn::GetValueText(double dValue)
{
	CString strValue, strFormat;		
	strFormat.Empty(); 
	
	if(fabs(dValue) >= 10000)
	{
		strValue.Format(_T("%d"), int(dValue));
	}
	else 
	{
		if(fabs(dValue) >= 1000)      strFormat += _T("%.1f");
		else if(fabs(dValue) >= 100)  strFormat += _T("%.2f");
		else if(fabs(dValue) >= 10)   strFormat += _T("%.3f");
		else if(fabs(dValue) >= 1)    strFormat += _T("%.4f");		
		else                           strFormat += _T("%.5f");
		
		strValue.Format(strFormat, dValue);
	}
	return strValue;
}

BOOL CDrawEditBtn::GetCoordFromString(CString& strCoord, double& dX, double& dY)
{
	CString strCoordX, strCoordY;
	int nDiv = strCoord.Find(',');
	
	if(nDiv <= 0) return FALSE;
	
	strCoordX = strCoord.Left(nDiv);
	strCoordY = strCoord.Right(strCoord.GetLength()-nDiv-1);
	
	if(strCoordY.GetLength() <= 0) return FALSE;
	if(strCoordY.Find(',') != -1) return FALSE;
	
	//strCoordX = strCoordX.TrimLeft(_T("G : "));
	strCoordX.TrimLeft(_T("G : "));
// 	dX = _ttof(strCoordX);
// 	dY = _ttof(strCoordY);
	dX = _tstof(strCoordX);
	dY = _tstof(strCoordY);

	return TRUE;
}

BOOL CDrawEditBtn::GetCoordFromEdit(CEdit* pEdit, double& dX, double& dY)
{
	if(pEdit == NULL) return FALSE;
	
	CString strCoord;
	pEdit->GetWindowText(strCoord);
	
	return GetCoordFromString(strCoord, dX, dY);
}

BOOL CDrawEditBtn::GetValueFromEdit(CEdit* pEdit, double& dValue)
{
	dValue = 0.0;
	if(pEdit == NULL) return FALSE;
	
	CString strValue;
	pEdit->GetWindowText(strValue);
	//dValue = _ttof(strValue);
	dValue = _tstof(strValue);
	
	return TRUE;
}

BOOL CDrawEditBtn::GetValueFromChkBox(CButton* pChk, BOOL& bValue)
{
	bValue = FALSE;
	if(pChk == NULL) return FALSE;
	
	bValue = pChk->GetCheck();
	
	return TRUE;
}

BOOL CDrawEditBtn::GetOpjectHighlightLine(CPoint currPoint, nrLine &aHighlightLine)
{
	return m_DrawCtrl.GetOpjectHighlightLine(currPoint, aHighlightLine);
}

BOOL CDrawEditBtn::GetOpjectSnapPoint(CPoint currPoint, CPoint& snapPoint, T_POINT& snapPointR)
{
	if(m_bObjectSnap)
	{
		return m_DrawCtrl.GetOpjectSnapPoint(currPoint, snapPoint, snapPointR);
	}

	return FALSE;
}

BOOL CDrawEditBtn::GetGridSnapPoint(CPoint currPoint, CPoint& snapPoint, T_POINT& snapPointR)
{
	if(m_bGridSnap && m_nGridMode != EN_DRAW_GRIDMODE_NONE)
	{
		if(m_DrawCtrl.m_PaperFormat.ScaleX == 0.0 || m_DrawCtrl.m_PaperFormat.ScaleY == 0.0) return FALSE;
		if(m_dGridInterval[0] == 0.0 || m_dGridInterval[1] == 0.0) return FALSE;

		double dCoord[2];
		GetGenuineCoord(currPoint, dCoord[0], dCoord[1]);

		int nGap_X = (int)(dCoord[0] / m_dGridInterval[0]);
		int nGap_Y = (int)(dCoord[1] / m_dGridInterval[1]);
		double dDis_X = dCoord[0] - nGap_X * m_dGridInterval[0];
		double dDis_Y = dCoord[1] - nGap_Y * m_dGridInterval[1];
	
		if(dDis_X >   m_dGridInterval[0] / 2.0) { nGap_X++; dDis_X -= m_dGridInterval[0]; }
		if(dDis_X <= -m_dGridInterval[0] / 2.0) { nGap_X--; dDis_X += m_dGridInterval[0]; }
		if(dDis_Y >   m_dGridInterval[1] / 2.0) { nGap_Y++; dDis_Y -= m_dGridInterval[1]; }
		if(dDis_Y <= -m_dGridInterval[1] / 2.0) { nGap_Y--; dDis_Y += m_dGridInterval[1]; }

		double dSnapDis_X = m_DrawCtrl.GetSnapDis()*m_DrawCtrl.m_PaperFormat.ScaleX;
		double dSnapDis_Y = m_DrawCtrl.GetSnapDis()*m_DrawCtrl.m_PaperFormat.ScaleY;

		if(fabs(dDis_X) < fabs(dSnapDis_X) && fabs(dDis_Y) < fabs(dSnapDis_Y))
		{
			snapPointR.x = nGap_X * m_dGridInterval[0];
			snapPointR.y = nGap_Y * m_dGridInterval[1];
			GetDisplayCoord(snapPointR.x, snapPointR.y, snapPoint);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDrawEditBtn::MakeEmfFile(CString strFileName, int nWidth, int nHeight)
{
	if(strFileName.IsEmpty()) return FALSE;
	if(nWidth == 0 || nHeight == 0) return FALSE;

	CDrawCtrl DrawCtrl_emf; // Emf 파일로 저장하기 위해 임시 Draw객체 저장

	// metafileDC 생성
	TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");  
	CRect rc(0, 0, nWidth, nHeight);

	CMetaFileDC metaDC;
	metaDC.CreateEnhanced(NULL, strFileName, rc, szDesc);
	metaDC.SetMapMode(MM_HIMETRIC);

	// DrawCtrl_emf에 m_DrawCtrl Copy
	DrawCtrl_emf.Copy(&m_DrawCtrl);

	// DrawCtrl_emf에 SetDC
	DrawCtrl_emf.SetHDC(metaDC.m_hDC);

	// DrawCtrl_emf의 m_DrawFunc에 SetIsEMF
	DrawCtrl_emf.m_DrawFunc.SetIsEMF(TRUE);

	// DrawCtrl_emf의 Canvas와 Size 비교해 Ratio 계산
	int nCanvasWidth = 0;
	int nCanvasHeight = 0;
	nCanvasWidth  = m_Canvas.Width();
	nCanvasHeight = m_Canvas.Height();
	
	if(nCanvasWidth == 0 || nCanvasHeight == 0) return FALSE;

	double dWidthRatio = 0.0;
	double dHeightRatio = 0.0;
	double dMaxRatio = 0.0;
	dWidthRatio  = (double)nWidth / (double)nCanvasWidth;
	dHeightRatio = (double)nHeight / (double)nCanvasHeight;
	ASSERT(dWidthRatio != 0.0);
	ASSERT(dHeightRatio != 0.0);
	dMaxRatio = max(dWidthRatio, dHeightRatio);

	// DrawCtrl_emf의 format(maps)에 Ratio 곱해주기
	T_DRAW_FORMAT DrawFormat;
	POSITION pos = DrawCtrl_emf.m_arDrawFormat.GetStartPosition();
	UINT index;
	while(pos)
	{
		DrawCtrl_emf.m_arDrawFormat.GetNextAssoc(pos, index, DrawFormat);
		DrawFormat.Width         = (int)(DrawFormat.Width         * dMaxRatio);
		DrawFormat.HatchingWidth = (int)(DrawFormat.HatchingWidth * dMaxRatio);
		DrawFormat.HatchingSpace = (int)(DrawFormat.HatchingSpace * dMaxRatio);
		DrawCtrl_emf.m_arDrawFormat[index] = DrawFormat;
	}   

	T_TEXT_FORMAT TextFormat;
	pos = DrawCtrl_emf.m_arTextFormat.GetStartPosition();
	while(pos)
	{
		DrawCtrl_emf.m_arTextFormat.GetNextAssoc(pos, index, TextFormat);
		TextFormat.Size          =(int)(TextFormat.Size * dMaxRatio);
		DrawCtrl_emf.m_arTextFormat[index] = TextFormat;
	}   
	DrawCtrl_emf.SetTextUnit_CurrFormatSizeRatio(dMaxRatio); // 모든 Text DrawUnit의 CurrFormat의 Size에 dRatio값을 곱하여 줍니다

	T_DIM_FORMAT DimFormat;
	pos = DrawCtrl_emf.m_arDimFormat.GetStartPosition();
	while(pos)
	{
		DrawCtrl_emf.m_arDimFormat.GetNextAssoc(pos, index, DimFormat);
		DimFormat.Width    = (int)(DimFormat.Width    * dMaxRatio);
		DimFormat.fontSize = (int)(DimFormat.fontSize * dMaxRatio);
		DrawCtrl_emf.m_arDimFormat[index] = DimFormat;
	}   

	T_CHART_FORMAT ChartFormat;
	pos = DrawCtrl_emf.m_arChartFormat.GetStartPosition();
	while(pos)
	{
		DrawCtrl_emf.m_arChartFormat.GetNextAssoc(pos, index, ChartFormat);
		ChartFormat.frame.Width           = (int)(ChartFormat.frame.Width           * dMaxRatio);
		ChartFormat.frame.fontSize        = (int)(ChartFormat.frame.fontSize        * dMaxRatio);
		ChartFormat.remark.Width          = (int)(ChartFormat.remark.Width          * dMaxRatio);
		ChartFormat.remark.fontSize       = (int)(ChartFormat.remark.fontSize       * dMaxRatio);
		ChartFormat.xAxis.Width           = (int)(ChartFormat.xAxis.Width           * dMaxRatio);
		ChartFormat.xAxis.nMajorGridWidth = (int)(ChartFormat.xAxis.nMajorGridWidth * dMaxRatio);
		ChartFormat.xAxis.nMinorGridWidth = (int)(ChartFormat.xAxis.nMinorGridWidth * dMaxRatio);
		ChartFormat.xAxis.fontSize        = (int)(ChartFormat.xAxis.fontSize        * dMaxRatio);
		ChartFormat.xAxis.fontSize_Title  = (int)(ChartFormat.xAxis.fontSize_Title  * dMaxRatio);
		ChartFormat.yAxis.Width           = (int)(ChartFormat.yAxis.Width           * dMaxRatio);
		ChartFormat.yAxis.nMajorGridWidth = (int)(ChartFormat.yAxis.nMajorGridWidth * dMaxRatio);
		ChartFormat.yAxis.nMinorGridWidth = (int)(ChartFormat.yAxis.nMinorGridWidth * dMaxRatio);
		ChartFormat.yAxis.fontSize        = (int)(ChartFormat.yAxis.fontSize        * dMaxRatio);
		ChartFormat.yAxis.fontSize_Title  = (int)(ChartFormat.yAxis.fontSize_Title  * dMaxRatio);
		DrawCtrl_emf.m_arChartFormat[index] = ChartFormat;
	}   

	// DrawCtrl_emf.AutoPaperFormat
	DrawCtrl_emf.AutoPaperFormat(rc, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);

	// DrawCtrl_emf.ConvertDrawData(TRUE);
	DrawCtrl_emf.ConvertDrawData(TRUE);

	// Draw
	DrawCtrl_emf.Draw();
	DrawFormat = m_AxisDrawFormat;	
	DrawFormat.Width = (int)(DrawFormat.Width*dMaxRatio);
	DrawAxisUnit(&DrawCtrl_emf, m_nAxisMode, (int)(m_nAxisSize*dMaxRatio), (int)(m_nAxisWidth*dMaxRatio), m_bCapital, DrawFormat, FALSE, m_AxisOrgPoint, m_AxisOrgPointR, m_dAxisVector1, m_dAxisVector2);  
	
	// metafile 생성 닫기
	HENHMETAFILE hMetaF = metaDC.CloseEnhanced();
	::DeleteEnhMetaFile(hMetaF);

	return TRUE;
}

void CDrawEditBtn::SetCursorType()
{
	if(m_nMButtonState == 1)
	{
		//::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_VIEW_MOVE));
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
	}
	else
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	/*
	UINT ID = 0;


	switch(abs(m_nDynamic))
	{
	case EN_STEP_DYNAMIC_WINDOW:ID = IDC_CURSOR_VIEW_ZOOMW; break;
	case EN_STEP_DYNAMIC_ZOOM:  ID = IDC_CURSOR_VIEW_ZOOMD; break;
	case EN_STEP_DYNAMIC_MOVE:  ID = IDC_CURSOR_VIEW_MOVE;	break;
	}

	if(ID)
	{
		::SetCursor(AfxGetApp()->LoadCursor(ID));
	}
	else
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}*/
}

BOOL CDrawEditBtn::MakeBMP(CString FileName)
{
	int length = FileName.GetLength();
	if(length < 1) return FALSE;

	AfxGetApp()->DoWaitCursor(1);

	UINT  nf = CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive;
	CFile ff;
	CFileException fe;

	if(! ff.Open(FileName, nf, &fe)) 
	{
		AfxMessageBox(_T("File open error"));
		AfxGetApp()->DoWaitCursor(0);
		return FALSE;
	}
	else
	{
		ReDraw();
		MakeDibFile(m_Bitmap, GetDC(), ff);
		ff.Close();
	}

	AfxGetApp()->DoWaitCursor(0);

	return TRUE;
}

void CDrawEditBtn::ReDraw()
{
	CClientDC	dc(this);
	DrawPaint(&dc);
}

BOOL CDrawEditBtn::MakeDibFile(CBitmap& gdiBmp, CDC* pDC, CFile& file)
{
	// 1. set Dib-Bitmap data
	BITMAP	bmp;
	DWORD	dwCount;
	BYTE*	pBmBits;

	gdiBmp.GetBitmap(&bmp);
	if (bmp.bmBitsPixel != 1  && bmp.bmBitsPixel != 4  && bmp.bmBitsPixel != 8 &&
		bmp.bmBitsPixel != 16 && bmp.bmBitsPixel != 24 && bmp.bmBitsPixel != 32) return FALSE;

	dwCount= bmp.bmWidthBytes*bmp.bmHeight;
	pBmBits= new BYTE[dwCount];
	gdiBmp.GetBitmapBits(dwCount, LPVOID(pBmBits));

	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	fileHeader.bfType = 0x4D42;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwCount;
	fileHeader.bfReserved1	= 0;
	fileHeader.bfReserved2	= 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	infoHeader.biSize	= sizeof(BITMAPINFOHEADER);
	infoHeader.biWidth	= bmp.bmWidth;
	infoHeader.biHeight = bmp.bmHeight;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = bmp.bmBitsPixel;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage	= dwCount;
	infoHeader.biXPelsPerMeter	= 0;
	infoHeader.biYPelsPerMeter	= 0;
	infoHeader.biClrUsed		= 0;
	infoHeader.biClrImportant	= 0;

	// 2. set palette
	int		nRgb = 0;
	RGBQUAD	*pRgb = NULL;
	DWORD	*pClr;

	if (bmp.bmBitsPixel < 16)	// palette color
	{
		int	nSysColors = ::GetDeviceCaps(pDC->m_hDC, NUMCOLORS);
		int	nPalEntries = ::GetDeviceCaps(pDC->m_hDC, SIZEPALETTE);

		nRgb = (nPalEntries == 0 ? nSysColors : nPalEntries);
		if (nRgb < 0) nRgb = 0;
		if (nRgb > 0)
		{
			pRgb = new RGBQUAD[nRgb];
			nRgb = ::GetSystemPaletteEntries(pDC->m_hDC, 0, nRgb, (PALETTEENTRY*)pRgb);
			for (int i = 0; i < nRgb; i++)
			{
				BYTE red = pRgb[i].rgbBlue;
				pRgb[i].rgbBlue= pRgb[i].rgbRed;
				pRgb[i].rgbRed = red;
			}
		}
	}
	else if (bmp.bmBitsPixel == 16)	// high color => need to modify
	{
		BOOL is16Bit = TRUE;	// 16-bit or 15-bit
		nRgb = 3;
		pRgb = new RGBQUAD[nRgb];
		pClr = (DWORD*)pRgb;
		pClr[0] = (is16Bit ? 0xF800 : 0x7C00);	// RED
		pClr[1] = (is16Bit ? 0x07E0 : 0x03E0);	// GREEN
		pClr[2] = (is16Bit ? 0x001F : 0x001F);	// BLUE
		infoHeader.biCompression = BI_BITFIELDS;
	}
	else if (bmp.bmBitsPixel == 32)	// full color
	{
		nRgb = 3;
		pRgb = new RGBQUAD[nRgb];
		pClr = (DWORD*)pRgb;
		pClr[0] = 0x00FF0000;	// RED
		pClr[1] = 0x0000FF00;	// GREEN
		pClr[2] = 0x000000FF;	// BLUE
		infoHeader.biCompression = BI_BITFIELDS;
	}

	if (nRgb>0)
	{
		fileHeader.bfSize += sizeof(RGBQUAD) * nRgb;
		fileHeader.bfOffBits += sizeof(RGBQUAD) * nRgb;
	}

	// 3. write to file
	BYTE	extra[4];
	int		i, nExtra;

	nExtra = bmp.bmWidthBytes % 4;
	memset(extra, 0, sizeof(BYTE) * 4);

	file.SeekToBegin();
	file.Write(&fileHeader, sizeof(BITMAPFILEHEADER));
	file.Write(&infoHeader, sizeof(BITMAPINFOHEADER));

	for (i = 0; i < nRgb; i++)
	{
		file.Write(&pRgb[i], sizeof(RGBQUAD));
	}

	for(i = 0; i < bmp.bmHeight; i++)
	{
		int	pos = (bmp.bmHeight - i - 1) * bmp.bmWidthBytes;
		file.Write(&pBmBits[pos], bmp.bmWidthBytes);
		if (nExtra > 0) file.Write(extra, nExtra * sizeof(BYTE));
	}

	if (pRgb) delete[] pRgb;
	if (pBmBits) delete[] pBmBits;
	return TRUE;
}