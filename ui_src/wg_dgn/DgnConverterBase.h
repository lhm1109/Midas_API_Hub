#pragma once
#include "DgnUnicodeTextWidth.h"
#include "../dgnengine/src/DgnDrawBase/DgnDrawBase_TextPageFunc_Struct.h"

class CDgnConverterBase
{
public:
	CDgnConverterBase();
	virtual ~CDgnConverterBase();

public:
	void SetOutputFileName(const CString& csFileName);

protected:
	bool IsPageWithMultipleZone(const DGNDRAW_TEXT_PAGE& Page);
	bool IsMergeTableREQ(UINT nTypePrev, UINT nTypeCurr, UINT nTypeNext, bool bLastTextUnit);
	CString GetWindowsListSeparator();
	CString GetDefaultFontName();
	CString GetCommaReplace(bool bEquation);
	CString GetSpace(UINT nSpaces, CString csSpaceText = _T(" "));

protected:
	UINT GetNumberOfDoubleSpaceText(const CString& csText);
	bool IsEmptyString(const CString& str);
	CString GetText(LPCTSTR lpszFmt, ...);
	bool GetSuccessiveReferenceCode(DGNDRAW_TEXT_UNIT_LIST& raTextUnit, const DGNDRAW_TEXT_PARA& Para);

	void AddSpaceBeforeAndAfterBrace(CString& rcsText);
	void RemoveMultipleSpaces(CString& rcsText);
	bool DivideText(CStringArray& raText, CString& csText, CString csDivide);
	bool DivideTextForScript(std::vector<CString>& rvText, CString& csText);
	void DivideTextBySpaceAndMiddleBrace(CStringArray& raText, CString& csText);
	void GetFractionMolecule(CString csSrcText, CString& rcsMolecule, CString& rcsRemains);
	void GetFractionDenominator(CString csSrcText, CString& rcsDenomin, CString& rcsRemains);
	void GetScriptNormalText(CString csSrcText, CString& rcsNormal, CString& rcsRemain);
	void GetScriptText(CString csSrcText, CString& rcsScript, CString& rcsRemain);

protected:
	void DivideTextForFraction(DGN_FRACTION_DATA_LIST& raFraction, CString& csText);
	bool IsFraction(const DGN_FRACTION_DATA& txtFract);
	bool IsFraction(const CString& csText);
	void RemoveTabPositionNum(CString& rcsText, CArray<double, double&>& raTabPos);
	CString GetLongerText(const DGN_FRACTION_DATA& txtFract);

private:
	bool IsTableUnit(UINT nTextUnitType);
	void TrimText(CString& rcsText);
	void GetScriptByBracket(CString& rcsBracket, CString& rcsRemain);

	int GetPositionOfSmallBracket(const CString& csText);
	int GetPositionOfMiddleBracket(const CString& csText);
	int GetReversePositionOfMiddleBracket(const CString& csText);
	int GetReversePositionOfSmallBracket(const CString& csText);

	void CheckFractionDenominator(CString& rcsLower, CString& rcsRemain);

protected:
	CDgnUnicodeTextWidth* m_pTextWidthTool;
	CString m_csListSeperator;

	UINT m_nLoopMax;
	CString m_csOutputFileName;
};
