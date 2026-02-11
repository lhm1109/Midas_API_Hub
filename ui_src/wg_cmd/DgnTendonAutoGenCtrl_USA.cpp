#include "StdAfx.h"

#include "DgnTendonAutoGenCtrl.h"
#include "DgnTendonTemplateCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

pAutoGenFunc CDgnTendonAutoGenCtrl::GetAutoGenFunc_USA(const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg)
{
	if(strName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_ODOT])
	{
		if(strNameSub.Find(_T("B")) == 0)
		{
			if(strNameSub == _T("B12-36"))       { rArg.Init(0); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B12-48"))  { rArg.Init(1); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B17-36"))  { rArg.Init(0); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B17-48"))  { rArg.Init(1); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B21-36"))  { rArg.Init(0); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B21-48"))  { rArg.Init(1); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B27-36"))  { rArg.Init(0); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B27-48"))  { rArg.Init(1); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B33-36"))  { rArg.Init(0); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B33-48"))  { rArg.Init(1); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B42-36"))  { rArg.Init(0); return AutoGenFuncTndn_USA_ODOT_B; }
			else if(strNameSub == _T("B42-48"))  { rArg.Init(1); return AutoGenFuncTndn_USA_ODOT_B; }
		}
		else if(strNameSub.Find(_T("WF")) == 0)
		{
			if(strNameSub == _T("WF36-49"))      { rArg.Init();  return AutoGenFuncTndn_USA_ODOT_WF; }
			else if(strNameSub == _T("WF42-49")) { rArg.Init();  return AutoGenFuncTndn_USA_ODOT_WF; }
			else if(strNameSub == _T("WF48-49")) { rArg.Init();  return AutoGenFuncTndn_USA_ODOT_WF; }
			else if(strNameSub == _T("WF54-49")) { rArg.Init();  return AutoGenFuncTndn_USA_ODOT_WF; }
			else if(strNameSub == _T("WF60-49")) { rArg.Init();  return AutoGenFuncTndn_USA_ODOT_WF; }
			else if(strNameSub == _T("WF66-49")) { rArg.Init();  return AutoGenFuncTndn_USA_ODOT_WF; }
			else if(strNameSub == _T("WF72-49")) { rArg.Init();  return AutoGenFuncTndn_USA_ODOT_WF; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_AASHTO])
	{
		if(strNameSub == _LS(IDS_CMD_PSC_I_AASHTO_TYPE2))      { rArg.Init(0);  return AutoGenFuncTndn_USA_ODOT_ASHT; }
		else if(strNameSub == _LS(IDS_CMD_PSC_I_AASHTO_TYPE3)) { rArg.Init(1);  return AutoGenFuncTndn_USA_ODOT_ASHT; }
		else if(strNameSub == _LS(IDS_CMD_PSC_I_AASHTO_TYPE4)) { rArg.Init(2);  return AutoGenFuncTndn_USA_ODOT_ASHT; }
		else if(strNameSub == _LS(IDS_CMD_PSC_I_MODIFIED_AASHTO_TYPE4_60)) { rArg.Init(0);  return AutoGenFuncTndn_USA_ODOT_MASHT; }
		else if(strNameSub == _LS(IDS_CMD_PSC_I_MODIFIED_AASHTO_TYPE4_66)) { rArg.Init(1);  return AutoGenFuncTndn_USA_ODOT_MASHT; }
		else if(strNameSub == _LS(IDS_CMD_PSC_I_MODIFIED_AASHTO_TYPE4_72)) { rArg.Init(2);  return AutoGenFuncTndn_USA_ODOT_MASHT; }
	}
	else if(strName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_LADOT])
	{
		if(strNameSub.Find(_T("LG")) == 0)
		{
			if(strNameSub == _T("LG-25"))      { rArg.Init(0); return AutoGenFuncTndn_USA_LADOT_LG; }
			else if(strNameSub == _T("LG-36")) { rArg.Init(1); return AutoGenFuncTndn_USA_LADOT_LG; }
			else if(strNameSub == _T("LG-45")) { rArg.Init(2); return AutoGenFuncTndn_USA_LADOT_LG; }
			else if(strNameSub == _T("LG-54")) { rArg.Init(3); return AutoGenFuncTndn_USA_LADOT_LG; }
			else if(strNameSub == _T("LG-63")) { rArg.Init(4); return AutoGenFuncTndn_USA_LADOT_LG; }
			else if(strNameSub == _T("LG-72")) { rArg.Init(5); return AutoGenFuncTndn_USA_LADOT_LG; }
			else if(strNameSub == _T("LG-78")) { rArg.Init(6); return AutoGenFuncTndn_USA_LADOT_LG; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_RIDOT])
	{
		if(strNameSub.Find(_T("S")) == 0)
		{
			if(strNameSub == _T("S1-36"))      { rArg.Init(0, FALSE, 0); return AutoGenFuncTndn_USA_RIDOT_S; }
			else if(strNameSub == _T("S2-36")) { rArg.Init(0, FALSE, 1); return AutoGenFuncTndn_USA_RIDOT_S; }
			else if(strNameSub == _T("S3-36")) { rArg.Init(0, FALSE, 2); return AutoGenFuncTndn_USA_RIDOT_S; }
			else if(strNameSub == _T("S4-36")) { rArg.Init(0, FALSE, 3); return AutoGenFuncTndn_USA_RIDOT_S; }
			else if(strNameSub == _T("S1-48")) { rArg.Init(1, FALSE, 0); return AutoGenFuncTndn_USA_RIDOT_S; }
			else if(strNameSub == _T("S2-48")) { rArg.Init(1, FALSE, 1); return AutoGenFuncTndn_USA_RIDOT_S; }
			else if(strNameSub == _T("S3-48")) { rArg.Init(1, FALSE, 2); return AutoGenFuncTndn_USA_RIDOT_S; }
			else if(strNameSub == _T("S4-48")) { rArg.Init(1, FALSE, 3); return AutoGenFuncTndn_USA_RIDOT_S; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_ILDOT])
	{
		if (strNameSub.Find(_T("PPC")) == 0)
		{
			if (strNameSub == _T("PPC 36 I"))      { rArg.Init(0);  return AutoGenFuncTndn_USA_ILDOT_PPC; }
			else if (strNameSub == _T("PPC 42 I")) { rArg.Init(1);  return AutoGenFuncTndn_USA_ILDOT_PPC; }
			else if (strNameSub == _T("PPC 48 I")) { rArg.Init(2);  return AutoGenFuncTndn_USA_ILDOT_PPC; }
			else if (strNameSub == _T("PPC 54 I")) { rArg.Init(3);  return AutoGenFuncTndn_USA_ILDOT_PPC; }
		}
		else if (strNameSub.Find(_T("BULB")) == 0)
		{
			if (strNameSub == _T("BULB 63 T"))      { rArg.Init(0);  return AutoGenFuncTndn_USA_ILDOT_BULB; }
			else if (strNameSub == _T("BULB 72 T")) { rArg.Init(1);  return AutoGenFuncTndn_USA_ILDOT_BULB; }
		}
		else if (strNameSub.Find(_T("IL")) == 0)
		{
			if (strNameSub == _T("IL27-1830"))      { rArg.Init(0);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL36-2438")) { rArg.Init(1);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL36-3838")) { rArg.Init(2);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL45-2438")) { rArg.Init(3);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL45-3838")) { rArg.Init(4);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL54-2438")) { rArg.Init(5);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL54-3838")) { rArg.Init(6);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL63-2438")) { rArg.Init(7);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL63-3838")) { rArg.Init(8);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL72-2438")) { rArg.Init(9);   return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL72-3838")) { rArg.Init(10);  return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL81-3838")) { rArg.Init(11);  return AutoGenFuncTndn_USA_ILDOT_IL; }
			else if (strNameSub == _T("IL90-3838")) { rArg.Init(12);  return AutoGenFuncTndn_USA_ILDOT_IL; }			
		}
		else if (strNameSub.Find(_T("PD")) == 0)
		{
			if (strNameSub == _T("PD-1148"))      { rArg.Init(0);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-1152")) { rArg.Init(1);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-1736")) { rArg.Init(2);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-1748")) { rArg.Init(3);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-2136")) { rArg.Init(4);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-2148")) { rArg.Init(5);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-2736")) { rArg.Init(6);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-2748")) { rArg.Init(7);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-3336")) { rArg.Init(8);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-3348")) { rArg.Init(9);  return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-4236")) { rArg.Init(10); return AutoGenFuncTndn_USA_ILDOT_PD; }
			else if (strNameSub == _T("PD-4248")) { rArg.Init(11); return AutoGenFuncTndn_USA_ILDOT_PD; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_massDOT])
	{
		if(strNameSub.Find(_T("S")) == 0)
		{
			if(strNameSub == _T("S36-12"))      { rArg.Init(0); return AutoGenFuncTndn_USA_massDOT_S; }
			else if(strNameSub == _T("S36-15")) { rArg.Init(1); return AutoGenFuncTndn_USA_massDOT_S; }
			else if(strNameSub == _T("S36-18")) { rArg.Init(2); return AutoGenFuncTndn_USA_massDOT_S; }
			else if(strNameSub == _T("S36-21")) { rArg.Init(3); return AutoGenFuncTndn_USA_massDOT_S; }
			else if(strNameSub == _T("S48-12")) { rArg.Init(4); return AutoGenFuncTndn_USA_massDOT_S; }
			else if(strNameSub == _T("S48-15")) { rArg.Init(5); return AutoGenFuncTndn_USA_massDOT_S; }
			else if(strNameSub == _T("S48-18")) { rArg.Init(6); return AutoGenFuncTndn_USA_massDOT_S; }
			else if(strNameSub == _T("S48-21")) { rArg.Init(7); return AutoGenFuncTndn_USA_massDOT_S; }
		}
		else if(strNameSub.Find(_T("NEXT")) == 0)
		{
			if(strNameSub == _T("NEXT 24F-96"))       { rArg.Init(0, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-96"))  { rArg.Init(0, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-96"))  { rArg.Init(0, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-96"))  { rArg.Init(0, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 24F-102")) { rArg.Init(1, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-102")) { rArg.Init(1, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-102")) { rArg.Init(1, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-102")) { rArg.Init(1, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 24F-108")) { rArg.Init(2, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-108")) { rArg.Init(2, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-108")) { rArg.Init(2, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-108")) { rArg.Init(2, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 24F-114")) { rArg.Init(3, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-114")) { rArg.Init(3, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-114")) { rArg.Init(3, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-114")) { rArg.Init(3, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 24F-120")) { rArg.Init(4, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-120")) { rArg.Init(4, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-120")) { rArg.Init(4, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-120")) { rArg.Init(4, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 24F-126")) { rArg.Init(5, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-126")) { rArg.Init(5, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-126")) { rArg.Init(5, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-126")) { rArg.Init(5, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 24F-132")) { rArg.Init(6, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-132")) { rArg.Init(6, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-132")) { rArg.Init(6, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-132")) { rArg.Init(6, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 24F-138")) { rArg.Init(7, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-138")) { rArg.Init(7, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-138")) { rArg.Init(7, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-138")) { rArg.Init(7, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 24F-144")) { rArg.Init(8, FALSE, 0); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 28F-144")) { rArg.Init(8, FALSE, 1); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 32F-144")) { rArg.Init(8, FALSE, 2); return AutoGenFuncTndn_USA_massDOT_NF; }
			else if(strNameSub == _T("NEXT 36F-144")) { rArg.Init(8, FALSE, 3); return AutoGenFuncTndn_USA_massDOT_NF; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_MoDOT])
	{
		if(strNameSub.Find(_T("PSBXB")) == 0)
		{
			if(strNameSub == _T("PSBXB 23"))      { rArg.Init(0); return AutoGenFuncTndn_USA_MoDOT_PSBXB; }
			else if(strNameSub == _T("PSBXB 29")) { rArg.Init(1); return AutoGenFuncTndn_USA_MoDOT_PSBXB; }
			else if(strNameSub == _T("PSBXB 33")) { rArg.Init(2); return AutoGenFuncTndn_USA_MoDOT_PSBXB; }
			else if(strNameSub == _T("PSBXB 36")) { rArg.Init(3); return AutoGenFuncTndn_USA_MoDOT_PSBXB; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_TXDOT])
	{
		if(strNameSub.Find(_T("B")) == 1)
		{
			if(strNameSub == _T("4B20"))      { rArg.Init(0, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("5B20")) { rArg.Init(0, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("4B28")) { rArg.Init(1, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("5B28")) { rArg.Init(1, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("4B34")) { rArg.Init(2, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("5B34")) { rArg.Init(2, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("4B40")) { rArg.Init(3, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("5B40")) { rArg.Init(3, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("4B40_over100ft")) { rArg.Init(4, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_B; }
			else if(strNameSub == _T("5B40_over100ft")) { rArg.Init(4, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_B; }
		}
		else if(strNameSub.Find(_T("XB")) == 1)
		{
			if(strNameSub == _T("4XB20"))      { rArg.Init(0, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_XB; }
			else if(strNameSub == _T("5XB20")) { rArg.Init(0, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_XB; }
			else if(strNameSub == _T("4XB28")) { rArg.Init(1, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_XB; }
			else if(strNameSub == _T("5XB28")) { rArg.Init(1, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_XB; }
			else if(strNameSub == _T("4XB34")) { rArg.Init(2, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_XB; }
			else if(strNameSub == _T("5XB34")) { rArg.Init(2, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_XB; }
			else if(strNameSub == _T("4XB40")) { rArg.Init(3, FALSE, 0); return AutoGenFuncTndn_USA_TXDOT_XB; }
			else if(strNameSub == _T("5XB40")) { rArg.Init(3, FALSE, 1); return AutoGenFuncTndn_USA_TXDOT_XB; }
		}
	}

	// 아직 정의되지 않은 자동 생성 타입이다...
	return NULL;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_RIDOT_S(const T_AUTOTNDN_ARG& tArg)
{
	// nAdditionalNum : 0 = 36, 1 = 48
	// nType : 0 = S1, 1 = S2, 2 = S3, 3=S4

	int nAdditionalNum = tArg.nAdditionalNum;
	int nType = tArg.nType;
	int i;
	int nTemp;
	double dTemp;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0) // 구멍이 없다.
	{
		if(nAdditionalNum == 0)      nTemp = 7; // 아랫줄부터
		else if(nAdditionalNum == 1) nTemp = 10; // 아랫줄부터
		else return FALSE;

		// 아래부터 1번째
		dTemp = 0.12065;
		for(i = 0 ; i < nTemp; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}
		// 아래부터 2번째
		dTemp = 0.06985;
		nTemp += 1;
		for(i = 0 ; i < nTemp; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.20955, TRUE, FALSE); // 높이 설정
		if(nAdditionalNum == 0)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.32385, TRUE, TRUE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.32385, FALSE, TRUE); // 높이 설정
		}
		if(nAdditionalNum == 1)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE); // 높이 설정
		}
	}
	else if(nType == 1)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.17145, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.17145, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.31115, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);

		if(nAdditionalNum == 0)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.32385, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.32385, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37465, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37465, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.37465, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37465, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.31115, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.37465, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37465, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
		}
		else if(nAdditionalNum == 1)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정
			dTemp = 0.32385;
			for(i = 0; i < 5; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
				dTemp += 0.05080;
			}

			STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.47625, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.47625, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.31115, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
		}
	}
	else if(nType == 2)
	{
		if(nAdditionalNum == 0)      nTemp = 7;
		else if(nAdditionalNum == 1) nTemp = 10;
		else return FALSE;

		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정
		dTemp = 0.12084;
		for(i = 0 ; i < nTemp; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.07004, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.07004, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12084, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12084, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.07004, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.07004, FALSE, TRUE);

		if(nAdditionalNum == 0)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42564, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42564, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42564, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42564, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.36195, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.12084, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.12084, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37484, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37484, FALSE, TRUE);
		}
		else if(nAdditionalNum == 1)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.37465, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37465, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.47625, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.47625, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.36195, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
		}
	}
	else if(nType == 3)
	{
		if(nAdditionalNum == 0)      nTemp = 7;
		else if(nAdditionalNum == 1) nTemp = 10;
		else return FALSE;

		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정
		dTemp = 0.12084;
		for(i = 0 ; i < nTemp; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.07004, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.07004, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12084, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12084, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.07004, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.07004, FALSE, TRUE);

		if(nAdditionalNum == 0)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.37484, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37484, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42564, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42564, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42564, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42564, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.46355, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.12084, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.12084, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37484, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.37484, FALSE, TRUE);
		}
		else if(nAdditionalNum == 1)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정
			dTemp = 0.37465;
			for(i = 0 ; i < 4; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
				dTemp += 0.05080;
			}

			STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

			STRAIGHT_DT(tempInfo, FALSE, 0.46355, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
		}
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_LADOT_LG(const T_AUTOTNDN_ARG& tArg)
{
	// nAdditionalNum : 0=LG25, 1=LG36, 2=LG45, 3=LG54, 4=LG63, 5=LG72, 6=LG78

	int nType = tArg.nAdditionalNum;
	int i;
	int nTemp;
	double dTemp, dOriginLeft, dHeight;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0) dOriginLeft = 0.0762;
	else           dOriginLeft = 0.2286;

	if(nType == 0)      nTemp = 4;
	else if(nType == 1) nTemp = 10;
	else if(nType == 2) nTemp = 14;
	else if(nType == 3) nTemp = 19;
	else if(nType == 4) nTemp = 23;
	else if(nType == 5) nTemp = 28;
	else if(nType == 6) nTemp = 31;
	else return FALSE;

	dHeight = 0.381;
	dTemp = dOriginLeft + (0.05080 * 7.0);
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, dHeight, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);

		dHeight += 0.05080;
	}

	// 아래부터 1번째 줄
	nTemp = 8;
	dTemp = dOriginLeft;
	dHeight = 0.0762;
	STRAIGHT_DT(tempInfo, FALSE, dHeight, TRUE, FALSE); // 높이 설정
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	// 아래부터 2번째 줄
	nTemp = 8;
	dTemp = dOriginLeft;
	dHeight += 0.05080;
	STRAIGHT_DT(tempInfo, FALSE, dHeight, TRUE, FALSE); // 높이 설정
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	// 아래부터 3번째 줄
	nTemp = 6;
	dTemp = dOriginLeft + (0.05080 * 2.0);
	dHeight += 0.05080;
	STRAIGHT_DT(tempInfo, FALSE, dHeight, TRUE, FALSE); // 높이 설정
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	// 아래부터 4번째 줄
	nTemp = 4;
	dTemp = dOriginLeft + (0.05080 * 4.0);
	dHeight += 0.05080;
	STRAIGHT_DT(tempInfo, FALSE, dHeight, TRUE, FALSE); // 높이 설정
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	// 아래부터 5번째 줄
	nTemp = 3;
	dTemp = dOriginLeft + (0.05080 * 5.0);
	dHeight += 0.05080;
	STRAIGHT_DT(tempInfo, FALSE, dHeight, TRUE, FALSE); // 높이 설정
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	// 아래부터 6번째 줄
	nTemp = 2;
	dTemp = dOriginLeft + (0.05080 * 6.0);
	dHeight += 0.05080;
	STRAIGHT_DT(tempInfo, FALSE, dHeight, TRUE, FALSE); // 높이 설정
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_ILDOT_PPC(const T_AUTOTNDN_ARG& tArg)
{
	int nType = tArg.nAdditionalNum;
	int i, j;
	double dTemp, dTopHeight, dBotHeight;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if (nType == 0) // PPC 36 I
	{
		dBotHeight = 0.0508;

		for (i = 0; i < 7; i++)
		{
			dTemp = 0.0;
			if (i > 0) dBotHeight += 0.0508;			

			for (j = 0; j < 4; j++)
			{
				dTemp += 0.0508;

				STRAIGHT_DT(tempInfo, FALSE, dBotHeight, TRUE, FALSE); // 높이 설정

				if (i == 2 && (j == 0)) continue;
				if (i == 3 && (j == 0 || j == 1)) continue;
				if (i == 4 && (j == 0 || j == 1 || j == 2)) continue;
				if (i == 5 && (j == 0 || j == 1 || j == 2)) continue;
				if (i == 6 && (j == 0 || j == 1 || j == 2)) continue;

				STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);				
			}
		}

		dTopHeight = 0.5842;

		for (i = 0; i < 6; i++)
		{
			dTemp = 0.0;
			if (i > 0) dTopHeight += 0.0508;

			STRAIGHT_DT(tempInfo, FALSE, dTopHeight, TRUE, FALSE); // 높이 설정

			dTemp = 0.2032;

			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		}
	}
	else if (nType == 1 || nType == 2 || nType == 3) // PPC 42 I, PPC 48 I, PPC 54 I
	{
		dBotHeight = 0.0508;

		for (i = 0; i < 7; i++)
		{
			dTemp = 0.0;
			if (i > 0) dBotHeight += 0.0508;

			for (j = 0; j < 5; j++)
			{
				dTemp += 0.0508;

				STRAIGHT_DT(tempInfo, FALSE, dBotHeight, TRUE, FALSE); // 높이 설정

				if (i == 2 && (j == 0)) continue;
				if (i == 3 && (j == 0 || j == 1)) continue;
				if (i == 4 && (j == 0 || j == 1 || j == 2)) continue;
				if (i == 5 && (j == 0 || j == 1 || j == 2 || j == 3)) continue;
				if (i == 6 && (j == 0 || j == 1 || j == 2 || j == 3)) continue;

				STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			}
		}

		if (nType == 1)      dTopHeight = 0.6858; // PPC 42 I
		else if (nType == 2) dTopHeight = 0.8382; // PPC 48 I
		else if (nType == 3) dTopHeight = 0.9906; // PPC 54 I

		for (i = 0; i < 7; i++)
		{
			dTemp = 0.0;
			if (i > 0) dTopHeight += 0.0508;

			STRAIGHT_DT(tempInfo, FALSE, dTopHeight, TRUE, FALSE); // 높이 설정

			dTemp = 0.254;

			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		}
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_ILDOT_BULB(const T_AUTOTNDN_ARG& tArg)
{
	int nType = tArg.nAdditionalNum;
	int i, j;
	double dTemp, dTopHeight, dBotHeight;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	dBotHeight = 0.0508;

	for (i = 0; i < 8; i++)
	{
		dTemp = 0.254;
		if (i > 0) dBotHeight += 0.0508;

		for (j = 0; j < 6; j++)
		{
			if (j > 0) dTemp += 0.0508;

			STRAIGHT_DT(tempInfo, FALSE, dBotHeight, TRUE, FALSE); // 높이 설정

			if (i == 2 && (j == 0 || j == 1)) continue;
			if (i == 3 && (j == 0 || j == 1 || j == 2 || j == 3)) continue;
			if (i == 4 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4)) continue;
			if (i == 5 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4)) continue;
			if (i == 6 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4)) continue;
			if (i == 7 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4)) continue;

			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		}
	}

	if (nType == 0)      dTopHeight = 1.1684; // BULB 63 T
	else if (nType == 1) dTopHeight = 1.397;  // BULB 72 T

	for (i = 0; i < 8; i++)
	{
		dTemp = 0.0;
		if (i > 0) dTopHeight += 0.0508;

		STRAIGHT_DT(tempInfo, FALSE, dTopHeight, TRUE, FALSE); // 높이 설정

		dTemp = 0.5588;

		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_ILDOT_IL(const T_AUTOTNDN_ARG& tArg)
{
	int nType = tArg.nAdditionalNum;
	int i, j;
	double dTemp, dTopHeight, dBotHeight;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if (nType == 0) // IL27-1830
	{
		dBotHeight = 0.0508;

		for (i = 0; i < 5; i++)
		{
			dTemp = 0.0;
			if (i > 0) dBotHeight += 0.0508;

			for (j = 0; j < 7; j++)
			{
				dTemp += 0.0508;

				STRAIGHT_DT(tempInfo, FALSE, dBotHeight, TRUE, FALSE); // 높이 설정

				if (i == 2 && (j == 0 || j == 1)) continue;
				if (i == 3 && (j == 0 || j == 1 || j == 2 || j == 3)) continue;
				if (i == 4 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4 || j == 5)) continue;

				STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			}
		}

		dTopHeight = 0.4064;

		for (i = 0; i < 5; i++)
		{
			dTemp = 0.0;
			if (i > 0) dTopHeight += 0.0508;

			STRAIGHT_DT(tempInfo, FALSE, dTopHeight, TRUE, FALSE); // 높이 설정

			dTemp = 0.3556;

			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		}
	}
	else
	{
		dBotHeight = 0.0508;

		for (i = 0; i < 8; i++)
		{
			dTemp = 0.0;
			if (i > 0) dBotHeight += 0.0508;

			for (j = 0; j < 9; j++)
			{
				dTemp += 0.0508;

				STRAIGHT_DT(tempInfo, FALSE, dBotHeight, TRUE, FALSE); // 높이 설정

				if (i == 2 && (j == 0)) continue;
				if (i == 3 && (j == 0 || j == 1 || j == 2)) continue;
				if (i == 4 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4)) continue;
				if (i == 5 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4 || j == 5)) continue;
				if (i == 6 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4 || j == 5 || j == 6)) continue;
				if (i == 7 && (j == 0 || j == 1 || j == 2 || j == 3 || j == 4 || j == 5 || j == 6 || j == 7)) continue;

				STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			}
		}

		if (nType == 1)       dTopHeight = 0.5842; // IL36-2438
		else if (nType == 2)  dTopHeight = 0.5842; // IL36-3838
		else if (nType == 3)  dTopHeight = 0.8128; // IL45-2438
		else if (nType == 4)  dTopHeight = 0.8128; // IL45-3838
		else if (nType == 5)  dTopHeight = 1.0414; // IL54-2438
		else if (nType == 6)  dTopHeight = 1.0414; // IL54-3838
		else if (nType == 7)  dTopHeight = 1.2700; // IL63-2438
		else if (nType == 8)  dTopHeight = 1.2700; // IL63-3838
		else if (nType == 9)  dTopHeight = 1.4986; // IL72-2438
		else if (nType == 10) dTopHeight = 1.4986; // IL72-3838
		else if (nType == 11) dTopHeight = 1.7272; // IL81-3838
		else if (nType == 12) dTopHeight = 1.9558; // IL90-3838

		for (i = 0; i < 6; i++)
		{
			dTemp = 0.0;
			if (i > 0) dTopHeight += 0.0508;

			STRAIGHT_DT(tempInfo, FALSE, dTopHeight, TRUE, FALSE); // 높이 설정

			dTemp = 0.4572;

			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		}
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_ILDOT_PD(const T_AUTOTNDN_ARG& tArg)
{
	// nAdditionalNum : Type 0 ~ 11 = PD-1148, PD-1152..... , PD-4248

	int nType = tArg.nAdditionalNum;
	int i;
	int nTemp;
	double dTemp;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0)       { nTemp = 5; dTemp = 0.381; } //1148
	else if(nType == 1)  { nTemp = 6; dTemp = 0.381; } //1152
	else if(nType == 2)  { nTemp = 3; dTemp = 0.330; } //1736
	else if(nType == 3)  { nTemp = 5; dTemp = 0.381; } //1748
	else if(nType == 4)  { nTemp = 3; dTemp = 0.330; } //2136
	else if(nType == 5)  { nTemp = 5; dTemp = 0.381; } //2148
	else if(nType == 6)  { nTemp = 3; dTemp = 0.330; } //2736
	else if(nType == 7)  { nTemp = 5; dTemp = 0.381; } //2748
	else if(nType == 8)  { nTemp = 3; dTemp = 0.330; } //3336
	else if(nType == 9)  { nTemp = 5; dTemp = 0.381; } //3348
	else if(nType == 10) { nTemp = 3; dTemp = 0.330; } //4236
	else if(nType == 11) { nTemp = 5; dTemp = 0.381; } //4248
	else return FALSE;

	// 젤 아래 2줄 가운데
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.04445, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.09525, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}
	STRAIGHT_DT(tempInfo, FALSE, 0.04445, TRUE, FALSE); // 높이 설정
	STRAIGHT_DT(tempInfo, TRUE, 0.127, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.127, FALSE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.1778, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.1778, FALSE, TRUE);

	STRAIGHT_DT(tempInfo, FALSE, 0.09525, TRUE, FALSE); // 높이 설정
	STRAIGHT_DT(tempInfo, TRUE, 0.127, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.127, FALSE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.1778, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.1778, FALSE, TRUE);

	if(nType == 2 || nType == 3) //1736 //1748
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.29845, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
	}
	else if(nType == 4 || nType == 5) //2136 //2148
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.14605, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.19685, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.40005, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
	}
	else if(nType == 6 || nType == 7) //2736 //2748
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.14605, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.19685, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.55245, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
	}
	else if(nType == 8 || nType == 9) //3336 //3348
	{
		dTemp = 0.14605;
		for(i = 0 ; i < 5 ; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
			dTemp += 0.05080;
		}
		dTemp += 0.3048 - 0.05080;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
			dTemp += 0.05080;
		}
	}
	else if(nType == 10 || nType == 11) //4236 //4248
	{
		dTemp = 0.14605;
		for(i = 0 ; i < 7 ; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
			dTemp += 0.05080;
		}
		dTemp += 0.4064 - 0.05080;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
			dTemp += 0.05080;
		}
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_ODOT_B(const T_AUTOTNDN_ARG& tArg)
{
	// nAdditionalNum : 0=B*36, 1=B*48
	int nType = tArg.nAdditionalNum;
	int i=0;
	int nTemp=0;
	double dTemp=0.0;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0)      nTemp = 7;
	else if(nType == 1) nTemp = 10;

	dTemp = 0.1016;
	for(i = 0 ; i < nTemp ; i++)
	{
		if(i != 2)
		{
			STRAIGHT_DT(tempInfo, FALSE, 0.05080, TRUE, FALSE); // 높이 설정 1째줄
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, FALSE, 0.1016, TRUE, FALSE); // 높이 설정 2째줄
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	STRAIGHT_DT(tempInfo, FALSE, 0.1524, TRUE, FALSE); // 높이 설정 2째줄
	STRAIGHT_DT(tempInfo, TRUE, 0.1016, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.1016, FALSE, TRUE);


	return TRUE;
}
BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_ODOT_WF(const T_AUTOTNDN_ARG& tArg)
{
	int i;
	double dTemp;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	dTemp = 0.2159;
	STRAIGHT_DT(tempInfo, FALSE, 0.05080, TRUE, FALSE); // 높이 설정 1째줄
	for(i = 0 ; i < 7 ; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	dTemp = 0.1651;
	STRAIGHT_DT(tempInfo, FALSE, 0.1016, TRUE, FALSE); // 높이 설정 2째줄
	for(i = 0 ; i < 8 ; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	dTemp = 0.2667;
	STRAIGHT_DT(tempInfo, FALSE, 0.1524, TRUE, FALSE); // 높이 설정 3째줄
	for(i = 0 ; i < 6 ; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	dTemp = 0.3683;
	STRAIGHT_DT(tempInfo, FALSE, 0.2032, TRUE, FALSE); // 높이 설정 4째줄
	for(i = 0 ; i < 4 ; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	dTemp = 0.4699;
	STRAIGHT_DT(tempInfo, FALSE, 0.254, TRUE, FALSE); // 높이 설정 5째줄
	for(i = 0 ; i < 2 ; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	dTemp = 0.05080;
	STRAIGHT_DT(tempInfo, TRUE, 0.6223, TRUE, FALSE); //가운데 줄
	for(i = 0 ; i < 6 ; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, TRUE); // 높이 설정 들...
		dTemp += 0.05080;
	}

	dTemp = 0.3683;
	STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 제일 위줄.
	for(i = 0 ; i < 3 ; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}
	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_massDOT_S(const T_AUTOTNDN_ARG& tArg)
{
	int nType = tArg.nAdditionalNum;
	int i;
	double dTemp;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0) // S36-12
	{
		dTemp = 0.12065;
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정 1째줄
		for(i = 0 ; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.06985;
		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정 2째줄
		for(i = 0 ; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 위에서 1째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.22225, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.22225, FALSE, TRUE);
	}
	else if(nType == 1) // S36-15
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정 1째줄
		dTemp = 0.12065;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}
		dTemp = 0.32385;
		for(i = 0 ; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정 2째줄
		dTemp = 0.12065 - 0.05080;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}
		dTemp = 0.3285 + 0.05080;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정 3째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 4째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.37465, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.37465, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.37465 + 0.05080, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.37465 + 0.05080, FALSE, TRUE);
	}
	else if(nType == 2) // S36-18
	{
		dTemp = 0.12065;
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정 1째줄
		for(i = 0 ; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정 2째줄
		dTemp = 0.12065 - 0.05080;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정 3째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 4째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.37465, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.37465, FALSE, TRUE);
	}
	else if(nType == 3) // S36-21
	{
		dTemp = 0.12065;
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정 1째줄
		for(i = 0 ; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정 2째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985 + 0.05080, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985 + 0.05080, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545 - 0.05080, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545 - 0.05080, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정 3째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 4째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.37465, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.37465, FALSE, TRUE);
	}
	else if(nType == 4) // S48-12
	{
		dTemp = 0.12065;
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정 1째줄
		for(i = 0 ; i < 10; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.06985;
		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정 2째줄
		for(i = 0 ; i < 11; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 위에서 1째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
	}
	else if(nType == 5) // S48-15
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정 1째줄
		dTemp = 0.12065;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}
		dTemp = 0.32385;
		for(i = 0 ; i < 5; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정 2째줄
		dTemp = 0.12065 - 0.05080;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}
		dTemp = 0.42545;
		for(i = 0 ; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정 3째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 4째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
	}
	else if(nType == 6) // S48-18
	{
		dTemp = 0.12065;
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정 1째줄
		for(i = 0 ; i < 10; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정 2째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);
		dTemp = 0.37465;
		for(i = 0 ; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정 3째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 4째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
	}
	else if(nType == 7) // S48-21
	{
		dTemp = 0.12065;
		STRAIGHT_DT(tempInfo, FALSE, 0.05715, TRUE, FALSE); // 높이 설정 1째줄
		for(i = 0 ; i < 10; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.10795, TRUE, FALSE); // 높이 설정 2째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.06985, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		dTemp = 0.37465;
		for(i = 0 ; i < 4; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.15875, TRUE, FALSE); // 높이 설정 3째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);

		STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 4째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12065, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.42545, FALSE, TRUE);
	}
	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_massDOT_NF(const T_AUTOTNDN_ARG& tArg)
{
	// nAdditionalNum : 0=96, 1=102, 2=108, 3=114, 4=120, 5=126, 6=132, 7=138, 8=144
	// nType : 0=24, 1=28, 2=32, 3=36

	int nAddNum = tArg.nAdditionalNum; // 너비
	int nType = tArg.nType;            // 높이
	int i, j;
	double dTemp, dTemp2, dTemp3;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nAddNum == 0)      dTemp = 0.34925; // 96  (13.75 inch)
	else if(nAddNum == 1) dTemp = 0.42545; // 102 (16.75 inch)
	else if(nAddNum == 2) dTemp = 0.50165; // 108 (19.75 inch)
	else if(nAddNum == 3) dTemp = 0.57785; // 114 (22.75 inch)
	else if(nAddNum == 4) dTemp = 0.65405; // 120 (25.75 inch)
	else if(nAddNum == 5) dTemp = 0.73025; // 126 (28.75 inch)
	else if(nAddNum == 6) dTemp = 0.80645; // 132 (31.75 inch)
	else if(nAddNum == 7) dTemp = 0.88265; // 138 (34.75 inch)
	else if(nAddNum == 8) dTemp = 0.95885; // 144 (37.75 inch)
	else return FALSE;

	dTemp3 = 0.0635;
	for(i = 0 ; i < 5 ; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, dTemp3, TRUE, FALSE); // 높이 설정 4째줄
		dTemp2 = dTemp;
		for(j = 0 ; j < 5; j++)
		{
			if(i != 0 || (j != 0 && j != 4))
			{
				STRAIGHT_DT(tempInfo, TRUE, dTemp2, TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dTemp2, FALSE, TRUE);
			}
			dTemp2 += 0.05080;
		}
		dTemp3 += 0.05080;
	}

	STRAIGHT_DT(tempInfo, FALSE, 0.0635, FALSE, FALSE); // 높이 설정 4째줄
	STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, dTemp + 0.2032, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, dTemp + 0.2032, FALSE, TRUE);

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_MoDOT_PSBXB(const T_AUTOTNDN_ARG& tArg)
{
	// nAdditionalNum : Type 0=PSBXB 23, 1=PSBXB 29 2=PSBXB 33 3=PSBXB 36

	int nType = tArg.nAdditionalNum;
	int i;
	double dTemp;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0)      dTemp = 0.5588;
	else if(nType == 1) dTemp = 0.7112;
	else if(nType == 2) dTemp = 0.8636;
	else if(nType == 3) dTemp = 0.9398;
	else return FALSE;

	// 위에서 1 번째
	STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
	STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);
	// 위에서 2 번째
	STRAIGHT_DT(tempInfo, FALSE, 0.1524, TRUE, FALSE); // 높이 설정
	STRAIGHT_DT(tempInfo, TRUE, 0.10795, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.10795, FALSE, TRUE);

	// 젤 아래 2줄 가운데
	dTemp = 0.381;
	for(i = 0 ; i < 5; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.05080, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.1016, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	dTemp = 0.127;
	for(i = 0 ; i < 2; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.05080, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.1016, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_TXDOT_B(const T_AUTOTNDN_ARG& tArg)
{
	// nAdditionalNum = 0:B20, 1:B28, 2:B34, 3:B40 4:B40(over100ft).. 세로 사이즈
	// nType = 0:4B, 1:5B .. 가로 사이즈

	int nType = tArg.nType;
	int nAdd = tArg.nAdditionalNum;
	int i, nTemp;
	double dTemp;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);
	
	dTemp = 0.073025;
	if(nType == 0)      nTemp = 11;
	else if(nType == 1) nTemp = 14;
	else return FALSE;

	STRAIGHT_DT(tempInfo, FALSE, 0.0635, TRUE, FALSE); // 높이 설정
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	if(nAdd == 0)      nTemp = 1;
	else if(nAdd == 1) nTemp = 1;
	else if(nAdd == 2) nTemp = 4;
	else if(nAdd == 3) nTemp = 7;
	else if(nAdd == 4) nTemp = 7;
	else return FALSE;

	dTemp = 0.1143;
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.073025, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.073025, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.123825, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.123825, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.174625, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.174625, FALSE, TRUE);
		dTemp += 0.05080;
	}

	STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
	STRAIGHT_DT(tempInfo, TRUE, 0.123825, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.123825, FALSE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.174625, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.174625, FALSE, TRUE);
	dTemp += 0.05080;
	STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
	STRAIGHT_DT(tempInfo, TRUE, 0.174625, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.174625, FALSE, TRUE);

	if(nAdd == 4) // 헌치가 있는 경우.. (over 100ft)
	{
		dTemp = 0.225425;
		STRAIGHT_DT(tempInfo, FALSE, 0.1143, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp + 0.05080, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp + 0.05080, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.1651, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);		
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_TXDOT_XB(const T_AUTOTNDN_ARG& tArg)
{
	int nType = tArg.nType;
	int nAdd = tArg.nAdditionalNum;
	int i, nTemp;
	double dTemp;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0)      nTemp = 11;
	else if(nType == 1) nTemp = 14;
	else return FALSE;

	dTemp = 0.073025;
	STRAIGHT_DT(tempInfo, FALSE, 0.0635, TRUE, FALSE); // 높이 설정 아래부터 1째줄
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	dTemp = 0.073025;
	STRAIGHT_DT(tempInfo, FALSE, 0.1143, TRUE, FALSE); // 높이 설정 아래부터 1째줄
	for(i = 0 ; i < nTemp; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	if(nAdd == 0 || nAdd == 1)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.1651, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.123825, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.123825, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.174625, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.174625, FALSE, TRUE);
		STRAIGHT_DT(tempInfo, FALSE, 0.2159, TRUE, FALSE); // 높이 설정
		STRAIGHT_DT(tempInfo, TRUE, 0.174625, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.174625, FALSE, TRUE);
	}
	else if(nAdd == 2 || nAdd == 3)
	{
		dTemp = 0.1651;
		for(i = 0 ; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE); // 높이 설정
			STRAIGHT_DT(tempInfo, TRUE, 0.073025, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.073025, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.123825, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.123825, FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.174625, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.174625, FALSE, TRUE);
			dTemp += 0.05080;
		}
	}
	else return FALSE;


	return TRUE;
}


BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_ODOT_ASHT(const T_AUTOTNDN_ARG& tArg)
{
	//nAdditionalNum : 0=TYPE2, 1=TYPE3, 2=TYPE4

	int nType = tArg.nAdditionalNum;
	int i, j, nTemp;
	double dTemp, dTemp2;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0) //TYPE 2
	{
		dTemp = 0.0762;
		STRAIGHT_DT(tempInfo, FALSE, 0.05080, TRUE, FALSE); // 높이 설정 아래부터 1째줄
		for(i = 0 ; i < 2 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.0762;
		STRAIGHT_DT(tempInfo, FALSE, 0.1016, TRUE, FALSE); // 높이 설정 아래부터 2째줄
		for(i = 0 ; i < 3 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.0762;
		STRAIGHT_DT(tempInfo, FALSE, 0.1524, TRUE, FALSE); // 높이 설정 아래부터 3째줄
		for(i = 0 ; i < 2 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.127;
		STRAIGHT_DT(tempInfo, FALSE, 0.2032, TRUE, FALSE); // 높이 설정 아래부터 4째줄
		for(i = 0 ; i < 2 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		STRAIGHT_DT(tempInfo, FALSE, 0.2540, TRUE, FALSE); // 높이 설정 아래부터 5째줄
		STRAIGHT_DT(tempInfo, TRUE, 0.1778, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.1778, FALSE, TRUE);

		dTemp = 0.05080;
		STRAIGHT_DT(tempInfo, TRUE, 0.2286, FALSE, FALSE);
		for(i = 0 ; i < 6 ; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, TRUE); // 높이 설정 세로줄.
			dTemp += 0.05080;
		}
	}
	else if(nType == 1) //TYPE 3
	{
		dTemp = 0.1016;
		STRAIGHT_DT(tempInfo, FALSE, 0.05080, TRUE, FALSE); // 높이 설정 아래부터 1째줄
		for(i = 0 ; i < 2 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.05080;
		STRAIGHT_DT(tempInfo, FALSE, 0.1016, TRUE, FALSE); // 높이 설정 아래부터 2째줄
		for(i = 0 ; i < 5 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.05080;
		STRAIGHT_DT(tempInfo, FALSE, 0.1524, TRUE, FALSE); // 높이 설정 아래부터 3째줄
		for(i = 0 ; i < 3 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.1016;
		STRAIGHT_DT(tempInfo, FALSE, 0.2032, TRUE, FALSE); // 높이 설정 아래부터 4째줄
		for(i = 0 ; i < 4 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.1524;
		STRAIGHT_DT(tempInfo, FALSE, 0.2540, TRUE, FALSE); // 높이 설정 아래부터 5째줄
		for(i = 0 ; i < 3 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.2032;
		STRAIGHT_DT(tempInfo, FALSE, 0.3048, TRUE, FALSE); // 높이 설정 아래부터 6째줄
		for(i = 0 ; i < 2 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}

		dTemp = 0.2540;
		STRAIGHT_DT(tempInfo, FALSE, 0.3556, TRUE, FALSE); // 높이 설정 아래부터 7째줄
		for(i = 0 ; i < 2 ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}
	}
	else if(nType == 2) //TYPE 4
	{
		dTemp2 = 0.05080; // 높이
		for(j = 0 ; j < 7; j++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp2, TRUE, FALSE); // 높이 설정 아래부터

			if(j == 0)      { dTemp = 0.0762; nTemp = 4; }
			else if(j == 1) { dTemp = 0.0762; nTemp = 4; }
			else if(j == 2) { dTemp = 0.0762; nTemp = 4; }
			else if(j == 3) { dTemp = 0.0762; nTemp = 4; }
			else if(j == 4) { dTemp = 0.1270; nTemp = 3; }
			else if(j == 5) { dTemp = 0.1778; nTemp = 2; }
			else if(j == 6) { dTemp = 0.2286; nTemp = 1; }
			else continue;

			for(i = 0 ; i < nTemp ; i++)
			{
				STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
				STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
				dTemp += 0.05080;
			}
			dTemp2 += 0.05080;
		}

		dTemp = 0.05080;
		STRAIGHT_DT(tempInfo, TRUE, 0.3302, FALSE, FALSE);
		for(i = 0 ; i < 8 ; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, TRUE); // 높이 설정 세로줄.
			dTemp += 0.05080;
		}
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_USA_ODOT_MASHT(const T_AUTOTNDN_ARG& tArg)
{
	//nAdditionalNum : 0=TYPE4 60_T(", 1=TYPE4 66"), 2=TYPE4 72"

	int nType = tArg.nAdditionalNum;
	int i, j, nTemp;
	double dTemp, dTemp2, dGap;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	if(nType == 0)      dGap = 0.1270;
	else if(nType == 1) dGap = 0.1270;
	else if(nType == 2) dGap = 0.2794;
	else return FALSE;

	dTemp2 = 0.05080; // 높이
	for(j = 0 ; j < 7; j++)
	{
		STRAIGHT_DT(tempInfo, FALSE, dTemp2, TRUE, FALSE); // 높이 설정 아래부터

		if(j == 0)      { dTemp = 0.0762 + dGap; nTemp = 4; }
		else if(j == 1) { dTemp = 0.0762 + dGap; nTemp = 4; }
		else if(j == 2) { dTemp = 0.0762 + dGap; nTemp = 4; }
		else if(j == 3) { dTemp = 0.0762 + dGap; nTemp = 4; }
		else if(j == 4) { dTemp = 0.1270 + dGap; nTemp = 3; }
		else if(j == 5) { dTemp = 0.1778 + dGap; nTemp = 2; }
		else if(j == 6) { dTemp = 0.2286 + dGap; nTemp = 1; }
		else continue;

		for(i = 0 ; i < nTemp ; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
			dTemp += 0.05080;
		}
		dTemp2 += 0.05080;
	}

	dTemp = 0.05080;
	STRAIGHT_DT(tempInfo, TRUE, 0.3302 + dGap, FALSE, FALSE);
	for(i = 0 ; i < 8 ; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, TRUE); // 높이 설정 세로줄.
		dTemp += 0.05080;
	}
	
	dTemp = 0.0762 + dGap;
	STRAIGHT_DT(tempInfo, FALSE, 0.06985, FALSE, FALSE); // 높이 설정 젤 위줄.
	for(i = 0 ; i < 3 ; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, dTemp, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dTemp, FALSE, TRUE);
		dTemp += 0.05080;
	}

	return TRUE;
}