#ifndef __DB_ST_DT_THIK_H__
#define __DB_ST_DT_THIK_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_THIK_K unsigned int
struct T_THIK_D_431
{
	int nThickType;  // 0=In&Out , 1=In,Out 따로
	double ThickIn;        // L, nThickType = 0 In/Out공용으로 사용
	double ThickOut;       // L, nThickType = 0 사용안됨
};
struct T_THIK_UDRD_D_431
{
	T_THIK_K key;
	T_THIK_D_431 data;
};

struct T_THIK_D_413
{
	double Thickness;
};
struct T_THIK_UDRD_D_413
{
	T_THIK_K key;
	T_THIK_D_413 data;
};
// nThickType == 1 인경우
// 질량,자중 계산:InPlane사용, "0."이면 OutPlane사용
// Thermal Gradient : OutPlane 사용
// Display : 둘중 두꺼운 것
// Edge 하중:Inplane
// Wall은 nThickType == 0 만 가능
struct T_THIK_VALUE_730
{
	int nThickType;   // 0=In&Out , 1=In,Out 따로
	double ThickIn;        // L, nThickType = 0 In/Out공용으로 사용
	double ThickOut;       // L, nThickType = 0 사용안됨
};
struct T_THIK_VALUE_883
{
	int nThickType;   // 0=In&Out , 1=In,Out 따로
	double ThickIn;        // L, nThickType = 0 In/Out공용으로 사용
	double ThickOut;       // L, nThickType = 0 사용안됨

	BOOL   bOffset;
	int    nOffsetType; // 0:ratio, 1:value
	double dValue;
	double dRatio;
};
struct T_THIK_VALUE
{
	CString strName;
	int nThickType;   // 0=In&Out , 1=In,Out 따로
	double ThickIn;        // L, nThickType = 0 In/Out공용으로 사용
	double ThickOut;       // L, nThickType = 0 사용안됨

	BOOL   bOffset;
	int    nOffsetType; // 0:ratio, 1:value
	double dValue;
	double dRatio;
	void Initialize()
	{
		strName.Empty();
		nThickType = 0;
		ThickIn = 0.0;
		ThickOut = 0.0;
		bOffset = FALSE;
		nOffsetType = 0;
		dValue = 0.0;
		dRatio = 0.5;
	}
	void Convert730(T_THIK_VALUE_730 &rData)
	{
		strName.Empty();
		nThickType = rData.nThickType;
		ThickIn = rData.ThickIn;
		ThickOut = rData.ThickOut;
		bOffset = FALSE;
		nOffsetType = 0;
		dValue = 0.0;
		dRatio = 0.0;
	}
	void Convert883(T_THIK_VALUE_883 &rData)
	{
		strName.Empty();
		nThickType = rData.nThickType;
		ThickIn = rData.ThickIn;
		ThickOut = rData.ThickOut;
		bOffset = rData.bOffset;
		nOffsetType = rData.nOffsetType;
		dValue = rData.dValue;
		dRatio = rData.dRatio;
	}
};

struct T_THIK_VALUE_CH
{
	char Name[40];
	int nThickType;   // 0=In&Out , 1=In,Out 따로
	double ThickIn;        // L, nThickType = 0 In/Out공용으로 사용
	double ThickOut;       // L, nThickType = 0 사용안됨

	BOOL   bOffset;
	int    nOffsetType; // 0:ratio, 1:value
	double dValue;
	double dRatio;
	void Initialize()
	{
		memset(Name, ' ', sizeof(Name));
		nThickType = 0;
		ThickIn = 0.0;
		ThickOut = 0.0;
		bOffset = FALSE;
		nOffsetType = 0;
		dValue = 0.0;
		dRatio = 0.5;
	}
	void ConvertToString(T_THIK_VALUE& rData)
	{
		ConvertCharStr(Name, rData.strName, sizeof(Name));
		rData.nThickType = nThickType;
		rData.ThickIn = ThickIn;
		rData.ThickOut = ThickOut;
		rData.bOffset = bOffset;
		rData.nOffsetType =nOffsetType;
		rData.dValue = dValue;
		rData.dRatio = dRatio;
	}
	void ConvertToChar(T_THIK_VALUE& rData)
	{
		ConvertStrChar(rData.strName, Name, sizeof(Name));
		nThickType = rData.nThickType;
		ThickIn = rData.ThickIn;
		ThickOut = rData.ThickOut;
		bOffset = rData.bOffset;
		nOffsetType = rData.nOffsetType;
		dValue = rData.dValue;
		dRatio = rData.dRatio;
	}
	void Convert730(T_THIK_VALUE_730 &rData)
	{
		memset(Name, ' ', sizeof(Name));
		nThickType = rData.nThickType;
		ThickIn = rData.ThickIn;
		ThickOut = rData.ThickOut;
		bOffset = FALSE;
		nOffsetType = 0;
		dValue = 0.0;
		dRatio = 0.0;
	}
	void Convert883(T_THIK_VALUE_883 &rData)
	{
		memset(Name, ' ', sizeof(Name));
		nThickType = rData.nThickType;
		ThickIn = rData.ThickIn;
		ThickOut = rData.ThickOut;
		bOffset = rData.bOffset;
		nOffsetType = rData.nOffsetType;
		dValue = rData.dValue;
		dRatio = rData.dRatio;
	}
};

struct T_THIK_VALUE_UNIT
{
	T_THIK_VALUE_UNIT()
	{
		nThickType = D_UNITSYS_NONE;
		ThickIn = D_UNITSYS_BASE_LENGTH;
		ThickOut = D_UNITSYS_BASE_LENGTH;
		bOffset = D_UNITSYS_NONE;
		nOffsetType = D_UNITSYS_NONE;
		dValue = D_UNITSYS_BASE_LENGTH;
		dRatio = D_UNITSYS_NONE;
	}
	int nThickType;
	int ThickIn;
	int ThickOut;
	int bOffset;
	int nOffsetType;
	int dValue;
	int dRatio;
};

// stiffened thickness
struct T_THIK_STIFFENED_SECT
{
	BOOL    bRibAttached;
	CString Shape;
	CString SName;
	double Dist;
	double Size[10];
	double PeriIn;
	double PeriOut;
	T_SECT_STIFFNESS Stiffness;
	void Initialize()
	{
		bRibAttached = FALSE;
		Shape.Empty();
		SName.Empty();
		Dist = 0.0;
		for (int i = 0; i < 10; i++) Size[i] = 0.0;
		PeriIn = 0.0;
		PeriOut = 0.0;
		Stiffness.Initialize();
	}
};

struct T_THIK_STIFFENED_SECT_CH_501
{
	BOOL bRibAttached;
	char Shape[4];
	char SName[28];
	double Dist;
	double Size[10];
	double Peri;
	T_SECT_STIFFNESS_600 Stiffness;
};
struct T_THIK_STIFFENED_SECT_CH_600
{
	BOOL bRibAttached;
	char Shape[4];
	char SName[28];
	double Dist;
	double Size[10];
	double PeriIn;
	double PeriOut;
	T_SECT_STIFFNESS_600 Stiffness;
};
struct T_THIK_STIFFENED_SECT_CH_815
{
	BOOL bRibAttached;
	char Shape[4];
	char SName[28];
	double Dist;
	double Size[10];
	double PeriIn;
	double PeriOut;
	T_SECT_STIFFNESS_815 Stiffness;
};
struct T_THIK_STIFFENED_SECT_CH
{
	BOOL bRibAttached;
	char Shape[4];
	char SName[28];
	double Dist;
	double Size[10];
	double PeriIn;
	double PeriOut;
	T_SECT_STIFFNESS Stiffness;
	void Initialize()
	{
		bRibAttached = FALSE;
		memset(Shape, ' ', sizeof(Shape));
		memset(SName, ' ', sizeof(SName));
		Dist = 0.0;
		for (int i = 0; i < 10; i++) Size[i] = 0.0;
		PeriIn = 0.0;
		PeriOut = 0.0;
		Stiffness.Initialize();
	}
	void ConvertToString(T_THIK_STIFFENED_SECT& rData)
	{
		rData.bRibAttached = bRibAttached;
		ConvertCharStr(Shape, rData.Shape, sizeof(Shape));
		ConvertCharStr(SName, rData.SName, sizeof(SName));
		rData.Dist = Dist;
		memcpy(rData.Size, Size, sizeof(Size));
		rData.PeriIn = PeriIn;
		rData.PeriOut = PeriOut;
		rData.Stiffness = Stiffness;
	}
	void ConvertToChar(T_THIK_STIFFENED_SECT& rData)
	{
		bRibAttached = rData.bRibAttached;
		ConvertStrChar(rData.Shape, Shape, sizeof(Shape));
		ConvertStrChar(rData.SName, SName, sizeof(SName));
		Dist = rData.Dist;
		memcpy(Size, rData.Size, sizeof(Size));
		PeriIn = rData.PeriIn;
		PeriOut = rData.PeriOut;
		Stiffness = rData.Stiffness;
	}
	void Convert501(T_THIK_STIFFENED_SECT_CH_501& rData)
	{
		bRibAttached = rData.bRibAttached;
		memcpy(Shape, rData.Shape, sizeof(Shape));
		memcpy(SName, rData.SName, sizeof(SName));
		Dist = rData.Dist;
		memcpy(Size, rData.Size, sizeof(Size));
		PeriIn = 0.0;
		PeriOut = rData.Peri;
		Stiffness.Convert600(rData.Stiffness);
	}
	void Convert600(T_THIK_STIFFENED_SECT_CH_600& rData)
	{
		bRibAttached = rData.bRibAttached;
		memcpy(Shape, rData.Shape, sizeof(Shape));
		memcpy(SName, rData.SName, sizeof(SName));
		Dist = rData.Dist;
		memcpy(Size, rData.Size, sizeof(Size));
		PeriIn = rData.PeriIn;
		PeriOut = rData.PeriOut;
		Stiffness.Convert600(rData.Stiffness);
	}
	void Convert815(T_THIK_STIFFENED_SECT_CH_815& rData)
	{
		bRibAttached = rData.bRibAttached;
		memcpy(Shape, rData.Shape, sizeof(Shape));
		memcpy(SName, rData.SName, sizeof(SName));
		Dist = rData.Dist;
		memcpy(Size, rData.Size, sizeof(Size));
		PeriIn = rData.PeriIn;
		PeriOut = rData.PeriOut;
		Stiffness.Convert815(rData.Stiffness);
	}
};

struct T_THIK_STIFFENED_SECT_UNIT
{
	T_THIK_STIFFENED_SECT_UNIT()
	{
		bRibAttached=D_UNITSYS_NONE;
		Shape=D_UNITSYS_NONE;
		SName=D_UNITSYS_NONE;
		Dist=D_UNITSYS_BASE_LENGTH;
		Size=D_UNITSYS_BASE_LENGTH;
		PeriIn=D_UNITSYS_BASE_LENGTH;
		PeriOut=D_UNITSYS_BASE_LENGTH;
	}
	int bRibAttached;
	int Shape;
	int SName;
	int Dist;
	int Size;
	int PeriIn;
	int PeriOut;
	T_SECT_STIFFNESS_UNIT Stiffness;
};

struct T_THIK_STIFFENED_VALUE
{
	CString Shape;
	double ThickIn;
	double ThickOut;
	double Hu;
	double Hl;
	void Initialize()
	{
		ThickIn = 0.0;
		ThickOut = 0.0;
		Hu = 0.0;
		Hl = 0.0;
	}
};

struct T_THIK_STIFFENED_VALUE_CH
{
	char Shape[4];
	double ThickIn;
	double ThickOut;
	double Hu;
	double Hl;
	void Initialize()
	{
		memset(Shape, ' ', sizeof(Shape));
		ThickIn = 0.0;
		ThickOut = 0.0;
		Hu = 0.0;
		Hl = 0.0;
	}
	void ConvertToString(T_THIK_STIFFENED_VALUE& rData)
	{
		ConvertCharStr(Shape, rData.Shape, sizeof(Shape));
		rData.ThickIn = ThickIn;
		rData.ThickOut = ThickOut;
		rData.Hu = Hu;
		rData.Hl = Hl;
	}
	void ConvertToChar(T_THIK_STIFFENED_VALUE& rData)
	{
		ConvertStrChar(rData.Shape, Shape, sizeof(Shape));
		ThickIn = rData.ThickIn;
		ThickOut = rData.ThickOut;
		Hu = rData.Hu;
		Hl = rData.Hl;
	}
};

struct T_THIK_STIFFENED_VALUE_UNIT
{
	T_THIK_STIFFENED_VALUE_UNIT()
	{
		Shape = D_UNITSYS_NONE;
		ThickIn = D_UNITSYS_BASE_LENGTH;
		ThickOut = D_UNITSYS_BASE_LENGTH;
		Hu = D_UNITSYS_BASE_LENGTH;
		Hl = D_UNITSYS_BASE_LENGTH;
	}
	int Shape;
	int ThickIn;
	int ThickOut;
	int Hu;
	int Hl;
};

struct T_THIK_STIFFENED
{
	int nType;  // Value=0, User=1, DB=2
	CString DBName;
	double dPlateThik;
	double dWeightThik;
	int nRibPosition; // Lower=1, Upper=2
	T_THIK_STIFFENED_VALUE yzValue;
	T_THIK_STIFFENED_VALUE xzValue;
	T_THIK_STIFFENED_SECT  yzSect;
	T_THIK_STIFFENED_SECT  xzSect;
	void Initialize()
	{
		nType = 1;
		DBName.Empty();
		dPlateThik = 0.0;
		dWeightThik = 0.0;
		nRibPosition = 1;
		yzValue.Initialize();
		xzValue.Initialize();
		yzSect.Initialize();
		xzSect.Initialize();
	}
};

struct T_THIK_STIFFENED_CH_501
{
	int nType;
	char DBName[8];
	double dPlateThik;
	double dWeightThik;
	int nRibPosition;
	T_THIK_STIFFENED_VALUE_CH yzValue;
	T_THIK_STIFFENED_VALUE_CH xzValue;
	T_THIK_STIFFENED_SECT_CH_501 yzSect;
	T_THIK_STIFFENED_SECT_CH_501 xzSect;
};
struct T_THIK_STIFFENED_CH_600
{
	int nType;
	char DBName[8];
	double dPlateThik;
	double dWeightThik;
	int nRibPosition;
	T_THIK_STIFFENED_VALUE_CH yzValue;
	T_THIK_STIFFENED_VALUE_CH xzValue;
	T_THIK_STIFFENED_SECT_CH_600 yzSect;
	T_THIK_STIFFENED_SECT_CH_600 xzSect;
};
struct T_THIK_STIFFENED_CH_815
{
	int nType;
	char DBName[8];
	double dPlateThik;
	double dWeightThik;
	int nRibPosition;
	T_THIK_STIFFENED_VALUE_CH yzValue;
	T_THIK_STIFFENED_VALUE_CH xzValue;
	T_THIK_STIFFENED_SECT_CH_815 yzSect;
	T_THIK_STIFFENED_SECT_CH_815 xzSect;
};
struct T_THIK_STIFFENED_CH
{
	int nType;
	char DBName[8];
	double dPlateThik;
	double dWeightThik;
	int nRibPosition;
	T_THIK_STIFFENED_VALUE_CH yzValue;
	T_THIK_STIFFENED_VALUE_CH xzValue;
	T_THIK_STIFFENED_SECT_CH yzSect;
	T_THIK_STIFFENED_SECT_CH xzSect;
	void Initialize()
	{
		nType = 0;
		memset(DBName, ' ', sizeof(DBName));
		dPlateThik = 0.0;
		dWeightThik = 0.0;
		nRibPosition = 1;
		yzValue.Initialize();
		xzValue.Initialize();
		yzSect.Initialize();
		xzSect.Initialize();
	}
	void ConvertToString(T_THIK_STIFFENED& rData)
	{
		rData.nType = nType;
		ConvertCharStr(DBName, rData.DBName, sizeof(DBName));
		rData.dPlateThik = dPlateThik;
		rData.dWeightThik = dWeightThik;
		rData.nRibPosition = nRibPosition;
		yzValue.ConvertToString(rData.yzValue);
		xzValue.ConvertToString(rData.xzValue);
		yzSect.ConvertToString(rData.yzSect);
		xzSect.ConvertToString(rData.xzSect);
	}
	void ConvertToChar(T_THIK_STIFFENED& rData)
	{
		nType = rData.nType;
		ConvertStrChar(rData.DBName, DBName, sizeof(DBName));
		dPlateThik = rData.dPlateThik;
		dWeightThik = rData.dWeightThik;
		nRibPosition = rData.nRibPosition;
		yzValue.ConvertToChar(rData.yzValue);
		xzValue.ConvertToChar(rData.xzValue);
		yzSect.ConvertToChar(rData.yzSect);
		xzSect.ConvertToChar(rData.xzSect);
	}
	void Convert501(T_THIK_STIFFENED_CH_501& rData)
	{
		nType = rData.nType;
		memcpy(DBName, rData.DBName, sizeof(DBName));
		dPlateThik = rData.dPlateThik;
		dWeightThik = rData.dWeightThik;
		nRibPosition = rData.nRibPosition;
		yzValue = rData.yzValue;
		xzValue = rData.xzValue;
		yzSect.Convert501(rData.yzSect);
		xzSect.Convert501(rData.xzSect);
	}
	void Convert600(T_THIK_STIFFENED_CH_600& rData)
	{
		nType = rData.nType;
		memcpy(DBName, rData.DBName, sizeof(DBName));
		dPlateThik = rData.dPlateThik;
		dWeightThik = rData.dWeightThik;
		nRibPosition = rData.nRibPosition;
		yzValue = rData.yzValue;
		xzValue = rData.xzValue;
		yzSect.Convert600(rData.yzSect);
		xzSect.Convert600(rData.xzSect);
	}
	void Convert815(T_THIK_STIFFENED_CH_815& rData)
	{
		nType = rData.nType;
		memcpy(DBName, rData.DBName, sizeof(DBName));
		dPlateThik = rData.dPlateThik;
		dWeightThik = rData.dWeightThik;
		nRibPosition = rData.nRibPosition;
		yzValue = rData.yzValue;
		xzValue = rData.xzValue;
		yzSect.Convert815(rData.yzSect);
		xzSect.Convert815(rData.xzSect);
	}
};

struct T_THIK_STIFFENED_UNIT
{
	T_THIK_STIFFENED_UNIT()
	{
		nType = D_UNITSYS_NONE;
		DBName = D_UNITSYS_NONE;
		dPlateThik = D_UNITSYS_BASE_LENGTH;
		dWeightThik = D_UNITSYS_BASE_LENGTH;
		nRibPosition = D_UNITSYS_NONE;
	};
	int nType;
	int DBName;
	int dPlateThik;
	int dWeightThik;
	int nRibPosition;
	T_THIK_STIFFENED_VALUE_UNIT Value;
	T_THIK_STIFFENED_SECT_UNIT  Sect;
};

//SteelWall
struct T_THIK_STLWALL
{
	double ThickCon;    // L, nThickType = 0 
	double ThickSteel;  // L, nThickType = 0 

	void Initialize()
	{
		ThickCon = 0.0;
		ThickSteel = 0.0;
	}
};

struct T_THIK_STLWALL_UNIT
{
	T_THIK_STLWALL_UNIT()
	{
		ThickCon = D_UNITSYS_BASE_LENGTH;
		ThickSteel = D_UNITSYS_BASE_LENGTH;
	}
	int ThickCon;
	int ThickSteel;
};

struct T_THIK_D
{
	int nType;        // 0=Value, 1=Stiffened (T_THKS_K == T_THIK_K)
	T_THIK_VALUE Value;
	T_THIK_STIFFENED Stiffened;
	T_THIK_STLWALL StlWall;
	void Initialize()
	{
		nType = 0;
		Value.Initialize();
		Stiffened.Initialize();
		StlWall.Initialize();
	}
	void Convert413(T_THIK_D_413& rData)
	{
		nType = 0;
		Value.Initialize();
		Value.ThickIn = rData.Thickness;
		Stiffened.Initialize();
		StlWall.Initialize();
	}
	void Convert431(T_THIK_D_431& rData)
	{
		nType=0;
		Value.Initialize();
		Value.nThickType = rData.nThickType;
		Value.ThickIn = rData.ThickIn;
		Value.ThickOut = rData.ThickOut;
		Stiffened.Initialize();
		StlWall.Initialize();
	}
};

struct T_THIK_D_CH_501
{
	int nType;
	T_THIK_VALUE_730 Value;
	T_THIK_STIFFENED_CH_501 Stiffened;
};
struct T_THIK_D_CH_600
{
	int nType;
	T_THIK_VALUE_730 Value;
	T_THIK_STIFFENED_CH_600 Stiffened;
};
struct T_THIK_D_CH_730
{
	int nType;
	T_THIK_VALUE_730 Value;
	T_THIK_STIFFENED_CH_815 Stiffened;
};
struct T_THIK_D_CH_815
{
	int nType;
	T_THIK_VALUE_883 Value;
	T_THIK_STIFFENED_CH_815 Stiffened;
};
struct T_THIK_D_CH_820
{
	int nType;
	T_THIK_VALUE_883 Value; 
	T_THIK_STIFFENED_CH Stiffened;
};
struct T_THIK_D_CH_883
{
	int nType;
	T_THIK_VALUE_883 Value;
	T_THIK_STIFFENED_CH Stiffened;
	T_THIK_STLWALL StlWall;
};
struct T_THIK_D_CH
{
	int nType;
	T_THIK_VALUE_CH Value;
	T_THIK_STIFFENED_CH Stiffened;
	T_THIK_STLWALL StlWall;
	void ConvertToString(T_THIK_D& rData)
	{
		rData.nType = nType;
		Value.ConvertToString(rData.Value);
		Stiffened.ConvertToString(rData.Stiffened);
		rData.StlWall = StlWall;
	}
	void ConvertToChar(T_THIK_D& rData)
	{
		nType = rData.nType;
		Value.ConvertToChar(rData.Value);
		Stiffened.ConvertToChar(rData.Stiffened);
		StlWall = rData.StlWall;
	}
	void Convert501(T_THIK_D_CH_501& rData)
	{
		nType = rData.nType;
		Value.Convert730(rData.Value);
		Stiffened.Convert501(rData.Stiffened);
	}
	void Convert600(T_THIK_D_CH_600& rData)
	{
		nType = rData.nType;
		Value.Convert730(rData.Value);
		Stiffened.Convert600(rData.Stiffened);
	}
	void Convert730(T_THIK_D_CH_730 &rData)
	{
		nType = rData.nType;
		Value.Convert730(rData.Value);
		Stiffened.Convert815(rData.Stiffened);
	}
	void Convert815(T_THIK_D_CH_815 &rData)
	{
		nType = rData.nType;
		Value.Convert883(rData.Value);
		Stiffened.Convert815(rData.Stiffened);
	}
	void Convert820(T_THIK_D_CH_820 &rData)
	{
		nType = rData.nType;
		Value.Convert883(rData.Value);
		Stiffened = rData.Stiffened;
	}
	void Convert883(T_THIK_D_CH_883 &rData)
	{
		nType = rData.nType;
		Value.Convert883(rData.Value);
		Stiffened = rData.Stiffened;
		StlWall = rData.StlWall;
	}
};

struct T_THIK_UDRD_D
{
	T_THIK_K key;
	T_THIK_D data;
};
struct T_THIK_UDRD_D_CH
{
	T_THIK_K key;
	T_THIK_D_CH data;
};
struct T_THIK_UDRD_D_CH_501
{
	T_THIK_K key;
	T_THIK_D_CH_501 data;
};
struct T_THIK_UDRD_D_CH_600
{
	T_THIK_K key;
	T_THIK_D_CH_600 data;
};
struct T_THIK_UDRD_D_CH_730
{
	T_THIK_K key;
	T_THIK_D_CH_730 data;
};
struct T_THIK_UDRD_D_CH_815
{
	T_THIK_K key;
	T_THIK_D_CH_815 data;
};
struct T_THIK_UDRD_D_CH_820
{
	T_THIK_K key;
	T_THIK_D_CH_820 data;
};
struct T_THIK_UDRD_D_CH_883
{
	T_THIK_K key;
	T_THIK_D_CH_883 data;
};
struct T_THIK_UNIT
{
	T_THIK_UNIT()
	{
		nType = D_UNITSYS_NONE;
	}
	int nType;
	T_THIK_VALUE_UNIT Value;
	T_THIK_STIFFENED_UNIT Stiffened;
	T_THIK_STLWALL_UNIT StlWall;
};
#define HASHSIZETHIK 101
#define D_THIK_MINNUM       1   // 입력가능한 최소 두께번호
#define D_THIK_MAXNUM    9999   // 입력가능한 최대 두께번호

#pragma pack(pop)

#endif  // __DB_ST_DT_THIK_H__
