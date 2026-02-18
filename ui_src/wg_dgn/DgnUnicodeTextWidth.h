#pragma once
#include "DgnTxtConverterStruct.h"

class CDgnUnicodeTextWidth
{
public:
	CDgnUnicodeTextWidth();
	virtual ~CDgnUnicodeTextWidth();

public:
	bool IsFullWidthText(const CString& csText);
	bool IsDoubleSpaceText(const CString& csText);

private:
	void BuildDataMap();
	void BuildTextData();
	void GetDataString(CStringArray& raData);
	long ConvertStringToInt(const CString& csStr);
	long GetStringInt(const CString& csStr);

	bool GetMapData(D_UNICODE_TEXT_WIDTH& rTextWidth, CString& csText);
	bool GetDataValue(D_UNICODE_TEXT_WIDTH& rTextWidth, CString& csText);
	bool GetDataInfo(D_UNICODE_TEXT_WIDTH& rTextWidth, CString& csText);

private:
	bool IsExistInSingleText(long nChar);
	bool IsExistInDoubleText(long nChar);

private:
	D_UNICODE_TEXT_WIDTH_MAPEX m_mData;
	CArray<long, long> m_aSingleText;
	CArray<long, long> m_aDoubleText;
};
