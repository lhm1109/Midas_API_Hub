#pragma once

#include "Dgn_DllManager_Base.h"

#include "HeaderPre.h"

struct UMD_STL_KR_D;
struct STL_KR_RES_D;
struct STL_KR_UMDC_STLOPTION;
struct DGNEXCEL_KR_STL_SECT;
struct DGNEXCEL_STL_KR_OPTION;
struct DGNEXCEL_STL_RES_D;

class __MY_EXT_CLASS__ CDgn_SteelRptManager_KSCE : public CDgn_DllManager_Base
{
public:
	CDgn_SteelRptManager_KSCE();
	virtual ~CDgn_SteelRptManager_KSCE();


public:
	BOOL ConvertToDLL_StlBeamCol_KCI(UMD_STL_KR_D& UstlD, DGNEXCEL_KR_STL_SECT& rXlSect);
	BOOL ConvertToDLL_StlBeamCol_Option(const int nDgnCode, const STL_KR_UMDC_STLOPTION& UcopStlData, DGNEXCEL_STL_KR_OPTION& rXlOptData);
	BOOL ConvertToDLL_StlBeamCol_DgnResD(const STL_KR_RES_D& ResD, DGNEXCEL_STL_RES_D& rXlResD);


public:
	BOOL ConvertToDLL_STL_SECT(STL_KR_SECT& UstlSect, DGNEXCEL_STL_KR_SECT& ExlSect);
	BOOL ConvertToDLL_STL_SECT_DATA(STL_KR_SECT& UstlSect, DGNEXCEL_STL_KR_SECT& ExlSect);
	BOOL ConvertToDll_STL_SECT_STFND(STL_KR_STFN& UstlSect, DGNEXCEL_STL_STFN& ExlSect);
	BOOL ConvertToDll_STL_SECT_STIFF(STL_KR_SECT_STIFFNESS& UstlD, DGNEXCEL_STL_SECT_STIFFNESS& ExlD);
	BOOL ConvertToDLL_STL_SECT_AOuterPolyList(const CArray<DGN_GSEC_POLYGON, DGN_GSEC_POLYGON&>& arUstlList, CArray<DGNEXCEL_KR_GSEC_POLYGON_STL, DGNEXCEL_KR_GSEC_POLYGON_STL&>& arExlList);
	BOOL ConvertToDLL_STL_SECT_ALineList(const CArray<DGN_GSEC_LINE, DGN_GSEC_LINE&>& arUstlList, CArray<DGNEXCEL_STL_GSEC_LINE, DGNEXCEL_STL_GSEC_LINE&>& arExlList);
	void Get_ALineList_ArraySize(const CArray<DGN_GSEC_LINE, DGN_GSEC_LINE&>& arUstlLineList, int& nCountaLineList);
	void Set_ALineList_ArraySize(int nCountaLineList, CArray<DGNEXCEL_STL_GSEC_LINE, DGNEXCEL_STL_GSEC_LINE&>& arExlLineList);
	BOOL ConvertToDLL_STL_SECT_ALineListD(const DGN_GSEC_LINE& UstlGsecD, DGNEXCEL_STL_GSEC_LINE& ExlGsecD);
	BOOL ConvertToDLL_STL_SECT_ALineLoopList(const CArray<int, int>& arUstlList, CArray<int, int>& arExlList);
	void Get_OuterPolygon_ArraySize(const CArray<DGN_GSEC_POLYGON, DGN_GSEC_POLYGON&>& arUstlList, int& nCountOuterPolygon);
	void Set_OuterPolygon_ArraySize(int nCountOuterPolygon, CArray<DGNEXCEL_KR_GSEC_POLYGON_STL, DGNEXCEL_KR_GSEC_POLYGON_STL&>& arExlList);
	void Get_LineLoopList_ArraySize(const CArray<int, int>& arUstlList, int& nCount);
	void Set_LineLoopList_ArraySize(int nCount, CArray<int, int>& arExlList);
	BOOL ConvertToDLL_STL_SECT_LineLoopList(const int& iUstlD, int& iExlD);
	BOOL ConvertToDLL_STL_GSEC_Polygon(const DGN_GSEC_POLYGON& UstlPolygonD, DGNEXCEL_KR_GSEC_POLYGON_STL& ExlPolygonD);
	void Get_GsecVertex_ArraySize(const CArray<DGN_GSEC_VERTEX, DGN_GSEC_VERTEX&>& arUstlVertexList, int& nCountGsecList);
	void Set_GsecVertex_ArraySize(int nCountGsecList, CArray<DGNEXCEL_KR_GSEC_VERTEX_STL, DGNEXCEL_KR_GSEC_VERTEX_STL&>& arExlVertexList);
	BOOL ConvertToDLL_STL_GSEC_VERTEX(const DGN_GSEC_VERTEX& UstlGsecD, DGNEXCEL_KR_GSEC_VERTEX_STL& ExlGsecD);
	BOOL ConvertToDLL_STL_SECT_AShapeList(const CArray<STL_KR_GSEC_LSHAP, STL_KR_GSEC_LSHAP&>& arUstlList, CArray<DGNEXCEL_STL_GSEC_LSHAP, DGNEXCEL_STL_GSEC_LSHAP&>& arExlList);
	void Get_ShapeList_ArraySize(const CArray<STL_KR_GSEC_LSHAP, STL_KR_GSEC_LSHAP&>& arUstlList, int& nCountaShape);
	void Set_ShapeList_ArraySize(int nCountaShape, CArray<DGNEXCEL_STL_GSEC_LSHAP, DGNEXCEL_STL_GSEC_LSHAP&>& arExlList);
	BOOL ConvertToDLL_STL_GSEC_ShapeList(const STL_KR_GSEC_LSHAP& UstlShapeD, DGNEXCEL_STL_GSEC_LSHAP& ExlShapeD);
	BOOL ConvertToDLL_STL_GSEC_ShapeD(const STL_KR_GSEC_LSHAP& UstlShapeD, DGNEXCEL_STL_GSEC_LSHAP& ExlShapeD);
	BOOL ConvertToDLL_STL_GSEC_LDGNList(const CArray<STL_KR_GSEC_LDGN, STL_KR_GSEC_LDGN&>& arUstlLdgnList, CArray<DGNEXCEL_STL_GSEC_LDGN, DGNEXCEL_STL_GSEC_LDGN&>& arExlLdgnList);
	void Get_LdgnList_ArraySize(const CArray<STL_KR_GSEC_LDGN, STL_KR_GSEC_LDGN&>& arUstlLdgnList, int& nCountLdgnList);
	void Set_LdgnList_ArraySize(int nCountLdgnList, CArray<DGNEXCEL_STL_GSEC_LDGN, DGNEXCEL_STL_GSEC_LDGN&>& arExlLdgnList);
	BOOL ConvertToDLL_STL_GSEC_LdgnListD(const STL_KR_GSEC_LDGN& UstlLdgnD, DGNEXCEL_STL_GSEC_LDGN& ExlLdgnD);
	BOOL ConvertToDLL_STL_GSEC_LdgnUnitD(const STL_KR_GSEC_LDGN& UstlLdgnD, DGNEXCEL_STL_GSEC_LDGN& ExlLdgnD);

	// Material
	BOOL ConvertToDLL_STL_MATL(const STL_KR_MATL& UstlMatlD, DGNEXCEL_STL_KR_MATL& ExlMatlD);
	BOOL ConvertToDLL_STL_StlMatlD(const STL_KR_STL_MATL& UstlMatlD, DGNEXCEL_STL_MATL& ExlMatlD);
	BOOL ConvertToDLL_STL_StkUnitMatlD(const STL_KR_STL_MATL& UstlStlMatlD, DGNEXCEL_STL_MATL& ExlStlMatlD);
	BOOL ConvertToDLL_STL_StlUnitMatl(const STL_KR_MATL_STEEL& UstlUnitStlMatlD, DGNEXCEL_MATL_STEEL& ExlUnitStlMatlD);

	// Load
	BOOL ConvertToDLL_STL_LOAD(const STL_KR_LOAD& UstlLoadD, DGNEXCEL_STL_KR_LOAD& ExlLoadD);
	void Get_LcomList_ArraySize(const CArray<STL_KR_UNITLCOM, STL_KR_UNITLCOM&>& arUstlLcomList, int& nCountLcomList);
	void Set_LcomList_ArraySize(int nCountLcomList, CArray<DGNEXCEL_STL_UNITLCOM, DGNEXCEL_STL_UNITLCOM&>& arExlLcomList);
	BOOL ConvertToDLL_STL_LcomList(const STL_KR_UNITLCOM& UstlLcomD, DGNEXCEL_STL_UNITLCOM& ExlLcomD);

	// Option
	BOOL ConvertToDLL_STL_OPTION(const STL_KR_MEMBEROPTION& UstlOptD, DGNEXCEL_STL_KR_MEMBEROPTION& ExlOptD);
	BOOL ConvertToDLL_STL_OptEWSF(const STL_KR_EWSF& UstlEwsf, DGNEXCEL_STL_KR_EWSF& ExlEwsf);


	BOOL ConvertToDLL_STL_StedBase(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaseD(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaesD1(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaesD2(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaesD3(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaesD4(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaesD5(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaesD6(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaesD7(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL ConvertToDLL_STL_StedBaesD8(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase);
	BOOL CovnertToDLL_STL_StedLindD(const STL_KR_RES_STED_LDATA& UstlLineD, DGNEXCEL_STL_RES_STED_LDATA& ExlLineD);

	BOOL ConvertToDLL_STL_StedBCASD(const STL_KR_RES_STED_BCAS_D& UstlStedBcasD, DGNEXCEL_STL_RES_STED_BCAS_D& ExlStedBcasD);

	BOOL ConvertToDLL_STL_StdrList(const CArray<STL_KR_RES_STDR_BASE, STL_KR_RES_STDR_BASE&>& arUstlStdrList, CArray<DGNEXCEL_STL_RES_STDR_BASE, DGNEXCEL_STL_RES_STDR_BASE&>& arExlStdrList);
	void Get_StdrList_ArraySize(const CArray<STL_KR_RES_STDR_BASE, STL_KR_RES_STDR_BASE&>& arUstlStdrList, int& nCountStdrList);
	void Set_StdrList_ArraySize(int nCountStdrList, CArray<DGNEXCEL_STL_RES_STDR_BASE, DGNEXCEL_STL_RES_STDR_BASE&>& arExlStdrList);
	BOOL ConvertToDLL_STL_StdrBase(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD1(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD2(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD3(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD4(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD5(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD6(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD7(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD8(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	BOOL ConvertToDLL_STL_StdrBaseD9(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD);
	
	
	BOOL ConvertToDLL_STL_StdrLine_Rail_Memb_D(const STL_STDR_KSCE_RAIL04_MEMB_D& UstlStdrRailD, DGNEXCEL_STL_RES_STDR_RAIL04_MEMB_D& ExlStdrRailD);
	BOOL ConvertToDLL_STL_StdrLine_Rail_D(const STL_STDR_KSCE_RAIL04_D& UstlStdrRailD, DGNEXCEL_STL_RES_STDR_RAIL04_D& ExlStdrRailD);
	BOOL ConvertToDLL_STL_StdrLineD(const STL_KR_RES_STDR_BF_DATA& UstlStdrUnitD, DGNEXCEL_STL_RES_STDR_BF_DATA& ExlStdrUnitD);
	BOOL ConvertToDLL_STL_SdcdBase(const STL_KR_RES_SDCD_BASE& UstlSdcdBase, DGNEXCEL_STL_RES_SDCD_BASE& ExlSdcdBase);
	BOOL ConvertToDLL_STL_SdrdBase(const STL_KR_RES_SDRD_BASE& UstlSdrdBase, DGNEXCEL_STL_RES_SDRD_BASE& ExlSdrdBase);

private:
	T_RPAS_D m_RpasD;
	T_DSTL_D m_DstlD;
};

#include "HeaderPost.h"