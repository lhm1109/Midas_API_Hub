#if !defined(__DgnSeisBrgInfoFile_H__)
#define __DgnSeisBrgInfoFile_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DB_CDataClass.h"

struct SBRI_RW
{
#define ATTR(DRW) \
	D_VT_STR__##DRW(strBrgName);\
	D_VT_STR__##DRW(strLocation);\
	D_VT_STR__##DRW(strManager);\
	D_VT_STR__##DRW(strLineNum);\
	D_VT_STR__##DRW(strOrgLoc);\
	D_VT_INT__##DRW(nBuildYear);\
	D_VT_INT__##DRW(nSeismicClass);\
	D_VT_INT__##DRW(nEarthquake);\
	D_VT_INT__##DRW(nMvCode);\
	D_VT_INT__##DRW(nMvhlStd);\
	D_VT_STR__##DRW(strMvhlType);\
	D_VT_STR__##DRW(strMvhlName);\
	D_VT_DBL__##DRW(dTotalLen);\
	D_VT_FARR_##DRW(aSpan, sizeof(double));\
	D_VT_DBL__##DRW(dBrgWidth);\
	D_VT_INT__##DRW(nUpper);\
	D_VT_STR__##DRW(strUpper);\
	D_VT_INT__##DRW(nAbutmentType);\
	D_VT_STR__##DRW(strAbutmentType);\
	D_VT_INT__##DRW(nAbutmentBase);\
	D_VT_STR__##DRW(strAbutmentBase);\
	D_VT_INT__##DRW(nPierType);\
	D_VT_STR__##DRW(strPierType);\
	D_VT_INT__##DRW(nPierBase);\
	D_VT_STR__##DRW(strPierBase);\
	D_VT_INT__##DRW(nSupportMatl);\
	D_VT_STR__##DRW(strSupportMatl);\
	D_VT_DBL__##DRW(dSupportCapaAbut);\
	D_VT_DBL__##DRW(dSupportCapaPier);\
	D_VT_INT__##DRW(nPierSect);\
	D_VT_STR__##DRW(strPierSect);\
	D_VT_INT__##DRW(SpfcK);
	ATTR(D)
	BOOL S2D(T_SBRI_D& rData, D_S2D_ARG_LIST) { ATTR(S2D); return TRUE; }
	BOOL D2S(T_SBRI_D& rData, D_D2S_ARG_LIST) { ATTR(D2S); return TRUE; }
#undef ATTR
};

class TDataRW_SBRI : public DB_TDataRW
{
public:
	TDataRW_SBRI() {}
	virtual ~TDataRW_SBRI() {}

public:
	virtual void Initialize() { m_data.Initialize(); }
	virtual BOOL IsFixedSize() { return FALSE; }
	virtual UINT GetSize();
	virtual BOOL Stream2Data(const void* pStream, int nMaxSize);
	virtual BOOL Data2Stream(void* pStream, int nMaxSize);

public:
	T_SBRI_D m_data;
};

class BrgInfoRW : public DB_CDataRW
{
public:
	BrgInfoRW() { m_bAdded = FALSE; }
	virtual void Init() {}
	virtual CString GetHdrName() { return _T("SBRI_M"); }  // 8자 이내
	virtual CString GetDataName() { return _T("Bridge Information Data"); }
	virtual UINT GetCount() { return 1; } // 변경 없음
	virtual POSITION GetStartData() { return POSITION(-1); } // 변경 없음
	virtual void GetNextData(POSITION& rPos, DB_TDataRW* pDataRW);
	virtual BOOL AddToDB(UINT nVer, DB_TDataRW* pDataRW);
	virtual DB_TDataRW* CreateData(UINT nVer);
	virtual UINT GetLatestVersion() { return D_PGM_VER(8,8,3,0); }
public:
	T_SBRI_D m_data;
	BOOL m_bAdded;
};

class CDgnSeisBrgInfoFile
{
public:
	CDgnSeisBrgInfoFile();
	virtual ~CDgnSeisBrgInfoFile();

	BOOL SaveToFile(T_SBRI_D& rData);
	BOOL ReadFromFile(T_SBRI_D& rData);
protected: 
};

#endif // !defined(__DgnSeisBrgInfoFile_H__)


