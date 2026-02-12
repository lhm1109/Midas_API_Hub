// Db_DllBaseManager.h: interface for the CDb_DllBaseManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_DLLBASEMANAGER_H__86EB1A15_F0F8_43A0_8F06_8D2C5DE9BD48__INCLUDED_)
#define AFX_DB_DLLBASEMANAGER_H__86EB1A15_F0F8_43A0_8F06_8D2C5DE9BD48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Db_DllManager_Base.h"

#include "..\dgnengine\src\DgnBase\DgnBase_Struct.h"
#include "..\dgndb\src\GEN_UmdDataBase\GEN_DB_ST_LOAD.H"
#include "..\wg_base\PolyMaker.h"

#include "MeshUtil.h"
#include "UserDefType.h"

struct T_SECT_ADDI_STIFF;
struct T_MESH_NODE_D;
struct T_SECT_STIFFNESS;

struct T_LINE_FOR_MESH_G : public T_LINE_G
{
	T_LINE_FOR_MESH_G() : T_LINE_G()
	{
		bNoDivWhenMesh = FALSE;
	}
	T_LINE_FOR_MESH_G& operator=(const T_LINE_G& src)
	{
		p1 = src.p1;
		p2 = src.p2;

		bIncludeCurv = src.bIncludeCurv;
		dR = src.dR;
		dBetaAng = src.dBetaAng;
		pc = src.pc;
		vecNormal = src.vecNormal;

		bNoDivWhenMesh = FALSE;

		return *this;
	}  
	BOOL bNoDivWhenMesh; // 이 값이 TRUE인 Edge는 Girder와Slab혹은 Girder와Stiff간에 공유되는 Edge이며.. CM2Mesh에서 추가로 쪼개지 않음.. CM2를 부르기전에 Edge자체를 쪼개놓음
};

struct T_ArEachPlanInfo_D
{
	T_ArEachPlanInfo_D()
	{
		Init();
	}
	T_ArEachPlanInfo_D( const T_ArEachPlanInfo_D &src )
	{ 
		*this = src; 
	}
	void Init()
	{
		aAr.RemoveAll();
	}  
	T_ArEachPlanInfo_D& operator=(const T_ArEachPlanInfo_D& src)
	{
		aAr.RemoveAll();
		aAr.Copy(src.aAr);

		return *this;
	}    
	CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> aAr;
};


struct T_EachPlanInfo
{
	T_EachPlanInfo()
	{
		Init();
	}
	T_EachPlanInfo( const T_EachPlanInfo &src )
	{ 
		*this = src; 
	}
	void Init()
	{
		aEachPlaneVCount.RemoveAll();
		aEachPlaneVertexIndex.RemoveAll();
		aEachVertex.RemoveAll();
	}  
	T_EachPlanInfo& operator=(const T_EachPlanInfo& src)
	{
		aEachPlaneVCount.RemoveAll();
		aEachPlaneVCount.Copy(src.aEachPlaneVCount);

		aEachPlaneVertexIndex.RemoveAll();
		aEachPlaneVertexIndex.Copy(src.aEachPlaneVertexIndex);

		aEachVertex.RemoveAll();
		aEachVertex.Copy(src.aEachVertex);

		return *this;
	}  
	CArray<int, int> aEachPlaneVCount, aEachPlaneVertexIndex;
	CArray<double, double> aEachVertex;
};



#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDb_DllBaseManager : public CDb_DllManager_Base      
{
public:
	CDb_DllBaseManager();
	virtual ~CDb_DllBaseManager();

public:
	BOOL GetSectMesh(T_SECT_D& SectD, BOOL bAdjustCoordToCentroid,
					 CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
					 CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD);
	//Regular단면의 단면계수를 계산
	BOOL Calc_SectRegular(CString strShape, double dSize[8], DGN_SECT_STIFFNESS& rStiffData);

	// 단면의 좌표정보를 계산하여 넘겨줌
	//   strShape, dSize : 단면형상, 단면치수정보
	// < arOutPolyData : 단면외곽 좌표정보 (dOffsetX, dOffsetY=0 일때 좌측하단이 원점임)
	// < arInPolyData  : 단면내부 공동부 좌표정보 (dOffsetX, dOffsetY=0 일때 좌측하단이 원점임) 
	// < dPos : 단면의 경계값 ([0]=dOffsetX, [1]=dOffsetY, [2]=dXmax, [3]=dXmin, [4]=dYmax, [5]=dYmin)
	//   dOffsetX, dOffsetY : 이동거리(원점을 단면중심에 두려면 중심까지의 거리를 빼주면 됨)
	//   bRev : 단면 역전여부 (L, T, 2L만 적용됨)
	
	BOOL Get_SectionPolygonData(CString strShape, double dSize[8], DGN_GSEC_POLYGON_LIST& arOutPolyData, DGN_GSEC_POLYGON_LIST& arInPolyData, double dPos[6], double dOffsetX=0.0, double dOffsetY=0.0, BOOL bRev=FALSE);


	BOOL IsClosedAfterComposite(T_SECT_D& rData, int nPos);

	BOOL Get_SectionPropertyMesh(T_SECT_D& rData, int nPos, int nTypeBeforeAfter, double dMatlValue[6],
                              	 double* pbeam, double* cdef, int nCdef, BOOL bConvertSectStiffData2BeforeI=TRUE, T_SECT_DESIGN* pSectDesign=NULL);
	BOOL Get_SectionPropertyMesh_AllSect(T_SECT_D& rData, int nPos, int nTypeBeforeAfter, double dMatlValue[6],
		                        double* pbeam, double* cdef, int nCdef, BOOL bConvertSectStiffData2BeforeI=TRUE, T_SECT_DESIGN* pSectDesign=NULL);

	BOOL Get_SectionPropertyMesh_Cscs(T_SECT_D& rData, int nPos, int nTypeBeforeAfter, double dMatlValue[6], int nResult, double** pbeam, int& rnprop);

	BOOL GetEffectiveNormalStressPoint(const T_SECT_D& cSectD, int nPos, int nTypeBeforeAfter, int& rnPoint, CArray<double, double>& raPointY, CArray<double, double>& raPointZ);
	BOOL GetPoint_Effective(T_SECT_D& SectD, int nPos, int nTypeBeforeAfter, int nPoint, double* pointy, double* pointz);
	BOOL Get_SectionPropertyMesh_Effective(T_SECT_D& rData, int nPos, int nCompMeshType, double dMatlValue[6],
																					double force[8], int& rnStress/*count of stress*/, double coordy[6], double coordz[6],
																					int& rnPoint /*count of stress point*/, int& rnProp/*count of domain (sect_e가 0이 아닌 것의 개수)*/,
																					CArray<double,double>& raStress, CArray<double,double>& raPointY, CArray<double,double>& raPointZ);
	BOOL Get_SectionPropertyMesh_BeamDetail(T_SECT_D& rData, int nPos, 
																					int nTypeBeforeAfter, 
																					double dMatlValue[6],
																					double force[8],
																					//int nload,        // beam section temperature 개수 
																					//double* bst,      // beam section temperature (8 x nload)
																					int& rnStress,    // count of stress
																					double coordy[6],
																					double coordz[6],
																					int& rnPoint,     // count of stress point
																					int& rnProp,      // count of domain (sect_e가 0이 아닌 것의 개수)
																					CArray<double,double>& raStress,
																					CArray<double,double>& raPointY,
																					CArray<double,double>& raPointZ);

	BOOL GetStressInfo4TorsionAndWarping(T_SECT_D& rData, int nPos, int nTypeBeforeAfter, double dMatlValue[6],
		                                  int& rNumPlane, CArray<int, int>* pPlaneVCount, CArray<BOOL, BOOL>* pPlaneVacant,
																			CArray<int, int>* pPlaneVertexIndex, CArray<double, double>* pVertex, double dValue[19], double dValue2[19], double dxyNew[6], double dxzNew[6]);
	BOOL CalcSectionPropertyMesh(T_SECT_D& rData, T_SECT_ADDI_STIFF& raddiStiff, int nPos, double dMatlValue[6],
															BOOL bAllStiffness=FALSE, int nTypeBeforeAfter=0, BOOL bReturnBySectBeforeI=FALSE, BOOL bConvertSectStiffData2BeforeI=TRUE);
	BOOL CalcSectionPropertyMesh_Cscs(T_SECT_SECTBASE_D& rSectBase_girder, T_SECT_SECTBASE_D& rSectBase_slab, 
															T_SECT_D& rData, int nPos, double dMatlValue[6],
															BOOL bAllStiffness=FALSE, int nTypeBeforeAfter=0, BOOL bReturnBySectBeforeI=FALSE);
		
	BOOL Get_PlasticSectionModulusMesh(T_SECT_D& rData, double dMatlValue[6], int nPos, double& dZpy, double& dZpz, BOOL bCalcByMesh = FALSE);
	BOOL Get_EachPlasticSectionModulus( CArray<XGL_3dp,XGL_3dp&>& arOutCut, CArray<XGL_3dp,XGL_3dp&>& arInnerCut, T_POINT3D PntCutLine[][2], double yc, double zc, T_SECT_D& SectD, int nTypeBeforeAfter, double * dMatlValue, double& dAreaSum, double& dAreaY, double& dAreaZ );
	BOOL Get_SectionFirstMomentOfAreaMesh( T_SECT_D &SectD, double& area, double& areay, double& areaz, double yc, double zc, std::vector<int>& aDomainElemCount, double dMatlValue[6], CArray<int,int>& aDomainMaterialType, CArray<T_MESH_NODE_D,T_MESH_NODE_D&> &aMeshNodeD, CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&> &aMeshElemD );
	BOOL Get_SectRegularWarpingFuncion(T_SECT_SECTBASE_D& rSectData, CString strShape, double dElaMod, double dNu=0.0);
	BOOL Get_WindLoad4WindowFrame(T_GLOAD_KR_WIND& rData, const CString &csWindCode, double& dResult1, double& dReault2); // 창호 위저드에서 WindLoad 계산시 사용
	BOOL GetSectTopologyType(T_SECT_D& rData, int& nTopologyType);

protected:
	BOOL GetMatl4Domain(const double dMatlValue[6], 
											const int nDomain,
											const CArray<int,int>& aDomainMaterialType,
											double& rdE, double& rdW, double& rdNu,
											CArray<double, double>& raElaMod, 
											CArray<double, double>& raSectW, 
											CArray<double, double>& raNu,
											BOOL bByConc=FALSE);
	BOOL IsSect_PSC(const T_SECT_D& SectD);

	BOOL ConvertSectStiffData2BeforeI(T_SECT_D& rSectD_I, const T_SECT_D& SectD, int nPos, int nTypeBeforeAfter);
	BOOL ConvertSectBeforeI2StiffData(T_SECT_D& rSectD, const T_SECT_D& SectD_I, int nPos, int nTypeBeforeAfter);
	void WritePBeamDllParamToFile(double* cdef, int nCdef, int* p_nnode, int* p_npoint, int* p_nelem, int* p_nprop, int* p_ncombined, double* p_ynode, double* p_znode, double* p_pointy, double* p_pointz, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem, double* p_E, double* p_NU, double* p_w, double* p_sect_e, double* p_sect_nu, double* p_sect_w, int* p_nrebar, double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS, int* p_ipotn, int* p_iref, int* p_elemtype);
	void WritePBeamDllParamToFileNew(double* cdef, int nCdef, int* p_nnode, int* p_npoint, int* p_nelem, int* p_nprop, int* p_ncombined, double* p_ynode, double* p_znode, double* p_pointy, double* p_pointz, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem, double* p_E, double* p_NU, double* p_w, double* p_sect_e, double* p_sect_nu, double* p_sect_w, int* p_nrebar, double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS, int* p_ipotn, int* p_iref, int* p_elemtype,LPCTSTR lpszPostFix);
	void WritePBeamDllParamToFileDetail(double* force, int nStressSize, int* p_nnode, int* p_npoint, int* p_nelem, int* p_nprop, int* p_ncombined,
										double* p_ynode, double* p_znode, double* p_pointy, double* p_pointz, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem, 
										double* p_E, double* p_NU, double* p_sect_e, double* p_sect_nu, int* p_nrebar, 
										double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, int* p_iref, int* p_nload, double* p_bst, double* p_ctex, int* p_elemtype, LPCTSTR lpszPostFix);
	double GetDistance(T_POINT3D& pt1, T_POINT3D& pt2);
	double GetDistance(XGL_3dp& pt1, XGL_3dp& pt2);

public:
	BOOL m_bUseMaxSectMesh;
	static double m_dMeshSizeRatio;
	static BOOL m_bAllSectMesh;
	
};
#include "HeaderPost.h"

#endif // !defined(AFX_DB_DLLBASEMANAGER_H__86EB1A15_F0F8_43A0_8F06_8D2C5DE9BD48__INCLUDED_)
