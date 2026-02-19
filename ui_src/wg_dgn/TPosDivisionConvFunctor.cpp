#include "StdAfx.h"

#include <array>

#include "../dgnengine/idesign/DGN_lib/DGNCompare.h"

#include "../dgnengine/idesign/DGN_def/PLevelPositionDef.h"
#include "../dgnengine/idesign/DGN_def/DgnSectorDef.h"

#include "TDBDataFunctor.h"

#include "TPosDivisionConvFunctor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;
using namespace dgn::converter;

const std::array<std::pair<UINT, CString>, 2U> ca2DivIntStr = 
{
    std::make_pair(0U, _T("I")),
    std::make_pair(4U, _T("J")),
};

const std::array<std::pair<UINT, CString>, 3U> ca3DivIntStr =
{
    std::make_pair(0U, _T("I")),
    std::make_pair(2U, _T("M")),
    std::make_pair(4U, _T("J")),
};

const std::array<std::pair<UINT, CString>, 5U> ca5DivIntStr =
{
    std::make_pair(0U, _T("I")),
    std::make_pair(1U, _T("1/4")),
    std::make_pair(2U, _T("1/2")),
    std::make_pair(3U, _T("3/4")),
    std::make_pair(4U, _T("J")),
};

TPosDivisionConvFunctor::TPosDivisionConvFunctor()
{
}

CString TPosDivisionConvFunctor::Str2Div(UINT Index)
{
    if ( Index > static_cast<UINT>(ca2DivIntStr.size()) )
    {
        ASSERT(0); return _T("");
    }

    return ca2DivIntStr[Index].second;
}

CString TPosDivisionConvFunctor::Str2Div(FIterator ItrRefer)
{
    auto* pPL = TDBDataFunctor<CPLevelPosition>()(ItrRefer);
    if ( !pPL )
    {
        ASSERT(0); return _T("");
    }

    const auto& Sector = pPL->Sector;
    const auto& dPoint = pPL->Pos.dPoint;

    if ( LT0(dPoint) || MT(dPoint, 1.00) )
    {
        ASSERT(0); return _T("");
    }

    switch ( Sector )
    {
    case enSector::I:
        {
            if ( EQ0(dPoint) )
            {
                return ca2DivIntStr[0U].second;
            }
        }
        break;
    case enSector::J:
        {
            if ( EQ(dPoint, 1.00) )
            {
                return ca2DivIntStr[1U].second;
            }
        }
        break;
    default: ASSERT(0); return _T("");
    }

    ASSERT(0);
    return _T("");
}

UINT TPosDivisionConvFunctor::Num2Div(const CString& strPos)
{
    if ( strPos.CompareNoCase(ca2DivIntStr[0U].second) )
    {
        return ca2DivIntStr[0U].first;
    }

    if ( strPos.CompareNoCase(ca2DivIntStr[1U].second) )
    {
        return ca2DivIntStr[1U].first;
    }

    ASSERT(0);
    return 0U;
}

UINT TPosDivisionConvFunctor::Num2Div(FIterator ItrRefer)
{
    auto* pPL = TDBDataFunctor<CPLevelPosition>()(ItrRefer);
    if ( !pPL )
    {
        ASSERT(0); return 0U;
    }

    const auto& Sector = pPL->Sector;
    const auto& dPoint = pPL->Pos.dPoint;

    if ( LT0(dPoint) || MT(dPoint, 1.00) )
    {
        ASSERT(0); return 0U;
    }

    switch ( Sector )
    {
    case enSector::I:
        {
            if ( EQ0(dPoint) )
            {
                return ca2DivIntStr[0U].first;
            }
        }
        break;
    case enSector::J:
        {
            if ( EQ(dPoint, 1.00) )
            {
                return ca2DivIntStr[1U].first;
            }
        }
        break;
    default: ASSERT(0); return 0U;
    }

    ASSERT(0);
    return 0U;
}

CString TPosDivisionConvFunctor::Str3Div(UINT Index)
{
    if ( Index > static_cast<UINT>(ca3DivIntStr.size()) )
    {
        ASSERT(0); return _T("");
    }

    return ca3DivIntStr[Index].second;
}

CString TPosDivisionConvFunctor::Str3Div(enSector Sector)
{
    switch ( Sector )
    {
    case enSector::I: return ca3DivIntStr[0U].second;
    case enSector::M: return ca3DivIntStr[1U].second;
    case enSector::J: return ca3DivIntStr[2U].second;
    default: ASSERT(0); return ca3DivIntStr.front().second;
    }
}

UINT TPosDivisionConvFunctor::Num3Div(const CString& strPos)
{
    if ( strPos.CompareNoCase(ca3DivIntStr[0U].second) )
    {
        return ca3DivIntStr[0U].first;
    }

    if ( strPos.CompareNoCase(ca3DivIntStr[1U].second) )
    {
        return ca3DivIntStr[1U].first;
    }

    if ( strPos.CompareNoCase(ca3DivIntStr[2U].second) )
    {
        return ca3DivIntStr[2U].first;
    }

    ASSERT(0);
    return 0U;
}

UINT TPosDivisionConvFunctor::Num3Div(enSector Sector)
{
    switch ( Sector )
    {
    case enSector::I: return ca3DivIntStr[0U].first;
    case enSector::M: return ca3DivIntStr[1U].first;
    case enSector::J: return ca3DivIntStr[2U].first;
    default: ASSERT(0); return ca3DivIntStr.front().first;
    }
}

CString TPosDivisionConvFunctor::Str5Div(UINT Index)
{
    if ( Index > static_cast<UINT>(ca5DivIntStr.size()) )
    {
        ASSERT(0); return _T("");
    }

    return ca5DivIntStr[Index].second;
}

CString TPosDivisionConvFunctor::Str5Div(FIterator ItrRefer)
{
    auto* pPL = TDBDataFunctor<CPLevelPosition>()(ItrRefer);
    if ( !pPL )
    {
        ASSERT(0); return _T("");
    }
    
    const auto& Sector = pPL->Sector;
    const auto& dPoint = pPL->Pos.dPoint;

    if ( LT0(dPoint) || MT(dPoint, 1.00) )
    {
        ASSERT(0); return _T("");
    }

    switch ( Sector )
    {
    case enSector::I:
        {
            if ( EQ0(dPoint) )
            {
                return ca5DivIntStr[0U].second;
            }

            ASSERT(0);
            return _T("");
        }
    case enSector::J:
        {
            if ( EQ(dPoint, 1.00) )
            {
                return ca5DivIntStr[4U].second;
            }

            ASSERT(0);
            return _T("");
        }
    case enSector::End:
        {
            if ( EQ0(dPoint) || EQ(dPoint, 0.25) )
            {
                return ca5DivIntStr[0U].second;
            }

            if ( EQ(dPoint, 0.75) || EQ(dPoint, 1.00) )
            {
                return ca5DivIntStr[4U].second;
            }

            ASSERT(0);
            return _T("");
        }
    case enSector::M:
        {
            if ( EQ(dPoint, 0.25) || EQ(dPoint, 0.50) || EQ(dPoint, 0.75) )
            {
                return ca5DivIntStr[2U].second;
            }

            ASSERT(0);
            return _T("");
        }
    case enSector::Mid:
        {
            if ( EQ0(dPoint) || EQ(dPoint, 1.00) )
            {
                ASSERT(0); return _T("");
            }

            //이건 야매룽다인데, 가운데면 무조건 _T("1/2")로 표기ㅠㅠ
            return ca5DivIntStr[2U].second;
        }
    default: ASSERT(0); return _T("");
    }
}

UINT TPosDivisionConvFunctor::Num5Div(const CString& strPos)
{
    if ( strPos.CompareNoCase(ca5DivIntStr[0U].second) )
    {
        return ca5DivIntStr[0U].first;
    }

    if ( strPos.CompareNoCase(ca5DivIntStr[1U].second) )
    {
        return ca5DivIntStr[1U].first;
    }

    if ( strPos.CompareNoCase(ca5DivIntStr[2U].second) )
    {
        return ca5DivIntStr[2U].first;
    }

    if ( strPos.CompareNoCase(ca5DivIntStr[3U].second) )
    {
        return ca5DivIntStr[3U].first;
    }

    if ( strPos.CompareNoCase(ca5DivIntStr[4U].second) )
    {
        return ca5DivIntStr[4U].first;
    }

    ASSERT(0);
    return 0U;
}

UINT TPosDivisionConvFunctor::Num5Div(FIterator ItrRefer)
{
    auto* pPL = TDBDataFunctor<CPLevelPosition>()(ItrRefer);
    if ( !pPL )
    {
        ASSERT(0); return 0U;
    }

    const auto& Sector = pPL->Sector;
    const auto& dPoint = pPL->Pos.dPoint;

    if ( LT0(dPoint) || MT(dPoint, 1.00) )
    {
        ASSERT(0); return 0U;
    }

    switch ( Sector )
    {
    case enSector::I:
        {
            if ( EQ0(dPoint) )
            {
                return ca5DivIntStr[0U].first;
            }

            ASSERT(0);
            return 0U;
        }
    case enSector::J:
        {
            if ( EQ(dPoint, 1.00) )
            {
                return ca5DivIntStr[4U].first;
            }

            ASSERT(0);
            return 0U;
        }
    case enSector::End:
        {
            if ( EQ0(dPoint) || EQ(dPoint, 0.25) )
            {
                return ca5DivIntStr[0U].first;
            }

            if ( EQ(dPoint, 0.75) || EQ(dPoint, 1.00) )
            {
                return ca5DivIntStr[4U].first;
            }

            ASSERT(0);
            return 0U;
        }
    case enSector::M:
        {
            if ( EQ(dPoint, 0.25) || EQ(dPoint, 0.50) || EQ(dPoint, 0.75))
            {
                return ca5DivIntStr[2U].first;
            }

            ASSERT(0);
            return 0U;
        }
    case enSector::Mid:
        {
            if ( EQ0(dPoint) || EQ(dPoint, 1.00) )
            {
                ASSERT(0); return 0U;
            }

            return ca5DivIntStr[2U].first;
        }
    default: ASSERT(0); return 0U;
    }
}