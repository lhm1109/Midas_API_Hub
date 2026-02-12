#pragma once

#include "DrawCtrl.h"

enum
{
	EN_DRAW_WINMODE_NONE, 
	EN_DRAW_WINMODE_VIEW,
	EN_DRAW_WINMODE_SELECT, 
	EN_DRAW_WINMODE_POINT,
	EN_DRAW_WINMODE_LINE,
	EN_DRAW_WINMODE_ARROWLINE, 
	EN_DRAW_WINMODE_CIRCLE, 
	EN_DRAW_WINMODE_DONUT, 
	EN_DRAW_WINMODE_ELLIPSE,
	EN_DRAW_WINMODE_RECT,
	EN_DRAW_WINMODE_ARC, 
	EN_DRAW_WINMODE_ARC_B,
	EN_DRAW_WINMODE_POLYLINE, 
	EN_DRAW_WINMODE_POLYGON, 
	EN_DRAW_WINMODE_ARROWPOLYLINE,
	EN_DRAW_WINMODE_TEXT,
	EN_DRAW_WINMODE_DIMENSION, 
	EN_DRAW_WINMODE_DIMANGLE,
	EN_DRAW_WINMODE_DIMLEADERLINE
};// 현재(08.03.09) VIEW, POINT, LINE, ARC, CIRCLE만 지원 

// 입력방식
// DGNDRAW_WINMODE_ARC   : 시작점->중간점->끝점
// DGNDRAW_WINMODE_ARC_B : 시작점->끝점->중간점(Bulge)

enum 
{
	EN_DRAW_AXISMODE_NONE, 
	EN_DRAW_AXISMODE_PANTALINE_XY,  
	EN_DRAW_AXISMODE_PANTALINE_YZ,  
	EN_DRAW_AXISMODE_PANTALINE_XZ,     // 원점  좌표축 기준 
	EN_DRAW_AXISMODE_PANTALINE_XY_L,
	EN_DRAW_AXISMODE_PANTALINE_YZ_L, 
	EN_DRAW_AXISMODE_PANTALINE_XZ_L
};  // Local 좌표축 기준

enum 
{
	EN_DRAW_GRIDMODE_NONE,
	EN_DRAW_GRIDMODE_DOT
};

enum 
{
	/*Select관련  */
	EN_DRAW_UPDATESELECT_NONE,
	EN_DRAW_UPDATESELECT_SELECT,
	EN_DRAW_UPDATESELECT_DESELECT,
	EN_DRAW_UPDATESELECT_DELETE, 
	EN_DRAW_UPDATESELECT_MOVE, 
	EN_DRAW_UPDATESELECT_COPY,	
	/*Complete관련*/
	EN_DRAW_UPDATECOMPLETE_POINT,
	EN_DRAW_UPDATECOMPLETE_LINE, 
	EN_DRAW_UPDATECOMPLETE_ARC,
	EN_DRAW_UPDATECOMPLETE_ARC_B, 
	EN_DRAW_UPDATECOMPLETE_CIRCLE,
	EN_DRAW_UPDATECOMPLETE_POLYLINE, // 추가 항목 지원시 추가

	EN_DRAW_UPDATE_LBTN_DBLCLK,
	/*미실행 항목 */
	EN_DRAW_UPDATE_LBTN_DOWN_NONE,
	EN_DRAW_UPDATE_LBTN_UP_NONE,	
	EN_DRAW_UPDATE_MBTN_DOWN_NONE,
	EN_DRAW_UPDATE_MBTN_UP_NONE, 
	EN_DRAW_UPDATE_MBTN_DBLCLK_NONE,
	EN_DRAW_UPDATE_RBTN_DOWN_NONE,
	EN_DRAW_UPDATE_RBTN_UP_NONE,
	EN_DRAW_UPDATE_RBTN_DBLCLK_NONE, 
	EN_DRAW_UPDATE_MOUSE_MOVE_NONE,
	EN_DRAW_UPDATE_KEY_DOWN_NONE,
	EN_DRAW_UPDATE_KEY_UP_NONE
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDrawEditBtn

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDrawEditBtn : public CButton 
{
	// Construction
public:
	CDrawEditBtn();   // standard constructor
	virtual ~CDrawEditBtn();

	virtual void DrawRender(); // 그림을 그리는 함수(이 함수에 그림을 그려~)

protected:
	CDrawCtrl m_DrawCtrl;     // Draw객체들을 보관 및 관리  


	CBitmap m_Bitmap;

	CRect    m_Canvas;        //작업공간
	COLORREF m_BkGroudColor;  //배경색

	BOOL     m_bCutoffInput;  //외부 입력 차단 여부(내부동작중 외부의 입력을 반영하지 않아야 하는 경우 TRUE)
	double   m_dZero;         //오차범위
	double   m_dFitMargin;    //Fit시 여백부분의 공간(0.0~0.5)
	double   m_dZoomRatio;    //Zoom시의 비율(Default = 2^(1/3) : 3번만에 2배가 되도록)

	BOOL     m_bObjectSnap;
	BOOL     m_bGridSnap;
	BOOL     m_bUseZoom;      //Zoom 기능을 사용할지 여부
	BOOL     m_bUseMove;      //Move기능을 사용할지 여부 (m_nMode == DGNDRAW_WINMODE_SELECT일때에만 사용)
	BOOL     m_bUseDel;       //Delete기능을 사용할지 여부 (m_nMode == DGNDRAW_WINMODE_SELECT일때에만 사용)
	BOOL     m_bUseCopy;      //Copy기능을 사용할지 여부 (m_nMode == DGNDRAW_WINMODE_SELECT일때에만 사용)
	BOOL     m_bContextBitmap;
	BOOL     m_bContextEmf;
	int      m_nOverlapType;  //Overlap 검토형식
	int      m_nMode;         //현재의 입력방식(DGNDRAW_WINMODE_XXX)

	UINT     m_DefaultDrawFormatKey; // Draw객체 생성시 Default로 적용될 FormatKey
	UINT     m_DefaultTextFormatKey; //
	UINT     m_DefaultDimFormatKey;  //
	
	nrUINT   m_arSelectKey;//현재(08.03.11) 미사용

	// Keyboard 현재상태
	int        m_nShiftButtonState; // 0:UP  1:DOWN
	int        m_nCtrlButtonState;  // 0:UP  1:DOWN
	// Mouse 현재상태
	int        m_nButtonCount;
	int        m_nLButtonState; // 0:UP  1:DOWN
	int        m_nMButtonState; // 0:UP  1:DOWN
	int        m_nRButtonState; // 0:UP  1:DOWN
	CPoint     m_StartOrgPoint; // Mouse 작업시작시의 좌표 원점 
	double     m_StartOrgScale[2];// Mouse 작업시작시의 Scale

	CPoint     m_StartPoint;    // 화면상의 작업의 시작점 
	CPoint     m_InterPoint;    // 화면상의 작업의 중간점
	double     m_StartPointR[2];// 실제 작업의 시작점
	double     m_InterPointR[2];// 실제 작업의 중간점 
	T_POLYLINE_R m_CurrPolylineRD; // Polyline 그리는 중간 Data
	// Select 상태
	BOOL       m_bReSelectDrawUnit;

	// 외부 입출력 Control
	CBCGPStatic*   m_pModeStatic;       // 현재 입력상태를 표시 Static Control
	CBCGPStatic*   m_pCoordinateStatic; // 현재 좌표점의 위치를 표시할 Static Control
	// Point
	CEdit*     m_pPointEdit;
	// Line
	CEdit*     m_pLineStartPEdit;
	CEdit*     m_pLineEndPEdit;
	// Arc
	CEdit*     m_pArcStartPEdit;
	CEdit*     m_pArcPassPEdit; // DGNDRAW_WINMODE_ARC일때에는 작업의 중간점 DGNDRAW_WINMODE_ARC_B일때에는 작업의 끝점이 된다.
	CEdit*     m_pArcEndPEdit;  // DGNDRAW_WINMODE_ARC일때에는 작업의 끝점   DGNDRAW_WINMODE_ARC_B일때에는 작업의 중간점이 된다.
	CEdit*     m_pArcRadiusEdit;
	CEdit*     m_pArcBulgeEdit;
	CButton*   m_pArcInsideCheck;
	// Circle
	CEdit*     m_pCircleStartPEdit;
	CEdit*     m_pCircleEndPEdit;
	CEdit*     m_pCircleRadiusEdit;
	//Polyline
	CEdit*     m_pPolylineStartPEdit;
	CEdit*     m_pPolylinePassPEdit; // 입력중인 마지막 좌표 또는 입력완료후 마지막좌표 이전 좌표
	CEdit*     m_pPolylineEndPEdit;
	//Rectangular
	CEdit*     m_pRectCorner1PEdit;
	CEdit*     m_pRectCorner2PEdit;

	// 모드(DGNDRAW_WINMODE_XXX)추가시 입력 Control추가할 것...

	// Axis
	int    m_nAxisMode;
	int    m_nAxisSize;
	int    m_nAxisWidth;
	BOOL   m_bCapital;// 대문자표시여부
	T_DRAW_FORMAT m_AxisDrawFormat;

	BOOL    m_bCDC_AxisOrgPoint; // Local좌표축을 사용할 경우 입력받은 좌표가 CDC좌표인지 실제 좌표인지 여부(TRUE:CDC, FALSE:실좌표)
	CPoint  m_AxisOrgPoint;  // Local좌표축을 사용할 경우 좌표원점에서 떨어진 거리(화면상의 좌표원점은 0,0이 아니므로 좌표원점으로부터 떨어진 상대거리를 사용함)
	T_POINT m_AxisOrgPointR; // Local좌표축을 사용할 경우 좌표원점에서 떨어진 거리(실제 좌표는 좌표원점이 0,0이므로 변경할원점의 좌표와 같음)
	double  m_dAxisVector1[2];      // Local좌표축을 사용할 경우 첫번째 좌표축의 방향(실좌표 방향:상+, 우+)
	double  m_dAxisVector2[2];      // Local좌표축을 사용할 경우 두번째 좌표축의 방향(실좌표 방향:상+, 우+)

	// Grid
	int    m_nGridMode;
	double m_dGridInterval[2]; // 0:x, 1:y
	T_DRAW_FORMAT      m_GridDrawFormat;
	// Snap
	T_DRAW_FORMAT      m_SnapDrawFormat;
	T_RECT_GROUP       m_SnapDrawGroup;

	// Hightlight
	T_DRAW_FORMAT      m_HighlightDrawFormat;
	T_LINE_GROUP       m_HighlightDrawGroup;

	// Select
	BOOL               m_bSelectMoveMode;
	T_DRAW_FORMAT      m_SelectAreaDrawFormat;
	T_RECT_GROUP       m_SelectAreaDrawGroup;
	
	// Curr 
	T_DRAW_FORMAT      m_CurrDrawFormat_Point_SelectMove;
	T_DRAW_FORMAT      m_CurrDrawFormat_SelectMove;//SelectMove시의 CurrUnit의 Point를 재외한 Drawing개열 객체Format
	T_TEXT_FORMAT      m_CurrTextFormat_SelectMove;//SelectMove시의 CurrUnit의 Text개열 객체Format
	T_DIM_FORMAT	   m_CurrDimFormat_SelectMove ;//SelectMove시의 CurrUnit의 Dimension개열 객체Format

	// 현재 작업중인 객체들의 형식 및 정보	
	T_DRAW_FORMAT      m_CurrDrawFormat_Point;//CurrUnit의 Point객체의 경우 선의 두께로 크기가 결정되므로 
	T_DRAW_FORMAT      m_CurrDrawFormat;      //CurrUnit의 Point를 재외한 Drawing개열 객체Format
	T_TEXT_FORMAT      m_CurrTextFormat;      //CurrUnit의 Text개열 객체Format
	T_DIM_FORMAT       m_CurrDimFormat ;      //CurrUnit의 Dimension개열 객체Format
	CDrawPoint         m_CurrDrawClass_Point;
	CDrawLine          m_CurrDrawClass_Line;
	CDrawArrowLine     m_CurrDrawClass_ArrowLine;
	CDrawCircle        m_CurrDrawClass_Circle;
	CDrawDonut         m_CurrDrawClass_Donut;
	CDrawEllipse       m_CurrDrawClass_Ellipse;
	CDrawRect          m_CurrDrawClass_Rect;
	CDrawArc           m_CurrDrawClass_Arc;
	CDrawPolyline      m_CurrDrawClass_Polyline;
	CDrawPolygon       m_CurrDrawClass_Polygon;
	CDrawArrowPolyline m_CurrDrawClass_ArrowPolyline;
	CDrawText          m_CurrDrawClass_Text;
	CDrawDimension     m_CurrDrawClass_Dimension;
	CDrawDimAngle      m_CurrDrawClass_DimAngle;
	CDrawDimLeaderline m_CurrDrawClass_DimLeaderline;
	// Overlap
	T_DIM_FORMAT		  m_OverlapDimFormat;
	T_DIMLEADERLINE_GROUP m_OverlapDrawGroup;

	// Member Function
public:
	BOOL Init(); 
	void SetFit();
	void SetMode(int nMode);
	void SetUseMove(BOOL bSet) { m_bUseMove = bSet; }
	int  GetMode() { return m_nMode; };	
	void SetSnap(BOOL bObjectSnap, BOOL bGridSnap, int nSnapDis = 2);// Opject에 Snap적용여부, Grid에 Snap적용여부
	void SetSelectDrawType(int nType);  // 0:표현안함, 1:외곽사각형표시, 2:Format변경, 3:색만변경, 4:색+선두깨변경
	void SetAxisMode(int nAxisMode, int nSize, int nWidth, BOOL bCapital = TRUE); //nAxisMode:표시형식, nSize:크기, nWidth:폭, bCapital:대문자 표시여부
	void SetAxisLocal(double dOrgPoint[2], double dAxisVector1[2], double dAxisVector2[2]);// Local축 사용시 원점의 실좌표 및 각 축의 방향을 입력함(DGNDRAW_AXISMODE_PANTALINE_XY_L, DGNDRAW_AXISMODE_PANTALINE_YZ_L, DGNDRAW_AXISMODE_PANTALINE_XZ_L 사용시에만 적용)
	void SetGridMode(int nGridMode, double dInterval_x, double dInterval_y);
	void SetMirrorMode(int nMirrorMode);
	void SetContext(BOOL bContextBitmap, BOOL bContextEmf);
	//void SetWndConnector(CDgnDrawBase_DrawView_Connector* pConnector);
	void SetWndCoordText(CBCGPStatic* pText);
	void SetWndModeText(CBCGPStatic* pText);
	void SetWndPointEdit(CEdit* pEdit);
	void SetWndLineEdit(CEdit* pStartEdit, CEdit* pEndEdit);	
	void SetWndArcEdit(CEdit* pStartEdit, CEdit* pPassEdit, CEdit* pEndEdit, CEdit* pBulgeEdit = NULL, CEdit* pRadiusEdit = NULL, CButton* pInsideCheck = NULL);	
	void SetWndCircleEdit(CEdit* pStartEdit, CEdit* pEndEdit, CEdit* pRadiusEdit);	
	void SetWndPolylineEdit(CEdit* pStartEdit, CEdit* pPassEdit, CEdit* pEndEdit);
	void SetWndRectEdit(CEdit* pStartEdit, CEdit* pPassEdit, CEdit* pEndEdit);

	// 모드(DGNDRAW_WINMODE_XXX)추가시 Control포인트 설정함수 추가할 것...
	void ZoomRatio(CPoint CurrP, double dZoomRatio);
	void ZoomOut(CPoint CurrP);
	void ZoomIn(CPoint CurrP);
	void ZoomOut();
	void ZoomIn();

	// SelectKey
	void SetSelectDistance(UINT nDist) { m_DrawCtrl.SetSelectDis(nDist); }
	int  GetSelectedKeyList(nrUINT& arKey);
	int  GetSelectedLinkKeyList(nrUINT& arLinkKey);
	void SetSelectedKeyList(nrUINT& arKey, BOOL bReDraw);
	void SetSelectedLinkKeyList(nrUINT& arLinkKey, BOOL bReDraw);
	void RemoveAllSelectedKey();
	void AddSelectedKey(UINT key);
	void AddSelectedKey(nrUINT& arKey);
	void DelSelectedKey(UINT key);
	void DelSelectedKey(nrUINT& arKey);
	BOOL IsSelected(UINT key);

	void ClearCurrUnit();

	// CurrUnit를 직접 입력	
	void SetCurrUnitPoint(double dStartP[2]);
	void SetCurrUnitLine(int nCount = 1);
	void SetCurrUnitLine(double dStartP[2], double dEndP[2], int nCount = 1);
	void SetCurrUnitLineA(double dStartP[2], double dEndP[2], int nCount = 1);
	void SetCurrUnitLineGuide(double dStartP[2], double dEndP[2], int no = 0, BOOL bRemove = TRUE);	
	void SetCurrUnitArc(int nCount = 2, int nType = 1);// nType(1:3점으로 계산 2:Bulge값으로 계산 3:반지름값으로 계산)
	void SetCurrUnitArc(double dStartP[2], double dPassP[2], double dEndP[2], int nCount = 2);
	void SetCurrUnitArcBugle(double dStartP[2], double dEndP[2], double dBugle, int nCount = 2);
	void SetCurrUnitArcRadius(double dStartP[2], double dEndP[2], double dRadius, BOOL bInside, int nCount = 2);
	void SetCurrUnitCircle(int nCount = 1, int nType = 1);// nType(1:반지름입력값으로 계산 2:마지막좌표값으로 계산)
	void SetCurrUnitCircle(double dStartP[2], double dEndP[2], int nCount = 1);
	void SetCurrUnitPolyline(int nEditType = 1);
	void SetCurrUnitPolyline(double dStartP[2], double dPassP[2], double dEndP[2], int nEditType = 2);	
	void SetCurrUnitRectangular(int nEditType = 1);
	void SetCurrUnitRectangular(double dStartP[2], double dEndP[2], int nEditType = 2);	
	//void Set_CurrUnit_Circle_Radius(double dStartP[2], double dRadius, int nCount=1);
	// 모드(DGNDRAW_WINMODE_XXX)추가시 직접입력 함수추가할 것...

	// Overlap Checking
	void OverlapChecking();
	void ClearOverlapUnit();

	void KeyDown(UINT nChar);// KeyDown 이벤트 발생시 반영
	void KeyUp(UINT nChar);  // KeyUp   이벤트 발생시 반영

	virtual BOOL MakeEmfFile(CString strFileName, int nWidth, int nHeight);
	void SetCursorType();

	BOOL MakeBMP(CString FileName);    

	afx_msg void OnMakeBMP();
	afx_msg void OnMakeEMF();

protected:
	void DrawPaint(CDC* pDC); // 그림을 그리는 함수(이 함수에 그림을 그려~)

	virtual void SetEntityFormat();

	virtual void AxisDraw();	
	virtual void GridDraw();	
	virtual void SnapDraw();
	virtual void HighlightDraw();	
	virtual void CurrDraw();
	virtual void SelectDraw();
	virtual void OverlapDraw();

	virtual void SnapRemoveDrawData();// Snap 정보 제거
	virtual void HighlightRemoveDrawData();// Highlight 정보 제거
	virtual void CurrRemoveDrawData();// 가상 입력 Draw 정보 제거
	virtual void SelectRemoveDrawData();//Select Draw 정보 제거
	virtual void OverlapRemoveDrawData();//Overlap Draw 정보 제거

	virtual void AxisOrgPointConvertDrawData(BOOL bCDC);// 축원점 정보를 좌표변환
	virtual void AxisOrgPointConvertDrawData(BOOL bCDC, CDrawCtrl* pDrawCtrl, CPoint& AxisOrgPoint, T_POINT& AxisOrgPointR);
	virtual void CurrConvertDrawData(BOOL bCDC);   // 가상 입력 Draw 정보를 좌표변환

	// 축	그림을 pDrawCtrl에 그림
	virtual void DrawAxisUnit(CDrawCtrl* pDrawCtrl, int nAxisMode, int nAxisSize, int nAxisWidth, BOOL bCapital, T_DRAW_FORMAT AxisDrawFormat,
		BOOL bCDC_AxisOrgPoint, CPoint AxisOrgPoint, T_POINT AxisOrgPointR, double dAxis_Vector1[2], double dAxis_Vector2[2]);

	// 좌표원점을 이동함	
	virtual void MovePan(CPoint CurrP);  // 화면 이동
	virtual void MoveZoom(CPoint CurrP); // 마우스를 이동하면 화면 줌조절

	// 작업전(Mouse Move)
	virtual void BeforeCurrUnitSelect(CPoint CurrP);
	virtual void BeforeCurrUnitPoint(CPoint CurrP);
	virtual void BeforeCurrUnitLine(CPoint CurrP);
	virtual void BeforeCurrUnitArc(CPoint CurrP);
	virtual void BeforeCurrUnitArcB(CPoint CurrP);
	virtual void BeforeCurrUnitCircle(CPoint CurrP);	
	virtual void BeforeCurrUnitPolyline(CPoint CurrP);
	virtual void BeforeCurrUnitRect(CPoint CurrP);
	// 모드(DGNDRAW_WINMODE_XXX)추가시 함수추가할 것...

	// 작업시작(시작좌표 입력 : 마우스 클릭)
	virtual void StartCurrUnitSelect(CPoint CurrP); // LButtonDown
	virtual void StartCurrUnitPoint(CPoint CurrP);  // 작업없음
	virtual void StartCurrUnitLine(CPoint CurrP);   // LButtonDown
	virtual void StartCurrUnitArc(CPoint CurrP);    // LButtonDown
	virtual void StartCurrUnitArcB(CPoint CurrP);  // LButtonDown
	virtual void StartCurrUnitCircle(CPoint CurrP); // LButtonDown
	virtual void StartCurrUnitPolyline(CPoint CurrP); // LButtonDown
	virtual void StartCurrUnitRect(CPoint CurrP); // LButtonDown
	// 모드(DGNDRAW_WINMODE_XXX)추가시 함수추가할 것...

	// 작업중 작업(중간좌표 입력 : 마우스 클릭)
	virtual void PassCurrUnitSelect(CPoint CurrP); // 작업없음
	virtual void PassCurrUnitPoint(CPoint CurrP);  // 작업없음
	virtual void PassCurrUnitLine(CPoint CurrP);   // 작업없음
	virtual void PassCurrUnitArc(CPoint CurrP);    // LButtonDown
	virtual void PassCurrUnitArcB(CPoint CurrP);  // LButtonDown
	virtual void PassCurrUnitCircle(CPoint CurrP); // 작업없음	
	virtual void PassCurrUnitPolyline(CPoint CurrP);  // LButtonDown
	virtual void PassCurrUnitRect(CPoint CurrP); // 작업없음	
	// 모드(DGNDRAW_WINMODE_XXX)추가시 함수추가할 것...

	// 작업중(Mouse Move)	
	virtual void InProgressCurrUnitSelect(CPoint CurrP);
	virtual void InProgressCurrUnitPoint(CPoint CurrP);  // 작업없음
	virtual void InProgressCurrUnitLine(CPoint CurrP);
	virtual void InProgressCurrUnitArc(CPoint CurrP);
	virtual void InProgressCurrUnitArcB(CPoint CurrP);
	virtual void InProgressCurrUnitCircle(CPoint CurrP);
	virtual void InProgressCurrUnitPolyline(CPoint CurrP);
	virtual void InProgressCurrUnitRect(CPoint CurrP);
	// 모드(DGNDRAW_WINMODE_XXX)추가시 함수추가할 것...

	// 작업완료(최종좌표 입력 : 마우스 클릭)
	virtual void CompleteCurrUnitSelect(CPoint CurrP); // LButtonUp
	virtual void CompleteCurrUnitPoint(CPoint CurrP);  // LButtonDown
	virtual void CompleteCurrUnitLine(CPoint CurrP);   // LButtonDown
	virtual void CompleteCurrUnitArc(CPoint CurrP);    // LButtonDown
	virtual void CompleteCurrUnitArcB(CPoint CurrP);  // LButtonDown
	virtual void CompleteCurrUnitCircle(CPoint CurrP); // LButtonDown
	virtual void CompleteCurrUnitPolyline(CPoint CurrP, BOOL bIsCurrP=TRUE); // ESC, LButtonDblClk, LButtonDown(시작점 동일시)
	virtual void CompleteCurrUnitRect(CPoint CurrP); // LButtonDown
	// 모드(DGNDRAW_WINMODE_XXX)추가시 함수추가할 것...

	// 입력하려는 가상 Draw 정보를 사용자에게 보여주기 위해 만듬			
	virtual void MakeCurrUnitPoint(double dStartP[2]);
	virtual void MakeCurrUnitLine(double dStartP[2], double dEndP[2]);
	virtual void MakeCurrUnitLine(int no, double dStartP[2], double dEndP[2]);
	virtual void MakeCurrUnitArc(double dStartP[2], double dPassP[2], double dEndP[2]);
	virtual void MakeCurrUnitArcB(double dStartP[2], double dEndP[2], double dBulgeP[2]);
	virtual void MakeCurrUnitCircle(double dStartP[2], double dEndP[2]);
	virtual void MakeCurrUnitPolyline(T_POLYLINE_R& PolylineRD, double dEndP[2], BOOL bIsEndP=TRUE);
	virtual void MakeCurrUnitRect(double dStartP[2], double dEndP[2]);
	virtual void MakeSnapGroup(CPoint snapPoint);
	virtual void MakeHighLightGroup(nrLine &aHighlightLine);
	virtual void MakeSelectRectGroup(CPoint startPoint, CPoint endPoint);
	virtual void MakeCurrUnitSelectMove(CPoint startPoint, CPoint endPoint);
	// 모드(DGNDRAW_WINMODE_XXX)추가시 함수추가할 것...

	// 선택 완료후 Update명령 실행 
	virtual void RunUpdateCompleteObjectData(UINT nFlags, double dVal1, double dVal2);

	// View창의 상태를 외부 Control에 표시
	virtual void DisplayCoordText(CPoint CurrP);
	virtual void DisplayModeText();

	// Snap에 적용될경우 해당 좌표를 Snap좌표로 변환하여 준다.
	virtual void ChangeSnapPoint(CPoint& CurrPoint, double CurrPointR[2]);
	
	// HighLight 좌표를 바꿈
	virtual void ChangeHighlightLine(CPoint currPoint);

	void    GetGenuineCoord(CPoint CurrP, double& dXR, double& dYR);//화면좌표를 실제좌표값으로 변환하여줌
	void    GetDisplayCoord(double dXR, double dYR, CPoint& CurrP); //실제좌표를 화면좌표값으로 변환하여줌
	CString GetCoordText(double dX, double dY);// 좌표값을 문자열로 변환하여줌
	CString GetValueText(double dValue);       // 값을 문자열로 변환하여줌
	BOOL    GetCoordFromString(CString& strCoord, double& dX, double& dY); //String으로부터 좌표값을 가져옴
	BOOL    GetCoordFromEdit(CEdit* pEdit, double& dX, double& dY); //EditBox로부터 좌표값을 가져옴	
	BOOL    GetValueFromEdit(CEdit* pEdit, double& dValue);         //EditBox로부터 값을 가져옴
	BOOL    GetValueFromChkBox(CButton* pChk, BOOL& bValue);        //CheckBox로부터 값을 가져옴

	BOOL	GetOpjectHighlightLine(CPoint currPoint, nrLine &aHighlightLine);

	BOOL    GetOpjectSnapPoint(CPoint currPoint, CPoint& snapPoint, T_POINT& snapPointR);
	BOOL    GetGridSnapPoint(CPoint currPoint, CPoint& snapPoint, T_POINT& snapPointR);

	void	GetConnectedKey(nrUINT &arSelectKey, nrUINT &arConnectedKey);
	void ReDraw();
	BOOL MakeDibFile(CBitmap& gdiBmp, CDC* pDC, CFile& file);
 
		// Implementation
protected:
	// Generated message map functions
	// 필수!!! : 실제 Message Map과 연결시킬것	
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); // 직접적으로 연결되지 않으므로 부모에서 부터 연결
	virtual void OnMButtonDown(UINT nFlags, CPoint point);
	virtual void OnMButtonUp(UINT nFlags, CPoint point);
	virtual void OnMButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnContextMenu(CWnd* pWnd, CPoint point);

	// 직접적으로 연결되지 않으므로 부모에서 부터 연결
	//(OnKeyXXX는 접근되지 않으므로 On_KeyXXX 함수보다는 부모의 PreTranslateMessage를 이용한 KeyXXX함수이용을 추천)
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);     
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

#include "HeaderPost.h"
