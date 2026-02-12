#include "StdAfx.h"

#include "DgnTendonAutoGenCtrl.h"
#include "DgnTendonTemplateCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

pAutoGenFunc CDgnTendonAutoGenCtrl::GetAutoGenFunc_AS(const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg)
{
	if(strName == m_aStrList[IDX_STR_LIST_AS][EN_SECT_TYPE_STRIDX_AS_SUPER_T])
	{
		if(strNameSub == _T("T1-2"))         { rArg.Init(0, FALSE, 0);	 return AutoGenFuncTndn_AS_SUPER_T; }
		else if(strNameSub == _T("T2-2"))    { rArg.Init(0, FALSE, 1);	 return AutoGenFuncTndn_AS_SUPER_T; }
		else if(strNameSub == _T("T3-2"))    { rArg.Init(0, FALSE, 2);	 return AutoGenFuncTndn_AS_SUPER_T; }
		else if(strNameSub == _T("T4-2"))    { rArg.Init(0, FALSE, 3);	 return AutoGenFuncTndn_AS_SUPER_T; }
		else if(strNameSub == _T("T5-2"))    { rArg.Init(0, FALSE, 4);	 return AutoGenFuncTndn_AS_SUPER_T; }
	}
	else if(strName == m_aStrList[IDX_STR_LIST_AS][EN_SECT_TYPE_STRIDX_AS_PLANK_GIRDER])
	{
		if(strNameSub == _T("Span-7m"))               { rArg.Init(0, FALSE, 0);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Span-8m"))          { rArg.Init(0, FALSE, 1);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Span-9m"))          { rArg.Init(0, FALSE, 2);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Span-10m"))         { rArg.Init(0, FALSE, 3);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Span-11m"))         { rArg.Init(0, FALSE, 4);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Span-12m"))         { rArg.Init(0, FALSE, 5);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Void-Span-13m"))    { rArg.Init(0, FALSE, 6);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Void-Span-14m"))    { rArg.Init(0, FALSE, 7);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Void-Span-15m"))    { rArg.Init(0, FALSE, 8);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Void-Span-16m"))    { rArg.Init(0, FALSE, 9);	  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Void-Span-17m"))    { rArg.Init(0, FALSE, 10);  return AutoGenFuncTndn_AS_PLANK_GIRDER; }
		else if(strNameSub == _T("Void-Span-18m"))    { rArg.Init(0, FALSE, 11);	return AutoGenFuncTndn_AS_PLANK_GIRDER; }
	}
	else if(strName == m_aStrList[IDX_STR_LIST_AS][EN_SECT_TYPE_STRIDX_AS_SUPER_T_RMS_2019])
	{
		if     (strNameSub == _T("T1"))    { rArg.Init(0, FALSE, 0);	 return AutoGenFuncTndn_AS_SUPER_T_RMS_2019; }
		else if(strNameSub == _T("T2"))    { rArg.Init(0, FALSE, 1);	 return AutoGenFuncTndn_AS_SUPER_T_RMS_2019; }
		else if(strNameSub == _T("T3"))    { rArg.Init(0, FALSE, 2);	 return AutoGenFuncTndn_AS_SUPER_T_RMS_2019; }
		else if(strNameSub == _T("T4"))    { rArg.Init(0, FALSE, 3);	 return AutoGenFuncTndn_AS_SUPER_T_RMS_2019; }
		else if(strNameSub == _T("T5"))    { rArg.Init(0, FALSE, 4);	 return AutoGenFuncTndn_AS_SUPER_T_RMS_2019; }
	}
	else if (strName == m_aStrList[IDX_STR_LIST_AS][EN_SECT_TYPE_STRIDX_AS_PSC_Deck_Unit_TMR_QLD])
	{
		if      (strNameSub == _T("10-11m_deck_unit")) { rArg.Init(0, FALSE, 0);	 return AutoGenFuncTndn_AS_PSC_Deck_Unit_TMR_QLD; }
		else if (strNameSub == _T("12-13m_deck_unit")) { rArg.Init(0, FALSE, 1);	 return AutoGenFuncTndn_AS_PSC_Deck_Unit_TMR_QLD; }
		else if (strNameSub == _T("14-15m_deck_unit")) { rArg.Init(0, FALSE, 2);	 return AutoGenFuncTndn_AS_PSC_Deck_Unit_TMR_QLD; }
		else if (strNameSub == _T("16-17m_deck_unit")) { rArg.Init(0, FALSE, 3);	 return AutoGenFuncTndn_AS_PSC_Deck_Unit_TMR_QLD; }
		else if (strNameSub == _T("18-19m_deck_unit")) { rArg.Init(0, FALSE, 4);	 return AutoGenFuncTndn_AS_PSC_Deck_Unit_TMR_QLD; }
		else if (strNameSub == _T("20-21m_deck_unit")) { rArg.Init(0, FALSE, 5);	 return AutoGenFuncTndn_AS_PSC_Deck_Unit_TMR_QLD; }
		else if (strNameSub == _T("22-23m_deck_unit")) { rArg.Init(0, FALSE, 6);	 return AutoGenFuncTndn_AS_PSC_Deck_Unit_TMR_QLD; }
		else if (strNameSub == _T("24-25m_deck_unit")) { rArg.Init(0, FALSE, 7);	 return AutoGenFuncTndn_AS_PSC_Deck_Unit_TMR_QLD; }
	}
	else
		ASSERT(0);


	// 아직 정의되지 않은 자동 생성 타입이다...
	return NULL;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_AS_SUPER_T( const T_AUTOTNDN_ARG& tArg )
{
	int nType = tArg.nType;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dH   = 0.0;
	double dRef = 0.0;
	double dRef2 = 0.0;
	double dRef3 = 0.0;
	if(nType == 0)       {dRef = 0.5505 + 0.1245 + 0.05;   dH = 0.750;}
	else if(nType == 1)  {dRef = 0.5740 + 0.1010 + 0.05;   dH = 1.000;}
	else if(nType == 2)  {dRef = 0.5930 + 0.1320 + 0.05;   dH = 1.200;}
	else if(nType == 3)  {dRef = 0.6215 + 0.1035 + 0.05;   dH = 1.500;}
	else if(nType == 4)  {dRef = 0.6500 + 0.0750 + 0.05;   dH = 1.800;}

	if(nType == 0 || nType == 1)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.065, TRUE, FALSE);

		for(i = 0 ; i < 7; i++)
		{
			if(i == 0 || i == 3) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.115, TRUE, FALSE);

		for(i = 0 ; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.165, TRUE, FALSE);

		for(i = 0 ; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.215, TRUE, FALSE);

		for(i = 0 ; i < 7; i++)
		{
			if(i > 3) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}
	}
	else if(nType == 2 || nType == 3)
	{
		dRef3 = 0.05;

		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.065, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(i == 0 || i == 2) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.115, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.165, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.215, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 5번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.265, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef, FALSE, TRUE);
	}	
	else if(nType == 4)
	{
		dRef3 = 0.05;

		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.065, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(i == 0 || i == 2) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.115, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(i == 0) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.165, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(i == 0) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.215, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 5번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.265, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 6번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.315, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}

	// 공통 - 최상위    
	dRef2 = dRef + (6 * 0.05) + 0.025 - 0.495 - dRef3;
	double dHeght = dH-0.065;
	STRAIGHT_DT(tempInfo, FALSE, dHeght, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, dRef2, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, dRef2, FALSE, TRUE);

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_AS_SUPER_T_RMS_2019( const T_AUTOTNDN_ARG& tArg )
{
	int nType = tArg.nType;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dH   = 0.0;
	double dRef = 0.0;
	double dRef2 = 0.0;
	double dRef3 = 0.0;
	if     (nType == 0)  {dRef = 0.5505 + 0.1245 + 0.05;   dH = 0.765;}
	else if(nType == 1)  {dRef = 0.5740 + 0.1010 + 0.05;   dH = 1.015;}
	else if(nType == 2)  {dRef = 0.5930 + 0.1320 + 0.05;   dH = 1.215;}
	else if(nType == 3)  {dRef = 0.6215 + 0.1035 + 0.05;   dH = 1.515;}
	else if(nType == 4)  {dRef = 0.6500 + 0.0750 + 0.05;   dH = 1.815;}


	if(nType == 0 || nType == 1)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.060, TRUE, FALSE);

		for(i = 0 ; i < 7; i++)
		{
			if(i == 0 || i == 3) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.110, TRUE, FALSE);

		for(i = 0 ; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.160, TRUE, FALSE);

		for(i = 0 ; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.210, TRUE, FALSE);

		for(i = 0 ; i < 7; i++)
		{
			if(i > 3) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}
	}
	else if(nType == 2 || nType == 3)
	{
		dRef3 = 0.05;

		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.060, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(i == 0 || i == 2) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.110, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.160, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.210, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 5번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.260, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef, FALSE, TRUE);
	}	
	else if(nType == 4)
	{
		dRef3 = 0.05;

		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.060, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(i == 0 || i == 2) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.110, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(i == 0) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.160, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(i == 0) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.210, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 5번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.260, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 6번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.310, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}

	// 공통 - 최상위    
	dRef2 = dRef + (6 * 0.05) + 0.025 - 0.495 - dRef3;
	double dHeght = dH-0.080;
	STRAIGHT_DT(tempInfo, FALSE, dHeght, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, dRef2, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, dRef2, FALSE, TRUE);

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_AS_PSC_Deck_Unit_TMR_QLD(const T_AUTOTNDN_ARG& tArg)
{
	int nType = tArg.nType;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dH = 0.0;
	double dRef = 0.0;

	if      (nType == 0) { dRef = 0.07;   dH = 0.50; } // 10-11m
 	else if (nType == 1) { dRef = 0.07;   dH = 0.54; } // 12-13m
	else if (nType == 2) { dRef = 0.07;   dH = 0.65; } // 14-15m
	else if (nType == 3) { dRef = 0.07;   dH = 0.70; } // 16-17m
	else if (nType == 4) { dRef = 0.07;   dH = 0.76; } // 18-19m
	else if (nType == 5) { dRef = 0.07;   dH = 0.85; } // 20-21m
	else if (nType == 6) { dRef = 0.07;   dH = 0.95; } // 22-23m
	else if (nType == 7) { dRef = 0.07;   dH = 1.10; } // 24-25m


	double dAddX = 0.0;
	// 제일 아래 1번째 줄
	STRAIGHT_DT(tempInfo, FALSE, 0.065, TRUE, FALSE);
	for (i = 0; i < 4; i++)
	{
		if      (i == 0) dAddX  = 0.0;
		else if (i == 1) dAddX += 0.045;
		else if (i >= 2) dAddX += 0.050;
		STRAIGHT_DT(tempInfo, TRUE, dRef+dAddX, TRUE/**/ , TRUE); // TURE  : 왼쪽부터
		STRAIGHT_DT(tempInfo, TRUE, dRef+dAddX, FALSE/**/, TRUE); // FALSE : 오른쪽부터
	}
		                                 
 	// 제일 아래 2번째 줄
	dAddX = 0.0;
 	STRAIGHT_DT(tempInfo, FALSE, 0.065+0.055, TRUE, FALSE);
	for (i = 0; i < 4; i++)
	{
		if      (i == 0) dAddX  = 0.0;
		else if (i == 1) dAddX += 0.045;
		else if (i >= 2) dAddX += 0.050;
		STRAIGHT_DT(tempInfo, TRUE, dRef + dAddX, TRUE , TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + dAddX, FALSE, TRUE);
	}

	// 제일 아래 3번째 줄
	dAddX = 0.0;
	STRAIGHT_DT(tempInfo, FALSE, 0.065+0.055+0.055, TRUE, FALSE);
	if (nType == 1 || nType == 4) // 12-13m, 18-19m
	{
		if(nType == 1) dAddX += 0.045;
		if(nType == 4) dAddX += 0.0;
		STRAIGHT_DT(tempInfo, TRUE, dRef + dAddX, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + dAddX, FALSE, TRUE);
	}
	else if (nType == 2 || nType == 3) // 14-15m, 16-17m
	{
		for (i = 0; i < 3; i++)
		{
			if      (i == 0) dAddX  = 0.0;
			else if (i == 1) dAddX += 0.045;
			else if (i >= 2) dAddX += 0.050;
			STRAIGHT_DT(tempInfo, TRUE, dRef + dAddX, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + dAddX, FALSE, TRUE);
		}
	}
	else if (nType == 5 || nType == 6 || nType == 7) // 20-21m, 22-23m, 24-25m
	{
		for (i = 0; i < 2; i++)
		{
			if      (i == 0) dAddX  = 0.0;
			else if (i == 1) dAddX += 0.045;
			STRAIGHT_DT(tempInfo, TRUE, dRef + dAddX, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + dAddX, FALSE, TRUE);
		}
	}

 	// 공통 - 최상위    
 	double dHeght = dH - 0.065;
	double dRef2  = 0.075;
 	STRAIGHT_DT(tempInfo, FALSE, dHeght, TRUE, FALSE);
 	STRAIGHT_DT(tempInfo, TRUE, dRef2, TRUE , TRUE);
 	STRAIGHT_DT(tempInfo, TRUE, dRef2, FALSE, TRUE);

	return TRUE;
}


BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_AS_PLANK_GIRDER( const T_AUTOTNDN_ARG& tArg )
{
	int nType = tArg.nType;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dRef = 0.075;
	
	if(nType == 0 || nType == 1)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.055, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			if(i == 2) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.105, TRUE, FALSE);

		if(nType == 1)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef, FALSE, TRUE);
		}

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.245, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}
	else if(nType == 2 || nType == 3)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.055, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			if(i == 2) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.105, TRUE, FALSE);

		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		if(nType == 3)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (3 * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (3 * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.325, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}
	else if(nType == 4 || nType == 5)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.055, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			if(i == 2) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.105, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			if(i == 2) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.155, TRUE, FALSE);

		if(nType == 5)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef, FALSE, TRUE);
		}

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.4, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}
	else if(nType == 6 || nType == 7)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.055, TRUE, FALSE);

		for(i = 0 ; i < 5; i++)
		{
			if(i == 2 || i == 3) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.105, TRUE, FALSE);

		for(i = 0 ; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.155, TRUE, FALSE);

		if(nType == 7)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef, FALSE, TRUE);
		}

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.48, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}
	else if(nType == 8 || nType == 9)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.055, TRUE, FALSE);

		for(i = 0 ; i < 5; i++)
		{
			if(i == 2 || i == 3) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.105, TRUE, FALSE);

		for(i = 0 ; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.155, TRUE, FALSE);

		for(i = 0 ; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		if(nType == 9)
		{
			// 제일 아래 3번째 줄
			STRAIGHT_DT(tempInfo, FALSE, 0.215, TRUE, FALSE);

			STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.545, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}
	else if(nType == 10 || nType == 11)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.055, TRUE, FALSE);

		for(i = 0 ; i < 5; i++)
		{
			if(i == 2 || i == 3) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.105, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.155, TRUE, FALSE);

		for(i = 0 ; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		if(nType == 11)
		{
			// 제일 아래 3번째 줄
			STRAIGHT_DT(tempInfo, FALSE, 0.215, TRUE, FALSE);

			STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.645, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}
	
	return TRUE;
}

