// DB_CDataClass.h: interface for the DB_CDataClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DB_CDATACLASS_H__)
#define __DB_CDATACLASS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning ( disable : 4267 )

#include "DB_Type.h"

///////////////////////////////////////////////////////////////////
// Macro Definitions
//-----------------------------------------------------------------
inline int GetCStringMemorySize(const CString& str)
{
	std::string arrStrTmp = CW2A(str);
	int iMemorySize = (int)arrStrTmp.length();

	return iMemorySize;
}
//-----------------------------------------------------------------
// Macro for Variable when Reading from File
#define D_VT_INT__R(var) rData.var = var
#define D_VT_UINT_R(var) rData.var = var
#define D_VT_BOOL_R(var) rData.var = var
#define D_VT_FLT__R(var) rData.var = var
#define D_VT_DBL__R(var) rData.var = var
#define D_VT_KEY__R(var) rData.var = var
#define D_VT_ARR__R(var) memcpy(rData.var, var, sizeof(var))
#define D_VT_STR__R(var) var.nDummy
#define D_VT_FST__R(var) rData.var = var
#define D_VT_VST__R(var) var.SetR(rData.var, paVD)
#define D_VT_FARR_R(var, size) rData.var.SetSize(var.nCount); \
															 paVD->Add(&var)
#define D_VT_ASTR_R(var, size, ix) for (ix=0; ix<size; ix++) { D_VT_STR__R(var[ix]); }

//-----------------------------------------------------------------
// Macro for Variable when Write to File
#define D_VT_INT__W(var) var = rData.var
#define D_VT_UINT_W(var) var = rData.var
#define D_VT_BOOL_W(var) var = rData.var
#define D_VT_FLT__W(var) var = rData.var
#define D_VT_DBL__W(var) var = rData.var
#define D_VT_KEY__W(var) var = rData.var
#define D_VT_ARR__W(var) memcpy(var, rData.var, sizeof(var))
#define D_VT_STR__W(var) var.Set(sizeof(char), GetCStringMemorySize(rData.var), (void*)((LPCTSTR)rData.var)); paVD->Add(&var)
#define D_VT_FST__W(var) var = rData.var
#define D_VT_VST__W(var) var.SetW(rData.var, paVD)
#define D_VT_FARR_W(var, size) var.Set(size, rData.var.GetSize(), (void*)rData.var.GetData()); paVD->Add(&var)
#define D_VT_ASTR_W(var, size, ix) for (ix=0; ix<size; ix++) { D_VT_STR__W(var[ix]); }

//-----------------------------------------------------------------
// Macro for Variable when Defining variable
#define D_VT_INT__D(var)              int var
#define D_VT_UINT_D(var)              UINT var
#define D_VT_BOOL_D(var)              BOOL var
#define D_VT_FLT__D(var)              float var
#define D_VT_DBL__D(var)              double var
#define D_VT_KEY__D(var)              T_KEY var
#define D_VT_TARR_D(type, var, size)  type var[size]
#define D_VT_STR__D(var)              T_VD_DATA var
#define D_VT_TFST_D(type, var)        type var
#define D_VT_TVST_D(type, var)        type##_RW var
#define D_VT_FARR_D(var, size)        T_VD_DATA var
#define D_VT_ASTR_D(var, size, ix)    T_VD_DATA var[size]
#define D_VT_VARR_D(type, var)        T_VD_DATA var
#define D_VT_DRW__D(type, var)        type##_RW var

#define D_VT_TARR_R(type, var, size)  D_VT_ARR__R(var)
#define D_VT_TFST_R(type, var)        D_VT_FST__R(var)
#define D_VT_TVST_R(type, var)        D_VT_VST__R(var)

#define D_VT_TARR_W(type, var, size)  D_VT_ARR__W(var)
#define D_VT_TFST_W(type, var)        D_VT_FST__W(var)
#define D_VT_TVST_W(type, var)        D_VT_VST__W(var)
														 

//-----------------------------------------------------------------
// Macro for Data-Stream Conversion
// S2D
#define D_VT_FXD__S2D(var, bytes) \
	if (!DB_CStream2Data::FXD(&rData.var, bytes, pStream, nStart, nEnd)) return FALSE
#define D_VT_INT__S2D(var) D_VT_FXD__S2D(var, sizeof(int))
#define D_VT_KEY__S2D(var) D_VT_FXD__S2D(var, sizeof(T_KEY))
#define D_VT_UINT_S2D(var) D_VT_FXD__S2D(var, sizeof(UINT))
#define D_VT_BOOL_S2D(var) D_VT_FXD__S2D(var, sizeof(BOOL))
#define D_VT_DBL__S2D(var) D_VT_FXD__S2D(var, sizeof(double))
#define D_VT_TARR_S2D(type, var, size) \
	if (!DB_CStream2Data::FXD(rData.var, sizeof(type)*size, pStream, nStart, nEnd)) return FALSE
#define D_VT_STR__S2D(var) \
	if (!DB_CStream2Data::STR(rData.var, pStream, nStart, nEnd)) return FALSE
#define D_VT_ASTR_S2D(var, size, ix) \
	for (ix=0; ix<size; ix++) { D_VT_STR__S2D(var[ix]); }
#define D_VT_FARR_S2D(var, size) \
	if (!DB_CStream2Data::VDATA(var, pStream, nStart, nEnd)) return FALSE; \
	if (var.nCount > 0) \
	{ \
		rData.var.SetSize(var.nCount); \
		if (!DB_CStream2Data::FXD(rData.var.GetData(), size*var.nCount, pStream, nStart, nEnd)) return FALSE; \
	}
#define D_VT_VARR_S2D(type, var) \
	T_VD_DATA var; \
	if (!DB_CStream2Data::VDATA(var, pStream, nStart, nEnd)) return FALSE; \
	if (var.nCount > 0) \
	{ \
		type##_RW RW_##var; \
		rData.var.SetSize(var.nCount); \
		for (UINT i = 0; i < var.nCount; i++) \
			if (!RW_##var.S2D(rData.var[i], pStream, nStart, nEnd)) return FALSE; \
	}
#define D_VT_TFST_S2D(type, var) D_VT_FXD__S2D(var, sizeof(type))
#define D_VT_TVST_S2D(type, var) \
	if (!var.S2D(rData.var, pStream, nStart, nEnd)) return FALSE
#define D_VT_DRW__S2D(type, var) \
	if (!var.S2D(rData.var, pStream, nStart, nEnd)) return FALSE

// D2S
#define D_VT_FXD__D2S(var, bytes) \
	if (!DB_CData2Stream::FXD(&rData.var, bytes, pStream, nStart, nEnd)) return FALSE
#define D_VT_INT__D2S(var) D_VT_FXD__D2S(var, sizeof(int))
#define D_VT_KEY__D2S(var) D_VT_FXD__D2S(var, sizeof(T_KEY))
#define D_VT_UINT_D2S(var) D_VT_FXD__D2S(var, sizeof(UINT))
#define D_VT_BOOL_D2S(var) D_VT_FXD__D2S(var, sizeof(BOOL))
#define D_VT_DBL__D2S(var) D_VT_FXD__D2S(var, sizeof(double))
#define D_VT_TARR_D2S(type, var, size) \
	if (!DB_CData2Stream::FXD(rData.var, sizeof(type)*size, pStream, nStart, nEnd)) return FALSE
#define D_VT_STR__D2S(var) \
	if (!DB_CData2Stream::STR(rData.var, pStream, nStart, nEnd)) return FALSE
#define D_VT_ASTR_D2S(var, size, ix) \
	for (ix=0; ix<size; ix++) { D_VT_STR__D2S(var[ix]); }
#define D_VT_FARR_D2S(var, size) \
	var.Set(size, rData.var.GetSize(), 0); \
	if (!DB_CData2Stream::VDATA(var, pStream, nStart, nEnd)) return FALSE; \
	if (var.nCount > 0) \
	{ \
		if (!DB_CData2Stream::FXD(rData.var.GetData(), size*var.nCount, pStream, nStart, nEnd)) return FALSE; \
	}
#define D_VT_VARR_D2S(type, var) \
	T_VD_DATA var; \
	var.Set(sizeof(T_VD_DATA), rData.var.GetSize(), 0); \
	if (!DB_CData2Stream::VDATA(var, pStream, nStart, nEnd)) return FALSE; \
	if (var.nCount > 0) \
	{ \
		type##_RW RW_##var; \
		for (UINT i = 0; i < var.nCount; i++) \
			if (!RW_##var.D2S(rData.var[i], pStream, nStart, nEnd)) return FALSE; \
	}
#define D_VT_TFST_D2S(type, var) D_VT_FXD__D2S(var, sizeof(type))
#define D_VT_TVST_D2S(type, var) \
	if (!var.D2S(rData.var, pStream, nStart, nEnd)) return FALSE
#define D_VT_DRW__D2S(type, var) \
	if (!var.D2S(rData.var, pStream, nStart, nEnd)) return FALSE

#define D_S2D_ARG_LIST unsigned char* pStream, int& nStart, const int nEnd
#define D_D2S_ARG_LIST unsigned char* pStream, int& nStart, const int nEnd


struct T_VD_DATA  // 고정길이 데이타를 가진 Array(FARR or String)
{
	T_VD_DATA& Set(UINT nSize, UINT nCount, void *pAddr) 
	{
		this->nSize = nSize;
		this->nCount = nCount;
		this->rSize = ~nSize;
		this->rCount = ~nCount;
		this->nDummy; //this->pAddr = pAddr;
		return *this;
	}
	BOOL IsValid() { return (nSize == ~rSize && nCount == ~rCount); }
	UINT nSize;
	UINT nCount;
	UINT rSize;   // 에러 검증용
	UINT rCount;  // 에러 검증용
	int nDummy;   //void *pAddr; // 64bit 에서는 Pointer가 8Byte로 바뀌기 때문에 구조체 크기에서는 size만 맞춰줌.
};
typedef CArray<T_VD_DATA*, T_VD_DATA*> T_VD_DATA_PLIST;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ DB_CData2Stream
{
public:
	static BOOL STR(CString& rcsVal, unsigned char* pStream, int& nStart, const int nEnd);
	static BOOL FXD(void* pAddr, int nSize, unsigned char* pStream, int& nStart, const int nEnd);
	static BOOL VDATA(T_VD_DATA& rvdVal, unsigned char* pStream, int& nStart, const int nEnd);
};

class __MY_EXT_CLASS__ DB_CStream2Data
{
public:
	static BOOL STR(CString& rcsVal, unsigned char* pStream, int& nStart, const int nEnd);
	static BOOL FXD(void* pAddr, int nSize, unsigned char* pStream, int& nStart, const int nEnd);
	static BOOL VDATA(T_VD_DATA& rvdVal, unsigned char* pStream, int& nStart, const int nEnd);
};

///////////////////////////////////////////////////////////////////
// DB_TDataRW
class __MY_EXT_CLASS__ DB_TDataRW
{
public:
	DB_TDataRW() {}
	virtual ~DB_TDataRW() {}

public:
	virtual void Initialize()=0;
	virtual BOOL IsFixedSize()=0;
	virtual UINT GetSize()=0;
	virtual BOOL Stream2Data(const void* pStream, int nMaxSize)=0;
	virtual BOOL Data2Stream(void* pStream, int nMaxSize)=0;

protected:
	BOOL Stream2DataFixed(const void* pStream, void* pKey, int nKeySize, void* pData, int nDataSize, int nMaxSize);
	BOOL Data2StreamFixed(void* pStream, const void* pKey, int nKeySize, const void* pData, int nDataSize, int nMaxSize);
	BOOL Stream2VD(const void* pStream, T_VD_DATA_PLIST* paVD, int nMaxSize);
	BOOL VD2Stream(void *pStream, T_VD_DATA_PLIST* paVD, int nMaxSize);
};

class __MY_EXT_CLASS__ DB_CDataRW
{
public:
	DB_CDataRW() {}
	virtual ~DB_CDataRW() {}
	virtual void Init()=0;
	virtual CString GetHdrName()=0;
	virtual CString GetDataName()=0;
	virtual UINT GetCount()=0;
	virtual POSITION GetStartData()=0;
	virtual void GetNextData(POSITION& rPos, DB_TDataRW* pDataRW)=0;
	virtual BOOL AddToDB(UINT nVer, DB_TDataRW* pDataRW)=0;
	virtual DB_TDataRW* CreateData(UINT nVer)=0;
	virtual UINT GetLatestVersion()=0;
};

class DB_TStream
{
public:
	virtual BOOL Read(void* pData, UINT nSize)=0;
	virtual BOOL Write(void* pData, UINT nSize)=0;
};

#define D_DB_S2D_FIXED(key, data) \
	Stream2DataFixed(pStream, &key, sizeof(key), &data, sizeof(data), nMaxSize)
#define D_DB_D2S_FIXED(key, data) \
	Data2StreamFixed(pStream, &key, sizeof(key), &data, sizeof(data), nMaxSize)

#include "HeaderPost.h"

#endif // !defined(__DB_CDATACLASS_H__)
