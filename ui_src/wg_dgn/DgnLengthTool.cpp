#include "StdAfx.h"

#include "DgnLengthTool.h"

#include "../wg_db/UnitCtrl.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/AttrCtrl2.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/MatlDB.h"
#include "../wg_db/DBLib.h"

#include "../wg_base/GTF.h"

#include "../wg_rcs/RCSStruct.h"
#include "../wg_db/MathFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDgnLengthTool::CDgnLengthTool()
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnLengthTool::~CDgnLengthTool()
{
}

double CDgnLengthTool::Get_2DMembLeng(T_SBDO_K SbdoK, int nLengthUnit, CMapEx<int, int, double, double>* pMap/* = nullptr*/)
{
	// PMS:xxx-Seungjun-20100830 요소별 length by sub-domain. sub-domain이 동일 평면이 아닐 경우. 
	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	int nElem =m_pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElemK);
	if ( nElem <= 0 )
	{
		return 0.0;
	}

	if ( pMap )
	{
		pMap->RemoveAll();
		pMap->InitHashTable(GF_GetHashSizeByCount(nElem));
	}

	CArray<CArray<UINT, UINT>*, CArray<UINT, UINT>*> aEachPlaneElem;
	int nPlane = m_pDoc->m_pAttrCtrl2->GetPlaneElemListInSbdo(SbdoK, aEachPlaneElem);  // Sub-Domain안에 평면요소 리스트

	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = nLengthUnit;
	CngIndex.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	double dPlaneTol = CDBLib::Get_SlabSamePlaneTol(nLengthUnit);
	// <Remember> Data를 받아올때는 Code Unit기준
	//////////////////////////////////////////////

	// MQC:8433 Seungjun-20140820 : Domain이 임의 각도를 가질 때 슬래브 길이 계산 수정.
	T_SBDO_D SbdoD;
	if ( !m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD) )   SbdoD.Initialize();
	double dDomainAngle = SbdoD.dAngle[0];

	double dMinLen = cRCS_Upon;

	for ( int k=0; k<nPlane; k++ )
	{
		CArray<T_ELEM_K, T_ELEM_K> aPlaneElemK;
		aPlaneElemK.RemoveAll();
		const INT_PTR nPlaneElemK = aEachPlaneElem[k]->GetSize();
		for ( INT_PTR i=0; i<nPlaneElemK; ++i )
		{
			aPlaneElemK.Add(aEachPlaneElem[k]->GetAt(i));
		}

		CArray<T_NODE_K, T_NODE_K> raNodeK;
		int nNode = m_pDoc->m_pAttrCtrl->Find3DCornerNode(aPlaneElemK, raNodeK, dPlaneTol);

		// MQC:5012-PARKHJ-20120726
		double dAxisVec[3] = { 0.0, 0.0, 0.0 };
		double dRotAngle = 0.0;
		if ( aPlaneElemK.GetSize() > 0 )
		{
			T_ELEM_K ElemK = aPlaneElemK[0];
			Get_ProjectionFactor2XYPlane(ElemK, dAxisVec, dRotAngle);
		}
		//////////////////////////

		double dMax[2]= { -1.0*cRCS_Upon, -1.0*cRCS_Upon };
		double dMin[2]= { cRCS_Upon, cRCS_Upon };
		for ( int i=0; i<nNode; i++ )
		{
			T_NODE_D OrgNodeD; OrgNodeD.Initialize();
			T_NODE_D TrsNodeD; TrsNodeD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetNode(raNodeK[i], OrgNodeD) ) continue;

			// MQC:5012-PARKHJ-20120726
			Get_ProjectionNode2XYPlane(dRotAngle, dAxisVec, OrgNodeD, TrsNodeD);
			CMathFunc::mathRotateZ(-dDomainAngle, TrsNodeD.x, TrsNodeD.y, TrsNodeD.z);// MQC:8433 Seungjun-20140820 : Domain이 임의 각도를 가질 때 슬래브 길이 계산 수정.
			if ( dMax[0] < TrsNodeD.x ) dMax[0] = TrsNodeD.x;
			if ( dMax[1] < TrsNodeD.y ) dMax[1] = TrsNodeD.y;

			if ( dMin[0] > TrsNodeD.x ) dMin[0] = TrsNodeD.x;
			if ( dMin[1] > TrsNodeD.y ) dMin[1] = TrsNodeD.y;
		}

		const double dLx = fabs(dMax[0]-dMin[0]);
		const double dLy = fabs(dMax[1]-dMin[1]);
		const double dLen = min(dLx, dLy);

		dMinLen = min(dMinLen, dLen);

		if ( pMap )
		{
			for ( INT_PTR i=0; i<nPlaneElemK; ++i )
			{
				pMap->SetAt(aPlaneElemK[i], dLen);
			}
		}
	}

	//////////////////////////////////////////////
	// <Remember> Data를 쓰고나서는 User Unit기준
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	// <Remember> Data를 쓰고나서는 User Unit기준
	//////////////////////////////////////////////

	// 메모리 해제 하는 부분
	for ( int i=0; i<nPlane; i++ )
	{
		if ( aEachPlaneElem[i] != NULL )
		{
			delete aEachPlaneElem[i];
			aEachPlaneElem[i] = NULL;
		}
	}
	aEachPlaneElem.RemoveAll();

	return dMinLen;
}

void CDgnLengthTool::Get_ProjectionFactor2XYPlane(T_ELEM_K ElemK, double dAxisVec[], double& dRotAngle)
{
	double PlaneLocalVector[3][3];
	m_pDoc->calcPlaneLocalVector(ElemK, PlaneLocalVector);

	double dXYPlane[3] = { 0.0, 0.0, 1.0 };
	dAxisVec[0] = 0.0;
	dAxisVec[1] = 0.0;
	dAxisVec[2] = 0.0;
	CMathFunc::mathCross(dXYPlane, PlaneLocalVector[2], dAxisVec);
	dRotAngle = CMathFunc::mathCrossAngleNormalize(dXYPlane, PlaneLocalVector[2]);
}

void CDgnLengthTool::Get_ProjectionNode2XYPlane(double dRotAngle, double dAxisVec[], const T_NODE_D& OrgNode, T_NODE_D& TrsNode)
{
	// MQC:5012-PARKHJ-20120726 : 경사진 슬래브 처리를 위해 XY 평면 위로 투사한 좌표로 전환
	TrsNode.Initialize();

	double dPx = OrgNode.x;
	double dPy = OrgNode.y;
	double dPz = OrgNode.z;
	CMathFunc::mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], dPx, dPy, dPz);

	TrsNode.x = dPx;
	TrsNode.y = dPy;
	TrsNode.z = dPz;
}
