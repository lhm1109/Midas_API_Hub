// DgnMsgDgn.h: interface for the DgnMsgDgn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNMSGDGN_H__081B6D85_33D8_4083_88DD_7C962D81464A__INCLUDED_)
#define AFX_DGNMSGDGN_H__081B6D85_33D8_4083_88DD_7C962D81464A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\choga10\wg_choga_ObjDB.h"
#include "..\MIT_Lib\rptostream.h"

//----------Add by Hong,jiseon. 2002.07.12
#include "HeaderPre.h"
//-----------Add by Yellowbug 2002.07.06


#ifdef _MGEN
	#if defined(_US) || defined(_RUS)
		#define iDGN_ENG 0
		#define iDGN_KOR 3
		#define iDGN_JPN 1
		#define iDGN_CHN 2
		#define iDGN_RUS 4	
	#elif defined(_JP)
		#define iDGN_ENG 1
		#define iDGN_KOR 3
		#define iDGN_JPN 0
		#define iDGN_CHN 2
		#define iDGN_RUS 4	
	#elif defined(_CH)
		#define iDGN_ENG 1
		#define iDGN_KOR 3
		#define iDGN_JPN 2
		#define iDGN_CHN 0
		#define iDGN_RUS 4	
	#else
		#define iDGN_ENG 0
		#define iDGN_KOR 3
		#define iDGN_JPN 1
		#define iDGN_CHN 2
		#define iDGN_RUS 4	
	#endif
#else
	#if defined(_RUS)
		#define iDGN_ENG 1
		#define iDGN_KOR 4
		#define iDGN_JPN 2
		#define iDGN_CHN 3
		#define iDGN_RUS 0	
	#else
		#define iDGN_ENG 0
		#define iDGN_KOR 3
		#define iDGN_JPN 1
		#define iDGN_CHN 2
		#define iDGN_RUS 4	
	#endif
#endif

// For message ini folder
#define NATION_MSG_KR  _T("Kor")
#define NATION_MSG_US  _T("Eng")
#define NATION_MSG_JP  _T("Jap")
#define NATION_MSG_CH  _T("Ch")
#define NATION_MSG_IN  _T("Eng")
#define NATION_MSG_TR  _T("Tur") // Turkish
#define NATION_MSG_CZ  _T("Cze") // Czech
#define NATION_MSG_PO  _T("Pol") // Polish
#define NATION_MSG_RU  _T("Rus") // Russia

class __MY_EXT_CLASS__ CDgnMsgDgn  
{
protected:
	CString m_strMsg;
//----------Add by Hong,jiseon 2002.7.12
	CString m_strReturn[20]; //for return many messages(Max 10)
	unsigned int m_iNo;

public:
	CDgnMsgDgn();
	virtual ~CDgnMsgDgn();

//----------Add by Hong,jiseon 2002.7.12
	CString ChkEmpty(CString str);

//-----------Add by Yellowbug 2002.07.10
// Convert 'IDS_ACS...' messages to Functions
	//IDS_ACS_TEXTOUT_PROJECT
	LPCTSTR Msg_acs_Textout_Project();
	//IDS_ACS_TEXTOUT_UNIT
	LPCTSTR Msg_acs_Textout_Unit();
	//IDS_ACS_TEXTOUT_TITLE
	LPCTSTR Msg_acs_Textout_Title();
	
	LPCTSTR Msg_acs_Table_Matl();
//------------Add by Hong,jiseon 2002.7.12 SRC에서 사용되므로 상위로 올림.
//IDS_RCS_TEXTOUT_REBAR
	LPCTSTR Msg_rcs_Textout_Rebar();

//------------Add by Hong,jiseon 2002.7.23
	LPCTSTR Msg_SingleLn(int rpt);
	LPCTSTR Msg_DoubleLn(int rpt);

// Add by ZINU.('02.11.02). To print regular width automatically.
	LPCTSTR Msg_RegularLen_OK(CObjDataBase* pDB, double dValue=0.0, CString strExp=_T(""), int iLenLim=95);
	LPCTSTR Msg_RegularLen_NG(CObjDataBase* pDB, double dValue=0.0, CString strExp=_T(""), int iLenLim=95);

	// Add by tss.('20.04.22). To print GB50017-17 Deflection result automatically.
	LPCTSTR Msg_DeflectLen_OK(CObjDataBase* pDB, double dValue=0.0, CString strExp=_T(""), int iLenLim=95);
	LPCTSTR Msg_DeflectLen_NG(CObjDataBase* pDB, double dValue=0.0, CString strExp=_T(""), int iLenLim=95);

	// Coded by Seungjun MNet:No.2393 ('20060831)
	LPCTSTR Msg_Deflect_OK(CObjDataBase* pDB, double dValue, CString strLCBPos=_T(""), int iLenLim=95);
	LPCTSTR Msg_Deflect_NG(CObjDataBase* pDB, double dValue, CString strLCBPos=_T(""), int iLenLim=95);

    LPCTSTR Get_MsgNation();

	LPCTSTR Msg_Regular_OK(CObjDataBase* pDB, CString strValue, CString strSign = _T("<"), CString strExp = _T(""), int iLenLim = 95);
	LPCTSTR Msg_Regular_NG(CObjDataBase* pDB, CString strValue, CString strSign = _T(">"), CString strExp = _T(""), int iLenLim = 95);
//-----------Add by Yellowbug 2002.07.06
protected:
	unsigned int m_iLang;

public:
		void SetLanguage(unsigned int lng);
        static LPCTSTR Get_MsgNation(const int iLang);

};

#include "HeaderPost.h"
#endif // !defined(AFX_DGNMSGDGN_H__081B6D85_33D8_4083_88DD_7C962D81464A__INCLUDED_)
