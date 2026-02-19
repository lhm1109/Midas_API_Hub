// DgnCodeCtrl.cpp: implementation of the CDgnCodeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCodeCtrl.h"

#include "..\wg_db\DBCodeDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\dgnengine\idesign\DGN_lib\NationalAnnexInterface.h"
#include "..\dgnengine\idesign\DGN_def\LocaleDef.h"
#include "..\wg_sw\HighSpeedTendonGenerator\ExtractHstgLinkData.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\InitCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;

CDgnCodeCtrl::CDgnCodeCtrl(void)
{
}

CDgnCodeCtrl::~CDgnCodeCtrl(void)
{
}

int CDgnCodeCtrl::GetStlCodeID(const CString& strCode)
{
	if ( strCode == STLCODE_AIK_ASD83 ) { return AIK_ASD83; }
	if ( strCode == STLCODE_AISC_ASD89 ) { return AISC_ASD89; }
	if ( strCode == STLCODE_AISC_LRFD93 ) { return AISC_LRFD93; }
	if ( strCode == STLCODE_AISC_LRFD2K ) { return AISC_LRFD2K; }
	if ( strCode == STLCODE_AIK_LSD97 ) { return AIK_LSD97; }
	if ( strCode == STLCODE_KSCE_ASD96 ) { return KSCE_ASD96; }
	if ( strCode == STLCODE_BS5950_90 ) { return BS5950_90; }
	if ( strCode == STLCODE_BS5950_2K ) { return BS5950_2K; }
	if ( strCode == STLCODE_EC3 ) { return Eurocode3; }
	if ( strCode == STLCODE_EC3_05 ) { return Eurocode3_05; }
	if ( strCode == STLCODE_EC3_2_05 ) { return Eurocode3_2_05; }
	if ( strCode == STLCODE_IS800_2007 ) { return IS800_2007; }
	if ( strCode == STLCODE_IS800_1984 ) { return IS800_1984; }
	if ( strCode == STLCODE_AISI_CFSD86 ) { return AISI_CFSD86; }
	if ( strCode == STLCODE_AISI_CFSD08 ) { return AISI_CFSD08; }
	if ( strCode == STLCODE_AIK_CFSD98 ) { return AIK_CFSD98; }
	if ( strCode == STLCODE_AIJ_ASD02 ) { return AIJ_ASD02; }
	if ( strCode == STLCODE_GBJ17_88 ) { return GBJ17_88; }
	if ( strCode == STLCODE_GB50017_03 ) { return GB50017_03; }
	if ( strCode == STLCODE_GB50017_15 ) { return GB50017_15; }//add by maxiao(2015-9-25)GB50017-15
	if ( strCode == STLCODE_GB50017_17 ) { return GB50017_17; }//add bu xuezc(2018/2/8)GB50017-17
	if ( strCode == STLCODE_GB51249_2017 ) { return GB51249_2017; }//add bu tss
	if ( strCode == STLCODE_JGJ209_2010 ) { return JGJ209_2010; }
	if ( strCode == STLCODE_AASHTO_LRFD02 ) { return AASHTO_LS02; }
	if ( strCode == STLCODE_AASHTO_LRFD12 ) { return AASHTO_LS12; }
	if ( strCode == STLCODE_AASHTO_LRFD16 ) { return AASHTO_LS16; }
	if ( strCode == STLCODE_AASHTO_LRFD17 ) { return AASHTO_LS17; }
	if ( strCode == STLCODE_AASHTO_LRFD20 ) { return AASHTO_LS20; }
	if ( strCode == STLCODE_AASHTO_LRFD24 ) { return AASHTO_LS24; }
	if ( strCode == STLCODE_JTJ025_86 ) { return JTJ025_86; }
	if ( strCode == STLCODE_CSA_S16_01 ) { return CSA_S16_01; }
	if ( strCode == STLCODE_CSA_S6_14 ) { return CSA_S6_14_ST; }
	if ( strCode == STLCODE_CSA_S6_19 ) { return CSA_S6_19_ST; }
	if ( strCode == STLCODE_AASHTO_LFD96 ) { return AASHTO_LS96; }
	if ( strCode == STLCODE_AASHTO_ASD96 ) { return AASHTO_AS96; }
	if ( strCode == STLCODE_TWN_BRG_LSD90 ) { return TWN_BRG_LS90; }
	if ( strCode == STLCODE_TWN_BRG_ASD90 ) { return TWN_BRG_AS90; }
	if ( strCode == STLCODE_TWN_ASD90 ) { return TWN_ASD90; }
	if ( strCode == STLCODE_TWN_LSD90 ) { return TWN_LSD90; }
	if ( strCode == STLCODE_KSSC_ASD03 ) { return KSSC_ASD03; }
	if ( strCode == STLCODE_KSCE_ASD05 ) { return KSCE_ASD05; }
	if ( strCode == STLCODE_KSCE_RAIL_ASD04 ) { return KSCE_RAIL_ASD04; }
	if ( strCode == STLCODE_KEPCO97_1111 ) { return KEPCO97_1111; }
	if ( strCode == STLCODE_KSSC_LSD09 ) { return KSSC_LSD09; }
	if ( strCode == STLCODE_KSSC_LSD16 ) { return KSSC_LSD16; }
	if ( strCode == STLCODE_KDS_41_31_2019 ) { return KDS_41_31_2019; }
	if ( strCode == STLCODE_KDS_41_30_10_2022 ) { return KDS_41_30_10_2022; }
	if ( strCode == STLCODE_AISC_LRFD05 ) { return AISC_LRFD05; }
	if ( strCode == STLCODE_AISC_ASD05 ) { return AISC_ASD05; }
	if ( strCode == STLCODE_AISC_LRFD10 ) { return AISC_LRFD10; }
	if ( strCode == STLCODE_AISC_ASD10 ) { return AISC_ASD10; }
	if ( strCode == STLCODE_AISC_LRFD16 ) { return AISC_LRFD16; }
	if ( strCode == STLCODE_AISC_ASD16 ) { return AISC_ASD16; }
	if ( strCode == STLCODE_AISC_LRFD22 ) { return AISC_LRFD22; }
	if ( strCode == STLCODE_AISC_ASD22 ) { return AISC_ASD22; }
	if ( strCode == STLCODE_TWN_ASD96 ) { return TWN_ASD96; }
	if ( strCode == STLCODE_TWN_LSD96 ) { return TWN_LSD96; }
	if ( strCode == STLCODE_KSCE_ASD10 ) { return KSCE_ASD10; }
	if ( strCode == STLCODE_KDS_24_14_30_2019 ) { return KDS_24_14_30_2019; }
	if ( strCode == STLCODE_KDS_24_14_31_2018 ) { return KDS_24_14_31_2018; }
	if ( strCode == STLCODE_KSCE_RAIL_ASD11 ) { return KSCE_RAIL_ASD11; }
	if ( strCode == STLCODE_KSCE_LSD15 ) { return KR_BRG_LSD15; }
	if ( strCode == STLCODE_IRC_24_2010 ) { return  IRC24_2010; }
	if ( strCode == STLCODE_NSCP_2015_LRFD ) { return NSCP_2015_S_LRFD; }
	if ( strCode == STLCODE_NSCP_2015_ASD ) { return NSCP_2015_S_ASD; }
	if ( strCode == STLCODE_IRS_SBC ) { return IRS_SBC; }
	if ( strCode == STLCODE_JROAD_H14 ) { return JROAD_H14; }
	if ( strCode == STLCODE_JROAD_H24 ) { return JROAD_H24; }
	if ( strCode == STLCODE_JROAD_H29 ) { return JROAD_H29; }
	if ( strCode == STLCODE_JROAD_R07 ) { return JROAD_R07; }
	if ( strCode == STLCODE_SP_16_13330_2017 ) { return SP_16_13330_2017; }

	ASSERT(0);
	return 0;

	/*  아래 항목은 정해진 번호가 없음
	if(strCode == _T("Taiwan"))      ;
	if(strCode == _T("JTJ021-89"))   ;
	if(strCode == _T("CJJ77-98"))    ;
	if(strCode == _T("AASHTO-Std2K"));
	if(strCode == _T("KBC-ASD05"))   ;
	*/
}

int CDgnCodeCtrl::GetStlMatlCodeID(const CString& strCode)
{
	if ( strCode == MATLCODE_STL_KS_CIVIL ) return 0;
	if ( strCode == MATLCODE_STL_KS08_CIVIL ) return 1;
	if ( strCode == MATLCODE_STL_KS10_CIVIL ) return 2;
	if ( strCode == MATLCODE_STL_KSCE_LSD15 ) return 3;
	if ( strCode == MATLCODE_STL_KS18 ) return 4;

	ASSERT(0);
	return -1;
}

CString CDgnCodeCtrl::GetStlCodeName(int nCode)
{
	switch ( nCode )
	{
	case AIK_ASD83:   return STLCODE_AIK_ASD83;
	case AISC_ASD89:  return STLCODE_AISC_ASD89;
	case AISC_LRFD93: return STLCODE_AISC_LRFD93;
	case AISC_LRFD2K: return STLCODE_AISC_LRFD2K;
	case AIK_LSD97:   return STLCODE_AIK_LSD97;
	case KSCE_ASD96:  return STLCODE_KSCE_ASD96;
	case BS5950_90:   return STLCODE_BS5950_90;
	case BS5950_2K:   return STLCODE_BS5950_2K;
	case Eurocode3:   return STLCODE_EC3;
	case Eurocode3_05:   return STLCODE_EC3_05;
	case Eurocode3_2_05: return STLCODE_EC3_2_05;
	case IS800_2007:  return STLCODE_IS800_2007;
	case IS800_1984:  return STLCODE_IS800_1984;
	case AISI_CFSD86: return STLCODE_AISI_CFSD86;
	case AISI_CFSD08: return STLCODE_AISI_CFSD08;
	case AIK_CFSD98:  return STLCODE_AIK_CFSD98;
	case AIJ_ASD02: return STLCODE_AIJ_ASD02;
	case GBJ17_88:  return STLCODE_GBJ17_88;
	case GB50017_03: return STLCODE_GB50017_03;
	case GB50017_15: return STLCODE_GB50017_15; //add by maxiao(2015-9-25)GB50017-15
	case GB50017_17: return STLCODE_GB50017_17;//add by xuezc(2018/2/8)GB50017-17
	case GB51249_2017: return STLCODE_GB51249_2017;
	case JGJ209_2010: return STLCODE_JGJ209_2010;
	case AASHTO_LS02:  return STLCODE_AASHTO_LRFD02;
	case AASHTO_LS12:  return STLCODE_AASHTO_LRFD12;
	case AASHTO_LS16:  return STLCODE_AASHTO_LRFD16;
	case AASHTO_LS17:  return STLCODE_AASHTO_LRFD17;
	case AASHTO_LS20:  return STLCODE_AASHTO_LRFD20;
	case AASHTO_LS24:  return STLCODE_AASHTO_LRFD24;
	case JTJ025_86:    return STLCODE_JTJ025_86;
	case CSA_S16_01:   return STLCODE_CSA_S16_01;
	case CSA_S6_14_ST: return STLCODE_CSA_S6_14;
	case CSA_S6_19_ST: return STLCODE_CSA_S6_19;
	case AASHTO_LS96: return STLCODE_AASHTO_LFD96;
	case AASHTO_AS96: return STLCODE_AASHTO_ASD96;
	case TWN_BRG_LS90: return STLCODE_TWN_BRG_LSD90;
	case TWN_BRG_AS90: return STLCODE_TWN_BRG_ASD90;
	case TWN_ASD90: return STLCODE_TWN_ASD90;
	case TWN_LSD90: return STLCODE_TWN_LSD90;
	case KSCE_ASD05: return STLCODE_KSCE_ASD05;
	case KSCE_ASD10: return STLCODE_KSCE_ASD10;
	case KDS_24_14_30_2019: return STLCODE_KDS_24_14_30_2019;
	case KDS_24_14_31_2018: return STLCODE_KDS_24_14_31_2018;
	case KSCE_RAIL_ASD04: return STLCODE_KSCE_RAIL_ASD04;
	case KSCE_RAIL_ASD11: return STLCODE_KSCE_RAIL_ASD11;
	case KEPCO97_1111: return STLCODE_KEPCO97_1111;
	case KSSC_ASD03: return STLCODE_KSSC_ASD03;
	case KSSC_LSD09: return STLCODE_KSSC_LSD09;
	case KSSC_LSD16: return STLCODE_KSSC_LSD16;
	case KDS_41_31_2019: return STLCODE_KDS_41_31_2019;
	case KDS_41_30_10_2022: return STLCODE_KDS_41_30_10_2022;
	case AISC_LRFD05: return STLCODE_AISC_LRFD05;
	case AISC_ASD05:  return STLCODE_AISC_ASD05;
	case AISC_LRFD10: return STLCODE_AISC_LRFD10;
	case AISC_ASD10:  return STLCODE_AISC_ASD10;
	case AISC_LRFD16: return STLCODE_AISC_LRFD16;
	case AISC_ASD16:  return STLCODE_AISC_ASD16;
	case AISC_LRFD22: return STLCODE_AISC_LRFD22;
	case AISC_ASD22:  return STLCODE_AISC_ASD22;
	case TWN_ASD96: return STLCODE_TWN_ASD96;
	case TWN_LSD96: return STLCODE_TWN_LSD96;
	case KR_BRG_LSD15: return STLCODE_KSCE_LSD15;
	case IRC24_2010: return STLCODE_IRC_24_2010;
	case NSCP_2015_S_LRFD: return STLCODE_NSCP_2015_LRFD;
	case NSCP_2015_S_ASD:  return STLCODE_NSCP_2015_ASD;
	case IRS_SBC:  return STLCODE_IRS_SBC;
	case JROAD_H14: return STLCODE_JROAD_H14;
	case JROAD_H24: return STLCODE_JROAD_H24;
	case JROAD_H29: return STLCODE_JROAD_H29;
	case JROAD_R07: return STLCODE_JROAD_R07;
	case SP_16_13330_2017: return STLCODE_SP_16_13330_2017;
	default: { ASSERT(0); return _T(""); }
	}

	/* 아래 항목은 정해진 번호가 없음
	if(strCode == _T("Taiwan"))      ;
	if(strCode == _T("JTJ021-89"))   ;
	if(strCode == _T("CJJ77-98"))    ;
	if(strCode == _T("AASHTO-Std2K"));
	if(strCode == _T("KBC-ASD05"))   ;
	*/
}

TLocale CDgnCodeCtrl::GetStlLocalePref()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);
	switch ( rPref.DgnCode.nStlReportLanguage )
	{
	case EN_REPORT_English:
		return TLocale(eng);
	case EN_REPORT_Czech:
		return TLocale(cze);
	case EN_REPORT_France:
		return TLocale(fra);
	case EN_REPORT_Poland:
		return TLocale(pol);
	case EN_REPORT_Turkey:
		return TLocale(tur);
	case EN_REPORT_Vietnamese:
		return TLocale(vie);
	default:
		ASSERT(0);
		return TLocale(eng);
		break;
	}
}

int CDgnCodeCtrl::GetConCodeID(const CString& strCode, BOOL bCivil/*=FALSE*/)
{
	/// bCivil = String은 같은데 Gen/Civil을 구별할 필요가 있을 때 사용.

	/// CONCRETE DESIGN CODE IN GEN.
	if ( strCode == CONCODE_ACI318_89 ) { return ACI318_89; }
	if ( strCode == CONCODE_ACI318_95 ) { return ACI318_95; }
	if ( strCode == CONCODE_ACI318_99 ) { return ACI318_99; }
	if ( strCode == CONCODE_ACI318_02 ) { return ACI318_02; }
	if ( strCode == CONCODE_ACI318_05 ) { return ACI318_05; }
	if ( strCode == CONCODE_ACI318_08 ) { return ACI318_08; }
	if ( strCode == CONCODE_ACI318_11 ) { return ACI318_11; }
	if ( strCode == CONCODE_ACI318_14 ) { return ACI318_14; }
	if ( strCode == CONCODE_ACI318M_14 ) { return ACI318M_14; }
	if ( strCode == CONCODE_ACI318_19 ) { return ACI318_19; }
	if ( strCode == CONCODE_ACI318M_19 ) { return ACI318M_19; }
	if ( strCode == CONCODE_ACI318_25 ) { return ACI318_25; }
	if ( strCode == CONCODE_ACI318M_25 ) { return ACI318M_25; }
	if ( strCode == CONCODE_NSR_10 ) { return NSR_10; }
	if ( strCode == CONCODE_AIK_USD94 ) { return AIK_USD94; }
	if ( strCode == CONCODE_KSCE_USD96 ) { return KSCE_USD96; }
	if ( strCode == CONCODE_KCI_USD99 ) { return KCI_USD99; }
	if ( strCode == CONCODE_KCI_USD03 ) { return KCI_USD03; }
	if ( strCode == CONCODE_KCI_USD07 ) { return bCivil ? KCI_USD07_CVL : KCI_USD07; }
	if ( strCode == CONCODE_KCI_USD12 ) { return KCI_USD12; }
	if ( strCode == CONCODE_KDS_41_30_2018 ) { return KDS_41_30_2018; }
	if ( strCode == CONCODE_KDS_41_20_2022 ) { return KDS_41_20_2022; }
	if ( strCode == CONCODE_TWN_USD92 ) { return TWN_USD92; }
	if ( strCode == CONCODE_TWN_USD100 ) { return TWN_USD100; }
	if ( strCode == CONCODE_TWN_USD112 ) { return TWN_USD112; }
	if ( strCode == CONCODE_AIK_WSD2K ) { return AIK_WSD2K; }
	if ( strCode == CONCODE_AIJ_WSD99 ) { return AIJ_WSD99; }
	if ( strCode == CONCODE_BS8110_97 ) { return BS8110_97; }
	if ( strCode == CONCODE_EC2 ) { return Eurocode2; }
	if ( strCode == CONCODE_EC2_04 ) { return Eurocode2_04; }
	if ( strCode == CONCODE_IS456_2000 ) { return IS456_2000; }
	if ( strCode == CONCODE_GB50010_02 ) { return GB50010_02; }
	if ( strCode == CONCODE_GB50010_10 ) { return GB50010_10; }
	if ( strCode == CONCODE_GB50010_19 ) { return GB50010_19; }
	if ( strCode == CONCODE_CSA_A23_3_94 ) { return CSA_A233_94; }
	if ( strCode == CONCODE_NSCP_2015 ) { return NSCP_2015_C; }
	if ( strCode == CONCODE_NTC_DCEC_2017 ) { return NTC_DCEC_2017; }
	if ( strCode == CONCODE_NTC_DCEC_2023 ) { return NTC_DCEC_2023; }
	/// CONCRETE  DESIGN CODE IN CIVIL
	if ( strCode == CONCODE_AASHTO_LRFD02 ) { return AASHTO_LC02; }
	if ( strCode == CONCODE_AASHTO_LRFD07 ) { return AASHTO_LC07; }
	if ( strCode == CONCODE_AASHTO_LRFD12 ) { return AASHTO_LC12; }
	if ( strCode == CONCODE_AASHTO_LRFD16 ) { return AASHTO_LC16; }
	if ( strCode == CONCODE_AASHTO_LRFD17 ) { return AASHTO_LC17; }
	if ( strCode == CONCODE_AASHTO_LRFD20 ) { return AASHTO_LC20; }
	if ( strCode == CONCODE_AASHTO_LRFD24 ) { return AASHTO_LC24; }
	if ( strCode == CONCODE_AREMA_2023 ) { return AREMA23; }
	if ( strCode == CONCODE_AASHTO_LFD96 ) { return AASHTO_LC96; }
	if ( strCode == CONCODE_JTJ023_85 ) { return JTJ023_85; }
	if ( strCode == CONCODE_CSA_S6_00 ) { return CSA_S6_00; }
	if ( strCode == CONCODE_CSA_S6S1_10 ) { return CSA_S6_10; }
	if ( strCode == CONCODE_CSA_S6_14 ) { return CSA_S6_14; }
	if ( strCode == CONCODE_CSA_S6_19 ) { return CSA_S6_19; }
	if ( strCode == CONCODE_IRC21_2000 ) { return IRC_21_00; }
	if ( strCode == CONCODE_IRC112_2011 ) { return IRC_112_11; }
	if ( strCode == CONCODE_IRC112_2020 ) { return IRC_112_20; }
	if ( strCode == CONCODE_TWN_BRG_LSD90 ) { return TWN_BRG_LC90; }
	if ( strCode == CONCODE_EC2_2_05 ) { return Eurocode2_2_05; }
	if ( strCode == CONCODE_BS5400_90 ) { return BS5400_4_90; }
	if ( strCode == CONCODE_TMH07_89 ) { return TMH07_89; }
	if ( strCode == CONCODE_SNIP_2_05_03_84 ) { return SNiP_20503_84RC; }
	if ( strCode == CONCODE_SP_35_13330_2011 ) { return SP_35_13330_11RC; }
	if ( strCode == CONCODE_SNIP_2_05_03_84_MKS ) { return SNiP_20503_84RC_MKS; }
	if ( strCode == CONCODE_SP_35_13330_2011_MKS ) { return SP_35_13330_11RC_MKS; }
	if ( strCode == CONCODE_KSCE_USD05 ) { return KSCE_RC_USD05; }
	if ( strCode == CONCODE_KSCE_USD10 ) { return KSCE_RC_USD10; }
	if ( strCode == CONCODE_KSCE_RAIL_USD04 ) { return KSCE_RC_RAIL_USD04; }
	if ( strCode == CONCODE_KSCE_RAIL_USD11 ) { return KSCE_RC_RAIL_USD11; }
	if ( strCode == CONCODE_KSCE_LSD15 ) { return KR_LRFD11_RC; }
	if ( strCode == CONCODE_KDS_24_14_21_2022 ) { return KDS_24_14_21_2022; }
	if ( strCode == CONCODE_KDS_14_20_00_2022 ) { return KDS_14_20_00_2022; }
	if ( strCode == CONCODE_IRS ) { return IRS_RC; }
	/// SRC DESIGN CODE IN GEN.
	if ( strCode == SRCCODE_AIK_SRC2K ) { return AIK_SRC2K; }
	if ( strCode == SRCCODE_AIJ_SRC01 ) { return AIJ_SRC01; }
	if ( strCode == SRCCODE_TWN_SRC92 ) { return TWN_SRC92; }
	if ( strCode == SRCCODE_TWN_SRC100 ) { return TWN_SRC100; }
	else if ( strCode == CONCODE_SP_63_13330_2018 ) { return SP_63_1330_2018; }

	ASSERT(0);
	return 0;

	/* 아래 항목은 정해진 번호가 없음
	if(strCode==_T("KRTA-BRG2K"))	return = ;
	if(strCode==_T("JTJ021-89"))	return = ;
	if(strCode==_T("CJJ77-98"))	return = ;
	if(strCode==_T("AASHTO-Std2K"))	return = ;
	if(strCode==_T("IRC:6-2000"))	return = ;
	if(strCode==_T("Taiwan"))	return = ;
	if(strCode==_T("JARA02"))	return = ;
	if(strCode==_T("KSCE-USD03"))	return = ;
	if(strCode==_T("JTG D60-04"))	return = ;
	if(strCode==_T("JTG D62-04"))	return = ;
	if(strCode==_T("KBC-USD05"))	return = ;
	if(strCode==_T("TB 10002.3-05"))	return = ;
	if(strCode==_T("TB 10002.1-05"))	return = ;
	*/
}

bool CDgnCodeCtrl::IsConCodeUSD(const CString& strCode)
{
	if ( strCode == CONCODE_ACI318_89 ) { return true; }
	if ( strCode == CONCODE_ACI318_95 ) { return true; }
	if ( strCode == CONCODE_ACI318_99 ) { return true; }
	if ( strCode == CONCODE_ACI318_02 ) { return true; }
	if ( strCode == CONCODE_ACI318_05 ) { return true; }
	if ( strCode == CONCODE_ACI318_08 ) { return true; }
	if ( strCode == CONCODE_ACI318_11 ) { return true; }
	if ( strCode == CONCODE_ACI318_14 ) { return true; }
	if ( strCode == CONCODE_ACI318M_14 ) { return true; }
	if ( strCode == CONCODE_ACI318_19 ) { return true; }
	if ( strCode == CONCODE_ACI318M_19 ) { return true; }
	if ( strCode == CONCODE_ACI318_25 ) { return true; }
	if ( strCode == CONCODE_ACI318M_25 ) { return true; }
	if ( strCode == CONCODE_NSR_10 ) { return true; }
	if ( strCode == CONCODE_AIK_USD94 ) { return true; }
	if ( strCode == CONCODE_KSCE_USD96 ) { return true; }
	if ( strCode == CONCODE_KCI_USD99 ) { return true; }
	if ( strCode == CONCODE_KCI_USD03 ) { return true; }
	if ( strCode == CONCODE_KCI_USD07 ) { return true; }
	if ( strCode == CONCODE_KCI_USD12 ) { return true; }
	if ( strCode == CONCODE_KDS_41_30_2018 ) { return true; }
	if ( strCode == CONCODE_KDS_41_20_2022 ) { return true; }
	if ( strCode == CONCODE_TWN_USD92 ) { return true; }
	if ( strCode == CONCODE_TWN_USD100 ) { return true; }
	if ( strCode == CONCODE_TWN_USD112 ) { return true; }
	if ( strCode == CONCODE_NSCP_2015 ) { return true; }
	if ( strCode == CONCODE_NTC_DCEC_2017 ) { return true; }
	if ( strCode == CONCODE_NTC_DCEC_2023 ) { return true; }

	return false;
}

bool CDgnCodeCtrl::IsConCodeTorsionCheck(int nCode)
{
	switch ( nCode )
	{
	case TWN_USD92:
	case TWN_USD100:
	case TWN_USD112:
	case GB50010_10:
	case GB50010_19:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case IS456_2000:
	case IRS_RC:
	case NSCP_2015_C:
	case NTC_DCEC_2017:
	case NTC_DCEC_2023:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case AREMA23:
		return true;
	default:
		return false;
	}
}

bool CDgnCodeCtrl::IsConCodeNotLimitCmGT04(int nCode)
{
	/// 횡구속 골조의 확대모멘트 계산시, 아래처럼 Cm의 하한값을 고려하는지의 여부.
	/// [ACI318-05] 10.12.3.1, Cm = 0.6+0.4M1/M2 _T(">= 0.4").
	switch ( nCode )
	{
	case ACI318_08:  /// 10.10.6.4.
	case ACI318_11:
	case ACI318_14:  /// 6.6.4.5.3
	case ACI318M_14: /// 6.6.4.5.3
	case ACI318_19:  /// 6.6.4.5.3
	case ACI318M_19: /// 6.6.4.5.3
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:     /// see NSR-10, C.10.10.6.4
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case NSCP_2015_C:
	case NTC_DCEC_2017:
		return true;
	default:
		return false;
	}
}

bool CDgnCodeCtrl::IsConCodeLimit2ndMomentRatio14(int nCode)
{
	/// 확대모멘트가 확대전 모멘트의 1.4배보다 큰 경우를 제한하는지의 여부.
	/// [ACI318-11] 10.10.2.1, Total moment including 2nd-order effects in compression members, (...)
	///   shall not exceed 1.4 times the moment due to 1st-order effects.
	switch ( nCode )
	{
	case ACI318_08:  /// 10.10.2.1.
	case ACI318_11:
	case ACI318_14:  /// 6.2.6.
	case ACI318M_14: /// 6.2.6.
	case ACI318_19:  /// 6.2.6.
	case ACI318M_19: /// 6.2.6.
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
		return true;
	default:
		return false;
	}
}

bool CDgnCodeCtrl::IsConCodeNotColmSwaySlenGT35(int nCode)
{
	/// 비횡구속 골조의 압축부재의 세장비에 따라 횡구속 골조의 식으로 추가로 확대하는지의 여부.
	/// [ACI318-02] lu/r >= 35/SQRT[Pu/(fc*Ag)], (...) using 10.12.3 -> Mc = δns*(M2ns + δs*M2s)
	switch ( nCode )
	{
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case NTC_DCEC_2017:
		return true;
	default:
		return false;
	}
}

bool CDgnCodeCtrl::IsConCodeServCheck(int nCode)
{
	switch ( nCode )
	{
	case Eurocode2:
	case Eurocode2_04:
	case IS456_2000:
	case IRS_RC:
		return true;
	default:
		return false;
	}
}

bool CDgnCodeCtrl::IsConCodeSlabForceWoodArmer(int nCode)
{
	switch ( nCode )
	{
	case Eurocode2_04:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case IS456_2000:
	case NTC_DCEC_2017:
		return true;
	default:
		return false;
	}
}

bool CDgnCodeCtrl::IsConCodePhiVariation(int nCode)
{
	switch ( nCode )
	{
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C: /// [NSCP 2015, Table 421.2.2]
	case NTC_DCEC_2017:
		{
			return true;
		}
	default:
		{
			return false;
		}
	}
}

bool CDgnCodeCtrl::IsConCodeRhomax075(int nCode)
{
	switch ( nCode )
	{
	case ACI318_02:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case NTC_DCEC_2017:
		{
			return false;
		}
	default:
		{
			return true;
		}
	}
}

bool CDgnCodeCtrl::IsConCodeMnIMF4SSWB(int nCode)
{
	switch ( nCode )
	{
	case ACI318_89:
	case ACI318_95:
	case ACI318_99:
	case ACI318_02:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case NTC_DCEC_2017:
		{
			return true;
		}
	default:
		{
			return false;
		}
	}
}

bool CDgnCodeCtrl::IsConCode08DSectSR4Shear(int nCode)
{
	switch ( nCode )
	{
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:  /// 22.5.2.2
	case ACI318M_14: /// 22.5.2.2
	case ACI318_19:  /// 22.5.2.2
	case ACI318M_19: /// 22.5.2.2
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case NTC_DCEC_2017:
		{
			return true;
		}
	default:
		{
			return false;
		}
	}
}

bool CDgnCodeCtrl::IsConCodeSpecialWall(int nCode)
{
	switch ( nCode )
	{
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case TWN_USD112:
	case NSCP_2015_C:
	case NTC_DCEC_2017:
	case NTC_DCEC_2023:
		return true;
	default:
		return false;
	}
}

bool CDgnCodeCtrl::IsConCodeUseAlpha2ColmSSWB(const CString& strCode)
{
	const int nCode = GetConCodeID(strCode);
	return IsConCodeUseAlpha2ColmSSWB(nCode);
}

bool CDgnCodeCtrl::IsConCodeUseAlpha2ColmSSWB(int nCode)
{
	switch ( nCode )
	{
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSCP_2015_C:
	case NTC_DCEC_2017:
	case NTC_DCEC_2023:
	case TWN_USD112:
		return true;
	default:
		return false;
	}
}

bool CDgnCodeCtrl::IsConCodeUseMeshedPM4DgnEngine(const CString& strCode)
{
	return DoPerformModuleConc(strCode);
}

bool CDgnCodeCtrl::IsConCodeGenSectColm(const int nCode)
{
	switch ( nCode )
	{
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case AS5100_5_2017:
	case BS5400_4_90:
	case Eurocode2_2_05:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
	case TMH07_89:
		{
			return true;
		}
		break;
	}
	return false;
}

bool CDgnCodeCtrl::IsConCodeRussia(const int nCode)
{
	switch ( nCode )
	{
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			return true;
		}
		break;
	}
	return false;
}

bool CDgnCodeCtrl::IsConCodeMultiOutput(const int nCode)
{
	switch ( nCode )
	{
	case AS5100_5_2017:
		{
			return true;
		}
		break;
	}
	return false;
}

CString CDgnCodeCtrl::GetConCodeName(int nCode)
{
	switch ( nCode )
	{
	case AASHTO_LC02:          return CONCODE_AASHTO_LRFD02;
	case AASHTO_LC07:          return CONCODE_AASHTO_LRFD07;
	case AASHTO_LC12:          return CONCODE_AASHTO_LRFD12;
	case AASHTO_LC16:          return CONCODE_AASHTO_LRFD16;
	case AASHTO_LC17:          return CONCODE_AASHTO_LRFD17;
	case AASHTO_LC20:          return CONCODE_AASHTO_LRFD20;
	case AASHTO_LC24:          return CONCODE_AASHTO_LRFD24;
	case AASHTO_LC96:          return CONCODE_AASHTO_LFD96;
	case ACI318_02:            return CONCODE_ACI318_02;
	case ACI318_05:            return CONCODE_ACI318_05;
	case ACI318_08:            return CONCODE_ACI318_08;
	case ACI318_11:            return CONCODE_ACI318_11;
	case ACI318_14:            return CONCODE_ACI318_14;
	case ACI318_19:            return CONCODE_ACI318_19;
	case ACI318_25:            return CONCODE_ACI318_25;
	case ACI318_89:            return CONCODE_ACI318_89;
	case ACI318_95:            return CONCODE_ACI318_95;
	case ACI318_99:            return CONCODE_ACI318_99;
	case ACI318M_14:           return CONCODE_ACI318M_14;
	case ACI318M_19:           return CONCODE_ACI318M_19;
	case ACI318M_25:           return CONCODE_ACI318M_25;
	case AIJ_SRC01:            return SRCCODE_AIJ_SRC01;
	case AIJ_WSD99:            return CONCODE_AIJ_WSD99;
	case AIK_SRC2K:            return SRCCODE_AIK_SRC2K;
	case AIK_USD94:            return CONCODE_AIK_USD94;
	case AIK_WSD2K:            return CONCODE_AIK_WSD2K;
	case AREMA23:              return CONCODE_AREMA_2023;
	case AS5100_5_2017:        return CONCODE_AS5100_5_17;
	case BS5400_4_90:          return CONCODE_BS5400_90;
	case BS8110_97:            return CONCODE_BS8110_97;
	case CSA_A233_94:          return CONCODE_CSA_A23_3_94;
	case CSA_S6_00:            return CONCODE_CSA_S6_00;
	case CSA_S6_10:            return CONCODE_CSA_S6S1_10;
	case CSA_S6_14:            return CONCODE_CSA_S6_14;
	case CSA_S6_14_RC:         return CONCODE_CSA_S6_14;
	case CSA_S6_19:            return CONCODE_CSA_S6_19;
	case CSA_S6_19_RC:         return CONCODE_CSA_S6_19;
	case Eurocode2:            return CONCODE_EC2;
	case Eurocode2_04:         return CONCODE_EC2_04;
	case Eurocode2_2_05:       return CONCODE_EC2_2_05;
	case GB50010_02:           return CONCODE_GB50010_02;
	case GB50010_10:           return CONCODE_GB50010_10;
	case GB50010_19:           return CONCODE_GB50010_19;
	case IRC_112_11:           return CONCODE_IRC112_2011;
	case IRC_112_20:           return CONCODE_IRC112_2020;
	case IRC_21_00:            return CONCODE_IRC21_2000;
	case IS456_2000:           return CONCODE_IS456_2000;
	case JTJ023_85:            return CONCODE_JTJ023_85;
	case KCI_USD03:            return CONCODE_KCI_USD03;
	case KCI_USD07:            return CONCODE_KCI_USD07;
	case KCI_USD07_CVL:        return CONCODE_KCI_USD07;
	case KCI_USD12:            return CONCODE_KCI_USD12;
	case KCI_USD99:            return CONCODE_KCI_USD99;
	case KDS_24_14_21_2022:    return CONCODE_KDS_24_14_21_2022;
	case KDS_14_20_00_2022:    return CONCODE_KDS_14_20_00_2022;
	case KDS_41_20_2022:       return CONCODE_KDS_41_20_2022;
	case KDS_41_30_2018:       return CONCODE_KDS_41_30_2018;
	case KR_LRFD11_RC:         return CONCODE_KSCE_LSD15;
	case KSCE_RC_RAIL_USD04:   return CONCODE_KSCE_RAIL_USD04;
	case KSCE_RC_RAIL_USD11:   return CONCODE_KSCE_RAIL_USD11;
	case KSCE_RC_USD05:        return CONCODE_KSCE_USD05;
	case KSCE_RC_USD10:        return CONCODE_KSCE_USD10;
	case KSCE_USD96:           return CONCODE_KSCE_USD96;
	case NSCP_2015_C:          return CONCODE_NSCP_2015;
	case NSR_10:               return CONCODE_NSR_10;
	case NTC_DCEC_2017:        return CONCODE_NTC_DCEC_2017;
	case NTC_DCEC_2023:        return CONCODE_NTC_DCEC_2023;
	case SNiP_20503_84RC:      return CONCODE_SNIP_2_05_03_84;
	case SNiP_20503_84RC_MKS:  return CONCODE_SNIP_2_05_03_84_MKS;
	case SP_35_13330_11RC:     return CONCODE_SP_35_13330_2011;
	case SP_35_13330_11RC_MKS: return CONCODE_SP_35_13330_2011_MKS;
	case TMH07_89:			   return CONCODE_TMH07_89;
	case TWN_BRG_LC90:         return CONCODE_TWN_BRG_LSD90;
	case TWN_SRC100:           return SRCCODE_TWN_SRC100;
	case TWN_SRC92:            return SRCCODE_TWN_SRC92;
	case TWN_USD100:           return CONCODE_TWN_USD100;
	case TWN_USD112:           return CONCODE_TWN_USD112;
	case TWN_USD92:            return CONCODE_TWN_USD92;
	default: ASSERT(0); break;
	}

	return _T("");

	/* 아래 항목은 정해진 번호가 없음
	if(strCode==_T("KRTA-BRG2K"))	return = ;
	if(strCode==_T("JTJ021-89"))	return = ;
	if(strCode==_T("CJJ77-98"))	return = ;
	if(strCode==_T("AASHTO-Std2K"))	return = ;
	if(strCode==_T("IRC:6-2000"))	return = ;
	if(strCode==_T("Taiwan"))	return = ;
	if(strCode==_T("JARA02"))	return = ;
	if(strCode==_T("KSCE-USD03"))	return = ;
	if(strCode==_T("JTG D60-04"))	return = ;
	if(strCode==_T("JTG D62-04"))	return = ;
	if(strCode==_T("KBC-USD05"))	return = ;
	if(strCode==_T("TB 10002.3-05"))	return = ;
	if(strCode==_T("TB 10002.1-05"))	return = ;
	*/
}

CString CDgnCodeCtrl::GetConCodeNumber4Log(const CString& strPrefix, const CString& strCode)
{
	// PARKHJ-20111007 : 번호 할당시 DgnStruct.h 참고!

	if ( strCode == _T("ACI318-89") ) return strPrefix + _T("101");
	if ( strCode == _T("AIK-USD94") ) return strPrefix + _T("102");
	if ( strCode == _T("ACI318-95") ) return strPrefix + _T("103");
	if ( strCode == _T("KSCE-USD96") ) return strPrefix + _T("104");
	if ( strCode == _T("KCI-USD99") ) return strPrefix + _T("105");
	if ( strCode == _T("ACI318-99") ) return strPrefix + _T("106");
	if ( strCode == _T("AIK-WSD2K") ) return strPrefix + _T("107");
	if ( strCode == _T("AIJ-WSD99") ) return strPrefix + _T("108");
	if ( strCode == _T("BS8110-97") ) return strPrefix + _T("109");
	if ( strCode == _T("Eurocode2") ) return strPrefix + _T("110");
	if ( strCode == _T("GB50010-02") ) return strPrefix + _T("111");
	if ( strCode == _T("ACI318-02") ) return strPrefix + _T("112");
	if ( strCode == _T("CSA-A233-94") ) return strPrefix + _T("113");
	if ( strCode == _T("IS456-2000") ) return strPrefix + _T("114");
	if ( strCode == _T("TWN-USD92") ) return strPrefix + _T("115");
	if ( strCode == _T("KCI-USD03") ) return strPrefix + _T("116");
	if ( strCode == _T("KSCE-RC-USD05") ) return strPrefix + _T("117");
	if ( strCode == _T("KSCE-RC-RAIL-USD04") ) return strPrefix + _T("118");
	if ( strCode == _T("Eurocode2:04") ) return strPrefix + _T("119");
	if ( strCode == _T("ACI318-05") ) return strPrefix + _T("120");
	if ( strCode == _T("KCI-USD07") ) return strPrefix + _T("121");
	if ( strCode == _T("Eurocode2-2:05") ) return strPrefix + _T("122");
	if ( strCode == CONCODE_BS5400_90 ) return strPrefix + _T("184");
	if ( strCode == CONCODE_TMH07_89 ) return strPrefix + _T("189");
	if ( strCode == _T("KCI-USD12") ) return strPrefix + _T("124");
	if ( strCode == _T("TWN-USD100") ) return strPrefix + _T("125");
	if ( strCode == _T("GB/T50010-10") ) return strPrefix + _T("126");
	if ( strCode == _T("ACI318-08") ) return strPrefix + _T("127");
	if ( strCode == _T("ACI318-11") ) return strPrefix + _T("128");
	if ( strCode == _T("NSR-10") ) return strPrefix + _T("129");
	if ( strCode == _T("ACI318-14") ) return strPrefix + _T("130");
	if ( strCode == _T("ACI318M-14") ) return strPrefix + _T("131");
	if ( strCode == _T("ACI318-19") ) return strPrefix + _T("136");
	if ( strCode == _T("ACI318M-19") ) return strPrefix + _T("137");
	if ( strCode == _T("ACI318-25") ) return strPrefix + _T("187");
	if ( strCode == _T("ACI318M-25") ) return strPrefix + _T("188");
	if ( strCode == _T("TWN-USD112") ) return strPrefix + _T("138");
	if ( strCode == CONCODE_NSCP_2015 ) return strPrefix + _T("134");
	if ( strCode == _T("AASHTO-LC02") ) return strPrefix + _T("151");
	if ( strCode == _T("JTJ023-85") ) return strPrefix + _T("152");
	if ( strCode == _T("IRC-21-00") ) return strPrefix + _T("153");
	if ( strCode == _T("CSA-S6-00") ) return strPrefix + _T("154");
	if ( strCode == _T("CSA-S6-14") ) return strPrefix + _T("180");
	if ( strCode == _T("CSA-S6-19") ) return strPrefix + _T("185");
	if ( strCode == _T("AASHTO-LC96") ) return strPrefix + _T("155");
	if ( strCode == _T("TWN-BRG-LC90") ) return strPrefix + _T("156");
	if ( strCode == _T("AASHTO-LRFD12") ) return strPrefix + _T("161");
	if ( strCode == _T("AASHTO-LRFD16") ) return strPrefix + _T("166");
	if ( strCode == _T("AASHTO-LRFD17") ) return strPrefix + _T("167");
	if ( strCode == _T("AASHTO-LRFD20") ) return strPrefix + _T("183");
	if ( strCode == CONCODE_AASHTO_LRFD24 ) return strPrefix + _T("192");
	if ( strCode == _T("JARA02") ) return strPrefix + _T("301");
	if ( strCode == _T("KSCE-USD03") ) return strPrefix + _T("302");
	if ( strCode == _T("KSCE-USD05") ) return strPrefix + _T("303");
	if ( strCode == _T("KSCE-RAIL-USD04") ) return strPrefix + _T("304");
	if ( strCode == _T("JTG-D62-04") ) return strPrefix + _T("305");

	return strPrefix + _T("Err");
}

TLocale CDgnCodeCtrl::GetConLocalePref()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);
	switch ( rPref.DgnCode.nConcReportLanguage )
	{
	case EN_REPORT_English:
		return TLocale(eng);
	case EN_REPORT_Czech:
		return TLocale(cze);
	case EN_REPORT_France:
		return TLocale(fra);
	case EN_REPORT_Poland:
		return TLocale(pol);
	case EN_REPORT_Turkey:
		return TLocale(tur);
	case EN_REPORT_Vietnamese:
		return TLocale(vie);
	default:
		ASSERT(0);
		return TLocale(eng);
		break;
	}
}

TLocale CDgnCodeCtrl::GetPscLocalePref()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);
	switch ( rPref.DgnCode.nPscReportLanguage )
	{
	case EN_REPORT_English:
		return TLocale(eng);
	case EN_REPORT_Czech:
		return TLocale(cze);
	case EN_REPORT_France:
		return TLocale(fra);
	case EN_REPORT_Poland:
		return TLocale(pol);
	case EN_REPORT_Turkey:
		return TLocale(tur);
	case EN_REPORT_Korean:
		return TLocale(kor);
	default:
		ASSERT(0);
		return TLocale(eng);
		break;
	}
}

int CDgnCodeCtrl::GetPscReportUnit()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);
	return rPref.DgnCode.nPscReportUnit;
}

int CDgnCodeCtrl::GetRcReportUnit()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);
	return rPref.DgnCode.nConcReportUnit;
}

int CDgnCodeCtrl::GetStlReportUnit()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);
	return rPref.DgnCode.nStlReportUnit;
}

int CDgnCodeCtrl::GetSrcCodeID(const CString& strCode)
{
	if ( strCode == SRCCODE_SSRC79 ) { return SSRC79; }
	if ( strCode == SRCCODE_AIK_SRC2K ) { return AIK_SRC2K; }
	if ( strCode == SRCCODE_AIJ_SRC01 ) { return AIJ_SRC01; }
	if ( strCode == SRCCODE_JGJ138_01 ) { return JGJ138_01; }
	if ( strCode == SRCCODE_TWN_SRC92 ) { return TWN_SRC92; }
	if ( strCode == SRCCODE_TWN_SRC100 ) { return TWN_SRC100; }

	ASSERT(0);
	return 0;
	/* 아래 항목은 정해진 번호가 없음
	if(strCode==_T("KBC-SRC05"))	  return = ;
	*/
}

CString CDgnCodeCtrl::GetSrcCodeName(int nCode)
{
	switch ( nCode )
	{
	case SSRC79: { return SRCCODE_SSRC79; }
	case AIK_SRC2K: { return SRCCODE_AIK_SRC2K; }
	case AIJ_SRC01: { return SRCCODE_AIJ_SRC01; }
	case JGJ138_01: { return SRCCODE_JGJ138_01; }
	case TWN_SRC92: { return SRCCODE_TWN_SRC92; }
	case TWN_SRC100: { return SRCCODE_TWN_SRC100; }
	default: { ASSERT(0); return _T(""); }
	}

	/* 아래 항목은 정해진 번호가 없음
	if(strCode==_T("KBC-SRC05"))	  return = ;
	*/
}

BOOL CDgnCodeCtrl::GetDefaultCpgdFactor(int nCsgCodeNo, int nSubCode, T_CPGD_D& cpgdD)
{
	switch ( nCsgCodeNo )
	{
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
		{
			cpgdD.dPhi_f = 0.95;
			cpgdD.dPhi_v = 0.95;
			cpgdD.dPhi_c = 0.9;
			cpgdD.dPhi_s_Tens = 0.95;
			cpgdD.dPhi_s_Tos = 0.9;
			cpgdD.dPhi_y = 0.9;
			cpgdD.dPhi_se = 0.85;
			cpgdD.dPhi_b = 0.75;
			cpgdD.dPhi_u = 0.75;
		}
		break;
	case AASHTO_LRFD07_CSG:
	case AASHTO_LRFD12_CSG:
	case AASHTO_LRFD16_CSG:
	case AASHTO_LRFD17_CSG:
	case AASHTO_LRFD20_CSG:
		{
			cpgdD.dPhi_f = 1.00;
			cpgdD.dPhi_v = 1.00;
			cpgdD.dPhi_c = 0.90;
			cpgdD.dPhi_s_Tens = 0.95;
			cpgdD.dPhi_s_Tos = 0.9;
			cpgdD.dPhi_y = 0.95;
			cpgdD.dPhi_se = 0.85;
			cpgdD.dPhi_b = 1.00;
			cpgdD.dPhi_u = 0.80;
		}
		break;
	case EUROCODE4_2_04_CSG:
		{
			cpgdD.dPhi_f = 1.00;
			cpgdD.dPhi_v = 1.00;
			cpgdD.dPhi_c = 0.90;
			cpgdD.dPhi_s_Tens = 0.95;
			cpgdD.dPhi_s_Tos = 0.9;
			cpgdD.dPhi_y = 0.95;
			cpgdD.dPhi_se = 0.85;
			cpgdD.dPhi_b = 0.75;
			cpgdD.dPhi_u = 0.80;

			auto* pNA = dgn::lib::CNationalAnnexInterface::Instance();
			const auto StlCode = pNA->GetBridgeCode(dgn::def::enDgn::STL, nSubCode);
			if ( pNA == nullptr )
			{
				ASSERT(0); return TRUE;
			}
			//필요시 추가해줄것
			pNA->SetCode(StlCode);
			cpgdD.dGamma_M0 = pNA->GetGammaM0();
			cpgdD.dGamma_M1 = pNA->GetGammaM1();

			const auto RcCode = pNA->GetBridgeCode(dgn::def::enDgn::RCS, nSubCode);
			if ( pNA == nullptr )
			{
				ASSERT(0); return TRUE;
			}

			pNA->SetCode(RcCode);
			cpgdD.dk3 = pNA->Getk3StressLimitation();
		}
		break;
	case AS5100_6_17_CSG:
		{
			cpgdD.dPhi_f = 0.9;   // flexure
			cpgdD.dPhi_v = 0.9;	  // shear
			cpgdD.dPhi_b = 0.9;	  // stiffener
			cpgdD.dPhi_se = 1.0;  // shear connector
		}
		break;
	default:
		{
			cpgdD.dPhi_f = 1.00;
			cpgdD.dPhi_v = 1.00;
			cpgdD.dPhi_c = 0.90;
			cpgdD.dPhi_s_Tens = 0.95;
			cpgdD.dPhi_s_Tos = 0.9;
			cpgdD.dPhi_y = 0.95;
			cpgdD.dPhi_se = 0.85;
			cpgdD.dPhi_b = 0.75;
			cpgdD.dPhi_u = 0.80;
		}
		break;
	}

	return TRUE;
}

TLocale CDgnCodeCtrl::GetCpgLocalePref()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);
	switch ( rPref.DgnCode.nPgReportLanguage )
	{
	case EN_REPORT_English:
		return TLocale(eng);
	case EN_REPORT_Czech:
		return TLocale(cze);
	case EN_REPORT_France:
		return TLocale(fra);
	case EN_REPORT_Poland:
		return TLocale(pol);
	case EN_REPORT_Turkey:
		return TLocale(tur);
	case EN_REPORT_Korean:
		return TLocale(kor);
	case EN_REPORT_Vietnamese:
		return TLocale(vie);
	default:
		ASSERT(0);
		return TLocale(eng);
		break;
	}
}

int CDgnCodeCtrl::GetCpgReportUnit()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);
	return rPref.DgnCode.nPgReportUnit;
}

int CDgnCodeCtrl::GetCfsCodeID(const CString& strCode)
{
	if ( strCode == CFSCODE_AIK_CFSD98 ) { return AIK_CFSD98; }
	if ( strCode == CFSCODE_AISI_CFSD86 ) { return AISI_CFSD86; }
	if ( strCode == CFSCODE_AISI_CFSD08 ) { return AISI_CFSD08; }
	if ( strCode == CFSCODE_EC3_06 ) { return Eurocode3_1_3_06; }
	if ( strCode == CFSCODE_GB50018_02 ) { return GB50018_02; }
	if (strCode == CFSCODE_GB50018_25) { return GB50018_25; }
	ASSERT(0);
	return 0;
}

CString CDgnCodeCtrl::GetCfsCodeName(int nCode)
{
	switch ( nCode )
	{
	case AIK_CFSD98: return CFSCODE_AIK_CFSD98;
	case AISI_CFSD86: return CFSCODE_AISI_CFSD86;
	case AISI_CFSD08: return CFSCODE_AISI_CFSD08;
	case Eurocode3_1_3_06: return CFSCODE_EC3_06;
	case GB50018_02:  return CFSCODE_GB50018_02;
	case GB50018_25:  return CFSCODE_GB50018_25;
	default: { ASSERT(0); return _T(""); }
	}
}

bool CDgnCodeCtrl::DoPerformModuleConc(const CString& strCode)
{
	int nCode = GetConCodeID(strCode);
	return IsIDesignModule(nCode);
}

bool CDgnCodeCtrl::DoPerformModuleStl(const CString& strCode)
{
	int nCode = GetStlCodeID(strCode);
	return IsIDesignModule(nCode);
}

bool CDgnCodeCtrl::DoPerformModuleSRC(const CString& strCode)
{
	int nCode = GetSrcCodeID(strCode);
	return IsIDesignModule(nCode);
}

bool CDgnCodeCtrl::IsIDesignModule(int nCode)
{
	const CString& strEnv = CProduct::GetTestEnvValue(_ULS(LegacyCalcModule));
	if ( strEnv == _ULS(yes) )
	{
		return false;
	}

	switch ( nCode )
	{
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case TWN_USD112:
	case NTC_DCEC_2023:
	case KDS_41_20_2022:
	case KDS_24_14_31_2018:
	case Eurocode2_2_05:
	case BS5400_4_90:
	case Eurocode3_2_05:
	case EUROCODE4_2_04_CSG:
	case AS5100_6_17_CSG:
	case AASHTO_LS17:
	case AASHTO_LS20:
	case AASHTO_LS24:
	case AREMA23:
	case TMH07_89:
		return true;
	}

	return false;
}

bool CDgnCodeCtrl::IsDetailReportOnPerformModule(int nCode)
{
	switch ( nCode )
	{
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case TWN_USD112:
	case NTC_DCEC_2023:
	case KDS_41_20_2022:
	case KDS_24_14_31_2018:
	case Eurocode2_2_05:
	case BS5400_4_90:
	case TMH07_89:
	case Eurocode3_2_05:
	case EUROCODE4_2_04_CSG:
	case AS5100_6_17_CSG:
		return true;
	}

	return false;
}

int CDgnCodeCtrl::GetCodeUnitLeng(const int nCode)
{
	switch ( nCode )
	{
	case AIK_ASD83:
	case AIK_CFSD98:
	case AIK_LSD97:
	case KSCE_ASD96:
	case TWN_ASD90:
	case TWN_ASD96:
	case TWN_BRG_AS90:
	case TWN_BRG_LS90:
	case TWN_LSD90:
	case TWN_LSD96:
		{
			return D_UNITSYS_LENGTH_INDEX_CM;
		}
		break;
	case AASHTO_AS96:
	case AASHTO_LS02:
	case AASHTO_LS12:
	case AASHTO_LS16:
	case AASHTO_LS17:
	case AASHTO_LS20:
	case AASHTO_LS24:
	case AASHTO_LS96:
	case AISC_ASD05:
	case AISC_ASD10:
	case AISC_ASD16:
	case AISC_ASD22:
	case AISC_ASD89:
	case AISC_LRFD05:
	case AISC_LRFD10:
	case AISC_LRFD16:
	case AISC_LRFD22:
	case AISC_LRFD2K:
	case AISC_LRFD93:
	case AISI_CFSD08:
	case AISI_CFSD86:
		{
			return D_UNITSYS_LENGTH_INDEX_IN;
		}
		break;
	case AIJ_ASD02:
	case AS5100_5_2017:
	case BS5950_2K:
	case BS5950_90:
	case CSA_S16_01:
	case CSA_S6_14_ST:
	case CSA_S6_19_ST:
	case Eurocode3:
	case Eurocode3_05:
	case Eurocode3_2_05:
	case GB50017_03:
	case GBJ17_88:
	case IRC24_2010:
	case IRS_SBC:
	case IS800_1984:
	case IS800_2007:
	case JROAD_H14:
	case JROAD_H24:
	case JROAD_H29:
	case JROAD_R07:
	case JTJ025_86:
	case KDS_24_14_30_2019:
	case KDS_24_14_31_2018:
	case KDS_41_30_10_2022:
	case KDS_41_31_2019:
	case KR_BRG_LSD15:
	case KSCE_ASD05:
	case KSCE_ASD10:
	case KSSC_ASD03:
	case KSSC_LSD09:
	case KSSC_LSD16:
	case NSCP_2015_S_ASD:
	case NSCP_2015_S_LRFD:
	case SP_16_13330_2017:
		{
			return D_UNITSYS_LENGTH_INDEX_MM;
		}
		break;
	}
	ASSERT(0); return -1;
}

int CDgnCodeCtrl::GetCodeUnitForc(const int nCode)
{
	switch ( nCode )
	{
	case AIK_ASD83:
	case AIK_CFSD98:
	case AIK_LSD97:
	case TWN_ASD90:
	case TWN_ASD96:
	case TWN_LSD90:
	case TWN_LSD96:
		{
			return D_UNITSYS_FORCE_INDEX_TON;
		}
		break;
	case AASHTO_LS02:
	case AASHTO_LS12:
	case AASHTO_LS16:
	case AASHTO_LS17:
	case AASHTO_LS20:
	case AASHTO_LS24:
	case AISC_ASD05:
	case AISC_ASD10:
	case AISC_ASD16:
	case AISC_ASD22:
	case AISC_ASD89:
	case AISC_LRFD05:
	case AISC_LRFD10:
	case AISC_LRFD16:
	case AISC_LRFD22:
	case AISC_LRFD2K:
	case AISC_LRFD93:
	case AISI_CFSD08:
	case AISI_CFSD86:
		{
			return D_UNITSYS_FORCE_INDEX_KIP;
		}
		break;
	case IRS_SBC:
	case KSCE_ASD96:
	case TWN_BRG_AS90:
	case TWN_BRG_LS90:
		{
			return D_UNITSYS_FORCE_INDEX_KG;
		}
		break;
	case AASHTO_AS96:
	case AASHTO_LS96:
		{
			return D_UNITSYS_FORCE_INDEX_LBF;
		}
		break;
	case AIJ_ASD02:
	case AS5100_5_2017:
	case BS5950_2K:
	case BS5950_90:
	case CSA_S16_01:
	case CSA_S6_14_ST:
	case CSA_S6_19_ST:
	case Eurocode3:
	case Eurocode3_05:
	case Eurocode3_2_05:
	case GB50017_03:
	case GBJ17_88:
	case IRC24_2010:
	case IS800_1984:
	case IS800_2007:
	case JROAD_H14:
	case JROAD_H24:
	case JROAD_H29:
	case JROAD_R07:
	case JTJ025_86:
	case KDS_24_14_30_2019:
	case KDS_24_14_31_2018:
	case KDS_41_30_10_2022:
	case KDS_41_31_2019:
	case KR_BRG_LSD15:
	case KSCE_ASD05:
	case KSCE_ASD10:
	case KSSC_ASD03:
	case KSSC_LSD09:
	case KSSC_LSD16:
	case NSCP_2015_S_ASD:
	case NSCP_2015_S_LRFD:
	case SP_16_13330_2017:
		{
			return D_UNITSYS_FORCE_INDEX_N;
		}
		break;
	}
	ASSERT(0); return -1;
}