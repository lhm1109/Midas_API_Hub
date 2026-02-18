// DgnMsgAcs.cpp: implementation of the CDgnMsgAcs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnMsgAcs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnMsgAcs::CDgnMsgAcs()
{

}

CDgnMsgAcs::~CDgnMsgAcs()
{

}

LPCTSTR CDgnMsgAcs::Msg_acs_STEEL_DESIGN()//_001()
{
/*
	switch(m_iLang)//Limited Characters:12
	{
		case iDGN_ENG:{m_strMsg=_T("Steel Design"); break;}
		case iDGN_KOR:{m_strMsg=_T("碍备炼 汲拌"); break;}
		case iDGN_JPN:{m_strMsg=_T("揝崪愝寁"); break;}
		case iDGN_CHN:{m_strMsg=_T("钢结构设计"); break;}
	}
*/
	m_strMsg=_LS(IDS_DGN_Msg_acs_STEEL_DESIGN);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_1_DESIGN_INFO()//002()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_1_DESIGN_INFO);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CODE()//004()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_CODE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_UNIT_SYSTEM()//005()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_UNIT_SYSTEM);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_ELEMENT_NO()//006()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_ELEMENT_NO);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MATERIAL()//007()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MATERIAL);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_NO()//008()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_NO);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SECTION_NAME()//009()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SECTION_NAME);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_POSITION()//010()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_POSITION);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_TAPERTED_SECTION()//011()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_TAPERTED_SECTION);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_POSITION_J()//012()
{
        /// by GAY. MQC:8891. ('14.10.23). modify to output section name for tapered section.
        m_strMsg = _LS(IDS_DGN_Msg_acs_POSITION_J);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BUILT_UP_SECTION()//013()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BUILT_UP_SECTION);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_ROLLED()//014()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_ROLLED);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MEMBER_LENGTH()//015()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MEMBER_LENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_DEPTH()//016()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_DEPTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_WEB_THICK()//017()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_WEB_THICK);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_TOP_FWIDTH()//018()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_TOP_FWIDTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOT_LFWIDTH()//018()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOT_LFWIDTH);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOT_RFWIDTH()//018()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOT_RFWIDTH);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_TOP_FTHICK()//019()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_TOP_FTHICK);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOT_FWIDTH()//020()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOT_FWIDTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOT_FTHICK()//021()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOT_FTHICK);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_FLG_WIDTH()//022()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_FLG_WIDTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_WEB_CENTER()//023()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_WEB_CENTER);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_OUT_DIA()//024()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_OUT_DIA);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_WALL_THICK()//025()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_WALL_THICK);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_WIDTH()//026()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_WIDTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_FLG_THICK()//027()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_FLG_THICK);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BTB_SPACE()//028()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BTB_SPACE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_THICKNESS()//029()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_THICKNESS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_ROUNDING()//030()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_ROUNDING);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_LIP_DEPTH()//031()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_LIP_DEPTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_AREA()//032()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_AREA);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_2_MEM_FORCE()//033()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_2_MEM_FORCE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_LOAD_COMB()//034()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_LOAD_COMB);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_AXIAL_FORCE()//035()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_AXIAL_FORCE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SHEAR_FORCE()//036()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SHEAR_FORCE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BEND_MOMENT()//037()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BEND_MOMENT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MOM_END()//038()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MOM_END);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_2_MEM_CHK_FORCE()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_2_MEM_CHK_FORCE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_2_MEM_CHK_ITEM_AXIS()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_2_MEM_ITEM_AXIAL);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_2_MEM_CHK_ITEM_STABLE()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_2_MEM_ITEM_STABLE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_2_MEM_CHK_ITEM_STABLE_Y()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_2_MEM_ITEM_STABLE_y);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_2_MEM_CHK_ITEM_STABLE_Z()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_2_MEM_ITEM_STABLE_z);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_2_MEM_CHK_ITEM_SHEAR()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_2_MEM_ITEM_SHEAR);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_3_DGN_PARAM_ELEM_TYPE()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_3_DGN_PARAM_ELEM_TYPE);
	return (ChkEmpty(m_strMsg));
}


LPCTSTR CDgnMsgAcs::Msg_acs_3_DGN_PARAM()//040()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_3_DGN_PARAM);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_EFFECT_LENGTH()//041()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_EFFECT_LENGTH);
	return (ChkEmpty(m_strMsg));
}

// MQC:8592 Seungjun-20140905 : Effective length for lateral-torsional-buckling
LPCTSTR CDgnMsgAcs::Msg_acs_EFFECT_LENGTH_LTB()//041()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_EFFECT_LENGTH_LTB);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MOM_CRT_FACTOR()//042()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MOM_CRT_FACTOR);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_UNBRC_LENGTH()//043()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_UNBRC_LENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_EFFECT_LENG_FACTOR()//044()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_EFFECT_LENG_FACTOR);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_EQUIP_MOM_FACTOR()//045()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_EQUIP_MOM_FACTOR);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_NET_AREA_RATIO()
{
        m_strMsg = _LS(IDS_DGN_Msg_acs_NET_AREA_RATIO);
        return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BEND_COEFF()//046()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BEND_COEFF);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MOM_FACTOR()//047()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MOM_FACTOR);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_WTR_NG()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_WTR_NG);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_4_STRESS_CHK_RESULT()//048()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_4_STRESS_CHK_RESULT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_AXIAL_STRESS()//049()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_AXIAL_STRESS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_PRINTFONT()//050()
{
	switch(m_iLang)//Limited Characters:12
	{
		case iDGN_ENG:{m_strMsg=_T("Arial Narrow"); break;}
		case iDGN_KOR:{m_strMsg=_T("Arial Narrow"); break;}
		case iDGN_JPN:{m_strMsg=_T("俵俽 俹僑僔僢僋"); break;}
		case iDGN_CHN:{m_strMsg=_T("Tahoma"); break;}
		case iDGN_RUS:{m_strMsg=_T("Arial Narrow"); break;}
	}
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SLEND_RATIO()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SLEND_RATIO);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SLEND_RATIO_KL()//051()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SLEND_RATIO_KL);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SLEND_RATIO_L()//052()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SLEND_RATIO_L);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BENDING_STRESS()//057()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BENDING_STRESS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_AXIAL_AND_BENDING_STRESS()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_AXIAL_AND_BENDING_STRESS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MAX_TENSION()//058()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MAX_TENSION);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MAJOR_AXIS()//059()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MAJOR_AXIS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MAX_COMP_TENS()//060()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MAX_COMP_TENS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MAX_COMP()//061()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MAX_COMP);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MINOR_AXIS()//062()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MINOR_AXIS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_4_STRENGTH_CHK_RESULT()//063()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_4_STRENGTH_CHK_RESULT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_4_RESIST_CHK_RESULT()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_4_RESIST_CHK_RESULT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_Flexural_STRESS()//
{
	m_strMsg = _LS(IDS_ACS_Check_Flexural_Stress_RV);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_Shear_STRESS()//
{
	m_strMsg = _LS(IDS_ACS_Check_Shear_Stress_RV);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_4_RESIST_CHK_RESULT_AXIAL()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_4_RESIST_CHK_RESULT_AXIAL);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_4_RESIST_CHK_RESULT_STABLE()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_4_RESIST_CHK_RESULT_STABLE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_4_RESIST_CHK_RESULT_SHEAR()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_4_RESIST_CHK_RESULT_SHEAR);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_AXIAL_STRENGTH()//064()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_AXIAL_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BEND_STRENGTH()//067()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BEND_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMB_STRESS()//068()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMB_STRESS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_TENS_BEND()//069()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_TENS_BEND);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMP_BEND()//070()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMP_BEND);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BENDING()//071()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BENDING);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMB_STRENGTH()//076()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMB_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMB_CAPACITY()//079()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMB_CAPACITY);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMB_RESIST()//080()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMB_RESIST);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SHEAR_STRENGTH()//081()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SHEAR_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SHEAR_RESIST()//081_1()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SHEAR_RESIST);
	return (ChkEmpty(m_strMsg));
}


LPCTSTR CDgnMsgAcs::Msg_acs_SHEAR_STRESS()//082()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SHEAR_STRESS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SHEAR_DISENERGY_BEAM()//Add By xuezc 2017/9/25
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_SHEAR_DISENERGYBEAM);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_4_CHK_RESULT()//085()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_4_CHK_RESULT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHK_SLD_XZV()//086()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_CHK_SLD_XZV);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHK_SLD_YZ()//087()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_CHK_SLD_YZ);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHK_SLD_V()//088()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_CHK_SLD_V);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SLD_RATIO_LK()//089()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SLD_RATIO_LK);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHK_AXIAL_STRESS()//090()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_CHK_AXIAL_STRESS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_5_CALC_BOLT_RESULT()//091()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_5_CALC_BOLT_RESULT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOLT_NAME_ARR_NUM()//092()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOLT_NAME_ARR_NUM);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MATERIAL_2()//093()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOLT_NAME_ARR_NUM);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOLT_TENS_STRENGTH()//094()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOLT_TENS_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_NOT_CALC_BUT_JOINT()//095()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOLT_TENS_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOLT_SHEAR_STRENGTH()//096()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOLT_SHEAR_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOLT_BEAR_STRENGTH()//097()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOLT_BEAR_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_MEMB_TENS_STRENGTH()//098()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_MEMB_TENS_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BOLT_CHK_STRENGTH()//_099()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BOLT_CHK_STRENGTH);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_ELEM_PROP()//102()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_ELEM_PROP);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHK_COM_SHR()//103()
{
	//change by cylee(05.04.12)
	//Increase the space between 'memb' & 'sect'
    m_strMsg = _LS(IDS_DGN_Msg_acs_CHK_COM_SHR);
	return (ChkEmpty(m_strMsg));
}

// Seungjun-20081209 EC2:05 2nd development.
LPCTSTR CDgnMsgAcs::Msg_acs_CHK_COM_STR()//103()
{
    //change by cylee(05.04.12)
    //Increase the space between 'memb' & 'sect'
    m_strMsg = _LS(IDS_DGN_Msg_acs_CHK_COM_STR);
    return (ChkEmpty(m_strMsg));
}


LPCTSTR CDgnMsgAcs::Msg_acs_ELEM_PROP_1()//108()
{
	
	//change by cylee(05.04.12)
	//Increase the space between 'memb' & 'sect'
    m_strMsg = _LS(IDS_DGN_Msg_acs_ELEM_PROP_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHK_COM_SHR_1()//_109()
{
	//change by cylee(05.04.12)
	//Increase the space between 'memb' & 'sect'
    m_strMsg = _LS(IDS_DGN_Msg_acs_CHK_COM_SHR_1);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_5_STRUCTRE_CHK_RESULT()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_5_STRUCTRE_CHK_RESULT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_5_STRUCTRE_CHK_RESULT_SLENDER()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_5_STRUCTRE_CHK_RESULT_SLENDER);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_5_STRUCTRE_CHK_RESULT_WTRATIO()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_5_STRUCTRE_CHK_RESULT_WTRATIO);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_5_STRUCTRE_CHK_RESULT_DEFLECTION()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_5_STRUCTRE_CHK_RESULT_DEFLECTION);
	return (ChkEmpty(m_strMsg));
}

//ADD Hong.js 02/11/05
LPCTSTR CDgnMsgAcs::Msg_acs_AXIAL_RESISTANCE()//064()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_AXIAL_RESISTANCE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BEND_RESISTANCE()//067()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BEND_RESISTANCE);
	return (ChkEmpty(m_strMsg));
}

// Coded by Seungjun MNet:No.2393 ('20060907)
LPCTSTR CDgnMsgAcs::Msg_acs_5_DEFLECTION_CHK_RESULT()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_5_DEFLECTION_CHK_RESULT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_2_DEFLECTION_CHK_RESULT()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_2_DEFLECTION_CHK_RESULT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_DEFLECTION()              
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_DEFLECTION);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_DEFLECTIONforColumn_h()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_DEFLECTION);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_DEFLECTIONforColumn_h0()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_DEFLECTIONforColumn_h0);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_LCB_POS_DIRX()              
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_LCB_POS_DIRX);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_LCB_POS_DIRY()              
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_LCB_POS_DIRY);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_LCB_POS_DIRZ()              
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_LCB_POS_DIRZ);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_TENS_BEND_SHEAR()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_TENS_BEND_SHEAR);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMP_BEND_SHEAR()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMP_BEND_SHEAR);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_AXIAL_BEND_SHEAR()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_AXIAL_BEND_SHEAR);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_AXIAL_BEND()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_AXIAL_BEND);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_AXIAL_BEND_IRS()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_AXIAL_FORCE_IRS);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMP_BEND_SHEAR_TORSION()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMP_BEND_SHEAR_TORSION);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_TENS_BEND_SHEAR_TORSION()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_TENS_BEND_SHEAR_TORSION);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_TORSION_STRENGTH()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_TORSION_STRENGTH);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMBINED_RATIO() 
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMBINED_RATIO);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BUCKLING_RESIS_COMP()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BUCKLING_RESIS_COMP);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BUCKLING_RESIS_BENDING()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BUCKLING_RESIS_BENDING);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BUCKLING_RESIS_COMP_BENDING()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_BUCKLING_RESIS_COMP_BENDING);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_SERVICEABLILITY_LIMIT_STATE()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_SERVICEABLILITY_LIMIT_STATE);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMBINED_RATIO_BENDING_AXIAL()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMBINED_RATIO_BENDING_AXIAL);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMBINED_RATIO_BENDING_TENS()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMBINED_RATIO_BENDING_TENS);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_COMBINED_RATIO_BENDING_COMP()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_COMBINED_RATIO_BENDING_COMP);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_FLEXURAL_BUCKLING_WITHOUT_FLEXURAL_BUCKILING()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_FLEXURAL_BUCKLING_WITHOUT_FLEXURAL_BUCKILING);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_FLEXURAL_BUCKLING_WITH_FLEXURAL_BUCKILING()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_FLEXURAL_BUCKLING_WITH_FLEXURAL_BUCKILING);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_LIP_SLOPE()
{
    m_strMsg = _LS(IDS_DGN_Msg_acs_LIP_SLOPE);
    return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_STRENGTH_CHECK()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_STRENGTH_CHECK);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHECK_STABILITY_A_PLANE_BENDING_SHAPE()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_CHECK_STABILITY_A_PLANE_BENDING_SHAPE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_STABILITY_CHECK_IN_MOMENT_PLANE_MY()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_STABILITY_CHECK_IN_MOMENT_PLANE_MY);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_STABILITY_CHECK_IN_MOMENT_PLANE_MZ()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_STABILITY_CHECK_IN_MOMENT_PLANE_MZ);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_BENDING_STABILITY_CHECK_IN_2_PLANES()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_BENDING_STABILITY_CHECK_IN_2_PLANES);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_STABILITY_CHECK_FROM_PLANE_OF_MOMENT()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_STABILITY_CHECK_FROM_PLANE_OF_MOMENT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHECK_LOCAL_STABILITY_WEB()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_CHECK_LOCAL_STABILITY_WEB);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHECK_LOCAL_STABILITY_FLANGE()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_CHECK_LOCAL_STABILITY_FLANGE);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_CHECK_SLENDERNESS_OF_ELEMENT()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_CHECK_SLENDERNESS_OF_ELEMENT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_Section_Total_CHK_Title()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_Section_Total_CHK_Title);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_Section_CHK_KSCE_ASD10_OK()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_Section_KSCE_CHK_ASD10_OK);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_Section_CHK_KSCE_ASD10_NG()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_Section_KSCE_CHK_ASD10_NG);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_REQUIRED_CLASS()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_REQIURED_CLASS);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgAcs::Msg_acs_REQUIRED_CLASS__s()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_REQIURED_CLASS__s);
	return (ChkEmpty(m_strMsg));
}