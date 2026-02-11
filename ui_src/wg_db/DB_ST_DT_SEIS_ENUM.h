#pragma once

typedef CArray<double, double> doubleArray;

namespace seis_jp
{
	/**
	* 응답변위(δmax) 계산방법
	*/
	enum eRspCalcType
	{
		kRspType_0 = 0,
		kNEXCO,			//설계요령 제2집 교량 건설편
		kJBEC,			//기설 교량의 내진보강 공법 사례집
	};

	/**
	* 허용변위(δa) 계산방법
	*/
	enum eAlwCalcType
	{
		kAlwCalcType_0 = 0,
		kAnalysisResult,//해석결과로 산정
		kMphi_Dy,		//M-φ로 산정(δy)
		kUserInput,		//제한값 직접 입력
		kMphi_Dpy,		//M-φ로 산정(δpy) - v965 추가
	};

	/**
	* JBEC 응답변위(δmax) 계산방법 - 既設橋梁の耐震補強工法事例集
	* (橋脚別)
	* RC : δmax=δt+Θt×h1-Θb×h'-δb
	*/
	enum eDmaxByJbec
	{
		kDmaxJbec = 0,	//δmax
		kDt,			//δt
		kTt,			//Θt
		kh1,			//h1
		kTb,			//Θb
		kh_,			//h'
		kDb,			//δb
		kSizeJbec,
	};

	/**
	* NEXCO 응답변위(δmax) 계산방법 - 設計要領第二集橋梁建設編
	* (橋脚別)
	* RC : δmax=δrmax-δfp-Θfp×h-δs
	*/
	enum eDmaxByNexco
	{
		kDmaxNexco = 0,	//δmax
		kDrmax,			//δrmax
		kDfp,			//δfp
		kTfp,			//Θfp
		kh,				//h
		kDs,			//δs
		kSizeNexco,
	};

	/**
	* H14 허용변위 계산식 (해석결과로)
	* (全体系)
	* RC : δa＝δy＋(δu-δy)/α
	* ST : δa＝δra
	* (橋脚別)
	* RC : δa＝δy＋(δu-δy)/α
	*			δy=δry-δfp-Θfp×h-δs
	*			δu=δru-δfp-Θfp×h-δs
	* ST : δa=δra-δfp-Θfp×h-δs
	*/
	enum eDa_AnalH14
	{
		kDa_AnalH14=0,
		kDy_AnalH14,
		kDu_AnalH14,
		kDry_AnalH14,
		kDru_AnalH14,//강재일 떄 δra
		kDfp_AnalH14,
		kTfp_AnalH14,
		kh_AnalH14,
		kDs_AnalH14,
		kSizeDa_AnalH14,
	};

	/**
	* H14 허용변위 계산식 (M-φ로)
	* (橋脚別)
	* RC : δa＝δy＋(δu-δy)/α
	*			δu=δy+(Φu-φy)Lp(h-Lp/2))
	* ST : δa＝柱基部がφaに達するときの曲率分布を積分して求める
	*/
	enum eDa_MPhiH14
	{
		kDa_MPhiH14 = 0,
		kDy_MPhiH14,
		kDu_MPhiH14,
		kPu_MPhiH14,
		kPy_MPhiH14,
		kLp_MPhiH14,
		kh_MPhiH14,
		kSizeDa_MPhiH14,
	};

	/**
	* H14 허용변위 계산식 (유저값)
	*/
	enum eDa_UserH14
	{
		kDa_UserH14 = 0,
		kSizeDa_UserH14,
	};

	/**
	* H24 허용변위 계산식 (해석결과로)
	* (全体系)
	* RC : δa＝(δrls-δfls)/α＋δfls or δrls/α
	*	   δfls = δfp+Θfp*h+δs
	* ST : δa＝δra
	* (橋脚別)
	* RC : δa＝δls/α
	*			δls=δrls-δfp-Θfp×h-δs
	* ST : δa=δra-δfp-Θfp×h-δs
	*/
	enum eDa_AnalH24
	{
		kDa_AnalH24 = 0,
		kD_f_ls_AnalH24,// δfls 또는 δls
		kDrls_AnalH24,	//(강재)δra
		kDfp_AnalH24,
		kTfp_AnalH24,
		kh_AnalH24,
		kDs_AnalH24,
		kSizeDa_AnalH24,
	};

	/**
	* H24 허용변위 계산식 (M-φ로)
	*  (橋脚別)
	* RC : δa＝δls/α
	*			δls=δy+(Φls-φy)Lp(h-Lp/2)
	* ST : δa＝柱基部がφaに達するときの曲率分布を積分して求める
	*/
	enum eDa_MPhiH24
	{
		kDa_MPhiH24 = 0,
		kDls_MPhiH24,
		kDy_MPhiH24,
		kPls_MPhiH24,
		kPy_MPhiH24,
		kLp_MPhiH24,
		kh_MPhiH24,
		kSizeDa_MPhiH24,
	};

	/**
	* H24 허용변위 계산식 (유저값)
	*/
	enum eDa_UserH24
	{
		kDa_UserH24 = 0,
		kSizeDa_UserH24,
	};

	/**
	* H29 허용변위 계산식 (해석결과로)
	* (全体系)
	* RC : δlsd＝ξ1・Φs(δrls-k・δfls)＋δfls　or　ξ1・Φs・δrls
	*								δfls = δfp+Θfp*h+δs
	* ST : δlsd＝ξ1・Φs(δra-k・δfls)＋δfls　or　ξ1・Φs・δra
	* (橋脚別)
	* δlsd＝ξ1・Φs・δls
	* δls=δrls-δfd-Θfp×h-δs
	* 
	*/
	enum eDa_AnalH29
	{
		kDlsd_AnalH29 = 0,
		kD_f_ls_AnalH29,// δfls 또는 δls
		kDrls_AnalH29,	//(강재)δra
		kDfp_AnalH29,
		kTfp_AnalH29,
		kh_AnalH29,
		kDs_AnalH29,
		kPs_AnalH29,
		kSizeDa_AnalH29,
	};

	/**
	* H29 허용변위 계산식 (M-φ로)
	* (橋脚別)
	* δlsd＝ξ1・Φs・δls
	* RC : δls=k2・(δyE+(φls-φy)Lp(h-Lp/2))
	* ST : δls＝k・δa
	*/
	enum eDa_MPhiH29
	{
		kDlsd_MPhiH29 = 0,
		kDls_MPhiH29,
		kDyE_MPhiH29,
		kPls_MPhiH29,
		kPy_MPhiH29,
		kLp_MPhiH29,
		kh_MPhiH29,
		kPs_MPhiH29,
		kSizeDa_MPhiH29,
	};

	/**
	* H29 허용변위 계산식 (유저값)
	*/
	enum eDa_UserH29
	{
		kDa_UserH29 = 0,
		kSizeDa_UserH29,
	};

	/**
	* 해석방향
	*/
	enum eDirAnal
	{
		kDirAnal_0 = 0,
		kPlus,
		kMinus,
	};

	/**
	* 변위방향
	*/
	enum eDirDisp
	{
		kDirDisp_0 = 0,
		kSeismicForce_Dir,	//지진력 방향
		kLocal_Dir,			//요소 좌표계 방향
		kUser_Dir,			//사용자 설정
	};

	/**
	* 전단력 방향 (교축방향)
	*/
	enum eDirShear
	{
		kDirShear_0 = 0,
		kY_Dir,
		kZ_Dir,
		kYZ_Dir,
	};
}