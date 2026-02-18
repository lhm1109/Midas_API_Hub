#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_lib/Macro.h"

#include "../dgnengine/idesign/DGN_def/DgnTypeEnumDef.h"

#include "TProductTypeStruct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

TProductMembType::TProductMembType() :
    PMembType(0),
    PSubType(enPSubType()),
    EMembType(enMemb())
{
}

TProductMembType::TProductMembType(int PMembType, 
                                   enPSubType PSubType,
                                   enMemb EMembType) :
    PMembType(PMembType),
    PSubType(PSubType),
    EMembType(EMembType)
{
}

bool TProductMembType::IsValid() const
{
    if ( PMembType == -1 )
    {
        return false;
    }

    if ( EMembType == enMemb() )
    {
        return false;
    }

    switch ( EMembType )
    {
    case enMemb::MeshedSlab:
        {
            if ( PSubType == enPSubType() )
            {
                return false;
            }
        }
        break;
    default:
        break;
    }

    return true;
}

int TProductMembType::GetPMembType() const
{
    return PMembType;
}

enProductSubType TProductMembType::GetPSubType() const
{
    return PSubType;
}

TProductMembType::enMemb TProductMembType::GetEMembType() const
{
    return EMembType;
}

bool TProductMembType::operator < (const TProductMembType& src) const
{
    const auto& left_pm = static_cast<int>(this->PMembType);
    const auto& left_ps = static_cast<int>(this->PSubType);
    const auto& left_em = Macro::Ordinal(this->EMembType);

    const auto& right_pm = static_cast<int>(src.PMembType);
    const auto& right_ps = static_cast<int>(src.PSubType);
    const auto& right_em = Macro::Ordinal(src.EMembType);

    return ( left_pm < right_pm ||
         (!(right_pm < left_pm) && left_ps < right_ps)   ||
         (!(right_pm < left_pm) && !(right_ps < left_ps) && left_em < right_em));
}

bool TProductMembType::operator == (const TProductMembType& src) const
{
    const auto& left_pm = this->PMembType;
    const auto& left_ps = this->PSubType;
    const auto& left_em = this->EMembType;

    const auto& right_pm = src.PMembType;
    const auto& right_ps = src.PSubType;
    const auto& right_em = src.EMembType;

    if ( left_pm != right_pm )
    {
        return false;
    }

    if ( left_ps != right_ps )
    {
        return false;
    }

    if ( left_em != right_em )
    {
        return false;
    }

    return true;
}

TProductDgnType::TProductDgnType() :
    PDgnType(CString()),
    EDgnType(enDgn())
{
}

TProductDgnType::TProductDgnType(const CString& PDgnType, 
                                 enDgn EDgnType) :
    PDgnType(PDgnType),
    EDgnType(EDgnType)
{
}

bool TProductDgnType::operator < (const TProductDgnType& src) const
{
    const auto& left_pd = this->PDgnType;
    const auto& left_ed = this->EDgnType;

    const auto& right_pd = src.PDgnType;
    const auto& right_ed = src.EDgnType;

    return ( left_pd < right_pd ||
         (!(right_pd < left_pd) && left_ed < right_ed));
}

bool TProductDgnType::operator == (const TProductDgnType& src) const
{
    const auto& left_pd = this->PDgnType;
    const auto& left_ed = this->EDgnType;

    const auto& right_pd = src.PDgnType;
    const auto& right_ed = src.EDgnType;

    if ( left_pd != right_pd )
    {
        return false;
    }

    if ( left_pd != right_pd )
    {
        return false;
    }

    return true;
}

bool TProductDgnType::IsValid() const
{
    if ( PDgnType == CString() )
    {
        return false;
    }

    if ( EDgnType == enDgn() )
    {
        return false;
    }

    return true;
}

CString TProductDgnType::GetPDgnType() const
{
    return PDgnType;
}

TProductDgnType::enDgn TProductDgnType::GetEDgnType() const
{
    return EDgnType;
}

CResultDataBaseTypeUtil::CResultDataBaseTypeUtil()
{

}

CResultDataBaseTypeUtil::~CResultDataBaseTypeUtil()
{

}

#if defined (_CIVIL)
    enResultDatabaseType nDefaultRcs = enResultDatabaseType::Rcsc;
#else
    enResultDatabaseType nDefaultRcs = enResultDatabaseType::Rcs;
#endif

dgn::converter::enResultDatabaseType CResultDataBaseTypeUtil::GetResultDataBaseType(dgn::def::enDgn nDgn, dgn::def::enMemb nMemb)
{
    switch ( nDgn )
    {
    case dgn::def::enDgn::RCS:
        {
            switch ( nMemb )
            {
            case dgn::def::enMemb::Shell:
                return enResultDatabaseType::Rcs;
            case dgn::def::enMemb::Beam:
            case dgn::def::enMemb::Column:
            case dgn::def::enMemb::Brace:
            case dgn::def::enMemb::PlateBeam:
            case dgn::def::enMemb::PlateColumn:
            case dgn::def::enMemb::Wall:
            case dgn::def::enMemb::InPlaneWall:
            case dgn::def::enMemb::MeshedSlab:
            case dgn::def::enMemb::MembraneSlab:
            case dgn::def::enMemb::FlatSlab:
            case dgn::def::enMemb::IsoFooting:
            case dgn::def::enMemb::StripFooting:
            case dgn::def::enMemb::MatFooting:
            case dgn::def::enMemb::Pile:
            case dgn::def::enMemb::BasementWall:
                return nDefaultRcs;
            case dgn::def::enMemb::None:
            case dgn::def::enMemb::Number:
            default:
                ASSERT(0); return nDefaultRcs;
                break;
            }
        }
        break;
    case dgn::def::enDgn::STL:
        return enResultDatabaseType::Acs;
    case dgn::def::enDgn::SRC:
    case dgn::def::enDgn::CSG:
    case dgn::def::enDgn::CFS:
    case dgn::def::enDgn::Number:
    default:
        break;
    }

    ASSERT(0);
    return enResultDatabaseType::Rcs;
}
