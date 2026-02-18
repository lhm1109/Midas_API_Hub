// DgnBridgeLoadRating.h: interface for the CDgnKSCE_USD03 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_DGNBRIDGELOADRATING_H__INCLUDED_)
#define AFX_DGNBRIDGELOADRATING_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"

const double DBLR_PI = 4.0*atan(1.0);

struct _DBLR_FLEX_DATA
{
	ElemPairK ElemKey;
	int      iPosi;
	BOOL     bPositive;

	BOOL   bStrainCompatibility;                       //해석방법(TRUE=StrainCompatibility, FALSE=Code)
	double dphi;                                       //PSC의 휨강도계수 φ
	double dMn;                                        //공칭 휨강도
	double dphiMn;                                     //설계 휨강도  
	double dTp , dTp_bond, dTp_unbond;                 //파괴시 인장측의 유효프리스트레스력(dTp=전체, dTp_bond=부착, dTp_unbond=비부착)
	double dTst, dCsc, dCc;                            //파괴시 인장철근의 전체인장력, 압축철근의 전체압축력, 콘크리트의 압축력
	double dfps, dfps_bond, dfps_unbond;               //파괴시 인장층의 PS강재의 평균응력, 부착 PS강재의 평균응력, 비부착 PS강재의 평균응력
	double dfst, dfsc, dfcc;                           //인장철근의 평균응력, 압축철근의 평균응력, 콘크리트의  평균응력
	double ddp, ddp_bond, ddp_unbond;                  //인장측 유효PS력의 도심까지의 거리, 인장측 부착 PS력의 도심까지의 거리, 인장측 비부착 PS력의 도심까지의 거리
	double ddc, ddst, ddsc;                            //압축연단에서 응력블력의 도심까지의 거리, 인장철근의 인장력의 도심까지의 거리, 압축철근의 압축력의 도심까지의 거리
	double dAp, dAp_bond, dAp_unbond;                  //인장측 PS강재의 전체 단면적, 부착 PS강재의 전체 단면적, 비부착 PS강재의 전체 단면적
	double dAst, dAsc, dAc;                            //인장철근의 전체 단면적, 압축철근의 전체 단면적, 콘크리트의 압축블럭이 지배하는 면적
	int    iNum_bond, iNum_unbond;                     //인장측 부착 긴장재의 수, 비부착 긴장재의 수
	BOOL   bOneType;                                   //부착조건이 단일조건인지 여부
	BOOL   bBondType;                                  //부착조건(TRUE=부착, FALSE=비부착)
	double dNeuC;                                      //압축연단에서 중립축까지의 거리
	double dH;                                         //단면의 높이  
	double da;                                         //응력블럭의 길이
	double dde;                                        //유효깊이:압축연단에서 인장력의 도심까지의 거리
	double ddv;                                        //유효전단깊이:압축력의 도심에서 인장력의 도심까지의 거리
	int    iCount;                                     //계산에 소요된 반복횟수

	double dMcr;	                                     //균열모멘트
	double dfr ,dfcpe;                                  //콘크리트의 균열강도, 탄성상태(비균열)에서 유효PS력에 의한 콘크리트의 응력
	double dSc;                                         //탄성상태(비균열)에서 합성단면의 단면계수(콘크리트+철근의 단면계수)


	// Caltrans요구로 추가로 출력할 항목(06.12.22 mail)
	double dfck;                                       //콘크리트 강도
	double dfy;                                        //철근의 항복강도
	double dfpy;                                       //긴장재의 항복강도
	double dfpu;                                       //긴장재의 극한강도
	double dB_flange;                                  //Code해석시 플랜지 폭
	double dB_web;                                     //Code해석시 웨브 폭
	double dThick_Top;                                 //Code해석시 상부플랜지 두께
	double dThick_Bottom;                              //Code해석시 하부플랜지 두께  



	
	void Initialize();
	_DBLR_FLEX_DATA()	{}
	_DBLR_FLEX_DATA(_DBLR_FLEX_DATA& rData)	{*this = rData;}
	_DBLR_FLEX_DATA& operator = (_DBLR_FLEX_DATA& rData);
};

struct _DBLR_SHEAR_DATA
{
	ElemPairK ElemKey;
	int      iPosi;
	BOOL     bPositive;

	BOOL   bStrainCompatibility;         //해석방법(TRUE=StrainCompatibility, FALSE=Code)
	BOOL   bSegmentalBoxGirder;          //SegmentalBoxGirderBridge인지 여부
	double dphi;                         //PSC의 전단강도계수 φ
	double dMu, dVu;
	double dVn, dVc, dVs, dVp;           //공칭 전단강도, 콘크리트의 전단강도, 전단철근의 전단강도, 유효PS력의 수직방향 분력
	double dphiVn;                       //설계 전단강도

	// Caltrans요구로 추가로 출력할 항목(06.12.22 mail)
	CString strRatingCase;
	double  dde, ddv;
	double  dAv_s;
	double  dex, dbeta, dtheta;


	void Initialize();
	_DBLR_SHEAR_DATA()	{}
	_DBLR_SHEAR_DATA(_DBLR_SHEAR_DATA& rData)	{*this = rData;}
	_DBLR_SHEAR_DATA& operator = (_DBLR_SHEAR_DATA& rData);
};

class CDgnBridgeLoadRating : public CDgnPscCommon
{
// Member Functions.
public:
	CDgnBridgeLoadRating();
	virtual ~CDgnBridgeLoadRating();
	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);  

	// 단면의 휨강도를 계산하는 함수
	//   ElemK      : 요소키
	//   iCheckPosi : 검사 위치(0:I단,  1:J단,  2:I단+J단) 
	// < arPhi      : φ값 리스트를 넘겨 받을 변수  (0:I단 정모멘트에 대한,  1:I단 부모멘트에 대한,  2:J단 정모멘트에 대한,  3:J단 부모멘트에 대한)     
	// < arMn       : 휨강도 리스트를 넘겨 받을 변수(0:I단 정모멘트에 대한,  1:I단 부모멘트에 대한,  2:J단 정모멘트에 대한,  3:J단 부모멘트에 대한)   
	// Modify, Jaeoh(08.04.25) Data가 추가되어 구조체로 변경 
	BOOL Get_RatingDgnFlexuralStrength(ElemPairK ElemK, int iCheckPosi, BLR_FLEX_DATA& FlexD);
	// 단면의 전단강도를 계산하는 함수
	//   ElemK      : 요소키
	//   iCheckPosi : 검사 위치(0:I단,  1:J단,  2:I단+J단) 
	//   bFlexure   : 전단유효길이 계산시 휨강도 계산시에 사용된 값을 사용할지 여부(FALSE일 경우 dv=0.72h 적용)
	//   bSegBox    : SegmentalBoxGirderBridge인지 여부
	//   blrcK,bDc,bDw,bT,iLine : Rating Case Name을 만들기 위한 변수
	//   dMu        : 계수 모멘트 
	//   dVu        : 계수 전단력
	//   da_g       : 최대 골재 크기
	// < dPhi_I     : I단의 φ값을 넘겨 받을 변수
	// < dPhi_J     : J단의 φ값을 넘겨 받을 변수
	// < arVn       : 전단강도 리스트를 넘겨 받을 변수(0:I단,  1:J단)
	BOOL Get_RatingDgnShearStrength(ElemPairK ElemK, int iCheckPosi, BOOL bFlexure, BOOL bSegBox, T_BLRC_K blrcK, BOOL bDc, BOOL bDw, BOOL bT, int iLine, double dNu, double dMu, double dVu, double dVp_TP, double da_g, double& dPhi_I, double& dPhi_J, CArray<double,double>& arVn);

	
	void    Prt_FlexData(BOOL bPrint, _DBLR_FLEX_DATA FlexData);
	void    Prt_ShearData(BOOL bPrint, _DBLR_SHEAR_DATA ShearData);


protected:
	double m_dZero;  

	// ※주의 : 여기서 계산시 사용되는 단위는 kip, in 단위입니다. /////////////////////////////////////////////////////////////////////

	// Main Function ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 단면의 휨강도를 계산하는 함수
	//   bYaxis    : 해석방향
	//   bIter     : 정밀해석여부(TRUE:정밀해석-실단면적사용 및 철근 긴장재 개별 해석,  FALSE:일반해석-기준상의 공식을 사용하며, 사각형&T형 환산단면적 사용 및 철근 긴장재 통합 해석) 
	//   bPositive : TRUE:정모멘트  FALSE:부모멘트
	//   MatlElem  : 재료 정보
	//   SpscSect  : 철근 및 긴장재를 고려한 환산단면에 대한 정보
	//   SpscgSect : 콘크리트만의 단면정보
	//   arOuter   : 외곽 좌표 리스트
	//   arInner   : 내부 좌표 리스트의 배열
	//   dbw       : 일반해석시에 사용되는 T형으로 환산 시켰을때의 웨브 폭
	//   dFlgThk   : 일반해석시에 사용되는 환산된 플랜지 두께
	//   dpMomPos  : 하면의 좌우 외곽좌표
	//   arRbarPosi: 철근의 정보
	//   arTndnPosi: 긴장재의 정보
	// < FlexData  : 계산된 휨강도 정보를 넘겨 받을 변수
	BOOL Get_FlexureStrength(BOOL bYaxis, BOOL bIter, BOOL bPositive, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, 
													 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, _DBLR_FLEX_DATA& FlexData);

	// 단면의 전단강도를 계산하는 함수
	//   bSegmBoxGirder : SegmentalBoxGirderBridge인지 여부
	//   bFlexure  : 전단유효길이 계산시 휨강도 계산시에 사용된 값을 사용할지 여부(FALSE일 경우 dv=0.72h 적용)
	//   bYaxis    : 해석방향
	//   bIter     : 정밀해석여부(TRUE:정밀해석-실단면적사용 및 철근 긴장재 개별 해석,  FALSE:일반해석-기준상의 공식을 사용하며, 사각형&T형 환산단면적 사용 및 철근 긴장재 통합 해석) 	
	//   dMu       : 계수 모멘트 
	//   dVu       : 계수 전단력
	//   da_g      : 최대 골재 크기
	//   MatlElem  : 재료 정보
	//   SpscSect  : 철근 및 긴장재를 고려한 환산단면에 대한 정보
	//   SpscgSect : 콘크리트만의 단면정보
	//   arOuter   : 외곽 좌표 리스트
	//   arInner   : 내부 좌표 리스트의 배열
	//   dbw       : 일반해석시에 사용되는 T형으로 환산 시켰을때의 웨브 폭
	//   dFlgThk   : 일반해석시에 사용되는 환산된 플랜지 두께
	//   dpMomPos  : 하면의 좌우 외곽좌표
	//   arRbarPosi: 철근의 정보
	//   arTndnPosi: 긴장재의 정보
	//   RpscSbar  : 전단철근 정보
	// < ShearData : 계산된 휨강도 정보를 넘겨 받을 변수
	BOOL Get_ShearDesign(BOOL bSegmBoxGirder, BOOL bFlexure, BOOL bYaxis, BOOL bIter, double dMu, double dVu, double dVp_TP, double da_g, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, 
											 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, _DBLR_SHEAR_DATA& ShearData);  

	// SegmentalBoxGirderBridge 이외의 단면의 전단강도를 계산하는 함수
	//   bIterBT   : β, θ값 계산시에 반복계산을 통해서 산출할지 정밀하게 산출할지 여부여부
	//   ETC       : Get_ShearDesign변수 내용과 동일
	BOOL Get_ShearDesign_WithoutSegmentalBoxGirderBridge(BOOL bFlexure, BOOL bYaxis, BOOL bIter, BOOL bIterBT, double dMu, double dVu, double dVp_TP, double da_g, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, 
											 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, _DBLR_SHEAR_DATA& ShearData);
	
	// SegmentalBoxGirderBridge단면의 전단강도를 계산하는 함수
	BOOL Get_ShearDesign_WithSegmentalBoxGirderBridge(BOOL bFlexure, BOOL bYaxis, BOOL bIter, double dMu, double dVu, double dVp_TP, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, 
											 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, _DBLR_SHEAR_DATA& ShearData);

	// Sub Function ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BOOL Is_CaltransCode() {return TRUE;}

	// PSC의 강도계수 φ값을 넘겨줌(AASHTO)
	//   dfy       : 철근의 항복강도
	//   arRbarPosi: 철근의 정보
	//   arTndnPosi: 긴장재의 정보  
	// < dPhib     : 휨에 대한 강도계수  
	// < dPhiv     : 전단에 대한 강도계수
	void Get_Phi_PrestressedConcrete_AASHTO(double dfy, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dPhib, double& dPhiv);
	// PSC의 휨강도계수 φ값을 넘겨줌(Caltlans 2006 개정판)
	//   dEpsi_pt  : 긴장재의 변형률
	// < dPhib     : 휨에 대한 강도계수  
	void Get_Phi_PrestressedConcrete_Flexure(double dEpsi_pt, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dPhib);
	// PSC의 전단강도계수 φ값을 넘겨줌
	// < dPhiv     : 전단에 대한 강도계수
	void Get_Phi_PrestressedConcrete_Shear(double& dPhiv);
	
	// 등가응력 블럭 계산시의 β1값을 계산하여 넘겨줌
	//   dfck   : 콘크리트의 압축강도
	// < RETURN : β1
	double Get_Beta1(double dfck);
	// 콘크리트의 극한 변형률을 넘겨줌
	double Get_MaxStrain_Concrete() {return 0.003;}
	// 긴장재의 Sterss-Strain선도에서 응력을 산출하여 넘겨줌
	double Get_StressOfTendon(double df_py, double df_pu, double dEp, double dEpsi_ps);
	// 유효 긴장재 길이 
	double Get_EffectiveTendonLength(_DGN_TNDN_CRC& TendonData);
	// 최소 횡방향철근량 이상일때의 전단강도를 구하기 위한 θ와 β를 구하는 함수
	//   dvu        : 콘크리트의 전단응력
	//   dfck       : 콘크리트의 압축강도
	//   dEpsilon_x : 0.5h 위치에서의 변형률
	// < dTheta     : θ값을 넘겨 받을 변수(단위:Radian)
	// < dBeta      : β값을 넘겨 받을 변수
	// < RETURN     : 계산성공여부 (FALSE 원인: dfck <= 0, εx > 0.001)
	// ※주의 : vu/fck > 0.25 시에는 vu/fck = 0.25로 하여 계산
	BOOL Get_ThetaAndBeta_TranverseReinforcement(double dvu, double dfck, double dEpsilon_x, double& dTheta, double& dBeta);  
	// 최소 횡방향철근량 미만일때의 전단강도를 구하기 위한 θ와 β를 구하는 함수
	//   dSxe       : 균열폭관련변수(AASHTO Figure 5.8.3.4.2-3 참조)
	//   dEpsilon_x : 0.5h 위치에서의 변형률
	// < dTheta     : θ값을 넘겨 받을 변수(단위:Radian)
	// < dBeta      : β값을 넘겨 받을 변수
	// < RETURN     : 계산성공여부 (FALSE 원인:εx > 0.002)
	// ※주의 : Sxe > 80 시에는 Sxe = 80으로 하여 계산
	BOOL Get_ThetaAndBeta_LessThanMinimumTranverseReinforcement(double dSxe, double dEpsilon_x, double& dTheta, double& dBeta);  
	// 입력단면을 해당축방향으로으로 임의 위치의 직선에 의해 절단된 단면의 면적과 직선에서의 거리를 계산하여 넘겨주는 함수  
	//   arOuter  : 외곽 Line정보
	//   arInner  : 내부 Line정보의 배열
	//   bDir     : 계산할 단면의 방향(TRUE:정방향(상or우)  FALSE:역방향(하or좌))
	//   baxis    : 축의 방향 (TRUE:Y축, FALSE:Z축) 
	//   dpos     : 임의 직선까지의 거리(좌측하단(원점)에서의 해당축의 직각거리)
	//   dlen     : 임의 직선의 길이 (단면의 해당방향의 최외곽 까지의 길이가 필요)
	// < dCutArea : 잘려진 단면의 면적을 넘겨받을 변수
	// < dQ       : 면적 도심이 임의직선까지 떨어진 거리를 넘겨 받을 변수
	BOOL CalcSectPSC_QA(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, double& dCutArea, double& dQ);


	//++++++++++++++++++++++++++
	// For test_DBLR.txt
	ElemPairK m_ElemKey_Flex;        // 휨 해석을 수행하는 요소키
	ElemPairK m_ElemKey_Shear;       // 전단 해석 수행하는 요소키
	BOOL    m_bTestDBLR;            // Debug시 출력파일에 기입여부
	BOOL    m_bTestDBLR_Release;    // Release시 출력파일에 기입여부
	CString m_strTestDBLRFile;
	
	CString Get_strPositionData(int iPosi, BOOL bPositive);


	//++++++++++++++++++++++++++
	// Coded by Seungjun for Test ('20060719).
	BOOL m_bTestMode;

public:
	BOOL    m_bNewElemNum_Flex;     // 해당요소를 첫번째 단위해석하는지 여부  
	BOOL    m_bNewElemNum_Shear;    // 해당요소를 첫번째 단위해석하는지 여부

	void    Prt_Setting(BOOL bPrint);
	void    Prt_ResultData(BOOL bPrint, CString strTxt); // (Release || Debug)시 출력
	void    Prt_Debug(BOOL bPrint, CString strTxt);      // Debug시 Resistery조건이 맞을 경우에만 출력
	void    Prt_ElementNumber(BOOL bPrint, ElemPairK ElemK);
	void    Prt_PositionData(BOOL bPrint, int iPosi, BOOL bPositive);

	CString GetRatingCaseString(T_BLRC_K blrcK, BOOL bDc, BOOL bDw, BOOL bT, int iLine);
	CString GetStringReSize(int nSize, CString strText);
	
};

#endif // !defined(AFX_DGNBRIDGELOADRATING_H__INCLUDED_)
