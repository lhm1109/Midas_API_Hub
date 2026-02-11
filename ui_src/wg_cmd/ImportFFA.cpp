#include "StdAfx.h"
#include "wg_cmd.h"
#include "ImportFFA.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_base\TextFileWithBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CImportFFA::CImportFFA()
{
    m_aSoilProp.RemoveAll();
}

CImportFFA::~CImportFFA()
{
}

void CImportFFA::GetSoilProp(CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA &>& raSoilProp)
{
    raSoilProp.Copy(m_aSoilProp);
}

FFAMsg CImportFFA::ImportFFA(const CString& csFilePath)
{
    CStringArray aAllTextLine;
    if ( !ReadTextFile(csFilePath, aAllTextLine) ) { ASSERT(0); return FFAMsg::EN_READ_FAIL; }

    RemoveBlankComment(aAllTextLine);
    if ( aAllTextLine.IsEmpty() ) { ASSERT(0); return FFAMsg::EN_FILE_EMPTY; }

    T_UNIT_INDEX FileUnit;
    bool bUnitLeng = false, bUnitForc = false;
    if ( !GetUnitInfo(aAllTextLine, FileUnit, bUnitLeng, bUnitForc) )
    {
        ASSERT(0);
        if ( !bUnitLeng ) return FFAMsg::EN_UNIT_LENGTH;
        if ( !bUnitForc ) return FFAMsg::EN_UNIT_FORCE;
    }

    if ( !GetSoilProp(aAllTextLine, m_aSoilProp) ) { ASSERT(0); return FFAMsg::EN_DATA_EMPTY; }

    ConvertUnit(FileUnit, m_aSoilProp);
    return FFAMsg::EN_DATA_VALID;
}

bool CImportFFA::ReadTextFile(const CString& csFilePath, CStringArray &raAllTextLine)
{
    CTextFileWithBuffer TextFile;
    UINT nFlag = CFile::typeText | CFile::modeRead;
    if ( !TextFile.Open(csFilePath, nFlag) ) { ASSERT(0); return false; }

    CString csLine;
    while ( TextFile.ReadLine(csLine) )
    {
        raAllTextLine.Add(csLine);
    }
    TextFile.Close();
    return true;
}

void CImportFFA::RemoveBlankComment(CStringArray& raText)
{
    for (int i = raText.GetCount() - 1; i >= 0; --i )
    {
        CString& rText = raText[i];
        rText.Trim();

        bool bRemove = false;
        if ( rText.IsEmpty() ) bRemove = true;
        if ( rText[0] == ';' ) bRemove = true;

        if ( bRemove )
        {
            raText.RemoveAt(i);
        }
    }
}

int CImportFFA::GetLineIndex(const CStringArray& aText, const CString& csHead)
{
	int nText = aText.GetCount();
    for (int i = 0; i < nText; ++i )
    {
        CString csHeadLow = csHead;
        CString csTextLow = aText[i];
        if ( csTextLow.MakeLower().Find(csHeadLow.MakeLower()) > -1 )
        {
            return i;
        }
    }
    ASSERT(0); return -1;
}

bool CImportFFA::ParseLine(const CString& csLine, CStringArray& raText)
{
    raText.RemoveAll();

    int nPosR = csLine.Find(_T(","));
    if ( nPosR < 0 ) { ASSERT(0); return false; }

    int nPosL = 0;
    int nCount = 0;
    while ( true )
    {
        CString csText = csLine.Mid(nPosL, nPosR);
        csText.Trim();
        if ( !csText.IsEmpty() )
        {
            raText.Add(csText);
        }

        nPosL = nPosR + 1;
        nPosR = csLine.Find(_T(","), nPosL);
        if ( nPosR < 0 )
        {
            CString csText = csLine.Right(csLine.GetLength() - nPosL);
            csText.Trim();
            if ( !csText.IsEmpty() )
            {
                raText.Add(csText);
            }
            break;
        }
        if ( nCount++ > 1000 ) { ASSERT(0); break; }
    }

    return ( raText.GetCount() > 0 ) ? true : false;
}

bool CImportFFA::GetUnitInfo(const CStringArray& aText, T_UNIT_INDEX& rFileUnit, bool& rbUnitLeng, bool& rbUnitForc)
{
    //rFileUnit.Initialize();
    rbUnitLeng = false;
    rbUnitForc = false;

    const CString csHeadText = _LSX(*Unit);
	int nLine = GetLineIndex(aText, csHeadText) + 1;
    if ( nLine + 1 > aText.GetCount() ) { ASSERT(0); return false; }

    CStringArray aUnit;
    if ( !ParseLine(aText[nLine], aUnit) ) { ASSERT(0); return false; }
    if ( aUnit.GetCount() < 2 ) { ASSERT(0); return false; }

    auto L_GetUnitForc = [ ] (const CString& csUnit, int& rnUnit) -> bool
    {
		UINT nUnitSt = D_UNITSYS_FORCE_INDEX_KG;
		UINT nUnitEd = D_UNITSYS_FORCE_INDEX_KIP + 1;
        for ( UINT nUnit = nUnitSt; nUnit < nUnitEd; ++nUnit )
        {
            if ( csUnit.CompareNoCase(CUnitCtrl::Get_ForUnit(nUnit)) == 0 )
            {
                rnUnit = nUnit;
                return true;
            }
        }
        return false;
    };
    if ( !L_GetUnitForc(aUnit[0], rFileUnit.nBase_Force) ) return false;
    rbUnitForc = true;

    auto L_GetUnitLeng = [ ] (const CString& csUnit, int& rnUnit) -> bool
    {
		UINT nUnitSt = D_UNITSYS_LENGTH_INDEX_MM;
		UINT nUnitEd = D_UNITSYS_LENGTH_INDEX_FT+1;
        for ( UINT nUnit = nUnitSt; nUnit < nUnitEd; ++nUnit )
        {
            if ( csUnit.CompareNoCase(CUnitCtrl::Get_LenUnit(nUnit)) == 0 )
            {
                rnUnit = nUnit;
                return true;
            }
        }
        return false;
    };
    if ( !L_GetUnitLeng(aUnit[1], rFileUnit.nBase_Length) ) return false;
    rbUnitLeng = true;

    return true;
}

bool CImportFFA::GetSoilProp(const CStringArray& aText, CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA &>& raSoilProp)
{
    raSoilProp.RemoveAll();

    const CString csHeadText = _LSX(*Soil Prop);
	const CString csEndText = _LSX(*ENDDATA);
	int nLine = GetLineIndex(aText, csHeadText) + 1;
	int nLine2 = GetLineIndex(aText, csEndText);
    if ( nLine + 1 > aText.GetCount() ) { ASSERT(0); return false; }
	if ( nLine2 > aText.GetCount()) { ASSERT(0); return false; }

    auto L_ConvertData = [ ] (const CStringArray& aText, T_POSP_BASE_DATA& rProp)
    {
        rProp.dHeight   = _tstof(aText[1]);
        rProp.dDensity  = _tstof(aText[2]);
        rProp.dVs       = _tstof(aText[3]);
        rProp.dDisp     = _tstof(aText[4]);
    };
    auto L_CheckData = [ ] (const T_POSP_BASE_DATA& cProp) -> bool
    {
        const double dZero = CMathFunc::Get_NormalZero();
        if ( cProp.dHeight  < dZero ) return false;
        if ( cProp.dDensity < dZero ) return false;
        if ( cProp.dVs      < dZero ) return false;
        //if ( cProp.dDisp    < dZero ) return false;
        return true;
    };

	int nText = nLine2;
    for (int nL = nLine; nL < nText; ++nL )
    {
        const CString& csLine = aText[nL];

        CStringArray aPropText;
        if ( !ParseLine(csLine, aPropText) ) { ASSERT(0); continue; }
        if ( aPropText.GetCount() < 5 ) { continue; }

		T_POSP_BASE_DATA SoilProp;
        L_ConvertData(aPropText, SoilProp);

        if ( L_CheckData(SoilProp) )
        {
            raSoilProp.Add(SoilProp);
        }
    }

    return ( raSoilProp.GetCount() > 0 ) ? true : false;
}

void CImportFFA::ConvertUnit(const T_UNIT_INDEX& FileUnit, CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA &>& raSoilProp)
{
    auto L_ConvertUnit = [ this ] (double& rdVal, const T_UNIT_INDEX& FileUnit, int nUnitType)
    {
        rdVal = CUnitCtrl::ConvValueTgtUnit2CurUnit(FileUnit.nBase_Force, FileUnit.nBase_Length, nUnitType, rdVal);
    };

	int nProp = raSoilProp.GetCount();
    for (int i = 0; i < nProp; ++i )
    {
		T_POSP_BASE_DATA& rProp = raSoilProp[i];
        L_ConvertUnit(rProp.dHeight , FileUnit, CUnitCtrl::m_POSP_UNIT.dHeight);
        L_ConvertUnit(rProp.dDensity, FileUnit, CUnitCtrl::m_POSP_UNIT.dDensity);
        L_ConvertUnit(rProp.dVs     , FileUnit, CUnitCtrl::m_POSP_UNIT.dVs);
        L_ConvertUnit(rProp.dDisp   , FileUnit, CUnitCtrl::m_POSP_UNIT.dDisp);
    }
}