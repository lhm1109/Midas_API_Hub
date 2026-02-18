#pragma once

#include "SeismicDesignBase.h"

class CDBDoc;
class CDgnProgressDlg;

struct ColumnData;

struct ParamAngleOption
{
public:
	T_THIS_K kThis;
	T_ELEM_K kElem;
	int nDirDisp;				//변위 방향 : enum ParamAngleOption::eDispDirOpt
	int nShearDir;				//교축방향 전단력 ParamAngleOption::eAxisShearDir
	double dUserAngle;
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection

	ParamAngleOption()
	{
		kThis = 0;
		kElem = 0;
		nDirDisp = 0;
		nShearDir = 0;
		dUserAngle = 0;
		nSeismicForceDirection = 0;
	}

	//변위방향 옵션(= enum T_SIMD_D, T_SIRD_D::eDirection ...)
	enum eDispDirOpt
	{
		kUnKnown_DispDir,
		kSeismicForce_Dir,	//지진력 방향
		kLocal_Dir,			//요소 좌표계 방향
		kUser_Dir,			//사용자 설정
	};

	//교축방향 전단력(= enum enum T_SIGR_UPPER_D, T_SIGR_USER_D, T_SIGR_LOWER_COLM_D, T_SIGR_LOWER_BEAM_D::eDirection ...)
	enum eAxisShearDir
	{
		kUnKnown_ShearDir,
		kY_Dir,
		kZ_Dir,
	};
};

/**
* tParam[0] T_SIGR_K : 하부구조그룹 key
* tParam[1] T_THIS_K : 시간이력해석 케이스 key
* tParam[2] T_NODE_K : 상부공 관성력 작용 위치 절점 key
* tParam[3] enum seis_jp::eDirAnal
* tParam[4] enum CSeismicDesignDisp_Lower::eRspCalcType
* tParam[5] ColumnData* : 기둥 최하단 요소의 정보
*/
typedef std::tuple<T_SIGR_K, T_THIS_K, T_NODE_K, seis_jp::eDirAnal, /*eRspCalcType*/int, ColumnData*> PDeltaVal;
// int nLpFormula, int nExamTarget, int nLocalAxis, double d_h, ColumnData* pColumn
typedef std::tuple<int, int, int, double, ColumnData*> PLp;
class CSeismicDesignDisp : public CSeismicDesignBase
{
protected:
	enum eLocal2d
	{
		kLy,
		kLz,
		kMax2d,
	};

	CSeismicDesignDisp();
	virtual ~CSeismicDesignDisp();

	/**
	* 최대응답변위, 잔류변위, 변위 소성율 조사의 변위방향 옵션에 대한 공통 함수
	* param : 변위방향 옵션에 따른 각도를 구하기 위한 파라미터
	* dGlobalDx : 해석결과 값으로 부터 얻은 Global X 방향 변위
	* dGlobalDy : 해석결과 값으로 부터 얻은 Global Y 방향 변위
	* dCompDisp : dGlobalDx, dGlobalDy 변위를 변위방향 옵션에 따른 합성벡터 크기
	*/
	void GetDispOfAngle(const ParamAngleOption& param, const double& dGlobalDx, const double& dGlobalDy, OUT double& dCompDisp);

	void GetAngleByOption(const ParamAngleOption& param, OUT double& dDispAngle);

	/**
	* 최대응답변위(H29), 변위 소성율 조사의 공통 함수
	* kNodeTop : 기둥의 상단 절점
	* kNodeLow : 기둥의 하단 절점
	* d_h : 기둥의 높이
	*/
	BOOL GetH(const T_NODE_K& kNodeTop, const T_NODE_K& kNodeLow, OUT double& d_h);

	/**
	* 최대응답변위(H29), 변위 소성율 조사의 공통 함수
	* nLpFormula : enum T_SIDP_LSLP_D::eLpFormula
	* nExamTarget : enum T_SIPA_D::eExamTarget
	* nLocalAxis : #define LOCAL_AXIS_?
	* pColumn : 기둥의 하단 정보
	* d_h : 기둥의 높이
	*/
	BOOL GetLp(IN PLp& param, OUT double& dLp);
	BOOL GetLpByD_h(IN PLp& param, OUT double& dLp);
	BOOL GetLpByD_h_Sect(IN PLp& param, OUT double& dLp);
	BOOL GetLpByD_h_Rein(IN PLp& param, IN T_REIN_SSM_D* pReinSsmD, OUT double& dLp);

	/**
	* 최대응답변위, 변위 소성율 조사의 공통 함수
	* kElem : 요소번호
	* nComponent : enum QIehp::eCom
	* nDirAnal : enum seis_jp::eDirAnal
	* ePosNodeLow : enum T_SIGR_LOWER_COLM_D::ePosType
	*/
	BOOL CalcPhiY_U(const T_ELEM_K& kElem, const int& nComponent, const seis_jp::eDirAnal& nDirAnal, const int& ePosNodeLow, OUT double& dPHI_y, OUT double& dPHI_u);

	/**
	* 최대응답변위, 변위 소성율 조사의 공통 함수
	* kElem : 요소번호
	* nComponent : enum QIehp::eCom
	* nDirAnal : enum seis_jp::eDirAnal
	* ePosNodeLow : enum T_SIGR_LOWER_COLM_D::ePosType
	*/
	BOOL GetPhiY_U(const T_ELEM_K& kElem, const int& nComponent, const seis_jp::eDirAnal& nDirAnal, const int& ePosNodeLow, OUT double& dPhiY, OUT double& dPhiU);

	/**
	* 최대응답변위 조사의 공통 함수
	* kElem : 요소번호
	* nComponent : enum QIehp::eCom
	* nDirAnal : enum seis_jp::eDirAnal
	* ePosNodeLow : enum T_SIGR_LOWER_COLM_D::ePosType
	*/
	BOOL GetMomentY_U(const T_ELEM_K& kElem, const int& nComponent, const seis_jp::eDirAnal& nDirAnal, const int& ePosNodeLow, OUT double& dMy, OUT double& dMu);

	/**
	* 로컬축 기준의 값을 Global 축 기준으로 변환
	* kElem : 요소번호
	* ePosNodeLow : enum T_SIGR_LOWER_COLM_D::ePosType
	* dVal_My : 로컬 Y축 값
	* dVal_Mz : 로컬 Z축 값
	* dVal[eVector2d::kMax2d] : 변환된 Global 축 기준 값
	*/
	void TransformLocalToGlobal(const T_ELEM_K& kElem, const int& ePosNodeLow, const double& dVal_My, const double& dVal_Mz, double* dVal);

	/**
	* 최대응답변위 조사의 공통 함수
	*/
	BOOL GetDispFromTimeHistory(const T_THIS_K& kThis, const T_NODE_K& kNode, const double& nStep, OUT double& dDx, OUT double& dDy);

	/**
	* 최대응답변위 조사의 공통 함수
	*/
	BOOL GetRotaFromTimeHistory(const T_THIS_K& kThis, const T_NODE_K& kNode, const double& nStep, OUT double& dRx, OUT double& dRy);

	/**
	 * 최대응답변위, 잔류변위 조사의 공통 함수
	 * δfls：시간이력해석 스텝에 해당하는 변위
	 * δfls = δfp+Θfp*h+δs
	 *
	 * tParam : <해석케이스, 기둥 하부절점, 기둥 관성력 작용위치 절점>
	 * dTimeStep : 시간이력해석 스텝
	 * dFls_Dx : Grobal X축 방향 변위
	 * dFls_Dy : Grobal Y축 방향 변위
	 */
	BOOL GetF_ls(IN PDeltaVal& tParam, const double& dTimeStep, OUT double& dFls_Dx, OUT double& dFls_Dy);
	// ↓↓↓↓ GetF_ls() 관련함수 ↓↓↓↓
	BOOL GetDelta_fd(IN PDeltaVal& tParam, const double& dTimeStep, OUT double& dDx, OUT double& dDy);
	BOOL GetTheta_fp(IN PDeltaVal& tParam, const double& dTimeStep, OUT double& dRx, OUT double& dRy);
	BOOL Get_h(const T_NODE_K& kNodeLow, const T_NODE_K& kPierTopNode, OUT double& d_h);

	void GetDispOfBearing(IN PDeltaVal& tParam, const int& nTimeIndex, OUT double* pDelta_s);
	BOOL FindBearingNode(IN PDeltaVal& tParam, OUT T_NODE_K& nodeBot, OUT T_NODE_K& nodeTop);
};

class CSeismicDesignDisp_Upper : public CSeismicDesignDisp
{
public:
	CSeismicDesignDisp_Upper() : CSeismicDesignDisp() {};
	virtual ~CSeismicDesignDisp_Upper() {};

	/**
	* δmax ：kNode 위치의 응답 변위
	*/
	BOOL CalcDeltaMax(const T_NODE_K& kNode, const T_THIS_K& kThis, const int& nTimeIdx, OUT double* pDeltaMax);
};

typedef std::tuple<T_SIPA_K, T_SIGR_K, T_NODE_K, CString, seis_jp::eDirAnal, /*T_SIPA_D::eSeismicForceDirection*/int, ColumnData*> PDeltaYandU;
class CSeismicDesignDisp_Lower : public CSeismicDesignDisp
{
public:
	// = T_SIMD_D::eRspCalcType, T_SIRD_D::eRspCalcType
	enum eRspCalcType
	{
		kUnKnownRspType = 0,
		kNEXCO,			//설계요령 제2집 교량 건설편
		kJBEC,			//기설 교량의 내진보강 공법 사례집
	};

	enum eRotate2d
	{
		kRx = 0,
		kRy,
		kMax2r
	};

	CSeismicDesignDisp_Lower() : CSeismicDesignDisp() {};
	virtual ~CSeismicDesignDisp_Lower() {};

	/**
	 * δmax = δrmax-δfd-Θfp×h-δs
	 * δmax："한계상태 시의 교각"에 대한 "상부공 관성력 작용 위치(교축,직각)"에서 지정한 절점에 대해 동시발생결과로 산출(m)
	 */
	BOOL CalcDeltaMax(	IN PDeltaVal& tParam, 
						const ParamAngleOption& angleP, 
						const int& nTimeIndex,
						OUT doubleArray& aDeltaMax);

	BOOL CalcDeltaMaxOnStepByNexco(	IN PDeltaVal& tParam,
									const ParamAngleOption& angleP,
									const int& nTimeIndex,
									OUT double& dDeltaMax);

	BOOL CalcDeltaMaxOnStepByJbec(	IN PDeltaVal& tParam,
									const ParamAngleOption& angleP,
									const int& nTimeIndex,
									OUT double& dDeltaMax);

	/**
	* δry, δru(δrls) ：최대응답변위 교각별 검토시 해당 스텝일때 교각의 관성력 작용위치에서의 기둥의 순수 수평변위
	*/
	BOOL CalcDeltaValAtStep(IN PDeltaVal& tParam,
							const int& nTimeStep,
							OUT double& dDls_Dx,
							OUT double& dDls_Dy);

	BOOL CalcDeltaValAtStepByNexco(	IN PDeltaVal& tParam,
									const int& nTimeStep,
									OUT double& dDls_Dx,
									OUT double& dDls_Dy);

	BOOL CalcDeltaValAtStepByJbec(	IN PDeltaVal& tParam,
									const int& nTimeStep,
									OUT double& dDls_Dx,
									OUT double& dDls_Dy);

	BOOL FindNodeRelatedPier(	IN PDeltaVal& tParam,
								OUT T_NODE_K& kPierTopNode,
								OUT T_NODE_K& kPierBotNode,
								OUT T_NODE_K& kBearingTopNode,
								OUT T_NODE_K& kBearingBotNode);

	/**
	* 최대응답변위, 잔류 변위 조사의 공통 함수
	* PDeltaYandU : 파라미터
	* aDeltaY : M-φ관계에 의한 곡률분포를 적분하여 산출된 δy0
	* aDeltaU : M-φ관계에 의한 곡률분포를 적분하여 산출된 δu0
	*/
	BOOL GetDeltaYandU(IN PDeltaYandU& param, OUT double& rDeltaY, OUT double& rDeltaU);
};