#ifndef __DB_ST_POST_DB_H__
#define __DB_ST_POST_DB_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

// PostCtrl에서만 사용되는 Data 들만 ...

struct T_POSTCTRL_DATACOUNT
{
	// 절점, 요소
	unsigned int m_nNumNode, m_nNumNodeBase, m_nNumElem, m_nNumElemBase;
	unsigned int m_nNumStrt, m_nNumStrtBase, m_nNumStrb, m_nNumStrbBase;
	unsigned int m_nNumVbem, m_nNumVbemBase;
	unsigned int m_nNumSecv, m_nNumSecvBase;
	unsigned int m_nNumSimb, m_nNumSimbBase;
	unsigned int m_nNumDspb, m_nNumDspbBase;  // Design Strip Beam
	unsigned int m_nNumDsps, m_nNumDspsBase;  // Design Strip Section
	unsigned int m_nNumStrm, m_nNumStrmBase, m_nNumStrp, m_nNumStrpBase;
	unsigned int m_nNumStrs, m_nNumStrsBase, m_nNumStrw, m_nNumStrwBase;
	unsigned int m_nNumStpn, m_nNumStpnBase, m_nNumStax, m_nNumStaxBase;
	unsigned int m_nNumSkew, m_nNumSkewBase;  // Local Coordinate System
	unsigned int m_nNumElnk, m_nNumElnkBase;  // Elastic Link
	unsigned int m_nNumNlnk, m_nNumNlnkBase;  // General Link
	unsigned int m_nNumCabl, m_nNumCablBase;  // Cable Element
	unsigned int m_nNumSsps, m_nNumSspsBase;  // Elastic Link
	unsigned int m_nNumStrbNew, m_nNumStrbNewBase;	// 6th DOF (v815 : m_nNumStrb, m_nNumStrbBase)
	unsigned int m_nNumStbw, m_nNumStbwBase;				// 7th DOF
	unsigned int m_nNumVmst, m_nNumVmstBase;				// Effective
	unsigned int m_nNumEprp; // Plate요소 중 할당된 Material이 Plastic Material을 할당하고 있는 경우
	// Static 
	unsigned int m_nNumStld;
	// Time History
	unsigned int m_nNumThis, m_nNumThis_Modal, m_nNumThis_Direct;
	unsigned int m_nNumThis_NonlDirect, m_nNumThis_NonlStatic, m_nNumThis_Init;
	unsigned int m_nNumThis_NonlLargeDisp;
	unsigned int m_nNumThfc;
	unsigned int m_nNumMaxTimeStep, m_nNumMaxTimeStepNotOut;
	unsigned int m_nNumMaxTimeStep_Modal, m_nNumMaxTimeStep_Direct;
	unsigned int m_nNumEquation_Modal, m_nNumEquation_Direct;
	unsigned int m_nNumNlbm;  // Inelastic Dynamic Beam
	unsigned int m_nNumNlwl;  // Inelastic Dynamic Wall
	unsigned int m_nNumNltr;  // Inelastic Dynamic Truss
	unsigned int m_nNumNllk;  // Inelastic Dynamic General Link
	unsigned int m_nNumNspr;  // Inelastic Dynamic Point Support Spring
	unsigned int m_nNumNlhg;  // Inelastic Dynamic Hinge
	unsigned int m_nNumNlhg_SelectOutputOnly;  // Inelastic Dynamic Hinge
	unsigned int m_nNumFimp       ;  // Fiber Inelastic Material Properties
	unsigned int m_nNumFibr       ;  // Fiber Model 요소 개수
	unsigned int m_nNumFibrCell   ;  // Fiber Model 요소의 전체 셀 개수
	unsigned int m_nNumFibrSect   ;  // Fiber Model 요소의 Section Position 수
	unsigned int m_nNumFibw       ;  // Fiber Model 요소 개수(wall)
	unsigned int m_nNumFibwCell   ;  // Fiber Model 요소의 전체 셀 개수(wall)
	unsigned int m_nNumFibwSect   ;  // Fiber Model 요소의 Section Position 수(wall)
	unsigned int m_nNumFibr_PO    ;  // Fiber Model 요소 개수
	unsigned int m_nNumFibrCell_PO;
	unsigned int m_nNumFibrSect_PO;
	unsigned int m_nNumFibw_PO    ;  // Fiber Model 요소 개수(wall)
	unsigned int m_nNumFibwCell_PO;
	unsigned int m_nNumFibwSect_PO;
	unsigned int m_nNumofSection; // 전체 요소에 할당된 Fiber 의 Number of Section 의 합.
	unsigned int m_nNumFibrCell_SelectOutputOnly; // Siho 고려한 Fiber Model 요소의 전체 셀 개수
	unsigned int m_nNumFibwCell_SelectOutputOnly; // Siho 고려한 Fiber Model 요소의 전체 셀 개수(wall)
	unsigned int m_nNumGlbm;  // Large Displacements 고려시 사용 : Inelastic Hinge 할당되지 않은 Beam
	unsigned int m_nNumGlbw;  // Large Displacements 고려시 사용 : Inelastic Hinge 할당되지 않은 Beam (7자유도 Beam)
	unsigned int m_nNumGltr;  // Large Displacements 고려시 사용 : Inelastic Hinge 할당되지 않은 Truss
	unsigned int m_nNumGlps;  // Large Displacements 고려시 사용 : Plane Stress
	unsigned int m_nNumGlnp;  // Large Displacements 고려시 사용 : Plate
	unsigned int m_nNumGlsd;  // Large Displacements 고려시 사용 : Solid
	unsigned int m_nNumGlSdvi;
	unsigned int m_nNumGlSdve;
	unsigned int m_nNumGlSdst;
	unsigned int m_nNumGlSdhy;
	unsigned int m_nNumGlSdis;
	unsigned int m_nNumGlMssp;
	unsigned int m_nNumGlSdgb;
	unsigned int m_nNumMlfc;
	unsigned int m_nNumRlfc;

	CArray<int, int>    m_aNumTimeStep;
	CArray<BOOL, BOOL>  m_aNonlDirectLoadCase;
	CArray<BOOL, BOOL>  m_aNonlStaticLoadCase;
	CArray<BOOL, BOOL>  m_aNonlInitLoadCase;
	CArray<BOOL, BOOL>  m_aNonlLargeDisp;
	CArray<BOOL, BOOL>  m_aNonlPDelta;
	CArray<BOOL, BOOL>  m_aSTWLLoadCase;
	// Time History Extended Result, Animation Result
	unsigned int    m_nNumThex;
	unsigned int    m_nNumTham;
	unsigned int    m_nNumThpo;
	// Design
	unsigned int m_nNumLcomGeneral, m_nNumLcomSteel, m_nNumLcomColdForm;
	unsigned int m_nNumLcomConcrete, m_nNumLcomFooting, m_nNumLcomSRC, m_nNumLcomAluminum, m_nNumLcomStlComp; 
	unsigned int m_nNumSteel, m_nNumColdForm;
	unsigned int m_nNumConcrete_Column, m_nNumConcrete_Beam;
	unsigned int m_nNumConcrete;//Drawing Shop용 설계결과저장(사용철근량) 060123 sshan
	unsigned int m_nNumConcrete_Brace, m_nNumConcrete_Wall, m_nNumSRC_Column, m_nNumSRC_Beam;
	unsigned int m_nNumConcreteSect_Beam, m_nNumConcreteSect_Column, m_nNumConcreteSect_Brace, m_nNumConcreteSect_Wall;
	unsigned int m_nNumConcreteSect;//Drawing Shop용 설계결과저장(사용철근량) 060123 sshan
	unsigned int m_nNumSteelSect, m_nNumColdFormSect, m_nNumSRCSect_Beam, m_nNumSRCSect_Column;
	unsigned int m_nNumSteelJoint, m_nNumSteelBasePL, m_nNumColdFormHS;
	unsigned int m_nNumLateralConfinement, m_nNumRLCF_LenList;
	// Add/Modify by Seungjun '20120724 for iGen JFD
	unsigned int m_nNumJFDIsolate, m_nNumJFDPile, m_nNumJFDLine, m_nNumJFDMat;


		// For KSCE-ASD05
	unsigned int m_nNumSteelForKsceAsd05;
	unsigned int m_nNumLcomSteelForKsceAsd05;
	// 편심하중
	unsigned int m_nNumRaem, m_nNumEccn;
	unsigned int m_nNumNspc, m_nNumNesp;
	// Story
	int          m_nNumStor, m_bStoryCenter, m_bStoryShearRatio, m_nNumStor_JUD;
	int          m_nNumStor_dummy;    //  다탑의 경계층 (추가정보). // 공간 개념 층 표현을 위해 경계층의 결과를 추가적으로 받기 위한 용도
	// Eigenvalue
	unsigned int m_nNumEigv;
	// Buckling
	unsigned int m_nNumBuck;
	// 시공단계
	unsigned int m_nNumSgld, m_nNumSgldRead, m_nNumSgldReadVmst, m_bCalcConcurrentForce;
	unsigned int m_nStageStep, m_nNumStage;	
	// crane 하중
	unsigned int m_nNumCran, m_nNumCranTot; // m_nNumCranTot=0,1,2 (0:X,1:Crane,2:Crane,Crane+Brk)
	// 합성단면
	unsigned int m_nNumSectPart;
	// Response Spectrum
	unsigned int m_nNumSplc;
	// Pushover
	unsigned int m_nNumPold, m_nPushOverStep, m_nNumHnge;  // Before Used(~v721)
	unsigned int m_nNumPolc, m_nNumPhge, m_nNumPhgeElem, m_nNumPhgeNlnk, m_nNumPhgeNode;  // m_nPushOverStep는 Load Case마다 다름.
	unsigned int m_nNumPhgeSprn; // Point Spring Hinge가 설정된 Spring 총수
	unsigned int m_nNumPNbm;  // Pushover Inelastic Dynamic Beam
	unsigned int m_nNumPNtr;  // Pushover Inelastic Dynamic Truss
	unsigned int m_nNumPNwa;  // Pushover Inelastic Dynamic Wall
	unsigned int m_nNumPNlk;  // Pushover Inelastic Dynamic General Link
	unsigned int m_nNumPNhg;  // Pushover Inelastic Dynamic Hinge	
	unsigned int m_nNumPNps;  // Pushover Inelastic Dynamic Point Spring
	unsigned int m_nNumFbcx;  // Pushover fiber beam cell strain (min/max)
	unsigned int m_nNumFbsx;  // Pushover fiber beam section strain (min/max)
	unsigned int m_nNumFwcx;  // Pushover fiber wall cell strain (min/max)
	unsigned int m_nNumFwsx;  // Pushover fiber wall section strain (min/max)
	unsigned int m_nNumPhif;
	// 수화열
	unsigned int m_nNumHylc;
	// 지점침하
	unsigned int m_nNumSmlc, m_nNumSmpt;
	// 이동하중
	unsigned int m_nNumInfl; // 영향선, 영향면 하중조건 개수
	unsigned int m_nNumLlan, m_nNumSlan, m_nNumSinf, m_nNumLlanop, m_nNumSlanop; // 
	unsigned int m_nNumMvld; // 이동하중 하중조건 개수
	unsigned int m_nMvCode;  // 이동하중 기준 코드
	unsigned int m_nNumInflDisp, m_nNumInflTrss, m_nNumInflBeam, m_nNumInflPlat, m_nNumInflSold; // 이동하중 결과 출력 개수
	unsigned int m_nNumNegative; // 부모멘트 구간찾기

	unsigned int m_nNumCrgr; //동시 발생 반력 그룹수
	unsigned int m_nNumCjfg; //Concurrent Joint Force Group수

	// 격자해석
	unsigned int m_nNumInflGrid;  // 영향선, 영향면 하중 개수(격자영향점 하중 개수)
	unsigned int m_nNumGmld;      // 격자해석 하중조건 개수
	unsigned int m_nNumGmldLive;  // 격자해석 활하중 하중조건 개수
	unsigned int m_nNumCref;      // Cross Reference 개수
	CArray<int, int> m_aCrefCount; // Cross Reference의 Entity 개수
	CArray<int, int> m_aCrefCountTotal; // Cross Reference의 Entity 개수 Total
	CArray<int, int> m_aCrefCalcItem;   // Cross Reference의 Entity 타입
	CArray<BOOL, BOOL> m_aGmldLive; // Live면 TRUE
	// P-Delta
	unsigned int m_nNumPdel;
	// Erection
	unsigned int m_nNumEseq;
	// Nonlinear
	unsigned int m_nNumNonl, m_nNonlType;
	unsigned int m_nNonLinearStep;
	// Composite Section
	unsigned int m_nNumPlcb;
	// Tendon
	unsigned int m_nNumTdna;
	int m_nNumUsedTendon;    // 해석에 사용된 실텐던수
	int m_nNumMaxTendonElem; // 텐던에 포함된 요소수중 가장큰수
	int m_nNumMaxPrestressed;   // 긴장된 회수가 가장많은 텐던의 긴장된 회수
	// PTBeam, PTSlab Tendon
	unsigned int m_nNumPTBeamTendon;
	unsigned int m_nNumPTSlabTendon;
	int m_nNumMaxPTSlabTendonElem; // 텐던에 포함된 요소수중 가장큰수
	int m_nMax_Vsect_Tend;         // 단면내부에 긴장된 텐던의 최대 개수

	// PCDesign
	unsigned int m_nNumPcdn;
	// [30/10/2006 : maxiao] For CIVIL(CH) 2006                                        
	unsigned int m_nNumCHRcdn;

	// Bridge Load Rating
	unsigned int m_nNumBlrg;
	unsigned int m_nNumBlrElem;
	unsigned int m_nNumStreRatingCase;
	unsigned int m_nNumServRatingCase;
	unsigned int m_nNumStlBlrElem;
	unsigned int m_nNumStlStreRatingCase;
	unsigned int m_nNumStlServRatingCase;
	unsigned int m_nNumStlFatiRatingCase;
	unsigned int m_nNumStlStreRCase;
	unsigned int m_nNumStlServRCase;
	unsigned int m_nNumStlFatiRCase;
	unsigned int m_nNumRCBlrElem;
	unsigned int m_nNumRCBlrMemb;

	// Plate Girder Design.
	unsigned int m_nNumPgdElem;
	unsigned int m_nNumPgdStreLcase;
	unsigned int m_nNumPgdServLcase;
	unsigned int m_nNumPgdFatiLcase;
	unsigned int m_nNumSpan;
	unsigned int m_nNumSpanElem;
	unsigned int m_nNumSpanVBeam;

	// Steel Orthotropic Deck 
	unsigned int m_nNumSodElem;
	unsigned int m_nNumSodStrnLcase;
	unsigned int m_nNumSodFatiLcase;

	// RUS Steel Composite Girder Design 
	unsigned int m_nNumStlCompGirderElem;
	unsigned int m_nNumStlCompGirderStrnLcase;
	unsigned int m_nNumStlCompGirderFatiLcase;
	unsigned int m_nNumStlCompGirderServLcase;

	// RC Plate Beam/Column Design
	unsigned int m_nNumSbdoBeam;
	unsigned int m_nNumSbdoWall;
	unsigned int m_nNumPRCLcomStrn;
	unsigned int m_nNumPRCLcomServ;
	
	// 사인장,전단응력 계산에 사용되는 보요소개수
	unsigned int m_nNumStssBeam;		// 6자유도
	unsigned int m_nNumStssBeam7th;	// 7자유도

	// Suspension Bridge
	unsigned int m_nSuspensionType;

	// Nodal Result for RS
	CArray<int,int> m_arUsedModeNumPerSplc; // Serial순서의 각 SPLC에 저장되는 모드수 
	unsigned int m_nNumCrack;
	
	// Camber
	unsigned int m_nNumCmcs;	 // 캠버가 입력된 노드의 수

	// Sub Domain
	unsigned int m_nNumSbdo;

	T_POSTCTRL_DATACOUNT()
	{
	}
	T_POSTCTRL_DATACOUNT(T_POSTCTRL_DATACOUNT& src)
	{
		*this = src;
	}
	T_POSTCTRL_DATACOUNT& operator=(T_POSTCTRL_DATACOUNT& src)
	{
		//
		CopyCommonData(&src);
		// Static 
		CopyStaticAnalysisData(&src);
		// Dynamic
		CopyDynamicAnalysisData(&src);
		// Time History
		CopyTimeHistoryAnalysisData(&src);
		// 시공단계, 수화열
		CopyStageAnalysisData(&src);
		// Pushover
		CopyPushoverAnalysisData(&src);
		// Moving, Grid Analysis
		CopyMovingLoadAnalysisData(&src);
		// Settlement
		CopySettlementAnalysisData(&src);
		//
		CopyETCAnalysisData(&src);
		return *this;
	}
	void Initialize()
	{
		//
		ClearCommonData(this);
		// Static 
		ClearStaticAnalysisData(this);
		// Dynamic
		ClearDynamicAnalysisData(this);
		// Time History
		ClearTimeHistoryAnalysisData(this);
		// 시공단계, 수화열
		ClearStageAnalysisData(this);
		// Pushover
		ClearPushoverAnalysisData(this);
		// Moving, Grid Analysis
		ClearMovingLoadAnalysisData(this);
		// Settlement
		ClearSettlementAnalysisData(this);
		//
		ClearETCAnalysisData(this);
	}

	void CopyCommonData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// 절점, 요소
		m_nNumNode = psrc->m_nNumNode, m_nNumNodeBase = psrc->m_nNumNodeBase, m_nNumElem = psrc->m_nNumElem, m_nNumElemBase = psrc->m_nNumElemBase;
		m_nNumStrt = psrc->m_nNumStrt, m_nNumStrtBase = psrc->m_nNumStrtBase, m_nNumStrb = psrc->m_nNumStrb, m_nNumStrbBase = psrc->m_nNumStrbBase;
		m_nNumVbem = psrc->m_nNumVbem, m_nNumVbemBase = psrc->m_nNumVbemBase;
		m_nNumSecv = psrc->m_nNumSecv, m_nNumSecvBase = psrc->m_nNumSecvBase;
		m_nNumSimb = psrc->m_nNumSimb, m_nNumSimbBase = psrc->m_nNumSimbBase;
		m_nNumDspb = psrc->m_nNumDspb, m_nNumDspbBase = psrc->m_nNumDspbBase;
		m_nNumDsps = psrc->m_nNumDsps, m_nNumDspsBase = psrc->m_nNumDspsBase;
		m_nNumStrm = psrc->m_nNumStrm, m_nNumStrmBase = psrc->m_nNumStrmBase, m_nNumStrp = psrc->m_nNumStrp, m_nNumStrpBase = psrc->m_nNumStrpBase;
		m_nNumStrs = psrc->m_nNumStrs, m_nNumStrsBase = psrc->m_nNumStrsBase, m_nNumStrw = psrc->m_nNumStrw, m_nNumStrwBase = psrc->m_nNumStrwBase;
		m_nNumStpn = psrc->m_nNumStpn, m_nNumStpnBase = psrc->m_nNumStpnBase, m_nNumStax = psrc->m_nNumStax, m_nNumStaxBase = psrc->m_nNumStaxBase;
		m_nNumSkew = psrc->m_nNumSkew, m_nNumSkewBase = psrc->m_nNumSkewBase;
		m_nNumElnk = psrc->m_nNumElnk, m_nNumElnkBase = psrc->m_nNumElnkBase;
		m_nNumNlnk = psrc->m_nNumNlnk, m_nNumNlnkBase = psrc->m_nNumNlnkBase;
		m_nNumSsps = psrc->m_nNumSsps, m_nNumSspsBase = psrc->m_nNumSspsBase;
		m_nNumCabl = psrc->m_nNumCabl, m_nNumCablBase = psrc->m_nNumCablBase;
		m_nNumStrbNew = psrc->m_nNumStrbNew, m_nNumStrbNewBase = psrc->m_nNumStrbNewBase;
		m_nNumStbw = psrc->m_nNumStbw, m_nNumStbwBase = psrc->m_nNumStbwBase;
		m_nNumMlfc= psrc->m_nNumMlfc;
		m_nNumRlfc= psrc->m_nNumRlfc;

		// Design
		m_nNumLcomGeneral = psrc->m_nNumLcomGeneral, m_nNumLcomSteel = psrc->m_nNumLcomSteel, m_nNumLcomColdForm = psrc->m_nNumLcomColdForm;
		m_nNumLcomConcrete = psrc->m_nNumLcomConcrete, m_nNumLcomFooting = psrc->m_nNumLcomFooting, m_nNumLcomSRC = psrc->m_nNumLcomSRC, m_nNumLcomAluminum = psrc->m_nNumLcomAluminum, m_nNumLcomStlComp = psrc->m_nNumLcomStlComp;
		m_nNumSteel = psrc->m_nNumSteel, m_nNumColdForm = psrc->m_nNumColdForm;
		m_nNumConcrete_Column = psrc->m_nNumConcrete_Column, m_nNumConcrete_Beam = psrc->m_nNumConcrete_Beam;
		m_nNumConcrete = psrc->m_nNumConcrete;
		m_nNumConcrete_Brace = psrc->m_nNumConcrete_Brace, m_nNumConcrete_Wall = psrc->m_nNumConcrete_Wall;
		m_nNumSRC_Column = psrc->m_nNumSRC_Column, m_nNumSRC_Beam = psrc->m_nNumSRC_Beam;
		m_nNumConcreteSect_Beam = psrc->m_nNumConcreteSect_Beam, m_nNumConcreteSect_Column = psrc->m_nNumConcreteSect_Column;
		m_nNumConcreteSect_Brace = psrc->m_nNumConcreteSect_Brace, m_nNumConcreteSect_Wall = psrc->m_nNumConcreteSect_Wall;
		m_nNumConcreteSect = psrc->m_nNumConcreteSect;    
		m_nNumSteelSect = psrc->m_nNumSteelSect;
		m_nNumColdFormSect = psrc->m_nNumColdFormSect;
		m_nNumSRCSect_Beam = psrc->m_nNumSRCSect_Beam;
		m_nNumSRCSect_Column = psrc->m_nNumSRCSect_Column;
		m_nNumSteelJoint = psrc->m_nNumSteelJoint;
		m_nNumColdFormHS = psrc->m_nNumColdFormHS;
		m_nNumSteelBasePL = psrc->m_nNumSteelBasePL;
		m_nNumLateralConfinement = psrc->m_nNumLateralConfinement;
		m_nNumRLCF_LenList = psrc->m_nNumRLCF_LenList;
			// For KSCE-ASD05
		m_nNumSteelForKsceAsd05 = psrc->m_nNumSteelForKsceAsd05;
		m_nNumLcomSteelForKsceAsd05 = psrc->m_nNumLcomSteelForKsceAsd05;
		// PCDesign
		m_nNumPcdn = psrc->m_nNumPcdn;
		// [30/10/2006 : maxiao] For CIVIL(CH) 2006                                        
		m_nNumCHRcdn = psrc->m_nNumCHRcdn;

		m_nNumMlfc= psrc->m_nNumMlfc;
		m_nNumRlfc= psrc->m_nNumRlfc;

		// Bridge Load Rating
		m_nNumBlrg = psrc->m_nNumBlrg;
		m_nNumBlrElem = psrc->m_nNumBlrElem;
		m_nNumStreRatingCase = psrc->m_nNumStreRatingCase;
		m_nNumServRatingCase = psrc->m_nNumServRatingCase;
		m_nNumStlBlrElem        = psrc->m_nNumStlBlrElem;
		m_nNumStlStreRatingCase = psrc->m_nNumStlStreRatingCase;
		m_nNumStlServRatingCase = psrc->m_nNumStlServRatingCase;
		m_nNumStlFatiRatingCase = psrc->m_nNumStlFatiRatingCase;
		m_nNumStlStreRCase      = psrc->m_nNumStlStreRCase;
		m_nNumStlServRCase      = psrc->m_nNumStlServRCase;
		m_nNumStlFatiRCase      = psrc->m_nNumStlFatiRCase;
		m_nNumRCBlrElem         = psrc->m_nNumRCBlrElem;
		m_nNumRCBlrMemb         = psrc->m_nNumRCBlrMemb;
		// Plate Girder Design.
		m_nNumPgdElem = psrc->m_nNumPgdElem;
		m_nNumPgdStreLcase = psrc->m_nNumPgdStreLcase;
		m_nNumPgdServLcase = psrc->m_nNumPgdServLcase;
		m_nNumPgdFatiLcase = psrc->m_nNumPgdFatiLcase;
		m_nNumSpan      = psrc->m_nNumSpan;
		m_nNumSpanElem  = psrc->m_nNumSpanElem;
		m_nNumSpanVBeam = psrc->m_nNumSpanVBeam;
		m_nNumSodElem      = psrc->m_nNumSodElem;
		m_nNumSodStrnLcase = psrc->m_nNumSodStrnLcase;
		m_nNumSodFatiLcase = psrc->m_nNumSodFatiLcase;

		m_nNumStlCompGirderElem      = psrc->m_nNumStlCompGirderElem     ;
		m_nNumStlCompGirderStrnLcase = psrc->m_nNumStlCompGirderStrnLcase;
		m_nNumStlCompGirderFatiLcase = psrc->m_nNumStlCompGirderFatiLcase;
		m_nNumStlCompGirderServLcase = psrc->m_nNumStlCompGirderServLcase;

		// RC Plate Beam/Column Design
		m_nNumSbdoBeam    = psrc->m_nNumSbdoBeam;
		m_nNumSbdoWall    = psrc->m_nNumSbdoWall;
		m_nNumPRCLcomStrn = psrc->m_nNumPRCLcomStrn;
		m_nNumPRCLcomServ = psrc->m_nNumPRCLcomServ;
		
		// Story
		m_nNumStor = psrc->m_nNumStor, m_bStoryCenter = psrc->m_bStoryCenter, m_bStoryShearRatio = psrc->m_bStoryShearRatio;
		m_nNumStor_dummy = psrc->m_nNumStor_dummy;
		m_nNumStor_JUD= psrc->m_nNumStor_JUD;

		// Tendon
		m_nNumTdna = psrc->m_nNumTdna;
		m_nNumUsedTendon = psrc->m_nNumUsedTendon;
		m_nNumMaxTendonElem = psrc->m_nNumMaxTendonElem;
		m_nNumMaxPrestressed = psrc->m_nNumMaxPrestressed;
		//PTBeam, PTSlab Tendon
		m_nNumPTBeamTendon = psrc->m_nNumPTBeamTendon;
		m_nNumPTSlabTendon = psrc->m_nNumPTSlabTendon;

		// 사인장,전단응력 계산에 사용되는 보요소개수
		m_nNumStssBeam = psrc->m_nNumStssBeam;
		m_nNumStssBeam7th = psrc->m_nNumStssBeam7th;
		// Erection
		m_nNumEseq = psrc->m_nNumEseq;  // 사용안함->시공단계기능으로로 대체
		// Suspension Bridge
		m_nSuspensionType = psrc->m_nSuspensionType;
		// 지점침하
		m_nNumSmpt = psrc->m_nNumSmpt;  // --> 데이터로 본다

		//동시 발생 반력 그룹수
		m_nNumCrgr = psrc->m_nNumCrgr;
		// Camber
		m_nNumCmcs = psrc->m_nNumCmcs;
		// Sub Domain
		m_nNumSbdo = psrc->m_nNumSbdo;
	}
	void ClearCommonData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// 절점, 요소
		psrc->m_nNumNode = psrc->m_nNumNodeBase = 0;
		psrc->m_nNumElem = psrc->m_nNumElemBase = 0;
		psrc->m_nNumStrt = psrc->m_nNumStrtBase = 0;
		psrc->m_nNumStrb = psrc->m_nNumStrbBase = 0;
		psrc->m_nNumVbem = psrc->m_nNumVbemBase = 0;
		psrc->m_nNumSecv = psrc->m_nNumSecvBase = 0;
		psrc->m_nNumSimb = psrc->m_nNumSimbBase = 0;
		psrc->m_nNumDspb = psrc->m_nNumDspbBase = 0;
		psrc->m_nNumDsps = psrc->m_nNumDspsBase = 0;
		psrc->m_nNumStrm = psrc->m_nNumStrmBase = 0;
		psrc->m_nNumStrp = psrc->m_nNumStrpBase = 0;
		psrc->m_nNumStrs = psrc->m_nNumStrsBase = 0;
		psrc->m_nNumStrw = psrc->m_nNumStrwBase = 0;
		psrc->m_nNumStpn = psrc->m_nNumStpnBase = 0;
		psrc->m_nNumStax = psrc->m_nNumStaxBase = 0;
		psrc->m_nNumSkew = psrc->m_nNumSkewBase = 0;
		psrc->m_nNumElnk = psrc->m_nNumElnkBase = 0;
		psrc->m_nNumNlnk = psrc->m_nNumNlnkBase = 0;
		psrc->m_nNumSsps = psrc->m_nNumSspsBase = 0;
		psrc->m_nNumCabl = psrc->m_nNumCablBase = 0;
		psrc->m_nNumStrbNew = psrc->m_nNumStrbNewBase = 0;
		psrc->m_nNumStbw = psrc->m_nNumStbwBase = 0;
		psrc->m_nNumMlfc = 0;
		psrc->m_nNumRlfc = 0;

		// Design
		psrc->m_nNumLcomGeneral = psrc->m_nNumLcomSteel = psrc->m_nNumLcomColdForm = 0;
		psrc->m_nNumLcomConcrete = psrc->m_nNumLcomFooting = psrc->m_nNumLcomSRC = psrc->m_nNumLcomAluminum = psrc->m_nNumLcomStlComp = 0;
		psrc->m_nNumSteel = psrc->m_nNumColdForm = psrc->m_nNumConcrete_Column = psrc->m_nNumConcrete_Beam = 0;
		psrc->m_nNumConcrete = 0;
		psrc->m_nNumConcrete_Brace = psrc->m_nNumConcrete_Wall = 0;
		psrc->m_nNumSRC_Column = psrc->m_nNumSRC_Beam = 0;
		psrc->m_nNumConcreteSect_Beam = psrc->m_nNumConcreteSect_Column = 0;
		psrc->m_nNumConcreteSect_Brace = psrc->m_nNumConcreteSect_Wall = 0;
		psrc->m_nNumConcreteSect = 0;
		psrc->m_nNumSteelSect = 0;
		psrc->m_nNumColdFormSect = 0;
		psrc->m_nNumSRCSect_Beam = 0;
		psrc->m_nNumSRCSect_Column = 0;
		psrc->m_nNumSteelJoint = 0;
		psrc->m_nNumColdFormHS = 0;
		psrc->m_nNumSteelBasePL = 0;
		psrc->m_nNumLateralConfinement = 0;
		psrc->m_nNumRLCF_LenList = 0;

		// PCDesign
		psrc->m_nNumPcdn = 0;
		// [30/10/2006 : maxiao] For CIVIL(CH) 2006                                        
		psrc->m_nNumCHRcdn = 0;
	 
		// Bridge Load Rating
		psrc->m_nNumBlrg = 0;
		psrc->m_nNumBlrElem = 0;
		psrc->m_nNumStreRatingCase = 0;
		psrc->m_nNumServRatingCase = 0;
		psrc->m_nNumStlBlrElem = 0;
		psrc->m_nNumStlStreRatingCase = 0;
		psrc->m_nNumStlServRatingCase = 0;
		psrc->m_nNumStlFatiRatingCase = 0;
		psrc->m_nNumStlStreRCase = 0;
		psrc->m_nNumStlServRCase = 0;
		psrc->m_nNumStlFatiRCase = 0;
		psrc->m_nNumRCBlrElem = 0;
		psrc->m_nNumRCBlrMemb = 0;
		// Plate Girder Design.
		psrc->m_nNumPgdElem = 0;
		psrc->m_nNumPgdStreLcase = 0;
		psrc->m_nNumPgdServLcase = 0;
		psrc->m_nNumPgdFatiLcase = 0;
		psrc->m_nNumSpan = 0;
		psrc->m_nNumSpanElem = 0;
		psrc->m_nNumSpanVBeam = 0;
		psrc->m_nNumSodElem = 0;
		psrc->m_nNumSodStrnLcase = 0;
		psrc->m_nNumSodFatiLcase = 0;

		psrc->m_nNumStlCompGirderElem      = 0;
		psrc->m_nNumStlCompGirderStrnLcase = 0;
		psrc->m_nNumStlCompGirderFatiLcase = 0;
		psrc->m_nNumStlCompGirderServLcase = 0;

		psrc->m_nNumSbdoBeam    = 0;
		psrc->m_nNumSbdoWall    = 0;
		psrc->m_nNumPRCLcomStrn = 0;
		psrc->m_nNumPRCLcomServ = 0;

		// Story
		psrc->m_nNumStor = psrc->m_nNumStor_JUD = 0, psrc->m_bStoryCenter = psrc->m_bStoryShearRatio = FALSE;
		psrc->m_nNumStor_dummy = 0;
		// Tendon
		psrc->m_nNumTdna = 0;
		psrc->m_nNumUsedTendon = 0;
		psrc->m_nNumMaxTendonElem = 0;
		psrc->m_nNumMaxPrestressed = 0;
		// PTSlab Tendon
		psrc->m_nNumPTSlabTendon = 0;

		// 사인장,전단응력 계산에 사용되는 보요소개수
		psrc->m_nNumStssBeam = 0;
		psrc->m_nNumStssBeam7th = 0;
		// Erection
		psrc->m_nNumEseq = 0;
		// Suspension Bridge
		psrc->m_nSuspensionType = 0;
		// 지점침하
		psrc->m_nNumSmpt = 0;

		//동시 발생 반력 그룹수
		m_nNumCrgr = 0;
		//Camber
		psrc->m_nNumCmcs = 0;
		//Sub Domain
		psrc->m_nNumSbdo = 0;
	}

	void CopyStaticAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		m_nNumStld = psrc->m_nNumStld;
		// Nonlinear
		m_nNumNonl = psrc->m_nNumNonl, m_nNonlType = psrc->m_nNonlType;
		m_nNonLinearStep = psrc->m_nNonLinearStep;
		// Composite Section
		m_nNumPlcb = psrc->m_nNumPlcb;
		// P-Delta
		m_nNumPdel = psrc->m_nNumPdel;
		// Crane
		m_nNumCran = psrc->m_nNumCran, m_nNumCranTot = psrc->m_nNumCranTot;
	}
	void ClearStaticAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		psrc->m_nNumStld = 0;
		// Nonlinear
		psrc->m_nNumNonl = 0, psrc->m_nNonlType = 0;
		psrc->m_nNonLinearStep = 0;
		// Composite Section
		psrc->m_nNumPlcb = 0;
		// P-Delta
		psrc->m_nNumPdel = 0;
		// Crane
		psrc->m_nNumCran = 0, psrc->m_nNumCranTot = 0;
	}
	void CopyStageAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		m_nNumSgld = psrc->m_nNumSgld;
		m_nNumSgldRead = psrc->m_nNumSgldRead;
		m_bCalcConcurrentForce = psrc->m_bCalcConcurrentForce;
		m_nStageStep = psrc->m_nStageStep;
		m_nNumStage = psrc->m_nNumStage;
		m_nNumHylc = psrc->m_nNumHylc;
		// 합성단면
		m_nNumSectPart = psrc->m_nNumSectPart;
	}
	void ClearStageAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		psrc->m_nNumSgld = 0;
		psrc->m_nNumSgldRead = 0;
		psrc->m_bCalcConcurrentForce = FALSE;
		psrc->m_nStageStep = 0;
		psrc->m_nNumStage = 0;
		psrc->m_nNumHylc = 0;
		// 합성단면
		psrc->m_nNumSectPart = 0;
	}
	void CopyDynamicAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// Eigenvalue
		m_nNumEigv = psrc->m_nNumEigv;
		// Response Spectrum
		m_nNumSplc = psrc->m_nNumSplc;
		// 편심하중
		m_nNumRaem = psrc->m_nNumRaem, m_nNumEccn = psrc->m_nNumEccn;
		// Time History
		CopyTimeHistoryAnalysisData(psrc);
	}
	void ClearDynamicAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// Eigenvalue
		psrc->m_nNumEigv = 0;
		// Response Spectrum
		psrc->m_nNumSplc = 0;
		// 편심하중
		psrc->m_nNumRaem = psrc->m_nNumEccn = 0;
		// Time History
		ClearTimeHistoryAnalysisData(psrc);
	}
	void CopyTimeHistoryAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// Time History
		m_nNumThis = psrc->m_nNumThis, m_nNumThis_Modal = psrc->m_nNumThis_Modal, m_nNumThis_Direct = psrc->m_nNumThis_Direct;
		m_nNumThis_NonlDirect = psrc->m_nNumThis_NonlDirect, m_nNumThis_NonlStatic = psrc->m_nNumThis_NonlStatic, m_nNumThis_Init = psrc->m_nNumThis_Init;
		m_nNumThis_NonlLargeDisp = psrc->m_nNumThis_NonlLargeDisp;
		m_nNumThfc = psrc->m_nNumThfc;
		m_nNumMaxTimeStep = psrc->m_nNumMaxTimeStep, m_nNumMaxTimeStepNotOut = psrc->m_nNumMaxTimeStepNotOut;
		m_nNumMaxTimeStep_Modal = psrc->m_nNumMaxTimeStep_Modal, m_nNumMaxTimeStep_Direct = psrc->m_nNumMaxTimeStep_Direct;
		m_nNumEquation_Modal = psrc->m_nNumEquation_Modal, m_nNumEquation_Direct = psrc->m_nNumEquation_Direct;
		m_nNumNlbm = psrc->m_nNumNlbm;
		m_nNumNlwl = psrc->m_nNumNlwl;
		m_nNumNltr = psrc->m_nNumNltr;
		m_nNumNllk = psrc->m_nNumNllk;
		m_nNumNlhg = psrc->m_nNumNlhg;
		m_nNumNlhg_SelectOutputOnly = psrc->m_nNumNlhg_SelectOutputOnly;
		m_nNumFibr      = psrc->m_nNumFibr;
		m_nNumFibrCell  = psrc->m_nNumFibrCell;
		m_nNumFibrSect  = psrc->m_nNumFibrSect;
		m_nNumFibw      = psrc->m_nNumFibw;
		m_nNumFibwCell  = psrc->m_nNumFibwCell;
		m_nNumFibwSect  = psrc->m_nNumFibwSect;
		m_nNumFibr_PO      = psrc->m_nNumFibr_PO;
		m_nNumFibrCell_PO  = psrc->m_nNumFibrCell_PO;
		m_nNumFibrSect_PO  = psrc->m_nNumFibrSect_PO;
		m_nNumFibw_PO      = psrc->m_nNumFibw_PO;
		m_nNumFibwCell_PO  = psrc->m_nNumFibwCell_PO;
		m_nNumFibwSect_PO  = psrc->m_nNumFibwSect_PO;
		m_nNumofSection = psrc->m_nNumofSection;
		m_nNumFibrCell_SelectOutputOnly = psrc->m_nNumFibrCell_SelectOutputOnly;
		m_nNumFibwCell_SelectOutputOnly = psrc->m_nNumFibwCell_SelectOutputOnly;
		m_nNumGlbm = psrc->m_nNumGlbm;
		m_nNumGlbw = psrc->m_nNumGlbw;
		m_nNumGltr = psrc->m_nNumGltr;
		m_nNumGlps = psrc->m_nNumGlps;
		m_nNumGlnp = psrc->m_nNumGlnp;
		m_nNumGlsd = psrc->m_nNumGlsd;
		m_nNumGlSdvi = psrc->m_nNumGlSdvi;
		m_nNumGlSdve = psrc->m_nNumGlSdve;
		m_nNumGlSdst = psrc->m_nNumGlSdst;
		m_nNumGlSdhy = psrc->m_nNumGlSdhy;
		m_nNumGlSdis = psrc->m_nNumGlSdis;
		m_nNumGlMssp = psrc->m_nNumGlMssp;
		m_nNumGlSdgb = psrc->m_nNumGlSdgb;
		m_aNumTimeStep.Copy(psrc->m_aNumTimeStep);
		m_aNonlDirectLoadCase.Copy(psrc->m_aNonlDirectLoadCase);
		m_aNonlStaticLoadCase.Copy(psrc->m_aNonlStaticLoadCase);
		m_aNonlInitLoadCase.Copy(psrc->m_aNonlInitLoadCase);
		m_aNonlLargeDisp.Copy(psrc->m_aNonlLargeDisp);
		m_aNonlPDelta.Copy(psrc->m_aNonlPDelta);
		m_aSTWLLoadCase.Copy(psrc->m_aSTWLLoadCase);
		// Time History Extended Result, Animation Result
		m_nNumThex = psrc->m_nNumThex;
		m_nNumTham = psrc->m_nNumTham;
		m_nNumThpo = psrc->m_nNumThpo;
	}
	void ClearTimeHistoryAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// Time History
		psrc->m_nNumThis = psrc->m_nNumThis_Modal = psrc->m_nNumThis_Direct = 0;
		psrc->m_nNumThis_NonlDirect = psrc->m_nNumThis_NonlStatic = psrc->m_nNumThis_Init = 0;
		psrc->m_nNumThis_NonlLargeDisp = 0;
		psrc->m_nNumThfc = 0;
		psrc->m_nNumMaxTimeStep = psrc->m_nNumMaxTimeStepNotOut = 0;
		psrc->m_nNumMaxTimeStep_Modal = psrc->m_nNumMaxTimeStep_Direct = 0;
		psrc->m_nNumEquation_Modal = psrc->m_nNumEquation_Direct = 0;
		psrc->m_nNumNlbm = 0;
		psrc->m_nNumNlwl = 0;
		psrc->m_nNumNltr = 0;
		psrc->m_nNumNllk = 0;
		psrc->m_nNumNlhg = 0;
		psrc->m_nNumNlhg_SelectOutputOnly = 0;
		psrc->m_nNumFibr = 0;
		psrc->m_nNumFibrCell = 0;
		psrc->m_nNumFibrSect = 0;
		psrc->m_nNumFibw = 0;
		psrc->m_nNumFibwCell = 0;
		psrc->m_nNumFibwSect = 0;
		psrc->m_nNumFibr_PO = 0;
		psrc->m_nNumFibrCell_PO = 0;
		psrc->m_nNumFibrSect_PO = 0;
		psrc->m_nNumFibw_PO = 0;
		psrc->m_nNumFibwCell_PO = 0;
		psrc->m_nNumFibwSect_PO = 0;
		psrc->m_nNumofSection = 0;
		psrc->m_nNumFibrCell_SelectOutputOnly = 0;
		psrc->m_nNumFibwCell_SelectOutputOnly = 0;
		psrc->m_nNumGlbm = 0;
		psrc->m_nNumGlbw = 0;
		psrc->m_nNumGltr = 0;
		psrc->m_nNumGlps = 0;
		psrc->m_nNumGlnp = 0;
		psrc->m_nNumGlsd = 0;
		psrc->m_nNumGlSdvi = 0;
		psrc->m_nNumGlSdve = 0;
		psrc->m_nNumGlSdst = 0;
		psrc->m_nNumGlSdhy = 0;
		psrc->m_nNumGlSdis = 0;
		psrc->m_nNumGlMssp = 0;
		psrc->m_nNumGlSdgb = 0;
		psrc->m_aNumTimeStep.RemoveAll();
		psrc->m_aNonlDirectLoadCase.RemoveAll();
		psrc->m_aNonlStaticLoadCase.RemoveAll();
		psrc->m_aNonlInitLoadCase.RemoveAll();
		psrc->m_aNonlLargeDisp.RemoveAll();
		psrc->m_aNonlPDelta.RemoveAll();
		psrc->m_aSTWLLoadCase.RemoveAll();
		// Time History Extended Result, Animation Result
		psrc->m_nNumThex = 0;
		psrc->m_nNumTham = 0;
		psrc->m_nNumThpo = 0;
	}
	void CopyPushoverAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// Pushover_old (not used)
		m_nNumPold = psrc->m_nNumPold, m_nPushOverStep = psrc->m_nPushOverStep, m_nNumHnge = psrc->m_nNumHnge;
		// Pushover_new
		m_nNumPolc = psrc->m_nNumPolc;
		m_nNumPhge = psrc->m_nNumPhge;
		m_nNumPhgeElem = psrc->m_nNumPhgeElem;
		m_nNumPhgeNlnk = psrc->m_nNumPhgeNlnk;
		m_nNumPhgeNode = psrc->m_nNumPhgeNode;
		m_nNumPhgeSprn = psrc->m_nNumPhgeSprn;
		// Pushover Inelastic Dynamic
		m_nNumPNbm = psrc->m_nNumPNbm;
		m_nNumPNtr = psrc->m_nNumPNtr;
		m_nNumPNwa = psrc->m_nNumPNwa;
		m_nNumPNlk = psrc->m_nNumPNlk;
		m_nNumPNhg = psrc->m_nNumPNhg;
		m_nNumPNps = psrc->m_nNumPNps;
	}
	void ClearPushoverAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// Pushover_old (not used)
		psrc->m_nNumPold = psrc->m_nPushOverStep = psrc->m_nNumHnge = 0;
		// Pushover_new
		psrc->m_nNumPolc = 0;
		psrc->m_nNumPhge = 0;
		psrc->m_nNumPhgeElem = 0;
		psrc->m_nNumPhgeNlnk = 0;
		psrc->m_nNumPhgeNode = 0;
		psrc->m_nNumPhgeSprn = 0;
		// Pushover Inelastic Dynamic
		psrc->m_nNumPNbm = 0;
		psrc->m_nNumPNtr = 0;
		psrc->m_nNumPNwa = 0;
		psrc->m_nNumPNlk = 0;
		psrc->m_nNumPNhg = 0;
		psrc->m_nNumPNps = 0;
	}
	void CopyMovingLoadAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// 이동하중
		m_nNumInfl = psrc->m_nNumInfl;
		m_nNumLlan = psrc->m_nNumLlan, m_nNumSlan = psrc->m_nNumSlan, m_nNumSinf = psrc->m_nNumSinf, m_nNumLlanop = psrc->m_nNumLlanop, m_nNumSlanop = psrc->m_nNumSlanop;
		m_nNumMvld = psrc->m_nNumMvld;
		m_nMvCode = psrc->m_nMvCode;
		m_nNumInflDisp = psrc->m_nNumInflDisp, m_nNumInflTrss = psrc->m_nNumInflTrss, m_nNumInflBeam = psrc->m_nNumInflBeam;
		m_nNumInflPlat = psrc->m_nNumInflPlat, m_nNumInflSold = psrc->m_nNumInflSold;
		m_nNumNegative = psrc->m_nNumNegative;
		// 격자해석
		m_nNumInflGrid = psrc->m_nNumInflGrid;
		m_nNumGmld = psrc->m_nNumGmld;
		m_nNumGmldLive = psrc->m_nNumGmldLive;
		m_nNumCref = psrc->m_nNumCref;
		m_aCrefCount.Copy(psrc->m_aCrefCount);
		m_aGmldLive.Copy(psrc->m_aGmldLive);
		// MovingLoad
	}
	void ClearMovingLoadAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// 이동하중
		psrc->m_nNumInfl = 0;
		psrc->m_nNumLlan = 0, psrc->m_nNumSlan = 0, psrc->m_nNumSinf = 0, psrc->m_nNumLlanop = 0, psrc->m_nNumSlanop = 0;
		psrc->m_nNumMvld = 0;
		psrc->m_nMvCode = 0;
		psrc->m_nNumInflDisp = 0, psrc->m_nNumInflTrss = 0, psrc->m_nNumInflBeam = 0;
		psrc->m_nNumInflPlat = 0, psrc->m_nNumInflSold = 0;
		psrc->m_nNumNegative = 0;

		// 격자해석
		psrc->m_nNumInflGrid = 0;
		psrc->m_nNumGmld = 0;
		psrc->m_nNumGmldLive = 0;
		psrc->m_nNumCref = 0;
		psrc->m_aCrefCount.RemoveAll();
		psrc->m_aGmldLive.RemoveAll();
	}
	void CopySettlementAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// 지점침하
		m_nNumSmlc = psrc->m_nNumSmlc;
	}
	void ClearSettlementAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// 지점침하
		psrc->m_nNumSmlc = 0;
	}
	void CopyETCAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// Buckling
		m_nNumBuck = psrc->m_nNumBuck;

		m_arUsedModeNumPerSplc.Copy(psrc->m_arUsedModeNumPerSplc);     
	}
	void ClearETCAnalysisData(T_POSTCTRL_DATACOUNT* psrc)
	{
		// Buckling
		psrc->m_nNumBuck = 0;

		psrc->m_arUsedModeNumPerSplc.RemoveAll(); 
	}
	BOOL ExistETCAnalysis()
	{
		if(m_nNumBuck > 0)return TRUE;

		return FALSE;
	}
};

// MNET:2686 KYUNG-HA 20070312
struct T_POSTCTRL_DATAEXIST
{
	BOOL bExistNDAC,bExistNDIF,bExistNDIM,bExistNDAM;
	BOOL bExistMACC,bExistMCFC,bExistMESF,bExistRsag;
	void Initialize()
	{
		bExistNDAC=bExistNDIF=bExistNDIM=bExistNDAM=FALSE;
		bExistMACC=bExistMCFC=bExistMESF=bExistRsag=FALSE;
	}
};

#define DGN_6DOF_COMPONENT          6
#define DGN_7DOF_COMPONENT          9
#define DGN_FORCE_CONCURRENT       12
#define DGN_FORCE_CONCURRENT_7DOF  18
#define DGN_FORCE_LCOM_SUB_MAX     20

//
	#define T_LCOM_KEY unsigned int
	union T_LCOM_MAXMIN_K
	{
		T_LCOM_KEY keymap;
		struct
		{
			// Modify by GAY. MNET:2683. ('07.05.09). To Consider Concurrent Force.
			unsigned int lcomkey : 27;
			unsigned int maxmin  :  1;
			unsigned int compType:  4;  // Component Type. (1~7 : FX, FY, FZ, MX, MY, MZ, Stress).
		}key;
	};

//
	struct T_WALLINTERNAL
	{
		CArray<unsigned int, unsigned int>arInternalWallId;
		void Initialize()
		{
			arInternalWallId.RemoveAll();
		}
		T_WALLINTERNAL()
		{
		}
		T_WALLINTERNAL(T_WALLINTERNAL& src)
		{
			*this = src;
		}
		T_WALLINTERNAL& operator=(T_WALLINTERNAL& src)
		{
			arInternalWallId.Copy(src.arInternalWallId);
			return *this;
		}
	};

	struct T_WALLELEM
	{
		CArray<unsigned int, unsigned int>arWallElemList;
		void Initialize()
		{
			arWallElemList.RemoveAll();
		}
		T_WALLELEM()
		{
		}
		T_WALLELEM(T_WALLELEM& src)
		{
			*this = src;
		}
		T_WALLELEM& operator=(T_WALLELEM& src)
		{
			arWallElemList.Copy(src.arWallElemList);
			return *this;
		}
	};

	struct T_STAGEELEM_SUB_D
	{
		int nActivePartIndex;
		T_MATL_K ActivePartMaterial;
		int nCountCurrentPart;
		int nActiveStageIndex;

		void Initialize()
		{
			nActivePartIndex=0;
			ActivePartMaterial=0;
			nCountCurrentPart=0;
			nActiveStageIndex=0;
		}

		T_STAGEELEM_SUB_D(){ Initialize(); }
		T_STAGEELEM_SUB_D(const T_STAGEELEM_SUB_D& src) { *this = src; }
		T_STAGEELEM_SUB_D& operator=(const T_STAGEELEM_SUB_D& rData)
		{
			nActivePartIndex    = rData.nActivePartIndex;
			ActivePartMaterial  = rData.ActivePartMaterial;
			nCountCurrentPart   = rData.nCountCurrentPart;
			nActiveStageIndex   = rData.nActiveStageIndex;
			return *this;
		}
		BOOL operator==(const T_STAGEELEM_SUB_D& src) const
		{

			if (nActivePartIndex    != src.nActivePartIndex)    return FALSE;
			if (ActivePartMaterial  != src.ActivePartMaterial)  return FALSE;
			if (nCountCurrentPart   != src.nCountCurrentPart)   return FALSE;
			if (nActiveStageIndex   != src.nActiveStageIndex)   return FALSE;

			return TRUE;
		}
	};

	struct T_STAGEELEM_D
	{
		CArray<T_STAGEELEM_SUB_D, T_STAGEELEM_SUB_D&> aStageElemD;
		void Initialize()
		{
			aStageElemD.RemoveAll();
		}
		T_STAGEELEM_D(){ Initialize(); }
		T_STAGEELEM_D(const T_STAGEELEM_D& src) { *this = src; }
		T_STAGEELEM_D& operator=(const T_STAGEELEM_D& rData)
		{
			aStageElemD.Copy(rData.aStageElemD);
			return *this;
		}
	};

	struct T_STAGESECT_BASE_DUMB
	{
		double dDist_y; // Dumbbell 단면 도심 (최종 - 해당 파트) 간격
		double dDist_z; // Dumbbell 단면 도심 (최종 - 해당 파트) 간격
		double dEcs; // Dumbbell 단면의 등가 탄성 계수       // Equiv. E  
		double dGcs; // Dumbbell 단면의 등가 전단 탄성 계수
		double dDns; // Steel 탄성계수와 Dumbbell 단면의 등가 탄성계수의 비율
		double dDnc; // Concrete 탄성계수와 Dumbbell 단면의 등가 탄성계수의 비율
		double dDens; // 등가 중량 밀도 - 자중 계산을 위한 값
		double dE_sc; // E    ratio Steel and Conc.
		double dA_sc; // Area ratio Steel and Conc.
		void Initialize()
		{
			dDist_y = 0.0;
			dDist_z = 0.0;
			dEcs = 0.0;
			dGcs = 0.0;
			dDns = 0.0;
			dDnc = 0.0;
			dDens = 0.0;
			dE_sc = 0.0;
			dA_sc = 0.0;
		}
	};

	struct T_STAGESECT_BASE
	{
		T_CSCS_K KeyCscs;
		T_SECT_STIFFNESS StiffnessI;   //Section for CS의 part
		double dDistI_y;
		double dDistI_z;
		double dAge;
		T_SECT_STIFFNESS dStiffScale; // 강성 Scale Factor
		BOOL bTapered;
		T_SECT_STIFFNESS StiffnessJ;
		double dDistJ_y;
		double dDistJ_z;
		T_STAGESECT_BASE_DUMB Dumb;

		void Initialize()
		{
			KeyCscs=0;
			StiffnessI.Initialize();
			dDistI_y=0.;
			dDistI_z=0.;
			dAge=0.;
			dStiffScale.Initialize();
			bTapered=FALSE;
			StiffnessJ.Initialize();
			dDistJ_y=0.;
			dDistJ_z=0.;
			Dumb.Initialize();
		}
	};
	struct T_STAGESECT_D
	{
		CArray<T_STAGESECT_BASE, T_STAGESECT_BASE&> aPart;
		T_STAGESECT_D(){}
		T_STAGESECT_D(const T_STAGESECT_D& src) { *this = src; }
		T_STAGESECT_D& operator=(const T_STAGESECT_D& rData)
		{
			aPart.Copy(rData.aPart);
			return *this;
		}
		void Initialize()
		{
			aPart.RemoveAll();
		}
	};
	// Add by ZINU.('04.03.24). To Calculate Stresses for 지점모멘트저감,하중조합별계산.
	struct T_STRSPOST_BASE
	{
		// For General Stress.
		double dArea;
		double dQyb, dQzb;
		double dIyy, dIzz;
		double dyzM[2][4];	// 휨검토위치 1st(0=y,1=z), 2nd(TL,TR,BR,BL).
		// For Special Stress.
		BOOL bSpecial;			// PSC=TRUE, Other=FALSE.
		// 0=Top(Web), 1=Center, 2=Bot(Web).
		double dQ[3];				// 단면1차 모멘트
		double db[3];				// 전체웨브두께
		double dyzV[2][6];	// 전단검토위치 1st(0=y,1=z), 2nd(TL,TR,CL,CR,BL,BR)
		double dKt;		    	// 비틀림에 의한 전단응력도에 관한 계수
		double dKtweb[6];   // 비틀림에 의한 전단응력도에 관한 계수, Web 전단위치에서의 두께, 
		double dKtThik;			// 외측웨브의 최소두께
		// Add by ZINU.('06.04.24). Consider N.A by Eeffective Width Scale Factor.
		double dMoveTop, dMoveBot;
		double dScaleIy;

		// 7Dof 해석 데이터 생성(6 Point) v850
		double dQ_7Dof[6];				// v850, 단면1차 모멘트
		double db_7Dof[6];				// v850, 전체웨브두께
		double dyz_7Dof[2][6];		// v850, 전단검토위치 1st(0=y,1=z), 2nd(6 Point)

		inline void Initialize()
		{
			#if 1
			//cklee@midasit.com double 0의 bit pattern은 "00000000 00000000" 이므로 
			// loop 돌지 않고  memset 한번만 호출해서 초기화 
			memset(this, 0, sizeof(T_STRSPOST_BASE));
			#else
			int i=0;
			dArea=0.0;
			dQyb=0.0; dQzb=0.0;
			dIyy=0.0; dIzz=0.0;
			for(int i = 0; i<4; i++)	{dyzM[0][i]=0.0; dyzM[1][i]=0.0;}
			dQ[0]=0.0; dQ[1]=0.0; dQ[2]=0.0;
			db[0]=0.0; db[1]=0.0; db[2]=0.0;
			for(int i = 0; i<6; i++)	{dyzV[0][i]=0.0; dyzV[1][i]=0.0;}
			dKt=0.0;
			memset(dKtweb,0,sizeof(dKtweb));
			dKtThik=0.0;
			dMoveTop=0.0; dMoveBot=0.0;
			dScaleIy=0.0;

			for(int i = 0; i<6; i++)
			{
				dQ_7Dof[i]=0.0;
				db_7Dof[i]=0.0;
				dyz_7Dof[0][i]=0.0;  // y
				dyz_7Dof[1][i]=0.0;  // z
			}
			#endif

		}
		T_STRSPOST_BASE()	{Initialize();}
		T_STRSPOST_BASE(const T_STRSPOST_BASE& src)	{*this = src;}
		T_STRSPOST_BASE& operator=(const T_STRSPOST_BASE& rData)
		{
			int i=0;
			dArea=rData.dArea;
			dQyb=rData.dQyb; dQzb=rData.dQzb;
			dIyy=rData.dIyy; dIzz=rData.dIzz;
			for(int i = 0; i<4; i++)	{dyzM[0][i]=rData.dyzM[0][i]; dyzM[1][i]=rData.dyzM[1][i];}
			dQ[0]=rData.dQ[0]; dQ[1]=rData.dQ[1]; dQ[2]=rData.dQ[2];
			db[0]=rData.db[0]; db[1]=rData.db[1]; db[2]=rData.db[2];
			for(int i = 0; i<6; i++)	{dyzV[0][i]=rData.dyzV[0][i]; dyzV[1][i]=rData.dyzV[1][i];}
			dKt=rData.dKt;
			memcpy(dKtweb,rData.dKtweb,sizeof(dKtweb));
			dKtThik=rData.dKtThik;
			dMoveTop=rData.dMoveTop; dMoveBot=rData.dMoveBot;
			dScaleIy=rData.dScaleIy;

			for(int i = 0; i<6; i++)
			{
				dQ_7Dof[i]=rData.dQ_7Dof[i];
				db_7Dof[i]=rData.db_7Dof[i];
				dyz_7Dof[0][i]=rData.dyz_7Dof[0][i];  // y
				dyz_7Dof[1][i]=rData.dyz_7Dof[1][i];  // z
			}

			return *this;
		}
		T_STRSPOST_BASE& operator+=(const T_STRSPOST_BASE& rData)
		{
			dArea+=rData.dArea;
			dQyb+=rData.dQyb; dQzb+=rData.dQzb;
			dIyy+=rData.dIyy; dIzz+=rData.dIzz;
			//cklee@midasit.com loop unrolling 적용 
			#if 1
			dyzM[0][0] += rData.dyzM[0][0]; dyzM[1][0] += rData.dyzM[1][0];
			dyzM[0][1] += rData.dyzM[0][1]; dyzM[1][1] += rData.dyzM[1][1];
			dyzM[0][2] += rData.dyzM[0][2]; dyzM[1][2] += rData.dyzM[1][2];
			dyzM[0][3] += rData.dyzM[0][3]; dyzM[1][3] += rData.dyzM[1][3];
			#else
			int i = 0;
			for(int i = 0; i<4; i++)	{dyzM[0][i]+=rData.dyzM[0][i]; dyzM[1][i]+=rData.dyzM[1][i];}
			#endif
			
			dQ[0]+=rData.dQ[0]; dQ[1]+=rData.dQ[1]; dQ[2]+=rData.dQ[2];
			db[0]+=rData.db[0]; db[1]+=rData.db[1]; db[2]+=rData.db[2];
			
			//cklee@midasit.com loop unrolling 적용 
			#if 1
			dyzV[0][0] += rData.dyzV[0][0]; dyzV[1][0] += rData.dyzV[1][0];	dKtweb[0] += rData.dKtweb[0];
			dyzV[0][1] += rData.dyzV[0][1]; dyzV[1][1] += rData.dyzV[1][1];	dKtweb[1] += rData.dKtweb[1];
			dyzV[0][2] += rData.dyzV[0][2]; dyzV[1][2] += rData.dyzV[1][2];	dKtweb[2] += rData.dKtweb[2];
			dyzV[0][3] += rData.dyzV[0][3]; dyzV[1][3] += rData.dyzV[1][3];	dKtweb[3] += rData.dKtweb[3];
			dyzV[0][4] += rData.dyzV[0][4]; dyzV[1][4] += rData.dyzV[1][4];	dKtweb[4] += rData.dKtweb[4];
			dyzV[0][5] += rData.dyzV[0][5]; dyzV[1][5] += rData.dyzV[1][5];	dKtweb[5] += rData.dKtweb[5];
			#else
			for(int i = 0; i<6; i++)	
			{
				dyzV[0][i]+=rData.dyzV[0][i]; dyzV[1][i]+=rData.dyzV[1][i];
				dKtweb[i] += rData.dKtweb[i];
			}			
			#endif
			dKt+=rData.dKt;
			dKtThik+=rData.dKtThik;
			dMoveTop+=rData.dMoveTop; dMoveBot+=rData.dMoveBot;
			dScaleIy+=rData.dScaleIy;
			//cklee@midasit.com loop unrolling 적용 
			#if 1                                                            // y                                    // z
			dQ_7Dof[0] += rData.dQ_7Dof[0]; db_7Dof[0] += rData.db_7Dof[0];	dyz_7Dof[0][0] += rData.dyz_7Dof[0][0];  dyz_7Dof[1][0] += rData.dyz_7Dof[1][0];
			dQ_7Dof[1] += rData.dQ_7Dof[1]; db_7Dof[1] += rData.db_7Dof[1];	dyz_7Dof[0][1] += rData.dyz_7Dof[0][1];  dyz_7Dof[1][1] += rData.dyz_7Dof[1][1];
			dQ_7Dof[2] += rData.dQ_7Dof[2]; db_7Dof[2] += rData.db_7Dof[2];	dyz_7Dof[0][2] += rData.dyz_7Dof[0][2];  dyz_7Dof[1][2] += rData.dyz_7Dof[1][2];
			dQ_7Dof[3] += rData.dQ_7Dof[3]; db_7Dof[3] += rData.db_7Dof[3];	dyz_7Dof[0][3] += rData.dyz_7Dof[0][3];  dyz_7Dof[1][3] += rData.dyz_7Dof[1][3];
			dQ_7Dof[4] += rData.dQ_7Dof[4]; db_7Dof[4] += rData.db_7Dof[4];	dyz_7Dof[0][4] += rData.dyz_7Dof[0][4];  dyz_7Dof[1][4] += rData.dyz_7Dof[1][4];
			dQ_7Dof[5] += rData.dQ_7Dof[5]; db_7Dof[5] += rData.db_7Dof[5];	dyz_7Dof[0][5] += rData.dyz_7Dof[0][5];  dyz_7Dof[1][5] += rData.dyz_7Dof[1][5];
			#else
			for(int i = 0; i<6; i++)
			{
				dQ_7Dof[i]+=rData.dQ_7Dof[i];
				db_7Dof[i]+=rData.db_7Dof[i];
				dyz_7Dof[0][i]+=rData.dyz_7Dof[0][i];  // y
				dyz_7Dof[1][i]+=rData.dyz_7Dof[1][i];  // z
			}
			#endif

			return *this;
		}
		T_STRSPOST_BASE& operator-=(const T_STRSPOST_BASE& rData)
		{
			dArea-=rData.dArea;
			dQyb-=rData.dQyb; dQzb-=rData.dQzb;
			dIyy-=rData.dIyy; dIzz-=rData.dIzz;
			//cklee@midasit.com loop unrolling 적용 
			#if 1
			dyzM[0][0] -= rData.dyzM[0][0]; dyzM[1][0] -= rData.dyzM[1][0];
			dyzM[0][1] -= rData.dyzM[0][1]; dyzM[1][1] -= rData.dyzM[1][1];
			dyzM[0][2] -= rData.dyzM[0][2]; dyzM[1][2] -= rData.dyzM[1][2];
			dyzM[0][3] -= rData.dyzM[0][3]; dyzM[1][3] -= rData.dyzM[1][3];
			#else
			int i = 0;
			for(int i = 0; i<4; i++)	{dyzM[0][i]-=rData.dyzM[0][i]; dyzM[1][i]-=rData.dyzM[1][i];}
			#endif
			dQ[0]-=rData.dQ[0]; dQ[1]-=rData.dQ[1]; dQ[2]-=rData.dQ[2];
			db[0]-=rData.db[0]; db[1]-=rData.db[1]; db[2]-=rData.db[2];
			//cklee@midasit.com loop unrolling 적용 
			#if 1
			dyzV[0][0] -= rData.dyzV[0][0]; dyzV[1][0] -= rData.dyzV[1][0];	dKtweb[0] -= rData.dKtweb[0];
			dyzV[0][1] -= rData.dyzV[0][1]; dyzV[1][1] -= rData.dyzV[1][1];	dKtweb[1] -= rData.dKtweb[1];
			dyzV[0][2] -= rData.dyzV[0][2]; dyzV[1][2] -= rData.dyzV[1][2];	dKtweb[2] -= rData.dKtweb[2];
			dyzV[0][3] -= rData.dyzV[0][3]; dyzV[1][3] -= rData.dyzV[1][3];	dKtweb[3] -= rData.dKtweb[3];
			dyzV[0][4] -= rData.dyzV[0][4]; dyzV[1][4] -= rData.dyzV[1][4];	dKtweb[4] -= rData.dKtweb[4];
			dyzV[0][5] -= rData.dyzV[0][5]; dyzV[1][5] -= rData.dyzV[1][5];	dKtweb[5] -= rData.dKtweb[5];
			#else
			for(int i = 0; i<6; i++)	
			{
				dyzV[0][i]-=rData.dyzV[0][i]; dyzV[1][i]-=rData.dyzV[1][i];
				dKtweb[i] -= rData.dKtweb[i];
			}
			#endif
			dKt-=rData.dKt;
			dKtThik-=rData.dKtThik;
			dMoveTop-=rData.dMoveTop; dMoveBot-=rData.dMoveBot;
			dScaleIy-=rData.dScaleIy;
			//cklee@midasit.com loop unrolling 적용 
			#if 1                                                            // y                                    // z
			dQ_7Dof[0] -= rData.dQ_7Dof[0]; db_7Dof[0] -= rData.db_7Dof[0];	dyz_7Dof[0][0] -= rData.dyz_7Dof[0][0];  dyz_7Dof[1][0] -= rData.dyz_7Dof[1][0];
			dQ_7Dof[1] -= rData.dQ_7Dof[1]; db_7Dof[1] -= rData.db_7Dof[1];	dyz_7Dof[0][1] -= rData.dyz_7Dof[0][1];  dyz_7Dof[1][1] -= rData.dyz_7Dof[1][1];
			dQ_7Dof[2] -= rData.dQ_7Dof[2]; db_7Dof[2] -= rData.db_7Dof[2];	dyz_7Dof[0][2] -= rData.dyz_7Dof[0][2];  dyz_7Dof[1][2] -= rData.dyz_7Dof[1][2];
			dQ_7Dof[3] -= rData.dQ_7Dof[3]; db_7Dof[3] -= rData.db_7Dof[3];	dyz_7Dof[0][3] -= rData.dyz_7Dof[0][3];  dyz_7Dof[1][3] -= rData.dyz_7Dof[1][3];
			dQ_7Dof[4] -= rData.dQ_7Dof[4]; db_7Dof[4] -= rData.db_7Dof[4];	dyz_7Dof[0][4] -= rData.dyz_7Dof[0][4];  dyz_7Dof[1][4] -= rData.dyz_7Dof[1][4];
			dQ_7Dof[5] -= rData.dQ_7Dof[5]; db_7Dof[5] -= rData.db_7Dof[5];	dyz_7Dof[0][5] -= rData.dyz_7Dof[0][5];  dyz_7Dof[1][5] -= rData.dyz_7Dof[1][5];
			#else
			for(int i = 0; i<6; i++)
			{
				dQ_7Dof[i]-=rData.dQ_7Dof[i];
				db_7Dof[i]-=rData.db_7Dof[i];
				dyz_7Dof[0][i]-=rData.dyz_7Dof[0][i];  // y
				dyz_7Dof[1][i]-=rData.dyz_7Dof[1][i];  // z
			}
			#endif
			return *this;
		}
		T_STRSPOST_BASE& operator*=(double dRat)
		{
			int i=0;
			dArea*=dRat;
			dQyb*=dRat; dQzb*=dRat;
			dIyy*=dRat; dIzz*=dRat;
			for(int i = 0; i<4; i++)	{dyzM[0][i]*=dRat; dyzM[1][i]*=dRat;}
			dQ[0]*=dRat; dQ[1]*=dRat; dQ[2]*=dRat;
			db[0]*=dRat; db[1]*=dRat; db[2]*=dRat;
			for(int i = 0; i<6; i++)	
			{
				dyzV[0][i]*=dRat; dyzV[1][i]*=dRat;
				dKtweb[i] *= dRat;
			}			
			dKt*=dRat;
			dKtThik*=dRat;
			dMoveTop*=dRat; dMoveBot*=dRat;
			dScaleIy*=dRat;

			for(int i = 0; i<6; i++)
			{
				dQ_7Dof[i]*=dRat;
				db_7Dof[i]*=dRat;
				dyz_7Dof[0][i]*=dRat;  // y
				dyz_7Dof[1][i]*=dRat;  // z
			}

			return *this;
		}
	};
	struct T_STRSPOST_D
	{
		// Change by ZINU.('04.08.26). Separate Sect Stiffness Factor from Data (Not SECF at T_STRSPOST_BASE).
		double dScaleFactorI[7];	// See T_SECF_D (Area, ..., WArea).
		double dScaleFactorJ[7];	// See T_SECF_D (Area, ..., WArea).
		T_STRSPOST_BASE StrsBaseI;
		T_STRSPOST_BASE StrsBaseJ;
		void Initialize()
		{
			int i=0;
			//cklee@midasit.com loop를 두번 반복할 필요없음 
			//for(int i = 0; i<7; i++)	dScaleFactorI[i]=1.0;	//  Use at CSectInfo::Get_StrsPost().
			//for(int i = 0; i<7; i++)	dScaleFactorJ[i]=1.0;	//  Use at CSectInfo::Get_StrsPost().
			for (int i = 0; i < 7; ++i)
			{
				dScaleFactorI[i] = 1.0;
				dScaleFactorJ[i] = 1.0;
			}
			//cklee@midasit.com T_STRSPOST_BASE 생성자에서 Initialize() 호출하고 있음.
			//별도 호출하는 경우가 있을것 같아서 일단 놔둠. 
			StrsBaseI.Initialize();
			StrsBaseJ.Initialize();
		}
	};

	// 
	struct T_STRSPROP_STIFFNESS
	{
		T_SECT_STIFFNESS Stiffness;
		double dw5, dw6, dw7, dw8, dw9, dw10, dw11, dw12, dw13, dw14, dw15, dw16;
		double dxy5, dxy6, dxy7, dxy8, dxy9, dxy10, dxy11, dxy12, dxy13, dxy14, dxy15, dxy16;
		double dxz5, dxz6, dxz7, dxz8, dxz9, dxz10, dxz11, dxz12, dxz13, dxz14, dxz15, dxz16;
		void Initialize()
		{
			Stiffness.Initialize();

#if 1
			// cklee@midasit.com 변수 Assign에 필요이상으로 CPU Cycle이 소모되고 있다. memset으로 바꿔본다.
			// 초기값이 0일때만 memset으로 설정 가능 , 0.0의 16진수 bit pattern이 00000000 00000000 이므로... 
			memset(this, 0, sizeof(T_STRSPROP_STIFFNESS));
			#else
			dw5 = dw6 = dw7 = dw8 = dw9 = dw10 = dw11 = dw12 = dw13 = dw14 = dw15 = dw16 = 0.0;
			dxy5 = dxy6 = dxy7 = dxy8 = dxy9 = dxy10 = dxy11 = dxy12 = dxy13 = dxy14 = dxy15 = dxy16 = 0.0;
			dxz5 = dxz6 = dxz7 = dxz8 = dxz9 = dxz10 = dxz11 = dxz12 = dxz13 = dxz14 = dxz15 = dxz16 = 0.0;
			#endif
		}
		T_STRSPROP_STIFFNESS() {Initialize();}
	};
	struct T_STRSPROP_BASE
	{
		double dScaleFactor[7];	// See T_SECF_D (Area, ..., WArea).
		//T_STRSPOST_BASE      StrsB;
		T_STRSPROP_STIFFNESS Stiff;
		
		void Initialize()
		{
			memset(dScaleFactor, 0, sizeof(dScaleFactor));
			//StrsB.Initialize();
			Stiff.Initialize();
		}
		T_STRSPROP_BASE() {Initialize();}
	};
	struct T_STRSPROP_D
	{
		T_STRSPROP_BASE PropB[5]; // I:0,1,2,3,J:4

 		void Initialize()
		{
			for (int i=0; i<5; ++i) PropB[i].Initialize();
		}

		T_STRSPROP_D() {Initialize();}
	};
	struct T_POSTCTRL_SLAVE
	{
		int nSlaveCode[6];

		void Initialize()
		{
			for (int i = 0; i < 6; i++) nSlaveCode[i] = 0;
		}
	};
#pragma pack()

#endif