#include "stdafx.h"

#include "..\dgnengine\idesign\DGN_def\DgnCodeDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnEUNationalAnnexDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnLoadCaseEnumDef.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\DB_ST_DN.h"

#include "DgnStruct.h"
#include "DgnCodeCtrl.h"
#include "DgnDataCtrl.h"

#include "DgnTypeConverter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;
using namespace dgn::def;

CDgnTypeConverter::CDgnTypeConverter()
{
}


CDgnTypeConverter::~CDgnTypeConverter()
{
}

enDgnCode CDgnTypeConverter::GetDgnCode(UINT unCode, UINT unNA /*= 0*/)
{
    switch ( unCode )
    {
    case AASHTO_LS17:
        {
            switch (unNA)
            {
            case Amend_Recommended:  return enDgnCode::AASHTO_LS17;
            case Amend_California:   return enDgnCode::AASHTO_LS17_CALIFORNIA;
            default: ASSERT(0); return enDgnCode::CODE_NULL;
            }
        }
        break;        
    case AASHTO_LS20:
        return enDgnCode::AASHTO_LS20;
    case AASHTO_LS24:
		return enDgnCode::AASHTO_LS24;
    case AASHTO_LC20:
        return enDgnCode::AASHTO_LC20;
    case AASHTO_LC24:
        return enDgnCode::AASHTO_LC24;
    case AREMA23:
        return enDgnCode::AREMA23;
	case KDS_41_30_10_2022:
		return enDgnCode::KDS41_20_00_2022;
	case KDS_24_14_31_2018:
		return enDgnCode::KDS24_14_31_2018;
    case AISC_LRFD16:
        return enDgnCode::AISC360_16_LRFD;
    case AISC_LRFD22:
        return enDgnCode::AISC360_22_LRFD;
    case ACI318_14:
        return enDgnCode::ACI318_14;
    case ACI318M_14:
        return enDgnCode::ACI318M_14;
    case ACI318_19:
        return enDgnCode::ACI318_19;
    case ACI318M_19:
        return enDgnCode::ACI318M_19;
    case ACI318_25:
        return enDgnCode::ACI318_25;
    case ACI318M_25:
        return enDgnCode::ACI318M_25;
    case KDS_41_30_2018:
        return enDgnCode::KDS41_30_00_2018;
    case KDS_41_20_2022:
        return enDgnCode::KDS41_20_00_2022;
    case TWN_USD112:
        return enDgnCode::TWN_USD112;
    case BS5400_4_90:
        return enDgnCode::BS5400_4_1990;
    case TMH07_89:
         return enDgnCode::TMH07_89;
    case Eurocode2_2_05:
        {
            switch ( unNA )
            {
            case Recommended:   return enDgnCode::EN1992_2_2005_REC;
            case Austria:       return enDgnCode::EN1992_2_2005_AUT;
            case Belgium:       return enDgnCode::EN1992_2_2005_BEL;
            case Cyprus:        return enDgnCode::EN1992_2_2005_CYP;
            case CzechRepublic: return enDgnCode::EN1992_2_2005_CZE;
            case Denmark:       return enDgnCode::EN1992_2_2005_DNK;
            case Netherlands:   return enDgnCode::EN1992_2_2005_NLD;
            case Finland:       return enDgnCode::EN1992_2_2005_FIN;
            case France:        return enDgnCode::EN1992_2_2005_FRA;
            case Germany:       return enDgnCode::EN1992_2_2005_DEU;
            case Greece:        return enDgnCode::EN1992_2_2005_GRC;
            case Ireland:       return enDgnCode::EN1992_2_2005_IRL;
            case Italy:         return enDgnCode::EN1992_2_2005_ITA;
            case Luxembourg:    return enDgnCode::EN1992_2_2005_LUX;
            case Norway:        return enDgnCode::EN1992_2_2005_NOR;
            case Poland:        return enDgnCode::EN1992_2_2005_POL;
            case Romania:       return enDgnCode::EN1992_2_2005_ROU;
            case Singapore:     return enDgnCode::EN1992_2_2005_SGP;
            case Slovakia:      return enDgnCode::EN1992_2_2005_SVK;
            case Slovenia:      return enDgnCode::EN1992_2_2005_SVN;
            case Spain:         return enDgnCode::EN1992_2_2005_ESP;
            case Sweden:        return enDgnCode::EN1992_2_2005_SWE;
            case UnitedKingdom: return enDgnCode::EN1992_2_2005_GBR;
            default: ASSERT(0); return enDgnCode::CODE_NULL;
            }
        }
        break;
    case Eurocode3_05:
        {
            switch ( unNA )
            {
            case Recommended:   return enDgnCode::EN1993_1_1_2005_REC;
            case Austria:       return enDgnCode::EN1993_1_1_2005_AUT;
            case Belgium:       return enDgnCode::EN1993_1_1_2005_BEL;
            case Cyprus:        return enDgnCode::EN1993_1_1_2005_CYP;
            case CzechRepublic: return enDgnCode::EN1993_1_1_2005_CZE;
            case Denmark:       return enDgnCode::EN1993_1_1_2005_DNK;
            case Netherlands:   return enDgnCode::EN1993_1_1_2005_NLD;
            case Finland:       return enDgnCode::EN1993_1_1_2005_FIN;
            case France:        return enDgnCode::EN1993_1_1_2005_FRA;
            case Germany:       return enDgnCode::EN1993_1_1_2005_DEU;
            case Greece:        return enDgnCode::EN1993_1_1_2005_GRC;
            case Ireland:       return enDgnCode::EN1993_1_1_2005_IRL;
            case Italy:         return enDgnCode::EN1993_1_1_2005_ITA;
            case Luxembourg:    return enDgnCode::EN1993_1_1_2005_LUX;
            case Norway:        return enDgnCode::EN1993_1_1_2005_NOR;
            case Poland:        return enDgnCode::EN1993_1_1_2005_POL;
            case Romania:       return enDgnCode::EN1993_1_1_2005_ROU;
            case Singapore:     return enDgnCode::EN1993_1_1_2005_SGP;
            case Slovakia:      return enDgnCode::EN1993_1_1_2005_SVK;
            case Slovenia:      return enDgnCode::EN1993_1_1_2005_SVN;
            case Spain:         return enDgnCode::EN1993_1_1_2005_ESP;
            case Sweden:
            case Sweden19:      return enDgnCode::EN1993_1_1_2005_SWE;
            case UnitedKingdom: return enDgnCode::EN1993_1_1_2005_GBR;
            case Malaysia:      return enDgnCode::EN1993_1_1_2005_MYS;
            default: ASSERT(0); return enDgnCode::CODE_NULL;
            }
        }
        break;
    case Eurocode3_2_05:
        {
            switch ( unNA )
            {
            case Recommended:   return enDgnCode::EN1993_2_2006_REC;
            case Austria:       return enDgnCode::EN1993_2_2006_AUT;
            case Belgium:       return enDgnCode::EN1993_2_2006_BEL;
            case Cyprus:        return enDgnCode::EN1993_2_2006_CYP;
            case CzechRepublic: return enDgnCode::EN1993_2_2006_CZE;
            case Denmark:       return enDgnCode::EN1993_2_2006_DNK;
            case Netherlands:   return enDgnCode::EN1993_2_2006_NLD;
            case Finland:       return enDgnCode::EN1993_2_2006_FIN;
            case France:        return enDgnCode::EN1993_2_2006_FRA;
            case Germany:       return enDgnCode::EN1993_2_2006_DEU;
            case Greece:        return enDgnCode::EN1993_2_2006_GRC;
            case Ireland:       return enDgnCode::EN1993_2_2006_IRL;
            case Italy:         return enDgnCode::EN1993_2_2006_ITA;
            case Luxembourg:    return enDgnCode::EN1993_2_2006_LUX;
            case Norway:        return enDgnCode::EN1993_2_2006_NOR;
            case Poland:        return enDgnCode::EN1993_2_2006_POL;
            case Romania:       return enDgnCode::EN1993_2_2006_ROU;
            case Singapore:     return enDgnCode::EN1993_2_2006_SGP;
            case Slovakia:      return enDgnCode::EN1993_2_2006_SVK;
            case Slovenia:      return enDgnCode::EN1993_2_2006_SVN;
            case Spain:         return enDgnCode::EN1993_2_2006_ESP;
            case Sweden:        return enDgnCode::EN1993_2_2006_SWE;
            case UnitedKingdom: return enDgnCode::EN1993_2_2006_GBR;
            //case Malaysia:      return enDgnCode::EN1993_2_2006_MYS;
            default: ASSERT(0); return enDgnCode::CODE_NULL;
            }
        }
        break;
    case EUROCODE4_2_04_CSG:
        {
            switch ( unNA )
            {
            case Recommended:   return enDgnCode::EN1994_2_2005_REC;
            case Austria:       return enDgnCode::EN1994_2_2005_AUT;
            case Belgium:       return enDgnCode::EN1994_2_2005_BEL;
            case Cyprus:        return enDgnCode::EN1994_2_2005_CYP;
            case CzechRepublic: return enDgnCode::EN1994_2_2005_CZE;
            case Denmark:       return enDgnCode::EN1994_2_2005_DNK;
            case Netherlands:   return enDgnCode::EN1994_2_2005_NLD;
            case Finland:       return enDgnCode::EN1994_2_2005_FIN;
            case France:        return enDgnCode::EN1994_2_2005_FRA;
            case Germany:       return enDgnCode::EN1994_2_2005_DEU;
            case Greece:        return enDgnCode::EN1994_2_2005_GRC;
            case Ireland:       return enDgnCode::EN1994_2_2005_IRL;
            case Italy:         return enDgnCode::EN1994_2_2005_ITA;
            case Luxembourg:    return enDgnCode::EN1994_2_2005_LUX;
            case Norway:        return enDgnCode::EN1994_2_2005_NOR;
            case Poland:        return enDgnCode::EN1994_2_2005_POL;
            case Romania:       return enDgnCode::EN1994_2_2005_ROU;
            case Singapore:     return enDgnCode::EN1994_2_2005_SGP;
            case Slovakia:      return enDgnCode::EN1994_2_2005_SVK;
            case Slovenia:      return enDgnCode::EN1994_2_2005_SVN;
            case Spain:         return enDgnCode::EN1994_2_2005_ESP;
            case Sweden:        return enDgnCode::EN1994_2_2005_SWE;
            case UnitedKingdom: return enDgnCode::EN1994_2_2005_GBR;
            //case Malaysia:      return enDgnCode::EN1994_2_2005_MYS;
            default: ASSERT(0); return enDgnCode::CODE_NULL;
            }
        }
        break;
	case AS5100_6_17_CSG:
		return enDgnCode::AS5100_6_2017;
    case NTC_DCEC_2023:
        return enDgnCode::NTC_DCEC_2023;
	default:
        break;
    }
	/*ASSERT(0);*/
    return enDgnCode::CODE_NULL;
}

enMemb CDgnTypeConverter::GetMembType(int nMembType)
{
    switch (nMembType)
    {
    case D_MBTP_COLUMN:
        return enMemb::Column;
    case D_MBTP_BEAM:
        return enMemb::Beam;
    case D_MBTP_BRACE:
        return enMemb::Brace;
    case D_MBTP_WALL:
        return enMemb::Wall;
    case D_MBTP_PLATEBEAM:
        return enMemb::PlateBeam;
    case D_MBTP_PLATECOLM:
        return enMemb::PlateColumn;
    case D_MBTP_SHELL:
        return enMemb::Shell;
    case D_MBTP_ETC:
        return enMemb::MeshedSlab;
    default:
        break;
    }

    ASSERT(0);
    return enMemb::Beam;
}


enMemb CDgnTypeConverter::GetMembTypeByElem(T_ELEM_K ElemK)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    auto nMembType = pDoc->m_pAttrCtrl->GetMemberType(ElemK);
    switch (nMembType)
    {
    case D_MBTP_COLUMN:
        return enMemb::Column;
    case D_MBTP_BEAM:
        return enMemb::Beam;
    case D_MBTP_BRACE:
        return enMemb::Brace;
    case D_MBTP_WALL:
        return enMemb::Wall;
    case D_MBTP_PLATEBEAM:
        return enMemb::PlateBeam;
    case D_MBTP_PLATECOLM:
        return enMemb::PlateColumn;
    case D_MBTP_ETC:
        return enMemb::MeshedSlab;
    case D_MBTP_SHELL:
        return enMemb::Shell;
    default:
        break;
    }

    ASSERT(0);
    return enMemb::Beam;
}

enLoadCase CDgnTypeConverter::GetLoadCaseType(CString strLoad)
{
    if (strLoad == _T("USER"))       return enLoadCase::USER;
    else if (strLoad == _T("D"))     return enLoadCase::D;
    else if (strLoad == _T("DC"))    return enLoadCase::DC;
    else if (strLoad == _T("DW"))    return enLoadCase::DW;
    else if (strLoad == _T("DD"))    return enLoadCase::DD;
    else if (strLoad == _T("EP"))    return enLoadCase::EP;
    else if (strLoad == _T("EH"))    return enLoadCase::EH;
    else if (strLoad == _T("EV"))    return enLoadCase::EV;
    else if (strLoad == _T("ES"))    return enLoadCase::ES;
    else if (strLoad == _T("EL"))    return enLoadCase::EL;
    else if (strLoad == _T("L"))     return enLoadCase::L;
    else if (strLoad == _T("LR"))    return enLoadCase::LR;
    else if (strLoad == _T("IL"))    return enLoadCase::IL;
    else if (strLoad == _T("LC"))    return enLoadCase::LC;
    else if (strLoad == _T("LSC"))   return enLoadCase::LSC;
    else if (strLoad == _T("LS"))    return enLoadCase::LS;
    else if (strLoad == _T("LP"))    return enLoadCase::LP;
    else if (strLoad == _T("ILP"))   return enLoadCase::ILP;
    else if (strLoad == _T("W"))     return enLoadCase::W;
    else if (strLoad == _T("WA"))    return enLoadCase::WA;
    else if (strLoad == _T("WT"))    return enLoadCase::WT;
    else if (strLoad == _T("WL"))    return enLoadCase::WL;
    else if (strLoad == _T("BRK"))   return enLoadCase::BRK;
    else if (strLoad == _T("E"))     return enLoadCase::E;
    else if (strLoad == _T("EVT"))   return enLoadCase::EVT;
    else if (strLoad == _T("CF"))    return enLoadCase::CF;
    else if (strLoad == _T("CO"))    return enLoadCase::CO;
    else if (strLoad == _T("CT"))    return enLoadCase::CT;
    else if (strLoad == _T("CV"))    return enLoadCase::CV;
    else if (strLoad == _T("STL"))   return enLoadCase::STL;
    else if (strLoad == _T("CR"))    return enLoadCase::CR;
    else if (strLoad == _T("SH"))    return enLoadCase::SH;
    else if (strLoad == _T("T"))     return enLoadCase::T;
    else if (strLoad == _T("TPG"))   return enLoadCase::TPG;
    else if (strLoad == _T("B"))     return enLoadCase::B;
    else if (strLoad == _T("WP"))    return enLoadCase::WP;
    else if (strLoad == _T("FP"))    return enLoadCase::FP;
    else if (strLoad == _T("SF"))    return enLoadCase::SF;
    else if (strLoad == _T("WPR"))   return enLoadCase::WPR;
    else if (strLoad == _T("PS"))    return enLoadCase::PS;
    else if (strLoad == _T("S"))     return enLoadCase::S;
    else if (strLoad == _T("R"))     return enLoadCase::R;
    else if (strLoad == _T("ER"))    return enLoadCase::ER;
    else if (strLoad == _T("CRL"))   return enLoadCase::CRL;
    else if (strLoad == _T("IP"))    return enLoadCase::IP;
    else if (strLoad == _T("FR"))    return enLoadCase::FR;
    else if (strLoad == _T("RS"))    return enLoadCase::RS;
    else if (strLoad == _T("BK"))    return enLoadCase::BK;
    else if (strLoad == _T("GE"))    return enLoadCase::GE;
    else if (strLoad == _T("SHV"))   return enLoadCase::SHV;
    else if (strLoad == _T("LIP"))   return enLoadCase::LIP;
    else if (strLoad == _T("LP"))    return enLoadCase::LP;
    else if (strLoad == _T("BL"))    return enLoadCase::BL;
    else if (strLoad == _T("EANN"))  return enLoadCase::EANN;
    else if (strLoad == _T("EANC"))  return enLoadCase::EANC;
    else if (strLoad == _T("EAMN"))  return enLoadCase::EAMN;
    else if (strLoad == _T("EAMC"))  return enLoadCase::EAMC;
    else if (strLoad == _T("EPNN"))  return enLoadCase::EPNN;
    else if (strLoad == _T("EPNC"))  return enLoadCase::EPNC;
    else if (strLoad == _T("EPMN"))  return enLoadCase::EPMN;
    else if (strLoad == _T("EPMC"))  return enLoadCase::EPMC;
    else if (strLoad == _T("DRL"))   return enLoadCase::DRL;
    else if (strLoad == _T("EEP"))   return enLoadCase::EEP;
    else if (strLoad == _T("CS"))    return enLoadCase::CS;
    else if (strLoad == _T("EX"))    return enLoadCase::EX;
    else if (strLoad == _T("I"))     return enLoadCase::I;
    else if (strLoad == _T("EE"))    return enLoadCase::EE;
    else if (strLoad == _T("GD"))    return enLoadCase::GD;
    else
    {
        ASSERT(0);
    }

    return enLoadCase::USER;
}

enDgn CDgnTypeConverter::GetDgnType(T_ELEM_K ElemK)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();

    T_MATD_D MatD;
    if ( !pDoc->m_pAttrCtrl->GetDgnGenMatd(ElemK, MatD) )
    {
        return enDgn::None; // User
    }

    if ( MatD.Type == _T("S") )
    {
        return enDgn::STL;
    }
    else if ( MatD.Type == _T("C") )
    {
        return enDgn::RCS;
    }
	else if ( MatD.Type == _T("SRC") )
	{
		return enDgn::None;//아직 지원 안함
	}
    else 
    {
        ASSERT(0);
    }

    return enDgn::None;
}

enDgnCode CDgnTypeConverter::GetDgnCodeByElem(T_ELEM_K ElemK)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    enDgn enDgnType = GetDgnType(ElemK);

    CDgnDataCtrl DataCtrl;
    switch ( enDgnType )
    {
    case enDgn::RCS:
        {
            T_DCON_D DconD;
            DataCtrl.Get_DgnConDcon(DconD);
            int nRCCode = CDgnCodeCtrl::GetConCodeID(DconD.DesignCode);
#if defined(_CIVIL )
            int nSubCode = DconD.nNationalAnnex;
#else
            int nSubCode = DconD.nNationalAnnex;
#endif
            return GetDgnCode(nRCCode, nSubCode);
        }
        break;
    case enDgn::STL:
        {
            T_DSTL_D DstlD;
            DataCtrl.Get_DgnStlDstl(DstlD);
#if defined(_CIVIL )
			int nSubCode = DstlD.nNationalAnnex;
#else
			int nSubCode = DstlD.nNationalAnnex;
#endif
            int nSTLCode = CDgnCodeCtrl::GetStlCodeID(DstlD.DesignCode);
            return GetDgnCode(nSTLCode, nSubCode);
        }
        break;
	case enDgn::None:
		return enDgnCode::CODE_NULL;
    default:
        ASSERT(0);
        break;
    }
    return enDgnCode::CODE_NULL;
}

