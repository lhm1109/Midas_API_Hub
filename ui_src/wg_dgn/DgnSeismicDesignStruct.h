#if !defined(__DGNSEISMICDESIGNSTRUCT_H__)
#define __DGNSEISMICDESIGNSTRUCT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DB_ST_DT_UNIT.h"

// 설계결과 값은 Code 단위계로 넘겨줍니다. (N, mm)

struct _DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES
{
	// 보유수평내력의 조사
	int     nFailureType;  //파괴형태(0:휨파괴형, 1:휨파괴에서 전단파괴 이행형, 2:전단파괴형)
	CString strFailureType;//파괴형태(nFailureType에 대한 문자열)
	double  dKhc;   //설계수평진도
	double  dmua;   //허용소성률(μa)
	double  dPa;    //지진시 보유수평내력
	BOOL    bCapaChk;   //_T("보유수평내력의 조사")에서 판정
	// 
	double  ddeltaR;  //잔류변위
	double  dmur;     //응답소성률
	BOOL    bDisChk;  //_T("잔류변위의 조사")에서 판정

	//수평내력 및 수평변위의 산출
	double  dMc;      //균열시의 휨모멘트 Mc
	double  dPhic;    //균열시의 곡률 φc
	double  dPc;      //균열시의 수평내력 Pc
	double  ddeltac;  //균열시의 수평변위 δc
	double  dMy0;     //초기항복시의 휨모멘트 My0
	double  dPhiy0;   //초기항복시의 곡률 φy0
	double  dPy0;     //초기항복시의 수평내력 Py0
	double  ddeltay0; //초기항복시의 수평변위 δy0
	double  dMy;      //항복시의 휨모멘트 My
	double  dPhiy;    //항복시의 곡률 φy
	double  dPy;      //항복시의 수평내력 Py
	double  ddeltay;  //항복시의 수평변위 δy
	double  dMu;      //종극시의 휨모멘트 Mu
	double  dPhiu;    //종극시의 곡률 φu
	double  dPu;      //종극시의 수평내력 Pu
	double  ddeltau;  //종극시의 수평변위 δu

	//콘크리트의 응력도 변형률관계
	CString strFilePath_MPhiFig;   //교각의 모멘트 곡률관계 삽도의 위치
	CString strFilePath_PDeltaFig; //교각의 수평력 변위관계 삽도의 위치

	//전단내력의 산출
	double  dPs;      //전단내력
	double  dPs0;     //정부교반작용의 영향에 관한 보정계수를 1.0으로써 산출한 전단내력
	double  dSc;      //콘크리트가 분담하는 전단내력
	double  dSs;      //철근이 분담하는 전단내력
	double  dcc;      //하중의 정부교반작용의 영향에 관한 보정계수
	double  dce;      //교각본체단면의 유효고d 에 관한 보정계수
	double  dcpt;     //축방향인장철근비 Pt 에 관한 보정계수
	double  dtc;      //콘크리트가 분담할 수 있는 평군전단응력도
	double  dbw;      //교각본체단면의 폭(직사각형 환산값)
	double  dDeff;    //교각본체단면의 유효고

	//설계수평진도
	double dKhc0;     //지진시보유수평내력에 사용하는 설계수평진도의 표준치 
	double dcs;       //구조물특성보정계수
	double dcz;       //지역별보정계수
	
	//등가중량
	double dW;        //지진시보유수평내력법에 사용하는 등가중량
	double dcp;       //등가중량산출계수
	
	//허용소성률
	double dalpha;    //교각의 허용소성류의 산출에 사용하는 안전계수

	//잔류변위 
	double dcR;       //잔류변위보정계수
	double dr;        //교각의 항복강성에 대한 강복후의 이차강성의 비

	void Initialize()
	{
		nFailureType   = 0;
		strFailureType = _T("");
		dKhc           = 0.0;
		dmua           = 0.0;
		dPa            = 0.0;
		bCapaChk       = FALSE;
		ddeltaR        = 0.0;
		dmur           = 0.0;
		bDisChk        = FALSE;
		dMc            = 0.0;
		dPhic          = 0.0;
		dPc            = 0.0;
		ddeltac        = 0.0;
		dMy0           = 0.0;
		dPhiy0         = 0.0;
		dPy0           = 0.0;
		ddeltay0       = 0.0;
		dMy            = 0.0;
		dPhiy          = 0.0;
		dPy            = 0.0;
		ddeltay        = 0.0;
		dMu            = 0.0;
		dPhiu          = 0.0;
		dPu            = 0.0;
		ddeltau        = 0.0;
		strFilePath_MPhiFig = _T("");
		strFilePath_PDeltaFig = _T("");
		dPs            = 0.0;
		dPs0           = 0.0;
		dSc            = 0.0;
		dSs            = 0.0;
		dcc            = 0.0;
		dce            = 0.0;
		dcpt           = 0.0;
		dtc            = 0.0;
		dbw            = 0.0;
		dDeff          = 0.0;
		dKhc0          = 0.0;
		dcs            = 0.0;
		dcz            = 0.0;
		dW             = 0.0;
		dcp            = 0.0;
		dalpha         = 0.0;
		dcR            = 0.0;
		dr             = 0.0;
	}
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES()	{ Initialize(); }
};

struct _DGN_RESERVE_LATERAL_CAPA_MEMB_RES
{
	BOOL    bIsRes;     //Result정보를 가지고 있는지 여부
	CString strName;    //이름	
	BOOL    bTraverse;  //교축방향 설계여부
	BOOL    bLongitude; //교축직각방향 설계여부
	//모델제원
	double  dLength;    //천단에서의 거리(부재의 수직상의 길이)
	int     nDivSize;   //분할수
	CString strShape;   //단면형상
	double  dWidth;     //폭
	double  dHeight;    //높이	
	//부재제원
	CString strType;    //부재의 형식
	double  dWu;        //상부공중량
	double  dWp_Beam;   //교각본체dml 보부분의 중량
	double  dWp;        //교각본체중량
	double  dM0[2];     //상부구조등의 사하중에 의한 편심 모멘트 Mo (0:교축방향, 1:교축직각방향)
	double  dActHeight; //교각기부에서 상부구조관성력의 작용위치까지의 거리 h
	double  dRD;        //사하중반력  RD
	double  ddeltaRa;   //허용잔류변위
	double  dT[2];      //지진시의 보유수평내력법에 있어서 교각의 고유주기 (0:교축방향, 1:교축직각방향) 
	//재료제원
	double  dWeightDensity;//철근 콘크리트의 단위 체적중량 γ
	double  dEc_conc;   //콘크리트의 탄성계수 Ec
	double  dfck_conc;  //콘크리트의 설계기준강도 σck
	double  dEs_rebar;  //축방향철근의 탄성계수Es
	double  dfsy_rebar; //축방향철근의 강복점 σsy(
	double  da_tie;     //띠철근의 간격a
	double  dAngle_tie; //띠철근과 연직축과의 각도(Degree˚)
	double  dAw_tie[2]; //A와 φ로 배근되는 띠철근의 단면적Aw (0:교축방향, 1:교축직각방향) 
	double  dfsy_tie;   //띠철근의 강복점 σsy
	
	double  dRhos[2];   //획구속근의 체적비 (0:교축방향, 1:교축직각방향) 
	double  dEdes[2];   //하강구배 (0:교축방향, 1:교축직각방향) 
	double  decc[2];    //최대압축응력시의 변형률 (0:교축방향, 1:교축직각방향) 
	double  decu[2];    //최종 변형률 (0:교축방향, 1:교축직각방향) 
	double  dSigcc[2];  //최대압축강도 (0:교축방향, 1:교축직각방향) 	
	//단면삽도
	CString strFilePath_ShapeFig; //단면형상 삽도의 위치

	//
	double  dPhi0[2];      //사하중시의 곡률φ0 (0:교축방향, 1:교축직각방향) 
	double  dP0[2];        //사하중시의 수평내력 P0 (0:교축방향, 1:교축직각방향) 
	double  ddelta0[2];    //사하중시의 수평변위 δ0 (0:교축방향, 1:교축직각방향) 
	//
	double dLp[2];         //소정힌지장 (0:교축방향, 1:교축직각방향) 
	int    nLpType[2];     //소정힌지장 산출근거 (0:교축방향, 1:교축직각방향) (0:0.1D≤Lp≤0.5D,  1:Lp<0.1D,  2:Lp>0.5D

	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType1Trav;//타입1지진동에 대한 교축방향 안정검토
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType1Long;//타입1지진동에 대한 교축직각방향 안정검토
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType2Trav;//타입2지진동에 대한 교축방향 안정검토
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType2Long;//타입2지진동에 대한 교축직각방향 안정검토

	//
	UINT ElemK_Bottom;
	int  iPosi_Bottom;
	CArray<UINT, UINT> KeyList;

	void Initialize()
	{
		bIsRes     = FALSE;
		strName    = _T("");
		bTraverse  = TRUE;
		bLongitude = TRUE;

		dLength    = 0.0;
		nDivSize   = 0;
		strShape   = _T("");
		dWidth     = 0.0;
		dHeight    = 0.0;

		strType    = _T("");
		dWu        = 0.0;
		dWp_Beam   = 0.0;
		dWp        = 0.0;
		dM0[0]     = 0.0;
		dM0[1]     = 0.0;
		dActHeight = 0.0;
		dRD        = 0.0;
		ddeltaRa   = 0.0;
		dT[0]      = 0.0;
		dT[1]      = 0.0;

		dWeightDensity = 0.0;
		dEc_conc   = 0.0;
		dfck_conc  = 0.0;
		dEs_rebar  = 0.0;
		dfsy_rebar = 0.0;
		da_tie     = 0.0;
		dAngle_tie = 0.0;
		dAw_tie[0] = 0.0;
		dAw_tie[1] = 0.0;
		dfsy_tie   = 0.0;
		
		dRhos[0]   = 0.0;
		dRhos[1]   = 0.0;
		dEdes[0]   = 0.0;
		dEdes[1]   = 0.0;
		decc[0]    = 0.0;
		decc[1]    = 0.0;
		decu[0]    = 0.0;
		decu[1]    = 0.0;
		dSigcc[0]  = 0.0;
		dSigcc[1]  = 0.0;
		
		strFilePath_ShapeFig = _T("");
		
		dPhi0[0]   = 0.0;
		dPhi0[1]   = 0.0;
		dP0[0]     = 0.0;
		dP0[1]     = 0.0;
		ddelta0[0] = 0.0;
		ddelta0[1] = 0.0;
		
		dLp[0]     = 0.0;
		dLp[1]     = 0.0;
		nLpType[0] = 0;
		nLpType[1] = 0;

		ResType1Trav.Initialize();
		ResType1Long.Initialize();
		ResType2Trav.Initialize();
		ResType2Long.Initialize();

		ElemK_Bottom = 0;
		iPosi_Bottom = 0;
		KeyList.RemoveAll();
	}
	_DGN_RESERVE_LATERAL_CAPA_MEMB_RES()	{ Initialize(); }
	_DGN_RESERVE_LATERAL_CAPA_MEMB_RES(_DGN_RESERVE_LATERAL_CAPA_MEMB_RES& rData)	{*this = rData;}
	_DGN_RESERVE_LATERAL_CAPA_MEMB_RES& operator = (_DGN_RESERVE_LATERAL_CAPA_MEMB_RES& rData)
	{
		Initialize();
		bIsRes     = rData.bIsRes    ;
		strName    = rData.strName   ;
		bTraverse  = rData.bTraverse ;
		bLongitude = rData.bLongitude;

		dLength    = rData.dLength   ;
		nDivSize   = rData.nDivSize  ;
		strShape   = rData.strShape  ;
		dWidth     = rData.dWidth    ;
		dHeight    = rData.dHeight   ;

		strType    = rData.strType   ;
		dWu        = rData.dWu       ;
		dWp_Beam   = rData.dWp_Beam  ;
		dWp        = rData.dWp       ;
		dM0[0]     = rData.dM0[0]    ;
		dM0[1]     = rData.dM0[1]    ;
		dActHeight = rData.dActHeight;
		dRD        = rData.dRD       ;
		ddeltaRa   = rData.ddeltaRa  ;
		dT[0]      = rData.dT[0]     ;
		dT[1]      = rData.dT[1]     ;

		dWeightDensity = rData.dWeightDensity;
		dEc_conc   = rData.dEc_conc  ;
		dfck_conc  = rData.dfck_conc ;
		dEs_rebar  = rData.dEs_rebar ;
		dfsy_rebar = rData.dfsy_rebar;
		da_tie     = rData.da_tie    ;
		dAngle_tie = rData.dAngle_tie;
		dAw_tie[0] = rData.dAw_tie[0];
		dAw_tie[1] = rData.dAw_tie[1];
		dfsy_tie   = rData.dfsy_tie  ;
		
		dRhos[0]   = rData.dRhos[0]  ;
		dRhos[1]   = rData.dRhos[1]  ;
		dEdes[0]   = rData.dEdes[0]  ;
		dEdes[1]   = rData.dEdes[1]  ;
		decc[0]    = rData.decc[0]   ;
		decc[1]    = rData.decc[1]   ;
		decu[0]    = rData.decu[0]   ;
		decu[1]    = rData.decu[1]   ;
		dSigcc[0]  = rData.dSigcc[0] ;
		dSigcc[1]  = rData.dSigcc[1] ;
		
		strFilePath_ShapeFig = rData.strFilePath_ShapeFig;
		
		dPhi0[0]   = rData.dPhi0[0]  ;
		dPhi0[1]   = rData.dPhi0[1]  ;
		dP0[0]     = rData.dP0[0]    ;
		dP0[1]     = rData.dP0[1]    ;
		ddelta0[0] = rData.ddelta0[0];
		ddelta0[1] = rData.ddelta0[1];
				
		dLp[0]     = rData.dLp[0]    ;
		dLp[1]     = rData.dLp[1]    ;
		nLpType[0] = rData.nLpType[0];
		nLpType[1] = rData.nLpType[1];

		ResType1Trav = rData.ResType1Trav;
		ResType1Long = rData.ResType1Long;
		ResType2Trav = rData.ResType2Trav;
		ResType2Long = rData.ResType2Long;
		
		ElemK_Bottom = rData.ElemK_Bottom;
		iPosi_Bottom = rData.iPosi_Bottom;
		KeyList.Copy(rData.KeyList);
		return *this;
	}
};

struct _DGN_RESERVE_LATERAL_CAPA_RES
{
	T_UNIT_INDEX DataUnit;
	BOOL bEarthquakeTypeI;  // 지진동형식 TypeI 검토여부
	BOOL bEarthquakeTypeII; // 지진동형식 TypeII 검토여부
	CArray<_DGN_RESERVE_LATERAL_CAPA_MEMB_RES, _DGN_RESERVE_LATERAL_CAPA_MEMB_RES&> List;
	CString strComp; //사업주체
	CString strWork; //공사명칭
	CString strCode; //설계기준

	CString strAnalysis;    // 설계방법
	CString strBridgeType;  // 교량의 종류
	CString strRegionalKind;// 지역구분
	CString strGroundKind;  // 지반의 종류
	CString strDgnLevel;    // 내진성능	(문자열)
	int     nDgnLevel;      // 내진성능	(0:내진성능3, 1:내진성능2)

	double  dczKhc0_LimType1;// TypeI 지진동 검토시의 cz*khc0의 최소한계값(0.3)
	double  dczKhc0_LimType2;// TypeII 지진동 검토시의 cz*khc0의 최소한계값(0.6)

	CString str2DimFileName;
	CString str3DimFileName;
	void Initialize()
	{		
		DataUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		DataUnit.nBase_Force	= D_UNITSYS_FORCE_INDEX_N;		// N.
		bEarthquakeTypeI = TRUE;
		bEarthquakeTypeI = TRUE;
		List.RemoveAll();
		strComp          = _T("");
		strWork          = _T("");
		strCode          = _T("");
		strAnalysis      = _T("");
		strBridgeType    = _T("");
		strRegionalKind  = _T("");
		strGroundKind    = _T("");
		strDgnLevel      = _T("");
		nDgnLevel        = 0;
		dczKhc0_LimType1 = 0.0;
		dczKhc0_LimType2 = 0.0;
		str2DimFileName  = _T("");
		str3DimFileName  = _T("");
	}
	_DGN_RESERVE_LATERAL_CAPA_RES()	{ Initialize(); }
	_DGN_RESERVE_LATERAL_CAPA_RES(_DGN_RESERVE_LATERAL_CAPA_RES& rData)	{*this = rData;}
	_DGN_RESERVE_LATERAL_CAPA_RES& operator = (_DGN_RESERVE_LATERAL_CAPA_RES& rData)
	{
		Initialize();
		bEarthquakeTypeI = rData.bEarthquakeTypeI;
		bEarthquakeTypeII= rData.bEarthquakeTypeII;
		strComp          = rData.strComp         ;
		strWork          = rData.strWork         ;
		strCode          = rData.strCode         ;
		strAnalysis      = rData.strAnalysis     ;
		strBridgeType    = rData.strBridgeType   ;
		strRegionalKind  = rData.strRegionalKind ;
		strGroundKind    = rData.strGroundKind   ;
		strDgnLevel      = rData.strDgnLevel     ;
		nDgnLevel        = rData.nDgnLevel       ;
		dczKhc0_LimType1 = rData.dczKhc0_LimType1;
		dczKhc0_LimType2 = rData.dczKhc0_LimType2;
		str2DimFileName  = rData.str2DimFileName ;
		str3DimFileName  = rData.str3DimFileName ;

		int nSize = rData.List.GetSize();
		List.SetSize(nSize);
		for(int i=0 ; i<nSize ; i++)
			List.SetAt(i, rData.List[i]);

		return *this;
	}
};

#endif // !defined(__DGNSEISMICDESIGNSTRUCT_H__)
