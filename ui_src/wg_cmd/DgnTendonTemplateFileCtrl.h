#pragma once

#include <vector>

struct TndnTemplate;
class CDgnTendonTemplateFileCtrl
{
public:
	CDgnTendonTemplateFileCtrl(void);
	~CDgnTendonTemplateFileCtrl(void);

public :
	BOOL Import(TCHAR* pFilePath, std::vector<TndnTemplate>* pVecTndn, BOOL& bUseRatio);
	BOOL Export(TCHAR* pFilePath, std::vector<TndnTemplate>* pVecTndn, BOOL bUseRatio);

	BOOL IsExist(const CString& str, std::vector<TndnTemplate>* pVecTndn);

private:
	void GetNewName(CString& str, std::vector<TndnTemplate>* pVecTndn);

	void SetValue(TndnTemplate& tndnInfo, const CString& strType, const CString& strValue, const CString& strXYZ, int& idx);

	BOOL ExportSub(const TndnTemplate& tndnTemp, std::vector<CString>* pVecStrOut);

private:
};

