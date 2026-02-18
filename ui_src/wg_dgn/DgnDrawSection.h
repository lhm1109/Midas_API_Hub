// DgnDrawSection.h: interface for the CDgnDrawSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNDRAWSECTION_H__8549B2FC_4EC1_4685_BF5E_82968917B95C__INCLUDED_)
#define AFX_DGNDRAWSECTION_H__8549B2FC_4EC1_4685_BF5E_82968917B95C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "DgnStruct.h"
#include "Dgn_DrawManager.h"
#include "..\dgnengine\src\DgnBase\DgnBaseShape_Struct.h"

struct _DGN_DRAW_SECT_OPTION
{
	// 공통항목
	CRect Canvas;        // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	int CanvasType;      // 0:Dlg형식, 1:Meta형식
	int AxisType;        // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시
	int AxisShape;       // 1:선으로 표시, 2:화살표로 표시
	int DimType;         // 1:전체치수선 표시, 2:B,H치수선만 표시
	int DimShape;        // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호), 12:Text중앙(값, 공간미확보시 밖에 표시), 14:Text중앙(기호, 공간미확보시 밖에 표시)
	int DimSize;         // 표현할 치수선의 글자 크기
	int RebarSize;       // 표현한 철근그림의 직경
	int SectLineSize;    // 단면의 선 두께(or LineType 선두께)
	int ChkPointSize;    // 표현한 ChkPoint의 직경
	int LengthUnit;      // 치수선의 표현 단위계 (-1:Current단위계 사용  'D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm  이하동일)
	int WidthLengthUnit; // 단위폭의 길이          ('D_UNITSYS_LENGTH_INDEX_MM'or 0:단위폭=1mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:단위폭=1cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:단위폭=1m  이하동일)
	double ZoomFactor;   // Canvas크기에 대한 그릴공간의 Zoom 비율(0.0~1.0);

	// 단면의 종류별 항목		
	double RotAngle;     // 중립축 회전각
	BOOL bReDraw;        // 다시그릴지 여부
	BOOL bRebar;         // 철근을 그릴지 여부
	BOOL bDimShape;      // 단면의 치수선을 그릴지 여부
	BOOL bDimRebar;      // 철근의 치수선을 그릴지 여부 (RC,SRC,PSC Only)
	BOOL bFillShape;     // 단면의 내부에 색을 채울지 여부
	BOOL bFillRebar;     // 철근의 내부에 색을 채울지 여부 (RC,SRC,PSC Only)
	BOOL bChkPoint;      // 응력검토 위치 좌표를 표시할지 여부 (Steel,PSC Only)
	int  nTypeRebarView; // 철근을 보여주는 방법(0:상단하단 모두 표시,  1:상단만 표시,  2:하단만 표시) (RC/Beam Only)
	int  nTypeStirrupRebarView;// 수직전단 철근을 보여주는 방법(0:표시하지 않음, 1:수직전단철근 표시) (RC/Beam Only)
	int  nRebarDataType; // 철근정보가 저장된 Data형식(0:Sect에 연결된 정보, 1:Elem에 연결된 정보 사용(없으시 Sect값 사용)) (RC/Beam,Column Only)
	int  nRebarDataType_Beam; // 철근정보를 Elem에 연결된 정보를 사용시 RC/Beam일 경우 Num값을 사용할지 CTC값을 사용할지 여부(0:Data가 가진값, 1:Num,  2:CTC) (RC/Beam Only)
	
	void Initialize()
	{
		Canvas.SetRect(0,0,0,0);
		CanvasType      =  0;
		AxisType        =  1;
		AxisShape       =  1;
		DimType         =  1;
		DimSize         = 10;
		DimShape        =  1;
		RebarSize       =  4;
		SectLineSize    =  1;
		ChkPointSize    =  4;
		LengthUnit      = -1;
		WidthLengthUnit = D_UNITSYS_LENGTH_INDEX_M;
		ZoomFactor      = 0.65;

		RotAngle        = 0.0;
		bReDraw         = FALSE;
		bRebar          = FALSE;
		bDimShape       = FALSE;
		bDimRebar       = FALSE;
		bFillShape      = TRUE;
		bFillRebar      = TRUE;
		bChkPoint       = FALSE;
		nTypeRebarView  = 0;
		nTypeStirrupRebarView = 0;
		nRebarDataType  = 0;
		nRebarDataType_Beam = 0;
	}
};






#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnDrawSection  
{
public:
	CDgnDrawSection();
	virtual ~CDgnDrawSection();

protected:
	CDBDoc*  m_pDoc;
	CDgn_DrawManager m_DrawManager;

public:	
	// RC, Steel겸용 (Key입력형) ※ 해당 함수는 철근은 그려주지 않습니다.
	
	// Sect 및 Elem에 따른 단면 삽도를 만들어 줍니다.(Make_SectionFig_For_XXX는  emf파일을 만들어 줍니다. Draw_Section_For_XXX는 
	//   strFileName or pDC : 만들 파일명 또는 화면상의 그림그릴곳의 CDC
	//   SectK or ElemK : 단면을 그릴 Sect Key 또는 Elem Key
	//   nIMJ : 그림을 그릴 단면의 위치(0:I  1:M  2:J)
	//   optionD : 그림 옵션(그림의 크기, 글자크기, 표현단위 등등...)
	//   bDesign : 단면을 그릴때 설계용단면을 사용하여 그릴지 여부
	//   MatlType : 단면의 재료형식(1:Conc, 2:Steel) ※형태에 따라 색표현이 달라짐
	//   bAfter   : 시공후 단면 여부 (D_SECT_TYPE_CONSTRUCTION일때에만 적용됩니다.)
	// < Return   : 그림을 생성 성공여부 (0:생성안함  1:그림을 그릴수 없음(축만표시)  2:생성성공) 
	int Make_SectionFig_For_SectK (CString& strFileName, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign=TRUE, int MatlType=1, BOOL bAfter=TRUE);
	int Make_SectionFig_For_ElemK (CString& strFileName, T_ELEM_K ElemK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign=TRUE, BOOL bAfter=TRUE);
	int Draw_Section_For_SectK    (CDC* pDC, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign=TRUE, int MatlType=1, BOOL bAfter=TRUE);
	int Draw_Section_For_ElemK    (CDC* pDC, T_ELEM_K ElemK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign=TRUE, BOOL bAfter=TRUE);

	// RC (Key입력형)
	BOOL Draw_Section_RcBeam_For_SectK  (CDC* pDC, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign=TRUE);
	BOOL Draw_Section_RcBeam_For_ElemK  (CDC* pDC, T_ELEM_K ElemK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign=TRUE);
	BOOL Draw_Section_RcColumn_For_SectK(CDC* pDC, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign=TRUE);
	BOOL Draw_Section_RcColumn_For_SectK(CDC* pDC, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign,
		                                   DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon);
	BOOL Draw_Section_RcColumn_For_ElemK(CDC* pDC, T_ELEM_K ElemK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign=TRUE);
	// RC (Data입력형)
	void Draw_Section_RcBeam  (CDC* pDC, T_SECT_D* pSectD, T_RCHK_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD);
	void Draw_Section_RcBeam  (CDC* pDC, T_SECT_D* pSectD, T_CPBE_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD);
	void Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_RCHK_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD);
	void Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_CPCO_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD);
	void Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_RBTE_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD);
	void Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_REBT_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD);
	void Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_RCHK_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD,
		                         DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon);
	void Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_CPCO_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD,
		                         DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon);
	void Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_RBTE_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD,
		                         DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon);
	void Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_REBT_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD,
		                         DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon);
	void Draw_Section_RcWall  (CDC* pDC, T_SECT_D* pSectD, T_CPWA_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, T_RCWA_D* pRcwaD);
	void Draw_Section_RcPlate (CDC* pDC, T_THIK_D* pSectD, T_REBT_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD);


	T_SECT_SECTBASE_D GetSectBaseD(T_SECT_D& SectD, int nIMJ);
	T_SECT_SECTBASE_D GetSectBaseD(T_SECT_D& SectD, int& nSType, int nIMJ, BOOL bAfter);
	T_SECT_SECTBASE_D GetSectBaseD(T_SECT_SECTBASE_D& SectI, T_SECT_SECTBASE_D& SectJ, int nIMJ);
	T_SECT_SECTBASE_D GetSectBaseD_For_SectK(T_SECT_K SectK, int nIMJ, BOOL bDesign=TRUE);
	T_SECT_SECTBASE_D GetSectBaseD_For_ElemK(T_ELEM_K ElemK, int nIMJ, BOOL bDesign=TRUE, int MatlType=1);// MatlType(1:Conc, 2:Steel)
	BOOL GetSectD_For_ElemK(T_ELEM_K ElemK, int nIMJ, T_SECT_D& SectD, BOOL bDesign=TRUE, int MatlType=1);// MatlType(1:Conc, 2:Steel)
	BOOL ConvertToDLL_SectData(T_SECT_SECTBASE_D& InData, DGNDRAW_SECT& OutData, BOOL bDesign=TRUE);
	BOOL ConvertToDLL_SectData(T_SECT_D& SectD, int nIMJ, DGN_GSEC_SHAPE_D& OutData, BOOL bDesign=TRUE, BOOL bAfter=TRUE);
protected:		
	// Data Convert
	BOOL ConvertToDLL_Polygon(T_GSEC_POLYGON& InData, DGNDRAW_GSEC_POLYGON& OutData);
	BOOL ConvertToDLL_Polygon(T_GSEC_POLYGON& InData, DGN_GSEC_POLYGON& OutData);
	BOOL ConvertToDLL_Line(T_GSEC_LINE& InData, DGNDRAW_GSEC_LINE& OutData);	
	BOOL ConvertToDLL_Line(T_GSEC_LINE& InData, DGN_GSEC_LINE& OutData);	
	BOOL ConvertToDLL_RcBeamData      (T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_RCHK_D& RbarData, DGNDRAW_SECT_BEAM&   OutData);
	BOOL ConvertToDLL_RcBeamData      (T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_CPBE_D& RbarData, DGNDRAW_SECT_BEAM&   OutData);
	BOOL ConvertToDLL_RcColumnData    (T_SECT_SECTBASE_D& SectData,                               T_RCHK_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcColumnData    (T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_CPCO_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_RBTE_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_REBT_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcWallData      (T_SECT_SECTBASE_D& SectData, int nIMJ, T_RCWA_D& WallData, T_CPWA_D& RbarData, DGNDRAW_SECT_WALL&   OutData);
	BOOL ConvertToDLL_RcPlateData     (T_THIK_D&          ThikData,                               _DGNC_CPPL_D& RbarData, DGNDRAW_SECT_PLATE&  OutData);
	CString ConvertToDLL_ShapeName(CString Shape);
	int  ConvertToDLL_LengthUnitID(int UnitID);
	int  ConvertToDLL_Stype(int nStype);
	void DoOffsetPolygon(DGN_GSEC_POLYGON_LIST& aGsecPolygon, const double dX, const double dY);

};	

#include "HeaderPost.h"      

#endif // !defined(AFX_DGNDRAWSECTION_H__8549B2FC_4EC1_4685_BF5E_82968917B95C__INCLUDED_)
