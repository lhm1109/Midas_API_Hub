// Time Dependent Material (Creep/Shrinkage/Relaxation) Function

#ifndef __DB_ST_DT_TDMF_H__
#define __DB_ST_DT_TDMF_H__

#define MIDAS_SDK

#pragma pack(push, 1)

struct T_TDMF_BASE
{
	double    dDay;
	double    dValue;

	T_TDMF_BASE()
	{
		Initialize();
	}

	void Initialize()
	{
		dDay    = 0.0;
		dValue  = 0.0;
	}
};

#define T_TDMF_DT                   CArray<T_TDMF_BASE, T_TDMF_BASE&>
#define T_TDMF_K                    unsigned int
#define HASHSIZETDMF                11

enum EN_TDMF_FUNC
{
	TDMF_FUNC_NONE                    = 0,  // None
	TDMF_FUNC_CREEP                   ,	    // Creep
	TDMF_FUNC_SHRINKAGE               ,	    // Shrinkage Strain
	TDMF_FUNC_RELAXATION              ,	    // Relaxation
};

enum EN_TDMF_CREEP_TYPE
{
	TDMF_CREEP_TYPE_NONE              = 0,  // None
	TDMF_CREEP_TYPE_SPECIFIC_CREEP    ,	    // Specific Creep
	TDMF_CREEP_TYPE_CREEP_FUNCTION    ,	    // Creep Compliance
	TDMF_CREEP_TYPE_CREEP_COEFFICIENT ,	    // Creep Coefficient
};

enum EN_TDMF_RELAX_TIME
{
	TDMF_RELAX_TIME_HOUR              = 0,  // Hour
	TDMF_RELAX_TIME_DAY               ,	    // Day
};

struct T_TDMF_D
{
	CString       MatlFuncName;
	int           nMatlFuncType;    // TDMF_FUNC_XXXX
	int           nCreepType;       // TDMF_CREEP_TYPE_XXXX
	double        dElast;           // nMatlFuncType == 1
	double        ScaleFactor;
	unsigned int  nMatlFuncId;      // 순번
	T_TDMF_DT     arMatlFuncData;
	CString       Description;
	int           nRelaxTime;       // 845추가, TDMF_RELAX_TIME_XXXX
	
	void Initialize()
	{
		MatlFuncName  .Empty();
		nMatlFuncType = TDMF_FUNC_CREEP;
		nCreepType    = TDMF_CREEP_TYPE_SPECIFIC_CREEP;
		dElast        = 0.0;
		ScaleFactor   = 1.0;
		nMatlFuncId   = 0;
		arMatlFuncData.RemoveAll();
		Description   .Empty();
		nRelaxTime    = TDMF_RELAX_TIME_DAY;
	}

	T_TDMF_D()
	{
		Initialize();
	}
	T_TDMF_D(T_TDMF_D& src)
	{
		*this = src;
	}
	T_TDMF_D& operator=(T_TDMF_D& src)
	{
		MatlFuncName  = src.MatlFuncName;
		nMatlFuncType = src.nMatlFuncType;
		nCreepType    = src.nCreepType;
		dElast        = src.dElast;
		ScaleFactor   = src.ScaleFactor;
		nMatlFuncId   = src.nMatlFuncId;
		arMatlFuncData.Copy(src.arMatlFuncData);
		Description   = src.Description;
		nRelaxTime    = src.nRelaxTime;

		return *this;
	}
};

struct T_TDMF_D_CH
{
	char          MatlFuncName[20];
	int           nMatlFuncType;
	int           nCreepType;
	double        dElast;
	double        ScaleFactor;
	unsigned int  nMatlFuncId;
	T_TDMF_DT     arMatlFuncData;
	char          Description[80];
	int           nRelaxTime; 

	void ConvertToString(T_TDMF_D& rData)
	{
		ConvertCharStr(MatlFuncName, rData.MatlFuncName, sizeof(MatlFuncName));

		rData.nMatlFuncType = nMatlFuncType;
		rData.nCreepType    = nCreepType;
		rData.dElast        = dElast;
		rData.ScaleFactor   = ScaleFactor;
		rData.nMatlFuncId   = nMatlFuncId;
		rData.arMatlFuncData.Copy(arMatlFuncData);

		ConvertCharStr(Description, rData.Description, sizeof(Description));

		rData.nRelaxTime    = nRelaxTime;
	}

	void ConvertToChar(T_TDMF_D& rData)
	{
		ConvertStrChar(rData.MatlFuncName, MatlFuncName, sizeof(MatlFuncName));

		nMatlFuncType       = rData.nMatlFuncType;
		nCreepType          = rData.nCreepType;
		dElast              = rData.dElast;
		ScaleFactor         = rData.ScaleFactor;
		nMatlFuncId         = rData.nMatlFuncId;
		arMatlFuncData      .Copy(rData.arMatlFuncData);

		ConvertStrChar(rData.Description, Description, sizeof(Description));

		nRelaxTime          = rData.nRelaxTime;
	}
};

struct T_TDMF_UDRD_D
{
	T_TDMF_K    key;
	T_TDMF_D    data;
};

struct T_TDMF_UDRD_D_CH
{
	T_TDMF_K    key;
	T_TDMF_D_CH data;
};

struct T_TDMF_UDRD_D_CH_RW_842
{
	T_TDMF_K      key;
	char          MatlFuncName[20];
	int           nMatlFuncType;
	int           nCreepType;
	double        dElast;
	double        ScaleFactor;
	unsigned int  nMatlFuncId;
	char          Description[80];
};

struct T_TDMF_UDRD_D_CH_RW
{
	T_TDMF_K      key;
	char          MatlFuncName[20];
	int           nMatlFuncType;
	int           nCreepType;
	double        dElast;
	double        ScaleFactor;
	unsigned int  nMatlFuncId;
	char          Description[80];
	int           nRelaxTime; 

	void GetTdmf(T_TDMF_K& rKey, T_TDMF_D_CH& rData)
	{
		rKey  = key;
		memcpy(rData.MatlFuncName, MatlFuncName, sizeof(MatlFuncName));
		rData.nMatlFuncType = nMatlFuncType;
		rData.nCreepType    = nCreepType;
		rData.dElast        = dElast;
		rData.ScaleFactor   = ScaleFactor;
		rData.nMatlFuncId   = nMatlFuncId;
		memcpy(rData.Description, Description, sizeof(Description));
		rData.nRelaxTime    = nRelaxTime;
	}

	void SetTdmf(const T_TDMF_K Key, const T_TDMF_D_CH& rData)
	{
		key = Key;
		memcpy(MatlFuncName, rData.MatlFuncName, sizeof(MatlFuncName));
		nMatlFuncType       = rData.nMatlFuncType;
		nCreepType          = rData.nCreepType;
		dElast              = rData.dElast;
		ScaleFactor         = rData.ScaleFactor;
		nMatlFuncId         = rData.nMatlFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nRelaxTime          = rData.nRelaxTime;
	}

	void Convert842(T_TDMF_UDRD_D_CH_RW_842& rData)
	{
		key = rData.key;
		memcpy(MatlFuncName, rData.MatlFuncName, sizeof(MatlFuncName));
		nMatlFuncType       = rData.nMatlFuncType;
		nCreepType          = rData.nCreepType;
		dElast              = rData.dElast;
		ScaleFactor         = rData.ScaleFactor;
		nMatlFuncId         = rData.nMatlFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nRelaxTime          = TDMF_RELAX_TIME_DAY;
	}
};

struct T_TDMF_UNIT
{
	int     dDay;
	int     dHour;
	int     dValue[2];
	int     dCreep[3];
	int     dElast;
	int     dRelax;

	T_TDMF_UNIT()
	{
		dDay      = D_UNITSYS_BASE_DAY;
		dHour     = D_UNITSYS_BASE_HOUR;
		dValue[0] = D_UNITSYS_NONE;         // dCreep 사용
		dValue[1] = D_UNITSYS_NONE;         // shrinkage strain
		dCreep[0] = D_UNITSYS_BASE_RSTRESS; // specific creep
		dCreep[1] = D_UNITSYS_BASE_RSTRESS; // creep compliance
		dCreep[2] = D_UNITSYS_NONE;         // creep coefficient
		dElast    = D_UNITSYS_BASE_ELAST;
		dRelax    = D_UNITSYS_BASE_PERCENT;
	}
};

#pragma pack(pop)

#endif