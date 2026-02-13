#if !defined(AFX_DGNDRAW_SECTVIEWPANNEL_H__0132035C_692C_42D0_AAAD_546DAA285F44__INCLUDED_)
#define AFX_DGNDRAW_SECTVIEWPANNEL_H__0132035C_692C_42D0_AAAD_546DAA285F44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnDraw_SectViewPannel.h : header file
//

//#include "UMD_DrawRes2.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\DgnLib\DgnBaseLib\DgnBase_Struct.h"
#include "..\DgnLib\DgnBaseLib\DgnBaseShape_Struct.h"

#include "DgnDrawBase_DrawView_Impl.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnDraw_SectViewPannel dialog
//
// CDgnDrawBase_DrawView_Impl에서 기본적으로 재공되는 그리기 형식을 
// 철근입력형식 그리기로 대채합니다. 
/////////////////////////////////////////////////////////////////////////////

class __MY_EXT_CLASS__ CDgnDraw_SectViewPannel : public CDgnDrawBase_DrawView_Impl
{
// Construction
public:
	CDgnDraw_SectViewPannel(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnDraw_SectViewPannel)
	enum { IDD = IDD_DGN_DRAW_SECTVIEW_PANNEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void Make_CurrUnit_Rebar(double dStartP[2]);
	
// Overrides	(CurrUnit그리기 형식을 재지정합니다.)
	// 작업전
	virtual void Before_CurrUnit_Point(CPoint CurrP);
	virtual void Before_CurrUnit_Line(CPoint CurrP);
	virtual void Before_CurrUnit_Arc(CPoint CurrP);
	virtual void Before_CurrUnit_Circle(CPoint CurrP);	
	// 작업완료
	virtual void Complete_CurrUnit_Point(CPoint CurrP);
	virtual void Complete_CurrUnit_Line(CPoint CurrP);
	virtual void Complete_CurrUnit_Arc(CPoint CurrP);
	virtual void Complete_CurrUnit_Circle(CPoint CurrP);		
	// Curr 그리기	
	virtual void Make_CurrUnit_Point(double dStartP[2]);
	virtual void Make_CurrUnit_Line(double dStartP[2], double dEndP[2]);
	virtual void Make_CurrUnit_Arc(double dStartP[2], double dPassP[2], double dEndP[2]);
	virtual void Make_CurrUnit_Circle(double dStartP[2], double dEndP[2]);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnDraw_SectViewPannel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	
	
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnDraw_SectViewPannel)
	  //
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////////////////////////////////////
protected:
	UINT m_nVoidFormatKey;      // 공동부
	UINT m_nConcFormatKey;      // Concrete 단면
	UINT m_nSteelFormatKey;     // Steel 단면
	UINT m_nSteelLineFormatKey; // Line타입의 Steel 부제

	UINT m_nRbarFormatKey;      // Rebar
	UINT m_nTendonFormatKey;    // PS강선
	UINT m_nDuctFormatKey;       // Duct
	UINT m_nStiffenerFormatKey; // Stiffener
	UINT m_nStressFixFormatKey; // Stress Point(Fix Point)
	UINT m_nStressMoveFormatKey;// Stress Point(Moving Point)
	UINT m_nPartFormatKey;      // 영역
	UINT m_nPartTextFormatKey;  // 영역

	UINT m_nGideFormatKey;      // Gide Line
	
	// 공통
	int    m_nDrawType;// 0:None, 1:Rebar, 2:Stiffener, 3:Stress Point, 4:Part, 5:Stiffness
	BOOL   m_bCenter;

	// Rebar
	BOOL   m_bNum;
	int    m_nRebarNum;
	double m_dRebarCTC;
	double m_dRebarDia;
	BOOL   m_bSideRebar;
	int    m_nRebarPosiType;	

	BOOL   m_bGideLine;
	double m_dGideDis;

	// Tendon
	BOOL   m_bPropertyName;
	BOOL   m_bDuctHall;

	// 공통 Data
	DGN_GSEC_SHAPE_D      m_SectD;   // 단면정보
	DGN_GSEC_POLYGON_LIST m_arConcOutPoly;
	DGN_GSEC_POLYGON_LIST m_arConcInPoly;
	DGN_GSEC_POLYGON_LIST m_arSteelOutPoly;
	DGN_GSEC_POLYGON_LIST m_arSteelInPoly;
	DGN_GSEC_LINE_LIST    m_arSteelLine;

	// Rebar Data
	CArray<T_RPSC_MBAR_SSM, T_RPSC_MBAR_SSM&> m_arRbarD; // 철근정보
	CArray<UINT, UINT>m_arRbarKey;

	// Stress Point
	CArray<T_POINT_SSM, T_POINT_SSM&> m_arStressPointD;
	CArray<UINT, UINT>m_arStressPointKey;
	int m_nFixPointSize;

	// Stiffener

	// Part
	
	// Tendon
	CArray<T_GSEC_TNDN, T_GSEC_TNDN&> m_arTendonD;
	
public: 
	// Sect
	BOOL SetSectData(T_SECT_D& SectD, int nPos, BOOL bAfter, BOOL bCenter, BOOL bFit=TRUE);
	// Rebar
	void SetGideLine(BOOL bGideLine, double dGideDis);
	void SetRbarParameter(BOOL bNum, int nRebarNum, double dRebarCTC, double dRebarDia, BOOL bSideRebar, int nRebarPosiType);
	void SetRebarData(CArray<T_RPSC_MBAR_SSM, T_RPSC_MBAR_SSM&> arRbarD, CArray<UINT, UINT>& arRbarKey, BOOL bCenter, BOOL bFit=TRUE);
	// Stress Point
	void SetStressPointData(CArray<T_POINT_SSM, T_POINT_SSM&> arStressPointD, CArray<UINT, UINT>& arStressPointKey, int nFixPointSize, BOOL bCenter, BOOL bFit=TRUE);
	// Stiffener

	// Part
	
	// Tendon
	void SetTendonParameter(BOOL bPropertyName, BOOL bDuctHall);
	void SetTendonData(CArray<T_GSEC_TNDN, T_GSEC_TNDN&> arTendonD, BOOL bCenter, BOOL bFit=TRUE);

protected:
	void Set_DrawFormat();
	void Make_DrawUnit(BOOL bFit=TRUE, BOOL bCreateRebarData=TRUE);
	void Make_DrawSectUnit();			
	void Make_DrawGideLine();	
	void Make_DrawRebarUnit();
	void Make_DrawStressPointUnit();
	void Make_DrawPartUnit();
	void Make_DrawStiffenerUnit();
	void Make_DrawTendonUnit();	


	BOOL Find_StartEndPoint(CArray<int, int> arLineID, CArray<DGN_GSEC_LINE, DGN_GSEC_LINE&>& arLineData_Sort);
	BOOL Change_ArrayData(CArray<int, int>& List, int nID1, int nID2);
	void Change_LineEndData(DGN_GSEC_LINE& LineD);
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNDRAW_SECTVIEWPANNEL_H__0132035C_692C_42D0_AAAD_546DAA285F44__INCLUDED_)


