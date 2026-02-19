// DgnMsgSrc.cpp: implementation of the CDgnMsgSrc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnMsgSrc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnMsgSrc::CDgnMsgSrc()
{

}

CDgnMsgSrc::~CDgnMsgSrc()
{

}
///


LPCTSTR CDgnMsgSrc::Msg_src_Textout_Type()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_Textout_Type);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_001()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_001);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_002()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_002);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_003()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_003);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_004()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_004);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_005()
{
	switch(m_iLang)//Limited Characters:14
	{
		case iDGN_ENG:{ m_strMsg=_T("Area (cA) = %s"); break;}
		case iDGN_KOR:{ m_strMsg=_T("Area (cA) = %s"); break;}
		case iDGN_JPN:{ m_strMsg=_T("ñ êœ (cA) = %s"); break;}
		case iDGN_CHN:{ m_strMsg=_T("√Êª˝(cA) = %s"); break;}
		case iDGN_RUS:{ m_strMsg=_T("Area (cA) = %s"); break;}
	}
	return (ChkEmpty(m_strMsg));
}

// Add by GAY.('06.06.15). Distinguish concrete area output expression.
LPCTSTR CDgnMsgSrc::Msg_src_005_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_005_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_006()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_006);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_007()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_007);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_008()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_008);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_009()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_009);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_010()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_010);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_011()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_011);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_012()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_012);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_013()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_013);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_010_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_010_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_011_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_011_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_012_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_012_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_013_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_013_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_014()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_014);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_015()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_015);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_016()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_016);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_017()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_017);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_018()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_018);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_019()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_019);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_020()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_020);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_021()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_021);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_021_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_021_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_022()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_022);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_023()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_023);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_024()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_024);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_024_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_024_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_024_2()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_024_2);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_025()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_025);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_026()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_026);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_027()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_027);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_028()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_028);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_030()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_030);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_031()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_031);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_032()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_032);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_033()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_033);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_034()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_034);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_035()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_035);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_036()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_036);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_037()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_037);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_038()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_038);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_039()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_039);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_040()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_040);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_041()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_041);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_042()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_042);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_043()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_043);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_044()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_044);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_045()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_045);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_046()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_046);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_048()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_048);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_049()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_049);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_050()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_050);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_051()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_051);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_052()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_052);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_053()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_053);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_054()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_054);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_055()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_055);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_056()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_056);
	return (ChkEmpty(m_strMsg));
}

// MNET:3803-Seungjun-20081223
LPCTSTR CDgnMsgSrc::Msg_src_056_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_056_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_057()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_057);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_058()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_058);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_059()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_059);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_060()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_060);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_061()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_061);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_062()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_062);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_063()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_063);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_064()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_064);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_065()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_065);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_070()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_070);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_071()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_071);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_072()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_072);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_073()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_073);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_073_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_073_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_074()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_074);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_075()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_075);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_076()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_076);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_077()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_077);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_078()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_078);
	return (ChkEmpty(m_strMsg));
}


LPCTSTR CDgnMsgSrc::Msg_src_081()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_081);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_082()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_082);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_086()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_086);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_087()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_087);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_087_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_087_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_088()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_088);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_091()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_091);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_091_1()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_091_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_094()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_094);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_095()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_095);
	return (ChkEmpty(m_strMsg));
}

// MNET:3803-Seungjun-20081223
LPCTSTR CDgnMsgSrc::Msg_src_096()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_096);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_097()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_097);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_098()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_098);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_099()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_099);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_100()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_100);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgSrc::Msg_src_101()
{
	m_strMsg = _LS(IDS_SRC_Msg_src_101);
	return (ChkEmpty(m_strMsg));
}