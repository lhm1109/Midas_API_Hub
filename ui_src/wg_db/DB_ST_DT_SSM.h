#ifndef __DB_ST_DT_SSM_H__
#define __DB_ST_DT_SSM_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#define HASHSIZESECT 101

#include "DB_ST_DT_SSM_RBAR.h"	// Section Manager - Rebar
#include "DB_ST_DT_SSM_MPST.h"	// Section Manager - M-Phi Parameter

//////////////////////////////////////////////////////////////////////////
// Section Manager - Strengthening
#define T_STRN_SSM_K T_SECT_K
struct T_STRN_SUB_SSM
{
	T_MATL_K  nMatlK;
	double dWidth;
	double dThick;

	int     nRefY;        // 0:Centroid 1:Left
	int     nRefZ;        // 0:Bottom 1:Top
	double  dRefY;
	double  dRefZ;

	void Initialize()
	{
		nMatlK = 0;
		dWidth = 0.0;
		dThick = 0.0;
		nRefY = 0;
		nRefZ = 0;
		dRefY = 0.0;
		dRefZ = 0.0;
	}
	T_STRN_SUB_SSM() { Initialize(); }
	T_STRN_SUB_SSM(const T_STRN_SUB_SSM& rData) { *this = rData; }
	T_STRN_SUB_SSM& operator=(const T_STRN_SUB_SSM& rData)
	{
		nMatlK = rData.nMatlK;
		dWidth = rData.dWidth;
		dThick = rData.dThick;
		nRefY = rData.nRefY;
		nRefZ = rData.nRefZ;
		dRefY = rData.dRefY;
		dRefZ = rData.dRefZ;

		return (*this);
	}
};

struct T_STRN_SSM_D   // Strengthening
{
	BOOL bSameJ;   // I==J:TRUE, I!=J:FALSE
	int nAfterStage;
	CArray<T_STRN_SUB_SSM, T_STRN_SUB_SSM&> StrnSub[2];

	void Initialize()
	{
		bSameJ = TRUE;
		nAfterStage = 0;
		StrnSub[0].RemoveAll();
		StrnSub[1].RemoveAll();
	}
	T_STRN_SSM_D() { Initialize(); }
	T_STRN_SSM_D(const T_STRN_SSM_D& rData) { *this = rData; }
	T_STRN_SSM_D& operator=(const T_STRN_SSM_D& rData)
	{
		bSameJ = rData.bSameJ;
		nAfterStage = rData.nAfterStage;
		StrnSub[0].Copy(rData.StrnSub[0]);
		StrnSub[1].Copy(rData.StrnSub[1]);

		return (*this);
	}
};


struct T_STRN_SSM_UDRD_D
{
	T_STRN_SSM_K key;
	T_STRN_SSM_D data;
};

struct T_STRN_SSM_D_RW
{
	BOOL bSameJ; 
	int  nAfterStage;
	int  nCountStrnSub[2];

	void Get(T_STRN_SSM_D& rData)
	{
		rData.bSameJ = bSameJ;
		rData.nAfterStage = nAfterStage;
		rData.StrnSub[0].SetSize(nCountStrnSub[0]);
		rData.StrnSub[1].SetSize(nCountStrnSub[1]);
	}
	void Set(T_STRN_SSM_D Data)
	{
		bSameJ = Data.bSameJ;
		nAfterStage = Data.nAfterStage;
		nCountStrnSub[0] = (int)Data.StrnSub[0].GetSize();
		nCountStrnSub[1] = (int)Data.StrnSub[1].GetSize();
	}
};

struct T_STRN_SSM_UDRD_D_RW
{
	T_STRN_SSM_K key;
	T_STRN_SSM_D_RW data;

	void GetStrnSsm(T_STRN_SSM_K& rKey, T_STRN_SSM_D &rData)
	{
		rKey = key;
		data.Get(rData);
	}
	void SetStrnSsm(const T_STRN_SSM_K Key, const T_STRN_SSM_D &rData)
	{
		key = Key;
		data.Set(rData);
	}
};

// struct T_STRN_SSM_UDRD_D_CH
// {
// 	T_STRN_SSM_K key;
// 	T_STRN_SSM_D_CH data;
// };


struct T_STRN_SSM_UNIT
{
	T_STRN_SSM_UNIT()
	{
		dWidth = D_UNITSYS_BASE_LENGTH;
		dThick = D_UNITSYS_BASE_LENGTH;
		dRefY  = D_UNITSYS_BASE_LENGTH;
		dRefZ  = D_UNITSYS_BASE_LENGTH;
	}
	int dWidth;
	int dThick;
	int dRefY;
	int dRefZ;
};
#define HASHSIZESTRNSSM HASHSIZESECT


// Section Manager - Stress Points
#define T_STRP_SSM_K T_SECT_K

struct T_STRP_SSM_D 
{
	BOOL bSameJ;  // I==J:TRUE, I!=J:FALSE
	CArray<T_POINT_SSM, T_POINT_SSM&> aPoint[2];

	void Initialize()
	{
		bSameJ = TRUE;
		aPoint[0].RemoveAll();
		aPoint[1].RemoveAll();
	}

	T_STRP_SSM_D() {}
	T_STRP_SSM_D(const T_STRP_SSM_D& rData) { *this = rData; }
	T_STRP_SSM_D& operator=(const T_STRP_SSM_D& rData)
	{
		bSameJ = rData.bSameJ;
		aPoint[0].Copy(rData.aPoint[0]);
		aPoint[1].Copy(rData.aPoint[1]);
		return *this;
	}
};

struct T_STRP_SSM_D_RW
{
	BOOL bSameJ;  // I==J:TRUE, I!=J:FALSE
	int  nCountPoint[2];

	void Get(T_STRP_SSM_D& rData)
	{
		rData.bSameJ = bSameJ;
		rData.aPoint[0].SetSize(nCountPoint[0]);
		rData.aPoint[1].SetSize(nCountPoint[1]);
	}
	void Set(T_STRP_SSM_D Data)
	{
		bSameJ = Data.bSameJ;
		nCountPoint[0] = (int)Data.aPoint[0].GetSize();
		nCountPoint[1] = (int)Data.aPoint[1].GetSize();
	}
};

struct T_STRP_SSM_UDRD_D
{
	T_STRP_SSM_K key;
	T_STRP_SSM_D data;
};

struct T_STRP_SSM_UDRD_D_RW
{
	T_STRP_SSM_K key;
	T_STRP_SSM_D_RW data;

	void GetStrpSsm(T_STRP_SSM_K& rKey, T_STRP_SSM_D &rData)
	{
		rKey = key;
		data.Get(rData);
	}
	void SetStrpSsm(const T_STRP_SSM_K Key, const T_STRP_SSM_D &rData)
	{
		key = Key;
		data.Set(rData);
	}
};

struct T_STRP_SSM_UNIT
{
	T_STRP_SSM_UNIT()
	{
		dPoint = D_UNITSYS_BASE_LENGTH;
	}
	int dPoint;
};

#define HASHSIZESTRPSSM HASHSIZESECT

// Section Manager - Design Group(설계 영역)
#define T_GRPD_SSM_K T_SECT_K

struct T_GRPD_SUB_SSM
{
	int  nGroupID;
	CString strGroupName;
	int  nGroupType; // 0:Top-Flange 1:Web 2:Bottom-Flange

	void Initialize()
	{
		nGroupID = 0;
		strGroupName.Empty();
		nGroupType = 0;
	}

	T_GRPD_SUB_SSM() {}
	T_GRPD_SUB_SSM(const T_GRPD_SUB_SSM& rData) { *this = rData; }
	T_GRPD_SUB_SSM& operator=(const T_GRPD_SUB_SSM& rData) 
	{
		nGroupID = rData.nGroupID;
		strGroupName = rData.strGroupName;
		nGroupType = rData.nGroupType;
		return (*this);
	}
};

struct T_GRPD_SUB_SSM_CH
{
	int  nGroupID;
	char strGroupName[40];
	int  nGroupType; // 0:Top-Flange 1:Web 2:Bottom-Flange
	
	void ConvertToString(T_GRPD_SUB_SSM& rData)
	{
		rData.nGroupID = nGroupID;
		ConvertCharStr(strGroupName, rData.strGroupName, sizeof(strGroupName));
		rData.nGroupType = nGroupType;
	}
	void ConvertToChar(T_GRPD_SUB_SSM& rData)
	{
		nGroupID = rData.nGroupID;
		ConvertStrChar(rData.strGroupName, strGroupName, sizeof(strGroupName));
		nGroupType = rData.nGroupType;
	} 
};

struct T_PART_SSM
{
	int  nType;              // 0:Polygon 1:Line 
													 // 현재는 Line Type 만 허용..Type 추가시 이 변수 이용 
	int  nGroupID;
	CString strPartName;     // Part Name
	int  nPartShape;         // Part Shape  0:자유돌출판 1:양연지지판
	BOOL bStiffener;         // TRUE:Stiffener

	CArray<int, int&> aLine; // Part 의 Line 정보

	void Initialize()
	{
		nType = 1;
		nGroupID = 0;
		strPartName.Empty();
		nPartShape = 0;
		bStiffener = FALSE;

		aLine.RemoveAll();
	}

	T_PART_SSM() {}
	T_PART_SSM(const T_PART_SSM& rData) { *this = rData; }
	T_PART_SSM& operator=(const T_PART_SSM& rData) 
	{
		nType = rData.nType;
		nGroupID = rData.nGroupID;
		strPartName = rData.strPartName;
		nPartShape = rData.nPartShape;
		bStiffener = rData.bStiffener;

		aLine.Copy(rData.aLine);
		return (*this);
	}
};

struct T_PART_SSM_CH
{
	int  nType;              // 0:Polygon 1:Line
													 // 현재는 Line Type 만 허용..Type 추가시 이 변수 이용  
	int  nGroupID;
	char strPartName[40];    // Part Name
	int  nPartShape;         // Part Shape  0:자유돌출판 1:양연지지판
	BOOL bStiffener;         // TRUE:Stiffener
	
	CArray<int, int&> aLine; // Part 의 Line 정보
	
	void ConvertToString(T_PART_SSM& rData)
	{
		rData.nType = nType;
		rData.nGroupID = nGroupID;
		ConvertCharStr(strPartName, rData.strPartName, sizeof(strPartName));
		rData.nPartShape = nPartShape;
		rData.bStiffener = bStiffener;

		rData.aLine.Copy(aLine);
	}
	void ConvertToChar(T_PART_SSM& rData)
	{
		nType = rData.nType;
		nGroupID = rData.nGroupID;
		ConvertStrChar(rData.strPartName, strPartName, sizeof(strPartName));
		nPartShape = rData.nPartShape;
		bStiffener = rData.bStiffener;

		aLine.Copy(rData.aLine);
	} 
};

struct T_PART_SSM_CH_RW
{
	int  nType;              // 0:Polygon 1:Line
													 // 현재는 Line Type 만 허용..Type 추가시 이 변수 이용 
	int  nGroupID;
	char strPartName[40];    // Part Name
	int  nPartShape;         // Part Shape  0:자유돌출판 1:양연지지판
	BOOL bStiffener;         // TRUE:Stiffener

	int nCountLine;          // Part 의 Line 정보

	void Get(T_PART_SSM_CH& rData)
	{
		rData.nType = nType;
		rData.nGroupID = nGroupID;
		memcpy(rData.strPartName, strPartName, sizeof(strPartName));  
		rData.nPartShape = nPartShape;
		rData.bStiffener = bStiffener;

		rData.aLine.SetSize(nCountLine);
	}
	void Set(T_PART_SSM_CH& Data)
	{
		nType = Data.nType;
		nGroupID = Data.nGroupID;
		memcpy(strPartName, Data.strPartName, sizeof(strPartName));   
		nPartShape = Data.nPartShape;
		bStiffener = Data.bStiffener;

		nCountLine = (int)Data.aLine.GetSize();
	}
};

struct T_GRPD_SSM_D 
{
	BOOL bSameJ;  // I==J:TRUE, I!=J:FALSE
	CArray<T_GRPD_SUB_SSM, T_GRPD_SUB_SSM&> aGrpd[2];
	CArray<T_PART_SSM, T_PART_SSM&> aPart[2];

	void Initialize()
	{
		bSameJ = TRUE;
		aGrpd[0].RemoveAll();
		aGrpd[1].RemoveAll();
		aPart[0].RemoveAll();
		aPart[1].RemoveAll();
	}

	T_GRPD_SSM_D() {}
	T_GRPD_SSM_D(const T_GRPD_SSM_D& rData) { *this = rData; }
	T_GRPD_SSM_D& operator=(const T_GRPD_SSM_D& rData)
	{
		bSameJ = rData.bSameJ;
		aGrpd[0].Copy(rData.aGrpd[0]);
		aGrpd[1].Copy(rData.aGrpd[1]);
		aPart[0].Copy(rData.aPart[0]);
		aPart[1].Copy(rData.aPart[1]);
		return *this;
	}
};

struct T_GRPD_SSM_D_CH 
{
	BOOL bSameJ;  // I==J:TRUE, I!=J:FALSE
	CArray<T_GRPD_SUB_SSM_CH, T_GRPD_SUB_SSM_CH&> aGrpd[2];
	CArray<T_PART_SSM_CH, T_PART_SSM_CH&> aPart[2];

	void ConvertToString(T_GRPD_SSM_D& rData)
	{
		rData.bSameJ = bSameJ;

		int nCount = (int)aGrpd[0].GetSize();
		rData.aGrpd[0].SetSize(nCount);
		for (int i=0; i<nCount; i++) aGrpd[0][i].ConvertToString(rData.aGrpd[0][i]);
		nCount     = (int)aGrpd[1].GetSize();    
		rData.aGrpd[1].SetSize(nCount);
		for (int i=0; i<nCount; i++) aGrpd[1][i].ConvertToString(rData.aGrpd[1][i]);

		nCount     = (int)aPart[0].GetSize();
		rData.aPart[0].SetSize(nCount);
		for (int i=0; i<nCount; i++) aPart[0][i].ConvertToString(rData.aPart[0][i]);
		nCount     = (int)aPart[1].GetSize();    
		rData.aPart[1].SetSize(nCount);
		for (int i=0; i<nCount; i++) aPart[1][i].ConvertToString(rData.aPart[1][i]);
	}
	void ConvertToChar(T_GRPD_SSM_D& rData)
	{
		bSameJ = rData.bSameJ;

		int nCount = (int)rData.aGrpd[0].GetSize();
		aGrpd[0].SetSize(nCount);
		for (int i=0; i<nCount; i++) aGrpd[0][i].ConvertToChar(rData.aGrpd[0][i]);
		nCount     = (int)rData.aGrpd[1].GetSize();    
		aGrpd[1].SetSize(nCount);
		for (int i=0; i<nCount; i++) aGrpd[1][i].ConvertToChar(rData.aGrpd[1][i]);

		nCount     = (int)rData.aPart[0].GetSize();
		aPart[0].SetSize(nCount);
		for (int i=0; i<nCount; i++) aPart[0][i].ConvertToChar(rData.aPart[0][i]);
		nCount     = (int)rData.aPart[1].GetSize();    
		aPart[1].SetSize(nCount);
		for (int i=0; i<nCount; i++) aPart[1][i].ConvertToChar(rData.aPart[1][i]);
	}
};

struct T_GRPD_SSM_UDRD_D
{
	T_GRPD_SSM_K key;
	T_GRPD_SSM_D data;
};

struct T_GRPD_SSM_UDRD_D_CH
{
	T_GRPD_SSM_K key;
	T_GRPD_SSM_D_CH data;
};

struct T_GRPD_SSM_UDRD_D_CH_RW
{
	T_GRPD_SSM_K key;
	BOOL bSameJ;
	int nCountGrpd[2];
	int nCountPart[2];

	void GetGrpdSsm(T_GRPD_SSM_K& rKey, T_GRPD_SSM_D_CH& rData)
	{
		rKey = key;
		rData.bSameJ  = bSameJ;
	}
	void SetGrpdSsm(const T_GRPD_SSM_K Key, const T_GRPD_SSM_D_CH& rData)
	{
		key = Key;
		bSameJ  = rData.bSameJ;

		nCountGrpd[0] = (int)rData.aGrpd[0].GetSize();
		nCountGrpd[1] = (int)rData.aGrpd[1].GetSize();
		nCountPart[0] = (int)rData.aPart[0].GetSize();
		nCountPart[1] = (int)rData.aPart[1].GetSize();
	}
};

struct T_GRPD_SSM_UNIT
{
	T_GRPD_SSM_UNIT()
	{ 
		// Nothing
	}
};
#define HASHSIZEGRPDSSM HASHSIZESECT

// Section Manager - Stiffener
#define T_STFN_SSM_K T_SECT_K

struct T_STFN_SUB_TYPE_SSM // Stiffener Type
{
	int    nStiffenerType;   // Stiffener Type
													 // 0:Solid Rectangular
													 // 1:T-Section
													 // 2:U-Rib
	double dSize[20];

	void Initialize()
	{
		nStiffenerType = 0;

		for(int i=0; i<20; i++)
		{
			dSize[i] = 0.;
		}
	}
};

struct T_STFN_SUB_SSM
{
	int    nType;            // 0:Outer Polygon 1:Inner Polygon 2:Line
	int    nStandRef;        // 배근 기준점 0:시점 1:Center 2:종점
	int    nPosonLine;       // Stiffener 위치 0:좌측 1:우측 2:양측
	double dStandRefL;       // 기준점으로 부터 떨어진 거리
	int    nNum;             // Num
	double dCTC;             // CTC 
	
	T_STFN_SUB_TYPE_SSM Stiffener;

	CArray<int, int&> aLine;  // Stiffener 가 할당된 Line 정보

	void Initialize()
	{
		nType = 2;
		nStandRef = 0;
		nPosonLine = 0;
		dStandRefL = 0.;
		nNum = 0;
		dCTC = 0.;

		Stiffener.Initialize();

		aLine.RemoveAll();
	}

	T_STFN_SUB_SSM() {}
	T_STFN_SUB_SSM(const T_STFN_SUB_SSM& rData) { *this = rData; }
	T_STFN_SUB_SSM& operator=(const T_STFN_SUB_SSM& rData) 
	{
		nType = rData.nType;
		nStandRef = rData.nStandRef;
		nPosonLine = rData.nPosonLine;
		dStandRefL = rData.dStandRefL;
		nNum = rData.nNum;
		dCTC = rData.dCTC;
		
		Stiffener = rData.Stiffener;

		aLine.Copy(rData.aLine);
		return (*this);
	}
};

struct T_STFN_SUB_SSM_RW 
{
	int    nType;            // 0:Outer Polygon 1:Inner Polygon 2:Line
	int    nStandRef;        // 배근 기준점 0:시점 1:Center 2:종점
	int    nPosonLine;       // Stiffener 위치 0:좌측 1:우측 2:양측
	double dStandRefL;       // 기준점으로 부터 떨어진 거리
	int    nNum;             // Num
	double dCTC;             // CTC 
	
	T_STFN_SUB_TYPE_SSM Stiffener;

	int nCountLine;          // Stiffener 가 할당된 Line 정보

	void Get(T_STFN_SUB_SSM& rData)
	{
		rData.nType = nType;
		rData.nStandRef = nStandRef;
		rData.nPosonLine = nPosonLine;
		rData.dStandRefL = dStandRefL;
		rData.nNum = nNum;
		rData.dCTC = dCTC;

		rData.Stiffener = Stiffener;

		rData.aLine.SetSize(nCountLine);
	}
	void Set(T_STFN_SUB_SSM Data)
	{
		nType = Data.nType;
		nStandRef = Data.nStandRef;
		nPosonLine = Data.nPosonLine;
		dStandRefL = Data.dStandRefL;
		nNum = Data.nNum;
		dCTC = Data.dCTC;

		Stiffener = Data.Stiffener;

		nCountLine = (int)Data.aLine.GetSize();
	}
};

struct T_STFN_SSM_D
{
	BOOL bSameJ;  // I==J:TRUE, I!=J:FALSE
	CArray<T_STFN_SUB_SSM, T_STFN_SUB_SSM&> aStiffener[2];

	void Initialize()
	{
		bSameJ = TRUE;
		aStiffener[0].RemoveAll();
		aStiffener[1].RemoveAll();
	}

	T_STFN_SSM_D() {}
	T_STFN_SSM_D(const T_STFN_SSM_D& rData) { *this = rData; }
	T_STFN_SSM_D& operator=(const T_STFN_SSM_D& rData)
	{
		bSameJ = rData.bSameJ;
		aStiffener[0].Copy(rData.aStiffener[0]);
		aStiffener[1].Copy(rData.aStiffener[1]);
		return *this;
	}
};

struct T_STFN_SSM_UDRD_D
{
	T_STFN_SSM_K key;
	T_STFN_SSM_D data;
};

struct T_STFN_SSM_UDRD_D_RW
{
	T_STFN_SSM_K key;
	BOOL bSameJ;
	int  nCountStiffener[2];

	void GetStfnSsm(T_STFN_SSM_K& rKey, T_STFN_SSM_D& rData)
	{
		rKey = key;
		rData.bSameJ  = bSameJ;
	}
	void SetStfnSsm(const T_STFN_SSM_K Key, const T_STFN_SSM_D& rData)
	{
		key = Key;
		bSameJ  = rData.bSameJ;
		nCountStiffener[0] = (int)rData.aStiffener[0].GetSize();
		nCountStiffener[1] = (int)rData.aStiffener[1].GetSize();
	}
};

struct T_STFN_SSM_UNIT
{
	T_STFN_SSM_UNIT()
	{
		dLength     = D_UNITSYS_BASE_LENGTH;
		dStandRefL  = D_UNITSYS_BASE_LENGTH;
		dCTC        = D_UNITSYS_BASE_LENGTH;
	}

	int dLength;
	int dStandRefL;
	int dCTC;
};


struct T_CGTS_SUB_STIFFENER_960_Temp
{
	int    nTransStiffType;       // 0:Web  1:Bearing  
	int    nPosition;             // 0:Top  1:Bot... 
	int    nStiffenerType;        // 0 : Flat 1 : Tee
	int    nPairNum;              // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dSize[4];              // (Length)
	double dFy;                   // Fy
	double dPitch;                // stiffener pitch (Length)
	double dFilletWeld;           // Fillet Weld (Length)

	BOOL   bUframeUse;
	int    nBeta;                  // (NR:Bearing) 0 : End  1: Internal
	double da;                     // (NR:Bearing) (Length)  
	double de;                     // (NR:Bearing) (Length)  
	int    nNumUftame;             // (NR:Web) 
	double dTheta;                 // (NR:Web) (radian)  

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;       // (NR:Bearing)
	CArray<T_SECT_STIFF_POS_D, T_SECT_STIFF_POS_D&> aStiffPosD;  // (NR:Bearing)
};

struct T_CGTS_SUB_STIFFENER_960_Temp2
{
	int    nTransStiffType;       // 0:Web  1:Bearing  
	int    nPosition;             // 0:Top  1:Bot... 
	int    nStiffenerTypeNoUse;   // 0 : Flat 1 : Tee - 사용안함 (v960 부터).
	int    nPairNum;              // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dSizeNotUse[4];        // (Length) - 사용안함 (v960 부터).
	CString strShapeNameUp;       // T_SECT_RIVET_STIFF_D 구조체 aStiffShape 의 Name (v960)
	CString strShapeNameLow;      // T_SECT_RIVET_STIFF_D 구조체 aStiffShape 의 Name (v960)

	double dFy;                   // Fy
	double dPitch;                // stiffener pitch (Length)
	double dFilletWeld;           // Fillet Weld (Length)

	BOOL   bSymmWeb;              // (v960)
	BOOL   bUframeUse;
	int    nBeta;                  // (NR:Bearing) 0 : End  1: Internal
	double da;                     // (NR:Bearing) (Length)  
	double de;                     // (NR:Bearing) (Length)  
	int    nNumUftame;             // (NR:Web) 
	double dTheta;                 // (NR:Web) (radian)  

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;       // (NR:Bearing)
	CArray<T_SECT_STIFF_POS_D, T_SECT_STIFF_POS_D&> aStiffPosD;  // (NR:Bearing)};
};

struct T_CGTS_SUB_STIFFENER
{
	int    nTransStiffType;       // 0:Web  1:Bearing  
	int    nPosition;             // 0:Top  1:Bot... 
	int    nStiffenerTypeNoUse;   // 0 : Flat 1 : Tee - 사용안함 (v960 부터).
	int    nPairNum;              // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dSizeNotUse[4];        // (Length) - 사용안함 (v960 부터).
	CString strShapeNameUp;       // T_SECT_RIVET_STIFF_D 구조체 aStiffShape 의 Name (v960)
	CString strShapeNameLow;      // T_SECT_RIVET_STIFF_D 구조체 aStiffShape 의 Name (v960)

	double dFy;                   // Fy
	double dPitch;                // stiffener pitch (Length)
	double dFilletWeld;           // Fillet Weld (Length)

	BOOL   bSymmWeb;              // (v960)
	BOOL   bUframeUse;             
	int    nBeta;                  // (NR:Bearing) 0 : End  1: Internal
	double da;                     // (NR:Bearing) (Length)  
	double de;                     // (NR:Bearing) (Length)  
	double dTheta_d;               // (NR:Bearing) (deg.)   (v960) 
	int    nNumUftame;             // (NR:Web) 
	double dTheta;                 // (NR:Web) (radian)  

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;       // (NR:Bearing)
	CArray<T_SECT_STIFF_POS_D, T_SECT_STIFF_POS_D&> aStiffPosD;  // (NR:Bearing)

	void Initialize()
	{
		nTransStiffType = 0;
		nPosition = 0;
		nStiffenerTypeNoUse = 0;
		nPairNum = 0;
		memset(dSizeNotUse, 0, sizeof(dSizeNotUse));
		strShapeNameUp.Empty();
		strShapeNameLow.Empty();
		dFy = 0.0;
		dPitch = 0.0;
		dFilletWeld = 0.0;

		bSymmWeb = FALSE;
		bUframeUse = FALSE;
		nBeta = 0;
		da = 0.0;
		de = 0.0;
		dTheta_d = 0.0;
		nNumUftame = 0;
		dTheta = 0.0;

		aStiffShape.RemoveAll();
		aStiffPosD.RemoveAll();
	}
	T_CGTS_SUB_STIFFENER() { Initialize(); }
	T_CGTS_SUB_STIFFENER(const T_CGTS_SUB_STIFFENER& rData) { *this = rData; }
	T_CGTS_SUB_STIFFENER& operator=(const T_CGTS_SUB_STIFFENER& rData)
	{
		nTransStiffType = rData.nTransStiffType;
		nPosition       = rData.nPosition;
		nStiffenerTypeNoUse = rData.nStiffenerTypeNoUse;
		nPairNum       = rData.nPairNum;
		memcpy(dSizeNotUse, rData.dSizeNotUse, sizeof(dSizeNotUse));
		strShapeNameUp = rData.strShapeNameUp;
		strShapeNameLow = rData.strShapeNameLow;

		dFy            = rData.dFy;
		dPitch         = rData.dPitch;
		dFilletWeld    = rData.dFilletWeld;

		bSymmWeb       = rData.bSymmWeb;
		bUframeUse     = rData.bUframeUse;
		nBeta          = rData.nBeta;
		da             = rData.da;
		de             = rData.de;
		dTheta_d       = rData.dTheta_d;
		nNumUftame     = rData.nNumUftame;
		dTheta         = rData.dTheta;

		aStiffShape.Copy(rData.aStiffShape);
		aStiffPosD.Copy(rData.aStiffPosD);
		return *this;
	}
};


struct T_CGTS_SUB_STIFFENER_RW_960_Temp
{
	int    nTransStiffType;       // 0:Web  1:Bearing  
	int    nPosition;             // 0:Top  1:Bot... 
	int    nStiffenerType;        // 0 : Flat 1 : Tee
	int    nPairNum;              // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dSize[4];              // (Length)
	double dFy;                   // Fy
	double dPitch;                // stiffener pitch (default=0, boundary >0, Length)
	double dFilletWeld;           // Fillet Weld (Length)

	BOOL   bUframeUse;
	int    nBeta;                  // (NR:Bearing) 0 : End  1: Internal
	double da;                     // (NR:Bearing) (Length)  
	double de;                     // (NR:Bearing) (Length)  
	int    nNumUftame;             // (NR:Web) 
	double dTheta;                 // (NR:Web) (radian)  

	int nStiffShapeNum;
	int nStiffPosDNum;
};
struct T_CGTS_SUB_STIFFENER_RW_960_Temp2
{
	int    nTransStiffType;       // 0:Web  1:Bearing  
	int    nPosition;             // 0:Top  1:Bot... 
	int    nStiffenerTypeNoUse;   // 0 : Flat 1 : Tee  (v960 부터)
	int    nPairNum;              // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dSizeNotUse[4];        // (Length) (v960 부터)
	char strShapeNameUp[20];      // (v960)  
	char strShapeNameLow[20];     // (v960)  

	double dFy;                   // Fy
	double dPitch;                // stiffener pitch (default=0, boundary >0, Length)
	double dFilletWeld;           // Fillet Weld (Length)

	BOOL   bSymmWeb;               // (v960)  
	BOOL   bUframeUse;
	int    nBeta;                  // (NR:Bearing) 0 : End  1: Internal
	double da;                     // (NR:Bearing) (Length)  
	double de;                     // (NR:Bearing) (Length)  
	int    nNumUftame;             // (NR:Web) 
	double dTheta;                 // (NR:Web) (radian)  

	int nStiffShapeNum;
	int nStiffPosDNum;
};
struct T_CGTS_SUB_STIFFENER_RW
{
	int    nTransStiffType;       // 0:Web  1:Bearing  
	int    nPosition;             // 0:Top  1:Bot... 
	int    nStiffenerTypeNoUse;   // 0 : Flat 1 : Tee  (v960 부터)
	int    nPairNum;              // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dSizeNotUse[4];        // (Length) (v960 부터)
	char strShapeNameUp[20];      // (v960)  
	char strShapeNameLow[20];     // (v960)  

	double dFy;                   // Fy
	double dPitch;                // stiffener pitch (default=0, boundary >0, Length)
	double dFilletWeld;           // Fillet Weld (Length)

	BOOL   bSymmWeb;               // (v960)  
	BOOL   bUframeUse;
	int    nBeta;                  // (NR:Bearing) 0 : End  1: Internal
	double da;                     // (NR:Bearing) (Length)  
	double de;                     // (NR:Bearing) (Length)  
	double dTheta_d;               // (NR:Bearing) (deg.)   (v960) 
	int    nNumUftame;             // (NR:Web) 
	double dTheta;                 // (NR:Web) (radian)  

	int nStiffShapeNum;
	int nStiffPosDNum;

	void Initialize()
	{
		nTransStiffType = 0;
		nPosition = 0;
		nStiffenerTypeNoUse = 0;
		nPairNum = 0;
		memset(dSizeNotUse, 0, sizeof(dSizeNotUse));
		memset(strShapeNameUp, 0, sizeof(strShapeNameUp));
		memset(strShapeNameLow, 0, sizeof(strShapeNameLow));

		dFy = 0.0;
		dPitch = 0.0;
		dFilletWeld = 0.0;

		bSymmWeb = FALSE;
		bUframeUse = FALSE;
		nBeta = 0;
		da = 0.0;
		de = 0.0;
		dTheta_d = 0.0;
		nNumUftame = 0;
		dTheta = 0.0;

		nStiffShapeNum = 0;
		nStiffPosDNum = 0;
	}

	void Get(T_CGTS_SUB_STIFFENER& rData)
	{
		rData.nTransStiffType= nTransStiffType;
		rData.nPosition      = nPosition;
		rData.nStiffenerTypeNoUse = nStiffenerTypeNoUse;
		rData.nPairNum       = nPairNum;
		memcpy(rData.dSizeNotUse, dSizeNotUse, sizeof(dSizeNotUse));
		ConvertCharStr(strShapeNameUp, rData.strShapeNameUp, sizeof(strShapeNameUp));
		ConvertCharStr(strShapeNameLow, rData.strShapeNameLow, sizeof(strShapeNameLow));

		rData.dFy            = dFy;
		rData.dPitch         = dPitch;
		rData.dFilletWeld    = dFilletWeld;

		rData.bSymmWeb       = bSymmWeb;
		rData.bUframeUse     = bUframeUse;
		rData.nBeta          = nBeta;
		rData.da             = da;
		rData.de             = de;
		rData.dTheta_d       = dTheta_d;
		rData.nNumUftame     = nNumUftame;
		rData.dTheta         = dTheta;

		rData.aStiffShape.SetSize(nStiffShapeNum);
		rData.aStiffPosD.SetSize(nStiffPosDNum);
	}
	void Set(T_CGTS_SUB_STIFFENER& rData)
	{
		nTransStiffType= rData.nTransStiffType;
		nPosition      = rData.nPosition;
		nStiffenerTypeNoUse = rData.nStiffenerTypeNoUse;
		nPairNum       = rData.nPairNum;
		memcpy(dSizeNotUse, rData.dSizeNotUse, sizeof(dSizeNotUse));
		ConvertStrChar(rData.strShapeNameUp, strShapeNameUp, sizeof(strShapeNameUp));
		ConvertStrChar(rData.strShapeNameLow, strShapeNameLow, sizeof(strShapeNameLow));

		dFy            = rData.dFy;
		dPitch         = rData.dPitch;
		dFilletWeld    = rData.dFilletWeld;

		bSymmWeb       = rData.bSymmWeb;
		bUframeUse     = rData.bUframeUse;
		nBeta          = rData.nBeta;
		da             = rData.da;
		de             = rData.de;
		dTheta_d       = rData.dTheta_d;
		nNumUftame     = rData.nNumUftame;
		dTheta         = rData.dTheta;

		nStiffShapeNum = rData.aStiffShape.GetSize();
		nStiffPosDNum  = rData.aStiffPosD.GetSize();
	}
	void Convert960_Temp2(T_CGTS_SUB_STIFFENER_RW_960_Temp2& rData)
	{
		nTransStiffType= rData.nTransStiffType;
		nPosition      = rData.nPosition;
		nStiffenerTypeNoUse = rData.nStiffenerTypeNoUse;
		nPairNum       = rData.nPairNum;
		memcpy(dSizeNotUse, rData.dSizeNotUse, sizeof(dSizeNotUse));
		memcpy(strShapeNameUp, rData.strShapeNameUp, sizeof(strShapeNameUp));
		memcpy(strShapeNameLow, rData.strShapeNameLow, sizeof(strShapeNameLow));

		dFy            = rData.dFy;
		dPitch         = rData.dPitch;
		dFilletWeld    = rData.dFilletWeld;

		bSymmWeb       = rData.bSymmWeb;
		bUframeUse     = rData.bUframeUse;
		nBeta          = rData.nBeta;
		da             = rData.da;
		de             = rData.de;
		dTheta_d       = 0.0;
		nNumUftame     = rData.nNumUftame;
		dTheta         = rData.dTheta;

		nStiffShapeNum = rData.nStiffShapeNum;
		nStiffPosDNum  = rData.nStiffPosDNum;
	}
	void Convert960_Temp(T_CGTS_SUB_STIFFENER_RW_960_Temp& rData)
	{
		nTransStiffType= rData.nTransStiffType;
		nPosition      = rData.nPosition;
		nStiffenerTypeNoUse = rData.nStiffenerType;
		nPairNum       = rData.nPairNum;
		memcpy(dSizeNotUse, rData.dSize, sizeof(dSizeNotUse));
		memset(strShapeNameUp, 0, sizeof(strShapeNameUp));
		memset(strShapeNameLow, 0, sizeof(strShapeNameLow));

		dFy            = rData.dFy;
		dPitch         = rData.dPitch;
		dFilletWeld    = rData.dFilletWeld;

		bSymmWeb       = TRUE;
		bUframeUse     = rData.bUframeUse;
		nBeta          = rData.nBeta;
		da             = rData.da;
		de             = rData.de;
		dTheta_d       = 0.0;
		nNumUftame     = rData.nNumUftame;
		dTheta         = rData.dTheta;

		nStiffShapeNum = rData.nStiffShapeNum;
		nStiffPosDNum  = rData.nStiffPosDNum;
	}
};

// Section Manager - Stiffener
//PMS:LRFD12 BSC-20131227
#define T_CGTS_SSM_K T_SECT_K
#define HASHSIZECGTSSSM HASHSIZESECT
struct T_CGTS_SUB_SSM_842
{
	int    nCodeTop;
	int    nGradeTop;
	BOOL   bUseStifTop;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Top에 대한
	int    nStiffenerTypeTop;     // 0 : Flat 1 : Tee
	int    nStifNumTop;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchTop;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeTop[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyTop;                // Fy

	int    nCodeBot;
	int    nGradeBot;
	BOOL   bUseStifBot;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Bot에 대한
	int    nStiffenerTypeBot;     // 0 : Flat 1 : Tee
	int    nStifNumBot;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchBot;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBot[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyBot;                // Fy

	int    nCodeWeb;
	int    nGradeWeb;
	BOOL   bUseStifWeb;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Web에 대한
	int    nStiffenerTypeWeb;     // 0 : Flat 1 : Tee
	int    nStifNumWeb;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchWeb;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeWeb[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyWeb;                // Fy
};

struct T_CGTS_SUB_SSM_955
{
	int    nCodeTop;
	int    nGradeTop;
	BOOL   bUseStifTop;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Top에 대한
	int    nStiffenerTypeTop;     // 0 : Flat 1 : Tee
	int    nStifNumTop;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchTop;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeTop[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyTop;                // Fy

	int    nCodeBot;
	int    nGradeBot;
	BOOL   bUseStifBot;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Bot에 대한
	int    nStiffenerTypeBot;     // 0 : Flat 1 : Tee
	int    nStifNumBot;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchBot;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBot[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyBot;                // Fy

	int    nCodeWeb;
	int    nGradeWeb;
	BOOL   bUseStifWeb;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Web에 대한
	int    nStiffenerTypeWeb;     // 0 : Flat 1 : Tee
	int    nStifNumWeb;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchWeb;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeWeb[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyWeb;                // Fy

	int    nCodeBearing;
	int    nGradeBearing;
	BOOL   bUseStifBearing;    // Bearing Stiffener
	int    nBearingType;          // 0 : Flat 1 : Tee
	int    nBearingPairNum;       // paris of stiffener  개수( default=1.0)
	double dPitchBearing;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBearing[4];           // 0: H 1:B 2:Tw 3:Tf
	double dClip4Fillet;              // Clip for Fillet Weld (default=0, boundary >0, Unit:Length)
	double dFyBearing;                // Fy
};

struct T_CGTS_SUB_SSM
{
	int    nCodeTop;
	int    nGradeTop;
	BOOL   bUseStifTop;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Top에 대한
	int    nStiffenerTypeTop;     // 0 : Flat 1 : Tee
	int    nStifNumTop;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchTop;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeTop[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyTop;                // Fy
	
	int    nCodeBot;
	int    nGradeBot;
	BOOL   bUseStifBot;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Bot에 대한
	int    nStiffenerTypeBot;     // 0 : Flat 1 : Tee
	int    nStifNumBot;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchBot;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBot[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyBot;                // Fy
	
	int    nCodeWeb;
	int    nGradeWeb;
	BOOL   bUseStifWeb;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Web에 대한
	int    nStiffenerTypeWeb;     // 0 : Flat 1 : Tee
	int    nStifNumWeb;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchWeb;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeWeb[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyWeb;                // Fy
	
	int    nCodeBearing;
	int    nGradeBearing;
	BOOL   bUseStifBearing;       // Bearing Stiffener
	int    nBearingType;          // 0 : Flat 1 : Tee
	int    nBearingPairNum;       // paris of stiffener  개수( default=1.0)
	double dPitchBearing;         // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBearing[4];       // 0: H 1:B 2:Tw 3:Tf
	double dFyBearing;            // Fy
	double dClip4Fillet;          // Clip for Fillet Weld (default=0, boundary >0, Unit:Length)
	
	T_CGTS_SUB_STIFFENER StiffNR_Web;
 	T_CGTS_SUB_STIFFENER StiffNR_Bearing;

	void Initialize()
	{
		nCodeTop = 0;
		nGradeTop = 0;
		bUseStifTop = FALSE;       
		nStiffenerTypeTop = 0;    
		nStifNumTop = 0;          
		dPitchTop   = 0.0;    
		memset(dSizeTop, 0, sizeof(dSizeTop));
		dFyTop      = 0.0;   
		
		nCodeBot = 0;
		nGradeBot = 0;
		bUseStifBot = FALSE;       
		nStiffenerTypeBot = 0;    
		nStifNumBot = 0;          
		dPitchBot   = 0.0;    
		memset(dSizeBot, 0, sizeof(dSizeBot));
		dFyBot      = 0.0;   
		
		nCodeWeb = 0;
		nGradeWeb = 0;
		bUseStifWeb = FALSE;       
		nStiffenerTypeWeb = 0;    
		nStifNumWeb = 0;          
		dPitchWeb   = 0.0;    
		memset(dSizeWeb, 0, sizeof(dSizeWeb));
		dFyWeb      = 0.0;   

		nCodeBearing        = 0;
		nGradeBearing       = 0;
		bUseStifBearing  = FALSE;
		nBearingType        = 0; 
		nBearingPairNum     = 1; 
		dPitchBearing       = 0.0;   
		memset(dSizeBearing, 0, sizeof(dSizeBearing));
		dClip4Fillet        = 0.0;
		dFyBearing          = 0.0;            

		StiffNR_Web.Initialize();
		StiffNR_Bearing.Initialize();
	}
	
	T_CGTS_SUB_SSM() { Initialize();}
	T_CGTS_SUB_SSM(const T_CGTS_SUB_SSM& rData) { *this = rData; }
	T_CGTS_SUB_SSM& operator=(const T_CGTS_SUB_SSM& rData) 
	{
		nCodeTop          = rData.nCodeTop;
		nGradeTop         = rData.nGradeTop;
		bUseStifTop       = rData.bUseStifTop;       
		nStiffenerTypeTop = rData.nStiffenerTypeTop; 
		nStifNumTop       = rData.nStifNumTop;       
		dPitchTop         = rData.dPitchTop;         
		memcpy(dSizeTop, rData.dSizeTop, sizeof(dSizeTop));
		dFyTop            = rData.dFyTop;            

		nCodeBot          = rData.nCodeBot;
		nGradeBot         = rData.nGradeBot;
		bUseStifBot       = rData.bUseStifBot;       
		nStiffenerTypeBot = rData.nStiffenerTypeBot; 
		nStifNumBot       = rData.nStifNumBot;       
		dPitchBot         = rData.dPitchBot;         
		memcpy(dSizeBot, rData.dSizeBot, sizeof(dSizeBot));
		dFyBot            = rData.dFyBot;            

		nCodeWeb          = rData.nCodeWeb;
		nGradeWeb         = rData.nGradeWeb;
		bUseStifWeb       = rData.bUseStifWeb;       
		nStiffenerTypeWeb = rData.nStiffenerTypeWeb; 
		nStifNumWeb       = rData.nStifNumWeb;       
		dPitchWeb         = rData.dPitchWeb;         
		memcpy(dSizeWeb, rData.dSizeWeb, sizeof(dSizeWeb));
		dFyWeb            = rData.dFyWeb;            

		nCodeBearing        = rData.nCodeBearing;       
		nGradeBearing       = rData.nGradeBearing;      
		bUseStifBearing     = rData.bUseStifBearing;  
		nBearingType        = rData.nBearingType;        
		nBearingPairNum     = rData.nBearingPairNum;     
		dPitchBearing       = rData.dPitchBearing;       
		memcpy(dSizeBearing, rData.dSizeBearing, sizeof(dSizeBearing));
		dClip4Fillet        = rData.dClip4Fillet;
		dFyBearing          = rData.dFyBearing;               
		
		StiffNR_Web        = rData.StiffNR_Web;
		StiffNR_Bearing    = rData.StiffNR_Bearing;

		return (*this);
	}

	void Convert955(T_CGTS_SUB_SSM_955& rData)
	{
		nCodeTop          = rData.nCodeTop;
		nGradeTop         = rData.nGradeTop;
		bUseStifTop       = rData.bUseStifTop;       
		nStiffenerTypeTop = rData.nStiffenerTypeTop; 
		nStifNumTop       = rData.nStifNumTop;       
		dPitchTop         = rData.dPitchTop;         
		memcpy(dSizeTop, rData.dSizeTop, sizeof(dSizeTop));
		dFyTop            = rData.dFyTop;            

		nCodeBot          = rData.nCodeBot;
		nGradeBot         = rData.nGradeBot;
		bUseStifBot       = rData.bUseStifBot;       
		nStiffenerTypeBot = rData.nStiffenerTypeBot; 
		nStifNumBot       = rData.nStifNumBot;       
		dPitchBot         = rData.dPitchBot;         
		memcpy(dSizeBot, rData.dSizeBot, sizeof(dSizeBot));
		dFyBot            = rData.dFyBot;            

		nCodeWeb          = rData.nCodeWeb;
		nGradeWeb         = rData.nGradeWeb;
		bUseStifWeb       = rData.bUseStifWeb;       
		nStiffenerTypeWeb = rData.nStiffenerTypeWeb; 
		nStifNumWeb       = rData.nStifNumWeb;       
		dPitchWeb         = rData.dPitchWeb;         
		memcpy(dSizeWeb, rData.dSizeWeb, sizeof(dSizeWeb));
		dFyWeb            = rData.dFyWeb;            

		nCodeBearing        = rData.nCodeBearing;       
		nGradeBearing       = rData.nGradeBearing;      
		bUseStifBearing     = rData.bUseStifBearing;  
		nBearingType        = rData.nBearingType;        
		nBearingPairNum     = rData.nBearingPairNum;     
		dPitchBearing       = rData.dPitchBearing;       
		memcpy(dSizeBearing, rData.dSizeBearing, sizeof(dSizeBearing));
		dClip4Fillet        = rData.dClip4Fillet;
		dFyBearing          = rData.dFyBearing;               

		StiffNR_Web.Initialize();
		StiffNR_Bearing.Initialize();
	}

	void Convert842(T_CGTS_SUB_SSM_842& rData)
	{
		nCodeTop          = rData.nCodeTop;
		nGradeTop         = rData.nGradeTop;
		bUseStifTop       = rData.bUseStifTop;       
		nStiffenerTypeTop = rData.nStiffenerTypeTop; 
		nStifNumTop       = rData.nStifNumTop;       
		dPitchTop         = rData.dPitchTop;         
		memcpy(dSizeTop, rData.dSizeTop, sizeof(dSizeTop));
		dFyTop            = rData.dFyTop;            

		nCodeBot          = rData.nCodeBot;
		nGradeBot         = rData.nGradeBot;
		bUseStifBot       = rData.bUseStifBot;       
		nStiffenerTypeBot = rData.nStiffenerTypeBot; 
		nStifNumBot       = rData.nStifNumBot;       
		dPitchBot         = rData.dPitchBot;         
		memcpy(dSizeBot, rData.dSizeBot, sizeof(dSizeBot));
		dFyBot            = rData.dFyBot;            

		nCodeWeb          = rData.nCodeWeb;
		nGradeWeb         = rData.nGradeWeb;
		bUseStifWeb       = rData.bUseStifWeb;       
		nStiffenerTypeWeb = rData.nStiffenerTypeWeb; 
		nStifNumWeb       = rData.nStifNumWeb;       
		dPitchWeb         = rData.dPitchWeb;         
		memcpy(dSizeWeb, rData.dSizeWeb, sizeof(dSizeWeb));
		dFyWeb            = rData.dFyWeb;            

		nCodeBearing        = 0;
		nGradeBearing       = 0;
		bUseStifBearing     = FALSE;
		nBearingType        = 0; 
		nBearingPairNum     = 1; 
		dPitchBearing       = 0.0;   
		memset(dSizeBearing, 0, sizeof(dSizeBearing));
		dClip4Fillet        = 0.0;
		dFyBearing          = 0.0;               

		StiffNR_Web.Initialize();
		StiffNR_Bearing.Initialize();
	}
};

struct T_CGTS_SUB_SSM_RW_842 
{
	int    nCodeTop;
	int    nGradeTop;
	BOOL   bUseStifTop;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Top에 대한
	int    nStiffenerTypeTop;     // 0 : Flat 1 : Tee
	int    nStifNumTop;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchTop;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeTop[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyTop;                // Fy

	int    nCodeBot;
	int    nGradeBot;
	BOOL   bUseStifBot;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Bot에 대한
	int    nStiffenerTypeBot;     // 0 : Flat 1 : Tee
	int    nStifNumBot;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchBot;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBot[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyBot;                // Fy

	int    nCodeWeb;
	int    nGradeWeb;
	BOOL   bUseStifWeb;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Web에 대한
	int    nStiffenerTypeWeb;     // 0 : Flat 1 : Tee
	int    nStifNumWeb;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchWeb;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeWeb[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyWeb;                // Fy
};

struct T_CGTS_SUB_SSM_RW_955
{
	int    nCodeTop;
	int    nGradeTop;
	BOOL   bUseStifTop;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Top에 대한
	int    nStiffenerTypeTop;     // 0 : Flat 1 : Tee
	int    nStifNumTop;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchTop;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeTop[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyTop;                // Fy

	int    nCodeBot;
	int    nGradeBot;
	BOOL   bUseStifBot;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Bot에 대한
	int    nStiffenerTypeBot;     // 0 : Flat 1 : Tee
	int    nStifNumBot;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchBot;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBot[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyBot;                // Fy

	int    nCodeWeb;
	int    nGradeWeb;
	BOOL   bUseStifWeb;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Web에 대한
	int    nStiffenerTypeWeb;     // 0 : Flat 1 : Tee
	int    nStifNumWeb;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchWeb;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeWeb[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyWeb;                // Fy

	int    nCodeBearing;
	int    nGradeBearing;
	BOOL   bUseStifBearing;       // Bearing Stiffener
	int    nBearingType;          // 0 : Flat 1 : Tee
	int    nBearingPairNum;       // paris of stiffener  개수( default=1.0)
	double dPitchBearing;         // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBearing[4];       // 0: H 1:B 2:Tw 3:Tf
	double dClip4Fillet;
	double dFyBearing;            // Fy
};

struct T_CGTS_SUB_SSM_RW_960_Temp
{
	int    nCodeTop;
	int    nGradeTop;
	BOOL   bUseStifTop;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Top에 대한
	int    nStiffenerTypeTop;     // 0 : Flat 1 : Tee
	int    nStifNumTop;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchTop;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeTop[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyTop;                // Fy

	int    nCodeBot;
	int    nGradeBot;
	BOOL   bUseStifBot;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Bot에 대한
	int    nStiffenerTypeBot;     // 0 : Flat 1 : Tee
	int    nStifNumBot;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchBot;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBot[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyBot;                // Fy

	int    nCodeWeb;
	int    nGradeWeb;
	BOOL   bUseStifWeb;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Web에 대한
	int    nStiffenerTypeWeb;     // 0 : Flat 1 : Tee
	int    nStifNumWeb;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchWeb;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeWeb[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyWeb;                // Fy

	int    nCodeBearing;
	int    nGradeBearing;
	BOOL   bUseStifBearing;       // Bearing Stiffener
	int    nBearingType;          // 0 : Flat 1 : Tee
	int    nBearingPairNum;       // paris of stiffener  개수( default=1.0)
	double dPitchBearing;         // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBearing[4];       // 0: H 1:B 2:Tw 3:Tf
	double dClip4Fillet;
	double dFyBearing;            // Fy

	T_CGTS_SUB_STIFFENER_RW_960_Temp StiffNR_Web;
	T_CGTS_SUB_STIFFENER_RW_960_Temp StiffNR_Bearing;
};

struct T_CGTS_SUB_SSM_RW_960_Temp2
{
	int    nCodeTop;
	int    nGradeTop;
	BOOL   bUseStifTop;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Top에 대한
	int    nStiffenerTypeTop;     // 0 : Flat 1 : Tee
	int    nStifNumTop;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchTop;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeTop[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyTop;                // Fy

	int    nCodeBot;
	int    nGradeBot;
	BOOL   bUseStifBot;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Bot에 대한
	int    nStiffenerTypeBot;     // 0 : Flat 1 : Tee
	int    nStifNumBot;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchBot;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBot[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyBot;                // Fy

	int    nCodeWeb;
	int    nGradeWeb;
	BOOL   bUseStifWeb;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Web에 대한
	int    nStiffenerTypeWeb;     // 0 : Flat 1 : Tee
	int    nStifNumWeb;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchWeb;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeWeb[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyWeb;                // Fy

	int    nCodeBearing;
	int    nGradeBearing;
	BOOL   bUseStifBearing;       // Bearing Stiffener
	int    nBearingType;          // 0 : Flat 1 : Tee
	int    nBearingPairNum;       // paris of stiffener  개수( default=1.0)
	double dPitchBearing;         // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBearing[4];       // 0: H 1:B 2:Tw 3:Tf
	double dClip4Fillet;
	double dFyBearing;            // Fy

	T_CGTS_SUB_STIFFENER_RW_960_Temp2 StiffNR_Web;
	T_CGTS_SUB_STIFFENER_RW_960_Temp2 StiffNR_Bearing;
};

struct T_CGTS_SUB_SSM_RW 
{
	int    nCodeTop;
	int    nGradeTop;
	BOOL   bUseStifTop;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Top에 대한
	int    nStiffenerTypeTop;     // 0 : Flat 1 : Tee
	int    nStifNumTop;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchTop;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeTop[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyTop;                // Fy
	
	int    nCodeBot;
	int    nGradeBot;
	BOOL   bUseStifBot;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Flange Bot에 대한
	int    nStiffenerTypeBot;     // 0 : Flat 1 : Tee
	int    nStifNumBot;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchBot;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBot[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyBot;                // Fy
	
	int    nCodeWeb;
	int    nGradeWeb;
	BOOL   bUseStifWeb;           // PMS:LRFD12 BSC-20131224 AASHTO, LSD Web에 대한
	int    nStiffenerTypeWeb;     // 0 : Flat 1 : Tee
	int    nStifNumWeb;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) (default=0, boundary=0,1)
	double dPitchWeb;             // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeWeb[4];           // 0: H 1:B 2:Tw 3:Tf
	double dFyWeb;                // Fy
	
	int    nCodeBearing;
	int    nGradeBearing;
	BOOL   bUseStifBearing;       // Bearing Stiffener
	int    nBearingType;          // 0 : Flat 1 : Tee
	int    nBearingPairNum;       // paris of stiffener  개수( default=1.0)
	double dPitchBearing;         // Rigid striffener pitch (default=0, boundary >0, Unit:Length)
	double dSizeBearing[4];       // 0: H 1:B 2:Tw 3:Tf
	double dClip4Fillet;
	double dFyBearing;            // Fy

	T_CGTS_SUB_STIFFENER_RW StiffNR_Web;
	T_CGTS_SUB_STIFFENER_RW StiffNR_Bearing;

	void Get(T_CGTS_SUB_SSM& rData)
	{
		rData.nCodeTop          = nCodeTop;
		rData.nGradeTop         = nGradeTop;
		rData.bUseStifTop       =        bUseStifTop;        
		rData.nStiffenerTypeTop =  nStiffenerTypeTop;  
		rData.nStifNumTop       =        nStifNumTop;        
		rData.dPitchTop        =    dPitchTop;          
		memcpy(rData.dSizeTop, dSizeTop, sizeof(dSizeTop));  
		rData.dFyTop           =    dFyTop;             

		rData.nCodeBot          = nCodeBot;
		rData.nGradeBot         = nGradeBot;
		rData.bUseStifBot       = bUseStifBot;        
		rData.nStiffenerTypeBot = nStiffenerTypeBot;  
		rData.nStifNumBot       = nStifNumBot;        
		rData.dPitchBot         = dPitchBot;          
		memcpy(rData.dSizeBot, dSizeBot, sizeof(dSizeBot));  
		rData.dFyBot            = dFyBot;             

		rData.nCodeWeb          = nCodeWeb;
		rData.nGradeWeb         = nGradeWeb;
		rData.bUseStifWeb       = bUseStifWeb;        
		rData.nStiffenerTypeWeb = nStiffenerTypeWeb;  
		rData.nStifNumWeb       = nStifNumWeb;        
		rData.dPitchWeb         = dPitchWeb;          
		memcpy(rData.dSizeWeb, dSizeWeb, sizeof(dSizeWeb));  
		rData.dFyWeb            = dFyWeb;             

		rData.nCodeBearing        = nCodeBearing;       
		rData.nGradeBearing       = nGradeBearing;      
		rData.bUseStifBearing     = bUseStifBearing;  
		rData.nBearingType        = nBearingType;        
		rData.nBearingPairNum     = nBearingPairNum;     
		rData.dPitchBearing       = dPitchBearing;       
		memcpy(rData.dSizeBearing, dSizeBearing, sizeof(dSizeBearing));
		rData.dClip4Fillet        = dClip4Fillet;
		rData.dFyBearing          = dFyBearing;            

		StiffNR_Web.Get(rData.StiffNR_Web);
		StiffNR_Bearing.Get(rData.StiffNR_Bearing);
	}
	void Set(T_CGTS_SUB_SSM Data)
	{
		nCodeTop          = Data.nCodeTop;
		nGradeTop         = Data.nGradeTop;
		bUseStifTop       = Data.bUseStifTop;        
		nStiffenerTypeTop = Data.nStiffenerTypeTop;  
		nStifNumTop       = Data.nStifNumTop;        
		dPitchTop         = Data.dPitchTop;          
		memcpy(dSizeTop, Data.dSizeTop, sizeof(dSizeTop));  
		dFyTop            = Data.dFyTop;             
		
		nCodeBot          = Data.nCodeBot;
		nGradeBot         = Data.nGradeBot;
		bUseStifBot       = Data.bUseStifBot;        
		nStiffenerTypeBot = Data.nStiffenerTypeBot;  
		nStifNumBot       = Data.nStifNumBot;        
		dPitchBot         = Data.dPitchBot;          
		memcpy(dSizeBot, Data.dSizeBot, sizeof(dSizeBot));  
		dFyBot            = Data.dFyBot;             
		
		nCodeWeb          = Data.nCodeWeb;
		nGradeWeb         = Data.nGradeWeb;
		bUseStifWeb       = Data.bUseStifWeb;        
		nStiffenerTypeWeb = Data.nStiffenerTypeWeb;  
		nStifNumWeb       = Data.nStifNumWeb;        
		dPitchWeb         = Data.dPitchWeb;          
		memcpy(dSizeWeb, Data.dSizeWeb, sizeof(dSizeWeb));  
		dFyWeb            = Data.dFyWeb;     

		nCodeBearing        = Data.nCodeBearing;       
		nGradeBearing       = Data.nGradeBearing;      
		bUseStifBearing     = Data.bUseStifBearing;  
		nBearingType        = Data.nBearingType;        
		nBearingPairNum     = Data.nBearingPairNum;     
		dPitchBearing       = Data.dPitchBearing;       
		memcpy(dSizeBearing, Data.dSizeBearing, sizeof(dSizeBearing));
		dClip4Fillet        = Data.dClip4Fillet;
		dFyBearing          = Data.dFyBearing;  

		StiffNR_Web.Set(Data.StiffNR_Web);
		StiffNR_Bearing.Set(Data.StiffNR_Bearing);
	}
	void Convert960_Temp2(T_CGTS_SUB_SSM_RW_960_Temp2& Data)
	{
		nCodeTop = Data.nCodeTop;
		nGradeTop = Data.nGradeTop;
		bUseStifTop = Data.bUseStifTop;
		nStiffenerTypeTop = Data.nStiffenerTypeTop;
		nStifNumTop = Data.nStifNumTop;
		dPitchTop = Data.dPitchTop;
		memcpy(dSizeTop, Data.dSizeTop, sizeof(dSizeTop));
		dFyTop = Data.dFyTop;

		nCodeBot = Data.nCodeBot;
		nGradeBot = Data.nGradeBot;
		bUseStifBot = Data.bUseStifBot;
		nStiffenerTypeBot = Data.nStiffenerTypeBot;
		nStifNumBot = Data.nStifNumBot;
		dPitchBot = Data.dPitchBot;
		memcpy(dSizeBot, Data.dSizeBot, sizeof(dSizeBot));
		dFyBot = Data.dFyBot;

		nCodeWeb = Data.nCodeWeb;
		nGradeWeb = Data.nGradeWeb;
		bUseStifWeb = Data.bUseStifWeb;
		nStiffenerTypeWeb = Data.nStiffenerTypeWeb;
		nStifNumWeb = Data.nStifNumWeb;
		dPitchWeb = Data.dPitchWeb;
		memcpy(dSizeWeb, Data.dSizeWeb, sizeof(dSizeWeb));
		dFyWeb = Data.dFyWeb;

		nCodeBearing = Data.nCodeBearing;
		nGradeBearing = Data.nGradeBearing;
		bUseStifBearing = Data.bUseStifBearing;
		nBearingType = Data.nBearingType;
		nBearingPairNum = Data.nBearingPairNum;
		dPitchBearing = Data.dPitchBearing;
		memcpy(dSizeBearing, Data.dSizeBearing, sizeof(dSizeBearing));
		dClip4Fillet = Data.dClip4Fillet;
		dFyBearing = Data.dFyBearing;

		StiffNR_Web    .Convert960_Temp2(Data.StiffNR_Web);
		StiffNR_Bearing.Convert960_Temp2(Data.StiffNR_Bearing);
	}
	void Convert960_Temp(T_CGTS_SUB_SSM_RW_960_Temp& Data)
	{
		nCodeTop = Data.nCodeTop;
		nGradeTop = Data.nGradeTop;
		bUseStifTop = Data.bUseStifTop;
		nStiffenerTypeTop = Data.nStiffenerTypeTop;
		nStifNumTop = Data.nStifNumTop;
		dPitchTop = Data.dPitchTop;
		memcpy(dSizeTop, Data.dSizeTop, sizeof(dSizeTop));
		dFyTop = Data.dFyTop;

		nCodeBot = Data.nCodeBot;
		nGradeBot = Data.nGradeBot;
		bUseStifBot = Data.bUseStifBot;
		nStiffenerTypeBot = Data.nStiffenerTypeBot;
		nStifNumBot = Data.nStifNumBot;
		dPitchBot = Data.dPitchBot;
		memcpy(dSizeBot, Data.dSizeBot, sizeof(dSizeBot));
		dFyBot = Data.dFyBot;

		nCodeWeb = Data.nCodeWeb;
		nGradeWeb = Data.nGradeWeb;
		bUseStifWeb = Data.bUseStifWeb;
		nStiffenerTypeWeb = Data.nStiffenerTypeWeb;
		nStifNumWeb = Data.nStifNumWeb;
		dPitchWeb = Data.dPitchWeb;
		memcpy(dSizeWeb, Data.dSizeWeb, sizeof(dSizeWeb));
		dFyWeb = Data.dFyWeb;

		nCodeBearing = Data.nCodeBearing;
		nGradeBearing = Data.nGradeBearing;
		bUseStifBearing = Data.bUseStifBearing;
		nBearingType = Data.nBearingType;
		nBearingPairNum = Data.nBearingPairNum;
		dPitchBearing = Data.dPitchBearing;
		memcpy(dSizeBearing, Data.dSizeBearing, sizeof(dSizeBearing));
		dClip4Fillet = Data.dClip4Fillet;
		dFyBearing = Data.dFyBearing;

		StiffNR_Web    .Convert960_Temp(Data.StiffNR_Web);
		StiffNR_Bearing.Convert960_Temp(Data.StiffNR_Bearing);
	}
	void Convert955(T_CGTS_SUB_SSM_RW_955& Data)
	{
		nCodeTop          = Data.nCodeTop;
		nGradeTop         = Data.nGradeTop;
		bUseStifTop       = Data.bUseStifTop;        
		nStiffenerTypeTop = Data.nStiffenerTypeTop;  
		nStifNumTop       = Data.nStifNumTop;        
		dPitchTop         = Data.dPitchTop;          
		memcpy(dSizeTop, Data.dSizeTop, sizeof(dSizeTop));  
		dFyTop            = Data.dFyTop;             
		
		nCodeBot          = Data.nCodeBot;
		nGradeBot         = Data.nGradeBot;
		bUseStifBot       = Data.bUseStifBot;        
		nStiffenerTypeBot = Data.nStiffenerTypeBot;  
		nStifNumBot       = Data.nStifNumBot;        
		dPitchBot         = Data.dPitchBot;          
		memcpy(dSizeBot, Data.dSizeBot, sizeof(dSizeBot));  
		dFyBot            = Data.dFyBot;             
		
		nCodeWeb          = Data.nCodeWeb;
		nGradeWeb         = Data.nGradeWeb;
		bUseStifWeb       = Data.bUseStifWeb;        
		nStiffenerTypeWeb = Data.nStiffenerTypeWeb;  
		nStifNumWeb       = Data.nStifNumWeb;        
		dPitchWeb         = Data.dPitchWeb;          
		memcpy(dSizeWeb, Data.dSizeWeb, sizeof(dSizeWeb));  
		dFyWeb            = Data.dFyWeb;     

		nCodeBearing        = Data.nCodeBearing;       
		nGradeBearing       = Data.nGradeBearing;      
		bUseStifBearing     = Data.bUseStifBearing;  
		nBearingType        = Data.nBearingType;        
		nBearingPairNum     = Data.nBearingPairNum;     
		dPitchBearing       = Data.dPitchBearing;       
		memcpy(dSizeBearing, Data.dSizeBearing, sizeof(dSizeBearing));
		dClip4Fillet        = Data.dClip4Fillet;
		dFyBearing          = Data.dFyBearing;  

		StiffNR_Web.Initialize();
		StiffNR_Bearing.Initialize();
	}
	void Convert842(T_CGTS_SUB_SSM_RW_842& rData)
	{
		nCodeTop          = rData.nCodeTop;
		nGradeTop         = rData.nGradeTop;
		bUseStifTop       = rData.bUseStifTop;       
		nStiffenerTypeTop = rData.nStiffenerTypeTop; 
		nStifNumTop       = rData.nStifNumTop;       
		dPitchTop         = rData.dPitchTop;         
		memcpy(dSizeTop, rData.dSizeTop, sizeof(dSizeTop));
		dFyTop            = rData.dFyTop;            

		nCodeBot          = rData.nCodeBot;
		nGradeBot         = rData.nGradeBot;
		bUseStifBot       = rData.bUseStifBot;       
		nStiffenerTypeBot = rData.nStiffenerTypeBot; 
		nStifNumBot       = rData.nStifNumBot;       
		dPitchBot         = rData.dPitchBot;         
		memcpy(dSizeBot, rData.dSizeBot, sizeof(dSizeBot));
		dFyBot            = rData.dFyBot;            

		nCodeWeb          = rData.nCodeWeb;
		nGradeWeb         = rData.nGradeWeb;
		bUseStifWeb       = rData.bUseStifWeb;       
		nStiffenerTypeWeb = rData.nStiffenerTypeWeb; 
		nStifNumWeb       = rData.nStifNumWeb;       
		dPitchWeb         = rData.dPitchWeb;         
		memcpy(dSizeWeb, rData.dSizeWeb, sizeof(dSizeWeb));
		dFyWeb            = rData.dFyWeb;            

		nCodeBearing        = 0;
		nGradeBearing       = 0;
		bUseStifBearing     = FALSE;
		nBearingType        = 0; 
		nBearingPairNum     = 1; 
		dPitchBearing       = 0.0;   
		memset(dSizeBearing, 0, sizeof(dSizeBearing));
		dClip4Fillet        = 0.0;
		dFyBearing          = 0.0;         

		StiffNR_Web.Initialize();
		StiffNR_Bearing.Initialize();
	}
};

struct T_CGTS_SSM_D
{
	BOOL bSameJ;  // I==J:TRUE, I!=J:FALSE
	T_CGTS_SUB_SSM aStiffener[2];
	void Initialize()
	{
		bSameJ = TRUE;
		aStiffener[0].Initialize();
		aStiffener[1].Initialize();
	}
	
	T_CGTS_SSM_D() { Initialize(); }
	T_CGTS_SSM_D(const T_CGTS_SSM_D& rData) { *this = rData; }
	T_CGTS_SSM_D& operator=(const T_CGTS_SSM_D& rData)
	{
		bSameJ = rData.bSameJ;
		aStiffener[0] = rData.aStiffener[0];
		aStiffener[1] = rData.aStiffener[1];
		return *this;
	}
};

struct T_CGTS_SSM_UDRD_D
{
	T_CGTS_SSM_K key;
	T_CGTS_SSM_D data;
};

struct T_CGTS_SSM_UDRD_D_RW_842
{
	T_CGTS_SSM_K key;
	BOOL bSameJ;
	T_CGTS_SUB_SSM_RW_842 aStiffener[2];
};

struct T_CGTS_SSM_UDRD_D_RW_955
{
	T_CGTS_SSM_K key;
	BOOL bSameJ;
	T_CGTS_SUB_SSM_RW_955 aStiffener[2];
};

struct T_CGTS_SSM_UDRD_D_RW_960_Temp
{
	T_CGTS_SSM_K key;
	BOOL bSameJ;
	T_CGTS_SUB_SSM_RW_960_Temp aStiffener[2];
};

struct T_CGTS_SSM_UDRD_D_RW_960_Temp2
{
	T_CGTS_SSM_K key;
	BOOL bSameJ;
	T_CGTS_SUB_SSM_RW_960_Temp2 aStiffener[2];
};

struct T_CGTS_SSM_UDRD_D_RW
{
	T_CGTS_SSM_K key;
	BOOL bSameJ;
	T_CGTS_SUB_SSM_RW aStiffener[2];
	
	void GetCgtsSsm(T_CGTS_SSM_K& rKey, T_CGTS_SSM_D& rData)
	{
		rKey = key;
		rData.bSameJ  = bSameJ;
		aStiffener[0].Get(rData.aStiffener[0]);
		aStiffener[1].Get(rData.aStiffener[1]);
	}
	void SetCgtsSsm(const T_CGTS_SSM_K Key, const T_CGTS_SSM_D& rData)
	{
		key = Key;
		bSameJ  = rData.bSameJ;
		aStiffener[0].Set(rData.aStiffener[0]);
		aStiffener[1].Set(rData.aStiffener[1]);
	}
	void Convert960_Temp2(T_CGTS_SSM_UDRD_D_RW_960_Temp2& rData)
	{
		key = rData.key;
		bSameJ = rData.bSameJ;
		aStiffener[0].Convert960_Temp2(rData.aStiffener[0]);
		aStiffener[1].Convert960_Temp2(rData.aStiffener[1]);
	}
	void Convert960_Temp(T_CGTS_SSM_UDRD_D_RW_960_Temp& rData)
	{
		key = rData.key;
		bSameJ = rData.bSameJ;
		aStiffener[0].Convert960_Temp(rData.aStiffener[0]);
		aStiffener[1].Convert960_Temp(rData.aStiffener[1]);
	}
	void Convert955(T_CGTS_SSM_UDRD_D_RW_955& rData)
	{
		key = rData.key;
		bSameJ = rData.bSameJ;
		aStiffener[0].Convert955(rData.aStiffener[0]);
		aStiffener[1].Convert955(rData.aStiffener[1]);
	}
	void Convert842(T_CGTS_SSM_UDRD_D_RW_842& rData)
	{
		key = rData.key;
		bSameJ  = rData.bSameJ;
		aStiffener[0].Convert842(rData.aStiffener[0]);
		aStiffener[1].Convert842(rData.aStiffener[1]);
	}
};

struct T_CGTS_SSM_UNIT
{
	T_CGTS_SSM_UNIT()
	{
		dLength     = D_UNITSYS_BASE_LENGTH;
		dFy         = D_UNITSYS_BASE_STRESS;
	}
	
	int dLength;
	int dFy;
};

#define HASHSIZESTFNSSM HASHSIZESECT

// Section Manager - Transverse Stiffener - Virtual Beam
typedef  T_CGTS_SSM_K  T_CGTV_SSM_K;
typedef  T_CGTS_SSM_D  T_CGTV_SSM_D;

struct T_CGTV_SSM_UDRD_D
{
	T_CGTV_SSM_K key;
	T_CGTV_SSM_D data;
};

struct T_CGTV_SSM_UNIT
{
	T_CGTV_SSM_UNIT()
	{
		dLength = D_UNITSYS_BASE_LENGTH;
		dFy = D_UNITSYS_BASE_STRESS;
	}

	int dLength;
	int dFy;
};

#define HASHSIZECGTVSSM HASHSIZESECV

// Section Manager - Tendon Table 용
struct T_SMTN_SSM_D
{
	CString strProfile;
	CString strProperty;
	T_POINT_SSM Point;
	double dArea;

	void Initialize()
	{
		strProfile.Empty();
		strProperty.Empty();
		Point.Initialize();
		dArea = 0.;
	}

	T_SMTN_SSM_D() { Initialize(); }
	T_SMTN_SSM_D(const T_SMTN_SSM_D& rData) { *this = rData; }
	T_SMTN_SSM_D& operator=(const T_SMTN_SSM_D& rData) 
	{
		strProfile = rData.strProfile;
		strProperty = rData.strProperty;
		Point = rData.Point;
		dArea = rData.dArea;
		
		return *this;
	}
};

// Section Manager - Slope
#define T_SLOP_SSM_K T_SECT_K
struct T_SLOP_SSM_D
{
	CArray<UINT, UINT> aTopLine;
	CArray<UINT, UINT> aBotLine;

	T_SLOP_SSM_D()
	{
	}
	T_SLOP_SSM_D(T_SLOP_SSM_D& src)
	{
		*this = src;
	}
	T_SLOP_SSM_D& operator=(const T_SLOP_SSM_D& src)
	{
		aTopLine.RemoveAll();
		aTopLine.Copy(src.aTopLine);

		aBotLine.RemoveAll();
		aBotLine.Copy(src.aBotLine);

		return *this;
	}
	void Initialize()
	{
		aTopLine.RemoveAll();
		aBotLine.RemoveAll();
	}
};
struct T_SLOP_SSM_D_CH
{
	CArray<UINT, UINT> aTopLine;
	CArray<UINT, UINT> aBotLine;
	void ConvertToString(T_SLOP_SSM_D& rData)
	{
		rData.aTopLine.RemoveAll();
		rData.aTopLine.Copy(aTopLine);
		rData.aBotLine.RemoveAll();
		rData.aBotLine.Copy(aBotLine);
	}
	void ConvertToChar(T_SLOP_SSM_D& rData)
	{
		aTopLine.RemoveAll();
		aTopLine.Copy(rData.aTopLine);
		aBotLine.RemoveAll();
		aBotLine.Copy(rData.aBotLine);
	}
};
struct T_SLOP_SSM_UDRD_D
{
	T_SLOP_SSM_K key;
	T_SLOP_SSM_D data;
};
struct T_SLOP_SSM_UDRD_D_CH
{
	T_SLOP_SSM_K key;
	T_SLOP_SSM_D_CH data;
};
struct T_SLOP_SSM_UDRD_D_CH_RW
{
	T_SLOP_SSM_K key;
	int nTopLine;
	int nBotLine;

	void GetSlopSsm(T_SLOP_SSM_K& rKey, T_SLOP_SSM_D_CH& rData)
	{
		rKey=key;
	}
	void SetSlopSsm(const T_SLOP_SSM_K Key, const T_SLOP_SSM_D_CH& rData)
	{
		key=Key;    
		nTopLine = (int)rData.aTopLine.GetSize();
		nBotLine = (int)rData.aBotLine.GetSize();
	}  
};
struct T_SLOP_SSM_UNIT
{
	T_SLOP_SSM_UNIT()
	{
		nType = D_UNITSYS_NONE;    
	}
	int nType;
};
#define HASHSIZESLOP_SSM HASHSIZESECT


//////////////////////////////////////////////////////////////////////////
// Add. Jaeoh [6/23/2010], Steel design용.
struct T_GLINE_ID
{
	CArray<int,int&> aLineIndex;
	void Initialize()
	{
		aLineIndex.RemoveAll();
	}

	T_GLINE_ID() { Initialize(); }
	T_GLINE_ID(const T_GLINE_ID& rData) { *this = rData; }
	T_GLINE_ID& operator=(const T_GLINE_ID& rData) 
	{    
		aLineIndex.Copy(rData.aLineIndex);    
		return *this;
	}
};

struct T_GENL_DATA
{
	CArray<T_GSEC_POLYGON,T_GSEC_POLYGON&> aSlabPolyD;
	T_GSEC_POLYGON PolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	
	void Initialize()
	{
		aSlabPolyD.RemoveAll();
		PolyD.Initialize();
		aLine.RemoveAll();
	}

	T_GENL_DATA() { Initialize(); }
	T_GENL_DATA(const T_GENL_DATA& rData) { *this = rData; }
	T_GENL_DATA& operator=(const T_GENL_DATA& rData) 
	{
		aSlabPolyD.Copy(rData.aSlabPolyD);
		PolyD = rData.PolyD;
		aLine.Copy(rData.aLine);
		
		return *this;
	}

};

struct T_GLINE_BASE
{  
	int iLineType;          // 0=Interanl, 1=Outstand
	double db;          // length,
	double dt;          // thickness,
	double dy1,dz1;     // 양끝단의 도심에서 떨어진 거리.
	double dy2,dz2;
	int Lix1, Lix2; // Line index
	int ve1, ve2; // 양 끝단에 있는 Line중 그 Line의 두 개 vertex중 끝단 vertex
	CArray<int, int&> aLineIx; // CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine 중 Stiffener로 나누어지는 Line(Stiffener가 없으면 자유돌출판 또는 양연지지)
	BOOL b1isFixed; // Outstand를 구성할 때 구속된 지점인가. 
	
	void Initialize()
	{
		iLineType=0;
		db = 0.0;
		dt = 0.0;
		dy1 = dz1 = 0.0;
		dy2 = dz2 = 0.0;
		Lix1 = Lix2 = 0;
		ve1 = ve2 = 0;
		aLineIx.RemoveAll();
		b1isFixed = FALSE;
	}

	T_GLINE_BASE() { Initialize(); }
	T_GLINE_BASE(const T_GLINE_BASE& rData) { *this = rData; }
	T_GLINE_BASE& operator=(const T_GLINE_BASE& rData) 
	{
		iLineType = rData.iLineType;
		db = rData.db;
		dt = rData.dt;
		dy1 = rData.dy1;
		dz1 = rData.dz1;
		dy2 = rData.dy2;
		dz2 = rData.dz2;
		Lix1 = rData.Lix1;
		Lix2 = rData.Lix2;
		ve1 = rData.ve1;
		ve2 = rData.ve2;
		aLineIx.Copy(rData.aLineIx);
		b1isFixed = rData.b1isFixed;
		
		return *this;
	}
};

struct T_SUB_PANEL_BASE
{
	int nType; // 0=양연압축에 시작 또는 끝, 1=양연압축, 2=양연압축/인장
	double dPsi;        // Sig2/Sig1, Sig1>Sig2
	double dk_sigma;    // buckling factor
	double dLamda_p;
	double dRho;      
	double dbc, dbt;    // 압축길이, 인장길이
	double db_inf, db_inf_eff;
	double db_sup, db_sup_eff;
	double dA_inf, dA_inf_eff;
	double dA_sup, dA_sup_eff; 
	double db_blank_eff;  // dbc - db_sup_eff - db_inf_eff
	
	void Initialize()
	{
		nType=0;
		dPsi=0.0;
		dk_sigma=0.0;
		dLamda_p=0.0;
		dRho=0.0;
		dbc = dbt = 0.0;
		db_inf = db_inf_eff = 0.0;
		db_sup = db_sup_eff = 0.0;
		dA_inf = dA_inf_eff = 0.0;
		dA_sup = dA_sup_eff = 0.0;  
		db_blank_eff = 0.0;
	}

	T_SUB_PANEL_BASE() { Initialize(); }
	T_SUB_PANEL_BASE(const T_SUB_PANEL_BASE& rData) { *this = rData; }
	T_SUB_PANEL_BASE& operator=(const T_SUB_PANEL_BASE& rData) 
	{
		nType      = rData.nType     ;
		dPsi       = rData.dPsi      ;
		dk_sigma   = rData.dk_sigma  ;
		dLamda_p   = rData.dLamda_p  ;
		dRho       = rData.dRho      ;
		dbc        = rData.dbc       ;
		dbt        = rData.dbt       ;
		db_inf     = rData.db_inf    ;
		db_inf_eff = rData.db_inf_eff;
		db_sup     = rData.db_sup    ;
		db_sup_eff = rData.db_sup_eff;
		dA_inf     = rData.dA_inf    ;
		dA_inf_eff = rData.dA_inf_eff;
		dA_sup     = rData.dA_sup    ;
		dA_sup_eff = rData.dA_sup_eff;
		db_blank_eff = rData.db_blank_eff;
		
		return *this;
	}
};

struct T_GLINE_DGN_B // T_GSEC_LINE 레벨
{
	int iLineType;
	double db;          // length,
	double dt;          // thickness,
	double dy1,dz1;     // 양끝단의 도심에서 떨어진 거리.
	double dy2,dz2;
	double dStr1, dStr2;
	double df1,df2;
	// -1=인장, 0=양연압축 이고 dStr1이 f1, 1=양연압축 이고 dStr2이 f1
	// 2=양연압축/인장 이고 dStr1이 f1, 3=양연압축/인장 이고 dStr2이 f1
	// Outstand일 경우는 지지된 곳을 0번째 Index로 채우자.
	int iStressType;   
	BOOL bCompStr;
	BOOL bDbCurve; // Double Curvature
	BOOL bIs1_sig1;
	double dy0,dz0; // Stress=0 인 지점.

	T_SUB_PANEL_BASE SubPanelD;
	
	void Initialize()
	{
		iLineType=0;
		db = 0.0;
		dt = 0.0;
		dy1 = dz1 = 0.0;
		dy2 = dz2 = 0.0;
		dStr1 = dStr2 = 0.0;
		df1 = df2 = 0.0;
		iStressType=-1;
		bCompStr = FALSE;
		bDbCurve = FALSE;
		bIs1_sig1 = FALSE;
		dy0 = dz0 = 0.0;    
		SubPanelD.Initialize();
	}

	T_GLINE_DGN_B() { Initialize(); }
	T_GLINE_DGN_B(const T_GLINE_DGN_B& rData) { *this = rData; }
	T_GLINE_DGN_B& operator=(const T_GLINE_DGN_B& rData) 
	{
		iLineType = rData.iLineType;
		db = rData.db;
		dt = rData.dt;
		dy1 = rData.dy1;
		dz1 = rData.dz1;
		dy2 = rData.dy2;
		dz2 = rData.dz2;
		dStr1 = rData.dStr1;
		dStr2 = rData.dStr2;
		df1 = rData.df1;
		df2 = rData.df2;
		iStressType = rData.iStressType;
		bCompStr = rData.bCompStr;
		bDbCurve = rData.bDbCurve;
		bIs1_sig1 = rData.bIs1_sig1;
		dy0 = rData.dy0;
		dz0 = rData.dz0;
		SubPanelD = rData.SubPanelD;
		
		return *this;
	}

};

struct T_GLINE_DGN_STFN
{
	int iStfnIndex; // T_GLINE_DST 의 Stiffener의 Index
	double dA,dIy,dZbar;
	double db_inf, db_inf_eff;
	double db_sup, db_sup_eff;
	double dAsl, dAsl_eff;
	double dIsl, dIsl_eff;
	double dIsl_sh;
	double dXsl;
	double de1,de2,de;
	double dSigma_sl;
	CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&> aStfnDgnBaseD; // Stiffener 유효단면 
	
	void Initialize()
	{
		iStfnIndex=0;
		dA=dIy=dZbar=0.0;
		db_inf=db_inf_eff=0.0;
		db_sup=db_sup_eff=0.0;
		dAsl=dIsl=dXsl=0.0;    
		dAsl_eff=dIsl_eff=0.0;
		dIsl_sh=0.0;
		de1=de2=de=0.0;
		dSigma_sl=0.0;
		aStfnDgnBaseD.RemoveAll();
	}
	T_GLINE_DGN_STFN() { Initialize(); }
	T_GLINE_DGN_STFN(const T_GLINE_DGN_STFN& rData) { *this = rData; }
	T_GLINE_DGN_STFN& operator=(const T_GLINE_DGN_STFN& rData) 
	{
		iStfnIndex = rData.iStfnIndex;
		dA         = rData.dA        ;
		dIy        = rData.dIy       ;
		dZbar      = rData.dZbar     ;
		db_inf     = rData.db_inf    ;
		db_inf_eff = rData.db_inf_eff;
		db_sup     = rData.db_sup    ;
		db_sup_eff = rData.db_sup_eff;
		dAsl       = rData.dAsl      ;
		dAsl_eff   = rData.dAsl_eff  ;
		dIsl       = rData.dIsl      ;
		dIsl_eff   = rData.dIsl_eff  ;
		dIsl_sh    = rData.dIsl_sh   ;
		dXsl       = rData.dXsl      ;
		de1        = rData.de1       ;
		de2        = rData.de2       ;
		de         = rData.de        ;
		dSigma_sl  = rData.dSigma_sl ;
		aStfnDgnBaseD.Copy(rData.aStfnDgnBaseD);

		return *this;
	}
};

struct T_GLINE_STIFFENER
{
	int nType; // I,T,L,U
	double dSize[20];
	int nPosLine; // Stiffener 위치 0:좌측 1:우측 2:양측
	int vi1, vi2; // Line 과 교차하는 vertex, I,T형일경우 vi1 하나만.
	CArray<int, int&> aLineIx; // stiffener를 구성 
	double dA;
	double dIy;
	double dZbar;

	void Initialize()
	{
		nType = 0;
		memset(dSize,0,sizeof(dSize));
		nPosLine=0;
		vi1=vi2=0;
		aLineIx.RemoveAll();
		dA=0.0;
		dIy=0.0;
		dZbar=0.0;
	}
	T_GLINE_STIFFENER() { Initialize(); }
	T_GLINE_STIFFENER(const T_GLINE_STIFFENER& rData) { *this = rData; }
	T_GLINE_STIFFENER& operator=(const T_GLINE_STIFFENER& rData) 
	{
		nType = rData.nType;
		memcpy(dSize,rData.dSize,sizeof(dSize));
		nPosLine = rData.nPosLine;
		vi1 = rData.vi1;
		vi2 = rData.vi2;
		aLineIx.Copy(rData.aLineIx);
		dA   = rData.dA;
		dIy  = rData.dIy;
		dZbar= rData.dZbar;
		
		return *this;
	}

};
// Top flange, left web등 한 영역을 나타냄.
struct T_GLINE_DST // Design shape type
{  
	int nType; // 0=Internal, 1=Outstand
	CString strShape;
	double db, dt;
	double dy1,dz1;     // 양끝단의 도심에서 떨어진 거리.
	double dy2,dz2;
	BOOL b1isFixed;
	CArray<T_GLINE_BASE,T_GLINE_BASE&> aLineDgn; 
	CArray<T_GLINE_STIFFENER,T_GLINE_STIFFENER&> aStiffener;
	void Initialize()
	{
		nType = 0;
		strShape.Empty();
		db=dt=0.0;
		dy1=dy2=0.0;
		dz1=dz2=0.0;
		b1isFixed=FALSE;
		aLineDgn.RemoveAll();
		aStiffener.RemoveAll();
	}

	T_GLINE_DST() { Initialize(); }
	T_GLINE_DST(const T_GLINE_DST& rData) { *this = rData; }
	T_GLINE_DST& operator=(const T_GLINE_DST& rData) 
	{
		nType = rData.nType;
		strShape = rData.strShape;
		db  = rData.db ;
		dt  = rData.dt ;
		dy1 = rData.dy1;
		dy2 = rData.dy2;
		dz1 = rData.dz1;
		dz2 = rData.dz2;
		b1isFixed = rData.b1isFixed;
		
		aLineDgn.Copy(rData.aLineDgn);
		aStiffener.Copy(rData.aStiffener);
		
		return *this;
	}
};

struct T_DGNLINE_PTB // Plate type buckling data
{
	double dgamma;
	double dalpha_lt;
	double ddelta;  
	double dk_sigma_p;
	double dSigma_cr_p;
	double dbeta_A_c;
	double dLamda_p;
	double dAc;
	double dAc_eff_loc;
	double dRho;
	void Initialize()
	{
		dgamma=0.0;
		dalpha_lt=0.0;
		ddelta=0.0;
		dk_sigma_p=0.0;
		dSigma_cr_p=0.0;
		dbeta_A_c=0.0;
		dLamda_p=0.0;
		dAc=0.0;
		dAc_eff_loc=0.0;
		dRho=0.0;
	}

	T_DGNLINE_PTB() { Initialize(); }
	T_DGNLINE_PTB(const T_DGNLINE_PTB& rData) { *this = rData; }
	T_DGNLINE_PTB& operator=(const T_DGNLINE_PTB& rData) 
	{
		dgamma     = rData.dgamma;
		dalpha_lt  = rData.dalpha_lt;
		ddelta     = rData.ddelta;
		dk_sigma_p = rData.dk_sigma_p;
		dSigma_cr_p= rData.dSigma_cr_p;
		dbeta_A_c  = rData.dbeta_A_c;
		dLamda_p   = rData.dLamda_p;
		dAc        = rData.dAc;
		dAc_eff_loc= rData.dAc_eff_loc;
		dRho       = rData.dRho;
		
		return *this;
	}
};

struct T_DGNLINE_CTB // Column type buckling data
{
	double dBeta_A_c;
	double dSigma_sl;
	double dSigma_cr_c;
	double dLamda_c;
	double di;
	double dalpha_E;
	double dPhi;
	double dXc;
	void Initialize()
	{
		dBeta_A_c=0.0;
		dSigma_sl=0.0;
		dLamda_c=0.0;
		di=0.0;
		dalpha_E=0.0;
		dPhi=0.0;
		dXc=0.0;
	}

	T_DGNLINE_CTB() { Initialize(); }
	T_DGNLINE_CTB(const T_DGNLINE_CTB& rData) { *this = rData; }
	T_DGNLINE_CTB& operator=(const T_DGNLINE_CTB& rData) 
	{
		dBeta_A_c   = rData.dBeta_A_c;
		dSigma_sl   = rData.dSigma_sl;
		dSigma_cr_c = rData.dSigma_cr_c;
		dLamda_c    = rData.dLamda_c;
		di          = rData.di;
		dalpha_E    = rData.dalpha_E;
		dPhi        = rData.dPhi;
		dXc         = rData.dXc;
		
		return *this;
	}
};

// Local buckling data, T_GLINE_DST 와 같은 레벨
struct T_DGNLINE_LBD 
{
	double dPsi; // Sigma2/Sigma1
	double dalpha; // a/b>=0.0
	double dbc; // 전체길이중 압축부분길이 
	double dbt; // 전체길이중 인장부분길이
	double dt;
	double dAp; // b*t
	double dIp; // is the second moment of area for bending of the plate = bt^3/(12(1-v))
	double dIsl_sum; // is the sum of the second moment of area of the whole stiffened plate
	double dAsl_sum; // is the sum of the gross area of individual longitudinal stiffeners;
	double dIsl_sh_sum;
	int iClass;
	// iLineStressType
	// -1=양연인장 
	// 0=양연압축이고 aDgnBaseD의 첫번째 dStr1이 Sig1(Sig1은 압축응력이며 둘 중 큰값)
	// 1=양연압축이고 aDgnBaseD의 마지막 dStr2가 Sig1
	// 2=양연압축/인장 aDgnBaseD의 첫번째 dStr1이 Sig1
	// 3=양연압축/인장 aDgnBaseD의 마지막 dStr2가 Sig1
	int iLineStressType;  
	int iCompPanelSize;
	CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&> aDgnBaseD;
	CArray<T_GLINE_DGN_STFN,T_GLINE_DGN_STFN&> aDgnStfnD;
	T_DGNLINE_PTB PlateBuckD;
	T_DGNLINE_CTB ColumnBuckD;
	double dxi;
	double dRho_c; // Interaction between plate and column buckling
	void Initialize()
	{
		dPsi = dalpha = 0.0;
		dbc = dbt = dt = 0.0;    
		dAp = 0.0;
		dIp = 0.0;
		dIsl_sum = dAsl_sum = 0.0;
		dIsl_sh_sum=0.0;
		iClass=0;
		iLineStressType=0;
		iCompPanelSize=0;
		aDgnBaseD.RemoveAll();
		aDgnStfnD.RemoveAll();
		PlateBuckD.Initialize();
		ColumnBuckD.Initialize();
		dxi=0.0;
		dRho_c=0.0;
	}

	T_DGNLINE_LBD() { Initialize(); }
	T_DGNLINE_LBD(const T_DGNLINE_LBD& rData) { *this = rData; }
	T_DGNLINE_LBD& operator=(const T_DGNLINE_LBD& rData) 
	{
		dPsi = rData.dPsi;
		dalpha = rData.dalpha;
		dbc = rData.dbc;
		dbt = rData.dbt;
		dt  = rData.dt;
		dAp = rData.dAp;
		dIp = rData.dIp;
		dIsl_sum = rData.dIsl_sum;
		dAsl_sum = rData.dAsl_sum;
		dIsl_sh_sum = rData.dIsl_sh_sum;
		iClass = rData.iClass;
		iLineStressType = rData.iLineStressType;
		iCompPanelSize = rData.iCompPanelSize;
		aDgnBaseD.Copy(rData.aDgnBaseD);
		aDgnStfnD.Copy(rData.aDgnStfnD);
		PlateBuckD = rData.PlateBuckD;
		ColumnBuckD = rData.ColumnBuckD;
		dxi = rData.dxi;
		dRho_c = rData.dRho_c;
		
		return *this;
	}
};

struct T_DLINE_PART_D
{
	int nPartType; // 0=Top flange, 1=Bottom flange, 3=web
	double db_in;
	double dt_in;
	CArray<T_DGNLINE_LBD,T_DGNLINE_LBD&> aOutstand; // 자유돌출판 
	CArray<T_DGNLINE_LBD,T_DGNLINE_LBD&> aInternal; // 양연지지.
	int iClassOut;
	int iClassIn;
	int iClass;

	void Initialize()
	{
		nPartType = 0;
		db_in=dt_in=0.0;
		aOutstand.RemoveAll();
		aInternal.RemoveAll();
		iClassOut=iClassIn=iClass=0;
	}
	
	T_DLINE_PART_D() { Initialize(); }
	T_DLINE_PART_D(const T_DLINE_PART_D& rData) { *this = rData; }
	T_DLINE_PART_D& operator=(const T_DLINE_PART_D& rData) 
	{
		nPartType = rData.nPartType;
		db_in = rData.db_in;
		dt_in = rData.dt_in;
		aOutstand.Copy(rData.aOutstand);
		aInternal.Copy(rData.aInternal);
		iClassOut = rData.iClassOut;
		iClassIn  = rData.iClassIn ;
		iClass    = rData.iClass   ;
		
		return *this;
	}
};

struct T_DLINE_CALC_D
{
	CArray<T_DLINE_PART_D,T_DLINE_PART_D&> aPartLine;
	int iClassFlgTop;
	int iClassFlgBot;
	int iClassWeb;
	int iClassFlg;
	int iClass;

	void Initialize()
	{
		aPartLine.RemoveAll();
		iClassFlgTop=0;
		iClassFlgBot=0;
		iClassWeb=0;
		iClassFlg=0;
		iClass=0;
	}
	T_DLINE_CALC_D() { Initialize(); }
	T_DLINE_CALC_D(const T_DLINE_CALC_D& rData) { *this = rData; }
	T_DLINE_CALC_D& operator=(const T_DLINE_CALC_D& rData) 
	{
		aPartLine.Copy(rData.aPartLine);
		iClassFlgTop=rData.iClassFlgTop;
		iClassFlgBot=rData.iClassFlgBot;
		iClassWeb=rData.iClassWeb;
		iClassFlg=rData.iClassFlg;
		iClass=rData.iClass;
		
		return *this;
	}
};

// T_GLINE_INFO로 만들 정보.
struct T_GLINE_PART_DGN
{  
	int nPartType; // 0=Top flange, 1=Bottom flange, 3=web
	CString strGroupName;
	double db_in;
	double dt_in;
	CArray<T_GLINE_DST,T_GLINE_DST&> aOutstand; // 자유돌출판 
	CArray<T_GLINE_DST,T_GLINE_DST&> aInternal; // 양연지지.

	void Initialize()
	{
		nPartType = 0;
		strGroupName.Empty();
		db_in=dt_in=0.0;
		aOutstand.RemoveAll();
		aInternal.RemoveAll();
	}
	
	T_GLINE_PART_DGN() { Initialize(); }
	T_GLINE_PART_DGN(const T_GLINE_PART_DGN& rData) { *this = rData; }
	T_GLINE_PART_DGN& operator=(const T_GLINE_PART_DGN& rData) 
	{
		nPartType = rData.nPartType;
		strGroupName = rData.strGroupName;
		db_in = rData.db_in;
		dt_in = rData.dt_in;
		aOutstand.Copy(rData.aOutstand);
		aInternal.Copy(rData.aInternal);
		
		return *this;
	}

};

struct T_GLINE_DGN_D
{
	T_GSEC_POLYGON ModifiedGsecPolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aModifiedLine;
	T_GSEC_POLYGON ModifiedSlabGsecPolyD;

	CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&> aPartLine;

	void Initialize()
	{
		ModifiedGsecPolyD.Initialize();
		aModifiedLine.RemoveAll();
		ModifiedSlabGsecPolyD.Initialize();
		aPartLine.RemoveAll();
	}
	T_GLINE_DGN_D() { Initialize(); }
	T_GLINE_DGN_D(const T_GLINE_DGN_D& rData) { *this = rData; }
	T_GLINE_DGN_D& operator=(const T_GLINE_DGN_D& rData) 
	{
		ModifiedGsecPolyD = rData.ModifiedGsecPolyD;
		aModifiedLine.Copy(rData.aModifiedLine);
		ModifiedSlabGsecPolyD = rData.ModifiedSlabGsecPolyD;
		aPartLine.Copy(rData.aPartLine);
		
		return *this;
	}
};

// SM으로 부터 만들 정보.
struct T_GLINE_SHAPE_STFN
{
	int nType;
	double dSize[20];
	int nPosLine; // Stiffener 위치 0:좌측 1:우측 2:양측
	void Initialize()
	{
		nType = 0;
		memset(dSize,0,sizeof(dSize));
		nPosLine=0;
	}
	T_GLINE_SHAPE_STFN() { Initialize(); }
	T_GLINE_SHAPE_STFN(const T_GLINE_SHAPE_STFN& rData) { *this = rData; }
	T_GLINE_SHAPE_STFN& operator=(const T_GLINE_SHAPE_STFN& rData) 
	{    
		nType = rData.nType;
		memcpy(dSize,rData.dSize,sizeof(dSize));
		nPosLine = rData.nPosLine;
		
		return *this;
	}
};
struct T_GLINE_SHAPE_B
{
	CString strShape;
	CArray<int,int&> aLineIx; // 하나의 Line 영역, 자유돌출판(outstand) 또는 양연지지(Internal)를 구성
	CArray<int,int&> aStiffVtxIx; // Stiffener가 있는 vertex index
	CMap<int,int&,T_GLINE_ID,T_GLINE_ID&> mapStfnLineD; // key=vertex index, Stiffener line info  
	CMap<int,int&,T_GLINE_SHAPE_STFN,T_GLINE_SHAPE_STFN&> mapStfnD; // key=vertex index, Stiffener line info  
	
	void Initialize()
	{
		strShape.Empty();
		aLineIx.RemoveAll();
		aStiffVtxIx.RemoveAll();
		mapStfnLineD.RemoveAll();
		mapStfnD.RemoveAll();
	}

	T_GLINE_SHAPE_B() { Initialize(); }
	T_GLINE_SHAPE_B(const T_GLINE_SHAPE_B& rData) { *this = rData; }
	T_GLINE_SHAPE_B& operator=(const T_GLINE_SHAPE_B& rData) 
	{    
		strShape = rData.strShape;
		aLineIx.Copy(rData.aLineIx);    
		aStiffVtxIx.Copy(rData.aStiffVtxIx);
		POSITION Pos = rData.mapStfnLineD.GetStartPosition();
		while(Pos)
		{
			T_GLINE_ID LineId;      
			int iVtxKey=0;
			rData.mapStfnLineD.GetNextAssoc(Pos, iVtxKey, LineId);
			mapStfnLineD.SetAt(iVtxKey, LineId);
		}    
		Pos = rData.mapStfnD.GetStartPosition();
		while(Pos)
		{
			T_GLINE_SHAPE_STFN StfnD;      
			int iVtxKey=0;
			rData.mapStfnD.GetNextAssoc(Pos, iVtxKey, StfnD);
			mapStfnD.SetAt(iVtxKey, StfnD);
		}
		return *this;
	}

};

struct T_GLINE_PART
{
	int nPartType; // 0=Top flange, 1=Bottom flange, 3=web
	CString strGroupName;
	CArray<T_GLINE_SHAPE_B,T_GLINE_SHAPE_B&> aOutstand; // 자유돌출판 
	CArray<T_GLINE_SHAPE_B,T_GLINE_SHAPE_B&> aInternal; // 양연지지.

	void Initialize()
	{
		nPartType=0;
		strGroupName.Empty();
		aOutstand.RemoveAll();
		aInternal.RemoveAll();
	}

	T_GLINE_PART() { Initialize(); }
	T_GLINE_PART(const T_GLINE_PART& rData) { *this = rData; }
	T_GLINE_PART& operator=(const T_GLINE_PART& rData) 
	{    
		nPartType = rData.nPartType;
		strGroupName = rData.strGroupName;
		aOutstand.Copy(rData.aOutstand);    
		aInternal.Copy(rData.aInternal);
		
		return *this;
	}
};

struct T_GLINE_INFO
{
	T_GSEC_POLYGON GsecPolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;

	T_GSEC_POLYGON ModifiedGsecPolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aModifiedLine;

	CArray<T_GLINE_PART, T_GLINE_PART&> aPartInfo; // Top flange, Left web ..

	void Initialize()
	{
		GsecPolyD.Initialize();
		aLine.RemoveAll();
		aPartInfo.RemoveAll();
	}
	
	T_GLINE_INFO() { Initialize(); }
	T_GLINE_INFO(const T_GLINE_INFO& rData) { *this = rData; }
	T_GLINE_INFO& operator=(const T_GLINE_INFO& rData) 
	{
		GsecPolyD = rData.GsecPolyD;
		aLine.Copy(rData.aLine);
		ModifiedGsecPolyD = rData.ModifiedGsecPolyD;
		aModifiedLine.Copy(rData.aModifiedLine);
		aPartInfo.Copy(rData.aPartInfo);
		
		return *this;
	}

};

struct T_STFN_DIV_INFO
{
	CArray<T_GSEC_LINE,T_GSEC_LINE&> aDivLineD;
	//CArray<int,int&> aDivLine; // Stiffener때문에 분리된 Line index
	CArray<int,int&> aStfnVtxId; // Stiffener가 연결된 Vertex Id
	void Initialize()
	{
		aDivLineD.RemoveAll();
		aStfnVtxId.RemoveAll();
	}

	T_STFN_DIV_INFO() { Initialize(); }
	T_STFN_DIV_INFO(const T_STFN_DIV_INFO& rData) { *this = rData; }
	T_STFN_DIV_INFO& operator=(const T_STFN_DIV_INFO& rData) 
	{
		aDivLineD.Copy(rData.aDivLineD);
		aStfnVtxId.Copy(rData.aStfnVtxId);    
		return *this;
	}

};
struct T_GLINE_DIV
{
	CMap<int,int&,T_STFN_DIV_INFO,T_STFN_DIV_INFO&> mapLineInfo;
	CMap<int,int&,T_STFN_DIV_INFO,T_STFN_DIV_INFO&> mapStfnLineInfo; // int= stiffener vertex
	CMap<int,int&,T_GLINE_SHAPE_STFN,T_GLINE_SHAPE_STFN&> mapStfnShapeD; // int= stiffener vertex
	void Initialize()
	{
		mapLineInfo.RemoveAll();    
		mapStfnLineInfo.RemoveAll();
		mapStfnShapeD.RemoveAll();
	}

	T_GLINE_DIV() { Initialize(); }
	T_GLINE_DIV(const T_GLINE_DIV& rData) { *this = rData; }
	T_GLINE_DIV& operator=(const T_GLINE_DIV& rData) 
	{    
		POSITION Pos = rData.mapLineInfo.GetStartPosition();
		while(Pos)
		{
			T_STFN_DIV_INFO DivLine;      
			int iLineKey=0;
			rData.mapLineInfo.GetNextAssoc(Pos, iLineKey, DivLine);
			mapLineInfo.SetAt(iLineKey, DivLine);
		}
		//
		Pos = rData.mapStfnLineInfo.GetStartPosition();
		while(Pos)
		{
			T_STFN_DIV_INFO DivLine;      
			int iLineKey=0;
			rData.mapStfnLineInfo.GetNextAssoc(Pos, iLineKey, DivLine);
			mapStfnLineInfo.SetAt(iLineKey, DivLine);
		}
		//
		Pos = rData.mapStfnShapeD.GetStartPosition();
		while(Pos)
		{
			T_GLINE_SHAPE_STFN StfnShape;      
			int iVtxKey=0;
			rData.mapStfnShapeD.GetNextAssoc(Pos, iVtxKey, StfnShape);
			mapStfnShapeD.SetAt(iVtxKey, StfnShape);
		}    
		return *this;
	}
};


// Points of Stiffeners for SOD
struct T_STPO_Base
{
	CArray<T_POINT_SSM, T_POINT_SSM&> aStiffenerPoint;
	
	CString strStiffName;  // stiffner name

	int nType;     // 0: flat, 1:T, 2:U

	int nDeckPos;  // I   : 0=Top, 1=Web, 2=Bottom
	               // Box : 0=Top, 1=Left Web, 2=Left Web, =Bottom
	int nDeckPart;
	
	void Initialize()
	{
		aStiffenerPoint.RemoveAll();
		strStiffName="";
		nType =0;
		nDeckPos = 0;
		nDeckPart = 0;
	}
	T_STPO_Base () { Initialize(); }
	T_STPO_Base(const T_STPO_Base& rData) {*this = rData;}
	T_STPO_Base& operator=(const T_STPO_Base& rData)
	{
		aStiffenerPoint.Copy(rData.aStiffenerPoint);
		strStiffName = rData.strStiffName;
		nType = rData.nType;
		nDeckPos = rData.nDeckPos;
		nDeckPart = rData.nDeckPart;
		
		return *this;
	}
};


struct T_STPO_D
{
	CArray<T_STPO_Base, T_STPO_Base&> aSTPO_Base;

	void Initialize()
	{
		aSTPO_Base.RemoveAll();
	}
	T_STPO_D () { Initialize(); }
	T_STPO_D(const T_STPO_D& rData) {*this = rData;}
	T_STPO_D& operator=(const T_STPO_D& rData)
	{
		aSTPO_Base.Copy(rData.aSTPO_Base);
		
		return *this;
	}

};


//////////////////////////////////////////////////////////////////////////
// PSC Value 유효폭
struct T_EFWD_FLANGE_D			
{
	CString strName;
	int nTop;		   //0: Top  1: Bottom
	CArray<UINT, UINT> aLine;		

	void Initialize()
	{
		strName.Empty();
		nTop = 0;
		aLine.RemoveAll();
	}

	T_EFWD_FLANGE_D() {Initialize();}
	T_EFWD_FLANGE_D(const T_EFWD_FLANGE_D& rData) { *this = rData; }
	T_EFWD_FLANGE_D& operator=(const T_EFWD_FLANGE_D& rData)
	{
		strName = rData.strName;
		nTop = rData.nTop;
		aLine.Copy(rData.aLine);
		return *this;
	}
};
struct T_EFWD_FLANGE_D_CH
{
	char strName[40];
	int nTop;	
	CArray<UINT, UINT> aLine;		

	void ConvertToString(T_EFWD_FLANGE_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nTop = nTop;
		rData.aLine.Copy(aLine);
	}
	void ConvertToChar(T_EFWD_FLANGE_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nTop = rData.nTop;
		aLine.Copy(rData.aLine);
	}
};
struct T_EFWD_FLANGE_D_CH_RW
{
	char strName[40];
	int nTop;		 //0 : Top   1: Bottom
	int  nCountLine;		

	void Get(T_EFWD_FLANGE_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));  
		rData.nTop = nTop;
		rData.aLine.SetSize(nCountLine);
	}
	void Set(T_EFWD_FLANGE_D_CH& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));    
		nTop = Data.nTop;
		nCountLine = (int)Data.aLine.GetSize();
	}
};

struct T_EFWD_WEB_D
{
	CString strName;
	int nDirection;	// 0: 수직  1: web 기울기		
	CArray<UINT, UINT> aLeftLine;			
	CArray<UINT, UINT> aRightLine;	

	void Initialize()
	{
		strName.Empty();
		nDirection = 0;
		aLeftLine.RemoveAll();
		aRightLine.RemoveAll();
	}

	T_EFWD_WEB_D() {Initialize();}
	T_EFWD_WEB_D(const T_EFWD_WEB_D& rData) { *this = rData; }
	T_EFWD_WEB_D& operator=(const T_EFWD_WEB_D& rData)
	{
		strName = rData.strName;
		nDirection = rData.nDirection;
		aLeftLine.Copy(rData.aLeftLine);
		aRightLine.Copy(rData.aRightLine);

		return *this;
	}
};
struct T_EFWD_WEB_D_CH
{
	char strName[40];
	int nDirection;			
	CArray<UINT, UINT> aLeftLine;			
	CArray<UINT, UINT> aRightLine;	

	void ConvertToString(T_EFWD_WEB_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nDirection = nDirection;
		rData.aLeftLine.Copy(aLeftLine);
		rData.aRightLine.Copy(aRightLine);
	}
	void ConvertToChar(T_EFWD_WEB_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nDirection = rData.nDirection;
		aLeftLine.Copy(rData.aLeftLine);
		aRightLine.Copy(rData.aRightLine);
	}
};

struct T_EFWD_WEB_D_CH_RW
{
	char strName[40];
	int nDirection;			
	int  nCountLeft;
	int  nCountRight;

	void Get(T_EFWD_WEB_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));  
		rData.nDirection = nDirection;
		rData.aLeftLine.SetSize(nCountLeft);
		rData.aRightLine.SetSize(nCountRight);
	}
	void Set(T_EFWD_WEB_D_CH& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));    
		nDirection = Data.nDirection;
		nCountLeft = (int)Data.aLeftLine.GetSize();
		nCountRight = (int)Data.aRightLine.GetSize();
	}
};


struct T_EFWD_WIDTH_D
{
	double dWidth;	
	double dFactor;	

	void Initialize()
	{
		dWidth = 0.0;
		dFactor = 1.0;
	}

	T_EFWD_WIDTH_D() {Initialize();}
	T_EFWD_WIDTH_D(const T_EFWD_WIDTH_D& rData) { *this = rData; }
	T_EFWD_WIDTH_D& operator=(const T_EFWD_WIDTH_D& rData)
	{
		dWidth = rData.dWidth;
		dFactor = rData.dFactor;

		return *this;
	}
};

#define HASHSIZEEFWD 1001 // 소수
#define T_EFWD_K unsigned int
struct T_EFWD_D
{
	BOOL bSameJ; 
	CArray<T_EFWD_FLANGE_D, T_EFWD_FLANGE_D&> aFlangeLine[2];	// 플랜지 line	
	CArray<T_EFWD_WEB_D,    T_EFWD_WEB_D&>    aWebLine[2];		// Web line

	CArray<T_EFWD_WIDTH_D, T_EFWD_WIDTH_D&> aTobWidth[2];	// 상부플랜지 구간별 width	
	CArray<T_EFWD_WIDTH_D, T_EFWD_WIDTH_D&> aBotWidth[2];	// 하부플랜지 구간별 width	

	void Initialize()
	{
		bSameJ = TRUE;
		for(int i=0; i<2; i++)
		{
			aFlangeLine[i].RemoveAll();
			aWebLine[i].RemoveAll();
			aTobWidth[i].RemoveAll();
			aBotWidth[i].RemoveAll();
		}
	}

	T_EFWD_D() {Initialize();}
	T_EFWD_D(const T_EFWD_D& rData) { *this = rData; }
	T_EFWD_D& operator=(const T_EFWD_D& rData)
	{
		bSameJ = rData.bSameJ;
		for(int i=0; i<2; i++)
		{
			aFlangeLine[i].Copy(rData.aFlangeLine[i]);
			aWebLine[i].Copy(rData.aWebLine[i]);
			aTobWidth[i].Copy(rData.aTobWidth[i]);
			aBotWidth[i].Copy(rData.aBotWidth[i]);
		}

		return *this;
	}
};

struct T_EFWD_D_CH
{
	BOOL bSameJ; 
	CArray<T_EFWD_FLANGE_D_CH, T_EFWD_FLANGE_D_CH&> aFlangeLine[2];	
	CArray<T_EFWD_WEB_D_CH,    T_EFWD_WEB_D_CH&>    aWebLine[2];	

	CArray<T_EFWD_WIDTH_D, T_EFWD_WIDTH_D&> aTobWidth[2];
	CArray<T_EFWD_WIDTH_D, T_EFWD_WIDTH_D&> aBotWidth[2];

	void ConvertToString(T_EFWD_D& rData)
	{
		rData.bSameJ = bSameJ;
		for(int i=0; i<2; i++)
		{
			int nCount = 0;
			nCount = (int)aFlangeLine[i].GetSize();
			rData.aFlangeLine[i].SetSize(nCount);
			for (int j = 0; j < nCount; j++) aFlangeLine[i][j].ConvertToString(rData.aFlangeLine[i][j]);	

			nCount = (int)aWebLine[i].GetSize();
			rData.aWebLine[i].SetSize(nCount);
			for (int j = 0; j < nCount; j++) aWebLine[i][j].ConvertToString(rData.aWebLine[i][j]);

			rData.aTobWidth[i].Copy(aTobWidth[i]);
			rData.aBotWidth[i].Copy(aBotWidth[i]);
		}
	}
	void ConvertToChar(T_EFWD_D& rData)
	{
		bSameJ = rData.bSameJ;

		for(int i=0; i<2; i++)
		{
			int nCount = 0;
			nCount = (int)rData.aFlangeLine[i].GetSize();
			aFlangeLine[i].SetSize(nCount);
			for (int j = 0; j < nCount; j++) aFlangeLine[i][j].ConvertToChar(rData.aFlangeLine[i][j]);

			nCount = (int)rData.aWebLine[i].GetSize();
			aWebLine[i].SetSize(nCount);
			for (int j = 0; j < nCount; j++) aWebLine[i][j].ConvertToChar(rData.aWebLine[i][j]);

			aTobWidth[i].Copy(rData.aTobWidth[i]);
			aBotWidth[i].Copy(rData.aBotWidth[i]);
		}

	}
};

struct T_EFWD_UDRD_D
{
	T_EFWD_K key;
	T_EFWD_D data;
};

struct T_EFWD_UDRD_D_CH
{
	T_EFWD_K key;
	T_EFWD_D_CH data;
};

struct T_EFWD_UDRD_D_CH_RW
{
	T_EFWD_K key;
	BOOL bSameJ; 
	int  nCountFlange[2];
	int  nCountWeb[2];

	int  nTobWidth[2];
	int  nBotWidth[2];

	void GetEfwd(T_EFWD_K& rKey, T_EFWD_D_CH& rData)
	{
		rKey = key;
		rData.bSameJ = bSameJ;

		for(int i=0; i<2; i++)
		{
			rData.aFlangeLine[i].SetSize(nCountFlange[i]);
			rData.aWebLine[i]   .SetSize(nCountWeb[i]);
			rData.aTobWidth[i]  .SetSize(nTobWidth[i]);
			rData.aBotWidth[i]  .SetSize(nBotWidth[i]);
		}
	}
	void SetEfwd(const T_EFWD_K Key, const T_EFWD_D_CH& rData)
	{
		key = Key;
		bSameJ = rData.bSameJ;
		for(int i=0; i<2; i++)
		{
			nCountFlange[i] = (int)rData.aFlangeLine[i].GetSize();
			nCountWeb[i]    = (int)rData.aWebLine[i].GetSize();
			nTobWidth[i]    = (int)(rData.aTobWidth[i].GetSize());
			nBotWidth[i]    = (int)(rData.aBotWidth[i].GetSize());
		}
	}
};


struct T_EFWD_UNIT
{
	T_EFWD_UNIT()
	{ 
		dWidth = D_UNITSYS_BASE_LENGTH;
	}
	int dWidth;
};

// Section Manager - Corrosion Depth
#define T_CRDP_SSM_K T_SECT_K
struct T_CRDP_SUB_SSM
{
	int nComp; // 0:Top Flange, 1:Web, 2:Bottom Flange, 3~10:Angle
	int nPos;  // Flange - Top / Bot / Left Edge / Right Edge
	           // Web - Left / Right / Left + Right
	           // Angle - Vertical / Horizontal
	double dCorrDepth;

	void Initialize()
	{
		nComp = 0;
		nPos = 0;
		dCorrDepth = 0.0;
	}
	T_CRDP_SUB_SSM() { Initialize(); }
	T_CRDP_SUB_SSM(const T_CRDP_SUB_SSM& rData) { *this = rData; }
	T_CRDP_SUB_SSM& operator=(const T_CRDP_SUB_SSM& rData)
	{
		nComp = rData.nComp;
		nPos = rData.nPos;
		dCorrDepth = rData.dCorrDepth;

		return (*this);
	}
};

struct T_CRDP_SSM_D
{
	BOOL bSameIJ;   // I==J:TRUE, I!=J:FALSE
	CArray<T_CRDP_SUB_SSM, T_CRDP_SUB_SSM&> CrdpSub[2];

	void Initialize()
	{
		bSameIJ = TRUE;
		CrdpSub[0].RemoveAll();
		CrdpSub[1].RemoveAll();
	}
	T_CRDP_SSM_D() { Initialize(); }
	T_CRDP_SSM_D(const T_CRDP_SSM_D& rData) { *this = rData; }
	T_CRDP_SSM_D& operator=(const T_CRDP_SSM_D& rData)
	{
		bSameIJ = rData.bSameIJ;
		CrdpSub[0].Copy(rData.CrdpSub[0]);
		CrdpSub[1].Copy(rData.CrdpSub[1]);

		return (*this);
	}
};

struct T_CRDP_SSM_UDRD_D
{
	T_CRDP_SSM_K key;
	T_CRDP_SSM_D data;
};

struct T_CRDP_SSM_D_RW
{
	BOOL bSameIJ;
	int  nCountCrdpSub[2];

	void Get(T_CRDP_SSM_D& rData)
	{
		rData.bSameIJ = bSameIJ;
		rData.CrdpSub[0].SetSize(nCountCrdpSub[0]);
		rData.CrdpSub[1].SetSize(nCountCrdpSub[1]);
	}
	void Set(T_CRDP_SSM_D Data)
	{
		bSameIJ = Data.bSameIJ;
		nCountCrdpSub[0] = (int)Data.CrdpSub[0].GetSize();
		nCountCrdpSub[1] = (int)Data.CrdpSub[1].GetSize();
	}
};

struct T_CRDP_SSM_UDRD_D_RW
{
	T_CRDP_SSM_K key;
	T_CRDP_SSM_D_RW data;

	void GetCrdpSsm(T_CRDP_SSM_K& rKey, T_CRDP_SSM_D& rData)
	{
		rKey = key;
		data.Get(rData);
	}
	void SetCrdpSsm(const T_CRDP_SSM_K Key, const T_CRDP_SSM_D& rData)
	{
		key = Key;
		data.Set(rData);
	}
};

struct T_CRDP_SSM_UNIT
{
	T_CRDP_SSM_UNIT()
	{
		dCorrDepth = D_UNITSYS_BASE_LENGTH;
	}
	int dCorrDepth;
};
#define HASHSIZECRDPSSM HASHSIZESECT


#pragma pack(pop)

#endif