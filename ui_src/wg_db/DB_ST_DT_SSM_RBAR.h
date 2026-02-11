#pragma once
#include "UserDefType.h"

#pragma region // T_RPSC_MBAR_SSM (변수 구조체) : PC단면 축방향 철근 배근 정보
struct T_POINT_SSM
{
	double dPointY;
	double dPointZ;

	void Initialize()
	{
		dPointY = 0.;
		dPointZ = 0.;
	}

	T_POINT_SSM() { Initialize(); }
	T_POINT_SSM(const T_POINT_SSM& rData) { *this = rData; }
	T_POINT_SSM& operator=(const T_POINT_SSM& rData)
	{
		dPointY = rData.dPointY;
		dPointZ = rData.dPointZ;
		return (*this);
	}
};
typedef CArray<T_POINT_SSM, T_POINT_SSM&> tPointSsmArray;

struct T_RPSC_MBAR_SSM   // Longitudinal Reinforcement
{
	int     nType;          // 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	int     nRefY;          // 0:Left 1:Center 2:Right (Tapered 에서 사용)
	int     nRefZ;          // 0:Top 1:Center 2:Bottom (Tapered 에서 사용)
	T_POINT_SSM Start;      // Start Point, Corner1 Point
	T_POINT_SSM Center;     // Center Point
	tPointSsmArray aPassage; // Passage Point(y,z)
	T_POINT_SSM End;        // End Point, Corner2 Point
	double  dRadius;        // Radius
	int     nNumorCTC;      // 0:Num. 1:CTC
	int     nNum;           // Num
	double  dCTC;           // CTC 
	BOOL    bEdgeBar;      // 가장자리 철근
	CString strBar;         // 철근
	int     nStandRef;      // 배근 기준점 0:시점 1:Center 2:종점
	int     nPart;          // 0:Part 구분 없는 단면
	// 1:Composite 의 Part1
	// 2:Composite 의 Part2

	int     nInputMethod;   // A(기존), B(신규)
	int     nRefY_A;        // 0:Centroid 1:Left
	int     nRefZ_A;        // 0:Top 1:Bottom
	double  dRefY_A;
	double  dRefZ_A;
	double  dSpacing;

	// �߰� 860
	int     nNumTopBot;     // Num. Top & Bot
	int     nNumLeftRight;  // Num. Left & Right
	double  dRotateAngle;

	void Initialize()
	{
		nType = 1;
		nRefY = 0;
		nRefZ = 0;
		Start.Initialize();
		Center.Initialize();
		aPassage.RemoveAll();
		End.Initialize();
		dRadius = 0.;
		nNumorCTC = 0;
		nNum = 0;
		dCTC = 0.;
		bEdgeBar = FALSE;
		strBar = "";
		nStandRef = 0;
		nPart = 0;

		nInputMethod = 0;
		nRefY_A = 0;
		nRefZ_A = 0;
		dRefY_A = 0.0;
		dRefZ_A = 0.0;
		dSpacing = 0.0;

		nNumTopBot = 0;
		nNumLeftRight = 0;
		dRotateAngle = 0.0;
	}

	T_RPSC_MBAR_SSM() { Initialize(); }
	T_RPSC_MBAR_SSM(const T_RPSC_MBAR_SSM& rData) { *this = rData; }
	T_RPSC_MBAR_SSM& operator=(const T_RPSC_MBAR_SSM& rData)
	{
		nType = rData.nType;
		nRefY = rData.nRefY;
		nRefZ = rData.nRefZ;
		Start = rData.Start;
		Center = rData.Center;
		aPassage.Copy(rData.aPassage);
		End = rData.End;
		dRadius = rData.dRadius;
		nNumorCTC = rData.nNumorCTC;
		nNum = rData.nNum;
		dCTC = rData.dCTC;
		bEdgeBar = rData.bEdgeBar;
		strBar = rData.strBar;
		nStandRef = rData.nStandRef;
		nPart = rData.nPart;

		nInputMethod = rData.nInputMethod;
		nRefY_A = rData.nRefY_A;
		nRefZ_A = rData.nRefZ_A;
		dRefY_A = rData.dRefY_A;
		dRefZ_A = rData.dRefZ_A;
		dSpacing = rData.dSpacing;

		nNumTopBot = rData.nNumTopBot;
		nNumLeftRight = rData.nNumLeftRight;
		dRotateAngle = rData.dRotateAngle;

		return (*this);
	}
};
typedef CArray<T_RPSC_MBAR_SSM, T_RPSC_MBAR_SSM&> tRpscMbarSsmArray;

struct T_RPSC_MBAR_SSM_CH // Longitudinal Reinforcement
{
	int     nType;          // 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	int     nRefY;          // 0:Left 1:Center 2:Right
	int     nRefZ;          // 0:Top 1:Center 2:Bottom 
	T_POINT_SSM Start;      // Start Point
	T_POINT_SSM Center;     // Center Point
	tPointSsmArray aPassage; // Passage Point(y,z)
	T_POINT_SSM End;        // End Point
	double  dRadius;        // Radius
	int     nNumorCTC;      // 0:Num. 1:CTC
	int     nNum;           // Num
	double  dCTC;           // CTC 
	BOOL    bEdgeBar;       // 가장자리 철근
	char    strBar[8];      // 철근
	int     nStandRef;      // 배근 기준점 0:시점 1:Center 2:종점
	int     nPart;          // 0:Part 구분 없는 단면
	// 1:Composite 의 Part1
	// 2:Composite 의 Part2

	int     nInputMethod;   // A(기존), B(신규)
	int     nRefY_A;        // 0:Centroid 1:Left
	int     nRefZ_A;        // 0:Top 1:Bottom
	double  dRefY_A;
	double  dRefZ_A;
	double  dSpacing;

	int     nNumTopBot;     // Num. Top & Bot
	int     nNumLeftRight;  // Num. Left & Right
	double  dRotateAngle;

	void ConvertToString(T_RPSC_MBAR_SSM& rData)
	{
		rData.nType = nType;
		rData.nRefY = nRefY;
		rData.nRefZ = nRefZ;
		rData.Start = Start;
		rData.Center = Center;
		rData.aPassage.Copy(aPassage);
		rData.End = End;
		rData.dRadius = dRadius;
		rData.nNumorCTC = nNumorCTC;
		rData.nNum = nNum;
		rData.dCTC = dCTC;
		rData.bEdgeBar = bEdgeBar;
		ConvertCharStr(strBar, rData.strBar, sizeof(strBar));
		rData.nStandRef = nStandRef;
		rData.nPart = nPart;

		rData.nInputMethod = nInputMethod;
		rData.nRefY_A = nRefY_A;
		rData.nRefZ_A = nRefZ_A;
		rData.dRefY_A = dRefY_A;
		rData.dRefZ_A = dRefZ_A;
		rData.dSpacing = dSpacing;

		rData.nNumTopBot = nNumTopBot;
		rData.nNumLeftRight = nNumLeftRight;
		rData.dRotateAngle = dRotateAngle;

	}
	void ConvertToChar(T_RPSC_MBAR_SSM& rData)
	{
		nType = rData.nType;
		nRefY = rData.nRefY;
		nRefZ = rData.nRefZ;
		Start = rData.Start;
		Center = rData.Center;
		aPassage.Copy(rData.aPassage);
		End = rData.End;
		dRadius = rData.dRadius;
		nNumorCTC = rData.nNumorCTC;
		nNum = rData.nNum;
		dCTC = rData.dCTC;
		bEdgeBar = rData.bEdgeBar;
		ConvertStrChar(rData.strBar, strBar, sizeof(strBar));
		nStandRef = rData.nStandRef;
		nPart = rData.nPart;

		nInputMethod = rData.nInputMethod;
		nRefY_A = rData.nRefY_A;
		nRefZ_A = rData.nRefZ_A;
		dRefY_A = rData.dRefY_A;
		dRefZ_A = rData.dRefZ_A;
		dSpacing = rData.dSpacing;

		nNumTopBot = rData.nNumTopBot;
		nNumLeftRight = rData.nNumLeftRight;
		dRotateAngle = rData.dRotateAngle;
	}
};

struct T_RPSC_MBAR_SSM_CH_RW_795 // Longitudinal Reinforcement
{
	int     nType;          // 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	int     nRefY;          // 0:Left 1:Center 2:Right
	int     nRefZ;          // 0:Top 1:Center 2:Bottom 
	T_POINT_SSM Start;      // Start Point
	T_POINT_SSM Center;     // Center Point
	int     nCountPassage;  // Passage Point(y,z)
	T_POINT_SSM End;        // End Point
	double  dRadius;        // Radius
	int     nNumorCTC;      // 0:Num. 1:CTC
	int     nNum;           // Num
	double  dCTC;           // CTC 
	BOOL    bEdgeBar;       // 가장자리 철근
	char    strBar[8];      // 철근
	int     nStandRef;      // 배근 기준점 0:시점 1:Center 2:종점
	int     nPart;          // 0:Part 구분 없는 단면
	// 1:Composite 의 Part1
	// 2:Composite 의 Part2
};

struct T_RPSC_MBAR_SSM_CH_RW_860 // Longitudinal Reinforcement
{
	int     nType;          // 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	int     nRefY;          // 0:Left 1:Center 2:Right
	int     nRefZ;          // 0:Top 1:Center 2:Bottom 
	T_POINT_SSM Start;      // Start Point
	T_POINT_SSM Center;     // Center Point
	int     nCountPassage;  // Passage Point(y,z)
	T_POINT_SSM End;        // End Point
	double  dRadius;        // Radius
	int     nNumorCTC;      // 0:Num. 1:CTC
	int     nNum;           // Num
	double  dCTC;           // CTC 
	BOOL    bEdgeBar;       // 가장자리 철근
	char    strBar[8];      // 철근
	int     nStandRef;      // 배근 기준점 0:시점 1:Center 2:종점
	int     nPart;          // 0:Part 구분 없는 단면
	// 1:Composite 의 Part1
	// 2:Composite 의 Part2

	int     nInputMethod;   // A(기존), B(신규)
	int     nRefY_A;        // 0:Centroid 1:Left
	int     nRefZ_A;        // 0:Top 1:Bottom
	double  dRefY_A;
	double  dRefZ_A;
	double  dSpacing;
};

struct T_RPSC_MBAR_SSM_CH_RW // Longitudinal Reinforcement
{
	int     nType;          // 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	int     nRefY;          // 0:Left 1:Center 2:Right
	int     nRefZ;          // 0:Top 1:Center 2:Bottom 
	T_POINT_SSM Start;      // Start Point
	T_POINT_SSM Center;     // Center Point
	int     nCountPassage;  // Passage Point(y,z)
	T_POINT_SSM End;        // End Point
	double  dRadius;        // Radius
	int     nNumorCTC;      // 0:Num. 1:CTC
	int     nNum;           // Num
	double  dCTC;           // CTC 
	BOOL    bEdgeBar;       // 가장자리 철근
	char    strBar[8];      // 철근
	int     nStandRef;      // 배근 기준점 0:시점 1:Center 2:종점
	int     nPart;          // 0:Part 구분 없는 단면
	// 1:Composite 의 Part1
	// 2:Composite 의 Part2

	int     nInputMethod;   // A(기존), B(신규)
	int     nRefY_A;        // 0:Centroid 1:Left
	int     nRefZ_A;        // 0:Top 1:Bottom
	double  dRefY_A;
	double  dRefZ_A;
	double  dSpacing;

	int nNumTopBot;
	int nNumLeftRight;
	double dRotateAngle;

	void Get(T_RPSC_MBAR_SSM_CH& rData)
	{
		rData.nType = nType;
		rData.nRefY = nRefY;
		rData.nRefZ = nRefZ;
		rData.Start = Start;
		rData.Center = Center;
		rData.aPassage.SetSize(nCountPassage);
		rData.End = End;
		rData.dRadius = dRadius;
		rData.nNumorCTC = nNumorCTC;
		rData.nNum = nNum;
		rData.dCTC = dCTC;
		rData.bEdgeBar = bEdgeBar;
		memcpy(rData.strBar, strBar, sizeof(strBar));
		rData.nStandRef = nStandRef;
		rData.nPart = nPart;

		rData.nInputMethod = nInputMethod;
		rData.nRefY_A = nRefY_A;
		rData.nRefZ_A = nRefZ_A;
		rData.dRefY_A = dRefY_A;
		rData.dRefZ_A = dRefZ_A;
		rData.dSpacing = dSpacing;

		rData.nNumTopBot = nNumTopBot;
		rData.nNumLeftRight = nNumLeftRight;
		rData.dRotateAngle = dRotateAngle;
	}
	void Set(T_RPSC_MBAR_SSM_CH& Data)
	{
		nType = Data.nType;
		nRefY = Data.nRefY;
		nRefZ = Data.nRefZ;
		Start = Data.Start;
		Center = Data.Center;
		nCountPassage = (int)Data.aPassage.GetSize();
		End = Data.End;
		dRadius = Data.dRadius;
		nNumorCTC = Data.nNumorCTC;
		nNum = Data.nNum;
		dCTC = Data.dCTC;
		bEdgeBar = Data.bEdgeBar;
		memcpy(strBar, Data.strBar, sizeof(strBar));
		nStandRef = Data.nStandRef;
		nPart = Data.nPart;

		nInputMethod = Data.nInputMethod;
		nRefY_A = Data.nRefY_A;
		nRefZ_A = Data.nRefZ_A;
		dRefY_A = Data.dRefY_A;
		dRefZ_A = Data.dRefZ_A;
		dSpacing = Data.dSpacing;

		nNumTopBot = Data.nNumTopBot;
		nNumLeftRight = Data.nNumLeftRight;
		dRotateAngle = Data.dRotateAngle;

	}
	void Convert860(T_RPSC_MBAR_SSM_CH_RW_860& Data)
	{
		nType = Data.nType;
		nRefY = Data.nRefY;
		nRefZ = Data.nRefZ;
		Start = Data.Start;
		Center = Data.Center;
		nCountPassage = Data.nCountPassage;
		End = Data.End;
		dRadius = Data.dRadius;
		nNumorCTC = Data.nNumorCTC;
		nNum = Data.nNum;
		dCTC = Data.dCTC;
		bEdgeBar = Data.bEdgeBar;
		memcpy(strBar, Data.strBar, sizeof(strBar));
		nStandRef = Data.nStandRef;
		nPart = Data.nPart;

		nInputMethod = Data.nInputMethod;
		nRefY_A = Data.nRefY_A;
		nRefZ_A = Data.nRefZ_A;
		dRefY_A = Data.dRefY_A;
		dRefZ_A = Data.dRefZ_A;
		dSpacing = Data.dSpacing;

		nNumTopBot = 0;
		nNumLeftRight = 0;
		dRotateAngle = 0.0;
	}
	void Convert795(T_RPSC_MBAR_SSM_CH_RW_795& rData)
	{
		nType = rData.nType;
		nRefY = rData.nRefY;
		nRefZ = rData.nRefZ;
		Start = rData.Start;
		Center = rData.Center;
		nCountPassage = rData.nCountPassage;
		End = rData.End;
		dRadius = rData.dRadius;
		nNumorCTC = rData.nNumorCTC;
		nNum = rData.nNum;
		dCTC = rData.dCTC;
		bEdgeBar = rData.bEdgeBar;
		memcpy(strBar, rData.strBar, sizeof(strBar));
		nStandRef = rData.nStandRef;
		nPart = rData.nPart;

		nInputMethod = 1;
		nRefY_A = 0;
		nRefZ_A = 0;
		dRefY_A = 0.0;
		dRefZ_A = 0.0;
		dSpacing = 0.0;

		nNumTopBot = 0;
		nNumLeftRight = 0;
		dRotateAngle = 0.0;
	}
};
#pragma endregion

#pragma region // T_RPSC_SBAR_SSM (변수 구조체) : PC단면 횡방향 철근(전단철근) 배근 정보
struct T_RPSC_SBAR_SSM_925     // reinforced Shear
{
	BOOL bDiagonal;          // 경사인장철근 - Diagonal Reinforcement
	double dDiagonalPitch;   // 경사인장철근의 간격
	double dDiagonalAngle;   // 경사인장철근의 각도
	double dDiagonalAw;      // 경사인장철근의 배치량
	BOOL bSteelBar;          // 전단강봉 - Steel Bar for Web
	double dSteelBarPitch;   // 전단강봉의 간격
	double dSteelBarAngle;   // 전단강봉의 각도
	double dSteelBarAp;      // 전단강봉의 배치량
	double dSteelBarPe;      // 전단강봉의 유효인장력
	double dSteelBarFactor;  // 전단강봉에 의한 발생수직응력저감계수 (JTG-D62-04 6.3.3-3)
	// Default (CH=0.6, Etc.=1.0) 
	BOOL bTorsional;         // 횡방향철근 - Torsional Reinforcement
	double dTorsionalPitch;  // 횡방향철근의 간격
	double dTorsionalAwt;    // 횡방향철근의 Pitch당 철근 1개의 단면적
	double dTorsionalAlt;    // 횡뱡향철근의 전단면적
	BOOL   bStirExist;       // 전단철근 정보 - JTG_D62_04, TB_10002_3_05 일때
	double dStirPitch;       // 전단철근의 간격
	double dStirAw;          // 전단철근의 배치량
	BOOL bLbarFlg;           // Acor계산시 플랜지 부분을 스트럽으로 감는것을 고려하는 변수
	double dLbarThick;       // 비틀림 스트럽에서 연단까지의 거리(피복두께)
	BOOL bIncFC;             // Include Flange/Cantilever
};

struct T_RPSC_SBAR_SSM     // reinforced Shear
{
	BOOL bDiagonal;          // 경사인장철근 - Diagonal Reinforcement
	double dDiagonalPitch;   // 경사인장철근의 간격
	double dDiagonalAngle;   // 경사인장철근의 각도
	double dDiagonalAw;      // 경사인장철근의 배치량
	BOOL bSteelBar;          // 전단강봉 - Steel Bar for Web
	double dSteelBarPitch;   // 전단강봉의 간격
	double dSteelBarAngle;   // 전단강봉의 각도
	double dSteelBarAp;      // 전단강봉의 배치량
	double dSteelBarPe;      // 전단강봉의 유효인장력
	double dSteelBarFactor;  // 전단강봉에 의한 발생수직응력저감계수 (JTG-D62-04 6.3.3-3)
	// Default (CH=0.6, Etc.=1.0) 
	BOOL bTorsional;         // 횡방향철근 - Torsional Reinforcement
	double dTorsionalPitch;  // 횡방향철근의 간격
	double dTorsionalAwt;    // 횡방향철근의 Pitch당 철근 1개의 단면적
	double dTorsionalAlt;    // 횡뱡향철근의 전단면적
	BOOL   bStirExist;       // 전단철근 정보 - JTG_D62_04, TB_10002_3_05 일때
	double dStirPitch;       // 전단철근의 간격
	double dStirAw;          // 전단철근의 배치량
	BOOL bLbarFlg;           // Acor계산시 플랜지 부분을 스트럽으로 감는것을 고려하는 변수
	double dLbarThick;       // 비틀림 스트럽에서 연단까지의 거리(피복두께)
	BOOL bIncFC;             // Include Flange/Cantilever

	int nAutoUser;       // (v930) 0:Auto 1:User
	double dAk;          // (v930) User - 폐합단면
	double dUk;          // (v930) User - 폐합단면둘레
	double dAkCover;     // (v930) User - 폐합단면 + cover 고려
	double dUkCover;     // (v930) User - 폐합단면둘레 + cover 고려

	void Initialize()
	{
		bDiagonal = FALSE;
		dDiagonalPitch = 0.0;
		dDiagonalAngle = 0.0;
		dDiagonalAw = 0.0;
		bSteelBar = FALSE;
		dSteelBarPitch = 0.0;
		dSteelBarAngle = 90.0;
		dSteelBarAp = 0.0;
		dSteelBarPe = 0.0;
#if defined(_CH)
		dSteelBarFactor = 0.6;
#else
		dSteelBarFactor = 1.0;
#endif
		bTorsional = FALSE;
		dTorsionalPitch = 0.0;
		dTorsionalAwt = 0.0;
		dTorsionalAlt = 0.0;
		bStirExist = FALSE;
		dStirPitch = 0.0;
		dStirAw = 0.0;
		bLbarFlg = FALSE;
		dLbarThick = 0.0;
		bIncFC = FALSE;

		nAutoUser = 0;
		dAk = 0.0;
		dUk = 0.0;
		dAkCover = 0.0;
		dUkCover = 0.0;
	}

	void Convert925(T_RPSC_SBAR_SSM_925& rData)
	{
		bDiagonal = rData.bDiagonal;
		dDiagonalPitch = rData.dDiagonalPitch;
		dDiagonalAngle = rData.dDiagonalAngle;
		dDiagonalAw = rData.dDiagonalAw;
		bSteelBar = rData.bSteelBar;
		dSteelBarPitch = rData.dSteelBarPitch;
		dSteelBarAngle = rData.dSteelBarAngle;
		dSteelBarAp = rData.dSteelBarAp;
		dSteelBarPe = rData.dSteelBarPe;
		dSteelBarFactor = rData.dSteelBarFactor;
		bTorsional = rData.bTorsional;
		dTorsionalPitch = rData.dTorsionalPitch;
		dTorsionalAwt = rData.dTorsionalAwt;
		dTorsionalAlt = rData.dTorsionalAlt;
		bStirExist = rData.bStirExist;
		dStirPitch = rData.dStirPitch;
		dStirAw = rData.dStirAw;
		bLbarFlg = rData.bLbarFlg;
		dLbarThick = rData.dLbarThick;
		bIncFC = rData.bIncFC;

		nAutoUser = 0;
		dAk = 0.0;
		dUk = 0.0;
		dAkCover = 0.0;
		dUkCover = 0.0;
	}
};
#pragma endregion

#pragma region // T_RBAR_MAINBAR (변수 구조체) : 정형단면 축방향 철근 배근 정보
struct T_RRSC_MAINBAR
{
	UINT nPos;				//단면 변의 위치 (0 ~ ...)
	UINT nRebarPlacingType;	//배근방법 - T_RRSC_MAINBAR::eRebarPlacingType
	double dOffsetDist;		//배근방법 - 뛰움거리 or 간격거리
	double dOffsetAngle;	//배근방법 - 뛰움각도 or 간격각도
	double dCoverThickness;	//피복두께
	UINT nRebarNum;			//철근 개수
	CString strRebarDia;    //철근 직경
	double dUserRebarDia;	//유저직접입력(철근 직경)
	BOOL bEmbedmentRebar;	//보강 단면에서의 철근단부 정착 여부

	void Initialize()
	{
		nPos = 0;
		nRebarPlacingType = 0;
		dOffsetDist = 0;
		dOffsetAngle = 0;
		dCoverThickness = 0;
		nRebarNum = 0;
		strRebarDia = _T("");
		dUserRebarDia = 0;
		bEmbedmentRebar = FALSE;
	}

	T_RRSC_MAINBAR() { Initialize(); }
	T_RRSC_MAINBAR(const T_RRSC_MAINBAR& src) { *this = src; }
	T_RRSC_MAINBAR& operator=(const T_RRSC_MAINBAR& src)
	{
		if (this != &src) {
			nPos = src.nPos;
			nRebarPlacingType = src.nRebarPlacingType;
			dOffsetDist = src.dOffsetDist;
			dOffsetAngle = src.dOffsetAngle;
			dCoverThickness = src.dCoverThickness;
			nRebarNum = src.nRebarNum;
			strRebarDia = src.strRebarDia;
			dUserRebarDia = src.dUserRebarDia;
			bEmbedmentRebar = src.bEmbedmentRebar;
		}
		return *this;
	}

	enum eRebarPos //�ܸ� ���� ��ġ (0 ~ ...)
	{
		kPos1,	
		kPos2,
		kPos3,
		kPos4,
		kPos5,
		kPos6,
		kPos7,
		kPos8,
	};

	enum eRebarPlacingType
	{
		kUnkowun,
		kSpacingDist,	// 간격 거리
		kOffsetDist,	//뛰움 거리
		kSpacingAngle,	// 간격 각도
		kOffsetAngle,		//뛰움각도
	};

	bool operator==(const T_RRSC_MAINBAR& rhs) const
	{
		if (nPos != rhs.nPos) return false;
		if (nRebarPlacingType != rhs.nRebarPlacingType) return false;
		
		// Compare doubles with a small tolerance for floating-point precision
		const double epsilon = 1e-10;
		if (fabs(dOffsetDist - rhs.dOffsetDist) > epsilon) return false;
		if (fabs(dOffsetAngle - rhs.dOffsetAngle) > epsilon) return false;
		if (fabs(dCoverThickness - rhs.dCoverThickness) > epsilon) return false;
		if (fabs(dUserRebarDia - rhs.dUserRebarDia) > epsilon) return false;
		
		if (nRebarNum != rhs.nRebarNum) return false;
		if (strRebarDia != rhs.strRebarDia) return false;
		if (bEmbedmentRebar != rhs.bEmbedmentRebar) return false;
		
		return true;
	}
	
	bool operator!=(const T_RRSC_MAINBAR& rhs) const
	{
		return !(*this == rhs);
	}
};
typedef CArray<T_RRSC_MAINBAR, T_RRSC_MAINBAR&> tRrscMainbarArray;

struct T_RRSC_MAINBAR_CH
{
	UINT nPos;				//단면 변의 위치 (0 ~ ...)
	UINT nRebarPlacingType;	//배근방법 - T_RRSC_MAINBAR::eRebarPlacingType
	double dOffsetDist;		//배근방법 - 뛰움거리 or 간격거리
	double dOffsetAngle;	//배근방법 - 뛰움각도 or 간격각도
	double dCoverThickness;	//피복두께
	UINT nRebarNum;			//철근 개수
	char strRebarDia[8];     //철근 직경
	double dUserRebarDia;	//유저직접입력(철근 직경)
	BOOL bEmbedmentRebar;	//보강 단면에서의 철근단부 정착 여부

	void Get(T_RRSC_MAINBAR& rData)
	{
		rData.nPos = nPos;
		rData.nRebarPlacingType = nRebarPlacingType;
		rData.dOffsetDist = dOffsetDist;
		rData.dOffsetAngle = dOffsetAngle;
		rData.dCoverThickness = dCoverThickness;
		rData.nRebarNum = nRebarNum;
		ConvertCharStr(strRebarDia, rData.strRebarDia, sizeof(strRebarDia));
		rData.dUserRebarDia = dUserRebarDia;
		rData.bEmbedmentRebar = bEmbedmentRebar;
	}

	void Set(/*const*/ T_RRSC_MAINBAR& data)
	{
		nPos = data.nPos;
		nRebarPlacingType = data.nRebarPlacingType;
		dOffsetDist = data.dOffsetDist;
		dOffsetAngle = data.dOffsetAngle;
		dCoverThickness = data.dCoverThickness;
		nRebarNum = data.nRebarNum;
		ConvertStrChar(data.strRebarDia, strRebarDia, sizeof(strRebarDia));
		dUserRebarDia = data.dUserRebarDia;
		bEmbedmentRebar = data.bEmbedmentRebar;
	}
};
#pragma endregion

#pragma region // T_RGSC_MAINBAR (변수 구조체) : 임의형상단면 축방향 철근 배근 정보
struct T_RGSC_MAINBAR
{
	int     nType;          // 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	int     nRefY;          // 0:Left 1:Center 2:Right (Tapered 에서 사용)
	int     nRefZ;          // 0:Top 1:Center 2:Bottom (Tapered 에서 사용)
	T_POINT_SSM Start;      // Start Point, Corner1 Point
	T_POINT_SSM Center;     // Center Point
	tPointSsmArray aPassage; // Passage Point(y,z)
	T_POINT_SSM End;        // End Point, Corner2 Point
	double  dRadius;        // Radius
	int     nNumorCTC;      // 0:Num. 1:CTC
	int     nNum;           // Num
	double  dCTC;           // CTC 
	BOOL    bEdgeBar;       // 가장자리 철근
	CString strBar;         // 철근
	double dUserRebarDia;	//사용자 입력 직경

	int     nStandRef;      // 배근 기준점 0:시점 1:Center 2:종점
	int     nPart;          // 0:Part 구분 없는 단면
	// 1:Composite 의 Part1
	// 2:Composite 의 Part2

	int     nInputMethod;   // A(기존), B(신규)
	int     nRefY_A;        // 0:Centroid 1:Left
	int     nRefZ_A;        // 0:Top 1:Bottom
	double  dRefY_A;
	double  dRefZ_A;
	double  dSpacing;

	// �߰� 860
	int     nNumTopBot;     // Num. Top & Bot
	int     nNumLeftRight;  // Num. Left & Right
	double  dRotateAngle;

	BOOL bEmbedmentRebar;	//보강 단면에서의 철근단부 정착 여부

	void Initialize()
	{
		nType = 1;
		nRefY = 0;
		nRefZ = 0;
		Start.Initialize();
		Center.Initialize();
		aPassage.RemoveAll();
		End.Initialize();
		dRadius = 0.;
		nNumorCTC = 0;
		nNum = 0;
		dCTC = 0.;
		bEdgeBar = FALSE;
		strBar = "";
		dUserRebarDia = 0;

		nStandRef = 0;
		nPart = 0;

		nInputMethod = 0;
		nRefY_A = 0;
		nRefZ_A = 0;
		dRefY_A = 0.0;
		dRefZ_A = 0.0;
		dSpacing = 0.0;

		nNumTopBot = 0;
		nNumLeftRight = 0;
		dRotateAngle = 0.0;

		bEmbedmentRebar = FALSE;
	}

	T_RGSC_MAINBAR() { Initialize(); }
	T_RGSC_MAINBAR(const T_RGSC_MAINBAR& rData) { *this = rData; }
	T_RGSC_MAINBAR& operator=(const T_RGSC_MAINBAR& rData)
	{
		nType = rData.nType;
		nRefY = rData.nRefY;
		nRefZ = rData.nRefZ;
		Start = rData.Start;
		Center = rData.Center;
		aPassage.Copy(rData.aPassage);
		End = rData.End;
		dRadius = rData.dRadius;
		nNumorCTC = rData.nNumorCTC;
		nNum = rData.nNum;
		dCTC = rData.dCTC;
		bEdgeBar = rData.bEdgeBar;
		strBar = rData.strBar;
		dUserRebarDia = rData.dUserRebarDia;

		nStandRef = rData.nStandRef;
		nPart = rData.nPart;

		nInputMethod = rData.nInputMethod;
		nRefY_A = rData.nRefY_A;
		nRefZ_A = rData.nRefZ_A;
		dRefY_A = rData.dRefY_A;
		dRefZ_A = rData.dRefZ_A;
		dSpacing = rData.dSpacing;

		nNumTopBot = rData.nNumTopBot;
		nNumLeftRight = rData.nNumLeftRight;
		dRotateAngle = rData.dRotateAngle;

		bEmbedmentRebar = rData.bEmbedmentRebar;
		return (*this);
	}

	bool operator==(const T_RGSC_MAINBAR& rhs) const
	{
		// Compare integer members
		if (nType != rhs.nType) return false;
		if (nRefY != rhs.nRefY) return false;
		if (nRefZ != rhs.nRefZ) return false;
		if (nNumorCTC != rhs.nNumorCTC) return false;
		if (nNum != rhs.nNum) return false;
		if (bEdgeBar != rhs.bEdgeBar) return false;
		if (nStandRef != rhs.nStandRef) return false;
		if (nPart != rhs.nPart) return false;
		if (nInputMethod != rhs.nInputMethod) return false;
		if (nRefY_A != rhs.nRefY_A) return false;
		if (nRefZ_A != rhs.nRefZ_A) return false;
		if (nNumTopBot != rhs.nNumTopBot) return false;
		if (nNumLeftRight != rhs.nNumLeftRight) return false;
		if (bEmbedmentRebar != rhs.bEmbedmentRebar) return false;
		
		// Compare point structures
		if (Start.dPointY != rhs.Start.dPointY || Start.dPointZ != rhs.Start.dPointZ) return false;
		if (Center.dPointY != rhs.Center.dPointY || Center.dPointZ != rhs.Center.dPointZ) return false;
		if (End.dPointY != rhs.End.dPointY || End.dPointZ != rhs.End.dPointZ) return false;
		
		// Compare double members with epsilon tolerance
		const double epsilon = 1e-10;
		if (fabs(dRadius - rhs.dRadius) > epsilon) return false;
		if (fabs(dCTC - rhs.dCTC) > epsilon) return false;
		if (fabs(dUserRebarDia - rhs.dUserRebarDia) > epsilon) return false;
		if (fabs(dRefY_A - rhs.dRefY_A) > epsilon) return false;
		if (fabs(dRefZ_A - rhs.dRefZ_A) > epsilon) return false;
		if (fabs(dSpacing - rhs.dSpacing) > epsilon) return false;
		if (fabs(dRotateAngle - rhs.dRotateAngle) > epsilon) return false;
		
		// Compare CString
		if (strBar != rhs.strBar) return false;
		
		// Compare passage point array
		if (aPassage.GetSize() != rhs.aPassage.GetSize()) return false;
		for (int i = 0; i < aPassage.GetSize(); i++)
		{
			if (aPassage[i].dPointY != rhs.aPassage[i].dPointY || 
			    aPassage[i].dPointZ != rhs.aPassage[i].dPointZ)
				return false;
		}
		
		return true;
	}
	
	bool operator!=(const T_RGSC_MAINBAR& rhs) const
	{
		return !(*this == rhs);
	}
};
typedef CArray<T_RGSC_MAINBAR, T_RGSC_MAINBAR&> tRgscMainbarArray;

struct T_RGSC_MAINBAR_CH
{
	int     nType;
	int     nRefY;
	int     nRefZ;
	T_POINT_SSM Start;
	T_POINT_SSM Center;
	tPointSsmArray aPassage;
	T_POINT_SSM End;
	double  dRadius;
	int     nNumorCTC;
	int     nNum;
	double  dCTC;
	BOOL    bEdgeBar;
	char	strBar[8];
	double dUserRebarDia;//사용자 입력 직경

	int     nStandRef;
	int     nPart;
	int     nInputMethod;
	int     nRefY_A;
	int     nRefZ_A;
	double  dRefY_A;
	double  dRefZ_A;
	double  dSpacing;
	int     nNumTopBot;     // Num. Top & Bot
	int     nNumLeftRight;  // Num. Left & Right
	double  dRotateAngle;
	BOOL	bEmbedmentRebar;	//보강 단면에서의 철근단부 정착 여부

	void Get(T_RGSC_MAINBAR& rData)
	{
		rData.nType = nType;
		rData.nRefY = nRefY;
		rData.nRefZ = nRefZ;
		rData.Start = Start;
		rData.Center = Center;
		rData.aPassage.Copy(aPassage);
		rData.End = End;
		rData.dRadius = dRadius;
		rData.nNumorCTC = nNumorCTC;
		rData.nNum = nNum;
		rData.dCTC = dCTC;
		rData.bEdgeBar = bEdgeBar;
		ConvertCharStr(strBar, rData.strBar, sizeof(strBar));
		rData.dUserRebarDia = dUserRebarDia;

		rData.nStandRef = nStandRef;
		rData.nPart = nPart;
		rData.nInputMethod = nInputMethod;
		rData.nRefY_A = nRefY_A;
		rData.nRefZ_A = nRefZ_A;
		rData.dRefY_A = dRefY_A;
		rData.dRefZ_A = dRefZ_A;
		rData.dSpacing = dSpacing;
		rData.nNumTopBot = nNumTopBot;
		rData.nNumLeftRight = nNumLeftRight;
		rData.dRotateAngle = dRotateAngle;
		rData.bEmbedmentRebar = bEmbedmentRebar;
	}
	void Set(/*const*/ T_RGSC_MAINBAR& data)
	{
		nType = data.nType;
		nRefY = data.nRefY;
		nRefZ = data.nRefZ;
		Start = data.Start;
		Center = data.Center;
		aPassage.Copy(data.aPassage);
		End = data.End;
		dRadius = data.dRadius;
		nNumorCTC = data.nNumorCTC;
		nNum = data.nNum;
		dCTC = data.dCTC;
		bEdgeBar = data.bEdgeBar;
		ConvertStrChar(data.strBar, strBar, sizeof(strBar));
		dUserRebarDia = data.dUserRebarDia;

		nStandRef = data.nStandRef;
		nPart = data.nPart;
		nInputMethod = data.nInputMethod;
		nRefY_A = data.nRefY_A;
		nRefZ_A = data.nRefZ_A;
		dRefY_A = data.dRefY_A;
		dRefZ_A = data.dRefZ_A;
		dSpacing = data.dSpacing;
		nNumTopBot = data.nNumTopBot;
		nNumLeftRight = data.nNumLeftRight;
		dRotateAngle = data.dRotateAngle;
		bEmbedmentRebar = data.bEmbedmentRebar;
	}
};

struct T_RGSC_MAINBAR_CH_RW
{
	int     nType;
	int     nRefY;
	int     nRefZ;
	T_POINT_SSM Start;
	T_POINT_SSM Center;
	int     nSizePassage;
	T_POINT_SSM End;
	double  dRadius;
	int     nNumorCTC;
	int     nNum;
	double  dCTC;
	BOOL    bEdgeBar;
	char	strBar[8];
	double dUserRebarDia;	//사용자 입력 직경

	int     nStandRef;
	int     nPart;
	int     nInputMethod;
	int     nRefY_A;
	int     nRefZ_A;
	double  dRefY_A;
	double  dRefZ_A;
	double  dSpacing;
	int     nNumTopBot;     // Num. Top & Bot
	int     nNumLeftRight;  // Num. Left & Right
	double  dRotateAngle;
	BOOL bEmbedmentRebar;	//보강 단면에서의 철근단부 정착 여부

	void Get(T_RGSC_MAINBAR_CH& rData)
	{
		rData.nType = nType;
		rData.nRefY = nRefY;
		rData.nRefZ = nRefZ;
		rData.Start = Start;
		rData.Center = Center;
		rData.aPassage.SetSize(nSizePassage);
		rData.End = End;
		rData.dRadius = dRadius;
		rData.nNumorCTC = nNumorCTC;
		rData.nNum = nNum;
		rData.dCTC = dCTC;
		rData.bEdgeBar = bEdgeBar;
		memcpy(rData.strBar, strBar, sizeof(strBar));
		rData.dUserRebarDia = dUserRebarDia;

		rData.nStandRef = nStandRef;
		rData.nPart = nPart;
		rData.nInputMethod = nInputMethod;
		rData.nRefY_A = nRefY_A;
		rData.nRefZ_A = nRefZ_A;
		rData.dRefY_A = dRefY_A;
		rData.dRefZ_A = dRefZ_A;
		rData.dSpacing = dSpacing;
		rData.nNumTopBot = nNumTopBot;
		rData.nNumLeftRight = nNumLeftRight;
		rData.dRotateAngle = dRotateAngle;
		rData.bEmbedmentRebar = bEmbedmentRebar;
	}
	void Set(T_RGSC_MAINBAR_CH& Data)
	{
		nType = Data.nType;
		nRefY = Data.nRefY;
		nRefZ = Data.nRefZ;
		Start = Data.Start;
		Center = Data.Center;
		nSizePassage = (int)Data.aPassage.GetSize();
		End = Data.End;
		dRadius = Data.dRadius;
		nNumorCTC = Data.nNumorCTC;
		nNum = Data.nNum;
		dCTC = Data.dCTC;
		bEdgeBar = Data.bEdgeBar;
		memcpy(strBar, Data.strBar, sizeof(strBar));
		dUserRebarDia = Data.dUserRebarDia;

		nStandRef = Data.nStandRef;
		nPart = Data.nPart;
		nInputMethod = Data.nInputMethod;
		nRefY_A = Data.nRefY_A;
		nRefZ_A = Data.nRefZ_A;
		dRefY_A = Data.dRefY_A;
		dRefZ_A = Data.dRefZ_A;
		dSpacing = Data.dSpacing;
		nNumTopBot = Data.nNumTopBot;
		nNumLeftRight = Data.nNumLeftRight;
		dRotateAngle = Data.dRotateAngle;
		bEmbedmentRebar = Data.bEmbedmentRebar;
	}
};
#pragma endregion

#pragma region // T_RBAR_STIRRUP (변수 구조체) : 정형&임의형상 단면 횡방향 철근(전단철근) 배근 정보
struct T_STIRRUP_ITEM
{
	double dLenghD; // 유효 길이
	double dSpacing;		//간격
	UINT nNum;				//개수
	double dAngle;			//각도(Degree)
	CString strRebarDia;	//철근 직경
	double dAs;				//횡구속 철근 1개의 단면적
	double dLs;				//횡구속 철근 1개의 단면 2차 모멘트
	double dBwRailway;  // bw- 철도교

	BOOL bPcSteel;						//PC강봉 유무
	double dPcSteelLenghD;		// PC강봉 - 유효 길이
	double dPcSteelSpacing;		//간격
	double dPcSteelDAs;				// 단면적
	double dPcSteelYield;			// 저감항복강도
	
	BOOL bAw;			//전단철근 단면적 별도 입력 유무
	double dAw;			//전단철근 전체 단면적

	void Initialize()
	{
		dLenghD = 0;
		dSpacing = 0;
		nNum = 0;
		dAngle = 0;
		strRebarDia = _T("");
		dAs = 0;
		dLs = 0;
		dBwRailway = 0,
		bPcSteel = FALSE;
		dPcSteelLenghD = 0;
		dPcSteelSpacing = 0;
		dPcSteelDAs = 0;
		dPcSteelYield = 0;

		bAw = FALSE;
		dAw = 0;
	}

	T_STIRRUP_ITEM() { Initialize(); }
	T_STIRRUP_ITEM(const T_STIRRUP_ITEM& src) { *this = src; }
	T_STIRRUP_ITEM& operator=(const T_STIRRUP_ITEM& src)
	{
		if (this != &src) {
			dLenghD = src.dLenghD;
			dSpacing = src.dSpacing;
			nNum = src.nNum;
			dAngle = src.dAngle;
			strRebarDia = src.strRebarDia;
			dAs = src.dAs;
			dLs = src.dLs;
			dBwRailway = src.dBwRailway;
			bPcSteel = src.bPcSteel;
			dPcSteelLenghD = src.dPcSteelLenghD;
			dPcSteelSpacing = src.dPcSteelSpacing;
			dPcSteelDAs = src.dPcSteelDAs;
			dPcSteelYield = src.dPcSteelYield;
			bAw= src.bAw;
			dAw = src.dAw;
		}
		return *this;
	}
};

struct T_STIRRUP_ITEM_CH
{
	double dLenghD; // 유효 길이

	double dSpacing;
	UINT nNum;
	double dAngle;
	char strRebarDia[8];
	double dAs;
	double dLs;
	double dBwRailway;

	BOOL bPcSteel;						//PC강봉 유무
	double dPcSteelLenghD;		// PC강봉 - 유효 길이
	double dPcSteelSpacing;		//간격
	double dPcSteelDAs;				// 단면적
	double dPcSteelYield;			// 저감항복강도

	BOOL bAw;
	double dAw;

	void Get(T_STIRRUP_ITEM& rData)
	{
		rData.dLenghD = dLenghD;
		rData.dSpacing = dSpacing;
		rData.nNum = nNum;
		rData.dAngle = dAngle;
		ConvertCharStr(strRebarDia, rData.strRebarDia, sizeof(strRebarDia));
		rData.dAs = dAs;
		rData.dLs = dLs;
		rData.dBwRailway = dBwRailway;
		rData.bPcSteel = bPcSteel;
		rData.dPcSteelLenghD = dPcSteelLenghD;
		rData.dPcSteelSpacing = dPcSteelSpacing;
		rData.dPcSteelDAs = dPcSteelDAs;
		rData.dPcSteelYield = dPcSteelYield;
		rData.bAw = bAw;
		rData.dAw = dAw;
	}
	void Set(/*const*/ T_STIRRUP_ITEM& data)
	{
		dLenghD = data.dLenghD;
		dSpacing = data.dSpacing;
		nNum = data.nNum;
		dAngle = data.dAngle;
		ConvertStrChar(data.strRebarDia, strRebarDia, sizeof(strRebarDia));
		dAs = data.dAs;
		dLs = data.dLs;
		dBwRailway = data.dBwRailway;
		bPcSteel = data.bPcSteel;
		dPcSteelLenghD = data.dPcSteelLenghD;
		dPcSteelSpacing = data.dPcSteelSpacing;
		dPcSteelDAs = data.dPcSteelDAs;
		dPcSteelYield = data.dPcSteelYield;
		bAw = data.bAw;
		dAw = data.dAw;
	}
};

struct T_RBAR_STIRRUP
{
	T_STIRRUP_ITEM StirrupY;
	T_STIRRUP_ITEM StirrupZ;
	int nRebarMethod;	// 고려방법 : 0:보강단면의 환산 횡구속철근 적용 , 1: 기설부/보강부 별도 적용, 2: 보강부 값 일괄 적용

	void Initialize()
	{
		StirrupY.Initialize();
		StirrupZ.Initialize();
		nRebarMethod = 0;
	}
	enum eRebarMethod
	{
		kRebarMethod1,	// 보강단면의 환산 횡구속철근 적용
		kRebarMethod2,	// 기설부/보강부 별도 적용
		kRebarMethod3,	// 보강부 값 일괄 적용
	};

	T_RBAR_STIRRUP() { Initialize(); }
	T_RBAR_STIRRUP(const T_RBAR_STIRRUP& src) { *this = src; }
	T_RBAR_STIRRUP& operator=(const T_RBAR_STIRRUP& src)
	{
		if (this != &src) {
			StirrupY = src.StirrupY;
			StirrupZ = src.StirrupZ;
			nRebarMethod = src.nRebarMethod;
		}
		return *this;
	}
};
typedef CArray<T_RBAR_STIRRUP, T_RBAR_STIRRUP&> tRbarStirrupArray;

struct T_RBAR_STIRRUP_CH
{
	T_STIRRUP_ITEM_CH StirrupY;
	T_STIRRUP_ITEM_CH StirrupZ;
	int nRebarMethod;	// 고려방법 : 0:보강단면의 환산 횡구속철근 적용 , 1: 기설부/보강부 별도 적용, 2: 보강부 값 일괄 적용


	void Get(T_RBAR_STIRRUP& rData)
	{
		StirrupY.Get(rData.StirrupY);
		StirrupZ.Get(rData.StirrupZ);
		rData.nRebarMethod = nRebarMethod;
	}
	void Set(/*const*/ T_RBAR_STIRRUP& data)
	{
		StirrupY.Set(data.StirrupY);
		StirrupZ.Set(data.StirrupZ);
		nRebarMethod = data.nRebarMethod;
	}
};
#pragma endregion

#pragma region //T_RPSC_SSM_D (DB 구조체) - Rebar PC Section - Section Manager
#define T_RPSC_SSM_K T_SECT_K
struct T_RPSC_SSM_D
{
	BOOL bMbarSameJ;   // I==J:TRUE, I!=J:FALSE
	BOOL bSbarSameJ;   // I==J:TRUE, I!=J:FALSE
	tRpscMbarSsmArray Mbar[2];
	T_RPSC_SBAR_SSM Sbar[2];
	BOOL bCracked;  // Cracked Section - Composite Section 일때 사용
	double dConcFact; // Concrete Contribution Factor

	BOOL bCrackH;
	double dCrackHTop;
	double dCrackHBot;

	void Initialize()
	{
		bMbarSameJ = bSbarSameJ = TRUE;
		Mbar[0].RemoveAll();
		Mbar[1].RemoveAll();
		Sbar[0].Initialize();
		Sbar[1].Initialize();
		bCracked = FALSE;
		dConcFact = 1.0;
		bCrackH = FALSE;
		dCrackHTop = 0.0;
		dCrackHBot = 0.0;
	}

	T_RPSC_SSM_D() { Initialize(); }
	T_RPSC_SSM_D(const T_RPSC_SSM_D& rData) { *this = rData; }
	T_RPSC_SSM_D& operator=(const T_RPSC_SSM_D& rData)
	{
		bMbarSameJ = rData.bMbarSameJ;
		bSbarSameJ = rData.bSbarSameJ;
		Mbar[0].Copy(rData.Mbar[0]);
		Mbar[1].Copy(rData.Mbar[1]);
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
		dConcFact = rData.dConcFact;
		bCrackH = rData.bCrackH;
		dCrackHTop = rData.dCrackHTop;
		dCrackHBot = rData.dCrackHBot;
		return *this;
	}
};

struct T_RPSC_SSM_D_CH
{
	BOOL bMbarSameJ;    // I==J:TRUE, I!=J:FALSE
	BOOL bSbarSameJ;    // I==J:TRUE, I!=J:FALSE
	CArray<T_RPSC_MBAR_SSM_CH, T_RPSC_MBAR_SSM_CH&> Mbar[2];
	T_RPSC_SBAR_SSM Sbar[2];
	BOOL bCracked;   // Cracked Section
	double dConcFact; // Concrete Contribution Factor
	BOOL bCrackH;
	double dCrackHTop;
	double dCrackHBot;

	void ConvertToString(T_RPSC_SSM_D& rData)
	{
		rData.bMbarSameJ = bMbarSameJ;
		rData.bSbarSameJ = bSbarSameJ;
		int nCount = (int)Mbar[0].GetSize();
		rData.Mbar[0].SetSize(nCount);
		for (int i = 0; i < nCount; i++) Mbar[0][i].ConvertToString(rData.Mbar[0][i]);
		nCount = (int)Mbar[1].GetSize();
		rData.Mbar[1].SetSize(nCount);
		for (int i = 0; i < nCount; i++) Mbar[1][i].ConvertToString(rData.Mbar[1][i]);
		rData.Sbar[0] = Sbar[0];
		rData.Sbar[1] = Sbar[1];
		rData.bCracked = bCracked;
		rData.dConcFact = dConcFact;
		rData.bCrackH = bCrackH;
		rData.dCrackHTop = dCrackHTop;
		rData.dCrackHBot = dCrackHBot;
	}
	void ConvertToChar(T_RPSC_SSM_D& rData)
	{
		bMbarSameJ = rData.bMbarSameJ;
		bSbarSameJ = rData.bSbarSameJ;
		int nCount = (int)rData.Mbar[0].GetSize();
		Mbar[0].SetSize(nCount);
		for (int i = 0; i < nCount; i++) Mbar[0][i].ConvertToChar(rData.Mbar[0][i]);
		nCount = (int)rData.Mbar[1].GetSize();
		Mbar[1].SetSize(nCount);
		for (int i = 0; i < nCount; i++) Mbar[1][i].ConvertToChar(rData.Mbar[1][i]);
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
		dConcFact = rData.dConcFact;
		bCrackH = rData.bCrackH;
		dCrackHTop = rData.dCrackHTop;
		dCrackHBot = rData.dCrackHBot;
	}
};

struct T_RPSC_SSM_UDRD_D
{
	T_RPSC_SSM_K key;
	T_RPSC_SSM_D data;
};

struct T_RPSC_SSM_UDRD_D_CH
{
	T_RPSC_SSM_K key;
	T_RPSC_SSM_D_CH data;
};

struct T_RPSC_SSM_UDRD_D_CH_RW_832
{
	T_RPSC_SSM_K key;
	BOOL bMbarSameJ;
	BOOL bSbarSameJ;
	int  nCountMbar[2];
	T_RPSC_SBAR_SSM_925 Sbar[2];
	BOOL bCracked;
};

struct T_RPSC_SSM_UDRD_D_CH_RW_925
{
	T_RPSC_SSM_K key;
	BOOL bMbarSameJ;
	BOOL bSbarSameJ;
	int  nCountMbar[2];
	T_RPSC_SBAR_SSM_925 Sbar[2];
	BOOL bCracked;
	double dConcFact;
};

struct T_RPSC_SSM_UDRD_D_CH_RW_940
{
	T_RPSC_SSM_K key;
	BOOL bMbarSameJ;
	BOOL bSbarSameJ;
	int  nCountMbar[2];
	T_RPSC_SBAR_SSM Sbar[2];
	BOOL bCracked;
	double dConcFact;
};

struct T_RPSC_SSM_UDRD_D_CH_RW
{
	T_RPSC_SSM_K key;
	BOOL bMbarSameJ;
	BOOL bSbarSameJ;
	int  nCountMbar[2];
	T_RPSC_SBAR_SSM Sbar[2];
	BOOL bCracked;
	double dConcFact;

	BOOL   bCrackH;
	double dCrackHTop;
	double dCrackHBot;

	void GetRpscSsm(T_RPSC_SSM_K& rKey, T_RPSC_SSM_D_CH& rData)
	{
		rKey = key;
		rData.bMbarSameJ = bMbarSameJ;
		rData.bSbarSameJ = bSbarSameJ;
		rData.Sbar[0] = Sbar[0];
		rData.Sbar[1] = Sbar[1];
		rData.bCracked = bCracked;
		rData.dConcFact = dConcFact;
		rData.bCrackH = bCrackH;
		rData.dCrackHTop = dCrackHTop;
		rData.dCrackHBot = dCrackHBot;
	}
	void SetRpscSsm(const T_RPSC_SSM_K Key, const T_RPSC_SSM_D_CH& rData)
	{
		key = Key;
		bMbarSameJ = rData.bMbarSameJ;
		bSbarSameJ = rData.bSbarSameJ;
		nCountMbar[0] = (int)rData.Mbar[0].GetSize();
		nCountMbar[1] = (int)rData.Mbar[1].GetSize();
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
		dConcFact = rData.dConcFact;
		bCrackH = rData.bCrackH;
		dCrackHTop = rData.dCrackHTop;
		dCrackHBot = rData.dCrackHBot;
	}
	void Convert940(T_RPSC_SSM_UDRD_D_CH_RW_940& rData)
	{
		key = rData.key;
		bMbarSameJ = rData.bMbarSameJ;
		bSbarSameJ = rData.bSbarSameJ;
		nCountMbar[0] = rData.nCountMbar[0];
		nCountMbar[1] = rData.nCountMbar[1];
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
		dConcFact = rData.dConcFact;
		bCrackH = FALSE;
		dCrackHTop = 0.0;
		dCrackHBot = 0.0;
	}
	void Convert925(T_RPSC_SSM_UDRD_D_CH_RW_925& rData)
	{
		key = rData.key;
		bMbarSameJ = rData.bMbarSameJ;
		bSbarSameJ = rData.bSbarSameJ;
		nCountMbar[0] = rData.nCountMbar[0];
		nCountMbar[1] = rData.nCountMbar[1];
		Sbar[0].Convert925(rData.Sbar[0]);
		Sbar[1].Convert925(rData.Sbar[1]);
		bCracked = rData.bCracked;
		dConcFact = rData.dConcFact;
		bCrackH = FALSE;
		dCrackHTop = 0.0;
		dCrackHBot = 0.0;
	}
	void Convert832(T_RPSC_SSM_UDRD_D_CH_RW_832& rData)
	{
		key = rData.key;
		bMbarSameJ = rData.bMbarSameJ;
		bSbarSameJ = rData.bSbarSameJ;
		nCountMbar[0] = rData.nCountMbar[0];
		nCountMbar[1] = rData.nCountMbar[1];
		Sbar[0].Convert925(rData.Sbar[0]);
		Sbar[1].Convert925(rData.Sbar[1]);
		bCracked = rData.bCracked;
		dConcFact = 1.0;
		bCrackH = FALSE;
		dCrackHTop = 0.0;
		dCrackHBot = 0.0;
	}
};

struct T_RPSC_SSM_UNIT
{
	T_RPSC_SSM_UNIT()
	{
		dPoint = D_UNITSYS_BASE_LENGTH;
		dCTC = D_UNITSYS_BASE_LENGTH;
		dRadius = D_UNITSYS_BASE_LENGTH;
		dRefY_A = D_UNITSYS_BASE_LENGTH;
		dRefZ_A = D_UNITSYS_BASE_LENGTH;
		dSpacing = D_UNITSYS_BASE_LENGTH;
		dDiagonalPitch = D_UNITSYS_BASE_LENGTH;
		dDiagonalAngle = D_UNITSYS_BASE_DEGREE;
		dDiagonalAw = D_UNITSYS_BASE_AREA;
		dSteelBarPitch = D_UNITSYS_BASE_LENGTH;
		dSteelBarAngle = D_UNITSYS_BASE_DEGREE;
		dSteelBarAp = D_UNITSYS_BASE_AREA;
		dSteelBarPe = D_UNITSYS_BASE_FORCE;
		dTorsionalPitch = D_UNITSYS_BASE_LENGTH;
		dTorsionalAwt = D_UNITSYS_BASE_AREA;
		dTorsionalAlt = D_UNITSYS_BASE_AREA;
		dStirPitch = D_UNITSYS_BASE_LENGTH;
		dStirAw = D_UNITSYS_BASE_AREA;
		dLbarThick = D_UNITSYS_BASE_LENGTH;
		dAk = D_UNITSYS_BASE_AREA;
		dUk = D_UNITSYS_BASE_LENGTH;
		dAkCover = D_UNITSYS_BASE_AREA;
		dUkCover = D_UNITSYS_BASE_LENGTH;
		dCrackHTop = D_UNITSYS_BASE_LENGTH;
		dCrackHBot = D_UNITSYS_BASE_LENGTH;
	}
	int dPoint;
	int dCTC;
	int dRadius;
	int dRefY_A;
	int dRefZ_A;
	int dSpacing;
	int dDiagonalPitch;
	int dDiagonalAngle;
	int dDiagonalAw;
	int dSteelBarPitch;
	int dSteelBarAngle;
	int dSteelBarAp;
	int dSteelBarPe;
	int dTorsionalPitch;
	int dTorsionalAwt;
	int dTorsionalAlt;
	int dStirPitch;
	int dStirAw;
	int dLbarThick;
	int dAk;
	int dUk;
	int dAkCover;
	int dUkCover;
	int dCrackHTop;
	int dCrackHBot;
};
#define HASHSIZERPSCSSM HASHSIZESECT
#pragma endregion

#pragma region 
#define T_RPSV_SSM_K T_SECV_K
struct T_RPSV_SSM_D
{
	BOOL bMbarSameJ;   // I==J:TRUE, I!=J:FALSE
	BOOL bSbarSameJ;   // I==J:TRUE, I!=J:FALSE
	CArray<T_RPSC_MBAR_SSM, T_RPSC_MBAR_SSM&> Mbar[2];
	T_RPSC_SBAR_SSM Sbar[2];
	BOOL bCracked;  // Cracked Section - Composite Section 특정 조건
	double dConcFact; // Concrete Contribution Factor

	void Initialize()
	{
		bMbarSameJ = bSbarSameJ = TRUE;
		Mbar[0].RemoveAll();
		Mbar[1].RemoveAll();
		Sbar[0].Initialize();
		Sbar[1].Initialize();
		bCracked = FALSE;
		dConcFact = 1.0;
	}

	T_RPSV_SSM_D() {}
	T_RPSV_SSM_D(const T_RPSV_SSM_D& rData) { *this = rData; }
	T_RPSV_SSM_D& operator=(const T_RPSV_SSM_D& rData)
	{
		bMbarSameJ = rData.bMbarSameJ;
		bSbarSameJ = rData.bSbarSameJ;
		Mbar[0].Copy(rData.Mbar[0]);
		Mbar[1].Copy(rData.Mbar[1]);
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
		dConcFact = rData.dConcFact;
		return *this;
	}


	void ConvertToRpscSSM(T_RPSC_SSM_D& rData)
	{
		rData.Initialize();

		rData.bMbarSameJ = bMbarSameJ;
		rData.bSbarSameJ = bSbarSameJ;
		rData.Mbar[0].Copy(Mbar[0]);
		rData.Mbar[1].Copy(Mbar[1]);
		rData.Sbar[0] = Sbar[0];
		rData.Sbar[1] = Sbar[1];
		rData.bCracked = rData.bCracked;
		rData.dConcFact = rData.dConcFact;
	}
	void ConvertToRpsvSSM(T_RPSC_SSM_D& rData)
	{
		Initialize();

		bMbarSameJ = rData.bMbarSameJ;
		bSbarSameJ = rData.bSbarSameJ;
		Mbar[0].Copy(rData.Mbar[0]);
		Mbar[1].Copy(rData.Mbar[1]);
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
		dConcFact = rData.dConcFact;
	}
};
struct T_RPSV_SSM_UDRD_D
{
	T_RPSV_SSM_K key;
	T_RPSV_SSM_D data;
};
struct T_RPSV_SSM_UNIT
{
	T_RPSV_SSM_UNIT()
	{
	}
};
#define HASHSIZERPSVSSM HASHSIZESECV


#pragma region //T_RRSC_SSM_D (DB 구조체) - Regular Section - Section Manager v970(?)추가
#define T_RRSC_SSM_K unsigned int
struct T_RRSC_SSM_D
{
	int nSameRebarPos;	// 축방향철근 같은배근 위치	T_RRSC_SSM_D::eSameRebarPos
	tRrscMainbarArray Anchor;
	tRrscMainbarArray MbarI;
	tRrscMainbarArray MbarM;
	tRrscMainbarArray MbarJ;

	tRbarStirrupArray SbarI;
	tRbarStirrupArray SbarM;
	tRbarStirrupArray SbarJ;

	void Initialize()
	{
		nSameRebarPos = 0;
		Anchor.RemoveAll();
		MbarI.RemoveAll();
		MbarM.RemoveAll();
		MbarJ.RemoveAll();
		SbarI.RemoveAll();
		SbarM.RemoveAll();
		SbarJ.RemoveAll();
	}

	T_RRSC_SSM_D() { Initialize(); }
	T_RRSC_SSM_D(const T_RRSC_SSM_D& src) { *this = src; }
	T_RRSC_SSM_D& operator=(const T_RRSC_SSM_D& src)
	{
		if (this != &src) {
			nSameRebarPos = src.nSameRebarPos;
			Anchor.Copy(src.Anchor);
			MbarI.Copy(src.MbarI);
			MbarM.Copy(src.MbarM);
			MbarJ.Copy(src.MbarJ);
			SbarI.Copy(src.SbarI);
			SbarM.Copy(src.SbarM);
			SbarJ.Copy(src.SbarJ);
		}
		return *this;
	}

	enum eSameRebarPos
	{
		kUnkowun,
		kSameNot,	//I,M,J 다른 배근
		kSameAll,	//I,M,J 같은 배근
		kSameIJ,	//I,J 같은 배근
	};
};

struct T_RRSC_SSM_UNIT
{
	T_RRSC_SSM_UNIT()
	{
		Area = D_UNITSYS_BASE_AREA;
		Angle = D_UNITSYS_BASE_DEGREE;
		Length = D_UNITSYS_BASE_LENGTH;
		MomentOfInertia = D_UNITSYS_BASE_STIF;
		MomentYieldStress = D_UNITSYS_BASE_STRESS;
	}
	int Area;
	int Angle;
	int Length;
	int MomentOfInertia;
	int MomentYieldStress;
};

struct T_RRSC_SSM_UDRD_D
{
	T_RRSC_SSM_K key;
	T_RRSC_SSM_D data;
};

struct T_RRSC_SSM_D_CH
{
	int nSameRebarPos;
	CArray<T_RRSC_MAINBAR_CH, T_RRSC_MAINBAR_CH&> Anchor;
	CArray<T_RRSC_MAINBAR_CH, T_RRSC_MAINBAR_CH&> MbarI;
	CArray<T_RRSC_MAINBAR_CH, T_RRSC_MAINBAR_CH&> MbarM;
	CArray<T_RRSC_MAINBAR_CH, T_RRSC_MAINBAR_CH&> MbarJ;

	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarI;
	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarM;
	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarJ;

	

	void Get(T_RRSC_SSM_D& rData)
	{
		rData.nSameRebarPos = nSameRebarPos;
		rData.Anchor.SetSize(Anchor.GetSize());
		for (int i = 0; i < Anchor.GetSize(); i++) Anchor[i].Get(rData.Anchor[i]);
		rData.MbarI.SetSize(MbarI.GetSize());
		for (int i = 0; i < MbarI.GetSize(); i++) MbarI[i].Get(rData.MbarI[i]);
		rData.MbarI.SetSize(MbarM.GetSize());
		for (int i = 0; i < MbarM.GetSize(); i++) MbarM[i].Get(rData.MbarM[i]);
		rData.MbarJ.SetSize(MbarJ.GetSize());
		for (int i = 0; i < MbarJ.GetSize(); i++) MbarJ[i].Get(rData.MbarJ[i]);
		rData.SbarI.SetSize(SbarI.GetSize());
		for (int i = 0; i < SbarI.GetSize(); i++) SbarI[i].Get(rData.SbarI[i]);
		rData.SbarM.SetSize(SbarM.GetSize());
		for (int i = 0; i < SbarM.GetSize(); i++) SbarM[i].Get(rData.SbarM[i]);
		rData.SbarJ.SetSize(SbarJ.GetSize());
		for (int i = 0; i < SbarJ.GetSize(); i++) SbarJ[i].Get(rData.SbarJ[i]);
	}

	void Set(/*const*/ T_RRSC_SSM_D& data)
	{
		nSameRebarPos = data.nSameRebarPos;
		Anchor.SetSize(data.Anchor.GetSize());
		for (int i = 0; i < data.Anchor.GetSize(); i++) Anchor[i].Set(data.Anchor[i]);
		MbarI.SetSize(data.MbarI.GetSize());
		for (int i = 0; i < data.MbarI.GetSize(); i++) MbarI[i].Set(data.MbarI[i]);
		MbarM.SetSize(data.MbarM.GetSize());
		for (int i = 0; i < data.MbarM.GetSize(); i++) MbarM[i].Set(data.MbarM[i]);
		MbarJ.SetSize(data.MbarJ.GetSize());
		for (int i = 0; i < data.MbarJ.GetSize(); i++) MbarJ[i].Set(data.MbarJ[i]);
		SbarI.SetSize(data.SbarI.GetSize());
		for (int i = 0; i < data.SbarI.GetSize(); i++) SbarI[i].Set(data.SbarI[i]);
		SbarM.SetSize(data.SbarM.GetSize());
		for (int i = 0; i < data.SbarM.GetSize(); i++) SbarM[i].Set(data.SbarM[i]);
		SbarJ.SetSize(data.SbarJ.GetSize());
		for (int i = 0; i < data.SbarJ.GetSize(); i++) SbarJ[i].Set(data.SbarJ[i]);
	}
};

struct T_RRSC_SSM_D_CH_RW
{
	T_RRSC_SSM_K key;
	int nSameRebarPos;
	int nSizeAnchor;
	int nSizeMbarI;
	int nSizeMbarM;
	int nSizeMbarJ;
	int nSizeSbarI;
	int nSizeSbarM;
	int nSizeSbarJ;

	void Get(T_RRSC_SSM_K& rKey, T_RRSC_SSM_D_CH& rData)
	{
		rKey = key;
		rData.nSameRebarPos = nSameRebarPos;
	}
	void Set(const T_RRSC_SSM_K Key, const T_RRSC_SSM_D_CH& data)
	{
		key = Key;
		nSameRebarPos = data.nSameRebarPos;
		nSizeAnchor = (int)data.Anchor.GetSize();
		nSizeMbarI = (int)data.MbarI.GetSize();
		nSizeMbarM = (int)data.MbarM.GetSize();
		nSizeMbarJ = (int)data.MbarJ.GetSize();
		nSizeSbarI = (int)data.SbarI.GetSize();
		nSizeSbarM = (int)data.SbarM.GetSize();
		nSizeSbarJ = (int)data.SbarJ.GetSize();
	}
};

#define HASHSIZERRSCSSM HASHSIZESECT
#pragma endregion

#pragma region //T_RGSC_SSM_D (DB 구조체) - General Section - Section Manager v970(?)추가
#define T_RGSC_SSM_K unsigned int
struct T_RGSC_SSM_D
{
	int nSameRebarPos;		// 횡방향철근 같은배근 위치	T_RGSC_SSM_D::eSameRebarPos
	tRgscMainbarArray Anchor;
	tRgscMainbarArray MbarI;
	tRgscMainbarArray MbarM;
	tRgscMainbarArray MbarJ;

	tRbarStirrupArray SbarI;
	tRbarStirrupArray SbarM;
	tRbarStirrupArray SbarJ;

	void Initialize()
	{
		nSameRebarPos = 0;
		Anchor.RemoveAll();
		MbarI.RemoveAll();
		MbarM.RemoveAll();
		MbarJ.RemoveAll();
		SbarI.RemoveAll();
		SbarM.RemoveAll();
		SbarJ.RemoveAll();
	}

	T_RGSC_SSM_D() { Initialize(); }
	T_RGSC_SSM_D(const T_RGSC_SSM_D& src) { *this = src; }
	T_RGSC_SSM_D& operator=(const T_RGSC_SSM_D& src)
	{
		if (this != &src) {
			nSameRebarPos = src.nSameRebarPos;
			Anchor.Copy(src.Anchor);
			MbarI.Copy(src.MbarI);
			MbarM.Copy(src.MbarM);
			MbarJ.Copy(src.MbarJ);
			SbarI.Copy(src.SbarI);
			SbarM.Copy(src.SbarM);
			SbarJ.Copy(src.SbarJ);
		}
		return *this;
	}

	enum eSameRebarPos
	{
		kUnkowun,
		kSameNot,	//I,M,J 다른 배근
		kSameAll,	//I,M,J 같은 배근
		kSameIJ,	//I,J 같은 배근
	};
	enum eRebarType
	{
		kUnKowunRebar,
		kReinRebar,			// 보강 철근
		kAnchorRebar,		// 앵커 철근
	};
};

struct T_RGSC_SSM_UNIT
{
	T_RGSC_SSM_UNIT()
	{
		Area = D_UNITSYS_BASE_AREA;
		Angle = D_UNITSYS_BASE_DEGREE;
		Length = D_UNITSYS_BASE_LENGTH;
		MomentOfInertia = D_UNITSYS_BASE_STIF;
		MomentYieldStress = D_UNITSYS_BASE_STRESS;
	}
	int Area;
	int Angle;
	int Length;
	int MomentOfInertia;
	int MomentYieldStress;
};

struct T_RGSC_SSM_UDRD_D
{
	T_RGSC_SSM_K key;
	T_RGSC_SSM_D data;
};

struct T_RGSC_SSM_D_CH
{
	int nSameRebarPos;
	CArray<T_RGSC_MAINBAR_CH, T_RGSC_MAINBAR_CH&> Anchor;
	CArray<T_RGSC_MAINBAR_CH, T_RGSC_MAINBAR_CH&> MbarI;
	CArray<T_RGSC_MAINBAR_CH, T_RGSC_MAINBAR_CH&> MbarM;
	CArray<T_RGSC_MAINBAR_CH, T_RGSC_MAINBAR_CH&> MbarJ;

	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarI;
	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarM;
	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarJ;

	void Get(T_RGSC_SSM_D& rData)
	{
		rData.nSameRebarPos = nSameRebarPos;
		rData.Anchor.SetSize(Anchor.GetSize());
		for (int i = 0; i < Anchor.GetSize(); i++) Anchor[i].Get(rData.Anchor[i]);
		rData.MbarI.SetSize(MbarI.GetSize());
		for (int i = 0; i < MbarI.GetSize(); i++) MbarI[i].Get(rData.MbarI[i]);
		rData.MbarM.SetSize(MbarM.GetSize());
		for (int i = 0; i < MbarM.GetSize(); i++) MbarM[i].Get(rData.MbarM[i]);
		rData.MbarJ.SetSize(MbarJ.GetSize());
		for (int i = 0; i < MbarJ.GetSize(); i++) MbarJ[i].Get(rData.MbarJ[i]);
		rData.SbarI.SetSize(SbarI.GetSize());
		for (int i = 0; i < SbarI.GetSize(); i++) SbarI[i].Get(rData.SbarI[i]);
		rData.SbarM.SetSize(SbarM.GetSize());
		for (int i = 0; i < SbarM.GetSize(); i++) SbarM[i].Get(rData.SbarM[i]);
		rData.SbarJ.SetSize(SbarJ.GetSize());
		for (int i = 0; i < SbarJ.GetSize(); i++) SbarJ[i].Get(rData.SbarJ[i]);
	}

	void Set(/*const*/ T_RGSC_SSM_D& data)
	{
		nSameRebarPos = data.nSameRebarPos;
		Anchor.SetSize(data.Anchor.GetSize());
		for (int i = 0; i < data.Anchor.GetSize(); i++) Anchor[i].Set(data.Anchor[i]);
		MbarI.SetSize(data.MbarI.GetSize());
		for (int i = 0; i < data.MbarI.GetSize(); i++) MbarI[i].Set(data.MbarI[i]);
		MbarM.SetSize(data.MbarM.GetSize());
		for (int i = 0; i < data.MbarM.GetSize(); i++) MbarM[i].Set(data.MbarM[i]);
		MbarJ.SetSize(data.MbarJ.GetSize());
		for (int i = 0; i < data.MbarJ.GetSize(); i++) MbarJ[i].Set(data.MbarJ[i]);
		SbarI.SetSize(data.SbarI.GetSize());
		for (int i = 0; i < data.SbarI.GetSize(); i++) SbarI[i].Set(data.SbarI[i]);
		SbarM.SetSize(data.SbarM.GetSize());
		for (int i = 0; i < data.SbarM.GetSize(); i++) SbarM[i].Set(data.SbarM[i]);
		SbarJ.SetSize(data.SbarJ.GetSize());
		for (int i = 0; i < data.SbarJ.GetSize(); i++) SbarJ[i].Set(data.SbarJ[i]);
	}
};

struct T_RGSC_SSM_D_CH_RW
{
	T_RGSC_SSM_K key;
	int nSameRebarPos;
	CArray<T_RGSC_MAINBAR_CH_RW, T_RGSC_MAINBAR_CH_RW&> Anchor;
	CArray<T_RGSC_MAINBAR_CH_RW, T_RGSC_MAINBAR_CH_RW&> MbarI;
	CArray<T_RGSC_MAINBAR_CH_RW, T_RGSC_MAINBAR_CH_RW&> MbarM;
	CArray<T_RGSC_MAINBAR_CH_RW, T_RGSC_MAINBAR_CH_RW&> MbarJ;

	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarI;
	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarM;
	CArray<T_RBAR_STIRRUP_CH, T_RBAR_STIRRUP_CH&> SbarJ;

	void Get(T_RRSC_SSM_K& rKey, T_RGSC_SSM_D_CH& rData)
	{
		rKey = key;
		rData.nSameRebarPos = nSameRebarPos;
		rData.Anchor.SetSize(Anchor.GetSize());
		for (int i = 0; i < Anchor.GetSize(); i++) Anchor[i].Get(rData.Anchor[i]);
		rData.MbarI.SetSize(MbarI.GetSize());
		for (int i = 0; i < MbarI.GetSize(); i++) MbarI[i].Get(rData.MbarI[i]);
		rData.MbarM.SetSize(MbarM.GetSize());
		for (int i = 0; i < MbarM.GetSize(); i++) MbarM[i].Get(rData.MbarM[i]);
		rData.MbarJ.SetSize(MbarJ.GetSize());
		for (int i = 0; i < MbarJ.GetSize(); i++) MbarJ[i].Get(rData.MbarJ[i]);
		rData.SbarI.SetSize(SbarI.GetSize());
		for (int i = 0; i < SbarI.GetSize(); i++) SbarI[i] = rData.SbarI[i];
		rData.SbarM.SetSize(SbarM.GetSize());
		for (int i = 0; i < SbarM.GetSize(); i++) SbarM[i] = rData.SbarM[i];
		rData.SbarJ.SetSize(SbarJ.GetSize());
		for (int i = 0; i < SbarJ.GetSize(); i++) SbarJ[i] = rData.SbarJ[i];
	}
	void Set(const T_RRSC_SSM_K Key, /*const*/ T_RGSC_SSM_D_CH& data)
	{
		key = Key;
		nSameRebarPos = data.nSameRebarPos;
		Anchor.SetSize(data.Anchor.GetSize());
		for (int i = 0; i < data.Anchor.GetSize(); i++) Anchor[i].Set(data.Anchor[i]);
		MbarI.SetSize(data.MbarI.GetSize());
		for (int i = 0; i < data.MbarI.GetSize(); i++) MbarI[i].Set(data.MbarI[i]);
		MbarM.SetSize(data.MbarM.GetSize());
		for (int i = 0; i < data.MbarM.GetSize(); i++) MbarM[i].Set(data.MbarM[i]);
		MbarJ.SetSize(data.MbarJ.GetSize());
		for (int i = 0; i < data.MbarJ.GetSize(); i++) MbarJ[i].Set(data.MbarJ[i]);
		SbarI.SetSize(data.SbarI.GetSize());
		for (int i = 0; i < data.SbarI.GetSize(); i++) SbarI[i] = data.SbarI[i];
		SbarM.SetSize(data.SbarM.GetSize());
		for (int i = 0; i < data.SbarM.GetSize(); i++) SbarM[i] = data.SbarM[i];
		SbarJ.SetSize(data.SbarJ.GetSize());
		for (int i = 0; i < data.SbarJ.GetSize(); i++) SbarJ[i] = data.SbarJ[i];
	}
};

struct T_RGSC_SSM_D_SIZE
{
	T_RGSC_SSM_K key;
	int nSameRebarPos;
	int nSizeAnchor;
	int nSizeMbarI;
	int nSizeMbarM;
	int nSizeMbarJ;
	int nSizeSbarI;
	int nSizeSbarM;
	int nSizeSbarJ;

	void Get(T_RRSC_SSM_K& rKey, T_RGSC_SSM_D_CH& rData)
	{
		rKey = key;
		rData.nSameRebarPos = nSameRebarPos;
	}
	void Set(const T_RRSC_SSM_K Key, const T_RGSC_SSM_D_CH& data)
	{
		key = Key;
		nSameRebarPos = data.nSameRebarPos;
		nSizeAnchor = (int)data.Anchor.GetSize();
		nSizeMbarI = (int)data.MbarI.GetSize();
		nSizeMbarM = (int)data.MbarM.GetSize();
		nSizeMbarJ = (int)data.MbarJ.GetSize();
		nSizeSbarI = (int)data.SbarI.GetSize();
		nSizeSbarM = (int)data.SbarM.GetSize();
		nSizeSbarJ = (int)data.SbarJ.GetSize();
	}
};

#define HASHSIZERGSCSSM HASHSIZESECT
#pragma endregion

#pragma region //T_RBAR_SSM_D (DB 구조체) - 단면별, 부재별 철근배근 타입정보 - Section Manager 입력 v970(?)추가
#define T_MPST_SSM_K unsigned int
#define T_RBAR_SSM_K unsigned int
struct T_RBAR_SSM_D
{
	T_ELEM_K kElem;			//T_SECT_D 와 1:N 관계 (T_SECT_K 하나에 기설부, 보강부 각각 철근 DB 존재)
	T_RPSC_SSM_K kRpscSsm;
	T_RRSC_SSM_K kRrscSsm;
	T_RGSC_SSM_K kRgscSsm;
	T_MPST_SSM_K kMpstSsm;
	int nShapeType;			// 단면형상 타입			T_RBAR_SSM_D::eShapeType
	int nRebarPlacTarget;	// 철근배근 대상 단면		T_RBAR_SSM_D::eRebarPlacTarget

	void Initialize()
	{
		kElem = 0;
		kRpscSsm = 0;
		kRrscSsm = 0;
		kRgscSsm = 0;
		kMpstSsm = 0;
		nShapeType = 0;
		nRebarPlacTarget = 0;
	}

	T_RBAR_SSM_D() { Initialize(); }
	T_RBAR_SSM_D(const T_RBAR_SSM_D& src) { *this = src; }
	T_RBAR_SSM_D& operator=(const T_RBAR_SSM_D& src)
	{
		if (this != &src) {
			kElem = src.kElem;
			kRpscSsm = src.kRpscSsm;
			kRrscSsm = src.kRrscSsm;
			kRgscSsm = src.kRgscSsm;
			kMpstSsm = src.kMpstSsm;
			nShapeType = src.nShapeType;
			nRebarPlacTarget = src.nRebarPlacTarget;
		}
		return *this;
	}

	enum eShapeType
	{
		kUnkowunShape,
		kRegular,	//정형단면
		kGeneral,	//임의형상 단면
		kPC,		//PC 단면
	};

	enum eRebarPlacTarget
	{
		kUnkowunTarget,
		kSection,	//기설부 단면
		kReinforce,	//보강부 단면
	};
};

struct T_RBAR_SSM_UNIT
{
	T_RBAR_SSM_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

struct T_RBAR_SSM_UDRD_D
{
	T_RBAR_SSM_K key;
	T_RBAR_SSM_D data;
};

#define HASHSIZERBARSSM HASHSIZEELEM
#pragma endregion