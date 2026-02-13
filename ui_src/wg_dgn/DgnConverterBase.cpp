#include "StdAfx.h"
#include "DgnConverterBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_MAX_PREVIEW_WIDTH  16000.0

CDgnConverterBase::CDgnConverterBase()
{
	m_csOutputFileName.Empty();
	m_nLoopMax = 1000;
	m_csListSeperator = GetWindowsListSeparator();
	if ( m_csListSeperator.IsEmpty() )
	{
		m_csListSeperator = _T(",");
	}

	m_pTextWidthTool = new CDgnUnicodeTextWidth();
}

CDgnConverterBase::~CDgnConverterBase()
{
	if ( m_pTextWidthTool != nullptr )
	{
		delete m_pTextWidthTool;
		m_pTextWidthTool = nullptr;
	}
}

void CDgnConverterBase::SetOutputFileName(const CString& csFileName)
{
	m_csOutputFileName = csFileName;
}

bool CDgnConverterBase::GetSuccessiveReferenceCode(DGNDRAW_TEXT_UNIT_LIST& raTextUnit, const DGNDRAW_TEXT_PARA& Para)
{
	raTextUnit.RemoveAll();

	long nText = Para.aText.GetCount();
	for ( long i = 0; i < nText; ++i )
	{
		const DGNDRAW_TEXT_UNIT& TextUnit = Para.aText[i];
		if ( TextUnit.nType != ParaTypeRefCode ) break;
		raTextUnit.Add(const_cast< DGNDRAW_TEXT_UNIT& >(TextUnit));
	}

	return ( raTextUnit.GetCount() > 0 ) ? true : false;
}

bool CDgnConverterBase::IsPageWithMultipleZone(const DGNDRAW_TEXT_PAGE& Page)
{
	long nPara = Page.aPara.GetCount();
	UINT nNewZone = 0;
	for ( long i = 0; i < nPara; ++i )
	{
		const DGNDRAW_TEXT_PARA& Para = Page.aPara[i];
		if ( Para.bNewZone )
		{
			nNewZone++;
		}
	}

	return ( nNewZone > 1 ) ? true : false;
}

bool CDgnConverterBase::IsMergeTableREQ(UINT nTypePrev, UINT nTypeCurr, UINT nTypeNext, bool bLastTextUnit)
{
	if ( nTypePrev == ParaTypeTableNS )
	{
		if ( bLastTextUnit )
		{
			if ( !IsTableUnit(nTypeCurr) ) return false;
		}
		else
		{
			if ( !IsTableUnit(nTypeCurr) ) return false;
			if ( nTypeCurr == ParaTypeTable ) return true;
			if ( !IsTableUnit(nTypeNext) ) return false;
		}
		return true;
	}
	return false;
}

bool CDgnConverterBase::IsTableUnit(UINT nTextUnitType)
{
	switch ( nTextUnitType )
	{
	case ParaTypeOutline:
	case ParaTypeBody:
	case ParaTypeBullet:
	case ParaTypePMCurve:
	case ParaTypePreview:
	case ParaTypeMetaFile:
	case ParaTypeRefCode:
	case ParaTypeComment:
	case ParaTypeBarCluster:
		{
			return false;
		}
		break;
	case ParaTypeTable:
	case ParaTypeTableNS:
		{
			return true;
		}
		break;
	}
	ASSERT(0); return false;
}

UINT CDgnConverterBase::GetNumberOfDoubleSpaceText(const CString& csText)
{
	if ( m_pTextWidthTool )
	{
		int nText = csText.GetLength();

		UINT nFullWidthText = 0;
		for ( int i = 0; i < nText; ++i )
		{
			CString csChar = csText.GetAt(i);
			if ( m_pTextWidthTool->IsDoubleSpaceText(csChar) )
			{
				nFullWidthText++;
			}
		}
		return nFullWidthText;
	}
	return 0;
}

CString CDgnConverterBase::GetText(LPCTSTR lpszFmt, ...)
{
	va_list args;
	va_start(args, lpszFmt);

	CString csText;
	csText.FormatV(lpszFmt, args);

	va_end(args);

	return csText;
}

CString CDgnConverterBase::GetWindowsListSeparator()
{
	TCHAR szBuf[64];
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, ( LPWSTR ) ( szBuf ), sizeof(szBuf));

	CString csSeparator = CString(szBuf);
	if ( csSeparator.IsEmpty() )
	{
		ASSERT(0);
		csSeparator = _T(",");
	}
	return csSeparator.Trim();
}

CString CDgnConverterBase::GetDefaultFontName()
{
	LANGID UILanguage = GetUserDefaultUILanguage();
	if ( UILanguage != 0 )
	{
		if ( UILanguage == 0x412 )
		{
			return _T("맑은 고딕");
		}
		else
		{
			return _T("Calibri");
		}
	}
	return _T("Calibri");
}

CString CDgnConverterBase::GetCommaReplace(bool bEquation)
{
	CString csComma = _T(",");
	CString csReplace = csComma;
	if ( bEquation )
	{
		csReplace = GetText(_T("\\%s"), csComma);
	}
	return csReplace;
}

CString CDgnConverterBase::GetSpace(UINT nSpaces, CString csSpaceText)
{
	if ( nSpaces > 0 )
	{
		CString csFormat = GetText(_T("%%%ds"), nSpaces); // (ex) %3s
		CString csSpaces = GetText(csFormat, _T(" "));
		csSpaces.Replace(_T(" "), csSpaceText);
		return csSpaces;
	}
	return CString();
}

bool CDgnConverterBase::IsEmptyString(const CString& str)
{
	if ( str.IsEmpty() ) return true;
	if ( str == _T(" ") ) return true;
	return false;
}

void CDgnConverterBase::AddSpaceBeforeAndAfterBrace(CString& rcsText)
{
	if ( rcsText.IsEmpty() ) return;

	if ( rcsText.Find(_T("forcebrace@")) > -1 )
	{
		rcsText.Replace(_T("forcebrace@"), _T(""));
		return;
	}

	int nSize = rcsText.GetLength();
	for ( int i = nSize - 1; i > -1; --i )
	{
		CString csCurr = rcsText[i];

		CString csPrev = ( i > 0 ) ? rcsText[i - 1] : _T('\0');
		if ( ( csCurr == _T(")") || csCurr == _T("}") || csCurr == _T("]") ) && csPrev != _T(' ') )
		{
			rcsText.Insert(i, _T(' '));
		}

		CString csNext = ( i + 1 < nSize - 1 ) ? rcsText[i + 1] : _T('\0');
		if ( ( csCurr == _T('(') || csCurr == _T('{') || csCurr == _T('[') ) && csNext != _T(' ') )
		{
			rcsText.Insert(i + 1, _T(' '));
		}
	}

	RemoveMultipleSpaces(rcsText);
}

void CDgnConverterBase::RemoveMultipleSpaces(CString& rcsText)
{
	UINT nLoop = 0;
	while ( true )
	{
		int nPosSpace = rcsText.Find(_T("  "));
		if ( nPosSpace < 0 ) break;
		if ( nLoop++ > m_nLoopMax ) { ASSERT(0); break; }
		rcsText.Replace(_T("  "), _T(" "));
	}
}

bool CDgnConverterBase::DivideTextForScript(std::vector<CString>& rvText, CString& rcsText)
{
	rvText.clear();

	CStringArray aText;
	if ( DivideText(aText, rcsText, _T("$")) )
	{
		long nText = aText.GetCount();
		for ( long i = 0; i < nText; ++i )
		{
			CString csText = aText[i] + _T("$");
			rvText.push_back(csText);
		}
	}
	return !rvText.empty();
}

bool CDgnConverterBase::DivideText(CStringArray& raText, CString& csText, CString csDivide)
{
	raText.RemoveAll();

	int nLenDivide = csDivide.GetLength();
	UINT nLoop = 0;
	while ( true )
	{
		int nPos = csText.Find(csDivide);
		if ( nPos < 0 ) break;
		if ( nLoop++ > m_nLoopMax ) { ASSERT(0); break; }

		CString csTemp = csText.Left(nPos);
		raText.Add(csTemp.Trim());
		csText = csText.Mid(nPos + nLenDivide);
	}

	if ( !csText.IsEmpty() )
	{
		raText.Add(csText.Trim());
	}
	return raText.IsEmpty() ? false : true;
}

void CDgnConverterBase::DivideTextBySpaceAndMiddleBrace(CStringArray& raText, CString& csText)
{
	raText.RemoveAll();

	UINT nLoop = 0;
	while ( true )
	{
		TrimText(csText);
		if ( csText.IsEmpty() ) break;
		if ( nLoop++ > m_nLoopMax ) { ASSERT(0); break; }

		CString csDivided;
		CString csFirst = csText.GetAt(0);
		if ( csFirst == _T("{") )
		{
			int nPosBrace = GetPositionOfMiddleBracket(csText);
			if ( nPosBrace < 0 ) break;
			csDivided = csText.Left(nPosBrace);
			csText = csText.Mid(nPosBrace + 1);
		}
		else
		{
			int nPosBlank = csText.Find(_T(" "));
			if ( nPosBlank < 0 ) break;
			csDivided = csText.Left(nPosBlank);
			csText = csText.Mid(nPosBlank);
		}

		if ( csDivided.IsEmpty() ) break;
		raText.Add(csDivided);
	}

	if ( !csText.IsEmpty() )
	{
		raText.Add(csText);
	}
}

void CDgnConverterBase::TrimText(CString& rcsText)
{ // 탭이 마지막에 포함되는 경우 탭도 같이 잘려나가는 문제가 있어서 별도의 트림 함수를 구성함.
	// "xxx\t "인 경우 혹은 "xxx \t"인 경우에 해당함.
	if ( !rcsText.IsEmpty() && ( rcsText[0] == _T(' ') ) )
	{
		rcsText.Delete(0);
	}
	if ( !rcsText.IsEmpty() && ( rcsText[rcsText.GetLength() - 1] == _T(' ') ) )
	{
		rcsText.Delete(rcsText.GetLength() - 1);
	}

	if ( !rcsText.IsEmpty() && ( rcsText[0] != _T('\t') ) )
	{
		rcsText.TrimLeft();
	}
	if ( !rcsText.IsEmpty() && ( rcsText[rcsText.GetLength() - 1] != _T('\t') ) )
	{
		rcsText.TrimRight();
	}
}

void CDgnConverterBase::GetFractionMolecule(CString csSrcText, CString& rcsMolecule, CString& rcsRemains)
{
	csSrcText.TrimRight();
	CString csLast = csSrcText[csSrcText.GetLength() - 1];
	if ( csLast == _T("}") )
	{
		int nPosBrace = GetReversePositionOfMiddleBracket(csSrcText);
		rcsMolecule = csSrcText.Mid(nPosBrace + 1, csSrcText.GetLength() - nPosBrace - 2);
		rcsRemains  = csSrcText.Left(nPosBrace - 1);
	}
	else if ( csLast == _T(")") )
	{
		int nPosBrace = GetReversePositionOfSmallBracket(csSrcText);
		rcsMolecule = csSrcText.Mid(nPosBrace + 1, csSrcText.GetLength() - nPosBrace - 2);
		rcsRemains  = csSrcText.Left(nPosBrace - 1);
	}
	else
	{
		int nPosBlank = csSrcText.ReverseFind(_T(' '));
		if ( nPosBlank < 0 )
		{
			rcsMolecule = csSrcText;
			rcsRemains.Empty();
		}
		else
		{
			rcsMolecule = csSrcText.Mid(nPosBlank + 1, csSrcText.GetLength() - 2);
			rcsRemains  = csSrcText.Left(nPosBlank);
		}
	}

	TrimText(rcsMolecule);
	TrimText(rcsRemains);
}

void CDgnConverterBase::GetFractionDenominator(CString csSrcText, CString& rcsDenomin, CString& rcsRemains)
{
	csSrcText.TrimLeft();
	CString csFirst = csSrcText[0];
	if ( csFirst == _T("{") )
	{
		int nPosBrace = GetPositionOfMiddleBracket(csSrcText);
		rcsDenomin = csSrcText.Mid(1, nPosBrace - 1);
		rcsRemains = csSrcText.Mid(nPosBrace + 2);
	}
	else
	{
		int nPosBlank = csSrcText.Find(_T(' '));
		if ( nPosBlank > -1 )
		{
			rcsDenomin = csSrcText.Left(nPosBlank);
			rcsRemains = csSrcText.Mid(nPosBlank + 1);

			int nDenominLen = rcsDenomin.GetLength();
			if ( nDenominLen > 0 && rcsDenomin[nDenominLen - 1] == _T(',') )
			{
				rcsDenomin.Delete(nDenominLen - 1);
				rcsRemains = _T(",") + rcsRemains;
			}
		}
		else
		{
			int nPosComma = csSrcText.Find(_T(","));
			if ( nPosComma > -1 )
			{
				rcsDenomin = csSrcText.Left(nPosComma);
				rcsRemains = csSrcText.Mid(nPosComma);
			}
			else
			{
				rcsDenomin = csSrcText;
				rcsRemains.Empty();
			}
		}
	}

	TrimText(rcsDenomin);
	TrimText(rcsRemains);
}

void CDgnConverterBase::GetScriptNormalText(CString csSrcText, CString& rcsNormal, CString& rcsRemain)
{
	csSrcText.TrimRight();
	int nPosBlank = csSrcText.ReverseFind(_T(' '));
	if ( nPosBlank > -1 )
	{
		rcsNormal = csSrcText.Mid(nPosBlank + 1, csSrcText.GetLength() - 1);
		rcsRemain = csSrcText.Left(nPosBlank + 1);
	}
	else
	{
		rcsNormal = csSrcText;
		rcsRemain.Empty();
	}

	TrimText(rcsNormal);
	TrimText(rcsRemain);

	if ( rcsNormal == _T(")") )
	{
		GetScriptByBracket(rcsNormal, rcsRemain);
	}
}

void CDgnConverterBase::GetScriptText(CString csSrcText, CString& rcsScript, CString& rcsRemain)
{
	csSrcText.TrimLeft();
	CString csFirst = csSrcText[0];
	if ( csFirst == _T("{") )
	{
		int nPosBrace = GetPositionOfMiddleBracket(csSrcText);
		rcsScript = csSrcText.Mid(1, nPosBrace - 1);
		rcsRemain = csSrcText.Mid(nPosBrace + 2);
	}
	else if ( csFirst == _T("(") )
	{
		int nPosBrace = GetPositionOfSmallBracket(csSrcText);
		rcsScript = csSrcText.Mid(1, nPosBrace - 1);
		rcsRemain = csSrcText.Mid(nPosBrace + 2);
	}
	else
	{
		int nPosMark = csSrcText.Find(_T('$'));
		if ( nPosMark > -1 )
		{
			rcsScript = csSrcText.Left(nPosMark);
			rcsRemain = csSrcText.Mid(nPosMark + 1);
		}
		else
		{
			int nPosDivider = csSrcText.FindOneOf(_T("/)"));
			rcsScript = csSrcText.Left(nPosDivider);
			rcsRemain = csSrcText.Mid(nPosDivider);
		}
	}

	int nScriptLen = rcsScript.GetLength();
	if ( ( nScriptLen > 0 ) && ( rcsScript[nScriptLen - 1] == _T('$') ) )
	{
		rcsScript.Delete(nScriptLen - 1);
	}

	TrimText(rcsScript);
	TrimText(rcsRemain);
}

void CDgnConverterBase::GetScriptByBracket(CString& rcsBracket, CString& rcsRemain)
{
	if ( rcsRemain.IsEmpty() ) return;
	int nPosBracket = GetPositionOfSmallBracket(rcsRemain);
	if ( nPosBracket < 0 ) return;

	rcsBracket = rcsRemain.Mid(nPosBracket) + rcsBracket;
	rcsRemain  = rcsRemain.Left(nPosBracket);
}

int CDgnConverterBase::GetPositionOfSmallBracket(const CString& csText)
{
	int nCount = 0;
	int nTextLeng = csText.GetLength();
	for ( int i = 0; i < nTextLeng; ++i )
	{
		const CString& csCur = csText[i];
		if ( csCur == _T("(") ) nCount++;
		if ( csCur == _T(")") ) nCount--;
		if ( nCount == 0 )
		{
			return i;
		}
	}
	/*ASSERT(0);*/ return -1;
}

int CDgnConverterBase::GetPositionOfMiddleBracket(const CString& csText)
{
	int nCount = 0;
	int nTextLeng = csText.GetLength();
	for ( int i = 0; i < nTextLeng; ++i )
	{
		const CString& csCur = csText[i];
		if ( csCur == _T("{") ) nCount++;
		if ( csCur == _T("}") ) nCount--;
		if ( nCount == 0 )
		{
			return i;
		}
	}
	/*ASSERT(0);*/ return -1;
}

int CDgnConverterBase::GetReversePositionOfMiddleBracket(const CString& csText)
{
	int nCount = 0;
	int nTextLeng = csText.GetLength();
	for ( int i = nTextLeng - 1; i >= 0; --i )
	{
		const CString& csCur = csText[i];
		if ( csCur == _T("}") ) nCount++;
		if ( csCur == _T("{") ) nCount--;

		if ( nCount == 0 )
		{
			return i;
		}
	}
	/*ASSERT(0);*/ return -1;
}

int CDgnConverterBase::GetReversePositionOfSmallBracket(const CString& csText)
{
	int nCount = 0;
	int nTextLeng = csText.GetLength();
	for ( int i = nTextLeng - 1; i >= 0; --i )
	{
		const CString& csCur = csText[i];
		if ( csCur == _T(")") ) nCount++;
		if ( csCur == _T("(") ) nCount--;

		if ( nCount == 0 )
		{
			return i;
		}
	}
	/*ASSERT(0);*/ return -1;
}

void CDgnConverterBase::RemoveTabPositionNum(CString& rcsText, CArray<double, double&>& raTabPos)
{
	CStringArray aText;
	DivideText(aText, rcsText, _T("\t"));

	rcsText.Empty();
	raTabPos.RemoveAll();

	long nText = aText.GetCount();
	for ( long i = 0; i < nText; ++i )
	{
		CString& csTemp = aText[i];

		if ( i + 1 < nText )
		{
			int nPosBlank = csTemp.ReverseFind(' ');
			CString csTabPos = csTemp.Mid(nPosBlank);
			double dTabPos = _tstof(csTabPos);
			raTabPos.Add(dTabPos);

			csTemp = csTemp.Left(nPosBlank + 1);
		}

		rcsText += csTemp;
		if ( i + 1 < nText )
		{
			rcsText += _T("\t "); // 탭문자뒤에 공백하나 필요함.
		}
	}
}

void CDgnConverterBase::DivideTextForFraction(DGN_FRACTION_DATA_LIST& raFraction, CString& csText)
{
	raFraction.RemoveAll();

	CString csFractMark = _T("over$");
	int nFractMarkLen = csFractMark.GetLength();

	UINT nLoop = 0;
	while ( true )
	{
		int nPosMark = csText.Find(csFractMark);
		if ( nPosMark < 0 ) break;
		if ( nLoop++ > m_nLoopMax ) { ASSERT(0); break; }

		CString csTempL = csText.Left(nPosMark);
		CString csTempR = csText.Mid(nPosMark + nFractMarkLen);

		CString csUpper, csUppRemain;
		GetFractionMolecule(csTempL, csUpper, csUppRemain);

		CString csLower, csLowRemain;
		GetFractionDenominator(csTempR, csLower, csLowRemain);
		CheckFractionDenominator(csLower, csLowRemain);

		if ( !csUppRemain.IsEmpty() )
		{
			raFraction.Add(DGN_FRACTION_DATA(csUppRemain, _T("")));
		}
		if ( !csUpper.IsEmpty() && !csLower.IsEmpty() )
		{
			raFraction.Add(DGN_FRACTION_DATA(csUpper, csLower));
		}
		csText = csLowRemain;
	}

	if ( !csText.IsEmpty() )
	{
		raFraction.Add(DGN_FRACTION_DATA(csText, _T("")));
	}
}

void CDgnConverterBase::CheckFractionDenominator(CString& rcsLower, CString& rcsRemain)
{
	auto L_IsSqrtSymbolOnly = [ ] (const CString& csText) -> bool
	{
		bool bSqr2 = csText.CompareNoCase(_T("sqrt$")) == 0;
		bool bSqr3 = csText.CompareNoCase(_T("rtn3$")) == 0;
		return ( bSqr2 || bSqr3 );
	};
	if ( !L_IsSqrtSymbolOnly(rcsLower) ) return;

	CStringArray aString;
	DivideTextBySpaceAndMiddleBrace(aString, rcsRemain);

	long nString = aString.GetCount();
	if ( nString < 1 ) { ASSERT(0); return; }

	rcsLower += _T(" ") + aString[0];
	rcsRemain = _T(" ");
	for ( long i = 1; i < nString; ++i )
	{
		rcsRemain += aString[i];
		if ( i < nString - 1 )
		{
			rcsRemain += _T(" ");
		}
	}
}

bool CDgnConverterBase::IsFraction(const CString& csText)
{
	return ( csText.Find(_T("over$")) > -1 ) ? true : false;
}

bool CDgnConverterBase::IsFraction(const DGN_FRACTION_DATA& txtFract)
{
	if ( txtFract.csUpper.IsEmpty() ) return false;
	if ( txtFract.csLower.IsEmpty() ) return false;
	return true;
}

CString CDgnConverterBase::GetLongerText(const DGN_FRACTION_DATA& txtFract)
{
	if ( txtFract.csUpper.GetLength() > txtFract.csLower.GetLength() )
	{
		return txtFract.csUpper;
	}
	else
	{
		return txtFract.csLower;
	}
};