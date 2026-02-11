#ifndef __DB_ST_DT_PRIS_H__
#define __DB_ST_DT_PRIS_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#include <afxtempl.h>

// for Civil JP

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// 결과정보 이미지 출력의 상세설정 ////////////////////////////////////

// 임시 선언
#define T_PRIS_K unsigned int
/**
* Post Result Image Setting (모델정보 이미지 출력의 상세설정)
*/
struct T_PRIS_D
{
	CString strTitle;
	BOOL bUse;
	int nNo;
	T_GRUP_K kGrup;		//요소그룹
	int nTargetType;
	int nLoadCaseType;	// enum T_PRIS_D::eLoadCaseType
	T_KEY kLoadCase;	//하중케이스 키
	int nLoadCaseMinMaxAll;  // 하중케이스의 최대/최소/모두
	int nDetailLoadCaseType;  // 동적/정적하중의 세부 종류
	int nStage;			//시공 스테이지
	int nResultKind;	// enum T_PRIS_D::eResultKind
	int nElementType;	//대상 enum T_PRIS_D::eElementType
	int nMode;			//nElementType값이 모드차수일 경우, 모드차수의 값
	int nComponent;		//결과성분 enum T_PRIS_D::eCompReaction, eCompDisplacement
	int nCombination;	//조합
	BOOL bBefore;		//변경전
	BOOL bAfter;		//변경후
	BOOL bLegend;		//범례
	BOOL bValue;		//수치
	BOOL bColor;		//색상
	BOOL bHidden;		//Hidden처리
	int  nContour;		//콘타표시Opt. enum T_PRIS_D::eContour
	BOOL bElemCenter;	//요소중심
	int nViewPoint;		// enum T_PRIS_D::eViewPoint
	double dAngleHori;
	double dAngleVert;
	int nLayoutType;	// enum T_PRIS_D::eLayoutType

	T_PRIS_D() { Initialize(); }
	T_PRIS_D(const T_PRIS_D& src) { *this = src; }
	T_PRIS_D& operator=(const T_PRIS_D& src)
	{
		strTitle = src.strTitle;
		bUse = src.bUse;
		nNo = src.nNo;
		kGrup = src.kGrup;
		nTargetType = src.nTargetType;
		nLoadCaseType = src.nLoadCaseType;
		kLoadCase = src.kLoadCase;
		nLoadCaseMinMaxAll = src.nLoadCaseMinMaxAll;
		nDetailLoadCaseType = src.nDetailLoadCaseType;
		nStage = src.nStage;
		nResultKind = src.nResultKind;
		nElementType = src.nElementType;
		nMode = src.nMode;
		nComponent = src.nComponent;
		nCombination = src.nCombination;
		bBefore = src.bBefore;
		bAfter = src.bAfter;
		bLegend = src.bLegend;
		bValue = src.bValue;
		bColor = src.bColor;
		bHidden = src.bHidden;
		nContour = src.nContour;
		bElemCenter = src.bElemCenter;
		nViewPoint = src.nViewPoint;
		dAngleHori = src.dAngleHori;
		dAngleVert = src.dAngleVert;
		nLayoutType = src.nLayoutType;
		return *this;
	}

	void Initialize()
	{
		strTitle.Empty();
		bUse = FALSE;
		nNo = 0;
		kGrup = 0;
		nTargetType = 0;
		nLoadCaseType = 0;
		kLoadCase = 0;
		nLoadCaseMinMaxAll = 0;
		nDetailLoadCaseType = 0;
		nStage = 0;
		nResultKind = 0;
		nElementType = 0;
		nMode = 0;
		nComponent = 0;
		nCombination = 0;
		bBefore = FALSE;
		bAfter = FALSE;
		bLegend = FALSE;
		bValue = FALSE;
		bColor = FALSE;
		bHidden = FALSE;
		nContour = 0;
		bElemCenter = FALSE;
		nViewPoint = 0;
		dAngleHori = 0;
		dAngleVert = 0;
		nLayoutType = 0;
	}

	enum ePrintType
	{
		kAllModel = 1, // 전체 모델
		kElementGrup,
	};

	enum eLoadCaseType
	{
		kConstructionStep = 1,// 시공단계
		kStatic,			// 정적
		kDynamic,			// 동적
		kEigenvalue,		// 고유치
		kBuckling,			// 좌굴
	};

	enum eLoadCaseMinMaxAll
	{
		kUnKnownMinMaxAll = 0,
		kLoadCaseMax,
		kLoadCaseMin,
		kLoadCaseAll,
	};

	enum eStaticLoadCaseType
	{
		kStaticDefaultNone = 0,
		kStldLoadCase, // stld
		kSmlcLoadCase,  // smlc
		kMvcdLoadCase, // mvcd ... 
		kStaticGlicLoadCase, // glic
		kStaticLcomLoadCase1, // Lcom
		kStaticLcomLoadCase2, // Lcom
		kStaticLcomLoadCase3, // Lcom
		kStaticLcomLoadCase4, // Lcom
		kStaticLcomLoadCase5, // Lcom
	};

	enum eDynamicLoadCaseType
	{
		kDynamicDefaultNone = 0,
		kSplcLoadCase, // splc
		kThisLoadCase,  // this
		kDynamicLcomLoadCase1, // Lcom
		kDynamicLcomLoadCase2, // Lcom
		kDynamicLcomLoadCase3, // Lcom
		kDynamicLcomLoadCase4, // Lcom 
		kDynamicLcomLoadCase5, // Lcom
	};

	enum eResultKind
	{
		kReaction = 1,		// 반력도
		kDisplacement,		// 변위도
		kSectionForce,		// 단면력도
		kStress,			// 응력도
		kEigenvalueMode,	// 고유모드
		kBucklingMode,		// 좌굴모드
		kSpeedMode,		// 속도
		kAccelMode,		//  가속도
	};

	enum eElementType
	{
		kTruss = 1,			// 트러스
		kBeam,				// 보
		kPlate,				// 판
		kPlaneStress_Plate,	// 평면응력/판요소
		kPlaneStrain,		// 평면변형
		kAxisymmetric,		// 축대칭
		kSolid,				// 솔리드
		kModeNumber,		// 모드차수
	};

	enum eCompReaction
	{
		kFX = 1,
		kFY,
		kFZ,
		kFXYZ,
		kMX,
		kMY,
		kMZ,
		kMXYZ,
	};

	enum eCompDisplacement
	{
		kDX = 1,
		kDY,
		kDZ,
		kDXY,
		kDYZ,
		kDXZ,
		kDXYZ,
	};

	enum eBeam
	{
		kbeam_FX = 1,
		kbeam_Fy,
		kbeam_Fz,
		kbeam_MX,
		kbeam_MY,
		kbeam_MZ 
	};

	enum ePlate
	{
		kFxx = 1,
		kFyy,
		kFxy,
		kMxx,
		kMyy,
		kVxx,
		kVyy,
	};

	enum eCompTruss
	{
		kAll = 1,
		kTension,
		kCompression,
	};

	enum eCompBeam
	{
		kDelta_ax=1,
		kDelta_sy,
		kDelta_sz,
		kDelta_by,
		kDelta_bz,
		kCombination,	// 조합
	};

	enum eCompPlaneStress_Plate
	{
		kPlDelta_xx=1,
		kPlDelta_yy,
		kPlDelta_xy,
		kPlDelta_max,
		kPlDelta_min,
		kPlDelta_eff,
		kPlMaxShear,		// 최대전단
	};

	enum eCompPlaneStrain
	{
		kStDelta_xx = 1,
		kStDelta_yy,
		kStDelta_zz,
		kStDelta_xy,
		kStDelta_1,
		kStDelta_2,
		kStDelta_3,
		kStDelta_eff,
		kStMaxShear,		// 최대전단
	};

	enum eCompAxisymmetric
	{
		kAxDelta_xx = 1,
		kAxDelta_yy,
		kAxDelta_zz,
		kAxDelta_xy,
		kAxDelta_1,
		kAxDelta_2,
		kAxDelta_3,
		kAxDelta_eff,
		kAxMaxShear,		// 최대전단
	};

	enum eCompSolid
	{
		kSoDelta_xx = 1,
		kSoDelta_yy,
		kSoDelta_zz,
		kSoDelta_xy,
		kSoDelta_yz,
		kSoDelta_xz,
		kSoDelta_1,
		kSoDelta_2,
		kSoDelta_3,
		kSoDelta_eff,
		kSoMaxShear,		// 최대전단
	};

	enum eCompModeNumber
	{
		kMoDX = 1,
		kMoDY,
		kMoDZ,
		kMoDXY,
		kMoDYZ,
		kMoDXZ,
		kMoDXYZ,
	};

	enum eCombination
	{
		kMaximum=1,
		k1_minusY_plusZ,
		k2_plusY_plusZ,
		k2_plusY_minusZ,
		k4_minusY_minusZ,
		kUpperSurface,		//상면
		kLowerSurface,		//하면
	};

	enum eContour
	{
		kShowNodes=1,	//절점표시
		kSmoothing,		//스무징
	};

	enum eViewPoint
	{
		kUnKnown,
		kIsome,
		kTop,
		kFront,
		kRight,
		kLeft,
		kUser,
	};

	enum eLayoutType
	{
		kVertPageUpNone=1,
		kHoriPageUpNone,
		kVertPageUp2,
		kHoriPageUp2,
	};
};


struct T_PRIS_D_CH
{
	// list, CString, key -> CArray, char[], int
	char strTitle_[40]; // CString 
	T_KEY kLoadCase;	// T_PRIS_K (하중케이스 키)
	int nLoadCaseMinMaxAll;  // 하중케이스의 최대/최소/모두
	int nDetailLoadCaseType;
	T_GRUP_K kGrup; // T_GRUP_K (요소그룹)
	int nTargetType;
	BOOL bUse;
	int nNo;
	int nLoadCaseType;	
	int nStage;			
	int nResultKind;	
	int nElementType;
	int nMode;
	int nComponent;		
	int nCombination;	
	BOOL bBefore;		
	BOOL bAfter;		
	BOOL bLegend;		
	BOOL bValue;		
	BOOL bColor;		
	BOOL bHidden;		
	int  nContour;		
	BOOL bElemCenter;	
	int nViewPoint;		
	double dAngleHori;
	double dAngleVert;
	int nLayoutType;

	void Get(T_PRIS_D& rData)
	{
		ConvertCharStr(strTitle_, rData.strTitle, sizeof(strTitle_));
		rData.kLoadCase = kLoadCase;
		rData.nLoadCaseMinMaxAll = nLoadCaseMinMaxAll;
		rData.nDetailLoadCaseType = nDetailLoadCaseType;
		rData.kGrup = kGrup;
		rData.nTargetType = nTargetType;
		rData.bUse = bUse;
		rData.nNo = nNo;
		rData.nLoadCaseType = nLoadCaseType;
		rData.nStage = nStage;
		rData.nResultKind = nResultKind;
		rData.nElementType = nElementType;
		rData.nMode = nMode;
		rData.nComponent = nComponent;
		rData.nCombination = nCombination;
		rData.bBefore = bBefore;
		rData.bAfter = bAfter;
		rData.bLegend = bLegend;
		rData.bValue = bValue;
		rData.bColor = bColor;
		rData.bHidden = bHidden;
		rData.nContour = nContour;
		rData.bElemCenter = bElemCenter;
		rData.nViewPoint = nViewPoint;
		rData.dAngleHori = dAngleHori;
		rData.dAngleVert = dAngleVert;
		rData.nLayoutType = nLayoutType;
	}

	void Set(T_PRIS_D& data)
	{
		ConvertStrChar(data.strTitle, strTitle_, sizeof(strTitle_));
		kLoadCase = data.kLoadCase;
		nLoadCaseMinMaxAll = data.nLoadCaseMinMaxAll;
		nDetailLoadCaseType = data.nDetailLoadCaseType;
		kGrup = data.kGrup;
		nTargetType = data.nTargetType;
		bUse = data.bUse;
		nNo = data.nNo;
		nLoadCaseType = data.nLoadCaseType;
		nStage = data.nStage;
		nResultKind = data.nResultKind;
		nElementType = data.nElementType;
		nMode = data.nMode;
		nComponent = data.nComponent;
		nCombination = data.nCombination;
		bBefore = data.bBefore;
		bAfter = data.bAfter;
		bLegend = data.bLegend;
		bValue = data.bValue;
		bColor = data.bColor;
		bHidden = data.bHidden;
		nContour = data.nContour;
		bElemCenter = data.bElemCenter;
		nViewPoint = data.nViewPoint;
		dAngleHori = data.dAngleHori;
		dAngleVert = data.dAngleVert;
		nLayoutType = data.nLayoutType;
	}
};

struct T_PRIS_UDRD_D
{
	T_PRIS_K key;
	T_PRIS_D data;
};

struct T_PRIS_UDRD_D_CH
{
	T_PRIS_K key;
	T_PRIS_D_CH data;
};

struct T_PRIS_UDRD_D_CH_RW
{
	T_PRIS_K key;
	char strTitle_[40];
	T_GRUP_K kGrup;
	int nTargetType;
	T_KEY kLoadCase;
	int nLoadCaseMinMaxAll;
	int nDetailLoadCaseType;
	BOOL bUse;
	int nNo;
	int nLoadCaseType;
	int nStage;
	int nResultKind;
	int nElementType;
	int nMode;
	int nComponent;
	int nCombination;
	BOOL bBefore;
	BOOL bAfter;
	BOOL bLegend;
	BOOL bValue;
	BOOL bColor;
	BOOL bHidden;
	int  nContour;
	BOOL bElemCenter;
	int nViewPoint;
	double dAngleHori;
	double dAngleVert;
	int nLayoutType;

	void Get(T_PRIS_K& rKey, T_PRIS_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strTitle_, strTitle_, sizeof(strTitle_));
		rData.kLoadCase = kLoadCase;
		rData.nLoadCaseMinMaxAll = nLoadCaseMinMaxAll;
		rData.nDetailLoadCaseType = nDetailLoadCaseType;
		rData.kGrup = kGrup;
		rData.nTargetType = nTargetType;
		rData.bUse = bUse;
		rData.nNo = nNo;
		rData.nLoadCaseType = nLoadCaseType;
		rData.nStage = nStage;
		rData.nResultKind = nResultKind;
		rData.nElementType = nElementType;
		rData.nMode = nMode;
		rData.nComponent = nComponent;
		rData.nCombination = nCombination;
		rData.bBefore = bBefore;
		rData.bAfter = bAfter;
		rData.bLegend = bLegend;
		rData.bValue = bValue;
		rData.bColor = bColor;
		rData.bHidden = bHidden;
		rData.nContour = nContour;
		rData.bElemCenter = bElemCenter;
		rData.nViewPoint = nViewPoint;
		rData.dAngleHori = dAngleHori;
		rData.dAngleVert = dAngleVert;
		rData.nLayoutType = nLayoutType;
	}
	
	void Set(const T_PRIS_K Key, const T_PRIS_D_CH& data)
	{
		key = Key;
		memcpy(strTitle_, data.strTitle_, sizeof(strTitle_));
		kLoadCase = data.kLoadCase;
		nLoadCaseMinMaxAll = data.nLoadCaseMinMaxAll;
		nDetailLoadCaseType = data.nDetailLoadCaseType;
		kGrup = data.kGrup;
		nTargetType = data.nTargetType;
		bUse = data.bUse;
		nNo = data.nNo;
		nLoadCaseType = data.nLoadCaseType;
		nStage = data.nStage;
		nResultKind = data.nResultKind;
		nElementType = data.nElementType;
		nMode = data.nMode;
		nComponent = data.nComponent;
		nCombination = data.nCombination;
		bBefore = data.bBefore;
		bAfter = data.bAfter;
		bLegend = data.bLegend;
		bValue = data.bValue;
		bColor = data.bColor;
		bHidden = data.bHidden;
		nContour = data.nContour;
		bElemCenter = data.bElemCenter;
		nViewPoint = data.nViewPoint;
		dAngleHori = data.dAngleHori;
		dAngleVert = data.dAngleVert;
		nLayoutType = data.nLayoutType;
	}
};

struct T_PRIS_UNIT
{
	T_PRIS_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZEPRIS 1001
#define HASHSIZELISTPRIS 11

#pragma pack(pop)

#endif  // __DB_ST_DT_PRIS_H__