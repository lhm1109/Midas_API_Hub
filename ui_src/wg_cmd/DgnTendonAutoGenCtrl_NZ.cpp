#include "StdAfx.h"

#include "DgnTendonAutoGenCtrl.h"
#include "DgnTendonTemplateCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

pAutoGenFunc CDgnTendonAutoGenCtrl::GetAutoGenFunc_NZ(const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg)
{
	if(strName == m_aStrList[IDX_STR_LIST_NZ][EN_SECT_TYPE_STRIDX_NZ_SUPER_T])
	{
		if(strNameSub == _T("T1025-20m"))         { rArg.Init(0, FALSE, 0);	 return AutoGenFuncTndn_NZ_SUPER_T; }
		else if(strNameSub == _T("T1025-22.5m"))  { rArg.Init(0, FALSE, 1);	 return AutoGenFuncTndn_NZ_SUPER_T; }
		else if(strNameSub == _T("T1225-25m"))    { rArg.Init(0, FALSE, 2);	 return AutoGenFuncTndn_NZ_SUPER_T; }
		else if(strNameSub == _T("T1225-27.5m"))  { rArg.Init(0, FALSE, 3);	 return AutoGenFuncTndn_NZ_SUPER_T; }
		else if(strNameSub == _T("T1225-30m"))    { rArg.Init(0, FALSE, 4);	 return AutoGenFuncTndn_NZ_SUPER_T; }
	}
	else if(strName == m_aStrList[IDX_STR_LIST_NZ][EN_SECT_TYPE_STRIDX_NZ_I_GIRDER])
	{
		if(strNameSub == _T("I1500-18m"))       { rArg.Init(0, FALSE, 0);	 return AutoGenFuncTndn_NZ_I_GIRDER; }
		else if(strNameSub == _T("I1500-20m"))  { rArg.Init(0, FALSE, 1);	 return AutoGenFuncTndn_NZ_I_GIRDER; }
		else if(strNameSub == _T("I1600-22m"))  { rArg.Init(0, FALSE, 2);	 return AutoGenFuncTndn_NZ_I_GIRDER; }
		else if(strNameSub == _T("I1600-24m"))  { rArg.Init(0, FALSE, 3);	 return AutoGenFuncTndn_NZ_I_GIRDER; }
	}
	else if(strName == m_aStrList[IDX_STR_LIST_NZ][EN_SECT_TYPE_STRIDX_NZ_HOLLOW_CORE])
	{
		if(strNameSub == _T("SH650-Inner-16m"))           { rArg.Init(0, FALSE, 0);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH650-Outer-L-16m"))    { rArg.Init(0, FALSE, 1);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH650-Outer-R-16m"))    { rArg.Init(0, FALSE, 2);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH650-Inner-18m"))      { rArg.Init(0, FALSE, 3);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH650-Outer-L-18m"))    { rArg.Init(0, FALSE, 4);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH650-Outer-R-18m"))    { rArg.Init(0, FALSE, 5);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }		
		else if(strNameSub == _T("SH900-Inner-20m"))      { rArg.Init(0, FALSE, 6);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH900-Outer-L-20m"))    { rArg.Init(0, FALSE, 7);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH900-Outer-R-20m"))    { rArg.Init(0, FALSE, 8);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }		
		else if(strNameSub == _T("SH900-Inner-22.5m"))    { rArg.Init(0, FALSE, 9);	  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH900-Outer-L-22.5m"))  { rArg.Init(0, FALSE, 10);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH900-Outer-R-22.5m"))  { rArg.Init(0, FALSE, 11);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH900-Inner-25m"))      { rArg.Init(0, FALSE, 12);  return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH900-Outer-L-25m"))    { rArg.Init(0, FALSE, 13);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("SH900-Outer-R-25m"))    { rArg.Init(0, FALSE, 14);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }		
		else if(strNameSub == _T("DH587-Inner-12m"))      { rArg.Init(0, FALSE, 15);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("DH587-Outer-L-12m"))    { rArg.Init(0, FALSE, 16);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("DH587-Outer-R-12m"))    { rArg.Init(0, FALSE, 17);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("DH587-Inner-14m"))      { rArg.Init(0, FALSE, 18);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("DH587-Outer-L-14m"))    { rArg.Init(0, FALSE, 19);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
		else if(strNameSub == _T("DH587-Outer-R-14m"))    { rArg.Init(0, FALSE, 20);	return AutoGenFuncTndn_NZ_HOLLOW_CORE; }
	}
	else if (strName == m_aStrList[IDX_STR_LIST_NZ][EN_SECT_TYPE_STRIDX_NZ_SUPERHI])
	{
		if (strNameSub == _T("SuperHI-1500")) { rArg.Init(0, FALSE, 0);	  return AutoGenFuncTndn_NZ_SUPERHI; }
		else if (strNameSub == _T("SuperHI-1800")) { rArg.Init(0, FALSE, 1);	  return AutoGenFuncTndn_NZ_SUPERHI; }
		else if (strNameSub == _T("SuperHI-1900")) { rArg.Init(0, FALSE, 2);	  return AutoGenFuncTndn_NZ_SUPERHI; }
		else if (strNameSub == _T("SuperHI-2200")) { rArg.Init(0, FALSE, 3);	  return AutoGenFuncTndn_NZ_SUPERHI; }
	}

	// 아직 정의되지 않은 자동 생성 타입이다...
	return NULL;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_NZ_SUPER_T( const T_AUTOTNDN_ARG& tArg )
{
	int nType = tArg.nType;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dRef = 0.0;
	if(nType == 0)       dRef = 0.82149 + 0.096;
	else if(nType == 1)  dRef = 0.82149 + 0.096;
	else if(nType == 2)  dRef = 0.83800 + 0.096;
	else if(nType == 3)  dRef = 0.83800 + 0.096;
	else if(nType == 4)  dRef = 0.58800 + 0.096;		

	if(nType == 0 || nType == 1)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.065, TRUE, FALSE);

		for(i = 0 ; i < 5; i++)
		{
			if(nType == 0 && i == 4) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.115, TRUE, FALSE);

		for(i = 0 ; i < 5; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.165, TRUE, FALSE);

		for(i = 0 ; i < 5; i++)
		{
			if(nType == 0 && i == 4) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.965, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, 0.75249, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.75249, FALSE, TRUE);
	}
	else if(nType == 2 || nType == 3)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.065, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(nType == 2 && i == 5) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.115, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.165, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			if(nType == 2 && i == 5) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 1.165, TRUE, FALSE);

		if(nType == 3)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.69, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.69, FALSE, TRUE);
		}

		STRAIGHT_DT(tempInfo, TRUE, 0.75, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.75, FALSE, TRUE);		
	}
	else if(nType == 4)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.065, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.115, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.165, TRUE, FALSE);

		for(i = 0 ; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.055), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 1.165, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, 0.44, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.44, FALSE, TRUE);
	
		STRAIGHT_DT(tempInfo, TRUE, 0.50, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.50, FALSE, TRUE);		
	}

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_NZ_I_GIRDER( const T_AUTOTNDN_ARG& tArg )
{
	int nType = tArg.nType;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dRef = 0.0;
	if(nType == 0)       dRef = 0.0625;
	else if(nType == 1)  dRef = 0.0625;
	else if(nType == 2)  dRef = 0.0700;
	else if(nType == 3)  dRef = 0.0700;
	
	if(nType == 0 || nType == 1)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			if(nType == 0)
			{
				if(i == 0 || i == 1) continue;
			}			

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 4번째 줄
		if(nType == 1)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);

			STRAIGHT_DT(tempInfo, TRUE, dRef + 0.1, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + 0.1, FALSE, TRUE);
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 1.44, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.05, FALSE, TRUE);
	}
	else if(nType == 2 || nType == 3)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.24, TRUE, TRUE);
		
		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.215, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.215, FALSE, TRUE);
		
		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);

		for(i = 0 ; i < 4; i++)
		{
			if(nType == 2)
			{
				if(i == 0 || i == 1) continue;
			}
			else if(nType == 3)
			{
				if(i == 0) continue;
			}

			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + (i * 0.05), FALSE, TRUE);
		}

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.215, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.215, FALSE, TRUE);

		// 제일 아래 4번째 줄
		if(nType == 3)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);

			STRAIGHT_DT(tempInfo, TRUE, dRef + 0.1, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRef + 0.1, FALSE, TRUE);
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 1.54, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.070, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRef + 0.070, FALSE, TRUE);
	}

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_NZ_HOLLOW_CORE( const T_AUTOTNDN_ARG& tArg )
{
	int nType = tArg.nType;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dRefL = 0.0;
	double dRefR = 0.0;
	if(nType == 0)       { dRefL = 0.085; dRefR = 0.085; }
	else if(nType == 1)  { dRefL = 0.105; dRefR = 0.085; }
	else if(nType == 2)  { dRefL = 0.085; dRefR = 0.105; }
	else if(nType == 3)  { dRefL = 0.085; dRefR = 0.085; }
	else if(nType == 4)  { dRefL = 0.105; dRefR = 0.085; }
	else if(nType == 5)  { dRefL = 0.085; dRefR = 0.105; }
	else if(nType == 6)  { dRefL = 0.085; dRefR = 0.085; }
	else if(nType == 7)  { dRefL = 0.105; dRefR = 0.085; }
	else if(nType == 8)  { dRefL = 0.085; dRefR = 0.105; }
	else if(nType == 9)  { dRefL = 0.085; dRefR = 0.085; }
	else if(nType == 10) { dRefL = 0.105; dRefR = 0.085; }
	else if(nType == 11) { dRefL = 0.085; dRefR = 0.105; }
	else if(nType == 12) { dRefL = 0.085; dRefR = 0.085; }
	else if(nType == 13) { dRefL = 0.105; dRefR = 0.085; }
	else if(nType == 14) { dRefL = 0.085; dRefR = 0.105; }
	else if(nType == 15) { dRefL = 0.095; dRefR = 0.095; }
	else if(nType == 16) { dRefL = 0.095; dRefR = 0.095; }
	else if(nType == 17) { dRefL = 0.095; dRefR = 0.095; }
	else if(nType == 18) { dRefL = 0.095; dRefR = 0.095; }
	else if(nType == 19) { dRefL = 0.095; dRefR = 0.095; }
	else if(nType == 20) { dRefL = 0.095; dRefR = 0.095; }

	if(nType == 0 || nType == 3)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 9; i++)
		{
			if(i == 0 || i == 6) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		int nSize = 0;
		if(nType == 0)      nSize = 2;
		else if(nType == 3) nSize = 3;
		
		for(i = 0 ; i < nSize; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		if(nType == 3)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);

			STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.59, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 1 || nType == 2)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 9; i++)
		{
			if(i == 0 || i == 6) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		if(nType == 1)
		{
			for(i = 0 ; i < 6; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 3; i++)
			{
				if( i == 0) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}
		}
		else if(nType == 2)
		{
			for(i = 0 ; i < 3; i++)
			{
				if( i == 0) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 6; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}
		}
		
		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);

		if(nType == 1)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		}
		else if(nType == 2)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
		}
		
		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.59, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 4 || nType == 5)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);
		
		for(i = 0 ; i < 9; i++)
		{
			if(nType == 4)
			{
				if(i == 0 || i == 6) continue;
			}
			else if(nType == 5)
			{
				if(i == 0 || i == 4) continue;
			}			

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
		}

		for(i = 0 ; i < 9; i++)
		{
			if(nType == 4)
			{
				if(i == 0 || i == 4) continue;
			}
			else if(nType == 5)
			{
				if(i == 0 || i == 6) continue;
			}			

			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		int nNumL = 0;
		int nNumR = 0;

		if(nType == 4)      { nNumL = 7; nNumR = 3; }
		else if(nType == 5) { nNumL = 3; nNumR = 7; }

		for(i = 0 ; i < nNumL; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
		}

		for(i = 0 ; i < nNumR; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);

		if(nType == 4)      { nNumL = 6; nNumR = 2; }
		else if(nType == 5) { nNumL = 2; nNumR = 6; }

		for(i = 0 ; i < nNumL; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
		}

		for(i = 0 ; i < nNumR; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.59, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 6 || nType == 9 || nType == 12)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 9; i++)
		{
			if(i == 0 || i == 6) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);
		
		int nSize = 0;
		if(nType == 6)       nSize = 3;
		else if(nType == 9)  nSize = 5;
		else if(nType == 12) nSize = 7;
		
		for(i = 0 ; i < nSize; i++)
		{
			if(i == 1) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}
		
		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.84, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 7 || nType == 8)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 9; i++)
		{
			if(i == 0 || i == 6) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		if(nType == 7)
		{
			for(i = 0 ; i < 8; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 2; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + 0.25 + (i * 0.05), FALSE, TRUE);
			}
		}
		else if(nType == 8)
		{
			for(i = 0 ; i < 2; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + 0.25 + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 8; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.84, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 10 || nType == 11)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 9; i++)
		{
			if(i == 0 || i == 6) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		if(nType == 10)
		{
			for(i = 0 ; i < 9; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 9; i++)
			{
				if(i == 1 || i == 3 || i == 4) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}
		}
		else if(nType == 11)
		{
			for(i = 0 ; i < 9; i++)
			{
				if(i == 1 || i == 3 || i == 4) continue;
			
				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 9; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);

		if(nType == 10)
		{
			for(i = 0 ; i < 5; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}
		}
		else if(nType == 11)
		{
			for(i = 0 ; i < 5; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.84, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 13 || nType == 14)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 9; i++)
		{
			if(i == 0 || i == 6) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		for(i = 0 ; i < 9; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		// 제일 아래 3번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);

		if(nType == 13)
		{
			for(i = 0 ; i < 6; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 2; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}
		}
		else if(nType == 14)
		{
			for(i = 0 ; i < 2; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 6; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.84, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 15 || nType == 18)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		for(i = 0 ; i < 3; i++)
		{
			if(i == 0) continue;

			STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
		}

		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL + 0.377 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR + 0.377 + (i * 0.05), FALSE, TRUE);
		}

		STRAIGHT_DT(tempInfo, TRUE, 0.572, TRUE, TRUE);

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		if(nType == 15)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
		}
		else if(nType == 18)
		{
			for(i = 0 ; i < 2; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}

			STRAIGHT_DT(tempInfo, TRUE, 0.522, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.522, FALSE, TRUE);
		}		

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.527, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 16 || nType == 17)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		if(nType == 16)
		{
			for(i = 0 ; i < 5; i++)
			{
				if(i == 0) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			STRAIGHT_DT(tempInfo, TRUE, dRefL + 0.270, TRUE, TRUE);

			for(i = 0 ; i < 5; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + 0.270 + 0.10 + (i * 0.05), TRUE, TRUE);
			}
						
			for(i = 0 ; i < 3; i++)
			{
				if(i == 0) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}

		}
		else if(nType == 17)
		{
			for(i = 0 ; i < 3; i++)
			{
				if(i == 0) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 5; i++)
			{
				if(i == 0) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);
			}

			STRAIGHT_DT(tempInfo, TRUE, dRefR + 0.270, FALSE, TRUE);

			for(i = 0 ; i < 5; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + 0.270 + 0.10 + (i * 0.05), TRUE, TRUE);
			}

		}
	
		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		if(nType == 16)
		{
			for(i = 0 ; i < 3; i++)
			{
				if(i == 1) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefL + (i * 0.05), TRUE, TRUE);
			}

			STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
		}
		else if(nType == 17)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);

			for(i = 0 ; i < 3; i++)
			{
				if(i == 1) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefR + (i * 0.05), FALSE, TRUE);	
			}			
		}

		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.527, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}
	else if(nType == 19 || nType == 20)
	{
		// 제일 아래 1번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);

		if(nType == 19)
		{
			for(i = 0 ; i < 11; i++)
			{
				if(i == 5) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefL + 0.077 + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 2; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefR + 0.065 + (i * 0.05), FALSE, TRUE);
			}
		}
		else if(nType == 20)
		{
			for(i = 0 ; i < 2; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dRefL + 0.065 + (i * 0.05), TRUE, TRUE);
			}

			for(i = 0 ; i < 11; i++)
			{
				if(i == 5) continue;
			
				STRAIGHT_DT(tempInfo, TRUE, dRefR + 0.077 + (i * 0.05), FALSE, TRUE);
			}
		}

		// 제일 아래 2번째 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);

		if(nType == 19)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);

			for(i = 0 ; i < 9; i++)
			{
				if(i == 7) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefL + 0.177 + (i * 0.05), TRUE, TRUE);
			}

			STRAIGHT_DT(tempInfo, TRUE, dRefL, FALSE, TRUE);
		}
		else if(nType == 20)
		{
			STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);

			for(i = 0 ; i < 9; i++)
			{
				if(i == 7) continue;

				STRAIGHT_DT(tempInfo, TRUE, dRefR + 0.177 + (i * 0.05), FALSE, TRUE);
			}

			STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
		}
		
		// 제일 마지막 줄
		STRAIGHT_DT(tempInfo, FALSE, 0.527, TRUE, FALSE);

		STRAIGHT_DT(tempInfo, TRUE, dRefL, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dRefR, FALSE, TRUE);
	}

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_NZ_SUPERHI(const T_AUTOTNDN_ARG& tArg)
{
	int nType = tArg.nType;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	// 제일 아래 1번째 줄
	STRAIGHT_DT(tempInfo, FALSE, 0.065, TRUE, FALSE);

	for (i = 0; i < 8; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.098 + (i * 0.05), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.098 + (i * 0.05), FALSE, TRUE);
	}

	// 제일 아래 2번째 줄
	STRAIGHT_DT(tempInfo, FALSE, 0.115, TRUE, FALSE);

	for (i = 0; i < 9; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.098 + (i * 0.05), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.098 + (i * 0.05), FALSE, TRUE);
	}

	// 제일 아래 3번째 줄
	STRAIGHT_DT(tempInfo, FALSE, 0.165, TRUE, FALSE);

	for (i = 3; i < 9; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.098 + (i * 0.05), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.098 + (i * 0.05), FALSE, TRUE);
	}

	// 제일 아래 4번째 줄
	STRAIGHT_DT(tempInfo, FALSE, 0.215, TRUE, FALSE);

	for (i = 6; i < 9; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.098 + (i * 0.05), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.098 + (i * 0.05), FALSE, TRUE);
	}

	double dRef = 0.;

	if (nType == 0)
	{
		dRef = 1.5 - 0.06;
	}
	else if (nType == 1)
	{
		dRef = 1.8 - 0.06;
	}
	else if (nType == 2)
	{
		dRef = 1.9 - 0.06;
	}
	else if (nType == 3)
	{
		dRef = 2.2 - 0.06;
	}
	else
	{
		ASSERT(0);
	}

	// 상부 1번째 줄
	STRAIGHT_DT(tempInfo, FALSE, dRef, TRUE, FALSE);

	STRAIGHT_DT(tempInfo, TRUE, 0.248 , TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.248 + 0.15, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.248, FALSE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.248 + 0.15, FALSE, TRUE);

	return TRUE;
}