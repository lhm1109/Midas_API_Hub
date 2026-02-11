#ifndef __DB_ST_DT_MHTP_H__
#define __DB_ST_DT_MHTP_H__

#define MIDAS_SDK

#pragma pack(push, 1)

///////////////////////////////////////////////////////////////////////////////////////////
//Material high temperature properties
struct T_MHTP_BASE
{
	double nC0;		//Temperature
	double dComp;
	double dElast;
	void Initialize()
	{
		nC0 = 0.0;
		dComp = 0.0;
		dElast = 0.0;
	}
};
#define T_MHTP_DT CArray<T_MHTP_BASE, T_MHTP_BASE&>
#define T_MHTP_K unsigned int
struct T_MHTP_D
{
	CString   strMatlName;
	int       nMatlType;      // Code:1, User:2
	int       nStlType;		//Structure steel:1, Fire:2
	double    dScaleFactor;
	//CString   strCode;        // nMatlType = Code(0)  // see CDBLib::GetHmtpCodeID
	T_MHTP_DT aFuncData;
	unsigned int nSeq;        // 순번

	void Initialize()
	{
		strMatlName.Empty();
		nMatlType = 1;
		nStlType = 1;
		dScaleFactor = 1.0;
		//strCode.Empty();
		aFuncData.RemoveAll();
		nSeq = 0;
	}
	T_MHTP_D()
	{
	}
	T_MHTP_D(T_MHTP_D& src)
	{
		*this = src;
	}
	T_MHTP_D& operator=(T_MHTP_D& src)
	{
		strMatlName=src.strMatlName;
		nMatlType=src.nMatlType;
		nStlType=src.nStlType;
		dScaleFactor=src.dScaleFactor;
		aFuncData.Copy(src.aFuncData);
		nSeq=src.nSeq;
		return *this;
	}
};

struct T_MHTP_D_CH
{
	char      strMatlName[20];
	int       nMatlType;      // Code:1, User:2
	int       nStlType;		//Structure steel:1, Fire:2
	double    dScaleFactor;
	//CString   strCode;        // nMatlType = Code(0)  // see CDBLib::GetHmtpCodeID
	T_MHTP_DT aFuncData;
	unsigned int nSeq;        // 순번

	void ConvertToString(T_MHTP_D& rData)
	{
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.nMatlType=nMatlType;
		rData.dScaleFactor=dScaleFactor;
		rData.nStlType=nStlType;
		rData.aFuncData.Copy(aFuncData);
		rData.nSeq=nSeq;
	}
	void ConvertToChar(T_MHTP_D& rData)
	{
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		nStlType=rData.nStlType;
		aFuncData.Copy(rData.aFuncData);
		nSeq=rData.nSeq;
	}
};

struct T_MHTP_UDRD_D
{
	T_MHTP_K key;
	T_MHTP_D data;
};
struct T_MHTP_UDRD_D_CH
{
	T_MHTP_K key;
	T_MHTP_D data;
};

struct T_MHTP_UDRD_D_CH_RW
{
	T_MHTP_K    key;
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	int         nFuncDataCount;
	int         nStlType;
	unsigned int nSeq;
	void GetMhtp(T_MHTP_K& rKey, T_MHTP_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strMatlName, strMatlName, sizeof(rData.strMatlName));
		rData.nMatlType=nMatlType;
		rData.dScaleFactor=dScaleFactor;
		rData.nStlType=nStlType;
		rData.nSeq=nSeq;
	}
	void SetMhtp(const T_MHTP_K Key, const T_MHTP_D_CH& rData)
	{
		key=Key;
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		nFuncDataCount=(int)rData.aFuncData.GetSize();
		nStlType=rData.nStlType;
		nSeq=rData.nSeq;
	}
};
struct T_MHTP_UNIT
{
	T_MHTP_UNIT()
	{
		dC0=D_UNITSYS_TEMPER_INDEX_C;
		dCompRto = D_UNITSYS_NONE;
		dElastRto = D_UNITSYS_NONE;
	}
	int dC0;
	double dCompRto;
	double dElastRto;
};
#define HASHSIZEMHTP 11
#define D_MHTP_MINNUM       1   // 
#define D_MHTP_MAXNUM    9999   // 

#define T_MHAT_K T_MATL_K
struct T_MHAT_D
{
	T_MHTP_K HighMatlTypeKey;
	void Initialize()
	{
		HighMatlTypeKey=0;
	}
};
struct T_MHAT_UDRD_D
{
	T_MHAT_K key;
	T_MHAT_D data;
};
struct T_MHAT_UNIT
{
	T_MHAT_UNIT()
	{
		HighMatlTypeKey=D_UNITSYS_NONE;
	}
	int HighMatlTypeKey;
};
#define HASHSIZEMHAT HASHSIZEMATL


#pragma pack(pop)

#endif  // __DB_ST_DT_MHTP_H__
