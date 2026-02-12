// AttrCtrl.h: interface for the CAttrCtrl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ATTRCTRL_DB_H__
#define __ATTRCTRL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DB_NODE.h"
#include "DB_CONS.h"
#include "DB_ST_DT_GMOD.h"
#include "PMInterCurve.h"
#include "DBGirderBaseDataDef.h"

#include "MultiDB.h"

#include "..\wg_base\AfxTemplEx.h"
#include "..\MIT_lib\StructKeyForStdMap.h"
#include <map>
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#include "HeaderPre.h"
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CDB_LCOM;

struct T_WINDLOAD
{
	T_STOR_K KeyStor;
	double dPosition_X;
	double dPosition_Y;
	double dWind_X;
	double dWind_Y;
	double dWind_RZ;
	void Initialize()
	{
		KeyStor=0;
		dPosition_X=0.;
		dPosition_Y=0.;
		dWind_X=0.;
		dWind_Y=0.;
		dWind_RZ=0.;
	}
};

struct T_SEISLOAD
{
	T_STOR_K KeyStor;
	double dPosition_X;
	double dPosition_Y;
	double dSeis_X;
	double dSeis_Y;
	double dSeis_RZ;
	void Initialize()
	{
		KeyStor=0;
		dPosition_X=0.;
		dPosition_Y=0.;
		dSeis_X=0.;
		dSeis_Y=0.;
		dSeis_RZ=0.;
	}
};

struct T_LOCALVECTOR
{
	T_ELEM_K KeyElem;
	double LocalVector[3][3];
};

struct T_STR_KEY
{
	CString csStr;
	UINT    nKey;
	static int CompByStr(const void* v1, const void* v2);
};

struct T_UINT_POS
{
	UINT uINT;
	int  nPos;
	static int CompByUINT(const void* v1, const void* v2);
	static int CompByPOS(const void* v1, const void* v2);
};

//////////////////////////////////////////////////////////////////////
// CLinkedKeyNode : 연결관계에 분기/병합이 포함된 경우에 활용되는 데이터 구조

class __MY_EXT_CLASS__ CLinkedKeyNode
{
	friend class CLinkedKeyMgr;

private:
	UINT m_key;

	CLinkedKeyNode(UINT key);
	virtual ~CLinkedKeyNode();

protected:
	CArray<CLinkedKeyNode*, CLinkedKeyNode*> m_aPrev;
	CArray<CLinkedKeyNode*, CLinkedKeyNode*> m_aNext;

	BOOL AddPrev(CLinkedKeyNode* pNode);
	BOOL AddNext(CLinkedKeyNode* pNode);
	int  GetNextKeyList(CArray<UINT,UINT>& raKeyNext) const;
	int  GetPrevKeyList(CArray<UINT,UINT>& raKeyPrev) const;

protected:
	UINT GetKey() const;
};


//////////////////////////////////////////////////////////////////////////
//Rail-Track Interaction Elnk  1.
struct KeyRlfcPos : public StructKeyForStdMap
{
	KeyRlfcPos() : StructKeyForStdMap()
	{
	}
	KeyRlfcPos(int Node1, int Node2, int nType ) : StructKeyForStdMap()
	{
		aInt.push_back(Node1);
		aInt.push_back(Node2);
		aInt.push_back(nType);
	}
};
struct ValForRlfcPos
{
	T_ELNK_D ElnkD;
	T_RLFC_D RlfcD;

	ValForRlfcPos()
	{
		Initialize();
	}
	ValForRlfcPos(ValForRlfcPos& src) { *this = src; }
	ValForRlfcPos& operator=(const ValForRlfcPos& src)
	{
		ElnkD = src.ElnkD;
		RlfcD = src.RlfcD;

		return *this;
	}
	void Initialize()
	{
		ElnkD.Initialize();
		RlfcD.Initialize();
	}
	void Set(T_ELNK_D& rElnkD, T_RLFC_D& rRlfcD)
	{
		ElnkD = rElnkD;
		RlfcD = rRlfcD;
	}
};

struct ValForElnDArray
{
	CArray<T_ELNK_D, T_ELNK_D&> arElnkD;

	ValForElnDArray()
	{
		Initialize();
	}
	ValForElnDArray(ValForElnDArray& src) { *this = src; }
	ValForElnDArray& operator=(const ValForElnDArray& src)
	{
		arElnkD.Copy(src.arElnkD);

		return *this;
	}
	void Initialize()
	{
		arElnkD.RemoveAll();
	}
	void Set(CArray<T_ELNK_D, T_ELNK_D&>& arrElnkD)
	{
		arElnkD.Copy(arrElnkD);
	}
};

//Rail-Track Interaction Elnk 2.
struct KeyRlfcNamePos : public StructKeyForStdMap
{
	KeyRlfcNamePos() : StructKeyForStdMap()
	{
	}
	KeyRlfcNamePos(T_RLFC_D& RlfcD) : StructKeyForStdMap()
	{
		aDbl.push_back(RlfcD.dLimDisp);
		aDbl.push_back(RlfcD.dUnLoadF);
		aDbl.push_back(RlfcD.dLoadF);
		aDbl.push_back(RlfcD.dLoadF_L);
		aDbl.push_back(RlfcD.dLoadF_R);
	}
};
struct ValRlfcNamePos
{
	CString strNewRlfcName;

	ValRlfcNamePos()
	{
		Initialize();
	}
	ValRlfcNamePos(ValRlfcNamePos& src) { *this = src; }
	ValRlfcNamePos& operator=(const ValRlfcNamePos& src)
	{
		strNewRlfcName = src.strNewRlfcName;

		return *this;
	}
	void Initialize()
	{
		strNewRlfcName = _T("");
	}
	void Set(CString rstrNewRlfcName)
	{
		strNewRlfcName = rstrNewRlfcName;
	}
};

//Rail-Track Interaction Elnk 3.
struct KeyReltPos : public StructKeyForStdMap
{
	KeyReltPos() : StructKeyForStdMap()
	{

	}
	KeyReltPos(int Node1, int Node2, int nStatus, int nGroup) : StructKeyForStdMap()
	{
		aInt.push_back(Node1);
		aInt.push_back(Node2);
		aInt.push_back(nStatus);
		aInt.push_back(nGroup);
	}

};
struct ValReltPos
{
	int Node1;
	int Node2;
	int nStatus;
	int nGroup;

	ValReltPos()
	{
		Initialize();
	}
	ValReltPos(ValReltPos& src) { *this = src; }
	ValReltPos& operator=(const ValReltPos& src)
	{
		Node1 = src.Node1;
		Node2 = src.Node2;
		nStatus = src.nStatus;
		nGroup = src.nGroup;

		return *this;
	}
	void Initialize()
	{
		Node1 = 0;
		Node2 = 0;
		nStatus = 0;
		nGroup = 0;
	}
	void Set(int rNode1, int rNode2, int rnStatus, int rnGroup)
	{
		Node1 = rNode1;
		Node2 = rNode2;
		nStatus = rnStatus;
		nGroup = rnGroup;
	}
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <cassert>

// struct T_NODE_D { double x, y, z; };                 // 전역 좌표 (z는 2D면 0으로 무시)
struct ElementD
{
	int id;                                          // 요소 번호 (외부 넘버링과 동일)
	std::vector<int> vNodes;                          // 노드 인덱스(1-based 권장). 크기 3 또는 4
	// 노드 순서는 CCW, Edge 규칙: 1:(1->2), 2:(2->3), 3:(3->4), 4:(4->1)
};

struct EdgeHit
{
	int    nElemId;
	int    dEdgeId;                                   // 0 이면 내부에서 시작/종료
	double dSrartOnEdge;                              // Edge 시작에서의 거리
	double dDistfromStart;                            // 전체 선분 시작노드에서의 누적거리
	double x, y;                                     // 교차 좌표
	double t;                                        // 전체 선분 파라미터 (0~1)
};

// struct ElementPass
// {
// 	int nElemId;  // 요소 번호 (Element ID)
// 	int nSpanId; // Design sprip 내 Span ID
// 
// 	// 요소 내 시작점 정보
// 	int nEdgeStart;     // 시작점이 위치한 Edge 번호 (1~4, 0이면 내부 시작)
// 	double dEdgeStart;     // Edge의 시작점으로부터의 거리 (Edge 방향 거리)
// 	double dDistStart;     // 선분 시작점(A)으로부터의 누적 거리
// 	double dx0, dy0, dz0;// 교차 좌표 (요소 내부 시작점)
// 	double dt0;         // 선분 파라미터(0~1) : A~B 구간에서의 위치 비율
// 	double dPositionI; // 전체 시작점에서의 거리
// 
// 	// 요소 내 끝점 정보
// 	int nEdgeEnd;       // 끝점이 위치한 Edge 번호 (1~4, 0이면 내부 시작)
// 	double dEdgeEnd;   	   // Edge의 시작점으로부터의 거리 (Edge 방향 거리)
// 	double dDistEnd;       // 시작점(A)으로부터의 누적 거리
// 	double dx1, dy1, dz1; // 교차 좌표 (요소 내부 시작점)
// 	double dt1;		   // 선분 파라미터(0~1) : A~B 구간에서의 위치 비율
// 	double dPositionJ; // 전체 시작점에서의 거리
// };

// ====== 기하 유틸 ======
static inline double dot2(double ax, double ay, double bx, double by) { return ax * bx + ay * by; }
static inline double cross2(double ax, double ay, double bx, double by) { return ax * ay - ay * bx; } // unused; left for reference
static inline double norm2(double x, double y) { return sqrt(x * x + y * y); }

static inline void to2D(const T_NODE_D& p, double& x, double& y) { x = p.x; y = p.y; } // z 무시(2D)

// p가 폴리곤 내부인지(짝수-홀수법). 단순 다각형 가정.
static bool pointInPolygon(double x, double y, const std::vector<std::pair<double, double>>& vecPoly)
{
	bool c = false;
	int n = (int)vecPoly.size();
	for (int i = 0, j = n - 1; i < n; j = i++) {
		double xi = vecPoly[i].first, yi = vecPoly[i].second;
		double xj = vecPoly[j].first, yj = vecPoly[j].second;
		bool intersect = ((yi > y) != (yj > y)) && (x < (xj - xi) * (y - yi) / (yj - yi + 1e-30) + xi);
		if (intersect) c = !c;
	}
	return c;
}

// 선분 AB 와 선분 CD의 교차 검사. 교차 시 true와 함께 t(AB상), u(CD상) 반환 (0~1)
// 정수배/평행 등 수치 안정 위해 EPS 사용. Collinear 겹침은 별도로 처리 필요.
static bool segIntersectParam(double Ax, double Ay, double Bx, double By,
	double Cx, double Cy, double Dx, double Dy,
	double& t, double& u, double EPS = 1e-12)
{
	double rX = Bx - Ax, rY = By - Ay;
	double sX = Dx - Cx, sY = Dy - Cy;
	double denom = rX * sY - rY * sX;
	if (std::fabs(denom) < EPS) return false; // 평행 또는 거의 평행(겹침은 따로)

	double qpx = Cx - Ax, qpy = Cy - Ay;
	t = (qpx * sY - qpy * sX) / denom;
	u = (qpx * rY - qpy * rX) / denom;
	return (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS);
}

// 선분 AB와 선분 CD가 거의 같은 직선(collinear)이며, 구간이 겹치는지 검사
static bool segCollinearOverlap(double Ax, double Ay, double Bx, double By,
	double Cx, double Cy, double Dx, double Dy,
	double EPS = 1e-12)
{
	// 두 벡터가 거의 평행이고, C가 AB 선상에 가까우면 collinear로 본다
	double rX = Bx - Ax, rY = By - Ay;
	double sX = Dx - Cx, sY = Dy - Cy;
	double denom = rX * sY - rY * sX;
	if (std::fabs(denom) >= EPS) return false; // not parallel

	auto distPointToLine = [&](double px, double py)
	{
		// 거리 = |(B-A)x(P-A)| / |B-A|
		double num = std::fabs((Bx - Ax) * (py - Ay) - (By - Ay) * (px - Ax));
		double den = norm2(rX, rY);
		return (den < EPS) ? num : (num / den);
	};

	if (distPointToLine(Cx, Cy) > 1e-9) return false; // 같은 직선 위가 아님(충분히 가깝지 않음)

	// 1D로 throw: AB, CD 를 각각 A 기준 투영한 t값 범위가 겹치는지 체크
	double rLen2 = rX * rX + rY * rY;
	auto projT = [&](double px, double py)
	{
		double vx = px - Ax, vy = py - Ay;
		if (rLen2 < EPS) return 0.0;
		return (vx * rX + vy * rY) / rLen2; // AB상 파라미터
	};
	double tC = projT(Cx, Cy), tD = projT(Dx, Dy);
	double tmin = min(tC, tD), tmax = max(tC, tD);
	return !(tmax < 0.0 || tmin > 1.0); // [0,1]와 겹치면 true
}

// edge (Pi -> Pj) 에 대해, 교차점이 edge의 "시작점에서의 거리 s" (edge 방향)에 해당
static double sAlongEdge(double ex0, double ey0, double ex1, double ey1, double px, double py)
{
	double vx = px - ex0, vy = py - ey0;
	return norm2(vx, vy); // 에지는 직선이므로 시작점부터의 유클리드 거리
}

// 3D
static inline double norm3(double x, double y, double z) {
	return sqrt(x * x + y * y + z * z);
}

enum class ProjPlane { XY, XZ, YZ };

//////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------
// CLinkedKeyMgr : 연결관계에 분기/병합이 포함된 경우에 활용되는 데이터 구조

class __MY_EXT_CLASS__ CLinkedKeyMgr
{
public:
	CLinkedKeyMgr();
	virtual ~CLinkedKeyMgr();
	void Clear();

private:
	CMapEx<UINT,UINT, CLinkedKeyNode*,CLinkedKeyNode*> m_map;

	CLinkedKeyNode* GetNode(UINT key) const;
	CLinkedKeyNode* GetOrAddNode(UINT key);

public:
	CLinkedKeyNode* AddNode(UINT key);
	BOOL AddNext(UINT key, UINT keyNext);
	BOOL AddPrev(UINT key, UINT keyPrev);
	BOOL RemoveKey(UINT key);

	BOOL ExistKey(UINT key) const;
	int  GetNextKeyList(UINT key, CArray<UINT,UINT>& raKeyNext) const;
	int  GetPrevKeyList(UINT key, CArray<UINT,UINT>& raKeyPrev) const;
	int  GetNextKeyList_all(T_STOR_K StorK, CArray<T_STOR_K,T_STOR_K>& raStorKAll) const;

	int  GetKeyListAll(CArray<UINT,UINT>& raKey) const;   // 등록 되어 있는 모든 key 리스트 반환
	void Copy(CLinkedKeyMgr& src);
};

//////////////////////////////////////////////////////////////////////////
///  Span for PT Slab 

// 쌍을 저장할 구조체 정의
struct NODE_PAIR
{
	UINT nStart;
	UINT nEnd;
};
struct POS_PAIR
{
	T_NODE_D StartPos;
	T_NODE_D EndPos;
};

struct T_SPAN_PT_BASE
{
	int  nSpanID;
	double dSpanLength;
	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	CArray<NODE_PAIR, NODE_PAIR&> arPairs;  // Span별 절점 쌍 배열
	CArray<POS_PAIR, POS_PAIR&> arPairPos;  // arPairs 에 대응되는 절점 좌표
	BOOL nSupportPT; // 0:No, 1:I, 2:J, 3:Both
	int nDirection;  // 0:X  1:Y
	double dOffset;

	void Initialize()
	{
		nSpanID = 0;
		dSpanLength = 0.0;
		aElemK.RemoveAll();
		arPairs.RemoveAll();
		arPairPos.RemoveAll();
		nSupportPT = 0;
		nDirection = 0;
		dOffset    = 0.0;
	}
	T_SPAN_PT_BASE() { Initialize(); }
	T_SPAN_PT_BASE(const T_SPAN_PT_BASE& src) { *this = src; }
	T_SPAN_PT_BASE& operator=(const T_SPAN_PT_BASE& src)
	{
		nSpanID     = src.nSpanID;
		dSpanLength = src.dSpanLength;
		aElemK.Copy(src.aElemK);
		arPairs.Copy(src.arPairs);
		arPairPos.Copy(src.arPairPos);
		nSupportPT = src.nSupportPT;
		nDirection = src.nDirection;
		dOffset    = src.dOffset  ;
		return *this;
	}
};

struct T_SPAN_PT
{
	BOOL bPTSlab;
	CArray<T_SPAN_PT_BASE, T_SPAN_PT_BASE&> aSpanPTBase;

	void Initialize()
	{
		bPTSlab = FALSE;
		aSpanPTBase.RemoveAll();
	}

	T_SPAN_PT() { Initialize(); }
	T_SPAN_PT(const T_SPAN_PT& src) { *this = src; }
	T_SPAN_PT& operator=(const T_SPAN_PT& src)
	{
		bPTSlab = src.bPTSlab;
		aSpanPTBase.Copy(src.aSpanPTBase);
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////
// CAttrCtrl

class CDBDoc;
class CUnitCtrl;
class CDataMemb;
class CInitCtrl;
class CStoryData;
struct T_SEIS_WIDTH;
class __MY_EXT_CLASS__ CAttrCtrl : public CObject
{

friend class CDBDoc;
friend class CAttrCtrl2;

public:
	CAttrCtrl(CDBDoc* pDoc);
	virtual ~CAttrCtrl();
// Preference가 변경되었을때 InitCtrl에서 호출
	void ChangePreference();
	//정적하중 Labeling이 필요한 조건인가?
	BOOL CheckLcomLabel(BOOL bAllLoadCase, T_LCOM_D& DataLcom);
	BOOL CheckLcomLabelThis(BOOL bAllLoadCase, T_LCOM_D& DataLcom);
	BOOL CheckLcomLabelGilc(BOOL bAllLoadCase, T_LCOM_D& DataLcom);
	//하중조건에 포함된 특정하중조건에 곱하여질 팩터, Envelope, Abs, Sqrt는 0.으로 Return, 새로운 하중조합방법인식
	BOOL GetLcomStldFactor(T_STLD_K KeyStld, T_LCOM_D& DataLcom, double& dStldFactor, double& dStagFactor, BOOL bTDPL=FALSE);  
	BOOL GetLcomThisFactor(T_THIS_K KeyThis, T_LCOM_D& DataLcom, double& dThisFactor);  
	BOOL GetLcomGilcFactor(T_GILC_K KeyGilc, T_LCOM_D& DataLcom, double& dGilcFactor);

protected:
	CDBDoc* m_pDoc;
	CUnitCtrl* m_pUnitCtrl;
	CDataMemb* m_pDataMemb; CDataMembPtrUpdator m_DataMembPtrUpdator;  // 반드시 쌍으로 선언
	CInitCtrl* m_pInitCtrl;

protected:
	void Initialize();

// ===== // DB_DATA

// project configuration
public:
	BOOL ExistPjcf();
	BOOL GetPjcf(T_PJCF_D& rData);

// china cooperate cdn setting
public:
	BOOL ExistMccs();
	BOOL GetMccs(T_MCCS_D& rData);

// structure type
public:
	BOOL ExistStyp();
	BOOL GetStyp(T_STYP_D& rData);
	void InitializeStyp(T_STYP_D& rData);
	double GetStypGrav();
	
// story changed Information
public:
	BOOL ExistStci();
	BOOL GetStci(T_STCI_D& rData);

// wall id changed Information
public:
	BOOL ExistWlci();
	BOOL GetWlci(T_WLCI_D& rData);

// story eccentricity 
public:
	BOOL ExistSecc();
	BOOL GetSecc(T_SECC_D& rData);

// analysis control
public:
	BOOL ExistActl();
	BOOL GetActl(T_ACTL_D& rData);  

// Analysis Control Option
public:
	BOOL ExistAcop();
	BOOL GetAcop(T_ACOP_D& rData);

	BOOL IsEnableAnalysis_LinearStatic();   // Linear Static Analysis
	BOOL IsEnableAnalysis_Buckling    ();   // Buckling Analysis
	BOOL IsEnableAnalysis_PDelta      ();   // P-Delta Analysis
	BOOL IsEnableAnalysis_ResSpec     ();   // Response Spectrum Analysis
	BOOL IsEnableAnalysis_Eigen       ();   // Eigen-value Analysis
	BOOL IsEnableAnalysis_MovingLoad  ();   // Moving Load Analysis
	BOOL IsEnableAnalysis_ConstStage  ();   // Construction Stage Analysis
	BOOL IsEnableAnalysis_Settle      ();   // Settlement Analysis
	BOOL IsEnableAnalysis_ComposSect  ();   // Composite Section Analysis
	BOOL IsEnableAnalysis_CraneLoad   ();   // Crane Load Analysis
	BOOL IsEnableAnalysis_TimeHist    ();   // Time History Analysis
	BOOL IsEnableAnalysis_Nonlinear   ();   // Non-Linear Analysis
	BOOL IsEnableAnalysis_Pushover    ();   // Pushover Analysis
	BOOL IsEnableAnalysis_HeatHydra   ();   // Heat of Hydration Analysis
	BOOL IsEnableAnalysis_BoundChange ();   // Boundary Change Assignment to Loadcase/Analysis
	BOOL IsEnableAnalysis_Suspension  ();   // Suspension Bridge Analysis (for CIVIL)

// nonlinear analysis control
public:
	void GetNonlList(CArray<CString, CString&>& rstrNonlList);  //serial 번호로 정렬된 리스트
	void GetNonlKeyList(CArray<T_NONL_K, T_NONL_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistNonl(CString& rstrName);
	BOOL ExistNonl(T_NONL_K Key);
	BOOL GetNonl(CString& rstrName, T_NONL_D& rData);
	BOOL GetNonl(T_NONL_K Key, T_NONL_D& rData);
	T_NONL_K GetNonlKey(CString& rstrName);
	int GetCountNonl();
	POSITION GetStartNonl();
	void GetNextNonl(POSITION& rNextPosition,T_NONL_K& rKey,T_NONL_D& rData);
	T_NONL_K GetStartNumNonl();
	T_NONL_K GetLastNumNonl();

// model degree of freedom
public:
	BOOL ExistMfrd();
	BOOL GetMfrd(T_MFRD_D& rData);

// Print Option
public:
	BOOL ExistPrnt();
	BOOL GetPrnt(T_PRNT_D& rData);

// analysis type
public:
void GetAtypList(CArray<CString, CString&>& rstrAtypList);  //serial 번호로 정렬된 리스트
	void GetAtypKeyList(CArray<T_ATYP_K, T_ATYP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistAtyp(CString& rstrName);
	BOOL ExistAtyp(T_ATYP_K Key);
	BOOL GetAtyp(CString& rstrName, T_ATYP_D& rData);
	BOOL GetAtyp(T_ATYP_K Key, T_ATYP_D& rData);
	T_ATYP_K GetAtypKey(CString& rstrName);
	int GetCountAtyp();
	POSITION GetStartAtyp();
	void GetNextAtyp(POSITION& rNextPosition,T_ATYP_K& rKey,T_ATYP_D& rData);
	T_ATYP_K GetStartNumAtyp();
	T_ATYP_K GetLastNumAtyp();

// data format
public:
	BOOL ExistMeth();
	BOOL GetMeth(T_METH_D& rData);

// nonlinear output option
public:
	BOOL ExistNlop();
	BOOL GetNlop(T_NLOP_D& rData);

// Eigenvalue Analysis Control Data for DYNA2E
public:
	BOOL ExistEvct();
	BOOL GetEvct(T_EVCT_D& rData);

// gdln
public:
	BOOL ExistGdln(CString& rGridName);
	BOOL ExistGdln(T_GDLN_K Key);

	BOOL GetGdln(T_GDLN_K Key, CString& rGridName, T_GDLN_DT& arXGrid, T_GDLN_DT& arYGrid, double& xAxisAngle, double& yAxisAngle, std::array<double, 3>& originPos);
	BOOL GetGdln(CString& rGridName, T_GDLN_DT& arXGrid, T_GDLN_DT& arYGrid, double& xAxisAngle, double& yAxisAngle, std::array<double, 3>& originPos);
	T_GDLN_K GetGdlnKey(CString& rGridName);
	int GetCountGdln();
	POSITION GetStartGdln();
	void GetNextGdln(POSITION& rNextPosition, CString& rGridName, T_GDLN_DT& arXGrid, T_GDLN_DT& arYGrid, double& xAxisAngle, double& yAxisAngle, std::array<double, 3>& originPos);
	T_GDLN_K GetStartNumGdln();

// node
public:
	BOOL GetNodeAttr(T_NODE_K NodeK, DBDataList*& pList);
	BOOL GetNodeFromBaseDB(T_NODE_K Key, T_NODE_D& rData);

	void GetNodeKeyList(std::vector<T_NODE_K>& rKeyList);
	void GetNodeKeyList(CArray<T_NODE_K, T_NODE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistNode(T_NODE_K Key);
	BOOL ExistNode(double x, double y, double z, T_NODE_K& ExistKey);
	BOOL GetNode(T_NODE_K Key,T_NODE_D& rData);
	BOOL GetNodeNeutral(T_NODE_K Key,T_NODE_D& rData);
	int GetCountNode();
	POSITION GetStartNode();
	void GetNextNode(POSITION& rNextPosition,T_NODE_K& rKey,T_NODE_D& rData);
	T_NODE_K GetStartNumNode();
	T_NODE_K GetLastNumNode();
	BOOL IsUnusedNode(T_NODE_K Key); // 어떤 데이터도 입력되지 않은 절점인가?
	BOOL IsUnconnectedNode(T_NODE_K Key); // 요소에 연결되지 않은 절점인가?
	void IsDofFixed(T_NODE_K KeyNode, BOOL bFixDof[CONS_SIZE]); // 강성없는 자유도인가?(요소,스프링,강제변위...)
	void IsDofConstraintForDesign(T_NODE_K KeyNode, BOOL bConstraint[6], BOOL bCheck_LT_ELEM_CMD=TRUE); 
	BOOL IsNodeOnWallLine(T_NODE_K KeyNode); 
	void GetSupportCondition     (T_NODE_K KeyNode, BOOL bConstraint[6], const T_KEY_LIST *lstExceptElems = nullptr); 
	void GetConnectedElem(T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& aElemList);
	void GetConnectedElem(T_NODE_K KeyNode, std::vector<T_ELEM_K>& aElemList);
	std::vector<T_NODE_K> GetNodeFromElementbyRelatedNode(const T_NODE_K nodeKey);
	int  FindCornerNode(const CArray<T_NODE_K, T_NODE_K>& craCandidateNodeK, CArray<T_NODE_K, T_NODE_K>& raCornerNodeK);
	int  Find3DCornerNode(CArray<T_ELEM_K, T_ELEM_K>& aElemK, CArray<T_NODE_K, T_NODE_K>& raNodeK, double& dPlaneTol);
	void GetConnectedElem4BeamTruss(T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& aElemList);
	BOOL SortNodeKeyListByDirection(CArray<T_NODE_K, T_NODE_K>& raNodeK, int nDir);
	T_NODE_D LinearInterpolNode(const T_NODE_D& A, const T_NODE_D& B, int nType, double Val);
	T_NODE_D MovePoint(const T_NODE_D& P, const T_NODE_D& vec, double L);

// element
public:
	BOOL GetElemAttr(T_ELEM_K ElemK, ElemList*& pList);
	BOOL GetElemFromBaseDB(T_ELEM_K Key, T_ELEM_D& rData);
	int GetElemAttr(T_ELEM_K KeyElem, int ltDataCmd, T_KEY_LIST &rKeyList);
	void GetElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rKeyList, BOOL bWithoutDummyElem=TRUE);  //Key 번호로 정렬된 리스트
	//요소에 연결된 절점번호를 정렬하여 넘겨줌
	void GetElemNodeKeyList(CArray<T_ELEM_K, T_ELEM_K>& rKeyElemList, CArray<T_NODE_K, T_NODE_K>& rKeyNodeList);
	void GetElemNodeKeyList(std::vector<T_ELEM_K>& rKeyElemList, std::vector<T_NODE_K>& rKeyNodeList);
	BOOL ExistElem(T_ELEM_K Key);
	BOOL ExistElemDummy(T_ELEM_K Key);
	BOOL GetElem(T_ELEM_K Key,T_ELEM_D& rData);
	int GetCountElem(BOOL bWithoutDummyElem=TRUE);
	POSITION GetStartElem();
	void GetNextElem(POSITION& rNextPosition,T_ELEM_K& rKey,T_ELEM_D& rData);
	T_ELEM_K GetStartNumElem();
	T_ELEM_K GetLastNumElem();
	void GetElemKeyListConnectedToNode(T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& rarKeyElem);   //절점에 연결된 요소번호
	void GetElemKeyListConnectedToNode(CArray<T_NODE_K, T_NODE_K>& rarKeyNode, CArray<T_ELEM_K, T_ELEM_K>& rarKeyElem); //절점에 연결된 요소번호
	int  GetElemKeyListConnectedToNodeWithBound(CMapEx<T_NODE_K,T_NODE_K, T_KEY_LIST_EX, T_KEY_LIST_EX&>& rmNode2ConnectedElemList, 
																							T_NODE_K NodeK, double dBoundSize, 
																							int nType, const T_ELEM_K_LIST& aElemK_candidate);
	void GetKeyListEltypeUser(CArray<int, int>* pKeyEltype,         //특정 요소종류를 사용중인 절점, 요소 번호
		CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	void GetKeyListEltypeAndSubtype(CArray<int, int>* pKeyEltype,   //특정 요소종류와 subtype을 사용중인 절점, 요소 번호
								CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL, CMap<int,int,BOOL,BOOL>* mSubType=NULL);
	void GetWallIdMap(CMap<int, int, int, int>& mapWallId);
	void GetWallIdList(CArray<int, int>& arWallId);  // Wall Id.로 정렬된 리스트
	void GetKeyListWallUser(CArray<int, int>* pWallId,         //특정 WallId를 사용중인 절점, 요소 번호
								CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);

	BOOL GetKeyListBy_WallID_ByStor(CArray<T_ELEM_K,T_ELEM_K>& ElemKey, const unsigned int BaseElemKey, T_WALL_KEY* prWallKey=NULL, CStoryData* pStoryData=NULL);
	void GetConnectedLinearElem(CArray<T_ELEM_K,T_ELEM_K>& ElemKey,unsigned int SearchElemKey, CArray<T_ELEM_K,T_ELEM_K>& SortedByWallIDList);
	void GetEqualCoordLinearElem(const CArray<T_ELEM_K,T_ELEM_K>& KeyList, CArray<T_ELEM_K,T_ELEM_K>& ElemKey);
	void GetRootWallElemKey(T_ELEM_K ElemK, T_ELEM_K& RootElemKey);
	void GetRootWallElemKey(CArray<T_ELEM_K,T_ELEM_K>& aElemK_WallID, T_ELEM_K& RootElemKey);
	BOOL FilteredNotRootElemKeyByList(CArray<T_ELEM_K,T_ELEM_K>& ElemKeyList,CArray<T_ELEM_K,T_ELEM_K>& NotRootElemList);
	BOOL MakeRootWallElemKeyList(CArray<T_ELEM_K,T_ELEM_K>& aElemK);
	void GetEqualWallIdElemList(CArray<T_ELEM_K,T_ELEM_K>& ElemKeyList, const T_ELEM_K BaseElemKey, CStoryData* pStoryData=NULL);
	BOOL GetWallIDFirstLastElem(CArray<T_ELEM_K, T_ELEM_K>& ElemKeyList, unsigned int& FirstElemKey, unsigned int& LastElemKey, BOOL bUseRound=TRUE);
	// bLocalSectionOffset : Section Offset일경우 TRUE 이면 Local 방향의 Offset을 넘겨줌
	BOOL GetElemOffs(T_ELEM_K KeyElem, T_OFFS_D& rData, BOOL bCalcTaperedGroup=FALSE, BOOL bLocalSectionOffset=FALSE, BOOL bBeforeComposite=FALSE, BOOL bNotConsiderBetaAngle=FALSE); 
	BOOL GetSectOffsetFromCentroid(T_SECT_D& sData, double& hi, double& bi, double& hj, double& bj, BOOL bBeforeComposite=FALSE, BOOL bStifness=TRUE);
	BOOL GetHiddenSectOffsetFromCentroid(T_ELEM_K ElemK, BOOL& rbTapered, double& hi, double& bi, double& hj, double& bj);
	BOOL IsElemHorizontalBeam(T_ELEM_K ElemK);
	BOOL IsElemHorizontalBeam(T_ELEM_D ElemD);
	BOOL GetWallSize(unsigned int nWallID, T_STOR_K StorK, double& dThick, double& dWidth);
	BOOL GetWallSize(T_ELEM_K ElemK_root, double& dThick, double& dWidth);
	double GetWallWidth(CArray<T_ELEM_K,T_ELEM_K>& aElemK);
	double GetThinnestThicknessFromElemKList(CArray<T_ELEM_K,T_ELEM_K>& aElemK);
	int GetWallInternalColumnKeyList(const IN CArray<T_ELEM_K,T_ELEM_K>& aElemK_WallID, OUT CArray<T_ELEM_K,T_ELEM_K>& aElemK_Column);
	BOOL GetColmorWallKeyConnectToInfillStrut(T_NODE_K NodeK[2], T_ELEM_K ConElemK[2], BOOL bIsColm[2]);
	BOOL GetColmorWallKeyConnectToInfillStrut_Sub(T_NODE_K NodeK, double dMin, double dMax, double dDirection[3], T_ELEM_K& ConElemK, BOOL& bIsColm);
	BOOL GetColmWallKeyListConnectedToNode(T_NODE_K NodeK, double dMin, double dMax, double dDirection[3], T_ELEM_K_LIST& aConElemK, T_ELEM_K_LIST& aWallElemK);

	int ChangeEltyp(int eltyp);             //요소종류번호 변경(bn1 용)
	BOOL IsValidEltyp(int eltyp);           //요소종류번호 검증
	int GetMaxNumNode(int eltyp);           //요소종류별 최대허용 절점수
	int GetNumNode(int eltyp,int* elnod);   //요소종류별 사용절점수
	CString GetEltypName(int eltyp);        //요소종류이름

	CString GetEltypNameEng(int eltyp);     //요소종류이름: 영어로만 
	BOOL IsBeamTypeElem(T_ELEM_K key);
	BOOL IsColmTypeElem(T_ELEM_K key);
	BOOL IsBraceTypeElem(T_ELEM_K key);
	BOOL IsWallTypeElem(T_ELEM_K key);
	int GetCountDof(T_ELEM_K key);       //절점당 자유도 갯수
	int GetCountDof(int eltyp);       //절점당 자유도 갯수
	BOOL IsOnlyDispDof(int eltyp);          //변위자유도만 있는 요소종류인가?
	BOOL IsXZPlaneDof(int eltyp);           //X-Z면 거동만 하는 요소인가?
	BOOL IsFrameType(int eltyp);            //선 종류인가?
	BOOL IsPlaneType(int eltyp);            //판 종류인가?
	BOOL IsSolidType(int eltyp);            //솔리드 종류인가?
	BOOL IsTruss(int eltyp);                //truss 인가?
	int GetTrussID();                       //truss 요소종류번호 얻기
	BOOL IsCable(int eltyp, int nSubType);  //Cable 인가?
	BOOL IsBeam(int eltyp);                 //beam 인가?
	int GetBeamID();                        //beam 요소종류번호 얻기
	BOOL IsPlstrs(int eltyp);               //plane stress 인가?
	int GetPlstrsID();                      //plane stress 요소종류번호 얻기
	BOOL IsPlate(int eltyp);                //plate 인가?
	int GetPlateID() const;                       //plate 요소종류번호 얻기
	BOOL IsWall(int eltyp);                 //wall 인가?
	int GetWallID();                        //wall 요소종류번호 얻기
	BOOL IsPlstrn(int eltyp);               //plane strain 인가?
	int GetPlstrnID();                      //plane strain 요소종류번호 얻기
	BOOL IsAxisym(int eltyp);               //axisymmetric 인가?
	int GetAxisymID();                      //axisymmetric 요소종류번호 얻기
	BOOL IsSolid(int eltyp);                //solid 인가?
	int GetSolidID();                       //solid 요소종류번호 얻기
	BOOL IsPanel(int eltyp);                //panel 인가?
	int GetPanelID();                       //panel 요소종류번호 얻기
	BOOL IsBDSSlab(int eltyp);              //bds slab 인가?
	int GetBDSSlabID();                     //bds slab 요소종류번호 얻기
	BOOL IsSDSSlab(int eltyp);              //sds slab 인가?
	int GetSDSSlabID();                     //sds slab 요소종류번호 얻기
	BOOL IsTensTruss(int eltyp);            //tension only truss 인가?
	int GetTensTrussID();                   //tension only truss 요소종류번호 얻기
	BOOL IsCompTruss(int eltyp);            //compression only truss 인가?
	int GetCompTrussID();                   //compression only truss 요소종류번호 얻기
	BOOL IsDummyElem(int eltyp);            // Dummy Element인가?
	BOOL IsNeedKnode(int eltyp);            //knode가 필요한 요소종류인가?
	BOOL IsNeedMatl(int eltyp);             //material이 필요한 요소종류인가?
	BOOL IsNeedProp(int eltyp);             //property가 필요한 요소종류인가?
	BOOL IsNeedSect(int eltyp);             //section이 필요한 요소종류인가?
	BOOL IsNeedThik(int eltyp);             //thickness가 필요한 요소종류인가?
	BOOL IsVerticalFrame(const T_NODE_D& Node_I, const T_NODE_D& Node_J);               //수직부재인가 1/100 오차허용?
	BOOL IsVerticalFrame(const double Coor_i[3], const double Coor_j[3]);
	BOOL IsHorizontalFrame(const T_NODE_D& Node_I, const T_NODE_D& Node_J);             //수평부재인가 1/100 오차허용?
	BOOL IsHorizontalFrame(const double Coor_i[3], const double Coor_j[3]);
// 1=column(수직-1/100 오차허용), 2=beam(수평-1/100 오차허용), 3=brace 0=Frame 요소가 아닐때
	int GetMemberType(T_ELEM_K KeyElem, BOOL bTrussAsBrace = TRUE);
	int GetMemberType_JUD(T_ELEM_K KeyElem, BOOL bPassMbtp=FALSE);	// JUD방식의 MemberType을 넘겨줍니다. (bPassMbtp:T_MBTP_D입력정보를 무시할지 여부)
	int GetEltypeCount(int eltyp);          //요소종류별 요소갯수
	int GetElemTypeAndName(CArray<int, int>& raType, CArray<CString, CString&>& raElemName);
	BOOL GetSolidFaceList(T_ELEM_D& DataElem, CArray<T_ELEM_D, T_ELEM_D&>& aFaceList); // solid요소의 각 Face를 Plate형태로 만들어줌
	BOOL IsTendonAssigned(T_ELEM_K KeyElem);
	BOOL IsWallBeam(T_ELEM_K keyElem);    // 벽 개구부로 인해 분할 생성된 개구부 상/하단 Beam인가?
	BOOL IsSpecialShearWall();
	BOOL IsWallEndHorBarCondition();  // 벽체의 단부 수평철근이 필요한 조건인가?
	BOOL IsWallEndVerBarCondition();
	int GetWallID_LastNumPlusOne();
	BOOL IsConnectedElem(CArray<T_ELEM_K, T_ELEM_K>& aElemKey);  //  2D Elem에 대하여 연결되어 있는지 조사해 줌
	BOOL CheckWallIDThicknessIsFixed(T_ELEM_K ElemK_OneOfWallID);
	BOOL CheckWallIDLevel(T_ELEM_K ElemK_OneOfWallID);
	BOOL CalcTsgrTotLength(T_TSGR_D& TsgrD, T_ELEM_K TargetElemK, double& dTsgrTotLength, double& dPositionI);
	BOOL GetElemLevel(IN CArray<T_ELEM_K, T_ELEM_K>& aElemKey, OUT double& dTopLevel, double& dBotLevel);
	BOOL CalcElemBetaAngle(T_ELEM_D& ElemD, BOOL bReletive, double dAngle);
	BOOL CalcElemBetaAngle(T_ELEM_D& ElemD, double dRefX, double dRefY, double dRefZ, BOOL bRefPnt);
	BOOL CalcElemBetaAngle(T_ELEM_D& ElemD, int nAxis, int nCoorDir, double dPointX, double dPointY, double dPointZ);
		
	bool IsConBeam(T_ELEM_K ElemK);
	bool IsConColm(T_ELEM_K ElemK);
	bool IsConWall(T_ELEM_K ElemK);
	
private:
	int GetAppliedMemberTypeByEachElem(T_ELEM_K ElemK);
	bool IsConcreteMemberType(T_ELEM_K ElemK, int nMbtpType);

protected:
	void GetKeyListNodeElem(CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>& KeyElemMap,
							CArray<T_NODE_K, T_NODE_K>* pKeyNode, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem);

// tapersed section group
public:
	void GetTsgrList(CArray<CString, CString&>& rstrTsgrList);  //serial 번호로 정렬된 리스트
	void GetTsgrKeyList(CArray<T_TSGR_K, T_TSGR_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistTsgr(CString& rGroupName);
	BOOL ExistTsgr(T_TSGR_K Key);
	BOOL GetTsgr(CString& rGroupName, T_TSGR_D& rData);
	BOOL GetTsgr(T_TSGR_K Key, T_TSGR_D& rData);
	T_TSGR_K GetTsgrKey(CString& rGroupName);
	int GetCountTsgr();
	POSITION GetStartTsgr();
	void GetNextTsgr(POSITION& rNextPosition,T_TSGR_K& rKey,T_TSGR_D& rData);
	T_TSGR_K GetStartNumTsgr();
	T_TSGR_K GetLastNumTsgr();
	T_TSGR_K GetElemTsgrKey(T_ELEM_K KeyElem);  // 그룹정의가 되어 있지 않으면 return 0;

// change element dependent material property
public:
	void GetEdmpKeyList(CArray<T_EDMP_K, T_EDMP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistEdmp(T_EDMP_K Key);
	BOOL GetEdmp(T_EDMP_K Key,T_EDMP_D& rData);
	int GetCountEdmp();
	POSITION GetStartEdmp();
	void GetNextEdmp(POSITION& rNextPosition,T_EDMP_K& rKey,T_EDMP_D& rData);

	// Element Stiffness Scale Factor
public:
	BOOL MakeKeyEssf(T_ELEM_K KeyElem, T_BNGR_K KeyBngr, T_ESSF_K& rKey);
	void GetEssfKeyList(CArray<T_ESSF_K, T_ESSF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetEssfElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistEssf(T_ESSF_K Key);
	BOOL ExistEssfElem(T_ELEM_K KeyElem);
	BOOL GetEssf(T_ESSF_K Key, T_ESSF_D& rData);
	void GetEssfElem(T_ELEM_K KeyElem, CArray<T_ESSF_K, T_ESSF_K>& rKeyList, CArray<T_ESSF_D, T_ESSF_D&>& rDataList);
	BOOL GetEssfElem(T_ELEM_K KeyElem, T_ESSF_D& rData);
	BOOL GetEssfBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_ESSF_D& rData);
	int GetCountEssf();
	POSITION GetStartEssf();
	void GetNextEssf(POSITION& rNextPosition, T_ESSF_K& rKey, T_ESSF_D& rData);
protected:
	BOOL GetSerialEssf(T_ELEM_K KeyElem, T_BNGR_K KeyBngr, T_ESSF_KEY& rKeySerial);

// wall shear stiffness reduction factor
public:
	BOOL MakeKeyWssf(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_WSSF_K& rKey);
	void GetWssfKeyList(CArray<T_WSSF_K, T_WSSF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetWssfElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistWssf(T_WSSF_K Key);
	BOOL ExistWssfElem(T_ELEM_K KeyElem);
	BOOL GetWssf(T_WSSF_K Key,T_WSSF_D& rData);
	void GetWssfElem(T_ELEM_K KeyElem, CArray<T_WSSF_K, T_WSSF_K>& rKeyList, CArray<T_WSSF_D, T_WSSF_D&>& rDataList);
	BOOL GetWssfElem(T_ELEM_K KeyElem, T_WSSF_D& rData);
	BOOL GetWssfBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_WSSF_D& rData);
	int GetCountWssf();
	POSITION GetStartWssf();
	void GetNextWssf(POSITION& rNextPosition,T_WSSF_K& rKey,T_WSSF_D& rData);
protected:
	BOOL GetSerialWssf(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_WSSF_KEY& rKeySerial);

// plate shear stiffness reduction factor
public:
	BOOL MakeKeyPssf(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_PSSF_K& rKey);
	void GetPssfKeyList(CArray<T_PSSF_K, T_PSSF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetPssfElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistPssf(T_PSSF_K Key);
	BOOL ExistPssfElem(T_ELEM_K KeyElem);
	BOOL GetPssf(T_PSSF_K Key,T_PSSF_D& rData);
	void GetPssfElem(T_ELEM_K KeyElem, CArray<T_PSSF_K, T_PSSF_K>& rKeyList, CArray<T_PSSF_D, T_PSSF_D&>& rDataList);
	BOOL GetPssfElem(T_ELEM_K KeyElem, T_PSSF_D& rData);
	BOOL GetPssfBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_PSSF_D& rData);
	int GetCountPssf();
	POSITION GetStartPssf();
	void GetNextPssf(POSITION& rNextPosition,T_PSSF_K& rKey,T_PSSF_D& rData);
	double GetLocalAxisAngle4Dir(T_ELEM_K KeyElem, int nLocalAxis, int nDirType, double OriginPoint[3]);
	double GetLocalAxisAngle4Vec(T_ELEM_K KeyElem, int nLocalAxis, double RefVector[3]);
	double GetLocalAxisAngle4Vec(T_ELEM_D& DataElem, int nLocalAxis, double RefVector[3]);
	BOOL IsSkipAnalData(T_PSSF_D& PssfD);
protected:
	BOOL GetSerialPssf(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_PSSF_KEY& rKeySerial);

// Material
public:
	BOOL IsUsedMatl(T_MATL_K Key);
	void GetMatlList(CArray<CString, CString&>& rstrMatlList);  //Key 번호로 정렬된 리스트
	void GetMatlKeyList(CArray<T_MATL_K, T_MATL_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMatl(T_MATL_K Key);
	BOOL ExistMatd(T_MATL_K Key);
	BOOL GetMatl(T_MATL_K Key,T_MATL_D& rData);
	T_MATL_ANALYSIS GetMatlAnalysis(T_MATL_D& rData);
	int GetCountMatl();
	POSITION GetStartMatl();
	void GetNextMatl(POSITION& rNextPosition,T_MATL_K& rKey,T_MATL_D& rData);
	T_MATL_K GetStartNumMatl();
	T_MATL_K GetLastNumMatl();
	T_MATL_K GetNextNumMatl(T_MATL_K StartNum);
	T_MATL_K GetStartNumMatlUnused();
	T_MATL_K GetLastNumMatlUnused();
	T_MATL_K GetNextNumMatlUnused(T_MATL_K StartNum);
	void GetKeyListMatlUser(CArray<T_MATL_K, T_MATL_K>* pKeyMatl,           //특정 재질을 사용중인 절점, 요소 번호
							CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	void GetMatlKeyListUsedInElem(CArray<T_MATL_K, T_MATL_K>& rKeyList);
	void GetMatlKeyListUsedInTdnt(CArray<T_MATL_K, T_MATL_K>& rKeyList);
// for design
	void GetMatlListDesign(CArray<CString, CString&>& rstrMatlList);  //Key 번호로 정렬된 리스트
	BOOL GetMatlDesign(T_MATL_K Key,T_MATD_D& rData);
	T_MATL_K GetMatlKey(CString& rMatlName);
	
//   // [2010-05-17] Kim, Geun Young (Tel: 2042, gykim@midasit.com) - PMS:4092
	BOOL GetDgnGenMatd(T_ELEM_K ElemK, T_MATD_D& rMatdD);        
	BOOL GetDgnStlMatd(T_MATD_D& rMatdD);
	BOOL GetDgnConMatd(T_MATD_D& rMatdD);
	BOOL GetDgnSrcMatd(T_MATD_D& rMatdD);
	BOOL IsStaticLCComb(T_LCOM_D& LcomD); // Static LC로만 구성된 하중조합인가?  
	BOOL IsLCombWithWind(T_LCOM_D& LcomD);
	BOOL IsLCombWithEQ(T_LCOM_D& LcomD);
	BOOL ExistCctl();
	BOOL GetCctl(T_CCTL_D& rData);
	
	POSITION GetStartMatlDesign();
	void GetNextMatlDesign(POSITION& rNextPosition,T_MATL_K& rKey,T_MATD_D& rData);

// section
public:
	BOOL IsUsedSect(T_SECT_K Key);
	BOOL IsColdFormedSect(T_SECT_K Key);
	BOOL IsColdFormedSect(T_SECT_D& SectD);
	BOOL IsColdFormedSect(CString& Shape);
	BOOL IsColdFormedSect4Code(CString& CodeName, T_SECT_K Key, BOOL bUseCFSect = FALSE);
	BOOL IsColdFormedSect4Code(CString& CodeName, CString& Shape, BOOL bUseCFSect = FALSE);

	void GetSectList(CArray<CString, CString&>& rstrSectList);  //Key 번호로 정렬된 리스트
	void GetSectKeyList(CArray<T_SECT_K, T_SECT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSect(T_SECT_K Key);
	BOOL ExistSecd(T_SECT_K Key);
	BOOL GetSect(T_SECT_K Key,T_SECT_D& rData, BOOL bConvertUnitOut = TRUE, BOOL bOrgConvert=FALSE);
	BOOL GetSectOrSecv(T_SECT_K Key, T_SECT_D& rData, BOOL bVirtual=FALSE, BOOL bConvertUnitOut=TRUE, BOOL bOrgConvert=FALSE);
	BOOL ExistSect(CString& rSectName);
	T_SECT_K GetSectKey(CString& rSectName);
	BOOL GetSect(CString& rSectName, T_SECT_D& rData);  
	int GetCountSect();
	POSITION GetStartSect();
	void GetNextSect(POSITION& rNextPosition,T_SECT_K& rKey,T_SECT_D& rData);
	T_SECT_K GetStartNumSect();
	T_SECT_K GetLastNumSect();
	T_SECT_K GetNextNumSect(T_SECT_K StartNum);
	T_SECT_K GetStartNumSectUnused();
	T_SECT_K GetLastNumSectUnused();
	T_SECT_K GetNextNumSectUnused(T_SECT_K StartNum);
	BOOL GetSectStiffness(T_SECT_K Key, BOOL bGetAfterConstruction, BOOL& bSRC, BOOL& bTapered, int& nLyType, int& nLzType, 
						 T_SECT_STIFFNESS& rStiffnessI, T_SECT_STIFFNESS& rStiffnessJ);
	BOOL GetSectStiffness(T_SECT_K Key, T_SECT_D& Data, T_SECF_D& DataSecf, BOOL bGetAfterConstruction, BOOL& bSRC, BOOL& bTapered, int& nLyType, int& nLzType, 
						 T_SECT_STIFFNESS& rStiffnessI, T_SECT_STIFFNESS& rStiffnessJ);
	//BOOL GetSectStiffness(T_SECT_D& Data, T_SSFS_D& DataSsfs, BOOL bGetAfterConstruction, BOOL& bSRC, BOOL& bTapered, int& nLyType, int& nLzType, 
	//           T_SECT_STIFFNESS& rStiffnessI, T_SECT_STIFFNESS& rStiffnessJ);
	void GetKeyListSectUser(CArray<T_SECT_K, T_SECT_K>* pKeySect,           //특정 단면을 사용중인 절점, 요소 번호
							CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	void GetKeyListSecvUser(CArray<T_SECV_K, T_SECV_K>* pKeySect,           //특정 단면을 사용중인 절점, 요소 번호
							CArray<T_NODE_K, T_NODE_K>* pKeyNode = NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem = NULL);
	BOOL GetSectDimen(T_SECT_K Key, double& hi, double& bi, double& hj, double& bj, int* pnShape = NULL, BOOL bSRCSteelOnly=TRUE, BOOL bBeforeComposite=FALSE, BOOL bStifness=TRUE);  // nShape == 1(Rect) ==2(Round)
	BOOL GetSectDimen(T_SECT_D& Data, double& hi, double& bi, double& hj, double& bj, int* pnShape = NULL, BOOL bSRCSteelOnly = TRUE, BOOL bBeforeComposite = FALSE, BOOL bStifness = TRUE);  // nShape == 1(Rect) ==2(Round)
	BOOL GetSectCentroid(T_SECT_K Key, double& hi, double& bi, double& hj, double& bj, BOOL bBeforeComposite=FALSE);
	BOOL GetSectCentroid(T_SECT_D& Data, double& hi, double& bi, double& hj, double& bj, BOOL bBeforeComposite=FALSE);
	void GetSectKeyListUsedInElem(CArray<T_SECT_K, T_SECT_K>& rKeyList);
	int  GetRealSectType(const T_SECT_K SectK);
// for design
	void GetSectListDesign(CArray<CString, CString&>& rstrSectList);  //Key 번호로 정렬된 리스트
	BOOL GetSectDesign(T_SECT_K Key,T_SECT_D& rData);
	POSITION GetStartSectDesign();
	void GetNextSectDesign(POSITION& rNextPosition,T_SECT_K& rKey,T_SECT_D& rData);
	int GetOffsetPosition(T_SECT_D& sData);
	int GetOffsetCenterType(T_SECT_D& sData);
	void GetSectionSize(T_SECT_D& sData, CString& rShape, double SectionSize[2][D_SECT_SIZE_NUM_MAX], BOOL bCompositUser4CS=FALSE);
// 특정 단면과 재질이 할당된 요소 리스트 가져오기
	int  GetElemKeyListMatlSectUser(CArray<T_MATL_K, T_MATL_K>* pKeyMatl,
																	CArray<T_SECT_K, T_SECT_K>* pKeySect,
																	CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	int  GetElemKeyListMatlSectUser(T_MATL_K MatlK, T_SECT_K SectK,
																	CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	BOOL GetSectDim4Tendon(T_SECT_K Key, double& hi, double& biU, double& biL, double& hj, double& bjU, double& bjL, double& dn);
	BOOL GetSectDim4Tendon(T_SECT_D& Data, double& hi, double& biU, double& biL, double& hj, double& bjU, double& bjL, double& dn);
	BOOL GetSectKey4InfillStrut(T_SECT_K& SectK, double dB, double dH);

	// MNET:XXXX-이종협-20081229
	int  GetElemKeyListMatlSectUser4AMR(CArray<T_MATL_K, T_MATL_K>* pKeyMatl, T_SECT_K SectK);
	// report
	BOOL GetReportSectStiffInfo(const T_SECT_K SectK, T_RSCT_STIFF_D &StiffInfo, T_RSCT_STIFF_D *pAfterStiffInfo=NULL);
	BOOL GetReportSectStiffInfo(const T_SECT_D SectD, T_RSCT_STIFF_D &StiffInfo, T_RSCT_STIFF_D *pAfterStiffInfo=NULL, BOOL bTapered=FALSE);
	BOOL GetReportRpscInfo(const T_SECT_K SectK, BOOL &bConsiderJ, T_RSCT_RBAR_D &RpscInfoI, T_RSCT_RBAR_D &RpscInfoJ);
	BOOL GetReportRpscInfo(const T_RPSC_D RpscD, BOOL &bConsiderJ, T_RSCT_RBAR_D &RpscInfoI, T_RSCT_RBAR_D &RpscInfoJ);
	int  GetReportCscsInfo(const T_SECT_K SectK, const T_STAG_K StagK, CArray<T_RSCT_CSCT_D, T_RSCT_CSCT_D&> &aCscsInfo);
	int  GetReportCscsInfo(const T_SECT_K SectK, const T_STAG_K StagK, const T_CSCS_D CscsD, CArray<T_RSCT_CSCT_D, T_RSCT_CSCT_D&> &aCscsInfo);
	CString GetCscsPartName(const int nCompType, const int nIndex);
	BOOL ConvertSODStiffnerRefPos(T_SECT_SOD_STIFF_SUB_D& StiffSubD, double dDeckLength);
	BOOL GetSrcSingletubePolygonData(T_SECT_D& SectD, int nIJ);
	BOOL GetSrcDumbbellPolygonData(T_SECT_D& SectD, int nIJ, int nDumbType = 0, BOOL bCompInProg = FALSE);
	void ReversePolygonData(T_GSEC_POLYGON& InputPolygon);
	void MakeEllipse(double xmin, double ymin, double xmax, double ymax, T_GSEC_POLYGON* pPolygon, int nDetailLevel = 1);
	void MakeEllipseMod(double xmin, double ymin, double xmax, double ymax, std::vector<double>& radianValues, bool clockwise, T_GSEC_POLYGON* pPolygon, int nDetailLevel = 1,
						double dRad1 = 0.0, double dRad2 = 0.0, T_GSEC_POLYGON* pPolygonTarget = NULL);
	void MakeEllipseSegment(double xmin, double ymin, double xmax, double ymax, double startRad, double endRad, bool clockwise, T_GSEC_POLYGON* pPolygon, int nDetailLevel = 1);

protected:
	void GetSectionSize_Sub(T_SECT_SECTION_D& Data, int nStype, CString& rShape, double SectionSize[2][D_SECT_SIZE_NUM_MAX]);
	BOOL GetSectStiffness_Sub(T_SECT_K SectK, T_SECT_SECTION_D& Data, int nStype, BOOL& bSRC, BOOL& bTapered, int& nLyType, int& nLzType, 
						 T_SECT_STIFFNESS& rStiffnessI, T_SECT_STIFFNESS& rStiffnessJ, BOOL bAfter=FALSE);
	BOOL GetSectDimen_Sub(T_SECT_SECTION_D& Data, int nStype, double& hi, double& bi, double& hj, double& bj, int* pnShape, BOOL bSRCSteelOnly=TRUE, BOOL bHSQType=FALSE);
	BOOL GetSectDimen_Sub_PSC(LPCTSTR lpszShape, int nCellType, int nCellShape, const T_SECT_SECTBASE_D& crSectBase, double& hi, double& bi);
	BOOL GetSectDimen_Sub_PSC_VALUE(const T_SECT_SECTBASE_D& crSectBase, double& h, double& b, BOOL bStifness=TRUE);
	BOOL GetSectCentroid_Sub(T_SECT_SECTION_D& Data, int nStype, double& hi, double& bi, double& hj, double& bj);
	BOOL GetSectDim_Sub4Tendon(T_SECT_SECTION_D& Data, int nStype, double& hi, double& biU, double& biL, double& hj, double& bjU, double& bjL, double& dn);
	BOOL GetSectDim_Sub_PSC4Tendon(LPCTSTR lpszShape, int nCellType, int nCellShape, T_SECT_SECTBASE_D& rSectBase, double& h, double& biU, double& biL, double& dn);

// section scale factor
public:
	BOOL MakeKeySecf(T_SECT_K KeySect,T_BNGR_K KeyBngr,T_SECF_K& rKey);
	void GetSecfKeyList(CArray<T_SECF_K, T_SECF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetSecfKeyListBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, CArray<T_SECF_K, T_SECF_K>& rKeyList);
	void GetSecfSectKeyList(CArray<T_SECT_K, T_SECT_K>& rSectKeyList); // Key 번호로 정렬된 리스트    
	BOOL ExistSecf(T_SECF_K Key);
	BOOL ExistSecfSect(T_SECT_K KeySect);
	BOOL GetSecf(T_SECF_K Key,T_SECF_D& rData);
	void GetSecfSect(T_SECT_K KeySect, CArray<T_SECF_K, T_SECF_K>& rKeyList, CArray<T_SECF_D, T_SECF_D&>& rDataList);
	BOOL GetSecfSect(T_SECT_K KeySect, T_SECF_D& rData);
	BOOL GetSecfBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_SECT_K KeySect, T_SECF_D& rData);
	int GetCountSecf();
	POSITION GetStartSecf();
	void GetNextSecf(POSITION& rNextPosition,T_SECF_K& rKey,T_SECF_D& rData);
protected:
	BOOL GetSerialSecf(T_SECT_K KeySect,T_BNGR_K KeyBngr,T_SECF_KEY& rKeySerial);

// M-Phi Inelastic Hinge
public:
	void GetMphgKeyList(CArray<T_MPHG_K, T_MPHG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMphg(T_MPHG_K Key);
	BOOL GetMphg(T_MPHG_K Key,T_MPHG_D& rData);
	int GetCountMphg();
	POSITION GetStartMphg();
	void GetNextMphg(POSITION& rNextPosition,T_MPHG_K& rKey,T_MPHG_D& rData);
	void CalcMphi();

// M-Phi Section
public:
	void GetMpstKeyList(CArray<T_MPST_K, T_MPST_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMpst(T_MPST_K Key);
	BOOL GetMpst(T_MPST_K Key,T_MPST_D& rData);
	int GetCountMpst();
	POSITION GetStartMpst();
	void GetNextMpst(POSITION& rNextPosition,T_MPST_K& rKey,T_MPST_D& rData);
	
// composite section for construction stage
public:
	void GetCscsKeyList(CArray<T_CSCS_K, T_CSCS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	int  GetCscsList(T_SECT_K SectK, CArray<T_CSCS_K, T_CSCS_K>& rKeyList, CArray<T_CSCS_D, T_CSCS_D&>& rDataList);
	BOOL ExistCscs(T_CSCS_K Key);
	BOOL GetCscs(T_CSCS_K Key,T_CSCS_D& rData);
	T_CSCS_K GetCscsKey(T_CSCS_D& rData);
	int GetCountCscs();
	POSITION GetStartCscs();
	void GetNextCscs(POSITION& rNextPosition,T_CSCS_K& rKey,T_CSCS_D& rData);
	T_CSCS_K GetStartNumCscs();

	void Calc_H_OfCscsBefore730(T_CSCS_D& Data); // MNET:2840 20070711 mylee
	BOOL Get_H_FromCscs(int nSize, T_CSCS_D CscsData, CArray<double, double>& aH);
	BOOL IsJapanCode4Cscs(T_CSCS_BASE &Part, const T_CSCS_D& CscsD);

	// cscs util function
	BOOL IsSectCscsType(T_SECT_K SectK);
	int GetElemSectCompositeInfo(T_ELEM_K ElemK, T_STAG_K StagK, T_CSCS_D& rCscsD);
	BOOL ConverCscsDataFromDependentMateriaData();
	int GetSecvCompositeInfo(T_VBEM_K VbemK, T_STAG_K StagK, T_CSCS_D& rCscsD);// GetElemSectCompositeInfo의 Virtual Beam용
	int GetElemSectCompositeInfoDgn(ElemPairK ElemK, T_STAG_K StagK, T_CSCS_D& rCscsD);

	// Consider Virtual
	BOOL GetCscsOrCscv(T_CSCS_K Key, T_CSCS_D& rData, BOOL bVirtual=FALSE);
	int GetCountCscsOrCscv(BOOL bVirtual=FALSE);
	void GetCscsOrCscvKeyList(CArray<T_CSCS_K, T_CSCS_K>& rKeyList, BOOL bVirtual=FALSE);  //Key 번호로 정렬된 리스트

// composite section for construction stage - Virtual
public:
	void GetCscvKeyList(CArray<T_CSCV_K, T_CSCV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	int  GetCscvList(T_SECV_K SecvK, CArray<T_CSCV_K, T_CSCV_K>& rKeyList, CArray<T_CSCV_D, T_CSCV_D&>& rDataList);
	BOOL ExistCscv(T_CSCV_K Key);
	BOOL GetCscv(T_CSCV_K Key,T_CSCV_D& rData);
	T_CSCV_K GetCscvKey(T_CSCV_D& rData);
	int GetCountCscv();
	POSITION GetStartCscv();
	void GetNextCscv(POSITION& rNextPosition,T_CSCV_K& rKey,T_CSCV_D& rData);
	T_CSCV_K GetStartNumCscv();

// reinforcement info of section
public:
	void GetRpscKeyList(CArray<T_RPSC_K, T_RPSC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRpsc(T_RPSC_K Key, BOOL bVirtual=FALSE);
	BOOL GetRpsc(T_RPSC_K Key,T_RPSC_D& rData);
	BOOL GetRpscByConsiderRbar(BOOL bMbar,T_RPSC_K Key,T_RPSC_D& rData);
	int GetCountRpsc();
	POSITION GetStartRpsc();
	void GetNextRpsc(POSITION& rNextPosition,T_RPSC_K& rKey,T_RPSC_D& rData);

	// design
	int  GetRpsdKeyList(CArray<T_RPSC_K, T_RPSC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRpsd(T_RPSC_K Key);
	BOOL GetRpsd(T_RPSC_K Key,T_RPSC_D& rData);
	BOOL GetRpsdByConsiderRbar(BOOL bMbar,T_RPSC_K Key,T_RPSC_D& rData);
	int  GetCountRpsd();
	POSITION GetStartRpsd();
	void GetNextRpsd(POSITION& rNextPosition,T_RPSC_K& rKey,T_RPSC_D& rData);

// section stiffness using effective width
public:
	void GetStfeKeyList(CArray<T_STFE_K, T_STFE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStfe(T_STFE_K Key);
	BOOL GetStfe(T_STFE_K Key,T_STFE_D& rData);
	int GetCountStfe();
	POSITION GetStartStfe();
	void GetNextStfe(POSITION& rNextPosition,T_STFE_K& rKey,T_STFE_D& rData);

// Design Calculation Condition
public:
	void GetStdcKeyList(CArray<T_STDC_K, T_STDC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStdc(T_STDC_K Key);
	BOOL GetStdc(T_STDC_K Key,T_STDC_D& rData);
	int GetCountStdc();
	POSITION GetStartStdc();
	void GetNextStdc(POSITION& rNextPosition,T_STDC_K& rKey,T_STDC_D& rData);

// thickness
public:
	BOOL IsUsedThik(T_THIK_K Key);
	void GetThikList(CArray<CString, CString&>& rstrThikList);  //Key 번호로 정렬된 리스트
	void GetThikKeyList(CArray<T_THIK_K, T_THIK_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistThik(T_THIK_K Key);
	BOOL GetThik(T_THIK_K Key,T_THIK_D& rData);
	int GetCountThik();
	POSITION GetStartThik() const;
	void GetNextThik(POSITION& rNextPosition,T_THIK_K& rKey,T_THIK_D& rData) const;
	T_THIK_K GetStartNumThik();
	T_THIK_K GetLastNumThik();
	T_THIK_K GetNextNumThik(T_THIK_K StartNum);
	T_THIK_K GetStartNumThikUnused();
	T_THIK_K GetLastNumThikUnused();
	T_THIK_K GetNextNumThikUnused(T_THIK_K StartNum);
	void GetKeyListThikUser(CArray<T_THIK_K, T_THIK_K>* pKeyThik,           //특정 두께를 사용중인 절점, 요소 번호
							CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	void GetThikKeyListUsedInElem(CArray<T_THIK_K, T_THIK_K>& rKeyList);
	BOOL GetThikForDataConversion(T_THIK_D& DataThik, double& dThickness);
	BOOL GetThikForDataConversion(T_THIK_K KeyThik, double& dThickness);
	BOOL GetThikForTemperatureGradient(T_THIK_D& DataThik, double& dThickness);
	BOOL GetThikForTemperatureGradient(T_THIK_K KeyThik, double& dThickness);
	BOOL GetThikForWeightAndMass(T_THIK_D& DataThik, double& dThickness);
	BOOL GetThikForWeightAndMass(T_THIK_K KeyThik, double& dThickness);
	BOOL GetThikForThikName(T_THIK_D& DataThik, double& dThickness);
	BOOL GetThikForThikNameTreeMenu(T_THIK_D& DataThik, double& dThickness, double& dThicknessOut);
	BOOL GetThikForThikName(T_THIK_K KeyThik, double& dThickness);
	BOOL GetThikForWallAssemble(T_THIK_D& DataThik, double& dThickness);
	BOOL GetThikForWallAssemble(T_THIK_K KeyThik, double& dThickness);
	BOOL GetThikForHidden(T_THIK_D& DataThik, double& dThickness, BOOL bUse4Offset = FALSE);
	BOOL GetThikForHidden(T_THIK_K KeyThik, double& dThickness);
	BOOL GetThikOffset(T_THIK_D DataThik, double &dOffset);
	BOOL GetThikOffset(T_THIK_K KeyThik, double &dOffset, BOOL bUse4Offset = FALSE);
	BOOL GetThikForPlaneStress(T_THIK_D& DataThik, double& dThickness);
	BOOL GetThikForPlaneStress(T_THIK_K KeyThik, double& dThickness);
	BOOL GetThikForLocalForceSum(T_THIK_D& DataThik, double& dThickness);
	BOOL GetThikForLocalForceSum(T_THIK_K KeyThik, double& dThickness);
	BOOL GetThikForDesign(T_THIK_D& DataThik, double& dThickness);
	BOOL GetThikForDesign(T_THIK_K KeyThik, double& dThickness);
	T_THIK_K FindThickness(double dThickness) const;
	CString GetThikNameAuto(T_THIK_K ThikK);
	CString GetThikNameAutoTreeMenu(T_THIK_K ThikK);
// Building Control
public:
	BOOL ExistBldc();
	BOOL GetBldc(T_BLDC_D& rData);
	double GetBaseLevel(BOOL bUseBedrock = TRUE);
	double GetBottomofFootingLevel();
	BOOL IsUseBedrockLevel();

// panel zone effect
public:
	BOOL ExistPzef();
	BOOL GetPzef(T_PZEF_D& rData);

// story
public:
	void GetStorData(CArray<T_STOR_D, T_STOR_D&>& rarStorList); //Story Level로 정렬된 리스트
	void GetStorList(CArray<CString, CString&>& rstrStorList);  //Story Level로 정렬된 리스트
	void GetStorKeyListByCS(CArray<T_STOR_K, T_STOR_K>& rKeyList); // 시공단계 정보 고려된 유효층 리스트
	void GetStorKeyList(CArray<T_STOR_K, T_STOR_K>& rKeyList, BOOL bConsiderCS=FALSE);  //Story Level로 정렬된 리스트
	void GetStorKeyListByModlSort(CArray<T_STOR_K, T_STOR_K>& rKeyList);  //Story Level로 정렬된 리스트 (Module별로 구분)
	int  GetStorKeyListByConnect(CArray<T_STOR_K, T_STOR_K>& rKeyList, T_STOR_K* pStorK_start, T_STOR_K* pStorK_end, CLinkedKeyMgr* pLinkedKeyMgr=NULL, BOOL bSameModl=FALSE);  // 상/하층 연결관계에 의한 Story key list 반환
	BOOL ExistStor(CString& rStoryName);
	BOOL ExistStor(T_STOR_K Key);
	BOOL GetStor(CString& rStoryName, T_STOR_D& rData);
	BOOL GetStor(T_STOR_K Key,T_STOR_D& rData, BOOL bStorNameWithModule=TRUE);
	int GetCountStor();
	void GetAccEccenticity(UINT StorK, double dEccentricity, double& dXDir, double& dYDir);
	POSITION GetStartStor();
	void GetNextStor(POSITION& rNextPosition,T_STOR_K& rKey,T_STOR_D& rData, BOOL bStorNameWithModule=FALSE);
	T_STOR_K GetStorKey(CString& rStoryName);
	T_STOR_K GetStorKey(CString& rStoryName, CString& rModuleName);
	T_STOR_K GetStartNumStor();
	T_STOR_K GetLastNumStor();
	T_STOR_K GetNextNumStor(T_STOR_K StartNum);
	BOOL GetStorKeyList_nextLevel(T_STOR_K StorK_cur, CArray<T_STOR_K,T_STOR_K>& raStorK_next, BOOL bIsUpOrDown);// 다음 Level story list (Multi-tower 고려)
	BOOL GetStorKeyByGL(CArray<T_STOR_K, T_STOR_K> &aStorK, CArray<T_STOR_K, T_STOR_K> &aUgStorK, BOOL bConsiderCS=FALSE); // aStorK:지상층, aUgStorK:지하층
	T_STOR_K GetBaseStor4GL();
	CString GetStorNameWithoutModuleName(LPCTSTR lpszName);
	CString GetStorNameWithModuleName(T_STOR_K StorK);
	CString GetModuleNameOfStor(const T_STOR_K StorK);
	int     GetModuleIndexOfStor(const T_STOR_K StorK, CString& rstrModuleName);
	void GetStorKeyByElemKey(T_STOR_K& StorK,const T_ELEM_K ElemK, CStoryData* pStoryData=NULL);
	// nSelectionType == 0   // 해당층과 층하부로 연결된 절점, 요소
	//                == 1   // 해당층과 층상부로 연결된 절점, 요소
	//                == 2   // 해당층과 층상부와 층하부로 연결된 모든 절점, 요소
	//                == 3   // 해당층과 층하부의 모든 절점, 요소
	//                == 4   // 해당층과 층상부의 모든 절점, 요소
	//                == 5   // 해당층만
	//void GetKeyListStorUser(CArray<T_STOR_K, T_STOR_K>* pKeyStor, int nSelectionType,
	//                        CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	void GetKeyListStorUser(T_STOR_K KeyStor, int nSelectionType,
													CArray<T_NODE_K, T_NODE_K>* pKeyNode, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem, CLinkedKeyMgr* pLinkedKeyMgr_stor=NULL);
	void GetKeyListStorUserNlnk(T_STOR_K KeyStor, int nSelectionType, CArray<T_NLNK_K, T_NLNK_K>* pKeyNlnk); // added by mylee
	//void GetKeyListStorUser(CArray<T_STOR_K, T_STOR_K>* pKeyStor,           //특정 층을 사용중인 절점, 요소 번호, 층의 하부 
	//            CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	//void GetKeyListStorUser(T_STOR_K KeyStor,                               //특정 층을 사용중인 절점, 요소 번호, 층의 하부 
	//            CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	//void GetKeyListBelowStor(T_STOR_K KeyStor,                              //특정 층 까지의 절점, 요소 번호
	//            CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);

	void GetNodeKeyListStorUser(const T_STOR_K KeyStor, CArray<T_NODE_K, T_NODE_K>* pKeyNode, BOOL bExcludeMidNode=TRUE, CLinkedKeyMgr* pLinkedKeyMgr_stor=NULL, CMapEx<T_NODE_K, T_NODE_K, T_STOR_K, T_STOR_K>* pmNodeK2StorK=NULL); // 층의 상.하부 반씩
	void GetMidNodeKeyListStorUser(T_STOR_K StorUserK, CArray<T_NODE_K, T_NODE_K>* pKeyNode, CString strPosition, CLinkedKeyMgr* pLinkedKeyMgr_stor=NULL); // 층의 중간에 속한 경우 아래 위층으로 반씩 분리  
	void GetNodeKeyListStorDiaphragm(T_STOR_K KeyStor, CArray<T_NODE_K, T_NODE_K>& arNodeKey, BOOL bIngnoreStorDiaphragm=FALSE, BOOL bSolver=FALSE);
	void CalcDiaphragmCenter(CArray<double, double>& arCenterX, CArray<double, double>& arCenterY, BOOL bUseBedrockLevel, BOOL bView=FALSE);
	void GetAllNodeforAllStor(CMap<T_NODE_K, T_NODE_K, T_STOR_K, T_STOR_K>& mapNodeStor);
	double GetBldgHeight();
	BOOL   GetStorHeight(const CArray<T_STOR_K,T_STOR_K>& aStorK, CMap<T_STOR_K,T_STOR_K, double,double>& rmStor2Height, BOOL bToUpOrDown, CMap<T_STOR_K,T_STOR_K, T_STOR_K,T_STOR_K>* pmStorCur2Ref_forHeight=NULL, BOOL bUseStorNodeKMapMgr=FALSE);
	double GetStorLevel(T_STOR_K StorK);
	int    GetStorLevel(const CArray<T_STOR_K,T_STOR_K>& aStorK, CMap<T_STOR_K,T_STOR_K, double,double>& rmStor2Level);
	BOOL GetNode2StorMap(const CLinkedKeyMgr& linkedKeyMgr_stor, CMap<T_NODE_K, T_NODE_K, T_STOR_K, T_STOR_K>& rKeyNodeMap, BOOL bCheckLevel, BOOL bExcludeMidNode=TRUE);
	BOOL GetMidNode2StorMap(const CLinkedKeyMgr& linkedKeyMgr_stor, CMap<T_NODE_K, T_NODE_K, T_STOR_K, T_STOR_K>& rKeyNodeMap, BOOL bCheckLevel);
	BOOL GetNodePosAboutStor(const T_NODE_D& NodeD, T_STOR_K& rStorK_lower, T_STOR_K& rStorK_upper, 
														const CLinkedKeyMgr& linkedKeyMgr_stor, 
														const CMap<T_STOR_K,T_STOR_K, double,double>& mStor2Level,
														T_STOR_K StorK_base,
														double dTol, BOOL bCheckLevel);
	BOOL GetStorArea(const T_STOR_D& StorD, int nVertex, double (*dPolyLine)[2]);
	BOOL GetStorLinkedData(CLinkedKeyMgr& rLinkedKeyMgr_stor, CArray<T_STOR_K,T_STOR_K>* pKeyStor=NULL, BOOL bUseStorNodeKMapMgr=FALSE);
	BOOL GetStorLinkedData_FilterUnlinkedStor(CLinkedKeyMgr& rLinkedKeyMgr_stor, CArray<T_STOR_K,T_STOR_K>* pKeyStor=NULL);
	BOOL SortStorKeyListByLevel(CArray<T_STOR_K,T_STOR_K>& raStorK);
	BOOL IsStorHaveArea(const T_STOR_D& StorD) const;
	BOOL IsStorOverlapArea(const T_STOR_D& StorD_src, int nVertex_tgt, double (*dPolyLine_tgt)[2]);
	BOOL IsStorOverlapArea(T_STOR_K StorK_src, const T_STOR_D& StorD_tgt);
	BOOL IsInsideOfStorArea2D(double dx, double dy, const T_STOR_D& StorD, int nVertex=0, double (*dPolyLine_storArea)[2]=NULL);
	BOOL IsInsideOfStorArea2D_sub(double dx, double dy, const T_STOR_D& StorD, int nVertex, double (*dPolyLine_storArea)[2]);
	BOOL IsInsideOfStorArea2D(const T_NODE_D& NodeD, const T_STOR_D& StorD);

// Module(old)
public:
	void GetMdulList(CArray<CString, CString&>& rstrGirderList);  //순번으로 정렬된 리스트
	void GetMdulKeyList(CArray<T_MDUL_K, T_MDUL_K>& rKeyList);  //순번으로 정렬된 리스트
	BOOL ExistMdul(CString& rMdulName);
	BOOL ExistMdul(T_MDUL_K Key);
	BOOL GetMdul(CString& rMdulName, T_MDUL_D& rData);
	BOOL GetMdul(T_MDUL_K Key,T_MDUL_D& rData);
	int GetCountMdul();
	POSITION GetStartMdul();
	void GetNextMdul(POSITION& rNextPosition,T_MDUL_K& rKey,T_MDUL_D& rData);
	T_MDUL_K GetMdulKey(CString& rMdulName);
	T_MDUL_K GetStartNumMdul();
	T_MDUL_K GetLastNumMdul();

// module(new)
public:
	BOOL ExistModl();
	BOOL GetModl(T_MODL_D& rData);
	BOOL GetModlWithBase(T_MODL_D& rData);    // modl 정보가 없어도 default module 정보를 만들어서 반환
	BOOL GetModlBySpace(T_MODL_D& rData);     // start ~ end story까지의 module 공간을 고려해서 module의 story list를 반환
	T_STOR_K GetEndStorKOfModule(const T_MODL_D& rData, T_STOR_K StorK_last);
	CString GetModlBaseName();    // base module name

// diaphragm mass
public:
	BOOL MakeKeyDmas(T_STOR_K Key,T_DMAS_K& rKey);
	BOOL ExistDmas(T_DMAS_K Key);
	BOOL GetDmas(T_DMAS_K Key,T_DMAS_D& rData);
    void GetDmasKeyList(CArray<T_DMAS_K, T_DMAS_K>& rKeyList); // 정렬된 리스트
	void GetDmas(T_STOR_K KeyStor, CArray<T_DMAS_K, T_DMAS_K>& rarKey, CArray<T_DMAS_D, T_DMAS_D&>& rarData);
	int GetCountDmas();
	POSITION GetStartDmas();
	void GetNextDmas(POSITION& rNextPosition,T_DMAS_K& rKey,T_DMAS_D& rData);
	BOOL CalcDmas(T_DMAS_D& rData);
protected:
	BOOL GetSerialDmas(T_STOR_K Key,T_DMAS_KEY& rKeySerial);

//diaphragm release
public:
	void GetDrlsKeyList(CArray<T_DRLS_K, T_DRLS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDrls(T_DRLS_K Key);
	BOOL GetDrls(T_DRLS_K Key,T_DRLS_D& rData);
	int GetCountDrls();
	POSITION GetStartDrls();
	void GetNextDrls(POSITION& rNextPosition,T_DRLS_K& rKey,T_DRLS_D& rData);

//story diaphragm group for construction stage
public:
	BOOL GetStdgFromBaseDB(T_STDG_K Key, T_STDG_D& rData);
	void GetStdgKeyList(CArray<T_STDG_K, T_STDG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStdg(T_STDG_K Key);
	BOOL GetStdg(T_STDG_K Key,T_STDG_D& rData);
	int  GetStdgList(CArray<T_STDG_K, T_STDG_K>& raKey, CArray<T_STDG_D, T_STDG_D&>& raData);
	BOOL GetStdgBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, CArray<T_STDG_K, T_STDG_K>& raKey, CArray<T_STDG_D, T_STDG_D&>& raData);
	int GetCountStdg();
	POSITION GetStartStdg();
	void GetNextStdg(POSITION& rNextPosition,T_STDG_K& rKey,T_STDG_D& rData);

	//Ignore Elements for Load Cases
public:
	BOOL MakeKeyIelc(T_ELEM_K KeyElem,T_STLD_K KeyStld,T_IELC_K& rKey);
	void GetIelcKeyList(CArray<T_IELC_K, T_IELC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistIelc(T_IELC_K Key);
	BOOL GetIelc(T_IELC_K Key,T_IELC_D& rData);
	int GetCountIelc();
	POSITION GetStartIelc();
	void GetNextIelc(POSITION& rNextPosition,T_IELC_K& rKey,T_IELC_D& rData);
	void GetIelcElem(T_ELEM_K ElemK,  CArray<T_IELC_K, T_IELC_K>& rKeyList, CArray<T_IELC_D, T_IELC_D&>& rDataList);
	void GetIelcStld(T_STLD_K StldK,  CArray<T_IELC_K, T_IELC_K>& rKeyList, CArray<T_IELC_D, T_IELC_D&>& rDataList);
	int  GetIelcStldKList(CArray<T_STLD_K, T_STLD_K>& aStldK);

	// Ignore Elements for NL. Analysis Initial Load 
public:
	void GetIepiKeyList(CArray<T_IEPI_K, T_IEPI_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistIepi(T_IEPI_K Key);
	BOOL GetIepi(T_IEPI_K Key,T_IEPI_D& rData);
	int GetCountIepi();
	POSITION GetStartIepi();
	void GetNextIepi(POSITION& rNextPosition,T_IEPI_K& rKey,T_IEPI_D& rData);

// named plane
public:
	void GetNplnList(CArray<CString, CString&>& rstrPlaneList); //Plane Name으로 정렬된 리스트
	void GetNplnKeyList(CArray<T_NPLN_K, T_NPLN_K>& rKeyList);  //Plane Name으로 정렬된 리스트
	BOOL ExistNpln(CString& rPlaneName);
	BOOL ExistNpln(T_NPLN_K Key);
	BOOL GetNpln(CString& rPlaneName, T_NPLN_D& rData);
	BOOL GetNpln(T_NPLN_K Key,T_NPLN_D& rData);
	int GetCountNpln();
	POSITION GetStartNpln();
	void GetNextNpln(POSITION& rNextPosition,T_NPLN_K& rKey,T_NPLN_D& rData);
	T_NPLN_K GetNplnKey(CString& rPlaneName);
	T_NPLN_K GetStartNumNpln();
	T_NPLN_K GetLastNumNpln();
	T_NPLN_K GetNextNumNpln(T_NPLN_K StartNum);
	void GetKeyListNplnUser(CArray<T_NPLN_K, T_NPLN_K>* pKeyNpln,           //특정 평면이름을 사용중인 절점, 요소 번호
							CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);

// Dimension
public:
	void GetDimmList(CArray<CString, CString&>& rstrNameList); //Dimension 으로 정렬된 리스트
	void GetDimmKeyList(CArray<T_DIMM_K, T_DIMM_K>& rKeyList);  //Dimension 으로 정렬된 리스트
	BOOL ExistDimm(CString& strName);
	BOOL ExistDimm(T_DIMM_K Key);
	BOOL GetDimm(CString& rstrName, T_DIMM_D& rData);
	BOOL GetDimm(T_DIMM_K Key,T_DIMM_D& rData);
	int GetCountDimm();
	POSITION GetStartDimm();
	void GetNextDimm(POSITION& rNextPosition,T_DIMM_K& rKey,T_DIMM_D& rData);
	T_DIMM_K GetDimmKey(CString& rstrName);
	T_DIMM_K GetStartNumDimm();
	T_DIMM_K GetLastNumDimm();
	T_DIMM_K GetNextNumDimm(T_DIMM_K StartNum);  

// Dimension Line & Text
public:
	void GetDmltKeyList(CArray<T_DMLT_K, T_DMLT_K>& rKeyList); 
	BOOL ExistDmlt(T_DMLT_K Key);
	BOOL GetDmlt(T_DMLT_K Key,T_DMLT_D& rData);
	int GetCountDmlt();
	POSITION GetStartDmlt();
	void GetNextDmlt(POSITION& rNextPosition,T_DMLT_K& rKey,T_DMLT_D& rData);
	T_DMLT_K GetStartNumDmlt();
	T_DMLT_K GetLastNumDmlt();
	T_DMLT_K GetNextNumDmlt(T_DMLT_K StartNum);  

// Material Coordinates System
public:
	void GetMtcsList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetMtcsKeyList(CArray<T_MTCS_K, T_MTCS_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistMtcs(CString& rName);
	BOOL ExistMtcs(T_MTCS_K Key);
	BOOL GetMtcs(CString& rName, T_MTCS_D& rData);
	BOOL GetMtcs(T_MTCS_K Key,T_MTCS_D& rData);
	int GetCountMtcs();
	POSITION GetStartMtcs();
	void GetNextMtcs(POSITION& rNextPosition,T_MTCS_K& rKey,T_MTCS_D& rData);
	T_MTCS_K GetMtcsKey(CString& rName);
	T_MTCS_K GetStartNumMtcs();
	T_MTCS_K GetLastNumMtcs();
	T_MTCS_K GetNextNumMtcs(T_MTCS_K StartNum);
	
// element group
public:
	void GetGrupList(CArray<CString, CString&>& rstrGrupList, BOOL bSort=FALSE);  //이름으로 정렬된 리스트
	void GetGrupKeyList(CArray<T_GRUP_K, T_GRUP_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistGrup(CString& rGroupName);
	BOOL ExistGrup(T_GRUP_K Key);
	BOOL GetGrup(CString& rGroupName,T_GRUP_D& rData);
	BOOL GetGrup(T_GRUP_K Key,T_GRUP_D& rData);
	T_GRUP_K GetGrupKey(CString& rGroupName);
	CString GetGrupName(T_GRUP_K Key);
	int GetCountGrup();
	POSITION GetStartGrup();
	void GetNextGrup(POSITION& rNextPosition,T_GRUP_K& rKey,T_GRUP_D& rData);
	T_GRUP_K GetStartNumGrup();
	T_GRUP_K GetLastNumGrup();
	void GetKeyListGrupUser(CArray<T_GRUP_K, T_GRUP_K>* pKeyGrup,           //특정 그룹에 속하는 절점, 요소 번호
							CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	BOOL GetGrup4Stage(T_GRUP_K Key, T_GRUP_D& rData);
	void GetNextGrup4Stage(POSITION& rNextPosition,T_GRUP_K& rKey,T_GRUP_D& rData);
	void RemoveNodeElemOfGrup(T_GRUP_D& rData);

	void GetGrupKeyListByElem(T_ELEM_K ElemK, CArray<T_GRUP_K, T_GRUP_K>& aGrupK);
	// Batch Report(for iGen)
public:
	void GetBrepList(CArray<CString, CString&>& rstrBrepList, BOOL bSort = FALSE);  //이름으로 정렬된 리스트
	void GetBrepKeyList(CArray<T_BREP_K, T_BREP_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistBrep(CString& rGroupName);
	BOOL ExistBrep(T_BREP_K Key);
	BOOL GetBrep(CString& rGroupName, T_BREP_D& rData);
	BOOL GetBrep(T_BREP_K Key, T_BREP_D& rData);
	T_BREP_K GetBrepKey(CString& rGroupName);
	CString GetBrepName(T_BREP_K Key);
	int GetCountBrep();
	POSITION GetStartBrep();
	void GetNextBrep(POSITION& rNextPosition, T_BREP_K& rKey, T_BREP_D& rData);
	T_BREP_K GetStartNumBrep();
	T_BREP_K GetLastNumBrep();

// Sub Domain
public:
	void GetSdomList(CArray<CString, CString&>& rstrSdomList);  //serial 번호로 정렬된 리스트
	void GetSdomKeyList(CArray<T_SDOM_K, T_SDOM_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistSdom(CString& rDomainName);
	BOOL ExistSdom(T_SDOM_K Key);
	BOOL GetSdom(CString& rDomainName,T_SDOM_D& rData);
	BOOL GetSdom(T_SDOM_K Key,T_SDOM_D& rData);
	T_SDOM_K GetSdomKey(CString& rDomainName);
	CString GetSdomName(T_SDOM_K Key);
	int GetCountSdom();
	POSITION GetStartSdom();
	void GetNextSdom(POSITION& rNextPosition,T_SDOM_K& rKey,T_SDOM_D& rData);
	T_SDOM_K GetStartNumSdom();
	T_SDOM_K GetLastNumSdom();  
	/*BOOL GetSdom4Stage(T_SDOM_K Key, T_SDOM_D& rData);
	void GetNextSdom4Stage(POSITION& rNextPosition,T_SDOM_K& rKey,T_SDOM_D& rData);
	void RemoveNodeElemOfSdom(T_SDOM_D& rData);*/

// main Domain
public:
	void GetMadoList(CArray<CString, CString&>& rstrNameList, BOOL bSortByName=FALSE); //Name으로 정렬된 리스트
	void GetMadoKeyList(std::vector<T_MADO_K>& rKeyList, BOOL bSortByName=FALSE); //Name으로 정렬된 리스트
	BOOL ExistMado(CString& rName);
	BOOL ExistMado(T_MADO_K Key);
	BOOL GetMado(CString& rName, T_MADO_D& rData);
	BOOL GetMado(T_MADO_K Key,T_MADO_D& rData);
	int GetCountMado();
	POSITION GetStartMado();
	void GetNextMado(POSITION& rNextPosition,T_MADO_K& rKey,T_MADO_D& rData);
	T_MADO_K GetMadoKey(CString& rPlaneName);
	T_MADO_K GetStartNumMado();
	T_MADO_K GetLastNumMado();
	T_MADO_K GetNextNumMado(T_MADO_K StartNum);
	int GetElemListMadoK(T_MADO_K MadoK, std::vector<T_ELEM_K>& rElemKeyList, CArray<T_DOEL_K, T_DOEL_K>& rDoelKeyList);
	int GetElemListMadoKIncudeSbdo(T_MADO_K MadoK, std::vector<T_ELEM_K>& rElemKeyList, CArray<T_DOEL_K, T_DOEL_K>& rDoelKeyList);

// Sub Domain
public:
	//void GetSbdoList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetSbdoKeyList(CArray<T_SBDO_K, T_SBDO_K>& rKeyList);
	void GetSbdoKeyList(std::vector<T_SBDO_K>& rKeyList);
	int GetSbdoKeyList(T_MADO_K MadoK, std::vector<T_SBDO_K>& rKeyList, BOOL bSortByName=FALSE); //Name으로 정렬된 리스트
	BOOL ExistSbdo(CString& rName);
	BOOL ExistSbdo(T_SBDO_K Key);
	BOOL GetSbdo(CString& rName, T_SBDO_D& rData);
	BOOL GetSbdo(T_SBDO_K Key,T_SBDO_D& rData);
	int GetCountSbdo();
	POSITION GetStartSbdo();
	void GetNextSbdo(POSITION& rNextPosition,T_SBDO_K& rKey,T_SBDO_D& rData);
	T_SBDO_K GetSbdoKey(CString& rPlaneName);
	T_SBDO_K GetStartNumSbdo();
	T_SBDO_K GetLastNumSbdo();
	T_SBDO_K GetNextNumSbdo(T_SBDO_K StartNum);
	int GetElemListSbdoK(T_SBDO_K SbdoK, CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList);
	int GetElemListSbdoK(T_SBDO_K SbdoK, std::vector<T_ELEM_K>& rElemKeyList, CArray<T_DOEL_K, T_DOEL_K>& rDoelKeyList);
	int GetSlabMatTypeSbdoLevel(T_SBDO_K SbdoK, CArray<double,double&>& aLevel);

// Domain Relation
public:
	void GetDoelKeyList(CArray<T_DOEL_K, T_DOEL_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDoel(T_DOEL_K Key);
	BOOL GetDoel(T_DOEL_K Key,T_DOEL_D& rData);
	int GetCountDoel();
	POSITION GetStartDoel();
	void GetNextDoel(POSITION& rNextPosition,T_DOEL_K& rKey,T_DOEL_D& rData);  

// boundary group
public:
	void GetKeyListBngrUser(CArray<T_BNGR_K, T_BNGR_K>* pKeyBngr,
							CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	void GetBngrList(CArray<CString, CString&>& rstrBngrList);  // serial 번호로 정렬된 리스트
	void GetBngrKeyList(CArray<T_BNGR_K, T_BNGR_K>& rKeyList);  // serial 번호로 정렬된 리스트
	BOOL ExistBngr(CString& rGroupName);
	BOOL ExistBngr(T_BNGR_K Key);
	BOOL GetBngr(CString& rGroupName,T_BNGR_D& rData);
	BOOL GetBngr(T_BNGR_K Key,T_BNGR_D& rData);
	T_BNGR_K GetBngrKey(CString& rGroupName);
	CString GetBngrName(T_BNGR_K Key);
	int GetCountBngr();
	POSITION GetStartBngr();
	void GetNextBngr(POSITION& rNextPosition,T_BNGR_K& rKey,T_BNGR_D& rData);
	T_BNGR_K GetStartNumBngr();
	T_BNGR_K GetLastNumBngr();
	void GetBngrKeyList4Wssf(CArray<T_BNGR_K, T_BNGR_K>& rKeyList);
	void GetBngrKeyList4Pssf(CArray<T_BNGR_K, T_BNGR_K>& rKeyList);
	void GetBngrKeyList4Secf(CArray<T_BNGR_K, T_BNGR_K>& rKeyList);

// load group
public:
	void GetKeyListLdgrUser(CArray<T_LDGR_K, T_LDGR_K>* pKeyLdgr,
							CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	void GetLdgrList(CArray<CString, CString&>& rstrLdgrList);  // serial 번호로 정렬된 리스트
	void GetLdgrKeyList(CArray<T_LDGR_K, T_LDGR_K>& rKeyList);  // serial 번호로 정렬된 리스트
	BOOL ExistLdgr(CString& rGroupName);
	BOOL ExistLdgr(T_LDGR_K Key);
	BOOL GetLdgr(CString& rGroupName,T_LDGR_D& rData);
	BOOL GetLdgr(T_LDGR_K Key,T_LDGR_D& rData);
	T_LDGR_K GetLdgrKey(CString& rGroupName);
	CString GetLdgrName(T_LDGR_K Key);
	int GetCountLdgr();
	POSITION GetStartLdgr();
	void GetNextLdgr(POSITION& rNextPosition,T_LDGR_K& rKey,T_LDGR_D& rData);
	T_LDGR_K GetStartNumLdgr();
	T_LDGR_K GetLastNumLdgr();
	void GetLdgrHavingFloorOrPlaneLoad(CMapEx<UINT, UINT, int, int>& rmLdgrK);
	void GetLdgrDeactivatedInStag(CMapEx<UINT, UINT, int, int>& rmLdgrK);

// tendon group
public:
	void GetKeyListTdgrUser(CArray<T_TDGR_K, T_TDGR_K>* pKeyTdgr,
													CArray<T_TDNA_K, T_TDNA_K>* pKeyTdna);
	void GetTdgrList(CArray<CString, CString&>& rstrTdgrList);  // serial 번호로 정렬된 리스트
	void GetTdgrKeyList(CArray<T_TDGR_K, T_TDGR_K>& rKeyList);  // serial 번호로 정렬된 리스트
	BOOL ExistTdgr(CString& rGroupName);
	BOOL ExistTdgr(T_TDGR_K Key);
	BOOL GetTdgr(CString& rGroupName,T_TDGR_D& rData);
	BOOL GetTdgr(T_TDGR_K Key,T_TDGR_D& rData);
	T_TDGR_K GetTdgrKey(CString& rGroupName);
	CString GetTdgrName(T_TDGR_K Key);
	int GetCountTdgr();
	POSITION GetStartTdgr();
	void GetNextTdgr(POSITION& rNextPosition,T_TDGR_K& rKey,T_TDGR_D& rData);
	T_TDGR_K GetStartNumTdgr();
	T_TDGR_K GetLastNumTdgr();

//offset
public:
	BOOL MakeKeyOffs(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_OFFS_K& rKey);
	void GetOffsKeyList(CArray<T_OFFS_K, T_OFFS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetOffsElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistOffs(T_OFFS_K Key);
	BOOL ExistOffsElem(T_ELEM_K KeyElem);
	BOOL GetOffs(T_OFFS_K Key,T_OFFS_D& rData);
	void GetOffsElem(T_ELEM_K KeyElem, CArray<T_OFFS_K, T_OFFS_K>& rKeyList, CArray<T_OFFS_D, T_OFFS_D&>& rDataList);
	BOOL GetOffsElem(T_ELEM_K KeyElem, T_OFFS_D& rData);
	BOOL GetOffsBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_OFFS_D& rData);
	int GetCountOffs();
	POSITION GetStartOffs();
	void GetNextOffs(POSITION& rNextPosition,T_OFFS_K& rKey,T_OFFS_D& rData);
protected:
	BOOL GetSerialOffs(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_OFFS_KEY& rKeySerial);

//constraint
public:
	BOOL MakeKeyCons(T_NODE_K KeyNode,T_BNGR_K KeyBngr,T_CONS_K& rKey);
	void GetConsKeyList(CArray<T_CONS_K, T_CONS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetConsNodeKeyList(CArray<T_NODE_K, T_NODE_K>& rNodeKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistCons(T_CONS_K Key);
	BOOL GetCons(T_CONS_K Key,T_CONS_D& rData);
	void GetConsNode(T_NODE_K KeyNode, CArray<T_CONS_K, T_CONS_K>& rKeyList, CArray<T_CONS_D, T_CONS_D&>& rDataList);
	BOOL GetConsNode(T_NODE_K KeyNode, T_CONS_D& rData);
	BOOL GetConsBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_NODE_K KeyNode, T_CONS_D& rData);
	int GetCountCons();
	POSITION GetStartCons();
	void GetNextCons(POSITION& rNextPosition,T_CONS_K& rKey,T_CONS_D& rData);
	void GetConstraintCondition(T_NODE_K KeyNode, T_CONS_D& rData);
protected:
	BOOL GetSerialCons(T_NODE_K KeyNode,T_BNGR_K KeyBngr,T_CONS_KEY& rKeySerial);

//nodal spring
public:
	//BOOL MakeKeyNspr(T_NODE_K KeyNode,T_BNGR_K KeyBngr,T_NSPR_K& rKey);
	BOOL MakeKeyNspr(T_NODE_K KeyNode,T_NSPR_K& rKey);
	void GetNsprKeyList(CArray<T_NSPR_K, T_NSPR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetNsprKeyList(T_NODE_K KeyNode,T_BNGR_K KeyBngr,CArray<T_NSPR_K, T_NSPR_K>& rNodeKeyList);
	void GetNsprNodeKeyList(CArray<T_NODE_K, T_NODE_K>& rNodeKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistNspr(T_NSPR_K Key);
	BOOL GetNspr(T_NSPR_K Key,T_NSPR_D& rData);
	void GetNsprNode(T_NODE_K KeyNode, CArray<T_NSPR_K, T_NSPR_K>& rKeyList, CArray<T_NSPR_D, T_NSPR_D&>& rDataList);
	//BOOL GetNsprNode(T_NODE_K KeyNode, T_NSPR_D& rData);
	BOOL GetNsprDrawNode(T_NODE_K KeyNode, T_NSPR_D& rData,int nType);
	//BOOL GetNsprBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_NODE_K KeyNode, T_NSPR_D& rData);
	BOOL GetNsprDrawBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_NODE_K KeyNode, T_NSPR_D& rData,int nType);
	int GetCountNspr();
	POSITION GetStartNspr();
	void GetNextNspr(POSITION& rNextPosition,T_NSPR_K& rKey,T_NSPR_D& rData);
	bool CompareNsprKey(const T_NSPR_K& Key1, const T_NSPR_K& Key2);
protected:
	//BOOL GetSerialNspr(T_NODE_K KeyNode,T_BNGR_K KeyBngr,T_NSPR_KEY& rKeySerial);
	BOOL GetSerialNspr(T_NODE_K KeyNode,T_NSPR_KEY& rKeySerial);

	// Multi Linear Function
public:
	void GetMlfcList(CArray<CString, CString&>& rstrMlfcList);  //serial 번호로 정렬된 리스트
	void GetMlfcKeyList(CArray<T_MLFC_K, T_MLFC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMlfc(CString& rFuncName);
	BOOL ExistMlfc(T_MLFC_K Key);
	BOOL GetMlfc(CString& rFuncName, T_MLFC_D& rData);
	BOOL GetMlfc(T_MLFC_K Key, T_MLFC_D& rData);
	T_MLFC_K GetMlfcByData(const T_MLFC_DT& GetMlfcByData);
	T_MLFC_K GetMlfcKey(CString& rFuncName);
	int GetCountMlfc();
	POSITION GetStartMlfc();
	void GetNextMlfc(POSITION& rNextPosition,T_MLFC_K& rKey,T_MLFC_D& rData, BOOL bConvertUnit = TRUE);
	T_MLFC_K GetStartNumMlfc();
	T_MLFC_K GetLastNumMlfc();
	void interpolateDuplicates(CArray<T_MLFC_BASE, T_MLFC_BASE&>& aFuncDataOri);

	//Rail-Track-Interaction Function
public:
	void GetRlfcList(CArray<CString, CString&>& rstrRlfcList);  //serial 번호로 정렬된 리스트
	void GetRlfcKeyList(CArray<T_RLFC_K, T_RLFC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistRlfc(CString& rFuncName);
	BOOL ExistRlfc(T_RLFC_K Key);
	BOOL GetRlfc(CString& rFuncName, T_RLFC_D& rData);
	BOOL GetRlfc(T_RLFC_K Key, T_RLFC_D& rData);
	T_RLFC_K GetRlfcKey(CString& rFuncName);
	int GetCountRlfc();
	POSITION GetStartRlfc();
	void GetNextRlfc(POSITION& rNextPosition, T_RLFC_K& rKey, T_RLFC_D& rData);
	T_RLFC_K GetStartNumRlfc();
	T_RLFC_K GetLastNumRlfc();


// Surface Spring Support
public:
	BOOL MakeKeySsps(T_ELEM_K Key,T_SSPS_K& rKey);
	void GetSspsKeyList(CArray<T_SSPS_K, T_SSPS_K>& rKeyList);
	BOOL ExistSsps(T_SSPS_K Key);
	BOOL GetSsps(T_SSPS_K Key,T_SSPS_D& rData);
	BOOL GetSsps(T_ELEM_K KeyElem, CArray<T_SSPS_K, T_SSPS_K>& rarKey, CArray<T_SSPS_D, T_SSPS_D&>& rarData);
	BOOL GetSspsBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, CArray<T_SSPS_D, T_SSPS_D&>& raData);
	int GetCountSsps();
	POSITION GetStartSsps();
	void GetNextSsps(POSITION& rNextPosition,T_SSPS_K& rKey,T_SSPS_D& rData);
	BOOL GetAssignedNodeSsps(T_SSPS_K SspsK, CArray<T_NODE_K, T_NODE_K>& aTmpNode);
protected:
	BOOL GetSerialSsps(T_ELEM_K Key,T_SSPS_KEY& rKeySerial);

//define general spring type
public:
	void GetGstpList(CArray<CString, CString&>& rstrGstpList);  //serial 번호로 정렬된 리스트
	void GetGstpKeyList(CArray<T_GSTP_K, T_GSTP_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistGstp(CString& rSpringTypeName);
	BOOL ExistGstp(T_GSTP_K Key);
	BOOL GetGstp(CString& rSpringTypeName, T_GSTP_D& rData);
	BOOL GetGstp(T_GSTP_K Key, T_GSTP_D& rData);
	T_GSTP_K GetGstpKey(CString& rSpringTypeName);
	int GetCountGstp();
	POSITION GetStartGstp();
	void GetNextGstp(POSITION& rNextPosition,T_GSTP_K& rKey,T_GSTP_D& rData);
	T_GSTP_K GetStartNumGstp();
	T_GSTP_K GetLastNumGstp();

// General link property
public:
	void GetNllpList(CArray<CString, CString&>& rstrNllpList);  //serial 번호로 정렬된 리스트
	void GetNllpKeyList(CArray<T_NLLP_K, T_NLLP_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistNllp(CString& rPropName);
	BOOL ExistNllp(T_NLLP_K Key);
	BOOL GetNllp(CString& rPropName, T_NLLP_D& rData);
	BOOL GetNllp(T_NLLP_K Key, T_NLLP_D& rData);
	BOOL GetNllpSeq(int nSeq, T_NLLP_K& Key);
	T_NLLP_K GetNllpKey(CString& rPropName);
	int GetCountNllp();
	POSITION GetStartNllp();
	void GetNextNllp(POSITION& rNextPosition,T_NLLP_K& rKey,T_NLLP_D& rData);
	T_NLLP_K GetStartNumNllp();
	T_NLLP_K GetLastNumNllp();

	// Damper
public:
	void GetDmgbList(CArray<CString, CString&>& rstrDmgbList);  //serial 번호로 정렬된 리스트
	void GetDmgbKeyList(CArray<T_DMGB_K, T_DMGB_K>& rKeyList);  //serial 번호로 정렬된 리스트
	void GetDmgbOildKeyList(CArray<T_DMGB_K, T_DMGB_K>& rKeyList);  //serial 번호로 정렬된 리스트
	void GetDmgbStldKeyList(CArray<T_DMGB_K, T_DMGB_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistDmgb(CString& rPropName);
	BOOL ExistDmgb(T_DMGB_K Key);
	BOOL GetDmgb(CString& rPropName, T_DMGB_D& rData);
	BOOL GetDmgb(T_DMGB_K Key, T_DMGB_D& rData);
	BOOL GetDmgbSeq(int nSeq, T_DMGB_K& Key);
	T_DMGB_K GetDmgbKey(CString& rPropName);
	int GetCountDmgb();
	POSITION GetStartDmgb();
	void GetNextDmgb(POSITION& rNextPosition, T_DMGB_K& rKey, T_DMGB_D& rData);
	T_DMGB_K GetStartNumDmgb();
	T_DMGB_K GetLastNumDmgb();
	int GetDmgbOildSerialNum(T_DMGB_K DmgbK); // Dmgb 중 번호순으로 몇번째인가?
	int GetDmgbStldSerialNum(T_DMGB_K DmgbK);
	void GetInitializedDmgbInCurUnit(T_DMGB_D& DmgbD);
	BOOL IsUserDMGB(T_DMGB_K DmgbK);

	//Damp performance check info:DPCI
	BOOL ExistDpci();
	BOOL GetDpci(T_DPCI_D& rData);

	//Assessment ReinForce
public:
	//ReinForce Check Total Info
	BOOL ExistRcti();
	BOOL GetRcti(T_RCTI_D& rData);

	//ReinForce Design Parameter
	BOOL ExistRdpa();
	BOOL GetRdpa(T_RDPA_D& rData);

	//Total Appraisal Information
	BOOL ExistTaif();
	BOOL GetTaif(T_TAIF_D& rData);

// Seismic Isolator
public:
	void GetAsgbList(CArray<CString, CString&>& rstrAsgbList);  //serial 번호로 정렬된 리스트
	void GetAsgbKeyList(CArray<T_ASGB_K, T_ASGB_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistAsgb(CString& rPropName);
	BOOL ExistAsgb(T_ASGB_K Key);
	BOOL GetAsgb(CString& rPropName, T_ASGB_D& rData);
	BOOL GetAsgb(T_ASGB_K Key, T_ASGB_D& rData);
	BOOL GetAsgbSeq(int nSeq, T_ASGB_K& Key);
	T_ASGB_K GetAsgbKey(CString& rPropName);
	int GetCountAsgb();
	POSITION GetStartAsgb();
	void GetNextAsgb(POSITION& rNextPosition, T_ASGB_K& rKey, T_ASGB_D& rData);
	T_ASGB_K GetStartNumAsgb();
	T_ASGB_K GetLastNumAsgb();
	int GetAsgbSerialNum(T_ASGB_K SdisK); // Asgb 중 번호순으로 몇번째인가?
	void GetInitializedAsgbInCurUnit(T_ASGB_D& AsgbD);

	// Isolation layer
public:
	BOOL ExistIsoLayer();
	BOOL GetIsoLayer(T_ISLY_D& rData);

	// Seismic Isolation
public:
	BOOL ExistIsolation();
	BOOL GetIsolation(T_ISGB_D& rData);

	// Seismic Damping
public:
	BOOL ExistDamping();
	BOOL GetDamping(T_DAGB_D& rData);

//general spring
public:
	BOOL MakeKeyGspr(T_NODE_K KeyNode,T_BNGR_K KeyBngr,T_GSPR_K& rKey);
	void GetGsprKeyList(CArray<T_GSPR_K, T_GSPR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetGsprNodeKeyList(CArray<T_NODE_K, T_NODE_K>& rNodeKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistGspr(T_GSPR_K Key);
	BOOL GetGspr(T_GSPR_K Key,T_GSPR_D& rData);
	void GetGsprNode(T_NODE_K KeyNode, CArray<T_GSPR_K, T_GSPR_K>& rKeyList, CArray<T_GSPR_D, T_GSPR_D&>& rDataList);
	BOOL GetGsprBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_NODE_K KeyNode, CArray<T_GSPR_D, T_GSPR_D&>& raData);
	int GetCountGspr();
	POSITION GetStartGspr();
	void GetNextGspr(POSITION& rNextPosition,T_GSPR_K& rKey,T_GSPR_D& rData);
protected:
	BOOL GetSerialGspr(T_NODE_K KeyNode,T_BNGR_K KeyBngr,T_GSPR_KEY& rKeySerial);

//Rail-Track Interaction Elnk Type
public:
	void GetReltKeyList(CArray<T_RELT_K, T_RELT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRelt(T_RELT_D& rData);
	BOOL ExistRelt(T_RELT_K Key);
	BOOL GetRelt(T_RELT_K Key, T_RELT_D& rData);
	T_RELT_K GetReltKey(T_RELT_D& rData);
	int  GetReltList(CArray<T_RELT_K, T_RELT_K>& raKey, CArray<T_RELT_D, T_RELT_D&>& raData);
	int GetCountRelt();
	POSITION GetStartRelt();
	void GetNextRelt(POSITION& rNextPosition, T_RELT_K& rKey, T_RELT_D& rData);
	T_RELT_K GetStartNumRelt();
	T_RELT_K GetLastNumRelt();

//elastic link
public:
	void GetElnkKeyList(CArray<T_ELNK_K, T_ELNK_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistElnk(T_ELNK_K Key);
	BOOL GetElnk(T_ELNK_K Key,T_ELNK_D& rData);
	int  GetElnkList(CArray<T_ELNK_K, T_ELNK_K>& raKey, CArray<T_ELNK_D, T_ELNK_D&>& raData);
	BOOL GetElnkBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, CArray<T_ELNK_K, T_ELNK_K>& raKey, CArray<T_ELNK_D, T_ELNK_D&>& raData);
	int GetCountElnk();
	POSITION GetStartElnk();
	void GetNextElnk(POSITION& rNextPosition,T_ELNK_K& rKey,T_ELNK_D& rData);
	T_ELNK_K GetStartNumElnk();
	T_ELNK_K GetLastNumElnk();
	//Rail-Track Interaction Elnk로 변환.
	BOOL IsConvertMElnktoRElnk(CArray<T_ELNK_D, T_ELNK_D&>& arElnkD, CArray<int, int&>& aStatus, CArray<T_ELNK_K, T_ELNK_K&>& rElnkDel, CMap<T_MLFC_K, T_MLFC_K, T_MLFC_K, T_MLFC_K>& mapMfclDel);
	BOOL ConvertMElnktoRElnk();
	void MakeElnkNewWithRlfc(CArray<T_ELNK_D, T_ELNK_D&>& arElnkD, CArray<int, int&>& aStatus, BOOL bFindSameMlfc,
							std::map<KeyRlfcPos, ValForRlfcPos>& mapRlfcPosData, std::map<KeyReltPos, ValReltPos>& mapReltData, BOOL bIsReadFile4Convett = FALSE);
	void SetElnktoRElnk(std::map<KeyRlfcPos, ValForRlfcPos>& mapRlfcPosData, std::map<KeyReltPos, ValReltPos>& mapReltData);
	void DeleteMElnkData(CArray<T_ELNK_K, T_ELNK_K&>& rElnkDel, CMap<T_MLFC_K, T_MLFC_K, T_MLFC_K, T_MLFC_K>& mapMfclDel);


//General link
public:
	void GetNlnkKeyList(CArray<T_NLNK_K, T_NLNK_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetNlnkKeyByNodeList(CArray<T_NLNK_K, T_NLNK_K>& rKeyList,const CArray<T_NODE_K,T_NODE_K>& aNodeList);
	int  GetNlnkKeyListBetween2Nodes(T_NODE_K NodeK1, T_NODE_K NodeK2, CArray<T_NLNK_K, T_NLNK_K>& rKeyList);
	BOOL ExistNlnk(T_NLNK_K Key);
	BOOL GetNlnk(T_NLNK_K Key,T_NLNK_D& rData);
	int  GetNlnkList(CArray<T_NLNK_K, T_NLNK_K>& raKey, CArray<T_NLNK_D, T_NLNK_D&>& raData);
	BOOL GetNlnkBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, CArray<T_NLNK_K, T_NLNK_K>& raKey, CArray<T_NLNK_D, T_NLNK_D&>& raData);
	int GetCountNlnk();
	POSITION GetStartNlnk();
	void GetNextNlnk(POSITION& rNextPosition,T_NLNK_K& rKey,T_NLNK_D& rData);
	T_NLNK_K GetStartNumNlnk();
	T_NLNK_K GetLastNumNlnk();

//Change General Link Property
public:
	void GetCglpKeyList(CArray<T_CGLP_K, T_CGLP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCglp(T_CGLP_K Key);
	BOOL GetCglp(T_CGLP_K Key,T_CGLP_D& rData);
	int  GetCglpList(CArray<T_CGLP_K, T_CGLP_K>& raKey, CArray<T_CGLP_D, T_CGLP_D&>& raData);
	BOOL GetCglpBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, CArray<T_CGLP_K, T_CGLP_K>& raKey, CArray<T_CGLP_D, T_CGLP_D&>& raData);
	int GetCountCglp();
	POSITION GetStartCglp();
	void GetNextCglp(POSITION& rNextPosition,T_CGLP_K& rKey,T_CGLP_D& rData);
	T_CGLP_K GetStartNumCglp();
	T_CGLP_K GetLastNumCglp();
	T_CGLP_K GetCglpKey(T_CGLP_D& rData);

//node local axis
public:
	void GetSkewKeyList(CArray<T_SKEW_K, T_SKEW_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSkew(T_SKEW_K Key);
	BOOL GetSkew(T_SKEW_K Key,T_SKEW_D& rData);
	int GetCountSkew();
	POSITION GetStartSkew();
	void GetNextSkew(POSITION& rNextPosition,T_SKEW_K& rKey,T_SKEW_D& rData);

// Constraint Label Direction
public:
	void GetCldrKeyList(CArray<T_CLDR_K, T_CLDR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCldr(T_CLDR_K Key);
	BOOL GetCldr(T_CLDR_K Key,T_CLDR_D& rData);
	int GetCountCldr();
	POSITION GetStartCldr();
	void GetNextCldr(POSITION& rNextPosition,T_CLDR_K& rKey,T_CLDR_D& rData);

//beam release
public:
	BOOL MakeKeyFrls(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_FRLS_K& rKey);
	void GetFrlsKeyList(CArray<T_FRLS_K, T_FRLS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetFrlsElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistFrls(T_FRLS_K Key);
	BOOL ExistFrlsElem(T_ELEM_K KeyElem);
	BOOL GetFrls(T_FRLS_K Key,T_FRLS_D& rData);
	void GetFrlsElem(T_ELEM_K KeyElem, CArray<T_FRLS_K, T_FRLS_K>& rKeyList, CArray<T_FRLS_D, T_FRLS_D&>& rDataList);
	BOOL GetFrlsElem(T_ELEM_K KeyElem, T_FRLS_D& rData);       // 요소에 중복입력일 경우 Pin 조건을 우선으로..
	BOOL GetFrlsElem_Fix(T_ELEM_K KeyElem, T_FRLS_D& rData);   // 요소에 중복입력일 경우 Fix 조건을 우선으로..
	BOOL GetFrlsBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_FRLS_D& rData);
	int GetCountFrls();
	POSITION GetStartFrls();
	void GetNextFrls(POSITION& rNextPosition,T_FRLS_K& rKey,T_FRLS_D& rData);
	BOOL CalcFrlsData(CArray<T_FRLS_K, T_FRLS_K>& aFrlsK, T_FRLS_D& rData);
	BOOL CalcFrlsData(CArray<T_FRLS_D, T_FRLS_D&>& aFrlsD, T_FRLS_D& rData);
protected:
	BOOL GetSerialFrls(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_FRLS_KEY& rKeySerial);

//plate release
public:
	BOOL MakeKeyPrls(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_PRLS_K& rKey);
	void GetPrlsKeyList(CArray<T_PRLS_K, T_PRLS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetPrlsElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistPrls(T_PRLS_K Key);
	BOOL ExistPrlsElem(T_ELEM_K KeyElem);
	BOOL GetPrls(T_PRLS_K Key,T_PRLS_D& rData);
	void GetPrlsElem(T_ELEM_K KeyElem, CArray<T_PRLS_K, T_PRLS_K>& rKeyList, CArray<T_PRLS_D, T_PRLS_D&>& rDataList);
	BOOL GetPrlsElem(T_ELEM_K KeyElem, T_PRLS_D& rData);
	BOOL GetPrlsBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_PRLS_D& rData);
	int GetCountPrls();
	POSITION GetStartPrls();
	void GetNextPrls(POSITION& rNextPosition,T_PRLS_K& rKey,T_PRLS_D& rData);
	BOOL CalcPrlsData(CArray<T_PRLS_K, T_PRLS_K>& aPrlsK, T_PRLS_D& rData);
	BOOL CalcPrlsData(CArray<T_PRLS_D, T_PRLS_D&>& aPrlsD, T_PRLS_D& rData);
protected:
	BOOL GetSerialPrls(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_PRLS_KEY& rKeySerial);

//rigid link
public:
	BOOL MakeKeyRigd(T_NODE_K KeyMaster, unsigned int nSlaveType, T_BNGR_K KeyBngr, T_RIGD_K& rKey);
	void GetRigdKeyList(CArray<T_RIGD_K, T_RIGD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRigd(T_RIGD_K Key);
	BOOL GetRigd(T_RIGD_K Key,T_RIGD_D& rData);
	void GetRigd(T_NODE_K KeyNode, CArray<T_RIGD_K, T_RIGD_K>& rarKey, CArray<T_RIGD_D, T_RIGD_D&>& rarData, BOOL bOnlyMaster=FALSE);
	int  GetRigdList(CArray<T_RIGD_K, T_RIGD_K>& raKey, CArray<T_RIGD_D, T_RIGD_D&>& raData);
	BOOL GetRigdBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, CArray<T_RIGD_K, T_RIGD_K>& raKey, CArray<T_RIGD_D, T_RIGD_D&>& raData);
	int GetCountRigd();
	POSITION GetStartRigd();
	void GetNextRigd(POSITION& rNextPosition,T_RIGD_K& rKey,T_RIGD_D& rData);
protected:
	BOOL GetSerialRigd(T_NODE_K KeyMaster, unsigned int nSlaveType, T_BNGR_K KeyBngr, T_RIGD_KEY& rKeySerial);

//mcon
public:
	BOOL MakeKeyMcon(T_NODE_K KeyMaster, unsigned int nSlaveType, int nType, T_BNGR_K KeyBngr, T_MCON_K& rKey);
	void GetMconKeyList(CArray<T_MCON_K, T_MCON_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMcon(T_MCON_K Key);
	BOOL GetMcon(T_MCON_K Key,T_MCON_D& rData);
	void GetMcon(T_NODE_K KeyNode, CArray<T_MCON_K, T_MCON_K>& rarKey, CArray<T_MCON_D, T_MCON_D&>& rarData, BOOL bOnlyMaster=FALSE);
	int  GetMconList(CArray<T_MCON_K, T_MCON_K>& raKey, CArray<T_MCON_D, T_MCON_D&>& raData);
	BOOL GetMconBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, CArray<T_MCON_K, T_MCON_K>& raKey, CArray<T_MCON_D, T_MCON_D&>& raData);
	int  GetCountMcon();
	POSITION GetStartMcon();
	void GetNextMcon(POSITION& rNextPosition,T_MCON_K& rKey,T_MCON_D& rData);
protected:
	BOOL GetSerialMcon(T_NODE_K KeyMaster, unsigned int nSlaveType, int nType, T_BNGR_K KeyBngr, T_MCON_KEY& rKeySerial);


// effective width for Steel Composite
public:
	BOOL MakeKeyEwsc(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_EWSC_K& rKey);
	void GetEwscKeyList(CArray<T_EWSC_K, T_EWSC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetEwscElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistEwsc(T_EWSC_K Key);
	BOOL ExistEwscElem(T_ELEM_K KeyElem);
	BOOL GetEwsc(T_EWSC_K Key,T_EWSC_D& rData);
	void GetEwscElem(T_ELEM_K KeyElem, CArray<T_EWSC_K, T_EWSC_K>& rKeyList, CArray<T_EWSC_D, T_EWSC_D&>& rDataList);
	BOOL GetEwscElem(T_ELEM_K KeyElem, T_EWSC_D& rData);
	BOOL GetEwscBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_EWSC_D& rData);
	int GetCountEwsc();
	POSITION GetStartEwsc();
	void GetNextEwsc(POSITION& rNextPosition,T_EWSC_K& rKey,T_EWSC_D& rData);
protected:
	BOOL GetSerialEwsc(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_EWSC_KEY& rKeySerial);


	// effective width scale factor for design
public:
	BOOL MakeKeyEwcd(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_EWSC_K& rKey);
	void GetEwcdKeyList(CArray<T_EWSC_K, T_EWSC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetEwcdElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistEwcd(T_EWSC_K Key);
	BOOL ExistEwcdElem(T_ELEM_K KeyElem);
	BOOL GetEwcd(T_EWSC_K Key,T_EWSC_D& rData);
	void GetEwcdElem(T_ELEM_K KeyElem, CArray<T_EWSC_K, T_EWSC_K>& rKeyList, CArray<T_EWSC_D, T_EWSC_D&>& rDataList);
	BOOL GetEwcdElem(T_ELEM_K KeyElem, T_EWSC_D& rData);
	BOOL GetEwcdBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_EWSC_D& rData);
	int GetCountEwcd();
	POSITION GetStartEwcd();
	void GetNextEwcd(POSITION& rNextPosition,T_EWSC_K& rKey,T_EWSC_D& rData);
protected:
	BOOL GetSerialEwcd(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_EWSC_KEY& rKeySerial);


// effective width scale factor
public:
	BOOL MakeKeyEwsf(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_EWSF_K& rKey);
	void GetEwsfKeyList(CArray<T_EWSF_K, T_EWSF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetEwsfElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistEwsf(T_EWSF_K Key);
	BOOL ExistEwsfElem(T_ELEM_K KeyElem);
	BOOL GetEwsf(T_EWSF_K Key,T_EWSF_D& rData);
	void GetEwsfElem(T_ELEM_K KeyElem, CArray<T_EWSF_K, T_EWSF_K>& rKeyList, CArray<T_EWSF_D, T_EWSF_D&>& rDataList);
	BOOL GetEwsfElem(T_ELEM_K KeyElem, T_EWSF_D& rData);
	BOOL GetEwsfBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_EWSF_D& rData);
	int GetCountEwsf();
	POSITION GetStartEwsf();
	void GetNextEwsf(POSITION& rNextPosition,T_EWSF_K& rKey,T_EWSF_D& rData);
protected:
	BOOL GetSerialEwsf(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_EWSF_KEY& rKeySerial);
// effective width scale factor for design
public:
	BOOL MakeKeyEwsd(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_EWSF_K& rKey);
	void GetEwsdKeyList(CArray<T_EWSF_K, T_EWSF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetEwsdElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistEwsd(T_EWSF_K Key);
	BOOL ExistEwsdElem(T_ELEM_K KeyElem);
	BOOL GetEwsd(T_EWSF_K Key,T_EWSF_D& rData);
	void GetEwsdElem(T_ELEM_K KeyElem, CArray<T_EWSF_K, T_EWSF_K>& rKeyList, CArray<T_EWSF_D, T_EWSF_D&>& rDataList);
	BOOL GetEwsdElem(T_ELEM_K KeyElem, T_EWSF_D& rData);
	BOOL GetEwsdBngr(BOOL bAllGroup, CArray<T_BNGR_K, T_BNGR_K>& raKeyBngr, T_ELEM_K KeyElem, T_EWSF_D& rData);
	int GetCountEwsd();
	POSITION GetStartEwsd();
	void GetNextEwsd(POSITION& rNextPosition,T_EWSF_K& rKey,T_EWSF_D& rData);
protected:
	BOOL GetSerialEwsd(T_ELEM_K KeyElem,T_BNGR_K KeyBngr,T_EWSF_KEY& rKeySerial);




//nodal mass
public:
	void GetNmasKeyList(CArray<T_NMAS_K, T_NMAS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistNmas(T_NMAS_K Key);
	BOOL GetNmas(T_NMAS_K Key,T_NMAS_D& rData);
	int GetCountNmas();
	POSITION GetStartNmas();
	void GetNextNmas(POSITION& rNextPosition,T_NMAS_K& rKey,T_NMAS_D& rData);

//grid nodal mass
public:
	void GetGmasKeyList(CArray<T_GMAS_K, T_GMAS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistGmas(T_GMAS_K Key);
	BOOL GetGmas(T_GMAS_K Key,T_GMAS_D& rData);
	int GetCountGmas();
	POSITION GetStartGmas();
	void GetNextGmas(POSITION& rNextPosition,T_GMAS_K& rKey,T_GMAS_D& rData);

// static load case
public:
	void GetStldList(CArray<CString, CString&>& rstrStldList);  //serial 번호로 정렬된 리스트  
	void GetStldKeyList(CArray<T_STLD_K, T_STLD_K>& rKeyList);  //serial 번호로 정렬된 리스트
	void GetStldKeyList4Exld(CArray<T_STLD_K, T_STLD_K>& rStldKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistStld(const CString& rLoadCaseName);
	BOOL ExistStld(T_STLD_K Key);
	BOOL GetStld(const CString& rLoadCaseName, T_STLD_D& rData);
	BOOL GetStld(T_STLD_K Key, T_STLD_D& rData);
	T_STLD_K GetStldKey(CString& rLoadCaseName);
	int GetCountStld();
	POSITION GetStartStld();
	void GetNextStld(POSITION& rNextPosition,T_STLD_K& rKey,T_STLD_D& rData);
	T_STLD_K GetStartNumStld();
	T_STLD_K GetLastNumStld();
	void GetStldKeyListVariableParam(CArray<T_STLD_K, T_STLD_K>& rStldKeyList, int nTypeCount, ...); //serial 번호로 정렬된 리스트

// impact factor - ref. hnge
public:
	void GetImpfKeyList(CArray<T_IMPF_K, T_IMPF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetImpfElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistImpf(T_IMPF_K Key);
	BOOL GetImpf(T_IMPF_K Key,T_IMPF_D& rData);
	void GetImpfElem(T_ELEM_K KeyElem, CArray<T_IMPF_K, T_IMPF_K>& rKeyList, CArray<T_IMPF_D, T_IMPF_D&>& rDataList);
	int GetCountImpf();
	POSITION GetStartImpf();
	void GetNextImpf(POSITION& rNextPosition,T_IMPF_K& rKey,T_IMPF_D& rData);

// line lane
public:
	void GetLlanList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetLlanKeyList(CArray<T_LLAN_K, T_LLAN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLlan(CString& rLaneName);
	BOOL ExistLlan(T_LLAN_K Key);
	BOOL GetLlan(CString& rLaneName, T_LLAN_D& rData);
	BOOL GetLlan(T_LLAN_K Key,T_LLAN_D& rData);
	int GetCountLlan();
	POSITION GetStartLlan();
	void GetNextLlan(POSITION& rNextPosition,T_LLAN_K& rKey,T_LLAN_D& rData);
	T_LLAN_K GetLlanKey(CString& rLaneName);
	T_LLAN_K GetStartNumLlan();
	T_LLAN_K GetLastNumLlan();

// line lane for japan version
public:
	BOOL GetLlanjpFromBaseDB(T_LLANjp_D_OLD& rData);
	BOOL ExistLlanjp_Old();
	BOOL GetLlanjp_Old(T_LLANjp_D_OLD& rData);
	///
	void GetLlanjpList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetLlanjpKeyList(CArray<T_LLANjp_K, T_LLANjp_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLlanjp(CString& rLaneName);
	BOOL ExistLlanjp(T_LLANjp_K Key);
	BOOL GetLlanjp(CString& rLaneName, T_LLANjp_D& rData);
	BOOL GetLlanjp(T_LLANjp_K Key, T_LLANjp_D& rData);
	int GetCountLlanjp();
	POSITION GetStartLlanjp();
	void GetNextLlanjp(POSITION& rNextPosition, T_LLANjp_K& rKey, T_LLANjp_D& rData);
	T_LLANjp_K GetLlanjpKey(CString& rLaneName);
	T_LLANjp_K GetStartNumLlanjp();
	T_LLANjp_K GetLastNumLlanjp();

// line lane for Chinese version
public:
	void GetLlanchList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetLlanchKeyList(CArray<T_LLANch_K, T_LLANch_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLlanch(CString& rLaneName);
	BOOL ExistLlanch(T_LLANch_K Key);
	BOOL GetLlanch(CString& rLaneName, T_LLANch_D& rData);
	BOOL GetLlanch(T_LLANch_K Key,T_LLANch_D& rData);
	int GetCountLlanch();
	POSITION GetStartLlanch();
	void GetNextLlanch(POSITION& rNextPosition,T_LLANch_K& rKey,T_LLANch_D& rData);
	T_LLANch_K GetLlanchKey(CString& rLaneName);
	T_LLANch_K GetStartNumLlanch();
	T_LLANch_K GetLastNumLlanch();

// line lane for India version
public:
	void GetLlanidList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetLlanidKeyList(CArray<T_LLANid_K, T_LLANid_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLlanid(CString& rLaneName);
	BOOL ExistLlanid(T_LLANid_K Key);
	BOOL GetLlanid(CString& rLaneName, T_LLANid_D& rData);
	BOOL GetLlanid(T_LLANid_K Key,T_LLANid_D& rData);
	int GetCountLlanid();
	POSITION GetStartLlanid();
	void GetNextLlanid(POSITION& rNextPosition,T_LLANid_K& rKey,T_LLANid_D& rData);
	T_LLANid_K GetLlanidKey(CString& rLaneName);
	T_LLANid_K GetStartNumLlanid();
	T_LLANid_K GetLastNumLlanid();

	// line lane for France version
public:
	void GetLlanfrList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetLlanfrKeyList(CArray<T_LLANfr_K, T_LLANfr_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLlanfr(CString& rLaneName);
	BOOL ExistLlanfr(T_LLANfr_K Key);
	BOOL GetLlanfr(CString& rLaneName, T_LLANfr_D& rData);
	BOOL GetLlanfr(T_LLANfr_K Key, T_LLANfr_D& rData);
	int GetCountLlanfr();
	POSITION GetStartLlanfr();
	void GetNextLlanfr(POSITION& rNextPosition, T_LLANfr_K& rKey, T_LLANfr_D& rData);
	T_LLANfr_K GetLlanfrKey(CString& rLaneName);
	T_LLANfr_K GetStartNumLlanfr();
	T_LLANfr_K GetLastNumLlanfr();

// line lane for transverse moving load
public:
	void GetLlantrList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetLlantrKeyList(CArray<T_LLANtr_K, T_LLANtr_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLlantr(CString& rLaneName);
	BOOL ExistLlantr(T_LLANtr_K Key);
	BOOL GetLlantr(CString& rLaneName, T_LLANtr_D& rData);
	BOOL GetLlantr(T_LLANtr_K Key,T_LLANtr_D& rData);
	int GetCountLlantr();
	POSITION GetStartLlantr();
	void GetNextLlantr(POSITION& rNextPosition,T_LLANtr_K& rKey,T_LLANtr_D& rData);
	T_LLANtr_K GetLlantrKey(CString& rLaneName);
	T_LLANtr_K GetStartNumLlantr();
	T_LLANtr_K GetLastNumLlantr();

// line lane Optimization
public:
	void GetLlanopList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetLlanopKeyList(CArray<T_LLANop_K, T_LLANop_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLlanop(CString& rLaneName);
	BOOL ExistLlanop(T_LLANop_K Key);
	BOOL GetLlanop(CString& rLaneName, T_LLANop_D& rData);
	BOOL GetLlanop(T_LLANop_K Key,T_LLANop_D& rData);
	int GetCountLlanop();
	POSITION GetStartLlanop();
	void GetNextLlanop(POSITION& rNextPosition,T_LLANop_K& rKey,T_LLANop_D& rData);
	T_LLANop_K GetLlanopKey(CString& rLaneName);
	T_LLANop_K GetStartNumLlanop();
	T_LLANop_K GetLastNumLlanop();
	void ConvertLlanoptoLlan(T_LLANop_D& LlanopD, double dOffset, T_LLAN_D& LlanD);
	void ConvertLlanoptoLlanch(T_LLANop_D& LlanopD, double dOffset, T_LLANch_D& LlanchD);

// surface lane
public:
	void GetSlanList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetSlanKeyList(CArray<T_SLAN_K, T_SLAN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSlan(CString& rLaneName);
	BOOL ExistSlan(T_SLAN_K Key);
	BOOL GetSlan(CString& rLaneName, T_SLAN_D& rData);
	BOOL GetSlan(T_SLAN_K Key,T_SLAN_D& rData);
	int GetCountSlan();
	POSITION GetStartSlan();
	void GetNextSlan(POSITION& rNextPosition,T_SLAN_K& rKey,T_SLAN_D& rData);
	T_SLAN_K GetSlanKey(CString& rLaneName);
	T_SLAN_K GetStartNumSlan();
	T_SLAN_K GetLastNumSlan();
	
// surface lane for Chinese version
public:
	void GetSlanchList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetSlanchKeyList(CArray<T_SLANch_K, T_SLANch_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSlanch(CString& rLaneName);
	BOOL ExistSlanch(T_SLANch_K Key);
	BOOL GetSlanch(CString& rLaneName, T_SLANch_D& rData);
	BOOL GetSlanch(T_SLANch_K Key,T_SLANch_D& rData);
	int GetCountSlanch();
	POSITION GetStartSlanch();
	void GetNextSlanch(POSITION& rNextPosition,T_SLANch_K& rKey,T_SLANch_D& rData);
	T_SLANch_K GetSlanchKey(CString& rLaneName);
	T_SLANch_K GetStartNumSlanch();
	T_SLANch_K GetLastNumSlanch();

// surface lane for France version
public:
	void GetSlanfrList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetSlanfrKeyList(CArray<T_SLANfr_K, T_SLANfr_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSlanfr(CString& rLaneName);
	BOOL ExistSlanfr(T_SLANfr_K Key);
	BOOL GetSlanfr(CString& rLaneName, T_SLANfr_D& rData);
	BOOL GetSlanfr(T_SLANfr_K Key, T_SLANfr_D& rData);
	int GetCountSlanfr();
	POSITION GetStartSlanfr();
	void GetNextSlanfr(POSITION& rNextPosition, T_SLANfr_K& rKey, T_SLANfr_D& rData);
	T_SLANfr_K GetSlanfrKey(CString& rLaneName);
	T_SLANfr_K GetStartNumSlanfr();
	T_SLANfr_K GetLastNumSlanfr();

// surface lane Optimization
public:
	void GetSlanopList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetSlanopKeyList(CArray<T_SLANop_K, T_SLANop_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSlanop(CString& rLaneName);
	BOOL ExistSlanop(T_SLANop_K Key);
	BOOL GetSlanop(CString& rLaneName, T_SLANop_D& rData);
	BOOL GetSlanop(T_SLANop_K Key,T_SLANop_D& rData);
	int GetCountSlanop();
	POSITION GetStartSlanop();
	void GetNextSlanop(POSITION& rNextPosition,T_SLANop_K& rKey,T_SLANop_D& rData);
	T_SLANop_K GetSlanopKey(CString& rLaneName);
	T_SLANop_K GetStartNumSlanop();
	T_SLANop_K GetLastNumSlanop();
	void ConvertSlanoptoSlan(T_SLANop_D& SlanopD, double dOffset, T_SLAN_D& SlanD);
	void ConvertSlanoptoSlanch(T_SLANop_D& SlanopD, double dOffset, T_SLANch_D& SlanchD);
	void ConvertSlanoptoSlanfr(T_SLANop_D& SlanopD, double dOffset, T_SLANfr_D& SlanfrD);

// Influence Surface
public:
	void GetSinfKeyList(CArray<T_SINF_K, T_SINF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSinf(T_SINF_K Key);
	BOOL GetSinf(T_SINF_K Key,T_SINF_D& rData);
	int GetCountSinf();
	POSITION GetStartSinf();
	void GetNextSinf(POSITION& rNextPosition,T_SINF_K& rKey,T_SINF_D& rData);

// Line Lane Support - Negative Moments at Interior Piers
public:
	void GetMlspKeyList(CArray<T_MLSP_K, T_MLSP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMlsp(T_MLSP_K Key);
	BOOL GetMlsp(T_MLSP_K Key,T_MLSP_D& rData);
	int GetCountMlsp();
	POSITION GetStartMlsp();
	void GetNextMlsp(POSITION& rNextPosition,T_MLSP_K& rKey,T_MLSP_D& rData);
	int  GetMlstKeyListByType(const int &nType, CArray<T_MLSP_K, T_MLSP_K>& rKeyList); // nType==0:Elem, 1:Grup

// Line Lane Support - Reactions at Interior Piers
public:
	void GetMlsrKeyList(CArray<T_MLSR_K, T_MLSR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMlsr(T_MLSR_K Key);
	BOOL GetMlsr(T_MLSR_K Key,T_MLSR_D& rData);
	int GetCountMlsr();
	POSITION GetStartMlsr();
	void GetNextMlsr(POSITION& rNextPosition,T_MLSR_K& rKey,T_MLSR_D& rData);

// Line Lane Support - Two Trucks
public:
	void GetMlttKeyList(CArray<T_MLTT_K, T_MLTT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMltt(T_MLTT_K Key);
	BOOL GetMltt(T_MLTT_K Key,T_MLTT_D& rData);
	int GetCountMltt();
	POSITION GetStartMltt();
	void GetNextMltt(POSITION& rNextPosition,T_MLTT_K& rKey,T_MLTT_D& rData);
	int  GetMlstKeyListByType(const int &nType, CArray<T_MLTT_K, T_MLTT_K>& rKeyList);

// Moving Load Code
public:
	BOOL ExistMvcd();
	BOOL GetMvcd(T_MVCD_D& rData);
	BOOL IsUsePlateConcurrentCode();

// Moving Load Control Data
public:
	BOOL ExistMvct();
	BOOL GetMvct(T_MVCT_D& rData);

// Moving Load Control Data(for japan version)
public:
	BOOL ExistMvctjp();
	BOOL GetMvctjp(T_MVCTjp_D& rData);

// Moving Load Control Data(for Chinese version)
public:
	BOOL ExistMvctch();
	BOOL GetMvctch(T_MVCTch_D& rData);

// Moving Load Control Data(for India version)
public:
	BOOL ExistMvctid();
	BOOL GetMvctid(T_MVCTid_D& rData);

// Moving Load Control Data(for BS version)
public:
	BOOL ExistMvctbs();
	BOOL GetMvctbs(T_MVCTbs_D& rData);

// Moving Load Control Data(for france version)
public:
	BOOL ExistMvctfr();
	BOOL GetMvctfr(T_MVCTfr_D& rData);

// Moving Load Control Data for transverse moving load
public:
	BOOL ExistMvcttr();
	BOOL GetMvcttr(T_MVCTtr_D& rData);

	BOOL IsMvCombindStressResult();

// Moving Load Vehicle Load
public:
	void GetMvhlList(CArray<CString, CString&>& rstrLoadList);  //Key 번호로 정렬된 리스트
	void GetMvhlKeyList(CArray<T_MVHL_K, T_MVHL_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMvhl(CString& rLoadName);
	BOOL ExistMvhl(T_MVHL_K Key);
	BOOL GetMvhl(CString& rLoadName, T_MVHL_D& rData);
	BOOL GetMvhl(T_MVHL_K Key,T_MVHL_D& rData);
	int GetCountMvhl();
	POSITION GetStartMvhl();
	void GetNextMvhl(POSITION& rNextPosition,T_MVHL_K& rKey,T_MVHL_D& rData);
	T_MVHL_K GetMvhlKey(const CString& rLoadName);
	T_MVHL_K GetStartNumMvhl();
	T_MVHL_K GetLastNumMvhl();
	BOOL IsPermitVehicle(T_MVHL_D& rData);
	BOOL IsBDoubleVehicle(T_MVHL_D& rData);
	BOOL IsWARatingVehicle(T_MVHL_D& rData);
	BOOL IsNZRatingVehicle(T_MVHL_D& rData);
	BOOL IsNZFatigueVehicle(T_MVHL_D& rData);
	BOOL IsPatchLoadVehicle(T_MVHL_D& rData);

// Vehicle Load for transverse moving load
public:
	void GetMvhltrList(CArray<CString, CString&>& rstrLoadList);  //Key 번호로 정렬된 리스트
	void GetMvhltrKeyList(CArray<T_MVHLtr_K, T_MVHLtr_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMvhltr(CString& rLoadName);
	BOOL ExistMvhltr(T_MVHLtr_K Key);
	BOOL GetMvhltr(CString& rLoadName, T_MVHLtr_D& rData);
	BOOL GetMvhltr(T_MVHLtr_K Key,T_MVHLtr_D& rData);
	int GetCountMvhltr();
	POSITION GetStartMvhltr();
	void GetNextMvhltr(POSITION& rNextPosition,T_MVHLtr_K& rKey,T_MVHLtr_D& rData);
	T_MVHLtr_K GetMvhltrKey(CString& rLoadName);
	T_MVHLtr_K GetStartNumMvhltr();
	T_MVHLtr_K GetLastNumMvhltr();

// Moving Load Vehicle Load Class
public:
	void GetMvhcList(CArray<CString, CString&>& rstrLoadList);  //Key 번호로 정렬된 리스트
	void GetMvhcKeyList(CArray<T_MVHC_K, T_MVHC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMvhc(CString& rLoadName);
	BOOL ExistMvhc(T_MVHC_K Key);
	BOOL GetMvhc(CString& rLoadName, T_MVHC_D& rData);
	BOOL GetMvhc(T_MVHC_K Key,T_MVHC_D& rData);
	int GetCountMvhc();
	POSITION GetStartMvhc();
	void GetNextMvhc(POSITION& rNextPosition,T_MVHC_K& rKey,T_MVHC_D& rData);
	T_MVHC_K GetMvhcKey(CString& rLoadName);
	T_MVHC_K GetStartNumMvhc();
	T_MVHC_K GetLastNumMvhc();

	// moving load case - 기준 공통 처리된 함수들
public:
	void GetMvldListCommon(CArray<CString, CString&>& rstrMvldList);  //serial 번호로 정렬된 리스트
	void GetMvldListCommon(CArray<T_MVLD_K, T_MVLD_K>& KeyList, CArray<CString, CString&>& rstrMvldList);  //serial 번호로 정렬된 리스트
	void GetMvldKeyListCommon(CArray<T_MVLD_K, T_MVLD_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMvldCommon(CString& rLoadCaseName);
	BOOL ExistMvldCommon(T_MVLD_K Key);
	T_MVLD_K GetMvldKeyCommon(CString& rLoadCaseName);
	void GetMvldNameCommon(T_MVLD_K Key, CString& rLoadCaseName);
	void GetMvldIDCommon(T_MVLD_K Key, int& rLoadCaseID);
	void GetExportMvldIndexCommon(T_MVLD_K Key, int& rExportIndex); // MINP 출력 순서와 동일한 순서로 Index 반환 (0부터 시작)

// moving load case
public:
		T_MVLD_K GetMvldKeyNew(CString& rLoadCaseName);
		void GetMvldNameNew(T_MVLD_K Key, CString& rLoadCaseName);
public:
	void GetMvldList(CArray<CString, CString&>& rstrMvldList);  //serial 번호로 정렬된 리스트
	void GetMvldKeyList(CArray<T_MVLD_K, T_MVLD_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMvld(CString& rLoadCaseName);
	BOOL ExistMvld(T_MVLD_K Key);
	BOOL GetMvld(CString& rLoadCaseName, T_MVLD_D& rData);
	BOOL GetMvld(T_MVLD_K Key, T_MVLD_D& rData);
	T_MVLD_K GetMvldKey(CString& rLoadCaseName);
	int GetCountMvld();
	POSITION GetStartMvld();
	void GetNextMvld(POSITION& rNextPosition,T_MVLD_K& rKey,T_MVLD_D& rData);
	T_MVLD_K GetStartNumMvld();
	T_MVLD_K GetLastNumMvld();

// moving load case for japan version
public:
	void InitializeMvldjp(T_MVLDjp_D& rData);
	void GetMvldjpList(CArray<CString, CString&>& rstrMvldjpList);  //serial 번호로 정렬된 리스트
	void GetMvldjpKeyList(CArray<T_MVLDjp_K, T_MVLDjp_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMvldjp(CString& rLoadCaseName);
	BOOL ExistMvldjp(T_MVLDjp_K Key);
	BOOL GetMvldjp(CString& rLoadCaseName, T_MVLDjp_D& rData);
	BOOL GetMvldjp(T_MVLDjp_K Key, T_MVLDjp_D& rData);
	T_MVLDjp_K GetMvldjpKey(CString& rLoadCaseName);
	int GetCountMvldjp();
	POSITION GetStartMvldjp();
	void GetNextMvldjp(POSITION& rNextPosition,T_MVLDjp_K& rKey,T_MVLDjp_D& rData);
	T_MVLDjp_K GetStartNumMvldjp();
	T_MVLDjp_K GetLastNumMvldjp();

// moving load case for Chinese version
public:  
	void GetMvldchList(CArray<CString, CString&>& rstrMvldList);  //serial 번호로 정렬된 리스트
	void GetMvldchKeyList(CArray<T_MVLDch_K, T_MVLDch_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMvldch(CString& rLoadCaseName);
	BOOL ExistMvldch(T_MVLDch_K Key);
	BOOL GetMvldch(CString& rLoadCaseName, T_MVLDch_D& rData);
	BOOL GetMvldch(T_MVLDch_K Key, T_MVLDch_D& rData);
	T_MVLDch_K GetMvldchKey(CString& rLoadCaseName);
	int GetCountMvldch();
	POSITION GetStartMvldch();
	void GetNextMvldch(POSITION& rNextPosition,T_MVLDch_K& rKey,T_MVLDch_D& rData);
	T_MVLDch_K GetStartNumMvldch();
	T_MVLDch_K GetLastNumMvldch();

	BOOL IsFatigueMvld(T_MVLDch_K Key);   // Dlg, Load Combination 에서 사용
	BOOL IsFatigueMvld(T_MVLDch_D& Data); // Dlg, Load Combination 에서 사용
	BOOL IsGeneralMvld(T_MVLDch_D& Data); // Dlg  에서 사용

// moving load case for India version
public:  
	void GetMvldidList(CArray<CString, CString&>& rstrMvldList);  //serial 번호로 정렬된 리스트
	void GetMvldidKeyList(CArray<T_MVLDid_K, T_MVLDid_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMvldid(CString& rLoadCaseName);
	BOOL ExistMvldid(T_MVLDid_K Key);
	BOOL GetMvldid(CString& rLoadCaseName, T_MVLDid_D& rData);
	BOOL GetMvldid(T_MVLDid_K Key, T_MVLDid_D& rData);
	T_MVLDid_K GetMvldidKey(CString& rLoadCaseName);
	int GetCountMvldid();
	POSITION GetStartMvldid();
	void GetNextMvldid(POSITION& rNextPosition,T_MVLDid_K& rKey,T_MVLDid_D& rData);
	T_MVLDid_K GetStartNumMvldid();
	T_MVLDid_K GetLastNumMvldid();

// moving load case for BS version
public:  
	void GetMvldbsList(CArray<CString, CString&>& rstrMvldList);  //serial 번호로 정렬된 리스트
	void GetMvldbsKeyList(CArray<T_MVLDbs_K, T_MVLDbs_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMvldbs(CString& rLoadCaseName);
	BOOL ExistMvldbs(T_MVLDbs_K Key);
	BOOL GetMvldbs(CString& rLoadCaseName, T_MVLDbs_D& rData);
	BOOL GetMvldbs(T_MVLDbs_K Key, T_MVLDbs_D& rData);
	T_MVLDbs_K GetMvldbsKey(CString& rLoadCaseName);
	int GetCountMvldbs();
	POSITION GetStartMvldbs();
	void GetNextMvldbs(POSITION& rNextPosition,T_MVLDbs_K& rKey,T_MVLDbs_D& rData);
	T_MVLDbs_K GetStartNumMvldbs();
	T_MVLDbs_K GetLastNumMvldbs();

// moving load case for France version
public:
	void GetMvldfrList(CArray<CString, CString&>& rstrMvldList);  //serial 번호로 정렬된 리스트
	void GetMvldfrKeyList(CArray<T_MVLDfr_K, T_MVLDfr_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMvldfr(CString& rLoadCaseName);
	BOOL ExistMvldfr(T_MVLDfr_K Key);
	BOOL GetMvldfr(CString& rLoadCaseName, T_MVLDfr_D& rData);
	BOOL GetMvldfr(T_MVLDfr_K Key, T_MVLDfr_D& rData);
	T_MVLDfr_K GetMvldfrKey(CString& rLoadCaseName);
	int GetCountMvldfr();
	POSITION GetStartMvldfr();
	void GetNextMvldfr(POSITION& rNextPosition, T_MVLDfr_K& rKey, T_MVLDfr_D& rData);
	T_MVLDfr_K GetStartNumMvldfr();
	T_MVLDfr_K GetLastNumMvldfr();

// moving load case for transverse moving load
public:
	void GetMvldtrList(CArray<CString, CString&>& rstrMvldList);  //serial 번호로 정렬된 리스트
	void GetMvldtrKeyList(CArray<T_MVLDtr_K, T_MVLDtr_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistMvldtr(CString& rLoadCaseName);
	BOOL ExistMvldtr(T_MVLDtr_K Key);
	BOOL GetMvldtr(CString& rLoadCaseName, T_MVLDtr_D& rData);
	BOOL GetMvldtr(T_MVLDtr_K Key, T_MVLDtr_D& rData);
	T_MVLDtr_K GetMvldtrKey(CString& rLoadCaseName);
	int GetCountMvldtr();
	POSITION GetStartMvldtr();
	void GetNextMvldtr(POSITION& rNextPosition,T_MVLDtr_K& rKey,T_MVLDtr_D& rData);
	T_MVLDtr_K GetStartNumMvldtr();
	T_MVLDtr_K GetLastNumMvldtr();

// load combination
protected:
	CDB_LCOM* m_arLcomMaps[D_LCOMTYPE_MAX + 1];
	void InitLcomMaps();
public:
	void GetLcomList(int nLcomType, CArray<CString, CString&>& rstrLcomList);  //serial 번호로 정렬된 리스트
	void GetLcomKeyList(int nLcomType, CArray<T_LCOM_K, T_LCOM_K>& rKeyList);  //serial 번호로 정렬된 리스트
	void GetLcomKeyListOnlySgld(int nLcomType, CArray<T_LCOM_K, T_LCOM_K>& rKeyList);  // 시공하중(Sgld)로만 구성된 Combination
	BOOL ExistLcom(int nLcomType, CString& rLoadCombName);
	BOOL ExistLcom(int nLcomType, T_LCOM_K Key);
	BOOL GetLcom(int nLcomType, CString& rLoadCombName, T_LCOM_D& rData);
	BOOL GetLcom(int nLcomType, T_LCOM_K Key, T_LCOM_D& rData);
//  BOOL GetLcom4Dgn(int nLcomType, T_LCOM_K Key, T_LCOM_D& rData);
	
	T_LCOM_K GetLcomKey(int nLcomType, CString& rLoadCombName);
	int GetCountLcom(int nLcomType);
	int GetCountLcomFromBaseDB(int nLcomType);
	int GetCountLcomCase(int nLcomType, T_LCOM_D& rData);   // 한개의 하중조합에 포함된 하중조건의 수
	int GetCountLcomCase(int nLcomType, CString& rLoadCombName);   // 한개의 하중조합에 포함된 하중조건의 수
	int GetCountLcomCase(int nLcomType, T_LCOM_K Key);             // 한개의 하중조합에 포함된 하중조건의 수
	POSITION GetStartLcom(int nLcomType);
	void GetNextLcom(int nLcomType, POSITION& rNextPosition,T_LCOM_K& rKey,T_LCOM_D& rData);
	T_LCOM_K GetStartNumLcom(int nLcomType);
	T_LCOM_K GetLastNumLcom(int nLcomType);
	BOOL IsMaxMinComb(T_LCOM_D& rData);
	BOOL IsCraneComb(T_LCOM_D& rData);  // Add by GAY. MNET:2683. ('07.05.15). Crane Load가 포함된 조합인가?
	BOOL IsConCurrentComb12(T_LCOM_D& rData); // Add by GAY. MNET:XXXX. ('07.06.05). 동시발생 부재력 Case12를 고려해야 하는 하중조합인가?
	BOOL IsEnvlopeLcom(T_LCOM_D& rData);
	BOOL IncludeEnvlopeLcom(T_LCOM_D& rData);
	int  Get_CombType(unsigned int nLcaseKey, int nAnalType);
	BOOL IsOnlySgldComb(int nLcomType, T_LCOM_D& rData);
	BOOL NormalizeLcom(const T_LCOM_D& rData, T_LCOM_D_UL& rNormLcom, BOOL bOnlyAdd=TRUE);
	int  GetLcomRSESOnlyCombined(const int &nLcomType, const T_SPLC_K &SplcK, CArray<T_LCOM_K, T_LCOM_K> &raKeyList);
	CString GetActivName4MDL(int nActive);
	BOOL CheckAndDeleteEccenSpecLCase(T_LCOM_D& rData);

	// 국내 내진
	void GetSeizEvalLcomType(T_LCOM_D& LcomD, int& nRs, int& nTh);

protected:
	BOOL NormalizeCombination(T_LCOM_D_UL& rData);
	int  FindCasePos(T_LCOM_D_UL& rData, int nAnalType, int nLoadCaseKey);

// envelope
public:
	void GetEnvlList(CArray<CString, CString&>& rstrEnvlList);  //serial 번호로 정렬된 리스트
	void GetEnvlKeyList(CArray<T_ENVL_K, T_ENVL_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistEnvl(CString& rEnvelopeName);
	BOOL ExistEnvl(T_ENVL_K Key);
	BOOL GetEnvl(CString& rEnvelopeName, T_ENVL_D& rData);
	BOOL GetEnvl(T_ENVL_K Key, T_ENVL_D& rData);
	T_ENVL_K GetEnvlKey(CString& rEnvelopeName);
	int GetCountEnvl();
	POSITION GetStartEnvl();
	void GetNextEnvl(POSITION& rNextPosition,T_ENVL_K& rKey,T_ENVL_D& rData);
	T_ENVL_K GetStartNumEnvl();
	T_ENVL_K GetLastNumEnvl();

// Load Contribution for NL Load Case
public:  
	void GetLctbList(CArray<CString, CString&>& rstrLctbList);  //serial 번호로 정렬된 리스트
	void GetLctbKeyList(CArray<T_LCTB_K, T_LCTB_K>& rKeyList);  //serial 번호로 정렬된 리스트  
	BOOL ExistLctb(CString& rLctbName);
	BOOL ExistLctb(T_LCTB_K Key);  
	BOOL GetLctb(CString& rLctbName, T_LCTB_D& rData);
	BOOL GetLctb(T_LCTB_K Key, T_LCTB_D& rData);  
	BOOL GetLctbKeyFromStldKey(T_STLD_K StldKey, T_LCTB_K& LctbKey);
	T_LCTB_K GetLctbKey(CString& rLctbName);
	int GetCountLctb();
	POSITION GetStartLctb();
	void GetNextLctb(POSITION& rNextPosition,T_LCTB_K& rKey,T_LCTB_D& rData);
	T_LCTB_K GetStartNumLctb();
	T_LCTB_K GetLastNumLctb();

// Load to mass
public:
	BOOL ExistLtom();
	BOOL GetLtom(T_LTOM_D& rData);

// Grid Mass Converter
public:
	BOOL ExistGltm();
	BOOL GetGltm(T_GLTM_D& rData);
//  BOOL MakeGridMassData(T_GLTM_D& DataGltm, CArray<T_GRIDINFO_MASSLOAD, T_GRIDINFO_MASSLOAD&>& aGridNodalLoad);

// M-Phi Global Data
public:
	BOOL ExistMpgb();
	BOOL GetMpgb(T_MPGB_D& rData);  

// body force
public:
    void GetBodfKeyList(CArray<T_BODF_K, T_BODF_K>& rKeyList);
	BOOL MakeKeyBodf(T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_BODF_K& rKey);
	BOOL ExistBodf(T_BODF_K Key);
	BOOL ExistBodf(T_STLD_K StldKey, T_LDGR_K LdgrKey);
	BOOL GetBodf(T_BODF_K Key,T_BODF_D& rData);
	BOOL GetBodf(T_STLD_K StldKey, CArray<T_BODF_K, T_BODF_K>& rKey, CArray<T_BODF_D, T_BODF_D&>& rData);
	BOOL GetBodf(T_STLD_K StldKey, T_BODF_K& rKey, T_BODF_D& rData);
	BOOL GetBodf(T_STLD_K StldKey, T_LDGR_K LdgrKey, T_BODF_K& rKey, T_BODF_D& rData);
	int GetCountBodf();
	int GetCountBodf(T_STLD_K StldKey);
	POSITION GetStartBodf();
	void GetNextBodf(POSITION& rNextPosition,T_BODF_K& rKey,T_BODF_D& rData);
	T_BODF_K GetStartNumBodf();

// nodal body force
public:
	void GetNbofKeyList(CArray<T_NBOF_K, T_NBOF_K>& rKeyList);
	BOOL MakeKeyNbof(T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_NBOF_K& rKey);
	BOOL ExistNbof(T_NBOF_K Key);
	BOOL ExistNbof(T_STLD_K StldKey, T_LDGR_K LdgrKey);
	BOOL GetNbof(T_NBOF_K Key,T_NBOF_D& rData);
	BOOL GetNbof(T_STLD_K StldKey, CArray<T_NBOF_K, T_NBOF_K>& rKey, CArray<T_NBOF_D, T_NBOF_D&>& rData);
	BOOL GetNbof(T_STLD_K StldKey, T_NBOF_K& rKey, T_NBOF_D& rData);
	BOOL GetNbof(T_STLD_K StldKey, T_LDGR_K LdgrKey, T_NBOF_K& rKey, T_NBOF_D& rData);
	int GetCountNbof();
	int GetCountNbof(T_STLD_K StldKey);
	POSITION GetStartNbof();
	void GetNextNbof(POSITION& rNextPosition,T_NBOF_K& rKey,T_NBOF_D& rData);
	T_NBOF_K GetStartNumNbof();  
	void CalcNbofLoad(T_NBOF_K Key,T_NBOF_D& rData, CArray<T_CNLD_UDRD_D, T_CNLD_UDRD_D&>& arNbofLoad);
	void CalcNbofLoadLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, T_NBOF_K KeyNbof, CArray<T_CNLD_UDRD_D, T_CNLD_UDRD_D&>& arNbofLoad);

// conload
public:
	void GetCnldKeyList(CArray<T_CNLD_K, T_CNLD_K>& rKeyList);  // 정렬 없음
	BOOL MakeKeyCnld(T_NODE_K KeyNode,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_CNLD_K& rKey);
	BOOL ExistCnld(T_CNLD_K Key);
	BOOL GetCnld(T_CNLD_K Key,T_CNLD_D& rData);
	void GetCnld(T_NODE_K KeyNode, CArray<T_CNLD_K, T_CNLD_K>& rarKey, CArray<T_CNLD_D, T_CNLD_D&>& rarData);
	BOOL GetCnldLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_NODE_K KeyNode, T_CNLD_D& DataCnld);
	int GetCountCnld();
	POSITION GetStartCnld();
	void GetNextCnld(POSITION& rNextPosition,T_CNLD_K& rKey,T_CNLD_D& rData);
	//
	BOOL GetCnldTypeList(T_STLD_K KeyStld,CArray<T_CNLD_D,T_CNLD_D&>& aData,CArray<CArrayEx<T_CNLD_KEY,T_CNLD_KEY&>, CArrayEx<T_CNLD_KEY,T_CNLD_KEY&>&>& aArrCnldK);
	//void GetElemListByCnldType(double Force[6],T_STLD_K KeyStld,CString& strTypeName,CArray<T_NODE_K, T_NODE_K>& aKey);
	BOOL GetCnldType(const CString& strName,CArray<T_CNLD_KEY, T_CNLD_KEY&>& pKeyList);
	int	 GetCnldTypeCount();
protected:
	BOOL GetSerialCnld(T_NODE_K KeyNode,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_CNLD_KEY& rKeySerial);

// specified displacement
public:
	void GetSdspKeyList(CArray<T_SDSP_K, T_SDSP_K>& rKeyList);
	BOOL MakeKeySdsp(T_NODE_K KeyNode,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_SDSP_K& rKey);
	BOOL ExistSdsp(T_SDSP_K Key);
	BOOL GetSdsp(T_SDSP_K Key,T_SDSP_D& rData);
	void GetSdsp(T_NODE_K KeyNode, CArray<T_SDSP_K, T_SDSP_K>& rarKey, CArray<T_SDSP_D, T_SDSP_D&>& rarData);
	BOOL GetSdspLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_NODE_K KeyNode, T_SDSP_D& DataSdsp);
	int GetCountSdsp();
	POSITION GetStartSdsp();
	void GetNextSdsp(POSITION& rNextPosition,T_SDSP_K& rKey,T_SDSP_D& rData);
protected:
	BOOL GetSerialSdsp(T_NODE_K KeyNode,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_SDSP_KEY& rKeySerial);

// beam load
public:
	void GetBmldKeyList(CArray<T_BMLD_K, T_BMLD_K>& rKeyList);  // 정렬 없음
	BOOL MakeKeyBmld(T_ELEM_K Key,T_BMLD_K& rKey, BOOL bUseTempStore=FALSE);
	BOOL ExistBmld(T_BMLD_K Key);
	BOOL GetBmld(T_BMLD_K Key,T_BMLD_D& rData);
	void GetBmld(T_ELEM_K KeyElem, CArray<T_BMLD_K, T_BMLD_K>& rarKey, CArray<T_BMLD_D, T_BMLD_D&>& rarData);
	void GetBmldLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, CArray<T_BMLD_D, T_BMLD_D&>& rarData);
	int GetCountBmld();
	POSITION GetStartBmld();
	void GetNextBmld(POSITION& rNextPosition,T_BMLD_K& rKey,T_BMLD_D& rData);
	void ConvertEccToLocalMoment(T_BMLD_UDRD_D& Bmld, CArray<T_BMLD_UDRD_D, T_BMLD_UDRD_D&>& arBeamLoad);
	int  ConvertEccToLocalMoment_from7thDOF(CArray<T_BMLD_UDRD_D, T_BMLD_UDRD_D&>& raBeamLoad, BOOL bConsiderSectOffset, BOOL bBeforeComposite);
	BOOL ConvertBmldToGCS(T_BMLD_UDRD_D& rBeamLoad, double& rx, double& ry, double& rz,
		BOOL bConsiderSectOffset, BOOL bBeforeComposite, BOOL bIsCS = FALSE);
	BOOL CalcTaperedSectGroupPosValue(double& rdRetValY, double& rdRetValZ,
		double dValY1, double dValZ1, double dValY2, double dValZ2,
		const T_SECT_D& DataSect, T_TSGR_D& DataTsgr,
		double dTotalLength, double dPosition);
	double GetInterpolateValue(const double dVal1, const double dVal2, const double dPosRatio);
	BOOL AdjustLocalAxisByElemLocal(const double dLocalVector1[3][3], double dLocalVector2[3][3]);
	void Consider_I_J_DifferentEcc(T_BMLD_K KeyBmld, T_BMLD_D& Bmld);
	void ConvertEccOffsetToLocalMoment(T_BMLD_UDRD_D& Bmld, CArray<T_BMLD_UDRD_D, T_BMLD_UDRD_D&>& arBeamLoad, BOOL bPressure);
	void ConvertEccOffsetToCentroid(T_BMLD_UDRD_D& Bmld);
protected:
	BOOL GetSerialBmld(T_ELEM_K Key,T_BMLD_KEY& rKeySerial);

// finishing material load
public:
	void GetFmldKeyList(CArray<T_FMLD_K, T_FMLD_K>& rKeyList);  // 정렬 없음
	BOOL MakeKeyFmld(T_ELEM_K Key,T_FMLD_K& rKey);
	BOOL ExistFmld(T_FMLD_K Key);
	BOOL GetFmld(T_FMLD_K Key,T_FMLD_D& rData);
	void GetFmld(T_ELEM_K KeyElem, CArray<T_FMLD_K, T_FMLD_K>& rarKey, CArray<T_FMLD_D, T_FMLD_D&>& rarData);
	void GetFmldLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, CArray<T_FMLD_D, T_FMLD_D&>& rarData);
	int GetCountFmld();
	POSITION GetStartFmld();
	void GetNextFmld(POSITION& rNextPosition,T_FMLD_K& rKey,T_FMLD_D& rData);
protected:
	BOOL GetSerialFmld(T_ELEM_K Key,T_FMLD_KEY& rKeySerial);

// floor load define
public:
	void GetFbldList(CArray<CString, CString&>& rstrFbldList);  //serial 번호로 정렬된 리스트
	void GetFbldKeyList(CArray<T_FBLD_K, T_FBLD_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistFbld(CString& rFloorLoadTypeName);
	BOOL ExistFbld(T_FBLD_K Key);
	BOOL GetFbld(CString& rFloorLoadTypeName, T_FBLD_D& rData);
	BOOL GetFbld(T_FBLD_K Key, T_FBLD_D& rData);
	T_FBLD_K GetFbldKey(CString& rFloorLoadTypeName);
	int GetCountFbld();
	int GetCountFbldCase(CString& rFloorLoadTypeName);   // 한개의 Fbld에 포함된 하중조건의 수
	int GetCountFbldCase(T_LCOM_K Key);             // 한개의 Fbld에 포함된 하중조건의 수
	POSITION GetStartFbld();
	void GetNextFbld(POSITION& rNextPosition,T_FBLD_K& rKey,T_FBLD_D& rData);
	T_FBLD_K GetStartNumFbld();
	T_FBLD_K GetLastNumFbld();

// floor load assign
public:
	void GetFblaKeyList(CArray<T_FBLA_K, T_FBLA_K>& rKeyList);
	BOOL ExistFbla(T_FBLA_K Key);
	BOOL GetFbla(T_FBLA_K Key,T_FBLA_D& rData);
	void GetFbla(T_NODE_K KeyNode, CArray<T_FBLA_K, T_FBLA_K>& rarKey, CArray<T_FBLA_D, T_FBLA_D&>& rarData);
	int GetCountFbla();
	POSITION GetStartFbla();
	void GetNextFbla(POSITION& rNextPosition,T_FBLA_K& rKey,T_FBLA_D& rData);
	T_FBLA_K GetStartNumFbla();
	BOOL IsActiveFbla(CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>& mapActiveElem, T_FBLA_D& rData);

// set back for construction stage
public:
	BOOL ExistStbk(T_STBK_K Key);
	BOOL GetStbk(T_STBK_K Key,T_STBK_D& rData);
	void GetStbk(T_NODE_K KeyNode, CArray<T_STBK_K, T_STBK_K>& rarKey, CArray<T_STBK_D, T_STBK_D&>& rarData);

	void GetStbkKeyList(CArray<T_STBK_K, T_STBK_K>& rKeyList);  //Key 번호로 정렬된 리스트
	int  GetStbkList(CArray<T_STBK_K, T_STBK_K>& raKey, CArray<T_STBK_D, T_STBK_D&>& raData);

	int GetCountStbk();
	POSITION GetStartStbk();
	void GetNextStbk(POSITION& rNextPosition,T_STBK_K& rKey,T_STBK_D& rData);
	T_STBK_K GetStartNumStbk();
	BOOL MakeKeyStbk(T_NODE_K KeyNode1, T_NODE_K KeyNode2, T_LDGR_K KeyLdgr, T_STBK_K& rKey);

// Unstrained Length of Cable for Nonlinear Construction Stage
public:
	BOOL ExistUlcb(T_ULCB_K Key);
	BOOL GetUlcb(T_ULCB_K Key,T_ULCB_D& rData);
	void GetUlcb(T_ELEM_K KeyElem, CArray<T_ULCB_K, T_ULCB_K>& rarKey, CArray<T_ULCB_D, T_ULCB_D&>& rarData);
	void GetUlcbKeyList(CArray<T_ULCB_K, T_ULCB_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetUlcbKeyList(T_ELEM_K KeyElem, CArray<T_ULCB_K, T_ULCB_K>& rKeyList);
	int GetCountUlcb();
	POSITION GetStartUlcb();
	void GetNextUlcb(POSITION& rNextPosition,T_ULCB_K& rKey,T_ULCB_D& rData);
	T_ULCB_K GetStartNumUlcb();
	BOOL MakeKeyUlcb(T_ELEM_K KeyElem, T_LDGR_K KeyLdgr, T_ULCB_K& rKey);

// pressure load
public:
	void GetPresKeyList(CArray<T_PRES_K, T_PRES_K>& rKeyList);   //Key 번호로 정렬된 리스트
	BOOL MakeKeyPres(T_ELEM_K Key,T_PRES_K& rKey);
	BOOL ExistPres(T_PRES_K Key);
	BOOL GetPres(T_PRES_K Key,T_PRES_D& rData);
	void GetPres(T_ELEM_K KeyElem, CArray<T_PRES_K, T_PRES_K>& rarKey, CArray<T_PRES_D, T_PRES_D&>& rarData);
	void GetPresLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, CArray<T_PRES_D, T_PRES_D&>& rarData);
	int GetCountPres();
	POSITION GetStartPres();
	void GetNextPres(POSITION& rNextPosition,T_PRES_K& rKey,T_PRES_D& rData);
	void MakeProjectionPres(T_PRES_K Key, T_PRES_D Data, T_PRES_D& rDataNew);
	BOOL ConvertPresToLocalPoint(T_ELEM_K keyElem, double dGX, double dGY, double dGZ, double& px, double& py);
protected:
	BOOL GetSerialPres(T_ELEM_K Key,T_PRES_KEY& rKeySerial);

// prestress load
public:
    void GetPrstKeyList(CArray<T_PRST_K, T_PRST_K>& rKeyList);   //Key 번호로 정렬된 리스트
	BOOL MakeKeyPrst(T_ELEM_K Key,T_PRST_K& rKey);
	BOOL ExistPrst(T_PRST_K Key);
	BOOL GetPrst(T_PRST_K Key,T_PRST_D& rData);
	void GetPrst(T_ELEM_K KeyElem, CArray<T_PRST_K, T_PRST_K>& rarKey, CArray<T_PRST_D, T_PRST_D&>& rarData);
	void GetPrstLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, CArray<T_PRST_D, T_PRST_D&>& rarData);
	int GetCountPrst();
	POSITION GetStartPrst();
	void GetNextPrst(POSITION& rNextPosition,T_PRST_K& rKey,T_PRST_D& rData);
	void MakePrestress(T_PRST_K Key,T_PRST_D& rData, CArray<T_NODE_K, T_NODE_K>& arNodeKey, CArray<T_CNLD_D, T_CNLD_D&>& arCnldData, CArray<T_BMLD_D, T_BMLD_D&>& arBmldData);
protected:
	BOOL GetSerialPrst(T_ELEM_K Key,T_PRST_KEY& rKeySerial);

// pretension load
public:
	BOOL MakeKeyPtns(T_ELEM_K Key,T_PTNS_K& rKey);
    void GetPtnsKeyList(CArray<T_PTNS_K, T_PTNS_K>& rKeyList);
	BOOL ExistPtns(T_PTNS_K Key);
	BOOL GetPtns(T_PTNS_K Key,T_PTNS_D& rData);
	void GetPtns(T_ELEM_K KeyElem, CArray<T_PTNS_K, T_PTNS_K>& rarKey, CArray<T_PTNS_D, T_PTNS_D&>& rarData);
	void GetPtnsLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, CArray<T_PTNS_D, T_PTNS_D&>& rarData);
	int GetCountPtns();
	POSITION GetStartPtns();
	void GetNextPtns(POSITION& rNextPosition,T_PTNS_K& rKey,T_PTNS_D& rData);
protected:
	BOOL GetSerialPtns(T_ELEM_K Key,T_PTNS_KEY& rKeySerial);

// system temperature
public:
	BOOL MakeKeyStmp(T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_STMP_K& rKey);
    void GetStmpKeyList(CArray<T_STMP_K, T_STMP_K>& rKeyList);
	BOOL ExistStmp(T_STMP_K Key);
	BOOL ExistStmp(T_STLD_K StldKey, T_LDGR_K LdgrKey);
	BOOL GetStmp(T_STMP_K Key,T_STMP_D& rData);
	BOOL GetStmp(T_STLD_K StldKey, CArray<T_STMP_K, T_STMP_K>& rKey, CArray<T_STMP_D, T_STMP_D&>& rData);
	BOOL GetStmp(T_STLD_K StldKey, T_STMP_K& rKey, T_STMP_D& rData);
	BOOL GetStmp(T_STLD_K StldKey, T_LDGR_K LdgrKey, T_STMP_K& rKey, T_STMP_D& rData);
	int GetCountStmp();
	int GetCountStmp(T_STLD_K StldKey);
	POSITION GetStartStmp();
	void GetNextStmp(POSITION& rNextPosition,T_STMP_K& rKey,T_STMP_D& rData);
	T_STMP_K GetStartNumStmp();

// nodal temperature
public:
	BOOL MakeKeyNtmp(T_NODE_K KeyNode,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_NTMP_K& rKey);
	BOOL ExistNtmp(T_NTMP_K Key);
	BOOL GetNtmp(T_NTMP_K Key,T_NTMP_D& rData);
	void GetNtmp(T_NODE_K KeyNode, CArray<T_NTMP_K, T_NTMP_K>& rarKey, CArray<T_NTMP_D, T_NTMP_D&>& rarData);
	BOOL GetNtmpLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_NODE_K KeyNode, T_NTMP_D& DataNtmp);
	int GetCountNtmp();
	POSITION GetStartNtmp();
	void GetNextNtmp(POSITION& rNextPosition,T_NTMP_K& rKey,T_NTMP_D& rData);
    void GetNtmpKeyList(CArray<T_NTMP_K, T_NTMP_K>& rKeyList);
protected:
	BOOL GetSerialNtmp(T_NODE_K KeyNode,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_NTMP_KEY& rKeySerial);

// element temperature
public:
    void GetEtmpKeyList(CArray<T_ETMP_K, T_ETMP_K>& rKeyList);   //Key 번호로 정렬된 리스트
	BOOL MakeKeyEtmp(T_ELEM_K KeyElem,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_ETMP_K& rKey);
	BOOL ExistEtmp(T_ETMP_K Key);
	BOOL GetEtmp(T_ETMP_K Key,T_ETMP_D& rData);
	void GetEtmp(T_ELEM_K KeyElem, CArray<T_ETMP_K, T_ETMP_K>& rarKey, CArray<T_ETMP_D, T_ETMP_D&>& rarData);
	BOOL GetEtmpLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, T_ETMP_D& DataEtmp);
	int GetCountEtmp();
	POSITION GetStartEtmp();
	void GetNextEtmp(POSITION& rNextPosition,T_ETMP_K& rKey,T_ETMP_D& rData);
	int GetElemFire(T_ELEM_K_LIST& aKey);
	BOOL IsExistFire(const T_ELEM_K_LIST& aKey);
	BOOL GetEtmpKey(T_ELEM_K KeyElem,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_ETMP_KEY& rKeySerial);
protected:
	BOOL GetSerialEtmp(T_ELEM_K KeyElem,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_ETMP_KEY& rKeySerial);

// beam section temperature
public:
    void GetBtmpKeyList(CArray<T_BTMP_K, T_BTMP_K>& rKeyList);
	BOOL MakeKeyBtmp(T_ELEM_K Key,T_BTMP_K& rKey);
	BOOL ExistBtmp(T_BTMP_K Key);
	BOOL GetBtmp(T_BTMP_K Key,T_BTMP_D& rData);
	void GetBtmp(T_ELEM_K KeyElem, CArray<T_BTMP_K, T_BTMP_K>& rarKey, CArray<T_BTMP_D, T_BTMP_D&>& rarData);
	void GetBtmpLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, CArray<T_BTMP_D, T_BTMP_D&>& rarData);
	int GetCountBtmp();
	POSITION GetStartBtmp();
	void GetNextBtmp(POSITION& rNextPosition,T_BTMP_K& rKey,T_BTMP_D& rData);
	BOOL ConvertBtmp4Label(T_BTMP_K Key, T_BTMP_D& rData);  // h1, h2를 Centroid로부터 거리로 바꾸어 주는 함수
protected:
	BOOL GetSerialBtmp(T_ELEM_K Key,T_BTMP_KEY& rKeySerial);

	// Plate Thickness  temperature
public:
	void GetPtmpKeyList(CArray<T_PTMP_K, T_PTMP_K>& rKeyList);
	BOOL MakeKeyPtmp(T_ELEM_K Key, T_PTMP_K& rKey);
	BOOL ExistPtmp(T_PTMP_K Key);
	BOOL GetPtmp(T_PTMP_K Key, T_PTMP_D& rData);
	void GetPtmp(T_ELEM_K KeyElem, CArray<T_PTMP_K, T_PTMP_K>& rarKey, CArray<T_PTMP_D, T_PTMP_D&>& rarData);
	void GetPtmpLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, CArray<T_PTMP_D, T_PTMP_D&>& rarData);
	int GetCountPtmp();
	POSITION GetStartPtmp();
	void GetNextPtmp(POSITION& rNextPosition, T_PTMP_K& rKey, T_PTMP_D& rData);
	BOOL ConvertPtmp4Label(T_PTMP_K Key, T_PTMP_D& rData);  // h1, h2를 Centroid로부터 거리로 바꾸어 주는 함수
	BOOL CheckAllPtmpByElem(T_ELEM_K KeyElem, T_ELEM_D DataElem, BOOL bDelete = FALSE);
	BOOL CheckAllPtmpByThick(T_THIK_K KeyThik, T_THIK_D DataThik, BOOL bDelete=FALSE);
	BOOL CheckAllPtmpByMatl(T_MATL_K KeyMatl, T_MATL_D DataMatl, BOOL bDelete=FALSE);
protected:
	BOOL GetSerialPtmp(T_ELEM_K Key, T_PTMP_KEY& rKeySerial);

// temperature gradient
public:
    void GetGtmpKeyList(CArray<T_GTMP_K, T_GTMP_K>& rKeyList);   //Key 번호로 정렬된 리스트
	BOOL MakeKeyGtmp(T_ELEM_K Key,T_GTMP_K& rKey);
	BOOL ExistGtmp(T_GTMP_K Key);
	BOOL GetGtmp(T_GTMP_K Key,T_GTMP_D& rData);
	void GetGtmp(T_ELEM_K KeyElem, CArray<T_GTMP_K, T_GTMP_K>& rarKey, CArray<T_GTMP_D, T_GTMP_D&>& rarData);
	void GetGtmpLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_ELEM_K KeyElem, CArray<T_GTMP_D, T_GTMP_D&>& rarData);
	int GetCountGtmp();
	POSITION GetStartGtmp();
	void GetNextGtmp(POSITION& rNextPosition,T_GTMP_K& rKey,T_GTMP_D& rData);
protected:
	BOOL GetSerialGtmp(T_ELEM_K Key,T_GTMP_KEY& rKeySerial);

// plane load type
public:
	void GetPnldList(CArray<CString, CString&>& rstrPnldList);  //serial 번호로 정렬된 리스트
	void GetPnldKeyList(CArray<T_PNLD_K, T_PNLD_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistPnld(CString& rLoadTypeName);
	BOOL ExistPnld(T_PNLD_K Key);
	BOOL GetPnld(CString& rLoadTypeName, T_PNLD_D& rData);
	BOOL GetPnld(T_PNLD_K Key, T_PNLD_D& rData);
	T_PNLD_K GetPnldKey(CString& rLoadTypeName);
	int GetCountPnld();
	POSITION GetStartPnld();
	void GetNextPnld(POSITION& rNextPosition,T_PNLD_K& rKey,T_PNLD_D& rData);
	T_PNLD_K GetStartNumPnld();
	T_PNLD_K GetLastNumPnld();

// crane load type
public:
	void GetCranList(CArray<CString, CString&>& rstrLaneList);  //Key 번호로 정렬된 리스트
	void GetCranKeyList(CArray<T_CRAN_K, T_CRAN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCran(CString& rLaneName);
	BOOL ExistCran(T_CRAN_K Key);
	BOOL GetCran(CString& rLaneName, T_CRAN_D& rData);
	BOOL GetCran(T_CRAN_K Key,T_CRAN_D& rData);
	int GetCountCran();
	BOOL IsExistCranBraking();
	POSITION GetStartCran();
	void GetNextCran(POSITION& rNextPosition,T_CRAN_K& rKey,T_CRAN_D& rData);
	T_CRAN_K GetCranKey(CString& rLaneName);
	T_CRAN_K GetStartNumCran();
	T_CRAN_K GetLastNumCran();
	BOOL VerifyAndSaveNodes(CArray<T_NODE_K, T_NODE_K>& arList, T_CRAN_D& CranD, CString& strErr);

// crane load number
public:
	BOOL ExistNcrn();
	BOOL GetNcrn(T_NCRN_D& rData);

// plane load assign
public:
	BOOL ExistPnla(T_PNLA_K Key);
	BOOL GetPnla(T_PNLA_K Key,T_PNLA_D& rData);
	void GetPnla(T_NODE_K KeyNode, CArray<T_PNLA_K, T_PNLA_K>& rarKey, CArray<T_PNLA_D, T_PNLA_D&>& rarData);
	void GetPnlaLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, CArray<T_PNLA_D, T_PNLA_D&>& rarData);
	int GetCountPnla();
	POSITION GetStartPnla();
	void GetNextPnla(POSITION& rNextPosition,T_PNLA_K& rKey,T_PNLA_D& rData);
	T_PNLA_K GetStartNumPnla();
	BOOL IsActivePnla(CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>& mapActiveElem, T_PNLA_D& rData);

// wind load code
public:
	void GetWindKeyList(CArray<T_WIND_K, T_WIND_K>& rKeyList);
	BOOL ExistWind(T_WIND_K Key);
	BOOL GetWind(T_WIND_K Key,T_WIND_D& rData);
	int GetCountWind();
	POSITION GetStartWind();
	void GetNextWind(POSITION& rNextPosition,T_WIND_K& rKey,T_WIND_D& rData);
	void CalcWindLoad(T_WIND_D& rData, CArray<T_WINDLOAD, T_WINDLOAD&>& arWindLoad, int nDir=0);
	void CalcWindLoadLcom_forLabel(BOOL bAllLoadCase, T_LCOM_D& DataLcom, CArray<T_WINDLOAD, T_WINDLOAD&>& arWindLoad);

// wvld load : Wave Load....BY HP
public:
	void GetWvldList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetWvldKeyList(CArray<T_WVLD_K, T_WVLD_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistWvld(CString& rName);
	BOOL ExistWvld(T_WVLD_K Key);
	BOOL GetWvld(CString& rName, T_WVLD_D& rData);
	BOOL GetWvld(T_WVLD_K Key,T_WVLD_D& rData);
	int GetCountWvld();
	POSITION GetStartWvld();
	void GetNextWvld(POSITION& rNextPosition,T_WVLD_K& rKey,T_WVLD_D& rData);
	T_WVLD_K GetWvldKey(CString& rPlaneName);
	T_WVLD_K GetStartNumWvld();
	T_WVLD_K GetLastNumWvld();
	T_WVLD_K GetNextNumWvld(T_WVLD_K StartNum);
	void GetDefaultWvld(T_WVLD_D &WvldD);

	// Calculate는 나중에	...
	//void CalcWvldLoad(T_WVLD_D& rData, CArray<T_WVLDLOAD, T_WVLDLOAD&>& arWvldLoad);
	//void CalcWvldLoadLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, CArray<T_WVLDLOAD, T_WVLDLOAD&>& arWvldLoad);
		

// seismic load code
public:
	BOOL ExistSeis(T_SEIS_K Key);
	BOOL GetSeis(T_SEIS_K Key,T_SEIS_D& rData);
	int GetCountSeis();
	POSITION GetStartSeis();
	void GetNextSeis(POSITION& rNextPosition,T_SEIS_K& rKey,T_SEIS_D& rData);
	void CalcSeisLoad(T_SEIS_D& rData, CArray<T_SEISLOAD, T_SEISLOAD&>& arSeisLoad, BOOL bCalcMassCenter=TRUE);
	void CalcSeisLoadLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, CArray<T_SEISLOAD, T_SEISLOAD&>& arSeisLoad);
	double CalcFundamentalPeriod(int nCodeType, int nMethod, double dPeriodAnal, double dPeriodAppr);
	double CalcFundamentalPeriod2(int nCodeType, double dSD1, int nMethod, double dPeriodAnal, double dPeriodAppr); // Taiwan2006 용으로 추가
	double CalcFundamentalPeriodKBC2009(int nCodeType, int nMethod, double dPeriodAnal, double dPeriodAppr, double dCu); // KBC2009
	BOOL CalcKBC2005SdsSd1(int nSeismicZone, int nSiteClass, double& dSds, double& dSd1);
	BOOL CalcKBC2009Fa(double dZoneFactor, int nSeismicZone, int nSiteClass, double& dFa);
	BOOL CalcKBC2009Fv(double dZoneFactor, int nSeismicZone, int nSiteClass, double& dFv);
	BOOL CalcKBC2009Sds(double dZoneFactor, double& dFa, double& dSds);
	BOOL CalcKBC2009Sd1(double dZoneFactor, double& dFv, double& dSd1);
	BOOL CalcKBC2009Cu(double& dSd1, double& dCu);
	BOOL CalcKBC2015Fa(double dZoneFactor, int nSiteClass, double dDepth2MR, double& dFa);
	BOOL CalcKBC2015Fv(double dZoneFactor, int nSiteClass, double dDepth2MR, double& dFv);
	BOOL CalcKSCE2019Fa(double dZoneFactor, int nSiteClass, double& dFa);
	BOOL CalcKSCE2019Fv(double dZoneFactor, int nSiteClass, double& dFv);
	BOOL CalcKDS2017Fa(double dZoneFactor, int nSiteClass, double& dFa);
	BOOL CalcKDS2017Fv(double dZoneFactor, int nSiteClass, double& dFv);

	BOOL CalcKDS2019Fa(double dZoneFactor, int nSiteClass, double& dFa);
	BOOL CalcKDS2019Fv(double dZoneFactor, int nSiteClass, double& dFv);

	//BOOL CalcKBC2009FaFvSdsSd1(int nSeismicZone, double dZoneFactor, int nSiteClass, double& dFa, double& dFv, double& dSds, double& dSd1, double& dCu);
	BOOL CalcKBC2005SeisDesignCategory(int nSeisUseGroup, double dSds, double dSd1,
																		 CString& SDC_S, CString& SDC_1, CString& SDC);
	BOOL CalcKBC2009SeisDesignCategory(int nSeisUseGroup, double dSds, double dSd1,
																		 CString& SDC_S, CString& SDC_1, CString& SDC);
	BOOL CalcTaiwan02SiteMagnifyFactor(SEIS_CODETYPE nSeisCode, int nSeisZone, int nSoilType,
		double dSds, double dSd1, double dSms, double dSm1,
		const double& dNda, const double& dNdv, const double& dNma, const double& dNmv,
		double& rdFda, double& rdFdv, double& rdFma, double& rdFmv);
	BOOL CalcTaiwan22SiteMagnifyFactor(SEIS_CODETYPE nSeisCode, int nSeisZone, int nSoilType,
		double dSds, double dSd1, double dSms, double dSm1, 
		const double& dNda, const double& dNdv, const double& dNma, const double& dNmv,
		double& rdFda, double& rdFdv, double& rdFma, double& rdFmv, double& rdFna, double& rdFnv);
	void GetSeisKeyList(CArray<T_SEIS_K, T_SEIS_K>& rKeyList); 
	T_SEIS_D GetSeis_InitializedByNation(); 

// sect factor
	BOOL CalcDumbbellMatEsc(double a_s, CString strStlDB, CString strConcDB, double& dEsc);
	BOOL CalcDumbbellMatGsc(double a_s, CString strStlDB, CString strConcDB, double& dGsc);


	// superstructure seismic load
public:
	BOOL ExistSsei(T_SSEI_K Key);
	BOOL GetSsei(T_SSEI_K Key,T_SSEI_D& rData);
	int GetCountSsei();
	POSITION GetStartSsei();
	void GetNextSsei(POSITION& rNextPosition,T_SSEI_K& rKey,T_SSEI_D& rData);
//  void CalcSseiLoad(T_SSEI_D& rData, CArray<T_BMLD_D, T_BMLD_D&>& arSseiLoad);
	void CalcSseiLoad(T_SSEI_K Key,T_SSEI_D& rData, CArray<T_BMLD_UDRD_D, T_BMLD_UDRD_D&>& arBmldData, BOOL bAnal = FALSE);
	void CalcSseiLoadLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, T_SSEI_K KeySsei, CArray<T_BMLD_UDRD_D, T_BMLD_UDRD_D&>& arSseiLoad);  

	// convert self weight to beam load
	BOOL CalcSWtoBeamLoad(T_BODF_D& Bdata, T_GRUP_D& Gdata);

// Time History Animation Time
public:
	BOOL ExistThat(T_THAT_K Key);
	BOOL GetThat(T_THAT_K Key,T_THAT_D& rData);
	int GetCountThat();
	POSITION GetStartThat();
	void GetNextThat(POSITION& rNextPosition, T_THAT_K& rKey, T_THAT_D& rData);
	
// tendon prestress load
public:
    void GetTdplKeyList(CArray<T_TDPL_K, T_TDPL_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL MakeKeyTdpl(T_TDNA_K KeyTdna,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_TDPL_K& rKey);
	BOOL ExistTdpl(T_TDPL_K Key);
	BOOL GetTdpl(T_TDPL_K Key,T_TDPL_D& rData);
	void GetTdpl(T_TDNA_K KeyTdna, CArray<T_TDPL_K, T_TDPL_K>& rarKey, CArray<T_TDPL_D, T_TDPL_D&>& rarData);
	void GetTdplLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, BOOL bAllLdgr, CArray<T_LDGR_K, T_LDGR_K>& raLdgrKey, T_TDNA_K KeyTdna, CArray<T_TDPL_D, T_TDPL_D&>& rarData);
	int GetCountTdpl();
	POSITION GetStartTdpl();
	POSITION GetStartTdplFromBaseDB();
	void GetNextTdpl(POSITION& rNextPosition,T_TDPL_K& rKey,T_TDPL_D& rData);  
protected:
	BOOL GetSerialTdpl(T_TDNA_K KeyTdna,T_STLD_K KeyStld,T_LDGR_K KeyLdgr,T_TDPL_KEY& rKeySerial);

public:
	BOOL CheckStageTdplData(BOOL bError = FALSE);


// Tendon Location Composite Section
public:
	void GetTdcsKeyList(CArray<T_TDCS_K, T_TDCS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistTdcs(T_TDCS_K Key);
	BOOL GetTdcs(T_TDCS_K Key,T_TDCS_D& rData);
	T_TDCS_K GetTdcsKey(T_TDCS_D& rData);
	int GetCountTdcs();
	POSITION GetStartTdcs();
	void GetNextTdcs(POSITION& rNextPosition,T_TDCS_K& rKey,T_TDCS_D& rData);
	T_TDCS_K GetStartNumTdcs();

// EigenValue Analysis
public:
	BOOL ExistEigv();
	BOOL GetEigv(T_EIGV_D& rData);
	int GetNumEigv();
	int GetNumEigv(T_EIGV_D& rData);
	int CalcEigvNlnkForceVectors();

// Buckling Analysis
public:
	BOOL ExistBuck();
	BOOL GetBuck(T_BUCK_D& rData);
	int GetNumBuck();

// P-Delta Analysis
public:
	BOOL ExistPdel();
	BOOL GetPdel(T_PDEL_D& rData);

// Suspension Bridge Analysis Control
public:
	BOOL ExistSbct();
	BOOL GetSbct(T_SBCT_D& rData);
	BOOL GetBottomNodeList(T_GRUP_K nGrupK, CArray<UINT,UINT>& aNode, CString& strMsg); // 그룹에 속하는 Element의 Bottom Node List를 Return.
	BOOL GetBottomNodeList(T_GRUP_D& GrupD, CArray<UINT,UINT>& aNode, CString& strMsg);
	int  UpdateHangerData(T_SBCT_D& OldSbctD, T_GRUP_K GrupK, T_GRUP_D& NewGrupD, T_SBCT_DISP& rDisp); // 새 그룹에 해당하는 Hanger Data를 업데이트 한다. (Node List, ZDisp List)
	void GetRealDispList(CArray<UINT,UINT>& aNode, double dVec[2], CArray<double,double>& aXDisp);  // Node의 벡터를 적용한 교축방향 거리를 return.

// Equilibrium Force Control
public:
	BOOL ExistEfct();
	BOOL GetEfct(T_EFCT_D& rData);

// External Type Load Case for Pretension
public:
	BOOL ExistExld();
	BOOL GetExld(T_EXLD_D& rData);

// Initial Forces for Geometric Stiffness Control Data
public:
	BOOL ExistIfct();
	BOOL GetIfct(T_IFCT_D& rData);

// Initial Forces for Geometric Stiffness
public:
	void GetIfgsKeyList(CArray<T_IFGS_K, T_IFGS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistIfgs(T_IFGS_K Key);
	BOOL GetIfgs(T_IFGS_K Key,T_IFGS_D& rData);
	int GetCountIfgs();
	POSITION GetStartIfgs();
	void GetNextIfgs(POSITION& rNextPosition,T_IFGS_K& rKey,T_IFGS_D& rData);
	double CalcIfgsInitForce(T_ELEM_K ElemK, const T_IFGS_D& crIfgsD);

// Erection Sequence
public:
	BOOL GetEseqFromBaseDB(T_ESEQ_D& rData);
	BOOL ExistEseq();
	BOOL GetEseq(T_ESEQ_D& rData);
// 요소번호를 입력받아 요소의 시공스텝 번호(모델,사하중)를 출력한다.
// 요소 하나씩 하면 속도 문제 발생함. 한번에 모든요소 다 하도록
	BOOL GetEseqId(CArray<T_ELEM_K, T_ELEM_K>& arElemKey, CArray<int, int>& arEseqModel, CArray<int, int>& arEseqLoad);

// Erection Sequence for wizard
public:
	BOOL GetEsqwFromBaseDB(T_ESQW_D& rData);
	BOOL ExistEsqw();
	BOOL GetEsqw(T_ESQW_D& rData);

// Pre-Combined Composite Bridge
public:
	BOOL ExistPlcb();
	BOOL GetPlcb(T_PLCB_D& rData);

//Flue-gas Temperature Function
public:
	void GetFlueList(CArray<CString, CString&>& rstrFlueList);
	void GetFlueKeyList(CArray<T_FLUE_K, T_FLUE_K>& rKeyList);
	BOOL ExistFlue(CString& rSpecFuncName);
	BOOL ExistFlue(T_FLUE_K Key);
	BOOL GetFlue(CString& rSpecFuncName, T_FLUE_D& rData);
	BOOL GetFlue(T_FLUE_K Key, T_FLUE_D& rData);
	T_FLUE_K GetFlueKey(CString& rFlueFuncName);
	int GetCountFlue();
	POSITION GetStartFlue();
	void GetNextFlue(POSITION& rNextPosition,T_FLUE_K& rKey,T_FLUE_D& rData);
	T_FLUE_K GetStartNumFlue();
	T_FLUE_K GetLastNumFlue();
	//BOOL IsEnableND(T_FLUE_K Key);
	void CalcFlueTemp(T_FLUE_D& rData);
	double GetFlueTemp(int nTime,T_FLUE_K FlueK);

//Steel Time-Temperatue Fun
public:
	void GetSttfList(CArray<CString, CString&>& rstrSttfList);
	void GetSttfKeyList(CArray<T_STTF_K, T_STTF_K>& rKeyList);
	BOOL ExistSttf(CString& rSpecFuncName);
	BOOL ExistSttf(T_STTF_K Key);
	BOOL GetSttf(CString& rSpecFuncName, T_STTF_D& rData);
	BOOL GetSttf(T_STTF_K Key, T_STTF_D& rData);
	T_STTF_K GetSttfKey(CString& rSttfFuncName);
	int GetCountSttf();
	POSITION GetStartSttf();
	void GetNextSttf(POSITION& rNextPosition,T_STTF_K& rKey,T_STTF_D& rData);
	T_STTF_K GetStartNumSttf();
	T_STTF_K GetLastNumSttf();
	//BOOL IsEnableND(T_STTF_K Key);
	double GetSttfTemp(int nTime,CString& strSttfName);
	double GetSttfTemp(int nTime,T_STTF_K SttfK);

//Fire Parameters
public:
	void GetFipaKeyList(CArray<T_FIPA_K, T_FIPA_K>& rKeyList);
	BOOL ExistFipa(T_FIPA_K Key);
	BOOL GetFipa(T_FIPA_K Key, T_FIPA_D& rData);
	int GetCountFipa();
	POSITION GetStartFipa();
	void GetNextFipa(POSITION& rNextPosition,T_FIPA_K& rKey,T_FIPA_D& rData);

//Fire Sect Shape factor
public:
	void GetFssfKeyList(CArray<T_FSSF_K, T_FSSF_K>& rKeyList);
	BOOL ExistFssf(T_FSSF_K Key);
	BOOL GetFssf(T_FSSF_K Key, T_FSSF_D& rData);
	int GetCountFssf();
	POSITION GetStartFssf();
	void GetNextFssf(POSITION& rNextPosition,T_FSSF_K& rKey,T_FSSF_D& rData);
	void MakeFssf(T_FSSF_K rKey,T_FSSF_D& rData);
private:
	double CalcFssf(T_FSSF_K Key,int nType);

// Spectrum Function
public:
	void GetSpfcList(CArray<CString, CString&>& rstrSpfcList);  //serial 번호로 정렬된 리스트
	void GetSpfcKeyList(CArray<T_SPFC_K, T_SPFC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistSpfc(CString& rSpecFuncName);
	BOOL ExistSpfc(T_SPFC_K Key);
	BOOL GetSpfc(CString& rSpecFuncName, T_SPFC_D& rData);
	BOOL GetSpfc(T_SPFC_K Key, T_SPFC_D& rData);
	T_SPFC_K GetSpfcKey(CString& rSpecFuncName);
	int GetCountSpfc();
	POSITION GetStartSpfc();
	void GetNextSpfc(POSITION& rNextPosition,T_SPFC_K& rKey,T_SPFC_D& rData);
	T_SPFC_K GetStartNumSpfc();
	T_SPFC_K GetLastNumSpfc();
	BOOL IsEnableND(T_SPFC_K Key);
	BOOL GetSpectralData4NonDissipative(T_SPFC_D& DataSpfc, const double dNonDissipative, T_SPFC_D& DataSpfcND);
	void GetSpectralData_EURO2004(const BOOL bPushover, double SRa, double SRv, const T_SPFC_EURO2004* pEURO2004, CArray <double,double>* parPeriod, CArray <double,double>* parAccel);
	void GetSpectralData_EURO2004Malaysia(const BOOL bPushover, double SRa, double SRv, const T_SPFC_EURO2004* pEURO2004, CArray <double,double>* parPeriod, CArray <double,double>* parAccel);
	void GetSpectralData_NTC2018(const BOOL bPushover, double SRa, double SRv, const T_SPFC_NTC2018* pNTC2018, CArray <double,double>* parPeriod, CArray <double,double>* parAccel);
	void GetSpectralData_SANS2010(const BOOL bPushover, double SRa, double SRv, const T_SPFC_SANS2010* pSANS2010, CArray <double, double>* parPeriod, CArray <double, double>* parAccel);
		
// Response Spectrum Analysis Control
public:
	BOOL ExistSpct();
	BOOL GetSpct(T_SPCT_D& rData);

// Response Spectrum Load Case
public:
	void GetSplcList(CArray<CString, CString&>& rstrSplcList);  //serial 번호로 정렬된 리스트
	void GetSplcKeyList(CArray<T_SPLC_K, T_SPLC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistSplc(CString& rLoadCaseName);
	BOOL ExistSplc(T_SPLC_K Key);
	BOOL GetSplc(CString& rLoadCaseName, T_SPLC_D& rData);
	BOOL GetSplc(T_SPLC_K Key, T_SPLC_D& rData);
	T_SPLC_K GetSplcKey(CString& rLoadCaseName);
	int GetCountSplc();
	POSITION GetStartSplc();
	void GetNextSplc(POSITION& rNextPosition,T_SPLC_K& rKey,T_SPLC_D& rData);
	T_SPLC_K GetStartNumSplc();
	T_SPLC_K GetLastNumSplc();
	void GetSplcEccenData(T_SPLC_D& rData, CLinkedKeyMgr* pLinkedKeyMgr=NULL);
	void GetSplcEccenDataAuto(const CArray<T_STOR_K, T_STOR_K>& aStorK, 
		CArray< CArray<double, double&>, CArray<double, double&>& >& aGeneratedEcc,
		CArray< CArray<double, double&>, CArray<double, double&>& >& aGeneratedEcc_AL, BOOL bUseGL, CLinkedKeyMgr* pLinkedKeyMgr=NULL);
	void CalcSplcEccn4UserMethod(OUT T_SPLC_D& Data,
		const double& dPercent4PlanDimEccn, const double& dMultiple4TopStoryEccn, const bool& bConsiderEccnbyAnal, const double& dRatio4AnalEccn);
	void CalcStorEccn4UserMethod(CArray<T_STOR_D, T_STOR_D&>& Data, const CArray<T_SEIS_WIDTH, T_SEIS_WIDTH&>& aSeisWidth,
		const double& dPercent4PlanDimEccn, const double& dMultiple4TopStoryEccn, const bool& bConsiderEccnbyAnal, const double& dRatio4AnalEccn);

	double GetGroundLevel();
	double GetBedrockLevel();
	// Seismic Perform Civil 에서 사용하는 함수 0:교축 1:교축 직각 
	// 교축 : 315~45, 135~225 (경계값 포함)
	// 교축 직각 : 교축 제외한 부분
	BOOL GetBridgeDir(T_SPLC_K Key, int &nDir);
	BOOL GetBridgeDir(T_SPLC_D& Data, int &nDir);
protected:  
	double GetEccenticity(UINT StorK, double dAngle, double dRate, CLinkedKeyMgr* pLinkedKeyMgr=NULL);
	// 자동주축에서 1도 간격으로 우발편심 계산해서 넣어주는 함수 (dRate=1.0)
	BOOL GetEccenticityAuto180(UINT StorK, CArray<double, double&>& aResult, CLinkedKeyMgr* pLinkedKeyMgr=NULL);
	void CalcStoryEccentricity(const double& dEccnXOrg, const double& dEccnYOrg, const double& dAngle, OUT double& dEccnX, OUT double& dEccnY);

// time history
public:
	void GetThisList(CArray<CString, CString&>& rstrThisList);  //serial 번호로 정렬된 리스트
	void GetThisKeyList(CArray<T_THIS_K, T_THIS_K>& rKeyList, BOOL bNeedThgcInitLoad=TRUE);  //serial 번호로 정렬된 리스트
	void GetNLThisList(CArray<CString, CString&>& rstrThisList);  //serial 번호로 정렬된 리스트
	void GetNLThisKeyList(CArray<T_THIS_K, T_THIS_K>& rKeyList);  //serial 번호로 정렬된 리스트
	int GetTimeStep(T_THIS_K Key, CArray<double, double>& rarTimeStep);
	BOOL ExistThis(CString& rLoadCaseName);
	BOOL ExistThis(T_THIS_K Key);
	BOOL GetThis(CString& rLoadCaseName, T_THIS_D& rData);
	BOOL GetThis(T_THIS_K Key, T_THIS_D& rData);
	T_THIS_K GetThisKey(CString& rLoadCaseName);
	int GetCountThis();
	POSITION GetStartThis();
	void GetNextThis(POSITION& rNextPosition,T_THIS_K& rKey,T_THIS_D& rData);
	T_THIS_K GetStartNumThis();
	T_THIS_K GetLastNumThis();
	BOOL IsThis_Nonlinear_DiSt(const T_THIS_D& ThisD);
	BOOL GetThgcInitLoadThis(T_THIS_D& rData);
	BOOL ExistThis_Nonlinear_DiSt();
	CString GetErrorStrForThisSubsequent(T_THIS_D& rData, BOOL bSkipSubSeq = FALSE);
	void ConvertThis_IntegParam_BounaryNL(T_THIS_D& rData);

// time Function
public:
	void GetThfcList(CArray<CString, CString&>& rstrThfcList);  //serial 번호로 정렬된 리스트
	void GetThfcKeyList(CArray<T_THFC_K, T_THFC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	void GetThfcKeyList_Direct(CArray<T_THFC_K, T_THFC_K>& rThfcKeyList);  // (속도개선) 복사연산 없이 주소상의 데이터를 바로 읽어서 처리
	BOOL ExistThfc(CString& rSpecFuncName);
	BOOL ExistThfc(T_THFC_K Key);
	BOOL GetThfc(CString& rThisFuncName, T_THFC_D& rData);
	BOOL GetThfc(T_THFC_K Key, T_THFC_D& rData);
	T_THFC_K GetThfcKey(CString& rThisFuncName);
	int GetCountThfc();
	POSITION GetStartThfc();
	void GetNextThfc(POSITION& rNextPosition,T_THFC_K& rKey,T_THFC_D& rData);
	T_THFC_K GetStartNumThfc();
	T_THFC_K GetLastNumThfc();

// ground acceleration
public:
	BOOL ExistThga(T_THGA_K Key);
	BOOL GetThga(T_THGA_K Key,T_THGA_D& rData);
	int GetCountThga();
	POSITION GetStartThga();
	void GetNextThga(POSITION& rNextPosition,T_THGA_K& rKey,T_THGA_D& rData);
	
// multiple support excitation
public:
	BOOL MakeKeyThms(T_NODE_K KeyNode, T_THIS_K KeyThis, T_THMS_K& rKey);
	BOOL ExistThms(T_THMS_K Key);  
	void GetThmsKeyList(CArray<T_THMS_K, T_THMS_K>& rKeyList);
	BOOL GetThms(T_THMS_K Key,T_THMS_D& rData);
	void GetThms(T_NODE_K KeyNode, CArray<T_THMS_K, T_THMS_K>& rarKey, CArray<T_THMS_D, T_THMS_D&>& rarData);
	void GetThmsLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, T_NODE_K KeyNode, CArray<T_THMS_D, T_THMS_D&>& rarData);  
	int GetCountThms();
	int GetCountThms(T_THIS_K ThisKey);
	POSITION GetStartThms();
	void GetNextThms(POSITION& rNextPosition,T_THMS_K& rKey,T_THMS_D& rData);
protected:
	BOOL GetSerialThms(T_NODE_K KeyNode,T_THIS_K KeyThis,T_THMS_KEY& rKeySerial);

// Group Damping
public:
	BOOL ExistGrdp();
	BOOL GetGrdp(T_GRDP_D& rData);
	//double GetStructDamp(T_ELEM_K rKey);
	//double GetElnkDamp(T_ELNK_K rKey);
	//double GetNlnkDamp(T_NLNK_K rKey);

// Equilibrium Element Force -> Equilibrium Element Nodal Force
public:
	T_EQMF_K GetEqmfKey(int nType, UINT ElemKey);
	void GetEqmfKeyList(CArray<T_EQMF_K, T_EQMF_K>& rKeyList);
	void GetEqmfList(CArray<T_EQMF_K, T_EQMF_K>& rKeyList, CArray<T_EQMF_D, T_EQMF_D&>& rarData);
	BOOL ExistEqmf(T_EQMF_K Key);
	BOOL GetEqmf(T_EQMF_K Key, T_EQMF_D& rData);
	int  GetCountEqmf();
	POSITION GetStartEqmf();
	void GetNextEqmf(POSITION& rNextPosition, T_EQMF_K& rKey, T_EQMF_D& rData);
	T_EQMF_K GetStartNumEqmf();
	T_EQMF_K GetLastNumEqmf();

// Initial Member Force
public:
	T_INMF_K GetInmfKey(int nType, UINT ElemKey);
	void GetInmfKeyList(CArray<T_INMF_K, T_INMF_K>& rKeyList);
	void GetInmfList(CArray<T_INMF_K, T_INMF_K>& rKeyList, CArray<T_INMF_D, T_INMF_D&>& rarData);
	BOOL ExistInmf(T_INMF_K Key);
	BOOL GetInmf(T_INMF_K Key, T_INMF_D& rData);
	int  GetCountInmf();
	POSITION GetStartInmf();
	void GetNextInmf(POSITION& rNextPosition, T_INMF_K& rKey, T_INMF_D& rData);
	T_INMF_K GetStartNumInmf();
	T_INMF_K GetLastNumInmf();
	BOOL MakeInmfData(T_ELEM_K Key, double dInitForce, T_INMF_D& rInmfD);

// dynamic spatial load
public:
	BOOL MakeKeyThsl(T_THSL_D& rData, T_THSL_K& rKey);
	void GetThslKeyList(CArray<T_THSL_K, T_THSL_K>& rKeyList);
	BOOL ExistThsl(T_THSL_K Key);
	BOOL GetThsl(T_THSL_K Key,T_THSL_D& rData);
	int GetCountThsl();
	POSITION GetStartThsl();
	void GetNextThsl(POSITION& rNextPosition,T_THSL_K& rKey,T_THSL_D& rData);
	T_THSL_K GetStartNumThsl();
	T_THSL_K GetLastNumThsl();

// Define Time history Concurrent Result
public:
	void GetThcrKeyList(CArray<T_THCR_K, T_THCR_K>& rKeyList);
	void GetThcrList(CArray<T_THCR_K, T_THCR_K>& rKeyList, CArray<T_THCR_D, T_THCR_D&>& rarData);
	BOOL MakeKeyThcr(T_THCR_D& rData, T_THCR_K& rKey);
	BOOL ExistThcr(T_THCR_K Key);
	BOOL GetThcr(T_THCR_K Key, T_THCR_D& rData);
	int GetCountThcr();
	POSITION GetStartThcr();
	void GetNextThcr(POSITION& rNextPosition, T_THCR_K& rKey, T_THCR_D& rData);

// nodal time function
public:
	BOOL MakeKeyThnl(T_NODE_K KeyNode,T_THIS_K KeyThis,T_THNL_K& rKey);
    void GetThnlKeyList(CArray<T_THNL_K, T_THNL_K>& rKeyList);
	BOOL ExistThnl(T_THNL_K Key);
	BOOL GetThnl(T_THNL_K Key,T_THNL_D& rData);
	void GetThnl(T_NODE_K KeyNode, CArray<T_THNL_K, T_THNL_K>& rarKey, CArray<T_THNL_D, T_THNL_D&>& rarData);
	void GetThnlLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, T_NODE_K KeyNode, CArray<T_THNL_D, T_THNL_D&>& rarData);
	int GetCountThnl();
	POSITION GetStartThnl();
	void GetNextThnl(POSITION& rNextPosition,T_THNL_K& rKey,T_THNL_D& rData);
protected:
	BOOL GetSerialThnl(T_NODE_K KeyNode,T_THIS_K KeyThis,T_THNL_KEY& rKeySerial);


// THRD
public:
	void GetThrdList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetThrdKeyList(CArray<T_THRD_K, T_THRD_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistThrd(CString& rName);
	BOOL ExistThrd(T_THRD_K Key);
	BOOL GetThrd(CString& rName, T_THRD_D& rData);
	BOOL GetThrd(T_THRD_K Key,T_THRD_D& rData);
	int GetCountThrd();
	POSITION GetStartThrd();
	void GetNextThrd(POSITION& rNextPosition,T_THRD_K& rKey,T_THRD_D& rData);
	T_THRD_K GetThrdKey(CString& rPlaneName);
	T_THRD_K GetStartNumThrd();
	T_THRD_K GetLastNumThrd();
	T_THRD_K GetNextNumThrd(T_THRD_K StartNum);

// SHRD(Stage Step Graph Data)
public:
	void GetStrdList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetStrdKeyList(CArray<T_STRD_K, T_STRD_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistStrd(CString& rName);
	BOOL ExistStrd(T_STRD_K Key);
	BOOL GetStrd(CString& rName, T_STRD_D& rData);
	BOOL GetStrd(T_STRD_K Key,T_STRD_D& rData);
	int GetCountStrd();
	POSITION GetStartStrd();
	void GetNextStrd(POSITION& rNextPosition,T_STRD_K& rKey,T_STRD_D& rData);
	T_STRD_K GetStrdKey(CString& rPlaneName);
	T_STRD_K GetStartNumStrd();
	T_STRD_K GetLastNumStrd();
	T_STRD_K GetNextNumStrd(T_STRD_K StartNum);

// settlement point
	void GetSmptList(CArray<CString, CString&>& rstrSmptList);  //serial 번호로 정렬된 리스트
	void GetSmptKeyList(CArray<T_SMPT_K, T_SMPT_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistSmpt(CString& rGroupName);
	BOOL ExistSmpt(T_SMPT_K Key);
	BOOL GetSmpt(CString& rGroupName, T_SMPT_D& rData);
	BOOL GetSmpt(T_SMPT_K Key, T_SMPT_D& rData);
	T_SMPT_K GetSmptKey(CString& rGroupName);
	int GetCountSmpt();
	POSITION GetStartSmpt();
	void GetNextSmpt(POSITION& rNextPosition,T_SMPT_K& rKey,T_SMPT_D& rData);
	T_SMPT_K GetStartNumSmpt();
	T_SMPT_K GetLastNumSmpt();

// settlement loadcase
	void GetSmlcList(CArray<CString, CString&>& rstrSmlcList);  //serial 번호로 정렬된 리스트
	void GetSmlcKeyList(CArray<T_SMLC_K, T_SMLC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistSmlc(CString& rLoadCaseName);
	BOOL ExistSmlc(T_SMLC_K Key);
	BOOL GetSmlc(CString& rLoadCaseName, T_SMLC_D& rData);
	BOOL GetSmlc(T_SMLC_K Key, T_SMLC_D& rData);
	T_SMLC_K GetSmlcKey(CString& rLoadCaseName);
	int GetCountSmlc();
	POSITION GetStartSmlc();
	void GetNextSmlc(POSITION& rNextPosition,T_SMLC_K& rKey,T_SMLC_D& rData);
	T_SMLC_K GetStartNumSmlc();
	T_SMLC_K GetLastNumSmlc();

// CutLine
public:
	void GetCutlList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetCutlKeyList(CArray<T_CUTL_K, T_CUTL_K>& rKeyList); //Name으로 정렬된 리스트
	//nTyep  0:일반후처리, 1:Slab Design, 2:Slab Shear check
	void GetCutlList(int nType, CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetCutlKeyList(int nType, CArray<T_CUTL_K, T_CUTL_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistCutl(CString& rName, int nType);
	BOOL ExistCutl(T_CUTL_K Key);
	BOOL GetCutl(CString& rName, T_CUTL_D& rData);
	BOOL GetCutl(T_CUTL_K Key,T_CUTL_D& rData);
	int GetCountCutl();
	POSITION GetStartCutl();
	void GetNextCutl(POSITION& rNextPosition,T_CUTL_K& rKey,T_CUTL_D& rData);
	T_CUTL_K GetCutlKey(CString& rName, int nType);
	T_CUTL_K GetStartNumCutl();
	T_CUTL_K GetLastNumCutl();
	T_CUTL_K GetNextNumCutl(T_CUTL_K StartNum);

// Cutting line with plane
public:
	void GetClwpList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetClwpKeyList(CArray<T_CLWP_K, T_CLWP_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistClwp(CString& rName);
	BOOL ExistClwp(T_CLWP_K Key);
	BOOL GetClwp(CString& rName, T_CLWP_D& rData);
	BOOL GetClwp(T_CLWP_K Key,T_CLWP_D& rData);
	int GetCountClwp();
	POSITION GetStartClwp();
	void GetNextClwp(POSITION& rNextPosition,T_CLWP_K& rKey,T_CLWP_D& rData);
	T_CLWP_K GetClwpKey(CString& rName);
	T_CLWP_K GetStartNumClwp();
	T_CLWP_K GetLastNumClwp();
	T_CLWP_K GetNextNumClwp(T_CLWP_K StartNum);

// Cable Force Tuning Result
public:
	void GetCftrList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetCftrKeyList(CArray<T_CFTR_K, T_CFTR_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistCftr(CString& rName);
	BOOL ExistCftr(T_CFTR_K Key);
	BOOL GetCftr(CString& rName, T_CFTR_D& rData);
	BOOL GetCftr(T_CFTR_K Key,T_CFTR_D& rData);
	int GetCountCftr();
	POSITION GetStartCftr();
	void GetNextCftr(POSITION& rNextPosition,T_CFTR_K& rKey,T_CFTR_D& rData);
	T_CFTR_K GetCftrKey(CString& rName);
	T_CFTR_K GetStartNumCftr();
	T_CFTR_K GetLastNumCftr();
	T_CFTR_K GetNextNumCftr(T_CFTR_K StartNum);

// Unknown Load Factor Constraint
public:
	void GetUlfcList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetUlfcKeyList(CArray<T_ULFC_K, T_ULFC_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistUlfc(CString& rName);
	BOOL ExistUlfc(T_ULFC_K Key);
	BOOL GetUlfc(CString& rName, T_ULFC_D& rData);
	BOOL GetUlfc(T_ULFC_K Key,T_ULFC_D& rData);
	int GetCountUlfc();
	POSITION GetStartUlfc();
	void GetNextUlfc(POSITION& rNextPosition,T_ULFC_K& rKey,T_ULFC_D& rData);
	T_ULFC_K GetUlfcKey(CString& rName);
	T_ULFC_K GetStartNumUlfc();
	T_ULFC_K GetLastNumUlfc();
	T_ULFC_K GetNextNumUlfc(T_ULFC_K StartNum);

// Unknown Load Factor for Final Stage
public:
	void GetUlfdList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetUlfdKeyList(CArray<T_ULFD_K, T_ULFD_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistUlfd(CString& rName);
	BOOL ExistUlfd(T_ULFD_K Key);
	BOOL GetUlfd(CString& rName, T_ULFD_D& rData);
	BOOL GetUlfd(T_ULFD_K Key,T_ULFD_D& rData);
	int GetCountUlfd();
	POSITION GetStartUlfd();
	void GetNextUlfd(POSITION& rNextPosition,T_ULFD_K& rKey,T_ULFD_D& rData);
	T_ULFD_K GetUlfdKey(CString& rName);
	T_ULFD_K GetStartNumUlfd();
	T_ULFD_K GetLastNumUlfd();
	T_ULFD_K GetNextNumUlfd(T_ULFD_K StartNum);

// Unknown Load Factor for Construction Stage
public:
	void GetUlfsList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetUlfsKeyList(CArray<T_ULFS_K, T_ULFS_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistUlfs(CString& rName);
	BOOL ExistUlfs(T_ULFS_K Key);
	BOOL GetUlfs(CString& rName, T_ULFS_D& rData);
	BOOL GetUlfs(T_ULFS_K Key,T_ULFS_D& rData);
	int GetCountUlfs();
	POSITION GetStartUlfs();
	void GetNextUlfs(POSITION& rNextPosition,T_ULFS_K& rKey,T_ULFS_D& rData);
	T_ULFS_K GetUlfsKey(CString& rName);
	T_ULFS_K GetStartNumUlfs();
	T_ULFS_K GetLastNumUlfs();
	T_ULFS_K GetNextNumUlfs(T_ULFS_K StartNum);

// Diagram (Post)
public:
	void GetDiagList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetDiagKeyList(CArray<T_DIAG_K, T_DIAG_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistDiag(CString& rName);
	BOOL ExistDiag(T_DIAG_K Key);
	BOOL GetDiag(CString& rName, T_DIAG_D& rData);
	BOOL GetDiag(T_DIAG_K Key,T_DIAG_D& rData);
	int GetCountDiag();
	POSITION GetStartDiag();
	void GetNextDiag(POSITION& rNextPosition,T_DIAG_K& rKey,T_DIAG_D& rData);
	T_DIAG_K GetDiagKey(CString& rName);
	T_DIAG_K GetStartNumDiag();
	T_DIAG_K GetLastNumDiag();
	T_DIAG_K GetNextNumDiag(T_DIAG_K StartNum);

// Batch Conversion From Moving Load Trace to Static Load
public:
	void GetBcvtList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetBcvtKeyList(CArray<T_BCVT_K, T_BCVT_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistBcvt(CString& rName);
	BOOL ExistBcvt(T_BCVT_K Key);
	BOOL GetBcvt(CString& rName, T_BCVT_D& rData);
	BOOL GetBcvt(T_BCVT_K Key,T_BCVT_D& rData);
	int GetCountBcvt();
	POSITION GetStartBcvt();
	void GetNextBcvt(POSITION& rNextPosition,T_BCVT_K& rKey,T_BCVT_D& rData);
	T_BCVT_K GetBcvtKey(CString& rName);
	T_BCVT_K GetStartNumBcvt();
	T_BCVT_K GetLastNumBcvt();
	T_BCVT_K GetNextNumBcvt(T_BCVT_K StartNum);

// Force Summary Table
public:
	void GetFsumList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetFsumKeyList(CArray<T_FSUM_K, T_FSUM_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistFsum(CString& rName);
	BOOL ExistFsum(T_FSUM_K Key);
	BOOL GetFsum(CString& rName, T_FSUM_D& rData);
	BOOL GetFsum(T_FSUM_K Key,T_FSUM_D& rData);
	int GetCountFsum();
	POSITION GetStartFsum();
	void GetNextFsum(POSITION& rNextPosition,T_FSUM_K& rKey,T_FSUM_D& rData);
	T_FSUM_K GetFsumKey(CString& rName);
	T_FSUM_K GetStartNumFsum();
	T_FSUM_K GetLastNumFsum();
	T_FSUM_K GetNextNumFsum(T_FSUM_K StartNum);

// Load Direction Force Sum
public:
	void GetLdfsList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetLdfsKeyList(CArray<T_LDFS_K, T_LDFS_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistLdfs(CString& rName);
	BOOL ExistLdfs(T_LDFS_K Key);
	BOOL GetLdfs(CString& rName, T_LDFS_D& rData);
	BOOL GetLdfs(T_LDFS_K Key,T_LDFS_D& rData);
	int GetCountLdfs();
	POSITION GetStartLdfs();
	void GetNextLdfs(POSITION& rNextPosition,T_LDFS_K& rKey,T_LDFS_D& rData);
	T_LDFS_K GetLdfsKey(CString& rName);
	T_LDFS_K GetStartNumLdfs();
	T_LDFS_K GetLastNumLdfs();
	T_LDFS_K GetNextNumLdfs(T_LDFS_K StartNum);

// camber control data
public:
	BOOL ExistCamb();
	BOOL GetCamb(T_CAMB_D& rData);

// general camber control data
public:
	BOOL ExistGcmb();
	BOOL GetGcmb(T_GCMB_D& rData);

// 설계용 단면력 Import
public:
	BOOL ExistIpfo();
	BOOL GetIpfo(T_IPFO_D& rData);

// negative moment reduction
public:
	BOOL ExistNmrd();
	BOOL GetNmrd(T_NMRD_D& rData);

// column shortening
public:
	void InitializeSclm(T_SCLM_D& rData);
	BOOL GetSclmNodeList(CString& rName, CArray<T_NODE_K, T_NODE_K>& raNodeKey, CArray<T_STAG_K, T_STAG_K>& raStagKey);
	BOOL GetSclmNodeList(T_SCLM_D& rData, CArray<T_NODE_K, T_NODE_K>& raNodeKey, CArray<T_STAG_K, T_STAG_K>& raStagKey);
	void GetSclmList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetSclmKeyList(CArray<T_SCLM_K, T_SCLM_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistSclm(CString& rName);
	BOOL ExistSclm(T_SCLM_K Key);
	BOOL GetSclm(CString& rName, T_SCLM_D& rData);
	BOOL GetSclm(T_SCLM_K Key,T_SCLM_D& rData);
	int GetCountSclm();
	POSITION GetStartSclm();
	void GetNextSclm(POSITION& rNextPosition,T_SCLM_K& rKey,T_SCLM_D& rData);
	T_SCLM_K GetSclmKey(CString& rName);
	T_SCLM_K GetStartNumSclm();
	T_SCLM_K GetLastNumSclm();
	T_SCLM_K GetNextNumSclm(T_SCLM_K StartNum);

// named ucs
public:
	void GetNucsList(CArray<CString, CString&>& rstrUcsList); //UCS Name으로 정렬된 리스트
	void GetNucsKeyList(CArray<T_NUCS_K, T_NUCS_K>& rKeyList);  //UCS Name으로 정렬된 리스트
	BOOL ExistNucs(CString& rUcsName);
	BOOL ExistNucs(T_NUCS_K Key);
	BOOL GetNucs(CString& rUcsName, T_NUCS_D& rData);
	BOOL GetNucs(T_NUCS_K Key,T_NUCS_D& rData);
	int GetCountNucs();
	POSITION GetStartNucs();
	void GetNextNucs(POSITION& rNextPosition,T_NUCS_K& rKey,T_NUCS_D& rData);
	T_NUCS_K GetNucsKey(CString& rUcsName);
	T_NUCS_K GetStartNumNucs();
	T_NUCS_K GetLastNumNucs();
	T_NUCS_K GetNextNumNucs(T_NUCS_K StartNum);
	T_NUCS_K GetNucsKeyForGCS();

// hydration control data
public:
	BOOL ExistHhct();
	BOOL GetHhct(T_HHCT_D& rData);
	int GetNumHhct();

// hydration node
public:
	void GetHhndList(CArray<CString, CString&>& rNameList); // Name으로 정렬된 리스트
	void GetHhndKeyList(CArray<T_HHND_K, T_HHND_K>& rKeyList);  // Name으로 정렬된 리스트
	BOOL ExistHhnd(CString& rName);
	BOOL ExistHhnd(T_HHND_K Key);
	BOOL GetHhnd(CString& rName, T_HHND_D& rData);
	BOOL GetHhnd(T_HHND_K Key,T_HHND_D& rData);
	int GetCountHhnd();
	POSITION GetStartHhnd();
	void GetNextHhnd(POSITION& rNextPosition,T_HHND_K& rKey,T_HHND_D& rData);
	T_HHND_K GetHhndKey(CString& rName);
	T_HHND_K GetStartNumHhnd();
	T_HHND_K GetLastNumHhnd();
	T_HHND_K GetNextNumHhnd(T_HHND_K StartNum);

// hydration : specified temperature
public:
	BOOL MakeKeyHspt(T_NODE_K KeyNode,T_BNGR_K KeyBngr,T_HSPT_K& rKey);
	void GetHsptKeyList(CArray<T_HSPT_K, T_HSPT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetHsptNodeKeyList(CArray<T_NODE_K, T_NODE_K>& rNodeKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistHspt(T_HSPT_K Key);
	BOOL GetHspt(T_HSPT_K Key,T_HSPT_D& rData);
	void GetHsptNode(T_NODE_K KeyNode, CArray<T_HSPT_K, T_HSPT_K>& rKeyList, CArray<T_HSPT_D, T_HSPT_D&>& rDataList);
	BOOL GetHsptNode(T_NODE_K KeyNode, T_HSPT_D& rData);
	int GetCountHspt();
	POSITION GetStartHspt();
	void GetNextHspt(POSITION& rNextPosition,T_HSPT_K& rKey,T_HSPT_D& rData);
protected:
	BOOL GetSerialHspt(T_NODE_K KeyNode,T_BNGR_K KeyBngr,T_HSPT_KEY& rKeySerial);

// hydration : pipe cooling element
public:
	void GetHpceList(CArray<CString, CString&>& rstrNameList);  //Key 번호로 정렬된 리스트
	void GetHpceKeyList(CArray<T_HPCE_K, T_HPCE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistHpce(CString& rstrName);
	BOOL ExistHpce(T_HPCE_K Key);
	BOOL GetHpce(CString& rstrName, T_HPCE_D& rData);
	BOOL GetHpce(T_HPCE_K Key,T_HPCE_D& rData);
	int GetCountHpce();
	POSITION GetStartHpce();
	void GetNextHpce(POSITION& rNextPosition,T_HPCE_K& rKey,T_HPCE_D& rData);
	T_HPCE_K GetHpceKey(CString& rstrName);
	T_HPCE_K GetStartNumHpce();
	T_HPCE_K GetLastNumHpce();

// Convection Coefficient Function
public:
	void GetCcfcList(CArray<CString, CString&>& rstrCcfcList);  //serial 번호로 정렬된 리스트
	void GetCcfcKeyList(CArray<T_CCFC_K, T_CCFC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistCcfc(CString& rstrFuncName);
	BOOL ExistCcfc(T_CCFC_K Key);
	BOOL GetCcfc(CString& rstrFuncName, T_CCFC_D& rData);
	BOOL GetCcfc(T_CCFC_K Key, T_CCFC_D& rData);
	T_CCFC_K GetCcfcKey(CString& rstrFuncName);
	int GetCountCcfc();
	POSITION GetStartCcfc();
	void GetNextCcfc(POSITION& rNextPosition,T_CCFC_K& rKey,T_CCFC_D& rData);
	T_CCFC_K GetStartNumCcfc();
	T_CCFC_K GetLastNumCcfc();

// Ambient Temperature Function
public:
	void GetEtfcList(CArray<CString, CString&>& rstrEtfcList);  //serial 번호로 정렬된 리스트
	void GetEtfcKeyList(CArray<T_ETFC_K, T_ETFC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistEtfc(CString& rstrFuncName);
	BOOL ExistEtfc(T_ETFC_K Key);
	BOOL GetEtfc(CString& rstrFuncName, T_ETFC_D& rData);
	BOOL GetEtfc(T_ETFC_K Key, T_ETFC_D& rData);
	T_ETFC_K GetEtfcKey(CString& rstrFuncName);
	int GetCountEtfc();
	POSITION GetStartEtfc();
	void GetNextEtfc(POSITION& rNextPosition,T_ETFC_K& rKey,T_ETFC_D& rData);
	T_ETFC_K GetStartNumEtfc();
	T_ETFC_K GetLastNumEtfc();

// Heat Source Function
public:
	void GetHsfcList(CArray<CString, CString&>& rstrHsfcList);  //serial 번호로 정렬된 리스트
	void GetHsfcKeyList(CArray<T_HSFC_K, T_HSFC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistHsfc(CString& rstrFuncName);
	BOOL ExistHsfc(T_HSFC_K Key);
	BOOL GetHsfc(CString& rstrFuncName, T_HSFC_D& rData);
	BOOL GetHsfc(T_HSFC_K Key, T_HSFC_D& rData);
	T_HSFC_K GetHsfcKey(CString& rstrFuncName);
	int GetCountHsfc();
	POSITION GetStartHsfc();
	void GetNextHsfc(POSITION& rNextPosition,T_HSFC_K& rKey,T_HSFC_D& rData);
	T_HSFC_K GetStartNumHsfc();
	T_HSFC_K GetLastNumHsfc();

// hydration : assign heat source
public:
	void GetHahsKeyList(CArray<T_HAHS_K, T_HAHS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistHahs(T_HAHS_K Key);
	BOOL GetHahs(T_HAHS_K Key,T_HAHS_D& rData);
	int GetCountHahs();
	POSITION GetStartHahs();
	void GetNextHahs(POSITION& rNextPosition,T_HAHS_K& rKey,T_HAHS_D& rData);

// hydration : element convection boundary
public:
	BOOL MakeKeyHecb(T_ELEM_K KeyElem,int nFaceNo,T_BNGR_K KeyBngr,T_HECB_K& rKey);
	BOOL ExistHecb(T_HECB_K Key);
	void GetHecbKeyList(CArray<T_HECB_K, T_HECB_K>& rKeyList);
	void GetHecbElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL GetHecb(T_HECB_K Key,T_HECB_D& rData);
	void GetHecb(T_ELEM_K KeyElem, CArray<T_HECB_K, T_HECB_K>& rarKey, CArray<T_HECB_D, T_HECB_D&>& rarData);
	int GetCountHecb();
	POSITION GetStartHecb();
	void GetNextHecb(POSITION& rNextPosition,T_HECB_K& rKey,T_HECB_D& rData);
protected:
	BOOL GetSerialHecb(T_ELEM_K KeyElem,int nFaceNo,T_BNGR_K KeyBngr,T_HECB_KEY& rKeySerial);

// hydration : material type
public:
	void GetHmtpList(CArray<CString, CString&>& rstrHmtpList);  //serial 번호로 정렬된 리스트
	void GetHmtpKeyList(CArray<T_HMTP_K, T_HMTP_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistHmtp(CString& rstrMatlName);
	BOOL ExistHmtp(T_HMTP_K Key);
	BOOL GetHmtp(CString& rstrMatlName, T_HMTP_D& rData);
	BOOL GetHmtp(T_HMTP_K Key, T_HMTP_D& rData);
	T_HMTP_K GetHmtpKey(CString& rstrMatlName);
	int GetCountHmtp();
	POSITION GetStartHmtp();
	void GetNextHmtp(POSITION& rNextPosition,T_HMTP_K& rKey,T_HMTP_D& rData);
	T_HMTP_K GetStartNumHmtp();
	T_HMTP_K GetLastNumHmtp();

// hydration material
public:
	void GetHmatKeyList(CArray<T_HMAT_K, T_HMAT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistHmat(T_HMAT_K Key);
	BOOL GetHmat(T_HMAT_K Key,T_HMAT_D& rData);
	int GetCountHmat();
	POSITION GetStartHmat();
	void GetNextHmat(POSITION& rNextPosition,T_HMAT_K& rKey,T_HMAT_D& rData);

// creep/shrinkage
public:
	BOOL ExistHcrp();
	BOOL GetHcrp(T_HCRP_D& rData);

// hydration stage
public:
	void GetHstgList(CArray<CString, CString&>& rstrStageList);  //serial 번호로 정렬된 리스트
	void GetHstgKeyList(CArray<T_HSTG_K, T_HSTG_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistHstg(CString& rStageName);
	BOOL ExistHstg(T_HSTG_K Key);
	BOOL GetHstg(CString& rStageName,T_HSTG_D& rData);
	BOOL GetHstg(T_HSTG_K Key,T_HSTG_D& rData);
	T_HSTG_K GetHstgKey(CString& rStageName);
	int GetCountHstg();
	POSITION GetStartHstg();
	void GetNextHstg(POSITION& rNextPosition,T_HSTG_K& rKey,T_HSTG_D& rData);
	T_HSTG_K GetStartNumHstg();
	T_HSTG_K GetLastNumHstg();

	void GetHstgActiveList(int nStageId, T_HSTG_D& rHstgData,
											 CArray<T_NODE_K, T_NODE_K>* paNode,
											 CArray<T_ELEM_K, T_ELEM_K>* paElem,
											 CArray<T_BNGR_K, T_BNGR_K>* paBngr,
											 CArray<T_LDGR_K, T_LDGR_K>* paLdgr);

	// Settlement Analysis Control Data
public:
	BOOL ExistSmct();
	BOOL GetSmct(T_SMCT_D& rData);

// non-linear analysis control data
public:
	void GetNlctKeyList(CArray<T_NLCT_K, T_NLCT_K>& rKeyList);  //serial 번호로 정렬된 리스트
	void GetNlctKeyListLcaseOnly(CArray<T_NLCT_K, T_NLCT_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistNlct();
	BOOL GetNlct(T_NLCT_D& rData);
	BOOL GetNlctLcaseOption(T_NLCT_K Key, T_NLCT_DATA& rLcaseData);  // Load Case 에 적용되는 옵션값을 Global, Lcase 옵션을 고려해서 return 해줌
	int GetNumNlct();
	POSITION GetStartNlct();
	void GetNextNlct(POSITION& rNextPosition, T_NLCT_K& rKey, T_NLCT_D& rData);

// Equivalent Beam Stresses check
public:
	BOOL CheckData4Vmst();
	BOOL CheckData4Vmst_VonMises(BOOL bMsg=FALSE);
	
//Define Boundary Combination
public:
	void GetBccdKeyList(CArray<T_BCCD_K, T_BCCD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistBccd(T_BCCD_K Key);
	BOOL ExistBccd(const CString& strName);
	BOOL GetBccd(T_BCCD_K Key, T_BCCD_D& rData);
	int GetCountBccd();
	POSITION GetStartBccd();
	void GetNextBccd(POSITION& rNextPosition, T_BCCD_K& rKey, T_BCCD_D& rData);
	T_BCCD_K GetStartNumBccd();
	T_BCCD_K GetLastNumBccd();
	T_BCCD_K GetBccdKeyByName(const CString& strName);
	CString  GetBccdNameByKey(T_BCCD_K Key);

// Boundary Condition Change Analysis Control Data
public:
	BOOL ExistBcct();
	BOOL GetBcct(T_BCCT_D& rData);
	int GetNumBcct();
	BOOL IsBoundaryGroup4SoilPressure(T_NODE_K KeyNode, int nLcomAnalType, int nLoadCaseKey, CArray<UINT, UINT>& aBngrKey, BOOL& bExistBngrGrupAssign);

	// MEC
	BOOL GetBcctBngrTypeItem(CArray<T_LIST_BOX_ITEM_MEC, T_LIST_BOX_ITEM_MEC>& rData, CArray<BOOL, BOOL>& aItemChecked);
	BOOL ConvertBcct_960ToMec(T_BCCT_D& BcctMaster, T_BCCT_D& BcctMec);
	BOOL ConvertBcctCombForMEC(T_BCCT_D& BcctMec);

// loading sequence for non-linear analysis
public:
	BOOL ExistLdsq();
	BOOL GetLdsq(T_LDSQ_D& rData);
	int GetNumLdsq();

// hinge data type
public:
	void GetHngtList(CArray<CString, CString&>& rstrHngtList);  //serial 번호로 정렬된 리스트
	void GetHngtKeyList(CArray<T_HNGT_K, T_HNGT_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistHngt(CString& rstrTypeName);
	BOOL ExistHngt(T_HNGT_K Key);
	BOOL GetHngt(CString& rstrTypeName, T_HNGT_D& rData);
	BOOL GetHngt(T_HNGT_K Key, T_HNGT_D& rData);
	T_HNGT_K GetHngtKey(CString& rstrTypeName);
	int GetCountHngt();
	POSITION GetStartHngt();
	void GetNextHngt(POSITION& rNextPosition,T_HNGT_K& rKey,T_HNGT_D& rData);
	T_HNGT_K GetStartNumHngt();
	T_HNGT_K GetLastNumHngt();
	void GetKeyListHngtUser(CArray<T_HNGT_K, T_HNGT_K>* pKeyHngt,         //특정 Hinge Type을 사용중인 절점, 요소 번호
								CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);

// MNET:2762-GSJI-20070518 : Pushover Renewal
public:
	void GetPhgtList(CArray<CString, CString&>& rstrPhgtList);  //serial 번호로 정렬된 리스트
	void GetPhgtKeyList(CArray<T_PHGT_K, T_PHGT_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistPhgt(CString& rstrTypeName);
	BOOL ExistPhgt(T_PHGT_K Key);
	BOOL GetPhgt(CString& rstrTypeName, T_PHGT_D& rData);
	BOOL GetPhgt(T_PHGT_K Key, T_PHGT_D& rData);
	const T_PHGT_D* GetPhgtConstPtr(T_PHGT_K Key);
	T_PHGT_K GetPhgtKey(CString& rstrTypeName);
	int GetCountPhgt();
	POSITION GetStartPhgt();
	void GetNextPhgt(POSITION& rNextPosition,T_PHGT_K& rKey,T_PHGT_D& rData);
	T_PHGT_K GetStartNumPhgt();
	T_PHGT_K GetLastNumPhgt();
	void GetKeyListPhgtUser(CArray<T_PHGT_K, T_PHGT_K>* pKeyPhgt,         //특정 Hinge Type을 사용중인 절점, 요소 번호
								CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	int GetPushoverMinMaxStepNum(); // Min Max 스텝의 개수를 리턴, 나중에 사용하게 되면 2 리턴으로 수정
	int GetPhgtListByElemType(const int nElemType, CArray<T_PHGT_K, T_PHGT_K> &raKey, CStringArray &raName);

	BOOL UpdateInitStiffMethod(T_PHGT_D &PhgtD);  // Set Initial Stiffness
	//BOOL GetPhgeAutoCalcPosition(T_PHGE_D &PhgeD, int nAutoCalcPosition[6]);
	BOOL GetPhgeAutoCalcPosition(T_PHGT_D &PhgtData, int nAutoCalcPosition[6]);
	BOOL CalcMY0ForYieldSurface(T_PHGT_YLDS &YldsD, const int &nTab, double dMY0P[2], double dMY0M[2], BOOL bFind2ndSurface=TRUE);
	BOOL GetAlpha1FromAlphaYInSkcv(double* dP1, double* dP2, double* a1, double* ay);
	//BOOL GetStiffReductRatio(const T_PHGT_D &PhgtD, T_PHGE_D &PhgeD);
	BOOL GetStiffReductRatio(const T_PHGT_D &PhgtD, T_PHGT_D &PhgtData);
	void ValidateInitStiffMethodInSkcv(const T_PHGT_D& PhgtD, T_PHGT_SKCV& rSkcv, const int nDOF);
	BOOL GetInitialStiffnessInSkcv(const T_PHGT_D &PhgtD, const int &nDOF, T_PHGT_SKCV &Skcv, double &dInitialStiffness, double &dInitialStiffness2);
	BOOL MakeSkcvStrength4YieldSurface(const T_PHGT_D &PhgtD, T_PHGT_YLDS &rYldsD, const int &nDOF, T_PHGT_SKCV &rSkcvD, T_PHGT_SKCV &rSkcvSubD);
	BOOL GetStiffReductRatioOption4Skcv(T_PHGT_SKCV &SkcvD, const BOOL &bAIJCheckOn, const int &nRatioOptionType);
	BOOL SetStiffReductRatioOption4Skcv(const int nStiffReductRatioOption, BOOL &bAIJCheckOn, int &nRatioOptionType);
	
	// nElemType : 0=Beam-Col, 1=Wall, 2=Truss, 3=GeneralLink(Spring), 4=Point Spring Support
	BOOL CalcAlphaFor_P_Disp_Type(int nDof, int nSkcvType, int nDefinition, T_PHGT_SKCV& SkcvD, double* alpha1, double* alpha2, CString& strErrMsg, int nElemType=0);
	BOOL ConvertPhgtUnloadStiff(T_PHGT_D& rData);

	bool GetPhgtByElemK(T_ELEM_K ElemK, OUT T_PHGT_D& rPhgtD);
	bool IsFiberHingePO(const T_ELEM_K ElemK);
	bool IsFiberHingeIE(const T_ELEM_K ElemK);
		
// pushover load case
public:
	void GetPoldList(CArray<CString, CString&>& rstrPoldList);  //serial 번호로 정렬된 리스트
	void GetPoldKeyList(CArray<T_POLD_K, T_POLD_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistPold(CString& rLoadCaseName);
	BOOL ExistPold(T_POLD_K Key);
	BOOL GetPold(CString& rLoadCaseName, T_POLD_D& rData);
	BOOL GetPold(T_POLD_K Key, T_POLD_D& rData);
	T_POLD_K GetPoldKey(CString& rLoadCaseName);
	int GetCountPold();
	POSITION GetStartPold();
	void GetNextPold(POSITION& rNextPosition,T_POLD_K& rKey,T_POLD_D& rData);
	T_POLD_K GetStartNumPold();
	T_POLD_K GetLastNumPold();
	// MNET:2762-GSJI-20070426 : Pold -> Polc 대체
	void GetPolcList(CArray<CString, CString&>& rstrPolcList);  //serial 번호로 정렬된 리스트
	void GetPolcKeyList(CArray<T_POLC_K, T_POLC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistPolc(CString& rLoadCaseName);
	BOOL ExistPolc(T_POLC_K Key);
	BOOL GetPolc(CString& rLoadCaseName, T_POLC_D& rData);
	BOOL GetPolc(T_POLC_K Key, T_POLC_D& rData);
	T_POLC_K GetPolcKey(CString& rLoadCaseName);
	int GetCountPolc();
	POSITION GetStartPolc();
	void GetNextPolc(POSITION& rNextPosition,T_POLC_K& rKey,T_POLC_D& rData);
	T_POLC_K GetStartNumPolc();
	T_POLC_K GetLastNumPolc();

	// Pushover Increment Function
	void GetPofcList(CArray<CString, CString&>& rstrPofcList);  //serial 번호로 정렬된 리스트
	void GetPofcKeyList(CArray<T_POFC_K, T_POFC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistPofc(CString& rLoadCaseName);
	BOOL ExistPofc(T_POFC_K Key);
	BOOL GetPofc(CString& rLoadCaseName, T_POFC_D& rData);
	BOOL GetPofc(T_POFC_K Key, T_POFC_D& rData);
	T_POFC_K GetPofcKey(CString& rCaseName);
	int GetCountPofc();
	POSITION GetStartPofc();
	void GetNextPofc(POSITION& rNextPosition,T_POFC_K& rKey,T_POFC_D& rData);
	T_POFC_K GetStartNumPofc();
	T_POFC_K GetLastNumPofc();

// assign hinge data
public:
	int  GetHngePos(T_HNGE_D& rHngeData);
	BOOL MakeKeyHnge(T_ELEM_K KeyElem, T_HNGE_D& rHngeData, T_HNGE_K& rKey);
	void GetHngeKeyList(CArray<T_HNGE_K, T_HNGE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetHngeElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	BOOL ExistHnge(T_HNGE_K Key);
	BOOL GetHnge(T_HNGE_K Key,T_HNGE_D& rData);
	void GetHngeElem(T_ELEM_K KeyElem, CArray<T_HNGE_K, T_HNGE_K>& rKeyList, CArray<T_HNGE_D, T_HNGE_D&>& rDataList);
	int GetCountHnge();
	POSITION GetStartHnge();
	void GetNextHnge(POSITION& rNextPosition,T_HNGE_K& rKey,T_HNGE_D& rData);
protected:
	BOOL GetSerialHnge(T_ELEM_K KeyElem, T_HNGE_D& rHngeData, T_HNGE_KEY& rKeySerial);
// MNET:2762-GSJI-20070521
public:
	BOOL MakeKeyPhge(const UINT nType, T_KEY KeyElem, T_PHGE_K& rKey);
	void GetPhgeKeyList(CArray<T_PHGE_K, T_PHGE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetPhgeKeyListSolOrd(CArray<T_PHGE_K, T_PHGE_K>& rKeyList);  //솔버에서 넘겨주는 순서로 정렬된 리스트
	void GetPhgeObjKeyList(const int &nType, CArray<T_KEY, T_KEY>& rObjKeyList); // Key 번호로 정렬된 리스트
	void GetPhgeElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	BOOL ExistPhge(T_PHGE_K Key);
	BOOL GetPhge(T_PHGE_K Key,T_PHGE_D& rData);
	BOOL GetPhgeObj(const int &nType, T_KEY Key, T_PHGE_K& rKey, T_PHGE_D& rData);
	int GetCountPhge();
	POSITION GetStartPhge();
	void GetNextPhge(POSITION& rNextPosition,T_PHGE_K& rKey,T_PHGE_D& rData);  
	BOOL ExistMasonry();
	BOOL MakePOHingeAssignedMembKList(int& nMaxElemSize__POHingeAssignedMemb, CArray<T_MEMB_K, T_MEMB_K>& aMembK_POHingeAssigned);

protected:
//  BOOL GetSerialPhge(T_ELEM_K KeyElem, T_PHGE_D& rPhgeData, T_PHGE_KEY& rKeySerial);

// generated hinge data type
public:
	BOOL GetGhng(T_HNGE_K HngeKey, T_HNGT_D& HngtData);
	void GetGhngList(CArray<T_HNGE_K, T_HNGE_K>& aHngeKey, CStringArray& aGhngName);
	BOOL GetGhngName(T_HNGE_K HngeKey, CString& rstrGhngName);
	BOOL CalcGhng(T_HNGE_K HngeKey, T_HNGT_D& HngtData,_PUSH_MESH_DATA& p_m_d);
	BOOL CalcPushMeshData(int nMatl, int nProp, _PUSH_MESH_DATA& p_m_d);
protected:  
	void SetGhngPropData(T_HNGT_D& data, double dTableY[9],double dTableX[9],
												double dIO_LS_CP_p[3],double dIO_LS_CP_n[3],BOOL bConvertPtoY=FALSE, BOOL bMyOnly=FALSE);
	BOOL SetGhngPropData2(_PUSH_MESH_DATA& p_m_d,T_HNGT_D& data,T_MATL_K matl_k,UINT prop_k,T_ELEM_K elem_k,int nPos);

// MNET:2762-GSJI-20070531 : Pushover Renewal 
public:
//   BOOL GetGhng(T_PHGE_K HngeKey, T_PHGT_D& HngtData);
//   void GetGhngList(CArray<T_PHGE_K, T_PHGE_K>& aHngeKey, CStringArray& aGhngName);
	BOOL GetGhngName(T_PHGE_K HngeKey, CString& rstrGhngName);
	BOOL GetGhngName(const T_PHGE_K &PhgeK, const CString &strPhgtName, CString& rstrGhngName);
	//BOOL CalcGhng(T_PHGE_K& PhgeK, T_PHGT_D& PhgtD, T_PHGE_D& PhgeD);
	BOOL CalcGhng(T_PHGE_K& PhgeK, const T_PHGT_D& PhgtD, T_PHGT_D& PhgtData, int nCalcDof=10); // nCalcDof=계산할 자유도 Default 10:All
protected:  
	//void SetGhngPropData(T_PHGE_D& PhgeD, const int nDof, double dTableY[9],double dTableX[9],
	//                                double dIO_LS_CP_p[3],double dIO_LS_CP_n[3], BOOL bConvertPtoY=FALSE, BOOL bCalcJend=FALSE);
	void SetGhngPropData(T_PHGT_D& PhgtData, const int nDof, double dTableY[9],double dTableX[9],
																	double dIO_LS_CP_p[3],double dIO_LS_CP_n[3], BOOL bConvertPtoY=FALSE, BOOL bCalcJend=FALSE);

// Yield Surface Calculation Option
public:
	BOOL ExistYsco();
	BOOL GetYsco(T_YSCO_D& rData);
	int GetYscoPQElemK();
	int GetYscoPQStep();

// pushover analysis control data
public:
	BOOL ExistPoct();
	BOOL GetPoct(T_POCT_D& rData);
	int GetNumPoct();
	// MNET:2762-GSJI-20070425 : Pushover Renewal : Poct -> Pogd
	BOOL ExistPogd();
	BOOL GetPogd(T_POGD_D& rData);
	int GetNumPogd();

// previous pushover load case
public:
	BOOL ExistPplc();
	BOOL GetPplc(T_PPLC_D& rData);
	int GetNumPplc();

// Factors For Seismic Design
public:
	BOOL ExistSlcf();
	BOOL GetSlcf(T_SLCF_D& rData);
	int GetNumSlcf();

// additional step for pushover analysis
public:
	BOOL ExistPoas();
	BOOL GetPoas(T_POAS_D& rData);

// PORD
public:
	void GetPordList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetPordKeyList(CArray<T_PORD_K, T_PORD_K>& rKeyList); //Name으로 정렬된 리스트
	BOOL ExistPord(CString& rName);
	BOOL ExistPord(T_PORD_K Key);
	BOOL GetPord(CString& rName, T_PORD_D& rData);
	BOOL GetPord(T_PORD_K Key,T_PORD_D& rData);
	int GetCountPord();
	POSITION GetStartPord();
	void GetNextPord(POSITION& rNextPosition,T_PORD_K& rKey,T_PORD_D& rData);
	T_PORD_K GetPordKey(CString& rPlaneName);
	T_PORD_K GetStartNumPord();
	T_PORD_K GetLastNumPord();
	T_PORD_K GetNextNumPord(T_PORD_K StartNum);

// Inelastic hinge property
public:
	void GetIehpList(CArray<CString, CString&>& rstrIehpList);  //serial 번호로 정렬된 리스트
	void GetIehpKeyList(CArray<T_IEHP_K, T_IEHP_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistIehp(CString& rstrName);
	BOOL ExistIehp(T_IEHP_K Key);
	BOOL GetIehp(CString& rstrName, T_IEHP_D& rData);
	BOOL GetIehp(T_IEHP_K Key, T_IEHP_D& rData);
	const T_IEHP_D* GetIehpConstPtr(T_IEHP_K Key);

	T_IEHP_K GetIehpKey(CString& rstrName);
	int GetCountIehp();
	POSITION GetStartIehp();
	void GetNextIehp(POSITION& rNextPosition,T_IEHP_K& rKey,T_IEHP_D& rData);
	T_IEHP_K GetStartNumIehp();
	T_IEHP_K GetLastNumIehp();
	void GetKeyListIehpUser(CArray<T_IEHP_K, T_IEHP_K>* pKeyIehp,         //특정 Hinge Type을 사용중인 절점, 요소 번호
								CArray<T_NODE_K, T_NODE_K>* pKeyNode=NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem=NULL);
	BOOL CalcIehpPropValue(T_IEHP_D& rData, T_ELEM_K ElemK, T_IEHP_PROP* PROP, BOOL bSubPropCalcRequest, BOOL bIsSub=FALSE, BOOL bChecError=FALSE);
	BOOL CompareAndSetIehpPropData(T_IEHP_D& rData, T_IEHP_PROP* PROP, T_IEHP_PROP* SubPROP);
	BOOL CompareUserAutoData(T_IEHP_D& rData);
	BOOL CompareUserAutoData(int nModelType, int nUserAuto, int nAlphaDelta, T_IEHP_PROP& Prop, T_IEHP_PROP& SubProp);
	BOOL CalcIehpYield(T_IEHP_D& rData, T_ELEM_K ElemK, T_IEHP_YLDSUR& YieldData, T_IEHP_YLDSUR& SubYieldData);
	BOOL GetIehpUserAutoOpt(T_IEHP_D& IehpD, int nUserAutoOpt[6]);
	BOOL IsIehpHysModel_Bilinear(int nHysModel);
	BOOL IsIehpHysModel_Trilinear(int nHysModel);
	BOOL IsIehpHysModel_Tetralinear(int nHysModel);
	BOOL Convert_IehpToPhgt_FEMA(int nDof, IN T_IEHP_D& IehpD, OUT T_PHGT_D& PhgtD); // FEMA Only
	BOOL Convert_PhgtToIehp_FEMA(int nDof, const T_PHGT_D& PhgtD, OUT T_IEHP_D& IehpD); // FEMA Only
	BOOL ConvertPhgt2Iehp(const T_PHGT_D& PhgtD, T_IEHP_D& IehpD, BOOL bAutoBeta, double dUserBeta, BOOL bFEMA, BOOL bFEMAInfill, BOOL bReplace); /// FEMA only.
	BOOL ConvertIehpUnloadStiff(T_IEHP_D& rData);
	BOOL CheckIehpAnalDataNew();

	void GetAlphafromDisplacement(int nDOF, T_IEHP_D& IehpD, double* dA1r, double* dA2r, double* dA3r);
	int  GetHingeTypeFromIehp(const T_IEHP_D& IehpD) const;
	BOOL IsAsymetricCompOfIehp(const T_IEHP_D& IehpD, int indexComp) const;

// Inelastic Time History Load Case
public:
	void GetIethList(CArray<CString, CString&>& rstrIethList);  //serial 번호로 정렬된 리스트
	void GetIethKeyList(CArray<T_IETH_K, T_IETH_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistIeth(CString& rLoadCaseName);
	BOOL ExistIeth(T_IETH_K Key);
	BOOL GetIeth(CString& rLoadCaseName, T_IETH_D& rData);
	BOOL GetIeth(T_IETH_K Key, T_IETH_D& rData);
	T_IETH_K GetIethKey(CString& rLoadCaseName);
	int GetCountIeth();
	POSITION GetStartIeth();
	void GetNextIeth(POSITION& rNextPosition,T_IETH_K& rKey,T_IETH_D& rData);
	T_IETH_K GetStartNumIeth();
	T_IETH_K GetLastNumIeth();

// assign Inelastic hinge
public:
	BOOL MakeKeyIehg(const UINT ElemType, T_ELEM_K KeyElem, T_IEHG_K& rKey);
	void GetIehgKeyList(CArray<T_IEHG_K, T_IEHG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistIehg(T_IEHG_K Key);
	BOOL ExistIehgElem(T_ELEM_K ElemK);
	BOOL GetIehg(T_IEHG_K Key,T_IEHG_D& rData);
	void GetIehgElem(T_ELEM_K KeyElem, CArray<T_IEHG_K, T_IEHG_K>& rKeyList, CArray<T_IEHG_D, T_IEHG_D&>& rDataList);
	BOOL GetIehgByElemK(T_ELEM_K ElemK, T_IEHG_D& rData);
	BOOL GetIehgObj(const int& nType, T_KEY Key, T_IEHG_K& rKey, T_IEHG_D& rData);
	void GetIehgObjKeyList(const int& nType, CArray<T_KEY, T_KEY>& rObjKeyList); // Key 번호로 정렬된 리스트
	void GetIehgElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	int GetCountIehg();
	POSITION GetStartIehg();
	void GetNextIehg(POSITION& rNextPosition,T_IEHG_K& rKey,T_IEHG_D& rData);
	T_IEHG_K MakeElemIehgK(T_ELEM_K ElemK);
	bool ConvertNlnk2IehgBeforeNS(const T_NLNK_K& NlnkK, T_IEHG_K& IehgK, T_IEHG_D& IehgD) const;
	T_IEHP_K GetIehpKeyByIehgObj(const int& nType, T_KEY ObjKey);

// M-N Time History Output
public:
	void GetMnthKeyList(CArray<T_MNTH_K, T_MNTH_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMnth(T_MNTH_K Key);
	BOOL GetMnth(T_MNTH_K Key,T_MNTH_D& rData);  
	int GetCountMnth();
	POSITION GetStartMnth();
	void GetNextMnth(POSITION& rNextPosition,T_MNTH_K& rKey,T_MNTH_D& rData);

// Fiber Material Property
public:
	void GetFimpList(CArray<CString, CString&>& rstrFimpList);  //serial 번호로 정렬된 리스트
	void GetFimpKeyList(CArray<T_FIMP_K, T_FIMP_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistFimp(CString& rstrName);
	BOOL ExistFimp(T_FIMP_K Key);
	bool ExistFimp(T_IMFM_D& ImfmD);
	BOOL GetFimp(CString& rstrName, T_FIMP_D& rData);
	BOOL GetFimp(T_FIMP_K Key, T_FIMP_D& rData);
	T_FIMP_K GetFimpKey(CString& rstrName);
	int GetCountFimp();
	POSITION GetStartFimp();
	void GetNextFimp(POSITION& rNextPosition,T_FIMP_K& rKey,T_FIMP_D& rData);
	T_FIMP_K GetStartNumFimp();
	T_FIMP_K GetLastNumFimp();
	void ConvertTriModel_AlphaToEpsilon(double dS1, double dS2, double dS3, double dK1, double dK2K1, double dK3K1, double& dE1, double& dE2, double& dE3);


// Section Division Property
public:
	void GetFibrList(CArray<CString, CString&>& rstrFibrList, int nAssignType=-1);  //serial 번호로 정렬된 리스트
	void GetFibrKeyList(CArray<T_FIBR_K, T_FIBR_K>& rKeyList, int nAssignType=-1);  //serial 번호로 정렬된 리스트
	BOOL ExistFibr(CString& rstrName, int nAssignType=-1);
	BOOL ExistFibr(T_FIBR_K Key);
	BOOL GetFibr(CString& rstrName, T_FIBR_D& rData, int nAssignType=-1);
	BOOL GetFibr(T_FIBR_K Key, T_FIBR_D& rData);
	T_FIBR_K GetFibrKey(CString& rstrName, int nAssignType=-1);
	int GetCountFibr(int nAssignType=-1);
	POSITION GetStartFibr();
	void GetNextFibr(POSITION& rNextPosition,T_FIBR_K& rKey,T_FIBR_D& rData);
	T_FIBR_K GetStartNumFibr();
	T_FIBR_K GetLastNumFibr();

	// Section Division Property (MCPM Wall)
public:	
	void GetFibwList(CArray<CString, CString&>& rstrFibwList, int nAssignType=-1);  //serial 번호로 정렬된 리스트
	void GetFibwKeyList(CArray<T_FIBW_K, T_FIBW_K>& rKeyList, int nAssignType=-1);  //serial 번호로 정렬된 리스트
	BOOL ExistFibw(CString& rstrName, int nAssignType=-1);
	BOOL ExistFibw(T_FIBW_K Key);
	BOOL GetFibw(CString& rstrName, T_FIBW_D& rData, int nAssignType=-1);
	BOOL GetFibw(T_FIBW_K Key, T_FIBW_D& rData);
	BOOL GetFibwWallIDNStorK(T_FIBW_K Key, int& nWallID, T_STOR_K& StorK);
	T_FIBW_K GetFibwKey(CString& rstrName, int nAssignType=-1);
	int GetCountFibw(int nAssignType=-1);
	POSITION GetStartFibw();
	void GetNextFibw(POSITION& rNextPosition,T_FIBW_K& rKey,T_FIBW_D& rData);
	T_FIBW_K GetStartNumFibw();
	T_FIBW_K GetLastNumFibw();
	void CalcFiberAreaConsiderRebar(CArray<T_FIBR_BASE, T_FIBR_BASE&>& aFibrBase);
	BOOL IsRebarFiber(const T_FIBR_BASE& Fiber, CArray<T_FIBR_BASE*, T_FIBR_BASE*>& aRebar);
	T_FIBW_K GetFibwKeyByWallID(int WallID, T_STOR_K StorK, int nAssignType);
// stage
public:
	void GetStagList(CArray<CString, CString&>& rstrStageList);  //serial 번호로 정렬된 리스트
	void GetStagKeyList(CArray<T_STAG_K, T_STAG_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistStag(CString& rStageName);
	BOOL ExistStag(T_STAG_K Key);
	BOOL GetStag(CString& rStageName,T_STAG_D& rData);
	BOOL GetStag(T_STAG_K Key,T_STAG_D& rData);
	T_STAG_K GetStagKey(CString& rStageName);
	int GetCountStag();
	POSITION GetStartStag();
	void GetNextStag(POSITION& rNextPosition,T_STAG_K& rKey,T_STAG_D& rData);
	T_STAG_K GetStartNumStag();
	T_STAG_K GetLastNumStag();

	int  GetCountConstStag();
	int  GetCountHydrStag();
	BOOL IsConstStag(T_STAG_K Key, T_STAG_D& rData);
	BOOL ExistConstStag();
	BOOL ExistHydrStag();

	void GetStagCurActiveGr(T_STAG_D& StagData,
											 CMap<T_GRUP_K, T_GRUP_K, UINT, UINT>& mapGrup,
											 CMap<T_BNGR_K, T_BNGR_K, UINT, UINT>& mapBngr,
											 CMap<T_LDGR_K, T_LDGR_K, UINT, UINT>& mapLdgr );
	void GetStagActiveGr(int nStageId, 
											 CMap<T_GRUP_K, T_GRUP_K, UINT, UINT>& mapGrup,
											 CMap<T_BNGR_K, T_BNGR_K, UINT, UINT>& mapBngr,
											 CMap<T_LDGR_K, T_LDGR_K, UINT, UINT>& mapLdgr );
	void GetStagActiveList(int nStageId, T_STAG_D& rStagData,
											 CArray<T_NODE_K, T_NODE_K>* paNode,
											 CArray<T_ELEM_K, T_ELEM_K>* paElem,
											 CArray<T_BNGR_K, T_BNGR_K>* paBngr,
											 CArray<T_LDGR_K, T_LDGR_K>* paLdgr);

	BOOL GetStagStepInfo(T_STAG_K Key, T_STAG_STEP_INFO& rStepInfo);
	BOOL GetFirstResultStag(T_STAG_K& rKey);
	BOOL GetFirstResultStep(T_STAG_K Key, int& nStep);
	BOOL GetLastResultStag(T_STAG_K& rKey);
	BOOL GetLastResultStep(T_STAG_K Key, int& nStep);
	BOOL GetStepSaveFlag(T_STAG_K Key, int nStepNo, BOOL& bSaveFlag);
	BOOL GetStepSaveFlag(T_STAG_STEP_INFO& rStepInfo, int nStepNo, BOOL& bSaveFlag);
	T_STAG_K GetFinalStag();
	T_STAG_K GetLastStagByUser();
	BOOL MakeElemRedistributionMap(CMap<T_ELEM_K, T_ELEM_K, double, double>& rElemRedistributionMap);

	T_STAG_K MakeStagKeyFromHstgKey(T_HSTG_K Key);
	T_HSTG_K MakeHstgKeyFromStagKey(T_STAG_K Key);

	CString GetStageStepName(T_STAG_K Key, int nCurStep);

	void GetDummyStage(CArray<BOOL, BOOL>& aDummyStage);
	void CheckStageSelfWeightData(BOOL bSamePos=FALSE);


// Time Dependent Material Function
public:
	void GetTdmfList(CArray<CString, CString&>& rstrTdmfList);  //serial 번호로 정렬된 리스트
	void GetTdmfKeyList(CArray<T_TDMF_K, T_TDMF_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistTdmf(CString& rMatlFuncName);
	BOOL ExistTdmf(T_TDMF_K Key);
	BOOL GetTdmf(CString& rMatlFuncName, T_TDMF_D& rData);
	BOOL GetTdmf(T_TDMF_K Key, T_TDMF_D& rData);
	T_TDMF_K GetTdmfKey(CString& rMatlFuncName);
	int GetCountTdmf();
	POSITION GetStartTdmf();
	void GetNextTdmf(POSITION& rNextPosition,T_TDMF_K& rKey,T_TDMF_D& rData);
	T_TDMF_K GetStartNumTdmf();
	T_TDMF_K GetLastNumTdmf();

// time dependent material(creep/shrinkage)
public:
	void GetTdmtList(CArray<CString, CString&>& rstrTdmtList);  //serial 번호로 정렬된 리스트
	void GetTdmtKeyList(CArray<T_TDMT_K, T_TDMT_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistTdmt(CString& rstrMatlName);
	BOOL ExistTdmt(T_TDMT_K Key);
	BOOL GetTdmt(CString& rstrMatlName, T_TDMT_D& rData);
	BOOL GetTdmt(T_TDMT_K Key, T_TDMT_D& rData);
	T_TDMT_K GetTdmtKey(CString& rstrMatlName);
	int GetCountTdmt();
	POSITION GetStartTdmt();
	void GetNextTdmt(POSITION& rNextPosition,T_TDMT_K& rKey,T_TDMT_D& rData);
	T_TDMT_K GetStartNumTdmt();
	T_TDMT_K GetLastNumTdmt();
	BOOL calcNotationalSize(int nCode, double dA, CArray<T_ELEM_K, T_ELEM_K>& aElemList, CArray<double, double>& aNotSize);

// time dependent material(elasticity)
public:
	void GetTdmeList(CArray<CString, CString&>& rstrTdmeList);  //serial 번호로 정렬된 리스트
	void GetTdmeKeyList(CArray<T_TDME_K, T_TDME_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistTdme(CString& rstrMatlName);
	BOOL ExistTdme(T_TDME_K Key);
	BOOL GetTdme(CString& rstrMatlName, T_TDME_D& rData);
	BOOL GetTdme(T_TDME_K Key, T_TDME_D& rData);
	T_TDME_K GetTdmeKey(CString& rstrMatlName);
	int GetCountTdme();
	POSITION GetStartTdme();
	void GetNextTdme(POSITION& rNextPosition,T_TDME_K& rKey,T_TDME_D& rData);
	T_TDME_K GetStartNumTdme();
	T_TDME_K GetLastNumTdme();

// time dependent material
public:
	void GetTmatKeyList(CArray<T_TMAT_K, T_TMAT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistTmat(T_TMAT_K Key);
	BOOL GetTmat(T_TMAT_K Key,T_TMAT_D& rData);
	int GetCountTmat();
	POSITION GetStartTmat();
	void GetNextTmat(POSITION& rNextPosition,T_TMAT_K& rKey,T_TMAT_D& rData);

	// material high temperature properties
public:
	void GetMhtpList(CArray<CString, CString&>& rstrMhtpList); 
	void GetMhtpKeyList(CArray<T_MHTP_K, T_MHTP_K>& rKeyList); 
	BOOL ExistMhtp(CString& rstrMatlName);
	BOOL ExistMhtp(T_MHTP_K Key);
	BOOL GetMhtp(CString& rstrMatlName, T_MHTP_D& rData);
	BOOL GetMhtp(T_MHTP_K Key, T_MHTP_D& rData);
	T_MHTP_K GetMhtpKey(CString& rstrMatlName);
	int GetCountMhtp();
	POSITION GetStartMhtp();
	void GetNextMhtp(POSITION& rNextPosition,T_MHTP_K& rKey,T_MHTP_D& rData);
	T_MHTP_K GetStartNumMhtp();
	T_MHTP_K GetLastNumMhtp();
	double GetStrengthCoef(double dC0, T_MHTP_D& MhtpD);
	double GetElastCoef(double dC0, T_MHTP_D& MhtpD);
	double GetStructureStlStrengthCoef(double dC0);
	double GetStructureStlElastCoef(double dC0);
	double GetFireStlStrengthCoef(double dC0);
	double GetFireStlElastCoef(double dC0);
	// material high temperature properties link
public:
	void GetMhatKeyList(CArray<T_MHAT_K, T_MHAT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMhat(T_MHAT_K Key);
	BOOL GetMhat(T_MHAT_K Key,T_MHAT_D& rData);
	int GetCountMhat();
	POSITION GetStartMhat();
	void GetNextMhat(POSITION& rNextPosition,T_MHAT_K& rKey,T_MHAT_D& rData);

// tendon property
public:
	BOOL IsUsedTdnt(T_TDNT_K Key);
	void GetTdntList(CArray<CString, CString&>& rstrTdntList);  //serial 번호로 정렬된 리스트
	void GetTdntKeyList(CArray<T_TDNT_K, T_TDNT_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistTdnt(CString& rstrTendonTypeName);
	BOOL ExistTdnt(T_TDNT_K Key);
	BOOL GetTdnt(CString& rstrTendonTypeName, T_TDNT_D& rData);
	BOOL GetTdnt(T_TDNT_K Key, T_TDNT_D& rData);
	T_TDNT_K GetTdntKey(CString& rstrTendonTypeName);
	int GetCountTdnt();
	POSITION GetStartTdnt();
	void GetNextTdnt(POSITION& rNextPosition,T_TDNT_K& rKey,T_TDNT_D& rData);
	T_TDNT_K GetStartNumTdnt();
	T_TDNT_K GetLastNumTdnt();
	void InitializeTdnt(T_TDNT_D& rData);
	void CalcXiTB(T_TDNT_D& rData); // MNET:2745 20070507 mylee - TB05 기준시 MatlK, bOverStressRedFactor -> dJTG04Factor 계산
	// Add by ZINU.('06.01.12). Option for Multi-Tendon(대표텐던).
	BOOL GetTdnt(T_TDNT_K Key, double dTendonNum, T_TDNT_D& rData, double& dDiaTndn4, double& dDiaDuct4);

// tendon profile
public:
	void ConvTdna501(T_TDNA_D_501& rData501, T_TDNA_D& rData);
	void GetTdnaList(CArray<CString, CString&>& rstrTdnaList);  //이름으로 정렬된 리스트
	void GetTdnaKeyList(CArray<T_TDNA_K, T_TDNA_K>& rKeyList);  //이름으로 정렬된 리스트
	void GetTdnaKeyListFromBaseDB(CArray<T_TDNA_K, T_TDNA_K>& rKeyList);
	BOOL ExistTdna(CString& rstrTendonName);
	BOOL ExistTdna(T_TDNA_K Key);
	BOOL GetTdna(CString& rstrTendonName, T_TDNA_D& rData);
	BOOL GetTdna(T_TDNA_K Key, T_TDNA_D& rData);
	BOOL GetTdnaFromBaseDB(CString& rstrTendonName, T_TDNA_D& rData);
	BOOL GetTdnaFromBaseDB(T_TDNA_K Key, T_TDNA_D& rData);
	T_TDNA_K GetTdnaKey(CString& rstrTendonName);
	T_TDNA_K GetTdnaKeyFromBaseDB(CString& rstrTendonName);
	void GetNextTdplFromBaseDB(POSITION& rNextPosition,T_TDPL_K& rKey,T_TDPL_D& rData);
	void GetTdnaListFromBaseDB(CArray<CString, CString&>& rstrTdnaList);
	int GetCountTdna();
	int GetCountTdnaFromBaseDB();
	POSITION GetStartTdna();
	POSITION GetStartTdnaFromBaseDB();
	void GetNextTdna(POSITION& rNextPosition,T_TDNA_K& rKey,T_TDNA_D& rData);
	void GetNextTdnaFromBaseDB(POSITION& rNextPosition,T_TDNA_K& rKey,T_TDNA_D& rData);
	T_TDNA_K GetStartNumTdna();
	T_TDNA_K GetLastNumTdna();
	BOOL calcTdnaRelativeToAbsolute(T_TDNA_BASE_501& rData);
	BOOL calcTdnaRelativeToAbsolute(T_TDNA_D_501& DataTdna);
	BOOL calcTendonProfile(T_TDNA_K KeyTdna, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint, int nDivide, BOOL bOnlyTendonLocalCoor=FALSE, BOOL bOnlyControlPoint=FALSE, BOOL bOnlyElementLocalCoor=FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);
	BOOL calcTendonProfile(T_TDNA_D DataTdna, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint, int nDivide, BOOL bOnlyTendonLocalCoor=FALSE, BOOL bOnlyControlPoint=FALSE, BOOL bOnlyElementLocalCoor=FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);
	///
	BOOL calcPTBeamTendonProfile(T_TDBM_K KeyTdbm, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint, int nDivide, BOOL bOnlyTendonLocalCoor=FALSE, BOOL bOnlyControlPoint=FALSE, BOOL bOnlyElementLocalCoor=FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);

	BOOL calPostTensionTendonProfileDetail_Beam(T_TDPF_D& TdpfD, T_SPAN_PT& SpanPT, T_TDNT_K TendonTypeKey, int nDivide, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint,
												 BOOL bOnlyTendonLocalCoor = FALSE, BOOL bOnlyControlPoint = FALSE, BOOL bOnlyElementLocalCoor = FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);

	BOOL calPostTensionTendonProfileDetail_Slab(T_TDPF_D& TdpfD, T_SPAN_PT& SpanPT, T_TDNT_K TendonTypeKey, int nDivide, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint,
												 BOOL bOnlyTendonLocalCoor = FALSE, BOOL bOnlyControlPoint = FALSE, BOOL bOnlyElementLocalCoor = FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);

	BOOL calPostTensionTendonProfileDetailSub(T_TDPF_INFO& TdpfInfo, T_SPAN_PT_BASE& PTbase, double dCzp, double dCzm, CArray<T_TDNA_BASE, T_TDNA_BASE&>& aProfile, BOOL& bSpline);

	BOOL calPostTensionTendonProfileDetail_Distribute(T_TDPF_D& TdpfD, T_SPAN_PT& NewSpanPT, T_TDNT_K TendonTypeKey, int nDivide, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint,
												 BOOL bOnlyTendonLocalCoor = FALSE, BOOL bOnlyControlPoint = FALSE, BOOL bOnlyElementLocalCoor = FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);


	double NodeDot(const T_NODE_D& a, const T_NODE_D& b);
	T_NODE_D NodeCross(const T_NODE_D& a, const T_NODE_D& b);
	double NodeLength(const T_NODE_D& v);
	T_NODE_D NodeNormalize(const T_NODE_D& v);
	double paramT(const T_NODE_D& p, const T_NODE_D& Data1, const T_NODE_D& Data2);
	bool ExistsInProfile(const CArray<T_NODE_D, T_NODE_D&>& arr, const T_NODE_D& p);
	void FindPointsBetweenNode(T_NODE_D& Data1, T_NODE_D& Data2, CArray<T_NODE_D, T_NODE_D&>& aCPointsR, CArray<T_NODE_D, T_NODE_D&>& arResult);

	T_NODE_D ComputeLeftFromUpAndT(const T_NODE_D& up_raw, const T_NODE_D& t);
	T_NODE_D ComputeRightFromUpAndT(const T_NODE_D& up_raw, const T_NODE_D& t);
	void BuildOffsetSections3D(	const T_NODE_D& A,	const T_NODE_D& B,
								double LA1, double LB1,   // A점에서 좌/우 팔 길이
								double LA2, double LB2,   // B점에서 좌/우 팔 길이
								CArray<T_DSPS_K, T_DSPS_K>& aDspsK,
								std::vector<std::pair<T_NODE_D, T_NODE_D>>& outSegments,
								std::vector<std::pair<double, double>>& aArms,
								CArray<T_NODE_D, T_NODE_D&>& aCenterNodeD);
	void ComputeLeftRightFromCenter(const T_NODE_D& Center,   // Dstr Sect Center 점
									const T_NODE_D& vec,      // Dstr Sect 단위벡터
									double dL, double dR,     // 좌/우 팔 길이
									T_NODE_D& left, T_NODE_D& right);

	///
	BOOL calcPTSlabTendonProfile    (T_TDSL_D& TdslD, double dPosition, T_SPAN_PT& SpanPT, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint, int nDivide, BOOL bOnlyTendonLocalCoor=FALSE, BOOL bOnlyControlPoint=FALSE, BOOL bOnlyElementLocalCoor=FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);

	BOOL calcPTSlabTendonProfile_Distribute(T_TDSL_D& TdslD, double dPosition, T_SPAN_PT& SpanPT, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint, int nDivide, BOOL bOnlyTendonLocalCoor=FALSE, BOOL bOnlyControlPoint=FALSE, BOOL bOnlyElementLocalCoor=FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);

	BOOL calcPttnSlabTendonProfile(T_PTTN_K PttnK, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint, int nDivide, BOOL bOnlyTendonLocalCoor=FALSE, BOOL bOnlyControlPoint=FALSE, BOOL bOnlyElementLocalCoor=FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);

	BOOL calcPttnSlabTendonProfile(T_PTTN_D& PttnD, CArray<T_NODE_D, T_NODE_D&>& aTendonProfile, CArray<BOOL, BOOL>& aProfileInputPoint, int nDivide, BOOL bOnlyTendonLocalCoor=FALSE, BOOL bOnlyControlPoint=FALSE, BOOL bOnlyElementLocalCoor=FALSE,
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xy = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xy = NULL, 
												 CArray<T_NODE_D, T_NODE_D&>* paTendonProfile_xz = NULL, CArray<BOOL, BOOL>* paProfileInputPoint_xz = NULL);
	BOOL MakePttnFromTdsl(T_TDSL_D& TdslDm, CArray<T_PTTN_D, T_PTTN_D&>& aPttnD);
	BOOL MakePttnFromTdsl_Distributed(T_TDSL_D& TdslD, CArray<T_PTTN_D, T_PTTN_D&>& aPttnD);
	BOOL MakePttnData(T_TDSL_D& TdslD, double dPosition, CArray<T_PTTN_D, T_PTTN_D&>& aPttnD, T_SPAN_PT* pNewSpanPT=NULL);
	BOOL MakePttnData4Read(T_PTTN_D& PttnD);
	void PostTensionTendonPosition(T_TDSL_D& TdslD, CArray<double, double>& aPosWidth);

	BOOL GetElemListLinked(CArray<T_ELEM_K, T_ELEM_K>* paKey, CMap<T_ELEM_K, T_ELEM_K, BOOL, BOOL>* pmSelKey, CArray<T_ELEM_K, T_ELEM_K>& arElemK);
	BOOL AlignElemListInOrderLinked(CArray<T_ELEM_K, T_ELEM_K>& arElemK);
	int GetSupportInfoPTBeam(T_NODE_K NodeK1, T_NODE_K NodeK2, CMap<T_NODE_K, T_NODE_K, int, int>& mapSupport, BOOL bSlab=FALSE);

	CString GetBeamProfileShape(const CString& strBoundaryCondition, int nType);
	CString GetSlabProfileShape(const CString& strBoundaryCondition, int nType);
	CString GetWallProfileShape(const CString& strBoundaryCondition, int nType);
	///

// time load
public:
	BOOL MakeKeyTmld(T_ELEM_K KeyElem,T_LDGR_K KeyLdgr,T_TMLD_K& rKey);
	BOOL ExistTmld(T_TMLD_K Key);
	BOOL ExistTmld(T_ELEM_K KeyElem,T_LDGR_K KeyLdgr);
	BOOL GetTmld(T_TMLD_K Key,T_TMLD_D& rData);
	void GetTmld(T_ELEM_K KeyElem, CArray<T_TMLD_K, T_TMLD_K>& rarKey, CArray<T_TMLD_D, T_TMLD_D&>& rarData);
	BOOL GetTmldElem(T_ELEM_K KeyElem, T_TMLD_D& rData);
	void GetTmldKeyList(CArray<T_TMLD_K, T_TMLD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetTmldElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	int GetCountTmld();
	POSITION GetStartTmld();
	void GetNextTmld(POSITION& rNextPosition,T_TMLD_K& rKey,T_TMLD_D& rData);
protected:
	BOOL GetSerialTmld(T_ELEM_K KeyElem,T_LDGR_K KeyLdgr,T_TMLD_KEY& rKeySerial);

// Elasto-Plastic Material
public:  
	void GetEpmtList(CArray<CString, CString&>& rstrEpmtList);  //serial 번호로 정렬된 리스트
	void GetEpmtKeyList(CArray<T_EPMT_K, T_EPMT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistEpmt(CString& rstrMatlName);
	BOOL ExistEpmt(T_EPMT_K Key);
	BOOL GetEpmt(CString& rstrMatlName, T_EPMT_D& rData);
	BOOL GetEpmt(T_EPMT_K Key, T_EPMT_D& rData);
	T_EPMT_K GetEpmtKey(CString& rstrMatlName);
	int GetCountEpmt();
	POSITION GetStartEpmt();
	void GetNextEpmt(POSITION& rNextPosition,T_EPMT_K& rKey,T_EPMT_D& rData);
	T_EPMT_K GetStartNumEpmt();
	T_EPMT_K GetLastNumEpmt();
	void GetDefaultEpmt(T_EPMT_D &rData);
	
// Multiple Point Constraint
public:  
	void GetMpctList(CArray<CString, CString&>& rstrMpctList);  //serial 번호로 정렬된 리스트
	void GetMpctKeyList(CArray<T_MPCT_K, T_MPCT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMpct(CString& rstrName);
	BOOL ExistMpct(T_MPCT_K Key);
	BOOL GetMpct(CString& rstrName, T_MPCT_D& rData);
	BOOL GetMpct(T_MPCT_K Key, T_MPCT_D& rData);
	T_MPCT_K GetMpctKey(CString& rstrName);
	int GetCountMpct();
	POSITION GetStartMpct();
	void GetNextMpct(POSITION& rNextPosition,T_MPCT_K& rKey,T_MPCT_D& rData);
	T_MPCT_K GetStartNumMpct();
	T_MPCT_K GetLastNumMpct();

// main girder
public:
	void GetMgdrList(CArray<CString, CString&>& rstrGirderList);  //순번으로 정렬된 리스트
	void GetMgdrKeyList(CArray<T_MGDR_K, T_MGDR_K>& rKeyList);  //순번으로 정렬된 리스트
	BOOL ExistMgdr(CString& rGirderName);
	BOOL ExistMgdr(T_MGDR_K Key);
	BOOL GetMgdr(CString& rGirderName, T_MGDR_D& rData);
	BOOL GetMgdr(T_MGDR_K Key,T_MGDR_D& rData);
	int GetCountMgdr();
	POSITION GetStartMgdr();
	void GetNextMgdr(POSITION& rNextPosition,T_MGDR_K& rKey,T_MGDR_D& rData);
	T_MGDR_K GetMgdrKey(CString& rGirderName);
	T_MGDR_K GetStartNumMgdr();
	T_MGDR_K GetLastNumMgdr();

// cross beam
public:
	void GetCbemList(CArray<CString, CString&>& rstrBeamList);  //순번으로 정렬된 리스트
	void GetCbemKeyList(CArray<T_CBEM_K, T_CBEM_K>& rKeyList);  //순번으로 정렬된 리스트
	BOOL ExistCbem(CString& rBeamName);
	BOOL ExistCbem(T_CBEM_K Key);
	BOOL GetCbem(CString& rBeamName, T_CBEM_D& rData);
	BOOL GetCbem(T_CBEM_K Key,T_CBEM_D& rData);
	int GetCountCbem();
	POSITION GetStartCbem();
	void GetNextCbem(POSITION& rNextPosition,T_CBEM_K& rKey,T_CBEM_D& rData);
	T_CBEM_K GetCbemKey(CString& rBeamName);
	T_CBEM_K GetStartNumCbem();
	T_CBEM_K GetLastNumCbem();

// load line
public:
	void GetLdlnList(CArray<CString, CString&>& rstrLineList);  //Key 번호로 정렬된 리스트
	void GetLdlnKeyList(CArray<T_LDLN_K, T_LDLN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLdln(CString& rLineName);
	BOOL ExistLdln(T_LDLN_K Key);
	BOOL GetLdln(CString& rLineName, T_LDLN_D& rData);
	BOOL GetLdln(T_LDLN_K Key,T_LDLN_D& rData);
	int GetCountLdln();
	POSITION GetStartLdln();
	void GetNextLdln(POSITION& rNextPosition,T_LDLN_K& rKey,T_LDLN_D& rData);
	T_LDLN_K GetLdlnKey(CString& rLineName);
	T_LDLN_K GetStartNumLdln();
	T_LDLN_K GetLastNumLdln();

// grid influence line load case
public:
	void GetGilcKeyListFromBaseDB(CArray<T_GILC_K, T_GILC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL GetGilcFromBaseDB(T_GILC_K Key, T_GILC_D& rData);

	void GetGilcList(CArray<CString, CString&>& rstrGilcList);  //serial 번호로 정렬된 리스트
	void GetGilcKeyList(CArray<T_GILC_K, T_GILC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistGilc(CString& rLoadCaseName);
	BOOL ExistGilc(T_GILC_K Key);
	BOOL GetGilc(CString& rLoadCaseName, T_GILC_D& rData);
	BOOL GetGilc(T_GILC_K Key, T_GILC_D& rData);
	T_GILC_K GetGilcKey(CString& rLoadCaseName);
	int GetCountGilc();
	POSITION GetStartGilc();
	void GetNextGilc(POSITION& rNextPosition,T_GILC_K& rKey,T_GILC_D& rData);
	T_GILC_K GetStartNumGilc();
	T_GILC_K GetLastNumGilc();
	BOOL IsGilcMaxMinType(T_GILC_D& rGilcD);

// grid influence line load
public:
	BOOL ExistGild(T_GILD_K Key);
	BOOL GetGild(T_GILD_K Key,T_GILD_D& rData);
	int GetCountGild();
	POSITION GetStartGild();
	void GetNextGild(POSITION& rNextPosition,T_GILD_K& rKey,T_GILD_D& rData);
	void GetGildLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, T_GILD_K KeyGild, T_GILD_D& rData);
	void GetGildKeyList(CArray<T_GILD_K, T_GILD_K>& rKeyList);

// Bridge Type
public:
	BOOL ExistBrgt();
	BOOL GetBrgt(T_BRGT_D& rData);

// Japan Grid Analysis Control Data 
public:
	BOOL ExistGacd();
	BOOL GetGacd(T_GACD_D& rData);

// bridge span length
public:
	void GetBspnKeyList(CArray<T_BSPN_K, T_BSPN_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistBspn(T_BSPN_K Key);
	BOOL GetBspn(T_BSPN_K Key, T_BSPN_D& rData);
	int GetCountBspn();
	POSITION GetStartBspn();
	void GetNextBspn(POSITION& rNextPosition,T_BSPN_K& rKey,T_BSPN_D& rData);
	T_BSPN_K GetStartNumBspn();
	T_BSPN_K GetLastNumBspn();

// Impact Factor in Grid Model
public:
	T_GIMP_K GetGimpKey(int nTgtType, UINT nTgtKey);
	void GetGimpKeyList(CArray<T_GIMP_K, T_GIMP_K>& rKeyList);
	BOOL ExistGimp(T_GIMP_K Key);
	BOOL GetGimp(T_GIMP_K Key, T_GIMP_D& rData);
	int GetCountGimp();
	POSITION GetStartGimp();
	void GetNextGimp(POSITION& rNextPosition,T_GIMP_K& rKey,T_GIMP_D& rData);
	T_GIMP_K GetStartNumGimp();
	T_GIMP_K GetLastNumGimp();

// Impact Factor in Japan Rail
public:
	T_JIMP_K GetJimpKey(int nTgtType, UINT nTgtKey, int nFactorType);
	void GetJimpKeyList(CArray<T_JIMP_K, T_JIMP_K>& rKeyList);
	BOOL ExistJimp(T_JIMP_K Key);
	BOOL GetJimp(T_JIMP_K Key, T_JIMP_D& rData);
	int GetCountJimp();
	POSITION GetStartJimp();
	void GetNextJimp(POSITION& rNextPosition, T_JIMP_K& rKey, T_JIMP_D& rData);
	T_JIMP_K GetStartNumJimp();
	T_JIMP_K GetLastNumJimp();

// Element Structure Type for Grid Model
public:
	T_GSTY_K GetGstyKey(int nTgtType, UINT nTgtKey);
	void GetGstyKeyList(CArray<T_GSTY_K, T_GSTY_K>& rKeyList);
	BOOL ExistGsty(T_GSTY_K Key);
	BOOL GetGsty(T_GSTY_K Key, T_GSTY_D& rData);
	int GetCountGsty();
	POSITION GetStartGsty();
	void GetNextGsty(POSITION& rNextPosition,T_GSTY_K& rKey,T_GSTY_D& rData);
	T_GSTY_K GetStartNumGsty();
	T_GSTY_K GetLastNumGsty();

// cross reference in grid model
public:
	void GetCrefList(CArray<CString, CString&>& rstrReferenceList);  //순번으로 정렬된 리스트
	void GetCrefKeyList(CArray<T_CREF_K, T_CREF_K>& rKeyList);  //순번으로 정렬된 리스트
	BOOL ExistCref(CString& rReferenceName);
	BOOL ExistCref(T_CREF_K Key);
	BOOL GetCref(CString& rReferenceName, T_CREF_D& rData);
	BOOL GetCref(T_CREF_K Key,T_CREF_D& rData);
	int GetCountCref();
	POSITION GetStartCref();
	void GetNextCref(POSITION& rNextPosition,T_CREF_K& rKey,T_CREF_D& rData);
	T_CREF_K GetCrefKey(CString& rReferenceName);
	T_CREF_K GetStartNumCref();
	T_CREF_K GetLastNumCref();

// span information
public:
	void GetSpanList(CArray<CString, CString&>& rstrSpanList);  //순번으로 정렬된 리스트
	void GetSpanKeyList(CArray<T_SPAN_K, T_SPAN_K>& rKeyList, int nSectType);  //순번으로 정렬된 리스트
	void GetSpanKeyList(CArray<T_SPAN_K, T_SPAN_K>& rKeyList);  //순번으로 정렬된 리스트
	BOOL ExistSpan(CString& rSpanName, int nSectType);
	BOOL ExistSpan(T_SPAN_K Key);
	BOOL GetSpan(CString& rSpanName, int nSectType, T_SPAN_D& rData);
	BOOL GetSpan(T_SPAN_K Key,T_SPAN_D& rData);
	int GetCountSpan();
	int GetCountSubSpan(T_SPAN_K rKey);
	POSITION GetStartSpan();
	void GetNextSpan(POSITION& rNextPosition,T_SPAN_K& rKey,T_SPAN_D& rData);
	T_SPAN_K GetSpanKey(CString& rSpanName, int nSectType);
	T_SPAN_K GetStartNumSpan();
	T_SPAN_K GetLastNumSpan();
	void GetSpanLengthByElementLength(T_SPAN_D& SpanD, CArray<double, double>& aElemSpan);

// span information - virtual beam
public:
	void GetSpavList(CArray<CString, CString&>& rstrSpavList);  //순번으로 정렬된 리스트
	void GetSpavKeyList(CArray<T_SPAV_K, T_SPAV_K>& rKeyList, int nSectType);  //순번으로 정렬된 리스트
	void GetSpavKeyList(CArray<T_SPAV_K, T_SPAV_K>& rKeyList);  //순번으로 정렬된 리스트
	BOOL ExistSpav(CString& rSpavName, int nSectType);
	BOOL ExistSpav(T_SPAV_K Key);
	BOOL GetSpav(CString& rSpavName, int nSectType, T_SPAV_D& rData);
	BOOL GetSpav(T_SPAV_K Key, T_SPAV_D& rData);
	int GetCountSpav();
	int GetCountSubSpav(T_SPAV_K rKey);
	POSITION GetStartSpav();
	void GetNextSpav(POSITION& rNextPosition, T_SPAV_K& rKey, T_SPAV_D& rData);
	T_SPAV_K GetSpavKey(CString& rSpavName, int nSectType);
	T_SPAV_K GetStartNumSpav();
	T_SPAV_K GetLastNumSpav();
	void GetSpavLengthByVbemLength(T_SPAV_D& SpavD, CArray<double, double>& aElemSpav);

// Span or Spav
	void GetSpanOrSpavKeyList(CArray<T_SPAV_K, T_SPAV_K>& rKeyList, BOOL bVirtual=FALSE);
	BOOL GetSpanOrSpav(T_SPAN_K Key, T_SPAN_D& rData, BOOL bVirtual=FALSE);
	void GetSpanOrSpavKeyList(CArray<T_SPAN_K, T_SPAN_K>& rKeyList, int nSectType, BOOL bVirtual);  //순번으로 정렬된 리스트


// Approximate Estimate of Time Dependent Tendon Losses
public:
	BOOL ExistAetl();
	BOOL GetAetl(T_AETL_D& rData);
	BOOL GetAetlFromBaseDB(T_AETL_D& rData);
	
// Diagram Color for Construction Stage
public:
	void GetDgmcKeyList(CArray<T_DGMC_K, T_DGMC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDgmc(T_DGMC_K Key);
	BOOL GetDgmc(T_DGMC_K Key,T_DGMC_D& rData);
	int GetCountDgmc();
	POSITION GetStartDgmc();
	void GetNextDgmc(POSITION& rNextPosition,T_DGMC_K& rKey,T_DGMC_D& rData);
	

// Diagram Color for Load Case
public:
	void GetDgmlKeyList(CArray<T_DGML_K, T_DGML_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDgml(T_DGML_K Key);
	BOOL ExistDgml(unsigned int AnalType, unsigned int LoadCaseKey);
	BOOL GetDgml(T_DGML_K Key,T_DGML_D& rData);
	BOOL GetDgmlKey(unsigned int AnalType, unsigned int LoadCaseKey, T_DGML_K& Key);
	int GetCountDgml();
	POSITION GetStartDgml();
	void GetNextDgml(POSITION& rNextPosition,T_DGML_K& rKey,T_DGML_D& rData);
	T_DGML_K GetStartNumDgml();
	T_DGML_K GetLastNumDgml();



// p1 load factor type
public:
	void GetP1ltList(CArray<CString, CString&>& rstrP1ltList);  //serial 번호로 정렬된 리스트
	void GetP1ltKeyList(CArray<T_P1LT_K, T_P1LT_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistP1lt(CString& rName);
	BOOL ExistP1lt(T_P1LT_K Key);
	BOOL GetP1lt(CString& rName, T_P1LT_D& rData);
	BOOL GetP1lt(T_P1LT_K Key, T_P1LT_D& rData);
	T_P1LT_K GetP1ltKey(CString& rName);
	int GetCountP1lt();
	POSITION GetStartP1lt();
	void GetNextP1lt(POSITION& rNextPosition,T_P1LT_K& rKey,T_P1LT_D& rData);
	T_P1LT_K GetStartNumP1lt();
	T_P1LT_K GetLastNumP1lt();

// P1 Load Factor
public:
	T_P1LA_K GetP1laKey(int nLoadItem, UINT NodeElemK);
	void GetP1laKeyList(CArray<T_P1LA_K, T_P1LA_K>& rKeyList);
	BOOL ExistP1la(T_P1LA_K Key);
	BOOL GetP1la(T_P1LA_K Key, T_P1LA_D& rData);
	int GetCountP1la();
	POSITION GetStartP1la();
	void GetNextP1la(POSITION& rNextPosition,T_P1LA_K& rKey,T_P1LA_D& rData);
	T_P1LA_K GetStartNumP1la();
	T_P1LA_K GetLastNumP1la();

// Impact and P1 Load Factor for Link
public:
	T_LNKF_K GetLnkfKey(int nLinkType, UINT LinkKey, int nLoadKind);
	void GetLnkfKeyList(CArray<T_LNKF_K, T_LNKF_K>& rKeyList);
	BOOL ExistLnkf(T_LNKF_K Key);
	BOOL GetLnkf(T_LNKF_K Key, T_LNKF_D& rData);
	int GetCountLnkf();
	POSITION GetStartLnkf();
	void GetNextLnkf(POSITION& rNextPosition,T_LNKF_K& rKey,T_LNKF_D& rData);
	T_LNKF_K GetStartNumLnkf();
	T_LNKF_K GetLastNumLnkf();

// Loadcases for reciprocal stress
public:
	BOOL ExistLcrs();
	BOOL GetLcrs(T_LCRS_D& rData);

// Define Concurrent Reaction Group...
public:
	BOOL ExistCrgr();
	BOOL GetCrgr(T_CRGR_D& rData);

// Define Concurrent Joint Force Group...
public:
	BOOL ExistCjfg();
	BOOL GetCjfg(T_CJFG_D& rData);
	BOOL GetMovingElemData4Cjfg(BOOL& bForce, int& nForcFilter, T_GRUP_K& ForcGrupK);

// creep coefficient
public:
	BOOL MakeKeyCrpc(T_ELEM_K KeyElem,T_LDGR_K KeyLdgr,T_CRPC_K& rKey);
	BOOL ExistCrpc(T_CRPC_K Key);
	BOOL ExistCrpc(T_ELEM_K KeyElem,T_LDGR_K KeyLdgr);
	BOOL GetCrpc(T_CRPC_K Key,T_CRPC_D& rData);
	void GetCrpc(T_ELEM_K KeyElem, CArray<T_CRPC_K, T_CRPC_K>& rarKey, CArray<T_CRPC_D, T_CRPC_D&>& rarData);
	BOOL GetCrpcElem(T_ELEM_K KeyElem, T_CRPC_D& rData);
	void GetCrpcKeyList(CArray<T_CRPC_K, T_CRPC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetCrpcElemKeyList(CArray<T_ELEM_K, T_ELEM_K>& rElemKeyList); // Key 번호로 정렬된 리스트
	int GetCountCrpc();
	POSITION GetStartCrpc();
	void GetNextCrpc(POSITION& rNextPosition,T_CRPC_K& rKey,T_CRPC_D& rData);
protected:
	BOOL GetSerialCrpc(T_ELEM_K KeyElem,T_LDGR_K KeyLdgr,T_CRPC_KEY& rKeySerial);

// stage analysis control data
public:
	BOOL ExistStct();
	BOOL GetStct(T_STCT_D& rData);
	BOOL GetStctForAnalysis(T_STCT_D& rData);
	int GetNumStct();
	BOOL IsRestartPrevStag4Rcsa(T_STAG_K CurStagK);
	BOOL IsRestartStag4Rcsa(T_STAG_K CurStagK);
	T_STAG_K GetRestartPrevStagKey(T_STAG_K CurRestartStagK);

	BOOL GetErectionSelected(CMap<T_SGLD_K, T_SGLD_K, int, int>& mapErect);
	int GetBeamSectionChage();

	// restart construction stage analysis
public:
	BOOL ExistRcsa();
	BOOL GetRcsa(T_RCSA_D& rData);
	int GetNumRcsa();
	

// reaction position
public:
	void GetRposList(CArray<CString, CString&>& rstrRposList);  //serial 번호로 정렬된 리스트
	void GetRposKeyList(CArray<T_RPOS_K, T_RPOS_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistRpos(CString& rPosName);
	BOOL ExistRpos(T_RPOS_K Key);
	BOOL GetRpos(CString& rPosName, T_RPOS_D& rData);
	BOOL GetRpos(T_RPOS_K Key, T_RPOS_D& rData);
	T_RPOS_K GetRposKey(CString& rPosName);
	int GetCountRpos();
	POSITION GetStartRpos();
	void GetNextRpos(POSITION& rNextPosition,T_RPOS_K& rKey,T_RPOS_D& rData);
	T_RPOS_K GetStartNumRpos();
	T_RPOS_K GetLastNumRpos();

// construction stage load case // old : use sgld
public:
	BOOL GetCsld(T_CSLD_K Key, T_CSLD_D& rData);
	int GetCountCsld();

// construction stage load case
public:
	void GetSgldList(CArray<CString, CString&>& rstrSgldList);  //serial 번호로 정렬된 리스트
	void GetSgldKeyList(CArray<T_SGLD_K, T_SGLD_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistSgld(CString& rLoadCaseName);
	BOOL ExistSgld(T_SGLD_K Key);
	BOOL GetSgld(CString& rLoadCaseName, T_SGLD_D& rData);
	BOOL GetSgld(T_SGLD_K Key, T_SGLD_D& rData);
	T_SGLD_K GetSgldKey(CString& rLoadCaseName);
	int GetCountSgld();
	void ClearSgldMap() { mSgldKey.clear(); }

	CString GetSgldName(T_SGLD_K Key);
private:	
	std::map<CString, T_SGLD_K> mSgldKey;

// eccentricity response spectrum load case
public:
	void GetEspcList(CArray<CString, CString&>& rstrEspcList);  //serial 번호로 정렬된 리스트
	void GetEspcKeyList(CArray<T_ESPC_K, T_ESPC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistEspc(CString& rLoadCaseName);
	BOOL ExistEspc(T_ESPC_K Key);
	BOOL GetEspc(CString& rLoadCaseName, T_ESPC_D& rData);
	BOOL GetEspc(T_ESPC_K Key, T_ESPC_D& rData);
	T_ESPC_K GetEspcKey(CString& rLoadCaseName);
	int GetCountEspc();

	// Non-Dissipative response spectrum load case
public:
	void GetNspcList(CArray<CString, CString&>& rstrNspcList);  //serial 번호로 정렬된 리스트
	void GetNspcKeyList(CArray<T_NSPC_K, T_NSPC_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistNspc(CString& rLoadCaseName);
	BOOL ExistNspc(T_NSPC_K Key);
	BOOL GetNspc(CString& rLoadCaseName, T_NSPC_D& rData);
	BOOL GetNspc(T_NSPC_K Key, T_NSPC_D& rData);
	T_NSPC_K GetNspcKey(CString& rLoadCaseName);
	int GetCountNspc();

// member type by group
public:
	void InitializeMtbg(T_MTBG_D& rData);
	BOOL ExistMtbg();
	BOOL GetMtbg(T_MTBG_D& rData);

// design variables for member type
public:
	BOOL ExistDvmt();
	BOOL GetDvmt(T_DVMT_D& rData);

// bolt
public:
	BOOL ExistBolt();
	BOOL GetBolt(T_BOLT_D& rData);
	void InitializeBolt(T_BOLT_D& rData);

// Tower Load Generator
public:
	BOOL ExistTlgd();
	BOOL GetTlgd(T_TLGD_D& rData);

// design
public:
	BOOL ExistDctl();
	BOOL GetDctl(T_DCTL_D& rData);
// Performance design
public:
	BOOL ExistPfdn();
	BOOL GetPfdn(T_PFDN_D& rData);
// crc psc bridge
public:
	BOOL ExistPscd();
	BOOL GetPscd(T_PSCD_D& rData);
	BOOL GetDgnPscd(T_PSCD_D& rData);

// crc shear, moment check
public:
	void GetPoscKeyList(CArray<T_POSC_K, T_POSC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPosc(T_POSC_K Key);
	BOOL GetPosc(T_POSC_K Key,T_POSC_D& rData);
	int  GetCountPosc();
	POSITION GetStartPosc();
	void GetNextPosc(POSITION& rNextPosition,T_POSC_K& rKey,T_POSC_D& rData);
	
// crc shear, moment check Virtual Beam
public:
	void GetPovcKeyList(CArray<T_POVC_K, T_POVC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPovc(T_POVC_K Key);
	BOOL GetPovc(T_POVC_K Key,T_POVC_D& rData);
	int  GetCountPovc();
	POSITION GetStartPovc();
	void GetNextPovc(POSITION& rNextPosition,T_POVC_K& rKey,T_POVC_D& rData);

// crc psc Exposure class
public:
	void GetPescKeyList(CArray<T_PESC_K, T_PESC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPesc(T_PESC_K Key);
	BOOL GetPesc(T_PESC_K Key,T_PESC_D& rData);
	int  GetCountPesc();
	POSITION GetStartPesc();
	void GetNextPesc(POSITION& rNextPosition,T_PESC_K& rKey,T_PESC_D& rData);

	// crc psc Exposure class virtual beam
public:
	void GetPesvKeyList(CArray<T_PESV_K, T_PESV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPesv(T_PESV_K Key);
	BOOL GetPesv(T_PESV_K Key, T_PESV_D& rData);
	int  GetCountPesv();
	POSITION GetStartPesv();
	void GetNextPesv(POSITION& rNextPosition, T_PESV_K& rKey, T_PESV_D& rData);

// crc Exposure class
public:
	void GetRescKeyList(CArray<T_RESC_K, T_RESC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistResc(T_RESC_K Key);
	BOOL GetResc(T_RESC_K Key, T_RESC_D& rData);
	int  GetCountResc();
	POSITION GetStartResc();
	void GetNextResc(POSITION& rNextPosition, T_RESC_K& rKey, T_RESC_D& rData);

// psc crack width
public:
	void GetPscwKeyList(CArray<T_PSCW_K, T_PSCW_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPscw(T_PSCW_K Key);
	BOOL GetPscw(T_PSCW_K Key,T_PSCW_D& rData);
	int  GetCountPscw();
	POSITION GetStartPscw();
	void GetNextPscw(POSITION& rNextPosition,T_PSCW_K& rKey,T_PSCW_D& rData);

// crc print option
public:
	void GetPosxKeyList(CArray<T_POSX_K, T_POSX_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPosx(T_POSX_K Key);
	BOOL GetPosx(T_POSX_K Key,T_POSX_D& rData);
	int  GetCountPosx();
	POSITION GetStartPosx();
	void GetNextPosx(POSITION& rNextPosition,T_POSX_K& rKey,T_POSX_D& rData);

// crc print option virtual beam
public:
	void GetPosvKeyList(CArray<T_POSV_K, T_POSV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPosv(T_POSV_K Key);
	BOOL GetPosv(T_POSV_K Key,T_POSV_D& rData);
	int  GetCountPosv();
	POSITION GetStartPosv();
	void GetNextPosv(POSITION& rNextPosition,T_POSV_K& rKey,T_POSV_D& rData);

//Civil CH RC Output Section Position (Print Option)
public:
	void GetPorcKeyList(CArray<T_PORC_K, T_PORC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPorc(T_PORC_K Key);
	BOOL GetPorc(T_PORC_K Key,T_PORC_D& rData);
	int  GetCountPorc();
	POSITION GetStartPorc();
	void GetNextPorc(POSITION& rNextPosition,T_PORC_K& rKey,T_PORC_D& rData);

//Civil CH RC Design Section Position (Design Option)
public:
	void GetDorcKeyList(CArray<T_DORC_K, T_DORC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDorc(T_DORC_K Key);
	BOOL GetDorc(T_DORC_K Key,T_DORC_D& rData);
	int  GetCountDorc();
	POSITION GetStartDorc();
	void GetNextDorc(POSITION& rNextPosition,T_DORC_K& rKey,T_DORC_D& rData);

//Civil CH RC Crack Width Factor Option
public:
	void GetCwrcKeyList(CArray<T_CWRC_K, T_CWRC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCwrc(T_CWRC_K Key);
	BOOL GetCwrc(T_CWRC_K Key,T_CWRC_D& rData);
	int  GetCountCwrc();
	POSITION GetStartCwrc();
	void GetNextCwrc(POSITION& rNextPosition,T_CWRC_K& rKey,T_CWRC_D& rData);

// design
public:
	BOOL ExistLctp();
	BOOL GetLctp(T_LCTP_D& rData);

// length
public:
	void GetLengKeyList(CArray<T_LENG_K, T_LENG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLeng(T_LENG_K Key);
	BOOL GetLeng(T_LENG_K Key,T_LENG_D& rData);
	int GetCountLeng();
	POSITION GetStartLeng();
	void GetNextLeng(POSITION& rNextPosition,T_LENG_K& rKey,T_LENG_D& rData);

// length
public:
	void GetLenvKeyList(CArray<T_LENV_K, T_LENV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLenv(T_LENV_K Key);
	BOOL GetLenv(T_LENV_K Key, T_LENV_D& rData);
	int GetCountLenv();
	POSITION GetStartLenv();
	void GetNextLenv(POSITION& rNextPosition, T_LENV_K& rKey, T_LENV_D& rData);
	//
	void GetLengOrLenvKeyList(CArray<T_LENG_K, T_LENG_K>& rKeyList, BOOL bVirtual=FALSE);
	BOOL GetLengOrLenv(T_LENG_K Key, T_LENG_D& rData, BOOL bVirtual=FALSE);

// span length
public:
	void GetSplnKeyList(CArray<T_SPLN_K, T_SPLN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSpln(T_SPLN_K Key);
	BOOL GetSpln(T_SPLN_K Key,T_SPLN_D& rData);
	int GetCountSpln();
	POSITION GetStartSpln();
	void GetNextSpln(POSITION& rNextPosition,T_SPLN_K& rKey,T_SPLN_D& rData);

// Span Length - Virtual Beam
public:
	void GetSplvKeyList(CArray<T_SPLV_K, T_SPLV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSplv(T_SPLV_K Key);
	BOOL GetSplv(T_SPLV_K Key, T_SPLV_D& rData);
	int GetCountSplv();
	POSITION GetStartSplv();
	void GetNextSplv(POSITION& rNextPosition, T_SPLV_K& rKey, T_SPLV_D& rData);
	//
	void GetSplnOrSplvKeyList(CArray<T_SPLN_K, T_SPLN_K>& rKeyList, BOOL bVirtual=FALSE);
	BOOL GetSplnOrSplv(T_SPLN_K Key, T_SPLN_D& rData, BOOL bVirtual=FALSE);

	// Arib
public:
	void GetAribKeyList(CArray<T_ARIB_K, T_ARIB_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistArib(T_ARIB_K Key);
	BOOL GetArib(T_ARIB_K Key,T_ARIB_D& rData);
	int GetCountArib();
	POSITION GetStartArib();
	void GetNextArib(POSITION& rNextPosition,T_ARIB_K& rKey,T_ARIB_D& rData);
	// Awed
public:
	void GetAwedKeyList(CArray<T_AWED_K, T_AWED_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistAwed(T_AWED_K Key);
	BOOL GetAwed(T_AWED_K Key,T_AWED_D& rData);
	int GetCountAwed();
	POSITION GetStartAwed();
	void GetNextAwed(POSITION& rNextPosition,T_AWED_K& rKey,T_AWED_D& rData);
	// Ahtr
public:
	void GetAhtrKeyList(CArray<T_AHTR_K, T_AHTR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistAhtr(T_AHTR_K Key);
	BOOL GetAhtr(T_AHTR_K Key,T_AHTR_D& rData);
	int GetCountAhtr();
	POSITION GetStartAhtr();
	void GetNextAhtr(POSITION& rNextPosition,T_AHTR_K& rKey,T_AHTR_D& rData);
	//csos
public:
	void GetCsosKeyList(CArray<T_CSOS_K, T_CSOS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCsos(T_CSOS_K Key);
	BOOL GetCsos(T_CSOS_K Key,T_CSOS_D& rData);
	int GetCountCsos();
	POSITION GetStartCsos();
	void GetNextCsos(POSITION& rNextPosition,T_CSOS_K& rKey,T_CSOS_D& rData);
	//Csms
public:
	void GetCsmsKeyList(CArray<T_CSMS_K, T_CSMS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCsms(T_CSMS_K Key);
	BOOL GetCsms(T_CSMS_K Key,T_CSMS_D& rData);
	int GetCountCsms();
	POSITION GetStartCsms();
	void GetNextCsms(POSITION& rNextPosition,T_CSMS_K& rKey,T_CSMS_D& rData);
	//purlin
public:
	void GetPlinKeyList(CArray<T_PLIN_K, T_PLIN_K>& rKeyList);
	BOOL ExistPlin(T_PLIN_K Key);
	BOOL GetPlin(T_PLIN_K Key, T_PLIN_D& rData);
	int GetCountPlin();
	POSITION GetStartPlin();
	void GetNextPlin(POSITION& rNextPosition, T_PLIN_K& rKey, T_PLIN_D& rData);
	// Aphi
public:
	void GetAphiKeyList(CArray<T_APHI_K, T_APHI_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistAphi(T_APHI_K Key);
	BOOL GetAphi(T_APHI_K Key,T_APHI_D& rData);
	int GetCountAphi();
	POSITION GetStartAphi();
	void GetNextAphi(POSITION& rNextPosition,T_APHI_K& rKey,T_APHI_D& rData);

// k-factor
public:
	void GetKfacKeyList(CArray<T_KFAC_K, T_KFAC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistKfac(T_KFAC_K Key);
	BOOL GetKfac(T_KFAC_K Key,T_KFAC_D& rData);
	int GetCountKfac();
	POSITION GetStartKfac();
	void GetNextKfac(POSITION& rNextPosition,T_KFAC_K& rKey,T_KFAC_D& rData);

// k-factor - Virtual Beam
public:
	void GetKfavKeyList(CArray<T_KFAV_K, T_KFAV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistKfav(T_KFAV_K Key);
	BOOL GetKfav(T_KFAV_K Key, T_KFAV_D& rData);
	int GetCountKfav();
	POSITION GetStartKfav();
	void GetNextKfav(POSITION& rNextPosition, T_KFAV_K& rKey, T_KFAV_D& rData);

// Special Load Combination...
public:
	void GetSlcmKeyList(CArray<T_SLCM_K, T_SLCM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSlcm(T_SLCM_K Key);
	BOOL GetSlcm(T_SLCM_K Key,T_SLCM_D& rData);
	int GetCountSlcm();
	POSITION GetStartSlcm();
	void GetNextSlcm(POSITION& rNextPosition,T_SLCM_K& rKey,T_SLCM_D& rData);

	// 부재별 내진 설계 타입..
public:
	void GetSdgnKeyList(CArray<T_SDGN_K, T_SDGN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSdgn(T_SDGN_K Key);
	BOOL GetSdgn(T_SDGN_K Key,T_SDGN_D& rData);
	int GetCountSdgn();
	POSITION GetStartSdgn();
	void GetNextSdgn(POSITION& rNextPosition,T_SDGN_K& rKey,T_SDGN_D& rData);

	// Underground Lcom Type
public:
	void GetUlctKeyList(CArray<T_ULCT_K, T_ULCT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistUlct(T_ULCT_K Key);
	BOOL GetUlct(T_ULCT_K Key, T_ULCT_D& rData);
	int GetCountUlct();
	POSITION GetStartUlct();
	void GetNextUlct(POSITION& rNextPosition, T_ULCT_K& rKey, T_ULCT_D& rData);

// Load cases for Othogonal Design...
public:
	BOOL ExistLcoe();
	BOOL GetLcoe(T_LCOE_D& rData);
	int GetNumLcoe();

	// Load cases for Wind Direction
public:
	BOOL ExistLcwd();
	BOOL GetLcwd(T_LCWD_D& rData);
	int GetNumLcwd();

// prestress allowable stress
public:
	void GetPsasKeyList(CArray<T_PSAS_K, T_PSAS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPsas(T_PSAS_K Key);
	BOOL GetPsas(T_PSAS_K Key,T_PSAS_D& rData);
	int GetCountPsas();
	POSITION GetStartPsas();
	void GetNextPsas(POSITION& rNextPosition,T_PSAS_K& rKey,T_PSAS_D& rData);

public:
	void GetCmftKeyList(CArray<T_CMFT_K, T_CMFT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCmft(T_CMFT_K Key);
	BOOL GetCmft(T_CMFT_K Key,T_CMFT_D& rData);
	int GetCountCmft();
	POSITION GetStartCmft();
	void GetNextCmft(POSITION& rNextPosition,T_CMFT_K& rKey,T_CMFT_D& rData);

public:
	void GetCmfvKeyList(CArray<T_CMFV_K, T_CMFV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCmfv(T_CMFV_K Key);
	BOOL GetCmfv(T_CMFV_K Key, T_CMFV_D& rData);
	int GetCountCmfv();
	POSITION GetStartCmfv();
	void GetNextCmfv(POSITION& rNextPosition, T_CMFV_K& rKey, T_CMFV_D& rData);

public:
	void GetMrftKeyList(CArray<T_MRFT_K, T_MRFT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMrft(T_MRFT_K Key);
	BOOL GetMrft(T_MRFT_K Key,T_MRFT_D& rData);
	int GetCountMrft();
	POSITION GetStartMrft();
	void GetNextMrft(POSITION& rNextPosition,T_MRFT_K& rKey,T_MRFT_D& rData);

public:
	void GetMcmbKeyList(CArray<T_MCMB_K, T_MCMB_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMcmb(T_MCMB_K Key);
	BOOL GetMcmb(T_MCMB_K Key, T_MCMB_D& rData);
	int GetCountMcmb();
	POSITION GetStartMcmb();
	void GetNextMcmb(POSITION& rNextPosition, T_MCMB_K& rKey, T_MCMB_D& rData);

public:
	void GetTrftKeyList(CArray<T_TRFT_K, T_TRFT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistTrft(T_TRFT_K Key);
	BOOL GetTrft(T_TRFT_K Key,T_TRFT_D& rData);
	int GetCountTrft();
	POSITION GetStartTrft();
	void GetNextTrft(POSITION& rNextPosition,T_TRFT_K& rKey,T_TRFT_D& rData);

public:
	void GetStanKeyList(CArray<T_STAN_K, T_STAN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStan(T_STAN_K Key);
	BOOL GetStan(T_STAN_K Key,T_STAN_D& rData);
	int GetCountStan();
	POSITION GetStartStan();
	void GetNextStan(POSITION& rNextPosition,T_STAN_K& rKey,T_STAN_D& rData);

public:
	void GetFmagKeyList(CArray<T_FMAG_K, T_FMAG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistFmag(T_FMAG_K Key);
	BOOL GetFmag(T_FMAG_K Key,T_FMAG_D& rData);
	int GetCountFmag();
	POSITION GetStartFmag();
	void GetNextFmag(POSITION& rNextPosition,T_FMAG_K& rKey,T_FMAG_D& rData);

public:
	void GetFmavKeyList(CArray<T_FMAV_K, T_FMAV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistFmav(T_FMAV_K Key);
	BOOL GetFmav(T_FMAV_K Key, T_FMAV_D& rData);
	int GetCountFmav();
	POSITION GetStartFmav();
	void GetNextFmav(POSITION& rNextPosition, T_FMAV_K& rKey, T_FMAV_D& rData);

public:
	void GetBcifKeyList(CArray<T_BCIF_K, T_BCIF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistBcif(T_BCIF_K Key);
	BOOL GetBcif(T_BCIF_K Key, T_BCIF_D& rData);
	int GetCountBcif();
	POSITION GetStartBcif();
	void GetNextBcif(POSITION& rNextPosition, T_BCIF_K& rKey, T_BCIF_D& rData);

public:
	void GetPmcrKeyList(CArray<T_PMCR_K, T_PMCR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPmcr(T_PMCR_K Key);
	BOOL GetPmcr(T_PMCR_K Key, T_PMCR_D& rData);
	int GetCountPmcr();
	POSITION GetStartPmcr();
	void GetNextPmcr(POSITION& rNextPosition, T_PMCR_K& rKey, T_PMCR_D& rData);

// Effective Length of U-frame
public:
	void GetElufKeyList(CArray<T_ELUF_K, T_ELUF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistEluf(T_ELUF_K Key);
	BOOL GetEluf(T_ELUF_K Key, T_ELUF_D& rData);
	int GetCountEluf();
	POSITION GetStartEluf();
	void GetNextEluf(POSITION& rNextPosition, T_ELUF_K& rKey, T_ELUF_D& rData);

// Moment Distribution Factor for Slenderness
public:
	void GetMdfsKeyList(CArray<T_MDFS_K, T_MDFS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMdfs(T_MDFS_K Key);
	BOOL GetMdfs(T_MDFS_K Key, T_MDFS_D& rData);
	int GetCountMdfs();
	POSITION GetStartMdfs();
	void GetNextMdfs(POSITION& rNextPosition, T_MDFS_K& rKey, T_MDFS_D& rData);

// Rivet
public:
	void GetRivtKeyList(CArray<T_RIVT_K, T_RIVT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRivt(T_RIVT_K Key);
	BOOL GetRivt(T_RIVT_K Key, T_RIVT_D& rData);
	int GetCountRivt();
	POSITION GetStartRivt();
	void GetNextRivt(POSITION& rNextPosition, T_RIVT_K& rKey, T_RIVT_D& rData);

// U-frame Resistance - Assign Member Type
public:
	void GetUfmtKeyList(CArray<T_UFMT_K, T_UFMT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistUfmt(T_UFMT_K Key);
	BOOL GetUfmt(T_UFMT_K Key, T_UFMT_D& rData);
	int GetCountUfmt();
	POSITION GetStartUfmt();
	void GetNextUfmt(POSITION& rNextPosition, T_UFMT_K& rKey, T_UFMT_D& rData);

public:
	void GetGreqKeyList(CArray<T_GREQ_K, T_GREQ_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistGreq(T_GREQ_K Key);
	BOOL GetGreq(T_GREQ_K Key,T_GREQ_D& rData);
	int GetCountGreq();
	POSITION GetStartGreq();
	void GetNextGreq(POSITION& rNextPosition,T_GREQ_K& rKey,T_GREQ_D& rData);

public:
	void GetReduKeyList(CArray<T_REDU_K, T_REDU_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRedu(T_REDU_K Key);
	BOOL GetRedu(T_REDU_K Key,T_REDU_D& rData);
	int GetCountRedu();
	POSITION GetStartRedu();
	void GetNextRedu(POSITION& rNextPosition,T_REDU_K& rKey,T_REDU_D& rData);

public:
	void GetMbtpKeyList(CArray<T_MBTP_K, T_MBTP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMbtp(T_MBTP_K Key);
	BOOL GetMbtp(T_MBTP_K Key,T_MBTP_D& rData);
	int GetCountMbtp();
	POSITION GetStartMbtp();
	void GetNextMbtp(POSITION& rNextPosition,T_MBTP_K& rKey,T_MBTP_D& rData);

public:
	void GetMbtvKeyList(CArray<T_MBTV_K, T_MBTV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMbtv(T_MBTV_K Key);
	BOOL GetMbtv(T_MBTV_K Key, T_MBTV_D& rData);
	int GetCountMbtv();
	POSITION GetStartMbtv();
	void GetNextMbtv(POSITION& rNextPosition, T_MBTV_K& rKey, T_MBTV_D& rData);

public:
	void GetSpvmKeyList(CArray<T_SPVM_K, T_SPVM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSpvm(T_SPVM_K Key);
	BOOL GetSpvm(T_SPVM_K Key, T_SPVM_D& rData);
	int GetCountSpvm();
	POSITION GetStartSpvm();
	void GetNextSpvm(POSITION& rNextPosition, T_SPVM_K& rKey, T_SPVM_D& rData);

public:
	void GetDsstKeyList(CArray<T_DSST_K, T_DSST_K>& rKeyList);
	BOOL ExistDsst(T_DSST_K Key);
	BOOL GetDsst(T_DSST_K Key, T_DSST_D& rData);
	int GetCountDsst();
	POSITION GetStartDsst();
	void GetNextDsst(POSITION& rNextPosition, T_DSST_K& rKey, T_DSST_D& rData);

public:
	void GetIsstKeyList(CArray<T_ISST_K, T_ISST_K>& rKeyList);
	BOOL ExistIsst(T_ISST_K Key);
	BOOL GetIsst(T_ISST_K Key, T_ISST_D& rData);
	int GetCountIsst();
	POSITION GetStartIsst();
	void GetNextIsst(POSITION& rNextPosition, T_ISST_K& rKey, T_ISST_D& rData);

public:
	void GetRfstKeyList(CArray<T_RFST_K, T_RFST_K>& rKeyList);
	BOOL ExistRfst(T_RFST_K Key);
	BOOL GetRfst(T_RFST_K Key, T_RFST_D& rData);
	int GetCountRfst();
	BOOL IsUsedRfst(T_RFST_K Key);
	POSITION GetStartRfst();
	void GetNextRfst(POSITION& rNextPosition, T_RFST_K& rKey, T_RFST_D& rData);
	T_RFST_K GetStartNumRfst();
	T_RFST_K GetLastNumRfst();
	T_RFST_K GetNextNumRfst(T_RFST_K StartNum);
	T_RFST_K GetStartNumRfstUnused();
	T_RFST_K GetLastNumRfstUnused();
	T_RFST_K GetNextNumRfstUnused(T_RFST_K StartNum);
	void GetKeyListRfstUser(CArray<T_RFST_K, T_RFST_K>* pKeyRfst, CArray<T_NODE_K, T_NODE_K>* pKeyNode = NULL, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem = NULL);
public:
	void GetArstKeyList(CArray<T_ARST_K, T_ARST_K>& rKeyList);
	BOOL ExistArst(T_ARST_K Key);
	BOOL GetArst(T_ARST_K Key, T_ARST_D& rData);
	int GetCountArst();
	POSITION GetStartArst();
	void GetNextArst(POSITION& rNextPosition, T_ARST_K& rKey, T_ARST_D& rData);
public:
	void GetIndfKeyList(CArray<T_INDF_K, T_INDF_K>& rKeyList);
	BOOL ExistIndf(T_INDF_K Key);
	BOOL GetIndf(T_INDF_K Key, T_INDF_D& rData);
	int GetCountIndf();
	POSITION GetStartIndf();
	void GetNextIndf(POSITION& rNextPosition, T_INDF_K& rKey, T_INDF_D& rData);
public:
	void GetMndcKeyList(CArray<T_MNDC_K, T_MNDC_K>& rKeyList);
	BOOL ExistMndc(T_MNDC_K Key);
	BOOL GetMndc(T_MNDC_K Key, T_MNDC_D& rData);
	int GetCountMndc();
	POSITION GetStartMndc();
	void GetNextMndc(POSITION& rNextPosition, T_MNDC_K& rKey, T_MNDC_D& rData);
public:
	void GetSafmKeyList(CArray<T_SAFM_K, T_SAFM_K>& rKeyList);
	BOOL ExistSafm(T_SAFM_K Key);
	BOOL GetSafm(T_SAFM_K Key, T_SAFM_D& rData);
	int GetCountSafm();
	POSITION GetStartSafm();
	void GetNextSafm(POSITION& rNextPosition, T_SAFM_K& rKey, T_SAFM_D& rData);
public:
	void Get_SP14SeisMembParams_KeyList(CArray<T_SP14SeisMembParams_K, T_SP14SeisMembParams_K>& keys_out);
	BOOL Exist_SP14SeisMembParams(T_SP14SeisMembParams_K key);
	BOOL Get_SP14SeisMembParams(T_SP14SeisMembParams_K key, T_SP14SeisMembParams_D& data_out);
	int GetCount_SP14SeisMembParams();
	POSITION GetStart_SP14SeisMembParams();
	void GetNext_SP14SeisMembParams(POSITION& pos_out, T_SP14SeisMembParams_K& key_out, T_SP14SeisMembParams_D& data_out);
public:
	void Get_SP16SafetyMembParams_KeyList(CArray<T_SP16SafetyMembParams_K, T_SP16SafetyMembParams_K>& keys_out);
	BOOL Exist_SP16SafetyMembParams(T_SP16SafetyMembParams_K key);
	BOOL Get_SP16SafetyMembParams(T_SP16SafetyMembParams_K key, T_SP16SafetyMembParams_D& data_out);
	int GetCount_SP16SafetyMembParams();
	POSITION GetStart_SP16SafetyMembParams();
	void GetNext_SP16SafetyMembParams(POSITION& pos_out, T_SP16SafetyMembParams_K& key_out, T_SP16SafetyMembParams_D& data_out);
public:
	void Get_SP16CombinedSectParams_KeyList(CArray<T_SP16CombinedSectParams_K, T_SP16CombinedSectParams_K>& keys_out);
	BOOL Exist_SP16CombinedSectParams(T_SP16CombinedSectParams_K key);
	BOOL Get_SP16CombinedSectParams(T_SP16CombinedSectParams_K key, T_SP16CombinedSectParams_D& data_out);
	int GetCount_SP16CombinedSectParams();
	POSITION GetStart_SP16CombinedSectParams();
	void GetNext_SP16CombinedSectParams(POSITION& pos_out, T_SP16CombinedSectParams_K& key_out, T_SP16CombinedSectParams_D& data_out);
public:
	void GetDsasKeyList(CArray<T_DSAS_K, T_DSAS_K>& rKeyList);
	BOOL ExistDsas(T_DSAS_K Key);
	BOOL GetDsas(T_DSAS_K Key, T_DSAS_D& rData);
	int GetCountDsas();
	POSITION GetStartDsas();
	void GetNextDsas(POSITION& rNextPosition, T_DSAS_K& rKey, T_DSAS_D& rData);
public:
	void GetDsauKeyList(CArray<T_DSAU_K, T_DSAU_K>& rKeyList);
	BOOL ExistDsau(T_DSAU_K Key);
	BOOL GetDsau(T_DSAU_K Key, T_DSAU_D& rData);
	int GetCountDsau();
	POSITION GetStartDsau();
	void GetNextDsau(POSITION& rNextPosition, T_DSAU_K& rKey, T_DSAU_D& rData);
public:
	void GetRrtrKeyList(CArray<T_RRTR_K, T_RRTR_K>& rKeyList);
	BOOL ExistRrtr(T_RRTR_K Key);
	BOOL GetRrtr(T_RRTR_K Key, T_RRTR_D& rData);
	int GetCountRrtr();
	POSITION GetStartRrtr();
	void GetNextRrtr(POSITION& rNextPosition, T_RRTR_K& rKey, T_RRTR_D& rData);
public:
	BOOL ExistMdgn();
	BOOL GetMdgn(T_MDGN_D& rData);

public:
	BOOL ExistDcfs();
	BOOL GetDcfs(T_DCFS_D& rData);
public:
	BOOL ExistDalu();
	BOOL GetDalu(T_DALU_D& rData);

public:
	BOOL ExistDstl();
	BOOL ExistDstl2();
	BOOL GetDstl(T_DSTL_D& rData);

public:
	void GetCbftKeyList(CArray<T_CBFT_K, T_CBFT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCbft(T_CBFT_K Key);
	BOOL GetCbft(T_CBFT_K Key,T_CBFT_D& rData);
	int GetCountCbft();
	POSITION GetStartCbft();
	void GetNextCbft(POSITION& rNextPosition,T_CBFT_K& rKey,T_CBFT_D& rData);

public:
	void GetJsftKeyList(CArray<T_JSFT_K, T_JSFT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistJsft(T_JSFT_K Key);
	BOOL GetJsft(T_JSFT_K Key,T_JSFT_D& rData);
	int GetCountJsft();
	POSITION GetStartJsft();
	void GetNextJsft(POSITION& rNextPosition,T_JSFT_K& rKey,T_JSFT_D& rData);

public:
	void GetCvftKeyList(CArray<T_CVFT_K, T_CVFT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCvft(T_CVFT_K Key);
	BOOL GetCvft(T_CVFT_K Key,T_CVFT_D& rData);
	int GetCountCvft();
	POSITION GetStartCvft();
	void GetNextCvft(POSITION& rNextPosition,T_CVFT_K& rKey,T_CVFT_D& rData);

public:
	void GetDalwKeyList(CArray<T_DALW_K, T_DALW_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDalw(T_DALW_K Key);
	BOOL GetDalw(T_DALW_K Key,T_DALW_D& rData);
	int GetCountDalw();
	POSITION GetStartDalw();
	void GetNextDalw(POSITION& rNextPosition,T_DALW_K& rKey,T_DALW_D& rData);

public:
	void GetUtcfKeyList(CArray<T_UTCF_K, T_UTCF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistUtcf(T_UTCF_K Key);
	BOOL GetUtcf(T_UTCF_K Key,T_UTCF_D& rData);
	int GetCountUtcf();
	POSITION GetStartUtcf();
	void GetNextUtcf(POSITION& rNextPosition,T_UTCF_K& rKey,T_UTCF_D& rData);

public:
	void GetLtsrKeyList(CArray<T_LTSR_K, T_LTSR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLtsr(T_LTSR_K Key);
	BOOL GetLtsr(T_LTSR_K Key,T_LTSR_D& rData);
	int GetCountLtsr();
	POSITION GetStartLtsr();
	void GetNextLtsr(POSITION& rNextPosition,T_LTSR_K& rKey,T_LTSR_D& rData);

public:
	void GetLtsvKeyList(CArray<T_LTSV_K, T_LTSV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLtsv(T_LTSV_K Key);
	BOOL GetLtsv(T_LTSV_K Key, T_LTSV_D& rData);
	int GetCountLtsv();
	POSITION GetStartLtsv();
	void GetNextLtsv(POSITION& rNextPosition, T_LTSV_K& rKey, T_LTSV_D& rData);

public:
	void GetStfnKeyList(CArray<T_STFN_K, T_STFN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStfn(T_STFN_K Key);
	BOOL GetStfn(T_STFN_K Key,T_STFN_D& rData);
	int GetCountStfn();
	POSITION GetStartStfn();
	void GetNextStfn(POSITION& rNextPosition,T_STFN_K& rKey,T_STFN_D& rData);

public:
	BOOL ExistDcon();
	BOOL GetDcon(T_DCON_D& rData);
	void ConvertQO_Before881(T_DCON_D& rDcon);
	BOOL IsDcon_CrackForAASHTO();

public:
	BOOL ExistDcrb();
	BOOL GetDcrb(T_DCRB_D& rData);

public:
	BOOL ExistMsrb();
	BOOL GetMsrb(T_MSRB_D& rData);

public:
	void GetDcbbKeyList(CArray<T_DCBB_K, T_DCBB_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDcbb(T_DCBB_K Key);
	BOOL GetDcbb(T_DCBB_K Key,T_DCBB_D& rData);
	int GetCountDcbb();
	POSITION GetStartDcbb();
	void GetNextDcbb(POSITION& rNextPosition,T_DCBB_K& rKey,T_DCBB_D& rData);

public:
	void GetDcbcKeyList(CArray<T_DCBC_K, T_DCBC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDcbc(T_DCBC_K Key);
	BOOL GetDcbc(T_DCBC_K Key,T_DCBC_D& rData);
	int GetCountDcbc();
	POSITION GetStartDcbc();
	void GetNextDcbc(POSITION& rNextPosition,T_DCBC_K& rKey,T_DCBC_D& rData);

public:
	void GetDcbrKeyList(CArray<T_DCBR_K, T_DCBR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDcbr(T_DCBR_K Key);
	BOOL GetDcbr(T_DCBR_K Key,T_DCBR_D& rData);
	int GetCountDcbr();
	POSITION GetStartDcbr();
	void GetNextDcbr(POSITION& rNextPosition,T_DCBR_K& rKey,T_DCBR_D& rData);

public:
	BOOL ExistDcbw(T_DCBW_K Key);
	BOOL GetDcbw(T_DCBW_K Key,T_DCBW_D& rData);
	int GetCountDcbw();
	POSITION GetStartDcbw();
	void GetNextDcbw(POSITION& rNextPosition,T_DCBW_K& rKey,T_DCBW_D& rData);


public:
	int GetRebbKeyList(CArray<T_REBB_K, T_REBB_K>& rKeyList);  //Section Key 번호로 정렬된 리스트
	BOOL ExistRebb(T_REBB_K Key);
	BOOL GetRebb(T_REBB_K Key,T_REBB_D& rData);
	BOOL ExistRebb(UINT SectK);
	BOOL GetRebb(UINT SectK,T_REBB_D& rData);
	int GetCountRebb();
	POSITION GetStartRebb();
	void GetNextRebb(POSITION& rNextPosition,T_REBB_K& rKey,T_REBB_D& rData);
	int GetElemKeyListUseRebb(T_REBB_K RebbK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);      // 대표요소만
	int GetAllElemKeyListUseRebb(T_REBB_K RebbK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);   // member로 지정 된 전체요소
	int GetRebbKeyListUsingSectK(T_SECT_K SectK, CArray<T_REBB_K, T_REBB_K>& rKeyList);
	BOOL GetRebbByElemK(T_ELEM_K ElemK, T_REBB_D& rData);  // Add by GAY. PMS:4073. ('09.09.28).
	T_REBB_K GetRebbByElemK(T_ELEM_K ElemK);  

public:
	void GetRbsbKeyList(CArray<T_RBSB_K, T_RBSB_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRbsb(T_RBSB_K Key);
	BOOL GetRbsb(T_RBSB_K Key,T_RBSB_D& rData);
	int GetCountRbsb();
	POSITION GetStartRbsb();
	void GetNextRbsb(POSITION& rNextPosition,T_RBSB_K& rKey,T_RBSB_D& rData);

public:
	int GetRebcKeyList(CArray<T_REBC_K, T_REBC_K>& rKeyList);  //Section Key 번호로 정렬된 리스트
	BOOL ExistRebc(T_REBC_K Key);
	BOOL GetRebc(T_REBC_K Key,T_REBC_D& rData);
	BOOL ExistRebc(UINT SectK);
	BOOL GetRebc(UINT SectK,T_REBC_D& rData);
	int GetCountRebc();
	POSITION GetStartRebc();
	void GetNextRebc(POSITION& rNextPosition,T_REBC_K& rKey,T_REBC_D& rData);
	int GetElemKeyListUseRebc(T_REBC_K RebcK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	int GetAllElemKeyListUseRebc(T_REBC_K RebcK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	int GetRebcKeyListUsingSectK(T_SECT_K SectK, CArray<T_REBC_K, T_REBC_K>& rKeyList);
	BOOL GetRebcByElemK(T_ELEM_K ElemK, T_REBC_D& rData);  // Add by GAY. PMS:4073. ('09.10.14).
	T_REBC_K GetRebcByElemK(T_ELEM_K ElemK);

public:
	int GetRebrKeyList(CArray<T_REBR_K, T_REBR_K>& rKeyList);  //Section Key 번호로 정렬된 리스트
	BOOL ExistRebr(T_REBR_K Key);
	BOOL GetRebr(T_REBR_K Key,T_REBR_D& rData);
	BOOL ExistRebr(UINT SectK);
	BOOL GetRebr(UINT SectK,T_REBR_D& rData);
	int GetCountRebr();
	POSITION GetStartRebr();
	void GetNextRebr(POSITION& rNextPosition,T_REBR_K& rKey,T_REBR_D& rData);
	int GetElemKeyListUseRebr(T_REBR_K RebrK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	int GetAllElemKeyListUseRebr(T_REBR_K RebrK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	int GetRebrKeyListUsingSectK(T_SECT_K SectK, CArray<T_REBR_K, T_REBR_K>& rKeyList);
	BOOL GetRebrByElemK(T_ELEM_K ElemK, T_REBR_D& rData); // Add by GAY. PMS:4073. ('09.10.14).
	T_REBR_K GetRebrByElemK(T_ELEM_K ElemK);

// wall rebar
public:
	int GetRebwKeyList(CArray<T_REBW_K, T_REBW_K>& rKeyList);  // Wall ID, Stor ID 순으로 정렬 된 리스트
	BOOL ExistWallID(int WallID);
	BOOL ExistRebw(T_REBW_K Key);     // 실제 유효한 Data가 있는 지 확인하려면 ExistRebwData 사용
	BOOL ExistRebwData(T_REBW_K Key);
	BOOL GetRebw(T_REBW_K Key,T_REBW_D& rData);
	int GetCountRebw();
	POSITION GetStartRebw();
	void GetNextRebw(POSITION& rNextPosition,T_REBW_K& rKey,T_REBW_D& rData);
	int GetElemKeyListUseRebw(T_REBW_K RebwK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	int GetRebwKeyListUsingWallID(int nWallID, CArray<T_REBW_K, T_REBW_K>& rKeyList);  
	int GetSortedRebwKey(CArray<T_REBW_K,T_REBW_K>& rKeyList);
	
	// meshed wall rebar
public:
	int GetRbmwKeyList(CArray<T_RBMW_K, T_RBMW_K>& rKeyList);
	BOOL ExistRbmw(T_RBMW_K Key);
	BOOL GetRbmw(T_RBMW_K Key,T_RBMW_D& rData);
	int GetCountRbmw();
	POSITION GetStartRbmw();
	void GetNextRbmw(POSITION& rNextPosition,T_RBMW_K& rKey,T_RBMW_D& rData);
	T_RBMW_K GetRbmwKey(CString& strName);
	T_RBMW_K GetStartNumRbmw();
	T_RBMW_K GetLastNumRbmw();
	T_RBMW_K GetNextNumRbmw(T_RBMW_K StartNum);
	int GetElemKeyListUseRbmw(T_RBMW_K RbmwK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	BOOL GetRbmwByElemK(T_ELEM_K ElemK, T_RBMW_D& rData);

	// meshed slab rebar
public:
	int GetRbmsKeyList(CArray<T_RBMS_K, T_RBMS_K>& rKeyList, int nLayer=-1);
	BOOL ExistRbms(T_RBMS_K Key);
	BOOL GetRbms(T_RBMS_K Key,T_RBMS_D& rData);
	int GetCountRbms();
	POSITION GetStartRbms();
	void GetNextRbms(POSITION& rNextPosition,T_RBMS_K& rKey,T_RBMS_D& rData);
	T_RBMS_K GetRbmsKey(CString& strName);
	T_RBMS_K GetStartNumRbms();
	T_RBMS_K GetLastNumRbms();
	T_RBMS_K GetNextNumRbms(T_RBMS_K StartNum);
	int GetElemKeyListUseRbms(T_RBMS_K RbmsK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	BOOL GetRbmsByElemK(T_ELEM_K ElemK, int nLayer, T_RBMS_D& rData);  

// Reinforcing Bar for Shell
public:
	int GetRbslKeyList(CArray<T_RBSL_K, T_RBSL_K>& rKeyList, int nLayer=-1);
	BOOL ExistRbsl(T_RBSL_K Key);
	BOOL ExistRbsl(CString& strName);
	BOOL GetRbsl(T_RBSL_K Key,T_RBSL_D& rData);
	int GetCountRbsl();
	POSITION GetStartRbsl();
	void GetNextRbsl(POSITION& rNextPosition,T_RBSL_K& rKey,T_RBSL_D& rData);
	T_RBSL_K GetRbslKey(CString& strName);
	T_RBSL_K GetStartNumRbsl();
	T_RBSL_K GetLastNumRbsl();
	T_RBSL_K GetNextNumRbsl(T_RBSL_K StartNum);
	int GetElemKeyListUseRbsl(T_RBSL_K RbslK, CArray<T_ELEM_K, T_ELEM_K>& rKeyList);
	BOOL GetRbslByElemK(T_ELEM_K ElemK, int nLayer, T_RBSL_D& rData);  
	BOOL ChangeRbslInMreb(T_MREB_D& MrebD, T_RBSL_K RbslK);
	BOOL GetRbslPosInMreb(T_MREB_D& MrebD, BOOL bRbslPos[4]);

// wall mark
public:
	void GetWmakList(CArray<CString, CString&>& rstrWmakList);  //serial 번호로 정렬된 리스트
	void GetWmakKeyList(CArray<T_WMAK_K, T_WMAK_K>& rKeyList);  //serial 번호로 정렬된 리스트
	BOOL ExistWmak(CString& rMarkName);
	BOOL ExistWmak(T_WMAK_K Key);
	BOOL ExistUsrw(int nUserWallId);
	BOOL GetWmak(CString& rMarkName, T_WMAK_D& rData);
	BOOL GetWmak(T_WMAK_K Key, T_WMAK_D& rData);
	T_WMAK_K GetWmakKey(CString& rMarkName);
	T_WMAK_K GetWmakKey(unsigned int nUserWallId);
	int GetCountWmak();
	POSITION GetStartWmak();
	void GetNextWmak(POSITION& rNextPosition,T_WMAK_K& rKey,T_WMAK_D& rData);
	T_WMAK_K GetStartNumWmak();
	T_WMAK_K GetLastNumWmak();
	T_WMAK_K GetWmakKeyByUserWallID(int nUserWallID);

// src
public:
	BOOL ExistDsrc();
	BOOL GetDsrc(T_DSRC_D& rData);

public:
	void GetRebsKeyList(CArray<T_REBS_K, T_REBS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRebs(T_REBS_K Key);
	BOOL GetRebs(T_REBS_K Key,T_REBS_D& rData);
	int GetCountRebs();
	POSITION GetStartRebs();
	void GetNextRebs(POSITION& rNextPosition,T_REBS_K& rKey,T_REBS_D& rData);

// rebar for beam/column/brace checking
public:
	void GetRchkKeyList(CArray<T_RCHK_K, T_RCHK_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRchk(T_RCHK_K Key);
	BOOL GetRchk(T_RCHK_K Key,T_RCHK_D& rData);
	int GetCountRchk();
	POSITION GetStartRchk();
	void GetNextRchk(POSITION& rNextPosition,T_RCHK_K& rKey,T_RCHK_D& rData);
	T_RCHK_K GetRchkByElemK(T_ELEM_K ElemK);

// rebar for beam/column/brace design
public:
	void GetRdgnKeyList(CArray<T_RDGN_K, T_RDGN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRdgn(T_RDGN_K Key);
	BOOL GetRdgn(T_RDGN_K Key,T_RDGN_D& rData);
	int GetCountRdgn();
	POSITION GetStartRdgn();
	void GetNextRdgn(POSITION& rNextPosition,T_RDGN_K& rKey,T_RDGN_D& rData);

// scale up factor for column
public:
	void GetSufcKeyList(CArray<T_SUFC_K, T_SUFC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSufc(T_SUFC_K Key);
	BOOL GetSufc(T_SUFC_K Key,T_SUFC_D& rData);
	int GetCountSufc();
	POSITION GetStartSufc();
	void GetNextSufc(POSITION& rNextPosition,T_SUFC_K& rKey,T_SUFC_D& rData);

// scale up factor for eq
public:
	void GetSueqKeyList(CArray<T_SUEQ_K, T_SUEQ_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSueq(T_SUEQ_K Key);
	BOOL GetSueq(T_SUEQ_K Key,T_SUEQ_D& rData);
	int GetCountSueq();
	POSITION GetStartSueq();
	void GetNextSueq(POSITION& rNextPosition,T_SUEQ_K& rKey,T_SUEQ_D& rData);
	int  GetElnkKeyListBetween2Nodes(T_NODE_K NodeK1, T_NODE_K NodeK2, T_BNGR_K bngrK,
		CArray<T_ELNK_K, T_ELNK_K>& rKeyList);
	int  GetElnk(T_NODE_K NodeK1,CArray<T_ELNK_K, T_ELNK_K>& rKeyList);

// Response Modification Factor (R)
public:
	void GetRmfrKeyList(CArray<T_RMFR_K, T_RMFR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRmfr(T_RMFR_K Key);
	BOOL GetRmfr(T_RMFR_K Key, T_RMFR_D& rData);
	int GetCountRmfr();
	POSITION GetStartRmfr();
	void GetNextRmfr(POSITION& rNextPosition, T_RMFR_K& rKey, T_RMFR_D& rData);

// Member
public:
	void GetMembKeyList(CArray<T_MEMB_K, T_MEMB_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetKeyListMembUser(CArray<T_MEMB_K, T_MEMB_K>* pKeyMemb,
									 CArray<T_NODE_K, T_NODE_K>* pKeyNode, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem);
	int  GetMembNodeList(const T_MEMB_DT &lstElemInMemb, T_KEY_LIST &lstSortNode);
	BOOL ExistMemb(T_MEMB_K Key);
	BOOL GetMemb(T_MEMB_K Key,T_MEMB_D& rData);
	int GetCountMemb();
	T_MEMB_K FindMembByName(LPCTSTR sName);
	POSITION GetStartMemb();
	void GetNextMemb(POSITION& rNextPosition,T_MEMB_K& rKey,T_MEMB_D& rData);
	T_MEMB_K GetStartNumMemb();
	T_MEMB_K GetLastNumMemb();
	BOOL ExistMembAssigned(T_ELEM_K KeyElem);
	BOOL GetMembAssigned(T_ELEM_K KeyElem, T_MEMB_K& rKeyMemb);
	BOOL CheckMembCompatibility(T_MEMB_K Key, T_MEMB_D& rData, BOOL bTargetCH = FALSE);
	BOOL AlignMembElemList(T_MEMB_K Key, T_MEMB_D& rData);
	BOOL IsMatchDirectionAttribute(T_MEMB_D& rData, BOOL bTargetCH = FALSE);
	BOOL CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL IsInterval45DegElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL IsInterval45DegVsecDirection(T_VSEC_D VsecD, T_ELEM_K KeyNew);
	BOOL CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew, BOOL bTargetCH=FALSE);
	void GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz);
	BOOL IsNegativeMomentMember(T_ELEM_K ElemKey);
	BOOL IsNegativeMomentMember(T_MEMB_D& rMembD);
	BOOL IsColumnBraceWallAttachedUnder(T_NODE_K NodeK);
	int GetBeamMemberKind( const T_MEMB_D MembD ); // Add Hong[06.1.25] for DShop Export
	void AlignElemListInOrder(T_MEMB_D& rData);

	void GetLinkedColm(CArray<T_ELEM_K, T_ELEM_K>& aElemKey);
	int GetMembKListFromElem(const CArray<T_ELEM_K,T_ELEM_K>& aElemKey, CArray<T_MEMB_K,T_MEMB_K>& aMembKey); // 입력된 aElemKey들의 멤버키를 리턴
	T_ELEM_K GetFirstElemKInMemb(T_ELEM_K ElemK); // Member내 요소 중 첫번째 키(대표요소)키를 가져온다.
	int GetElemKByMemb(T_MEMB_K MembK, CArray<T_ELEM_K, T_ELEM_K>& aElemKey); // 할당된 Member 안의 요소키를 찾아옴. Member가 아니면 자기 키 리턴
	BOOL IsMembStraight(T_MEMB_K MembK);
	
// PSeg
	void GetPsegKeyList(CArray<T_PSEG_K, T_PSEG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetKeyListPsegUser(CArray<T_PSEG_K, T_PSEG_K>* pKeyPseg,
									 CArray<T_NODE_K, T_NODE_K>* pKeyNode, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem);
	BOOL ExistPseg(T_PSEG_K Key);
	BOOL GetPseg(T_PSEG_K Key,T_PSEG_D& rData);
	int GetCountPseg();
	POSITION GetStartPseg();
	void GetNextPseg(POSITION& rNextPosition,T_PSEG_K& rKey,T_PSEG_D& rData);
	T_PSEG_K GetStartNumPseg();
	T_PSEG_K GetLastNumPseg();
	BOOL ExistPsegAssigned(T_ELEM_K KeyElem);
	BOOL GetPsegAssigned(T_ELEM_K KeyElem, T_PSEG_K& rKeyMemb);
	BOOL CheckPsegCompatibility(T_PSEG_K Key, T_PSEG_D& rData);
	BOOL AlignPsegElemList(T_PSEG_K Key, T_PSEG_D& rData);

// Haunched Beam
public:
	void GetHcbmKeyList(CArray<T_HCBM_K, T_HCBM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	void GetKeyListHcbmUser(CArray<T_HCBM_K, T_HCBM_K>* pKeyHcbm,
									 CArray<T_NODE_K, T_NODE_K>* pKeyNode, CArray<T_ELEM_K, T_ELEM_K>* pKeyElem);
	BOOL ExistHcbm(T_HCBM_K Key);
	BOOL GetHcbm(T_HCBM_K Key,T_HCBM_D& rData);
	int GetCountHcbm();
	POSITION GetStartHcbm();
	void GetNextHcbm(POSITION& rNextPosition,T_HCBM_K& rKey,T_HCBM_D& rData);
	T_HCBM_K GetStartNumHcbm();
	T_HCBM_K GetLastNumHcbm();
	BOOL ExistHcbmAssigned(T_ELEM_K KeyElem);
	BOOL GetHcbmAssigned(T_ELEM_K KeyElem, T_HCBM_K& rKeyHcbm);
	BOOL CheckHcbmCompatibility(T_HCBM_K Key, T_HCBM_D& rData);
	BOOL AlignHcbmElemList(T_HCBM_K Key, T_HCBM_D& rData);
	BOOL IsMatchDirectionAttribute4Hcbm(T_HCBM_D& rData);
	BOOL CompareElemAttribute4Hcbm(T_ELEM_K KeyRef, T_ELEM_K KeyNew, BOOL bIsHaunchPart=FALSE);
//	BOOL CheckSectShape4Hcbm(T_ELEM_K ElemKey);

// Horizontal Displacement Control
public:
	BOOL ExistHdct();
	BOOL GetHdct(T_HDCT_D& rData);

// Horizontal Displacement Global Parameter
public:
	BOOL ExistHdgp();
	BOOL GetHdgp(T_HDGP_D& rData);

public:
	void GetHdppKeyList(CArray<T_HDPP_K, T_HDPP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistHdpp(T_HDPP_K Key);
	BOOL GetHdpp(T_HDPP_K Key,T_HDPP_D& rData);
	int GetCountHdpp();
	POSITION GetStartHdpp();
	void GetNextHdpp(POSITION& rNextPosition,T_HDPP_K& rKey,T_HDPP_D& rData);

// Load Combination for RC Beam(KCI-USD03)
public:
	BOOL ExistRcbg();
	BOOL GetRcbg(T_RCBG_D& rData);

// Design Parameter for RC Beam
public:
	void GetRcbeKeyList(CArray<T_RCBE_K, T_RCBE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRcbe(T_RCBE_K Key);
	BOOL GetRcbe(T_RCBE_K Key,T_RCBE_D& rData);
	int GetCountRcbe();
	POSITION GetStartRcbe();
	void GetNextRcbe(POSITION& rNextPosition,T_RCBE_K& rKey,T_RCBE_D& rData);

// Load Combination for RC Column
public:
	BOOL ExistRccg();
	BOOL GetRccg(T_RCCG_D& rData);

// Design Parameter for RC Column
public:
	void GetRccoKeyList(CArray<T_RCCO_K, T_RCCO_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRcco(T_RCCO_K Key);
	BOOL GetRcco(T_RCCO_K Key,T_RCCO_D& rData);
	int GetCountRcco();
	POSITION GetStartRcco();
	void GetNextRcco(POSITION& rNextPosition,T_RCCO_K& rKey,T_RCCO_D& rData);

// Checking Parameger for RC Beam
public:
	void GetCpbeKeyList(CArray<T_CPBE_K, T_CPBE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCpbe(T_CPBE_K Key);
	BOOL GetCpbe(T_CPBE_K Key,T_CPBE_D& rData);
	int GetCountCpbe();
	POSITION GetStartCpbe();
	void GetNextCpbe(POSITION& rNextPosition,T_CPBE_K& rKey,T_CPBE_D& rData);
// Checking Parameter for RC Column(KSCE-USD05)

	public:
	void GetCpcoKeyList(CArray<T_CPCO_K, T_CPCO_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCpco(T_CPCO_K Key);
	BOOL GetCpco(T_CPCO_K Key,T_CPCO_D& rData);
	int GetCountCpco();
	POSITION GetStartCpco();
	void GetNextCpco(POSITION& rNextPosition,T_CPCO_K& rKey,T_CPCO_D& rData);
// Rebar Data for Section
public:
	void GetRebtKeyList(CArray<T_REBT_K, T_REBT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRebt(T_REBT_K Key);
	BOOL GetRebt(T_REBT_K Key,T_REBT_D& rData);
	int GetCountRebt();
	POSITION GetStartRebt();
	void GetNextRebt(POSITION& rNextPosition,T_REBT_K& rKey,T_REBT_D& rData);
// Rebar Data for Element
public:
	void GetRbteKeyList(CArray<T_RBTE_K, T_RBTE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRbte(T_RBTE_K Key);
	BOOL GetRbte(T_RBTE_K Key,T_RBTE_D& rData);
	int GetCountRbte();
	POSITION GetStartRbte();
	void GetNextRbte(POSITION& rNextPosition,T_RBTE_K& rKey,T_RBTE_D& rData);

// Rating Case 입력을 위한 구조체
public:
	void GetBlrcKeyList(CArray<T_BLRC_K, T_BLRC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistBlrc(T_BLRC_K Key);
	BOOL GetBlrc(T_BLRC_K Key,T_BLRC_D& rData);
	int GetCountBlrc();
	POSITION GetStartBlrc();
	void GetNextBlrc(POSITION& rNextPosition,T_BLRC_K& rKey,T_BLRC_D& rData);

// Rating Parameter
public:
	BOOL ExistBlrs();
	BOOL GetBlrs(T_BLRS_D& rData);

// Assessment Parameter
public:
	BOOL ExistAspm();
	BOOL GetAspm(T_ASPM_D& rData);

// Group별 입력
public:
	void GetBlrgKeyList(CArray<T_BLRG_K, T_BLRG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistBlrg(T_BLRG_K Key);
	BOOL GetBlrg(T_BLRG_K Key,T_BLRG_D& rData);
	int GetCountBlrg();
	POSITION GetStartBlrg();
	void GetNextBlrg(POSITION& rNextPosition,T_BLRG_K& rKey,T_BLRG_D& rData);

// Serviceability Parameters
public:
	void GetServKeyList(CArray<T_SERV_K, T_SERV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistServ(T_SERV_K Key);
	BOOL GetServ(T_SERV_K Key,T_SERV_D& rData);
	int  GetCountServ();
	POSITION GetStartServ();
	void GetNextServ(POSITION& rNextPosition,T_SERV_K& rKey,T_SERV_D& rData);

	// 슬래브 전단 보강의 설정
public:
	void GetRessKeyList(CArray<T_RESS_K, T_RESS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRess(T_RESS_K Key);
	BOOL GetRess(T_RESS_K Key, T_RESS_D& rData);
	int  GetCountRess();
	POSITION GetStartRess();
	void GetNextRess(POSITION& rNextPosition, T_RESS_K& rKey, T_RESS_D& rData);
// EC2
public:
	void GetUlecKeyList(CArray<T_ULEC_K, T_ULEC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistUlec(T_ULEC_K Key);
	BOOL GetUlec(T_ULEC_K Key,T_ULEC_D& rData);
	int  GetCountUlec();
	POSITION GetStartUlec();
	void GetNextUlec(POSITION& rNextPosition,T_ULEC_K& rKey,T_ULEC_D& rData);

// plate girder
public:
	void GetCgtsKeyList(CArray<T_CGTS_K, T_CGTS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgts(T_CGTS_K Key);
	BOOL GetCgts(T_CGTS_K Key,T_CGTS_D& rData);
	int GetCountCgts();
	POSITION GetStartCgts();
	void GetNextCgts(POSITION& rNextPosition,T_CGTS_K& rKey,T_CGTS_D& rData);

public:
	void GetCgtvKeyList(CArray<T_CGTV_K, T_CGTV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgtv(T_CGTV_K Key);
	BOOL GetCgtv(T_CGTV_K Key, T_CGTV_D& rData);
	int GetCountCgtv();
	POSITION GetStartCgtv();
	void GetNextCgtv(POSITION& rNextPosition, T_CGTV_K& rKey, T_CGTV_D& rData);

public:
	void GetCgteKeyList(CArray<T_CGTE_K, T_CGTE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgte(T_CGTE_K Key);
	BOOL GetCgte(T_CGTE_K Key,T_CGTE_D& rData);
	int GetCountCgte();
	POSITION GetStartCgte();
	void GetNextCgte(POSITION& rNextPosition,T_CGTE_K& rKey,T_CGTE_D& rData);

public:
	void GetCgteKeyList_IRC(CArray<T_CGTE_IRC_K, T_CGTE_IRC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgte_IRC(T_CGTE_IRC_K Key);
	BOOL GetCgte_IRC(T_CGTE_IRC_K Key,T_CGTE_IRC_D& rData);
	int GetCountCgte_IRC();
	POSITION GetStartCgte_IRC();
	void GetNextCgte_IRC(POSITION& rNextPosition,T_CGTE_IRC_K& rKey,T_CGTE_IRC_D& rData);

public:
	BOOL GetCgsc_IRC(T_CGSC_K Key,T_CGSC_D& rData);
	void GetNextCgsc_IRC(POSITION& rNextPosition,T_CGSC_K& rKey,T_CGSC_D& rData);

public:
	BOOL GetCgsv_IRC(T_CGSV_K Key, T_CGSV_D& rData);
	void GetNextCgsv_IRC(POSITION& rNextPosition, T_CGSV_K& rKey, T_CGSV_D& rData);

public:
	void GetCgscKeyList(CArray<T_CGSC_K, T_CGSC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgsc(T_CGSC_K Key);
	BOOL GetCgsc(T_CGSC_K Key,T_CGSC_D& rData);
	int GetCountCgsc();
	POSITION GetStartCgsc();
	void GetNextCgsc(POSITION& rNextPosition,T_CGSC_K& rKey,T_CGSC_D& rData);

public:
	void GetCgsvKeyList(CArray<T_CGSV_K, T_CGSV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgsv(T_CGSV_K Key);
	BOOL GetCgsv(T_CGSV_K Key, T_CGSV_D& rData);
	int GetCountCgsv();
	POSITION GetStartCgsv();
	void GetNextCgsv(POSITION& rNextPosition, T_CGSV_K& rKey, T_CGSV_D& rData);

public:
	void GetCgdpKeyList(CArray<T_CGDP_K, T_CGDP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgdp(T_CGDP_K Key);
	BOOL GetCgdp(T_CGDP_K Key,T_CGDP_D& rData);
	int GetCountCgdp();
	POSITION GetStartCgdp();
	void GetNextCgdp(POSITION& rNextPosition,T_CGDP_K& rKey,T_CGDP_D& rData);

public:
	void GetCgdvKeyList(CArray<T_CGDV_K, T_CGDV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgdv(T_CGDV_K Key);
	BOOL GetCgdv(T_CGDV_K Key, T_CGDV_D& rData);
	int GetCountCgdv();
	POSITION GetStartCgdv();
	void GetNextCgdv(POSITION& rNextPosition, T_CGDV_K& rKey, T_CGDV_D& rData);
	//
	void GetCgdpOrCgdvKeyList(CArray<T_CGDP_K, T_CGDP_K>& rKeyList, BOOL bVirtual=FALSE);
	BOOL GetCgdpOrCgdv(T_CGDP_K Key, T_CGDP_D& rData, BOOL bVirtual=FALSE);

public:
	void GetClrsKeyList(CArray<T_CLRS_K, T_CLRS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistClrs(T_CLRS_K Key);
	BOOL GetClrs(T_CLRS_K Key, T_CLRS_D& rData);
	int GetCountClrs();
	POSITION GetStartClrs();
	void GetNextClrs(POSITION& rNextPosition,T_CLRS_K& rKey,T_CLRS_D& rData);

public:
	BOOL ExistCpgd();
	BOOL GetCpgd(T_CPGD_D& rData);
	BOOL GetDgnCpgd(T_CPGD_D& rData);	

public:
	void GetCptfKeyList(CArray<T_CPTF_K, T_CPTF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCptf(T_CPTF_K Key);
	BOOL GetCptf(T_CPTF_K Key,T_CPTF_D& rData);
	int GetCountCptf();
	POSITION GetStartCptf();
	void GetNextCptf(POSITION& rNextPosition,T_CPTF_K& rKey,T_CPTF_D& rData);

public:
	void GetCptvKeyList(CArray<T_CPTV_K, T_CPTV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCptv(T_CPTV_K Key);
	BOOL GetCptv(T_CPTV_K Key, T_CPTV_D& rData);
	int GetCountCptv();
	POSITION GetStartCptv();
	void GetNextCptv(POSITION& rNextPosition, T_CPTV_K& rKey, T_CPTV_D& rData);

public:
	void GetCgltKeyList(CArray<T_CGLT_K, T_CGLT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCglt(T_CGLT_K Key);
	BOOL GetCglt(T_CGLT_K Key,T_CGLT_D& rData);
	int GetCountCglt();
	POSITION GetStartCglt();
	void GetNextCglt(POSITION& rNextPosition,T_CGLT_K& rKey,T_CGLT_D& rData);

public:
	void GetCglvKeyList(CArray<T_CGLV_K, T_CGLV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCglv(T_CGLV_K Key);
	BOOL GetCglv(T_CGLV_K Key, T_CGLV_D& rData);
	int GetCountCglv();
	POSITION GetStartCglv();
	void GetNextCglv(POSITION& rNextPosition, T_CGLV_K& rKey, T_CGLV_D& rData);

	//
	void GetCgltOrCglvKeyList(CArray<T_CGLT_K, T_CGLT_K>& rKeyList, BOOL bVirtual=FALSE);
	BOOL GetCgltOrCglv(T_CGLT_K Key, T_CGLT_D& rData, BOOL bVirtual=FALSE);

public:
	void GetCgfrKeyList(CArray<T_CGFR_K, T_CGFR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgfr(T_CGFR_K Key);
	BOOL GetCgfr(T_CGFR_K Key,T_CGFR_D& rData);
	int GetCountCgfr();
	POSITION GetStartCgfr();
	void GetNextCgfr(POSITION& rNextPosition,T_CGFR_K& rKey,T_CGFR_D& rData);

public:
	void GetCgfvKeyList(CArray<T_CGFV_K, T_CGFV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCgfv(T_CGFV_K Key);
	BOOL GetCgfv(T_CGFV_K Key, T_CGFV_D& rData);
	int GetCountCgfv();
	POSITION GetStartCgfv();
	void GetNextCgfv(POSITION& rNextPosition, T_CGFV_K& rKey, T_CGFV_D& rData);

//China RC Design Parameter
public:
	BOOL ExistChrp();
	BOOL GetChrp(T_CHRP_D& rData);

	public:
	BOOL ExistRcpg();
	BOOL GetRcpg(T_RCPG_D& rData);
public:
	BOOL ExistStlc();
	BOOL GetStlc(T_STLC_D& rData);
public:
	BOOL ExistRcwg();
	BOOL GetRcwg(T_RCWG_D& rData);

public:
	void GetStdpKeyList(CArray<T_STDP_K, T_STDP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStdp(T_STDP_K Key);
	BOOL GetStdp(T_STDP_K Key,T_STDP_D& rData);
	int GetCountStdp();
	POSITION GetStartStdp();
	void GetNextStdp(POSITION& rNextPosition,T_STDP_K& rKey,T_STDP_D& rData);

// Allowable Stress Upper Limit
public:
	void GetStusKeyList(CArray<T_STUS_K, T_STUS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStus(T_STUS_K Key);
	BOOL GetStus(T_STUS_K Key, T_STUS_D& rData);
	int GetCountStus();
	POSITION GetStartStus();
	void GetNextStus(POSITION& rNextPosition, T_STUS_K& rKey, T_STUS_D& rData);

// Allowable Stress Reduction
public:
	void GetStreKeyList(CArray<T_STRE_K, T_STRE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStre(T_STRE_K Key);
	BOOL GetStre(T_STRE_K Key, T_STRE_D& rData);
	int GetCountStre();
	POSITION GetStartStre();
	void GetNextStre(POSITION& rNextPosition, T_STRE_K& rKey, T_STRE_D& rData);

//  steel design Bolt Section Deduction
public:
	void GetStbdKeyList(CArray<T_STBD_K, T_STBD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistStbd(T_STBD_K Key);
	BOOL GetStbd(T_STBD_K Key, T_STBD_D& rData);
	int GetCountStbd();
	POSITION GetStartStbd();
	void GetNextStbd(POSITION& rNextPosition, T_STBD_K& rKey, T_STBD_D& rData);

public:
	void GetRcwaKeyList(CArray<T_RCWA_K, T_RCWA_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRcwa(T_RCWA_K Key);
	BOOL GetRcwa(T_RCWA_K Key,T_RCWA_D& rData);
	int GetCountRcwa();
	POSITION GetStartRcwa();
	void GetNextRcwa(POSITION& rNextPosition,T_RCWA_K& rKey,T_RCWA_D& rData);
public:
	void GetRcplList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetRcplKeyList(CArray<T_RCPL_K, T_RCPL_K>& rKeyList); //Name으로 정렬된 리스트
	void GetRcplKeyListHasElem(T_ELEM_K ElemK, CArray<T_RCPL_K, T_RCPL_K>& rKeyList); // Elem 을 가진 RCPL
	BOOL ExistRcpl(CString& rName);
	BOOL ExistRcpl(T_RCPL_K Key);
	BOOL GetRcpl(CString& rName, T_RCPL_D& rData);
	BOOL GetRcpl(T_RCPL_K Key,T_RCPL_D& rData);
	int GetCountRcpl();
	POSITION GetStartRcpl();
	void GetNextRcpl(POSITION& rNextPosition,T_RCPL_K& rKey,T_RCPL_D& rData);
	T_RCPL_K GetRcplKey(CString& rName);
	T_RCPL_K GetStartNumRcpl();
	T_RCPL_K GetLastNumRcpl();
	T_RCPL_K GetNextNumRcpl(T_RCPL_K StartNum);
public:
	void GetCpplKeyList(CArray<T_CPPL_K, T_CPPL_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCppl(T_CPPL_K Key);
	BOOL GetCppl(T_CPPL_K Key,T_CPPL_D& rData);
	int GetCountCppl();
	POSITION GetStartCppl();
	void GetNextCppl(POSITION& rNextPosition,T_CPPL_K& rKey,T_CPPL_D& rData);
public:
	void GetCpwaKeyList(CArray<T_CPWA_K, T_CPWA_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistCpwa(T_CPWA_K Key);
	BOOL GetCpwa(T_CPWA_K Key,T_CPWA_D& rData);
	int GetCountCpwa();
	POSITION GetStartCpwa();
	void GetNextCpwa(POSITION& rNextPosition,T_CPWA_K& rKey,T_CPWA_D& rData);
public:
	void GetDlcmKeyList(CArray<T_DLCM_K, T_DLCM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistDlcm(T_DLCM_K Key);
	BOOL GetDlcm(T_DLCM_K Key,T_DLCM_D& rData);
	int GetCountDlcm();
	POSITION GetStartDlcm();
	void GetNextDlcm(POSITION& rNextPosition,T_DLCM_K& rKey,T_DLCM_D& rData);
	T_DLCM_K GetStartNumDlcm();
	T_DLCM_K GetLastNumDlcm();

// KR DGN Rating - Load Combination
public:
	void GetRkceKeyList(CArray<T_RKCE_K, T_RKCE_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRkce(T_RKCE_K Key);
	BOOL GetRkce(T_RKCE_K Key,T_RKCE_D& rData);
	int  GetCountRkce();
	POSITION GetStartRkce();
	void GetNextRkce(POSITION& rNextPosition,T_RKCE_K& rKey,T_RKCE_D& rData);
// KR DGN Rating - Parameter by Elem
public:
	void GetRkcvKeyList(CArray<T_RKCV_K, T_RKCV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRkcv(T_RKCV_K Key);
	BOOL GetRkcv(T_RKCV_K Key, T_RKCV_D& rData);
	int  GetCountRkcv();
	POSITION GetStartRkcv();
	void GetNextRkcv(POSITION& rNextPosition, T_RKCV_K& rKey, T_RKCV_D& rData);
	// KR DGN Rating - Parameter by Virtual Elem
public:
	void GetRklcKeyList(CArray<T_RKLC_K, T_RKLC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRklc(T_RKLC_K Key);
	BOOL GetRklc(T_RKLC_K Key,T_RKLC_D& rData);
	int GetCountRklc();
	POSITION GetStartRklc();
	void GetNextRklc(POSITION& rNextPosition,T_RKLC_K& rKey,T_RKLC_D& rData);

// KR DGN Rating - Print Option by Elem
public:
	void GetRkpoKeyList(CArray<T_RKPO_K, T_RKPO_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRkpo(T_RKPO_K Key);
	BOOL GetRkpo(T_RKPO_K Key,T_RKPO_D& rData);
	int GetCountRkpo();
	POSITION GetStartRkpo();
	void GetNextRkpo(POSITION& rNextPosition,T_RKPO_K& rKey,T_RKPO_D& rData);

	// KR DGN Rating - Print Option by Virtual Elem
public:
	void GetRkpvKeyList(CArray<T_RKPV_K, T_RKPV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRkpv(T_RKPV_K Key);
	BOOL GetRkpv(T_RKPV_K Key, T_RKPV_D& rData);
	int GetCountRkpv();
	POSITION GetStartRkpv();
	void GetNextRkpv(POSITION& rNextPosition, T_RKPV_K& rKey, T_RKPV_D& rData);

// Assessment Option
public:
	void GetAsopKeyList(CArray<T_ASOP_K, T_ASOP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistAsop(T_ASOP_K Key);
	BOOL GetAsop(T_ASOP_K Key,T_ASOP_D& rData);
	int GetCountAsop();
	POSITION GetStartAsop();
	void GetNextAsop(POSITION& rNextPosition,T_ASOP_K& rKey,T_ASOP_D& rData);

	// KR DGN Rating - Print Option by Elem - RC
public:
	void GetRprcKeyList(CArray<T_RPRC_K, T_RPRC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRprc(T_RPRC_K Key);
	BOOL GetRprc(T_RPRC_K Key,T_RPRC_D& rData);
	int GetCountRprc();
	POSITION GetStartRprc();
	void GetNextRprc(POSITION& rNextPosition,T_RPRC_K& rKey,T_RPRC_D& rData);

public:
	BOOL ExistRsdm(T_RSDM_K Key);
	BOOL GetRsdm(T_RSDM_K Key,T_RSDM_D& rData);
	void GetRsdmKeyList(CArray<T_RSDM_K, T_RSDM_K>& rKeyList);
	void GetRsdmKeyList(int nOType,CArray<T_RSDM_K, T_RSDM_K>& rKeyList);
	int GetCountRsdm();
	POSITION GetStartRsdm();
	void GetNextRsdm(POSITION& rNextPosition,T_RSDM_K& rKey,T_RSDM_D& rData);

public:
	void GetRsdlKeyList(CArray<T_RSDL_K, T_RSDL_K>& rKeyList);
	void GetRsdlKeyListHasElem(T_ELEM_K ElemK, CArray<T_RSDL_K, T_RSDL_K>& rKeyList); // Elem 을 가진 RSDL
	BOOL ExistRsdl(T_RSDL_K Key);
	BOOL GetRsdl(T_RSDL_K Key,T_RSDL_D& rData);
	int GetCountRsdl();
	POSITION GetStartRsdl();
	void GetNextRsdl(POSITION& rNextPosition,T_RSDL_K& rKey,T_RSDL_D& rData);
	T_RSDL_K GetStartNumRsdl();
	T_RSDL_K GetLastNumRsdl();
	T_RSDL_K GetNextNumRsdl(T_RSDL_K StartNum);

// 일본 보유수평내력법 (신봉진)
public:
	BOOL ExistRlco();
	BOOL GetRlco(T_RLCO_D& rData);	
	void GetDefaultRlco(T_RLCO_D& rData);
public:
	void GetRlccList(CArray<CString, CString&>& rstrNameList); //Name으로 정렬된 리스트
	void GetRlccKeyList(CArray<T_RLCC_K, T_RLCC_K>& rKeyList); //Name으로 정렬된 리스트
	void GetRlccKeyListHasElem(T_ELEM_K ElemK, CArray<T_RLCC_K, T_RLCC_K>& rKeyList); // Elem 을 가진 RLCC
	BOOL ExistRlcc(CString& rName);
	BOOL ExistRlcc(T_RLCC_K Key);
	BOOL GetRlcc(CString& rName, T_RLCC_D& rData);
	BOOL GetRlcc(T_RLCC_K Key,T_RLCC_D& rData);
	int GetCountRlcc();
	POSITION GetStartRlcc();
	void GetNextRlcc(POSITION& rNextPosition,T_RLCC_K& rKey,T_RLCC_D& rData);
	T_RLCC_K GetRlccKey(CString& rName);
	T_RLCC_K GetStartNumRlcc();
	T_RLCC_K GetLastNumRlcc();
	T_RLCC_K GetNextNumRlcc(T_RLCC_K StartNum);
public:
	void GetMrebKeyList(CArray<T_MREB_K, T_MREB_K>& rKeyList);
	BOOL ExistMreb(T_MREB_K Key);
	BOOL GetMreb(T_MREB_K Key,T_MREB_D& rData);
	int GetCountMreb();
	POSITION GetStartMreb();
	void GetNextMreb(POSITION& rNextPosition,T_MREB_K& rKey,T_MREB_D& rData);
public:
	void GetSlrsKeyList(CArray<T_SLRS_K, T_SLRS_K>& rKeyList);
	BOOL ExistSlrs(T_SLRS_K Key);
	BOOL GetSlrs(T_SLRS_K Key,T_SLRS_D& rData);
	int GetCountSlrs();
	POSITION GetStartSlrs();
	void GetNextSlrs(POSITION& rNextPosition,T_SLRS_K& rKey,T_SLRS_D& rData);
public:
	BOOL ExistMlcb();
	BOOL GetMlcb(T_MLCB_D& rData);
	BOOL Get_DefaultMlcb(T_MLCB_D& rMlcbD);
// color
	BOOL GetCo_m(T_MATL_K Key, T_COLOR_D& rData);
	BOOL GetCo_s(T_SECT_K Key, T_COLOR_D& rData);
	BOOL GetCo_t(T_THIK_K Key, T_COLOR_D& rData);
	BOOL GetCo_f(T_FBLD_K Key, T_COLOR_D& rData);
	BOOL GetCo_g(T_GRUP_K Key, T_COLOR_D& rData);

	void GetWallLocalVector(CArray<T_LOCALVECTOR, T_LOCALVECTOR&>& arLocalVector);

// Pushover Renewal
	BOOL ConvertPoctPplc2Pogd(const T_POCT_D *pPoctD, const T_PPLC_D *pPplcD, T_POGD_D *pPogdD);
	BOOL ConvertPold2Polc(const T_POLD_D *pPoldD, const int &nStepNum, T_POLC_D *pPolcD);
	BOOL ModifyNewPoas(T_POAS_D *pPoasD);
	BOOL ConvertHngt2Phgt(T_HNGT_D *pHngtD, T_PHGT_D *pPhgtD, BOOL bTargetDof[6], 
												T_HNGE_K *pHngeK=NULL, BOOL bSetExternal=TRUE);
	BOOL ConvertHnge2PhgtPhge(const T_ELEM_K &ElemK, T_PHGT_D *pPhgtD, T_PHGE_D *pPhgeD, CMap<T_HNGT_K, T_HNGT_K, BOOL, BOOL> *pmUsedHngtK);

// User Defined Figures
public:
	void GetUfigList(CArray<CString, CString&>& rstrGirderList);
	void GetUfigKeyList(CArray<T_UFIG_K, T_UFIG_K>& rKeyList);
	BOOL ExistUfig(CString& rUfigName);
	BOOL ExistUfig(T_UFIG_K Key);
	BOOL GetUfig(CString& rUfigName, T_UFIG_D& rData);
	BOOL GetUfig(T_UFIG_K Key,T_UFIG_D& rData);
	int GetCountUfig();
	POSITION GetStartUfig();
	void GetNextUfig(POSITION& rNextPosition,T_UFIG_K& rKey,T_UFIG_D& rData);
	T_UFIG_K GetUfigKey(CString& rUfigName);
	T_UFIG_K GetStartNumUfig();
	T_UFIG_K GetLastNumUfig();
	int  GetActiveElemList4Ufig(const T_UFIG_K &UfigK, CMapEx<UINT, UINT, BOOL, BOOL> *pmKey);
	int  GetActiveElemList4Ufig(T_UFIG_D UfigD, CMapEx<UINT, UINT, BOOL, BOOL> *pmKey);
	int  GetActiveNodeList4Ufig(const T_UFIG_K &UfigK, CMapEx<UINT, UINT, BOOL, BOOL> *pmKey);
	int  GetActiveNodeList4Ufig(T_UFIG_D UfigD, CMapEx<UINT, UINT, BOOL, BOOL> *pmKey);

// User Defined table
public:
	void GetUtblList(CArray<CString, CString&>& rstrGirderList);
	void GetUtblKeyList(CArray<T_UTBL_K, T_UTBL_K>& rKeyList);
	BOOL ExistUtbl(CString& rUtblName);
	BOOL ExistUtbl(T_UTBL_K Key);
	BOOL GetUtbl(CString& rUtblName, T_UTBL_D& rData);
	BOOL GetUtbl(T_UTBL_K Key,T_UTBL_D& rData);
	int GetCountUtbl();
	POSITION GetStartUtbl();
	void GetNextUtbl(POSITION& rNextPosition,T_UTBL_K& rKey,T_UTBL_D& rData);
	T_UTBL_K GetUtblKey(CString& rUtblName);
	T_UTBL_K GetStartNumUtbl();
	T_UTBL_K GetLastNumUtbl();

// User Defined chart
public:
	void GetUchtList(CArray<CString, CString&>& rstrGirderList);
	void GetUchtKeyList(CArray<T_UCHT_K, T_UCHT_K>& rKeyList);
	BOOL ExistUcht(CString& rUchtName);
	BOOL ExistUcht(T_UCHT_K Key);
	BOOL GetUcht(CString& rUchtName, T_UCHT_D& rData);
	BOOL GetUcht(T_UCHT_K Key,T_UCHT_D& rData);
	int GetCountUcht();
	int GetCountTypeUcht(int nType);
	POSITION GetStartUcht();
	void GetNextUcht(POSITION& rNextPosition,T_UCHT_K& rKey,T_UCHT_D& rData);
	T_UCHT_K GetUchtKey(CString& rUchtName);
	T_UCHT_K GetStartNumUcht();
	T_UCHT_K GetLastNumUcht();

// User Defined Table Template
public:
	void GetUtplList(CArray<CString, CString&>& rstrGirderList);
	void GetUtplKeyList(CArray<T_UTPL_K, T_UTPL_K>& rKeyList);
	BOOL ExistUtpl(CString& rUtplName);
	BOOL ExistUtpl(T_UTPL_K Key);
	BOOL GetUtpl(CString& rUtplName, T_UTPL_D& rData);
	BOOL GetUtpl(T_UTPL_K Key,T_UTPL_D& rData);
	int GetCountUtpl();
	POSITION GetStartUtpl();
	void GetNextUtpl(POSITION& rNextPosition,T_UTPL_K& rKey,T_UTPL_D& rData);
	T_UTPL_K GetUtplKey(CString& rUtplName);
	T_UTPL_K GetStartNumUtpl();
	T_UTPL_K GetLastNumUtpl();

// Image Files
public:
	void GetUimgList(CArray<CString, CString&>& rstrGirderList);
	void GetUimgKeyList(CArray<T_UIMG_K, T_UIMG_K>& rKeyList);
	BOOL ExistUimg(CString& rUimgName);
	BOOL ExistUimg(T_UIMG_K Key);
	BOOL GetUimg(CString& rUimgName, T_UIMG_D& rData);
	BOOL GetUimg(T_UIMG_K Key,T_UIMG_D& rData);
	int GetCountUimg();
	POSITION GetStartUimg();
	void GetNextUimg(POSITION& rNextPosition,T_UIMG_K& rKey,T_UIMG_D& rData);
	T_UIMG_K GetUimgKey(CString& rUimgName);
	T_UIMG_K GetStartNumUimg();
	T_UIMG_K GetLastNumUimg();

// User Defined Text (Design Summary)
public:
	void GetUsumList(CArray<CString, CString&>& rstrGirderList);
	void GetUsumKeyList(CArray<T_USUM_K, T_USUM_K>& rKeyList);
	BOOL ExistUsum(CString& rUsumName);
	BOOL ExistUsum(T_USUM_K Key);
	BOOL GetUsum(CString& rUsumName, T_USUM_D& rData);
	BOOL GetUsum(T_USUM_K Key,T_USUM_D& rData);
	int GetCountUsum();
	POSITION GetStartUsum();
	void GetNextUsum(POSITION& rNextPosition,T_USUM_K& rKey,T_USUM_D& rData);
	T_USUM_K GetUsumKey(CString& rUsumName);
	T_USUM_K GetStartNumUsum();
	T_USUM_K GetLastNumUsum();

	// Boundary Element Method by WallID
public:
	int GetBemwKeyList(CArray<T_BEMW_K, T_BEMW_K>& rKeyList);  // Wall ID, Stor ID 순으로 정렬 된 리스트
	BOOL ExistBemw(T_BEMW_K Key);
	BOOL GetBemw(T_BEMW_K Key,T_BEMW_D& rData);
	int GetCountBemw();
	POSITION GetStartBemw();
	void GetNextBemw(POSITION& rNextPosition,T_BEMW_K& rKey,T_BEMW_D& rData);

	// Fundamental Period for Slender Walls
public:
	int GetFpswKeyList(CArray<T_FPSW_K, T_FPSW_K>& rKeyList);  // Wall ID, Stor ID 순으로 정렬 된 리스트
	BOOL ExistFpsw(T_FPSW_K Key);
	BOOL GetFpsw(T_FPSW_K Key, T_FPSW_D& rData);
	int GetCountFpsw();
	POSITION GetStartFpsw();
	void GetNextFpsw(POSITION& rNextPosition, T_FPSW_K& rKey, T_FPSW_D& rData);

public:
	int GetEbmwKeyList( CArray<T_EBMW_K, T_EBMW_K>& rKeyList );
	BOOL ExistEbmw(T_EBMW_K Key);
	BOOL GetEbmw(T_EBMW_K Key, T_EBMW_D& rData);
	int GetCountEbmw();
	POSITION GetStartEbmw();
	void GetNextEbmw(POSITION& rNextPosition, T_EBMW_K& rKey, T_EBMW_D& rData);

//Add by xuezc for performance Dgn 2019-3-26
public:
	void GetPfmcKeyList(CArray<T_PFMC_K, T_PFMC_K>& rKeyList);
	BOOL ExistPfmc(T_PFMC_K Key);
	BOOL GetPfmc(T_PFMC_K Key,T_PFMC_D& rData);
	int GetCountPfmc();
	POSITION GetStartPfmc();
	void GetNextPfmc(POSITION& rNextPosition,T_PFMC_K& rKey,T_PFMC_D& rData);

public:
	void GetPfcmKeyList(CArray<T_PFCM_K, T_PFCM_K>& rKeyList);
	BOOL ExistPfcm(T_PFCM_K Key);
	BOOL GetPfcm(T_PFCM_K Key,T_PFCM_D& rData);
	int GetCountPfcm();
	POSITION GetStartPfcm();
	void GetNextPfcm(POSITION& rNextPosition,T_PFCM_K& rKey,T_PFCM_D& rData);

public:
	void GetPfdlKeyList(CArray<T_PFDL_K, T_PFDL_K>& rKeyList);
	BOOL ExistPfdl(T_PFDL_K Key);
	BOOL GetPfdl(T_PFDL_K Key,T_PFDL_D& rData);
	int GetCountPfdl();
	POSITION GetStartPfdl();
	void GetNextPfdl(POSITION& rNextPosition,T_PFDL_K& rKey,T_PFDL_D& rData);

public:
	void GetPfmpKeyList(CArray<T_PFMP_K, T_PFMP_K>& rKeyList);
	BOOL ExistPfmp(T_PFMP_K Key);
	BOOL GetPfmp(T_PFMP_K Key,T_PFMP_D& rData);
	int GetCountPfmp();
	POSITION GetStartPfmp();
	void GetNextPfmp(POSITION& rNextPosition,T_PFMP_K& rKey,T_PFMP_D& rData);

public:
	void GetPfnpKeyList(CArray<T_PFNP_K, T_PFNP_K>& rKeyList);
	BOOL ExistPfnp(T_PFNP_K Key);
	BOOL GetPfnp(T_PFNP_K Key,T_PFNP_D& rData);
	int GetCountPfnp();
	POSITION GetStartPfnp();
	void GetNextPfnp(POSITION& rNextPosition,T_PFNP_K& rKey,T_PFNP_D& rData);

public:
	void GetSmspKeyList(CArray<T_SMSP_K, T_SMSP_K>& rKeyList);
	BOOL ExistSmsp(T_SMSP_K Key);
	BOOL GetSmsp(T_SMSP_K Key,T_SMSP_D& rData);
	int  GetSmspList(CArray<T_SMSP_K, T_SMSP_K>& raKey, CArray<T_SMSP_D, T_SMSP_D&>& raData);
	int GetCountSmsp();
	POSITION GetStartSmsp();
	void GetNextSmsp(POSITION& rNextPosition,T_SMSP_K& rKey,T_SMSP_D& rData);
	BOOL IsSmsp(T_ELEM_K Key);
	BOOL IsSmspWithSectShape(T_SECT_K rKey); //SECT를 가지고 Smsp의 조건인 Solid Round/Pipe인지 확인

public:
	void GetSpscKeyList(CArray<T_SPSC_K, T_SPSC_K>& rKeyList);
	BOOL ExistSpsc(T_SPSC_K Key);
	BOOL GetSpsc(T_SPSC_K Key,T_SPSC_D& rData);
	int GetCountSpsc();
	POSITION GetStartSpsc();
	void GetNextSpsc(POSITION& rNextPosition,T_SPSC_K& rKey,T_SPSC_D& rData);
	BOOL IsSpsc(T_ELEM_K Key);

	//splb
public:
	void GetSplbKeyList(CArray<T_SPLB_K, T_SPLB_K>& rKeyList);
	BOOL ExistSplb(T_SPLB_K Key);
	BOOL GetSplb(T_SPLB_K Key, T_SPLB_D& rData);
	int GetCountSplb();
	POSITION GetStartSplb();
	void GetNextSplb(POSITION& rNextPosition, T_SPLB_K& rKey, T_SPLB_D& rData);
	BOOL IsSplb(T_ELEM_K Key);

	//spcs
public:
	void GetSpcsKeyList(CArray<T_SPCS_K, T_SPCS_K>& rKeyList);
	BOOL ExistSpcs(T_SPCS_K Key);
	BOOL GetSpcs(T_SPCS_K Key, T_SPCS_D& rData);
	int GetCountSpcs();
	POSITION GetStartSpcs();
	void GetNextSpcs(POSITION& rNextPosition, T_SPCS_K& rKey, T_SPCS_D& rData);
	BOOL IsSpcs(T_ELEM_K Key);
	//spls
public:
	void GetSplsKeyList(CArray<T_SPLS_K, T_SPLS_K>& rKeyList);
	BOOL ExistSpls(T_SPLS_K Key);
	BOOL GetSpls(T_SPLS_K Key, T_SPLS_D& rData);
	int GetCountSpls();
	POSITION GetStartSpls();
	void GetNextSpls(POSITION& rNextPosition, T_SPLS_K& rKey, T_SPLS_D& rData);
	BOOL IsSpls(T_ELEM_K Key);

	//sppb
public:
	void GetSppbKeyList(CArray<T_SPPB_K, T_SPPB_K>& rKeyList);
	BOOL ExistSppb(T_SPPB_K Key);
	BOOL GetSppb(T_SPPB_K Key, T_SPPB_D& rData);
	int GetCountSppb();
	POSITION GetStartSppb();
	void GetNextSppb(POSITION& rNextPosition, T_SPPB_K& rKey, T_SPPB_D& rData);
	BOOL IsSppb(T_ELEM_K Key);

//add by tss 2020/02/14
public:
	void GetSectClassifyKeyList(CArray<T_SECTCLASSIFY_K, T_SECTCLASSIFY_K>& rKeyList);
	BOOL ExistSectClassify(T_SECTCLASSIFY_K Key);
	BOOL GetSectClassify(T_SECTCLASSIFY_K Key,T_SECTCLASSIFY_D& rData);
	int GetCountSectClassify();
	POSITION GetStartSectClassify();
	void GetNextSectClassify(POSITION& rNextPosition,T_SECTCLASSIFY_K& rKey,T_SECTCLASSIFY_D& rData);

	//add by tss 2021/01/13
public:
	void GetBendModulusKeyList(CArray<T_BENDMODULUS_K, T_BENDMODULUS_K>& rKeyList);
	BOOL ExistBendModulus(T_BENDMODULUS_K Key);
	BOOL GetBendModulus(T_BENDMODULUS_K Key,T_BENDMODULUS_D& rData);
	int GetCountBendModulus();
	POSITION GetStartBendModulus();
	void GetNextBendModulus(POSITION& rNextPosition,T_BENDMODULUS_K& rKey,T_BENDMODULUS_D& rData);

	//add by tss 2024/05/21
public:
	void GetImelKeyList(CArray<T_IMEL_K, T_IMEL_K>& rKeyList);
	BOOL ExistImel(T_IMEL_K Key);
	BOOL GetImel(T_IMEL_K Key, T_IMEL_D& rData);
	int GetCountImel();
	POSITION GetStartImel();
	void GetNextImel(POSITION& rNextPosition, T_IMEL_K& rKey, T_IMEL_D& rData);

	//add by tss 2024/02/29
public:
	void GetSpdfKeyList(CArray<T_SPDF_K, T_SPDF_K>& rKeyList);
	BOOL ExistSpdf(T_SPDF_K Key);
	BOOL GetSpdf(T_SPDF_K Key, T_SPDF_D& rData);
	int GetCountSpdf();
	POSITION GetStartSpdf();
	void GetNextSpdf(POSITION& rNextPosition, T_SPDF_K& rKey, T_SPDF_D& rData);

	////add by tss 2022/10/10
public:
	void GetSPhiKeyList(CArray<T_SPHI_K, T_SPHI_K>& rKeyList); 
	BOOL ExistSPhi(T_SPHI_K Key);
	BOOL GetSPhi(T_SPHI_K Key, T_SPHI_D& rData);
	int GetCountSPhi();
	POSITION GetStartSPhi();
	void GetNextSPhi(POSITION& rNextPosition, T_SPHI_K& rKey, T_SPHI_D& rData);

////add by maxiao 2015-9-29 for GB50017-15
public:
	void GetPhibKeyList(CArray<T_PHIB_K, T_PHIB_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPhib(T_PHIB_K Key);
	BOOL GetPhib(T_PHIB_K Key,T_PHIB_D& rData);
	int GetCountPhib();
	POSITION GetStartPhib();
	void GetNextPhib(POSITION& rNextPosition,T_PHIB_K& rKey,T_PHIB_D& rData);

////add by maxiao 2015-10-20 for GB50017-15
public:
	void GetSdswKeyList(CArray<T_SDSW_K, T_SDSW_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSdsw(T_SDSW_K Key);
	BOOL GetSdsw(T_SDSW_K Key,T_SDSW_D& rData);
	int GetCountSdsw();
	POSITION GetStartSdsw();
	void GetNextSdsw(POSITION& rNextPosition,T_SDSW_K& rKey,T_SDSW_D& rData);

//add by maxiao(2015-11-3)GB50017-15
 public:
	void GetSlsdKeyList(CArray<T_SLSD_K, T_SLSD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSlsd(T_SLSD_K Key);
	BOOL GetSlsd(T_SLSD_K Key,T_SLSD_D& rData);
	int GetCountSlsd();
	POSITION GetStartSlsd();
	void GetNextSlsd(POSITION& rNextPosition,T_SLSD_K& rKey,T_SLSD_D& rData);

	// Program Info
public:
	BOOL GetPgif(T_PGIF_D& rPgifD);

private:
	BOOL SetConnectedStor_Next(CLinkedKeyMgr& rLinkedKeyMgr_stor, UINT StorK_cur, UINT StorK_next);

public:
	BOOL GetLevelMinMaxOfModl(T_MODL_BASE& Modl_cur, double& rdLevelMax, double& rdLevelMin);
	BOOL IsEndStorOfModl(T_MODL_BASE& Modl_cur, UINT StorK);
	BOOL IsIncludeStor_start(T_MODL_BASE& Modl_cur);
	BOOL IsIncludeStor_end(T_MODL_BASE& Modl_cur);

	// Time History Swept Frequency Graph - Swept Frequency Response Analysis
public:
	BOOL MakeKeyThsf(T_THSF_D& rData, T_THSF_K& rKey);
	BOOL ExistThsf(T_THSF_K Key);
	BOOL GetThsf(T_THSF_K Key, T_THSF_D& rData);
	int GetCountThsf();
	POSITION GetStartThsf();
	void GetNextThsf(POSITION& rNextPosition, T_THSF_K& rKey, T_THSF_D& rData);
	void GetThsfKeyList(CArray<T_THSF_K, T_THSF_K>& rKeyList);

	// Time History Result Graph - General Link
public:
	BOOL MakeKeyThrg(T_THRG_D& rData, T_THRG_K& rKey);
	BOOL ExistThrg(T_THRG_K Key);
	BOOL GetThrg(T_THRG_K Key,T_THRG_D& rData);
	int GetCountThrg();
	POSITION GetStartThrg();
	void GetNextThrg(POSITION& rNextPosition,T_THRG_K& rKey,T_THRG_D& rData);
	void GetThrgKeyList(CArray<T_THRG_K, T_THRG_K>& rKeyList);  //Key 번호로 정렬된 리스트

	// Time History Result Graph - Inelastic Hinge
public:
	BOOL MakeKeyThri(T_THRI_D& rData, T_THRI_K& rKey);
	BOOL ExistThri(T_THRI_K Key);
	BOOL GetThri(T_THRI_K Key,T_THRI_D& rData);
	int GetCountThri();
	POSITION GetStartThri();
	void GetNextThri(POSITION& rNextPosition,T_THRI_K& rKey,T_THRI_D& rData);
	void GetThriKeyList(CArray<T_THRI_K, T_THRI_K>& rKeyList);  //Key 번호로 정렬된 리스트
	T_IEHP_K GetThriIehpK(const T_THRI_D& ThriD);
	T_ELEM_K GetElemKByWallIDStorK(const unsigned int nWallID, T_STOR_K StorK);
	T_IEHP_D GetIehpByThri(const T_THRI_D& ThriD);

	// Time History Result Graph - Seismic Control Devices
public:
	BOOL MakeKeyThrs(T_THRS_D& rData, T_THRS_K& rKey);
	BOOL ExistThrs(T_THRS_K Key);
	BOOL GetThrs(T_THRS_K Key,T_THRS_D& rData);
	int GetCountThrs();
	POSITION GetStartThrs();
	void GetNextThrs(POSITION& rNextPosition,T_THRS_K& rKey,T_THRS_D& rData);
	void GetThrsKeyList(CArray<T_THRS_K, T_THRS_K>& rKeyList);  //Key 번호로 정렬된 리스트

	// Time History Result Graph - Element Force
public:
	BOOL MakeKeyThef(T_THEF_D& rData, T_THEF_K& rKey);
	BOOL ExistThef(T_THEF_K Key);
	BOOL GetThef(T_THEF_K Key,T_THEF_D& rData);
	int GetCountThef();
	POSITION GetStartThef();
	void GetNextThef(POSITION& rNextPosition,T_THEF_K& rKey,T_THEF_D& rData);
	void GetThefKeyList(CArray<T_THEF_K, T_THEF_K>& rKeyList);  //Key 번호로 정렬된 리스트

	// Pushover Result Graph - Element Force
public:
	BOOL MakeKeyPoef(T_POEF_D& rData, T_POEF_K& rKey);
	BOOL ExistPoef(T_POEF_K Key);
	BOOL GetPoef(T_POEF_K Key,T_POEF_D& rData);
	int GetCountPoef();
	POSITION GetStartPoef();
	void GetNextPoef(POSITION& rNextPosition,T_POEF_K& rKey,T_POEF_D& rData);
	void GetPoefKeyList(CArray<T_POEF_K, T_POEF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	T_THEF_D ConvertPoefToThef(const T_POEF_D& PoefD);
	T_POEF_D ConvertThefToPoef(const T_THEF_D& ThefD);

	// Pushover Result Graph - General Link
public:
	BOOL MakeKeyPorg(T_PORG_D& rData, T_PORG_K& rKey);
	BOOL ExistPorg(T_PORG_K Key);
	BOOL GetPorg(T_PORG_K Key,T_PORG_D& rData);
	int GetCountPorg();
	POSITION GetStartPorg();
	void GetNextPorg(POSITION& rNextPosition,T_PORG_K& rKey,T_PORG_D& rData);
	void GetPorgKeyList(CArray<T_PORG_K, T_PORG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	T_THRG_D ConvertPorgToThrg(const T_PORG_D& PorgD);
	T_PORG_D ConvertThrgToPorg(const T_THRG_D& ThrgD);

	// Pushover Result Graph - Elastic Link
public:
	BOOL MakeKeyPore(T_PORE_D& rData, T_PORE_K& rKey);
	BOOL ExistPore(T_PORE_K Key);
	BOOL GetPore(T_PORE_K Key,T_PORE_D& rData);
	int GetCountPore();
	POSITION GetStartPore();
	void GetNextPore(POSITION& rNextPosition,T_PORE_K& rKey,T_PORE_D& rData);
	void GetPoreKeyList(CArray<T_PORE_K, T_PORE_K>& rKeyList);  //Key 번호로 정렬된 리스트

	// Pushover Result Graph - Inelastic Hinge
public:
	BOOL MakeKeyPori(T_PORI_D& rData, T_PORI_K& rKey);
	BOOL ExistPori(T_PORI_K Key);
	BOOL GetPori(T_PORI_K Key,T_PORI_D& rData);
	int GetCountPori();
	POSITION GetStartPori();
	void GetNextPori(POSITION& rNextPosition,T_PORI_K& rKey,T_PORI_D& rData);
	void GetPoriKeyList(CArray<T_PORI_K, T_PORI_K>& rKeyList);  //Key 번호로 정렬된 리스트
	T_THRI_D ConvertPoriToThri(const T_PORI_D& PoriD);
	T_PORI_D ConvertThriToPori(const T_THRI_D& ThriD);
	T_PHGT_D GetPhgtByPori(const T_PORI_D& PoriD);

	//Load Combination Live Load Type
public:
	BOOL ExistLllt(T_LLLT_K Key);
	BOOL GetLllt(T_LLLT_K Key, T_LLLT_D& rData);
	int GetCountLllt();
	POSITION GetStartLllt();
	void GetNextLllt(POSITION& rNextPosition, T_LLLT_K& rKey, T_LLLT_D& rData);
	int  GetLlltKeyList(CArray<T_LLLT_K, T_LLLT_K>& rKeyList);

	// 내진성능평가 - 의사지진력
public:
	BOOL ExistPsll(T_PSLL_K Key);
	BOOL GetPsll(T_PSLL_K Key,T_PSLL_D& rData);
	int GetCountPsll();
	POSITION GetStartPsll();
	void GetNextPsll(POSITION& rNextPosition,T_PSLL_K& rKey,T_PSLL_D& rData);
	void CalcPsllLoad(T_PSLL_D& rData, CArray<T_SEISLOAD, T_SEISLOAD&>& arPsllLoad, BOOL bCalcMassCenter=TRUE);
	void CalcPsllLcom(BOOL bAllLoadCase, T_LCOM_D& DataLcom, CArray<T_SEISLOAD, T_SEISLOAD&>& arSeisLoad);
	void GetPsllKeyList(CArray<T_PSLL_K, T_PSLL_K>& rKeyList);

	// 내진성능평가 : 기둥유효강성 계수
public:
	BOOL ExistLesf(T_LESF_K Key);
	BOOL GetLesf(T_LESF_K Key,T_LESF_D& rData);
	int GetCountLesf();
	POSITION GetStartLesf();
	void GetNextLesf(POSITION& rNextPosition,T_LESF_K& rKey,T_LESF_D& rData);
	int  GetLesfKeyList(CArray<T_LESF_K, T_LESF_K>& rKeyList);

	// Seismic Evaluation - Effective Weight
public:
	void GetSeewKeyList(CArray<T_SEEW_K, T_SEEW_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSeew(T_SEEW_K Key);
	BOOL GetSeew(T_SEEW_K Key,T_SEEW_D& rData);
	int  GetSeewList(CArray<T_SEEW_K, T_SEEW_K>& raKey, CArray<T_SEEW_D, T_SEEW_D&>& raData);
	int GetCountSeew();
	POSITION GetStartSeew();
	void GetNextSeew(POSITION& rNextPosition,T_SEEW_K& rKey,T_SEEW_D& rData);
	BOOL GetSeew_default(T_SEEW_K Key,T_SEEW_D& rData);

	// Seismic Evaluation - Story CDR Method
public:
	void GetScdrKeyList(CArray<T_SCDR_K, T_SCDR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistScdr(T_SCDR_K Key);
	BOOL GetScdr(T_SCDR_K Key,T_SCDR_D& rData);
	int  GetScdrList(CArray<T_SCDR_K, T_SCDR_K>& raKey, CArray<T_SCDR_D, T_SCDR_D&>& raData);
	int GetCountScdr();
	POSITION GetStartScdr();
	void GetNextScdr(POSITION& rNextPosition,T_SCDR_K& rKey,T_SCDR_D& rData);
	T_SCDR_K GetStartNumScdr();
	T_SCDR_K GetScdrKByStorLCK(T_STOR_K StorK, unsigned int AnalType, unsigned int LoadCaseKey);
	void GetScdrKeyListByStorK(T_STOR_K StorK, OUT CArray<T_SCDR_K,T_SCDR_K> aScdrK);

	// Response Spectrum - Modification Factor
public:
	void GetRsmfKeyList(CArray<T_RSMF_K, T_RSMF_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRsmf(T_RSMF_K Key);
	BOOL GetRsmf(T_RSMF_K Key,T_RSMF_D& rData);
	int  GetRsmfList(CArray<T_RSMF_K, T_RSMF_K>& raKey, CArray<T_RSMF_D, T_RSMF_D&>& raData);
	int GetCountRsmf();
	POSITION GetStartRsmf();
	void GetNextRsmf(POSITION& rNextPosition,T_RSMF_K& rKey,T_RSMF_D& rData);
	double GetRsmf_C(T_RSMF_K Key);
	double GetDefaultC4Rsmf();

	// Named View
public:
	void GetNmdvKeyList(CArray<T_NMDV_K, T_NMDV_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistNmdv(T_NMDV_K Key);
	BOOL ExistNmdvName(CString strName);
	BOOL GetNmdv(T_NMDV_K Key,T_NMDV_D& rData);
	T_NMDV_K GetNmdvKey(CString& rStrName);
	int  GetNmdvList(CArray<T_NMDV_K, T_NMDV_K>& raKey, CArray<T_NMDV_D, T_NMDV_D&>& raData);
	int GetCountNmdv();
	POSITION GetStartNmdv();
	void GetNextNmdv(POSITION& rNextPosition,T_NMDV_K& rKey,T_NMDV_D& rData);
	T_NMDV_K GetStartNumNmdv();
	T_NMDV_K GetLastNumNmdv();

	// Load Case for Force Controlled Action
public:
	void GetLfcaKeyList(CArray<T_LFCA_K, T_LFCA_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistLfca(T_LFCA_K Key);
	BOOL GetLfca(T_LFCA_K Key,T_LFCA_D& rData);
	int  GetLfcaList(CArray<T_LFCA_K, T_LFCA_K>& raKey, CArray<T_LFCA_D, T_LFCA_D&>& raData);
	int GetCountLfca();
	POSITION GetStartLfca();
	void GetNextLfca(POSITION& rNextPosition,T_LFCA_K& rKey,T_LFCA_D& rData);
	BOOL GetLfcaDefault(T_LFCA_K Key,T_LFCA_D& rData);

	// Seismic Evaluation - Clear Height of Column
public:
	void GetSechKeyList(CArray<T_SECH_K, T_SECH_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSech(T_SECH_K Key);
	BOOL GetSech(T_SECH_K Key,T_SECH_D& rData);
	int  GetSechList(CArray<T_SECH_K, T_SECH_K>& raKey, CArray<T_SECH_D, T_SECH_D&>& raData);
	int GetCountSech();
	POSITION GetStartSech();
	void GetNextSech(POSITION& rNextPosition,T_SECH_K& rKey,T_SECH_D& rData);
	BOOL GetSech_default(T_SECH_K Key,T_SECH_D& rData);

	// Seismic Evaluation - Use Seismic Hook
public:
	void GetSehkKeyList(CArray<T_SEHK_K, T_SEHK_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSehk(T_SEHK_K Key);
	BOOL GetSehk(T_SEHK_K Key,T_SEHK_D& rData);
	int  GetSehkList(CArray<T_SEHK_K, T_SEHK_K>& raKey, CArray<T_SEHK_D, T_SEHK_D&>& raData);
	int GetCountSehk();
	POSITION GetStartSehk();
	void GetNextSehk(POSITION& rNextPosition,T_SEHK_K& rKey,T_SEHK_D& rData);
	BOOL GetSehk_default(T_SEHK_K Key,T_SEHK_D& rData);

	// Seismic Evaluation - RC Column with Masonry Infill Type
public:
	void GetRccmKeyList(CArray<T_RCCM_K, T_RCCM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRccm(T_RCCM_K Key);
	BOOL GetRccm(T_RCCM_K Key,T_RCCM_D& rData);
	int  GetRccmList(CArray<T_RCCM_K, T_RCCM_K>& raKey, CArray<T_RCCM_D, T_RCCM_D&>& raData);
	int GetCountRccm();
	POSITION GetStartRccm();
	void GetNextRccm(POSITION& rNextPosition,T_RCCM_K& rKey,T_RCCM_D& rData);
	BOOL GetRccm_default(T_RCCM_K Key,T_RCCM_D& rData);
	BOOL IsColumn(T_MEMB_K& MembK);

	// Seismic Evaluation - P-M Curve Calculation Method
public:
	void GetPmcmKeyList(CArray<T_PMCM_K, T_PMCM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPmcm(T_PMCM_K Key);
	BOOL GetPmcm(T_PMCM_K Key,T_PMCM_D& rData);
	int  GetPmcmList(CArray<T_PMCM_K, T_PMCM_K>& raKey, CArray<T_PMCM_D, T_PMCM_D&>& raData);
	int GetCountPmcm();
	POSITION GetStartPmcm();
	void GetNextPmcm(POSITION& rNextPosition,T_PMCM_K& rKey,T_PMCM_D& rData);

	// Seismic Evaluation - Seismic Component Type
public:
	void GetScmtKeyList(CArray<T_SCMT_K, T_SCMT_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistScmt(T_SCMT_K Key);
	BOOL GetScmt(T_SCMT_K Key,T_SCMT_D& rData);
	int  GetScmtList(CArray<T_SCMT_K, T_SCMT_K>& raKey, CArray<T_SCMT_D, T_SCMT_D&>& raData);
	int GetCountScmt();
	POSITION GetStartScmt();
	void GetNextScmt(POSITION& rNextPosition,T_SCMT_K& rKey,T_SCMT_D& rData);
	BOOL GetScmt_default(T_SCMT_K Key,T_SCMT_D& rData);

	// Seismic Evaluation - m-Factor
public:
	void GetMfacKeyList(CArray<T_MFAC_K, T_MFAC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistMfac(T_MFAC_K Key);
	BOOL GetMfac(T_MFAC_K Key,T_MFAC_D& rData);
	int  GetMfacList(CArray<T_MFAC_K, T_MFAC_K>& raKey, CArray<T_MFAC_D, T_MFAC_D&>& raData);
	int GetCountMfac();
	POSITION GetStartMfac();
	void GetNextMfac(POSITION& rNextPosition,T_MFAC_K& rKey,T_MFAC_D& rData);
	BOOL GetMfac_default(T_MFAC_K Key,T_MFAC_D& rData);

	// Seismic Evaluation - XY Factor
public:
	void GetXfacKeyList(CArray<T_XFAC_K, T_XFAC_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistXfac(T_XFAC_K Key);
	BOOL GetXfac(T_XFAC_K Key,T_XFAC_D& rData);
	int  GetXfacList(CArray<T_XFAC_K, T_XFAC_K>& raKey, CArray<T_XFAC_D, T_XFAC_D&>& raData);
	int GetCountXfac();
	POSITION GetStartXfac();
	void GetNextXfac(POSITION& rNextPosition,T_XFAC_K& rKey,T_XFAC_D& rData);
	double GetXfacBase() { return 1.5; };

	// Seismic Evaluation - Reduce Infill Strut Stiffness
public:
	void GetRissKeyList(CArray<T_RISS_K, T_RISS_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistRiss(T_RISS_K Key);
	BOOL GetRiss(T_RISS_K Key,T_RISS_D& rData);
	int  GetRissList(CArray<T_RISS_K, T_RISS_K>& raKey, CArray<T_RISS_D, T_RISS_D&>& raData);
	int GetCountRiss();
	POSITION GetStartRiss();
	void GetNextRiss(POSITION& rNextPosition,T_RISS_K& rKey,T_RISS_D& rData);
	BOOL GetRiss_default(T_RISS_K Key,T_RISS_D& rData);

	// Seismic Evaluation - Transverse Reinforcement Details
public:
	void GetSerdKeyList(CArray<T_SERD_K, T_SERD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSerd(T_SERD_K Key);
	BOOL GetSerd(T_SERD_K Key,T_SERD_D& rData);
	int  GetSerdList(CArray<T_SERD_K, T_SERD_K>& raKey, CArray<T_SERD_D, T_SERD_D&>& raData);
	int GetCountSerd();
	POSITION GetStartSerd();
	void GetNextSerd(POSITION& rNextPosition,T_SERD_K& rKey,T_SERD_D& rData);
	BOOL GetSerd_default(T_SERD_K Key,T_SERD_D& rData);

	// Seismic Performance Design - Seismic Rebar Detail ( Beam )
public:
	void GetSebeKeyList(T_SEBE_K_LIST& raKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSebe(T_SEBE_K Key);
	BOOL GetSebe(T_SEBE_K Key, T_SEBE_D& rData);
	int  GetSebeList(T_SEBE_K_LIST& raKey, T_SEBE_D_LIST& raData);
	int  GetCountSebe();
	POSITION GetStartSebe();
	void GetNextSebe(POSITION& rNextPosition, T_SEBE_K& rKey, T_SEBE_D& rData);
	BOOL GetSebe_default(T_SEBE_K Key, T_SEBE_D& rData);

	// Seismic Performance Design - Seismic Rebar Detail ( Column )
public:
	void GetSecoKeyList(T_SECO_K_LIST& raKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSeco(T_SECO_K Key);
	BOOL GetSeco(T_SECO_K Key, T_SECO_D& rData);
	int  GetSecoList(T_SECO_K_LIST& raKey, T_SECO_D_LIST& raData);
	int  GetCountSeco();
	POSITION GetStartSeco();
	void GetNextSeco(POSITION& rNextPosition, T_SECO_K& rKey, T_SECO_D& rData);
	BOOL GetSeco_default(T_SECO_K Key, T_SECO_D& rData);

	// Seismic Performance Design - Special Boundary Element ( Wall )
public:
	void GetSewaKeyList(T_SEWA_K_LIST& raKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSewa(T_SEWA_K Key);
	BOOL GetSewa(T_SEWA_K Key, T_SEWA_D& rData);
	int  GetSewaList(T_SEWA_K_LIST& raKey, T_SEWA_D_LIST& raData);
	int  GetCountSewa();
	POSITION GetStartSewa();
	void GetNextSewa(POSITION& rNextPosition, T_SEWA_K& rKey, T_SEWA_D& rData);
	BOOL GetSewa_default(T_SEWA_K Key, T_SEWA_D& rData);

	// Seismic Performance Design - Plastic Hinge Length for Fiber Wall
public:
	void GetHlfwKeyList(T_HLFW_K_LIST& raKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistHlfw(T_HLFW_K Key);
	BOOL GetHlfw(T_HLFW_K Key, T_HLFW_D& rData);
	int  GetHlfwList(T_HLFW_K_LIST& raKey, T_HLFW_D_LIST& raData);
	int  GetCountHlfw();
	POSITION GetStartHlfw();
	void GetNextHlfw(POSITION& rNextPosition, T_HLFW_K& rKey, T_HLFW_D& rData);
	BOOL GetHlfw_default(T_HLFW_K Key, T_HLFW_D& rData);

	// Seismic Performance Design - Shear Span of Beam / Column
public:
	void GetSespKeyList(T_SESP_K_LIST& raKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSesp(T_SESP_K Key);
	BOOL GetSesp(T_SESP_K Key, T_SESP_D& rData);
	int  GetSespList(T_SESP_K_LIST& raKey, T_SESP_D_LIST& raData);
	int  GetCountSesp();
	POSITION GetStartSesp();
	void GetNextSesp(POSITION& rNextPosition, T_SESP_K& rKey, T_SESP_D& rData);
	BOOL GetSesp_default(T_SESP_K Key, T_SESP_D& rData);

	// Time History Load Combination
public:
	BOOL MakeKeyThlc(T_THLC_D& rData, T_THLC_K& rKey);
	BOOL ExistThlc(T_THLC_K Key);
	BOOL GetThlc(T_THLC_K Key,T_THLC_D& rData);
	int GetCountThlc();
	POSITION GetStartThlc();
	void GetNextThlc(POSITION& rNextPosition,T_THLC_K& rKey,T_THLC_D& rData);
	void GetThlcKeyList(CArray<T_THLC_K, T_THLC_K>& rKeyList);  //Key 번호로 정렬된 리스트

	// Time History Load Combination (Json File Import)
public:
	BOOL MakeKeyThji(T_THJI_D& rData, T_THJI_K& rKey);
	BOOL ExistThji(T_THJI_K Key);
	BOOL GetThji(T_THJI_K Key, T_THJI_D& rData);
	int GetCountThji();
	POSITION GetStartThji();
	void GetNextThji(POSITION& rNextPosition, T_THJI_K& rKey, T_THJI_D& rData);
	void GetThjiKeyList(CArray<T_THJI_K, T_THJI_K>& rKeyList);  //Key 번호로 정렬된 리스트

	// Evaluation Group by Load Direction
public:
	BOOL ExistEgld(T_EGLD_K Key);
	BOOL GetEgld(T_EGLD_K Key,T_EGLD_D& rData);
	int GetCountEgld();
	POSITION GetStartEgld();
	void GetNextEgld(POSITION& rNextPosition,T_EGLD_K& rKey,T_EGLD_D& rData);
	void GetEglcSystemGroupList(CArray<CString, CString&>& aSystemGroup);
	CString GetEglcGroupString(int nSystemType);
	T_EGLD_K GetStartNumEgld();
	void GetEgldKeyList(CArray<T_EGLD_K, T_EGLD_K>& rKeyList);
	void GetEgldKeyList_User(CArray<T_EGLD_K, T_EGLD_K>& rKeyList);
	int GetEgldKeyListByLoadCase(OUT CArray<T_EGLD_K, T_EGLD_K>& rKeyList, IN int nLCType, IN int LCKey, IN T_ELEM_K ElemK=0);
	CString GetEgldLoadCaseName(T_EGLD_D& rData);

	// Select Inelastic Hinge Result Output - Element
public:
	void GetSihoKeyList(CArray<T_SIHO_K, T_SIHO_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSiho(T_SIHO_K Key);
	BOOL GetSiho(T_SIHO_K Key,T_SIHO_D& rData);
	int GetCountSiho();
	POSITION GetStartSiho();
	void GetNextSiho(POSITION& rNextPosition,T_SIHO_K& rKey,T_SIHO_D& rData);
	BOOL CheckHingeResultExists(T_ELEM_K ElemK);
	BOOL CheckFiberResultExists(T_ELEM_K ElemK);

	// Select Inelastic Hinge Result Output - General Link
public:
	void GetSihgKeyList(CArray<T_SIHG_K, T_SIHG_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSihg(T_SIHG_K Key);
	BOOL GetSihg(T_SIHG_K Key,T_SIHG_D& rData);
	int GetCountSihg();
	POSITION GetStartSihg();
	void GetNextSihg(POSITION& rNextPosition,T_SIHG_K& rKey,T_SIHG_D& rData);
	BOOL CheckNlnkHingeResultExists(T_NLNK_K NlnkK);

	// Select Inelastic Hinge Result Output - Point Spring Support
public:
	void GetSihpKeyList(CArray<T_SIHP_K, T_SIHP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSihp(T_SIHP_K Key);
	BOOL GetSihp(T_SIHP_K Key, T_SIHP_D& rData);
	int GetCountSihp();
	POSITION GetStartSihp();
	void GetNextSihp(POSITION& rNextPosition, T_SIHP_K& rKey, T_SIHP_D& rData);

	// Select Pushover Hinge Result Output
public:
	void GetSphoKeyList(CArray<T_SPHO_K, T_SPHO_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistSpho(T_SPHO_K Key);
	BOOL GetSpho(T_SPHO_K Key,T_SPHO_D& rData);
	int GetCountSpho();
	POSITION GetStartSpho();
	void GetNextSpho(POSITION& rNextPosition,T_SPHO_K& rKey,T_SPHO_D& rData);

	//Parameters of Soil Properties
public:
	void GetPospKeyList(CArray<T_POSP_K, T_POSP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPosp(T_POSP_K Key);
	BOOL ExistPosp(const CString& strName);
	BOOL GetPosp(T_POSP_K Key, T_POSP_D& rData);
	int GetCountPosp();
	POSITION GetStartPosp();
	void GetNextPosp(POSITION& rNextPosition, T_POSP_K& rKey, T_POSP_D& rData);
	T_POSP_K GetStartNumPosp();
	T_POSP_K GetLastNumPosp();
	T_POSP_K GetPospKeyByName(const CString& strName);
	CString  GetPospNameByKey(T_POSP_K Key);
	//Parameters of Seismic Loads
public:
	void GetPoslKeyList(CArray<T_POSL_K, T_POSL_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL ExistPosl(T_POSL_K Key);
	BOOL GetPosl(T_POSL_K Key, T_POSL_D& rData);
	CString GetPoslCodeName(int nCodeType);
	CString GetPoslMethodName(int nMethod);
	int GetCodeTypeByPoslCodeName(const CString& strCodeName);
	int GetCountPosl();
	POSITION GetStartPosl();
	void GetNextPosl(POSITION& rNextPosition, T_POSL_K& rKey, T_POSL_D& rData);
	T_POSL_K GetStartNumPosl();
	T_POSL_K GetLastNumPosl();
	T_POSL_K GetPoslKeyByName(const CString& strName);
	CString  GetPoslNameByKey(T_POSL_K Key);

public:
	bool InsertPlateDeckElem(const std::map<double, PC_GIRDER_VEC_PLATE_DECK>& mapPlateDeck, const unsigned int matlKey, const double thickness, const CString& grupName) const;
	BOOL AddPlateDeckElem(int eltyp, T_MATL_K MatlK, UINT PropK, T_NODE_K NodeK1, T_NODE_K NodeK2, T_NODE_K NodeK3, T_NODE_K NodeK4, T_ELEM_K& ElemKAdded, double dAngle = 0.0) const;
	T_THIK_K GetOrAddPlateDeckThikToDB(double dThickness) const;
	BOOL AppendPlateDeckElemToGrup(CString strGrupName, std::vector<T_ELEM_K>& aElemK) const;
	BOOL AddPlateDeckRigidLink(T_NODE_K MastNodeK, std::vector<T_NODE_K> aDeckNodeKey, CString strGrupName, BOOL bReplace = TRUE) const;
	T_NODE_K GetOrAddNodeToPlateDeckDB(T_POINT3D posNode, double dMergeTol) const;
	double CAttrCtrl::TRUNC_PlateDeck(double x, double dMergeTol) const;

	std::vector<ElementPass> traceSegmentThroughMesh_Map(const std::map<int, T_NODE_D>& mapNodes,
														 const std::map<int, ElementD>& mapElems,
														 const T_NODE_D& startNode, const T_NODE_D& endNode);

	BOOL IntersectSegmentAB_CD(const T_NODE_D& A, const T_NODE_D& B, const T_NODE_D& C, const T_NODE_D& D,
							  T_NODE_D& cross,  double& tAB, double eps = 1e-10);
	void CheckPointSideAndDistance(const T_NODE_D& Center, const T_NODE_D& Vec, const T_NODE_D& Point, int& side, double& dist);
};

struct T_NEWLY_ACTIVATED_ELEM
{
	T_NEWLY_ACTIVATED_ELEM() { nStageId = 0; }
	T_NEWLY_ACTIVATED_ELEM(const T_NEWLY_ACTIVATED_ELEM& rSrc) { *this = rSrc; }
	T_NEWLY_ACTIVATED_ELEM& operator=(const T_NEWLY_ACTIVATED_ELEM& rhs)
	{
		nStageId = rhs.nStageId;
		aElem.Copy(rhs.aElem);
		aElemType.Copy(rhs.aElemType);
		return *this;
	}
	int nStageId;                // stage id(Key가 아니고 순서 번호이다.)
	CArray<T_ELEM_K, T_ELEM_K> aElem;
	CArray<int, int> aElemType;
};

class __MY_EXT_CLASS__ CNewlyActivatedElemInStag
{
public:
	CNewlyActivatedElemInStag() { m_bInit = FALSE; }
	virtual ~CNewlyActivatedElemInStag() {}
	void Initialize(CAttrCtrl* pAttrCtrl);
	BOOL GetNewlyActivatedElemList(int nElemType, T_STAG_K StagK, CArray<T_ELEM_K, T_ELEM_K>& raElem);

protected:
	BOOL MakeNewlyActivatedElemList(int nStageId);

private:
	CArray<T_STAG_K, T_STAG_K> m_aStagK;
	CMap<T_STAG_K, T_STAG_K, int, int> m_mStagK2Id;
	CMap<int, int, T_NEWLY_ACTIVATED_ELEM, T_NEWLY_ACTIVATED_ELEM&> m_mStagId2Elem;
	CMap<T_ELEM_K, T_ELEM_K, int, int> m_mElemK2Id; // 누적해서 Active된 요소들(데이타는 마지막으로 Active되었던 stage의 id)
	BOOL m_bInit;
	int  m_nLastStageId; // 최종 구해진 Stage Id
	CAttrCtrl* m_pAttrCtrl;
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#include "HeaderPost.h"
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#endif // !defined(__ATTRCTRL_DB_H__)
