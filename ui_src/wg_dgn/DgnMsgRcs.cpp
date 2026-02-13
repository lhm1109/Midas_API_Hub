#include "stdafx.h"
#include "DgnMsgRcs.h"

CDgnMsgRcs::CDgnMsgRcs()
{

}

CDgnMsgRcs::~CDgnMsgRcs()
{

}

LPCTSTR CDgnMsgRcs::GetMsg(const CString& strMsg)
{
	m_strMsg = strMsg;
	return ChkEmpty(m_strMsg);
}

LPCTSTR CDgnMsgRcs::GetMsg(const LPCTSTR& strMsg)
{
	m_strMsg = strMsg;
	return ChkEmpty(m_strMsg);
}

LPCTSTR CDgnMsgRcs::Msg_rcs_001()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_001));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_002()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_002));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_003()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_003));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_004()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_004));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_004_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_004_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_005()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_005));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_006()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_006));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_006_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_006_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_007()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_007));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_008()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_008));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_009()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_009));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_010()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_010));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_012(BOOL bNextLine/*=TRUE*/)
{
	return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_012));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_013(BOOL bNextLine/*TRUE*/)
{
	return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_013));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_014()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_014));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_015(BOOL bNextLine/*TRUE*/)
{
	const CString& strChar = (bNextLine)? _T("\n") : _T("");
	return GetMsg(strChar + _LS(IDS_RCS_Msg_rcs_015));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_016()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_016));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_017()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_017));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_018()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_018));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_018_0()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_018_0));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_019()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_019));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_019_0()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_019_0));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_020()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_020));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_021()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_021));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_022()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_022));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_023()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_023));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_024()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_024));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_024_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_024_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_024_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_024_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_025()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_025));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_025_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_025_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_025_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_025_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_025_3()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_025_3));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_025_4()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_025_4));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_026()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_026));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_026_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_026_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_027()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_027));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_027_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_027_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_028()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_028));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_029()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_029));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_029_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_029_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_030()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_030));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_030_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_030_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_031()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_031));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_031_0()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_031_0));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_031_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_031_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_032()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_032));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_032_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_032_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_032_2()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_032_2));
}

// Add by GAY.('06.02.16). In AIJ_SRC01
LPCTSTR CDgnMsgRcs::Msg_rcs_032_3()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_032_3));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_032_4()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_032_4));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_033()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_033));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_033_EC()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_033_EC));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_033_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_033_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_033_1_EC()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_033_1_EC));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_033_2()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_033_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_033_3()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_033_3));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_033_4()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_033_4));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_034()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_034));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_034_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_034_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_035()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_035));	
}

LPCTSTR CDgnMsgRcs::Msg_rcs_035_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_035_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_036()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_036));	
}

LPCTSTR CDgnMsgRcs::Msg_rcs_036_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_036_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_036_2()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_036_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_036_3()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_036_3));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_037()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_037));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_038()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_038));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_039()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_039));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_039_0()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_039_0));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_040()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_040));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_040_1()
{
	// Add by ZINU.('07.01.17). NO:2484, Shear-y,z.
	return GetMsg(_LS(IDS_RCS_Msg_rcs_040_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_040_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_040_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_041()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_041));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_042()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_042));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_043()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_043));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_044()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_044));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_044_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_044_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_045()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_045));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_046()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_046));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_047()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_047));
}

//Add by Unsang :: EC에서 Capacity -> Resistance로 변경함.
LPCTSTR CDgnMsgRcs::Msg_rcs_047_EC()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_047_EC));
}
LPCTSTR CDgnMsgRcs::Msg_rcs_047_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_047_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_047_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_047_2));
}

LPCTSTR CDgnMsgRcs::Msg_CONCENTRIC_MAX_AXIAL_LOAD()
{
	return GetMsg(_LS(IDS_RCS_MSG_CONCENTRIC_MAX_AXIAL_LOAD));	
}

// PMS:xxxx-Seungjun-20100514 for EC2:04 capacity design.
LPCTSTR CDgnMsgRcs::Msg_NORMALIZED_AXIAL_RATIO()
{
	return GetMsg(_LS(IDS_RCS_MSG_NORMALIZED_AXIAL_RATIO));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_049()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_049));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_049_F()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_049_F));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_050()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_050));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_051()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_051));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_052()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_052));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_053()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_053));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_053_RUS()
{
    const CString& strVal = [] (int nLanguage) -> CString
    {
        switch ( nLanguage )//Limited Characters:28
        {
        case iDGN_ENG:
        case iDGN_KOR:
        case iDGN_JPN:
        case iDGN_CHN:
        case iDGN_RUS:
            return _T("= *****                     ");
        default:
            ASSERT(0);
            return _T("");
        }

    }(m_iLang);

    return GetMsg(strVal);
}

LPCTSTR CDgnMsgRcs::Msg_rcs_053_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_053_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_054()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_054));
}

//KJH20020824, column check based on JTJ023-85
LPCTSTR CDgnMsgRcs::Msg_rcs_054_F()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_054_F));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_055()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_055));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_055_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_055_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_056()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_056));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_057()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_057));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_058()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_058));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_059()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_059));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_060()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_060));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_061()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_061));
}

LPCTSTR CDgnMsgRcs::Msg_P_M_INTERACTION_DIAGRAM()
{
	return GetMsg(_LS(IDS_RCS_Msg_P_M_INTERACTION_DIAGRAM));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_061_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_061_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_062()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_062));
}

LPCTSTR CDgnMsgRcs::Msg_M_N_INTERACTION_DIGGRAM()
{
	return GetMsg(_LS(IDS_RCS_Msg_M_N_INTERACTION_DIAGRAM));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_063()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_063));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_063_BS()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_063_BS));
}

//Add by Unsang :: EC에서 Capacity -> Resistance로 변경함.
LPCTSTR CDgnMsgRcs::Msg_rcs_063_EC()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_063_EC));
}

LPCTSTR CDgnMsgRcs::Msg_SLS_STRESS_CHK()
{
	return GetMsg(_LS(IDS_RCS_MSG_SLS_STRESS_CHK));	
}

LPCTSTR CDgnMsgRcs::Msg_SLS_CRACK_CHK()
{
    return GetMsg(_LS(IDS_RCS_MSG_SLS_CRACK_CHK));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_063(BOOL bMiddle)
{
	return GetMsg(bMiddle ? _LS(IDS_RCS_Msg_rcs_063_MID) : _LS(IDS_RCS_Msg_rcs_063_END));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_063_EC(BOOL bMiddle)
{
    return GetMsg(bMiddle ? _LS(IDS_RCS_Msg_rcs_063_EC_MID) : _LS(IDS_RCS_Msg_rcs_063_EC_END));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_063_1(BOOL bMiddle)
{
	return GetMsg(bMiddle ? _LS(IDS_RCS_Msg_rcs_063_1_MID) : _LS(IDS_RCS_Msg_rcs_063_1_END));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_064()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_064));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_064(BOOL bMiddle)
{
	return GetMsg(bMiddle ? _LS(IDS_RCS_Msg_rcs_064_MID) : _LS(IDS_RCS_Msg_rcs_064_END));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_065()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_065));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_065_AREMA()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_065_AREMA));
}


//Add by Unsang :: EC에서 Strength -> Resistance로 변경함.
LPCTSTR CDgnMsgRcs::Msg_rcs_065_EC()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_065_EC));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_066()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_066));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_067()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_067));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_068()
{
	//change by cylee 05.08.11 increase decimal places to properly show a value
	return GetMsg(_LS(IDS_RCS_Msg_rcs_068));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_069()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_069));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_069_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_069_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_069_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_069_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_069_3()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_069_3));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_070()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_070));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_071()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_071));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_072()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_072));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_073()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_073));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_074()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_074));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_075()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_075));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_076()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_076));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_077()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_077));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_078()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_078));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_079()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_079));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_080()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_080));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_081()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_081));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_082()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_082));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_083()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_083));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_084()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_084));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_085()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_085));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_086()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_086));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_087()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_087));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_088()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_088));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_089()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_089));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_090()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_090));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_091()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_091));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_091_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_091_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_092()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_092));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_092_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_092_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_093()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_093));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_094()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_094));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_095()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_095));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_096()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_096));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_097()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_097));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_097_EC()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_097_EC));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_097_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_097_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_097_1_EC()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_097_1_EC));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_098()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_098));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_099()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_099));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_100()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_100));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_101()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_101));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_102()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_102));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_103()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_103));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_104()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_104));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_106_1(BOOL bExistCrane/*FALSE*/)
{
    if ( bExistCrane )
    {
        return GetMsg(_T("----------------------+------------------------------+------------------------------+-----------------------"));
    }
    else
    {
        return GetMsg(_T("----------------------+---------------------------+---------------------------+--------------------"));
    }
}

LPCTSTR CDgnMsgRcs::Msg_rcs_106_1_1(BOOL bExistCrane/*FALSE*/)
{
    if ( bExistCrane )
    {
        return GetMsg(_T("-----------------------+------------------------------+------------------------------+----------------------------------------------"));
    }
    else
    {
        return GetMsg(_T("-----------------------+---------------------------+---------------------------+----------------------------------"));
    }
}

LPCTSTR CDgnMsgRcs::Msg_rcs_106_1_2(BOOL bExistCrane/*FALSE*/)
{
    if ( bExistCrane )
    {
        return GetMsg(_T("----------------------+------------------------------+------------------------------+----------------------+------------------------------"));
    }
    else
    {
        return GetMsg(_T("----------------------+---------------------------+---------------------------+-------------------+------------------------------"));
    }
}

LPCTSTR CDgnMsgRcs::Msg_rcs_106_1_3(BOOL bExistCrane/*FALSE*/)
{
    if ( bExistCrane )
    {
        return GetMsg(_T("-----------------------+------------------------------+------------------------------+---------------------------------------------+------------------------------"));
    }
    else
    {
        return GetMsg(_T("-----------------------+---------------------------+---------------------------+---------------------------------+------------------------------"));
	}	
}

LPCTSTR CDgnMsgRcs::Msg_rcs_106_2()
{
	return GetMsg(_T("-----------------------------+---------------------------+---------------------------+-------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_106_3()
{
	return GetMsg(_T("--------+-----------------------------+-----------------------------+---------------+----------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_106_4()
{
	return GetMsg(_T("----------------------+-------------------+-------------------+-------------------+------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_107()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_107));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_108()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_108));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_112()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_112));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_113(BOOL bExistCrane/*FALSE*/)
{
	return GetMsg(bExistCrane ? _LS(IDS_RCS_Msg_rcs_113) : _LS(IDS_RCS_Msg_rcs_113_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_114(BOOL bExistCrane/*FALSE*/)
{
	return GetMsg(bExistCrane ? _LS(IDS_RCS_Msg_rcs_114_LCB1) : _LS(IDS_RCS_Msg_rcs_114_LCB2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_114_1(BOOL bExistCrane/*FALSE*/)
{
	return GetMsg(bExistCrane ? _LS(IDS_RCS_Msg_rcs_114_1) : _LS(IDS_RCS_Msg_rcs_114_1_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_115(BOOL bExistCrane/*FALSE*/)
{
    if ( bExistCrane )
    {
        return GetMsg(_T("--------+--------------------------------+--------------------------------+------------------------------------"));
    }
    else
    {
        return GetMsg(_T("--------+-----------------------------+-----------------------------+---------------------------------"));
    }
}

LPCTSTR CDgnMsgRcs::Msg_rcs_115_1(BOOL bExistCrane/*FALSE*/)
{
    if ( bExistCrane )
    {
        return GetMsg(_T("--------+--------------------------------+--------------------------------+---------------------------------------+------------------------"));
    }
    else
    {
        return GetMsg(_T("--------+-----------------------------+-----------------------------+-----------------------------------------+---------------------"));
    }
}

LPCTSTR CDgnMsgRcs::Msg_rcs_115_2(BOOL bExistCrane/*FALSE*/)
{
    if ( bExistCrane )
    {
        return GetMsg(_T("--------+---------------------------------------+---------------------------------------+------------------------------------"));
    }
    else
    {
        return GetMsg(_T("--------+------------------------------------+------------------------------------+---------------------------------"));
	}	
}

LPCTSTR CDgnMsgRcs::Msg_rcs_115_3(BOOL bExistCrane/*FALSE*/)
{
    if ( bExistCrane )
    {
        return GetMsg(_T("--------+---------------------------------------+---------------------------------------+---------------------------------------+------------------------"));
    }
    else
    {
        return GetMsg(_T("--------+------------------------------------+------------------------------------+-----------------------------------------+---------------------"));
    }
}

LPCTSTR CDgnMsgRcs::Msg_rcs_119()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_119));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_119_1(BOOL bExistCrane/*FALSE*/)
{
    return GetMsg(bExistCrane ? _LS(IDS_RCS_Msg_rcs_119_1) : _LS(IDS_RCS_Msg_rcs_119_1_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_119_2(BOOL bExistCrane/*FALSE*/)
{
	return GetMsg(bExistCrane ? _LS(IDS_RCS_Msg_rcs_119_2) : _LS(IDS_RCS_Msg_rcs_119_2_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_119_3(BOOL bExistCrane/*FALSE*/, BOOL bExistCorner/*FALSE*/)
{
	const CString& strVoid  = bExistCrane? _T("    ") : _T(" ");
	const CString& strVoid2 = bExistCorner? _T("     ") : _T("");

	m_strMsg.Format(_LS(IDS_RCS_Msg_rcs_119_3), strVoid, strVoid2);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_120()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_120));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_123()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_123));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_126(BOOL bExistCrane/*FALSE*/)
{
	return GetMsg(bExistCrane ? _LS(IDS_RCS_Msg_rcs_126_A) : _LS(IDS_RCS_Msg_rcs_126_B));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_126_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_126_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_126_2(BOOL bExistCrane/*FALSE*/)
{
	return GetMsg(bExistCrane ? _LS(IDS_RCS_Msg_rcs_126_2_A) : _LS(IDS_RCS_Msg_rcs_126_2_B));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_138()
{
	//Limited Characters:69
	return GetMsg(_T("       +============================================================+"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_142()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_142));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_143()
{
    const CString& strVal = [] (int nLanguage) -> CString
    {
        switch ( nLanguage )//Limited Characters:69
        {
        case iDGN_ENG: 
        case iDGN_KOR: 
        case iDGN_JPN: 
        case iDGN_CHN: 
        case iDGN_RUS: 
            return _T("       |                                                            |");	
        default:
            ASSERT(0);
            return _T("");
        }
    }(m_iLang);
	
	return GetMsg(strVal);
}

LPCTSTR CDgnMsgRcs::Msg_rcs_144()
{
    const CString& strVal = [] (int nLanguage) -> CString
    {
        switch ( nLanguage )//Limited Characters:69
        {
        case iDGN_ENG:
        case iDGN_KOR:
        case iDGN_JPN:
        case iDGN_CHN:
        case iDGN_RUS: 
            return _T("       |                                            (c)1990 - 2002  |");	
        default:
            ASSERT(0);
            return _T("");
        }
    }(m_iLang);
	
	return (GetMsg(strVal));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_150()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_150));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_152()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_152));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_153()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_153));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_154()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_154));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_155()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_155));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_156()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_156));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_159()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_159));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_160()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_160));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_161()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_161));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_162()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_162));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_163()
{
	//Limited Characters:95
    return GetMsg(_T("-----------+--------------------------+--------------------------+--------------------------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_163_1()
{
	//Limited Characters:120
    return GetMsg(_T("-----------+--------------------------+--------------------------+---------------------------------------------------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_166()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_166));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_168()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_168));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_169()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_169));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_170()
{
	//Limited Characters:99
    return GetMsg(_T("-----------------------+------------------------------+------------------------------+----------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_170_1()
{
    return GetMsg(_T("-----------------------+------------------------------+------------------------------+-----------------------+----------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_173()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_173));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_174()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_174));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_174_1()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_174_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_175()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_175));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_175_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_175_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_175_2()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_175_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_176()
{
	//Limited Characters:88
	return GetMsg(_T("-----------+---------------------------+---------------------------+--------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_176_1()
{
	//Limited Characters:88
	return GetMsg(_T("-----------+---------------------------+---------------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_179()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_179));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_182()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_182));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_182_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_182_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_183_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_183_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_183_2()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_183_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_184()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_184));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_185()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_185));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_186()
{
	//Limited Characters:73
	return GetMsg(_T("  -----------------------------------------------------------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_187()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_187));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_188()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_188));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_188_1()
{
	return GetMsg( _LS(IDS_RCS_Msg_rcs_188_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_189_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_189_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_189_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_189_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_190()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_190));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_191()
{
	//Limited Characters:62
	return GetMsg(_T("  ------------------------------------------------------------"));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_192()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_192));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_193()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_193));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_194()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_194));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_195()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_195));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_196()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_196));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_197()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_197));
}

// Add by GAY. MNET:3780. ('08.11.05). Output for Torsion Result Term.
LPCTSTR CDgnMsgRcs::Msg_rcs_198()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_198));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_199_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_199_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_199_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_199_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_200_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_200_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_200_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_200_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_201()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_201));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_202()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_202));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_202_Horz()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_202_Horz));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_202_Vert()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_202_Vert));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_203_WallStability()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_203_WallStability));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_204()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_204));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_205()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_205));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_206()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_206));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_207()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_207));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_208()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_208));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_209()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_209));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_210()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_210));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_211()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_211));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_212()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_212));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_213()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_213));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_214()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_214));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_214_0()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_214_0));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_214_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_214_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_215()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_215));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_216()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_216));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_217(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_217));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_218(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_218));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_219(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_219));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_220(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_220));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_221(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_221));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_222_1(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_222_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_222_2(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_222_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_222_3(BOOL bNextLine)
{
	return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_222_3));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_223(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_223));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_224()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_224));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_225( BOOL bNextLine/*=TRUE*/ )
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_225));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_226( BOOL bNextLine/*=TRUE*/ )
{
	return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_226));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_227( BOOL bNextLine/*=TRUE*/ )
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_227));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_228( BOOL bNextLine/*=TRUE*/ )
{
	return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_228));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_229( BOOL bNextLine/*=TRUE*/ )
{
	return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_229));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_229_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_229_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_230()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_230));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_231()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_231));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_232()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_232));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_233()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_233));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Plate_001()
{
	return GetMsg(_LS(IDS_RCSC_Msg_Plate_001));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Span()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_SPAN));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Stirrup()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_STIRRUP));
}

//---------------Add by Hong,jiseon 2002.7.11
LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Beam_Dgn_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_BEAM_DGN_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Beam_Chk_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_BEAM_CHK_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_PSBeam_Dgn_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_PSBEAM_DGN_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_PSBeam_Chk_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_PSBEAM_CHK_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_SRCBeam_Chk_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_SRCBEAM_CHK_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Colm_Dgn_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_COLM_DGN_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Brce_Dgn_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_BRCE_DGN_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Colm_Chk_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_COLM_CHK_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Brce_Chk_Title()
{
    return GetMsg(_LS(IDS_RCS_TEXTOUT_BRCE_CHK_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Wall_Dgn_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_WALL_DGN_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Story()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_STORY));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Wall_Mark()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_WALL_MARK));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_V_Rebar()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_V_REBAR));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_H_Rebar()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_H_REBAR));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_End_Rebar()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_END_REBAR));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Bar_Layer()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_BAR_LAYER));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Wall_Chk_Title()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_WALL_CHK_TITLE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Shape()
{
	return GetMsg(_LS(IDS_RCS_TEXTOUT_SHAPE));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_Height()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Textout_Height));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_BE_Rebar()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Textout_BE_Rebar));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Textout_BE_Leng()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Textout_BE_Leng));
}

// Coded by Seungjun MNet:No.2393 ('20060830)
LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00()               // 3. Crack Width.
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_EC()               // 3. Crack Width.
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_EC));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_3()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_3));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_3_1()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_3_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_3_2()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_3_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_4()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_4));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_5()//Beam
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_5));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_5_1()//Beam
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_5_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_6()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_6));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_01()               // 4. Crack Width.
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_01));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_01_1()               // Euro2:04 일경우
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_01_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_01_2()               // Civil RUS SNiP/SP
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_01_2));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_02(int iCrackType) // Negative - Dry(or Humidity, Corrosion, High Corrosion).
{
    const CString& strCrack =[] (int nCrack) -> CString
    {
        switch ( nCrack )
        {
        case 0:
            return _LS(IDS_RCS_Msg_rcs_Service_02_0);
        case 1:
            return _LS(IDS_RCS_Msg_rcs_Service_02_1);
        case 2:
            return _LS(IDS_RCS_Msg_rcs_Service_02_2);
        case 3:
            return _LS(IDS_RCS_Msg_rcs_Service_02_3);
        case 4:
            return _LS(IDS_RCS_Msg_rcs_Service_02_4);
        case 5:
            return _LS(IDS_RCS_Msg_rcs_Service_02_5);
        default:
            ASSERT(0);
            return _T("");
        }
    }(iCrackType);
	
	return GetMsg(strCrack);
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_03(int iCrackType) // Positive - Dry(or Humidity, Corrosion, High Corrosion).
{
    const CString& strCrack =[] (int nCrack) -> CString
    {
        switch ( nCrack )
        {
        case 0:
            return _LS(IDS_RCS_Msg_rcs_Service_03_0);
        case 1:
            return _LS(IDS_RCS_Msg_rcs_Service_03_1);
        case 2:
            return _LS(IDS_RCS_Msg_rcs_Service_03_2);
        case 3:
            return _LS(IDS_RCS_Msg_rcs_Service_03_3);
        case 4:
            return _LS(IDS_RCS_Msg_rcs_Service_03_4);
        case 5:
            return _LS(IDS_RCS_Msg_rcs_Service_03_5);
        default:
            ASSERT(0);
            return _T("");
        }
    }(iCrackType);
	
	return GetMsg(strCrack);
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_04()               // Allowable Crack Width(wa)
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_04));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_05()               // Crack Width(w)
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_05));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_06()               // Check Ratio(w/wa)
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_06));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_07()               // Allowable Crack Width(sa)
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_07));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_08()               // Crack Width(s)
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_08));
}

// Deflection checking
LPCTSTR CDgnMsgRcs::Msg_rcs_Service_11()               // 4. Deflection.
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_11));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_11_1()               // 4. Deflection.
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_11_1));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_12()              
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_12));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_13()              
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_13));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_14()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_14));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_15()               // SNiP/SP
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_15));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Fatigue_01()               // SNiP/SP
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Fatigue_01));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Fatigue_02()               // SNiP/SP
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Fatigue_02));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Fatigue_03()               // SNiP/SP
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Fatigue_03));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Fatigue_04()               // SNiP/SP
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Fatigue_04));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Mu()
{
	return GetMsg(_LS(IDS_RCS_MU));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Mr()
{
	return GetMsg(_LS(IDS_RCS_MR));
}

// for russia plate
LPCTSTR CDgnMsgRcs::Msg_rcs_300()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_300));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_301()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_301));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_302()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_302));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_303()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_303));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_304()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_304));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_305(BOOL bNextLine)
{
    return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_305));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_306(BOOL bNextLine)
{
	return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_306));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_307(BOOL bNextLine)
{
	return GetMsg(GetCharLineBreak(bNextLine) + _LS(IDS_RCS_Msg_rcs_307));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_00_7()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_00_7));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_01_3()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service_01_3));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_16()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service16));
}

LPCTSTR CDgnMsgRcs::Msg_rcs_Service_17()
{
	return GetMsg(_LS(IDS_RCS_Msg_rcs_Service17));
}

LPCTSTR CDgnMsgRcs::Msg_DesignCode()
{
    return GetMsg(_LS(IDS_RCS_Msg_rcs_012));
}

LPCTSTR CDgnMsgRcs::Msg_JOINT()
{
	return GetMsg(_LS(IDS_RCS_MSG_JOINT));
}

LPCTSTR CDgnMsgRcs::Msg_JOINT_RATIO()
{
	return GetMsg(_LS(IDS_RCS_MSG_JOINT_RATIO));
}

LPCTSTR CDgnMsgRcs::Msg_END()
{
	return GetMsg(_LS(IDS_RCS_MSG_END));
}

LPCTSTR CDgnMsgRcs::Msg_MIDDLE()
{
	return GetMsg(_LS(IDS_RCS_MSG_MIDDLE));
}

LPCTSTR CDgnMsgRcs::Msg_LINEAR_CREEP()
{
	return GetMsg(_LS(IDS_RCS_MSG_LINEAR_CREEP));
}

LPCTSTR CDgnMsgRcs::Msg_NONLINEAR_CREEP()
{
	return GetMsg(_LS(IDS_RCS_MSG_NONLINEAR_CREEP));
}

LPCTSTR CDgnMsgRcs::Msg_CRACKED_SECTION()
{
	return GetMsg(_LS(IDS_RCS_MSG_CRACKED_SECTION));
}

LPCTSTR CDgnMsgRcs::Msg_DESIGN_FOR_AXIAL_FLEXURE()
{
	return GetMsg(_LS(IDS_RCS_MSG_DESIGN_FOR_AXIAL_FLEXURE));
}

LPCTSTR CDgnMsgRcs::Msg_DESIGN_FOR_SHEAR()
{
	return GetMsg(_LS(IDS_RCS_MSG_DESIGN_FOR_SHEAR));
}

CString CDgnMsgRcs::GetCharLineBreak(BOOL bNextLine)
{
    return bNextLine? _T("\n") : _T("");
}