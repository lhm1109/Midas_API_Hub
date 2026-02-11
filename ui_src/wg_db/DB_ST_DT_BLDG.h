#ifndef __DB_ST_DT_BLDG_H__
#define __DB_ST_DT_BLDG_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_BLDC_K unsigned int
#define D_BLDC_NUMBLDC  15
struct T_BLDC_BASE
{
	T_STLD_K LoadCaseKey;
	double dblFactor;
	void Initialize()
	{
		LoadCaseKey=0;
		dblFactor=0;
	}
};
struct T_BLDC_D_431
{
	BOOL bUseBaseLevel;  // TRUE=BaseLevel 사용
	double dBaseLevel;
	BOOL bBaseMass;     // TRUE=고유치해석에 포함  FALSE=불포함
};
struct T_BLDC_D_540
{
	BOOL bUseBaseLevel;  // TRUE=BaseLevel 사용
	double dBaseLevel;
	BOOL bBaseMass;     // TRUE=고유치해석에 포함  FALSE=불포함
	BOOL bStoryCenter;
	int  nUseOption;       // 0=Use Mass, 1=Use Static Loadcase
	T_STLD_K LoadCaseKey;
};
struct T_BLDC_D_570
{
	BOOL bUseBaseLevel;  // TRUE=BaseLevel 사용
	double dBaseLevel;
	BOOL bBaseMass;     // TRUE=고유치해석에 포함  FALSE=불포함
	BOOL bStoryCenter;
	int  nUseOption;       // 0=Use Mass, 1=Use Static Loadcase
	int  nNumBldcCase;     // 5.5.0 부터 추가
	T_BLDC_BASE BldcCase[D_BLDC_NUMBLDC];   // 5.5.0 부터 추가  
};
struct T_BLDC_D_633
{
	BOOL bUseBaseLevel;    // TRUE=BaseLevel 사용
	double dBaseLevel;
	BOOL bBaseMass;        // TRUE=고유치해석에 포함  FALSE=불포함
	BOOL bStoryCenter;     
	int  nUseOption;       // 0=Use Mass, 1=Use Axial Force, 2=Use Shear Force  // 580에서 의미 변경
	int  nNumBldcCase;     // 5.5.0 부터 추가
	T_BLDC_BASE BldcCase[D_BLDC_NUMBLDC];   // 5.5.0 부터 추가
	BOOL bStorShearRatio;  // 580에서 추가, Story Shear Force Ratio of Member
	T_STLD_K LoadCaseKeyX; // 580에서 추가, X-Dir Seismic Load Case
	T_STLD_K LoadCaseKeyY; // 580에서 추가, Y-Dir Seismic Load Case
};
struct T_BLDC_D_635
{
	BOOL bUseBaseLevel;    // TRUE=BaseLevel 사용
	double dBaseLevel;
	BOOL bBaseMass;        // TRUE=고유치해석에 포함  FALSE=불포함
	BOOL bStoryCenter;     
	int  nUseOption;       // 0=Use Mass, 1=Use Axial Force, 2=Use Shear Force  // 580에서 의미 변경
	int  nNumBldcCase;     // 5.5.0 부터 추가
	T_BLDC_BASE BldcCase[D_BLDC_NUMBLDC];   // 5.5.0 부터 추가
	BOOL bStorShearRatio;  // 580에서 추가, Story Shear Force Ratio of Member
	T_STLD_K LoadCaseKeyX; // 580에서 추가, X-Dir Seismic Load Case
	T_STLD_K LoadCaseKeyY; // 580에서 추가, Y-Dir Seismic Load Case
	T_STLD_K StsfStldKeyX; // 635에서 추가, X-Dir Load Case for Story Stiffness Center
	T_STLD_K StsfStldKeyY; // 635에서 추가, Y-Dir Load Case for Story Stiffness Center
};
struct T_BLDC_D_761
{
	BOOL bUseBaseLevel;    // TRUE=BaseLevel 사용
	double dBaseLevel;
	BOOL bBaseMass;        // TRUE=고유치해석에 포함  FALSE=불포함
	BOOL bStoryCenter;     // Eccentricity Ratio...
	int  nUseOption;       // 0=Use Mass, 1=Use Axial Force, 2=Use Shear Force  // 580에서 의미 변경
	int  nNumBldcCase;     // 5.5.0 부터 추가
	T_BLDC_BASE BldcCase[D_BLDC_NUMBLDC];   // 5.5.0 부터 추가
	BOOL bStorShearRatio;  // 580에서 추가, Story Shear Force Ratio of Member
	T_STLD_K LoadCaseKeyX; // 580에서 추가, X-Dir Seismic Load Case
	T_STLD_K LoadCaseKeyY; // 580에서 추가, Y-Dir Seismic Load Case
	T_STLD_K StsfStldKeyX; // 635에서 추가, X-Dir Load Case for Story Stiffness Center
	T_STLD_K StsfStldKeyY; // 635에서 추가, Y-Dir Load Case for Story Stiffness Center
	// MNET1372-HP 2005.05.30
	BOOL bStorRespThrs;		 // 691에서 추가, Default: JP에서는 TRUE, 다른 OS는 모두 FALSE
	int	 nStorCenterMethod;// 691에서 추가, 0:Story Center, 1:Stroy Average
	BOOL bConsiderStorMdul;// 691에서 추가, Default: FALSE		// MNET1516-HP 2005.06.23
};
struct T_BLDC_D_895
{
	BOOL bUseBaseLevel;    // TRUE=BaseLevel 사용
	double dBaseLevel;
	BOOL bBaseMass;        // TRUE=고유치해석에 포함  FALSE=불포함
	BOOL bStoryCenter;     // Eccentricity Ratio...
	int  nUseOption;       // 0=Use Mass, 1=Use Axial Force, 2=Use Shear Force  // 580에서 의미 변경
	int  nNumBldcCase;     // 5.5.0 부터 추가
	T_BLDC_BASE BldcCase[D_BLDC_NUMBLDC];   // 5.5.0 부터 추가
	BOOL bStorShearRatio;  // 580에서 추가, Story Shear Force Ratio of Member
	T_STLD_K LoadCaseKeyX; // 580에서 추가, X-Dir Seismic Load Case
	T_STLD_K LoadCaseKeyY; // 580에서 추가, Y-Dir Seismic Load Case
	T_STLD_K StsfStldKeyX; // 635에서 추가, X-Dir Load Case for Story Stiffness Center
	T_STLD_K StsfStldKeyY; // 635에서 추가, Y-Dir Load Case for Story Stiffness Center
	// MNET1372-HP 2005.05.30
	BOOL bStorRespThrs;		 // 691에서 추가, Default: JP에서는 TRUE, 다른 OS는 모두 FALSE
	int	 nStorCenterMethod;// 691에서 추가, 0:Story Center, 1:Stroy Average, 2:Story Drift by Maximum of Vertical Elements
	BOOL bConsiderStorMdul;// 691에서 추가, Default: FALSE		// MNET1516-HP 2005.06.23
	BOOL bConsiderWindSeisForFlex;    // 770에서 추가, Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE). Consider Wind and Seismic Loads for Flexible Floors
};
struct T_BLDC_D
{
	BOOL bUseBaseLevel;    // TRUE=BaseLevel 사용
	double dBaseLevel;
	BOOL bUseBedrockLevel;    // TRUE=BerockLevel 사용 v896 추가
	double dBedrockLevel;	//v896 추가
	BOOL bBaseMass;        // TRUE=고유치해석에 포함  FALSE=불포함
	BOOL bStoryCenter;     // Eccentricity Ratio...
	int  nUseOption;       // 0=Use Mass, 1=Use Axial Force, 2=Use Shear Force  // 580에서 의미 변경
	int  nNumBldcCase;     // 5.5.0 부터 추가
	T_BLDC_BASE BldcCase[D_BLDC_NUMBLDC];   // 5.5.0 부터 추가
	BOOL bStorShearRatio;  // 580에서 추가, Story Shear Force Ratio of Member
	T_STLD_K LoadCaseKeyX; // 580에서 추가, X-Dir Seismic Load Case
	T_STLD_K LoadCaseKeyY; // 580에서 추가, Y-Dir Seismic Load Case
	T_STLD_K StsfStldKeyX; // 635에서 추가, X-Dir Load Case for Story Stiffness Center
	T_STLD_K StsfStldKeyY; // 635에서 추가, Y-Dir Load Case for Story Stiffness Center
	// MNET1372-HP 2005.05.30
	BOOL bStorRespThrs;		 // 691에서 추가, Default: JP에서는 TRUE, 다른 OS는 모두 FALSE
	int	 nStorCenterMethod;// 691에서 추가, 0:Story Center, 1:Stroy Average, 2:Story Drift by Maximum of Vertical Elements
	BOOL bConsiderStorMdul;// 691에서 추가, Default: FALSE		// MNET1516-HP 2005.06.23
	BOOL bConsiderWindSeisForFlex;    // 770에서 추가, Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE). Consider Wind and Seismic Loads for Flexible Floors

	void Initialize()
	{
		bUseBaseLevel=FALSE;
		dBaseLevel=0.;
		bUseBedrockLevel = FALSE;
		dBedrockLevel = 0.;
		bBaseMass=TRUE;   // 03.03.04 수정 
		bStoryCenter = FALSE;
		nUseOption = 1;
		nNumBldcCase = 0;
		for(int i = 0; i < D_BLDC_NUMBLDC ; i++) BldcCase[i].Initialize();    
		bStorShearRatio = FALSE;
#if defined(_MGEN)
	//#if defined(_CH) // MNET1426-jbseon 2005.05.12
			bStorShearRatio = TRUE;   // 2003.10.30 박임구 이사님 요청
	//#endif
#endif
		LoadCaseKeyX = 0;
		LoadCaseKeyY = 0;
		StsfStldKeyX = 0;
		StsfStldKeyY = 0;
#if defined(_JP)
		#if defined(_IGEN) || defined(_MGEN)
			bStorRespThrs = FALSE;
		#endif
		bStorRespThrs = TRUE;
#else
		bStorRespThrs = FALSE;
#endif
		nStorCenterMethod        = 0;
		bConsiderStorMdul        = FALSE;
		bConsiderWindSeisForFlex = TRUE;    // Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE)

#if defined(_CH)
		bStorRespThrs = TRUE;
		nStorCenterMethod = 2;    // 0:Story Center, 1:Stroy Average, 2:Story Drift by Maximum of Vertical Elements
#endif
	}
	void Convert895(T_BLDC_D_895& rData)
	{
		Initialize();
		bUseBaseLevel = rData.bUseBaseLevel;
		dBaseLevel = rData.dBaseLevel;
		bUseBedrockLevel = FALSE;
		dBedrockLevel = 0.;
		bBaseMass = rData.bBaseMass;
		bStoryCenter = rData.bStoryCenter;
		nUseOption = rData.nUseOption;
		nNumBldcCase = rData.nNumBldcCase;
		for (int i = 0; i < D_BLDC_NUMBLDC; i++)
		{
			BldcCase[i].LoadCaseKey = rData.BldcCase[i].LoadCaseKey;
			BldcCase[i].dblFactor = rData.BldcCase[i].dblFactor;
		}
		bStorShearRatio = rData.bStorShearRatio;
		LoadCaseKeyX = rData.LoadCaseKeyX;
		LoadCaseKeyY = rData.LoadCaseKeyY;
		StsfStldKeyX = rData.StsfStldKeyX;
		StsfStldKeyY = rData.StsfStldKeyY;
		bStorRespThrs = rData.bStorRespThrs;		// JP에서 Check on 상태라도 읽어 올때는 check off로 읽어 들인다...이훈희씨와 상의...
		nStorCenterMethod = rData.nStorCenterMethod;
		bConsiderStorMdul = rData.bConsiderStorMdul;
		bConsiderWindSeisForFlex = rData.bConsiderWindSeisForFlex;    // Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE)
	}
	void Convert761(T_BLDC_D_761& rData)
	{
		Initialize();
		bUseBaseLevel = rData.bUseBaseLevel;
		dBaseLevel = rData.dBaseLevel;
		bBaseMass = rData.bBaseMass;
		bStoryCenter = rData.bStoryCenter;
		nUseOption = rData.nUseOption;
		nNumBldcCase = rData.nNumBldcCase;    
		for(int i = 0; i < D_BLDC_NUMBLDC ; i++) 
		{
			BldcCase[i].LoadCaseKey = rData.BldcCase[i].LoadCaseKey;    
			BldcCase[i].dblFactor   = rData.BldcCase[i].dblFactor;
		}
		bStorShearRatio = rData.bStorShearRatio;
		LoadCaseKeyX = rData.LoadCaseKeyX;
		LoadCaseKeyY = rData.LoadCaseKeyY;
		StsfStldKeyX = rData.StsfStldKeyX;
		StsfStldKeyY = rData.StsfStldKeyY;
		bStorRespThrs            = rData.bStorRespThrs;		// JP에서 Check on 상태라도 읽어 올때는 check off로 읽어 들인다...이훈희씨와 상의...
		nStorCenterMethod        = rData.nStorCenterMethod;
		bConsiderStorMdul        = rData.bConsiderStorMdul;
		bConsiderWindSeisForFlex = FALSE;    // Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE)
	}
	void Convert635(T_BLDC_D_635& rData)
	{
		Initialize();
		bUseBaseLevel = rData.bUseBaseLevel;
		dBaseLevel = rData.dBaseLevel;
		bBaseMass = rData.bBaseMass;
		bStoryCenter = rData.bStoryCenter;
		nUseOption = rData.nUseOption;
		nNumBldcCase = rData.nNumBldcCase;    
		for(int i = 0; i < D_BLDC_NUMBLDC ; i++) 
		{
			BldcCase[i].LoadCaseKey = rData.BldcCase[i].LoadCaseKey;    
			BldcCase[i].dblFactor   = rData.BldcCase[i].dblFactor;
		}
		bStorShearRatio = rData.bStorShearRatio;
		LoadCaseKeyX = rData.LoadCaseKeyX;
		LoadCaseKeyY = rData.LoadCaseKeyY;
		StsfStldKeyX = rData.StsfStldKeyX;
		StsfStldKeyY = rData.StsfStldKeyY;
		bStorRespThrs            = FALSE;		// JP에서 Check on 상태라도 읽어 올때는 check off로 읽어 들인다...이훈희씨와 상의...
		nStorCenterMethod        = 0;
		bConsiderStorMdul        = FALSE;
		bConsiderWindSeisForFlex = FALSE;    // Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE)
	}
	void Convert633(T_BLDC_D_633& rData)
	{
		Initialize();
		bUseBaseLevel = rData.bUseBaseLevel;
		dBaseLevel = rData.dBaseLevel;
		bBaseMass = rData.bBaseMass;
		bStoryCenter = rData.bStoryCenter;
		nUseOption = rData.nUseOption;
		nNumBldcCase = rData.nNumBldcCase;    
		for(int i = 0; i < D_BLDC_NUMBLDC ; i++) 
		{
			BldcCase[i].LoadCaseKey = rData.BldcCase[i].LoadCaseKey;    
			BldcCase[i].dblFactor   = rData.BldcCase[i].dblFactor;
		}
		bStorShearRatio = rData.bStorShearRatio;
		LoadCaseKeyX = rData.LoadCaseKeyX;
		LoadCaseKeyY = rData.LoadCaseKeyY;
		StsfStldKeyX = 0;
		StsfStldKeyY = 0;
		bStorRespThrs            = FALSE;		// JP에서 Check on 상태라도 읽어 올때는 check off로 읽어 들인다...이훈희씨와 상의...
		nStorCenterMethod        = 0;
		bConsiderStorMdul        = FALSE;
		bConsiderWindSeisForFlex = FALSE;    // Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE)
	}
	void Convert570(T_BLDC_D_570& rData)
	{
		Initialize();
		bUseBaseLevel = rData.bUseBaseLevel;
		dBaseLevel = rData.dBaseLevel;
		bBaseMass = rData.bBaseMass;
		bStoryCenter = rData.bStoryCenter;
		nUseOption = rData.nUseOption;
		nNumBldcCase = rData.nNumBldcCase;    
		for(int i = 0; i < D_BLDC_NUMBLDC ; i++) 
		{
			BldcCase[i].LoadCaseKey = rData.BldcCase[i].LoadCaseKey;    
			BldcCase[i].dblFactor   = rData.BldcCase[i].dblFactor;
		}
		bStorShearRatio = FALSE;
#if defined(_MGEN)
	#if defined(_CH)
			bStorShearRatio = TRUE;   // 2003.10.30 박임구 이사님 요청
	#endif
#endif
		LoadCaseKeyX = 0;
		LoadCaseKeyY = 0;
		StsfStldKeyX = 0;
		StsfStldKeyY = 0;
		bStorRespThrs            = FALSE;		// JP에서 Check on 상태라도 읽어 올때는 check off로 읽어 들인다...이훈희씨와 상의...
		nStorCenterMethod        = 0;
		bConsiderStorMdul        = FALSE;
		bConsiderWindSeisForFlex = FALSE;    // Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE)
	}
	void Convert540(T_BLDC_D_540& rData)
	{
		Initialize();
		bUseBaseLevel = rData.bUseBaseLevel;
		dBaseLevel = rData.dBaseLevel;
		bBaseMass = rData.bBaseMass;
		bStoryCenter = rData.bStoryCenter;
		nUseOption = rData.nUseOption;
		nNumBldcCase = 0;
		for(int i = 0; i < D_BLDC_NUMBLDC ; i++) BldcCase[i].Initialize();    
		if(nUseOption == 1)    
		{
			BldcCase[0].LoadCaseKey = rData.LoadCaseKey;
			BldcCase[0].dblFactor = 1.0;
			nNumBldcCase = 1;
		}
		bStorShearRatio = FALSE;
#if defined(_MGEN)
	#if defined(_CH)
			bStorShearRatio = TRUE;   // 2003.10.30 박임구 이사님 요청
	#endif
#endif
		LoadCaseKeyX = 0;
		LoadCaseKeyY = 0;
		StsfStldKeyX = 0;
		StsfStldKeyY = 0;
		bStorRespThrs            = FALSE;		// JP에서 Check on 상태라도 읽어 올때는 check off로 읽어 들인다...이훈희씨와 상의...
		nStorCenterMethod        = 0;
		bConsiderStorMdul        = FALSE;
		bConsiderWindSeisForFlex = FALSE;    // Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE)
	}
	void Convert431(T_BLDC_D_431& rData)
	{
		Initialize();
		bUseBaseLevel = rData.bUseBaseLevel;
		dBaseLevel = rData.dBaseLevel;
		bBaseMass = rData.bBaseMass;
		bStoryCenter = FALSE;
		nUseOption = 1;
		nNumBldcCase = 0;
		for(int i = 0; i < D_BLDC_NUMBLDC ; i++) BldcCase[i].Initialize();    
		bStorShearRatio = FALSE;
#if defined(_MGEN)
	#if defined(_CH)
			bStorShearRatio = TRUE;   // 2003.10.30 박임구 이사님 요청
	#endif
#endif
		LoadCaseKeyX = 0;
		LoadCaseKeyY = 0;
		StsfStldKeyX = 0;
		StsfStldKeyY = 0;
		bStorRespThrs            = FALSE;		// JP에서 Check on 상태라도 읽어 올때는 check off로 읽어 들인다...이훈희씨와 상의...
		nStorCenterMethod        = 0;
		bConsiderStorMdul        = FALSE;
		bConsiderWindSeisForFlex = FALSE;    // Default: TRUE. (단, 이전 파일 불러올 때는 default: FALSE)
	}
};
struct T_BLDC_UDRD_D_895
{
	T_BLDC_D_895 data;
};
struct T_BLDC_UDRD_D_761
{
	T_BLDC_D_761 data;
};
struct T_BLDC_UDRD_D_635
{
	T_BLDC_D_635 data;
};
struct T_BLDC_UDRD_D_633
{
	T_BLDC_D_633 data;
};
struct T_BLDC_UDRD_D_570
{
	T_BLDC_D_570 data;
};
struct T_BLDC_UDRD_D_540
{
	T_BLDC_D_540 data;
};
struct T_BLDC_UDRD_D_431
{
	T_BLDC_D_431 data;
};
struct T_BLDC_UDRD_D
{
	T_BLDC_D data;
};
struct T_BLDC_UNIT
{
	T_BLDC_UNIT()
	{
		bUseBaseBaseLevel=D_UNITSYS_NONE;
		dBaseLevel = D_UNITSYS_BASE_LENGTH;
		dBedrockLevel = D_UNITSYS_BASE_LENGTH;
		bBaseMass=D_UNITSYS_NONE;
	}
	int bUseBaseBaseLevel;
	int dBaseLevel;
	int dBedrockLevel;
	int bBaseMass;
};
#define HASHSIZEBLDC 1

// PMS:XXXX-HSSHIM-20140416
/// story eccentricity
#define T_SECC_K unsigned int
struct T_SECC_D
{
	BOOL bIncludeEccSeis;
	BOOL bIncludeEccWind;
	double dEccSeis;
	double dEccWind;
	T_SECC_D() { Initialize(); };
	void Initialize()
	{
		// [MQC 5013-72] 일본버전에서는 디폴트를 체크 오프로 한다. JWKWON-2015-07-22
	#if defined(_JP) 
		bIncludeEccSeis = FALSE;
		bIncludeEccWind = FALSE;
	#else
		bIncludeEccSeis = TRUE;
		bIncludeEccWind = TRUE;
	#endif
		dEccSeis = 5.0;
		dEccWind = 15.0;
	}
};
struct T_SECC_UDRD_D
{
	T_SECC_D data;
};
struct T_SECC_UNIT
{
	T_SECC_UNIT()
	{
		nDummy=D_UNITSYS_NONE;
	}
	int nDummy;
};
#define HASHSIZESECC 1

// PMS:XXXX-HSSHIM-20140414
/// Wall ID Changed Info : Wall ID 연속 정보가 변경되었는지 여부를 저장하기 위한 용도 ///
#define T_WLCI_K unsigned int
struct T_WLCI_D
{
	int nChangedInfo;   // 0: Not Changed  1: Changed By Node
	T_WLCI_D() { Initialize(); };
	void Initialize()
	{
		nChangedInfo=0;
	}
};
struct T_WLCI_UDRD_D
{
	T_WLCI_D data;
};
struct T_WLCI_UNIT
{
	T_WLCI_UNIT()
	{
		nChangedInfo=D_UNITSYS_NONE;
	}
	int nChangedInfo;
};
#define HASHSIZEWLCI 1

// PMS:4207-HSSHIM-20100729
/// Story Changed Info : 층정보가 변경되었는지 여부를 저장하기 위한 용도 ///
#define T_STCI_K unsigned int
struct T_STCI_D
{
	int nChangedInfo;   // 0: Not Changed  1: Changed By Node
	T_STCI_D() { Initialize(); };
	void Initialize()
	{
		nChangedInfo=0;
	}
};
struct T_STCI_UDRD_D
{
	T_STCI_D data;
};
struct T_STCI_UNIT
{
	T_STCI_UNIT()
	{
		nChangedInfo=D_UNITSYS_NONE;
	}
	int nChangedInfo;
};
#define HASHSIZESTCI 1

/// Story ///
#define T_STOR_K unsigned int
struct T_STOR_PONT
{
	double dX,dY,dZ;

	void Initialize()
	{
		dX=dY=dZ=0.0;
	}

};
struct T_STOR_D
{
	CString StoryName;
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 추가
	double WindEccentY;   // 8.0.0 추가
	double dWindwardCoefX;     // Windward pressure coefficient-X //8.1.1 maxiao@midasit.com
	double dLeewardCoefX;      // Leeward pressure coefficient-X //8.1.1
	double dWindwardCoefY;     // Windward pressure coefficient-Y //8.1.1
	double dLeewardCoefY;      // Leeward pressure coefficient-Y //8.1.1
	double dShieldCoefX; //8.1.1
	double dShieldCoefY; //8.1.1
	double dForceCoefX ;
	double dForceCoefY ;
	double dGustFactorX;
	double dGustFactorY;
	CArray<T_STOR_PONT,T_STOR_PONT&> aStoryArea;  // 8.8.1 추가(16개의 좌표에서 Array로 개선)
//   double aAreaPosX[16]; // 8.3.1 추가 (Story 영역)
//   double aAreaPosY[16]; // 8.3.1 추가 (Story 영역)
//   double aAreaPosZ[16]; // 8.3.1 추가 (Story 영역)

	UINT TdasK;

	void Initialize()
	{
		StoryName.Empty();
		dStoryLevel          =0.;
		bFloorDiaphragm      =TRUE;
		WindWidthX           =0.;
		WindWidthY           =0.;
		WindCenterX          =0.;
		WindCenterY          =0.;
		SeisEccentX          =0.;
		SeisEccentY          =0.;
		dInherentEccenX      =0.;
		dInherentEccenY      =0.;
		dTorsionalAmpFactorX =1.;
		dTorsionalAmpFactorY =1.;
		WindEccentX          =0.;
		WindEccentY          =0.;
		dWindwardCoefX       = 0.8;
		dWindwardCoefY       = 0.8;
		dLeewardCoefX        = -0.5;
		dLeewardCoefY        = -0.5;
		dShieldCoefX         = 1.0;
		dShieldCoefY         = 1.0;
		dForceCoefX          = 1.0;
		dForceCoefY          = 1.0;
		dGustFactorX         = 1.0;
		dGustFactorY         = 1.0;
		aStoryArea.RemoveAll();
		TdasK = 0;
	}
	T_STOR_D()  {Initialize(); }
	T_STOR_D(const T_STOR_D& src) { *this = src; }
	T_STOR_D& operator=(const T_STOR_D& src)
	{
		StoryName             = src.StoryName           ;
		dStoryLevel           = src.dStoryLevel         ;    
		bFloorDiaphragm       = src.bFloorDiaphragm     ;
		WindWidthX            = src.WindWidthX          ;     
		WindWidthY            = src.WindWidthY          ;     
		WindCenterX           = src.WindCenterX         ;    
		WindCenterY           = src.WindCenterY         ;    
		SeisEccentX           = src.SeisEccentX         ;    
		SeisEccentY           = src.SeisEccentY         ;   
		dInherentEccenX       = src.dInherentEccenX     ;
		dInherentEccenY       = src.dInherentEccenY     ;     
		dTorsionalAmpFactorX  = src.dTorsionalAmpFactorX;     
		dTorsionalAmpFactorY  = src.dTorsionalAmpFactorY;    
		WindEccentX           = src.WindEccentX         ;    
		WindEccentY           = src.WindEccentY         ; 
		dWindwardCoefX        = src.dWindwardCoefX      ;   
		dWindwardCoefY        = src.dWindwardCoefY      ;
		dLeewardCoefX         = src.dLeewardCoefX       ;     
		dLeewardCoefY         = src.dLeewardCoefY       ;     
		dShieldCoefX          = src.dShieldCoefX        ;    
		dShieldCoefY          = src.dShieldCoefY        ;   
		dForceCoefX           = src.dForceCoefX         ;
		dForceCoefY	          = src.dForceCoefY         ;
		dGustFactorX          = src.dGustFactorX        ;
		dGustFactorY          = src.dGustFactorY        ;
		aStoryArea.Copy(src.aStoryArea);
		TdasK				  = src.TdasK;
		return *this;
	}
};

struct T_STOR_D_995
{
	CString StoryName;
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 추가
	double WindEccentY;   // 8.0.0 추가
	double dWindwardCoefX;     // Windward pressure coefficient-X //8.1.1 maxiao@midasit.com
	double dLeewardCoefX;      // Leeward pressure coefficient-X //8.1.1
	double dWindwardCoefY;     // Windward pressure coefficient-Y //8.1.1
	double dLeewardCoefY;      // Leeward pressure coefficient-Y //8.1.1
	double dShieldCoefX; //8.1.1
	double dShieldCoefY; //8.1.1
	double dForceCoefX;
	double dForceCoefY;
	double dGustFactorX;
	double dGustFactorY;
	CArray<T_STOR_PONT, T_STOR_PONT&> aStoryArea;  // 8.8.1 추가(16개의 좌표에서 Array로 개선)
	//   double aAreaPosX[16]; // 8.3.1 추가 (Story 영역)
	//   double aAreaPosY[16]; // 8.3.1 추가 (Story 영역)
	//   double aAreaPosZ[16]; // 8.3.1 추가 (Story 영역)

	UINT TdasK = 0;			// NX

	void Initialize()
	{
		StoryName.Empty();
		dStoryLevel = 0.;
		bFloorDiaphragm = TRUE;
		WindWidthX = 0.;
		WindWidthY = 0.;
		WindCenterX = 0.;
		WindCenterY = 0.;
		SeisEccentX = 0.;
		SeisEccentY = 0.;
		dInherentEccenX = 0.;
		dInherentEccenY = 0.;
		dTorsionalAmpFactorX = 1.;
		dTorsionalAmpFactorY = 1.;
		WindEccentX = 0.;
		WindEccentY = 0.;
		dWindwardCoefX = 0.8;
		dWindwardCoefY = 0.8;
		dLeewardCoefX = -0.5;
		dLeewardCoefY = -0.5;
		dShieldCoefX = 1.0;
		dShieldCoefY = 1.0;
		dForceCoefX = 1.0;
		dForceCoefY = 1.0;
		dGustFactorX = 1.0;
		dGustFactorY = 1.0;
		aStoryArea.RemoveAll();
		TdasK = 0;
	}
};
struct T_STOR_D_CH_671
{
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dForceCoefX;
	double dForceCoefY;
	double dGustFactorX;
	double dGustFactorY;
};
struct T_STOR_D_CH_796
{
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
};
struct T_STOR_D_CH_811
{
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 added
	double WindEccentY;   // 8.0.0 added
};
struct T_STOR_D_CH_831
{
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 추가
	double WindEccentY;   // 8.0.0 추가
	double dWindwardCoefX;     //8.1.1 // Windward pressure coefficient-X // maxiao@midasit.com
	double dLeewardCoefX;      //8.1.1 // Leeward pressure coefficient-X  
	double dWindwardCoefY;     //8.1.1 // Windward pressure coefficient-Y 
	double dLeewardCoefY;      //8.1.1 // Leeward pressure coefficient-Y  
	double dShieldCoefX;       //8.1.1
	double dShieldCoefY;       //8.1.1
};
struct T_STOR_D_CH_880
{
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 추가
	double WindEccentY;   // 8.0.0 추가
	double dWindwardCoefX;     //8.1.1 // Windward pressure coefficient-X // maxiao@midasit.com
	double dLeewardCoefX;      //8.1.1 // Leeward pressure coefficient-X  
	double dWindwardCoefY;     //8.1.1 // Windward pressure coefficient-Y 
	double dLeewardCoefY;      //8.1.1 // Leeward pressure coefficient-Y  
	double dShieldCoefX;       //8.1.1
	double dShieldCoefY;       //8.1.1
	int nAreaPosNum;
	double aAreaPosX[16];
	double aAreaPosY[16];
	double aAreaPosZ[16];
};

struct T_STOR_D_CH
{
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 추가
	double WindEccentY;   // 8.0.0 추가
	double dWindwardCoefX;     //8.1.1 // Windward pressure coefficient-X // maxiao@midasit.com
	double dLeewardCoefX;      //8.1.1 // Leeward pressure coefficient-X  
	double dWindwardCoefY;     //8.1.1 // Windward pressure coefficient-Y 
	double dLeewardCoefY;      //8.1.1 // Leeward pressure coefficient-Y  
	double dShieldCoefX;       //8.1.1
	double dShieldCoefY;       //8.1.1
	double dForceCoefX ;
	double dForceCoefY ;
	double dGustFactorX;
	double dGustFactorY;
	CArray<T_STOR_PONT,T_STOR_PONT&> aStoryArea;  // 8.8.1 추가(16개의 좌표에서 Array로 개선)
	UINT TdasK;
//   double aAreaPosX[16];
//   double aAreaPosY[16];
//   double aAreaPosZ[16];
	void Convert671(T_STOR_D_CH_671& rData)
	{
		memcpy(StoryName, rData.StoryName, sizeof(StoryName));
		dStoryLevel = rData.dStoryLevel;
		bFloorDiaphragm = rData.bFloorDiaphragm;
		WindWidthX = rData.WindWidthX;
		WindWidthY = rData.WindWidthY;
		WindCenterX = rData.WindCenterX;
		WindCenterY = rData.WindCenterY;
		SeisEccentX = rData.SeisEccentX;
		SeisEccentY = rData.SeisEccentY;
		dInherentEccenX = 0.0;
		dInherentEccenY = 0.0;
		dTorsionalAmpFactorX = 1.0;
		dTorsionalAmpFactorY = 1.0;
		WindEccentX=0.;
		WindEccentY=0.;
		dWindwardCoefX = 0.8;
		dWindwardCoefY = 0.8;
		dLeewardCoefX  = -0.5;
		dLeewardCoefY  = -0.5;
		dShieldCoefX   = 1.0;
		dShieldCoefY   = 1.0;
		dForceCoefX    = 1.0;
		dForceCoefY    = 1.0;
		dGustFactorX   = 1.0;
		dGustFactorY   = 1.0;
		aStoryArea.RemoveAll();
		TdasK = 0;
	}
	void Convert796(T_STOR_D_CH_796& rData)
	{
		memcpy(StoryName, rData.StoryName, sizeof(StoryName));
		dStoryLevel = rData.dStoryLevel;
		bFloorDiaphragm = rData.bFloorDiaphragm;
		WindWidthX = rData.WindWidthX;
		WindWidthY = rData.WindWidthY;
		WindCenterX = rData.WindCenterX;
		WindCenterY = rData.WindCenterY;
		SeisEccentX = rData.SeisEccentX;
		SeisEccentY = rData.SeisEccentY;
		dInherentEccenX = rData.dInherentEccenX;
		dInherentEccenY = rData.dInherentEccenY;
		dTorsionalAmpFactorX = rData.dTorsionalAmpFactorX;
		dTorsionalAmpFactorY = rData.dTorsionalAmpFactorY;
		WindEccentX=0.;
		WindEccentY=0.;
		dWindwardCoefX = 0.8;
		dWindwardCoefY = 0.8;
		dLeewardCoefX  = -0.5;
		dLeewardCoefY  = -0.5;
		dShieldCoefX   = 1.0;
		dShieldCoefY   = 1.0;
		aStoryArea.RemoveAll();
		TdasK = 0;
	}
	void Convert811(T_STOR_D_CH_811& rData)
	{
		memcpy(StoryName, rData.StoryName, sizeof(StoryName));
		dStoryLevel = rData.dStoryLevel;
		bFloorDiaphragm = rData.bFloorDiaphragm;
		WindWidthX = rData.WindWidthX;
		WindWidthY = rData.WindWidthY;
		WindCenterX = rData.WindCenterX;
		WindCenterY = rData.WindCenterY;
		SeisEccentX = rData.SeisEccentX;
		SeisEccentY = rData.SeisEccentY;
		dInherentEccenX = rData.dInherentEccenX;
		dInherentEccenY = rData.dInherentEccenY;
		dTorsionalAmpFactorX = rData.dTorsionalAmpFactorX;
		dTorsionalAmpFactorY = rData.dTorsionalAmpFactorY;
		WindEccentX = rData.WindEccentX;
		WindEccentY = rData.WindEccentY;
		dWindwardCoefX = 0.8;
		dWindwardCoefY = 0.8;
		dLeewardCoefX  = -0.5;
		dLeewardCoefY  = -0.5;
		dShieldCoefX   = 1.0;
		dShieldCoefY   = 1.0;
		aStoryArea.RemoveAll();
		TdasK = 0;
	}
	void Convert831(T_STOR_D_CH_831& rData)
	{
		memcpy(StoryName, rData.StoryName, sizeof(StoryName));
		dStoryLevel = rData.dStoryLevel;
		bFloorDiaphragm = rData.bFloorDiaphragm;
		WindWidthX = rData.WindWidthX;
		WindWidthY = rData.WindWidthY;
		WindCenterX = rData.WindCenterX;
		WindCenterY = rData.WindCenterY;
		SeisEccentX = rData.SeisEccentX;
		SeisEccentY = rData.SeisEccentY;
		dInherentEccenX = rData.dInherentEccenX;
		dInherentEccenY = rData.dInherentEccenY;
		dTorsionalAmpFactorX = rData.dTorsionalAmpFactorX;
		dTorsionalAmpFactorY = rData.dTorsionalAmpFactorY;
		WindEccentX = rData.WindEccentX;
		WindEccentY = rData.WindEccentY;
		dWindwardCoefX = rData.dWindwardCoefX ;
		dWindwardCoefY = rData.dWindwardCoefY ;
		dLeewardCoefX  = rData.dLeewardCoefX  ;
		dLeewardCoefY  = rData.dLeewardCoefY  ;
		dShieldCoefX   = rData.dShieldCoefX   ;
		dShieldCoefY   = rData.dShieldCoefY   ;
		aStoryArea.RemoveAll();
		TdasK = 0;
	}
	void Convert880(T_STOR_D_CH_880& rData)
	{
		memcpy(StoryName, rData.StoryName, sizeof(StoryName));
		dStoryLevel = rData.dStoryLevel;
		bFloorDiaphragm = rData.bFloorDiaphragm;
		WindWidthX = rData.WindWidthX;
		WindWidthY = rData.WindWidthY;
		WindCenterX = rData.WindCenterX;
		WindCenterY = rData.WindCenterY;
		SeisEccentX = rData.SeisEccentX;
		SeisEccentY = rData.SeisEccentY;
		dInherentEccenX = rData.dInherentEccenX;
		dInherentEccenY = rData.dInherentEccenY;
		dTorsionalAmpFactorX = rData.dTorsionalAmpFactorX;
		dTorsionalAmpFactorY = rData.dTorsionalAmpFactorY;
		WindEccentX = rData.WindEccentX;
		WindEccentY = rData.WindEccentY;
		dWindwardCoefX = rData.dWindwardCoefX ;
		dWindwardCoefY = rData.dWindwardCoefY ;
		dLeewardCoefX  = rData.dLeewardCoefX  ;
		dLeewardCoefY  = rData.dLeewardCoefY  ;
		dShieldCoefX   = rData.dShieldCoefX   ;
		dShieldCoefY   = rData.dShieldCoefY   ;
		aStoryArea.RemoveAll();
		for (int i=0; i<rData.nAreaPosNum; i++)
		{
			T_STOR_PONT StorPont;
			StorPont.dX = rData.aAreaPosX[i];
			StorPont.dY = rData.aAreaPosY[i];
			StorPont.dZ = rData.aAreaPosZ[i];
			aStoryArea.Add(StorPont);
		}
		TdasK = 0;
		dForceCoefX = 1.0;
		dForceCoefY = 1.0;
		dGustFactorX = 1.0;
		dGustFactorY = 1.0;
	}
};
struct T_STOR_UDRD_D
{
	T_STOR_K key;
	T_STOR_D data;
};
struct T_STOR_UDRD_D_CH_831
{
	T_STOR_K key;
	T_STOR_D_CH_831 data;
};
struct T_STOR_UDRD_D_CH_811
{
	T_STOR_K key;
	T_STOR_D_CH_811 data;
};
struct T_STOR_UDRD_D_CH_796
{
	T_STOR_K key;
	T_STOR_D_CH_796 data;
};
struct T_STOR_UDRD_D_CH_671
{
	T_STOR_K key;
	T_STOR_D_CH_671 data;
};
struct T_STOR_UDRD_D_CH_880
{
	T_STOR_K key;
	T_STOR_D_CH_880 data;
};

struct T_STOR_UDRD_D_CH
{
	T_STOR_K key;
	T_STOR_D_CH data;
};
struct T_STOR_UDRD_D_CH_RW_950
{
	T_STOR_K key;
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 추가
	double WindEccentY;   // 8.0.0 추가
	double dWindwardCoefX;     //8.1.1 // Windward pressure coefficient-X // maxiao@midasit.com
	double dLeewardCoefX;      //8.1.1 // Leeward pressure coefficient-X  
	double dWindwardCoefY;     //8.1.1 // Windward pressure coefficient-Y 
	double dLeewardCoefY;      //8.1.1 // Leeward pressure coefficient-Y  
	double dShieldCoefX;       //8.1.1
	double dShieldCoefY;       //8.1.1
	int nAreaPosNum;
	void GetStor950(T_STOR_K& rKey, T_STOR_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.StoryName, StoryName, sizeof(StoryName));
		rData.dStoryLevel = dStoryLevel;
		rData.bFloorDiaphragm = bFloorDiaphragm;
		rData.WindWidthX = WindWidthX;
		rData.WindWidthY = WindWidthY;
		rData.WindCenterX = WindCenterX;
		rData.WindCenterY = WindCenterY;
		rData.SeisEccentX = SeisEccentX;
		rData.SeisEccentY = SeisEccentY;
		rData.dInherentEccenX = dInherentEccenX;
		rData.dInherentEccenY = dInherentEccenY;
		rData.dTorsionalAmpFactorX = dTorsionalAmpFactorX;
		rData.dTorsionalAmpFactorY = dTorsionalAmpFactorY;
		rData.WindEccentX = WindEccentX;
		rData.WindEccentY = WindEccentY;
		rData.dWindwardCoefX = dWindwardCoefX;
		rData.dWindwardCoefY = dWindwardCoefY;
		rData.dLeewardCoefX = dLeewardCoefX;
		rData.dLeewardCoefY = dLeewardCoefY;
		rData.dShieldCoefX = dShieldCoefX;
		rData.dShieldCoefY = dShieldCoefY;
		rData.TdasK = 0;
		rData.dForceCoefX = 1.0;
		rData.dForceCoefY = 1.0;
		rData.dGustFactorX = 1.0;
		rData.dGustFactorY = 1.0;
	}
};

struct T_STOR_UDRD_D_CH_RW_965
{
	T_STOR_K key;
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 추가
	double WindEccentY;   // 8.0.0 추가
	double dWindwardCoefX;     //8.1.1 // Windward pressure coefficient-X // maxiao@midasit.com
	double dLeewardCoefX;      //8.1.1 // Leeward pressure coefficient-X  
	double dWindwardCoefY;     //8.1.1 // Windward pressure coefficient-Y 
	double dLeewardCoefY;      //8.1.1 // Leeward pressure coefficient-Y  
	double dShieldCoefX;       //8.1.1
	double dShieldCoefY;       //8.1.1
	int nAreaPosNum;
	UINT TdasK;
	void GetStor965(T_STOR_K& rKey, T_STOR_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.StoryName, StoryName, sizeof(StoryName));
		rData.dStoryLevel = dStoryLevel;
		rData.bFloorDiaphragm = bFloorDiaphragm;
		rData.WindWidthX = WindWidthX;
		rData.WindWidthY = WindWidthY;
		rData.WindCenterX = WindCenterX;
		rData.WindCenterY = WindCenterY;
		rData.SeisEccentX = SeisEccentX;
		rData.SeisEccentY = SeisEccentY;
		rData.dInherentEccenX = dInherentEccenX;
		rData.dInherentEccenY = dInherentEccenY;
		rData.dTorsionalAmpFactorX = dTorsionalAmpFactorX;
		rData.dTorsionalAmpFactorY = dTorsionalAmpFactorY;
		rData.WindEccentX = WindEccentX;
		rData.WindEccentY = WindEccentY;
		rData.dWindwardCoefX = dWindwardCoefX;
		rData.dWindwardCoefY = dWindwardCoefY;
		rData.dLeewardCoefX = dLeewardCoefX;
		rData.dLeewardCoefY = dLeewardCoefY;
		rData.dShieldCoefX = dShieldCoefX;
		rData.dShieldCoefY = dShieldCoefY;
		rData.TdasK = TdasK;
		rData.dForceCoefX = 1.0;
		rData.dForceCoefY = 1.0;
		rData.dGustFactorX = 1.0;
		rData.dGustFactorY = 1.0;
	}
};

struct T_STOR_UDRD_D_CH_RW
{
	T_STOR_K key;
	char StoryName[20];
	double dStoryLevel;
	BOOL bFloorDiaphragm;
	double WindWidthX;
	double WindWidthY;
	double WindCenterX;
	double WindCenterY;
	double SeisEccentX;
	double SeisEccentY;
	double dInherentEccenX;        // 6.8.0 추가
	double dInherentEccenY;        // 6.8.0 추가
	double dTorsionalAmpFactorX;   // 6.8.0 추가
	double dTorsionalAmpFactorY;   // 6.8.0 추가
	double WindEccentX;   // 8.0.0 추가
	double WindEccentY;   // 8.0.0 추가
	double dWindwardCoefX;     //8.1.1 // Windward pressure coefficient-X // maxiao@midasit.com
	double dLeewardCoefX;      //8.1.1 // Leeward pressure coefficient-X  
	double dWindwardCoefY;     //8.1.1 // Windward pressure coefficient-Y 
	double dLeewardCoefY;      //8.1.1 // Leeward pressure coefficient-Y  
	double dShieldCoefX;       //8.1.1
	double dShieldCoefY;       //8.1.1
	int nAreaPosNum;
	UINT TdasK;
	double dForceCoefX;
	double dForceCoefY;
	double dGustFactorX;
	double dGustFactorY;
	void GetStor(T_STOR_K& rKey, T_STOR_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.StoryName, StoryName, sizeof(StoryName));
		rData.dStoryLevel     = dStoryLevel;
		rData.bFloorDiaphragm = bFloorDiaphragm;
		rData.WindWidthX      = WindWidthX;
		rData.WindWidthY      = WindWidthY;
		rData.WindCenterX     = WindCenterX;
		rData.WindCenterY     = WindCenterY;
		rData.SeisEccentX     = SeisEccentX;
		rData.SeisEccentY     = SeisEccentY;
		rData.dInherentEccenX = dInherentEccenX;
		rData.dInherentEccenY = dInherentEccenY;
		rData.dTorsionalAmpFactorX = dTorsionalAmpFactorX;
		rData.dTorsionalAmpFactorY = dTorsionalAmpFactorY;
		rData.WindEccentX          = WindEccentX;
		rData.WindEccentY          = WindEccentY;
		rData.dWindwardCoefX       = dWindwardCoefX ;
		rData.dWindwardCoefY       = dWindwardCoefY ;
		rData.dLeewardCoefX        = dLeewardCoefX  ;
		rData.dLeewardCoefY        = dLeewardCoefY  ;
		rData.dShieldCoefX         = dShieldCoefX   ;
		rData.dShieldCoefY         = dShieldCoefY   ;
		rData.TdasK				   = TdasK;
		rData.dForceCoefX          = dForceCoefX;
		rData.dForceCoefY          = dForceCoefY;
		rData.dGustFactorX         = dGustFactorX;
		rData.dGustFactorY         = dGustFactorY;
	}
	void SetStor(const T_STOR_K Key, const T_STOR_D_CH& rData)
	{
		key=Key;
		memcpy(StoryName, rData.StoryName, sizeof(StoryName));
		dStoryLevel     = rData.dStoryLevel;
		bFloorDiaphragm = rData.bFloorDiaphragm;
		WindWidthX      = rData.WindWidthX;
		WindWidthY      = rData.WindWidthY;
		WindCenterX     = rData.WindCenterX;
		WindCenterY     = rData.WindCenterY;
		SeisEccentX     = rData.SeisEccentX;
		SeisEccentY     = rData.SeisEccentY;
		dInherentEccenX = rData.dInherentEccenX;
		dInherentEccenY = rData.dInherentEccenY;
		dTorsionalAmpFactorX = rData.dTorsionalAmpFactorX;
		dTorsionalAmpFactorY = rData.dTorsionalAmpFactorY;
		WindEccentX          = rData.WindEccentX;
		WindEccentY          = rData.WindEccentY;
		dWindwardCoefX       = rData.dWindwardCoefX ;
		dWindwardCoefY       = rData.dWindwardCoefY ;
		dLeewardCoefX        = rData.dLeewardCoefX  ;
		dLeewardCoefY        = rData.dLeewardCoefY  ;
		dShieldCoefX         = rData.dShieldCoefX   ;
		dShieldCoefY         = rData.dShieldCoefY   ;
		nAreaPosNum          = (int)rData.aStoryArea.GetSize();
		TdasK				 = rData.TdasK;	
		dForceCoefX          = rData.dForceCoefX;
		dForceCoefY          = rData.dForceCoefY;
		dGustFactorX         = rData.dGustFactorX;
		dGustFactorY         = rData.dGustFactorY;
	}
};
struct T_STOR_UNIT
{
	T_STOR_UNIT()
	{
		StoryName=D_UNITSYS_NONE;
		dStoryLevel=D_UNITSYS_BASE_LENGTH;
		bFloorDiaphragm=D_UNITSYS_NONE;
		WindWidthX=D_UNITSYS_BASE_LENGTH;
		WindWidthY=D_UNITSYS_BASE_LENGTH;
		WindCenterX=D_UNITSYS_BASE_LENGTH;
		WindCenterY=D_UNITSYS_BASE_LENGTH;
		SeisEccentX=D_UNITSYS_BASE_LENGTH;
		SeisEccentY=D_UNITSYS_BASE_LENGTH;
		dInherentEccenX=D_UNITSYS_BASE_LENGTH;
		dInherentEccenY=D_UNITSYS_BASE_LENGTH;
		dTorsionalAmpFactorX=D_UNITSYS_NONE;
		dTorsionalAmpFactorY=D_UNITSYS_NONE;
		WindEccentX=D_UNITSYS_BASE_LENGTH;
		WindEccentY=D_UNITSYS_BASE_LENGTH;
		dWindwardCoefX = D_UNITSYS_NONE;
		dWindwardCoefY = D_UNITSYS_NONE;
		dLeewardCoefX = D_UNITSYS_NONE;
		dLeewardCoefY = D_UNITSYS_NONE;
		dShieldCoefX = D_UNITSYS_NONE;
		dShieldCoefY = D_UNITSYS_NONE;
		dForceCoefX =  D_UNITSYS_NONE;
		dForceCoefY =  D_UNITSYS_NONE;
		dGustFactorX = D_UNITSYS_NONE;
		dGustFactorY = D_UNITSYS_NONE;
		aStoryArea = D_UNITSYS_BASE_LENGTH;
	}
	int StoryName;
	int dStoryLevel;
	int bFloorDiaphragm;
	int WindWidthX;
	int WindWidthY;
	int WindCenterX;
	int WindCenterY;
	int SeisEccentX;
	int SeisEccentY;
	int dInherentEccenX;
	int dInherentEccenY;
	int dTorsionalAmpFactorX;
	int dTorsionalAmpFactorY;
	int WindEccentX;
	int WindEccentY;
	int	dWindwardCoefX;
	int dWindwardCoefY;
	int	dLeewardCoefX;
	int dLeewardCoefY;
	int	dShieldCoefX;
	int dShieldCoefY;
	int dForceCoefX  ;
	int dForceCoefY  ;
	int dGustFactorX ;
	int dGustFactorY ;
	int aStoryArea;
};
#define HASHSIZESTOR 1001
#define D_STOR_MINNUM       1       // 입력가능한 최소 층번호
#define D_STOR_MAXNUM     999       // 입력가능한 최대 층번호


// Module(Old)
#define T_MDUL_K unsigned int
#define T_MDUL_DT CArray<T_STOR_K, T_STOR_K>
struct T_MDUL_D
{
	CString		strName;
	T_MDUL_DT aStorK;

	void Initialize()
	{
		strName.Empty();
		aStorK.RemoveAll();
	}
	T_MDUL_D()
	{
	}
	T_MDUL_D(T_MDUL_D& src)
	{
		*this = src;
	}
	T_MDUL_D& operator=(T_MDUL_D& src)
	{
		strName = src.strName;
		aStorK.Copy(src.aStorK);
		return *this;
	}
};
struct T_MDUL_D_CH
{
	char			strName[40];
	T_MDUL_DT aStorK;
	void ConvertToString(T_MDUL_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.aStorK.Copy(aStorK);
	}
	void ConvertToChar(T_MDUL_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		aStorK.Copy(rData.aStorK);
	}
};
struct T_MDUL_UDRD_D
{
	T_MDUL_K key;
	T_MDUL_D data;
};
struct T_MDUL_UDRD_D_CH
{
	T_MDUL_K key;
	T_MDUL_D_CH data;
};
struct T_MDUL_UDRD_D_CH_RW
{
	T_MDUL_K			key;
	char					strName[40];
	unsigned int	nCountStorK;
	void GetMdul(T_MDUL_K& rKey, T_MDUL_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));
	}
	void SetMdul(const T_MDUL_K rKey, const T_MDUL_D_CH& rData)
	{
		key = rKey;
		memcpy(strName, rData.strName, sizeof(strName));
		nCountStorK = (int)rData.aStorK.GetSize();
	}
};
struct T_MDUL_UNIT
{
	T_MDUL_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEMDUL 11

// Module(New)
#define T_MODL_K unsigned int
struct T_MODL_BASE
{
	CString csModuleName;
	CArray<T_STOR_K, T_STOR_K> aStoryK;
	T_STOR_K StorK_start;   // module 아래층/위층 검사용
	T_STOR_K StorK_end;     // module 아래층/위층 검사용
	T_MODL_BASE() {}
	T_MODL_BASE(const T_MODL_BASE& crSrc) { *this = crSrc; }
	T_MODL_BASE& operator=(const T_MODL_BASE& crData)
	{
		csModuleName = crData.csModuleName;
		aStoryK.Copy(crData.aStoryK);
		StorK_start = crData.StorK_start;
		StorK_end = crData.StorK_end;
		return *this;
	}
	void Initialize()
	{
		csModuleName.Empty();
		aStoryK.RemoveAll();
		StorK_start = 0;
		StorK_end = 0;
	}
};
#define T_MODL_DT CArray<T_MODL_BASE, T_MODL_BASE&>
struct T_MODL_D
{
	T_MODL_DT aModule;
	void Initialize()
	{
		aModule.RemoveAll();
	}
	T_MODL_D()
	{
	}
	T_MODL_D(const T_MODL_D& crSrc)
	{
		*this = crSrc;
	}
	T_MODL_D& operator=(const T_MODL_D& crData)
	{
		aModule.Copy(crData.aModule);
		return *this;
	}
};

struct T_MODL_D_TEMP  // 중간 포맷
{
	CArray<int, int> aModuleNameCharNum;  // Module 갯수 만큼
	CArray<char,char> aModuleNameChar;
	CArray<int, int> aStoryNum; // Module별 Story 갯수
	CArray<T_STOR_K,T_STOR_K> aStoryK; // 전체 모듈의 Story Key
	CArray<T_STOR_K,T_STOR_K> aStorK_start;  // Module별 StorK_start
	CArray<T_STOR_K,T_STOR_K> aStorK_end;    // Module별 StorK_end
	void GetModl(T_MODL_D& rData)
	{
		int ix1 = 0, ix2 = 0;
		int nModuleNameCharNum;
		int nStoryNum;
		int nModule = (int)aModuleNameCharNum.GetSize();
		rData.aModule.SetSize(nModule);
		for (int i = 0; i < nModule; i++)
		{
			nModuleNameCharNum = aModuleNameCharNum[i];
			CStringA csModuleName_mbcs;
			csModuleName_mbcs.GetBufferSetLength(nModuleNameCharNum);
			int j;  
			for (j = 0; j < nModuleNameCharNum; j++)
				csModuleName_mbcs.SetAt(j, aModuleNameChar[ix1++]);
			rData.aModule[i].csModuleName = csModuleName_mbcs;
			nStoryNum = aStoryNum[i];
			rData.aModule[i].aStoryK.SetSize(nStoryNum);
			for (j = 0; j < nStoryNum; j++)
				rData.aModule[i].aStoryK[j] = aStoryK[ix2++];

			if (aStorK_start.GetSize() > 0)  rData.aModule[i].StorK_start = aStorK_start[i];
			if (aStorK_end.GetSize() > 0)  rData.aModule[i].StorK_end   = aStorK_end[i];
		}
	}
	void SetModl(const T_MODL_D& rData)
	{
		int nModuleNameCharNum;
		int nStoryNum;
		int nCountModule = (int)rData.aModule.GetSize();
		aStorK_start.SetSize(nCountModule);
		aStorK_end.SetSize(nCountModule);
		for (int i = 0; i < rData.aModule.GetSize(); i++)
		{
			CStringA astrModuleName(rData.aModule[i].csModuleName);
			nModuleNameCharNum = astrModuleName.GetLength();
			aModuleNameCharNum.Add(nModuleNameCharNum);

			int j;
			//CStringA astrModuleName(rData.aModule[i].csModuleName);
			for (j = 0; j < nModuleNameCharNum; j++)
				aModuleNameChar.Add(astrModuleName.GetAt(j));
			nStoryNum = (int)rData.aModule[i].aStoryK.GetSize();
			aStoryNum.Add(nStoryNum);
			for (j = 0; j < nStoryNum; j++)
				aStoryK.Add(rData.aModule[i].aStoryK[j]);

			aStorK_start[i] = rData.aModule[i].StorK_start;
			aStorK_end[i] = rData.aModule[i].StorK_end;
		}
	}
};

struct T_MODL_D_RW_831
{
	int nCount_aModuleNameCharNum;
	int nCount_aModuleNameChar;
	int nCount_aStoryNum;
	int nCount_aStoryK;
	void GetModlTemp(T_MODL_D_TEMP& rData)
	{
		rData.aModuleNameCharNum.SetSize(nCount_aModuleNameCharNum);
		rData.aModuleNameChar.SetSize(nCount_aModuleNameChar);
		rData.aStoryNum.SetSize(nCount_aStoryNum);
		rData.aStoryK.SetSize(nCount_aStoryK);
		rData.aStorK_start.SetSize(0);
		rData.aStorK_end.SetSize(0);
	}
	void SetModlTemp(const T_MODL_D_TEMP& rData)
	{
		nCount_aModuleNameCharNum = (int)rData.aModuleNameCharNum.GetSize();
		nCount_aModuleNameChar    = (int)rData.aModuleNameChar.GetSize();
		nCount_aStoryNum          = (int)rData.aStoryNum.GetSize();
		nCount_aStoryK            = (int)rData.aStoryK.GetSize();
		//nCount_aStorK_start = rData.aStorK_start.GetSize();
		//nCount_aStorK_end = rData.aStorK_end.GetSize();
	}
};

struct T_MODL_D_RW
{
	int nCount_aModuleNameCharNum;
	int nCount_aModuleNameChar;
	int nCount_aStoryNum;
	int nCount_aStoryK;
	int nCount_aStorK_start;
	int nCount_aStorK_end;
	void GetModlTemp(T_MODL_D_TEMP& rData)
	{
		rData.aModuleNameCharNum.SetSize(nCount_aModuleNameCharNum);
		rData.aModuleNameChar.SetSize(nCount_aModuleNameChar);
		rData.aStoryNum.SetSize(nCount_aStoryNum);
		rData.aStoryK.SetSize(nCount_aStoryK);
		rData.aStorK_start.SetSize(nCount_aStorK_start);
		rData.aStorK_end.SetSize(nCount_aStorK_end);
	}
	void SetModlTemp(const T_MODL_D_TEMP& rData)
	{
		nCount_aModuleNameCharNum = (int)rData.aModuleNameCharNum.GetSize();
		nCount_aModuleNameChar    = (int)rData.aModuleNameChar.GetSize();
		nCount_aStoryNum          = (int)rData.aStoryNum.GetSize();
		nCount_aStoryK            = (int)rData.aStoryK.GetSize();
		nCount_aStorK_start       = (int)rData.aStorK_start.GetSize();
		nCount_aStorK_end         = (int)rData.aStorK_end.GetSize();
	}

	void Convert831(T_MODL_D_RW_831& rData)
	{
		nCount_aModuleNameCharNum = rData.nCount_aModuleNameCharNum;
		nCount_aModuleNameChar = rData.nCount_aModuleNameChar;
		nCount_aStoryNum = rData.nCount_aStoryNum;
		nCount_aStoryK = rData.nCount_aStoryK;
		nCount_aStorK_start = 0;   // rData.nCount_aStorK_start;
		nCount_aStorK_end = 0;     // rData.nCount_aStorK_end;
	}
};
struct T_MODL_UDRD_D
{
	T_MODL_K key;
	T_MODL_D data;
};
struct T_MODL_UNIT
{
	T_MODL_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEMODL 1
#pragma pack(pop)

#endif  // __DB_ST_DT_BLDG_H__



