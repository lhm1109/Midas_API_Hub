#pragma once
#include "DgnTxtConverterBase.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnTxtConverter : public CDgnTxtConverterBase
{
public:
	CDgnTxtConverter(const CString& csFileName);
	virtual ~CDgnTxtConverter();

public:
	bool ConvertToTXT(const DGNDRAW_TEXT_REPORT* pData);
	void SetPageBreak(const DGN_PAGE_BREAK& PageBreak);
	void SetPageCount(int nPage);
	long GetPageCount();
	void SetLineCount(int nLine);
	long GetLineCount();

private:
	bool ConvertPage(const DGNDRAW_TEXT_PAGE& Page);
	bool ConvertPara(const DGNDRAW_TEXT_PARA& Para);
	bool ConvertTextUnit(const DGNDRAW_TEXT_UNIT& TextUnit);

private:
	bool ConvertOutline(const DGNDRAW_TEXT_UNIT& TextUnit);
	bool ConvertText(const DGNDRAW_TEXT_UNIT& TextUnit);
	bool ConvertTable(const DGNDRAW_TEXT_UNIT& TextUnit, bool bSizeByText);
	bool ConvertPMCurve(const DGNDRAW_TEXT_UNIT& TextUnit);
	bool ConvertPreview(const DGNDRAW_TEXT_UNIT& TextUnit);
	bool ConvertMetaFile(const DGNDRAW_TEXT_UNIT& TextUnit);
	bool ConvertReferenceCode(const DGNDRAW_TEXT_UNIT& TextUnit, UINT nOutline);
	bool ConvertCluster(const DGNDRAW_TEXT_UNIT& TextUnit);

private:
	const DGNDRAW_TEXT_REPORT* m_pData;
	DGNDRAW_TEXT_PAGE* m_pPage;
};
#include "HeaderPost.h"