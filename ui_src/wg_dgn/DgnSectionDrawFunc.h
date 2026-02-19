//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
// 2002. 5. 8
// For Drawing Section  by TAE
//////////////////////////////////////////////////////////////////////////////////////

#if !defined(__SECTION_DRAWFUNC_DGN_H__)
#define __SECTION_DRAWFUNC_DGN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "DgnStruct.h"
#include "Dgn_DrawManager.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSectionDrawFunc : public CWnd
{
// Construction
public:
	CDgnSectionDrawFunc();
	virtual ~CDgnSectionDrawFunc();

	// Set_Canvas ////////////////////////////////////

	// 그림을 그릴 공간을 설정함
	//    Canvas    : CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	//    CanvasType: 0:Dlg형식, 1:Meta형식
	//    AxisType  : 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시 
	//    DimSize   : 표현할 치수선의 글자 크기
	//    RebarSize : 표현한 철근그림의 직경
	//    LengthUnit: 치수선의 표현 단위계 (-1:Current단위계 사용  'D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm  이하동일)
	//    WidthLengthUnit
	//    DimType   : 1:전체치수선 표시, 2:B,H치수선만 표시 (DgnDrawDLL사용시만 적용)
	void Set_Canvas(CRect Canvas, int CanvasType=0, int AxisType=1, int DimSize=10, int RebarSize=4, int LengthUnit= -1, int WidthLengthUnit=D_UNITSYS_LENGTH_INDEX_M, int DimType=1, int DimShape=1, double ZoomFactor=0.65, int ChkPointSize=4, int nLineSize=1);

// Main Draw (Dll방식)////////////////////////////////////	
// Civil에서 Result View 및 계산서에 사용
public:
	// RC, Steel겸용
	void Draw_Section_For_SectK   (CDC* pDC, T_SECT_K SectK, int nIMJ, int MatlType=1, BOOL bDim=FALSE, BOOL bFill=TRUE, BOOL bChkPoint=FALSE);// MatlType(1:Conc, 2:Steel)
	void Draw_Section_For_ElemK   (CDC* pDC, T_ELEM_K ElemK, int nIMJ, int MatlType=1, BOOL bDim=FALSE, BOOL bFill=TRUE, BOOL bChkPoint=FALSE);// MatlType(1:Conc, 2:Steel)
	// RC전용
	void Draw_Section_For_RcBeam  (CDC* pDC, T_ELEM_K ElemK, int nIMJ, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int nRebarDataType=0, int nTypeRebarView=0);
	void Draw_Section_For_RcColumn(CDC* pDC, T_ELEM_K ElemK, int nIMJ, double RotAngle, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE);
	void Draw_Section_For_RcWall  (CDC* pDC, T_ELEM_K ElemK, int nIMJ, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int nRebarDataType=0, int nTypeRebarView=0);
	void Draw_Section_For_RcPlate (CDC* pDC, T_RCPL_K RcplK, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int nTypeRebarView = 0, int nRebarDataType=0);

protected:	
	// Data Convert (Dll용)
	BOOL ConvertToDLL_Polygon(T_GSEC_POLYGON& InData, DGNDRAW_GSEC_POLYGON& OutData);
	BOOL ConvertToDLL_Line(T_GSEC_LINE& InData, DGNDRAW_GSEC_LINE& OutData);
	BOOL ConvertToDLL_SectData        (T_SECT_SECTBASE_D& InData, DGNDRAW_SECT& OutData);
	BOOL ConvertToDLL_RcBeamData      (T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_CPBE_D& RbarData, DGNDRAW_SECT_BEAM&   OutData);
	BOOL ConvertToDLL_RcColumnData    (T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_CPCO_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_RBTE_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcWallData      (T_SECT_SECTBASE_D& SectData, int nIMJ, T_RCWA_D& WallData, T_CPWA_D& RbarData, DGNDRAW_SECT_WALL&   OutData);
	BOOL ConvertToDLL_RcPlateData     (T_THIK_D&          ThikData,                               _DGNC_CPPL_D& RbarData, DGNDRAW_SECT_PLATE&  OutData);
	CString ConvertToDLL_ShapeName(CString Shape);
	int  ConvertToDLL_LengthUnitID(int UnitID);

public:
	// DLL방식의 Draw_Section_For_XXX함수사용(08.01.10) ////////////////////////////////////
	// - Beam, Column ()
	
	// 해당 Element에 대한 해당 Postion의 단면 및 철근을 CDC객체에 그림
	//    pDC       : 그림을 그릴 CDC 객체
	//    Canvas    : CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	//    ElemK     : Section ID
	//    nIMJ      : Position ID ( 0:I,  1:M,  2:J)
	//    RotAngle  : 중립축 회전각
	//    CanvasType: 0:Dlg형식, 1:Meta형식
	//    AxisType  : 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시 
	//    DimSize   : 표현할 치수선의 글자 크기
	//    RebarSize : 표현한 철근그림의 직경
	//    bShape,     bRebar    : 단면, 철근을 그릴지 여부
	//    bDimShape,  bDimRebar : 단면, 철근의 치수선을 그릴지 여부
	//    bFillShape, bFillRebar: 단면, 철근의 내부에 색을 채울지 여부
	//    LengthUnit: 치수선의 표현 단위계 (-1:Current단위계 사용  'D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm  이하동일)
	//    nRebarDataType: Rebar표현방식(0:철근정보의 bNum값을 따름  1:Num정보로 철근표시  2:CTC정보로 철근표시)
	//    nTypeRebarView        : 철근을 보여주는 방법(0:상단하단 모두 표시,  1:상단만 표시,  2:하단만 표시)
	void Draw(CDC* pDC, CRect Canvas, T_ELEM_K ElemK , int nIMJ, double RotAngle=0.0, int CanvasType=0, int AxisType=1, int DimSize=12, int RebarSize=4, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int LengthUnit= -1, int WidthLengthUnit=D_UNITSYS_LENGTH_INDEX_M, int nRebarDataType=0, int nTypeRebarView=0);
	// 해당 Element에 대한 해당 Postion의 단면 및 철근을 CDC객체에 그림
	//    pDC       : 그림을 그릴 CDC 객체
	//    ElemK     : Section ID
	//    nIMJ      : Position ID ( 0:I,  1:M,  2:J)
	//    RotAngle  : 중립축 회전각
	//    bShape,     bRebar    : 단면, 철근을 그릴지 여부
	//    bDimShape,  bDimRebar : 단면, 철근의 치수선을 그릴지 여부
	//    bFillShape, bFillRebar: 단면, 철근의 내부에 색을 채울지 여부
	//    nRebarDataType: Rebar표현방식(0:철근정보의 bNum값을 따름  1:Num정보로 철근표시  2:CTC정보로 철근표시)
	//    nTypeRebarView        : 철근을 보여주는 방법(0:상단하단 모두 표시,  1:상단만 표시,  2:하단만 표시)
	void Draw_Section(CDC* pDC, T_ELEM_K ElemK , int nIMJ, double RotAngle, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int nRebarDataType=0, int nTypeRebarView=0);
		
	// - Wall(Frame)

	// 해당 Element에 대한 해당 Postion의 단면 및 철근을 CDC객체에 그림
	//    pDC       : 그림을 그릴 CDC 객체
	//    Canvas    : CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	//    ElemK     : Section ID
	//    nIMJ      : Position ID ( 0:I,  1:M,  2:J)
	//    RotAngle  : 중립축 회전각
	//    CanvasType: 0:Dlg형식, 1:Meta형식
	//    AxisType  : 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시 
	//    DimSize   : 표현할 치수선의 글자 크기
	//    RebarSize : 표현한 철근그림의 직경
	//    bShape,     bRebar    : 단면, 철근을 그릴지 여부
	//    bDimShape,  bDimRebar : 단면, 철근의 치수선을 그릴지 여부
	//    bFillShape, bFillRebar: 단면, 철근의 내부에 색을 채울지 여부
	//    LengthUnit: 치수선의 표현 단위계 (-1:Current단위계 사용  'D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm  이하동일)
	//    nRebarDataType: Rebar표현방식(0:철근정보의 bNum값을 따름  1:Num정보로 철근표시  2:CTC정보로 철근표시)
	void Draw_Wall(CDC* pDC, CRect Canvas, T_ELEM_K ElemK , int nIMJ, double RotAngle=0.0, int CanvasType=0, int AxisType=1, int DimSize=12, int RebarSize=4, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int LengthUnit= -1, int WidthLengthUnit=D_UNITSYS_LENGTH_INDEX_M, int nRebarDataType=0, int nTypeRebarView=0);
	// 해당 Element에 대한 해당 Postion의 단면 및 철근을 CDC객체에 그림
	//    pDC       : 그림을 그릴 CDC 객체
	//    ElemK     : Section ID
	//    nIMJ      : Position ID ( 0:I,  1:M,  2:J)
	//    RotAngle  : 중립축 회전각
	//    bShape,     bRebar    : 단면, 철근을 그릴지 여부
	//    bDimShape,  bDimRebar : 단면, 철근의 치수선을 그릴지 여부
	//    bFillShape, bFillRebar: 단면, 철근의 내부에 색을 채울지 여부
	//    nRebarDataType: Rebar표현방식(0:철근정보의 bNum값을 따름  1:Num정보로 철근표시  2:CTC정보로 철근표시)
	void Draw_Section_Wall(CDC* pDC, T_ELEM_K ElemK , int nIMJ, double RotAngle, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int nRebarDataType=0, int nTypeRebarView=0);

	// - Plate(Rcpl)

	// 해당 RC Plate(Rcpl)에 대한 해당 Postion의 단면 및 철근을 CDC객체에 그림
	//    pDC       : 그림을 그릴 CDC 객체
	//    Canvas    : CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	//    RcplK     : Rcpl ID
	//    RotAngle  : 중립축 회전각
	//    CanvasType: 0:Dlg형식, 1:Meta형식
	//    AxisType  : 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시 
	//    DimSize   : 표현할 치수선의 글자 크기
	//    RebarSize : 표현한 철근그림의 직경
	//    bShape,     bRebar    : 단면, 철근을 그릴지 여부
	//    bDimShape,  bDimRebar : 단면, 철근의 치수선을 그릴지 여부
	//    bFillShape, bFillRebar: 단면, 철근의 내부에 색을 채울지 여부
	//    nTypeRebarView        : 철근을 보여주는 방법(0:상단하단 모두 표시,  1:상단만 표시,  2:하단만 표시)
	//    LengthUnit: 치수선의 표현 단위계 (-1:Current단위계 사용  'D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm  이하동일)
	//    WidthLengthUnit : 단위폭표현시 단위폭의 단위계
	//    nRebarDataType: Rebar표현방식(0:철근정보의 bNum값을 따름  1:Num정보로 철근표시  2:CTC정보로 철근표시)
	void Draw_Rcpl(CDC* pDC, CRect Canvas, T_RCPL_K RcplK, double RotAngle=0.0, int CanvasType=0, int AxisType=1, int DimSize=12, int RebarSize=4, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int nTypeRebarView = 0, int LengthUnit= -1, int WidthLengthUnit=D_UNITSYS_LENGTH_INDEX_M, int nRebarDataType=0);
	// 해당 RC Plate(Rcpl)에 대한 해당 Postion의 단면 및 철근을 CDC객체에 그림
	//    pDC       : 그림을 그릴 CDC 객체
	//    RcplK     : Rcpl ID
	//    RotAngle  : 중립축 회전각
	//    bShape,     bRebar    : 단면, 철근을 그릴지 여부
	//    bDimShape,  bDimRebar : 단면, 철근의 치수선을 그릴지 여부
	//    bFillShape, bFillRebar: 단면, 철근의 내부에 색을 채울지 여부
	//    nTypeRebarView        : 철근을 보여주는 방법(0:상단하단 모두 표시,  1:상단만 표시,  2:하단만 표시)
	//    nRebarDataType: Rebar표현방식(0:철근정보의 bNum값을 따름  1:Num정보로 철근표시  2:CTC정보로 철근표시)
	void Draw_Section_Rcpl(CDC* pDC, T_RCPL_K RcplK, double RotAngle, BOOL bShape=TRUE, BOOL bRebar=TRUE, BOOL bDimShape=FALSE, BOOL bDimRebar=FALSE, BOOL bFillShape=TRUE, BOOL bFillRebar=TRUE, int nTypeRebarView = 0, int nRebarDataType=0);

// Main Draw (기존)////////////////////////////////////
// 철근입력 Dlg에 사용 (차후 Dll방식으로 변환 요망 08.01.10)
	
	// Draw_Shape
		
	// RC단면을 그림 
	// ※ 이때 SectK는 Beam, Column, Wall일때에는 Sect.Key,  Plate일 경우에는 Thik.Key를 넘겨야 함
	void Draw_Shape(CDC* pDC, int nType, int SectK, BOOL bDim=FALSE, BOOL bFill=TRUE, int iPosi=0, double dHaunch=0.0);  

	void Draw_Beam        (CDC* pDC, T_SECT_K SectK, BOOL bDim=FALSE, BOOL bFill=TRUE, int iPosi=0);   
	void Draw_Beam_Rect   (CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Beam_Tee    (CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Beam_PSC_MID(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);	

	void Draw_Colm(CDC* pDC, T_SECT_K SectK, BOOL bDim=FALSE, BOOL bFill=TRUE, int iPosi=0);  
	void Draw_Colm_vOcta(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_sOcta(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_vTrck(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_sTrck(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_sHTrk(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_vCirc(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_sCirc(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_vRect(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_sRect(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_Gen  (CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Colm_H(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);

	void Draw_Wall        (CDC* pDC, T_SECT_K SectK, BOOL bDim=FALSE, BOOL bFill=TRUE, int iPosi=0, double dHaunch=0.0, BOOL bUWidth=TRUE);   
	void Draw_Wall_Rect   (CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE, double dHaunch=0.0);
	void Draw_Wall_UWidth (CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE, double dHaunch=0.0);

	void Draw_Plate       (CDC* pDC, T_THIK_K ThikK, BOOL bDim=FALSE, BOOL bFill=TRUE, int iPosi=0);   
	void Draw_Plate_UWidth(CDC* pDC, T_THIK_K ThikK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);

	// Add, Jaeoh.('07.11.12)
	// For KSCE-ASD05, Steel Section.	
	void Draw_Section_For_Steel(CDC* pDC, T_ELEM_K ElemK , int nIMJ, double RotAngle, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nRebarDataType=0);
	void Draw_Section_4KR_STL(CDC* pDC, T_SECT_K SectK, BOOL bDim, BOOL bFill, int iPosi);
	void Draw_Steel_L(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_Steel_C(CDC* pDC, T_SECT_K SectK, int iPosi=0, BOOL bDim=FALSE, BOOL bFill=TRUE);

	//////////////////////////////////////////////////////////////////////
	// Draw_ChkRebar
	// T_RCHK_D 용
	void Draw_ChkRebar          (CDC* pDC, T_SECT_K SectK, int nIMJ, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Beam     (CDC* pDC, T_SECT_K SectK, int nIMJ, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Beam_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Beam_Tee (CDC* pDC, T_SECT_K SectK, int nIMJ, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	
	void Draw_ChkRebar_Colm(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Rect(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Circ(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Octa(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Trck(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Htrk(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_H(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);

	// T_CPBE_D & T_CPCO_D 용 (SHIN '06.03.20 추가)
	
	void Draw_ChkRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);

	void Draw_ChkRebar_Beam        (CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Beam_Rect   (CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Beam_Tee    (CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Beam_PSC_MID(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	// 입력된 철근을 그림
	//   pDC   : 바탕 객체
	//   SectK : Section ID 
	//   nIMJ  : Position ID (0:I, 1:M, 2:J)
	//   pCpcoD: 철근 정보
	//   bDim  : 치수선 표시여부
	//   bFill : 내부에 색을 채울지 여부
	void Draw_ChkRebar_Colm     (CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Circ(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Octa(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Trck(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Htrk(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar_Colm_Gen (CDC* pDC, T_SECT_K SectK, int nIMJ, T_RBTE_D* pRbteD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);

	// T_CPWA_D & _DGNC_CPPL_D 용 (SHIN '07.07.01 추가)
	void Draw_ChkRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPWA_D* pCpwaD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE, double dHaunch=0.0);
	void Draw_ChkRebar(CDC* pDC, T_THIK_K ThikK, _DGNC_CPPL_D* pCpplD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE, int nTypeRebarView=0);

	void Draw_ChkRebar_Wall_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPWA_D* pCpwaD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE, double dHaunch=0.0);
	void Draw_ChkRebar_Wall_UWidth(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPWA_D* pCpwaD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE, double dHaunch=0.0);

	void Draw_ChkRebar_Plate_UWidth(CDC* pDC, T_THIK_K ThikK, _DGNC_CPPL_D* pCpplD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE, int nTypeRebarView=0);



	//////////////////////////////////////////////////////////////////////
	// Draw_DgnRebar
	void Draw_DgnRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_DgnRebar_Beam(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_DgnRebar_Beam_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_DgnRebar_Beam_Tee( CDC* pDC, T_SECT_K SectK, int nIMJ, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);

	void Draw_DgnRebar_Colm(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_DgnRebar_Colm_Rect(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_DgnRebar_Colm_Circ(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_DgnRebar_Colm_Octa(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_DgnRebar_Colm_Trck(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_DgnRebar_Colm_Htrk(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD=NULL, BOOL bDim=FALSE, BOOL bFill=TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSectionDrawFunc)
	//}}AFX_VIRTUAL

protected:
	double  m_dB;
	double  m_dX, m_dY;	
	double  m_ScaleF;
	double  m_dAngle;// radian
	int     m_nCanvasType;
	int     m_nAxisType;	
	int     m_nDimSize;	
	int     m_nRebarSize;	
	int     m_nLengthUnit;
	int     m_nWidthLengthUnit;

	double  m_dZoomFactor;

	//(DgnDrawDLL사용시만 적용)
	int     m_nLineSize;	
	int     m_nDimType;      // 1:전체치수선 표시, 2:B,H치수선만 표시
	int     m_nDimShape;     // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	int     m_nChkPointSize; // 표현한 ChkPoint의 직경
	

	CDBDoc*  m_pDoc;
	CRect    m_Canvas;
	T_RDGN_D m_tmpRdgnD;
	T_RCHK_D m_tmpRchkD;
	CDgn_DrawManager m_DrawManager;

	BOOL CalcScaleFactor(double left, double top, double right, double bottom);
	BOOL CalcCanvasRange(double dB, double dH, double Lx=0., double By=0., double Rx=0., double Ty=0.);
	BOOL CalcCanvasRange(double left, double top, double right, double bottom, double Lx, double By, double Rx, double Ty, double dRotAngle);
	double GetLPByGPForLength(LONG dValGP);
	void MappingCoord (CArray<CPoint,CPoint>& rLPt, CArray<CPoint,CPoint>& rGPt);
	void OffsetCoord  (CArray<CPoint,CPoint>& rLPt, CArray<CPoint,CPoint>& rGPt, double dx, double dy);
	void RotationCoord(CArray<CPoint,CPoint>& rLPt, CArray<CPoint,CPoint>& rGPt, double dAngle);
	T_SECT_SECTBASE_D GetSectBaseD(T_SECT_D& SectD, int nIMJ);
	T_SECT_SECTBASE_D GetSectBaseD_For_SectK(T_SECT_K SectK, int nIMJ);
	T_SECT_SECTBASE_D GetSectBaseD_For_ElemK(T_ELEM_K ElemK, int nIMJ, int MatlType=1);// MatlType(1:Conc, 2:Steel)

	void Draw_AxisYZ(CDC* pDC, CArray<CPoint, CPoint>& rGPt);
	void Draw_NeutralAxis(CDC* pDC, CArray<CPoint, CPoint>& rGPt);
	void Draw_Concrete(CDC* pDC, CArray<CPoint, CPoint>& rGPt, BOOL bFill);
	void Draw_rConcrete(CDC* pDC, CArray<CPoint, CPoint>& rGPt, BOOL bFill);
	void Draw_StirrupBar(CDC* pDC, CArray<CPoint, CPoint>& rGPt, BOOL bFill, BOOL bClose);
	void Draw_Rebar(CDC* pDC, CArray<CPoint, CPoint>& rGPt, BOOL bFill, BOOL bEndCrossMark = FALSE);
	void Draw_Dimension(CDC* pDC, CArray<CPoint, CPoint>& rGPt, CString& strTxt);
	void Draw_DgnRebar(CDC* pDC, CArray<CPoint, CPoint>& arGPt, BOOL bFill);
	void Draw_DgnCRebar(CDC* pDC, CArray<CPoint, CPoint>& arGPt, BOOL bFill);
	void Draw_DgnLRebar(CDC* pDC, CArray<CPoint, CPoint>& arGPt, BOOL bFill);

	// T_RCHK_D 용
	BOOL Draw_LineChkRebar(BOOL bIncEdge, int iPosi, int iGrup, int iBarLay, double dStaY, double dEndY, double dStaZ, double dEndZ, 
												T_RCHK_D* pRchkD, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CircChkRebar(int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ, double dCenZ,
												T_RCHK_D* pRchkD, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CirLChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ, double dCenZ, 
												double dCenDist, T_RCHK_D* pRchkD, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CirRChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ, double dCenZ, 
												double dCenDist, T_RCHK_D* pRchkD, CArray<CPoint, CPoint>& rGPt);

	// T_CPBE_D 용
	BOOL Draw_LineChkRebar(BOOL bIncEdge, int iPosi, int iGrup, int iBarLay, double dStaY, double dEndY, double dStaZ, double dEndZ, 
												T_CPBE_D* pCpbeD, CArray<CPoint, CPoint>& rGPt, BOOL& bEndCrossMark);
	// T_CPCO_D 용 (SHIN '06.03.20 추가)
	BOOL Draw_LineChkRebar(BOOL bIncEdge, int iPosi, int iGrup, int iBarLay, double dStaY, double dEndY, double dStaZ, double dEndZ, 
												T_CPCO_D* pCpcoD, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CircChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ, double dCenZ,
												T_CPCO_D* pCpcoD, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CirLChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ, double dCenZ, 
												double dCenDist, T_CPCO_D* pCpcoD, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CirRChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ, double dCenZ, 
												double dCenDist, T_CPCO_D* pCpcoD, CArray<CPoint, CPoint>& rGPt);

	// T_CPWA_D 용
	BOOL Draw_LineChkRebar(BOOL bIncEdge, int iPosi, int iGrup, int iBarLay, double dStaY, double dEndY, double dStaZ, double dEndZ, 
												T_CPWA_D* pCpwaD, CArray<CPoint, CPoint>& rGPt, BOOL& bEndCrossMark);

	// _DGN_CPPL_D 용
	BOOL Draw_LineChkRebar(BOOL bIncEdge, int iGrup, int iBarLay, double dStaY, double dEndY, double dStaZ, double dEndZ, 
												_DGNC_CPPL_D* pCpplD, CArray<CPoint, CPoint>& rGPt, BOOL& bEndCrossMark);

	// 공통 용 (SHIN '06.03.20 추가)
	BOOL Draw_LineChkRebar(BOOL bIncEdge, double dStaY, double dEndY, double dStaZ, double dEndZ, 
												int iDivNum, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CircChkRebar(double dStaY, double dCenY, double dStaZ, double dCenZ,
												int iDivNum, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CirLChkRebar(double dStaY, double dCenY, double dStaZ, double dCenZ, double dCenDist, 
												int iDivNum, CArray<CPoint, CPoint>& rGPt);
	BOOL Draw_CirRChkRebar(double dStaY, double dCenY, double dStaZ, double dCenZ, double dCenDist, 
												int iDivNum, CArray<CPoint, CPoint>& rGPt);

	// Current Unit에 대한 길이 단위를 iCngUnitLen로 변환할때 곱하여 줘야 하는 값을 넘겨줌
	//   iCngUnitLen : 변경할 길이 단위
	//   RETURN : 단위 변경을 위해 곱해줄 값
	double Cng_UnitLen(int iCodeUnitLen);


	//{{AFX_MSG(CDgnSectionDrawFunc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"      

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSECTIONDRAWFUNC_H__43E09501_4DFA_45C6_951F_41648E81D002__INCLUDED_)
