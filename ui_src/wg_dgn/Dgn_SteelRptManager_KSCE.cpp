#include "stdafx.h"

#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
#include "..\dgnengine\src\CVL_DgnReport_KR\DgnExcel_KR_STL_XLStruct.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"

#include "Dgn_SteelRptManager_KSCE.h"
#include "DgnEngineProgressDlg.h"

CDgn_SteelRptManager_KSCE::CDgn_SteelRptManager_KSCE()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_RpasD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRpas(m_RpasD);

	m_DstlD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDstl(m_DstlD);
}

CDgn_SteelRptManager_KSCE::~CDgn_SteelRptManager_KSCE()
{

}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_StlBeamCol_KCI(UMD_STL_KR_D& UstlD, DGNEXCEL_KR_STL_SECT& rXlSect)
{
	rXlSect.Initialize();

	// 연계정보!!
	rXlSect.StlBaseData.strName = UstlD.strName;
	rXlSect.StlBaseData.nRatingPosi = UstlD.nRatingPosi;
	rXlSect.StlBaseData.bConnect = UstlD.bConnect;

	// Add ElemID & PositionInfo!
	rXlSect.StlBaseData.ElemID = UstlD.ElemID;
	rXlSect.StlBaseData.nPosiDgn = UstlD.nPosiDgn;

	// Sect Data!!
	ConvertToDLL_STL_SECT(UstlD.Sect, rXlSect.StlBaseData.Sect);
	// Matl Data!!
	ConvertToDLL_STL_MATL(UstlD.Matl, rXlSect.StlBaseData.Matl);
	// Load Data!!
	ConvertToDLL_STL_LOAD(UstlD.Load, rXlSect.StlBaseData.Load);
	// Option Data!!
	ConvertToDLL_STL_OPTION(UstlD.Option, rXlSect.StlBaseData.Option);

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_StlBeamCol_Option(const int nDgnCode, const STL_KR_UMDC_STLOPTION& UcopStlData, DGNEXCEL_STL_KR_OPTION& ExlOptData)
{
	ExlOptData.strDgnCode = UcopStlData.strDgnCode;
	ExlOptData.strMatlCode= UcopStlData.strMatlCode;

	switch (nDgnCode)
	{
	case STL_KSCE_LSD15:
		ExlOptData.bPrintChk[0] = m_RpasD.bPrintParm[0];
		ExlOptData.bPrintChk[1] = FALSE;
		ExlOptData.bPrintChk[2] = FALSE;
		ExlOptData.bPrintChk[3] = FALSE;
		ExlOptData.bPrintChk[4] = m_RpasD.bPrintParm[4];
		break;

	case JROAD_H14:
	case JROAD_H24:
	case JROAD_H29:
	case JROAD_R07:
		for (int i = 0; i < 5; ++i)
		{
			ExlOptData.bPrintChk[i] = m_DstlD.bPrintChk[i];
		}
		break;

	default:
		for (int i = 1; i < 5; ++i)
		{
			ExlOptData.bPrintChk[i] = m_RpasD.bPrintParm[i - 1];
		}
		break;
	}

	
	// 0=Tens-Axial-Yield, 1=Tens-Axial-Crack, 2=Comp-Axial, 3=Bending, 4=Shear
	ExlOptData.dPhi[0] = 0.95; 
	ExlOptData.dPhi[1] = 0.8 ;  
	ExlOptData.dPhi[2] = 0.9 ;  
	ExlOptData.dPhi[3] = 1.0 ; 
	ExlOptData.dPhi[4] = 1.0 ; 
	
	ExlOptData.dPhi_STL[0] = 0.9;      // 0=Comp-Axial, 1=Bending, 2=Shear
	ExlOptData.dPhi_STL[1] = 0.9;
	ExlOptData.dPhi_STL[2] = 0.9;
	
	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_StlBeamCol_DgnResD(const STL_KR_RES_D& ResD, DGNEXCEL_STL_RES_D& rXlResD)
{
	ConvertToDLL_STL_StedBase(ResD.StedBaseD, rXlResD.StedBaseD);
	ConvertToDLL_STL_StdrList(ResD.arStdrData, rXlResD.arStdrData);
	ConvertToDLL_STL_StdrBase(ResD.StdrBaseMaxRat, rXlResD.StdrBaseMaxRat);
	ConvertToDLL_STL_SdcdBase(ResD.SdcdBaseD, rXlResD.SdcdBaseD);
	ConvertToDLL_STL_SdrdBase(ResD.MaxSdrdBaseD, rXlResD.MaxSdrdBaseD);

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SECT(STL_KR_SECT& UstlSect, DGNEXCEL_STL_KR_SECT& ExlSect)
{
	ConvertToDLL_STL_SECT_DATA(UstlSect, ExlSect);
	ConvertToDll_STL_SECT_STFND(UstlSect.StfnD, ExlSect.StfnD);
	ConvertToDll_STL_SECT_STIFF(UstlSect.Stiffness, ExlSect.Stiffness);
	ConvertToDll_STL_SECT_STIFF(UstlSect.EffeStiffness, ExlSect.EffeStiffness);

	ConvertToDLL_STL_SECT_AOuterPolyList(UstlSect.aOuterPolygon, ExlSect.aOuterPolygon);
	ConvertToDLL_STL_SECT_ALineList(UstlSect.aLine, ExlSect.aLine);
	ConvertToDLL_STL_SECT_ALineLoopList(UstlSect.aLineLoop, ExlSect.aLineLoop);
	ConvertToDLL_STL_SECT_AShapeList(UstlSect.aShape, ExlSect.aShape);

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SECT_DATA(STL_KR_SECT& UstlSect, DGNEXCEL_STL_KR_SECT& ExlSect)
{
	ExlSect.Shape         = UstlSect.Shape;
	ExlSect.iShapeType    = UstlSect.iShapeType;
	ExlSect.iSectOpt      = UstlSect.iSectOpt;
	ExlSect.iSectDBIndex  = UstlSect.iSectDBIndex;
	
	int i=0;
	for(i=0; i<8; ++i)  ExlSect.Size[i]       = UstlSect.Size[i];
	for(i=0; i<4; ++i)  ExlSect.ShapeIndex[i] = UstlSect.ShapeIndex[i];
	for(i=0; i<4; ++i)  ExlSect.LdgnIndex[i]  = UstlSect.LdgnIndex[i]; 
	
	//(2009.07.25) Add by Unsang :: ADD Rail-Code
	ExlSect.iShapeInfo    = UstlSect.iShapeInfo;
	ExlSect.iGenSectType  = UstlSect.iGenSectType;
	ExlSect.bRatio_Afg_Afn= UstlSect.bRatio_Afg_Afn;
	for(i=0; i<5; ++i)  ExlSect.dAf_gn_rat[i] = UstlSect.dAf_gn_rat[i];
	
	//
	ExlSect.dL_Buckling   = UstlSect.dL_Buckling;
	ExlSect.db_Star       = UstlSect.db_Star;
	ExlSect.dAlpha        = UstlSect.dAlpha;
	ExlSect.dBeta         = UstlSect.dBeta;
	
	ExlSect.NetAreaD = UstlSect.NetAreaD;
	ExlSect.bUpperAllowableStress4LocalBuckling = UstlSect.bUpperAllowableStress4LocalBuckling;
	ExlSect.bUpperAllowableStress4BendingCompression = UstlSect.bUpperAllowableStress4BendingCompression;
	ExlSect.bGusset = UstlSect.bGusset;
	ExlSect.bGussetPlaneLocalY = UstlSect.bGussetPlaneLocalY;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDll_STL_SECT_STFND(STL_KR_STFN& UstlD, DGNEXCEL_STL_STFN& ExlD)
{
	ExlD.Astf = UstlD.Astf;
	ExlD.Bstf = UstlD.Bstf;
	ExlD.Tstf = UstlD.Tstf;
	ExlD.Noy  = UstlD.Noy;
	ExlD.Noz  = UstlD.Noz;
	
	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDll_STL_SECT_STIFF(STL_KR_SECT_STIFFNESS& UstlD, DGNEXCEL_STL_SECT_STIFFNESS& ExlD)
{
	ExlD.Area   = UstlD.Area;
	ExlD.Asy    = UstlD.Asy;
	ExlD.Asz    = UstlD.Asz;
	ExlD.Rxx    = UstlD.Rxx;
	ExlD.Ryy    = UstlD.Ryy;
	ExlD.Rzz    = UstlD.Rzz;
	ExlD.Cyp    = UstlD.Cyp;
	ExlD.Cym    = UstlD.Cym;
	ExlD.Czp    = UstlD.Czp;
	ExlD.Czm    = UstlD.Czm;
	ExlD.Qyb    = UstlD.Qyb;
	ExlD.Qzb    = UstlD.Qzb;
	ExlD.WArea  = UstlD.WArea;
	ExlD.dx1    = UstlD.dx1;
	ExlD.dx2    = UstlD.dx2;
	ExlD.dx3    = UstlD.dx3;
	ExlD.dx4    = UstlD.dx4;
	ExlD.dy1    = UstlD.dy1;
	ExlD.dy2    = UstlD.dy2;
	ExlD.dy3    = UstlD.dy3;
	ExlD.dy4    = UstlD.dy4;
	
	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SECT_AOuterPolyList(const CArray<DGN_GSEC_POLYGON, DGN_GSEC_POLYGON&>& arUstlList, CArray<DGNEXCEL_KR_GSEC_POLYGON_STL, DGNEXCEL_KR_GSEC_POLYGON_STL&>& arExlList)
{
	int nCountOuterPolygon = 0;
	DGNEXCEL_KR_GSEC_POLYGON_STL  UnitPoly; UnitPoly.Initialize();
	
	Get_OuterPolygon_ArraySize(arUstlList, nCountOuterPolygon);
	Set_OuterPolygon_ArraySize(nCountOuterPolygon, arExlList);
	
	int i=0;
	for(i=0; i<nCountOuterPolygon; ++i)
	{
		ConvertToDLL_STL_GSEC_Polygon(arUstlList.GetAt(i), UnitPoly); arExlList.SetAt(i, UnitPoly);
	}
	
	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SECT_ALineList(const CArray<DGN_GSEC_LINE, DGN_GSEC_LINE&>& arUstlList, CArray<DGNEXCEL_STL_GSEC_LINE, DGNEXCEL_STL_GSEC_LINE&>& arExlList)
{
	int nCountaLineList = 0;
	DGNEXCEL_STL_GSEC_LINE UnitLineD; UnitLineD.Initialize();
	
	Get_ALineList_ArraySize(arUstlList, nCountaLineList);
	Set_ALineList_ArraySize(nCountaLineList, arExlList);
	
	int i=0;
	for(i=0; i<nCountaLineList; ++i)
	{
		ConvertToDLL_STL_SECT_ALineListD(arUstlList.GetAt(i), UnitLineD);  arExlList.SetAt(i, UnitLineD);
	}
	
	return TRUE;
}

void CDgn_SteelRptManager_KSCE::Get_ALineList_ArraySize(const CArray<DGN_GSEC_LINE, DGN_GSEC_LINE&>& arUstlLineList, int& nCountaLineList)
{
	nCountaLineList = arUstlLineList.GetSize();
}

void CDgn_SteelRptManager_KSCE::Set_ALineList_ArraySize(int nCountaLineList, CArray<DGNEXCEL_STL_GSEC_LINE, DGNEXCEL_STL_GSEC_LINE&>& arExlLineList)
{
	arExlLineList.SetSize(nCountaLineList);
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SECT_ALineListD(const DGN_GSEC_LINE& UstlGsecD, DGNEXCEL_STL_GSEC_LINE& ExlGsecD)
{
	ExlGsecD.v1      = UstlGsecD.v1;
	ExlGsecD.v2      = UstlGsecD.v2;
	ExlGsecD.dThik   = UstlGsecD.dThik;
	ExlGsecD.nAlign  = UstlGsecD.nAlign;
	ExlGsecD.dA      = UstlGsecD.dA;
	ExlGsecD.dVF     = UstlGsecD.dVF;
	
	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SECT_ALineLoopList(const CArray<int, int>& arUstlList, CArray<int, int>& arExlList)
{
	int nCount = 0;
	int iUnitD = 0;
	
	Get_LineLoopList_ArraySize(arUstlList, nCount);
	Set_LineLoopList_ArraySize(nCount, arExlList);
	
	int i=0;
	for(i=0; i<nCount; i++)
	{
		ConvertToDLL_STL_SECT_LineLoopList(arUstlList.GetAt(i), iUnitD); arExlList.SetAt(i, iUnitD);
	}
	
	return TRUE;
}

void CDgn_SteelRptManager_KSCE::Get_OuterPolygon_ArraySize(const CArray<DGN_GSEC_POLYGON, DGN_GSEC_POLYGON&>& arUstlList, int& nCountOuterPolygon)
{
	nCountOuterPolygon = arUstlList.GetSize();
}

void CDgn_SteelRptManager_KSCE::Set_OuterPolygon_ArraySize(int nCountOuterPolygon, CArray<DGNEXCEL_KR_GSEC_POLYGON_STL, DGNEXCEL_KR_GSEC_POLYGON_STL&>& arExlList)
{
	arExlList.SetSize(nCountOuterPolygon);
}

void CDgn_SteelRptManager_KSCE::Get_LineLoopList_ArraySize(const CArray<int, int>& arUstlList, int& nCount)
{
	nCount = arUstlList.GetSize();
}

void CDgn_SteelRptManager_KSCE::Set_LineLoopList_ArraySize(int nCount, CArray<int, int>& arExlList)
{
	arExlList.SetSize(nCount);
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SECT_LineLoopList(const int& iUstlD, int& iExlD)
{
	iExlD = iUstlD;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_GSEC_Polygon(const DGN_GSEC_POLYGON& UstlPolygonD, DGNEXCEL_KR_GSEC_POLYGON_STL& ExlPolygonD)
{
	int nCountGsecVertex = 0;
	DGNEXCEL_KR_GSEC_VERTEX_STL UnitVertex; UnitVertex.Initialize();

	Get_GsecVertex_ArraySize(UstlPolygonD.aVertex, nCountGsecVertex);
	Set_GsecVertex_ArraySize(nCountGsecVertex, ExlPolygonD.aVertex);

	int i = 0;
	for (i = 0; i < nCountGsecVertex; ++i)
	{
		ConvertToDLL_STL_GSEC_VERTEX(UstlPolygonD.aVertex.GetAt(i), UnitVertex);
		ExlPolygonD.aVertex.SetAt(i, UnitVertex);
	}

	return TRUE;
}

void CDgn_SteelRptManager_KSCE::Get_GsecVertex_ArraySize(const CArray<DGN_GSEC_VERTEX, DGN_GSEC_VERTEX&>& arUstlVertexList, int& nCountGsecList)
{
	nCountGsecList = arUstlVertexList.GetSize();
}

void CDgn_SteelRptManager_KSCE::Set_GsecVertex_ArraySize(int nCountGsecList, CArray<DGNEXCEL_KR_GSEC_VERTEX_STL, DGNEXCEL_KR_GSEC_VERTEX_STL&>& arExlVertexList)
{
	arExlVertexList.SetSize(nCountGsecList);
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_GSEC_VERTEX(const DGN_GSEC_VERTEX& UstlGsecD, DGNEXCEL_KR_GSEC_VERTEX_STL& ExlGsecD)
{
	ExlGsecD.dX = UstlGsecD.dX;
	ExlGsecD.dY = UstlGsecD.dY;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SECT_AShapeList(const CArray<STL_KR_GSEC_LSHAP,STL_KR_GSEC_LSHAP&>& arUstlList, CArray<DGNEXCEL_STL_GSEC_LSHAP,DGNEXCEL_STL_GSEC_LSHAP&>& arExlList)
{
	int nCountaShape = 0;
	DGNEXCEL_STL_GSEC_LSHAP UnitShapeD; UnitShapeD.Initialize();

	Get_ShapeList_ArraySize(arUstlList, nCountaShape);
	Set_ShapeList_ArraySize(nCountaShape, arExlList);

	for(int i=0; i<nCountaShape; ++i)
	{
		ConvertToDLL_STL_GSEC_ShapeList(arUstlList.GetAt(i), UnitShapeD);  arExlList.SetAt(i, UnitShapeD);
	}

	return TRUE;
}

void CDgn_SteelRptManager_KSCE::Get_ShapeList_ArraySize(const CArray<STL_KR_GSEC_LSHAP, STL_KR_GSEC_LSHAP&>& arUstlList, int& nCountaShape)
{
	nCountaShape = arUstlList.GetSize();
}

void CDgn_SteelRptManager_KSCE::Set_ShapeList_ArraySize(int nCountaShape, CArray<DGNEXCEL_STL_GSEC_LSHAP, DGNEXCEL_STL_GSEC_LSHAP&>& arExlList)
{
	arExlList.SetSize(nCountaShape);
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_GSEC_ShapeList(const STL_KR_GSEC_LSHAP& UstlShapeD, DGNEXCEL_STL_GSEC_LSHAP& ExlShapeD)
{
	ConvertToDLL_STL_GSEC_ShapeD(UstlShapeD, ExlShapeD);
	ConvertToDLL_STL_GSEC_LDGNList(UstlShapeD.aLdgn, ExlShapeD.aLdgn);

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_GSEC_ShapeD(const STL_KR_GSEC_LSHAP& UstlShapeD, DGNEXCEL_STL_GSEC_LSHAP& ExlShapeD)
{
	ExlShapeD.sName   = UstlShapeD.sName;
	ExlShapeD.iShape  = UstlShapeD.iShape;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_GSEC_LDGNList(const CArray<STL_KR_GSEC_LDGN,STL_KR_GSEC_LDGN&>& arUstlLdgnList, CArray<DGNEXCEL_STL_GSEC_LDGN, DGNEXCEL_STL_GSEC_LDGN&>& arExlLdgnList)
{
	int nCountLdgnList = 0;
	DGNEXCEL_STL_GSEC_LDGN UnitLdgnD; UnitLdgnD.Initialize();

	Get_LdgnList_ArraySize(arUstlLdgnList, nCountLdgnList);
	Set_LdgnList_ArraySize(nCountLdgnList, arExlLdgnList);
	
	int i=0;
	for(i=0; i<nCountLdgnList; ++i)
	{
		ConvertToDLL_STL_GSEC_LdgnListD(arUstlLdgnList.GetAt(i), UnitLdgnD);  arExlLdgnList.SetAt(i, UnitLdgnD);
	}

	return TRUE;
}

void CDgn_SteelRptManager_KSCE::Get_LdgnList_ArraySize(const CArray<STL_KR_GSEC_LDGN,STL_KR_GSEC_LDGN&>& arUstlLdgnList, int& nCountLdgnList)
{
	nCountLdgnList = arUstlLdgnList.GetSize();
}

void CDgn_SteelRptManager_KSCE::Set_LdgnList_ArraySize(int nCountLdgnList, CArray<DGNEXCEL_STL_GSEC_LDGN, DGNEXCEL_STL_GSEC_LDGN&>& arExlLdgnList)
{
	arExlLdgnList.SetSize(nCountLdgnList);
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_GSEC_LdgnListD(const STL_KR_GSEC_LDGN& UstlLdgnD, DGNEXCEL_STL_GSEC_LDGN& ExlLdgnD)
{
	ConvertToDLL_STL_GSEC_LdgnUnitD(UstlLdgnD, ExlLdgnD);
	ConvertToDLL_STL_SECT_ALineList(UstlLdgnD.aLine, ExlLdgnD.aLine);

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_GSEC_LdgnUnitD(const STL_KR_GSEC_LDGN& UstlLdgnD, DGNEXCEL_STL_GSEC_LDGN& ExlLdgnD)
{
	ExlLdgnD.sName      = UstlLdgnD.sName;
	ExlLdgnD.iBoundary  = UstlLdgnD.iBoundary;
	ExlLdgnD.db         = UstlLdgnD.db;
	ExlLdgnD.dt         = UstlLdgnD.dt;
	ExlLdgnD.iPanNum    = UstlLdgnD.iPanNum;
	ExlLdgnD.dy1        = UstlLdgnD.dy1;
	ExlLdgnD.dz1        = UstlLdgnD.dz1;
	ExlLdgnD.dy2        = UstlLdgnD.dy2;
	ExlLdgnD.dz2        = UstlLdgnD.dz2;
	ExlLdgnD.bExcept    = UstlLdgnD.bExcept;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_MATL(const STL_KR_MATL& UstlMatlD, DGNEXCEL_STL_KR_MATL& ExlMatlD)
{
	ConvertToDLL_STL_StlMatlD(UstlMatlD.StlMatL, ExlMatlD.StlMatL);
	//
	ExlMatlD.bUseFactor = UstlMatlD.bUseFactor;
	ExlMatlD.dMagnFactor = UstlMatlD.dMagnFactor;
	ExlMatlD.bAlwStressWOThickness4H = UstlMatlD.bAlwStressWOThickness4H;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StlMatlD(const STL_KR_STL_MATL& UstlStlMatlD, DGNEXCEL_STL_MATL& ExlStlMatlD)
{
	ConvertToDLL_STL_StkUnitMatlD(UstlStlMatlD, ExlStlMatlD);
	ConvertToDLL_STL_StlUnitMatl(UstlStlMatlD.Steel, ExlStlMatlD.Steel);

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StkUnitMatlD(const STL_KR_STL_MATL& UstlStlMatlD, DGNEXCEL_STL_MATL& ExlStlMatlD)
{
	ExlStlMatlD.CodeName = UstlStlMatlD.CodeName;
	ExlStlMatlD.MatlName = UstlStlMatlD.MatlName;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StlUnitMatl(const STL_KR_MATL_STEEL& UstlUnitStlMatlD, DGNEXCEL_MATL_STEEL& ExlUnitStlMatlD)
{
	ExlUnitStlMatlD.Elast = UstlUnitStlMatlD.Elast;
	ExlUnitStlMatlD.Poisson = UstlUnitStlMatlD.Poisson;
	ExlUnitStlMatlD.Thermal = UstlUnitStlMatlD.Thermal;
	ExlUnitStlMatlD.Density = UstlUnitStlMatlD.Density;
	ExlUnitStlMatlD.MassDensity = UstlUnitStlMatlD.MassDensity;
	ExlUnitStlMatlD.S_Fu = UstlUnitStlMatlD.S_Fu;
	ExlUnitStlMatlD.S_Fy = UstlUnitStlMatlD.S_Fy;
	ExlUnitStlMatlD.S_Aft = UstlUnitStlMatlD.S_Aft;
	ExlUnitStlMatlD.S_Aft2 = UstlUnitStlMatlD.S_Aft2;
	ExlUnitStlMatlD.S_Aft3 = UstlUnitStlMatlD.S_Aft3;
	ExlUnitStlMatlD.S_Afv = UstlUnitStlMatlD.S_Afv;
	ExlUnitStlMatlD.S_Afv2 = UstlUnitStlMatlD.S_Afv2;
	ExlUnitStlMatlD.S_Afv3 = UstlUnitStlMatlD.S_Afv3;

	return TRUE;
}


BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_LOAD(const STL_KR_LOAD& UstlLoadD, DGNEXCEL_STL_KR_LOAD& ExlLoadD)
{
	int nCountLcomList = 0;
	DGNEXCEL_STL_UNITLCOM UnitLcom; UnitLcom.Initialize();

	// Default!
	Get_LcomList_ArraySize(UstlLoadD.LcomList, nCountLcomList);
	Set_LcomList_ArraySize(nCountLcomList, ExlLoadD.LcomList);

	int i=0;
	for(i=0; i<nCountLcomList; ++i)
	{
		ConvertToDLL_STL_LcomList(UstlLoadD.LcomList.GetAt(i), UnitLcom); ExlLoadD.LcomList.SetAt(i, UnitLcom);
	}

	// For LRFD : DGN_KR_BRG_LSD12 : Ml관련 Array!
	int nCountLcomList2 = 0;
	UnitLcom.Initialize();
	Get_LcomList_ArraySize(UstlLoadD.Lcom2List, nCountLcomList2);
	Set_LcomList_ArraySize(nCountLcomList2, ExlLoadD.Lcom2List);

	for(i = 0 ; i<nCountLcomList2 ; ++i)
	{
		ConvertToDLL_STL_LcomList(UstlLoadD.Lcom2List.GetAt(i), UnitLcom); ExlLoadD.Lcom2List.SetAt(i, UnitLcom);
	}

	// For LRFD : DGN_KR_BRG_LSD12 : ServStateLcomList
	int nCountServLcomList = 0;
	UnitLcom.Initialize();
	Get_LcomList_ArraySize(UstlLoadD.ServStateLcomList, nCountServLcomList);
	Set_LcomList_ArraySize(nCountServLcomList, ExlLoadD.ServStateLcomList);
	
	for(i = 0 ; i<nCountServLcomList ; ++i)
	{
		ConvertToDLL_STL_LcomList(UstlLoadD.ServStateLcomList.GetAt(i), UnitLcom); ExlLoadD.ServStateLcomList.SetAt(i, UnitLcom);
	}

	// For LRFD : DGN_KR_BRG_LSD12 : FatiStateLcomList
	int nCountFatgLcomList = 0;
	UnitLcom.Initialize();
	Get_LcomList_ArraySize(UstlLoadD.FatiStateLcomList, nCountFatgLcomList);
	Set_LcomList_ArraySize(nCountFatgLcomList, ExlLoadD.FatiStateLcomList);
	
	for(i = 0 ; i<nCountFatgLcomList ; ++i)
	{
		ConvertToDLL_STL_LcomList(UstlLoadD.FatiStateLcomList.GetAt(i), UnitLcom); ExlLoadD.FatiStateLcomList.SetAt(i, UnitLcom);
	}
 
	return TRUE;
}

void CDgn_SteelRptManager_KSCE::Get_LcomList_ArraySize(const CArray<STL_KR_UNITLCOM, STL_KR_UNITLCOM&>& arUstlLcomList, int& nCountLcomList)
{
	nCountLcomList  = arUstlLcomList.GetSize();
}

void CDgn_SteelRptManager_KSCE::Set_LcomList_ArraySize(int nCountLcomList, CArray<DGNEXCEL_STL_UNITLCOM, DGNEXCEL_STL_UNITLCOM&>& arExlLcomList)
{
	arExlLcomList.SetSize(nCountLcomList);
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_LcomList(const STL_KR_UNITLCOM& UstlLcomD, DGNEXCEL_STL_UNITLCOM& ExlLcomD)
{
	ExlLcomD.bUse     = UstlLcomD.bUse;
	ExlLcomD.nType    = UstlLcomD.nType;
	ExlLcomD.strName  = UstlLcomD.strName;
	ExlLcomD.strDesc  = UstlLcomD.strDesc;
	
	int i=0;
	for(i=0; i<6; ++i)  ExlLcomD.dForce[i]  = UstlLcomD.dForce[i];

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_OPTION(const STL_KR_MEMBEROPTION& UstlOptD, DGNEXCEL_STL_KR_MEMBEROPTION& ExlOptD)
{
	int i=0;
	ExlOptD.iSelDgnLcom = UstlOptD.iSelDgnLcom;
	for(i = 0 ; i < 12 ; i++) ExlOptD.bEnvelopeForce[i] = UstlOptD.bEnvelopeForce[i];
	for(    i = 0 ; i < 5  ; i++) ExlOptD.bPrintChk[i]  = UstlOptD.bPrintChk[i];
	ExlOptD.iChkMethod  = UstlOptD.iChkMethod;
	ExlOptD.bBiaxial    = UstlOptD.bBiaxial;
	ExlOptD.iChkAxial   = UstlOptD.iChkAxial;
	ExlOptD.dLy         = UstlOptD.dLy;
	ExlOptD.dLz         = UstlOptD.dLz;
	ExlOptD.dLb         = UstlOptD.dLb;
	ExlOptD.dKy         = UstlOptD.dKy;
	ExlOptD.dKz         = UstlOptD.dKz;
	ExlOptD.bDoNotLb    = UstlOptD.bDoNotLb;

	ConvertToDLL_STL_OptEWSF(UstlOptD.Ewsf, ExlOptD.Ewsf);

	//(2009.07.25) Add by Unsang :: ADD Rail-Code
	ExlOptD.iMemberType = UstlOptD.iMemberType;
	ExlOptD.bReducingFac= UstlOptD.bReducingFac;

	// Add by Unsang :: Add LRFD
	ExlOptD.iAxialMembType  = UstlOptD.iAxialMembType;
	ExlOptD.iCalcType       = UstlOptD.iCalcType;
	ExlOptD.nShearStressCalcType   = UstlOptD.nShearStressCalcType;
	ExlOptD.nDesignForceCalcMethod = UstlOptD.nDesignForceCalcMethod;

	ExlOptD.dCb             = UstlOptD.dCb;
	ExlOptD.dU              = UstlOptD.dU;
	ExlOptD.dAn             = UstlOptD.dAn;
	ExlOptD.bAn             = UstlOptD.bAn;
	for(i = 0 ; i < 2 ; ++i)  ExlOptD.bCheckState[i] = UstlOptD.bCheckState[i];
	ExlOptD.bSlenLimit = UstlOptD.bSlenLimit;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_OptEWSF(const STL_KR_EWSF& UstlEwsf, DGNEXCEL_STL_KR_EWSF& ExlEwsf)
{
	ExlEwsf.bUseEwsf  = UstlEwsf.bUseEwsf;
	ExlEwsf.dArea_SF  = UstlEwsf.dArea_SF;
	ExlEwsf.dIxx_SF   = UstlEwsf.dIxx_SF;
	ExlEwsf.dIyy_SF   = UstlEwsf.dIyy_SF;
	ExlEwsf.dIzz_SF   = UstlEwsf.dIzz_SF;
	ExlEwsf.dCyp_SF   = UstlEwsf.dCyp_SF;
	ExlEwsf.dCym_SF   = UstlEwsf.dCym_SF;
	ExlEwsf.dCzp_SF   = UstlEwsf.dCzp_SF;
	ExlEwsf.dCzm_SF   = UstlEwsf.dCzm_SF;

	return TRUE;
}


BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBase(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ConvertToDLL_STL_StedBaseD(UstlStedBase, ExlStedBase);

	CovnertToDLL_STL_StedLindD(UstlStedBase.LData_TF, ExlStedBase.LData_TF);
	CovnertToDLL_STL_StedLindD(UstlStedBase.LData_BF, ExlStedBase.LData_BF);
	CovnertToDLL_STL_StedLindD(UstlStedBase.LData_LW, ExlStedBase.LData_LW);
	CovnertToDLL_STL_StedLindD(UstlStedBase.LData_RW, ExlStedBase.LData_RW);

	// ADD for KSCE-RAIL-ASD04
	int i=0;
	for(i=0; i<4; ++i)
	{
		ConvertToDLL_STL_StedBCASD(UstlStedBase.BendCompStrD[i], ExlStedBase.BendCompStrD[i]);
	}
	

	return TRUE;
}


BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaseD(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ConvertToDLL_STL_StedBaesD1(UstlStedBase, ExlStedBase);  // BaseDAta
	ConvertToDLL_STL_StedBaesD2(UstlStedBase, ExlStedBase);  // Section Stiffness
	ConvertToDLL_STL_StedBaesD3(UstlStedBase, ExlStedBase);  // Effective Stiffness
	ConvertToDLL_STL_StedBaesD4(UstlStedBase, ExlStedBase);  // Als Stress
	ConvertToDLL_STL_StedBaesD5(UstlStedBase, ExlStedBase);  // 표 3.3.1 허용축방향인장응력 및 허용휨인장응력/ 표 3.3.2 국부좌굴을 고려하지 않은 허용축방향압축응력
	ConvertToDLL_STL_StedBaesD6(UstlStedBase, ExlStedBase);  // Table.3.3.4(b)
	ConvertToDLL_STL_StedBaesD7(UstlStedBase, ExlStedBase);  // 허용오일러 좌굴응력
	ConvertToDLL_STL_StedBaesD8(UstlStedBase, ExlStedBase);  // shear / Buckling / 합성응력

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaesD1(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ExlStedBase.ElemK         = UstlStedBase.ElemK;
	ExlStedBase.nPosi         = UstlStedBase.nPosi;
	ExlStedBase.nSectK        = UstlStedBase.nSectK;
	ExlStedBase.nMatlK        = UstlStedBase.nMatlK;
	ExlStedBase.nMatlType     = UstlStedBase.nMatlType;
	ExlStedBase.strSectNa     = UstlStedBase.strSectNa;
	ExlStedBase.strUserSectNa = UstlStedBase.strUserSectNa;
	ExlStedBase.strMatlNa     = UstlStedBase.strMatlNa;
	ExlStedBase.iSectType     = UstlStedBase.iSectType;
    ExlStedBase.iGenShapeType = UstlStedBase.iGenShapeType;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaesD2(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ExlStedBase.dArea = UstlStedBase.dArea; 
	ExlStedBase.dIyy  = UstlStedBase.dIyy;
	ExlStedBase.dIzz  = UstlStedBase.dIzz;
	ExlStedBase.dIxx  = UstlStedBase.dIxx;
	ExlStedBase.dy1   = UstlStedBase.dy1;
	ExlStedBase.dy2   = UstlStedBase.dy2;
	ExlStedBase.dy3   = UstlStedBase.dy3;
	ExlStedBase.dy4   = UstlStedBase.dy4;
	ExlStedBase.dz1   = UstlStedBase.dz1;
	ExlStedBase.dz2   = UstlStedBase.dz2;
	ExlStedBase.dz3   = UstlStedBase.dz3;
	ExlStedBase.dz4   = UstlStedBase.dz4;
	ExlStedBase.drop  = UstlStedBase.drop;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaesD3(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ExlStedBase.dArea_eff = UstlStedBase.dArea_eff;
	ExlStedBase.dIyy_eff  = UstlStedBase.dIyy_eff;
	ExlStedBase.dIzz_eff  = UstlStedBase.dIzz_eff;
	ExlStedBase.dIxx_eff  = UstlStedBase.dIxx_eff;
	ExlStedBase.dy1_eff   = UstlStedBase.dy1_eff;
	ExlStedBase.dy2_eff   = UstlStedBase.dy2_eff;
	ExlStedBase.dy3_eff   = UstlStedBase.dy3_eff;
	ExlStedBase.dy4_eff   = UstlStedBase.dy4_eff;
	ExlStedBase.dz1_eff   = UstlStedBase.dz1_eff;
	ExlStedBase.dz2_eff   = UstlStedBase.dz2_eff;
	ExlStedBase.dz3_eff   = UstlStedBase.dz3_eff;
	ExlStedBase.dz4_eff   = UstlStedBase.dz4_eff;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaesD4(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ExlStedBase.dLen          = UstlStedBase.dLen;
	ExlStedBase.dLy           = UstlStedBase.dLy;
	ExlStedBase.dLz           = UstlStedBase.dLz;
	ExlStedBase.dLu           = UstlStedBase.dLu;
	ExlStedBase.dKy           = UstlStedBase.dKy;
	ExlStedBase.dKz           = UstlStedBase.dKz;
	ExlStedBase.dry           = UstlStedBase.dry;
	ExlStedBase.drz           = UstlStedBase.drz;
	ExlStedBase.dCb           = UstlStedBase.dCb;
	ExlStedBase.dCmy          = UstlStedBase.dCmy;
	ExlStedBase.dCmz          = UstlStedBase.dCmz;
	ExlStedBase.dSlenValueY   = UstlStedBase.dSlenValueY;
	ExlStedBase.dSlenValueZ   = UstlStedBase.dSlenValueZ;
	ExlStedBase.dSlenValueMax = UstlStedBase.dSlenValueMax;
	ExlStedBase.dSlenValueP   = UstlStedBase.dSlenValueP;
	ExlStedBase.bChkSlender   = UstlStedBase.bChkSlender;

	int i=0;
	for(i=0; i<2; ++i)
	{
		ExlStedBase.dSlenValue[i] = UstlStedBase.dSlenValue[i];
		ExlStedBase.dSlenLimit[i] = UstlStedBase.dSlenLimit[i];
	}

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaesD5(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ExlStedBase.dftaTF    = UstlStedBase.dftaTF;
	ExlStedBase.dftaBF    = UstlStedBase.dftaBF;
	ExlStedBase.dftaLW    = UstlStedBase.dftaLW;
	ExlStedBase.dftaRW    = UstlStedBase.dftaRW;
	ExlStedBase.dftaTL    = UstlStedBase.dftaTL;
	ExlStedBase.dftaTR    = UstlStedBase.dftaTR;
	ExlStedBase.dftaBR    = UstlStedBase.dftaBR;
	ExlStedBase.dftaBL    = UstlStedBase.dftaBL;
	ExlStedBase.dfcagyTF  = UstlStedBase.dfcagyTF;
	ExlStedBase.dfcagyBF  = UstlStedBase.dfcagyBF;
	ExlStedBase.dfcagyLW  = UstlStedBase.dfcagyLW;
	ExlStedBase.dfcagyRW  = UstlStedBase.dfcagyRW;
	ExlStedBase.dfcagzTF  = UstlStedBase.dfcagzTF;
	ExlStedBase.dfcagzBF  = UstlStedBase.dfcagzBF;
	ExlStedBase.dfcagzLW  = UstlStedBase.dfcagzLW;
	ExlStedBase.dfcagzRW  = UstlStedBase.dfcagzRW;
	ExlStedBase.dfcagyTL  = UstlStedBase.dfcagyTL;
	ExlStedBase.dfcagyTR  = UstlStedBase.dfcagyTR;
	ExlStedBase.dfcagyBR  = UstlStedBase.dfcagyBR;
	ExlStedBase.dfcagyBL  = UstlStedBase.dfcagyBL;
	ExlStedBase.dfcagzTL  = UstlStedBase.dfcagzTL;
	ExlStedBase.dfcagzTR  = UstlStedBase.dfcagzTR;
	ExlStedBase.dfcagzBR  = UstlStedBase.dfcagzBR;
	ExlStedBase.dfcagzBL  = UstlStedBase.dfcagzBL;
	ExlStedBase.dfcaoTF   = UstlStedBase.dfcaoTF;
	ExlStedBase.dfcaoBF   = UstlStedBase.dfcaoBF;
	ExlStedBase.dfcaoLW   = UstlStedBase.dfcaoLW;
	ExlStedBase.dfcaoRW   = UstlStedBase.dfcaoRW;
	ExlStedBase.dfcaoTL   = UstlStedBase.dfcaoTL;
	ExlStedBase.dfcaoTR   = UstlStedBase.dfcaoTR;
	ExlStedBase.dfcaoBR   = UstlStedBase.dfcaoBR;
	ExlStedBase.dfcaoBL   = UstlStedBase.dfcaoBL;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaesD6(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ExlStedBase.dAw_334_T   = UstlStedBase.dAw_334_T;
	ExlStedBase.dAw_334_B   = UstlStedBase.dAw_334_B;
	ExlStedBase.dAw_334_L   = UstlStedBase.dAw_334_L;
	ExlStedBase.dAw_334_R   = UstlStedBase.dAw_334_R;
	ExlStedBase.dAc_334_T   = UstlStedBase.dAc_334_T;
	ExlStedBase.dAc_334_B   = UstlStedBase.dAc_334_B;
	ExlStedBase.dAc_334_L   = UstlStedBase.dAc_334_L;
	ExlStedBase.dAc_334_R   = UstlStedBase.dAc_334_R;
	ExlStedBase.db_334_T    = UstlStedBase.db_334_T;
	ExlStedBase.db_334_B    = UstlStedBase.db_334_B;
	ExlStedBase.db_334_L    = UstlStedBase.db_334_L;
	ExlStedBase.db_334_R    = UstlStedBase.db_334_R;
	ExlStedBase.dK_334_T    = UstlStedBase.dK_334_T;
	ExlStedBase.dK_334_B    = UstlStedBase.dK_334_B;
	ExlStedBase.dK_334_L    = UstlStedBase.dK_334_L;
	ExlStedBase.dK_334_R    = UstlStedBase.dK_334_R;
	ExlStedBase.dSlen_334_T = UstlStedBase.dSlen_334_T;
	ExlStedBase.dSlen_334_B = UstlStedBase.dSlen_334_B;
	ExlStedBase.dSlen_334_L = UstlStedBase.dSlen_334_L;
	ExlStedBase.dSlen_334_R = UstlStedBase.dSlen_334_R;
	ExlStedBase.dfbagyTF    = UstlStedBase.dfbagyTF;
	ExlStedBase.dfbagyBF    = UstlStedBase.dfbagyBF;
	ExlStedBase.dfbagzLW    = UstlStedBase.dfbagzLW;
	ExlStedBase.dfbagzRW    = UstlStedBase.dfbagzRW;
	ExlStedBase.dfbagyTL    = UstlStedBase.dfbagyTL;
	ExlStedBase.dfbagyTR    = UstlStedBase.dfbagyTR;
	ExlStedBase.dfbagyBR    = UstlStedBase.dfbagyBR;
	ExlStedBase.dfbagyBL    = UstlStedBase.dfbagyBL;
	ExlStedBase.dfbagzTL    = UstlStedBase.dfbagzTL;
	ExlStedBase.dfbagzTR    = UstlStedBase.dfbagzTR;
	ExlStedBase.dfbagzBR    = UstlStedBase.dfbagzBR;
	ExlStedBase.dfbagzBL    = UstlStedBase.dfbagzBL;
	ExlStedBase.dfbaoTF     = UstlStedBase.dfbaoTF;
	ExlStedBase.dfbaoBF     = UstlStedBase.dfbaoBF;
	ExlStedBase.dfbaoLW     = UstlStedBase.dfbaoLW;
	ExlStedBase.dfbaoRW     = UstlStedBase.dfbaoRW;  
	ExlStedBase.dfbaoTL     = UstlStedBase.dfbaoTL;
	ExlStedBase.dfbaoTR     = UstlStedBase.dfbaoTR;
	ExlStedBase.dfbaoBR     = UstlStedBase.dfbaoBR;
	ExlStedBase.dfbaoBL     = UstlStedBase.dfbaoBL;  

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaesD7(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ExlStedBase.df_Ey = UstlStedBase.df_Ey;
	ExlStedBase.df_Ez = UstlStedBase.df_Ez;
	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBaesD8(const STL_KR_RES_STED_BASE& UstlStedBase, DGNEXCEL_STL_RES_STED_BASE& ExlStedBase)
{
	ExlStedBase.dAsy              = UstlStedBase.dAsy;
	ExlStedBase.dAsz              = UstlStedBase.dAsz;
	ExlStedBase.dr_y              = UstlStedBase.dr_y;
	ExlStedBase.dr_z              = UstlStedBase.dr_z;
	ExlStedBase.dvay              = UstlStedBase.dvay;
	ExlStedBase.dvaz              = UstlStedBase.dvaz;
	ExlStedBase.dvaTF             = UstlStedBase.dvaTF;
	ExlStedBase.dvaBF             = UstlStedBase.dvaBF;
	ExlStedBase.dvaLW             = UstlStedBase.dvaLW;
	ExlStedBase.dvaRW             = UstlStedBase.dvaRW;
	ExlStedBase.dBuckRatioLimit   = UstlStedBase.dBuckRatioLimit;
	ExlStedBase.dCombStressLimit  = UstlStedBase.dCombStressLimit;
	
	return TRUE;
}


BOOL CDgn_SteelRptManager_KSCE::CovnertToDLL_STL_StedLindD(const STL_KR_RES_STED_LDATA& UstlLineD, DGNEXCEL_STL_RES_STED_LDATA& ExlLineD)
{
	ExlLineD.bPanel           = UstlLineD.bPanel;
	ExlLineD.in               = UstlLineD.in;
	ExlLineD.dCant1_b         = UstlLineD.dCant1_b;
	ExlLineD.dBothFix_b       = UstlLineD.dBothFix_b;
	ExlLineD.dCant2_b         = UstlLineD.dCant2_b;
	ExlLineD.dCant1_t         = UstlLineD.dCant1_t;
	ExlLineD.dBothFix_t       = UstlLineD.dBothFix_t;
	ExlLineD.dCant2_t         = UstlLineD.dCant2_t;
	ExlLineD.bCant1           = UstlLineD.bCant1;
	ExlLineD.bBothFix         = UstlLineD.bBothFix;
	ExlLineD.bCant2           = UstlLineD.bCant2;
	ExlLineD.dfcal1           = UstlLineD.dfcal1;
	ExlLineD.dfcal2           = UstlLineD.dfcal2;
	ExlLineD.dCant1_t_min     = UstlLineD.dCant1_t_min;
	ExlLineD.dCant2_t_min     = UstlLineD.dCant2_t_min;
	ExlLineD.bChk_Cant1_t_min = UstlLineD.bChk_Cant1_t_min;
	ExlLineD.bChk_Cant2_t_min = UstlLineD.bChk_Cant2_t_min;
	ExlLineD.db_sum           = UstlLineD.db_sum;
	ExlLineD.dt_max           = UstlLineD.dt_max;
	ExlLineD.dbt_s_cant1      = UstlLineD.dbt_s_cant1  ;
	ExlLineD.dbt_s_cant2      = UstlLineD.dbt_s_cant2  ;
	ExlLineD.dbt_s_BF         = UstlLineD.dbt_s_BF     ;
	ExlLineD.dbt_lim_cant1    = UstlLineD.dbt_lim_cant1;
	ExlLineD.dbt_lim_cant2    = UstlLineD.dbt_lim_cant2;
	ExlLineD.dbt_lim_BF       = UstlLineD.dbt_lim_BF   ;
	ExlLineD.bChk_bt_cant1    = UstlLineD.bChk_bt_cant1;
	ExlLineD.bChk_bt_cant2    = UstlLineD.bChk_bt_cant2;
	ExlLineD.bChk_bt_BF       = UstlLineD.bChk_bt_BF   ;  
	ExlLineD.dfcao_BA_cant1   = UstlLineD.dfcao_BA_cant1;
	ExlLineD.dfcao_BA_cant2   = UstlLineD.dfcao_BA_cant2;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StedBCASD(const STL_KR_RES_STED_BCAS_D& UstlStedBcasD, DGNEXCEL_STL_RES_STED_BCAS_D& ExlStedBcasD)
{
	ExlStedBcasD.db_comp  = UstlStedBcasD.db_comp;
	ExlStedBcasD.dalpha   = UstlStedBcasD.dalpha;
	ExlStedBcasD.dbeta    = UstlStedBcasD.dbeta;
	ExlStedBcasD.dbeta0   = UstlStedBcasD.dbeta0;
	ExlStedBcasD.dF       = UstlStedBcasD.dF;
	ExlStedBcasD.iF_type  = UstlStedBcasD.iF_type;
	ExlStedBcasD.dl_eq    = UstlStedBcasD.dl_eq;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrList(const CArray<STL_KR_RES_STDR_BASE,STL_KR_RES_STDR_BASE&>& arUstlStdrList, CArray<DGNEXCEL_STL_RES_STDR_BASE,DGNEXCEL_STL_RES_STDR_BASE&>& arExlStdrList)
{
	int nCountStdrList = 0;
	DGNEXCEL_STL_RES_STDR_BASE UnitStdr; UnitStdr.Initialize();
	
	Get_StdrList_ArraySize(arUstlStdrList, nCountStdrList);
	Set_StdrList_ArraySize(nCountStdrList, arExlStdrList);
	
	for(int i=0; i<nCountStdrList; ++i)
	{
		ConvertToDLL_STL_StdrBase(arUstlStdrList.GetAt(i), UnitStdr);  arExlStdrList.SetAt(i, UnitStdr);
	}
	
	return TRUE;
}

void CDgn_SteelRptManager_KSCE::Get_StdrList_ArraySize(const CArray<STL_KR_RES_STDR_BASE, STL_KR_RES_STDR_BASE&>& arUstlStdrList, int& nCountStdrList)
{
	nCountStdrList = arUstlStdrList.GetSize();
}

void CDgn_SteelRptManager_KSCE::Set_StdrList_ArraySize(int nCountStdrList, CArray<DGNEXCEL_STL_RES_STDR_BASE, DGNEXCEL_STL_RES_STDR_BASE&>& arExlStdrList)
{
	arExlStdrList.SetSize(nCountStdrList);
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBase(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ConvertToDLL_STL_StdrBaseD(UstlStdrBaseD, ExlStdrBaseD);

	ConvertToDLL_STL_StdrLineD(UstlStdrBaseD.TopBoth, ExlStdrBaseD.TopBoth);
	ConvertToDLL_STL_StdrLineD(UstlStdrBaseD.BotBoth, ExlStdrBaseD.BotBoth);
	ConvertToDLL_STL_StdrLineD(UstlStdrBaseD.LeftBoth, ExlStdrBaseD.LeftBoth);
	ConvertToDLL_STL_StdrLineD(UstlStdrBaseD.RightBoth, ExlStdrBaseD.RightBoth);

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrLineD(const STL_KR_RES_STDR_BF_DATA& UstlStdrUnitD, DGNEXCEL_STL_RES_STDR_BF_DATA& ExlStdrUnitD)
{
	ExlStdrUnitD.bCheck        = UstlStdrUnitD.bCheck      ;
	ExlStdrUnitD.bChk_t_min    = UstlStdrUnitD.bChk_t_min  ;
	ExlStdrUnitD.df1           = UstlStdrUnitD.df1         ;
	ExlStdrUnitD.df2           = UstlStdrUnitD.df2         ; 
	ExlStdrUnitD.dphi          = UstlStdrUnitD.dphi        ;
	ExlStdrUnitD.di            = UstlStdrUnitD.di          ;      
	ExlStdrUnitD.dfcal         = UstlStdrUnitD.dfcal       ;   
	ExlStdrUnitD.dt_min        = UstlStdrUnitD.dt_min      ;   
	ExlStdrUnitD.dalpha        = UstlStdrUnitD.dalpha      ;
	ExlStdrUnitD.iCType_cant1  = UstlStdrUnitD.iCType_cant1;
	ExlStdrUnitD.iCType_cant2  = UstlStdrUnitD.iCType_cant2;
	ExlStdrUnitD.iCType_BF     = UstlStdrUnitD.iCType_BF   ;
	ExlStdrUnitD.bCant1_comp   = UstlStdrUnitD.bCant1_comp ;
	ExlStdrUnitD.bCant2_comp   = UstlStdrUnitD.bCant2_comp ;
	//
	ExlStdrUnitD.dPsi          = UstlStdrUnitD.dPsi;
	ExlStdrUnitD.df1_cant1     = UstlStdrUnitD.df1_cant1  ;
	ExlStdrUnitD.df2_cant1     = UstlStdrUnitD.df2_cant1  ;
	ExlStdrUnitD.df1_cant2     = UstlStdrUnitD.df1_cant2  ;
	ExlStdrUnitD.df2_cant2     = UstlStdrUnitD.df2_cant2  ;
	ExlStdrUnitD.dbt0_lim_AF   = UstlStdrUnitD.dbt0_lim_AF;
	ExlStdrUnitD.bBT0_Check_AF = UstlStdrUnitD.bBT0_Check_AF;
	//
	ExlStdrUnitD.dfcao_BA_BF   = UstlStdrUnitD.dfcao_BA_BF;
	//
	ConvertToDLL_STL_StdrLine_Rail_D(UstlStdrUnitD.Rail04yD, ExlStdrUnitD.Rail04yD);
	ConvertToDLL_STL_StdrLine_Rail_D(UstlStdrUnitD.Rail04zD, ExlStdrUnitD.Rail04zD);
	//
	ExlStdrUnitD.bConsiderMagn_cant1 = UstlStdrUnitD.bConsiderMagn_cant1;
	ExlStdrUnitD.bConsiderMagn_cant2 = UstlStdrUnitD.bConsiderMagn_cant2;
	ExlStdrUnitD.bConsiderMagn_BF    = UstlStdrUnitD.bConsiderMagn_BF   ;


	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SdcdBase(const STL_KR_RES_SDCD_BASE& UstlSdcdBase, DGNEXCEL_STL_RES_SDCD_BASE& ExlSdcdBase)
{
	ExlSdcdBase.dStress             = UstlSdcdBase.dStress;
	ExlSdcdBase.dAlwStress          = UstlSdcdBase.dAlwStress;
	ExlSdcdBase.dShearStress        = UstlSdcdBase.dShearStress;
	ExlSdcdBase.dAlwShearStress     = UstlSdcdBase.dAlwShearStress;
	ExlSdcdBase.dBuckRatio          = UstlSdcdBase.dBuckRatio;
	ExlSdcdBase.dAlwBuckRatio       = UstlSdcdBase.dAlwBuckRatio;
	ExlSdcdBase.dCombStressRatio    = UstlSdcdBase.dCombStressRatio;
	ExlSdcdBase.dAlwCombStressRatio = UstlSdcdBase.dAlwCombStressRatio;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_SdrdBase(const STL_KR_RES_SDRD_BASE& UstlSdrdBase, DGNEXCEL_STL_RES_SDRD_BASE& ExlSdrdBase)
{
	ExlSdrdBase.bCheck              = UstlSdrdBase.bCheck;
	ExlSdrdBase.dShrStressY         = UstlSdrdBase.dShrStressY;
	ExlSdrdBase.dShrStressZ         = UstlSdrdBase.dShrStressZ;
	ExlSdrdBase.dAlwShrStressY      = UstlSdrdBase.dAlwShrStressY;
	ExlSdrdBase.dAlwShrStressZ      = UstlSdrdBase.dAlwShrStressZ;
	ExlSdrdBase.dCompStressRatio    = UstlSdrdBase.dCompStressRatio;
	ExlSdrdBase.dAlwCompStressRatio = UstlSdrdBase.dAlwCompStressRatio;
	ExlSdrdBase.dAlwBuckRatio       = UstlSdrdBase.dAlwBuckRatio;

	int i=0;
	for(i=0; i<5; ++i)
	{
		ExlSdrdBase.dTensStress[i]    = UstlSdrdBase.dTensStress[i];
		ExlSdrdBase.dCompStress[i]    = UstlSdrdBase.dCompStress[i];
		ExlSdrdBase.dAlwTensStress[i] = UstlSdrdBase.dAlwTensStress[i];
		ExlSdrdBase.dAlwCompStress[i] = UstlSdrdBase.dAlwCompStress[i];
		ExlSdrdBase.dBuckRatioY[i]    = UstlSdrdBase.dBuckRatioY[i];
		ExlSdrdBase.dBuckRatioZ[i]    = UstlSdrdBase.dBuckRatioZ[i];
	}

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ConvertToDLL_STL_StdrBaseD1(UstlStdrBaseD, ExlStdrBaseD);
	ConvertToDLL_STL_StdrBaseD2(UstlStdrBaseD, ExlStdrBaseD);
	ConvertToDLL_STL_StdrBaseD3(UstlStdrBaseD, ExlStdrBaseD);
	ConvertToDLL_STL_StdrBaseD4(UstlStdrBaseD, ExlStdrBaseD);
	ConvertToDLL_STL_StdrBaseD5(UstlStdrBaseD, ExlStdrBaseD);
	ConvertToDLL_STL_StdrBaseD6(UstlStdrBaseD, ExlStdrBaseD);
	ConvertToDLL_STL_StdrBaseD7(UstlStdrBaseD, ExlStdrBaseD);
	ConvertToDLL_STL_StdrBaseD8(UstlStdrBaseD, ExlStdrBaseD);
	ConvertToDLL_STL_StdrBaseD9(UstlStdrBaseD, ExlStdrBaseD);

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD1(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.bCheck        = UstlStdrBaseD.bCheck;
	ExlStdrBaseD.iChkResult    = UstlStdrBaseD.iChkResult;
	ExlStdrBaseD.ElemK         = UstlStdrBaseD.ElemK;
	ExlStdrBaseD.nPosi         = UstlStdrBaseD.nPosi;
	ExlStdrBaseD.nOrgLcomK     = UstlStdrBaseD.nOrgLcomK;
	ExlStdrBaseD.nDgnLcomK     = UstlStdrBaseD.nDgnLcomK;
	ExlStdrBaseD.nOrgDgnLcomK  = UstlStdrBaseD.nOrgDgnLcomK;
	ExlStdrBaseD.iMax          = UstlStdrBaseD.iMax;
	
	CString strFind = _T("(");
	CStringW strWF(strFind);
	int nPosi = UstlStdrBaseD.strLcomNa.Find(strWF);
	if (nPosi != -1)
		ExlStdrBaseD.strLcomNa = UstlStdrBaseD.strLcomNa.Left(nPosi);
	else
		ExlStdrBaseD.strLcomNa     = UstlStdrBaseD.strLcomNa;
	ExlStdrBaseD.dFx           = UstlStdrBaseD.dFx;
	ExlStdrBaseD.dFy           = UstlStdrBaseD.dFy;
	ExlStdrBaseD.dFz           = UstlStdrBaseD.dFz;
	ExlStdrBaseD.dMx           = UstlStdrBaseD.dMx;
	ExlStdrBaseD.dMy           = UstlStdrBaseD.dMy;
	ExlStdrBaseD.dMz           = UstlStdrBaseD.dMz;
	ExlStdrBaseD.dMup          = UstlStdrBaseD.dMup;
	ExlStdrBaseD.dFx_org       = UstlStdrBaseD.dFx_org;
	ExlStdrBaseD.dFy_org       = UstlStdrBaseD.dFy_org;
	ExlStdrBaseD.dFz_org       = UstlStdrBaseD.dFz_org;
	ExlStdrBaseD.dMx_org       = UstlStdrBaseD.dMx_org;
	ExlStdrBaseD.dMy_org       = UstlStdrBaseD.dMy_org;
	ExlStdrBaseD.dMz_org       = UstlStdrBaseD.dMz_org;
	ExlStdrBaseD.dSlenLimit    = UstlStdrBaseD.dSlenLimit;
	ExlStdrBaseD.bChekSlen     = UstlStdrBaseD.bChekSlen;
	ExlStdrBaseD.dLKy          = UstlStdrBaseD.dLKy;
	ExlStdrBaseD.dLKz          = UstlStdrBaseD.dLKz;
	ExlStdrBaseD.dSlenValueY   = UstlStdrBaseD.dSlenValueY;
	ExlStdrBaseD.dSlenValueZ   = UstlStdrBaseD.dSlenValueZ;
	ExlStdrBaseD.dSlenValueMax = UstlStdrBaseD.dSlenValueMax;
	ExlStdrBaseD.dSlenValueP   = UstlStdrBaseD.dSlenValueP;

	
	int i=0;
	for(i=0; i<4; ++i)  ExlStdrBaseD.bChkAlwValue[i] = UstlStdrBaseD.bChkAlwValue[i];
	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD2(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.bWebCompL       = UstlStdrBaseD.bWebCompL;
	ExlStdrBaseD.bWebCompR       = UstlStdrBaseD.bWebCompR;
	ExlStdrBaseD.bFlgCompTL      = UstlStdrBaseD.bFlgCompTL;
	ExlStdrBaseD.bFlgCompTR      = UstlStdrBaseD.bFlgCompTR;
	ExlStdrBaseD.bFlgCompBR      = UstlStdrBaseD.bFlgCompBR;
	ExlStdrBaseD.bFlgCompBL      = UstlStdrBaseD.bFlgCompBL;     
	ExlStdrBaseD.bFlgCompT       = UstlStdrBaseD.bFlgCompT;
	ExlStdrBaseD.bFlgCompB       = UstlStdrBaseD.bFlgCompB;
	ExlStdrBaseD.dAlwFlexCompYT  = UstlStdrBaseD.dAlwFlexCompYT;
	ExlStdrBaseD.dAlwFlexCompYB  = UstlStdrBaseD.dAlwFlexCompYB;
	ExlStdrBaseD.dAlwFlexCompZL  = UstlStdrBaseD.dAlwFlexCompZL;
	ExlStdrBaseD.dAlwFlexCompZR  = UstlStdrBaseD.dAlwFlexCompZR;
	ExlStdrBaseD.dAlwW           = UstlStdrBaseD.dAlwW;
	ExlStdrBaseD.dAlwCW_L        = UstlStdrBaseD.dAlwCW_L;
	ExlStdrBaseD.dAlwCW_R        = UstlStdrBaseD.dAlwCW_R;
	ExlStdrBaseD.dAlwTW_L        = UstlStdrBaseD.dAlwTW_L;
	ExlStdrBaseD.dAlwTW_R        = UstlStdrBaseD.dAlwTW_R;
	ExlStdrBaseD.dAlwCF_TL       = UstlStdrBaseD.dAlwCF_TL;
	ExlStdrBaseD.dAlwCF_TR       = UstlStdrBaseD.dAlwCF_TR;
	ExlStdrBaseD.dAlwCF_BR       = UstlStdrBaseD.dAlwCF_BR;
	ExlStdrBaseD.dAlwCF_BL       = UstlStdrBaseD.dAlwCF_BL;
	ExlStdrBaseD.dAlwCF_T        = UstlStdrBaseD.dAlwCF_T;
	ExlStdrBaseD.dAlwCF_B        = UstlStdrBaseD.dAlwCF_B;
	ExlStdrBaseD.dAlwTF_TL       = UstlStdrBaseD.dAlwTF_TL;
	ExlStdrBaseD.dAlwTF_TR       = UstlStdrBaseD.dAlwTF_TR;
	ExlStdrBaseD.dAlwTF_BR       = UstlStdrBaseD.dAlwTF_BR;
	ExlStdrBaseD.dAlwTF_BL       = UstlStdrBaseD.dAlwTF_BL;
	ExlStdrBaseD.dAlwTF_T        = UstlStdrBaseD.dAlwTF_T;
	ExlStdrBaseD.dAlwTF_B        = UstlStdrBaseD.dAlwTF_B;
	ExlStdrBaseD.dAlwCW_Min      = UstlStdrBaseD.dAlwCW_Min;
	ExlStdrBaseD.dAlwCF_Min      = UstlStdrBaseD.dAlwCF_Min;
	ExlStdrBaseD.iMinPosi_CW     = UstlStdrBaseD.iMinPosi_CW;
	ExlStdrBaseD.iMinPosi_CF     = UstlStdrBaseD.iMinPosi_CF;
	ExlStdrBaseD.iMinPosi        = UstlStdrBaseD.iMinPosi;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD3(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.dfcalTF   = UstlStdrBaseD.dfcalTF;
	ExlStdrBaseD.dfcalBF   = UstlStdrBaseD.dfcalBF;
	ExlStdrBaseD.dfcalLW   = UstlStdrBaseD.dfcalLW;
	ExlStdrBaseD.dfcalRW   = UstlStdrBaseD.dfcalRW;
	ExlStdrBaseD.dfcalTL   = UstlStdrBaseD.dfcalTL;
	ExlStdrBaseD.dfcalTR   = UstlStdrBaseD.dfcalTR;
	ExlStdrBaseD.dfcalBR   = UstlStdrBaseD.dfcalBR;
	ExlStdrBaseD.dfcalBL   = UstlStdrBaseD.dfcalBL;
	ExlStdrBaseD.dfcayTF   = UstlStdrBaseD.dfcayTF;
	ExlStdrBaseD.dfcayBF   = UstlStdrBaseD.dfcayBF;
	ExlStdrBaseD.dfcayLW   = UstlStdrBaseD.dfcayLW;
	ExlStdrBaseD.dfcayRW   = UstlStdrBaseD.dfcayRW;
	ExlStdrBaseD.dfcazTF   = UstlStdrBaseD.dfcazTF;
	ExlStdrBaseD.dfcazBF   = UstlStdrBaseD.dfcazBF;
	ExlStdrBaseD.dfcazLW   = UstlStdrBaseD.dfcazLW;
	ExlStdrBaseD.dfcazRW   = UstlStdrBaseD.dfcazRW;
	ExlStdrBaseD.dfcayTL   = UstlStdrBaseD.dfcayTL;
	ExlStdrBaseD.dfcayTR   = UstlStdrBaseD.dfcayTR;
	ExlStdrBaseD.dfcayBR   = UstlStdrBaseD.dfcayBR;
	ExlStdrBaseD.dfcayBL   = UstlStdrBaseD.dfcayBL;
	ExlStdrBaseD.dfcazTL   = UstlStdrBaseD.dfcazTL;
	ExlStdrBaseD.dfcazTR   = UstlStdrBaseD.dfcazTR;
	ExlStdrBaseD.dfcazBR   = UstlStdrBaseD.dfcazBR;
	ExlStdrBaseD.dfcazBL   = UstlStdrBaseD.dfcazBL;
	ExlStdrBaseD.dfcal_min = UstlStdrBaseD.dfcal_min;
	//
	ExlStdrBaseD.dfcayTF_Beam = UstlStdrBaseD.dfcayTF_Beam;
	ExlStdrBaseD.dfcayBF_Beam = UstlStdrBaseD.dfcayBF_Beam;
	ExlStdrBaseD.dfcayLW_Beam = UstlStdrBaseD.dfcayLW_Beam;
	ExlStdrBaseD.dfcayRW_Beam = UstlStdrBaseD.dfcayRW_Beam;
	ExlStdrBaseD.dfcazTF_Beam = UstlStdrBaseD.dfcazTF_Beam;
	ExlStdrBaseD.dfcazBF_Beam = UstlStdrBaseD.dfcazBF_Beam;
	ExlStdrBaseD.dfcazLW_Beam = UstlStdrBaseD.dfcazLW_Beam;
	ExlStdrBaseD.dfcazRW_Beam = UstlStdrBaseD.dfcazRW_Beam;
	ExlStdrBaseD.dfcayTL_Beam = UstlStdrBaseD.dfcayTL_Beam;
	ExlStdrBaseD.dfcayTR_Beam = UstlStdrBaseD.dfcayTR_Beam;
	ExlStdrBaseD.dfcayBR_Beam = UstlStdrBaseD.dfcayBR_Beam;
	ExlStdrBaseD.dfcayBL_Beam = UstlStdrBaseD.dfcayBL_Beam;
	ExlStdrBaseD.dfcazTL_Beam = UstlStdrBaseD.dfcazTL_Beam;
	ExlStdrBaseD.dfcazTR_Beam = UstlStdrBaseD.dfcazTR_Beam;
	ExlStdrBaseD.dfcazBR_Beam = UstlStdrBaseD.dfcazBR_Beam;
	ExlStdrBaseD.dfcazBL_Beam = UstlStdrBaseD.dfcazBL_Beam;
	//
	ExlStdrBaseD.dfcaoTF_Beam = UstlStdrBaseD.dfcaoTF_Beam;
	ExlStdrBaseD.dfcaoBF_Beam = UstlStdrBaseD.dfcaoBF_Beam;
	ExlStdrBaseD.dfcaoLW_Beam = UstlStdrBaseD.dfcaoLW_Beam;
	ExlStdrBaseD.dfcaoRW_Beam = UstlStdrBaseD.dfcaoRW_Beam;
	ExlStdrBaseD.dfcaoTL_Beam = UstlStdrBaseD.dfcaoTL_Beam;
	ExlStdrBaseD.dfcaoTR_Beam = UstlStdrBaseD.dfcaoTR_Beam;
	ExlStdrBaseD.dfcaoBR_Beam = UstlStdrBaseD.dfcaoBR_Beam;
	ExlStdrBaseD.dfcaoBL_Beam = UstlStdrBaseD.dfcaoBL_Beam;

	int i=0;
	for(i=0; i<4; ++i)
	{
		ExlStdrBaseD.ifcalShapeCase[i]  = UstlStdrBaseD.ifcalShapeCase[i];
		ExlStdrBaseD.ifcalPosiCase[i]   = UstlStdrBaseD.ifcalPosiCase[i];

		// ADD for KSCE-RAIL-ASD04
		ExlStdrBaseD.iModifyStress[i]   = UstlStdrBaseD.iModifyStress[i];
	}

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD4(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.dfcaTL       = UstlStdrBaseD.dfcaTL;
	ExlStdrBaseD.dfcaTR       = UstlStdrBaseD.dfcaTR;
	ExlStdrBaseD.dfcaBR       = UstlStdrBaseD.dfcaBR;
	ExlStdrBaseD.dfcaBL       = UstlStdrBaseD.dfcaBL;
	ExlStdrBaseD.dTens348TL   = UstlStdrBaseD.dTens348TL;
	ExlStdrBaseD.dTens348TR   = UstlStdrBaseD.dTens348TR;
	ExlStdrBaseD.dTens348BR   = UstlStdrBaseD.dTens348BR;
	ExlStdrBaseD.dTens348BL   = UstlStdrBaseD.dTens348BL;
	ExlStdrBaseD.dTens3410TL  = UstlStdrBaseD.dTens3410TL;
	ExlStdrBaseD.dTens3410TR  = UstlStdrBaseD.dTens3410TR;
	ExlStdrBaseD.dTens3410BR  = UstlStdrBaseD.dTens3410BR;
	ExlStdrBaseD.dTens3410BL  = UstlStdrBaseD.dTens3410BL;
	ExlStdrBaseD.dTRat348TL   = UstlStdrBaseD.dTRat348TL;
	ExlStdrBaseD.dTRat348TR   = UstlStdrBaseD.dTRat348TR;
	ExlStdrBaseD.dTRat348BR   = UstlStdrBaseD.dTRat348BR;
	ExlStdrBaseD.dTRat348BL   = UstlStdrBaseD.dTRat348BL;
	ExlStdrBaseD.dTRat349yTL  = UstlStdrBaseD.dTRat349yTL;
	ExlStdrBaseD.dTRat349yTR  = UstlStdrBaseD.dTRat349yTR;
	ExlStdrBaseD.dTRat349yBR  = UstlStdrBaseD.dTRat349yBR;
	ExlStdrBaseD.dTRat349yBL  = UstlStdrBaseD.dTRat349yBL;
	ExlStdrBaseD.dTRat349zTL  = UstlStdrBaseD.dTRat349zTL;
	ExlStdrBaseD.dTRat349zTR  = UstlStdrBaseD.dTRat349zTR;
	ExlStdrBaseD.dTRat349zBR  = UstlStdrBaseD.dTRat349zBR;
	ExlStdrBaseD.dTRat349zBL  = UstlStdrBaseD.dTRat349zBL;
	ExlStdrBaseD.dTRat3410TL  = UstlStdrBaseD.dTRat3410TL;
	ExlStdrBaseD.dTRat3410TR  = UstlStdrBaseD.dTRat3410TR;
	ExlStdrBaseD.dTRat3410BR  = UstlStdrBaseD.dTRat3410BR;
	ExlStdrBaseD.dTRat3410BL  = UstlStdrBaseD.dTRat3410BL;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD5(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.dfc           = UstlStdrBaseD.dfc;
	ExlStdrBaseD.dft           = UstlStdrBaseD.dft;
	ExlStdrBaseD.dStressTL     = UstlStdrBaseD.dStressTL;
	ExlStdrBaseD.dStressTR     = UstlStdrBaseD.dStressTR;
	ExlStdrBaseD.dStressBR     = UstlStdrBaseD.dStressBR;
	ExlStdrBaseD.dStressBL     = UstlStdrBaseD.dStressBL;

	ExlStdrBaseD.dStressTL_g     = UstlStdrBaseD.dStressTL_g;
	ExlStdrBaseD.dStressTR_g     = UstlStdrBaseD.dStressTR_g;
	ExlStdrBaseD.dStressBR_g     = UstlStdrBaseD.dStressBR_g;
	ExlStdrBaseD.dStressBL_g     = UstlStdrBaseD.dStressBL_g;
	ExlStdrBaseD.dStressTL_st     = UstlStdrBaseD.dStressTL_st;
	ExlStdrBaseD.dStressTR_st     = UstlStdrBaseD.dStressTR_st;
	ExlStdrBaseD.dStressBR_st     = UstlStdrBaseD.dStressBR_st;
	ExlStdrBaseD.dStressBL_st     = UstlStdrBaseD.dStressBL_st;
	ExlStdrBaseD.dStressTL_lt     = UstlStdrBaseD.dStressTL_lt;
	ExlStdrBaseD.dStressTR_lt     = UstlStdrBaseD.dStressTR_lt;
	ExlStdrBaseD.dStressBR_lt     = UstlStdrBaseD.dStressBR_lt;
	ExlStdrBaseD.dStressBL_lt     = UstlStdrBaseD.dStressBL_lt;

	ExlStdrBaseD.dfbcyTL       = UstlStdrBaseD.dfbcyTL;
	ExlStdrBaseD.dfbcyTR       = UstlStdrBaseD.dfbcyTR;
	ExlStdrBaseD.dfbcyBR       = UstlStdrBaseD.dfbcyBR;
	ExlStdrBaseD.dfbcyBL       = UstlStdrBaseD.dfbcyBL;
	ExlStdrBaseD.dfbczTL       = UstlStdrBaseD.dfbczTL;
	ExlStdrBaseD.dfbczTR       = UstlStdrBaseD.dfbczTR;
	ExlStdrBaseD.dfbczBR       = UstlStdrBaseD.dfbczBR;
	ExlStdrBaseD.dfbczBL       = UstlStdrBaseD.dfbczBL;
	ExlStdrBaseD.dfbtyTL       = UstlStdrBaseD.dfbtyTL;
	ExlStdrBaseD.dfbtyTR       = UstlStdrBaseD.dfbtyTR;
	ExlStdrBaseD.dfbtyBR       = UstlStdrBaseD.dfbtyBR;
	ExlStdrBaseD.dfbtyBL       = UstlStdrBaseD.dfbtyBL;
	ExlStdrBaseD.dfbtzTL       = UstlStdrBaseD.dfbtzTL;
	ExlStdrBaseD.dfbtzTR       = UstlStdrBaseD.dfbtzTR;
	ExlStdrBaseD.dfbtzBR       = UstlStdrBaseD.dfbtzBR;
	ExlStdrBaseD.dfbtzBL       = UstlStdrBaseD.dfbtzBL;
	ExlStdrBaseD.dfas          = UstlStdrBaseD.dfas;
	ExlStdrBaseD.dfbsyTL       = UstlStdrBaseD.dfbsyTL;
	ExlStdrBaseD.dfbsyTR       = UstlStdrBaseD.dfbsyTR;
	ExlStdrBaseD.dfbsyBR       = UstlStdrBaseD.dfbsyBR;
	ExlStdrBaseD.dfbsyBL       = UstlStdrBaseD.dfbsyBL;
	ExlStdrBaseD.dfbszTL       = UstlStdrBaseD.dfbszTL;
	ExlStdrBaseD.dfbszTR       = UstlStdrBaseD.dfbszTR;
	ExlStdrBaseD.dfbszBR       = UstlStdrBaseD.dfbszBR;
	ExlStdrBaseD.dfbszBL       = UstlStdrBaseD.dfbszBL;

	// ADD for KSCE-RAIL-ASD04
	ExlStdrBaseD.dSt_TL        = UstlStdrBaseD.dSt_TL;
	ExlStdrBaseD.dSt_TR        = UstlStdrBaseD.dSt_TR;
	ExlStdrBaseD.dSt_BR        = UstlStdrBaseD.dSt_BR;
	ExlStdrBaseD.dSt_BL        = UstlStdrBaseD.dSt_BL;

	ExlStdrBaseD.dfsp1         = UstlStdrBaseD.dfsp1;
	ExlStdrBaseD.dfsp2         = UstlStdrBaseD.dfsp2;
	ExlStdrBaseD.dCRat3411yTL  = UstlStdrBaseD.dCRat3411yTL;
	ExlStdrBaseD.dCRat3411yTR  = UstlStdrBaseD.dCRat3411yTR;
	ExlStdrBaseD.dCRat3411yBR  = UstlStdrBaseD.dCRat3411yBR;
	ExlStdrBaseD.dCRat3411yBL  = UstlStdrBaseD.dCRat3411yBL;
	ExlStdrBaseD.dCRat3411zTL  = UstlStdrBaseD.dCRat3411zTL;
	ExlStdrBaseD.dCRat3411zTR  = UstlStdrBaseD.dCRat3411zTR;
	ExlStdrBaseD.dCRat3411zBR  = UstlStdrBaseD.dCRat3411zBR;
	ExlStdrBaseD.dCRat3411zBL  = UstlStdrBaseD.dCRat3411zBL;
	ExlStdrBaseD.dComp3412TL   = UstlStdrBaseD.dComp3412TL;
	ExlStdrBaseD.dComp3412TR   = UstlStdrBaseD.dComp3412TR;
	ExlStdrBaseD.dComp3412BR   = UstlStdrBaseD.dComp3412BR;
	ExlStdrBaseD.dComp3412BL   = UstlStdrBaseD.dComp3412BL;
	ExlStdrBaseD.dCRat3412TL   = UstlStdrBaseD.dCRat3412TL;
	ExlStdrBaseD.dCRat3412TR   = UstlStdrBaseD.dCRat3412TR;
	ExlStdrBaseD.dCRat3412BR   = UstlStdrBaseD.dCRat3412BR;
	ExlStdrBaseD.dCRat3412BL   = UstlStdrBaseD.dCRat3412BL;
	ExlStdrBaseD.dRatComb      = UstlStdrBaseD.dRatComb;
	ExlStdrBaseD.dBuckRatio    = UstlStdrBaseD.dBuckRatio;
	ExlStdrBaseD.bChkBuckling  = UstlStdrBaseD.bChkBuckling;
	ExlStdrBaseD.dPca          = UstlStdrBaseD.dPca;
	ExlStdrBaseD.dMyao         = UstlStdrBaseD.dMyao;
	ExlStdrBaseD.dMzao         = UstlStdrBaseD.dMzao;
	ExlStdrBaseD.dfcaAC        = UstlStdrBaseD.dfcaAC;
	ExlStdrBaseD.dfcayAC       = UstlStdrBaseD.dfcayAC;
	ExlStdrBaseD.dfcazAC       = UstlStdrBaseD.dfcazAC;
	ExlStdrBaseD.dSFy          = UstlStdrBaseD.dSFy;
	ExlStdrBaseD.dSFz          = UstlStdrBaseD.dSFz;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD6(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.dMaxChkStress       = UstlStdrBaseD.dMaxChkStress;
	ExlStdrBaseD.dMaxChkAlwStress    = UstlStdrBaseD.dMaxChkAlwStress;
	ExlStdrBaseD.dMaxChkStressRat    = UstlStdrBaseD.dMaxChkStressRat;
	ExlStdrBaseD.dMaxShearStress     = UstlStdrBaseD.dMaxShearStress;
	ExlStdrBaseD.dMaxAlwShearStress  = UstlStdrBaseD.dMaxAlwShearStress;
	ExlStdrBaseD.dMaxShearStressRat  = UstlStdrBaseD.dMaxShearStressRat;
	ExlStdrBaseD.dShearStress4Comb   = UstlStdrBaseD.dShearStress4Comb;
	ExlStdrBaseD.dMaxShearStressRat4Comb = UstlStdrBaseD.dMaxShearStressRat4Comb;
	ExlStdrBaseD.dMaxBuckRatio       = UstlStdrBaseD.dMaxBuckRatio;
	ExlStdrBaseD.dMaxCombStrRatio    = UstlStdrBaseD.dMaxCombStrRatio;
	ExlStdrBaseD.dMaxCombStrRat      = UstlStdrBaseD.dMaxCombStrRat;
	ExlStdrBaseD.dCriticalRatio      = UstlStdrBaseD.dCriticalRatio;
	ExlStdrBaseD.iCriticalCase       = UstlStdrBaseD.iCriticalCase;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD7(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.dvy           = UstlStdrBaseD.dvy;
	ExlStdrBaseD.dvz           = UstlStdrBaseD.dvz;
	ExlStdrBaseD.dvy1          = UstlStdrBaseD.dvy1;
	ExlStdrBaseD.dvy2          = UstlStdrBaseD.dvy2;
	ExlStdrBaseD.dvyf2         = UstlStdrBaseD.dvyf2;
	ExlStdrBaseD.dvyw          = UstlStdrBaseD.dvyw;
	ExlStdrBaseD.dvzf          = UstlStdrBaseD.dvzf;
	ExlStdrBaseD.dvzw          = UstlStdrBaseD.dvzw;
	ExlStdrBaseD.dvzw2         = UstlStdrBaseD.dvzw2;
	ExlStdrBaseD.dvFlange      = UstlStdrBaseD.dvFlange;
	ExlStdrBaseD.dvWeb         = UstlStdrBaseD.dvWeb;
	ExlStdrBaseD.dtau_ty       = UstlStdrBaseD.dtau_ty;
	ExlStdrBaseD.dtau_tz       = UstlStdrBaseD.dtau_tz;
	ExlStdrBaseD.dtau_y        = UstlStdrBaseD.dtau_y;
	ExlStdrBaseD.dtau_z        = UstlStdrBaseD.dtau_z;
	ExlStdrBaseD.dRatShearY    = UstlStdrBaseD.dRatShearY;
	ExlStdrBaseD.dRatShearZ    = UstlStdrBaseD.dRatShearZ;
	ExlStdrBaseD.dRatShrStress = UstlStdrBaseD.dRatShrStress;
    ExlStdrBaseD.ShearStrD     = UstlStdrBaseD.ShearStrD;
	ExlStdrBaseD.dRatStrTL     = UstlStdrBaseD.dRatStrTL;
	ExlStdrBaseD.dRatStrTR     = UstlStdrBaseD.dRatStrTR;
	ExlStdrBaseD.dRatStrBR     = UstlStdrBaseD.dRatStrBR;
	ExlStdrBaseD.dRatStrBL     = UstlStdrBaseD.dRatStrBL;
	ExlStdrBaseD.iStrRatChkTL  = UstlStdrBaseD.iStrRatChkTL;
	ExlStdrBaseD.iStrRatChkTR  = UstlStdrBaseD.iStrRatChkTR;
	ExlStdrBaseD.iStrRatChkBR  = UstlStdrBaseD.iStrRatChkBR;
	ExlStdrBaseD.iStrRatChkBL  = UstlStdrBaseD.iStrRatChkBL;

	return TRUE;
}


BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD8(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.bCompStrTL     = UstlStdrBaseD.bCompStrTL;
	ExlStdrBaseD.bCompStrTR     = UstlStdrBaseD.bCompStrTR;
	ExlStdrBaseD.bCompStrBR     = UstlStdrBaseD.bCompStrBR;
	ExlStdrBaseD.bCompStrBL     = UstlStdrBaseD.bCompStrBL;
	ExlStdrBaseD.iChk348TL      = UstlStdrBaseD.iChk348TL;
	ExlStdrBaseD.iChk348TR      = UstlStdrBaseD.iChk348TR;
	ExlStdrBaseD.iChk348BR      = UstlStdrBaseD.iChk348BR;
	ExlStdrBaseD.iChk348BL      = UstlStdrBaseD.iChk348BL;
	ExlStdrBaseD.iChk349yTL     = UstlStdrBaseD.iChk349yTL;
	ExlStdrBaseD.iChk349yTR     = UstlStdrBaseD.iChk349yTR;
	ExlStdrBaseD.iChk349yBR     = UstlStdrBaseD.iChk349yBR;
	ExlStdrBaseD.iChk349yBL     = UstlStdrBaseD.iChk349yBL;
	ExlStdrBaseD.iChk349zTL     = UstlStdrBaseD.iChk349zTL;
	ExlStdrBaseD.iChk349zTR     = UstlStdrBaseD.iChk349zTR;
	ExlStdrBaseD.iChk349zBR     = UstlStdrBaseD.iChk349zBR;
	ExlStdrBaseD.iChk349zBL     = UstlStdrBaseD.iChk349zBL;
	ExlStdrBaseD.iChk3410TL     = UstlStdrBaseD.iChk3410TL;
	ExlStdrBaseD.iChk3410TR     = UstlStdrBaseD.iChk3410TR;
	ExlStdrBaseD.iChk3410BR     = UstlStdrBaseD.iChk3410BR;
	ExlStdrBaseD.iChk3410BL     = UstlStdrBaseD.iChk3410BL;
	ExlStdrBaseD.iChk3411yTL    = UstlStdrBaseD.iChk3411yTL;
	ExlStdrBaseD.iChk3411yTR    = UstlStdrBaseD.iChk3411yTR;
	ExlStdrBaseD.iChk3411yBR    = UstlStdrBaseD.iChk3411yBR;
	ExlStdrBaseD.iChk3411yBL    = UstlStdrBaseD.iChk3411yBL;
	ExlStdrBaseD.iChk3411zTL    = UstlStdrBaseD.iChk3411zTL;
	ExlStdrBaseD.iChk3411zTR    = UstlStdrBaseD.iChk3411zTR;
	ExlStdrBaseD.iChk3411zBR    = UstlStdrBaseD.iChk3411zBR;
	ExlStdrBaseD.iChk3411zBL    = UstlStdrBaseD.iChk3411zBL;
	ExlStdrBaseD.iChk3412TL     = UstlStdrBaseD.iChk3412TL;
	ExlStdrBaseD.iChk3412TR     = UstlStdrBaseD.iChk3412TR;
	ExlStdrBaseD.iChk3412BR     = UstlStdrBaseD.iChk3412BR;
	ExlStdrBaseD.iChk3412BL     = UstlStdrBaseD.iChk3412BL;
	ExlStdrBaseD.iMaxStressPosi = UstlStdrBaseD.iMaxStressPosi;
	ExlStdrBaseD.iMax3411yPosi  = UstlStdrBaseD.iMax3411yPosi;
	ExlStdrBaseD.iMax3411zPosi  = UstlStdrBaseD.iMax3411zPosi;
	ExlStdrBaseD.iMax3412Posi   = UstlStdrBaseD.iMax3412Posi;
	ExlStdrBaseD.iMax348Posi    = UstlStdrBaseD.iMax348Posi;
	ExlStdrBaseD.iMax349yPosi   = UstlStdrBaseD.iMax349yPosi;
	ExlStdrBaseD.iMax349zPosi   = UstlStdrBaseD.iMax349zPosi;
	ExlStdrBaseD.iMax3410Posi   = UstlStdrBaseD.iMax3410Posi;
	ExlStdrBaseD.iMaxShrStress  = UstlStdrBaseD.iMaxShrStress;

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrBaseD9(const STL_KR_RES_STDR_BASE& UstlStdrBaseD, DGNEXCEL_STL_RES_STDR_BASE& ExlStdrBaseD)
{
	ExlStdrBaseD.dfb             = UstlStdrBaseD.dfb;
	ExlStdrBaseD.dfa             = UstlStdrBaseD.dfa;
	ExlStdrBaseD.dvb             = UstlStdrBaseD.dvb;
	ExlStdrBaseD.dva             = UstlStdrBaseD.dva;
	ExlStdrBaseD.dRatCombStress  = UstlStdrBaseD.dRatCombStress;
	for (int i = 0; i < 4; ++i)
	{
		ExlStdrBaseD.dfb_str[i] = UstlStdrBaseD.dfb_str[i];
		ExlStdrBaseD.dCombStressRat[i] = UstlStdrBaseD.dCombStressRat[i];
	}
	for (int i = 0; i < 2; ++i)
	{
		ExlStdrBaseD.dfb_TB[i] = UstlStdrBaseD.dfb_TB[i];
		ExlStdrBaseD.dCombStressRatTB[i] = UstlStdrBaseD.dCombStressRatTB[i];
	}

	//  ADD for KSCE-RAIL-ASD04
	for(int i=0; i<4; ++i)
	{
		ExlStdrBaseD.dPcay[i] = UstlStdrBaseD.dPcay[i];
		ExlStdrBaseD.dPcaz[i] = UstlStdrBaseD.dPcaz[i];
		ExlStdrBaseD.dMya[i]  = UstlStdrBaseD.dMya[i] ;
		ExlStdrBaseD.dMza[i]  = UstlStdrBaseD.dMza[i] ;
	}

	ExlStdrBaseD.db_star     = UstlStdrBaseD.db_star;
	ExlStdrBaseD.dSlender_eq = UstlStdrBaseD.dSlender_eq;
	ExlStdrBaseD.dalpha_Eq   = UstlStdrBaseD.dalpha_Eq;
	ExlStdrBaseD.dbeta_Eq    = UstlStdrBaseD.dbeta_Eq;
	ExlStdrBaseD.dbeta0_Eq   = UstlStdrBaseD.dbeta0_Eq;
	ExlStdrBaseD.dF_Eq       = UstlStdrBaseD.dF_Eq;
	ExlStdrBaseD.RelaxedAlwStr4Pca = UstlStdrBaseD.RelaxedAlwStr4Pca;
	ExlStdrBaseD.RelaxedAlwStr4Myca = UstlStdrBaseD.RelaxedAlwStr4Myca;	 

	return TRUE;
}

BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrLine_Rail_Memb_D(const STL_STDR_KSCE_RAIL04_MEMB_D& UstlStdrRailD, DGNEXCEL_STL_RES_STDR_RAIL04_MEMB_D& ExlStdrRailD)
{
	ExlStdrRailD.dMagnFactor_cant1   = UstlStdrRailD.dMagnFactor_cant1  ;
	ExlStdrRailD.dMagnFactor_cant2   = UstlStdrRailD.dMagnFactor_cant2  ;
	ExlStdrRailD.dMagnFactor_BF      = UstlStdrRailD.dMagnFactor_BF     ;
	ExlStdrRailD.dMagnFactor_BF_AF   = UstlStdrRailD.dMagnFactor_BF_AF  ;
	ExlStdrRailD.dbt_Magn_cant1      = UstlStdrRailD.dbt_Magn_cant1     ;
	ExlStdrRailD.dbt_Magn_cant2      = UstlStdrRailD.dbt_Magn_cant2     ;
	ExlStdrRailD.dbt_Magn_BF         = UstlStdrRailD.dbt_Magn_BF        ;
	ExlStdrRailD.dbt_Magn_BF_AF      = UstlStdrRailD.dbt_Magn_BF_AF     ;
	ExlStdrRailD.dfca_cant1          = UstlStdrRailD.dfca_cant1         ;
	ExlStdrRailD.dfca_cant2          = UstlStdrRailD.dfca_cant2         ;
	ExlStdrRailD.dfca_BF             = UstlStdrRailD.dfca_BF            ;
	ExlStdrRailD.dfca_cant1_cal      = UstlStdrRailD.dfca_cant1_cal     ;
	ExlStdrRailD.dfca_cant2_cal      = UstlStdrRailD.dfca_cant2_cal     ;
	ExlStdrRailD.dfca_BF_cal         = UstlStdrRailD.dfca_BF_cal        ;  
	ExlStdrRailD.bBT_Check_cant1     = UstlStdrRailD.bBT_Check_cant1    ;
	ExlStdrRailD.bBT_Check_cant2     = UstlStdrRailD.bBT_Check_cant2    ;
	ExlStdrRailD.bBT_Check_BF        = UstlStdrRailD.bBT_Check_BF       ;
	ExlStdrRailD.bBT_Check_BF_AF     = UstlStdrRailD.bBT_Check_BF_AF    ;

	return TRUE;
}


BOOL CDgn_SteelRptManager_KSCE::ConvertToDLL_STL_StdrLine_Rail_D(const STL_STDR_KSCE_RAIL04_D& UstlStdrRailD, DGNEXCEL_STL_RES_STDR_RAIL04_D& ExlStdrRailD)
{
	ConvertToDLL_STL_StdrLine_Rail_Memb_D(UstlStdrRailD.BeamD, ExlStdrRailD.BeamD);  // Beam
	ConvertToDLL_STL_StdrLine_Rail_Memb_D(UstlStdrRailD.ColumnD, ExlStdrRailD.ColumnD);  // Column

	return TRUE;
}













