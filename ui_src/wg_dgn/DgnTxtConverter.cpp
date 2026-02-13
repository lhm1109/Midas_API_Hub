#include "StdAfx.h"
#include "DgnTxtConverter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnTxtConverter::CDgnTxtConverter(const CString& csFileName) : CDgnTxtConverterBase(csFileName)
{
	m_pData = nullptr;
	m_pPage = nullptr;
}

CDgnTxtConverter::~CDgnTxtConverter()
{
}

bool CDgnTxtConverter::ConvertToTXT(const DGNDRAW_TEXT_REPORT* pData)
{
	m_pData = pData;
	if ( m_pData == nullptr ) { ASSERT(0); return false; }
	if ( m_pData->aPage.GetCount() < 1 ) { ASSERT(0); return true; }

	long nPage = m_pData->aPage.GetCount();
	for ( long i = 0; i < nPage; ++i )
	{
		const DGNDRAW_TEXT_PAGE& Page = m_pData->aPage[i];
		m_pPage = &const_cast < DGNDRAW_TEXT_PAGE& >(Page);

		if ( i == 0 )
		{
			CDgnTxtConverterBase::PageBreak(3);
		}
		WriteOutline1(Page.csTitle);
		ConvertPage(Page);
	}
	return true;
}

bool CDgnTxtConverter::ConvertPage(const DGNDRAW_TEXT_PAGE& Page)
{
	const bool bMultiZonePage = CDgnConverterBase::IsPageWithMultipleZone(Page);

	const long nPara = Page.aPara.GetCount();
	for ( long i = 0; i < nPara; ++i )
	{
		const DGNDRAW_TEXT_PARA& Para = Page.aPara[i];
		if ( Para.bNewZone )
		{
			CString csTitle = Page.csTitle;
			if ( bMultiZonePage )
			{
				csTitle = GetText(_T("%s [ %s ]"), Page.csTitle, Para.csTitle);
			}
			WriteOutline1(csTitle);
		}
		else
		{
			WriteOutline2(Para.csTitle);
		}
		ConvertPara(Para);
	}
	return true;
}

bool CDgnTxtConverter::ConvertPara(const DGNDRAW_TEXT_PARA& Para)
{
	const long nText = Para.aText.GetCount();
	for ( long i = 0; i < nText; ++i )
	{
		DGNDRAW_TEXT_UNIT_LIST aRefCode;
		if ( ( i == 0 ) && GetSuccessiveReferenceCode(aRefCode, Para) )
		{
			// 연속적인 Reference Code : 동일한 개요 레벨로 출력.
			long nRefCode = aRefCode.GetCount();
			for ( long k = 0; k < nRefCode; ++k )
			{
				ConvertReferenceCode(aRefCode[k], 2);
			}
			i += nRefCode - 1;
			continue;
		}

		long nPara = CDgnTxtConverterBase::GetPara(Para.aText, i);
		if ( nPara )
		{
			CDgnTxtConverterBase::PageBreak(nPara);
		}

		const DGNDRAW_TEXT_UNIT& TextUnit = Para.aText[i];
		ConvertTextUnit(TextUnit);
	}
	return true;
}

bool CDgnTxtConverter::ConvertTextUnit(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	switch ( TextUnit.nType )
	{
	case ParaTypeOutline:
	case ParaTypeBullet:
		{
			return ConvertOutline(TextUnit);
		}
		break;
	case ParaTypeBody:
	case ParaTypeComment:
		{
			return ConvertText(TextUnit);
		}
		break;
	case ParaTypeTable:
	case ParaTypeTableNS:
		{
			return ConvertTable(TextUnit, true);
		}
		break;
	case ParaTypePMCurve:
		{
			return ConvertPMCurve(TextUnit);
		}
		break;
	case ParaTypePreview:
		{
			return ConvertPreview(TextUnit);
		}
		break;
	case ParaTypeMetaFile:
		{
			return ConvertMetaFile(TextUnit);
		}
		break;
	case ParaTypeRefCode:
		{
			return ConvertReferenceCode(TextUnit, 3);
		}
		break;
	case ParaTypeBarCluster:
		{
			return ConvertCluster(TextUnit);
		}
		break;
	}
	ASSERT(0); return false;
}

bool CDgnTxtConverter::ConvertReferenceCode(const DGNDRAW_TEXT_UNIT& TextUnit, UINT nOutline)
{
	WriteReferenceCode(TextUnit.csText, TextUnit.TextStyle.bBrackets, nOutline);
	return true;
}

bool CDgnTxtConverter::ConvertOutline(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	switch ( TextUnit.nOutline )
	{
	case 2:
		{
			CDgnTxtConverterBase::WriteOutline3(TextUnit);
		}
		break;
	case 3:
		{
			CDgnTxtConverterBase::WriteOutline4(TextUnit);
		}
		break;
	default:
		{
			ASSERT(0);
			CDgnTxtConverterBase::WriteText(TextUnit.csText);
		}
		break;
	}
	return true;
}

bool CDgnTxtConverter::ConvertText(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	CDgnTxtConverterBase::WriteText(TextUnit.csText);
	return true;
}

bool CDgnTxtConverter::ConvertTable(const DGNDRAW_TEXT_UNIT& TextUnit, bool bSizeByText)
{
	const long nTable = m_pPage->aTable.GetCount();
	if ( TextUnit.nTableindex < nTable )
	{
		const DGNDRAW_TABLE& TableD = m_pPage->aTable[TextUnit.nTableindex];
		CDgnTxtConverterBase::WriteTable(TableD, bSizeByText);
		return true;
	}
	ASSERT(0); return false;
}

bool CDgnTxtConverter::ConvertPMCurve(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	return true;
}

bool CDgnTxtConverter::ConvertPreview(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	return true;
}

bool CDgnTxtConverter::ConvertMetaFile(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	return true;
}

bool CDgnTxtConverter::ConvertCluster(const DGNDRAW_TEXT_UNIT& TextUnit)
{
	return true;
}

void CDgnTxtConverter::SetPageBreak(const DGN_PAGE_BREAK& PageBreak)
{
	CDgnTxtConverterBase::SetPageBreak(PageBreak);
}

void CDgnTxtConverter::SetPageCount(int nPage)
{
	CDgnTxtConverterBase::SetPageCount(nPage);
}

long CDgnTxtConverter::GetPageCount()
{
	return CDgnTxtConverterBase::GetPageCount();
}

void CDgnTxtConverter::SetLineCount(int nLine)
{
	CDgnTxtConverterBase::SetLineCount(nLine);
}

long CDgnTxtConverter::GetLineCount()
{
	return CDgnTxtConverterBase::GetLineCount();
}