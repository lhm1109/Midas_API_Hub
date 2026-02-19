// DgnDrawBase_DrawView_Impl.h: interface for the CDgnDrawBase_DrawView_Impl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNDRAWBASE_DRAWVIEW_IMPL_H__84A55205_D05D_4B91_812A_A0255BE40539__INCLUDED_)
#define AFX_DGNDRAWBASE_DRAWVIEW_IMPL_H__84A55205_D05D_4B91_812A_A0255BE40539__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  CDgnDrawBase_DrawView을 상속받은 Implement용 Class입니다.
//  MESSAGE MAP의 연결 문제로 
//  해당 Message를 CDgnDrawBase_DrawView와 연결시켜주는 역활을 수행합니다.
//  
//  따라서 해당 Project는 CDgnDrawBase_DrawView_Impl을 사용하여 CDgnDrawBase_DrawView을 구현합니다.
//
//  ※ CDgnDrawBase_DrawViews는 DgnDrawBase.dll의 클래스로 해당 DLL은 Implicit Link방식으로 
//     사용을 위해서는 Project Setting >> Link >> Object/library modules에 DgnDrawBase.lib를 연결하여야 합니다.
//
//  메시지 연결 (OnPaint...OnRButtonDblClk등에서 On_Paint...On_RButtonDblClk을 호출) 
//
//  일반함수 연결 (SetFit...SetCurrUnit_Circle등에서 Se_tFit...Set_CurrUnit_Circle을 호출) 
//  (현재 Project에서는 상속받은 CDgnDrawBase_DrawView의 함수를 직접 사용이 가능하나 
//   다른 Project에서 CDgnDrawBase_DrawView의 일반 함수를 사용하기 위해서는 위의 Link설정을 하여야 하므로
//   Link설정없이 사용할 수 있도록 함수를 다시 지정하여 둔다.) 
//  ※ 현재(08.04.14) 일반함수만 재지정하였으며 다른 Project에서 가상함수를 사용하기 위해서는 추가 지정해주어야 한다.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\DgnLib\DgnDrawBaseLib\DgnDrawBase_DrawView.h"

#include "HeaderPre.h"


class __MY_EXT_CLASS__ CDgnDrawBase_DrawView_Connector_Impl : public CDgnDrawBase_DrawView_Connector
{
// Construction
public:
	CDgnDrawBase_DrawView_Connector_Impl();   // standard constructor
	virtual ~CDgnDrawBase_DrawView_Connector_Impl();
};

class __MY_EXT_CLASS__ CDgnDrawBase_DrawView_Impl : public CDgnDrawBase_DrawView
{
// Construction
public:
	CDgnDrawBase_DrawView_Impl(UINT resID, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnDrawBase_DrawView_Impl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnDrawBase_DrawView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Member Function
public:
	BOOL OnInitialDialog(CWnd* pParentWnd, UINT nID_PlaceHolder);
	void SetFit();
	void SetMode(int nMode);
	int  GetMode();	
	void SetSnap(BOOL bObjectSnap, BOOL bGridSnap, int nSnapDis=2);// Opject에 Snap적용여부, Grid에 Snap적용여부
	void SetAxisMode(int nAxisMode, int nSize, int nWidth, BOOL bCapital=TRUE); //nAxisMode:표시형식, nSize:크기, nWidth:폭, bCapital:대문자 표시여부
	void SetAxisLocal(double dOrgPoint[2], double dAxisVector1[2], double dAxisVector2[2]);// Local축 사용시 원점의 실좌표 및 각 축의 방향을 입력함(DGNDRAW_AXISMODE_PANTALINE_XY_L, DGNDRAW_AXISMODE_PANTALINE_YZ_L, DGNDRAW_AXISMODE_PANTALINE_XZ_L 사용시에만 적용)
	void SetGridMode(int nGridMode, double dInterval_x, double dInterval_y);
	
	void Set_WndConnector(CDgnDrawBase_DrawView_Connector* pConnector);
	void SetWndCoordText(CBCGPStatic* pText);
	void SetWndModeText(CBCGPStatic* pText);
	void SetWndPointEdit(CEdit* pEdit);
	void SetWndLineEdit(CEdit* pStartEdit, CEdit* pEndEdit);	
	void SetWndArcEdit(CEdit* pStartEdit, CEdit* pPassEdit, CEdit* pEndEdit);
	void SetWndCircleEdit(CEdit* pStartEdit, CEdit* pEndEdit, CEdit* pRadiusEdit);	
	
	void Zoom(CPoint CurrP, double dZoomRatio);
	void ZoomOut(CPoint CurrP);
	void ZoomIn(CPoint CurrP);
	void ZoomOut();
	void ZoomIn();
	
	// SelectKey
	int  Get_SelectedKeyList(CArray<UINT, UINT>& arKey);
	int  Get_SelectedLinkKeyList(CArray<UINT, UINT>& arLinkKey);
	void Set_SelectedKeyList(CArray<UINT, UINT>& arKey, BOOL bReDraw);
	void Set_SelectedLinkKeyList(CArray<UINT, UINT>& arLinkKey, BOOL bReDraw);

	// CurrUnit를 직접 입력	
	void ClearCurrUnit();
	void SetCurrUnit_Point();
	void SetCurrUnit_Point(double dStartP[2]);
	void SetCurrUnit_Line(int nCount=1);
	void SetCurrUnit_Line(double dStartP[2], double dEndP[2], int nCount=1);
	void SetCurrUnit_Arc(int nCount=2);
	void SetCurrUnit_Arc(double dStartP[2], double dPassP[2], double dEndP[2], int nCount=2);
	void SetCurrUnit_Circle(int nCount=1, int nType=1);// nType(1:반지름입력값으로 계산 2:마지막좌표값으로 계산)
	void SetCurrUnit_Circle(double dStartP[2], double dEndP[2], int nCount=1);

protected:
	// 가상함수는 필요시 설정하여 줄것

	void    GetGenuineCoord(CPoint CurrP, double& dXR, double& dYR);//화면좌표를 실제 좌표값으로 변환하여줌
	void    GetDisplayCoord(double dXR, double dYR, CPoint& CurrP);//화면좌표를 실제 좌표값으로 변환하여줌
	CString GetCoordText(double dX, double dY);// 좌표값을 문자열로 변환하여줌
	BOOL    GetCoordFromEdit(CEdit* pEdit, double& dX, double& dY); //EditBox로부터 좌표값을 가져옴
	
	BOOL    GetOpjectSnapPoint(CPoint currPoint, CPoint& snapPoint, T_DGN_POINT& snapPointR);
	BOOL    GetGridSnapPoint(CPoint currPoint, CPoint& snapPoint, T_DGN_POINT& snapPointR);
	
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnDrawBase_DrawView)	
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	
	// OnKeyDown, OnKeyUp 이벤트를 받아 오지 못하므로 일단 보류함(2010.05.25)
	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags); 
	//afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNDRAWBASE_DRAWVIEW_IMPL_H__84A55205_D05D_4B91_812A_A0255BE40539__INCLUDED_)
