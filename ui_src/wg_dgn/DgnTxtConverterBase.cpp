#include "StdAfx.h"
#include <unordered_map>
#include "DgnTxtConverterBase.h"
#include "..\mit_lib\rptostream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_TEXT_OUTLINE_1 1
#define D_TEXT_OUTLINE_2 2
#define D_TEXT_OUTLINE_3 3
#define D_TEXT_OUTLINE_4 4

#define TEXT_ALIGNMENT_LEFT  0
#define TEXT_ALIGNMENT_CENT  1
#define TEXT_ALIGNMENT_RIGHT 2

#define TEXT_TABLE_WIDTH  100
#define DEFAULT_EQUAL_POSI_STA 20
#define DEFAULT_EQUAL_POSI_END 50

typedef CDgnConverterBase ConverterBase;

CDgnTxtConverterBase::CDgnTxtConverterBase(const CString& csFileName) : CDgnConverterBase()
{
	m_bAlignByEqual1st = true;
	m_bAlignByEqualEnd = true;
	m_csFileName = csFileName;
	m_nPageCount = 1;
	m_nLineCount = 1;
	m_PageBreak.Init();
	MakeASCIImap();

	m_nOutline1 = 1;
	m_nOutline2 = 1;
	m_nOutline3 = 1;
	m_nOutline4 = 1;

	m_vTabPos.clear();
	m_vTabPos.push_back(30);
	m_vTabPos.push_back(40);
	m_vTabPos.push_back(50);
	m_vTabPos.push_back(55);
}

CDgnTxtConverterBase::~CDgnTxtConverterBase()
{
}

void CDgnTxtConverterBase::WriteBlank(int nRows)
{
	for ( int i = 0; i < nRows; ++i )
	{
		WriteToFile(CString());
	}
	m_nLineCount += nRows;
}

void CDgnTxtConverterBase::WriteToFile(const CString& csText)
{
	if ( !m_csFileName.IsEmpty() )
	{
		std::ofstream fout(m_csFileName, ios::app);
		fout << ConvertToAscii(std::wstring(csText)) <<endl;
	}
}

void CDgnTxtConverterBase::WriteText(const CString& str)
{
	if ( ConverterBase::IsEmptyString(str) )
	{
		WriteToFile(CString());
	}
	else
	{
		WriteToFile(str);
	}
	m_nLineCount++;
}

void CDgnTxtConverterBase::WriteTableRowText(CString& csText)
{
	csText = GetSpace(3) + csText;
	WriteText(csText);
}

UINT CDgnTxtConverterBase::GetTextLength(const CString& csText)
{
	UINT nDoubleText = ConverterBase::GetNumberOfDoubleSpaceText(csText);
	UINT nText = ( UINT ) ( csText.GetLength() );
	return nText + nDoubleText;
}

CString CDgnTxtConverterBase::GetAlignedText(const CString& csText, UINT nTotalLen, UINT nAlignment)
{
	UINT nTextLen = GetTextLength(csText);
	if ( nTextLen >= nTotalLen ) return csText;

	UINT nSpaces = nTotalLen - nTextLen;
	switch ( nAlignment )
	{
	case TEXT_ALIGNMENT_LEFT:
		{
			return csText + GetSpace(nSpaces);
		}
		break;
	case TEXT_ALIGNMENT_CENT:
		{
			UINT nSpaceR = nSpaces / 2;
			UINT nSpaceL = nSpaces - nSpaceR;
			return GetSpace(nSpaceL) + csText + GetSpace(nSpaceR);
		}
		break;
	case TEXT_ALIGNMENT_RIGHT:
		{
			return GetSpace(nSpaces) + csText;
		}
		break;
	}
	ASSERT(0); return csText;
}

void CDgnTxtConverterBase::WriteReferenceCode(const CString& csCode, const bool bBrackets, UINT nOutline)
{
	CString csOutlineNum;
	switch ( nOutline )
	{
	case D_TEXT_OUTLINE_2:
	case D_TEXT_OUTLINE_3:
		{
			csOutlineNum = GetTextToCalcParagraphLength(nOutline);
		}
		break;
	}

	UINT nSpace = GetTextLength(csOutlineNum);
	if ( nSpace < 1 )
	{
		switch ( nOutline )
		{
		case D_TEXT_OUTLINE_2:
			{
				nSpace = 3;
			}
			break;
		case D_TEXT_OUTLINE_3:
			{
				nSpace = 7;
			}
			break;
		}
	}

	CString csText = csCode;
	ConvertTextForFile(csText);

	CString csRefCode;
	if ( bBrackets )
	{
		csRefCode.Format(_T("%s[ %s ]"), GetSpace(nSpace), csText);
	}
	else
	{
		csRefCode.Format(_T("%s%s"), GetSpace(nSpace), csText);
	}

	WriteText(csRefCode);
}

long CDgnTxtConverterBase::GetPara(const DGNDRAW_TEXT_UNIT_LIST& aText, long nIdx)
{
	long nText = aText.GetCount();

	long nIdxStart = nIdx + 1;
	if ( nIdxStart < nText )
	{
		long nParaSum = 0;
		for ( long i = nIdxStart; i < nText; ++i )
		{
			const DGNDRAW_TEXT_UNIT& TextUnit = aText[i];
			if ( IsParaForPageBreak(TextUnit) )
			{
				break;
			}
			long nPara = GetPara(TextUnit);
			nParaSum += nPara;
		}

		if ( nParaSum != 0 )
		{
			return nParaSum + 1;
		}
	}
	return 0;
}

long CDgnTxtConverterBase::GetPara(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	auto L_GetPara = [ ] (const CString& csText) -> long
	{
		if ( csText.Find(_T("over$")) > -1 ) return 3;
		return 1;
	};

	switch ( TextUnit.nType )
	{
	case ParaTypeOutline:
	case ParaTypeBullet:
		{
			return L_GetPara(TextUnit.csText);
		}
		break;
	case ParaTypeBody:
	case ParaTypeComment:
	case ParaTypeRefCode:
		{
			return 1;
		}
		break;
	case ParaTypeTable:
	case ParaTypeTableNS:
	case ParaTypePMCurve:
	case ParaTypePreview:
	case ParaTypeMetaFile:
	case ParaTypeBarCluster:
		{
			return 0;
		}
	}
	ASSERT(0); return 0;
}

bool CDgnTxtConverterBase::IsParaForPageBreak(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	switch ( TextUnit.nType )
	{
	case ParaTypeOutline:
		{
			switch ( TextUnit.nOutline )
			{
			case D_TEXT_OUTLINE_2:
				{
					return true;
				}
				break;
			case D_TEXT_OUTLINE_1:
			case D_TEXT_OUTLINE_3:
			case D_TEXT_OUTLINE_4:
				{
					return false;
				}
				break;
			}
		}
		break;
	case ParaTypeBullet:
	case ParaTypeBody:
	case ParaTypeComment:
	case ParaTypeRefCode:
	case ParaTypeTable:
	case ParaTypeTableNS:
	case ParaTypePMCurve:
	case ParaTypePreview:
	case ParaTypeMetaFile:
	case ParaTypeBarCluster:
		{
			return false;
		}
	}
	ASSERT(0); return false;
}

void CDgnTxtConverterBase::PageBreak(long nPara)
{
	if ( m_nLineCount + nPara > m_PageBreak.nLineCount )
	{
		m_nPageCount++;
		m_nLineCount = 1;

		if ( m_PageBreak.bPageBreak )
		{
			WriteText(_T(""));

			for ( const CString& csStr : m_PageBreak.vPageBreak )
			{
				WriteText(csStr);
			}
		}
	}
}

void CDgnTxtConverterBase::SetPageBreak(const DGN_PAGE_BREAK& PageBreak)
{
	m_PageBreak = PageBreak;
}

void CDgnTxtConverterBase::SetPageCount(int nPage)
{
	m_nPageCount = nPage;
}

long CDgnTxtConverterBase::GetPageCount()
{
	return m_nPageCount;
}

void CDgnTxtConverterBase::SetLineCount(int nLine)
{
	m_nLineCount = nLine;
}

long CDgnTxtConverterBase::GetLineCount()
{
	return m_nLineCount;
}

void CDgnTxtConverterBase::MakeASCIImap()
{
	m_mapASCII.clear();
	m_mapASCII.insert(std::make_pair(_T('•'), "*"));
	m_mapASCII.insert(std::make_pair(_T('×'), "x"));
	m_mapASCII.insert(std::make_pair(_T('ㆍ'), "x"));
	m_mapASCII.insert(std::make_pair(_T('·'), "."));
	m_mapASCII.insert(std::make_pair(_T('÷'), "/"));
	m_mapASCII.insert(std::make_pair(_T('°'), " deg"));
	m_mapASCII.insert(std::make_pair(_T('→'), "--->"));
	m_mapASCII.insert(std::make_pair(_T('≥'), ">"));
	m_mapASCII.insert(std::make_pair(_T('≤'), "<"));

	m_mapASCII.insert(std::make_pair(_T('²'), "^2"));
	m_mapASCII.insert(std::make_pair(_T('³'), "^3"));
	m_mapASCII.insert(std::make_pair(_T('½'), "1/2"));
	m_mapASCII.insert(std::make_pair(_T('⅓'), "1/3"));
	m_mapASCII.insert(std::make_pair(_T('¼'), "1/4"));
	m_mapASCII.insert(std::make_pair(_T('⅔'), "2/3"));
	m_mapASCII.insert(std::make_pair(_T('¾'), "3/4"));
	m_mapASCII.insert(std::make_pair(_T('⅕'), "1/5"));
	m_mapASCII.insert(std::make_pair(_T('⅖'), "2/5"));
	m_mapASCII.insert(std::make_pair(_T('⅗'), "3/5"));
	m_mapASCII.insert(std::make_pair(_T('⅘'), "4/5"));
	m_mapASCII.insert(std::make_pair(_T('⅛'), "1/8"));
	m_mapASCII.insert(std::make_pair(_T('⅜'), "3/8"));
	m_mapASCII.insert(std::make_pair(_T('⅝'), "5/8"));
	m_mapASCII.insert(std::make_pair(_T('⅞'), "7/8"));

	m_mapASCII.insert(std::make_pair(_T('α'), "alpha"));
	m_mapASCII.insert(std::make_pair(_T('β'), "beta"));
	m_mapASCII.insert(std::make_pair(_T('γ'), "gamma"));
	m_mapASCII.insert(std::make_pair(_T('δ'), "delta"));
	m_mapASCII.insert(std::make_pair(_T('κ'), "kappa"));
	m_mapASCII.insert(std::make_pair(_T('π'), "pi"));
	m_mapASCII.insert(std::make_pair(_T('θ'), "theta"));
	m_mapASCII.insert(std::make_pair(_T('ρ'), "rho"));
	m_mapASCII.insert(std::make_pair(_T('σ'), "sigma"));
	m_mapASCII.insert(std::make_pair(_T('ω'), "omega"));
	m_mapASCII.insert(std::make_pair(_T('ξ'), "xi"));
	m_mapASCII.insert(std::make_pair(_T('ε'), "epsilon"));
	m_mapASCII.insert(std::make_pair(_T('ζ'), "zeta"));
	m_mapASCII.insert(std::make_pair(_T('ψ'), "psi"));
	m_mapASCII.insert(std::make_pair(_T('η'), "eta"));
	m_mapASCII.insert(std::make_pair(_T('ø'), "phi"));
	m_mapASCII.insert(std::make_pair(_T('λ'), "lambda"));
	m_mapASCII.insert(std::make_pair(_T('ν'), "nu"));
	m_mapASCII.insert(std::make_pair(_T('μ'), "mu"));
	m_mapASCII.insert(std::make_pair(_T('υ'), "upsilon"));
	m_mapASCII.insert(std::make_pair(_T('τ'), "tau"));
	m_mapASCII.insert(std::make_pair(_T('χ'), "chi"));

	m_mapASCII.insert(std::make_pair(_T('∑'), "SUM"));
	m_mapASCII.insert(std::make_pair(_T('Α'), "ALPHA"));
	m_mapASCII.insert(std::make_pair(_T('Β'), "BETA"));
	m_mapASCII.insert(std::make_pair(_T('Γ'), "GAMMA"));
	m_mapASCII.insert(std::make_pair(_T('Δ'), "DELTA"));
	m_mapASCII.insert(std::make_pair(_T('Π'), "PI"));
	m_mapASCII.insert(std::make_pair(_T('Θ'), "THETA"));
	m_mapASCII.insert(std::make_pair(_T('Ρ'), "RHO"));
	m_mapASCII.insert(std::make_pair(_T('Ω'), "OMEGA"));
	m_mapASCII.insert(std::make_pair(_T('Ξ'), "XI"));
	m_mapASCII.insert(std::make_pair(_T('Ε'), "EPSILON"));
	m_mapASCII.insert(std::make_pair(_T('Ζ'), "ZETA"));
	m_mapASCII.insert(std::make_pair(_T('Ψ'), "PSI"));
	m_mapASCII.insert(std::make_pair(_T('Η'), "ETA"));
	m_mapASCII.insert(std::make_pair(_T('Φ'), "PHI"));
	m_mapASCII.insert(std::make_pair(_T('Λ'), "LAMBDA"));
	m_mapASCII.insert(std::make_pair(_T('Ν'), "NU"));
	m_mapASCII.insert(std::make_pair(_T('Μ'), "MU"));
	m_mapASCII.insert(std::make_pair(_T('Υ'), "UPSILON"));
	m_mapASCII.insert(std::make_pair(_T('Τ'), "TAU"));
}

std::string CDgnTxtConverterBase::ConvertToAscii(const std::wstring& wStr)
{
	std::string wRes;
	for ( wchar_t ch : wStr )
	{
		if ( ch < 128 )
		{
			wRes += static_cast< char >(ch);
		}
		else if ( m_mapASCII.count(ch) )
		{
			wRes += m_mapASCII[ch];
		}
		else
		{
			static bool bAssert = true;
			if ( bAssert )
			{
				ASSERT(0);
				bAssert = false;
			}
			wRes += ch;
		}
	}
	return wRes;
}

void CDgnTxtConverterBase::WriteOutline1(const CString& str)
{
	if ( !str.IsEmpty() )
	{
		const CString csLine = GetSpace(90, _T("#"));
		WriteText(csLine);
		WriteText(csLine);

		CString csText = str;
		ConvertTextForFile(csText);

		csText = GetAlignedText(csText, 90, TEXT_ALIGNMENT_CENT);
		csText.MakeUpper();
		WriteText(csText);

		WriteText(csLine);
		WriteText(csLine);
	}
	m_nOutline2 = 1;
	m_nOutline3 = 1;
	m_nOutline4 = 1;
}

void CDgnTxtConverterBase::WriteOutline2(const CString& str)
{
	WriteBlank(2);

	const CString csLine = GetSpace(90, _T("="));
	WriteText(csLine);

	CString csText = str;
	ConvertTextForFile(csText, GetTextToCalcParagraphLength(D_TEXT_OUTLINE_2));
	CString csOutlineText = GetTextWithOutline(csText, D_TEXT_OUTLINE_2, m_nOutline2++);
	WriteText(csOutlineText.MakeUpper());

	WriteText(csLine);
	m_nOutline3 = 1;
	m_nOutline4 = 1;
}

void CDgnTxtConverterBase::WriteOutline3(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	WriteBlank();

	CString csText = TextUnit.csText;
	ConvertTextForFile(csText, GetTextToCalcParagraphLength(D_TEXT_OUTLINE_3));
	CString csOutlineText = GetTextWithOutline(csText, D_TEXT_OUTLINE_3, m_nOutline3++);
	AddSymbolForCheckItem(csOutlineText, TextUnit);
	WriteText(csOutlineText);
	m_nOutline4 = 1;
}

void CDgnTxtConverterBase::WriteOutline4(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	CString csText = TextUnit.csText;
	if ( ConverterBase::IsFraction(csText) )
	{
		DGN_FRACTION_DATA_LIST aFraction;
		ConverterBase::DivideTextForFraction(aFraction, csText);
		ConvertFractionText(aFraction, GetTextToCalcParagraphLength(D_TEXT_OUTLINE_4));
		WriteFractionToFile(aFraction, TextUnit, D_TEXT_OUTLINE_4, m_nOutline4++);
	}
	else
	{
		ConvertTextForFile(csText, GetTextToCalcParagraphLength(D_TEXT_OUTLINE_4));
		CString csOutlineText = GetTextWithOutline(csText, D_TEXT_OUTLINE_4, m_nOutline4++);
		AddSymbolForCheckItem(csOutlineText, TextUnit);

		csOutlineText = GetAsciiText(csOutlineText);

		D_EQUAL_POS_INFO pos1st;
		if ( GetEqualPosInfo1st(pos1st, csOutlineText) )
		{
			csOutlineText.Insert(pos1st.nPosi, GetSpace(pos1st.nSpac));
		}
		D_EQUAL_POS_INFO posEnd;
		if ( GetEqualPosInfoEnd(posEnd, csOutlineText) )
		{
			csOutlineText.Insert(posEnd.nPosi, GetSpace(posEnd.nSpac));
		}
		WriteText(csOutlineText);
	}
}

bool CDgnTxtConverterBase::GetEqualPosInfo1st(D_EQUAL_POS_INFO& rData, const CString& csText)
{
	rData.Init();
	if ( !m_bAlignByEqual1st ) return false;

	int nPosiSta = csText.Find(_T("="));
	if ( nPosiSta < 0 ) return false;

	if ( nPosiSta < DEFAULT_EQUAL_POSI_STA )
	{
		rData.nPosi = nPosiSta;
		rData.nSpac = DEFAULT_EQUAL_POSI_STA - nPosiSta - 1;
		return ( rData.nSpac > 0 ) ? true : false;
	}
	return false;
}

bool CDgnTxtConverterBase::GetEqualPosInfoEnd(D_EQUAL_POS_INFO& rData, const CString& csText)
{
	rData.Init();
	if ( !m_bAlignByEqualEnd ) return false;

	auto L_GetEqualCount = [ ] (const CString& csText) -> int
	{
		int nCount = 0;
		int nPosi = 0;
		while ( ( nPosi = csText.Find(_T("="), nPosi) ) >= 0 )
		{
			nCount++;
			nPosi++;
		}
		return nCount;
	};
	if ( L_GetEqualCount(csText) < 2 ) return false;

	int nPosiEnd = csText.ReverseFind('=');
	if ( nPosiEnd < DEFAULT_EQUAL_POSI_END )
	{
		rData.nPosi = nPosiEnd;
		rData.nSpac = DEFAULT_EQUAL_POSI_END - nPosiEnd - 1;
		return ( rData.nSpac > 0 ) ? true : false;
	}
	return false;
}

void CDgnTxtConverterBase::AddSymbolForCheckItem(CString& rcsText, const DGNDRAW_TEXT_UNIT& TextUnit)
{
	if ( TextUnit.TextStyle.TextColor != RGB(0, 0, 0) )
	{
		auto L_IsRedText = [ ] (const DGNDRAW_TEXT_UNIT& TextUnit) -> bool
		{
			if ( TextUnit.TextStyle.TextColor != RGB(255, 0, 0) ) return false;
			if ( TextUnit.csText.Find(_T("N.G")) < 0 ) return false;
			return true;
		};

		if ( L_IsRedText(TextUnit) )
		{
			rcsText += _T("  **********");
		}
		else
		{
			rcsText += _T("  *****");
		}
	}
}

CString CDgnTxtConverterBase::GetAsciiText(const CString& csText)
{
	std::string stdStr = ConvertToAscii(std::wstring(csText));
	return CString(stdStr.c_str());
}

CString CDgnTxtConverterBase::GetTextToCalcParagraphLength(UINT nOutlineLevel)
{
	switch ( nOutlineLevel )
	{
	case D_TEXT_OUTLINE_1:
		{
			return GetTextWithOutline(CString(), D_TEXT_OUTLINE_1, m_nOutline1);
		}
		break;
	case D_TEXT_OUTLINE_2:
		{
			return GetTextWithOutline(CString(), D_TEXT_OUTLINE_2, m_nOutline2);
		}
		break;
	case D_TEXT_OUTLINE_3:
		{
			return GetTextWithOutline(CString(), D_TEXT_OUTLINE_3, m_nOutline3);
		}
		break;
	case D_TEXT_OUTLINE_4:
		{
			return GetTextWithOutline(CString(), D_TEXT_OUTLINE_4, m_nOutline4);
		}
		break;
	}
	return CString();
}

CString CDgnTxtConverterBase::GetTextWithOutline(const CString& csText, UINT nOutlineLevel, UINT nOutlineNo)
{
	auto L_GetFormatString = [ ] (UINT nOutlineLevel) -> CString
	{
		switch ( nOutlineLevel )
		{
		case D_TEXT_OUTLINE_1:
		case D_TEXT_OUTLINE_2:
			{
				return _T("%s%s%s");
			}
			break;
		case D_TEXT_OUTLINE_3:
		case D_TEXT_OUTLINE_4:
			{
				return _T("%s%s %s");
			}
			break;
		}
		ASSERT(0); return _T("%s%s %s");
	};
	CString csNumber = GetOutlineNumber(nOutlineLevel, nOutlineNo);
	CString csBlank  = GetBlankByLevel(nOutlineLevel);
	CString csFormat = L_GetFormatString(nOutlineLevel);
	return ConverterBase::GetText(csFormat, csBlank, csNumber, csText);
}

CString CDgnTxtConverterBase::GetOutlineNumber(UINT nOutlineLevel, UINT nOutlineNo)
{
	switch ( nOutlineLevel )
	{
	case D_TEXT_OUTLINE_1:
		{
			return _T("▣");
		}
		break;
	case D_TEXT_OUTLINE_2:
		{
			return ConverterBase::GetText(_T("   [[[*]]]   "));
		}
		break;
	case D_TEXT_OUTLINE_3:
		{
			return ConverterBase::GetText(_T("( )."));
		}
		break;
	case D_TEXT_OUTLINE_4:
		{
			return _T("-.");
		}
		break;
	}
	ASSERT(0); return CString();
}

CString CDgnTxtConverterBase::GetBlankByLevel(UINT nOutlineLevel)
{
	switch ( nOutlineLevel )
	{
	case D_TEXT_OUTLINE_1:
	case D_TEXT_OUTLINE_2:
		{
			return CString();
		}
		break;
	case D_TEXT_OUTLINE_3:
		{
			return GetSpace(6);
		}
		break;
	case D_TEXT_OUTLINE_4:
		{
			return GetSpace(11);
		}
		break;
	}
	ASSERT(0); return CString();
}

void CDgnTxtConverterBase::ConvertTextForFile(CString& rcsText, const CString& csOutlineText)
{
	ConverterBase::AddSpaceBeforeAndAfterBrace(rcsText);

	if ( rcsText.Find(_T("over$")) > -1 )
	{
		ConvertFraction(rcsText);
	}
	if ( rcsText.Find(_T("sqrt$")) > -1 )
	{
		ConvertSquareRoot(rcsText, _T("sqrt$"));
	}
	if ( rcsText.Find(_T("rtn3$")) > -1 )
	{
		ConvertSquareRoot(rcsText, _T("rtn3$"));
	}
	if ( rcsText.Find(_T("$")) > -1 )
	{
		ConvertSubscript(rcsText);
		ConvertSuperscript(rcsText);
	}

	rcsText.Replace(_T("$"), _T(""));
	ConverterBase::RemoveMultipleSpaces(rcsText);
	rcsText.Replace(_T(" → "), _T("  →  "));
	if ( rcsText.Find(_T("\t")) > -1 )
	{
		RemoveTabPositionNum(rcsText, csOutlineText);
	}

	if ( rcsText != "\n" )
	{
		rcsText.Trim();
	}
}

void CDgnTxtConverterBase::RemoveTabPositionNum(CString& rcsText, const CString& csOutlineText)
{
	CStringArray aText;
	ConverterBase::DivideText(aText, rcsText, _T("\t"));

	rcsText.Empty();
	UINT nOutlineTextLen = GetTextLength(csOutlineText);

	long nText = aText.GetCount();
	for ( long i = 0; i < nText; ++i )
	{
		CString& csTemp = aText[i];

		if ( i + 1 < nText )
		{
			int nPosBlank = csTemp.ReverseFind(' ');
			if ( nPosBlank > -1 )
			{
				csTemp = csTemp.Left(nPosBlank + 1);
			}
			else
			{
				int nPosComma = csTemp.ReverseFind(_T(','));
				csTemp = csTemp.Left(nPosComma + 1);
			}

			if ( i < m_vTabPos.size() )
			{
				UINT nTabPos = m_vTabPos[i];
				UINT nTxtLen = max(0, nTabPos - nOutlineTextLen);
				csTemp = GetAlignedText(csTemp, nTxtLen, TEXT_ALIGNMENT_LEFT);
			}
		}

		rcsText += csTemp;
	}
}

void CDgnTxtConverterBase::ConvertFraction(CString& rcsText)
{
	DGN_FRACTION_DATA_LIST aFraction;
	ConverterBase::DivideTextForFraction(aFraction, rcsText);
	long nFraction = aFraction.GetCount();
	if ( nFraction < 1 ) return;

	auto L_IsBraceReq = [ ] (const CString& csText) -> bool
	{
		if ( csText.IsEmpty() ) return false;
		if ( csText.Find(_T(' ')) < 0 ) return false;
		return true;
	};
	auto L_AddBrace = [ ] (CString& rcsText)
	{
		rcsText = _T("(") + rcsText.Trim() + _T(")");
	};

	rcsText.Empty();

	for ( long i = 0; i < nFraction; ++i )
	{
		DGN_FRACTION_DATA& txtFract = aFraction[i];
		if ( ConverterBase::IsFraction(txtFract) )
		{
			if ( L_IsBraceReq(txtFract.csUpper) )
			{
				L_AddBrace(txtFract.csUpper);
			}
			if ( L_IsBraceReq(txtFract.csLower) )
			{
				L_AddBrace(txtFract.csLower);
			}
			CString csFract;
			csFract.Format(_T("%s/%s"), txtFract.csUpper, txtFract.csLower);
			rcsText += csFract;
		}
		else
		{
			rcsText += txtFract.csUpper;
		}

		if ( i + 1 < nFraction )
		{
			rcsText += _T(" ");
		}
	}
}

void CDgnTxtConverterBase::ConvertSquareRoot(CString& rcsText, CString csSqrtMark)
{
	int nSqrtMarkLen = csSqrtMark.GetLength();

	UINT nLoop = 0;
	while ( true )
	{
		int nPosSqrt = rcsText.Find(csSqrtMark);
		if ( nPosSqrt < 0 ) break;
		if ( nLoop++ > m_nLoopMax ) { /*ASSERT(0);*/ break; }

		CString csTempL = rcsText.Left(nPosSqrt);
		CString csTempR = rcsText.Mid(nPosSqrt + nSqrtMarkLen);
		csTempR.TrimLeft();

		CString csSquareRoot, csSqrtRemain;
		int nPosBraceL = csTempR.Find(_T("{"));
		int nPosBraceR = csTempR.Find(_T("}"));
		if ( nPosBraceL == -1 && nPosBraceR == -1 )
		{
			int nPosDivider = csTempR.FindOneOf(_T(" ,"));
			if ( nPosDivider < 0 )
			{
				csSquareRoot = csTempR;
				csSqrtRemain.Empty();
			}
			else
			{
				csSquareRoot = csTempR.Left(nPosDivider);
				csSqrtRemain = csTempR.Mid(nPosDivider);
			}
		}
		else if ( nPosBraceL > -1 && nPosBraceR > -1 )
		{
			csSquareRoot = csTempR.Mid(nPosBraceL + 1, nPosBraceR - 1);
			csSqrtRemain = csTempR.Mid(nPosBraceR + 1);
		}
		else { /*ASSERT(0);*/ break; }

		csSquareRoot.Trim();
		if ( !csSquareRoot.IsEmpty() && csSquareRoot.Find(csSqrtMark) < 0 )
		{
			if ( csSqrtMark == _T("sqrt$") )
			{
				rcsText.Format(_T("%s SQRT( %s ) %s"), csTempL, csSquareRoot, csSqrtRemain);
			}
			else if ( csSqrtMark == _T("rtn3$") )
			{
				rcsText.Format(_T("%s ( %s )^1/3 %s"), csTempL, csSquareRoot, csSqrtRemain);
			}
			else ASSERT(0);
		}
		//else ASSERT(0);
	}
}

void CDgnTxtConverterBase::ConvertSubscript(CString& rcsText)
{
	CString csSubMark = _T('_');
	int nSubMarkLen = csSubMark.GetLength();

	UINT nLoop = 0;
	while ( true )
	{
		int nPosMark = rcsText.Find(csSubMark);
		if ( nPosMark < 0 ) break;
		if ( nLoop++ > m_nLoopMax ) { ASSERT(0); break; }

		CString csTempL = rcsText.Left(nPosMark);
		CString csTempR = rcsText.Mid(nPosMark + nSubMarkLen);

		CString csNormal, csNormalRemain;
		ConverterBase::GetScriptNormalText(csTempL, csNormal, csNormalRemain);

		CString csSubscript, csLowerRemain;
		ConverterBase::GetScriptText(csTempR, csSubscript, csLowerRemain);

		if ( csNormalRemain.IsEmpty() )
		{
			rcsText.Format(_T("%s%s %s"), csNormal, csSubscript, csLowerRemain);
		}
		else
		{
			rcsText.Format(_T("%s %s%s %s"), csNormalRemain, csNormal, csSubscript, csLowerRemain);
		}
	}
}

void CDgnTxtConverterBase::ConvertSuperscript(CString& rcsText)
{
	CString csSupMark = _T('^');
	int nSupMarkLen = csSupMark.GetLength();

	UINT nLoop = 0;
	while ( true )
	{
		int nPosMark = rcsText.Find(csSupMark);
		if ( nPosMark < 0 ) break;
		if ( nLoop++ > m_nLoopMax ) { ASSERT(0); break; }

		CString csTempL = rcsText.Left(nPosMark);
		CString csTempR = rcsText.Mid(nPosMark + nSupMarkLen);

		CString csNormal, csNormalRemain;
		ConverterBase::GetScriptNormalText(csTempL, csNormal, csNormalRemain);

		CString csSupscript, csSupscriptRemain;
		ConverterBase::GetScriptText(csTempR, csSupscript, csSupscriptRemain);

		if ( csNormalRemain.IsEmpty() )
		{
			rcsText.Format(_T("%s!@#$%s %s"), csNormal, csSupscript, csSupscriptRemain);
		}
		else
		{
			rcsText.Format(_T("%s %s!@#$%s %s"), csNormalRemain, csNormal, csSupscript, csSupscriptRemain);
		}
	}

	rcsText.Replace(_T("!@#$"), _T("^"));
}

void CDgnTxtConverterBase::ConvertFractionText(DGN_FRACTION_DATA_LIST& raFraction, CString csOutlineText)
{
	long nFraction = raFraction.GetCount();
	for ( long i = 0; i < nFraction; ++i )
	{
		DGN_FRACTION_DATA& txtFract = raFraction[i];

		if ( !txtFract.csUpper.IsEmpty() )
		{
			ConvertTextForFile(txtFract.csUpper, csOutlineText);
		}
		if ( !txtFract.csLower.IsEmpty() )
		{
			ConvertTextForFile(txtFract.csLower);
		}
		csOutlineText += ConverterBase::GetLongerText(txtFract);

		if ( ( i < nFraction - 1 ) && ConverterBase::IsFraction(raFraction[i + 1]) )
		{
			if ( !ConverterBase::IsFraction(txtFract) && txtFract.csUpper[txtFract.csUpper.GetLength() - 1] != _T(' ') )
			{
				txtFract.csUpper += _T(" ");
			}
		}
		if ( ( i > 0 ) && ConverterBase::IsFraction(raFraction[i - 1]) )
		{
			if ( !ConverterBase::IsFraction(txtFract) && txtFract.csUpper[0] != _T(' ') )
			{
				txtFract.csUpper.Insert(0, _T(" "));
			}
		}
	}
}

void CDgnTxtConverterBase::WriteFractionToFile(const DGN_FRACTION_DATA_LIST& aFraction, const DGNDRAW_TEXT_UNIT& TextUnit, UINT nOutlineLevel, UINT nOutlineNo)
{
	CString csUpper, csCentr, csLower;
	if ( !GetTextForFraction(aFraction, csUpper, csCentr, csLower) ) { ASSERT(0); return; }

	CString csOutlineNum = GetTextWithOutline(_T(""), nOutlineLevel, nOutlineNo);
	UINT nOutlineNum = GetTextLength(csOutlineNum);
	csUpper = GetSpace(nOutlineNum) + csUpper;
	csCentr = csOutlineNum + csCentr;
	csLower = GetSpace(nOutlineNum) + csLower;
	AddSymbolForCheckItem(csCentr, TextUnit);

	D_EQUAL_POS_INFO pos1st;
	if ( GetEqualPosInfo1st(pos1st, csCentr) )
	{
		csUpper.Insert(pos1st.nPosi, GetSpace(pos1st.nSpac));
		csCentr.Insert(pos1st.nPosi, GetSpace(pos1st.nSpac));
		csLower.Insert(pos1st.nPosi, GetSpace(pos1st.nSpac));
	}
	D_EQUAL_POS_INFO posEnd;
	if ( GetEqualPosInfoEnd(posEnd, csCentr) )
	{
		csUpper.Insert(posEnd.nPosi, GetSpace(posEnd.nSpac));
		csCentr.Insert(posEnd.nPosi, GetSpace(posEnd.nSpac));
		csLower.Insert(posEnd.nPosi, GetSpace(posEnd.nSpac));
	}
	WriteText(csUpper);
	WriteText(csCentr);
	WriteText(csLower);
}

bool CDgnTxtConverterBase::AddBraceForFraction()
{
	return false;
}

bool CDgnTxtConverterBase::GetTextForFraction(const DGN_FRACTION_DATA_LIST& aFraction, CString& rcsUpper, CString& rcsCentr, CString& rcsLower)
{
	long nFraction = aFraction.GetCount();
	if ( nFraction < 1 ) { ASSERT(0); return false; }

	for ( long i = 0; i < nFraction; ++i )
	{
		const DGN_FRACTION_DATA& txtFract = aFraction[i];
		const CString csUpper = GetAsciiText(txtFract.csUpper);
		const CString csLower = GetAsciiText(txtFract.csLower);

		if ( ConverterBase::IsFraction(txtFract) )
		{
			UINT nUppLen = GetTextLength(csUpper);
			UINT nLowLen = GetTextLength(csLower);
			UINT nTxtLen = max(nUppLen, nLowLen) + 2;

			bool bAddBrace = AddBraceForFraction();
			CString csBraceL = bAddBrace ? _T("[") : _T("");
			CString csBraceR = bAddBrace ? _T("]") : _T("");
			rcsUpper += csBraceL + GetAlignedText(csUpper, nTxtLen, TEXT_ALIGNMENT_CENT) + csBraceR;
			rcsCentr += csBraceL + GetSpace(nTxtLen, _T("-")) + csBraceR;
			rcsLower += csBraceL + GetAlignedText(csLower, nTxtLen, TEXT_ALIGNMENT_CENT) + csBraceR;

			if ( ( i < nFraction - 1 ) && ConverterBase::IsFraction(aFraction[i + 1]) )
			{
				rcsUpper += _T(" ");
				rcsCentr += _T(" ");
				rcsLower += _T(" ");
			}
		}
		else
		{
			UINT nTextLen = GetTextLength(csUpper);
			if ( nTextLen < 1 ) { ASSERT(0); continue; }

			rcsUpper += GetSpace(nTextLen);
			rcsCentr += csUpper;
			rcsLower += GetSpace(nTextLen);
		}
	}

	if ( rcsUpper.IsEmpty() ) return false;
	if ( rcsCentr.IsEmpty() ) return false;
	if ( rcsLower.IsEmpty() ) return false;
	return true;
}

void CDgnTxtConverterBase::WriteTable(const DGNDRAW_TABLE& DrawTable, bool bSizeByText)
{
	D_TEXT_TABLE TextTable;
	ConvertTable(TextTable, DrawTable);

	CArray<UINT, UINT> aColumnSize;
	if ( bSizeByText )
	{
		GetTableColumnSizeByText(aColumnSize, TextTable);
	}
	else
	{
		GetTableColumnSizeByQuick(aColumnSize, DrawTable);
	}

	long nRow = TextTable.aRow.GetCount();
	for ( long i = 0; i < nRow; ++i )
	{
		const D_TEXT_TABLE_ROW& Row = TextTable.aRow[i];
		WriteTableLine(aColumnSize, ( i == 0 ? true : false ));
		WriteTableRow(Row, aColumnSize);
	}
	WriteTableLine(aColumnSize, true);
}

void CDgnTxtConverterBase::ConvertTable(D_TEXT_TABLE& rTextTable, const DGNDRAW_TABLE& DrawTable)
{
	rTextTable.Init();

	UINT nRow = static_cast< UINT >( DrawTable.Size.nRow );
	UINT nCol = static_cast< UINT >( DrawTable.Size.nCol );
	rTextTable.SetTableSize(nRow, nCol);
	for ( UINT i = 0; i < nRow; ++i )
	{
		D_TEXT_TABLE_ROW& Row = rTextTable.aRow[i];
		for ( UINT k = 0; k < nCol; ++k )
		{
			D_TEXT_TABLE_CELL& Cell = Row.aCol[k];
			ConvertTableCell(Cell, DrawTable.aRow[i].aCol[k]);
		}
	}
}

void CDgnTxtConverterBase::ConvertTableCell(D_TEXT_TABLE_CELL& rTextCell, const DGNDRAW_TABLE_CELL& DrawCell)
{
	rTextCell.Init();

	CString csDrawText = DrawCell.csContent;
	ConverterBase::DivideText(rTextCell.aText, csDrawText, _T("\n"));

	UINT nMaxWidth = 0;
	long nText = rTextCell.aText.GetCount();
	for ( long i = 0; i < nText; ++i )
	{
		CString& csCellText = rTextCell.aText[i];
		ConvertTextForFile(csCellText);

		UINT nTextLen = GetTextLength(csCellText);
		nMaxWidth = max(nMaxWidth, nTextLen);
	}

	rTextCell.nLine  = static_cast< UINT >(nText);
	rTextCell.nWidth = nMaxWidth;
	rTextCell.nAlign = DrawCell.Style.Para.nAlignment;
}

void CDgnTxtConverterBase::AddAdditionalSpaceToFitSize(CArray<UINT, UINT>& raColumnSize)
{
	long nColSize = raColumnSize.GetCount();
	if ( nColSize < 1 ) { ASSERT(0); return; }

	UINT nCellWidthSum = 0;
	for ( long i = 0; i < nColSize; ++i )
	{
		nCellWidthSum += raColumnSize[i];
	}

	UINT nTotalWidth = TEXT_TABLE_WIDTH - ( nColSize + 1 );
	if ( nCellWidthSum >= nTotalWidth ) return;

	UINT nAddWidth = ( nTotalWidth - nCellWidthSum ) / nColSize;
	for ( long i = 0; i < nColSize; ++i )
	{
		raColumnSize[i] += nAddWidth;
	}

	UINT nAddWidth2 = ( nTotalWidth - nCellWidthSum ) % nColSize;
	for ( long i = 0; i < nAddWidth2; ++i )
	{
		raColumnSize[i] += 1;
	}
}

void CDgnTxtConverterBase::GetTableColumnSizeByText(CArray<UINT, UINT>& raColumnSize, const D_TEXT_TABLE& TextTable)
{
	long nRow = TextTable.aRow.GetCount();
	if ( nRow < 1 ) { ASSERT(0); return; }

	long nColumn = TextTable.aRow[0].aCol.GetCount();
	raColumnSize.SetSize(nColumn);
	for ( long i = 0; i < nRow; ++i )
	{
		const D_TEXT_TABLE_ROW& Row = TextTable.aRow[i];
		for ( long k = 0; k < nColumn; ++k )
		{
			UINT& nColumnSize = raColumnSize[k];
			nColumnSize = max(nColumnSize, Row.aCol[k].nWidth);
		}
	}

	AddAdditionalSpaceToFitSize(raColumnSize);
}

void CDgnTxtConverterBase::GetTableColumnSizeByQuick(CArray<UINT, UINT>& raColumnSize, const DGNDRAW_TABLE& DrawTable)
{
	raColumnSize.RemoveAll();

	const CArray<double, double>& aColSize = DrawTable.Size.aCol;
	long nColSize = aColSize.GetCount();
	if ( nColSize < 1 ) { ASSERT(0); return; }

	raColumnSize.SetSize(nColSize);

	UINT nCellWidthSum = 0;
	for ( long i = 0; i < nColSize; ++i )
	{
		double dCellWidth = aColSize[i] * TEXT_TABLE_WIDTH / 1500.0;
		UINT nCellWidth = static_cast< UINT >(dCellWidth);

		raColumnSize[i] = nCellWidth;
		nCellWidthSum += nCellWidth;
	}

	AddAdditionalSpaceToFitSize(raColumnSize);
}

void CDgnTxtConverterBase::GetTableColumnSizeByDataTable(CArray<UINT, UINT>& raColumnSize, const DGNDRAW_TABLE& HeadTable, const DGNDRAW_TABLE& DataTable)
{
	raColumnSize.RemoveAll();

	const CArray<double, double>& aHeadColSize = HeadTable.Size.aCol;
	long nHeadColSize = aHeadColSize.GetCount();
	if ( nHeadColSize < 1 ) { ASSERT(0); return; }

	raColumnSize.SetSize(nHeadColSize);

	auto L_FindEquivalentWidthIndex = [ ] (const double& dCellWidth, const DGNDRAW_TABLE& DrawTable, long nStartIndex) -> long
	{
		const CArray<double, double>& aColSize = DrawTable.Size.aCol;
		long nColSize = aColSize.GetCount();
		if ( nStartIndex + 1 > nColSize ) { ASSERT(0); return nColSize - 1; }

		double dCellWidthSum = 0.0;
		for ( long i = nStartIndex; i < nColSize; ++i )
		{
			dCellWidthSum += aColSize[i];
			if ( fabs(dCellWidthSum - dCellWidth) < 1.0 )
			{
				return i;
			}
		}
		ASSERT(0); return nColSize - 1;
	};
	auto L_GetCellWidth = [ ] (const CArray<UINT, UINT>& aColSize, long nIndexSt, long nIndexEd) -> UINT
	{
		long nColSize = aColSize.GetCount();
		if ( nIndexSt + 1 > nColSize ) { ASSERT(0); return 0; }
		if ( nIndexEd + 1 > nColSize ) { ASSERT(0); return 0; }
		if ( nIndexSt > nIndexEd ) { ASSERT(0); return 0; }

		UINT nCellWidthSum = 0;
		for ( long i = nIndexSt; i < nIndexEd; ++i )
		{
			nCellWidthSum += aColSize[i];
		}
		return nCellWidthSum;
	};

	D_TEXT_TABLE TextDataTable;
	ConvertTable(TextDataTable, DataTable);

	CArray<UINT, UINT> aColSizeData;
	GetTableColumnSizeByText(aColSizeData, TextDataTable);

	long nIndexSt = 0;
	for ( long i = 0; i < nHeadColSize; ++i )
	{
		double dHeadCellWidth = aHeadColSize[i];

		long nIndexEd = L_FindEquivalentWidthIndex(dHeadCellWidth, DataTable, nIndexSt);
		UINT nCellWidth = L_GetCellWidth(aColSizeData, nIndexSt, nIndexEd);
		ASSERT(nCellWidth > 0);

		raColumnSize[i] = nCellWidth;
		nIndexSt = nIndexEd + 1;
	}
}

void CDgnTxtConverterBase::WriteTableRow(const D_TEXT_TABLE_ROW& Row, const CArray<UINT, UINT>& aColumnSize)
{
	long nCol = Row.aCol.GetCount();
	if ( nCol < 1 ) { ASSERT(0); return; }

	auto L_GetMaxTextLine = [ ] (const D_TEXT_TABLE_ROW& Row) -> UINT
	{
		UINT nMaxLine = 0;
		long nCol = Row.aCol.GetCount();
		for ( long i = 0; i < nCol; ++i )
		{
			const D_TEXT_TABLE_CELL& Cell = Row.aCol[i];
			nMaxLine = max(nMaxLine, Cell.nLine);
		}
		return nMaxLine;
	};

	UINT nMaxLine = L_GetMaxTextLine(Row);
	for ( UINT i = 0; i < nMaxLine; ++i )
	{
		CString csRowText = _T("|");
		for ( long k = 0; k < nCol; ++k )
		{
			const D_TEXT_TABLE_CELL& Cell = Row.aCol[k];
			UINT nWidth = aColumnSize[k];
			UINT nAlign = Cell.nAlign;

			CString csCellText;
			if ( i < ( UINT ) ( Cell.aText.GetCount() ) )
			{
				csCellText = GetAlignedText(Cell.aText[i], nWidth, nAlign);
			}
			else
			{
				csCellText = GetSpace(nWidth);
			}
			csRowText += csCellText + _T("|");
		}

		WriteTableRowText(csRowText);
	}
}

void CDgnTxtConverterBase::WriteTableLine(const CArray<UINT, UINT>& aColumnSize, bool bDoubleLine)
{
	long nCol = aColumnSize.GetCount();
	if ( nCol < 1 ) { ASSERT(0); return; }

	CString csLine = bDoubleLine ? _T("=") : _T("+");
	for ( long i = 0; i < nCol; ++i )
	{
		if ( bDoubleLine )
		{
			CString csTemp = GetSpace(aColumnSize[i], _T("="));
			csLine += csTemp + _T("=");
		}
		else
		{
			CString csTemp = GetSpace(aColumnSize[i], _T("-"));
			csLine += csTemp + _T("+");
		}
	}

	WriteTableRowText(csLine);
}