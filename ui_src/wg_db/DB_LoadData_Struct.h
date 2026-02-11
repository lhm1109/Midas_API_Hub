// DB_LoadData_Struct.h
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DB_LOAD_DATA_STRUCT_H____INCLUDED_)
#define AFX_DB_LOAD_DATA_STRUCT_H____INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LOAD_TYPE_WIND_KR 0
#define LOAD_TYPE_SNOW_KR 1
#define LOAD_TYPE_WIND_US 2
#define LOAD_TYPE_SNOW_US 3

// buildnig type for wind load.
#define WIND_LOAD_KR_BUILD_TYPE_CASE_CLOSE   0
#define WIND_LOAD_KR_BUILD_TYPE_CASE_POPEN   1
#define WIND_LOAD_KR_BUILD_TYPE_CASE_WOPEN   2
#define WIND_LOAD_KR_BUILD_TYPE_CASE_FOPEN   3
#define WIND_LOAD_KR_BUILD_TYPE_CHAR_CLOSE   _T("Enclosed")
#define WIND_LOAD_KR_BUILD_TYPE_CHAR_POPEN   _T("Partially Opened")
#define WIND_LOAD_KR_BUILD_TYPE_CHAR_WOPEN   _T("Widely Opened")
#define WIND_LOAD_KR_BUILD_TYPE_CHAR_FOPEN   _T("Opened")

#define WIND_LOAD_US_BUILD_TYPE_CASE_ENCLOSE 0
#define WIND_LOAD_US_BUILD_TYPE_CASE_PARTENC 1
#define WIND_LOAD_US_BUILD_TYPE_CHAR_ENCLOSE _T("Enclosed")
#define WIND_LOAD_US_BUILD_TYPE_CHAR_PARTENC _T("Partially Enclosed")

// building type for snow load.
#define SNOW_LOAD_BUILD_TYPE_CASE_HEATED  0
#define SNOW_LOAD_BUILD_TYPE_CASE_UNHEAT  1
#define SNOW_LOAD_BUILD_TYPE_CHAR_HEATED  _T("Heated Building")
#define SNOW_LOAD_BUILD_TYPE_CHAR_UNHEAT  _T("Unheated Building")

// roof type for wind load.
#define WIND_LOAD_KR_ROOF_TYPE_CASE_GABLET   0
#define WIND_LOAD_KR_ROOF_TYPE_CASE_MSLOPE   1
#define WIND_LOAD_KR_ROOF_TYPE_CASE_MGABLE   2
#define WIND_LOAD_KR_ROOF_TYPE_CASE_STOOTH   3
#define WIND_LOAD_KR_ROOF_TYPE_CHAR_GABLET   _T("Gablet")
#define WIND_LOAD_KR_ROOF_TYPE_CHAR_MSLOPE   _T("Mono Slope")
#define WIND_LOAD_KR_ROOF_TYPE_CHAR_MGABLE   _T("Multi Gablet")
#define WIND_LOAD_KR_ROOF_TYPE_CHAR_STOOTH   _T("Sawtooth")

#define WIND_LOAD_US_ROOF_TYPE_CASE_FLAT       0  
#define WIND_LOAD_US_ROOF_TYPE_CASE_GABLE      1
#define WIND_LOAD_US_ROOF_TYPE_CASE_MULTIGABLE 2
#define WIND_LOAD_US_ROOF_TYPE_CASE_MONOSLOPE  3
#define WIND_LOAD_US_ROOF_TYPE_CASE_SAWTOOTH   4
#define WIND_LOAD_US_ROOF_TYPE_CHAR_FLAT       _T("Flat")
#define WIND_LOAD_US_ROOF_TYPE_CHAR_GABLE      _T("Gable")  
#define WIND_LOAD_US_ROOF_TYPE_CHAR_MULTIGABLE _T("Multi Gable")
#define WIND_LOAD_US_ROOF_TYPE_CHAR_MONOSLOPE  _T("Mono Slope")
#define WIND_LOAD_US_ROOF_TYPE_CHAR_SAWTOOTH   _T("Sawtooth")

#define SNOW_LOAD_KR_ROOF_TYPE_CASE_GABLET   0
#define SNOW_LOAD_KR_ROOF_TYPE_CASE_CURVED   1
#define SNOW_LOAD_KR_ROOF_TYPE_CASE_MGABLE   2
#define SNOW_LOAD_KR_ROOF_TYPE_CASE_CYLIND   3
#define SNOW_LOAD_KR_ROOF_TYPE_CASE_STOOTH   4
#define SNOW_LOAD_KR_ROOF_TYPE_CHAR_GABLET   _T("Gablet")
#define SNOW_LOAD_KR_ROOF_TYPE_CHAR_CURVED   _T("Curved Roof")
#define SNOW_LOAD_KR_ROOF_TYPE_CHAR_MGABLE   _T("Multi Gablet")
#define SNOW_LOAD_KR_ROOF_TYPE_CHAR_CYLIND   _T("Cylindrical")
#define SNOW_LOAD_KR_ROOF_TYPE_CHAR_STOOTH   _T("Sawtooth")

#define SNOW_LOAD_US_ROOF_TYPE_CASE_SLOPE      0
#define SNOW_LOAD_US_ROOF_TYPE_CASE_GABLE      1
#define SNOW_LOAD_US_ROOF_TYPE_CASE_CURVED     2
#define SNOW_LOAD_US_ROOF_TYPE_CASE_MULTIFOLD  3
#define SNOW_LOAD_US_ROOF_TYPE_CASE_BARREL     4
#define SNOW_LOAD_US_ROOF_TYPE_CASE_SAWTOOTH   5
#define SNOW_LOAD_US_ROOF_TYPE_CHAR_SLOPE      _T("Slope")
#define SNOW_LOAD_US_ROOF_TYPE_CHAR_GABLE      _T("Gable")
#define SNOW_LOAD_US_ROOF_TYPE_CHAR_CURVED     _T("Curved")
#define SNOW_LOAD_US_ROOF_TYPE_CHAR_MULTIFOLD  _T("Multiple Folded Plate")
#define SNOW_LOAD_US_ROOF_TYPE_CHAR_BARREL     _T("Barrel Vault")
#define SNOW_LOAD_US_ROOF_TYPE_CHAR_SAWTOOTH   _T("Sawtooth")

#define WIND_LOAD_CHECK_CASE_1  1
#define WIND_LOAD_CHECK_CASE_2  2
#define WIND_LOAD_CHECK_CASE_3  3
#define WIND_LOAD_CHECK_CASE_4  4
#define WIND_LOAD_CHECK_CASE_5  5
#define WIND_LOAD_CHECK_CHAR_1  _T("1")
#define WIND_LOAD_CHECK_CHAR_2  _T("2")
#define WIND_LOAD_CHECK_CHAR_3  _T("3")
#define WIND_LOAD_CHECK_CHAR_4  _T("4")
#define WIND_LOAD_CHECK_CHAR_5  _T("5")

#define BUILDING_IMPORTANT_CATEGORY_S  _T("Ư")
#define BUILDING_IMPORTANT_CATEGORY_1  _T("1" )
#define BUILDING_IMPORTANT_CATEGORY_2  _T("2" )
#define BUILDING_IMPORTANT_CATEGORY_3  _T("3" )

#define BUILDING_IMPORTANT_CATEGORY_I   _T("I")
#define BUILDING_IMPORTANT_CATEGORY_II  _T("II")
#define BUILDING_IMPORTANT_CATEGORY_III _T("III")
#define BUILDING_IMPORTANT_CATEGORY_IV  _T("IV")

struct T_LOADDATA_D
{
	CString csZone;
	CString csCity;
	double  dValue;
	
	void Initialize()
	{
		csZone.Empty();
		csCity.Empty();
		dValue = 0.0;
	}
	
	T_LOADDATA_D()	{ Initialize(); }
	T_LOADDATA_D(const T_LOADDATA_D& src)	{*this = src;}
	T_LOADDATA_D& operator = (const T_LOADDATA_D& src)
	{
		Initialize();
		csZone = src.csZone;
		csCity = src.csCity;
		dValue  = src.dValue;
		
		return *this;
	}
};

typedef CArray<T_LOADDATA_D, T_LOADDATA_D&> T_LOADDATA_LIST;

#endif // !defined(AFX_DB_LOAD_DATA_STRUCT_H____INCLUDED_)
