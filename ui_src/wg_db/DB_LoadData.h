// DB_LoadData.h: interface for the CDB_LoadData class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_LOAD_DATA_H____INCLUDED_)
#define AFX_DB_LOAD_DATA_H____INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DB_LoadData_Struct.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDB_LoadData  
{
public:
	CDB_LoadData();
	virtual ~CDB_LoadData();

public:
	static void GetZoneListForWindLoad(CArray<CString, CString&>& aString);
	static void GetZoneListForSnowLoad(CArray<CString, CString&>& aString);
	static void GetCityListForWindLoad(CArray<CString, CString&>& aString, CString csZone);
	static void GetCityListForSnowLoad(CArray<CString, CString&>& aString, CString csZone);

	static void GetImportanceListForWindLoad(CArray<CString, CString&>& aString);
	static void GetImportanceListForSnowLoad(CArray<CString, CString&>& aString);
	static void GetImportanceListForSnowLoadUS(CArray<CString, CString&>& aString);

	static void GetBuildingTypeForWindLoadKR(CArray<CString, CString&>& aString);	
	static void GetBuildingTypeForWindLoadUS(CArray<CString, CString&>& aString);	
	static void GetBuildingTypeForSnowLoad(CArray<CString, CString&>& aString);
	static void GetExposureCategory(CArray<CString, CString&>& aString);
	static void GetCheckPoint(CArray<CString, CString&>& aString, UINT nMembType, UINT nRoofType, double dRoofHeight, double dRoofAngle);

	static void GetRoofTypeForWindLoadKR(CArray<CString, CString&>& aString);
	static void GetRoofTypeForWindLoadUS(CArray<CString, CString&>& aString);
	static void GetRoofTypeForSnowLoadKR(CArray<CString, CString&>& aString);
	static void GetRoofTypeForSnowLoadUS(CArray<CString, CString&>& aString);

	static void GetTopographicFactor(CArray<CString, CString&>& aString);
	static void GetSurfaceRoughnessCategory(CArray<CString, CString&>& aString);
	static void GetBasicRoofSnowLoadFactor(CArray<CString, CString&>& aString);
	static void GetSnowLoadExposureFactor(CArray<CString, CString&>& aString);
	static void GetWindDirectionalityFactor(CArray<CString, CString&>& aString);
	static void GetSnowLoadThermalFactor(CArray<CString, CString&>& aString);

public:
	static CString GetBuildingTypeForWindLoadKR(int nBuildType);
	static CString GetBuildingTypeForWindLoadUS(int nBuildType);
	static CString GetBuildingTypeForSnowLoad(int nBuildType);
	static CString GetRoofType(int nRoofType, int nLoadType);
	static CString GetCheckPoint(int nCheckPoint);
	static CString GetTopographicFactor(double dFactor);
	static CString GetBasicRoofSnowLoadFactor(double dFactor);
	static CString GetSnowLoadExposureFactor(double dFactor);
	static CString GetWindDirectionalityFactor(double dFactor);
	static CString GetSnowLoadThermalFactor(double dFactor);

	static int GetBuildingTypeForWindLoadKR(CString csBuildType);
	static int GetBuildingTypeForWindLoadUS(CString csBuildType);
	static int GetBuildingTypeForSnowLoad(CString csBuildType);
	static int GetRoofType(CString csRoofType, int nLoadType);	
	static int GetCheckPoint(CString csCheckPoint);

public:
	static void GetWindLoadData(T_LOADDATA_D& rLoad);
	static void GetSnowLoadData(T_LOADDATA_D& rLoad);

private:
	static void CreateLoadDB();
	static void CreateWindLoad();
	static void CreateSnowLoad();
	static void AddLoadData(T_LOADDATA_LIST& aLoadDB, CString csZone, CString csCity, double dValue);
	static void GetLoadData(T_LOADDATA_D& rLoad);

	static void GetZoneList(CArray<CString, CString&>& aString);
	static void GetCityList(CArray<CString, CString&>& aString, CString csZone);
	static void GetImportanceList(CArray<CString, CString&>& aString);
	static void GetImportanceListUS(CArray<CString, CString&>& aString);

private:
	static double GetCurrentUnitWindLoad(double dValue);
	static double GetCurrentUnitSnowLoad(double dValue);
	static CString GetStringForFactor(double dFactor, int nDecimal);

	static BOOL IsExist(CString csString, CArray<CString, CString&>& aString);

private:
	static T_LOADDATA_LIST m_aLoadWind;
	static T_LOADDATA_LIST m_aLoadSnow;
	static int m_nLoadType;
};

#include "HeaderPost.h"

#endif // !defined(AFX_DB_LOAD_DATA_H____INCLUDED_)