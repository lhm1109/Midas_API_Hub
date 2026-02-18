#include "stdafx.h"
#include "DgnCodeTool.h"
#include "..\dgnengine\idesign\DGN_def\DgnCodeDef.h"

CDgnCodeTool::CDgnCodeTool()
{
}


CDgnCodeTool::~CDgnCodeTool()
{
}

bool CDgnCodeTool::IsSupportCodeIDesignReport(const dgn::def::enDgnCode enDgnCode)
{
    switch (enDgnCode)
    {
        case dgn::def::enDgnCode::AASHTO_LS17:
        case dgn::def::enDgnCode::AASHTO_LS17_CALIFORNIA:
        case dgn::def::enDgnCode::AASHTO_LS20:
        case dgn::def::enDgnCode::AASHTO_LS24:
            return true;
        case dgn::def::enDgnCode::ACI318M_19:
        case dgn::def::enDgnCode::ACI318_19:
        case dgn::def::enDgnCode::ACI318M_25:
        case dgn::def::enDgnCode::ACI318_25:
        {
            return false;

            if (CProduct::GetTestEnvValue(_ULS(PerformGraphicOut)) == _ULS(yes))
                return true;
            else
                return false;
        }
        default:
            break;
    }

    return false;
}

bool CDgnCodeTool::IsEurocode2(const dgn::def::enDgnCode enDgnCode)
{
    switch ( enDgnCode )
    {
    case dgn::def::enDgnCode::EN1992_2_2005_REC:
    case dgn::def::enDgnCode::EN1992_2_2005_POL:
    case dgn::def::enDgnCode::EN1992_2_2005_SGP:
    case dgn::def::enDgnCode::EN1992_2_2005_GBR:
    case dgn::def::enDgnCode::EN1992_2_2005_IRL:
    case dgn::def::enDgnCode::EN1992_2_2005_ITA:
    case dgn::def::enDgnCode::EN1992_2_2005_AUT:
    case dgn::def::enDgnCode::EN1992_2_2005_BEL:
    case dgn::def::enDgnCode::EN1992_2_2005_CYP:
    case dgn::def::enDgnCode::EN1992_2_2005_CZE:
    case dgn::def::enDgnCode::EN1992_2_2005_DNK:
    case dgn::def::enDgnCode::EN1992_2_2005_NLD:
    case dgn::def::enDgnCode::EN1992_2_2005_FIN:
    case dgn::def::enDgnCode::EN1992_2_2005_FRA:
    case dgn::def::enDgnCode::EN1992_2_2005_DEU:
    case dgn::def::enDgnCode::EN1992_2_2005_GRC:
    case dgn::def::enDgnCode::EN1992_2_2005_LUX:
    case dgn::def::enDgnCode::EN1992_2_2005_NOR:
    case dgn::def::enDgnCode::EN1992_2_2005_ROU:
    case dgn::def::enDgnCode::EN1992_2_2005_SVK:
    case dgn::def::enDgnCode::EN1992_2_2005_SVN:
    case dgn::def::enDgnCode::EN1992_2_2005_ESP:
    case dgn::def::enDgnCode::EN1992_2_2005_SWE:
        return true;
    default:
        break;
    }

    return false;
}

bool CDgnCodeTool::IsEurocode4(const dgn::def::enDgnCode enDgnCode)
{
    switch ( enDgnCode )
    {
    case dgn::def::enDgnCode::EN1994_2_2005_REC:
    case dgn::def::enDgnCode::EN1994_2_2005_POL:
    case dgn::def::enDgnCode::EN1994_2_2005_SGP:
    case dgn::def::enDgnCode::EN1994_2_2005_GBR:
    case dgn::def::enDgnCode::EN1994_2_2005_IRL:
    case dgn::def::enDgnCode::EN1994_2_2005_ITA:
    case dgn::def::enDgnCode::EN1994_2_2005_AUT:
    case dgn::def::enDgnCode::EN1994_2_2005_BEL:
    case dgn::def::enDgnCode::EN1994_2_2005_CYP:
    case dgn::def::enDgnCode::EN1994_2_2005_CZE:
    case dgn::def::enDgnCode::EN1994_2_2005_DNK:
    case dgn::def::enDgnCode::EN1994_2_2005_NLD:
    case dgn::def::enDgnCode::EN1994_2_2005_FIN:
    case dgn::def::enDgnCode::EN1994_2_2005_FRA:
    case dgn::def::enDgnCode::EN1994_2_2005_DEU:
    case dgn::def::enDgnCode::EN1994_2_2005_GRC:
    case dgn::def::enDgnCode::EN1994_2_2005_LUX:
    case dgn::def::enDgnCode::EN1994_2_2005_NOR:
    case dgn::def::enDgnCode::EN1994_2_2005_ROU:
    case dgn::def::enDgnCode::EN1994_2_2005_SVK:
    case dgn::def::enDgnCode::EN1994_2_2005_SVN:
    case dgn::def::enDgnCode::EN1994_2_2005_ESP:
    case dgn::def::enDgnCode::EN1994_2_2005_SWE:
        return true;
    default:
        break;
    }

    return false;
}
