#pragma once
#include "DgnConverterBase.h"
#include "DgnTxtConverterStruct.h"
#include "../dgnengine/src/DgnDrawBase/DgnDrawBase_TextPageFunc_Struct.h"

class CDgnTxtConverterBase : public CDgnConverterBase
{
public:
	CDgnTxtConverterBase(const CString& csFileName);
	virtual ~CDgnTxtConverterBase();

protected:
	void SetPageBreak(const DGN_PAGE_BREAK& PageBreak);
	void SetPageCount(int nPage);
	long GetPageCount();
	void SetLineCount(int nLine);
	long GetLineCount();

protected:
	void WriteBlank(int nRows = 1);
	void WriteOutline1(const CString& str);
	void WriteOutline2(const CString& str);
	void WriteOutline3(const DGNDRAW_TEXT_UNIT& TextUnit);
	void WriteOutline4(const DGNDRAW_TEXT_UNIT& TextUnit);

	void WriteText(const CString& str);
	void WriteReferenceCode(const CString& csCode, const bool bBrackets, UINT nOutline);
	void WriteTable(const DGNDRAW_TABLE& DrawTable, bool bSizeByText);

protected:
	bool IsParaForPageBreak(const DGNDRAW_TEXT_UNIT& TextUnit);
	long GetPara(const DGNDRAW_TEXT_UNIT_LIST& aText, long nIdx);
	long GetPara(const DGNDRAW_TEXT_UNIT& TextUnit);
	void PageBreak(long nPara);

private:
	void WriteToFile(const CString& csText);
	void AddSymbolForCheckItem(CString& rcsText, const DGNDRAW_TEXT_UNIT& TextUnit);
	void ConvertTextForFile(CString& rcsText, const CString& csOutlineText = _T(""));
	void RemoveTabPositionNum(CString& rcsText, const CString& csOutlineText = _T(""));
	void ConvertSquareRoot(CString& rcsText, CString csSqrtMark);
	void ConvertFraction(CString& rcsText);
	void ConvertSubscript(CString& rcsText);
	void ConvertSuperscript(CString& rcsText);

	std::string ConvertToAscii(const std::wstring& wStr);
	CString GetAsciiText(const CString& csText);
	CString GetTextWithOutline(const CString& csText, UINT nOutlineLevel, UINT nOutlineNo);
	CString GetAlignedText(const CString& csText, UINT nTotalLen, UINT nAlignment);
	CString GetTextToCalcParagraphLength(UINT nOutlineLevel);

	void ConvertFractionText(DGN_FRACTION_DATA_LIST& raFraction, CString csOutlineText = _T(""));
	void WriteFractionToFile(const DGN_FRACTION_DATA_LIST& aFraction, const DGNDRAW_TEXT_UNIT& TextUnit, UINT nOutlineLevel, UINT nOutlineNo);
	bool GetTextForFraction(const DGN_FRACTION_DATA_LIST& aFraction, CString& csUpper, CString& csCentr, CString& csLower);
	bool AddBraceForFraction();

private:
	void ConvertTable(D_TEXT_TABLE& rTextTable, const DGNDRAW_TABLE& DrawTable);
	void ConvertTableCell(D_TEXT_TABLE_CELL& rTextCell, const DGNDRAW_TABLE_CELL& DrawCell);
	void AddAdditionalSpaceToFitSize(CArray<UINT, UINT>& raColumnSize);
	void GetTableColumnSizeByText(CArray<UINT, UINT>& raColumnSize, const D_TEXT_TABLE& TextTable);
	void GetTableColumnSizeByQuick(CArray<UINT, UINT>& raColumnSize, const DGNDRAW_TABLE& DrawTable);
	void GetTableColumnSizeByDataTable(CArray<UINT, UINT>& raColumnSize, const DGNDRAW_TABLE& HeadTable, const DGNDRAW_TABLE& DataTable);
	void WriteTableRow(const D_TEXT_TABLE_ROW& Row, const CArray<UINT, UINT>& aColumnSize);
	void WriteTableLine(const CArray<UINT, UINT>& aColumnSize, bool bDoubleLine);

private:
	void WriteTableRowText(CString& csText);
	UINT GetTextLength(const CString& csText);
	bool GetEqualPosInfo1st(D_EQUAL_POS_INFO& rData, const CString& csText);
	bool GetEqualPosInfoEnd(D_EQUAL_POS_INFO& rData, const CString& csText);

private:
	void MakeASCIImap();
	CString GetBlankByLevel(UINT nOutlineLevel);
	CString GetOutlineNumber(UINT nOutlineLevel, UINT nOutlineNo);

private:
	bool m_bAlignByEqual1st;
	bool m_bAlignByEqualEnd;

	CString m_csFileName;
	long m_nPageCount;
	long m_nLineCount;
	DGN_PAGE_BREAK m_PageBreak;
	std::unordered_map<wchar_t, std::string> m_mapASCII;

	std::vector<UINT> m_vTabPos;
	UINT m_nOutline1;
	UINT m_nOutline2;
	UINT m_nOutline3;
	UINT m_nOutline4;
};
