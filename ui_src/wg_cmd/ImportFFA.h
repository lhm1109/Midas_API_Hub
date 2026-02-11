#pragma once

#include "..\wg_db\DB_ST_DT_ERPR.h"
#include "..\wg_db\DB_ST_DT_UNIT.h"

enum class FFAMsg
{
    EN_READ_FAIL = 0,
    EN_FILE_EMPTY,  // "FFA Text File is Empty"
    EN_UNIT_LENGTH, // "Unit for Length is not Valid"
    EN_UNIT_FORCE,  // "Unit for Force is not Valid"
    EN_DATA_EMPTY,  // "FFA Text File has no Valid Data"
    EN_DATA_VALID,  // OK
};

class CImportFFA
{
public:
    CImportFFA();
    virtual ~CImportFFA();

public:
    FFAMsg ImportFFA(const CString& csFilePath);
    void GetSoilProp(CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA &>& raSoilProp);

private:
    bool ReadTextFile(const CString& csFilePath, CStringArray& raAllTextLine);
    void RemoveBlankComment(CStringArray& raText);
    bool ParseLine(const CString& csLine, CStringArray& raText);
    int GetLineIndex(const CStringArray& aText, const CString& csHead);

private:
    bool GetUnitInfo(const CStringArray& aText, T_UNIT_INDEX& rFileUnit, bool& rbUnitLeng, bool& rbUnitForc);
    bool GetSoilProp(const CStringArray& aText, CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA &>& raSoilProp);
    void ConvertUnit(const T_UNIT_INDEX& FileUnit, CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA &>& raSoilProp);

private:
	CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA &> m_aSoilProp;
    //T_GURBW_SOIL_PROPERTY_LIST m_aSoilProp;
    CStringArray m_aHeadText;
};
