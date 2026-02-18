#if !defined(__DgnSeisAnchorShapeMgr_H__)
#define __DgnSeisAnchorShapeMgr_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorShapeMgr.h : header file
//

#include "..\wg_base\TypeEx.h"
#include "..\wg_db\GeomFunc.h"
#include <set>
#include "..\MIT_lib\StructKeyForStdMap.h"
#include <vector>
struct TPryoutGroupInfo
{
	TPryoutGroupInfo()
	{
		Init();
	}
	void Init()
	{
		aBearingIdx.clear();
		dArea = 0.0;
		nBc = 0;
	}
	std::vector<std::pair<int, int>> aBearingIdx;
	double dArea;
	int nBc;
};

struct TConcFailBcCalc
{
	TConcFailBcCalc()
	{
		Init();
	}
	void Init()
	{
		aBearingIdx.clear();
		LpLine[0][0] = 0.0;
		LpLine[0][1] = 0.0;
		LpLine[1][0] = 0.0;
		LpLine[1][1] = 0.0;
		dSax = 0.0;
		dLp = 0.0;
		nBc = 0;		
		bSingleResist = FALSE;
	}
	std::vector<std::pair<int, int>> aBearingIdx;
	double LpLine[2][2];
	double dSax; // 단일 저항일 경우엔 좌,우 혹은 위,아래를 보고 sax를 결정함.. 그룹 저항일 경우엔 그룹 내부에서의 Gap 만으로 sax 를 결정함
	double dLp;
	int nBc;
	BOOL bSingleResist;
};

struct TAnchorPosIdx 
{
	TAnchorPosIdx()
	{
		Init();
	}
	TAnchorPosIdx(int BearingI, int BearingJ, int AnchorI, int AnchorJ)
	{
		nBearingI = BearingI;
		nBearingJ = BearingJ;
		nAnchorI = AnchorI;
		nAnchorJ = AnchorJ;
	}

	void Init()
	{
		nBearingI = 0;
		nBearingJ = 0;
		nAnchorI = 0;
		nAnchorJ = 0;
	}
	int nBearingI;
	int nBearingJ;
	int nAnchorI;
	int nAnchorJ;
};

//////////////////////////////////////////////////////////////////////////
struct KeyForConFailPos : public StructKeyForStdMap
{
	KeyForConFailPos() : StructKeyForStdMap()
	{

	}
	KeyForConFailPos(int BrevK, int nDir, TConcFailBcCalc& EachGroup) : StructKeyForStdMap()
	{
		int nResist = EachGroup.bSingleResist ? 0 : 1;

		aInt.push_back(BrevK);
		aInt.push_back(nDir);
		aInt.push_back(EachGroup.nBc);
		aInt.push_back(nResist);
		for (int i = 0; i < EachGroup.aBearingIdx.size(); i++)
		{
			aInt.push_back(EachGroup.aBearingIdx[i].first);
			aInt.push_back(EachGroup.aBearingIdx[i].second);
		}
		aDbl.push_back(EachGroup.LpLine[0][0]);
		aDbl.push_back(EachGroup.LpLine[0][1]);
		aDbl.push_back(EachGroup.LpLine[1][0]);
		aDbl.push_back(EachGroup.LpLine[1][1]);
		aDbl.push_back(EachGroup.dSax);
		aDbl.push_back(EachGroup.dLp);
	}
};

struct ValForConFail
{
	double dRatio;

	ValForConFail()
	{
		Initialize();
	}
	ValForConFail(ValForConFail& src) { *this = src; }
	ValForConFail& operator=(const ValForConFail& src)
	{
		dRatio = src.dRatio;

		return *this;
	}
	void Initialize()
	{
		dRatio = 0.0;
	}
	void Set(double dRatioCal)
	{
		dRatio = dRatioCal;
	}

};

struct KeyForPryoutPos : public StructKeyForStdMap
{
	KeyForPryoutPos() : StructKeyForStdMap()
	{

	}
	KeyForPryoutPos(int BrevK, int nDir, TPryoutGroupInfo& EachGroup) : StructKeyForStdMap()
	{
		aInt.push_back(BrevK);
		aInt.push_back(nDir);
		aInt.push_back(EachGroup.nBc);
		for (int i = 0; i < EachGroup.aBearingIdx.size(); i++)
		{
			aInt.push_back(EachGroup.aBearingIdx[i].first);
			aInt.push_back(EachGroup.aBearingIdx[i].second);
		}
		aDbl.push_back(EachGroup.dArea);
	}
};

struct ValForPryout
{
	double dRatio;

	ValForPryout()
	{
		Initialize();
	}
	ValForPryout(ValForPryout& src) { *this = src; }
	ValForPryout& operator=(const ValForPryout& src)
	{
		dRatio = src.dRatio;

		return *this;
	}
	void Initialize()
	{
		dRatio = 0.0;
	}
	void Set(double dRatioCal)
	{
		dRatio = dRatioCal;
	}
};

//////////////////////////////////////////////////////////////////////////
struct ValForPryoutBC
{
	CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&> aArProjShapeX;
	CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&> aArProjShapeY;
	std::set<int> setBcIdx;
	int nBc;
	double dArea;
	double dSax;
	double dSay;
	BOOL bSingleResist;

	void Initialize()
	{
		aArProjShapeX.RemoveAll();
		aArProjShapeY.RemoveAll();
		setBcIdx.clear();

		nBc = 0;
		dArea = 0.0;
		dSax = 0.0;
		dSay = 0.0;
		bSingleResist = FALSE;
	}
	ValForPryoutBC() { Initialize(); }
	ValForPryoutBC(ValForPryoutBC& src) { *this = src; }
	ValForPryoutBC& operator=(const ValForPryoutBC& src)
	{
		aArProjShapeX.Copy(src.aArProjShapeX);
		aArProjShapeY.Copy(src.aArProjShapeY);
		for (double EachVal : src.setBcIdx)
		{
			setBcIdx.insert(EachVal);
		}
		nBc = src.nBc;
		dArea = src.dArea;
		dSax = src.dSax;
		dSay = src.dSay;
		bSingleResist = src.bSingleResist;
		return *this;
	}
	void Set(CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&>& raArProjShapeX,
		CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&>& raArProjShapeY,
		std::set<int>& rsetBcIdx, int& rnBc, double& rdArea, double& rdSax, double& rdSay, BOOL& rbSingleResist)
	{
		aArProjShapeX.Copy(raArProjShapeX);
		aArProjShapeY.Copy(raArProjShapeY);
		for (double EachVal : rsetBcIdx)
		{
			setBcIdx.insert(EachVal);
		}
		nBc = rnBc;
		dArea = rdArea;
		dSax = rdSax;
		dSay = rdSay;
		bSingleResist = rbSingleResist;
	}
};

struct KeyForPryoutBCPos : public StructKeyForStdMap
{
	KeyForPryoutBCPos() : StructKeyForStdMap()
	{

	}
	KeyForPryoutBCPos(int nDir, int nPryoutRepresentGrpIdx, int nDownUpPry, int nLeftRightPry) : StructKeyForStdMap()
	{
		aInt.push_back(nDir);
		aInt.push_back(nPryoutRepresentGrpIdx);
		aInt.push_back(nDownUpPry);
		aInt.push_back(nLeftRightPry);
	}
};

class CDBDoc;
class CBrdgSeisEval;
#include "HeaderPre.h" 
class __MY_EXT_CLASS__ CDgnSeisAnchorShapeMgr
{
public:
	enum EN_DIR
	{
		DIR_LONGI = 0,	// 교축 방향
		DIR_TRANS = 1   // 교축 직각 방향		
	};	

	enum EN_DIR_CHECK
	{
		DIR_POS = 0,	// 정방향 검토
		DIR_NEG = 1     // 역방향 검토
	};


	// 각 index 위치	
	//    Coping Shape Index
	//    1 ------------ 2
	//    |              |
	//    |              |
	//    0 ------------ 3

	// Anchor, Bearing Index
	//  -------------------------------------------------
	// |                                                |
	// |    |-----[1][0]----|       |-----[1][1]----|   |
	// |    |               |       |               |   |
	// |    | [1][0] [1][1] |       | [1][0] [1][1] |   |
	// |    |               |       |               |   |
	// |    | [0][0] [0][1] |       | [0][0] [0][1] |   |
	// |	|               |		|               |   |
	// |    |---------------|       |---------------|   |
	// |                                                |
	// |    |-----[0][0]----|       |-----[0][1]----|   |
	// |    |               |       |               |   |
	// |    | [1][0] [1][1] |       | [1][0] [1][1] |   |
	// |    |               |       |               |   |
	// |    | [0][0] [0][1] |       | [0][0] [0][1] |   |
	// |    |               |       |               |   |
	// |    |---------------|       |---------------|   |
	// |                                                |
	//  -------------------------------------------------


	CDgnSeisAnchorShapeMgr();
    CDgnSeisAnchorShapeMgr(const T_ANEV_K& AnevK);
	~CDgnSeisAnchorShapeMgr();

	void Init();
	void SetData(T_ANEV_D& CopyingData);
	void SetData(std::vector<std::vector<T_BRPR_D>>& aArBearingData, T_ANEV_D& CopyingData, BOOL bAnchorDir = TRUE);	
	void SetRepresentGrpIdx4Ratio(int nRepresentGrpIdx) { m_nPryoutRepresentGrpIdx = nRepresentGrpIdx; }
	void SetRatioChk(BOOL bRatioChk) { m_bRatioChk = bRatioChk; }
	BOOL CheckBrpr(T_BRPR_D& BearingData);	
	static void GetBrpr(T_ANEV_BRPR_D& BearingProp, int nBearingI, int nBearingJ);

	void SetAnchorTenConcOption(BOOL bAnchorTenConc)
	{
		m_bAnchorTenConc = bAnchorTenConc;
	}
	BOOL CheckDataBrpr();
	BOOL CheckDataBrpr(int nBearingI, int nBearingJ);
	BOOL CheckDataAnev();
	BOOL CheckDataAnev(int i, int j);	
	BOOL CheckData();

	void SetBearingWH();
	void SetCopyingWH();
	
	static T_BRPR_K GetBrprK(const T_ANEV_BRPR_D& BearingProp, int nBearingI, int nBearingJ);
	BOOL GetConcFailRepresentVal(int nDir, double& dLp, int& nBc, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P, BOOL bFixedCond=TRUE);
	BOOL GetConcFailRepresentVal4Ratio(int nDir, double& dLp, int& nBc, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P, TConcFailBcCalc& MinConcFailCal, BOOL bFixedCond = TRUE);
	BOOL GetPryoutRepresentVal(int nDir, std::vector<std::pair<int, int>> aRepBearingIdx, double& dArea, int& nBc, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P, BOOL& bSingleResist, int& nResistAnchorNum);
	double GetBearingCenterGap(int nDir, int BearingIdx[2][2]);
	double GetBearingGap(int nDir, int BearingIdx[2][2]);
	void GetPryoutRepresentGrpBearing(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdx);
	void GetConcfailRepresentGrpBearing(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdx);
	BOOL GetPryoutBearingCa(int nDir, int nBearingIdxI, int nBearingIdxJ, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P);
	BOOL GetConcFailBearingCa(int nDir, int nBearingIdxI, int nBearingIdxJ, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P);
	void GetRepresentCa1Bearing(int nDir, int& nBearingI, int& nBearingJ);
	BOOL GetReCalcHef(int nDir, int nBearingI, int nBearingJ, double& dReCalcHef);
	BOOL GetReCalcCa1(int nDir, int nBearingI, int nBearingJ, double& dReCalcCa1, std::vector<TConcFailBcCalc>& aConcFailBc);
	int GetConcFailCa1AnchorNum(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdxChk, BOOL bGetIdx = FALSE);
	int GetPryoutCa1AnchorNum(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdx);
	BOOL GetBearingLink(int nBearingI, int nBearingJ, CArray<T_ELNK_K, T_ELNK_K>& aElnkK, CArray<T_NLNK_K, T_NLNK_K>& aNlnkK, BOOL bOrgPos=FALSE);
	BOOL GetBearingLinkNode(int nBearingI, int nBearingJ, T_NODE_K& NodeK1, T_NODE_K& NodeK2, BOOL bOrgPos = FALSE);

	void GetCopyingShape(CArray<double, double>& aX, CArray<double, double>& aY);	
	void GetBearingShape(int nIdxI, int nIdxJ, CArray<double, double>& aX, CArray<double, double>& aY);		
	BOOL GetConcFailCa(int nDir, double& dCa1, double& dCa1Pos1X, double& dCa1Pos1Y, double& dCa1Pos2X, double& dCa1Pos2Y, 
		int& nCa1BearingIIdx, int& nCa1BearingJIdx, int& nCa1AnchorIIdx, int& nCa1AnchorJIdx,
		double& dCa1P, double& dCa1PPos1X, double& dCa1PPos1Y, double& dCa1PPos2X, double& dCa1PPos2Y, 
		double& dCa2, double& dCa2Pos1X, double& dCa2Pos1Y, double& dCa2Pos2X, double& dCa2Pos2Y, 
		double& dCa2P, double& dCa2PPos1X, double& dCa2PPos1Y, double& dCa2PPos2X, double& dCa2PPos2Y, 
		std::vector<std::pair<int, int>>& aRepresentBearingIdx, BOOL bFixedCond=TRUE);

    void GetFirstLastBearingIdx(int nDir, int nType, int nRepresentGrp, int& nFirstIdx, int& nLastIdx, BOOL bFixedCond, int nFailType);
	BOOL CheckFilter(int nBearingI, int nBearingJ, std::vector<std::pair<int, int>>& aRepresentBearingIdx);	
	void GetConcFailCaIdx(int nDir, std::vector<TAnchorPosIdx> &aCa1Idx, std::vector<TAnchorPosIdx> &aCa1PIdx, std::vector<TAnchorPosIdx> &aCa2Idx, std::vector<TAnchorPosIdx> &aCa2PIdx, std::vector<std::pair<int, int>>& aBearingIdxFilter, BOOL bFixedCond=TRUE);
	void GetConcFailCaAnchorIdx(int nDir, int nType, int nBearingI, int nBearingJ, std::vector<int>& aIdxI, std::vector<int>& aIdxJ);
	void GetConcFailCaIdxAtBearing(int nDir, int nBearingIdxI, int nBearingIdxJ, std::vector<TAnchorPosIdx> &aCa1Idx, std::vector<TAnchorPosIdx> &aCa1PIdx, std::vector<TAnchorPosIdx> &aCa2Idx, std::vector<TAnchorPosIdx> &aCa2PIdx);

	BOOL GetPryoutCa(int nDir, double& dCa1, double& dCa1Pos1X, double& dCa1Pos1Y, double& dCa1Pos2X, double& dCa1Pos2Y,
		int& nCa1BearingIIdx, int& nCa1BearingJIdx, int& nCa1AnchorIIdx, int& nCa1AnchorJIdx,
		double& dCa1P, double& dCa1PPos1X, double& dCa1PPos1Y, double& dCa1PPos2X, double& dCa1PPos2Y, 
		double& dCa2, double& dCa2Pos1X, double& dCa2Pos1Y, double& dCa2Pos2X, double& dCa2Pos2Y, 
		double& dCa2P, double& dCa2PPos1X, double& dCa2PPos1Y, double& dCa2PPos2X, double& dCa2PPos2Y);	
		
	void GetPryoutCaIdx(int nDir, std::vector<TAnchorPosIdx> &aCa1Idx, std::vector<TAnchorPosIdx> &aCa1PIdx, std::vector<TAnchorPosIdx> &aCa2Idx, std::vector<TAnchorPosIdx> &aCa2PIdx, std::vector<std::pair<int, int>>& aBearingIdxFilter);
	void GetPryoutCaAnchorIdx(int nDir, int nType, int nBearingI, int nBearingJ, std::vector<int>& aIdxI, std::vector<int>& aIdxJ);
	void GetPryoutCaIdxAtBearing(int nDir, int nBearingIdxI, int nBearingIdxJ, std::vector<TAnchorPosIdx> &aCa1Idx, std::vector<TAnchorPosIdx> &aCa1PIdx, std::vector<TAnchorPosIdx> &aCa2Idx, std::vector<TAnchorPosIdx> &aCa2PIdx);	

	void GetConcFailBc(int nDir, std::vector<TConcFailBcCalc>& aConcFailBc, BOOL bFixedCond=TRUE, int nBi=0, int nBj=0);
	void GetConcFailBc(int nDir, TConcFailBcCalc& MinConcFailBc, BOOL bFixedCond=TRUE);
	void GetConcFailBcRecal(int nDir, std::vector<TConcFailBcCalc>& aConcFailBc, BOOL bFixedCond = TRUE, int nBi = 0, int nBj = 0);
	void GetConcFailBcRecal(int nDir, TConcFailBcCalc& MinConcFailBc, BOOL bFixedCond = TRUE);
	void GetMinConcFailBc(int nDir, std::vector<TConcFailBcCalc>& aConcFailBc, TConcFailBcCalc& MinConcFailBc);
	double CalcConcFailBearingRepresentRatio(int nDir, TConcFailBcCalc& EachGroup);
	double CalcPryoutBearingRepresentRatio(int nDir, int nBearingI, int nBearingJ);
	double CalcPryoutBearingRepresentRatio(int nDir, int nRepresentGrpIdx);
	void GetPrjTgtIdxConcFail(int nDir, CArray<int, int>& aBearingI, CArray<int, int>& aBearingJ, CArray<int, int>& aAnchorI, CArray<int, int>& aAnchorJ, BOOL bFixedCond=TRUE, int nBi = 0, int nBj=0);
	void GetPrjTgtIdxPryout(int nDir, CArray<int, int>& aBearingI, CArray<int, int>& aBearingJ, CArray<int, int>& aAnchorI, CArray<int, int>& aAnchorJ);
    void CalcAnchorProjPosConcFail(int nDir, int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, double &dX1, double &dY1, double &dX2, double &dY2);
	void CalcRecalAnchorProjPosConcFail(int nDir, int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, double dDistMin, double &dX1, double &dY1, double &dX2, double &dY2);
	void CalcRecalAnchorProjPosConcFailForDist(int nDir, int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, double& dDist);

	void GetPryoutBcNoUnion(int nDir, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeInX, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeInY, 
		CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeOutX, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeOutY, double dSetShorter);
	
	void GetPryoutBc(int nDir, std::vector<std::pair<int, int>> aRepBearingIdx4Ratio, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeX, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeY, std::set<int>& setBcIdx, int& nBc, double& dArea, double& dSax, double& dSay, BOOL& bSingleResist);
	void GetPryoutBc(int nDir, CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&>& aArProjShapeX, CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&>& aArProjShapeY, std::set<int>& setBcIdx, BOOL& bSingleResist);
	BOOL FindFirstInnerBearingIdx(T_COMPLEX_POLYGON2D& Poly, int& nBearingIdxI, int& nBearingIdxJ);
	BOOL IsInnerPolygon(double dX, double dY, const T_COMPLEX_POLYGON2D& Polygon);
	void PntMove(double dDirX, double dDirY, double dDist, double& dX, double& dY);
	void GetDir(double dX1 , double dY1, double dX2, double dY2, double& dDirX, double& dDirY);

	void CalcAnchorPos(int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, double &dX, double &dY);

	void GetBearingNumber(int& rnNumI, CArray<int, int>& raNumJ);
	static void GetBearingNumberNew2(const T_ANEV_D& AnevD, int& nNumI, int& nNumJ);

	void GetAnchorNumber(int nBearingI, int nBearingJ, int& nNumI, int& nNumJ);
	T_BRPR_D GetAnchorProp(int nBearingI, int nBearingJ);

	void SetCa1(int nDir, int nBearingIdxI, int nBearingIdxJ, double dLastCa1, double dCa1);
	void Sethef(int nDir, int nBearingIdxI, int nBearingIdxJ, double dhef);

	void GetConcFailSax(int nDir, double& dSax);
	void GetConcFailSaxNew(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdx, double& dSax);
	
	void GetPryoutSaxSay(int nDir, double& dSax, double& dSay);	
	void UpdateCa1Auto();
	void UpdateCa1Auto(int nDir, int nBearingI, int nBearingJ, std::vector<TConcFailBcCalc>& aConcFailBc);
	void UpdatehefAuto();
	void UpdatehefAuto(int nDir, int nBearingI, int nBearingJ);		

	void AllocHefVal();

	void GetConcFailSideLine(int nDir, std::vector<T_LINE2D> &aSideLine, int DW, int LR);
		
	double Gethef(int nDir, int nBearingI, int nBearingJ);	
	BOOL IsFixedBearing(int nIdxI, int nIdxJ, int nDir, BOOL bFixedCond=TRUE);
	BOOL IsSelectedBearing(int nIdxI, int nIdxJ, int nDir, int nFailType);
	BOOL GetReCalcCa1(int nDir, int nBearingIdxI, int nBearingIdxJ, double& dCa1);

	double m_dCopyingWidth;
	double m_dCopyingHeight;

	BOOL m_bAnchorDirPosi;  // TRUE : 정방향검토(기존), FALSE : 역방향 검토 

	static void InitMapData4CalcShape();
	void SetDULR(int nDu, int nLR);
	void SetPryoutDULR(int nDu, int nLR);
	std::vector<std::pair<int, int>> m_SideLineIdx;

protected:	
	void AllocCa1Var();
	BOOL SetLineSmallerAndCheckInclude(int nDir, double xi, double yi, double xj, double yj, double x, double y, double Tol);
	void SetLineSmaller(double& xi, double& yi, double& xj, double& yj);
	void SetPolygonSmaller(T_COMPLEX_POLYGON2D& rPoly);
	BOOL CheckUnionPolygon(const T_COMPLEX_POLYGON2D& rPoly1, const T_COMPLEX_POLYGON2D& rPoly2);
	void CalcMainCoord(double dStation, double dOffset, double dAngle, double& dX, double& dY);		
	BOOL GetDistAndPosByDir(int nBearingI, int nBearingJ, double dPos1X, double dPos1Y, double dPos2X, double dPos2Y, double dPntPosX, double dPntPosY, double dDirX, double dDirY, double& dDist, double& dRstPosX, double& dRstPosY);
	void CalcEachCa(std::vector<TAnchorPosIdx>& aCaIdx, double Dir[2], double& dCaPos1X, double& dCaPos1Y, double& dCaPos2X, double& dCaPos2Y, double& dCa, int& nCaBearingIIdx, int& nCaBearingJIdx, int& nCaAnchorIIdx, int& nCaAnchorJIdx);	
	BOOL GetCaByIdx(int nDir, std::vector<TAnchorPosIdx>& aCa1Idx, std::vector<TAnchorPosIdx>& aCa1PIdx, std::vector<TAnchorPosIdx>& aCa2Idx, std::vector<TAnchorPosIdx>& aCa2PIdx,
		double& dCa1, double& dCa1Pos1X, double& dCa1Pos1Y, double& dCa1Pos2X, double& dCa1Pos2Y, 
		int& nCa1BearingIIdx, int& nCa1BearingJIdx, int& nCa1AnchorIIdx, int& nCa1AnchorJIdx,
		double& dCa1P, double& dCa1PPos1X, double& dCa1PPos1Y, double& dCa1PPos2X, double& dCa1PPos2Y, 
		double& dCa2, double& dCa2Pos1X, double& dCa2Pos1Y, double& dCa2Pos2X, double& dCa2Pos2Y, 
		double& dCa2P, double& dCa2PPos1X, double& dCa2PPos1Y, double& dCa2PPos2X, double& dCa2PPos2Y);
	
	
	void Union2Polygon(const T_COMPLEX_POLYGON2D& rPoly1, const T_COMPLEX_POLYGON2D& rPoly2, CArray<T_COMPLEX_POLYGON2D, T_COMPLEX_POLYGON2D&>& raResultPoly, double tolerance);
	void CopyToCArray(T_POLYGON2D& Poly, CArray<double,double>& aX, CArray<double,double>& aY);
	void CopyToComplexPoly(CArray<double,double>& aX, CArray<double,double>& aY, T_POLYGON2D& Poly);
	void MakeCycle(T_COMPLEX_POLYGON2D& Poly);	
	
	void SetShorter(double& dX1 , double& dY1, double& dX2, double& dY2, double dVal);
	BOOL InnerBearing(T_CARRAY<double,double>& aX, T_CARRAY<double,double>& aY);
	BOOL InnerCopping(T_CARRAY<double,double>& aX, T_CARRAY<double,double>& aY);
	void DivByOutline(CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArX, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArY);
	
	void GetPryoutBasicSaxy(int nDir, int nBearingIdxI, int nBearingIdxJ, double& dSax, double& dSay);	
	BOOL NextBearingIdx(int nDir, int nNextI, int nNextJ, int& nBearingIdxI, int& nBearingIdxJ);
	
	void CalcAnchorProjPosPryoutEachBearing(int nDir, int nBearingI, int nBearingJ, BOOL bNoUnion, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeXEachBearing, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeYEachBearing);
	void CalcLocalAnchorProjPosPryout(int nDir, int nAnchorI, int nAnchorJ, CArray<double, double>& aProjShapeX, CArray<double, double>& aProjShapeY);
	void CalcEachAnchorProjPosPryout(int nDir, int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, CArray<double, double>& aProjShapeX, CArray<double, double>& aProjShapeY);
	BOOL GetLineInterSectPoint(double dPos1X, double dPos1Y, double dPos2X, double dPos2Y, double dPos3X, double dPos3Y, double dPos4X, double dPos4Y, double& dRstX, double& dRstY);
	void CalcBearingCenterPos(int nIdxI, int nIdxJ, double& dCenterX, double& dCenterY);
	
	void CalcLocalAnchorPos(int nBearingI, int nBearingJ, std::vector<double>& m_aLocalAnchorPosX, std::vector<double>& m_aLocalAnchorPosY);		

	BOOL IsPryoutRepresentGrp(int nDir, int nBearingI, int nBearingJ);
	

	double m_dCopyingTopBotY[2]; // 0:Top, 1:Bot
	std::vector<std::vector<double>> m_aArBearingHeight;
	std::vector<std::vector<double>> m_aArBearingWidth;
	
	std::vector<std::vector<T_BRPR_D>> m_aArBrprD; // Bearing
	T_ANEV_D m_AnevD; // Coping

	BOOL m_bSetBrpr;
	BOOL m_bSetAnev;

	std::vector<std::vector<BOOL>> m_aArSetCa1Longi;	
	std::vector<std::vector<BOOL>> m_aArSetCa1Trans;	
	std::vector<std::vector<double>> m_aArCa1Longi;	
	std::vector<std::vector<double>> m_aArCa1Trans;
	std::vector<std::vector<double>> m_aArLastCa1Longi;	
	std::vector<std::vector<double>> m_aArLastCa1Trans;

	std::vector<std::vector<BOOL>> m_aArSethefLongi;
	std::vector<std::vector<BOOL>> m_aArSethefTrans;
	std::vector<std::vector<double>> m_aArhefLongi;
	std::vector<std::vector<double>> m_aArhefTrans;
	double m_dBigZero;

	std::vector<TPryoutGroupInfo> m_aPryoutGroupInfo;
	int m_nPryoutRepresentGrpIdx;
	int m_nLastPryoutGroupDir;
	BOOL m_bRatioChk;
	BOOL m_bAnchorTenConc;

/*        [ 이전 ]                       [변경 후]

             (UP)                           (R) 
       -------------                     ----------- 
       |			|                   |           |
   (L) |			| (R)          (UP) |           |--> (Down)
       |	  |	    |                   |           | 
       -------|-----                    |           | 
              |                          -----------
       	      V                              (L)
       	    (Down)
*/
	int m_nDownUp;     //Down: 교축 정방향, UP : 교축 역방향
	int m_nLeftRight;  //L   : 교직 정방향, R  : 교직 역방향
	int m_nDownUpPryMgr;
	int m_nLeftRightPryMgr;
	int m_nLIdx;

private:
	CBrdgSeisEval* m_pSeisEval;
};
#include "HeaderPost.h"

#endif // !defined(__DgnSeisAnchorShapeMgr_H__)


