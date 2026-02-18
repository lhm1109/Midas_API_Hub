// Dgn_DrawManager.h: interface for the CDgn_DrawManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_DRAWMANAGER_H__)
#define __DGN_DRAWMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\dgnengine\src\DgnBase\DgnBaseShape_Struct.h"
#include "..\dgnengine\src\DgnDrawBase\DgnDrawBase_SectionFunc_Struct.h"
#include "..\dgnengine\src\DgnDrawBase\DgnDrawBase_EtcDraw_Struct.h"
#include "..\dgnengine\src\CVL_DgnDraw_Ex\DgnDraw_Ex_Struct.h"
#include "..\dgnengine\src\DgnDrawBase\DgnDrawBase_SoilColumn_Struct.h"

#include "Dgn_DllManager_Base.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgn_DrawManager : public CDgn_DllManager_Base      
{
public:
	CDgn_DrawManager();
	virtual ~CDgn_DrawManager();

public:	
	// GEN,CVL 공통항목 (수정시 XXX_DgnDraw_Ex에 공통작업) //////////////////
	BOOL GGet_SectionBoundary(DGN_GSEC_SHAPE_D& SectData, double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	BOOL GDraw_Section(CDC* pDC, DGNDRAW_SECT& SectData, DGNDRAW_SECT_OPTION& pSectOption);
	BOOL GDraw_Section(CDC* pDC, DGN_GSEC_SHAPE_D& SectData, DGNDRAW_SECT_OPTION& pSectOption, BOOL& bVersionOK);


	// Civil 전용 함수 //////////////////

	BOOL Draw_Section(CDC* pDC, DGNDRAW_SECT& SectData, DGNDRAW_EX_SECT_OPTION& pSectOption);

	BOOL Draw_Section_Beam  (CDC* pDC, DGNDRAW_SECT_BEAM&   SectData, DGNDRAW_EX_RCSECT_OPTION& pSectOption);
	BOOL Draw_Section_Column(CDC* pDC, DGNDRAW_SECT_COLUMN& SectData, DGNDRAW_EX_RCSECT_OPTION& pSectOption);
	BOOL Draw_Section_Wall  (CDC* pDC, DGNDRAW_SECT_WALL&   SectData, DGNDRAW_EX_RCSECT_OPTION& pSectOption);
	BOOL Draw_Section_Plate (CDC* pDC, DGNDRAW_SECT_PLATE&  SectData, DGNDRAW_EX_RCSECT_OPTION& pSectOption);
	
	BOOL Draw_Chart_MPhi    (CDC* pDC, DGNDRAW_MPHI_CHART_DATA&   MPhiData,   DGNDRAW_MPHI_PDELTA_CHART_OPTION& MPhiOption);
	BOOL Draw_Chart_PDelta  (CDC* pDC, DGNDRAW_PDELTA_CHART_DATA& PDeltaData, DGNDRAW_MPHI_PDELTA_CHART_OPTION& PDeltaOption);
	BOOL Draw_Chart_MPhi_Multi(CDC* pDC, DGNDRAW_MPHI_MULTI_CHART_DATA& MPhiData, DGNDRAW_MPHI_MULTI_CHART_OPTION& MPhiOption);
		
	// GEN 전용 함수 //////////////////
	// ...

	//iGen
	BOOL Draw_SoilColumn(CDC* pDC, DGNDRAW_GRND_SECT& SoilData, DGNDRAW_FOUN_SECT& FounData, DGNDRAW_SECT_OPTION& SoilOption, BOOL DrawPlate, DGNDRAW_GRND_STR_MSG* pStrRes);
	BOOL Draw_SoilColumn_Pile(CDC* pDC, DGNDRAW_GRND_SECT& GrndData, DGNDRAW_PILE_SECT& PileData, DGNDRAW_FOUN_SECT& FounData, DGNDRAW_SECT_OPTION& SoilOption, DGNDRAW_GRND_STR_MSG* pStrRes);
	BOOL Draw_PileArrange(CDC* pDC, DGNDRAW_PILE_ARRANGE& PileArrData, DGNDRAW_SECT_OPTION& PileOption);
	//DawEmf
	BOOL Draw_PileArrange_Emf(CDC* pDC, DGNDRAW_PILE_ARRANGE& PileArrData, DGNDRAW_SECT_OPTION& PileOption, CString* pStrPilePath);
	BOOL Draw_SoilColumn_Emf(CDC* pDC, DGNDRAW_GRND_SECT& SoilData, DGNDRAW_FOUN_SECT& FounData, DGNDRAW_SECT_OPTION& SoilOption, BOOL bDrawPlate, DGNDRAW_GRND_STR_MSG* pStrRes, CString* pStrPilePath);
	BOOL Draw_SoilColumn_Pile_Emf(CDC* pDC, DGNDRAW_GRND_SECT& GrndData, DGNDRAW_PILE_SECT& PileData, DGNDRAW_FOUN_SECT& FounData, DGNDRAW_SECT_OPTION& SoilOption, DGNDRAW_GRND_STR_MSG* pStrRes, CString* pStrPilePath);

public:	
	// Data Convert
	BOOL ConvertToDLL_SectDrawData(T_SECT_SECTBASE_D& InData, DGNDRAW_SECT& OutData, BOOL bDesign=TRUE);	
	BOOL ConvertToDLL_DrawLine(T_GSEC_LINE& InData, DGNDRAW_GSEC_LINE& OutData);	
	BOOL ConvertToDLL_DrawPolygon(T_GSEC_POLYGON& InData, DGNDRAW_GSEC_POLYGON& OutData);
	BOOL ConvertToDLL_RcBeamData      (T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_RCHK_D& RbarData, DGNDRAW_SECT_BEAM&   OutData);
	BOOL ConvertToDLL_RcBeamData      (T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_CPBE_D& RbarData, DGNDRAW_SECT_BEAM&   OutData);
	BOOL ConvertToDLL_RcColumnData    (T_SECT_SECTBASE_D& SectData,                               T_RCHK_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcColumnData    (T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_CPCO_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_RBTE_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ,                     T_REBT_D& RbarData, DGNDRAW_SECT_COLUMN& OutData);
	BOOL ConvertToDLL_RcWallData      (T_SECT_SECTBASE_D& SectData, int nIMJ, T_RCWA_D& WallData, T_CPWA_D& RbarData, DGNDRAW_SECT_WALL&   OutData);
	BOOL ConvertToDLL_RcPlateData     (T_THIK_D&          ThikData,                               _DGNC_CPPL_D& RbarData, DGNDRAW_SECT_PLATE&  OutData);
	
	BOOL ConvertToDrawDLL_DrawPolygon(DGN_GSEC_POLYGON_LIST& InData, CArray<DGNDRAW_GSEC_POLYGON, DGNDRAW_GSEC_POLYGON&>& OutData);
	BOOL ConvertToDrawDLL_DrawPolygon(DGN_GSEC_POLYGON& InData, DGNDRAW_GSEC_POLYGON& OutData);
};

#include "HeaderPost.h"

#endif // !defined(__DGN_DRAWMANAGER_H__)
