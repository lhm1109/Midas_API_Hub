// DgnMsgDgn.cpp: implementation of the DgnMsgDgn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnMsgDgn.h"
#include "..\choga10\choga10all.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnMsgDgn::CDgnMsgDgn()
{
	m_strMsg.Empty();
	for(int i=0;i<20;i++)
		m_strReturn[i].Empty();
	m_iNo = 0;
}

CDgnMsgDgn::~CDgnMsgDgn()
{
}

CString CDgnMsgDgn::ChkEmpty(CString str)
{
	int nEnter		 = str.Find(_T("\r\n"));
	str.Replace(_T("\r\n"),_T("\n"));

	unsigned int iMax = 20;
	if(m_iNo>=iMax)
	{
		m_iNo = m_iNo-iMax;
	}
	m_strReturn[m_iNo] = str;	
	m_iNo++;
	return m_strReturn[m_iNo-1];
}
void CDgnMsgDgn::SetLanguage(unsigned int lng)
{
	m_iLang=lng;
}

LPCTSTR CDgnMsgDgn::Msg_acs_Textout_Project()
{//IDS_ACS_TEXTOUT_PROJECT
	m_strMsg = _LS(IDS_ACS_TEXTOUT_PROJECT);
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgDgn::Msg_acs_Textout_Unit()
{//IDS_ACS_TEXTOUT_UNIT
	m_strMsg = _LS(IDS_ACS_TEXTOUT_UNIT);
	return (ChkEmpty(m_strMsg));	
}

LPCTSTR CDgnMsgDgn::Msg_acs_Textout_Title()
{//IDS_ACS_TEXTOUT_TITLE
	m_strMsg = _LS(IDS_ACS_TEXTOUT_TITLE);
	return (ChkEmpty(m_strMsg));	
}

LPCTSTR CDgnMsgDgn::Msg_acs_Table_Matl()
{
	m_strMsg = _LS(IDS_DGN_Msg_acs_Table_Matl);
	return (ChkEmpty(m_strMsg));
}

// DS_RCS_TEXTOUT_REBAR
LPCTSTR CDgnMsgDgn::Msg_rcs_Textout_Rebar()
{
	m_strMsg = _LS(IDS_DGN_Msg_rcs_Textout_Rebar);
	return (ChkEmpty(m_strMsg));
}

//----Add by Hong,jiseon 2002.7.23
LPCTSTR CDgnMsgDgn::Msg_SingleLn(int rpt)
{
	m_strMsg.Empty();
	for(int i=0; i<rpt; i++)
		m_strMsg = m_strMsg + CString(_T("-"));
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgDgn::Msg_DoubleLn(int rpt)
{
	m_strMsg.Empty();
	for(int i=0; i<rpt; i++)
		m_strMsg = m_strMsg + CString(_T("="));
	return (ChkEmpty(m_strMsg));
}

// Add by ZINU.('02.11.02). To print regular width automatically.
LPCTSTR CDgnMsgDgn::Msg_RegularLen_OK(CObjDataBase* pDB, double dValue/*0.0*/, CString strExp/*_T("")*/, int iLenLim/*85*/)
{
	CTextObj* pTObj = pDB->Get_LastTextObj();
	int iPrevLen = pTObj->m_Text.GetLength();
	//int iPrevLen = m_strMsg.GetLength();
	m_strMsg.Empty();
	CString strTxt1=_T("");		// 9.
	if(dValue > 0.0)	strTxt1.Format(_T(" < %5.1f "), dValue);
	else							strTxt1 = _T(" < 1.000 ");
	CString strTxt3 = _LS(IDS_DGN_Msg_OK);	// 4.
	int iTotLen = iPrevLen + strTxt1.GetLength() + strTxt3.GetLength() + strExp.GetLength();
	CString strTxt2=_T("");
	if(iTotLen > iLenLim-2)	strTxt2 = _T("..");
	else
	{
		for(int i=0; i<(iLenLim-iTotLen); i++)	strTxt2 = strTxt2 + _T(".");
	}
	m_strMsg = strTxt1 + strExp + strTxt2 + strTxt3;
	return (ChkEmpty(m_strMsg));
}

// Add by ZINU.('02.11.02). To print regular width automatically.
LPCTSTR CDgnMsgDgn::Msg_RegularLen_NG(CObjDataBase* pDB, double dValue/*0.0*/, CString strExp/*_T("")*/, int iLenLim/*85*/)
{
	CTextObj* pTObj = pDB->Get_LastTextObj();
	int iPrevLen = pTObj->m_Text.GetLength();
	//int iPrevLen = m_strMsg.GetLength();
	m_strMsg.Empty();
	CString strTxt1=_T("");		// 9.
	if(dValue > 0.0)	strTxt1.Format(_T(" > %5.1f "), dValue);
	else							strTxt1 = _T(" > 1.000 ");
	CString strTxt3 = _LS(IDS_DGN_Msg_NG);	// 4.
	int iTotLen = iPrevLen + strTxt1.GetLength() + strTxt3.GetLength() + strExp.GetLength();
	CString strTxt2=_T("");
	if(iTotLen > iLenLim-2)	strTxt2 = _T("..");
	else
	{
		for(int i=0; i<(iLenLim-iTotLen); i++)	strTxt2 = strTxt2 + _T(".");
	}
	m_strMsg = strTxt1 + strExp + strTxt2 + strTxt3;
	return (ChkEmpty(m_strMsg));
}

// Add by tss.('20.04.22). To print GB50017-17 Deflection result automatically.
LPCTSTR CDgnMsgDgn::Msg_DeflectLen_OK(CObjDataBase* pDB, double dValue/*0.0*/, CString strExp/*_T("")*/, int iLenLim/*85*/)
{
	CTextObj* pTObj = pDB->Get_LastTextObj();
	int iPrevLen = pTObj->m_Text.GetLength();
	//int iPrevLen = m_strMsg.GetLength();
	m_strMsg.Empty();
	CString strTxt1=_T("");		// 9.
	strTxt1.Format(_T(" < %s%5.0f "), strExp, dValue);
	CString strTxt3 = _LS(IDS_DGN_Msg_OK);	// 4.
	int iTotLen = iPrevLen + strTxt1.GetLength() + strTxt3.GetLength() + strExp.GetLength();
	CString strTxt2=_T("");
	if(iTotLen > iLenLim-2)	strTxt2 = _T("..");
	else
	{
		for(int i=0; i<(iLenLim-iTotLen); i++)	strTxt2 = strTxt2 + _T(".");
	}
	m_strMsg = strTxt1 + strTxt2 + strTxt3;
	return (ChkEmpty(m_strMsg));
}

// Add by tss.('20.04.22). To print GB50017-17 Deflection result automatically.
LPCTSTR CDgnMsgDgn::Msg_DeflectLen_NG(CObjDataBase* pDB, double dValue/*0.0*/, CString strExp/*_T("")*/, int iLenLim/*85*/)
{
	CTextObj* pTObj = pDB->Get_LastTextObj();
	int iPrevLen = pTObj->m_Text.GetLength();
	//int iPrevLen = m_strMsg.GetLength();
	m_strMsg.Empty();
	CString strTxt1=_T("");		// 9.
	strTxt1.Format(_T(" > %s%5.0f "), strExp, dValue);
	CString strTxt3 = _LS(IDS_DGN_Msg_NG);	// 4.
	int iTotLen = iPrevLen + strTxt1.GetLength() + strTxt3.GetLength() + strExp.GetLength();
	CString strTxt2=_T("");
	if(iTotLen > iLenLim-2)	strTxt2 = _T("..");
	else
	{
		for(int i=0; i<(iLenLim-iTotLen); i++)	strTxt2 = strTxt2 + _T(".");
	}
	m_strMsg = strTxt1 + strTxt2 + strTxt3;
	return (ChkEmpty(m_strMsg));
}

// Coded by Seungjun MNet:No.2393 ('20060831)
LPCTSTR CDgnMsgDgn::Msg_Deflect_OK(CObjDataBase* pDB, double dValue, CString strLCBPos, int iLenLim/*85*/)
{
	CTextObj* pTObj = pDB->Get_LastTextObj();
	int iPrevLen = pTObj->m_Text.GetLength();
	//int iPrevLen = m_strMsg.GetLength();
	m_strMsg.Empty();
	CString strTxt1=_T("");		// 9.
	strTxt1.Format(_T(" > %.4f "), dValue);

	CString strTxt3 = _LS(IDS_DGN_Msg_OK);	// 4.
	int iTotLen = iPrevLen + strTxt1.GetLength() + strLCBPos.GetLength() + strTxt3.GetLength();
	CString strTxt2=_T("");
	if(iTotLen > iLenLim-2)	strTxt2 = _T("..");
	else
	{
		for(int i=0; i<(iLenLim-iTotLen); i++)	strTxt2 = strTxt2 + _T(".");
	}
	m_strMsg = strTxt1 + strLCBPos + strTxt2 + strTxt3;
	return (ChkEmpty(m_strMsg));
}

// Coded by Seungjun MNet:No.2393 ('20060831)
LPCTSTR CDgnMsgDgn::Msg_Deflect_NG(CObjDataBase* pDB, double dValue, CString strLCBPos, int iLenLim/*85*/)
{
	CTextObj* pTObj = pDB->Get_LastTextObj();
	int iPrevLen = pTObj->m_Text.GetLength();
	//int iPrevLen = m_strMsg.GetLength();
	m_strMsg.Empty();
	CString strTxt1=_T("");		// 9.
	strTxt1.Format(_T(" < %.4f "), dValue);

	CString strTxt3 = _LS(IDS_DGN_Msg_NG);	// 4.
	int iTotLen = iPrevLen + strTxt1.GetLength() + strLCBPos.GetLength() + strTxt3.GetLength();
	CString strTxt2=_T("");
	if(iTotLen > iLenLim-2)	strTxt2 = _T("..");
	else
	{
		for(int i=0; i<(iLenLim-iTotLen); i++)	strTxt2 = strTxt2 + _T(".");
	}
	m_strMsg = strTxt1 + strLCBPos + strTxt2 + strTxt3;
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgDgn::Msg_Regular_OK(CObjDataBase* pDB, CString strValue, CString strSign, CString strExp, int iLenLim)
{
	CTextObj* pTObj = pDB->Get_LastTextObj();
	int iPrevLen = pTObj->m_Text.GetLength();
	//int iPrevLen = m_strMsg.GetLength();
	m_strMsg.Empty();
	CString strTxt1 = _T("");		// 9.
	strTxt1.Format(_T(" %s %s"), strSign, strValue);
	CString strTxt3 = _LS(IDS_DGN_Msg_OK);	// 4.
	int iTotLen = iPrevLen + strTxt1.GetLength() + strTxt3.GetLength() + strExp.GetLength();
	CString strTxt2 = _T("");
	if (iTotLen > iLenLim - 2)	strTxt2 = _T("..");
	else
	{
		for (int i = 0; i < (iLenLim - iTotLen); i++)	strTxt2 = strTxt2 + _T(".");
	}
	m_strMsg = strTxt1 + strExp + strTxt2 + strTxt3;
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgDgn::Msg_Regular_NG(CObjDataBase* pDB, CString strValue, CString strSign, CString strExp, int iLenLim)
{
	CTextObj* pTObj = pDB->Get_LastTextObj();
	int iPrevLen = pTObj->m_Text.GetLength();
	//int iPrevLen = m_strMsg.GetLength();
	m_strMsg.Empty();
	CString strTxt1 = _T("");		// 9.
	strTxt1.Format(_T(" %s %s"), strSign, strValue);
	CString strTxt3 = _LS(IDS_DGN_Msg_NG);	// 4.
	int iTotLen = iPrevLen + strTxt1.GetLength() + strTxt3.GetLength() + strExp.GetLength();
	CString strTxt2 = _T("");
	if (iTotLen > iLenLim - 2)	strTxt2 = _T("..");
	else
	{
		for (int i = 0; i < (iLenLim - iTotLen); i++)	strTxt2 = strTxt2 + _T(".");
	}
	m_strMsg = strTxt1 + strExp + strTxt2 + strTxt3;
	return (ChkEmpty(m_strMsg));
}

LPCTSTR CDgnMsgDgn::Get_MsgNation()
{
    return CDgnMsgDgn::Get_MsgNation(m_iLang);
}

LPCTSTR CDgnMsgDgn::Get_MsgNation(const int iLang)
{
    switch (iLang)
    {
        case iDGN_ENG:
            return NATION_MSG_US;
        case iDGN_KOR:
            return NATION_MSG_KR;
        case iDGN_JPN:
            return NATION_MSG_JP;
        case iDGN_CHN:
            return NATION_MSG_CH;
        case iDGN_RUS:
            return NATION_MSG_RU;
        default:
            break;
    }
    return NATION_MSG_US;
}