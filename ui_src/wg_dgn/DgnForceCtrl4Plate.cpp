// PMS:4020-Seungjun-20090708 Italy Slab/Wall Design.
	// DgnForceCtrl4Plate.cpp: implementation of the CDgnForceCtrl class.
	//
	//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnForceCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_GTF.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_VectorUtil.h"
#include "..\wg_base\Profiler.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_MathLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_StoryData.h"
#include "..\wg_db\DesignResult.h"
#include "..\wg_db\WamtInfo.h"

	// MQC:6366 Seungjun-20130904 : oneway check시 마지막 하중조합만 저장 & 선택된 cutting line 계산하도록 수정.
#include "..\wg_base\ServiceProvider.h"
#include "..\wg_gps\GPS_ServiceDef.h"

	// PMS:4020-Seungjun-20090711 EC2:04 Slab/Wall Design.
#include "DgnCalcCutlLine.h"
#include "DgnProgressDlg.h"

#include "DgnDataCtrl.h"

#if _MSC_VER >= 1700    // vc++ 11.0
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
	using namespace std;
#else
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
#endif

#define _PROFILING
#include "profilewrite.h"

#ifdef _DEBUG
#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum EN_MESH_MEMBTYPE
{
	EN_MESH_SLAB = 0,
	EN_MESH_WALL,
	EN_MESH_SHEL
};

//////////////////////////////////////////////////////////////////////
enum class Position {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	ON_LINE    // 점이 직선 상에 있는 경우
};

class CPositionChecker {
private:
	// 벡터의 크기 계산
	static double magnitude(const double v[3]) {
		return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}

	// 외적 계산
	static void crossProduct(const double a[3], const double b[3], double result[3]) {
		result[0] = a[1] * b[2] - a[2] * b[1];
		result[1] = a[2] * b[0] - a[0] * b[2];
		result[2] = a[0] * b[1] - a[1] * b[0];
	}

	// 내적 계산
	static double dotProduct(const double a[3], const double b[3]) {
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

public:
	static Position checkPosition(const double lineStart[3], const double lineEnd[3], const double point[3]) 
	{
		double lineDir[3];
		double pointVec[3];
		for(int i=0; i<3; ++i)
		{
			// 1. 선분의 방향 벡터 계산
			lineDir[i]  = lineEnd[i] - lineStart[i];
			// 2. 선분의 시작점에서 주어진 점으로의 벡터 계산
			pointVec[i] = point[i]   - lineStart[i];
		}

		// 3. 외적 계산
		double cross[3];
		CPositionChecker::crossProduct(lineDir, pointVec, cross);
		double crossMagnitude = CPositionChecker::magnitude(cross);

		// 4. 점이 직선 상에 있는지 검사
		if(LT0(crossMagnitude)) {
			return Position::ON_LINE;
		}

		// 5. 선분을 xy 평면에 투영했을 때의 방향 벡터
		double lineDirProj[3] = {lineDir[0], lineDir[1], 0};
		double projMagnitude = CPositionChecker::magnitude(lineDirProj);

		// 6. 선분이 거의 수직인 경우
		if(LT0(projMagnitude)) {
			// 점이 선분의 시작점보다 x 좌표가 큰 경우 오른쪽
			return (point[0] - lineStart[0] > 0) ? Position::RIGHT : Position::LEFT;
		}

		// 7. 일반적인 경우
		// 외적의 z 성분으로 좌우 판단
		if(UQ0(cross[2])) {
			return (cross[2] > 0) ? Position::LEFT : Position::RIGHT;
		}

		// 8. 좌우를 판단할 수 없는 경우 (거의 같은 x,y 좌표)
		// y축과 평행한 선분의 경우 x 성분으로 상하 판단
		if(EQ0(lineDir[0]) && EQ0(lineDir[2])) {
			return (cross[0] < 0) ? Position::BOTTOM : Position::TOP;
		}

		// 그 외의 경우 y 성분으로 상하 판단
		return (cross[1] > 0) ? Position::BOTTOM : Position::TOP;
	}
};
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CDgnForceCtrl::Init_PlateData()
{
	//Slab Design Options
	m_bSlabDesignCheck   = FALSE;
	m_bAverage           = FALSE;
	m_nTopBottom         = 0;
	m_nCellOption        = 0;
	m_dCellSize          = 0.0;
	m_dCreepCoef         = 0.0;
	m_nSlabDsgDir        = 0;
	m_nLegendType        = 0; // 0: Rebar 1:As Value, 2:ratio_value
	m_bOneWayMomentCheck = FALSE;

	m_bPunchingShearCheck= FALSE;
	m_nPnChkType         = 0;	//0:Force, 1:Stress
	m_nPnChkStrsType     = 0; //0:Avg. by Elem, 1:Avg. by Side  
	m_bOneWayShearCheck  = FALSE;
	m_bUnbalMomIS      = FALSE;//Pinakin - IS456 SLAB, consider unbalanced moment for IS code

	m_bMeshWallDgn = FALSE;

	m_nSelectLcomK       = 0; // GPS에서 선택한 LcomK. 0이면 All combination.

	m_PForcComponent     = PFORC_COMPONENT_MXX;

	// cutting line
	m_nCuttingDiagramLeftRight = 0; // one way shear cutting line option.

	// Member force.
	for(int i=0; i<4; i++)
	{
		m_dMuTop[i] = 0.0;
		m_dMuBot[i] = 0.0;

		m_dMuDTop[i] = 0.0;
		m_dMuDBot[i] = 0.0;
		m_dMuLTop[i] = 0.0;
		m_dMuLBot[i] = 0.0;

		m_dStressx[i]  = 0.0;
		m_dStressy[i]  = 0.0;
		m_dStressxy[i] = 0.0;
	}
	
	m_arPnChkCutl.InitHashTable(1001);
	m_arPnChkCutl.RemoveAll();

	// 설계 결과
	// 2way For Flexural.
	m_OnewaySlabResTopData.InitHashTable(HASHSIZEELEM);
	m_OnewaySlabResTopData.RemoveAll();
	m_OnewaySlabResBotData.InitHashTable(101);
	m_OnewaySlabResBotData;
	// 1way For Flexural.
	m_OnewaySlabResData.InitHashTable(101);
	m_OnewaySlabResData.RemoveAll();
	// 2way For Shear(punching shear)
	m_mapPunchingData.InitHashTable(1001);
	m_mapPunchingData.RemoveAll();
	// 1way For Shear.
	m_OnewayResData.InitHashTable(101);
	m_OnewayResData.RemoveAll();

	// Element and sub-domain
	m_aSbdoByElem.InitHashTable(HASHSIZEELEM);
	m_aSbdoByElem.RemoveAll();
	m_aSbdoList.InitHashTable(1001);
	m_aSbdoList.RemoveAll();
	m_aMadoBySbdo.InitHashTable(1001);
	m_aMadoBySbdo.RemoveAll();

	m_PlateKeyList.RemoveAll();

	m_mapNodeInElem.InitHashTable(HASHSIZEELEM);
	m_mapNodeInElem.RemoveAll();
	m_mapSlabForc_Compact.InitHashTable(HASHSIZEELEM);
	m_mapSlabForc_Compact.RemoveAll();
	m_mapSlabForc_Compact_D.InitHashTable(HASHSIZEELEM);
	m_mapSlabForc_Compact_D.RemoveAll();
	m_mapSlabForc_Compact_L.InitHashTable(HASHSIZEELEM);
	m_mapSlabForc_Compact_L.RemoveAll();
	m_mapCellElem.InitHashTable(HASHSIZENODE);
	m_mapCellElem.RemoveAll();  

	m_arStorbyElem.InitHashTable(HASHSIZEELEM);
	m_arStorbyElem.RemoveAll();
	m_arElemListbyStor.InitHashTable(101);
	m_arElemListbyStor.RemoveAll();
	m_arStorK.RemoveAll();

	m_bCalcOneway = FALSE;
	m_dCuttingLineAngle = 0.0;
	m_bOneWayVec = FALSE;
	memset(m_vecU, 0, sizeof(m_vecU));

	m_bPlateForce = FALSE;
	m_dPlateAngle = 0.0;

	// Coded by Seungjun MNET:No.2293 ('20060719)
	m_bTestEnvValue = CDBLib::CheckTheDesignTestType(2);
	if(m_bTestEnvValue)
	{
		m_bTestFile = _T("zzzWoodArmer.txt");
		rptwofstream fout(m_bTestFile, ios::trunc);
	}
	
	memset(m_dTimeChk, 0, sizeof(m_dTimeChk));

	m_nCrackType=0; // check 785 //

	// PMS:xxx-Seungjun-20100830 sub-domain이 동일 평면이 아닐 경우 사용. only wall. 
	m_aSubSubDomain.clear();
	m_aSubSubDomainVec.clear();
}

//////////////////////////////////////////////////////////////////////
// Set Force data before design.
//////////////////////////////////////////////////////////////////////
BOOL CDgnForceCtrl::MakePForcDataforDesign_Compact(_DGN_LCOM& LcomDesign, UINT SbdoK/* = NULL*/)
{
	// Get Load combination data.
    T_LCOM_D LcomD;
    if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomDesign.OriginalLcomKey, LcomD) ) return FALSE;

    m_nMaxMinType = LcomDesign.bMaxLcom ? DGN_LCOM_MAX : DGN_LCOM_MIN;
    if ( IsLcomServ(LcomD.nActive) )
    {
        m_mapSlabForc_Compact_D.RemoveAll();
        m_mapSlabForc_Compact_L.RemoveAll();

        const T_LCOM_D_UL& LcomUlData = LcomDesign.LcomUlData;
        INT_PTR nLcomUlNum = LcomUlData.Combination.GetSize();
        for ( INT_PTR i=0; i<nLcomUlNum; ++i )
        {
            const T_LCOM_BASE& CurBase = LcomUlData.Combination[i];
            const T_STLD_K iLcaseKey = CurBase.LoadCaseKey;
            const double   dFactor   = CurBase.Factor;

            // Get Load Case Type(D,L,W,E,...).
            T_STLD_D Lcase;
            Lcase.Initialize();
            m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);
            int iLcaseType = 0;
            if ( CDBLib::IsStldDL(Lcase.LoadCaseType) )      iLcaseType=1;
            else if ( CDBLib::IsStldLL(Lcase.LoadCaseType) ) iLcaseType=2;

            if ( iLcaseType ) // Dead/Live Load.
            {
                T_LCOM_D LcasD;
                LcasD.Initialize();
                if ( !m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STATIC, iLcaseKey, LcasD) )	continue;
                if ( !MakePForcDataforDesign_Compact4LType(LcasD, iLcaseType, dFactor, SbdoK) ) continue;
            }
        }
    }

    return MakePForcDataforDesign_Compact4LType(LcomD, 0, 1.0, SbdoK);
}

BOOL CDgnForceCtrl::MakePForcDataforDesign_Compact4LType(T_LCOM_D& LcomD, int LcaseType, double dFactor, UINT SbdoK/* = NULL*/)
{
	m_aSbdoByElem.RemoveAll();
	m_aSbdoList.RemoveAll();

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit=FALSE;
	if(CurIndex.nBase_Length!=m_nCode_Length || CurIndex.nBase_Force!=m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force  = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++

	// Set Loadcombination.
	if(LcomD.nActive!=-1)	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

	m_PlateKeyList.RemoveAll();
	m_mapSlabForc_Compact.RemoveAll();
	m_mapNodeInElem.RemoveAll();

	// RC Meshed Wall = TRUE
	// RC Meshed Slab, Inclined Meshed Slab, Meshed Shell = FALSE
	const BOOL bWall = (m_iRcsTypeKind/10==9) ? TRUE : FALSE;

	CArray<T_ELEM_K, T_ELEM_K> aEKeyListActive;
	aEKeyListActive.RemoveAll();

	int nSbdo = 1;
	CArray<T_SBDO_K, T_SBDO_K> aSbdoKeys; // Sub domain key list. (설계시는 sub-domain만 사용)
	aSbdoKeys.RemoveAll();
	if(m_bPlateForce)
	{
		aSbdoKeys.Add(0);
		m_pDgnDataCtrl->GetPlateList(aEKeyListActive, bWall);
	}
	else
	{
		if(SbdoK)   aSbdoKeys.Add(SbdoK);
		else        m_pDoc->m_pAttrCtrl->GetSbdoKeyList(aSbdoKeys);
		nSbdo = static_cast<int>(aSbdoKeys.GetSize());

		if(nSbdo==0)
		{
			if(bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
			return FALSE; // if not exist Domain, can't not design.
		}

		CArrayUtil::Sort(aSbdoKeys);
		m_pDgnDataCtrl->m_pMembCtrl->GetPlateListForDgn(aEKeyListActive, bWall); // Get active elem key list.
	}

	CArray<T_ELEM_K, T_ELEM_K> aSbdoElemKeys, aElemKeys;
	for(int w=0; w<nSbdo; w++)
	{
		T_SBDO_D SbdoD;
		T_SBDO_K SbdoK = aSbdoKeys[w];
		if(!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) SbdoD.Initialize();
		if(m_bPlateForce) SbdoD.nType=1; // slab

		double dVectrSBDO[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};//**// wood armer
		if(m_iRcsTypeKind/10==8 || m_iRcsTypeKind/10==30)  // Slab design.
		{
			if(SbdoD.IsNone() || SbdoD.IsWall() || SbdoD.IsShell()) continue; // only Slab and mat.
			if(IsWoodArmerBefore()) GetOrSetSbdoVector(SbdoK, dVectrSBDO);//**// wood armer
		}
		else if(m_iRcsTypeKind/10==9) // Wall design.
		{
			if(!SbdoD.IsWall()) continue; // only Wall.
		}
		else if(m_iRcsTypeKind/10==70) // Shell Design.
		{
			if(!SbdoD.IsShell()) continue; // only Shell.
		}

		// Get elem key list in Sub-Domain.
		aSbdoElemKeys.RemoveAll();
		int nSubElem = m_pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aSbdoElemKeys);
		if(m_bPlateForce)
		{
			aSbdoElemKeys.RemoveAll();
			aSbdoElemKeys.Copy(aEKeyListActive);
			nSubElem = static_cast<int>(aSbdoElemKeys.GetSize());
		}

		aElemKeys.RemoveAll();
		if(!m_bAverage)	CArrayUtil::IntersectUINT(aEKeyListActive,aSbdoElemKeys,aElemKeys);
		else      			aElemKeys.Copy(aSbdoElemKeys);

		double UCS_vector[3][3] = {{1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}};  // 기준이 되는 Wall/Slab의 Local Direction.
		if(m_iRcsTypeKind/10==9 || m_iRcsTypeKind/10==30 || m_iRcsTypeKind/10==70)
		{
			// PMS:xxx-Seungjun-20100830 sub-domain이 동일 평면이 아닐 경우 사용. only wall. 
			m_aSubSubDomain.clear();
			m_aSubSubDomainVec.clear();
			m_pDoc->m_pPostCtrl->Get_SBDO_LocalVector(SbdoK, UCS_vector, m_aSubSubDomain, m_aSubSubDomainVec, TRUE);
		}

		BOOL bInclinedUCS=FALSE;
		int elem_num = static_cast<int>(aElemKeys.GetSize());
		if(elem_num==0) continue;

		auto lambda_LVec = [&](T_ELEM_K& elem_key, T_VECTOR3BY3D& LVec)
		{
			int nSSdomain=0;
			auto itrFind = m_aSubSubDomain.find(elem_key);
			if(itrFind == m_aSubSubDomain.end()) LVec.Initialize();
			else
			{
				elem_key  = itrFind->first;
				nSSdomain = itrFind->second;

				auto itrFindVec = m_aSubSubDomainVec.find(nSSdomain);
				if(itrFindVec == m_aSubSubDomainVec.end()) LVec.Initialize();
				else
				{
					nSSdomain = itrFindVec->first;
					LVec      = itrFindVec->second;
				}
			}
		};

		for (int i=0;i<nSubElem;i++) 
		{
			T_ELEM_K elem_key = aSbdoElemKeys[i];
			m_aSbdoByElem.SetAt(elem_key, SbdoK);
			m_aSbdoList.SetAt(SbdoK, SbdoK);

			T_ELEM_D	elem_data;
			elem_data.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(elem_key,elem_data))	{ASSERT(0);  continue;}

			T_STRP_DL	pforc;			
			if(m_pDoc->m_pAttrCtrl->IsPlaneType(elem_data.eltyp)) 
			{
				T_STRP_DL	pforcMax, pforcMin;
				BOOL bGetOK = m_pDoc->m_pPostCtrl->GetStrpNew_L(elem_key,&pforcMax,&pforcMin,NULL);

                double local_vector[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
#ifdef _CIVIL
				// PMS.5537 Civil Shell Design Eurocode2_2_05
				CalcPlateLocalForce4Civil(SbdoD.nRebarAxisType, SbdoD.strUCS, SbdoK, elem_key, pforcMax, pforcMin, local_vector);
                pforc = m_nMaxMinType == DGN_LCOM_MAX ? pforcMax : pforcMin;

                if(m_iRcsTypeKind/10==8 && m_iDgnCode != GB50010_10) // Slab : 경사슬래브가 없으므로, GX,GY,GZ = (1,0,0)(0,1,0)(0,0,1)이 기준 UCS가 됨.
                {		  
                    if(m_bOneWayVec)    m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector,m_vecU,pforc);
                    else
                    {
                        GetShellDgnVector(SbdoD.dAngle[0], local_vector, UCS_vector);
                        m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector,UCS_vector,pforc);
                    }
                }
                else if(m_iRcsTypeKind/10==9 || m_iRcsTypeKind/10==30 || m_iRcsTypeKind/10==70) // Wall
                {	
                    // PMS.5537 Civil Shell Design Eurocode2_2_05
                    double adDgnVec[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
                    GetShellDgnVector(SbdoD.dAngle[0], local_vector, adDgnVec);
                    m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector, adDgnVec, pforc);
                }
#else
                pforc = m_nMaxMinType == DGN_LCOM_MAX ? pforcMax : pforcMin;
				if (m_iRcsTypeKind / 10 == 8 && (m_bOneWayVec && (m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19))) // Slab : 경사슬래브가 없으므로, GX,GY,GZ = (1,0,0)(0,1,0)(0,0,1)이 기준 UCS가 됨.
                {		  
                    memcpy(UCS_vector, local_vector, sizeof(UCS_vector)); // 초기화

                    m_pDoc->calcPlaneLocalVector(elem_key, local_vector);
                    CMathFunc::mathNormalize(local_vector[2],local_vector[2]);
                    double dDot=fabs(CMathFunc::mathDot(local_vector[2],UCS_vector[2]));
                    // PMS:4352-Seungjun-20111012 EC Inclined slab.
                    if(fabs(dDot-1.0) > 0.0001523 && !m_bOneWayVec)
                    {
                        if(!bInclinedUCS)
                        {
                            // sub-domain이 동일 평면이 아닐 경우 사용. only wall. 
                            m_aSubSubDomain.clear();
                            m_aSubSubDomainVec.clear();
                            m_pDoc->m_pPostCtrl->Get_SBDO_LocalVector(SbdoK, UCS_vector, m_aSubSubDomain, m_aSubSubDomainVec);
                            bInclinedUCS = TRUE;
                        }

                        T_VECTOR3BY3D LVec; LVec.Initialize();
                        lambda_LVec(elem_key, LVec);
                        memcpy(UCS_vector, LVec.dVector, sizeof(UCS_vector));
                    }

                    if(m_bOneWayVec)	m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector,m_vecU,pforc);
                    else              m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector,UCS_vector,pforc);
                }
                else if(m_iRcsTypeKind/10==9 || m_iRcsTypeKind/10==30 || m_iRcsTypeKind/10==70) // Wall
                {	
                    m_pDoc->calcPlaneLocalVector(elem_key, local_vector);

                    T_VECTOR3BY3D LVec; LVec.Initialize();
                    lambda_LVec(elem_key, LVec);

                    double adDgnVec[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
                    GetShellDgnVector(SbdoD.dAngle[0], LVec.dVector, adDgnVec);
                    m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector, adDgnVec, pforc);
                }
#endif // _CIVIL
				if(!bGetOK)
					continue; 
			}
			else 
				continue;

			// Plate가 물고 있는 노드 수
			int nie=GetPlateNodeNum(elem_key, elem_data);
			m_PlateKeyList.Add(elem_key);
			
			T_SLAB_FORCE_COMPACT SlabForce;
			SlabForce.initialize();
			for(int n=0; n<nie; n++)
			{
				if(m_iRcsTypeKind/10==8) // Slab
				{
                    if ( !m_pDgnDataCtrl->IsMeshedSlab(m_iDgnCode) )
                    {
                        SlabForce.dForce[n] = CalcPForcValue_L(pforc, n+1, m_PForcComponent);
                    }						
					else if (m_PForcComponent==PFORC_COMPONENT_MXX || m_PForcComponent==PFORC_COMPONENT_MYY)
					{
						if (m_bOneWayVec && (m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19))
						{
                            double dmxx = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_MXX);
                            double dmyy = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_MYY);
                            double dmxy = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_MXY);
							SlabForce.dForce[n] = dmxx*dFactor;
							SlabForce.dForce[n + 4] = dmyy*dFactor;
							SlabForce.dForce[n + 8] = dmxy*dFactor;

							//**// wood armer
							if(IsWoodArmerBefore())
							{ 
								double dUpper=0.0, dLower=0.0;
								CalcWoodArmer(dmxx, dmyy, dmxy, SbdoD.dAngle[0], SbdoD.dAngle[1], dUpper, dLower, dVectrSBDO, m_bWoodArmer);

								SlabForce.dForce[n + 12] = dLower;
								SlabForce.dForce[n + 16] = dUpper;
							}
						}
						else
						{
							int nIndexForce = 0;
							if(m_bAverage)
								nIndexForce = n+1;
							SlabForce.dForce[n]      = CalcPForcValue_L(pforc, nIndexForce, PFORC_COMPONENT_MXX)*dFactor;
							SlabForce.dForce[n + 4]  = CalcPForcValue_L(pforc, nIndexForce, PFORC_COMPONENT_MYY)*dFactor;
							SlabForce.dForce[n + 8]  = CalcPForcValue_L(pforc, nIndexForce, PFORC_COMPONENT_MXY)*dFactor;
							SlabForce.dForce[n + 12] = CalcPForcValue_L(pforc, nIndexForce, PFORC_COMPONENT_FXX)*dFactor;
							SlabForce.dForce[n + 16] = CalcPForcValue_L(pforc, nIndexForce, PFORC_COMPONENT_FYY)*dFactor;
							SlabForce.dForce[n + 20] = CalcPForcValue_L(pforc, nIndexForce, PFORC_COMPONENT_FXY)*dFactor;
							SlabForce.dForce[n + 24] = CalcPForcValue_L(pforc, nIndexForce, PFORC_COMPONENT_VXX)*dFactor;
							SlabForce.dForce[n + 28] = CalcPForcValue_L(pforc, nIndexForce, PFORC_COMPONENT_VYY)*dFactor;
						}
					}
                    else
                    {
                        if ( m_iDgnCode == AIJ_WSD99 && (m_nShearnAdhesive == 1 || m_nShearnAdhesive == 2))
                        {
                            SlabForce.dForce[n]      = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_MXX);
                            SlabForce.dForce[n + 4]  = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_MYY);
                            SlabForce.dForce[n + 24] = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_VXX);
                            SlabForce.dForce[n + 28] = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_VYY);
                        }
                        else
                        {
                            SlabForce.dForce[n]   = CalcPForcValue_L(pforc, n+1, m_PForcComponent);
							SlabForce.dForce[n+4] = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_MXX);
							SlabForce.dForce[n+8] = CalcPForcValue_L(pforc, n+1, PFORC_COMPONENT_MYY);
                        }                        
                    }
				}
				else if(m_iRcsTypeKind/10==9) // Meshed Wall
				{          
					if (m_pDgnDataCtrl->IsMeshedWall(m_iDgnCode))
					{
						SlabForce.dForce[n]    = pforc.dblStress[n+1][14];  // bottom local-x stress
						SlabForce.dForce[n+4]  = pforc.dblStress[n+1][15];  // bottom local-y stress
						SlabForce.dForce[n+8]  = pforc.dblStress[n+1][16];  // bottom shear stress
					}
				}
				// PMS:4352-Seungjun-20110930 EC Inclined slab.
				else if(m_iRcsTypeKind/10==30) // inclined Slab
				{
					if(m_iDgnCode!=Eurocode2_04)  SlabForce.dForce[n] = CalcPForcValue_L(pforc,n+1,m_PForcComponent);
					else
					{
						SlabForce.dForce[n]     = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_MXX)*dFactor;
						SlabForce.dForce[n+4]   = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_MYY)*dFactor;
						SlabForce.dForce[n+8]   = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_MXY)*dFactor;
						SlabForce.dForce[n+12]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_FXX)*dFactor;
						SlabForce.dForce[n+16]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_FYY)*dFactor;
						SlabForce.dForce[n+20]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_FXY)*dFactor;
						SlabForce.dForce[n+24]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_VXX)*dFactor;
						SlabForce.dForce[n+28]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_VYY)*dFactor;
					}
				}
				else if (m_iRcsTypeKind / 10 == 70) // Shell
				{
					const bool bCodeShell = [](int nCode)
					{
						switch (nCode)
						{
						case Eurocode2_04:
						case Eurocode2_2_05:
						case ACI318_14:
						case ACI318M_14:
						case ACI318_19:
						case ACI318M_19:
						case ACI318_25:
						case ACI318M_25:
						case NSCP_2015_C:
						case NSR_10:
						case NTC_DCEC_2023:
						case TWN_USD112:
							return true;
						default:
							return false;
						}
					}(m_iDgnCode);

					if(bCodeShell) // PMS.5537 Civil Shell Design Eurocode2_2_05
					{
						SlabForce.dForce[n   ]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_MXX)*dFactor;
						SlabForce.dForce[n+ 4]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_MYY)*dFactor;
						SlabForce.dForce[n+ 8]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_MXY)*dFactor;
						SlabForce.dForce[n+12]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_FXX)*dFactor;
						SlabForce.dForce[n+16]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_FYY)*dFactor;
						SlabForce.dForce[n+20]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_FXY)*dFactor;
						SlabForce.dForce[n+24]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_VXX)*dFactor;
						SlabForce.dForce[n+28]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_VYY)*dFactor;	
						SlabForce.dForce[n+32]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_SMX_TOP)*dFactor;
						SlabForce.dForce[n+36]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_SMN_TOP)*dFactor;
						SlabForce.dForce[n+40]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_SMX_BOT)*dFactor;
						SlabForce.dForce[n+44]  = CalcPForcValue_L(pforc,n+1,PFORC_COMPONENT_SMN_BOT)*dFactor;
					}
					else
						SlabForce.dForce[n] = CalcPForcValue_L(pforc,n+1,m_PForcComponent);
				}
			}

			if(LcaseType==0)  // All
			{
				m_mapSlabForc_Compact.SetAt(elem_key,SlabForce);
			}
			else if(LcaseType==1) // DL 
			{
				T_SLAB_FORCE_COMPACT SlabForceSum;
				if(!m_mapSlabForc_Compact_D.Lookup(elem_key,SlabForceSum)) SlabForceSum.initialize();
				for (int kk=0; kk<SLAB_FORCE_COMPACT_NUM; kk++)
				{
					SlabForceSum.dForce[kk] += SlabForce.dForce[kk]; 
				}
				m_mapSlabForc_Compact_D.SetAt(elem_key,SlabForceSum);
			}
			else if(LcaseType==2) // LL 
			{
				T_SLAB_FORCE_COMPACT SlabForceSum;
				if(!m_mapSlabForc_Compact_L.Lookup(elem_key,SlabForceSum)) SlabForceSum.initialize();
				for (int kk=0; kk<SLAB_FORCE_COMPACT_NUM; kk++)
				{
					SlabForceSum.dForce[kk] += SlabForce.dForce[kk];
				}
				m_mapSlabForc_Compact_L.SetAt(elem_key,SlabForceSum);
			}
		}

		// 각 노드별로 인접한 요소의 Plate Force의 값의 평균값을 구한다.
		//!(m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19) By xuezc for Slab_CH;m_bAverage->Take only the maximum value of the nodes
		if (m_bAverage && !(m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19))
		{
			auto lambda_AverageNodal = [&](CArray<T_ELEM_K, T_ELEM_K>& arDgnElemK)
			{
				switch (LcaseType)
				{
				case 0:	 // All
					GetAverageNodalDataList_Compact(arDgnElemK, m_mapSlabForc_Compact);
					break;
				case 1:	// DL
					GetAverageNodalDataList_Compact(arDgnElemK, m_mapSlabForc_Compact_D);
					break;
				case 2:	// LL
					GetAverageNodalDataList_Compact(arDgnElemK, m_mapSlabForc_Compact_L);
					break;
				default:
					ASSERT(0);
					break;
				}
			};

			// 층별로 smoothing을 하도록 처리.
			if(ExistStory() && m_iRcsTypeKind/10==9)
			{
				_DGN_ELEM_LIST StorElemKList;
				CArray<T_ELEM_K, T_ELEM_K> arDgnElemK;
				int nStor = static_cast<int>(m_arStorK.GetSize());
				for(int k=0; k<nStor; ++k)
				{
					T_STOR_K StorK = m_arStorK[k];
					StorElemKList.Initialize();
					if(!m_arElemListbyStor.Lookup(StorK, StorElemKList))  continue;
					arDgnElemK.RemoveAll();
					CArrayUtil::IntersectUINT(aSbdoElemKeys,StorElemKList.arElemList,arDgnElemK);
					if(arDgnElemK.GetSize()==0)  continue;

					lambda_AverageNodal(arDgnElemK);
				}
			}
			else
			{
				lambda_AverageNodal(aSbdoElemKeys);
			}
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	if(bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	return TRUE;
}

BOOL CDgnForceCtrl::MakePForcDataforDesign_Compact(int nType)
{
	m_mapSlabForc_Compact.RemoveAll();
	m_mapSlabForc_Compact_D.RemoveAll();
	m_mapSlabForc_Compact_L.RemoveAll();
	m_mapSlabForc_CompactbyCell.RemoveAll();

	SetWoodArmerOption();

	// Set loadcombination
	T_LCOM_D LcomD;
	LcomD.Initialize();
	LcomD.nActive = -1;

	// Get Con.
	T_DCON_D DconD;
	DconD.Initialize();
	m_pDgnDataCtrl->Get_DgnConDcon(DconD);
	m_iDgnCode = m_pDgnDataCtrl->GetCodeID4Meshed(DconD.DesignCode, m_bPlateForce);
	if(m_iDgnCode == 0) return FALSE;

	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	m_nBase_Length = CurIndex.nBase_Length;
	m_nBase_Force  = CurIndex.nBase_Force;
	m_nCode_Length = CurIndex.nBase_Length;
	m_nCode_Force  = CurIndex.nBase_Force;

	LcomD.LoadCombType=-1;
	if(!MakePForcDataforDesign_Compact4LType(LcomD, 0, 1.0)) return FALSE;

	GetActivePlateList(nType==1 ? TRUE : FALSE);

	if(nType==0)  // Slab
		MakePForcDataforDesign_Compact_SaveSlab();
	else if(nType==1) // Wall
		MakePForcDataforDesign_Compact_SaveWall();

	return TRUE;
}

void CDgnForceCtrl::MakePForcDataforDesign_Compact_SaveSlab()
{
	auto lambda_CalcForce = [&](const int& iElem, double dMaxMu[4], T_SLAB_FORCE_COMPACT& MaxForceD)->BOOL
	{
		if(!Set_PlateForceMoment(iElem)) return FALSE;

		T_SLAB_FORCE_COMPACT ForceD;
		ForceD.initialize();
		if(!m_mapSlabForc_CompactbyCell.Lookup(iElem, ForceD))
		{ 
			if(!m_mapSlabForc_Compact.Lookup(iElem, ForceD)) return FALSE;
		}

		for(int k=0; k<4; k++)
		{
			// Mu
			MaxForceD.dForce[k]   =  fabs(MaxForceD.dForce[k]  ) > fabs(m_dMuBot[k]) ? MaxForceD.dForce[k]   : m_dMuBot[k];
			MaxForceD.dForce[k+4] =  fabs(MaxForceD.dForce[k+4]) > fabs(m_dMuTop[k]) ? MaxForceD.dForce[k+4] : m_dMuTop[k];

			double dCurMu = max(fabs(MaxForceD.dForce[k]), fabs(MaxForceD.dForce[k+4]));
			if(dMaxMu[k] < dCurMu)
			{
				// mxx
				MaxForceD.dForce[k+8] = ForceD.dForce[k];
				// myy
				MaxForceD.dForce[k+12] = ForceD.dForce[k+4];
				// mxy
				MaxForceD.dForce[k+16] = ForceD.dForce[k+8];

				dMaxMu[k] = dCurMu;
			}
		}

		return TRUE;
	};

	auto ladmbda_SaveForce = [&](const CArray<UINT, UINT>& arPlate)
	{
		// Save data.
		INT_PTR nPlate = arPlate.GetSize();
		for(INT_PTR i=0; i<nPlate; i++)
		{
			T_ELEM_K iElem = arPlate[i];      

			T_SLAB_FORCE_COMPACT FinalForce;
			FinalForce.initialize();

			double dMaxMu[4] = {0.0,0.0,0.0,0.0};
			if(m_nMaxMinType==DGN_LCOM_ALL)
			{
				for(int j=DGN_LCOM_MAX; j<DGN_LCOM_ALL; ++j)
				{
					m_nMaxMinType = static_cast<DGN_LCOM_MAX_TYPE>(j);
					if(!lambda_CalcForce(iElem, dMaxMu, FinalForce)) continue;
				}
			}
			else
			{
				if(!lambda_CalcForce(iElem, dMaxMu, FinalForce)) continue;
			}

			// If callde from GPS, save compact forces data.
			m_pDoc->m_pPostCtrl->GetDesignResult()->Save_SlabForce(iElem, m_nSlabDsgDir, FinalForce);
		}
	};

	// 최적화를 위해 호출
	m_pDoc->m_pPostCtrl->MakeSubdomainInfo();
	if(m_nCellOption)
	{
		CArray<T_SBDO_K, T_SBDO_K> arSbdoK;
		int nSbdo = m_aSbdoList.GetKeyList(arSbdoK);
		for(int kk=0; kk<nSbdo; kk++)
		{
			CArray<T_ELEM_K, T_ELEM_K> arSbdoElemK;
			arSbdoElemK.RemoveAll();
			
			if(!m_bPlateForce)
			{
				T_SBDO_D SbdoD;
				T_SBDO_K SbdoK = arSbdoK[kk];
				m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD);
				if(SbdoD.IsNone() || SbdoD.IsWall()) continue; // only Slab and mat.

				CArray<T_ELEM_K, T_ELEM_K> arSbdoAllElemK;
				m_pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, arSbdoAllElemK);
				CArrayUtil::IntersectUINT(m_PlateKeyList,arSbdoAllElemK,arSbdoElemK);
				MakeSbdoQuadTree(SbdoK);
			}
			else
			{
				m_pDgnDataCtrl->GetPlateList(arSbdoElemK, FALSE);
				m_QuadTree.MakePlateQuadTree(arSbdoElemK);
			}

			// Save data.
			ladmbda_SaveForce(arSbdoElemK);
		}
	}
	else
	{
		// Save data.
		ladmbda_SaveForce(m_PlateKeyList);
	}
}

void CDgnForceCtrl::MakePForcDataforDesign_Compact_SaveWall()
{
	auto lambda_CalcForce = [&](const int& iElem, double& dMaxSig, T_SLAB_FORCE_COMPACT& MaxForceD)->BOOL
	{
		if(!Set_PlateForceMoment(iElem)) return FALSE;

		for(int k=0; k<4; k++)
		{
			double dftdx=0.0, dftdy=0.0, dSigxy=0.0;
			CalcMeshWallForce(iElem, 0.0, m_dStressx[k], m_dStressy[k], m_dStressxy[k], dftdx, dftdy, dSigxy);

			// force
			MaxForceD.dForce[k]   =  fabs(MaxForceD.dForce[k]  ) > fabs(dftdx ) ? MaxForceD.dForce[k]   : dftdx;
			MaxForceD.dForce[k+4] =  fabs(MaxForceD.dForce[k+4]) > fabs(dftdy ) ? MaxForceD.dForce[k+4] : dftdy;
			MaxForceD.dForce[k+8] =  fabs(MaxForceD.dForce[k+8]) > fabs(dSigxy) ? MaxForceD.dForce[k+8] : dSigxy;

			MaxForceD.dForce[k + 12] = m_dStressx[k];  // Fxx
			MaxForceD.dForce[k + 16] = m_dStressy[k];  // myy							
			MaxForceD.dForce[k + 20] = m_dStressxy[k]; // mxy

			double dCurSig = max(max(fabs(MaxForceD.dForce[k]), MaxForceD.dForce[k+4]), MaxForceD.dForce[k+8]);
			if(dMaxSig < dCurSig)
			{
				dMaxSig = dCurSig;
			}
		}

		return TRUE;
	};

	int nPlate = static_cast<int>(m_PlateKeyList.GetSize());
	for(int i=0; i<nPlate; i++)
	{
		T_ELEM_K iElem = m_PlateKeyList[i];   
		T_SLAB_FORCE_COMPACT FinalForce;
		FinalForce.initialize();

		double dMaxSig = 0.0;
		if (m_nMaxMinType == DGN_LCOM_ALL)
		{
			for (int j=DGN_LCOM_MAX; j<DGN_LCOM_ALL; ++j)
			{
				m_nMaxMinType = static_cast<DGN_LCOM_MAX_TYPE>(j);
				if(!lambda_CalcForce(iElem, dMaxSig, FinalForce)) continue;
			}
		}
		else
		{
			if(!lambda_CalcForce(iElem, dMaxSig, FinalForce)) continue;
		}

		// If callde from GPS, save compact forces data.
		m_pDoc->m_pPostCtrl->GetDesignResult()->Save_MeshedWallForce(iElem, FinalForce);
	}
}

void CDgnForceCtrl::MakePForcDataforDesign_Compact_SaveShel()
{
	auto lambda_CalcForce = [&](const int& iElem, double& dMaxSig, T_SLAB_FORCE_COMPACT& MaxForceD)->BOOL
	{
		if(!Set_PlateForceMoment(iElem)) return FALSE;

		for(int k=0; k<4; k++)
		{
			double dftdx=0.0, dftdy=0.0, dSigxy=0.0;
			CalcMeshWallForce(iElem, 0.0, m_dStressx[k], m_dStressy[k], m_dStressxy[k], dftdx, dftdy, dSigxy);

			// force
			MaxForceD.dForce[k]   =  fabs(MaxForceD.dForce[k]  ) > fabs(dftdx ) ? MaxForceD.dForce[k]   : dftdx;
			MaxForceD.dForce[k+4] =  fabs(MaxForceD.dForce[k+4]) > fabs(dftdy ) ? MaxForceD.dForce[k+4] : dftdy;
			MaxForceD.dForce[k+8] =  fabs(MaxForceD.dForce[k+8]) > fabs(dSigxy) ? MaxForceD.dForce[k+8] : dSigxy;

			MaxForceD.dForce[k + 12] = m_dStressx[k];  // Fxx
			MaxForceD.dForce[k + 16] = m_dStressy[k];  // myy							
			MaxForceD.dForce[k + 20] = m_dStressxy[k]; // mxy

			double dCurSig = max(max(fabs(MaxForceD.dForce[k]), MaxForceD.dForce[k+4]), MaxForceD.dForce[k+8]);
			if(dMaxSig < dCurSig)
			{				
				dMaxSig = dCurSig;
			}
		}

		return TRUE;
	};

	int nPlate = static_cast<int>(m_PlateKeyList.GetSize());
	for(int i=0; i<nPlate; i++)
	{
		T_ELEM_K iElem = m_PlateKeyList[i];   
		T_SLAB_FORCE_COMPACT FinalForce;
		FinalForce.initialize();

		double dMaxSig = 0.0;
		if (m_nMaxMinType == DGN_LCOM_ALL)
		{
			for (int j=DGN_LCOM_MAX; j<DGN_LCOM_ALL; ++j)
			{
				m_nMaxMinType = static_cast<DGN_LCOM_MAX_TYPE>(j);
				if(!lambda_CalcForce(iElem, dMaxSig, FinalForce)) continue;
			}
		}
		else
		{
			if(!lambda_CalcForce(iElem, dMaxSig, FinalForce)) continue;
		}

		// If callde from GPS, save compact forces data.
		m_pDoc->m_pPostCtrl->GetDesignResult()->Save_MeshedWallForce(iElem, FinalForce);
	}
}

////////////////////////////////////////////////////////////////////////
// Calculate moments for EC2 2way slab design - WoodArmer / Wall Force.
////////////////////////////////////////////////////////////////////////
void CDgnForceCtrl::CalcWoodArmer(double dmxx, double dmyy, double dmxy, double dalpha, double dphi, double& dUpper, double& dLower, double dVectrSBDO[3][3], BOOL bUseWoodArmer)
{
	dUpper = dLower = 0.0;
	if (bUseWoodArmer)	CalcWoodArmer(dmxx, dmyy, dmxy, dalpha, dphi, dUpper, dLower, dVectrSBDO);
	else          		  CalcNotWoodArmer(dmxx, dmyy, dmxy, dalpha, dphi, dUpper, dLower, dVectrSBDO);

	// MQC 18374 : Wood Armer Moment는 절대값으로 출력함
	dUpper = fabs(dUpper);
	dLower = fabs(dLower);
}

void CDgnForceCtrl::CalcWoodArmer(double dmxx, double dmyy, double dmxy, double dalpha, double dphi, double& dUpper, double& dLower, double dVectrSBDO[3][3])
{
	dUpper = dLower = 0.0;
	if(fabs(dphi) < cDgn_Zero) dphi=cDgn_Zero;

	// If one-way flexural design.
	if(m_bCalcOneway || m_bPlateForce)
	{
		dalpha = m_bPlateForce ? m_dPlateAngle : m_dCuttingLineAngle; // Angle 1 : Cutting Line Angle about Global-X.
		dphi   = 90.0;                 // Angle 2 : Normal angle about Angle 1.
	}

	// Deg to Rad.
	dalpha = max(dalpha, cDgn_Zero);
	dalpha *= CMathFunc::m_trrad;
	dphi   *= (-1.0*CMathFunc::m_trrad);

	double dcos2a = cos(2.*dalpha);
	double dsin2a = sin(2.*dalpha);
	double dcotp  = 1./tan(dphi);
	double dsinp  = sin(dphi);

	// Step 1 ; x,y -> xi, nu
	double dMx = (dmxx+dmyy)/2.0 + (dmxx-dmyy)/2.0*dcos2a + dmxy*dsin2a;
	double dMy = (dmxx+dmyy)/2.0 - (dmxx-dmyy)/2.0*dcos2a - dmxy*dsin2a;
	double dMxy = -1.0*(dmxx-dmyy)/2.0*dsin2a + dmxy*dcos2a;
	double dM_x=0.0, dM_y=0.0;

	//////////////////////////////////////////////////////////////////////
	// Bottom Design Forces ; Positive
	//////////////////////////////////////////////////////////////////////
	// Step 2 ; Calculate Design Force.
	// Lower
	dM_x = dMx + 2.*dMxy*dcotp + dMy*dcotp*dcotp + fabs((dMxy+dMy*dcotp)/dsinp);
	dM_y = (dMy/dsinp/dsinp) + fabs((dMxy+dMy*dcotp)/dsinp);

	// Step 3 ; Check First Design Force.
	if(max(dM_x, dM_y) < 0.0)
	{
		dM_x  = 0.0;
		dM_y = 0.0;
	}
	else if(dM_x < 0.0)
	{
		dM_x = 0.0;
		double dUp = (dMxy+dMy*dcotp)*(dMxy+dMy*dcotp);
		double dDn = dMx + 2.*dMxy*dcotp + dMy*dcotp*dcotp;
		dM_y = (dMy + fabs(dUp/dDn)) / (dsinp*dsinp);
	}
	else if(dM_y < 0.0)
	{
		dM_x  = dMx + 2.*dMxy*dcotp + dMy*dcotp*dcotp + fabs((dMxy+dMy*dcotp)*(dMxy+dMy*dcotp)/dMy);
		dM_y = 0.0;
	} 

	// Step 4 ; Check Second Design Force.
	if(m_nSlabDsgDir==0) // X
		dLower = max(dM_x, 0.0);
	else
		dLower = max(dM_y, 0.0);
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Top Design Forces ; Negative
	//////////////////////////////////////////////////////////////////////
	// Step 2 ; Calculate Design Force.
	// Upper
	dM_x  = dMx + 2.*dMxy*dcotp + dMy*dcotp*dcotp - fabs((dMxy+dMy*dcotp)/dsinp);
	dM_y = (dMy/dsinp/dsinp) - fabs((dMxy+dMy*dcotp)/dsinp);   

	// Step 3 ; Check First Design Force.
	if(min(dM_x, dM_y) > 0.0)
	{
		dM_x = 0.0;
		dM_y = 0.0;
	}
	else if(dM_x > 0.0)
	{
		dM_x = 0.0;
		double dUp = (dMxy+dMy*dcotp)*(dMxy+dMy*dcotp);
		double dDn = dMx + 2.*dMxy*dcotp + dMy*dcotp*dcotp;
		dM_y = (dMy - fabs(dUp/dDn)) / (dsinp*dsinp);
	}
	else if(dM_y > 0.0)
	{
		dM_x  = dMx + 2.*dMxy*dcotp + dMy*dcotp*dcotp - fabs((dMxy+dMy*dcotp)*(dMxy+dMy*dcotp)/dMy);
		dM_y = 0.0;
	}

	// Step 4 ; Check Second Design Force.
	if(m_nSlabDsgDir==0) // X
		dUpper = min(dM_x, 0.0);
	else
		dUpper = min(dM_y, 0.0);
	//////////////////////////////////////////////////////////////////////
}

/*
// 09.10.20 이태리 요청으로 수정. 이게 original.
void CDgnForceCtrl::CalcWoodArmer(double dmxx, double dmyy, double dmxy, double dalpha, double dphi, double& dUpper, double& dLower)
{
dUpper = 0.0;
dLower = 0.0;

// If one-way flexural design.
if(m_bCalcOneway)
{
dalpha = m_dCuttingLineAngle; // Angle 1 : Cutting Line Angle about Global-X.
dphi   = 90.;                 // Angle 2 : Normal angle about Angle 1.
}

// Deg to Rad.
dalpha = max(dalpha, cDgn_Zero);
dalpha *= CMathFunc::m_trrad;
dphi   *= CMathFunc::m_trrad;

double dcos2a = cos(2.*dalpha);
double dsin2a = sin(2.*dalpha);
double dcosp  = cos(dphi);
double dsinp  = sin(dphi);
double dsin2p = sin(2.*dphi);

// Step 1 ; x,y -> xi, nu
double dmxi = (dmxx+dmyy)/2.0 + (dmxx-dmyy)/2.0*dcos2a + dmxy*dsin2a;
double dmnu = (dmxx+dmyy)/2.0 - (dmxx-dmyy)/2.0*dcos2a - dmxy*dsin2a;
double dmxn = -1.0*(dmxx-dmyy)/2.0*dsin2a + dmxy*dcos2a;
double dmudxi=0.0, dmudeta=0.0;

//////////////////////////////////////////////////////////////////////
// Bottom Design Forces ; Positive
//////////////////////////////////////////////////////////////////////
// Step 2 ; Calculate Design Force.
// Lower
dmudxi  = dmxi - dmnu*dcosp/(1+dcosp) + dmxn*(1.-2.*dcosp)/dsinp;
dmudeta = dmnu/(1+dcosp) + dmxn/dsinp;

// Step 3 ; Check First Design Force.
if(max(dmudxi, dmudeta) < 0.0)
{
dmudxi  = 0.0;
dmudeta = 0.0;
}
else if(dmudxi < 0.0)
{
dmudxi = 0.0;
double dUp = dmxi*dmnu - dmxn*dmxn;
double dDn = dmxi*dsinp*dsinp + dmnu*dcosp*dcosp - dmxn*dsin2p;
dmudeta = dUp / dDn;
}
else if(dmudeta < 0.0)
{
dmudxi  = dmxi - dmxn*dmxn/dmnu;
dmudeta = 0.0;
} 

// Step 4 ; Check Second Design Force.
if(m_nSlabDsgDir==0) // X
dLower = max(dmudxi, 0.0);
else
dLower = max(dmudeta, 0.0);
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Top Design Forces ; Negative
//////////////////////////////////////////////////////////////////////
// Step 2 ; Calculate Design Force.
// Upper
dmudxi  = dmxi + dmnu*dcosp/(1-dcosp) - dmxn*(1.+2.*dcosp)/dsinp;
dmudeta = dmnu/(1-dcosp) - dmxn/dsinp;      

// Step 3 ; Check First Design Force.
if(min(dmudxi, dmudeta) > 0.0)
{
dmudxi  = 0.0;
dmudeta = 0.0;
}
else if(dmudxi > 0.0)
{
dmudxi = 0.0;
double dUp = dmxi*dmnu - dmxn*dmxn;
double dDn = dmxi*dsinp*dsinp + dmnu*dcosp*dcosp - dmxn*dsin2p;
dmudeta = dUp / dDn;
}
else if(dmudeta > 0.0)
{
dmudxi  = dmxi - dmxn*dmxn/dmnu;
dmudeta = 0.0;
}

// Step 4 ; Check Second Design Force.
if(m_nSlabDsgDir==0) // X
dUpper = min(dmudxi, 0.0);
else
dUpper = min(dmudeta, 0.0);
//////////////////////////////////////////////////////////////////////
}
*/


void CDgnForceCtrl::ConvertForce(UINT iElem, double &dmxx, double &dmyy, double dalpha, double dphi, double dVectrSBDO[3][3])
{
		double dmxy = 0;
	double dVectrUCS[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
	/*   memcpy(dVectrUCS, dVectrSBDO, sizeof(dVectrUCS));

		double dSbdoAng1 = CMathFunc::GetAngle(0.0, 0.0, dVectrSBDO[0][0], dVectrSBDO[0][1]);
		double dSbdoAng2 = CMathFunc::GetAngle(0.0, 0.0, dVectrSBDO[1][0], dVectrSBDO[1][1]);
		double dRotAngle = 0.0;
		switch (m_nSlabDsgDir) // Sub Domain 각도와 배근 각도의 차이만큼만 회전
		{
		case 0: dRotAngle = dSbdoAng1 - dalpha;          break; // Direction X
		case 1: dRotAngle = dSbdoAng2 - (dphi + dalpha); break; // Direction Y
		default: ASSERT(0); break;
		}
		for (int i=0; i<3; ++i)
		{
				CMathFunc::mathRotateZ(dRotAngle, dVectrUCS[i][0], dVectrUCS[i][1], dVectrUCS[i][2]);
		}*/
	m_pDoc->calcElemLocalVector(iElem, dVectrUCS);
		T_STRP_DL Strp; Strp.Initialize();
		Strp.dblStress[0][20] = dmxx;
		Strp.dblStress[0][21] = dmyy;
		Strp.dblStress[0][22] = dmxy;
		if (!m_pDoc->m_pPostCtrl->ConvertStrp_UCS(dVectrUCS, dVectrSBDO,  Strp)) { ASSERT(0); return; }
		dmxx = Strp.dblStress[0][20];
		dmyy = Strp.dblStress[0][21];
		dmxy = Strp.dblStress[0][22];

	if(fabs(dphi) < cDgn_Zero) dphi=cDgn_Zero;
	// Deg to Rad.
	dalpha = max(dalpha, cDgn_Zero);
	dalpha *= CMathFunc::m_trrad;
	dphi   *= (-1.0*CMathFunc::m_trrad);

	double dcos2a = cos(2.*dalpha);
	double dsin2a = sin(2.*dalpha);
	double dcotp  = 1./tan(dphi);
	double dsinp  = sin(dphi);

	double dMx = (dmxx+dmyy)/2.0 + (dmxx-dmyy)/2.0*dcos2a + dmxy*dsin2a;
	double dMy = (dmxx+dmyy)/2.0 - (dmxx-dmyy)/2.0*dcos2a - dmxy*dsin2a;

		dmxx = dMx;
		dmyy = dMy;
}


void CDgnForceCtrl::CalcNotWoodArmer(double dmxx, double dmyy, double dmxy, double dalpha, double dphi, double& dUpper, double& dLower, double dVectrSBDO[3][3])
{
	dUpper = dLower = 0.0;
	double dVectrUCS[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
	memcpy(dVectrUCS, dVectrSBDO, sizeof(dVectrUCS));

	double dSbdoAng1 = CMathFunc::GetAngle(0.0, 0.0, dVectrSBDO[0][0], dVectrSBDO[0][1]);
	double dSbdoAng2 = CMathFunc::GetAngle(0.0, 0.0, dVectrSBDO[1][0], dVectrSBDO[1][1]);
	double dRotAngle = 0.0;
	switch (m_nSlabDsgDir) // Sub Domain 각도와 배근 각도의 차이만큼만 회전
	{
		case 0: dRotAngle = dalpha - dSbdoAng1;          break; // Direction X
		case 1: dRotAngle = (dphi + dalpha) - dSbdoAng2; break; // Direction Y
		default: ASSERT(0); break;
	}
	for (int i=0; i<3; ++i)
	{
		CMathFunc::mathRotateZ(dRotAngle, dVectrUCS[i][0], dVectrUCS[i][1], dVectrUCS[i][2]);
	}

	T_STRP_DL Strp; Strp.Initialize();
	Strp.dblStress[0][20] = dmxx;
	Strp.dblStress[0][21] = dmyy;
	Strp.dblStress[0][22] = dmxy;
	if (!m_pDoc->m_pPostCtrl->ConvertStrp_UCS(dVectrSBDO, dVectrUCS, Strp)) { ASSERT(0); return; }

	if (m_nSlabDsgDir == 0)
	{
		double dConvertMxx = Strp.dblStress[0][20];
		dLower = max(dConvertMxx, 0.0);
		dUpper = min(dConvertMxx, 0.0);
	}
	else
	{
		double dConvertMyy = Strp.dblStress[0][21];
		dLower = max(dConvertMyy, 0.0);
		dUpper = min(dConvertMyy, 0.0);
	}
}

void CDgnForceCtrl::CalcMeshWallForce(int ElemK, double dThk, double dSigx, double dSigy, double dSigxy, double& dftdx, double& dftdy, double& dSigcd)
{
	dftdx = dftdy = dSigcd = 0.0;

	if(dThk < cDgn_Zero)
	{
		//////////////////////////////////////////////
		// <Remember> Data를 받아올때는 Code Unit기준
		T_UNIT_INDEX CurIndex, CngIndex;
		m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
		if(m_nCode_Length!=0)
		{
			CngIndex.nBase_Length = m_nCode_Length;
			CngIndex.nBase_Force  = m_nCode_Force;
		}
		else CngIndex = CurIndex;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		//////////////////////////////////////////////

		T_SBDO_K Sbdok=0;
		T_SBDO_D SbdoD;
		m_aSbdoByElem.Lookup(ElemK, Sbdok);
		if(!m_pDoc->m_pAttrCtrl->GetSbdo(Sbdok, SbdoD)) SbdoD.Initialize();
		if(!SbdoD.bUseMt && SbdoD.dThickness > 0.0) dThk = SbdoD.dThickness;
		else
		{
			T_ELEM_D ElemData;
			ElemData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemData))	ASSERT(0);
			double dt=0.0;
			m_pDoc->m_pAttrCtrl->GetThikForDesign(ElemData.elpro, dThk);
		}

		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	}

	// Set Stress.
	dSigx  /= dThk;
	dSigy  /= dThk;
	dSigxy /= dThk;

	if(max(dSigx, dSigy) < cDgn_Zero)
	{
		// do nothing.
	}

	BOOL bChange = FALSE;
	if(dSigx < dSigy)
	{
		bChange = TRUE;
		double dTempSig = dSigx;
		dSigx = dSigy;
		dSigy = dTempSig;
	}

	double dSig_xy = dSigx*dSigy;
	double dTau2   = dSigxy*dSigxy;

	//if(dSigy < 0.0 || (dSig_xy-cDgn_Zero <= dTau2))  // Need Reinforcement.
	if(TRUE)
	{
		if(dSigx-cDgn_Zero <= fabs(dSigxy))
		{
			dftdx = fabs(dSigxy) - dSigx;
			dftdy = fabs(dSigxy) - dSigy;
			dSigcd = 2.*fabs(dSigxy);
		}
		else
		{
			dftdx = 0.0;
			dftdy = dTau2/dSigx - dSigy;
			dSigcd = dSigx*(1.0+dTau2/(dSigx*dSigx));
		}
	}

	dftdx = max(dftdx, 0.0);
	dftdy = max(dftdy, 0.0);

	if(bChange)
	{
		double dTempftd = dftdx;
		dftdx = dftdy;
		dftdy = dTempftd;
	}
}

////////////////////////////////////////////////////////////////////////
// Calculate Forces
////////////////////////////////////////////////////////////////////////
BOOL CDgnForceCtrl::Set_PlateForceMoment(UINT iElem)
{
	BOOL bSuccess = FALSE;  

	switch(m_nCellOption)
	{
	case 0:
        {
            bSuccess = Set_SlabDesignData_byElem(iElem);
        }
		break;
	case 1:
		{
			if(m_iRcsTypeKind/10==8) // Slab
			{
				bSuccess = Set_SlabDesignData_byCell(iElem);
			}
			else if(m_iRcsTypeKind/10==9) // Wall
			{
				bSuccess = Set_WallDesignData_byCell(iElem);
			}
			else if(m_iRcsTypeKind/10==70) // Shell
			{
				bSuccess = Set_ShelDesignData_byCell(iElem);
			}
			break;
		}
	}

	return bSuccess;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byElem(UINT iElem)
{
	memset(m_dMuBot   , NULL, sizeof(m_dMuBot   ));// for Slab from this line.
	memset(m_dMuTop   , NULL, sizeof(m_dMuTop   ));
	memset(m_dMuDBot  , NULL, sizeof(m_dMuDBot  ));
	memset(m_dMuDTop  , NULL, sizeof(m_dMuDTop  ));
	memset(m_dMuLBot  , NULL, sizeof(m_dMuLBot  ));
	memset(m_dMuLTop  , NULL, sizeof(m_dMuLTop  ));
	memset(m_dStressx , NULL, sizeof(m_dStressx ));// for Wall from this line.
	memset(m_dStressy , NULL, sizeof(m_dStressy ));
	memset(m_dStressxy, NULL, sizeof(m_dStressxy));

	memset(m_dMxx, NULL, sizeof(m_dMxx));
	memset(m_dMyy, NULL, sizeof(m_dMyy));
	memset(m_dMxy, NULL, sizeof(m_dMxy));
	memset(m_dFxx, NULL, sizeof(m_dFxx));
	memset(m_dFyy, NULL, sizeof(m_dFyy));
	memset(m_dFxy, NULL, sizeof(m_dFxy));
	memset(m_dVxx, NULL, sizeof(m_dVxx));
	memset(m_dVyy, NULL, sizeof(m_dVyy));
	
	BOOL bOK = FALSE;
    if ( m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19 )
    {
        bOK = Set_SlabDesignData_byElem_Code_CH(iElem);
    }    
    else if ( m_iRcsTypeKind==85 )  // Slab Serviceability
    {
        bOK = Set_SlabDesignData_byElem_SlabServ(iElem);
    }		
    else if ( m_iRcsTypeKind/10==8 )  // slab
    {
        bOK = Set_SlabDesignData_byElem_Slab(iElem);
    }		
    else if ( m_iRcsTypeKind/10==9 )  // wall
    {
        bOK = Set_SlabDesignData_byElem_Wall(iElem);
    }		
    else if ( m_iRcsTypeKind/10==70 )
    {
        bOK = Set_SlabDesignData_byElem_Shell(iElem);
    }		
    else if ( m_iRcsTypeKind/10==30 )  // PMS:4352-Seungjun-20111011 EC Inclined slab.
    {
        bOK = Set_SlabDesignData_byElem_IncSlab(iElem);
    }		

	return bOK;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byElem_Slab(UINT iElem)
{
    if ( m_iDgnCode == AIJ_WSD99 && (m_nShearnAdhesive == 1 || m_nShearnAdhesive == 2) )
    {
        return Set_SlabDesignData_byElem_SlabShrJP(iElem);
    }

    if ( m_iRcsTypeKind/10!=8 )  // slab
    {
        return FALSE;
    }
    	
	T_SLAB_FORCE_COMPACT slab_forc;
    if ( !m_mapSlabForc_Compact.Lookup(iElem, slab_forc) ) { return FALSE; }

	int nie = 0;
	m_mapNodeInElem.Lookup(iElem,nie);

	//**//
    if ( IsWoodArmerBefore() )
    {
        for ( int i=0; i<nie; i++ )
        {
            m_dMuBot[i] = slab_forc.dForce[i+12];
            m_dMuTop[i] = slab_forc.dForce[i+16];
        }
    }
    else
    {
        T_SBDO_K SbdoK=0;
        T_SBDO_D SbdoD; SbdoD.Initialize();
        m_aSbdoByElem.Lookup(iElem, SbdoK);

        double dVectrSBDO[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
        GetOrSetSbdoVector(SbdoK, dVectrSBDO);

        if ( !m_bPlateForce )
        {
            if ( !m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD) ) return FALSE;
        }

        T_ELEM_D ElemD;
        if ( !m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD) ) return FALSE;

        if ( CDBLib::IsCodeForMeshedDesignAIJ_JP() && m_nShearnAdhesive !=0 )
        {
            const double dAlpha = SbdoD.dAngle[0];
            const double dPhi   = SbdoD.dAngle[1];
            for ( int i=0; i<nie; i++ )
            {
                const double dmxx = slab_forc.dForce[i];
                const double dmyy = slab_forc.dForce[i+4];
                
                m_dMxx[i] = CalcMomentByDgnDir(0, dmxx, dmyy, dAlpha, dPhi, dVectrSBDO);
                m_dMyy[i] = CalcMomentByDgnDir(1, dmxx, dmyy, dAlpha, dPhi, dVectrSBDO);

                const double dvxx = slab_forc.dForce[i+24];
                const double dvyy = slab_forc.dForce[i+28];
                
                m_dVxx[i] = CalcShearByDgnDir(0, dvxx, dvyy, dAlpha, dPhi, dVectrSBDO);
                m_dVyy[i] = CalcShearByDgnDir(1, dvxx, dvyy, dAlpha, dPhi, dVectrSBDO);
            }
        }
        else
        {
            for ( int i=0; i<nie; i++ )
            {
                double dmxx = slab_forc.dForce[i];
                double dmyy = slab_forc.dForce[i+4];
                double dmxy = slab_forc.dForce[i+8];

                double dUpper=0.0, dLower=0.0;
                CalcWoodArmer(dmxx, dmyy, dmxy, SbdoD.dAngle[0], SbdoD.dAngle[1], dUpper, dLower, dVectrSBDO, m_bWoodArmer);

                m_dMuBot[i] = dLower;
                m_dMuTop[i] = dUpper;
                
                if ( m_bTestEnvValue ) // For Test.
                {
                    rptwofstream fout(m_bTestFile, ios::app);

                    CString strVal=_T("");
                    strVal.Format(_T("Elem-%d, Node-%d  %f %f  %f  %f  %f"), iElem, ElemD.elnod[i], dmxx, dmyy, dmxy, dLower, dUpper);
                    fout<<strVal<<endl;
                }
            }
        }        
    }

	return TRUE;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byElem_SlabServ(UINT iElem)
{
	if(m_iRcsTypeKind!=85)  // Slab Serviceability
		return FALSE;

	//	T_STRP_DG	pforc;
	T_SLAB_FORCE_COMPACT slab_forc;
	if(!m_mapSlabForc_Compact.Lookup(iElem, slab_forc)) return FALSE;

	int nie;
	m_mapNodeInElem.Lookup(iElem,nie);

	T_SLAB_FORCE_COMPACT slab_forcD;  slab_forcD.initialize();
	T_SLAB_FORCE_COMPACT slab_forcL;  slab_forcL.initialize();
	BOOL bExistDL=TRUE, bExistLL=TRUE;
	if(!m_mapSlabForc_Compact_D.Lookup(iElem,slab_forcD)) bExistDL=FALSE;
	if(!m_mapSlabForc_Compact_L.Lookup(iElem,slab_forcL)) bExistLL=FALSE;

	//**//
	if(IsWoodArmerBefore())
	{
		for(int i=0; i<nie; i++)
		{
			m_dMuBot[i] = slab_forc.dForce[i+12];
			m_dMuTop[i] = slab_forc.dForce[i+16];

			// Dead Load
			if(bExistDL)
			{
				m_dMuDBot[i] = slab_forcD.dForce[i+12];
				m_dMuDTop[i] = slab_forcD.dForce[i+16];
			}
			else
			{
				m_dMuDBot[i] = 0.0;
				m_dMuDTop[i] = 0.0;
			}
			// Live Load
		  if(bExistLL)
		  {
				m_dMuLBot[i] = slab_forcD.dForce[i+12];
				m_dMuLTop[i] = slab_forcD.dForce[i+16];
		  }
		  else
		  {
			  m_dMuLBot[i] = 0.0;
			  m_dMuLTop[i] = 0.0;
		  }
		}
	}
	else
	{
	  T_SBDO_K SbdoK=0;
	  T_SBDO_D SbdoD; SbdoD.Initialize();
	  m_aSbdoByElem.Lookup(iElem, SbdoK);

	  double dVectrSBDO[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
	  GetOrSetSbdoVector(SbdoK, dVectrSBDO);

	  if(!m_bPlateForce)
	  {
		  if(!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) return FALSE;
	  }
	
	  for(int i=0; i<nie; i++)
	  {
		  // Load Combination
		  double dmxx = slab_forc.dForce[i];
		  double dmyy = slab_forc.dForce[i+4];
		  double dmxy = slab_forc.dForce[i+8];

		  double dUpper=0.0, dLower=0.0;
		  CalcWoodArmer(dmxx, dmyy, dmxy, SbdoD.dAngle[0], SbdoD.dAngle[1], dUpper, dLower, dVectrSBDO, m_bWoodArmer);

		  m_dMuBot[i] = dLower;
		  m_dMuTop[i] = dUpper;

		  // Dead Load
		  if(bExistDL)
		  {
			  dmxx = slab_forcD.dForce[i];
			  dmyy = slab_forcD.dForce[i+4];
			  dmxy = slab_forcD.dForce[i+8];

			  dUpper=0.0; dLower=0.0;
			  CalcWoodArmer(dmxx, dmyy, dmxy, SbdoD.dAngle[0], SbdoD.dAngle[1], dUpper, dLower, dVectrSBDO, m_bWoodArmer);

			  m_dMuDBot[i] = dLower;
			  m_dMuDTop[i] = dUpper;
		  }
		  else
		  {
			  m_dMuDBot[i] = 0.0;
			  m_dMuDTop[i] = 0.0;
		  }

		  // Live Load
		  if(bExistLL)
		  {
			  dmxx = slab_forcL.dForce[i];
			  dmyy = slab_forcL.dForce[i+4];
			  dmxy = slab_forcL.dForce[i+8];

			  dUpper=0.0; dLower=0.0;
			  CalcWoodArmer(dmxx, dmyy, dmxy, SbdoD.dAngle[0], SbdoD.dAngle[1], dUpper, dLower, dVectrSBDO, m_bWoodArmer);

			  m_dMuLBot[i] = dLower;
			  m_dMuLTop[i] = dUpper;
		  }
		  else
		  {
			  m_dMuLBot[i] = 0.0;
			  m_dMuLTop[i] = 0.0;
		  }
	  }
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byElem_SlabShrJP(UINT iElem)
{
    if ( m_iRcsTypeKind/10!=8 )  // slab
    {
        return FALSE;
    }

    T_SLAB_FORCE_COMPACT slab_forc;
    if ( !m_mapSlabForc_Compact.Lookup(iElem, slab_forc) ) { return FALSE; }

    int nie = 0;
    if ( !m_mapNodeInElem.Lookup(iElem, nie) ) { ASSERT(0); }

    T_SBDO_K SbdoK = 0;    
    if ( !m_aSbdoByElem.Lookup(iElem, SbdoK) ) { ASSERT(0); }

    double dVectrSBDO[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
    GetOrSetSbdoVector(SbdoK, dVectrSBDO);

    T_SBDO_D SbdoD; SbdoD.Initialize();
    if ( !m_bPlateForce )
    {
        if ( !m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD) ) { return FALSE; }
    }

    T_ELEM_D ElemD;
    if ( !m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD) ) { return FALSE; }

    const double dAlpha = SbdoD.dAngle[0];
    const double dPhi   = SbdoD.dAngle[1];
    for ( int i=0; i<nie; i++ )
    {
        const double dmxx = slab_forc.dForce[i];
        const double dmyy = slab_forc.dForce[i+4];

        m_dMxx[i] = CalcMomentByDgnDir(m_nSlabDsgDir, dmxx, dmyy, dAlpha, dPhi, dVectrSBDO);
        
        const double dvxx = slab_forc.dForce[i+24];
        const double dvyy = slab_forc.dForce[i+28];

        m_dVxx[i] = CalcShearByDgnDir(m_nSlabDsgDir, dvxx, dvyy, dAlpha, dPhi, dVectrSBDO);
    }

    return TRUE;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byElem_Wall(UINT iElem)
{
	if(m_iRcsTypeKind/10!=9)  // wall
		return FALSE;

	//	T_STRP_DG	pforc;
	T_SLAB_FORCE_COMPACT slab_forc;
	if(!m_mapSlabForc_Compact.Lookup(iElem,slab_forc)) return FALSE;

	T_SBDO_K SbdoK=0;
	T_SBDO_D SbdoD; SbdoD.Initialize();
	m_aSbdoByElem.Lookup(iElem, SbdoK);

	T_ELEM_D ElemD;
	if(!m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD)) return FALSE;

	int nie;
	m_mapNodeInElem.Lookup(iElem,nie);
	
	// Compression is positive.
	for(int i=0; i<nie; i++)
	{
		m_dStressx[i]  = (-1.)*slab_forc.dForce[   i];
		m_dStressy[i]  = (-1.)*slab_forc.dForce[ 4+i];
		m_dStressxy[i] =       slab_forc.dForce[ 8+i];

		// For Test.
		if(m_bTestEnvValue)
		{
			double dftdx=0.0, dftdy=0.0, dSigxy=0.0;
			CalcMeshWallForce(iElem, 0.0, m_dStressx[i], m_dStressy[i], m_dStressxy[i], dftdx, dftdy, dSigxy);

			rptwofstream fout(m_bTestFile, ios::app);

			CString strVal=_T("");
			strVal.Format(_T("Elem-%d, Node-%d  %f %f  %f  %f  %f %f"), iElem, ElemD.elnod[i], m_dStressx[i], m_dStressy[i], m_dStressxy[i], dftdx, dftdy, dSigxy);
			fout<<strVal<<endl;
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byElem_IncSlab(UINT iElem)
{
	if(m_iRcsTypeKind/10!=30)  // PMS:4352-Seungjun-20111011 EC Inclined slab.
		return FALSE;

	//	T_STRP_DG	pforc;
	T_SLAB_FORCE_COMPACT slab_forc;
	if(!m_mapSlabForc_Compact.Lookup(iElem,slab_forc)) return FALSE;

	int nie;
	m_mapNodeInElem.Lookup(iElem,nie);
	for(int i=0; i<nie; i++)
	{
		m_dMxx[i] = slab_forc.dForce[   i];
		m_dMyy[i] = slab_forc.dForce[ 4+i];
		m_dMxy[i] = slab_forc.dForce[ 8+i];
		m_dFxx[i] = slab_forc.dForce[12+i];
		m_dFyy[i] = slab_forc.dForce[16+i];
		m_dFxy[i] = slab_forc.dForce[20+i];
		m_dVxx[i] = slab_forc.dForce[24+i];
		m_dVyy[i] = slab_forc.dForce[28+i];
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byElem_Code_CH(UINT iElem)
{
	if (m_iRcsTypeKind != 85 && m_iRcsTypeKind / 10 != 8)
		return FALSE;
	T_SLAB_FORCE_COMPACT slab_forc;
	if (!m_mapSlabForc_Compact.Lookup(iElem, slab_forc))
		return FALSE;
		//////////////////////////////////////////////////////////////////////////
		m_bWoodArmer = FALSE;
		T_SBDO_K SbdoK=0;
		T_SBDO_D SbdoD; SbdoD.Initialize();
		if(!m_aSbdoByElem.Lookup(iElem, SbdoK))
		{
				ASSERT(FALSE);
				return FALSE;
		}
		double dVectrSBDO[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
		GetOrSetSbdoVector(SbdoK, dVectrSBDO);
	 if(!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) 
			 return FALSE;
	 int i=0;
	 auto lambda_NodalMax = [&](double dTemp[4])
	 {
		 double dRlt = 0.0;
		 for(int i = 0;i < 4;i++)
		 {
			 if(fabs(dTemp[i]) > fabs(dRlt))
				 dRlt = dTemp[i];
		 }
		 return dRlt;
	 };
	 m_dMxx[0] = m_bAverage ? lambda_NodalMax(&slab_forc.dForce[0])		: slab_forc.dForce[0];
	 m_dMyy[0] = m_bAverage ? lambda_NodalMax(&slab_forc.dForce[4])		: slab_forc.dForce[4];
	 m_dMxy[0] = m_bAverage ? lambda_NodalMax(&slab_forc.dForce[8])		: slab_forc.dForce[8];
	 m_dFxx[0] = m_bAverage ? lambda_NodalMax(&slab_forc.dForce[12])	: slab_forc.dForce[12];
	 m_dFyy[0] = m_bAverage ? lambda_NodalMax(&slab_forc.dForce[16])	: slab_forc.dForce[16];
	 m_dFxy[0] = m_bAverage ? lambda_NodalMax(&slab_forc.dForce[20])	: slab_forc.dForce[20];
	 m_dVxx[0] = m_bAverage ? lambda_NodalMax(&slab_forc.dForce[24])	: slab_forc.dForce[24];
	 m_dVyy[0] = m_bAverage ? lambda_NodalMax(&slab_forc.dForce[28])	: slab_forc.dForce[28];
	 //ConvertForce(iElem, m_dMxx[0], m_dMyy[0], SbdoD.dAngle[0], SbdoD.dAngle[1],  dVectrSBDO);
	 //ConvertForce(iElem, m_dFxx[0], m_dFyy[0], SbdoD.dAngle[0], SbdoD.dAngle[1],  dVectrSBDO);
	int nie;
	m_mapNodeInElem.Lookup(iElem, nie);
	for (int i = 0; i<nie; i++)
	{
		//m_dMxx[i] = slab_forc.dForce[i];
		//m_dMyy[i] = slab_forc.dForce[4 + i];
		//m_dMxy[i] = slab_forc.dForce[8 + i];
		//m_dFxx[i] = slab_forc.dForce[12 + i];
		//m_dFyy[i] = slab_forc.dForce[16 + i];
		//m_dFxy[i] = slab_forc.dForce[20 + i];
		//m_dVxx[i] = slab_forc.dForce[24 + i];
		//m_dVyy[i] = slab_forc.dForce[28 + i];
		if(i>0)
		{
			m_dMxx[i] = m_dMxx[0];
			m_dMyy[i] = m_dMyy[0];
			m_dMxy[i] = m_dMxy[0];
			m_dFxx[i] = m_dFxx[0];
			m_dFyy[i] = m_dFyy[0];
			m_dFxy[i] = m_dFxy[0];
			m_dVxx[i] = m_dVxx[0];
			m_dVyy[i] = m_dVyy[0];
		}    
		if (m_nSlabDsgDir == 0)
		{
			m_dMuBot[i] = m_dMxx[i];
			m_dMuTop[i] = m_dMxx[i];
		}
		else
		{
			m_dMuBot[i] = m_dMyy[i];
			m_dMuTop[i] = m_dMyy[i];
		}
	}
	return TRUE;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byElem_Shell(UINT nElem)
{
	BOOL bKindOK = FALSE;
	switch (m_iRcsTypeKind)
	{
	case RCS_SHFD:
	case RCS_SHFC:
	case RCS_SHSC:
		bKindOK = TRUE;
		break;
	default:
		break;
	}

	if (!bKindOK) return FALSE;
		
	T_SLAB_FORCE_COMPACT slab_forc;
	if (!m_mapSlabForc_Compact.Lookup(nElem, slab_forc)) { return FALSE; }

	int nNodeInElem = 0;
	if (!m_mapNodeInElem.Lookup(nElem, nNodeInElem)) { return FALSE; }

	for (int i=0; i<nNodeInElem; ++i)
	{
		m_dMxx[i] = slab_forc.dForce[   i];
		m_dMyy[i] = slab_forc.dForce[ 4+i];
		m_dMxy[i] = slab_forc.dForce[ 8+i];
		m_dFxx[i] = slab_forc.dForce[12+i];
		m_dFyy[i] = slab_forc.dForce[16+i];
		m_dFxy[i] = slab_forc.dForce[20+i];
		m_dVxx[i] = slab_forc.dForce[24+i];
		m_dVyy[i] = slab_forc.dForce[28+i];
		m_dSigMaxTop[i] = slab_forc.dForce[32+i];
		m_dSigMinTop[i] = slab_forc.dForce[36+i];
		m_dSigMaxBot[i] = slab_forc.dForce[40+i];
		m_dSigMinBot[i] = slab_forc.dForce[44+i];
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byCell(UINT iElem)
{
	memset(m_dMuBot , NULL, sizeof(m_dMuBot ));
	memset(m_dMuTop , NULL, sizeof(m_dMuTop ));
	memset(m_dMuDTop, NULL, sizeof(m_dMuDTop));
	memset(m_dMuDBot, NULL, sizeof(m_dMuDBot));
	memset(m_dMuLTop, NULL, sizeof(m_dMuLTop));
	memset(m_dMuLBot, NULL, sizeof(m_dMuLBot));

	// Get sub-domain Key.
	T_SBDO_K SbdoK;
	if(!m_aSbdoByElem.Lookup(iElem, SbdoK)) return FALSE;

	T_ELEM_D ElemD;
	if(!m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD)) return FALSE;

	int nNode = GetPlateNodeNum(iElem, ElemD);
	if(nNode==0) return FALSE;

	T_SLAB_FORCE_COMPACT slab_forcbycell;
	slab_forcbycell.initialize();
	T_SLAB_FORCE_COMPACT slab_forc1;
	if(!m_mapSlabForc_Compact.Lookup(iElem, slab_forc1)) slab_forc1.initialize();

	DWORD stTick = 0.0;
	DWORD edTick = 0.0;

	for(int i=0; i<nNode; i++)
	{
		T_NODE_K NodeK = ElemD.elnod[i];
		T_NODE_D NodeD;
		if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD))  continue;

		double dMu[5]={0.0, 0.0, 0.0, 0.0, 0.0}; //**//
		double dLen=0.0;
		double dMu11 = slab_forc1.dForce[i   ];
		double dMu12 = slab_forc1.dForce[i+ 4];
		double dMu13 = slab_forc1.dForce[i+ 8];
		double dMu14 = slab_forc1.dForce[i+12]; //**//
		double dMu15 = slab_forc1.dForce[i+16]; //**//

		_DGN_CELL_FORC arNodeElemList; arNodeElemList.Initialize();
		int nCellNode = Get_SlabNode_In_Cell(SbdoK, NodeK, arNodeElemList);
		stTick = GetTickCount();
		if(nCellNode==1)
		{
            slab_forcbycell.dForce[i] = dMu11;
            slab_forcbycell.dForce[i + 4] = dMu12;
            slab_forcbycell.dForce[i + 8] = dMu13;
            //**//
            if (IsWoodArmerBefore())
            {
                slab_forcbycell.dForce[i + 12] = dMu12;
                slab_forcbycell.dForce[i + 16] = dMu13;
            }

// 			double stTick3 = GetTickCount();
// 			if(!Set_SlabDesignData_byElem(iElem)) return TRUE;
// 			double edTick3 = GetTickCount();
// 			m_dTimeChk[7] += (edTick3-stTick3);
// 			return TRUE;
		}
		else
		{
			memset(dMu, NULL, sizeof(dMu));

			double stTick2 = GetTickCount();
			POSITION pos = arNodeElemList.arNodeElemList.GetStartPosition();
			while(pos)
			{
				T_NODE_K NextNodeK;
				T_KEY_LIST_EX ElemListK;
				arNodeElemList.arNodeElemList.GetNextAssoc(pos, NextNodeK, ElemListK);
				if(NodeK==NextNodeK)  continue;

				int nElemNum = static_cast<int>(ElemListK.GetSize());
				double stTick1 = GetTickCount();
				for(int kk=0; kk<nElemNum; kk++)
				{
					T_ELEM_K NextElemK = ElemListK[kk];
					// PMS:4352-Seungjun-20111012 EC Inclined slab.
					double local_vector[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
					m_pDoc->calcPlaneLocalVector(NextElemK, local_vector);
					BOOL bInclined = TRUE;
					// Add/Modify by Seungjun MQC:5035  '20120802 경사판단 수정.
					if(fabs(fabs(local_vector[2][2])-1.0) < cDgn_Zero) bInclined = FALSE;

					T_ELEM_D NextElemD;
					if(!m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD)) continue;

					int nNode_in_elem = GetPlateNodeNum(NextElemK, NextElemD);
					for(int jj=0; jj<nNode_in_elem; jj++)
					{
						if(NextNodeK == NextElemD.elnod[jj])
						{
							T_SLAB_FORCE_COMPACT slab_forc2;
							slab_forc2.initialize();
							if(!m_mapSlabForc_Compact.Lookup(NextElemK, slab_forc2)) continue;

							T_NODE_D NodeD2;
							m_pDoc->m_pAttrCtrl->GetNode(NextElemD.elnod[jj], NodeD2);
							double dLength = m_nSlabDsgDir==0 ? fabs(NodeD.x-NodeD2.x) : fabs(NodeD.y-NodeD2.y);
							// PMS:4352-Seungjun-20111012 EC Inclined slab.
							if(bInclined)
							{
								double UCS_vector[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};  // 기준이 되는 Wall/Slab의 Local Direction.
								T_VECTOR3BY3D LVec; LVec.Initialize();
								int nSSdomain=0;

								auto itrFind = m_aSubSubDomain.find(NextElemK);
								if(itrFind == m_aSubSubDomain.end()) 
								{
									LVec.Initialize();
									m_aSubSubDomain.clear();
									m_aSubSubDomainVec.clear();
									m_pDoc->m_pPostCtrl->Get_SBDO_LocalVector(SbdoK, UCS_vector, m_aSubSubDomain, m_aSubSubDomainVec);

									NextElemK  = itrFind->first;
									nSSdomain = itrFind->second;

									auto itrFindVec = m_aSubSubDomainVec.find(nSSdomain);
									if(itrFindVec == m_aSubSubDomainVec.end()) LVec.Initialize();
									else
									{
										nSSdomain = itrFindVec->first;
										LVec      = itrFindVec->second;
									}
								}               
								else
								{
									NextElemK  = itrFind->first;
									nSSdomain = itrFind->second;

									auto itrFindVec = m_aSubSubDomainVec.find(nSSdomain);
									if(itrFindVec == m_aSubSubDomainVec.end()) LVec.Initialize();
									else
									{
										nSSdomain = itrFindVec->first;
										LVec      = itrFindVec->second;
									}
								}
								//////////////////////////////////////////////////////////////////////////

								T_NODE_D RotNodeD1, RotNodeD2;
								RotNodeD1.Initialize();
								RotNodeD2.Initialize();

								RotNodeD1.x = LVec.dVector[0][0]*NodeD.x + LVec.dVector[0][1]*NodeD.y + LVec.dVector[0][2]*NodeD.z;
								RotNodeD1.y = LVec.dVector[1][0]*NodeD.x + LVec.dVector[1][1]*NodeD.y + LVec.dVector[1][2]*NodeD.z;
								RotNodeD1.z = LVec.dVector[2][0]*NodeD.x + LVec.dVector[2][1]*NodeD.y + LVec.dVector[2][2]*NodeD.z;

								RotNodeD2.x = LVec.dVector[0][0]*NodeD2.x + LVec.dVector[0][1]*NodeD2.y + LVec.dVector[0][2]*NodeD2.z;
								RotNodeD2.y = LVec.dVector[1][0]*NodeD2.x + LVec.dVector[1][1]*NodeD2.y + LVec.dVector[1][2]*NodeD2.z;
								RotNodeD2.z = LVec.dVector[2][0]*NodeD2.x + LVec.dVector[2][1]*NodeD2.y + LVec.dVector[2][2]*NodeD2.z;

								double dDiffx = (RotNodeD1.x-RotNodeD2.x);
								double dDiffy = (RotNodeD1.y-RotNodeD2.y);
								double dDiffz = (RotNodeD1.z-RotNodeD2.z);

								if(m_nSlabDsgDir==0)  dLength = sqrt(dDiffx*dDiffx + dDiffz*dDiffz);
								else                  dLength = sqrt(dDiffy*dDiffy + dDiffz*dDiffz);
							}
							double dMu21 = slab_forc2.dForce[jj   ];
							double dMu22 = slab_forc2.dForce[jj+ 4];
							double dMu23 = slab_forc2.dForce[jj+ 8];
							double dMu24 = slab_forc2.dForce[jj+12]; //**//
							double dMu25 = slab_forc2.dForce[jj+16]; //**//
							dMu[0] += (dMu11+dMu21)*dLength/2.0;
							dMu[1] += (dMu12+dMu22)*dLength/2.0;
							dMu[2] += (dMu13+dMu23)*dLength/2.0;
							dMu[3] += (dMu14+dMu24)*dLength/2.0; //**//
							dMu[4] += (dMu15+dMu25)*dLength/2.0; //**//
							dLen   += dLength;
							break;
						}
					}
				}
				double edTick1 = GetTickCount();
				m_dTimeChk[5] += (edTick1-stTick1);
			}
			double edTick2 = GetTickCount();
			m_dTimeChk[6] += (edTick2-stTick2);

			if(dLen > 0.0)
			{
				slab_forcbycell.dForce[i   ] = dMu[0] / dLen;
				slab_forcbycell.dForce[i+ 4] = dMu[1] / dLen;
				slab_forcbycell.dForce[i+ 8] = dMu[2] / dLen;
				//**//
				if(IsWoodArmerBefore())
				{
					slab_forcbycell.dForce[i+12] = dMu[1] / dLen;
					slab_forcbycell.dForce[i+16] = dMu[2] / dLen;
				}
			}
			// MQC:3364-Seungjun-20100512 dLen가 0.0인 경우 자기 부재력으로 처리. nCellNode==1 경우와 동일.
			else
			{
                slab_forcbycell.dForce[i    ] = dMu11;
                slab_forcbycell.dForce[i + 4] = dMu12;
                slab_forcbycell.dForce[i + 8] = dMu13;
                //**//
                if (IsWoodArmerBefore())
                {
                    slab_forcbycell.dForce[i + 12] = dMu12;
                    slab_forcbycell.dForce[i + 16] = dMu13;
                }
				//Set_SlabDesignData_byElem(iElem);
				//return TRUE;
			}
		}
		edTick = GetTickCount();
		m_dTimeChk[3] += (edTick-stTick);
	}

	m_mapSlabForc_CompactbyCell.SetAt(iElem, slab_forcbycell);
	//**//
	stTick = GetTickCount();
	if(IsWoodArmerBefore())
	{
		for(int i=0; i<nNode; i++)
		{
			m_dMuBot[i] = slab_forcbycell.dForce[i+12];
			m_dMuTop[i] = slab_forcbycell.dForce[i+16];
		}
	}
	else
	{
	  T_SBDO_D SbdoD; SbdoD.Initialize();
	  m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD);

	  double dVectrSBDO[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
	  GetOrSetSbdoVector(SbdoK, dVectrSBDO);

	  for(int i=0; i<nNode; i++)
	  {
		  double dmxx = slab_forcbycell.dForce[i];
		  double dmyy = slab_forcbycell.dForce[i+4];
		  double dmxy = slab_forcbycell.dForce[i+8];

		  double dUpper=0.0, dLower=0.0;
		  CalcWoodArmer(dmxx, dmyy, dmxy, SbdoD.dAngle[0], SbdoD.dAngle[1], dUpper, dLower, dVectrSBDO, m_bWoodArmer);

		  m_dMuBot[i] = dLower;
		  m_dMuTop[i] = dUpper;
	  }
	}
	edTick = GetTickCount();
	m_dTimeChk[4] += (edTick-stTick);

	return TRUE;
}

BOOL CDgnForceCtrl::Set_SlabDesignData_byCell_SlabShrJP(UINT ElemK)
{
    memset(m_dMxx, 0.0, sizeof(m_dMxx));
    memset(m_dVxx, 0.0, sizeof(m_dVxx));
    
    T_SBDO_K SbdoK = 0;
    if ( !m_aSbdoByElem.Lookup(ElemK, SbdoK) ) { return FALSE; }

    T_ELEM_D ElemD;
    if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { return FALSE; }

    const int nNode = GetPlateNodeNum(ElemK, ElemD);
    if ( nNode==0 ) { return FALSE; }

    T_SLAB_FORCE_COMPACT ForcByCell;
    ForcByCell.initialize();

    T_SLAB_FORCE_COMPACT ForcByElem1;
    if ( !m_mapSlabForc_Compact.Lookup(ElemK, ForcByElem1) ) { ForcByElem1.initialize(); }

    DWORD stTick = 0.0;
    DWORD edTick = 0.0;

    for ( int i=0; i<nNode; ++i )
    {
        T_NODE_K NodeK = ElemD.elnod[i];
        T_NODE_D NodeD;
        if ( !m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD) ) { continue; }
        
        double adForc[4] = { 0.0, };
        double dLen  = 0.0;

        double dmxx1 = ForcByElem1.dForce[i];
        double dmyy1 = ForcByElem1.dForce[i+ 4];
        double dvxx1 = ForcByElem1.dForce[i+24];
        double dvyy1 = ForcByElem1.dForce[i+28];
        
        _DGN_CELL_FORC NodeInCell; NodeInCell.Initialize();
        const int nCellNode = Get_SlabNode_In_Cell(SbdoK, NodeK, NodeInCell);
        
        stTick = GetTickCount();
        
        if ( nCellNode == 1 )
        {
            double stTick3 = GetTickCount();
            if ( !Set_SlabDesignData_byElem_SlabShrJP(ElemK) ) { return TRUE; }
            double edTick3 = GetTickCount();
            m_dTimeChk[7] += (edTick3-stTick3);
            
            return TRUE;
        }
        else
        {
            memset(adForc, 0.0, sizeof(adForc));

            double stTick2 = GetTickCount();
            POSITION pos = NodeInCell.arNodeElemList.GetStartPosition();
            while ( pos )
            {
                T_NODE_K NextNodeK;
                T_KEY_LIST_EX ElemListK;
                NodeInCell.arNodeElemList.GetNextAssoc(pos, NextNodeK, ElemListK);
                if ( NodeK == NextNodeK ) { continue; }

                const INT_PTR nElemNum = ElemListK.GetSize();
                double stTick1 = GetTickCount();
                for ( INT_PTR kk=0; kk<nElemNum; ++kk )
                {
                    T_ELEM_K NextElemK = ElemListK[kk];
                    // PMS:4352-Seungjun-20111012 EC Inclined slab.
                    double local_vector[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
                    m_pDoc->calcPlaneLocalVector(NextElemK, local_vector);
                    BOOL bInclined = TRUE;
                    // Add/Modify by Seungjun MQC:5035  '20120802 경사판단 수정.
                    if ( fabs(fabs(local_vector[2][2])-1.0) < cDgn_Zero ) { bInclined = FALSE; }

                    T_ELEM_D NextElemD;
                    if ( !m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD) ) { continue; }

                    const int nNode_in_elem = GetPlateNodeNum(NextElemK, NextElemD);
                    for ( int jj=0; jj<nNode_in_elem; jj++ )
                    {
                        if ( NextNodeK == NextElemD.elnod[jj] )
                        {
                            T_SLAB_FORCE_COMPACT ForcByElem2;
                            ForcByElem2.initialize();
                            if ( !m_mapSlabForc_Compact.Lookup(NextElemK, ForcByElem2) ) { continue; }

                            T_NODE_D NodeD2;
                            m_pDoc->m_pAttrCtrl->GetNode(NextElemD.elnod[jj], NodeD2);
                            double dLength = m_nSlabDsgDir==0 ? fabs(NodeD.x-NodeD2.x) : fabs(NodeD.y-NodeD2.y);
                            // PMS:4352-Seungjun-20111012 EC Inclined slab.
                            if ( bInclined )
                            {
                                double UCS_vector[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };  // 기준이 되는 Wall/Slab의 Local Direction.
                                T_VECTOR3BY3D LVec; LVec.Initialize();
                                int nSSdomain=0;

                                auto itrFind = m_aSubSubDomain.find(NextElemK);
                                if ( itrFind == m_aSubSubDomain.end() )
                                {
                                    LVec.Initialize();
                                    m_aSubSubDomain.clear();
                                    m_aSubSubDomainVec.clear();
                                    m_pDoc->m_pPostCtrl->Get_SBDO_LocalVector(SbdoK, UCS_vector, m_aSubSubDomain, m_aSubSubDomainVec);

                                    NextElemK  = itrFind->first;
                                    nSSdomain = itrFind->second;

                                    auto itrFindVec = m_aSubSubDomainVec.find(nSSdomain);
                                    if ( itrFindVec == m_aSubSubDomainVec.end() ) LVec.Initialize();
                                    else
                                    {
                                        nSSdomain = itrFindVec->first;
                                        LVec      = itrFindVec->second;
                                    }
                                }
                                else
                                {
                                    NextElemK  = itrFind->first;
                                    nSSdomain = itrFind->second;

                                    auto itrFindVec = m_aSubSubDomainVec.find(nSSdomain);
                                    if ( itrFindVec == m_aSubSubDomainVec.end() ) LVec.Initialize();
                                    else
                                    {
                                        nSSdomain = itrFindVec->first;
                                        LVec      = itrFindVec->second;
                                    }
                                }
                                //////////////////////////////////////////////////////////////////////////

                                T_NODE_D RotNodeD1, RotNodeD2;
                                RotNodeD1.Initialize();
                                RotNodeD2.Initialize();

                                RotNodeD1.x = LVec.dVector[0][0]*NodeD.x + LVec.dVector[0][1]*NodeD.y + LVec.dVector[0][2]*NodeD.z;
                                RotNodeD1.y = LVec.dVector[1][0]*NodeD.x + LVec.dVector[1][1]*NodeD.y + LVec.dVector[1][2]*NodeD.z;
                                RotNodeD1.z = LVec.dVector[2][0]*NodeD.x + LVec.dVector[2][1]*NodeD.y + LVec.dVector[2][2]*NodeD.z;

                                RotNodeD2.x = LVec.dVector[0][0]*NodeD2.x + LVec.dVector[0][1]*NodeD2.y + LVec.dVector[0][2]*NodeD2.z;
                                RotNodeD2.y = LVec.dVector[1][0]*NodeD2.x + LVec.dVector[1][1]*NodeD2.y + LVec.dVector[1][2]*NodeD2.z;
                                RotNodeD2.z = LVec.dVector[2][0]*NodeD2.x + LVec.dVector[2][1]*NodeD2.y + LVec.dVector[2][2]*NodeD2.z;

                                double dDiffx = (RotNodeD1.x-RotNodeD2.x);
                                double dDiffy = (RotNodeD1.y-RotNodeD2.y);
                                double dDiffz = (RotNodeD1.z-RotNodeD2.z);

                                if ( m_nSlabDsgDir==0 )
                                {
                                    dLength = sqrt(dDiffx*dDiffx + dDiffz*dDiffz);
                                }
                                else
                                {
                                    dLength = sqrt(dDiffy*dDiffy + dDiffz*dDiffz);
                                }
                            }

                            double dmxx2 = ForcByElem2.dForce[jj];
                            double dmyy2 = ForcByElem2.dForce[jj+ 4];
                            double dvxx2 = ForcByElem2.dForce[jj+24];
                            double dvyy2 = ForcByElem2.dForce[jj+28];
                            
                            adForc[0] += (dmxx1+dmxx2)*dLength/2.0;
                            adForc[1] += (dmyy1+dmyy2)*dLength/2.0;
                            adForc[2] += (dvxx1+dvxx2)*dLength/2.0;
                            adForc[3] += (dvyy1+dvyy2)*dLength/2.0;
                            
                            dLen += dLength;
                            break;
                        }
                    }
                }
                double edTick1 = GetTickCount();
                m_dTimeChk[5] += (edTick1-stTick1);
            }
            double edTick2 = GetTickCount();
            m_dTimeChk[6] += (edTick2-stTick2);

            if ( dLen > 0.0 )
            {
                ForcByCell.dForce[i]    = adForc[0] / dLen;
                ForcByCell.dForce[i+ 4] = adForc[1] / dLen;
                ForcByCell.dForce[i+24] = adForc[0] / dLen;
                ForcByCell.dForce[i+28] = adForc[1] / dLen;
            }            
            else // MQC:3364-Seungjun-20100512 dLen가 0.0인 경우 자기 부재력으로 처리. nCellNode==1 경우와 동일.
            {
                Set_SlabDesignData_byElem_SlabShrJP(ElemK);
                return TRUE;
            }
        }
        edTick = GetTickCount();
        m_dTimeChk[3] += (edTick-stTick);
    }

    m_mapSlabForc_CompactbyCell.SetAt(ElemK, ForcByCell);
    
    stTick = GetTickCount();

    T_SBDO_D SbdoD; SbdoD.Initialize();
    m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD);

    double dVectrSBDO[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
    GetOrSetSbdoVector(SbdoK, dVectrSBDO);

    const double dAlpha = SbdoD.dAngle[0];
    const double dPhi   = SbdoD.dAngle[1];
    for ( int i=0; i<nNode; i++ )
    {
        const double dmxx = ForcByCell.dForce[i];
        const double dmyy = ForcByCell.dForce[i+4];

        m_dMxx[i] = CalcMomentByDgnDir(m_nSlabDsgDir, dmxx, dmyy, dAlpha, dPhi, dVectrSBDO);

        const double dvxx = ForcByCell.dForce[i+24];
        const double dvyy = ForcByCell.dForce[i+28];

        m_dVxx[i] = CalcShearByDgnDir(m_nSlabDsgDir, dvxx, dvyy, dAlpha, dPhi, dVectrSBDO);
    }
    
    edTick = GetTickCount();
    m_dTimeChk[4] += (edTick-stTick);

    return TRUE;
}

BOOL CDgnForceCtrl::Set_WallDesignData_byCell(UINT iElem)
{
	memset(m_dMuBot   , NULL, sizeof(m_dMuBot   ));
	memset(m_dMuTop   , NULL, sizeof(m_dMuTop   ));
	memset(m_dStressx , NULL, sizeof(m_dStressx ));
	memset(m_dStressy , NULL, sizeof(m_dStressy ));
	memset(m_dStressxy, NULL, sizeof(m_dStressxy));

	// Get sub-domain Key.
	T_SBDO_K SbdoK;
	if(!m_aSbdoByElem.Lookup(iElem, SbdoK)) return FALSE;

	T_ELEM_D ElemD;
	if(!m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD)) return FALSE;

	int nNode = GetPlateNodeNum(iElem, ElemD);
	if(nNode==0) return FALSE;

	T_SLAB_FORCE_COMPACT slab_forc1;
	m_mapSlabForc_Compact.Lookup(iElem, slab_forc1);

	// 층별 smoothing.
	BOOL bExistStory = ExistStory();
	T_STOR_K ElemStorK = 0;
	if(bExistStory)
	{
		if(!m_arStorbyElem.Lookup(iElem, ElemStorK)) bExistStory = FALSE;
	}

	for(int i=0; i<nNode; i++)
	{
		T_NODE_K NodeK = ElemD.elnod[i];
		T_NODE_D NodeD;
		if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD))  continue;

		double dStrX   = 0.0;
		double dStrY   = 0.0;
		double dStrXY  = 0.0;
		double dLen    = 0.0;
		double dBotX1  = slab_forc1.dForce[i];
		double dBotY1  = slab_forc1.dForce[i+4];
		double dBotXY1 = slab_forc1.dForce[i+8];

		_DGN_CELL_FORC arNodeElemList;
		int nCellNode = Get_WallNode_In_Cell(SbdoK, NodeK, arNodeElemList);
		if(nCellNode==1)
		{
			if(!Set_SlabDesignData_byElem(iElem)) return FALSE;
		}
		else
		{
			POSITION pos = arNodeElemList.arNodeElemList.GetStartPosition();
			while(pos)
			{
				T_NODE_K NextNodeK;
				T_KEY_LIST_EX ElemListK;
				arNodeElemList.arNodeElemList.GetNextAssoc(pos, NextNodeK, ElemListK);
				if(NodeK==NextNodeK)  continue;

				int nElemNum = static_cast<int>(ElemListK.GetSize());
				for(int kk=0; kk<nElemNum; kk++)
				{
					T_ELEM_K NextElemK = ElemListK[kk];
					if(bExistStory)
					{
						T_STOR_K NextStorK = 0;
						if(!m_arStorbyElem.Lookup(iElem, NextStorK)) continue;
						if(ElemStorK!=NextStorK) continue;
					}

					T_ELEM_D NextElemD;
					if(!m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD)) continue;
					int nNode_in_elem = GetPlateNodeNum(NextElemK, NextElemD);

					T_SLAB_FORCE_COMPACT slab_forc2;
					m_mapSlabForc_Compact.Lookup(NextElemK, slab_forc2);
					for(int jj=0; jj<nNode_in_elem; jj++)
					{
						if(NextNodeK == NextElemD.elnod[jj])
						{
							T_NODE_D NodeD2;
							m_pDoc->m_pAttrCtrl->GetNode(NextElemD.elnod[jj], NodeD2);
							double dLength = CMathFunc::mathLength(NodeD.x, NodeD.y, NodeD.z, NodeD2.x, NodeD2.y, NodeD2.z);
							double dBotX2  = slab_forc2.dForce[jj];
							double dBotY2  = slab_forc2.dForce[jj+4];
							double dBotXY2 = slab_forc2.dForce[jj+8];

							dStrX  += (dBotX1+dBotX2)*dLength/2.0;
							dStrY  += (dBotY1+dBotY2)*dLength/2.0;
							dStrXY += (dBotXY1+dBotXY2)*dLength/2.0;
							dLen   += dLength;
							break;
						}
					}
				}
			}

			if(dLen > 0.0)
			{
				// Compression is positive.
				m_dStressx[i]  = (-1.)*dStrX / dLen;
				m_dStressy[i]  = (-1.)*dStrY / dLen;
				m_dStressxy[i] = dStrXY / dLen;
			}
			// MQC:3364-Seungjun-20100512 dLen가 0.0인 경우 자기 부재력으로 처리. nCellNode==1 경우와 동일.
			else
			{
				if(!Set_SlabDesignData_byElem(iElem)) return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Set_ShelDesignData_byCell(UINT iElem)
{
	// GpsDlgBar의 [Element | Width] option 중, Width를 선택한 경우 다음과 같이 계산.
	// (길이에 대한 가중평균을 고려하는 의미인 듯....)

	memset(m_dMxx, NULL, sizeof(m_dMxx));
	memset(m_dMyy, NULL, sizeof(m_dMyy));
	memset(m_dMxy, NULL, sizeof(m_dMxy));
	memset(m_dFxx, NULL, sizeof(m_dFxx));
	memset(m_dFyy, NULL, sizeof(m_dFyy));
	memset(m_dFxy, NULL, sizeof(m_dFxy));
	memset(m_dVxx, NULL, sizeof(m_dVxx));
	memset(m_dVyy, NULL, sizeof(m_dVyy));
	memset(m_dSigMaxTop, NULL, sizeof(m_dSigMaxTop));
	memset(m_dSigMinTop, NULL, sizeof(m_dSigMinTop));
	memset(m_dSigMaxBot, NULL, sizeof(m_dSigMaxBot));
	memset(m_dSigMinBot, NULL, sizeof(m_dSigMinBot));

	T_SBDO_K SbdoK = 0;
	if (!m_aSbdoByElem.Lookup(iElem, SbdoK)) return FALSE;

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD)) return FALSE;

	int nNode = GetPlateNodeNum(iElem, ElemD);
	if (nNode==0) return FALSE;

	T_SLAB_FORCE_COMPACT slab_forc1;
	m_mapSlabForc_Compact.Lookup(iElem, slab_forc1);

	// 층별 smoothing.
	BOOL bExistStory = ExistStory();
	T_STOR_K ElemStorK = 0;
	if(bExistStory)
	{
		if(!m_arStorbyElem.Lookup(iElem, ElemStorK)) bExistStory = FALSE;
	}

	for (int i=0; i<nNode; i++)
	{
		T_NODE_K NodeK = ElemD.elnod[i];
		T_NODE_D NodeD;
		if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD))  continue;

		double dMxx = 0.0;
		double dMyy = 0.0;
		double dMxy = 0.0;
		double dFxx = 0.0;
		double dFyy = 0.0;
		double dFxy = 0.0;
		double dVxx = 0.0;
		double dVyy = 0.0;
		double dSigMaxTop = 0.0;
		double dSigMinTop = 0.0;
		double dSigMaxBot = 0.0;
		double dSigMinBot = 0.0;
		double dLen = 0.0;

		_DGN_CELL_FORC arNodeElemList;
		int nCellNode = Get_ShelNode_In_Cell(SbdoK, NodeK, arNodeElemList);
		if (nCellNode==1)
		{
			if(!Set_SlabDesignData_byElem(iElem)) return FALSE;
		}
		else
		{
			POSITION pos = arNodeElemList.arNodeElemList.GetStartPosition();
			while(pos)
			{
				T_NODE_K NextNodeK;
				T_KEY_LIST_EX ElemListK;
				arNodeElemList.arNodeElemList.GetNextAssoc(pos, NextNodeK, ElemListK);
				if(NodeK==NextNodeK)  continue;

				int nElemNum = static_cast<int>(ElemListK.GetSize());
				for(int kk=0; kk<nElemNum; kk++)
				{
					T_ELEM_K NextElemK = ElemListK[kk];
					if(bExistStory)
					{
						T_STOR_K NextStorK = 0;
						if(!m_arStorbyElem.Lookup(iElem, NextStorK)) continue;
						if(ElemStorK!=NextStorK) continue;
					}

					T_ELEM_D NextElemD;
					if(!m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD)) continue;
					int nNode_in_elem = GetPlateNodeNum(NextElemK, NextElemD);

					T_SLAB_FORCE_COMPACT slab_forc2;
					m_mapSlabForc_Compact.Lookup(NextElemK, slab_forc2);
					for(int jj=0; jj<nNode_in_elem; jj++)
					{
						if(NextNodeK == NextElemD.elnod[jj])
						{
							T_NODE_D NodeD2;
							m_pDoc->m_pAttrCtrl->GetNode(NextElemD.elnod[jj], NodeD2);
							double dLength = CMathFunc::mathLength(NodeD.x, NodeD.y, NodeD.z, NodeD2.x, NodeD2.y, NodeD2.z);

							dMxx += GetShelForc4Cell(EN_SHELFORC_IDX_MXX, slab_forc1, i, slab_forc2, jj, dLength);
							dMyy += GetShelForc4Cell(EN_SHELFORC_IDX_MYY, slab_forc1, i, slab_forc2, jj, dLength);
							dMxy += GetShelForc4Cell(EN_SHELFORC_IDX_MXY, slab_forc1, i, slab_forc2, jj, dLength);
							dFxx += GetShelForc4Cell(EN_SHELFORC_IDX_FXX, slab_forc1, i, slab_forc2, jj, dLength);
							dFyy += GetShelForc4Cell(EN_SHELFORC_IDX_FYY, slab_forc1, i, slab_forc2, jj, dLength);
							dFxy += GetShelForc4Cell(EN_SHELFORC_IDX_FXY, slab_forc1, i, slab_forc2, jj, dLength);
							dVxx += GetShelForc4Cell(EN_SHELFORC_IDX_VXX, slab_forc1, i, slab_forc2, jj, dLength);
							dVyy += GetShelForc4Cell(EN_SHELFORC_IDX_VYY, slab_forc1, i, slab_forc2, jj, dLength);
							dSigMaxTop += GetShelForc4Cell(EN_SHELFORC_IDX_SXT, slab_forc1, i, slab_forc2, jj, dLength);
							dSigMinTop += GetShelForc4Cell(EN_SHELFORC_IDX_SNT, slab_forc1, i, slab_forc2, jj, dLength);
							dSigMaxBot += GetShelForc4Cell(EN_SHELFORC_IDX_SXB, slab_forc1, i, slab_forc2, jj, dLength);
							dSigMinBot += GetShelForc4Cell(EN_SHELFORC_IDX_SNB, slab_forc1, i, slab_forc2, jj, dLength);
							dLen += dLength;
							break;
						}
					}
				}
			}

			if(dLen > 0.0)
			{
				//### 부호를 체크해야 겠다. 
				m_dMxx[i] = dMxx / dLen;
				m_dMyy[i] = dMyy / dLen;
				m_dMxy[i] = dMxy / dLen;
				m_dFxx[i] = dFxx / dLen;
				m_dFyy[i] = dFyy / dLen;
				m_dFxy[i] = dFxy / dLen;
				m_dVxx[i] = dVxx / dLen;
				m_dVyy[i] = dVyy / dLen;
				m_dSigMaxTop[i] = dSigMaxTop / dLen;
				m_dSigMinTop[i] = dSigMinTop / dLen;
				m_dSigMaxBot[i] = dSigMaxBot / dLen;
				m_dSigMinBot[i] = dSigMinBot / dLen;
				
			}
			// MQC:3364-Seungjun-20100512 dLen가 0.0인 경우 자기 부재력으로 처리. nCellNode==1 경우와 동일.
			else
			{
				if(!Set_SlabDesignData_byElem(iElem)) return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Get_InclinedSlabPF(int ElemK, int nSerial, double dPlateF[8])
{
	// PMS:4352-Seungjun-20110930 EC Inclined slab.
	for(int i=0; i<8; i++)  dPlateF[i] = 0.0;
	T_SLAB_FORCE_COMPACT SlabForce;
	SlabForce.initialize();
	if(!m_mapSlabForc_Compact.Lookup(ElemK, SlabForce)) return FALSE;

	for(int i=0; i<8; i++)
		dPlateF[i] = SlabForce.dForce[nSerial+i*4]; // Mxx, Myy, Mxy, Fxx, Fyy, Fxy, Vxx, Vyy

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Forces Smoothing
////////////////////////////////////////////////////////////////////////
void CDgnForceCtrl::GetAverageNodalDataList_Compact(CArray<T_ELEM_K, T_ELEM_K>& aElemKeys, CMap<UINT,UINT,T_SLAB_FORCE_COMPACT, T_SLAB_FORCE_COMPACT&>& mapSlabForc)
{//주어진 요소들(주로, 한 도메인에 소속한 요소들)에 대해서 노드들의 평균값을 구한다.// T_SLAB_FORCE_COMPACT버전
	CArray<T_NODE_K, T_NODE_K> aNodeKeys;
	aNodeKeys.RemoveAll();
	m_pDoc->m_pPostCtrl->GetNodeKeyListPost(aNodeKeys);

	int node_num = static_cast<int>(aNodeKeys.GetSize());
	CArray<_DGN_SMOO_FORC, _DGN_SMOO_FORC&> aForceList;
	aForceList.RemoveAll();
	aForceList.SetSize(node_num);

	CMapEx<T_NODE_K, T_NODE_K, UINT, UINT> aSerialNodeK;
	aSerialNodeK.InitHashTable(HASHSIZENODE);
	aSerialNodeK.RemoveAll();

	for(int i=0; i<node_num; ++i) 
	{
		aSerialNodeK.SetAt(aNodeKeys[i], i);
	}

	int nFNum = 32;
	int elem_num = static_cast<int>(aElemKeys.GetSize());
	for(int i=0; i<elem_num; ++i)
	{
		T_ELEM_D elem_data;
		if(!m_pDoc->m_pAttrCtrl->GetElem(aElemKeys[i],elem_data))	{ASSERT(0);  continue;}
		int nodes_in_elem=GetPlateNodeNum(aElemKeys[i], elem_data);
		if(nodes_in_elem==0) continue;

		T_SLAB_FORCE_COMPACT SlabForce;
		if(!mapSlabForc.Lookup(aElemKeys[i],SlabForce)) continue;
		int iNum=0;
		for (int j=0; j<nFNum; ++j) 
		{
			iNum++;
			if(iNum==5) iNum=1;
			if(iNum > nodes_in_elem)  continue;

			UINT SerialK = 0;
			aSerialNodeK.Lookup(elem_data.elnod[iNum-1], SerialK);
			int iIndex = (j/4);
			aForceList[SerialK].aForce[iIndex].Add(SlabForce.dForce[j]);
		}
	}

	double pforc_val[8]; 
	for(int i=0; i<node_num; ++i) 
	{
		int linked_elem_num = static_cast<int>(aForceList[i].aForce[0].GetSize());
		if(linked_elem_num < 2)continue;
		memset(pforc_val, 0, sizeof(pforc_val));
		for(int j=0; j<linked_elem_num; ++j) 
		{
			for(int k=0; k<DGN_SMOO_FORC_NUM; ++k) pforc_val[k] += aForceList[i].aForce[k].GetAt(j);
		}
		for(int j=0; j<DGN_SMOO_FORC_NUM; ++j)	pforc_val[j] /= static_cast<double>(linked_elem_num);
		for(int j=0; j<DGN_SMOO_FORC_NUM; ++j)	aForceList[i].aForce[j].SetAt(0, pforc_val[j]);
	}

	for(int i=0; i<elem_num; ++i) 
	{
		T_ELEM_D elem_data;
		if(!m_pDoc->m_pAttrCtrl->GetElem(aElemKeys[i],elem_data))	{ASSERT(0);  continue;}

		int nodes_in_elem=GetPlateNodeNum(aElemKeys[i], elem_data);
		if(nodes_in_elem==0) continue;

		int nNode=0;
		for (int j=0; j<nFNum; ++j) 
		{
			nNode++;
			if(nNode==5)  nNode=1;
			if(nNode > nodes_in_elem)  continue;

			UINT SerialK = 0;
			aSerialNodeK.Lookup(elem_data.elnod[nNode-1], SerialK);

			T_SLAB_FORCE_COMPACT SlabForce;
			if(!mapSlabForc.Lookup(aElemKeys[i],SlabForce)) continue;
			int iIndex = (j/4);
			aForceList[SerialK].aForce[iIndex].Add(SlabForce.dForce[j]);
			SlabForce.dForce[j] = aForceList[SerialK].aForce[iIndex].GetAt(0);

			mapSlabForc.SetAt(aElemKeys[i],SlabForce);
		}
	}
}

int  CDgnForceCtrl::Get_SlabNode_In_Cell(UINT SbdoK, T_NODE_K NodeK, _DGN_CELL_FORC& arNodeElemList)
{
	// Cell에 속한 slab 절점과 그 절점이 속한 요소키를 얻어옴.
	arNodeElemList.Initialize();

	DWORD stTick = GetTickCount();
	DWORD edTick = 0.0;
	if(m_mapCellElem.Lookup(NodeK, arNodeElemList))
	{
		edTick = GetTickCount();
		m_dTimeChk[0] += (edTick-stTick);
		return arNodeElemList.arNodeElemList.GetCount();
	}

	double dCellSize = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(m_nCode_Force, m_nCode_Length, m_nBase_Force, m_nBase_Length, D_UNITSYS_BASE_LENGTH, m_dCellSize);

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit=FALSE;
	if(CurIndex.nBase_Length!=m_nCode_Length || CurIndex.nBase_Force!=m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force  = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++

	T_NODE_D NodeD;
	NodeD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD))
	{
		if(bCngUnit)  m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
		return 0;
	}

	double dMinX = NodeD.x - 0.5*dCellSize;
	double dMaxX = NodeD.x + 0.5*dCellSize;
	double dMinY = NodeD.y - 0.5*dCellSize;
	double dMaxY = NodeD.y + 0.5*dCellSize;
	double dMaxZ = NodeD.z + 0.5*dCellSize;
	double dMinZ = NodeD.z - 0.5*dCellSize;
	double dZ    = NodeD.z;

	DgnRect rect;
	rect.top    = dMaxY;
	rect.bottom = dMinY;
	rect.left   = dMinX;
	rect.right  = dMaxX;

	stTick = GetTickCount();
	CArray<T_ELEM_K, T_ELEM_K> arElemK; // key list in Domain.
	CArray<T_ELEM_K, T_ELEM_K> arTempElemK; arTempElemK.RemoveAll();
	// MQC:3364-Seungjun-20100512 Merge Tolerance로 변경. 모델링시 10^-7보다 조금 큰 오차 발생하는 경우 있음.
	// auto-mash에서 1cm를 tolerance로 사용.
	m_QuadTree.CalcInAreaElem(rect, dZ, arTempElemK, CDBLib::Get_SlabSamePlaneTol(m_nCode_Length)); //1cm
	int nNum = m_pDgnDataCtrl->Get_SamePlaneList(NodeK, arTempElemK, arElemK); // 동일 평면에 있는 요소리스트를 찾음.

	for(int i=0; i<nNum; i++)
	{
		T_ELEM_K ElemK = arElemK[i];
		T_ELEM_D ElemD;

		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))     continue;
		if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))  continue;
		int nNode = GetPlateNodeNum(ElemK, ElemD);
		if(nNode==0) continue;

		T_NODE_D ElemNodeD;   ElemNodeD.Initialize();
		T_KEY_LIST_EX arElemList;
		for(int j=0; j<nNode; j++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[j], ElemNodeD))  continue;

			// 해당 좌표(p1)가 2차원 polyLine내에 들어가는지 여부를 판단함 
			if (ElemNodeD.x < dMinX) continue;
			if (ElemNodeD.y < dMinY) continue;
			if (ElemNodeD.z < dMinZ) continue;
			if (ElemNodeD.x > dMaxX) continue;
			if (ElemNodeD.y > dMaxY) continue;
			if (ElemNodeD.z > dMaxZ) continue;
			
			if(!arNodeElemList.arNodeElemList.Lookup(ElemD.elnod[j], arElemList)) arElemList.RemoveAll();
			arElemList.Add(ElemK);
			arNodeElemList.arNodeElemList.SetAt(ElemD.elnod[j], arElemList);
		}
	}
	edTick = GetTickCount();
	m_dTimeChk[1] += (edTick-stTick);

	stTick = GetTickCount();
	int nAllNum = arNodeElemList.arNodeElemList.GetCount();
	m_mapCellElem.SetAt(NodeK, arNodeElemList);
	edTick = GetTickCount();
	m_dTimeChk[2] += (edTick-stTick);

	if(bCngUnit)  m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	return nAllNum;
}

int  CDgnForceCtrl::Get_WallNode_In_Cell(UINT SbdoK, T_NODE_K NodeK, _DGN_CELL_FORC& arNodeElemList)
{
	// Cell에 속한 Wall 절점과 그 절점이 속한 요소키를 얻어옴.
	arNodeElemList.Initialize();

	if(m_mapCellElem.Lookup(NodeK, arNodeElemList))
	{
		return arNodeElemList.arNodeElemList.GetCount();
	}
	double dCellSize = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(m_nCode_Force, m_nCode_Length, m_nBase_Force, m_nBase_Length, D_UNITSYS_BASE_LENGTH, m_dCellSize);

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit=FALSE;
	if(CurIndex.nBase_Length!=m_nCode_Length || CurIndex.nBase_Force!=m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force  = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++

	T_NODE_D NodeD;
	NodeD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD))
	{
		if(bCngUnit)  m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
		return 0;
	}
	// normal vector
	double dNormal[3] = {0.0,0.0,0.0};

	double dPoly[4][2];
	CArray<T_NODE_K, T_NODE_K> arNodeInCube;
	arNodeInCube.RemoveAll();

	T_NODE_D BndN[4]; // cubic 좌표.
	CArray<T_ELEM_K, T_ELEM_K> arAllElemK; // key list in Domain.
	CArray<T_ELEM_K, T_ELEM_K> arElemK; // key list in Domain.
	int nNum = m_pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, arAllElemK);
	nNum = m_pDgnDataCtrl->Get_SamePlaneList(NodeK, arAllElemK, arElemK); // 동일 평면에 있는 요소리스트를 찾음.
	for(int i=0; i<nNum; i++)
	{
		T_ELEM_K ElemK = arElemK[i];
		T_ELEM_D ElemD;
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))     continue;
		if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))  continue;

		// Get Normal vector of wall, 단 곡면등 이형벽체는 적용되지 않음.
		// 최초 x,y,z max에 값으로 영역 생성, 직육면체 좌표.
		if(i==0)
		{
			// cell에 boundary 좌표 계산.
			//
			//   1+---------+0
			//    |         |
			//    |    +    |
			//    |         |
			//   2+--------+3
			// 벽체의 x,y평면에 대한 2D Normal vector 계산.
			// cell 크기만큰 Normal vector 확대.
			// 기준절점에 대하여 위치이동.
			double dx_max=-DBL_MAX, dx_min=DBL_MAX;
			double dy_max=-DBL_MAX, dy_min=DBL_MAX;

			T_NODE_D P1, P2;
			m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], P1);
			m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], P2);
			CMathFunc::mathNormalize(P2.x-P1.x, P2.y-P1.y, 0.0, dNormal[0], dNormal[1], dNormal[2]);
			double dx = (dCellSize/2.0);
			double dy = (dCellSize/2.0);
			double dz = (dCellSize/2.0);

			for(int k=0; k<4; k++)
			{
				double dsignx = (k==0 || k==3) ? 1.0 : -1.0;
				double dsigny = (k==0 || k==1) ? 1.0 : -1.0;
				double dx1 = NodeD.x + dsignx*dx;
				double dy1 = NodeD.y + dsigny*dy;

				BndN[k].x = dx1;
				BndN[k].y = dy1;

				if(dx1 > dx_max)  dx_max=dx1;
				if(dy1 > dy_max)  dy_max=dy1;
				if(dx1 < dx_min)  dx_min=dx1;
				if(dy1 < dy_min)  dy_min=dy1;
			}

			// x,y에 대한 ploygon생성.
			dPoly[0][0] = BndN[0].x + cDgn_Zero;
			dPoly[0][1] = BndN[0].y + cDgn_Zero;
			dPoly[1][0] = BndN[1].x - cDgn_Zero;
			dPoly[1][1] = BndN[1].y + cDgn_Zero;
			dPoly[2][0] = BndN[2].x - cDgn_Zero;
			dPoly[2][1] = BndN[2].y - cDgn_Zero;
			dPoly[3][0] = BndN[3].x + cDgn_Zero;
			dPoly[3][1] = BndN[3].y - cDgn_Zero;
		}

		int nNode = GetPlateNodeNum(ElemK, ElemD);
		for(int j=0; j<nNode; j++)
		{
			T_NODE_D ElemNodeD;
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[j], ElemNodeD))  continue;

			// polygon에 포함된 절점만 설택.
			double dP1[2] = {ElemNodeD.x, ElemNodeD.y};
			if(!CMathFunc::mathIsInsidePoint2D(dP1, 4, dPoly, FALSE)) continue;

			// 절점이 큐빅에 포함하는지 검토.
			if(ElemNodeD.z > NodeD.z+(dCellSize/2.0)) continue;
			if(ElemNodeD.z < NodeD.z-(dCellSize/2.0)) continue;

			T_KEY_LIST_EX arElemList;
			if(!arNodeElemList.arNodeElemList.Lookup(ElemD.elnod[j], arElemList)) arElemList.RemoveAll();
			arElemList.Add(ElemK);
			arNodeElemList.arNodeElemList.SetAt(ElemD.elnod[j], arElemList);
		}
	}      
	int nAllNum = arNodeElemList.arNodeElemList.GetCount();
	m_mapCellElem.SetAt(NodeK, arNodeElemList);

	if(bCngUnit)  m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	return nAllNum;
}

int  CDgnForceCtrl::Get_ShelNode_In_Cell(UINT SbdoK, T_NODE_K NodeK, _DGN_CELL_FORC& arNodeElemList)
{
	// Cell에 속한 Shell 절점과 그 절점이 속한 요소키를 얻어옴.
	arNodeElemList.Initialize();

	if(m_mapCellElem.Lookup(NodeK, arNodeElemList))
	{
		return arNodeElemList.arNodeElemList.GetCount();
	}
	double dCellSize = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(m_nCode_Force, m_nCode_Length, m_nBase_Force, m_nBase_Length, D_UNITSYS_BASE_LENGTH, m_dCellSize);

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit=FALSE;
	if(CurIndex.nBase_Length!=m_nCode_Length || CurIndex.nBase_Force!=m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force  = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++

	T_NODE_D NodeD;
	NodeD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD))
	{
		if(bCngUnit)  m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
		return 0;
	}
	// normal vector
	double dNormal[3] = {0.0,0.0,0.0};

	double dPoly[4][2];
	CArray<T_NODE_K, T_NODE_K> arNodeInCube;
	arNodeInCube.RemoveAll();

	T_NODE_D BndN[4]; // cubic 좌표.
	CArray<T_ELEM_K, T_ELEM_K> arAllElemK; // key list in Domain.
	CArray<T_ELEM_K, T_ELEM_K> arElemK; // key list in Domain.
	int nNum = m_pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, arAllElemK);
	nNum = m_pDgnDataCtrl->Get_SamePlaneList(NodeK, arAllElemK, arElemK); // 동일 평면에 있는 요소리스트를 찾음.
	for(int i=0; i<nNum; i++)
	{
		T_ELEM_K ElemK = arElemK[i];
		T_ELEM_D ElemD;
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))     continue;
		if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))  continue;

		// Get Normal vector of wall, 단 곡면등 이형벽체는 적용되지 않음.
		// 최초 x,y,z max에 값으로 영역 생성, 직육면체 좌표.
		if(i==0)
		{
			// cell에 boundary 좌표 계산.
			//
			//   1+---------+0
			//    |         |
			//    |    +    |
			//    |         |
			//   2+--------+3
			// 벽체의 x,y평면에 대한 2D Normal vector 계산.
			// cell 크기만큰 Normal vector 확대.
			// 기준절점에 대하여 위치이동.
			double dx_max=-DBL_MAX, dx_min=DBL_MAX;
			double dy_max=-DBL_MAX, dy_min=DBL_MAX;

			T_NODE_D P1, P2;
			m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], P1);
			m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], P2);
			CMathFunc::mathNormalize(P2.x-P1.x, P2.y-P1.y, 0.0, dNormal[0], dNormal[1], dNormal[2]);
			double dx = (dCellSize/2.0);
			double dy = (dCellSize/2.0);
			double dz = (dCellSize/2.0);

			for(int k=0; k<4; k++)
			{
				double dsignx = (k==0 || k==3) ? 1.0 : -1.0;
				double dsigny = (k==0 || k==1) ? 1.0 : -1.0;
				double dx1 = NodeD.x + dsignx*dx;
				double dy1 = NodeD.y + dsigny*dy;

				BndN[k].x = dx1;
				BndN[k].y = dy1;

				if(dx1 > dx_max)  dx_max=dx1;
				if(dy1 > dy_max)  dy_max=dy1;
				if(dx1 < dx_min)  dx_min=dx1;
				if(dy1 < dy_min)  dy_min=dy1;
			}

			// x,y에 대한 ploygon생성.
			dPoly[0][0] = BndN[0].x + cDgn_Zero;
			dPoly[0][1] = BndN[0].y + cDgn_Zero;
			dPoly[1][0] = BndN[1].x - cDgn_Zero;
			dPoly[1][1] = BndN[1].y + cDgn_Zero;
			dPoly[2][0] = BndN[2].x - cDgn_Zero;
			dPoly[2][1] = BndN[2].y - cDgn_Zero;
			dPoly[3][0] = BndN[3].x + cDgn_Zero;
			dPoly[3][1] = BndN[3].y - cDgn_Zero;
		}

		int nNode = GetPlateNodeNum(ElemK, ElemD);
		for(int j=0; j<nNode; j++)
		{
			T_NODE_D ElemNodeD;
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[j], ElemNodeD))  continue;

			// polygon에 포함된 절점만 설택.
			double dP1[2] = {ElemNodeD.x, ElemNodeD.y};
			if(!CMathFunc::mathIsInsidePoint2D(dP1, 4, dPoly, FALSE)) continue;

			// 절점이 큐빅에 포함하는지 검토.
			if(ElemNodeD.z > NodeD.z+(dCellSize/2.0)) continue;
			if(ElemNodeD.z < NodeD.z-(dCellSize/2.0)) continue;

			T_KEY_LIST_EX arElemList;
			if(!arNodeElemList.arNodeElemList.Lookup(ElemD.elnod[j], arElemList)) arElemList.RemoveAll();
			arElemList.Add(ElemK);
			arNodeElemList.arNodeElemList.SetAt(ElemD.elnod[j], arElemList);
		}
	}      
	int nAllNum = arNodeElemList.arNodeElemList.GetCount();
	m_mapCellElem.SetAt(NodeK, arNodeElemList);

	if(bCngUnit)  m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	return nAllNum;
}

///////////////////////////////////////////////////////////////
// one way slab flexural design (cutting line).
///////////////////////////////////////////////////////////////
void CDgnForceCtrl::Set_OneWayMomentCheckData()
{
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force  = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data를 받아올때는 Code Unit기준
	//////////////////////////////////////////////

	m_nSlabDsgDir = 1; // local-y.
	m_bCalcOneway = TRUE;

	m_OnewaySlabResTopData.RemoveAll();
	m_OnewaySlabResBotData.RemoveAll();

    const auto dLenTor = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_nCode_Force, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1.0e-3); // 1μm = 1/1000mm

	// MQC:6366 Seungjun-20130904 : oneway check시 마지막 하중조합만 저장 & 선택된 cutting line 계산하도록 수정.
	// mement에 대한 cutting line 가져오기
	CArray <UINT,UINT> aMomentCheckLineKeys;
	int nCount = GetCutlKeyList(aMomentCheckLineKeys);

	GetActivePlateList();

	CArray <UINT,UINT> aCutLinesKey;       aCutLinesKey.RemoveAll();
	CArray <T_CUTL_D,T_CUTL_D&> aCutLines; aCutLines.RemoveAll();
	for(int i=0; i<nCount; ++i)
	{
		T_CUTL_K KeyMomentCheckLine = aMomentCheckLineKeys[i];
		T_CUTL_D DataCutLine;
		DataCutLine.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetCutl(KeyMomentCheckLine, DataCutLine)) continue;

		aCutLines.Add(DataCutLine);
		aCutLinesKey.Add(KeyMomentCheckLine);
	}
	
    /// [GEN-2822] 위험단면에 걸리는 요소들이 여러 종류의 하중조합을 가진 경우, 설계를 수행하지 않습니다. (skip)
    /// skip 하는 부재 정보들은 아래의 array에 모아뒀다가 message window에 한번에 출력하도록.
    CStringArray astrSkipMsg;

	int nNumCutLines = static_cast<int>(aCutLines.GetSize());
    for ( int nCL = 0; nCL < nNumCutLines; ++nCL )
    {
        UINT ChkLineKey = aCutLinesKey[nCL];

        _DGN_1WAY_SLAB_RES _dgn_1way_data_Bot;  _dgn_1way_data_Bot.Initialize();
        _DGN_1WAY_SLAB_RES _dgn_1way_data_Top;  _dgn_1way_data_Top.Initialize();

        T_CUTL_D cutline = aCutLines[nCL];

        T_NODE_D OneWayCheckResPoint;
        OneWayCheckResPoint.x = (cutline.Point1[0]+cutline.Point2[0])/2.0;
        OneWayCheckResPoint.y = (cutline.Point1[1]+cutline.Point2[1])/2.0;
        OneWayCheckResPoint.z = (cutline.Point1[2]+cutline.Point2[2])/2.0;

        double dCutLinePoint1[3], dCutLinePoint2[3];
        memcpy(dCutLinePoint1, cutline.Point1, sizeof(dCutLinePoint1));
        memcpy(dCutLinePoint2, cutline.Point2, sizeof(dCutLinePoint2));

        double dCutLineVector[2]     = { (dCutLinePoint2[0] - dCutLinePoint1[0]), (dCutLinePoint2[1] - dCutLinePoint1[1]) };
        double dCutLinePerpVector[2] = { -dCutLineVector[1],  dCutLineVector[0] }; // counterclockwise perp vector

        // calculate the angle of the cutting line.
        // MQC 9473
        m_dCuttingLineAngle = CMathFunc::GetAngle(1.0, 0.0, dCutLineVector[0], dCutLineVector[1], TRUE);

        T_ELEM_K_LIST aCutElemK;
        GetCutlElemList(ChkLineKey, m_PlateKeyList, aCutElemK);
        if ( !IsSameDgnLcomTypeInSlab(aCutElemK) )
        {
            CString strMsg = _T("");
            strMsg.Format(_LS(IDS_RCS_SKIP_SLAB_1WAY_FLEX_IN_CUTLINE), cutline.CutLineName);
            astrSkipMsg.Add(strMsg);            
            continue;
        }

		CArray<T_ELEM_K, T_ELEM_K> aActiveElem;
		aActiveElem.RemoveAll();
		aActiveElem.Copy(m_PlateKeyList);
		int elem_num = static_cast<int>(aActiveElem.GetSize());

        bool bExcludeSeis = false;
        if ( elem_num > 0 )
        {
            bExcludeSeis = IsApplyNonDissipative() && GetSeisClassSlab(aActiveElem[0]) == EN_DGN_SEIS_CLASS::NONDISSIPATIVE ? true : false;
        }

		int iCountLcom = -1;
		for(int l = 0; l<m_iLCBAddDesignNum; l++)
		{
			CArray<DGN_CUTLINE_D,DGN_CUTLINE_D&> aCutLineDataLeft,aCutLineDataRight;
			aCutLineDataLeft.RemoveAll();
			aCutLineDataRight.RemoveAll();

			// Get Load Combination Data.
			_DGN_LCOM LcomDesign;
			LcomDesign.Initialize();
			m_parLcomData->Lookup(l+1, LcomDesign);
			const int nActive = LcomDesign.LcomUlData.nActive;
            
            if ( !IsValidLcomSlabStrn(m_nULSnELS, bExcludeSeis, LcomDesign) ) continue;
            if ( nActive == EN_DGN_STRE ) continue; /// slab 1-way flexure는 탄성휨강도 검토하지 않습니다.

            if ( !MakePForcDataforDesign_Compact(LcomDesign, 0) ) { continue; } // 안에서 m_PlateKeyList 바뀜
            
			iCountLcom++;
			for (int elem=0; elem<elem_num; ++elem) 
			{
				CArray <T_NODE_D,T_NODE_D&> aCutPoints;
				CArray <double,double> aCutValues1, aCutValues2;
				aCutPoints.RemoveAll();
				aCutValues1.RemoveAll();
				aCutValues2.RemoveAll();

				T_ELEM_K ElemK = aActiveElem[elem];
				T_ELEM_D elem_d;
				if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,elem_d))	{ASSERT(0);  continue; }

				double dElemCenter[3] = {0.0, 0.0, 0.0};
				int node_in_elem = 0;
				m_mapNodeInElem.Lookup(ElemK, node_in_elem);

				int TempCellOption = m_nCellOption;
				m_nCellOption = 0;
				Set_PlateForceMoment(ElemK);
				m_nCellOption = TempCellOption;

				for(int i=0; i<node_in_elem; ++i)
				{
					T_NODE_D		node_d1,node_d2;
					if(!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[i], node_d1)) continue;
					int j = (i==node_in_elem-1)? 0:(i+1);
					if(!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[j], node_d2)) continue;

					dElemCenter[0] += node_d1.x;
					dElemCenter[1] += node_d1.y;
					dElemCenter[2] += node_d1.z;

					double dPoint1[3], dPoint2[3], dPoint3[3], dDistance;
					node_d1.GetData(dPoint1);
					node_d2.GetData(dPoint2);

                    bool bInterSect = false;
					if(CMathFunc::mathIntersectLine2(dCutLinePoint1,dCutLinePoint2,dPoint1,dPoint2,1e-7,dDistance,dPoint3))
					{
                        bInterSect = true;
					}
                    else
                    {
                        // GEN-4558 한 점이 라인 위에 있는 경우 오류
                        double d1 = CMathFunc::mathDistanceToLine(dCutLinePoint1, dCutLinePoint2, dPoint1);
                        double d2 = CMathFunc::mathDistanceToLine(dCutLinePoint1, dCutLinePoint2, dPoint2);

                        if ((EQ0(d1, dLenTor) && UQ0(d2, dLenTor))) // Point1 is on line.
                        {
                            bInterSect = true;
                            memcpy(dPoint3, dPoint1, sizeof(double) * 3);
                        }
                        else if ((UQ0(d1, dLenTor) && EQ0(d2, dLenTor)))  // Point2 is on line.
                        {
                            bInterSect = true;
                            memcpy(dPoint3, dPoint2, sizeof(double) * 3);
                        }
                    }

                    if(bInterSect)
                    {
                        double dPlateCutVal1 = m_dMuBot[i];
                        double dPlateCutVal2 = m_dMuBot[j];
                        double dPlateCutVal3 = m_dMuTop[i];
                        double dPlateCutVal4 = m_dMuTop[j];

                        double d1 = CMathFunc::mathLength(dPoint3[0], dPoint3[1], dPoint3[2], dPoint1[0], dPoint1[1], dPoint1[2]);
                        double d2 = CMathFunc::mathLength(dPoint3[0], dPoint3[1], dPoint3[2], dPoint2[0], dPoint2[1], dPoint2[2]);

                        double dCutValue1 = dPlateCutVal1 + (dPlateCutVal2 - dPlateCutVal1)*d1 / (d1 + d2);
                        double dCutValue2 = dPlateCutVal3 + (dPlateCutVal4 - dPlateCutVal3)*d1 / (d1 + d2);

                        T_NODE_D Point3d3;
                        Point3d3.SetData(dPoint3);

                        aCutPoints.Add(Point3d3);
                        aCutValues1.Add(dCutValue1);
                        aCutValues2.Add(dCutValue2);
                    }
				}
				if(aCutPoints.GetSize()==0) continue;;

				// 같은 점이 있을 경우 없앤다.
				for(INT_PTR i =0; i<aCutPoints.GetSize()-1; ++i)
				{
					for(INT_PTR j=i+1; j<aCutPoints.GetSize(); ++j)
					{
						if(m_pDgnDataCtrl->IsSame3DPoint(aCutPoints[i],aCutPoints[j]))
						{
							aCutPoints.RemoveAt(j);
							aCutValues1.RemoveAt(j);
							aCutValues2.RemoveAt(j);
							j--;
						}
					}	
				}

				if(aCutPoints.GetSize() != 2) //최종적으로 2개의 서로다른 점이 존재해야한다.
					continue;

				DGN_CUTLINE_D	CutLineData;
				CutLineData.dPoint1Val1 = aCutValues1[0];
				CutLineData.dPoint2Val1 = aCutValues1[1];
				CutLineData.dPoint1Val2 = aCutValues2[0];
				CutLineData.dPoint2Val2 = aCutValues2[1];
				CutLineData.Point1      = aCutPoints[0];
				CutLineData.Point2      = aCutPoints[1];
				CutLineData.KeyElem     = ElemK;
				CutLineData.dDist1      = CMathFunc::mathLength(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2],
																      									aCutPoints[0].x,aCutPoints[0].y,aCutPoints[0].z);
				CutLineData.dDist2      = CMathFunc::mathLength(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2],
																									      aCutPoints[1].x,aCutPoints[1].y,aCutPoints[1].z);
				int nLeftRight = m_nCuttingDiagramLeftRight;        
				if(!IsPossibleCuttingLine(ElemK,dCutLinePoint1,dCutLinePoint2,aCutPoints[0],aCutPoints[1],nLeftRight))
					continue; 

				// Left or Right side?
				dElemCenter[0] /= static_cast<double>(node_in_elem);
				dElemCenter[1] /= static_cast<double>(node_in_elem);
				dElemCenter[2] /= static_cast<double>(node_in_elem);

				const auto enDir = CPositionChecker::checkPosition(dCutLinePoint1, dCutLinePoint2, dElemCenter);
				switch(enDir)
				{
					case Position::LEFT:
					case Position::TOP:
					{
						CutLineData.nSide = 0;// Left Side
						aCutLineDataLeft.Add(CutLineData);
						break;
					}
					case Position::RIGHT:
					case Position::BOTTOM:
					{
						CutLineData.nSide = 1;// Right Side
						aCutLineDataRight.Add(CutLineData);
						break;
					}
					default:
					ASSERT(0);
					break;
				}
			}

			// Left쪽과 Right쪽이 모두 채워지도록 한다. 즉 같은 위치에 둘 다 있으면 넘어가고 
			//  어느 한쪽에만 있으면, 그 값을 다른 쪽에도 채워준다.
			qsort((void*)aCutLineDataLeft.GetData() , aCutLineDataLeft.GetSize() , sizeof(DGN_CUTLINE_D),CDgnForceCtrl::Compare_Cutline_Dist);
			qsort((void*)aCutLineDataRight.GetData(), aCutLineDataRight.GetSize(), sizeof(DGN_CUTLINE_D),CDgnForceCtrl::Compare_Cutline_Dist);

			int indx_lt = 0,indx_rt = 0;
			double dist_lt,dist_rt;
			while(indx_lt < aCutLineDataLeft.GetSize() && indx_rt < aCutLineDataRight.GetSize())
			{
				dist_lt = __min(aCutLineDataLeft[indx_lt].dDist1,aCutLineDataLeft[indx_lt].dDist2);
				dist_rt = __min(aCutLineDataRight[indx_rt].dDist1,aCutLineDataRight[indx_rt].dDist2);
				if(fabs(dist_lt-dist_rt)<1e-8)
				{
					indx_lt++; indx_rt++;
					continue;		
				}
				else if(dist_lt>dist_rt)	aCutLineDataLeft.InsertAt(indx_lt,aCutLineDataRight[indx_rt]);
				else                      aCutLineDataRight.InsertAt(indx_rt,aCutLineDataLeft[indx_lt]);

				indx_lt++; indx_rt++;
			}

			if(aCutLineDataLeft.GetSize()>aCutLineDataRight.GetSize())
			{
				int nDiff = aCutLineDataLeft.GetSize()-aCutLineDataRight.GetSize();
				for(int n=0; n<nDiff; ++n)
					aCutLineDataRight.Add(aCutLineDataLeft[aCutLineDataLeft.GetSize()-nDiff+n]);
			}
			if(aCutLineDataLeft.GetSize()<aCutLineDataRight.GetSize())
			{
				int nDiff = aCutLineDataRight.GetSize()-aCutLineDataLeft.GetSize();
				for(int n=0; n<nDiff; ++n)
					aCutLineDataLeft.Add(aCutLineDataRight[aCutLineDataRight.GetSize()-nDiff+n]);
			}

			ASSERT( aCutLineDataLeft.GetSize() == aCutLineDataRight.GetSize());

			double Mu_lt1=0,Mu_rt1=0;
			double Mu_lt2=0,Mu_rt2=0;
			for(int n=0; n<aCutLineDataLeft.GetSize(); ++n)
			{
				Mu_lt1 += fabs(aCutLineDataLeft[n].dDist2-aCutLineDataLeft[n].dDist1)*0.5*
					        (aCutLineDataLeft[n].dPoint1Val1 + aCutLineDataLeft[n].dPoint2Val1);

				Mu_rt1 += fabs(aCutLineDataRight[n].dDist2-aCutLineDataRight[n].dDist1)*0.5*
					        (aCutLineDataRight[n].dPoint1Val1 + aCutLineDataRight[n].dPoint2Val1);

				Mu_lt2 += fabs(aCutLineDataLeft[n].dDist2-aCutLineDataLeft[n].dDist1)*0.5*
					        (aCutLineDataLeft[n].dPoint1Val2 + aCutLineDataLeft[n].dPoint2Val2);

				Mu_rt2 += fabs(aCutLineDataRight[n].dDist2-aCutLineDataRight[n].dDist1)*0.5*
					        (aCutLineDataRight[n].dPoint1Val2 + aCutLineDataRight[n].dPoint2Val2);
			}

			double Mu_lt=max(fabs(Mu_lt1), fabs(Mu_lt2));
			double Mu_rt=max(fabs(Mu_rt1), fabs(Mu_rt2));
			if(iCountLcom==0)
			{ 
				if(fabs(Mu_lt)>fabs(Mu_rt))
				{
					Set_OneWaySlabDataParts(_dgn_1way_data_Bot,LcomDesign.DesignLcomKey,Mu_lt1,aCutLineDataLeft,TRUE);
					Set_OneWaySlabDataParts(_dgn_1way_data_Top,LcomDesign.DesignLcomKey,Mu_lt2,aCutLineDataLeft,FALSE);
				}
				else
				{
					Set_OneWaySlabDataParts(_dgn_1way_data_Bot,LcomDesign.DesignLcomKey,Mu_rt1,aCutLineDataRight,TRUE);
					Set_OneWaySlabDataParts(_dgn_1way_data_Top,LcomDesign.DesignLcomKey,Mu_rt2,aCutLineDataRight,FALSE);
				}
			}
			else
			{
				double dMax=max(fabs(_dgn_1way_data_Bot.dMu), fabs(_dgn_1way_data_Top.dMu));
				if(fabs(Mu_lt)>fabs(Mu_rt))
				{
					if(dMax<fabs(Mu_lt))
					{
						Set_OneWaySlabDataParts(_dgn_1way_data_Bot,LcomDesign.DesignLcomKey,Mu_lt,aCutLineDataLeft,TRUE);
						Set_OneWaySlabDataParts(_dgn_1way_data_Top,LcomDesign.DesignLcomKey,Mu_lt,aCutLineDataLeft,FALSE);
					}
				}
				else
				{
					if(dMax<fabs(Mu_rt))
					{
						Set_OneWaySlabDataParts(_dgn_1way_data_Bot,LcomDesign.DesignLcomKey,Mu_rt,aCutLineDataRight,TRUE);
						Set_OneWaySlabDataParts(_dgn_1way_data_Top,LcomDesign.DesignLcomKey,Mu_rt,aCutLineDataRight,FALSE);
					}
				}
			}

		}

		_dgn_1way_data_Top.strCutLineNa = cutline.CutLineName;
		m_OnewaySlabResTopData.SetAt(ChkLineKey,_dgn_1way_data_Top);

		_dgn_1way_data_Bot.strCutLineNa = cutline.CutLineName;
		m_OnewaySlabResBotData.SetAt(ChkLineKey,_dgn_1way_data_Bot);
	}

	m_bCalcOneway = FALSE;

    const INT_PTR nSizeMsg = astrSkipMsg.GetSize();
    for ( INT_PTR i=0; i<nSizeMsg; ++i )
    {
        GDisplayAnalString(astrSkipMsg[i]);
    }
    GDisplayAnalString(_T(""));
}

void CDgnForceCtrl::Set_OneWaySlabDataParts(_DGN_1WAY_SLAB_RES& _dgn_1way_slab_data,unsigned int lcom_k, double dMu, CArray<DGN_CUTLINE_D,DGN_CUTLINE_D&>& PartData, BOOL bTop)
{
	_DGN_1WAY_SLAB_PART part;
	// MQC:6366 Seungjun-20130904 : oneway check시 마지막 하중조합만 저장 & 선택된 cutting line 계산하도록 수정.
	if(fabs(_dgn_1way_slab_data.dMu) > fabs(dMu)) return;
	_dgn_1way_slab_data.dMu = dMu;

	_dgn_1way_slab_data.PartList.RemoveAll();

  int nPart = static_cast<int>(PartData.GetSize());
	for(int i=0; i<nPart; ++i)
	{
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(PartData[i].KeyElem, ElemD);

		// Modify by GAY. PMS:4020. ('09.12.02). Sub-Domain에서 Use Model Thickness Check-Off시에 입력된 Sub-Domain의 Slab Thickness로 Setting.
		T_SBDO_K SbdoK;
		T_SBDO_D SbdoD;
		if(!m_aSbdoByElem.Lookup(PartData[i].KeyElem, SbdoK)) ASSERT(0);
		if(!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) ASSERT(0);

		double thickness = 0.0;
		if(!SbdoD.bUseMt && SbdoD.dThickness>cDgn_Zero)      
		{
			thickness = SbdoD.dThickness;
		}
		else
		{
			T_THIK_D ThikD;
			m_pDoc->m_pAttrCtrl->GetThik(ElemD.elpro, ThikD);    
			m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, thickness);      
		}
		thickness = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_LENGTH, thickness);

		part.Initialize();
		part.dDist[0] = PartData[i].dDist1;
		part.dDist[1] = PartData[i].dDist2;
		part.dMu[0] = bTop ? PartData[i].dPoint1Val1 : PartData[i].dPoint1Val2;
		part.dMu[1] = bTop ? PartData[i].dPoint2Val1 : PartData[i].dPoint2Val2;
		part.iSlabNo = PartData[i].KeyElem;
		part.dThk	= thickness;
		part.iLCBNo = lcom_k;

		part.dPoint1[0] = PartData[i].Point1.x;
		part.dPoint1[1] = PartData[i].Point1.y;
		part.dPoint1[2] = PartData[i].Point1.z;

		part.dPoint2[0] = PartData[i].Point2.x;
		part.dPoint2[1] = PartData[i].Point2.y;
		part.dPoint2[2] = PartData[i].Point2.z;

		_dgn_1way_slab_data.PartList.Add(part);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Make Punching shear force/stress data.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDgnForceCtrl::SubMakePeripartForTwowayForceShearCheck(_DGN_PERI_PART_MAP& mPeriPart)
{
	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_RC_SLAB_PUNCHING_MAKE_FORCE);

	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force  = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	//////////////////////////////////////////////

    const auto dLenTor = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_nCode_Force, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1.0e-3); // 1μm = 1/1000mm

    // [GEN-2013] 위험단면에 걸리는 요소들을 검색할 때는 대상 요소를 전체 요소로 합니다. (before : 설계 대상 요소)
    T_ELEM_K_LIST aPlatElemK;
    aPlatElemK.RemoveAll();    
    GetAllPlateListForShear(aPlatElemK);
	m_QuadTree.MakePlateQuadTree(aPlatElemK);
	
    if ( m_PlateKeyList.GetSize() == 0 ) { return FALSE; }

    /// [GEN-2822] 위험단면에 걸리는 요소들이 여러 종류의 하중조합을 가진 경우, 설계를 수행하지 않습니다. (skip)
    /// skip 하는 부재 정보들은 아래의 array에 모아뒀다가 message window에 한번에 출력하도록.
    CStringArray astrSkipMsg;

	int iResCount=0;
	const INT_PTR nSizePeri = mPeriPart.GetCount();
	BOOL bBreak = FALSE;	
	POSITION nPos = mPeriPart.GetStartPosition();
	while(nPos)
	{
		iResCount++;
		CString sKey;
		_DGN_PERI_PART PeriPartD;
        mPeriPart.GetNextAssoc(nPos,sKey,PeriPartD);

		T_NODE_D NodeD;
		if(!m_pDoc->m_pAttrCtrl->GetNode(PeriPartD.iNodeNo, NodeD))	{ASSERT(0);}
		double dLevel = NodeD.z;

		const INT_PTR nSizePos = PeriPartD.aPosList.GetSize();

		CArray<int, int> aPeriSectNum;
		aPeriSectNum.RemoveAll();
		aPeriSectNum.SetSize(nSizePos);

		CArray<T_ELEM_K,T_ELEM_K> arIntsecElem;
        double dCutLinePoint1[3] = { 0.0, };
        double dCutLinePoint2[3] = { 0.0, };
		if(PeriPartD.iPunchShp==1)  // round 필요없는 부분 처리.
		{
            for ( INT_PTR nCL=0; nCL<nSizePos; ++nCL )
			{                 
                GetCutLinePoints(PeriPartD.aPosList[nCL], dLevel, dCutLinePoint1, dCutLinePoint2);

				arIntsecElem.RemoveAll(); 
				m_QuadTree.CalcIntersectElem(dCutLinePoint1,dCutLinePoint2,arIntsecElem);

				// check position
				aPeriSectNum[nCL] = static_cast<int>(arIntsecElem.GetSize());
			}

			Set_CuttingLine(m_QuadTree, aPeriSectNum, PeriPartD);

			aPeriSectNum.RemoveAll();
			aPeriSectNum.SetSize(nSizePos);
		}

        for ( INT_PTR nCL=0; nCL<nSizePos; ++nCL )
		{
            GetCutLinePoints(PeriPartD.aPosList[nCL], dLevel, dCutLinePoint1, dCutLinePoint2);
			// ---------------------------------------------------
			arIntsecElem.RemoveAll(); 
			m_QuadTree.CalcIntersectElem(dCutLinePoint1,dCutLinePoint2,arIntsecElem);
			int nElemNum = static_cast<int>(arIntsecElem.GetSize());
			
			// check position
			aPeriSectNum[nCL] = nElemNum;
			// ---------------------------------------------------

#ifdef _DEBUG      
			// ---------------------------------------------------
			// TEST
			for(int nElem=0; nElem<nElemNum; nElem++)
				GDebugOut(_T(" %d"),arIntsecElem[nElem]);
			GDebugOut(_T("===================================="));
			// ---------------------------------------------------
#endif

			for(int elem=0;elem<nElemNum;elem++) 
			{
				_DGN_PERI_RES PeriResD;
				PeriResD.nPos = nCL;
				T_ELEM_D elem_d;
				if(!m_pDoc->m_pAttrCtrl->GetElem(arIntsecElem[elem], elem_d))	{ASSERT(0); continue;}
				if(!m_pDoc->m_pAttrCtrl->IsPlate(elem_d.eltyp))continue; // Plate아니면 통과 

				// MNET:4599-HSSHIM-20120414 : Punching point와 동일 평면에 있지 않은 elem 제외 
				if (!m_pDgnDataCtrl->IsNodeOnElemPlane(NodeD, elem_d))  continue;

                const bool bExcludeSeis = IsApplyNonDissipative() && GetSeisClassSlab(arIntsecElem[elem]) == EN_DGN_SEIS_CLASS::NONDISSIPATIVE ? true : false;

				int node_in_elem = GetPlateNodeNum(arIntsecElem[elem], elem_d);
				for(int l = 0; l<m_iLCBAddDesignNum; l++)
				{
					_DGN_LCOM LcomDesign;
					LcomDesign.Initialize();
					m_parLcomData->Lookup(l+1, LcomDesign);
					                    
                    if (!IsValidLcomSlabStrn(m_nULSnELS, bExcludeSeis, LcomDesign) ) continue;
					
					// Element Local Axis기준의 Plate Force값을 구해옴 
					T_LCOM_D LcomD; LcomD.Initialize();
					m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomDesign.OriginalLcomKey, LcomD);
					m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
					T_STRP_DL StrpDLMax, StrpDLMin, StrpDL;
					StrpDL.Initialize();
                    if ( !m_pDoc->m_pPostCtrl->GetStrpNew_L(arIntsecElem[elem], &StrpDLMax, &StrpDLMin, NULL) )
                    {
                        continue;
                    }
					else
					{
						StrpDL = m_nMaxMinType == DGN_LCOM_MAX ? StrpDLMax : StrpDLMin;
						double local_vector[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
						double UCS_vector[3][3]   = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
						m_pDoc->calcPlaneLocalVector(arIntsecElem[elem], local_vector);
						CMathFunc::mathNormalize(local_vector[2],local_vector[2]);
						double dDot=fabs(CMathFunc::mathDot(local_vector[2],UCS_vector[2]));
						m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector,UCS_vector,StrpDL);
					}

					CDgnCalcCutlLine CalcCutl, CalcCutlM;
					CalcCutl.Initialize();
                    CalcCutl.SetLenTol(dLenTor);
					CalcCutl.AddLine(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2]);
					CalcCutl.AddLine(dCutLinePoint2[0],dCutLinePoint2[1],dCutLinePoint2[2]);
					CalcCutlM.Initialize();
                    CalcCutlM.SetLenTol(dLenTor);
					CalcCutlM.AddLine(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2]);
					CalcCutlM.AddLine(dCutLinePoint2[0],dCutLinePoint2[1],dCutLinePoint2[2]);

					for(int i=0; i<node_in_elem; i++)
					{
						T_NODE_D node_d1,node_d2;
						if(!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[i],node_d1))	{continue;}
						int j = (i==node_in_elem-1)? 0:(i+1);
						if(!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[j],node_d2))	{continue;}

						double dPoint1[3] = {node_d1.x, node_d1.y, dLevel};
						double dPoint2[3] = {node_d2.x, node_d2.y, dLevel};
						double dPlateCutVal[2] = {0.0,0.0};
						CalcCutl.AddData(dPoint1[0],dPoint1[1],dPoint1[2],dPlateCutVal[0]); 
						// Set Moment
						double dMxx = fabs(StrpDL.dblStress[i+1][20]);
						double dMyy = fabs(StrpDL.dblStress[i+1][21]);
						CalcCutlM.AddData(dPoint1[0],dPoint1[1],dPoint1[2],max(dMxx, dMyy)); 
					}

					double dCalcX[2],dCalcY[2],dCalcZ[2],dValues[2];
					if(!CalcCutl.CalcCultLine(dCalcX,dCalcY,dCalcZ,dValues))continue; 
					double dCalcXM[2],dCalcYM[2],dCalcZM[2],dValuesM[2];
					if(!CalcCutlM.CalcCultLine(dCalcXM,dCalcYM,dCalcZM,dValuesM)) {dValuesM[0]=0.0; dValuesM[1]=0.0;}

					for(int i=0; i<2; i++)
					{
						PeriResD.dCx[i]=dCalcX[i];
						PeriResD.dCy[i]=dCalcY[i];
						PeriResD.arVu[i].Add(dValues[i]);
						PeriResD.arMu[i].Add(dValuesM[i]);
					}
					PeriResD.arLcomNo.Add(LcomDesign.DesignLcomKey); 
				}
				if(PeriResD.arLcomNo.GetSize() > 0)
				{
					PeriResD.iSlabNo = arIntsecElem[elem];
					// Seungjun-20090506 커팅라인을 2개의 슬라브가 공유할 때 필요.
                    if ( IsExistSamePeriResData(PeriPartD.iNodeNo, PeriResD, PeriPartD.aResList) )
                    {
                        continue; 
                    }
                    else
                    {
                        PeriPartD.aResList.Add(PeriResD);
                    }
				}
			}
		}

        if ( !IsSameDgnLcomTypeInSlab(PeriPartD) )
        {
            CString strMsg = _T("");
            strMsg.Format(_LS(IDS_RCS_SKIP_SLAB_2WAY_SHER), PeriPartD.iNodeNo);
            astrSkipMsg.Add(strMsg);
            continue;
        }
        		
		int nTopIdx = 0;
		PeriPartD.iPosition = Get_Position(PeriPartD.iPileNo, PeriPartD.iPunchShp, aPeriSectNum, nTopIdx);
		double dMemVec[3][3] = {{0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0}};
		if(PeriPartD.iPunchShp==1 && PeriPartD.iPileNo > 0)
		{
			m_pDoc->calcElemLocalVector(PeriPartD.iPileNo, dMemVec);
		}
		double dVec[2] = {0.0,};
		if(PeriPartD.iPunchShp==1) // rectangle
		{
			dVec[0] = PeriPartD.aPosList[nTopIdx].dCx[1] - PeriPartD.aPosList[nTopIdx].dCx[0];
			dVec[1] = PeriPartD.aPosList[nTopIdx].dCy[1] - PeriPartD.aPosList[nTopIdx].dCy[0];
		}
		else
		{
			const int& iSt = nTopIdx;
			const int& iEd = nTopIdx!=21 ? nTopIdx+5 : 2;

			dVec[0] = PeriPartD.aPosList[iEd].dCx[1] - PeriPartD.aPosList[iSt].dCx[0];
			dVec[1] = PeriPartD.aPosList[iEd].dCy[1] - PeriPartD.aPosList[iSt].dCy[0];
		}
		CMathFunc::mathNormalize(dVec[0], dVec[1], dVec[0], dVec[1]);
		const double& dDot = CMathFunc::mathDot(dMemVec[1][0], dMemVec[1][1], dVec[0], dVec[1]);
		PeriPartD.bPararel_Localy_Top = EQ0(dDot) ? false : true;

        mPeriPart.SetAt(sKey, PeriPartD);

        if ( ProDlg.Update_ProgressDlg(iResCount, nSizePeri) ) 
        {
            bBreak = TRUE;
            break; 
        }
	}

    const INT_PTR nSizeMsg = astrSkipMsg.GetSize();
    for ( INT_PTR i=0; i<nSizeMsg; ++i )
    {
        GDisplayAnalString(astrSkipMsg[i]);
    }
    GDisplayAnalString(_T(""));

    if ( bBreak ) { return FALSE; }
	
	return TRUE; 
}

BOOL CDgnForceCtrl::SubMakePeripartForTwowayStressShearCheck(_DGN_PERI_PART_MAP& mapPeriData)
{
	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_RC_SLAB_PUNCHING_MAKE_STRESS);

	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force  = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	//////////////////////////////////////////////

    const auto dLenTor = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_nCode_Force, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1.0e-3); // 1μm = 1/1000mm

	// DB로부터 데이터 받아온다 
	int nOldPForcComp = m_PForcComponent ;
	m_PForcComponent  = PFORC_COMPONENT_VYY;

	if(m_PlateKeyList.GetSize() == 0)
	{
		//////////////////////////////////////////////
		// <Remember> Data를 쓰고나서는 User Unit기준
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
		//////////////////////////////////////////////
		return FALSE; 
	}

    // [GEN-2013] 위험단면에 걸리는 요소들을 검색할 때는 대상 요소를 전체 요소로 합니다. (before : 설계 대상 요소)
    T_ELEM_K_LIST aPlatElemK;
    aPlatElemK.RemoveAll();
    GetAllPlateListForShear(aPlatElemK);
    m_QuadTree.MakePlateQuadTree(aPlatElemK);

    if ( m_PlateKeyList.GetSize() == 0 ) { return FALSE; }

    /// [GEN-2822] 위험단면에 걸리는 요소들이 여러 종류의 하중조합을 가진 경우, 설계를 수행하지 않습니다. (skip)
    /// skip 하는 부재 정보들은 아래의 array에 모아뒀다가 message window에 한번에 출력하도록.
    CStringArray astrSkipMsg;

	int iResCount=0;
	int nPeri = mapPeriData.GetCount();
	BOOL bBreak=FALSE;
	POSITION nPos=mapPeriData.GetStartPosition();
	while(nPos)
	{
		iResCount++;
		CString sKey;
		_DGN_PERI_PART PeriPartD;
		mapPeriData.GetNextAssoc(nPos,sKey,PeriPartD);

		T_NODE_D NodeD;
		if(!m_pDoc->m_pAttrCtrl->GetNode(PeriPartD.iNodeNo, NodeD))	{ASSERT(0);}
		double dLevel = NodeD.z;

		int nPos = static_cast<int>(PeriPartD.aPosList.GetSize());
		CArray<int, int> aPeriSectNum;
		aPeriSectNum.RemoveAll();
		aPeriSectNum.SetSize(nPos);

		double dCutLinePoint1[3],dCutLinePoint2[3];
		CArray<T_ELEM_K,T_ELEM_K> arIntsecElem;
		if(PeriPartD.iPunchShp==1)  // round 필요없는 부분 처리.
		{
			for(int nCL=0; nCL<nPos; nCL++)
			{
                GetCutLinePoints(PeriPartD.aPosList[nCL], dLevel, dCutLinePoint1, dCutLinePoint2);
                
				// ---------------------------------------------------
				arIntsecElem.RemoveAll(); 
				m_QuadTree.CalcIntersectElem(dCutLinePoint1,dCutLinePoint2,arIntsecElem);

				// check position
				aPeriSectNum[nCL] = static_cast<int>(arIntsecElem.GetSize());
			}

			Set_CuttingLine(m_QuadTree, aPeriSectNum, PeriPartD);
		}
        		
		for(int nCL=0; nCL<nPos; nCL++)
		{            
            GetCutLinePoints(PeriPartD.aPosList[nCL], dLevel, dCutLinePoint1, dCutLinePoint2);

			// ---------------------------------------------------
			arIntsecElem.RemoveAll(); 
			m_QuadTree.CalcIntersectElem(dCutLinePoint1,dCutLinePoint2,arIntsecElem);
			int nElemNum = static_cast<int>(arIntsecElem.GetSize());
			// ---------------------------------------------------

			double vecU[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}; 
			double vecV1[3]={0.0,0.0,0.0};
			double vecV2[3]={0.0,0.0,1.0};
			for(int i=0; i<3; i++)	vecV1[i]=dCutLinePoint2[i]-dCutLinePoint1[i];

			CMathFunc::mathCross(vecV2,vecV1,vecU[1]);     // UY
			CMathFunc::mathCross(vecV1,vecU[1],vecU[2]);   // UZ
			CMathFunc::mathCross(vecU[2],vecU[1],vecU[0]); // UX

			CMathFunc::mathNormalize(vecU[0],vecU[0]);
			CMathFunc::mathNormalize(vecU[1],vecU[1]);
			CMathFunc::mathNormalize(vecU[2],vecU[2]);
            
			T_STRP_DL StrpDL, StrpDLMax, StrpDLMin;
			for(int elem=0;elem<nElemNum;elem++) 
			{
				_DGN_PERI_RES PeriResD;
				PeriResD.nPos = nCL;
				T_ELEM_D elem_d;
				if(!m_pDoc->m_pAttrCtrl->GetElem(arIntsecElem[elem], elem_d)) { ASSERT(0); continue; }
				if(!m_pDoc->m_pAttrCtrl->IsPlaneType(elem_d.eltyp))continue; // Plate아니면 통과 
				// Punching point와 동일 평면에 있지 않은 elem 제외 
				if (!m_pDgnDataCtrl->IsNodeOnElemPlane(NodeD, elem_d))  continue;

                const bool bExcludeSeis = IsApplyNonDissipative() && GetSeisClassSlab(arIntsecElem[elem]) == EN_DGN_SEIS_CLASS::NONDISSIPATIVE ? true : false;

				double ElementLocal[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
				m_pDoc->calcPlaneLocalVector(arIntsecElem[elem], ElementLocal);
				int node_in_elem = GetPlateNodeNum(arIntsecElem[elem], elem_d);
				for(int l = 0; l<m_iLCBAddDesignNum; l++)
				{
					_DGN_LCOM LcomDesign;
					LcomDesign.Initialize();
					m_parLcomData->Lookup(l+1, LcomDesign);

                    if (!IsValidLcomSlabStrn(m_nULSnELS, bExcludeSeis, LcomDesign)) continue;
                    
					// Element Local Axis기준의 Plate Force값을 구해옴 
					T_LCOM_D LcomD; LcomD.Initialize();
					m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomDesign.OriginalLcomKey, LcomD);
					m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
					StrpDL.Initialize();
					if(!m_pDoc->m_pPostCtrl->GetStrpNew_L(arIntsecElem[elem],&StrpDLMax,&StrpDLMin,NULL)) continue;
					StrpDL = m_nMaxMinType == DGN_LCOM_MAX ? StrpDLMax : StrpDLMin;
					StrpDL.calcPrincipalStress();

					CDgnCalcCutlLine CalcCutl, CalcCutlM;
					CalcCutl.Initialize();
                    CalcCutl.SetLenTol(dLenTor);
					CalcCutl.AddLine(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2]);
					CalcCutl.AddLine(dCutLinePoint2[0],dCutLinePoint2[1],dCutLinePoint2[2]);
					CalcCutlM.Initialize();
                    CalcCutlM.SetLenTol(dLenTor);
					CalcCutlM.AddLine(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2]);
					CalcCutlM.AddLine(dCutLinePoint2[0],dCutLinePoint2[1],dCutLinePoint2[2]);          

					m_pDoc->m_pPostCtrl->ConvertStrp_UCS(ElementLocal,vecU,StrpDL);

					for(int i=0; i<node_in_elem; i++)
					{
						T_NODE_D node_d1,node_d2;
						m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[i],node_d1);
						int j = (i==node_in_elem-1)? 0:(i+1);
						m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[j],node_d2);

						double dPoint1[3] = {node_d1.x, node_d1.y, dLevel};
						double dPoint2[3] = {node_d2.x, node_d2.y, dLevel};
						double dPlateCutVal[2] = {0.0,0.0};
						dPlateCutVal[0] = CalcPForcValue_L(StrpDL,i+1,m_PForcComponent);
						dPlateCutVal[1] = CalcPForcValue_L(StrpDL,j+1,m_PForcComponent);
						CalcCutl.AddData(dPoint1[0],dPoint1[1],dPoint1[2],dPlateCutVal[0]); 
						// Set Moment
						double dMxx = fabs(StrpDL.dblStress[i+1][20]);
						double dMyy = fabs(StrpDL.dblStress[i+1][21]);
						CalcCutlM.AddData(dPoint1[0],dPoint1[1],dPoint1[2],max(dMxx, dMyy)); 
					}

					double dCalcX[2],dCalcY[2],dCalcZ[2],dValues[2];
					if(!CalcCutl.CalcCultLine(dCalcX,dCalcY,dCalcZ,dValues))continue; 
					double dCalcXM[2],dCalcYM[2],dCalcZM[2],dValuesM[2];
					if(!CalcCutlM.CalcCultLine(dCalcXM,dCalcYM,dCalcZM,dValuesM)) {dValuesM[0]=0.0; dValuesM[1]=0.0;}

					for(int i=0; i<2; i++)
					{
						PeriResD.dCx[i]=dCalcX[i];
						PeriResD.dCy[i]=dCalcY[i];
						PeriResD.arVu[i].Add(dValues[i]);
						PeriResD.arMu[i].Add(dValuesM[i]);
					}
					PeriResD.arLcomNo.Add(LcomDesign.DesignLcomKey); 
				}
				if(PeriResD.arLcomNo.GetSize() > 0)
				{
					PeriResD.iSlabNo = arIntsecElem[elem];
					// Seungjun-20090506 커팅라인을 2개의 슬라브가 공유할 때 필요.
                    if ( IsExistSamePeriResData(PeriPartD.iNodeNo, PeriResD, PeriPartD.aResList) )
                    {
                        continue;
                    }
                    else
                    {
                        PeriPartD.aResList.Add(PeriResD);
                    }
				}
			}
		}

        if ( !IsSameDgnLcomTypeInSlab(PeriPartD) )
        {
            CString strMsg = _T("");
            strMsg.Format(_LS(IDS_RCS_SKIP_SLAB_2WAY_SHER), PeriPartD.iNodeNo);
            astrSkipMsg.Add(strMsg);
            continue;
        }
        
		int nTopIdx = 0;
		PeriPartD.iPosition = Get_Position(PeriPartD.iPileNo, PeriPartD.iPunchShp, aPeriSectNum, nTopIdx);
		mapPeriData.SetAt(sKey,PeriPartD);

        if ( ProDlg.Update_ProgressDlg(iResCount, nPeri) )
        {
            bBreak = TRUE;
            break;
        }
	}

	m_PForcComponent = nOldPForcComp;

    const INT_PTR nSizeMsg = astrSkipMsg.GetSize();
    for ( INT_PTR i=0; i<nSizeMsg; ++i )
    {
        GDisplayAnalString(astrSkipMsg[i]);
    }
    GDisplayAnalString(_T(""));

    if ( bBreak ) { return FALSE; }

	return TRUE; 
}

BOOL CDgnForceCtrl::SubMakePeripartForTwowayRebarCheck(_DGN_PUNCH_RES_MAP& mapPunchingData, _DGN_PERI_PART_MAP& mapPeriData)
{
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force  = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	//////////////////////////////////////////////

    const auto dLenTor = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_nCode_Force, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1.0e-3); // 1μm = 1/1000mm

	//-------------------------------------------------------
	// MQC:3735-Seungjun-20101203 Punching shear계산에서 철근량 계산시는 전체 요소에서 찾도록 수정.
	CArray<UINT, UINT> aAllKeys;
	aAllKeys.RemoveAll();
	aAllKeys.Copy(m_PlateKeyList);
	int nAllKeys = GetAllPlateListForShear(m_PlateKeyList);

	m_QuadTree.MakePlateQuadTree(m_PlateKeyList);

	m_PlateKeyList.RemoveAll();
	m_PlateKeyList.Copy(aAllKeys);
	if(!aAllKeys.GetSize())	return FALSE; 
	//-------------------------------------------------------

	int iResCount=0;
	int nPeri = mapPeriData.GetCount();
	BOOL bBreak=FALSE;
	POSITION nPos=mapPeriData.GetStartPosition();
	while(nPos)
	{
		iResCount++;
		CString sKey;
		_DGN_PERI_PART PeriPartD;
		mapPeriData.GetNextAssoc(nPos,sKey,PeriPartD);

		T_NODE_D NodeD;
		if(!m_pDoc->m_pAttrCtrl->GetNode(PeriPartD.iNodeNo, NodeD))	{ASSERT(0);}
		double dLevel = NodeD.z;

		int nPos = static_cast<int>(PeriPartD.aPosList.GetSize());

		CArray<int, int> aPeriSectNum;
		aPeriSectNum.RemoveAll();
		aPeriSectNum.SetSize(nPos);

		CArray<T_ELEM_K,T_ELEM_K> arIntsecElem;
		double dCutLinePoint1[3],dCutLinePoint2[3];
		if(PeriPartD.iPunchShp==1)  // round 필요없는 부분 처리.
		{
			for(int nCL=0; nCL<nPos; nCL++)
			{
                GetCutLinePoints(PeriPartD.aPosList[nCL], dLevel, dCutLinePoint1, dCutLinePoint2);				
				// ---------------------------------------------------
				arIntsecElem.RemoveAll(); 
				m_QuadTree.CalcIntersectElem(dCutLinePoint1,dCutLinePoint2,arIntsecElem);

				// check position
				aPeriSectNum[nCL] = static_cast<int>(arIntsecElem.GetSize());
			}

			Set_CuttingLine(m_QuadTree, aPeriSectNum, PeriPartD);

			aPeriSectNum.RemoveAll();
			aPeriSectNum.SetSize(nPos);
		}

		for(int nCL=0; nCL<nPos; nCL++)
		{
            GetCutLinePoints(PeriPartD.aPosList[nCL], dLevel, dCutLinePoint1, dCutLinePoint2);
			
			// ---------------------------------------------------
			arIntsecElem.RemoveAll(); 
			m_QuadTree.CalcIntersectElem(dCutLinePoint1,dCutLinePoint2,arIntsecElem);
			int nElemNum = static_cast<int>(arIntsecElem.GetSize());

			// check position
			aPeriSectNum[nCL] = nElemNum;
			// ---------------------------------------------------

#ifdef _DEBUG      
			// ---------------------------------------------------
			// TEST
			for(int nElem=0; nElem<nElemNum; ++nElem)
				GDebugOut(_T(" %d"),arIntsecElem[nElem]);
			GDebugOut(_T("===================================="));
			// ---------------------------------------------------
#endif

			for(int elem=0; elem<nElemNum; ++elem) 
			{
				T_ELEM_K ElemK = arIntsecElem[elem];
				T_ELEM_D elem_d;
				if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, elem_d))	{ASSERT(0);}
				if(!m_pDoc->m_pAttrCtrl->IsPlate(elem_d.eltyp))continue; // Plate아니면 통과 

				// MNET:4599-HSSHIM-20120414 : Punching point와 동일 평면에 있지 않은 elem 제외 
				if (!m_pDgnDataCtrl->IsNodeOnElemPlane(NodeD, elem_d))  continue;

				T_SBDO_K SbdoK;
				m_aSbdoByElem.Lookup(ElemK, SbdoK);
				double dAngle1=0.0, dAngle2=90.0;
				T_SBDO_D SbdoD;
				if(m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))
				{
					dAngle1 = SbdoD.dAngle[0];
					dAngle2 = SbdoD.dAngle[1];
				}

				double dUseAsTop[2] = {0.0,0.0};
				double dUseAsBot[2] = {0.0,0.0};
				CArray<T_RBMS_D, T_RBMS_D&> aRbmsD; 
				if(!m_pDgnDataCtrl->Get_DgnConRbms(ElemK, aRbmsD, m_nCode_Length)){ aRbmsD.RemoveAll(); }
				else
				{
					// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2
					for(int nd=0; nd<4; nd++)
					{
						// Modify by GAY. MQC:5308. ('12.11.01). Slab의 휨철근 계산 수정. 
						double dAsUse = GetSlabUsedAs(aRbmsD[nd], 1.0);

						if(nd==0) dUseAsTop[0] = dAsUse;
						else if(nd==1) dUseAsBot[0] = dAsUse;
						else if(nd==2) dUseAsTop[1] = dAsUse;
						else if(nd==3) dUseAsBot[1] = dAsUse;            
					}
				}

				CDgnCalcCutlLine CalcCutl;
				CalcCutl.Initialize();
                CalcCutl.SetLenTol(dLenTor);
				CalcCutl.AddLine(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2]);
				CalcCutl.AddLine(dCutLinePoint2[0],dCutLinePoint2[1],dCutLinePoint2[2]);

				int node_in_elem = GetPlateNodeNum(ElemK, elem_d);
				for(int i=0; i<node_in_elem; ++i)
				{
					T_NODE_D node_d1,node_d2;
					if(!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[i],node_d1))	{continue;}
					int j = (i==node_in_elem-1)? 0:(i+1);
					if(!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[j],node_d2))	{continue;}

					double dPoint1[3] = {node_d1.x, node_d1.y, dLevel};
					double dPoint2[3] = {node_d2.x, node_d2.y, dLevel};
					double dPlateCutVal[2] = {0.0, 0.0};
					CalcCutl.AddData(dPoint1[0],dPoint1[1],dPoint1[2],dPlateCutVal[0]); 
				}

				double dCalcX[2],dCalcY[2],dCalcZ[2],dValues[2];
				if(!CalcCutl.CalcCultLine(dCalcX,dCalcY,dCalcZ,dValues))continue; 

				double dAsTop[2] = {0.0,0.0};
				double dAsBot[2] = {0.0,0.0};
				Calc_RebarAreaByCutLine(dUseAsTop[0], dAngle1, dCalcX, dCalcY, dAsTop);
				Calc_RebarAreaByCutLine(dUseAsTop[1], dAngle1+dAngle2, dCalcX, dCalcY, dAsTop);
				Calc_RebarAreaByCutLine(dUseAsBot[0], dAngle1, dCalcX, dCalcY, dAsBot);
				Calc_RebarAreaByCutLine(dUseAsBot[1], dAngle1+dAngle2, dCalcX, dCalcY, dAsBot);
				
				_DGN_PERI_RES PeriResD;
				PeriResD.nPos = nCL;
				for(int i=0; i<2; ++i)
				{
					PeriResD.dCx[i]=dCalcX[i];
					PeriResD.dCy[i]=dCalcY[i];
					PeriResD.arVu[i].Add(dAsTop[i]); // Top
					PeriResD.arMu[i].Add(dAsBot[i]); // Bottom
				}
				PeriResD.arLcomNo.Add(0); 

				if(PeriResD.arLcomNo.GetSize() > 0)
				{
					PeriResD.iSlabNo = arIntsecElem[elem];
					// Seungjun-20090506 커팅라인을 2개의 슬라브가 공유할 때 필요.
					if(IsExistSamePeriResData(PeriPartD.iNodeNo, PeriResD,PeriPartD.aResList))continue; 
					else
						PeriPartD.aResList.Add(PeriResD);
				}
			}
		}
		mapPeriData.SetAt(sKey,PeriPartD);
	}

	if(bBreak) return FALSE;

	return TRUE; 
}

void CDgnForceCtrl::Set_CuttingLine(CDgnQuadTree &QuadTree, CArray<int, int>& aPeriSectNum, _DGN_PERI_PART& PeriPartD)
{
	if(m_iDgnCode!=Eurocode2_04)          return;
	if(PeriPartD.aPosList.GetSize()!=(DGN_PUNCH_SHEAR_DIV*4+4))  return; // round가 6등분된 경우임.

	// Check the Position.
	BOOL bTop=TRUE, bBottom=TRUE, bRight=TRUE, bLeft=TRUE;
	int  nCount = aPeriSectNum.GetSize();
	int  iDiv   = (nCount-4)/4;
	int  iPos[4] = {iDiv, 2*iDiv+1, 3*iDiv+2, 4*iDiv+3}; // Top,Left,Bottom,Right.

	//  1----------0
	//  |          |
	//  |          |
	//  |          |
	//  2----------3
	double Point[4][2];
	for(int i=0; i<4; i++)
	{
		int j= 0;
		if(i==0)       j=3;
		else if(i==1)  j=0;
		else if(i==2)  j=1;
		else if(i==3)  j=2;

		// make rectangle points.
		double line1[2][2];
		double line2[2][2];

		// point 1
		line1[0][0] = PeriPartD.aPosList[iPos[i]].dCx[0];
		line1[0][1] = PeriPartD.aPosList[iPos[i]].dCy[0];
		line1[1][0] = PeriPartD.aPosList[iPos[i]].dCx[1];
		line1[1][1] = PeriPartD.aPosList[iPos[i]].dCy[1];

		line2[0][0] = PeriPartD.aPosList[iPos[j]].dCx[0];
		line2[0][1] = PeriPartD.aPosList[iPos[j]].dCy[0];
		line2[1][0] = PeriPartD.aPosList[iPos[j]].dCx[1];
		line2[1][1] = PeriPartD.aPosList[iPos[j]].dCy[1];

		CMathFunc::mathLineLineCross2D(line1, line2, Point[i]);
	}

	const double dTolerance = 1e-8;    // Point 위치에 있는 요소를 찾는데 사용되는 Point의 위치 영역 계산용
	for(int i=0; i<4; i++)
	{
		// MNET:XXXX-HSSHIM-20091203
		// 직사각형의 꼭지점이 있는 위치에 요소가 없으면 => 직선으로 변경 
		//                                  있으면 => round로 유지
		DgnRect rcPoint_corner;
		rcPoint_corner.Set(Point[i][0]-dTolerance, Point[i][1]+dTolerance, Point[i][0]+dTolerance, Point[i][1]-dTolerance);
		CArray <CDgn_Object*, CDgn_Object*> arDgnObj;   arDgnObj.RemoveAll();
		QuadTree.GetObjectsIntersectArea(rcPoint_corner, arDgnObj);

		BOOL bChange2StraightEdge = FALSE;
		if (arDgnObj.GetSize() == 0)    bChange2StraightEdge = TRUE;

		if (bChange2StraightEdge)
		{
			_DGN_PERI_POS PosD_moveTo;    // 곡선을 다시 직선으로 만들기 위해 이동해야 할 위치 (교차점)
			PosD_moveTo.dCx[0] = Point[i][0];
			PosD_moveTo.dCx[1] = Point[i][0];
			PosD_moveTo.dCy[0] = Point[i][1];
			PosD_moveTo.dCy[1] = Point[i][1];

			// 곡선부분을 옮긴다.
			int iTemp = 0;
			for (int k=0; k<iDiv; ++k)
			{
				iTemp = i*(iDiv+1) +k;
				PeriPartD.aPosList[iTemp] = PosD_moveTo;
			}

			// 직선 부분의 시작점을 옮긴다
			iTemp = iPos[i];
			PeriPartD.aPosList[iTemp].dCx[0] = PosD_moveTo.dCx[0];
			PeriPartD.aPosList[iTemp].dCy[0] = PosD_moveTo.dCy[0];

			// 직선 부분의 끝점을 옮긴다.
			iTemp = iPos[(i+3)%4];
			PeriPartD.aPosList[iTemp].dCx[1] = PosD_moveTo.dCx[1];
			PeriPartD.aPosList[iTemp].dCy[1] = PosD_moveTo.dCy[1];
		}
	}
}

int CDgnForceCtrl::GetPositionByUser(UINT ElemK)
{
	T_COLC_D ColcD;
	if (!m_pDoc->m_pAttrCtrl2->GetColc(ElemK, ColcD)) return -1;

	int nPos = -1;

	int nLocation = ColcD.nColumnLocation;
	if (nLocation == COLPOS_INT)
	{
		nPos = 0;
	}
	else if (nLocation == COLPOS_EDG)
	{
		int nEdgeDir = ColcD.nCriSectDir[0];
		switch (nEdgeDir)
		{
		case COLEDGE_DIR_XPOS: nPos = 3; break;
		case COLEDGE_DIR_XNEG: nPos = 4; break;
		case COLEDGE_DIR_YPOS: nPos = 1; break;
		case COLEDGE_DIR_YNEG: nPos = 2; break;
		default: ASSERT(0); break;
		}
	}
	else if (nLocation == COLPOS_COR)
	{
		int nEdgeDir1 = ColcD.nCriSectDir[0];
		int nEdgeDir2 = ColcD.nCriSectDir[1];

		if      (nEdgeDir1 == COLEDGE_DIR_XPOS && nEdgeDir2 == COLEDGE_DIR_YPOS) nPos = 8;
		else if (nEdgeDir1 == COLEDGE_DIR_XPOS && nEdgeDir2 == COLEDGE_DIR_YNEG) nPos = 7;
		else if (nEdgeDir1 == COLEDGE_DIR_XNEG && nEdgeDir2 == COLEDGE_DIR_YPOS) nPos = 5;
		else if (nEdgeDir1 == COLEDGE_DIR_XNEG && nEdgeDir2 == COLEDGE_DIR_YNEG) nPos = 6;

		else if (nEdgeDir1 == COLEDGE_DIR_YPOS && nEdgeDir2 == COLEDGE_DIR_XPOS) nPos = 8;
		else if (nEdgeDir1 == COLEDGE_DIR_YPOS && nEdgeDir2 == COLEDGE_DIR_XNEG) nPos = 7;
		else if (nEdgeDir1 == COLEDGE_DIR_YNEG && nEdgeDir2 == COLEDGE_DIR_XPOS) nPos = 5;
		else if (nEdgeDir1 == COLEDGE_DIR_YNEG && nEdgeDir2 == COLEDGE_DIR_XNEG) nPos = 6;

		else ASSERT(0);
	}
	else ASSERT(0);

	return nPos;
}

int CDgnForceCtrl::Get_Position(UINT ElemK, int iPunchShp, CArray<int, int>& aPeriSectNum, int& iTopIdx)
{
	int nPosByUser = GetPositionByUser(ElemK);
	if (nPosByUser >= 0) return nPosByUser;

	// Check the Position.
	BOOL bTop=TRUE, bBottom=TRUE, bRight=TRUE, bLeft=TRUE;
	int  nCount = static_cast<int>(aPeriSectNum.GetSize());
	int  iDiv   = (nCount-4)/4;

	int iIdxT = 0;
	int iIdxL = 1;
	int iIdxB = 2;	
	int iIdxR = 3;
	if(iPunchShp==1)
	{ 
		if(m_iDgnCode==Eurocode2_04)
		{
			// Check Top			
			bTop    = aPeriSectNum[iDiv]==0 ? FALSE : TRUE;
			// Check Left
			bLeft   = aPeriSectNum[2*iDiv+1]==0 ? FALSE : TRUE;
			// Check Bottom
			bBottom = aPeriSectNum[3*iDiv+2]==0 ? FALSE : TRUE;
			// Check Right
			bRight  = aPeriSectNum[4*iDiv+3]==0 ? FALSE : TRUE;      

			iIdxT   = iDiv;
			iIdxL   = 2*iDiv+1;
			iIdxB   = 3*iDiv+2;
			iIdxR   = 4*iDiv+3;
		}
		else
		{
			// Check Top
			bTop    = aPeriSectNum[0]==0 ? FALSE : TRUE;
			// Check Left
			bLeft   = aPeriSectNum[1]==0 ? FALSE : TRUE;
			// Check Bottom
			bBottom = aPeriSectNum[2]==0 ? FALSE : TRUE;
			// Check Right
			bRight  = aPeriSectNum[3]==0 ? FALSE : TRUE;      
		}
	}
	else
	{
		double dPreLen = 1.0;

		iIdxT = 3;
		iIdxL = 9;
		iIdxB = 15;
		iIdxR = 21;
		for(int kk=0; kk<nCount; kk++)
		{
			// Check Top
			if((3 <= kk && kk <= 8)) // 3~8
			{
				if(bTop)	bTop = aPeriSectNum[kk]==0 ? FALSE : TRUE;
			}
			// Check Left
			else if((9 <= kk && kk <= 14)) // 9~14
			{
				if(bLeft)	bLeft = aPeriSectNum[kk]==0 ? FALSE : TRUE;
			}
			// Check Bottom
			else if((15 <= kk && kk <= 20)) // 15~20
			{
				if(bBottom) bBottom = aPeriSectNum[kk]==0 ? FALSE : TRUE;
			}
			// Check Right
			else if((21<= kk || kk <= 2)) // etc.
			{
				if(bRight)	bRight = aPeriSectNum[kk]==0 ? FALSE : TRUE;
			}
		}
	}

	if(ElemK>0)
	{
		T_ELEM_D ElemD;
		ElemD.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);

		T_NODE_D NodeD1; NodeD1.Initialize();
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1);
		T_NODE_D NodeD2; NodeD2.Initialize();
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2);

		BOOL bInverse = (NodeD1.z > NodeD2.z) ? TRUE : FALSE;
		if(ElemD.angle < 0.0) bInverse = bInverse ? FALSE : TRUE;

		BOOL bTempT = bTop;
		BOOL bTempB = bBottom;
		BOOL bTempL = bLeft;
		BOOL bTempR = bRight;

		iTopIdx = iIdxT;
		if(bInverse)
		{			
			if(45. < fabs(ElemD.angle) && fabs(ElemD.angle) < 135.)
			{
				bTop    = bTempL; iTopIdx = iIdxL;
				bLeft   = bTempB;
				bBottom = bTempR;
				bRight  = bTempT;        
			}
			else if(135. <= fabs(ElemD.angle) && fabs(ElemD.angle) <= 225.)
			{
				bTop    = bTempB; iTopIdx = iIdxB;
				bLeft   = bTempR;
				bBottom = bTempT;
				bRight  = bTempL;
			}
			else if(225. < fabs(ElemD.angle) && fabs(ElemD.angle) < 315.)
			{
				bTop    = bTempR; iTopIdx = iIdxR;
				bLeft   = bTempT;
				bBottom = bTempL;
				bRight  = bTempB;
			}
		}
		else
		{
			if(45. < fabs(ElemD.angle) && fabs(ElemD.angle) < 135.)
			{
				bTop    = bTempR; iTopIdx = iIdxR;
				bLeft   = bTempT;
				bBottom = bTempL;
				bRight  = bTempB;        
			}
			else if(135. <= fabs(ElemD.angle) && fabs(ElemD.angle) <= 225.)
			{
				bTop    = bTempB; iTopIdx = iIdxB;
				bLeft   = bTempR;
				bBottom = bTempT;
				bRight  = bTempL;
			}
			else if(225. < fabs(ElemD.angle) && fabs(ElemD.angle) < 315.)
			{
				bTop    = bTempL; iTopIdx = iIdxL;
				bLeft   = bTempB;
				bBottom = bTempR;
				bRight  = bTempT;
			}
		}
	}

	int iPos = RCS_SLAB_POS_INTERNAL; // 0:Inner, 1:Side T/B, 2:Side L/R, 3:Corner.
	int dSum = bTop+bBottom+bLeft+bRight;
	if(dSum == 3) // Side
	{
		if(!bTop || !bBottom)
		{
			iPos = !bTop ? RCS_SLAB_POS_SIDE_T : RCS_SLAB_POS_SIDE_B;
		}
		else if(!bLeft || !bRight)
		{
			iPos = !bLeft ? RCS_SLAB_POS_SIDE_L : RCS_SLAB_POS_SIDE_R;
		}
	}
	else if(dSum == 2)  // Corner
	{
		if(!bTop && !bLeft)   		  iPos = RCS_SLAB_POS_COR_LT;
		else if(!bLeft && !bBottom)   iPos = RCS_SLAB_POS_COR_LB;
		else if(!bBottom && !bRight)  iPos = RCS_SLAB_POS_COR_RB;
		else if(!bRight && !bTop)	  iPos = RCS_SLAB_POS_COR_RT;
	}
	else if(dSum == 1)  // Side
	{
		if(bTop)         iPos = RCS_SLAB_POS_SIDE_T;
		else if(bBottom) iPos = RCS_SLAB_POS_SIDE_B;
		else if(bLeft)   iPos = RCS_SLAB_POS_SIDE_L;
		else if(bRight)  iPos = RCS_SLAB_POS_SIDE_R;
	}

	return iPos;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Make shear Cut-Perimeter data.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDgnForceCtrl::Set_OneWayShearCheckData()
{
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force  = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	//////////////////////////////////////////////

    const auto dLenTor = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_nCode_Force, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1.0e-3); // 1μm = 1/1000mm

	int nOldPForcComp = m_PForcComponent;
	m_PForcComponent  = PFORC_COMPONENT_VYY;

	m_OnewayResData.RemoveAll();
	GetActivePlateList();

	// MQC:6366 Seungjun-20130904 : oneway check시 마지막 하중조합만 저장 & 선택된 cutting line 계산하도록 수정.
	CArray <UINT,UINT> aShearCheckLineKeys;
	int nCount = GetCutlKeyList(aShearCheckLineKeys);
	
	CArray <T_CUTL_D,T_CUTL_D&> aCutLines; 
    aCutLines.RemoveAll();
	for(int i=0; i<nCount; i++)
	{
		T_CUTL_D DataCutLine;
		T_CUTL_K KeyShearCheckLine = aShearCheckLineKeys[i];
		DataCutLine.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetCutl(KeyShearCheckLine, DataCutLine)) continue;		

		aCutLines.Add(DataCutLine);
	}

    /// [GEN-2822] 위험단면에 걸리는 요소들이 여러 종류의 하중조합을 가진 경우, 설계를 수행하지 않습니다. (skip)
    /// skip 하는 부재 정보들은 아래의 array에 모아뒀다가 message window에 한번에 출력하도록.
    CStringArray astrSkipMsg;

	T_ELEM_K_LIST PlateElemK;
	PlateElemK.Copy(m_PlateKeyList);

	const INT_PTR nNumCutLines = aCutLines.GetSize();
	for( INT_PTR nCL = 0; nCL < nNumCutLines; ++nCL)
	{
		UINT nChkLineKey = aShearCheckLineKeys[nCL];

		_DGN_1WAY_RES _dgn_1way_data;
		_dgn_1way_data.Initialize();

		T_CUTL_D cutline = aCutLines[nCL];

		double dCutLinePoint1[3];
		memcpy(dCutLinePoint1, cutline.Point1, sizeof(dCutLinePoint1));
        double dCutLinePoint2[3];
		memcpy(dCutLinePoint2, cutline.Point2, sizeof(dCutLinePoint2));

		double dCutLineVector[3] = {(dCutLinePoint2[0]-dCutLinePoint1[0]), (dCutLinePoint2[1]-dCutLinePoint1[1]), (dCutLinePoint2[2]-dCutLinePoint1[2])};
        double vecV1[3] = { 0.0, };
        for ( int i=0; i<3; i++ )
        {
            vecV1[i]=dCutLinePoint2[i]-dCutLinePoint1[i];
        }
		
        double vecV2[3]= { 0.0,0.0,1.0 };
		CMathFunc::mathCross(vecV2,vecV1,m_vecU[1]);     // UY
		CMathFunc::mathCross(vecV1,m_vecU[1],m_vecU[2]);   // UZ
		CMathFunc::mathCross(m_vecU[2],m_vecU[1],m_vecU[0]); // UX

		CMathFunc::mathNormalize(m_vecU[0],m_vecU[0]);
		CMathFunc::mathNormalize(m_vecU[1],m_vecU[1]);
		CMathFunc::mathNormalize(m_vecU[2],m_vecU[2]);

        T_ELEM_K_LIST aCutElemK;
        GetCutlElemList(nChkLineKey, PlateElemK, aCutElemK);
        if ( !IsSameDgnLcomTypeInSlab(aCutElemK) )
        {
            CString strMsg = _T("");
            strMsg.Format(_LS(IDS_RCS_SKIP_SLAB_1WAY_SHER), cutline.CutLineName);
            astrSkipMsg.Add(strMsg);            
            continue;
        }
		
        int elem_num = static_cast<int>(m_PlateKeyList.GetSize());

        bool bExcludeSeis = false;
        if ( elem_num > 0 )
        {
            bExcludeSeis = IsApplyNonDissipative() && GetSeisClassSlab(m_PlateKeyList[0]) == EN_DGN_SEIS_CLASS::NONDISSIPATIVE ? true : false;
        }       
		
		for(int l = 0; l<m_iLCBAddDesignNum; l++)
		{
			CArray<DGN_CUTLINE_D,DGN_CUTLINE_D&> aCutLineDataLeft,aCutLineDataRight;
			aCutLineDataLeft.RemoveAll();
			aCutLineDataRight.RemoveAll();

			_DGN_LCOM LcomDesign;
			LcomDesign.Initialize();
			m_parLcomData->Lookup(l+1, LcomDesign);
			
            if (!IsValidLcomSlabStrn(m_nULSnELS, false, LcomDesign) ) continue;
			
			CArray<T_ELEM_K, T_ELEM_K> aActiveElem;
			aActiveElem.RemoveAll();
			aActiveElem.Copy(m_PlateKeyList);

			m_bOneWayVec = TRUE;
			BOOL bOK = MakePForcDataforDesign_Compact(LcomDesign, 0);
			m_bOneWayVec = FALSE;
			if(!bOK) continue;

			m_PlateKeyList.RemoveAll();
			m_PlateKeyList.Copy(aActiveElem);
			for (int elem=0;elem<elem_num;elem++) 
			{
				CDgnCalcCutlLine CalcCutl, CalcCutlM;
				CalcCutl.Initialize();
                CalcCutl.SetLenTol(dLenTor);
				CalcCutl.AddLine(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2]);
				CalcCutl.AddLine(dCutLinePoint2[0],dCutLinePoint2[1],dCutLinePoint2[2]);
				CalcCutlM.Initialize();
				CalcCutlM.SetLenTol(dLenTor);
				CalcCutlM.AddLine(dCutLinePoint1[0], dCutLinePoint1[1], dCutLinePoint1[2]);
				CalcCutlM.AddLine(dCutLinePoint2[0], dCutLinePoint2[1], dCutLinePoint2[2]);

				T_ELEM_D elem_d;
				if(!m_pDoc->m_pAttrCtrl->GetElem(m_PlateKeyList[elem],elem_d))	{ASSERT(0);  continue; }
				int node_in_elem = GetPlateNodeNum(m_PlateKeyList[elem], elem_d);

				double dElemCenter[3] = {0.0,0.0,0.0};
				for(int i=0; i<node_in_elem; i++)
				{
					T_NODE_D node_d1,node_d2;
					if(!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[i], node_d1)) continue;
					int j = (i==node_in_elem-1)? 0:(i+1);
					if(!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[j], node_d2)) continue;

					dElemCenter[0] += node_d1.x;
					dElemCenter[1] += node_d1.y;
					dElemCenter[2] += node_d1.z;

					double dPoint1[3] = {node_d1.x, node_d1.y, node_d1.z};
					double dPoint2[3] = {node_d2.x, node_d2.y, node_d2.z};

					T_SLAB_FORCE_COMPACT  SlabForce;
					m_mapSlabForc_Compact.Lookup(m_PlateKeyList[elem], SlabForce);
					double dPlateCutVal = SlabForce.dForce[i];
					CalcCutl.AddData(dPoint1[0],dPoint1[1],dPoint1[2],dPlateCutVal); 

					double dMxx = SlabForce.dForce[i+4];
					double dMyy = SlabForce.dForce[i+8];
					CalcCutlM.AddData(dPoint1[0], dPoint1[1], dPoint1[2], AbsMax(dMxx, dMyy)); 
				}

				double dCalcX[2],dCalcY[2],dCalcZ[2],dValues[2];
				if(!CalcCutl.CalcCultLine(dCalcX,dCalcY,dCalcZ,dValues))continue; 
				T_NODE_D tCutPoints[2];
				tCutPoints[0].SetData(dCalcX[0], dCalcY[0], dCalcZ[0]);
				tCutPoints[1].SetData(dCalcX[1], dCalcY[1], dCalcZ[1]);
				double dCalcXM[2], dCalcYM[2], dCalcZM[2], dValuesM[2];
				if(!CalcCutlM.CalcCultLine(dCalcXM, dCalcYM, dCalcZM, dValuesM))continue;

				// Cutting Line Vector의 오른쪽, 왼쪽에 있는가? 에 따라서 포함할것인지 말것인지 판단
				int nLeftRight = m_nCuttingDiagramLeftRight;        
				if(!IsPossibleCuttingLine(m_PlateKeyList[elem],dCutLinePoint1,dCutLinePoint2,tCutPoints[0],tCutPoints[1],nLeftRight))continue; 

				// Calc As.
				double dAsTop=0.0;
				double dAsBot=0.0;
				CArray<T_RBMS_D, T_RBMS_D&> aRbmsD; 
				if(m_pDgnDataCtrl->Get_DgnConRbms(m_PlateKeyList[elem], aRbmsD, m_nCode_Length)) 
				{
					T_SBDO_K SbdoK;
					m_aSbdoByElem.Lookup(m_PlateKeyList[elem], SbdoK);
					double dAngle1=0.0, dAngle2=90.0;
					T_SBDO_D SbdoD;
					if(m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))
					{
						dAngle1 = SbdoD.dAngle[0];
						dAngle2 = dAngle1+SbdoD.dAngle[1];
					}

					double dUseAsTop[2] = {0.0,0.0};
					double dUseAsBot[2] = {0.0,0.0};
					// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2
					for(int nd=0; nd<4; nd++)
					{
						// Modify by GAY. MQC:5308. ('12.11.01). Slab의 휨철근 계산 수정. 
						double dAsUse = GetSlabUsedAs(aRbmsD[nd], 1.0);

						if(nd==0) dUseAsTop[0] = dAsUse;
						else if(nd==1) dUseAsBot[0] = dAsUse;
						else if(nd==2) dUseAsTop[1] = dAsUse;
						else if(nd==3) dUseAsBot[1] = dAsUse;            
					}

					double dVecCut[3]={dCutLineVector[0],dCutLineVector[1],0.0};
					double dVec1[3]={0.0,0.0,0.0};
					double dVec2[3]={0.0,0.0,0.0};
					double dAsVec[3]={0.0,0.0,0.0};

					CMathFunc::mathNormalize(dVecCut, dVec1);
					dAsVec[0] = dUseAsTop[0] * cos(dAngle1*CMathFunc::m_trrad);
					dAsVec[1] = dUseAsTop[0] * sin(dAngle1*CMathFunc::m_trrad);
					CMathFunc::mathNormalize(dAsVec, dVec2);

					double dAng = CMathFunc::mathCrossAngle(dVec1, dVec2) * CMathFunc::m_trrad; //두벡터 사이의 각도를 계산
					dAsTop += (dUseAsTop[0]*fabs(sin(dAng)));
					dAsBot += (dUseAsBot[0]*fabs(sin(dAng)));

					dAsVec[0] = dUseAsTop[1] * cos(dAngle2*CMathFunc::m_trrad);
					dAsVec[1] = dUseAsTop[1] * sin(dAngle2*CMathFunc::m_trrad);
					CMathFunc::mathNormalize(dAsVec, dVec2);

					dAng = CMathFunc::mathCrossAngle(dVec1, dVec2) * CMathFunc::m_trrad; //두벡터 사이의 각도를 계산
					dAsTop += (dUseAsTop[1]*fabs(sin(dAng)));
					dAsBot += (dUseAsBot[1]*fabs(sin(dAng)));
				}

				DGN_CUTLINE_D CutLineData;
				CutLineData.initialize();
				CutLineData.dPoint1Val1 = dValues[0];
				CutLineData.dPoint2Val1 = dValues[1];
				CutLineData.dPoint1Val2 = dValuesM[0];
				CutLineData.dPoint2Val2 = dValuesM[1];
				CutLineData.Point1 = tCutPoints[0];
				CutLineData.Point2 = tCutPoints[1];
				CutLineData.KeyElem = m_PlateKeyList[elem];
				CutLineData.dDist1 = CMathFunc::mathLength(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2],
                    tCutPoints[0].x,tCutPoints[0].y,tCutPoints[0].z);
				CutLineData.dDist2 = CMathFunc::mathLength(dCutLinePoint1[0],dCutLinePoint1[1],dCutLinePoint1[2],
                    tCutPoints[1].x,tCutPoints[1].y,tCutPoints[1].z);

				CutLineData.dAsTop = dAsTop;
				CutLineData.dAsBot = dAsBot;

				// Left or Right side?
				dElemCenter[0] /= (double)node_in_elem;
				dElemCenter[1] /= (double)node_in_elem;
				dElemCenter[2] /= (double)node_in_elem;

				const auto enDir = CPositionChecker::checkPosition(dCutLinePoint1, dCutLinePoint2, dElemCenter);
				switch(enDir)
				{
					case Position::LEFT:
					case Position::TOP:
					{
						CutLineData.nSide = 0;// Left Side
						aCutLineDataLeft.Add(CutLineData);
						break;
					}
					case Position::RIGHT:
					case Position::BOTTOM:
					{
						CutLineData.nSide = 1;// Right Side
						aCutLineDataRight.Add(CutLineData);
						break;
					}
					default:
					ASSERT(0);
					break;
				}
			}

			// Left쪽과 Right쪽이 모두 채워지도록 한다. 즉 같은 위치에 둘 다 있으면 넘어가고 
			//  어느 한쪽에만 있으면, 그 값을 다른 쪽에도 채워준다.
			qsort((void*)aCutLineDataLeft.GetData() , aCutLineDataLeft.GetSize() , sizeof(DGN_CUTLINE_D),CDgnForceCtrl::Compare_Cutline_Dist);
			qsort((void*)aCutLineDataRight.GetData(), aCutLineDataRight.GetSize(), sizeof(DGN_CUTLINE_D),CDgnForceCtrl::Compare_Cutline_Dist);

			int indx_lt = 0,indx_rt = 0;
			double dist_lt,dist_rt;
			while(indx_lt < aCutLineDataLeft.GetSize() && indx_rt < aCutLineDataRight.GetSize())
			{
				dist_lt = __min(aCutLineDataLeft[indx_lt].dDist1,aCutLineDataLeft[indx_lt].dDist2);
				dist_rt = __min(aCutLineDataRight[indx_rt].dDist1,aCutLineDataRight[indx_rt].dDist2);
				if(fabs(dist_lt-dist_rt)<1e-8)
				{
					indx_lt++; indx_rt++;
					continue;		
				}
				else if(dist_lt>dist_rt)	aCutLineDataLeft.InsertAt(indx_lt,aCutLineDataRight[indx_rt]);
				else   					    aCutLineDataRight.InsertAt(indx_rt,aCutLineDataLeft[indx_lt]);

				indx_lt++; indx_rt++;
			}

			if(aCutLineDataLeft.GetSize()>aCutLineDataRight.GetSize())
			{
				int nDiff = aCutLineDataLeft.GetSize()-aCutLineDataRight.GetSize();
				for(int n=0; n<nDiff; n++)
					aCutLineDataRight.Add(aCutLineDataLeft[aCutLineDataLeft.GetSize()-nDiff+n]);
			}
			if(aCutLineDataLeft.GetSize()<aCutLineDataRight.GetSize())
			{
				int nDiff = aCutLineDataRight.GetSize()-aCutLineDataLeft.GetSize();
				for(int n=0; n<nDiff; n++)
					aCutLineDataLeft.Add(aCutLineDataRight[aCutLineDataRight.GetSize()-nDiff+n]);
			}

			ASSERT( aCutLineDataLeft.GetSize() == aCutLineDataRight.GetSize());

			double Vu_lt=0,Vu_rt=0;
			double Mu_lt=0,Mu_rt=0;
			for(int n=0; n<aCutLineDataLeft.GetSize(); n++)
			{
				Vu_lt += fabs(aCutLineDataLeft[n].dDist2-aCutLineDataLeft[n].dDist1)*
					       0.5*(aCutLineDataLeft[n].dPoint1Val1 + aCutLineDataLeft[n].dPoint2Val1);

				Vu_rt += fabs(aCutLineDataRight[n].dDist2-aCutLineDataRight[n].dDist1)*
					       0.5*(aCutLineDataRight[n].dPoint1Val1 + aCutLineDataRight[n].dPoint2Val1);

				Mu_lt += fabs(aCutLineDataLeft[n].dDist2-aCutLineDataLeft[n].dDist1)*
					0.5*(aCutLineDataLeft[n].dPoint1Val2 + aCutLineDataLeft[n].dPoint2Val2);

				Mu_rt += fabs(aCutLineDataRight[n].dDist2-aCutLineDataRight[n].dDist1)*
					0.5*(aCutLineDataRight[n].dPoint1Val2 + aCutLineDataRight[n].dPoint2Val2);
			}

			//if(l==0)
			{
				if(fabs(Vu_lt)>fabs(Vu_rt))
				{
					Set_OneWayDataParts(_dgn_1way_data,LcomDesign.DesignLcomKey,Vu_lt,aCutLineDataLeft);
					_dgn_1way_data.dMu = Mu_lt;
				}
				else
				{
					Set_OneWayDataParts(_dgn_1way_data,LcomDesign.DesignLcomKey,Vu_rt,aCutLineDataRight);
					_dgn_1way_data.dMu = Mu_rt;
				}
			}
		}

		_dgn_1way_data.strCutLineNa = cutline.CutLineName;
		m_OnewayResData.SetAt(nChkLineKey,_dgn_1way_data);
	}

	m_PForcComponent = nOldPForcComp;

    const INT_PTR nSizeMsg = astrSkipMsg.GetSize();
    for ( INT_PTR i=0; i<nSizeMsg; ++i )
    {
        GDisplayAnalString(astrSkipMsg[i]);
    }
    GDisplayAnalString(_T(""));
}

void CDgnForceCtrl::Set_OneWayDataParts(_DGN_1WAY_RES& _dgn_1way_data,unsigned int lcom_k, double dVu,CArray<DGN_CUTLINE_D,DGN_CUTLINE_D&>& PartData)
{
	// MQC:6366 Seungjun-20130904 : oneway check시 마지막 하중조합만 저장 & 선택된 cutting line 계산하도록 수정.
	if(fabs(_dgn_1way_data.dVu) > fabs(dVu)) return;
	_dgn_1way_data.dVu = dVu;

	_dgn_1way_data.PartList.RemoveAll();

	int iNumPart = static_cast<int>(PartData.GetSize());
	for(int i=0; i<iNumPart; i++)
	{
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(PartData[i].KeyElem, ElemD);

		// Modify by GAY. PMS:4020. ('09.12.02). Sub-Domain에서 Use Model Thickness Check-Off시에 입력된 Sub-Domain의 Slab Thickness로 Setting.
		T_SBDO_K SbdoK;
		T_SBDO_D SbdoD;
		if(!m_aSbdoByElem.Lookup(PartData[i].KeyElem, SbdoK)) ASSERT(0);
		if(!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) ASSERT(0);

		double thickness = 0.0;
		if(!SbdoD.bUseMt && SbdoD.dThickness>cDgn_Zero)      
		{
			thickness = SbdoD.dThickness;
		}
		else
		{
			T_THIK_D ThikD;
			m_pDoc->m_pAttrCtrl->GetThik(ElemD.elpro, ThikD);    
			m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, thickness);      
		}
		thickness = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_LENGTH, thickness);

		_DGN_1WAY_PART part;
		part.Initialize();
		part.dDist[0] = PartData[i].dDist1;
		part.dDist[1] = PartData[i].dDist2;
		part.dVu[0]   = PartData[i].dPoint1Val1;
		part.dVu[1]   = PartData[i].dPoint2Val1;
		part.iSlabNo  = PartData[i].KeyElem;
		part.dThk	    = thickness;
		part.iLCBNo   = lcom_k;

		part.dPoint1[0] = PartData[i].Point1.x;
		part.dPoint1[1] = PartData[i].Point1.y;
		part.dPoint1[2] = PartData[i].Point1.z;

		part.dPoint2[0] = PartData[i].Point2.x;
		part.dPoint2[1] = PartData[i].Point2.y;
		part.dPoint2[2] = PartData[i].Point2.z;

		part.dAsTop     = PartData[i].dAsTop;
		part.dAsBot     = PartData[i].dAsBot;

		_dgn_1way_data.PartList.Add(part);
	}
}

BOOL CDgnForceCtrl::SubMakeCutlLineForTwowayShearCheck(_DGN_PUNCH_RES_MAP& mapPunchingData, _DGN_PERI_PART_MAP& mapPeriData)
{
    if ( m_PlateKeyList.GetSize()==0 ) { return FALSE; }

	POSITION nPos=mapPeriData.GetStartPosition();
	while(nPos)
	{
		CString sKey;
		_DGN_PERI_PART  PeriPartD;
		mapPeriData.GetNextAssoc(nPos,sKey,PeriPartD);

		_DGN_PUNCH_RES  PunchResD;
		if(!mapPunchingData.Lookup(PeriPartD.iNodeNo,PunchResD))continue;     

		T_NODE_D NodeD; NodeD.Initialize();
		m_pDoc->m_pAttrCtrl->GetNode(PeriPartD.iNodeNo, NodeD);
		double dLevel = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(m_nCode_Force, m_nCode_Length, m_nBase_Force, m_nBase_Length, D_UNITSYS_BASE_LENGTH, NodeD.z);
		double dLen   = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(m_nCode_Force, m_nCode_Length, m_nBase_Force, m_nBase_Length, D_UNITSYS_BASE_LENGTH, 1.0);
        
        CArray<DGN_CUTLINE_D, DGN_CUTLINE_D&> aCutLineData;
        aCutLineData.RemoveAll();
        const INT_PTR nPart = PeriPartD.aResList.GetSize();
        for ( INT_PTR i=0; i<nPart; ++i )
		{
			const _DGN_PERI_RES& PeriResD = PeriPartD.aResList[i];

			T_ELEM_D ElemD;
			m_pDoc->m_pAttrCtrl->GetElem(PeriResD.iSlabNo, ElemD);
			// 투영 점을 찾기위한 평면vector 계산.
			double PlaneNormal[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
			m_pDoc->calcPlaneLocalVector(PeriResD.iSlabNo, PlaneNormal);
			// 평면위 절점.
			T_NODE_D ElemNodeD;
			m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], ElemNodeD);
			double PlanePoint[3] = {ElemNodeD.x*dLen, ElemNodeD.y*dLen, NodeD.z*dLen};
            
            const INT_PTR nLcom = PeriResD.arLcomNo.GetSize();
            for ( INT_PTR j=0; j<nLcom; ++j )
			{
				DGN_CUTLINE_D CutlD;
				CutlD.initialize();

                if ( PeriResD.arLcomNo[j] == PunchResD.iLCBNo )
				{
					if(fabs(PunchResD.dThk)<=1.0E-20) 
					{
						CutlD.dPoint1Val1=PeriResD.arVu[0][j];
						CutlD.dPoint2Val1=PeriResD.arVu[1][j];
					}
					else
					{
						// Change by ZINU.('03.02.24). Average Stress per Thickness.
						double dCover = PeriPartD.IsUseTopBarCover() ? PunchResD.dT : PunchResD.dB;
                        double dd = PunchResD.dThk - dCover;
                        if ( dd > 0.0 )
                        {
                            CutlD.dPoint1Val1 = (PeriResD.arVu[0][j]/dd);
                            CutlD.dPoint2Val1 = (PeriResD.arVu[1][j]/dd);
                        }
                        else
                        {
                            CutlD.dPoint1Val1 = PeriResD.arVu[0][j];
                            CutlD.dPoint2Val1 = PeriResD.arVu[1][j];
                        }
                    }

					double LineVector[3] = {0.0,0.0,1.0};
					double LinePoint[3]  = {PeriResD.dCx[0], PeriResD.dCy[0], dLevel};
					double pInts[3] = {0.0};
					// 경사슬라브의 경우 경사면에 투영된 좌표를 계산함.
					// LineVector : 직선vector, 여기서는 Global 연직방향.
					// LinePoint : 직선위의 한점. 여기서느 위험단면 좌표.
					// PlaneNormal : 평면vector의 local-z 방향 성분
					// PlanePoint : 평면위의 한점
					// pints : 평면에 투영된 점.
					CMathFunc::mathIntersectPointToPlane(LineVector, LinePoint, PlaneNormal[2], PlanePoint, pInts);

					CutlD.Point1.x = pInts[0];
					CutlD.Point1.y = pInts[1];
					CutlD.Point1.z = pInts[2];

					LinePoint[0]  = PeriResD.dCx[1];
					LinePoint[1]  = PeriResD.dCy[1];
					LinePoint[2]  = dLevel;
					// 경사슬라브의 경우 경사면에 투영된 좌표를 계산함.
					CMathFunc::mathIntersectPointToPlane(LineVector, LinePoint, PlaneNormal[2], PlanePoint, pInts);

					CutlD.Point2.x = pInts[0];
					CutlD.Point2.y = pInts[1];
					CutlD.Point2.z = pInts[2];
					CutlD.KeyElem  = PeriResD.iSlabNo;
					aCutLineData.Add(CutlD);

					break; 
				}
			}
		}

		DGN_CUTLINE_AR arCutLine; arCutLine.initialize();
		const INT_PTR nCut = static_cast<int>(aCutLineData.GetSize());
        for ( INT_PTR kk=0; kk<nCut; ++kk )
        {
            arCutLine.arCutLine.Add(aCutLineData[kk]);
        }

		m_arPnChkCutl.SetAt(PeriPartD.iNodeNo, arCutLine);
	}

    CheckOverlapCutLine(m_arPnChkCutl);

	return TRUE; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get plate forces.
//////////////////////////////////////////////////////////////////////////////////////////////////
double CDgnForceCtrl::CalcPForcValue_G(const T_STRP_DG &pforc, int node, int nComponent)
{
	// dblForce     : global force                 , 4 node       ,
	//                component : FX, FY, FZ, MX, MY, MZ
	// dblStressIOS : global stress(membrane, bending, shear), cent + 4 node, 
	//                component : SMX, SMY, SMZ, SBX, SBY, SBZ, VX, VY
	// dblStress    : global stress + unit force   , cent + 4 node, 
	//                component : SXXt, SYYt, SZZt, SXYt, SYZt, SXZt, P1t, P2t, ANGt, EFFt, 
	//                            SXXb, SYYb, SZZb, SXYb, SYZb, SXZb, P1b, P2b, ANGb, EFFb
	//                            FXX, FYY, FZZ, FXY, FYZ, FXZ, MAXF, MINF, ANGF,
	//                            MXX, MYY, MZZ, MXY, MYZ, MXZ, MAXM, MINM, ANGM,
	//                            Vxx, Vyy, Max-Shear_t, Max-Shear_b
	//			20,  21,  22,  23,  24,  25,   26,   27,   28
	//		 FXX, FYY, FZZ, FXY, FYZ, FXZ, MAXF, MINF, ANGF,
	//			29,  30,  31,  32,  33,  34,   35,   36,   37
	//     MXX, MYY, MZZ, MXY, MYZ, MXZ, MAXM, MINM, ANGM,
	//      38,  39
	//     Vxx, Vyy
	switch (nComponent) 
	{
	case PFORC_COMPONENT_FXX: { return pforc.dblStress[node][20]; }
	case PFORC_COMPONENT_FYY: { return pforc.dblStress[node][21]; }
	case PFORC_COMPONENT_FXY: { return pforc.dblStress[node][23]; }
	case PFORC_COMPONENT_FMX: { return pforc.dblStress[node][26]; }
	case PFORC_COMPONENT_FMN: { return pforc.dblStress[node][27]; }
	case PFORC_COMPONENT_MXX: { return pforc.dblStress[node][29]; }
	case PFORC_COMPONENT_MYY: { return pforc.dblStress[node][30]; }
	case PFORC_COMPONENT_MXY: { return pforc.dblStress[node][32]; }
	case PFORC_COMPONENT_MMX: { return pforc.dblStress[node][35]; }
	case PFORC_COMPONENT_MMN: { return pforc.dblStress[node][36]; }
	case PFORC_COMPONENT_VXX: { return pforc.dblStress[node][38]; }
	case PFORC_COMPONENT_VYY: { return pforc.dblStress[node][39]; }
    case PFORC_COMPONENT_SMX_TOP: { return pforc.dblStress[node][6]; }
    case PFORC_COMPONENT_SMN_TOP: { return pforc.dblStress[node][7]; }
    case PFORC_COMPONENT_SMX_BOT: { return pforc.dblStress[node][16]; }
    case PFORC_COMPONENT_SMN_BOT: { return pforc.dblStress[node][17]; }
    case PFORC_COMPONENT_FMX_ABS: { return (fabs(pforc.dblStress[node][26]) > fabs(pforc.dblStress[node][27]))? pforc.dblStress[node][26]:pforc.dblStress[node][27]; }
    case PFORC_COMPONENT_MMX_ABS: { return (fabs(pforc.dblStress[node][35]) > fabs(pforc.dblStress[node][36]))? pforc.dblStress[node][35]:pforc.dblStress[node][36]; }
    case PFORC_COMPONENT_VMX_ABS: { return (fabs(pforc.dblStress[node][38]) > fabs(pforc.dblStress[node][39]))? pforc.dblStress[node][38]:pforc.dblStress[node][39]; }
    default: { ASSERT(0); return 0.0; }
	}
}

double CDgnForceCtrl::CalcPForcValue_L(const T_STRP_DL &pforc, int node, int nComponent)
{
	//  component : Sxxt 0, Syyt 1, Sxyt 2, P1t 3, P2t 4, ANGt 5, EFFt 6, 
	//              Sxxb 7, Syyb 8, Sxyb 9, P1b 10, P2b 11, ANGb 12, EFFb 13, 
	//              Fxx 14, Fyy 15, Fxy 16, MAXF 17, MINF 18, ANGF 19, Mxx 20, 
	//              Myy 21, Mxy 22, MAXM 23, MINM 24, ANGM 25, Vxx 26, Vyy 27
	switch(nComponent) 
	{
    case PFORC_COMPONENT_FXX: { return pforc.dblStress[node][14]; }
    case PFORC_COMPONENT_FYY: { return pforc.dblStress[node][15]; }
    case PFORC_COMPONENT_FXY: { return pforc.dblStress[node][16]; }
    case PFORC_COMPONENT_FMX: { return pforc.dblStress[node][17]; }
    case PFORC_COMPONENT_FMN: { return pforc.dblStress[node][18]; }
    case PFORC_COMPONENT_MXX: { return pforc.dblStress[node][20]; }
    case PFORC_COMPONENT_MYY: { return pforc.dblStress[node][21]; }
    case PFORC_COMPONENT_MXY: { return pforc.dblStress[node][22]; }
    case PFORC_COMPONENT_MMX: { return pforc.dblStress[node][23]; }
    case PFORC_COMPONENT_MMN: { return pforc.dblStress[node][24]; }
    case PFORC_COMPONENT_VXX: { return pforc.dblStress[node][26]; }
    case PFORC_COMPONENT_VYY: { return pforc.dblStress[node][27]; }
    case PFORC_COMPONENT_SMX_TOP: { return pforc.dblStress[node][3]; }
    case PFORC_COMPONENT_SMN_TOP: { return pforc.dblStress[node][4]; }
    case PFORC_COMPONENT_SMX_BOT: { return pforc.dblStress[node][10]; }
    case PFORC_COMPONENT_SMN_BOT: { return pforc.dblStress[node][11]; }
    default: { ASSERT(0); return 0.0; }
	}    
}

double CDgnForceCtrl::CalcMomentByDgnDir(int nDgnDir, double dmxx, double dmyy, double dAlpha, double dPhi, double dVectrSBDO[3][3])
{
    double dVectrUCS[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
    memcpy(dVectrUCS, dVectrSBDO, sizeof(dVectrUCS));

    const double dSbdoAng1 = CMathFunc::GetAngle(0.0, 0.0, dVectrSBDO[0][0], dVectrSBDO[0][1]);
    const double dSbdoAng2 = CMathFunc::GetAngle(0.0, 0.0, dVectrSBDO[1][0], dVectrSBDO[1][1]);
    double dRotAngle = 0.0;
    switch ( nDgnDir ) // Sub Domain 각도와 배근 각도의 차이만큼만 회전
    {
    case 0: { dRotAngle = dAlpha - dSbdoAng1;          } break; // Direction X
    case 1: { dRotAngle = (dPhi + dAlpha) - dSbdoAng2; } break; // Direction Y
    default: ASSERT(0); break;
    }

    for ( int i=0; i<3; ++i )
    {
        CMathFunc::mathRotateZ(dRotAngle, dVectrUCS[i][0], dVectrUCS[i][1], dVectrUCS[i][2]);
    }

    T_STRP_DL Strp; Strp.Initialize();
    Strp.dblStress[0][20] = dmxx;
    Strp.dblStress[0][21] = dmyy;    
    if ( !m_pDoc->m_pPostCtrl->ConvertStrp_UCS(dVectrSBDO, dVectrUCS, Strp) ) { ASSERT(0); return 0.0; }

    switch ( nDgnDir )
    {
    case 0: { return Strp.dblStress[0][20]; }
    case 1: { return Strp.dblStress[0][21]; }
    default: { ASSERT(0); return 0.0; }
    }   
}

double CDgnForceCtrl::CalcShearByDgnDir(int nDgnDir, double dvxx, double dvyy, double dAlpha, double dPhi, double dVectrSBDO[3][3])
{
    double dVectrUCS[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
    memcpy(dVectrUCS, dVectrSBDO, sizeof(dVectrUCS));

    const double dSbdoAng1 = CMathFunc::GetAngle(0.0, 0.0, dVectrSBDO[0][0], dVectrSBDO[0][1]);
    const double dSbdoAng2 = CMathFunc::GetAngle(0.0, 0.0, dVectrSBDO[1][0], dVectrSBDO[1][1]);
    double dRotAngle = 0.0;
    switch ( nDgnDir ) // Sub Domain 각도와 배근 각도의 차이만큼만 회전
    {
    case 0: { dRotAngle = dAlpha - dSbdoAng1;          } break; // Direction X
    case 1: { dRotAngle = (dPhi + dAlpha) - dSbdoAng2; } break; // Direction Y
    default: ASSERT(0); break;
    }

    for ( int i=0; i<3; ++i )
    {
        CMathFunc::mathRotateZ(dRotAngle, dVectrUCS[i][0], dVectrUCS[i][1], dVectrUCS[i][2]);
    }

    T_STRP_DL Strp; Strp.Initialize();
    Strp.dblStress[0][26] = dvxx;
    Strp.dblStress[0][27] = dvyy;
    
    if ( !m_pDoc->m_pPostCtrl->ConvertStrp_UCS(dVectrSBDO, dVectrUCS, Strp) ) { ASSERT(0); return 0.0; }

    switch ( nDgnDir )
    {
    case 0: { return Strp.dblStress[0][26]; }
    case 1: { return Strp.dblStress[0][27]; }
    default: { ASSERT(0); return 0.0; }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check Geometry Information
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDgnForceCtrl::MakeSbdoQuadTree(T_SBDO_K SbdoK)
{
	m_QuadTree.Initialize();

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit=FALSE;
	if(CurIndex.nBase_Length!=m_nCode_Length || CurIndex.nBase_Force!=m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force  = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++
	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	arElemK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, arElemK);
	BOOL bOK = m_QuadTree.MakePlateQuadTree(arElemK);

	if(bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	return bOK; 
}

BOOL CDgnForceCtrl::IsPossibleCuttingLine(T_ELEM_K ElemK,double dCutLinePt1[3],double dCutLinePt2[3],
			 																	  T_NODE_D &ptCutted1,T_NODE_D &ptCutted2,int nLeftRight)
{
	//-----------------------------------------------------------------------------------------------
	// INIT : 2004. 7. 7. by KYE-HONG
	// FUNC : Cutting Line에 오른쪽 또는 왼쪽 요소인가를 따져서 Diagram을 그릴것인지 말것인지를 결정 
	// DESC : nLeftRight=0 : Both
	//        nLeftRight=1 : Left
	//        nLeftRight=2 : Right
	//-----------------------------------------------------------------------------------------------
	if(nLeftRight == 0)return TRUE; // Both면 무조건 그림 

	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD);

	// Element의 Edge에 놓인 Line에 대해서만 Left, Right 를 Check한다 	
	int nNodeNum = GetPlateNodeNum(ElemK, ElemD);
	if(nNodeNum==0) return FALSE;

    BOOL bNeedCheck = FALSE;
	for(int i=0; i<nNodeNum; ++i)
	{
        T_NODE_D Pt1;
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[i],Pt1);
        T_NODE_D Pt2;
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[(i+1)%nNodeNum],Pt2);

		// Element Edge상에 두 점이 있음 
		if( (m_pDgnDataCtrl->IsSame3DPoint(Pt1,ptCutted1) && m_pDgnDataCtrl->IsSame3DPoint(Pt2,ptCutted2)) || 
			  (m_pDgnDataCtrl->IsSame3DPoint(Pt2,ptCutted1) && m_pDgnDataCtrl->IsSame3DPoint(Pt1,ptCutted2)) )
		{
			bNeedCheck = TRUE; 
			break; 
		}
	}

    if ( !bNeedCheck ) { return TRUE; }

	double dVectCutLine[3] = {0.0,0.0,0.0};
	for(int i=0; i<3; ++i)	dVectCutLine[i] = dCutLinePt2[i] - dCutLinePt1[i];
	if(!CMathFunc::mathNormalize(dVectCutLine,dVectCutLine)){ ASSERT(0); return TRUE; }

	// --------------------------------------------------------------------
	// Cutting Line Vector의 왼쪽,오른쪽 Vector
	// --------------------------------------------------------------------
	double dVecGZ[3] = {0.0,0.0,0.0};
	if(nLeftRight == 1)       dVecGZ[2] = -1.0;
	else if(nLeftRight == 2)  dVecGZ[2] = 1.0;
	else  { ASSERT(0); return TRUE; }

	double dVectLeftRight[3] = {0.0,0.0,0.0};
	CMathFunc::mathCross(dVectCutLine,dVecGZ,dVectLeftRight);
	if(!CMathFunc::mathNormalize(dVectLeftRight,dVectLeftRight)){ ASSERT(0); return TRUE; }

	// --------------------------------------------------------------------
	// 요소의 중심과 요소중심을 Cutting Line에 투영한 점사이의 Vector
	// --------------------------------------------------------------------
	double dElemCenter[3] = {0.0,0.0,0.0};
	m_pDoc->calcCenterElem(ElemD,dElemCenter[0],dElemCenter[1],dElemCenter[2]);

	const auto enDir = CPositionChecker::checkPosition(dCutLinePt1, dCutLinePt2, dElemCenter);
	switch(enDir)
	{
		case Position::LEFT:
		case Position::TOP:
		{
			if(nLeftRight == 1) return TRUE;
			break;
		}
		case Position::RIGHT:
		case Position::BOTTOM:
		{
			if(nLeftRight == 2) return TRUE;
			break;
		}
		default:
		break;
	}
	return FALSE;

	double PrjPt[3] = {0.0,0.0,0.0};
	//선과 점의 수직교점, a,b,c:선상의 1점, l,m,n:선의 방향벡터, x,y,z:점의 좌표, rpx,rpy,rpz:교점의 좌표
	CMathFunc::mathPLCrossPoint(dCutLinePt1[0] , dCutLinePt1[1] , dCutLinePt1[2] , 
        dVectCutLine[0], dVectCutLine[1], dVectCutLine[2],
        dElemCenter[0], dElemCenter[1], dElemCenter[2],
        PrjPt[0], PrjPt[1], PrjPt[2]);

	double dVectPrj2Cent[3] = {0.0,0.0,0.0};
    for ( int i=0; i<3; i++ )
    {
        dVectPrj2Cent[i] = dElemCenter[i] - PrjPt[i];
    }

	if(!CMathFunc::mathNormalize(dVectPrj2Cent,dVectPrj2Cent))	{ ASSERT(0); return TRUE; }

	// --------------------------------------------------------------------
	// 두 Vector가 같은 방향이 아니면 Diagram그리지 않음 
	// --------------------------------------------------------------------
	double dDot=CMathFunc::mathDot(dVectPrj2Cent,dVectLeftRight);
	if( fabs(1.0-dDot) <  0.0001523 ) return TRUE; 

	return FALSE; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calc Rebar area
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDgnForceCtrl::Calc_RebarAreaByCutLine(double dUseAs, double dAngle, double dX[2], double dY[2], double dAs[2])
{
	double dVecX[3]={1.0,0.0,0.0};
	double dVec1[3]={0.0,0.0,0.0};
	double dVec2[3]={0.0,0.0,0.0};
	double dVec3[3]={0.0,0.0,0.0};
	double dVec4[3]={0.0,0.0,0.0};
	// Top X
	dVec1[0] = dUseAs * cos(dAngle*CMathFunc::m_trrad);
	dVec1[1] = dUseAs * sin(dAngle*CMathFunc::m_trrad);
	dVec2[0] = dX[0]-dX[1];
	dVec2[1] = dY[0]-dY[1];
	CMathFunc::mathNormalize(dVec1, dVec3); // 단위 벡터로 변환
	CMathFunc::mathNormalize(dVec2, dVec4); // 단위 벡터로 변환
	double dAng  = CMathFunc::mathCrossAngle(dVec3, dVec4); //두벡터 사이의 각도를 계산
	double dAng2 = CMathFunc::mathCrossAngle(dVecX, dVec4); //두벡터 사이의 각도를 계산
	BOOL bVertical = FALSE;
	if(fabs(dAng2)>45.0 && fabs(dAng2)<135.0) bVertical=TRUE;
	BOOL bInLine=FALSE;
	if(fabs(dAng)<cDgn_Zero || fabs(dAng-180.0)<cDgn_Zero) bInLine=TRUE;

	double dCosAng = cos(dAng*CMathFunc::m_trrad);
	double dSinAng = sin(dAng*CMathFunc::m_trrad);
	if(bVertical)
	{
		dAs[0] += bInLine ? 0.0 : dUseAs * fabs(dSinAng);
		dAs[1] += bInLine ? 0.0 : dUseAs * fabs(dCosAng);
	}
	else
	{
		dAs[0] += bInLine ? 0.0 : dUseAs * fabs(dCosAng);
		dAs[1] += bInLine ? 0.0 : dUseAs * fabs(dSinAng);
	}
}

double CDgnForceCtrl::GetSlabUsedAs(const T_RBMS_D &RbmsD, const double &dWidth)
{
	double dAs1 = GetSlabUsedAs(RbmsD.strBasicRebarName, RbmsD.dBasicRebarSpace, dWidth);
	double dAs2 = GetSlabUsedAs(RbmsD.strAdd1RebarName, RbmsD.dAdd1RebarSpace, dWidth);
	double dAs3 = GetSlabUsedAs(RbmsD.strAdd2RebarName, RbmsD.dAdd1RebarSpace, dWidth); // Add 철근의 Space는 1에서 입력받은 항목을 1,2 모두에 적용.

	double dAsUse = 0.0;
	if (dAs3 > 0.0) dAsUse = dAs1 + (dAs2 + dAs3)/2.0;
	else            dAsUse = dAs1 + dAs2;

	return dAsUse;
}

double CDgnForceCtrl::GetSlabUsedAs(const CString& strBarName, double dSpace, double dWidth)
{
	if (strBarName == _T("") || strBarName.CompareNoCase(_T("NONE"))==0) return 0.0;
	double dAs = m_pDoc->m_pMatlDB->Get_RebarArea4Slab(strBarName);
	return dSpace > 0.0 ? dAs*dWidth/dSpace : 0.0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set/Get/Check Data
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MQC:3735-Seungjun-20101203 Punching shear계산에서 철근량 계산시는 전체 요소에서 찾도록 수정.
int CDgnForceCtrl::GetAllPlateListForShear(CArray<UINT, UINT>& aAllKeys, BOOL bWall/*FLASE*/)
{
	aAllKeys.RemoveAll();
	std::vector<T_MADO_K> aMadoKeys; // Main domain key list. (설계시는 sub-domain만 사용)
	m_pDoc->m_pAttrCtrl->GetMadoKeyList(aMadoKeys);
	int nMado = static_cast<int>(aMadoKeys.size());
	if(nMado==0)  return 0;
	for(int k=0; k<nMado; k++)
	{
		std::vector<T_SBDO_K> aSbdoKeys; // Sub domain key list. (설계시는 sub-domain만 사용)
		T_MADO_K MadoK=aMadoKeys[k];
		int nSbdo = m_pDoc->m_pAttrCtrl->GetSbdoKeyList(MadoK, aSbdoKeys);
		if(nSbdo==0)  continue;
		CVectorUtil::Sort(aSbdoKeys);
		for(int i=0; i<nSbdo; i++)
		{
			T_SBDO_K SbdoK = aSbdoKeys[i];
			m_aMadoBySbdo.SetAt(SbdoK, MadoK);

			T_SBDO_D SbdoD;
			if(!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))  continue;
            if ( SbdoD.IsShell() ) continue;

			if(!bWall)
			{
				if(SbdoD.IsNone() || SbdoD.IsWall())           continue;
			}
			else
			{
				if(!SbdoD.IsWall())                            continue;
			}

			CArray<T_ELEM_K, T_ELEM_K> aSbdoElemKeys;
			aSbdoElemKeys.RemoveAll();
			int nSubElem = m_pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoKeys[i], aSbdoElemKeys);

			aAllKeys.Append(aSbdoElemKeys);
		}
	}

	return static_cast<int>(aAllKeys.GetSize());
}

void CDgnForceCtrl::GetActivePlateList(BOOL bWall/*FLASE*/)
{
	m_PlateKeyList.RemoveAll();
    m_ValidPlateKeyList.RemoveAll();
	m_aSbdoByElem.RemoveAll();
	m_aSbdoList.RemoveAll();
	m_aMadoBySbdo.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> aAllKeys;
	m_pDgnDataCtrl->m_pMembCtrl->GetPlateListForDgn(aAllKeys, bWall);
	if(aAllKeys.GetSize() == 0) return;

	std::vector<T_MADO_K> aMadoKeys; // Main domain key list. (설계시는 sub-domain만 사용)
	m_pDoc->m_pAttrCtrl->GetMadoKeyList(aMadoKeys);
	int nMado = static_cast<int>(aMadoKeys.size());
	if(nMado==0)  return;
	for(int k=0; k<nMado; k++)
	{
		std::vector<T_SBDO_K> aSbdoKeys; // Sub domain key list. (설계시는 sub-domain만 사용)
		T_MADO_K MadoK=aMadoKeys[k];
		int nSbdo = m_pDoc->m_pAttrCtrl->GetSbdoKeyList(MadoK, aSbdoKeys);
		if(nSbdo==0)  continue;
		CVectorUtil::Sort(aSbdoKeys);
		for(int i=0; i<nSbdo; i++)
		{
			T_SBDO_K SbdoK = aSbdoKeys[i];
			m_aMadoBySbdo.SetAt(SbdoK, MadoK);

			T_SBDO_D SbdoD;
			if(!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))  continue;
			if(!bWall)
			{
				if(SbdoD.IsNone() || SbdoD.IsWall())  continue;
			}
			else
			{
				if(!SbdoD.IsWall()) continue;
			}

			CArray<T_ELEM_K, T_ELEM_K> aSbdoElemKeys;
			aSbdoElemKeys.RemoveAll();
			int nSubElem = m_pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoKeys[i], aSbdoElemKeys);

			CArray<T_ELEM_K, T_ELEM_K> aActiveK;
			aActiveK.RemoveAll();
			CArrayUtil::IntersectUINT(aAllKeys,aSbdoElemKeys,aActiveK);

            const INT_PTR nSizeLcom = aActiveK.GetSize();
            for ( INT_PTR j=0; j<nSizeLcom; ++j )
            {
                m_aSbdoByElem.SetAt(aActiveK[j], SbdoK);
                m_aSbdoList.SetAt(SbdoK, SbdoK);
            }

			m_PlateKeyList.Append(aActiveK);
		}
	}
}

BOOL CDgnForceCtrl::IsExistSamePeriResData(UINT PntK, _DGN_PERI_RES &PeriRes,CArray<_DGN_PERI_RES,_DGN_PERI_RES> &arPeriRes)
{	
	double dCompDist = m_pDgnDataCtrl->GetMaxDistFormPnt(PntK, PeriRes.iSlabNo);
  int nPeri = static_cast<int>(arPeriRes.GetSize());
	for(int i=0; i<nPeri; ++i)
	{
		if(m_pDgnDataCtrl->IsSameLine(arPeriRes[i].dCx,arPeriRes[i].dCy,PeriRes.dCx,PeriRes.dCy))
		{
			// Seungjun-20090506 PilePoint에서 더 먼 곳을 저장.
			double dOrgDist = m_pDgnDataCtrl->GetMaxDistFormPnt(PntK, arPeriRes[i].iSlabNo);
			if(dCompDist < dOrgDist)
			{
				arPeriRes[i] = PeriRes;
			}
			return TRUE; 
		}
	}
	return FALSE; 
}

void CDgnForceCtrl::GetOrSetSbdoVector(T_SBDO_K SbdoK, double dVectrSBDO[3][3])
{
    if ( m_bWoodArmer )
    {
        double dVectorGZ[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
        memcpy(dVectrSBDO, dVectorGZ, sizeof(double)*3*3);
        return;
    }

    memset(dVectrSBDO, NULL, sizeof(double)*3*3);

	T_VECTOR3BY3D DgnVec;
	if (!m_mSbdoVectr.Lookup(SbdoK, DgnVec))
	{
		m_pDoc->m_pPostCtrl->Get_SBDO_LocalVector(SbdoK, dVectrSBDO, m_aSubSubDomain, m_aSubSubDomainVec, TRUE);

		memcpy(DgnVec.dVector, dVectrSBDO, sizeof(DgnVec.dVector));
		m_mSbdoVectr.SetAt(SbdoK, DgnVec);
	}
	else
	{
		memcpy(dVectrSBDO, DgnVec.dVector, sizeof(DgnVec.dVector));
	}
}

void CDgnForceCtrl::SetWoodArmerOption()
{
	m_bWoodArmer = TRUE;

    if ( CDBLib::IsCodeForMeshedDesignCH() )
    {
        m_bWoodArmer = FALSE;
    }

    if ( CDBLib::IsCodeMeshSlabDesignControl() )
    {        
        T_SLDC_D SldcD;
        if ( m_pDoc->m_pAttrCtrl2->GetSldc(SldcD) )
        {
            m_bWoodArmer = SldcD.bConsiderMxy;
        }
    }
}

int CDgnForceCtrl::GetCutlKeyList(OUT CArray<UINT,UINT>& raCutlK)
{
	raCutlK.RemoveAll();
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	ASSERT(pServiceHeadOffice);	  
	return pServiceHeadOffice->ReqService(_ULS(gps), D_GPS_GET_CUTLINE_KEYS, (void*)&raCutlK);    
}

int CDgnForceCtrl::GetCutlElemList(T_CUTL_K CutlK, const T_ELEM_K_LIST& aElemK, OUT T_ELEM_K_LIST& raCutlElemK)
{
	raCutlElemK.RemoveAll();

	T_CUTL_D CutlD;
	if(!m_pDoc->m_pAttrCtrl->GetCutl(CutlK, CutlD)) { return 0; }

	// 	T_NODE_D OneWayCheckResPoint;
	// 	OneWayCheckResPoint.x = (cutline.Point1[0]+cutline.Point2[0])/2.0;
	// 	OneWayCheckResPoint.y = (cutline.Point1[1]+cutline.Point2[1])/2.0;
	// 	OneWayCheckResPoint.z = (cutline.Point1[2]+cutline.Point2[2])/2.0;

	double dCutlPnt1[3] = { 0.0, }; // cutting line 시작점
	double dCutlPnt2[3] = { 0.0, }; // cutting line 끝점
	memcpy(dCutlPnt1, CutlD.Point1, sizeof(dCutlPnt1));
	memcpy(dCutlPnt2, CutlD.Point2, sizeof(dCutlPnt2));

	double dCutlVec[2]     = { (dCutlPnt2[0] - dCutlPnt1[0]), (dCutlPnt2[1] - dCutlPnt1[1]) };
	double dCutlVecPerp[2] = { -dCutlVec[1],  dCutlVec[0] }; // counterclockwise perp vector

	// calculate the angle of the cutting line.
	// MQC 9473
	//m_dCuttingLineAngle = CMathFunc::GetAngle(1.0, 0.0, dCutLineVector[0], dCutLineVector[1], TRUE);

	int nElem = static_cast<int>(aElemK.GetSize());

    const auto dLenTor = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_nCode_Force, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1.0e-3); // 1μm = 1/1000mm

	for (int m=0; m<nElem; ++m) 
	{
		CArray <T_NODE_D,T_NODE_D&> aCutPoints;
		aCutPoints.RemoveAll();

		T_ELEM_K ElemK = aElemK[m];
		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	{ ASSERT(FALSE);  continue; }

		double dElemCenter[3] = {0.0, 0.0, 0.0};
		int nNodeInElem = GetPlateNodeNum(ElemK, ElemD);

		for (int i=0; i<nNodeInElem; ++i)
		{
			T_NODE_D NodeD1, NodeD2;
			if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[i], NodeD1)) { continue; }

			int j = (i==nNodeInElem-1)? 0 : (i+1);
			if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[j], NodeD2)) { continue; }

			dElemCenter[0] += NodeD1.x;
			dElemCenter[1] += NodeD1.y;
			dElemCenter[2] += NodeD1.z;

			double dPoint1[3], dPoint2[3], dPoint3[3], dDistance;
			NodeD1.GetData(dPoint1);
			NodeD2.GetData(dPoint2);

			if(CMathFunc::mathIntersectLine2(dCutlPnt1, dCutlPnt2, dPoint1, dPoint2, cDgn_Zero, dDistance, dPoint3))
			{
				T_NODE_D Point3d3;
				Point3d3.SetData(dPoint3);

				aCutPoints.Add(Point3d3);
			}
            else
            {
                // GEN-4558 한 점이 라인 위에 있는 경우 오류
                double d1 = CMathFunc::mathDistanceToLine(dCutlPnt1, dCutlPnt2, dPoint1);
                double d2 = CMathFunc::mathDistanceToLine(dCutlPnt1, dCutlPnt2, dPoint2);

                if((EQ0(d1, dLenTor) && UQ0(d2, dLenTor))) // Point1 is on line.
                {
                    T_NODE_D Point3d3;
                    Point3d3.SetData(dPoint1);

                    aCutPoints.Add(Point3d3);
                }
                else if ((UQ0(d1, dLenTor) && EQ0(d2, dLenTor)))  // Point2 is on line.
                {
                    T_NODE_D Point3d3;
                    Point3d3.SetData(dPoint2);

                    aCutPoints.Add(Point3d3);
                }
            }

			if(aCutPoints.GetSize()==0) { continue; }
		}

		// 같은 점이 있을 경우 없앤다.        
		for (int i=0; i<aCutPoints.GetSize()-1; ++i)
		{
			for (int j=i+1; j<aCutPoints.GetSize(); ++j)
			{
				if (m_pDgnDataCtrl->IsSame3DPoint(aCutPoints[i],aCutPoints[j]))
				{
					aCutPoints.RemoveAt(j);
					j--;
				}
			}	
		}

		if(aCutPoints.GetSize() != 2) { continue; }//최종적으로 2개의 서로다른 점이 존재해야한다.				

		int nLeftRight = m_nCuttingDiagramLeftRight; // 0=Both, 1=Left, 2=Right
		if (!IsPossibleCuttingLine(ElemK, dCutlPnt1, dCutlPnt2, aCutPoints[0], aCutPoints[1], nLeftRight)) { continue; }

		// Left or Right side?
		dElemCenter[0] /= static_cast<double>(nNodeInElem);
		dElemCenter[1] /= static_cast<double>(nNodeInElem);
		dElemCenter[2] /= static_cast<double>(nNodeInElem);

		const auto enDir = CPositionChecker::checkPosition(dCutlPnt1, dCutlPnt1, dElemCenter);
		switch(enDir)
		{
			case Position::LEFT:
			case Position::TOP:
			{
				if (nLeftRight != 2) raCutlElemK.Add(ElemK);
				break;
			}
			case Position::RIGHT:
			case Position::BOTTOM:
			{
				if (nLeftRight != 1) raCutlElemK.Add(ElemK);
				break;
			}
			default:
			ASSERT(0);
			break;
		}
	}

	return static_cast<int>(raCutlElemK.GetSize());
}


void CDgnForceCtrl::InitSbdoVector(int nSbdoNo)
{
	int nHashSize = GF_GetHashSizeByCount(nSbdoNo);
	m_mSbdoVectr.RemoveAll();
	m_mSbdoVectr.InitHashTable(max(1, nHashSize));
}

void CDgnForceCtrl::ResetStoryData()
{
	m_arStorbyElem.RemoveAll();
	m_arElemListbyStor.RemoveAll();
	m_arStorK.RemoveAll();
}

void CDgnForceCtrl::SetStoryData()
{
	m_arStorbyElem.RemoveAll();
	m_arElemListbyStor.RemoveAll();
	m_arStorK.RemoveAll();

	CStoryData* pStoryData = new CStoryData(m_pDoc);
	//  0   // 해당층과 층하부로 연결된 절점, 요소
	//  1   // 해당층과 층상부로 연결된 절점, 요소
	//  2   // 해당층과 층상부와 층하부로 연결된 모든 절점, 요소
	//  3   // 해당층과 층하부의 모든 절점, 요소
	//  4   // 해당층과 층상부의 모든 절점, 요소
	//  5   // 해당층만
	pStoryData->MakeStoryData(0);//0 또는 1이 아니면 return
	m_pDoc->m_pAttrCtrl->GetStorKeyList(m_arStorK);

	_DGN_ELEM_LIST arStroElemK;

	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	int nStor = static_cast<int>(m_arStorK.GetSize());
	for(int i=0; i<nStor; i++)
	{
		T_STOR_K StorK = m_arStorK[i];
		arElemK.RemoveAll();
		pStoryData->GetStoryElemKey(StorK, arElemK);
		arStroElemK.Initialize();
		arStroElemK.arElemList.Copy(arElemK);
		m_arElemListbyStor.SetAt(StorK, arStroElemK);

		int nElem = static_cast<int>(arElemK.GetSize());
		for(int j=0; j<nElem; j++)
		{
			int ElemK = arElemK[j];
			m_arStorbyElem.SetAt(ElemK, StorK);
		}
	}
	if(pStoryData!=NULL)	delete pStoryData;
}

BOOL CDgnForceCtrl::ExistStory()
{
	// void CDgnForceCtrl::SetStoryData() 호출 후 사용
	return m_arStorK.GetSize()>0 ? TRUE : FALSE;
}

int  CDgnForceCtrl::GetPlateNodeNum(const T_ELEM_K& ElemK, const T_ELEM_D& ElemD)
{
	int nie = 0;
	if(!m_mapNodeInElem.Lookup(ElemK, nie))
	{
		nie=m_pDgnDataCtrl->Get_Node_in_Elem(const_cast<T_ELEM_D&>(ElemD));
		m_mapNodeInElem.SetAt(ElemK,nie);
	}

	return nie;
}

double CDgnForceCtrl::GetShelForc4Cell(EN_SHELFORC_IDX enForcIdx, const T_SLAB_FORCE_COMPACT& Forc1, int nNodeIdx1, const T_SLAB_FORCE_COMPACT& Forc2, int nNodeIdx2, double dLeng)
{
	return (Forc1.dForce[nNodeIdx1+enForcIdx] + Forc2.dForce[nNodeIdx2+enForcIdx])*dLeng/2.0;
}

void CDgnForceCtrl::CalcPlateLocalForce4Civil(const int& nPlateForceLocalGlobal, const CString& strUSC, const T_SBDO_K& SbdoK, const int& elem_key, T_STRP_DL&	data_max, T_STRP_DL& data_min, double LVec[3][3])
{// CForceEngine::SetPForcDataSub 참조
    memset(LVec,0,sizeof(double)*9);

	// UCS mode
	if(nPlateForceLocalGlobal == 1)
	{ 
		// Get UCS Vector
		double UCS_vector[3][3];
		memset(UCS_vector,0,sizeof(UCS_vector));

		T_NUCS_D NucsD; // UCS Data
		NucsD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetNucs(const_cast<CString&>(strUSC), NucsD))
		{ NucsD.Initialize();  NucsD.dVx[0] = 1.0;  NucsD.dVy[1] = 1.0; }

		double dVz[3];
		CMathFunc::mathCross(NucsD.dVx,NucsD.dVy,dVz);

		memcpy(UCS_vector[0],NucsD.dVx,sizeof(UCS_vector[0]));
		memcpy(UCS_vector[1],NucsD.dVy,sizeof(UCS_vector[1]));
		memcpy(UCS_vector[2],      dVz,sizeof(UCS_vector[2]));
		CMathFunc::mathNormalize(UCS_vector[0],UCS_vector[0]);
		CMathFunc::mathNormalize(UCS_vector[1],UCS_vector[1]);
		CMathFunc::mathNormalize(UCS_vector[2],UCS_vector[2]);	

		double local_vector[3][3];
		memset(local_vector,0,sizeof(local_vector));
		if(!m_pDoc->calcPlaneLocalVector(elem_key, local_vector)) { ASSERT(0); return; }

		CMathFunc::mathNormalize(local_vector[2],local_vector[2]);
		double dDot=fabs(CMathFunc::mathDot(local_vector[2],UCS_vector[2]));

		if( 1.0-dDot < 0.0001523) // 1.0 Degree
		{
			if(!m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector,UCS_vector,data_max)) ASSERT(0);
			if(!m_pDoc->m_pPostCtrl->ConvertStrp_UCS(local_vector,UCS_vector,data_min)) ASSERT(0);
		}

        memcpy(LVec, UCS_vector, sizeof(UCS_vector));
	}
	else if(nPlateForceLocalGlobal == 2)
	{
		double dDummy = 0.0;
		double aDummy[3];
		
		double dVecProjectIntoElem[3][3];
		if(m_pDoc->m_pPostCtrl->GetWamtInfo() == nullptr) m_pDoc->m_pPostCtrl->MakeSubdomainInfo(); // MQC.5537-5 WamtInfo포인터가 null인 경우 생성.
		if(!m_pDoc->m_pPostCtrl->GetWamtInfo()->GetWamtProjUCS(SbdoK, elem_key, dVecProjectIntoElem, dDummy, aDummy)) { ASSERT(0); return; }

		double dElemLocalVec[3][3];	
		if(!m_pDoc->calcElemLocalVector(elem_key, dElemLocalVec)) { ASSERT(0); return; }

		// 방향이 같다면 축변환 필요없음.
		const double dRegardZero = 1.0e-9;
		BOOL bSameLocalVec = TRUE;
		for (int i=0; i<3; ++i)
		{
			for (int k=0; k<3; ++k)
			{
				if (fabs(dElemLocalVec[i][k] - dVecProjectIntoElem[i][k]) > dRegardZero) { bSameLocalVec = FALSE;  break; }
			}
			if (!bSameLocalVec) break;
		}

		if (!bSameLocalVec)
		{
			m_pDoc->m_pPostCtrl->ConvertStrp_UCS(dElemLocalVec, dVecProjectIntoElem, data_max);
			m_pDoc->m_pPostCtrl->ConvertStrp_UCS(dElemLocalVec, dVecProjectIntoElem, data_min);
		}

        memcpy(LVec, dVecProjectIntoElem, sizeof(dVecProjectIntoElem));
	}
    else
    {
        if(!m_pDoc->calcPlaneLocalVector(elem_key, LVec)) { ASSERT(0); return; }
    }
}

void CDgnForceCtrl::GetShellDgnVector(double dAngle, double adSrcVec[3][3], OUT double adDgnVec[3][3])
{	
	for (int i=0; i<3; ++i)
	{
		for (int m=0; m<3; ++m)
		{
			adDgnVec[i][m] = adSrcVec[i][m];
		}
	}

	CMathFunc::mathRotate(dAngle, adSrcVec[2][0], adSrcVec[2][1], adSrcVec[2][2], adDgnVec[0][0], adDgnVec[0][1], adDgnVec[0][2]);
	CMathFunc::mathRotate(dAngle, adSrcVec[2][0], adSrcVec[2][1], adSrcVec[2][2], adDgnVec[1][0], adDgnVec[1][1], adDgnVec[1][2]);
}

BOOL CDgnForceCtrl::CheckOverlapCutLine(const CMapEx<UINT, UINT, DGN_CUTLINE_AR, DGN_CUTLINE_AR&>& mCutl)
{
    T_KEY_LIST aNodeK;
    int nNode = mCutl.GetKeyList(aNodeK);
    if (nNode < 1) return FALSE;

    qsort(aNodeK.GetData(), aNodeK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

    union _DGN_OVERLAP_INFO
    {
        UINT KeyMap;
        struct
        {
            unsigned int NodeK1 : 16;
            unsigned int NodeK2 : 16;
        }Key;
    };

    CMap<UINT, UINT, BOOL, BOOL> mOverlap;  

    for (int i=0; i<nNode; ++i)
    {
        T_KEY NodeK_1st = aNodeK[i];

        T_NODE_D NodeD_1st;
        if ( !m_pDoc->m_pAttrCtrl->GetNode(NodeK_1st, NodeD_1st) ) continue;

        const double dZ_1st = NodeD_1st.z;

        DGN_CUTLINE_AR Cutl1st;
        if (!mCutl.Lookup(NodeK_1st, Cutl1st)) continue;

        CRgn Rgn1st;
        if (!MakeRgnCutLine(Cutl1st, Rgn1st)) continue;        

        for (int j=i+1; j<nNode; ++j)
        {
            T_KEY NodeK_2nd = aNodeK[j];

            T_NODE_D NodeD_2nd;
            if ( !m_pDoc->m_pAttrCtrl->GetNode(NodeK_2nd, NodeD_2nd) ) continue;

            const double dZ_2nd = NodeD_2nd.z;

            if ( fabs(dZ_1st-dZ_2nd) > cDgn_Zero ) continue;

            DGN_CUTLINE_AR Cutl2nd;
            if (!mCutl.Lookup(NodeK_2nd, Cutl2nd)) continue;

            CRgn Rgn2nd;
            if (!MakeRgnCutLine(Cutl2nd, Rgn2nd)) continue;        

            CRgn RgnCom;
            RgnCom.CreateRectRgn(0, 0, 100, 100);
            int nCombResult = RgnCom.CombineRgn(&Rgn1st, &Rgn2nd, RGN_AND);
            if (nCombResult != ERROR && nCombResult != NULLREGION)
            {
                _DGN_OVERLAP_INFO OverInfo;
                OverInfo.Key.NodeK1 = NodeK_1st;
                OverInfo.Key.NodeK2 = NodeK_2nd;

                mOverlap.SetAt(OverInfo.KeyMap, TRUE);                                
            }
        }
    }

    if (mOverlap.GetCount() > 0)
    {
        if ( m_pDoc->IsRegTestMode() )
        {
            GSaveHistoryNF(_LS(IDS_DGN_PUNCHING_PERIMETER_OVERLAP));
        }
        else
        {
            AfxMessageBox(_LS(IDS_DGN_PUNCHING_PERIMETER_OVERLAP));
        }
    }

    return TRUE;
}

BOOL CDgnForceCtrl::MakeRgnCutLine(const DGN_CUTLINE_AR& Cutl, OUT CRgn& rRgn)    
{
    int nPos = static_cast<int>(Cutl.arCutLine.GetSize());
    if (nPos < 1) { return FALSE; }

    CPoint* aPnt = new CPoint[nPos+1];
    for (int i=0; i<nPos; ++i)
    {
        const DGN_CUTLINE_D& CurCutl = Cutl.arCutLine[i];
        aPnt[i].x = int(CurCutl.Point1.x);
        aPnt[i].y = int(CurCutl.Point1.y);        
    }

    aPnt[nPos].x = int(Cutl.arCutLine[nPos-1].Point2.x);
    aPnt[nPos].y = int(Cutl.arCutLine[nPos-1].Point2.y);

    return rRgn.CreatePolygonRgn(aPnt, nPos+1, ALTERNATE) == 0 ? FALSE : TRUE;
}

BOOL CDgnForceCtrl::CheckOverlapPunchingCutLine()
{
	const bool bForce = m_nPnChkType == 0 ? true : false;
	CArray<UINT, UINT> aNodeK; aNodeK.RemoveAll();
    const int nNode = m_pDoc->m_pPostCtrl->GetDesignResult()->Get_PunchKeyList((bForce ? 0 : 1), aNodeK);
    if (nNode < 1) return FALSE;

    qsort(aNodeK.GetData(), aNodeK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

    union _DGN_OVERLAP_INFO
    {
        UINT KeyMap;
        struct
        {
            unsigned int NodeK1 : 16;
            unsigned int NodeK2 : 16;
        }Key;
    };

    CMap<UINT, UINT, BOOL, BOOL> mOverlap;  

    for (int i=0; i<nNode; ++i)
    {
        T_KEY NodeK_1st = aNodeK[i];

        T_NODE_D NodeD_1st;
        if ( !m_pDoc->m_pAttrCtrl->GetNode(NodeK_1st, NodeD_1st) ) continue;

        const double dZ_1st = NodeD_1st.z;

        CArray<T_RSS2_CTLN,T_RSS2_CTLN&> Cutl1st;
		if(bForce)
		{
			T_RSF2_D SlabRes;
			if(!m_pDoc->m_pPostCtrl->GetDesignResult()->Get_SlabPunchShearForResult(NodeK_1st, SlabRes)) continue;
			Cutl1st.Copy(SlabRes.arPnChkCutl);
		}
		else
		{
			T_RSS2_D SlabRes;
			if(!m_pDoc->m_pPostCtrl->GetDesignResult()->Get_SlabPunchShearStrResult(NodeK_1st, SlabRes)) continue;
			Cutl1st.Copy(SlabRes.arPnChkCutl);
		}

        CRgn Rgn1st;
        if (!MakeRgnPunchingCutLine(Cutl1st, Rgn1st)) continue;        

        for (int j=i+1; j<nNode; ++j)
        {
            T_KEY NodeK_2nd = aNodeK[j];

            T_NODE_D NodeD_2nd;
            if ( !m_pDoc->m_pAttrCtrl->GetNode(NodeK_2nd, NodeD_2nd) ) continue;

            const double dZ_2nd = NodeD_2nd.z;

            if ( fabs(dZ_1st-dZ_2nd) > cDgn_Zero ) continue;

            CArray<T_RSS2_CTLN,T_RSS2_CTLN&> Cutl2nd;
			if(bForce)
			{
				T_RSF2_D SlabRes;
				if(!m_pDoc->m_pPostCtrl->GetDesignResult()->Get_SlabPunchShearForResult(NodeK_2nd, SlabRes)) continue;
				Cutl2nd.Copy(SlabRes.arPnChkCutl);
			}
			else
			{
				T_RSS2_D SlabRes;
				if(!m_pDoc->m_pPostCtrl->GetDesignResult()->Get_SlabPunchShearStrResult(NodeK_2nd, SlabRes)) continue;
				Cutl2nd.Copy(SlabRes.arPnChkCutl);
			}

            CRgn Rgn2nd;
            if (!MakeRgnPunchingCutLine(Cutl2nd, Rgn2nd)) continue;        

            CRgn RgnCom;
            RgnCom.CreateRectRgn(0, 0, 100, 100);
            int nCombResult = RgnCom.CombineRgn(&Rgn1st, &Rgn2nd, RGN_AND);
            if (nCombResult != ERROR && nCombResult != NULLREGION)
            {
                _DGN_OVERLAP_INFO OverInfo;
                OverInfo.Key.NodeK1 = NodeK_1st;
                OverInfo.Key.NodeK2 = NodeK_2nd;

                mOverlap.SetAt(OverInfo.KeyMap, TRUE);                                
            }
        }
    }

    if (mOverlap.GetCount() > 0)
    {
        if ( m_pDoc->IsRegTestMode() )
        {
            GSaveHistoryNF(_LS(IDS_DGN_PUNCHING_PERIMETER_OVERLAP));
        }
        else
        {
            AfxMessageBox(_LS(IDS_DGN_PUNCHING_PERIMETER_OVERLAP));
        }
    }

    return TRUE;
}

BOOL CDgnForceCtrl::MakeRgnPunchingCutLine(const CArray<T_RSS2_CTLN,T_RSS2_CTLN&>& arPnChkCutl, OUT CRgn& rRgn)
{
    int nPos = static_cast<int>(arPnChkCutl.GetSize());
    if (nPos < 1) { return FALSE; }

    CPoint* aPnt = new CPoint[nPos+1];
    for (int i=0; i<nPos; ++i)
    {
        const T_RSS2_CTLN& CurCutl = arPnChkCutl[i];
        aPnt[i].x = int(CurCutl.Point1[0]);
        aPnt[i].y = int(CurCutl.Point1[1]);        
    }

    aPnt[nPos].x = int(arPnChkCutl[nPos-1].Point2[0]);
    aPnt[nPos].y = int(arPnChkCutl[nPos-1].Point2[1]);

    return rRgn.CreatePolygonRgn(aPnt, nPos+1, ALTERNATE) == 0 ? FALSE : TRUE;
}

void CDgnForceCtrl::GetCutLinePoints(const _DGN_PERI_POS& PeriPos, double dLevel, OUT double adCutPnt1[3], OUT double adCutPnt2[3])
{
    adCutPnt1[0] = PeriPos.dCx[0];
    adCutPnt1[1] = PeriPos.dCy[0];
    adCutPnt1[2] = dLevel; 

    adCutPnt2[0] = PeriPos.dCx[1];
    adCutPnt2[1] = PeriPos.dCy[1];
    adCutPnt2[2] = dLevel; 
}