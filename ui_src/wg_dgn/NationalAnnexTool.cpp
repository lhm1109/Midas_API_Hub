#include "StdAfx.h"

#include "NationalAnnexTool.h"
#include "DgnStruct.h"
#include "../dgnengine/idesign/DGN_def/DgnASRelevantDef.h"

#include "../wg_db/wg_db_DBLib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;

CNationalAnnexTool::CNationalAnnexTool()
{
    
}

CNationalAnnexTool::~CNationalAnnexTool()
{
}

int CNationalAnnexTool::ConvertEnumDBtoDGN(int nCode)
{
	// enNationalAnnex to DgnStruct
	switch ( nCode )
    {
    case dgn::def::enNationalAnnex::Recommended:	return RECOMMENDED;
    case dgn::def::enNationalAnnex::Austria:		return AUSTRIA;
    case dgn::def::enNationalAnnex::Belgium:		return BELGIUM;
    case dgn::def::enNationalAnnex::Cyprus:			return CYPRUS;
    case dgn::def::enNationalAnnex::CzechRepublic:	return CZECHREPUBLIC;
    case dgn::def::enNationalAnnex::Denmark:		return DENMARK;
    case dgn::def::enNationalAnnex::Netherlands:	return NETHERLANDS;
    case dgn::def::enNationalAnnex::Finland:		return FINLAND;
    case dgn::def::enNationalAnnex::France:			return FRANCE;
    case dgn::def::enNationalAnnex::Germany:		return GERMANY;
    case dgn::def::enNationalAnnex::Greece:			return GREECE;
    case dgn::def::enNationalAnnex::Ireland:		return IRELAND;
    case dgn::def::enNationalAnnex::Italy:			return ITALY;
    case dgn::def::enNationalAnnex::Luxembourg:		return LUXEMBOURG;
	case dgn::def::enNationalAnnex::Malaysia:		return MALAYSIA;
    case dgn::def::enNationalAnnex::Norway:			return NORWAY;
    case dgn::def::enNationalAnnex::Poland:			return POLAND;
    case dgn::def::enNationalAnnex::Romania:		return ROMANIA;
    case dgn::def::enNationalAnnex::Singapore:		return SINGAPORE;
    case dgn::def::enNationalAnnex::Slovakia:		return SLOVAKIA;
    case dgn::def::enNationalAnnex::Slovenia:		return SLOVENIA;
    case dgn::def::enNationalAnnex::Spain:			return SPAIN;
    case dgn::def::enNationalAnnex::Sweden:			return SWEDEN;
	case dgn::def::enNationalAnnex::Sweden19:		return SWEDEN_2019;
    case dgn::def::enNationalAnnex::UnitedKingdom:	return UK;
    default: ASSERT(0);								return RECOMMENDED;
    }
}

int CNationalAnnexTool::ConvertEnumDGNtoDB(int nCode)
{
	// enNationalAnnex to DgnStruct
	switch ( nCode )
    {
    case RECOMMENDED	:	return dgn::def::enNationalAnnex::Recommended	;
    case AUSTRIA		:	return dgn::def::enNationalAnnex::Austria		;
    case BELGIUM		:	return dgn::def::enNationalAnnex::Belgium		;
    case CYPRUS			:	return dgn::def::enNationalAnnex::Cyprus		;
    case CZECHREPUBLIC	:	return dgn::def::enNationalAnnex::CzechRepublic	;
    case DENMARK		:	return dgn::def::enNationalAnnex::Denmark		;
    case NETHERLANDS	:	return dgn::def::enNationalAnnex::Netherlands	;
    case FINLAND		:	return dgn::def::enNationalAnnex::Finland		;
    case FRANCE			:	return dgn::def::enNationalAnnex::France		;
    case GERMANY		:	return dgn::def::enNationalAnnex::Germany		;
    case GREECE			:	return dgn::def::enNationalAnnex::Greece		;
    case IRELAND		:	return dgn::def::enNationalAnnex::Ireland		;
    case ITALY			:	return dgn::def::enNationalAnnex::Italy			;
    case LUXEMBOURG		:	return dgn::def::enNationalAnnex::Luxembourg	;
	case MALAYSIA		:	return dgn::def::enNationalAnnex::Malaysia		;
    case NORWAY			:	return dgn::def::enNationalAnnex::Norway		;
    case POLAND			:	return dgn::def::enNationalAnnex::Poland		;
    case ROMANIA		:	return dgn::def::enNationalAnnex::Romania		;
    case SINGAPORE		:	return dgn::def::enNationalAnnex::Singapore		;
    case SLOVAKIA		:	return dgn::def::enNationalAnnex::Slovakia		;
    case SLOVENIA		:	return dgn::def::enNationalAnnex::Slovenia		;
    case SPAIN			:	return dgn::def::enNationalAnnex::Spain			;
    case SWEDEN			:	return dgn::def::enNationalAnnex::Sweden		;
	case SWEDEN_2019	:	return dgn::def::enNationalAnnex::Sweden19		;
	case UK				:	return dgn::def::enNationalAnnex::UnitedKingdom	;
    default: ASSERT(0);		return dgn::def::enNationalAnnex::Recommended	;
    }
}

int CNationalAnnexTool::ConvertNationalAnnex(CString strCode)
{
    int nNationalAnnex = -1;

    nNationalAnnex = [strCode] () -> int
    {
        if ( strCode == _LS(IDS_DGN_RECOMMENDED) )    return enNationalAnnex::Recommended;
        if ( strCode == _LS(IDS_DGN_Austria) )        return enNationalAnnex::Austria;
        if ( strCode == _LS(IDS_DGN_Belgium) )        return enNationalAnnex::Belgium;
        if ( strCode == _LS(IDS_DGN_Cyprus) )         return enNationalAnnex::Cyprus;
        if ( strCode == _LS(IDS_DGN_Czech_Republic) ) return enNationalAnnex::CzechRepublic;
        if ( strCode == _LS(IDS_DGN_Denmark) )        return enNationalAnnex::Denmark;
        if ( strCode == _LS(IDS_DGN_Netherlands) )    return enNationalAnnex::Netherlands;
        if ( strCode == _LS(IDS_DGN_Finland) )        return enNationalAnnex::Finland;
        if ( strCode == _LS(IDS_DGN_France) )         return enNationalAnnex::France;
        if ( strCode == _LS(IDS_DGN_Germany) )        return enNationalAnnex::Germany;
        if ( strCode == _LS(IDS_DGN_Greece) )         return enNationalAnnex::Greece;
        if ( strCode == _LS(IDS_DGN_Ireland) )        return enNationalAnnex::Ireland;
        if ( strCode == _LS(IDS_DGN_ITALY) )          return enNationalAnnex::Italy;
        if ( strCode == _LS(IDS_DGN_Luxembourg) )     return enNationalAnnex::Luxembourg;
		if ( strCode == _LS(IDS_DGN_Malaysia) )       return enNationalAnnex::Malaysia;
        if ( strCode == _LS(IDS_DGN_Norway) )         return enNationalAnnex::Norway;
        if ( strCode == _LS(IDS_DGN_Poland) )         return enNationalAnnex::Poland;
        if ( strCode == _LS(IDS_DGN_Romania) )        return enNationalAnnex::Romania;
        if ( strCode == _LS(IDS_DGN_Singapore) )      return enNationalAnnex::Singapore;
        if ( strCode == _LS(IDS_DGN_Slovakia) )       return enNationalAnnex::Slovakia;
        if ( strCode == _LS(IDS_DGN_Slovenia) )       return enNationalAnnex::Slovenia;
        if ( strCode == _LS(IDS_DGN_Spain) )          return enNationalAnnex::Spain;
		if ( strCode == _LS(IDS_DGN_Sweden) )         return enNationalAnnex::Sweden;
		if ( strCode == _LS(IDS_DGN_SWEDEN2019) )     return enNationalAnnex::Sweden19;
        if ( strCode == _LS(IDS_DGN_UK) )             return enNationalAnnex::UnitedKingdom;

        ASSERT(0); return Recommended;
    }();

    return nNationalAnnex;
}

CString CNationalAnnexTool::ConvertNationalAnnex(int nNationalAnnex)
{
    CString strNationalAnnex = _T("");

    strNationalAnnex = [nNationalAnnex] () -> CString
    {
        switch ( nNationalAnnex )
        {
        case enNationalAnnex::Recommended:   return _LS(IDS_DGN_RECOMMENDED);
        case enNationalAnnex::Austria:       return _LS(IDS_DGN_Austria);
        case enNationalAnnex::Belgium:       return _LS(IDS_DGN_Belgium);
        case enNationalAnnex::Cyprus:        return _LS(IDS_DGN_Cyprus);
        case enNationalAnnex::CzechRepublic: return _LS(IDS_DGN_Czech_Republic);
        case enNationalAnnex::Denmark:       return _LS(IDS_DGN_Denmark);
        case enNationalAnnex::Netherlands:   return _LS(IDS_DGN_Netherlands);
        case enNationalAnnex::Finland:       return _LS(IDS_DGN_Finland);
        case enNationalAnnex::France:        return _LS(IDS_DGN_France);
        case enNationalAnnex::Germany:       return _LS(IDS_DGN_Germany);
        case enNationalAnnex::Greece:        return _LS(IDS_DGN_Greece);
        case enNationalAnnex::Ireland:       return _LS(IDS_DGN_Ireland);
        case enNationalAnnex::Italy:         return _LS(IDS_DGN_ITALY);
        case enNationalAnnex::Luxembourg:    return _LS(IDS_DGN_Luxembourg);
		case enNationalAnnex::Malaysia:	     return _LS(IDS_DGN_Malaysia);
        case enNationalAnnex::Norway:        return _LS(IDS_DGN_Norway);
        case enNationalAnnex::Poland:        return _LS(IDS_DGN_Poland);
        case enNationalAnnex::Romania:       return _LS(IDS_DGN_Romania);
        case enNationalAnnex::Singapore:     return _LS(IDS_DGN_Singapore);
        case enNationalAnnex::Slovakia:      return _LS(IDS_DGN_Slovakia);
        case enNationalAnnex::Slovenia:      return _LS(IDS_DGN_Slovenia);
        case enNationalAnnex::Spain:         return _LS(IDS_DGN_Spain);
        case enNationalAnnex::Sweden:        return _LS(IDS_DGN_Sweden);
		case enNationalAnnex::Sweden19:      return _LS(IDS_DGN_SWEDEN2019);
        case enNationalAnnex::UnitedKingdom: return _LS(IDS_DGN_UK);
        default: ASSERT(0); return _LS(IDS_DGN_RECOMMENDED); // Default _T("Recommended")
        }
    }();

    return strNationalAnnex;
}

CString CNationalAnnexTool::Get_Alpha_3_code(const int& nNationalAnnex)
{
    switch (nNationalAnnex)
	{
		case enNationalAnnex::Recommended:	return _T("");
		case enNationalAnnex::Austria:		return _LSX(AUT);
		case enNationalAnnex::Belgium:		return _LSX(BEL);
		case enNationalAnnex::Cyprus:		return _LSX(CYP);
		case enNationalAnnex::CzechRepublic:return _LSX(CZE);
		case enNationalAnnex::Denmark:		return _LSX(DNK);
		case enNationalAnnex::Finland:		return _LSX(FIN);
		case enNationalAnnex::France:		return _LSX(FRA);
		case enNationalAnnex::Germany:		return _LSX(DEU);
		case enNationalAnnex::Greece:		return _LSX(GRC);
		case enNationalAnnex::Ireland:		return _LSX(IRL);
		case enNationalAnnex::Italy:		return _LSX(ITA);
		case enNationalAnnex::Luxembourg:	return _LSX(LUX);
		case enNationalAnnex::Malaysia:		return _LSX(MYS);
		case enNationalAnnex::Netherlands:	return _LSX(NLD);
		case enNationalAnnex::Norway:		return _LSX(NOR);
		case enNationalAnnex::Poland:		return _LSX(POL);
		case enNationalAnnex::Romania:		return _LSX(ROU);
		case enNationalAnnex::Singapore:	return _LSX(SGP);
		case enNationalAnnex::Slovakia:		return _LSX(SVK);
		case enNationalAnnex::Slovenia:		return _LSX(SVN);
		case enNationalAnnex::Spain:		return _LSX(ESP);
		case enNationalAnnex::Sweden:		return _LSX(SWE);
		case enNationalAnnex::Sweden19:		return _LSX(SWE2019);
		case enNationalAnnex::UnitedKingdom:return _LSX(GBR);

		default: ASSERT(0); return _T("");
	}
}

CString CNationalAnnexTool::Get_EC2_04_Codename(const int& nNationalAnnex)
{
/*
    switch (nNationalAnnex)
	{
		case enNationalAnnex::Recommended:	return _T("EN 1992-1-1:2004");
		case enNationalAnnex::Austria:		return _T("ÖNORM EN 1992-1-1:2004/NA:2007");
		case enNationalAnnex::Belgium:		return _T("NBN EN 1992-1-1:2004/NA:2010");
		case enNationalAnnex::Cyprus:		return _T("CYS EN 1992-1-1:2004");
		case enNationalAnnex::CzechRepublic:return _T("ČSN EN 1992-1-1:2004/NA:2011");
		case enNationalAnnex::Denmark:		return _T("DS EN 1992-1-1:2004/NA:2017");
		case enNationalAnnex::Finland:		return _T("SFS-EN 1992-1-1:2004");
		case enNationalAnnex::France:		return _T("NF EN 1992-1-1:2004/NA:2007");
		case enNationalAnnex::Germany:		return _T("DIN EN 1992-1-1:2004/NA:2011");
		case enNationalAnnex::Greece:		return _T("ELOT EN 1992-1-1:2004/NA:2009");
		case enNationalAnnex::Ireland:		return _T("I.S. EN 1992-1-1:2004/NA:2010");
		case enNationalAnnex::Italy:		return _T("UNI-EN 1992-1-1:2004/NA:2007");
		case enNationalAnnex::Luxembourg:	return _T("ILNAS-EN 1992-1-1:2004/NA:2011");
		case enNationalAnnex::Malaysia:		return _T("MS EN 1992-1-1:2004/NA:2010");
		case enNationalAnnex::Netherlands:	return _T("NEN-EN 1992-1-1:2004/NA:2011");
		case enNationalAnnex::Norway:		return _T("NS-EN 1992-1-1:2004/NA:2010");
		case enNationalAnnex::Poland:		return _T("PN-EN 1992-1-1:2004/NA:2008");
		case enNationalAnnex::Romania:		return _T("SR EN 1992-1-1:2004/NA:2008");
		case enNationalAnnex::Singapore:	return _T("SS EN 1992-1-1:2004/NA:2008");
		case enNationalAnnex::Slovakia:		return _T("STN EN 1992-1-1:2004/NA:2008");
		case enNationalAnnex::Slovenia:		return _T("SIST EN 1992-1-1:2004/NA:2008");
		case enNationalAnnex::Spain:		return _T("AN/UNE-EN 1992-1-1:2004/NA:2008");
		case enNationalAnnex::Sweden:		return _T("SS-EN 1992-1-1:2004/NA:2010");
		case enNationalAnnex::Sweden19:		return _T("SS-EN 1992-1-1:2004/NA:2019");
		case enNationalAnnex::UnitedKingdom:return _T("BS EN 1992-1-1:2004/NA:2005");
		default: ASSERT(0); return _T("");
	}
*/
	switch (nNationalAnnex)
	{
		case enNationalAnnex::Recommended:	return _T("EN 1992-1-1:2004");
		case enNationalAnnex::Austria:		return _T("ÖNORM EN 1992-1-1:2007");
		case enNationalAnnex::Belgium:		return _T("NBN EN 1992-1-1:2010");
		case enNationalAnnex::Cyprus:		return _T("CYS EN 1992-1-1:2004");
		case enNationalAnnex::CzechRepublic:return _T("ČSN EN 1992-1-1:2011");
		case enNationalAnnex::Denmark:		return _T("DS/EN 1992-1-1 DK NA:2017");
		case enNationalAnnex::Finland:		return _T("SFS-EN 1992-1-1:2008"); //??//
		case enNationalAnnex::France:		return _T("NF EN 1992-1-1:2007");
		case enNationalAnnex::Germany:		return _T("DIN EN 1992-1-1:2011");
		case enNationalAnnex::Greece:		return _T("ELOT EN 1992-1-1:2009");
		case enNationalAnnex::Ireland:		return _T("I.S. EN 1992-1-1:2010");
		case enNationalAnnex::Italy:		return _T("UNI-EN 1992-1-1:2007");
		case enNationalAnnex::Luxembourg:	return _T("ILNAS-EN 1992-1-1:2004/AN-LU:2011");
		case enNationalAnnex::Malaysia:		return _T("MS EN 1992-1-1:2010");
		case enNationalAnnex::Netherlands:	return _T("NEN-EN 1992-1-1:2011");
		case enNationalAnnex::Norway:		return _T("NS-EN 1992-1-1:2004/NA:2010");
		case enNationalAnnex::Poland:		return _T("PN-EN 1992-1-1:2008");
		case enNationalAnnex::Romania:		return _T("SR EN 1992-1-1:2008");
		case enNationalAnnex::Singapore:	return _T("SS EN 1992-1-1:2008");
		case enNationalAnnex::Slovakia:		return _T("STN EN 1992-1-1:2008");
		case enNationalAnnex::Slovenia:		return _T("SIST EN 1992-1-1:2008");
		case enNationalAnnex::Spain:		return _T("AN/UNE-EN 1992-1-1:2008");
		case enNationalAnnex::Sweden:		return _T("SS-EN 1992-1-1:2010");
		case enNationalAnnex::Sweden19:		return _T("SS-EN 1992-1-1:2019");
		case enNationalAnnex::UnitedKingdom:return _T("BS EN 1992-1-1:2005");
		default: ASSERT(0); return _T("");
	}
}

CString CNationalAnnexTool::Get_EC3_05_Codename(const int& nNationalAnnex)
{
	switch (nNationalAnnex)
	{
		case enNationalAnnex::Recommended:	return _T("EN 1993-1-1:2005");
		case enNationalAnnex::Austria:		return _T("ÖNORM EN 1993-1-1:2007");
		case enNationalAnnex::Belgium:		return _T("NBN EN 1993-1-1:2018");
		case enNationalAnnex::Cyprus:		return _T("CYS EN 1993-1-1:2010");
		case enNationalAnnex::CzechRepublic:return _T("ČSN EN 1993-1-1:2016");
		case enNationalAnnex::Denmark:		return _T("DS/EN 1993-1-1 DK NA:2015");
		case enNationalAnnex::Finland:		return _T("SFS-EN 1993-1-1:2005");
		case enNationalAnnex::France:		return _T("NF EN 1993-1-1:2007");
		case enNationalAnnex::Germany:		return _T("DIN EN 1993-1-1:2010");
		case enNationalAnnex::Greece:		return _T("ELOT EN 1993-1-1:2009");
		case enNationalAnnex::Ireland:		return _T("I.S. EN 1993-1-1:2005");
		case enNationalAnnex::Italy:		return _T("UNI-EN 1993-1-1:2010");
		case enNationalAnnex::Luxembourg:	return _T("ILNAS-EN 1993-1-1:2005/AN-LU:2011");
		case enNationalAnnex::Malaysia:		return _T("MS EN 1993-1-1:2010");
		case enNationalAnnex::Netherlands:	return _T("NEN-EN 1993-1-1:2016");
		case enNationalAnnex::Norway:		return _T("NS-EN 1993-1-1:2005/NA:2008");
		case enNationalAnnex::Poland:		return _T("PN-EN 1993-1-1:2006");
		case enNationalAnnex::Romania:		return _T("SR EN 1993-1-1:2008");
		case enNationalAnnex::Singapore:	return _T("SS EN 1993-1-1:2010");
		case enNationalAnnex::Slovakia:		return _T("STN EN 1993-1-1:2007");
		case enNationalAnnex::Slovenia:		return _T("SIST EN 1993-1-1:2006");
		case enNationalAnnex::Spain:		return _T("AN/UNE-EN 1993-1-1:2013");
		case enNationalAnnex::Sweden:		return _T("SS-EN 1993-1-1:2010");
		case enNationalAnnex::Sweden19:		return _T("SS-EN 1993-1-1:2019");
		case enNationalAnnex::UnitedKingdom:return _T("BS EN 1993-1-1:2008");
		default: ASSERT(0); return _T("");
	}
}

CString CNationalAnnexTool::Get_EC3_1_3_06_Codename(const int& nNationalAnnex)
{
	switch (nNationalAnnex)
	{
		case enNationalAnnex::Recommended:	return _T("EN 1993-1-3:2006");
		case enNationalAnnex::Austria:		return _T("ÖNORM EN 1993-1-3:2007");
		case enNationalAnnex::Belgium:		return _T("NBN EN 1993-1-3:2011");
		case enNationalAnnex::Cyprus:		return _T("CYS EN 1993-1-3:2010");
		case enNationalAnnex::CzechRepublic:return _T("ČSN EN 1993-1-3:2012");
		case enNationalAnnex::Denmark:		return _T("DS/EN 1993-1-3 DK NA:2007");
		case enNationalAnnex::Finland:		return _T("SFS-EN 1993-1-3:2006");
		case enNationalAnnex::France:		return _T("NF EN 1993-1-3:2007");
		case enNationalAnnex::Germany:		return _T("DIN EN 1993-1-3:2010");
		case enNationalAnnex::Greece:		return _T("ELOT EN 1993-1-3:2009");
		case enNationalAnnex::Ireland:		return _T("I.S. EN 1993-1-3:2006");
		case enNationalAnnex::Italy:		return _T("UNI-EN 1993-1-3:2010");
		case enNationalAnnex::Luxembourg:	return _T("ILNAS-EN 1993-1-3:2005/AN-LU:2011");
		case enNationalAnnex::Malaysia:		return _T("MS EN 1993-1-3:2019");
		case enNationalAnnex::Netherlands:	return _T("NEN-EN 1993-1-3:2011");
		case enNationalAnnex::Norway:		return _T("NS-EN 1993-1-3:2005/NA:2009");
		case enNationalAnnex::Poland:		return _T("PN-EN 1993-1-3:2008");
		case enNationalAnnex::Romania:		return _T("SR EN 1993-1-3:2008");
		case enNationalAnnex::Singapore:	return _T("SS EN 1993-1-3:2010");
		case enNationalAnnex::Slovakia:		return _T("STN EN 1993-1-3:2010");
		case enNationalAnnex::Slovenia:		return _T("SIST EN 1993-1-3:2009");
		case enNationalAnnex::Spain:		return _T("AN/UNE-EN 1993-1-3:2012");
		case enNationalAnnex::Sweden:		return _T("SS-EN 1993-1-3:2010");
		case enNationalAnnex::Sweden19:		return _T("SS-EN 1993-1-3:2019");
		case enNationalAnnex::UnitedKingdom:return _T("BS EN 1993-1-3:2009");
		default: ASSERT(0); return _T("");
	}
}

int CNationalAnnexTool::ConvertAASHTOAmendment(CString strCode)
{
	int nNationalAnnex = -1;

	nNationalAnnex = [strCode]() -> int
		{
			if (strCode == _LS(IDS_DGN_RECOMMENDED))        return Amend_Recommended;
			if (strCode == _LS(IDS_DGN_AASHTO_California))  return Amend_California;

			ASSERT(0); return Amend_Recommended;
		}();

	return nNationalAnnex;
}

CString CNationalAnnexTool::ConvertAASHTOAmendment(int nAmendment)
{
	CString strNationalAnnex = _T("");

	strNationalAnnex = [nAmendment]() -> CString
		{
			switch (nAmendment)
			{
			case Amend_Recommended:   return _LS(IDS_DGN_RECOMMENDED);
			case Amend_California:    return _LS(IDS_DGN_AASHTO_California);

			default: ASSERT(0); return _LS(IDS_DGN_RECOMMENDED); // Default _T("Recommended")
			}
		}();

	return strNationalAnnex;
}

int CNationalAnnexTool::ConvertPscASAmendment(CString strCode)
{
	int nNationalAnnex = -1;

	nNationalAnnex = [strCode]() -> int
		{
			if (strCode == _LS(IDS_DGN_RECOMMENDED))    return Spec;
			if (strCode == _LS(IDS_DGN_AS_AMD_2_2024))  return Amd2_2024;

			ASSERT(0); return Amd2_2024;
		}();

	return nNationalAnnex;
}

CString CNationalAnnexTool::ConvertPscASAmendment(int nAmendment)
{
	CString strAmendment = _T("");

	strAmendment = [nAmendment]() -> CString
		{
			switch (nAmendment)
			{
			case Spec:         return _LS(IDS_DGN_RECOMMENDED);
			case Amd2_2024:    return _LS(IDS_DGN_AS_AMD_2_2024);

			default: ASSERT(0); return _LS(IDS_DGN_RECOMMENDED); // Default _T("Recommended")
			}
		}();

	return strAmendment;
}

int CNationalAnnexTool::ConvertRcASAmendment(CString strCode)
{
	int nNationalAnnex = -1;

	nNationalAnnex = [strCode]() -> int
	{
		if (strCode == _LS(IDS_DGN_AS_AMD_2_2024))  return RC_Amd2_2024;

		ASSERT(0); return RC_Amd2_2024;
	}();

	return nNationalAnnex;
}

CString CNationalAnnexTool::ConvertRcASAmendment(int nAmendment)
{
	CString strAmendment = _T("");

	strAmendment = [nAmendment]() -> CString
	{
		switch (nAmendment)
		{
		default:
		case RC_Amd2_2024:    return _LS(IDS_DGN_AS_AMD_2_2024);
		}
	}();

	return strAmendment;
}
std::vector<CString> CNationalAnnexTool::GetRcNationalAnnex()
{
    std::vector<CString> vString;
#if defined (_CIVIL)
	vString.push_back(_LS(IDS_DGN_RECOMMENDED));
	vString.push_back(_LS(IDS_DGN_Austria));
	vString.push_back(_LS(IDS_DGN_Belgium));
	vString.push_back(_LS(IDS_DGN_Cyprus));
	vString.push_back(_LS(IDS_DGN_Czech_Republic));
	vString.push_back(_LS(IDS_DGN_Denmark));
	vString.push_back(_LS(IDS_DGN_Finland));
	vString.push_back(_LS(IDS_DGN_France));
	vString.push_back(_LS(IDS_DGN_Germany));
	vString.push_back(_LS(IDS_DGN_Greece));
	vString.push_back(_LS(IDS_DGN_Ireland));
	vString.push_back(_LS(IDS_DGN_ITALY));
	vString.push_back(_LS(IDS_DGN_Luxembourg));
	vString.push_back(_LS(IDS_DGN_Netherlands));
	vString.push_back(_LS(IDS_DGN_Norway));
	vString.push_back(_LS(IDS_DGN_Poland));
	vString.push_back(_LS(IDS_DGN_Romania));
	vString.push_back(_LS(IDS_DGN_Singapore));
	vString.push_back(_LS(IDS_DGN_Slovakia));
	vString.push_back(_LS(IDS_DGN_Slovenia));
	vString.push_back(_LS(IDS_DGN_Spain));
	vString.push_back(_LS(IDS_DGN_Sweden));
	vString.push_back(_LS(IDS_DGN_UK));
#elif defined(_MGEN)
	if(CDBLib::ActivateEC_NA23())
	{
		vString.push_back(_LS(IDS_DGN_RECOMMENDED));
		vString.push_back(_LS(IDS_DGN_Austria));
		vString.push_back(_LS(IDS_DGN_Belgium));
		vString.push_back(_LS(IDS_DGN_Cyprus));
		vString.push_back(_LS(IDS_DGN_Czech_Republic));
		vString.push_back(_LS(IDS_DGN_Denmark));
		vString.push_back(_LS(IDS_DGN_Finland));
		vString.push_back(_LS(IDS_DGN_France));
		vString.push_back(_LS(IDS_DGN_Germany));
		vString.push_back(_LS(IDS_DGN_Greece));
		vString.push_back(_LS(IDS_DGN_Ireland));
		vString.push_back(_LS(IDS_DGN_ITALY));
		vString.push_back(_LS(IDS_DGN_Luxembourg));
		vString.push_back(_LS(IDS_DGN_Malaysia));
		vString.push_back(_LS(IDS_DGN_Netherlands));
		vString.push_back(_LS(IDS_DGN_Norway));
		vString.push_back(_LS(IDS_DGN_Poland));
		vString.push_back(_LS(IDS_DGN_Romania));
		vString.push_back(_LS(IDS_DGN_Singapore));
		vString.push_back(_LS(IDS_DGN_Slovakia));
		vString.push_back(_LS(IDS_DGN_Slovenia));
		vString.push_back(_LS(IDS_DGN_Spain));
		vString.push_back(_LS(IDS_DGN_Sweden));
		vString.push_back(_LS(IDS_DGN_SWEDEN2019));
		vString.push_back(_LS(IDS_DGN_UK));
	}
	else
	{
		vString.push_back(_LS(IDS_DGN_ITALY));
		vString.push_back(_LS(IDS_DGN_RECOMMENDED));
		vString.push_back(_LS(IDS_DGN_Sweden));
		vString.push_back(_LS(IDS_DGN_Singapore));
		vString.push_back(_LS(IDS_DGN_SWEDEN2019));
	}
#endif
    return vString;
}

std::vector<CString> CNationalAnnexTool::GetStlNationalAnnex()
{
	std::vector<CString> vString;
#if defined (_CIVIL)
	CString csVisibleCode = CProduct::GetTestEnvValue(_ULS(AllVisibleDgnCode));
	if ( csVisibleCode == _T("yes") )
	{
		vString.push_back(_LS(IDS_DGN_RECOMMENDED));
		vString.push_back(_LS(IDS_DGN_Austria));
		vString.push_back(_LS(IDS_DGN_Belgium));
		vString.push_back(_LS(IDS_DGN_Cyprus));
		vString.push_back(_LS(IDS_DGN_Czech_Republic));
		vString.push_back(_LS(IDS_DGN_Denmark));
		vString.push_back(_LS(IDS_DGN_Finland));
 		vString.push_back(_LS(IDS_DGN_France));
		vString.push_back(_LS(IDS_DGN_Germany));
		vString.push_back(_LS(IDS_DGN_Greece));
		vString.push_back(_LS(IDS_DGN_Ireland));
		vString.push_back(_LS(IDS_DGN_ITALY));
		vString.push_back(_LS(IDS_DGN_Luxembourg));
		vString.push_back(_LS(IDS_DGN_Netherlands));
		vString.push_back(_LS(IDS_DGN_Norway));
		vString.push_back(_LS(IDS_DGN_Poland));
		vString.push_back(_LS(IDS_DGN_Romania));
		vString.push_back(_LS(IDS_DGN_Singapore));
		vString.push_back(_LS(IDS_DGN_Slovakia));
		vString.push_back(_LS(IDS_DGN_Slovenia));
		vString.push_back(_LS(IDS_DGN_Spain));
		vString.push_back(_LS(IDS_DGN_Sweden));
		vString.push_back(_LS(IDS_DGN_UK));
	}
	else
	{
		vString.push_back(_LS(IDS_DGN_RECOMMENDED));
 		vString.push_back(_LS(IDS_DGN_France));
		vString.push_back(_LS(IDS_DGN_ITALY));
		vString.push_back(_LS(IDS_DGN_UK));
	}
#elif defined(_MGEN)
		if(CDBLib::ActivateEC_NA23())
		{
			vString.push_back(_LS(IDS_DGN_RECOMMENDED));
			vString.push_back(_LS(IDS_DGN_Austria));
			vString.push_back(_LS(IDS_DGN_Belgium));
			vString.push_back(_LS(IDS_DGN_Cyprus));
			vString.push_back(_LS(IDS_DGN_Czech_Republic));
			vString.push_back(_LS(IDS_DGN_Denmark));
			vString.push_back(_LS(IDS_DGN_Finland));
			vString.push_back(_LS(IDS_DGN_France));
			vString.push_back(_LS(IDS_DGN_Germany));
			vString.push_back(_LS(IDS_DGN_Greece));
			vString.push_back(_LS(IDS_DGN_Ireland));
			vString.push_back(_LS(IDS_DGN_ITALY));
			vString.push_back(_LS(IDS_DGN_Luxembourg));
			vString.push_back(_LS(IDS_DGN_Malaysia));
			vString.push_back(_LS(IDS_DGN_Netherlands));
			vString.push_back(_LS(IDS_DGN_Norway));
			vString.push_back(_LS(IDS_DGN_Poland));
			vString.push_back(_LS(IDS_DGN_Romania));
			vString.push_back(_LS(IDS_DGN_Singapore));
			vString.push_back(_LS(IDS_DGN_Slovakia));
			vString.push_back(_LS(IDS_DGN_Slovenia));
			vString.push_back(_LS(IDS_DGN_Spain));
			vString.push_back(_LS(IDS_DGN_Sweden));
			vString.push_back(_LS(IDS_DGN_SWEDEN2019));
			vString.push_back(_LS(IDS_DGN_UK));
		}
		else
		{
			vString.push_back(_LS(IDS_DGN_RECOMMENDED));
			vString.push_back(_LS(IDS_DGN_Sweden));
			vString.push_back(_LS(IDS_DGN_SWEDEN2019));
			vString.push_back(_LS(IDS_DGN_Singapore));
		}
#endif
    return vString;
}

std::vector<CString> CNationalAnnexTool::GetCfsNationalAnnex()
{
	std::vector<CString> vString;
	if(CDBLib::ActivateEC_NA23())
	{
		vString.push_back(_LS(IDS_DGN_RECOMMENDED));
		vString.push_back(_LS(IDS_DGN_Austria));
		vString.push_back(_LS(IDS_DGN_Belgium));
		vString.push_back(_LS(IDS_DGN_Cyprus));
		vString.push_back(_LS(IDS_DGN_Czech_Republic));
		vString.push_back(_LS(IDS_DGN_Denmark));
		vString.push_back(_LS(IDS_DGN_Finland));
		vString.push_back(_LS(IDS_DGN_France));
		vString.push_back(_LS(IDS_DGN_Germany));
		vString.push_back(_LS(IDS_DGN_Greece));
		vString.push_back(_LS(IDS_DGN_Ireland));
		vString.push_back(_LS(IDS_DGN_ITALY));
		vString.push_back(_LS(IDS_DGN_Luxembourg));
		vString.push_back(_LS(IDS_DGN_Malaysia));
		vString.push_back(_LS(IDS_DGN_Netherlands));
		vString.push_back(_LS(IDS_DGN_Norway));
		vString.push_back(_LS(IDS_DGN_Poland));
		vString.push_back(_LS(IDS_DGN_Romania));
		vString.push_back(_LS(IDS_DGN_Singapore));
		vString.push_back(_LS(IDS_DGN_Slovakia));
		vString.push_back(_LS(IDS_DGN_Slovenia));
		vString.push_back(_LS(IDS_DGN_Spain));
		vString.push_back(_LS(IDS_DGN_Sweden));
		vString.push_back(_LS(IDS_DGN_SWEDEN2019));
		vString.push_back(_LS(IDS_DGN_UK));
	}
	else
	{
		vString.push_back(_LS(IDS_DGN_RECOMMENDED));
		vString.push_back(_LS(IDS_DGN_Singapore));
		vString.push_back(_LS(IDS_DGN_ITALY));
	}
    return vString;
}

std::vector<CString> CNationalAnnexTool::GetCpgNationalAnnex()
{
	CString csVisibleCode = CProduct::GetTestEnvValue(_ULS(AllVisibleDgnCode));
	std::vector<CString> vString;
	if ( csVisibleCode == _T("yes") )
	{
		vString.push_back(_LS(IDS_DGN_RECOMMENDED));
		vString.push_back(_LS(IDS_DGN_Austria));
		vString.push_back(_LS(IDS_DGN_Belgium));
		vString.push_back(_LS(IDS_DGN_Cyprus));
		vString.push_back(_LS(IDS_DGN_Czech_Republic));
		vString.push_back(_LS(IDS_DGN_Denmark));
		vString.push_back(_LS(IDS_DGN_Finland));
		vString.push_back(_LS(IDS_DGN_France));
		vString.push_back(_LS(IDS_DGN_Germany));
		vString.push_back(_LS(IDS_DGN_Greece));
		vString.push_back(_LS(IDS_DGN_Ireland));
		vString.push_back(_LS(IDS_DGN_ITALY));
		vString.push_back(_LS(IDS_DGN_Luxembourg));
		vString.push_back(_LS(IDS_DGN_Netherlands));
		vString.push_back(_LS(IDS_DGN_Norway));
		vString.push_back(_LS(IDS_DGN_Poland));
		vString.push_back(_LS(IDS_DGN_Romania));
		vString.push_back(_LS(IDS_DGN_Singapore));
		vString.push_back(_LS(IDS_DGN_Slovakia));
		vString.push_back(_LS(IDS_DGN_Slovenia));
		vString.push_back(_LS(IDS_DGN_Spain));
		vString.push_back(_LS(IDS_DGN_Sweden));
		vString.push_back(_LS(IDS_DGN_UK));
	}
	else
	{
		vString.push_back(_LS(IDS_DGN_RECOMMENDED));
		vString.push_back(_LS(IDS_DGN_France));
		vString.push_back(_LS(IDS_DGN_ITALY));
		vString.push_back(_LS(IDS_DGN_UK));
	}
    return vString;
}

std::vector<CString> CNationalAnnexTool::GetRcAASHTOAmendment()
{
	CString csVisibleCode = CProduct::GetTestEnvValue(_ULS(AllVisibleDgnCode));

	std::vector<CString> vString;

	vString.push_back(_LS(IDS_DGN_RECOMMENDED));
	vString.push_back(_LS(IDS_DGN_AASHTO_California));

	return vString;
}

std::vector<CString> CNationalAnnexTool::GetPscASAmendment()
{
	CString csVisibleCode = CProduct::GetTestEnvValue(_ULS(AllVisibleDgnCode));

	std::vector<CString> vString;

	vString.push_back(_LS(IDS_DGN_RECOMMENDED));
	vString.push_back(_LS(IDS_DGN_AS_AMD_2_2024));

	return vString;
}

std::vector<CString> CNationalAnnexTool::GetRcASAmendment()
{
	CString csVisibleCode = CProduct::GetTestEnvValue(_ULS(AllVisibleDgnCode));

	std::vector<CString> vString;
	vString.push_back(_LS(IDS_DGN_AS_AMD_2_2024));

	return vString;
}

int CNationalAnnexTool::ConvertReportLanguage(CString strCode)
{
	int nNationalAnnex = -1;

#if defined(_CIVIL)
	nNationalAnnex = [strCode]() -> int
	{
		if ( strCode == _LS(IDS_DGNPREPORT_LANGUAGE_Czech) )         return EN_REPORT_Czech;
		if ( strCode == _LS(IDS_DGNPREPORT_LANGUAGE_English) )       return EN_REPORT_English;
		if ( strCode == _LS(IDS_DGNPREPORT_LANGUAGE_France) )        return EN_REPORT_France;
		if ( strCode == _LS(IDS_DGNPREPORT_LANGUAGE_Poland) )        return EN_REPORT_Poland;
		if ( strCode == _LS(IDS_DGNPREPORT_LANGUAGE_Turkey) )        return EN_REPORT_Turkey;
		if ( strCode == _LS(IDS_DGNPREPORT_LANGUAGE_Korean) )        return EN_REPORT_Korean;
		if ( strCode == _LS(IDS_DGNPREPORT_LANGUAGE_Vietnamese) )    return EN_REPORT_Vietnamese;
		ASSERT(0); 
		return EN_REPORT_English;
	}();
#else
	nNationalAnnex = [strCode]() -> int
	{
		if (strCode == _LS(IDS_DGNPREPORT_LANGUAGE_English))   return EN_REPORT_English;

		ASSERT(0);
		return EN_REPORT_English;
	}();
#endif

	return nNationalAnnex;
}

CString CNationalAnnexTool::ConvertReportLanguage(int nNationalAnnex)
{
	CString strNationalAnnex = _T("");

#if defined(_CIVIL)
	strNationalAnnex = [nNationalAnnex]() -> CString
	{
		switch ( nNationalAnnex )
		{
		case EN_REPORT_Czech:    return _LS(IDS_DGNPREPORT_LANGUAGE_Czech);
		case EN_REPORT_English:  return _LS(IDS_DGNPREPORT_LANGUAGE_English);
		case EN_REPORT_France:   return _LS(IDS_DGNPREPORT_LANGUAGE_France);
		case EN_REPORT_Poland:   return _LS(IDS_DGNPREPORT_LANGUAGE_Poland);
		case EN_REPORT_Turkey:   return _LS(IDS_DGNPREPORT_LANGUAGE_Turkey);
		case EN_REPORT_Korean:   return _LS(IDS_DGNPREPORT_LANGUAGE_Korean);
		case EN_REPORT_Vietnamese:   return _LS(IDS_DGNPREPORT_LANGUAGE_Vietnamese);

		default: ASSERT(0); return _LS(IDS_DGNPREPORT_LANGUAGE_English); // Default
		}
	}();
#elif defined(_MGEN)
	strNationalAnnex = [nNationalAnnex]() -> CString
	{
		switch (nNationalAnnex)
		{
		case EN_REPORT_English:  return _LS(IDS_DGNPREPORT_LANGUAGE_English);

		default: ASSERT(0); return _LS(IDS_DGNPREPORT_LANGUAGE_English);
		}
	}();
#endif

	return strNationalAnnex;
}

std::vector<CString> CNationalAnnexTool::GetReportLanguage(const CString& strCode)
{
	static const std::set<CString> EurocodeGroup =
	{
		STLCODE_EC3_2_05,
		CONCODE_EC2_04,
		CONCODE_EC2_2_05,
		CSGCODE_EN1994_2,
		CSGCODE_CS457_R1
	};

	static const std::set<CString> AASHTOGroup =
	{
		CONCODE_AASHTO_LRFD17,
		CONCODE_AASHTO_LRFD20,
		CONCODE_AASHTO_LRFD24
	};

	std::vector<CString> languages;

	if ( EurocodeGroup.find(strCode) != EurocodeGroup.end() )
	{
		languages =
		{
			_LS(IDS_DGNPREPORT_LANGUAGE_Czech),
			_LS(IDS_DGNPREPORT_LANGUAGE_English),
			_LS(IDS_DGNPREPORT_LANGUAGE_France),
			_LS(IDS_DGNPREPORT_LANGUAGE_Poland),
			_LS(IDS_DGNPREPORT_LANGUAGE_Vietnamese)
		};
	}
	else if ( AASHTOGroup.find(strCode) != AASHTOGroup.end() )
	{
		languages =
		{
			_LS(IDS_DGNPREPORT_LANGUAGE_English),
			_LS(IDS_DGNPREPORT_LANGUAGE_Turkey),
			_LS(IDS_DGNPREPORT_LANGUAGE_Korean),
			_LS(IDS_DGNPREPORT_LANGUAGE_Vietnamese)
		};
	}
	else
	{
		languages.push_back(_LS(IDS_DGNPREPORT_LANGUAGE_English));
	}

	return languages;
}

int CNationalAnnexTool::ConvertReportUnit(CString strCode)
{
	int nNationalAnnex = -1;

#if defined(_CIVIL)
	nNationalAnnex = [strCode]() -> int
	{
		if (strCode == _T("SI"))   return EN_REPORT_UNIT_SI;
		if (strCode == _T("US"))   return EN_REPORT_UNIT_US;

		ASSERT(0);
		return 0;
	}();
#else
	ASSERT(0);
	return 0;
#endif

	return nNationalAnnex;
}

CString CNationalAnnexTool::ConvertReportUnit(int nNationalAnnex)
{
	CString strNationalAnnex = _T("");

#if defined(_CIVIL)
	strNationalAnnex = [nNationalAnnex]() -> CString
	{
		switch (nNationalAnnex)
		{
		case EN_REPORT_UNIT_SI:  return _T("SI");
		case EN_REPORT_UNIT_US:  return _T("US");

		default: ASSERT(0); return _T("SI");
		}
	}();
#elif defined(_MGEN)
	ASSERT(0); 
	return _T("US");
#endif

	return strNationalAnnex;
}

std::vector<CString> CNationalAnnexTool::GetReportUnitString()
{
	std::vector<CString> vString;

	vString.push_back(_T("US"));
	vString.push_back(_T("SI"));

	return vString;
}

BOOL CNationalAnnexTool::IsAvailableNationalAnnex(int nNationalAnnex)
{
	// for Civil; CONCODE_EC2_2_05, EUROCODE2_2_05_PSC
	switch (nNationalAnnex)
	{
	case Recommended:   return TRUE; // 955
	case Austria:       return FALSE;
	case Belgium:       return FALSE;
	case Cyprus:        return FALSE;
	case CzechRepublic: return FALSE;
	case Denmark:       return FALSE;
	case Netherlands:   return FALSE;
	case Finland:       return FALSE;
	case France:        return TRUE; // 955
	case Germany:       return FALSE;
	case Greece:        return FALSE;
	case Ireland:       return FALSE;
	case Italy:         return TRUE; // 955
	case Luxembourg:    return FALSE;
	case Norway:        return FALSE;
	case Poland:        return FALSE;
	case Romania:       return FALSE;
	case Singapore:     return FALSE;
	case Slovakia:      return FALSE;
	case Slovenia:      return FALSE;
	case Spain:         return FALSE;
	case Sweden:        return FALSE;
	case UnitedKingdom: return TRUE; // 955
	default: ASSERT(0); return FALSE;
	}

	return FALSE;
}