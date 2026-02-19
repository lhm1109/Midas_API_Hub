#pragma once

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CNationalAnnexTool final
{
public:
    CNationalAnnexTool();
    virtual ~CNationalAnnexTool();

public:
    static int ConvertNationalAnnex(CString strCode);
    static CString ConvertNationalAnnex(int nNationalAnnex);
	
    static int ConvertAASHTOAmendment(CString strCode);
    static CString ConvertAASHTOAmendment(int nAmendment);

	static int ConvertPscASAmendment(CString strCode);
	static CString ConvertPscASAmendment(int nAmendment);
    static int ConvertRcASAmendment(CString strCode);
    static CString ConvertRcASAmendment(int nAmendment);

	static int ConvertReportLanguage(CString strCode);
	static CString ConvertReportLanguage(int nNationalAnnex);

	static int ConvertReportUnit(CString strCode);
	static CString ConvertReportUnit(int nNationalAnnex);

    static int IsAvailableNationalAnnex(int nNationalAnnex);

    static CString Get_Alpha_3_code(const int& nNationalAnnex);

    static int ConvertEnumDBtoDGN(int nCode);
    static int ConvertEnumDGNtoDB(int nCode);
    static CString Get_EC2_04_Codename(const int& nNationalAnnex);
    static CString Get_EC3_05_Codename(const int& nNationalAnnex);
    static CString Get_EC3_1_3_06_Codename(const int& nNationalAnnex);

public:
    static std::vector<CString> GetRcNationalAnnex();
    static std::vector<CString> GetStlNationalAnnex();
    static std::vector<CString> GetCfsNationalAnnex();
    static std::vector<CString> GetCpgNationalAnnex();

    static std::vector<CString> GetRcAASHTOAmendment();
    static std::vector<CString> GetRcASAmendment();
    static std::vector<CString> GetPscASAmendment();

	static std::vector<CString> GetReportLanguage(const CString& strCode);
	static std::vector<CString> GetReportUnitString();    
};

#include "HeaderPost.h"
