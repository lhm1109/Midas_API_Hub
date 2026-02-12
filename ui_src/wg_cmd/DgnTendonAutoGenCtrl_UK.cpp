#include "StdAfx.h"

#include "DgnTendonAutoGenCtrl.h"
#include "DgnTendonTemplateCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

pAutoGenFunc CDgnTendonAutoGenCtrl::GetAutoGenFunc_UK(const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg)
{
	if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_M])
	{
		if(strNameSub.Find(_T("M")) == 0)
		{
			if(strNameSub == _T("M1"))          { rArg.Init(0);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M680"))   { rArg.Init(1);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M2"))     { rArg.Init(2);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M3"))     { rArg.Init(3);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M4"))     { rArg.Init(4);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M5"))     { rArg.Init(5);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M6"))     { rArg.Init(6);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M7"))     { rArg.Init(7);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M8"))     { rArg.Init(8);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M9"))     { rArg.Init(9);	 return AutoGenFuncTndn_UK_M; }
			else if(strNameSub == _T("M10"))    { rArg.Init(10); return AutoGenFuncTndn_UK_M; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_SY])
	{
		if(strNameSub.Find(_T("SY")) == 0)
		{
			if(strNameSub == _T("SY1"))		 { rArg.Init(0); return AutoGenFuncTndn_UK_SY; }
			else if(strNameSub == _T("SY2")) { rArg.Init(1); return AutoGenFuncTndn_UK_SY; }
			else if(strNameSub == _T("SY3")) { rArg.Init(2); return AutoGenFuncTndn_UK_SY; }
			else if(strNameSub == _T("SY4")) { rArg.Init(3); return AutoGenFuncTndn_UK_SY; }
			else if(strNameSub == _T("SY5")) { rArg.Init(4); return AutoGenFuncTndn_UK_SY; }
			else if(strNameSub == _T("SY6")) { rArg.Init(5); return AutoGenFuncTndn_UK_SY; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_U])
	{
		if(strNameSub.Find(_T("U")) == 0)
		{
			if(strNameSub == _T("U1-35 35")       || strNameSub == _T("U1"))   { rArg.Init(0); return AutoGenFuncTndn_UK_U; }
			else if(strNameSub == _T("U3-35 35")  || strNameSub == _T("U3"))   { rArg.Init(1); return AutoGenFuncTndn_UK_U; }
			else if(strNameSub == _T("U5-35 35")  || strNameSub == _T("U5"))   { rArg.Init(2); return AutoGenFuncTndn_UK_U; }
			else if(strNameSub == _T("U7-35 35")  || strNameSub == _T("U7"))   { rArg.Init(3); return AutoGenFuncTndn_UK_U; }
			else if(strNameSub == _T("U8-35 35")  || strNameSub == _T("U8"))   { rArg.Init(4); return AutoGenFuncTndn_UK_U; }
			else if(strNameSub == _T("U9-35 35")  || strNameSub == _T("U9"))   { rArg.Init(5); return AutoGenFuncTndn_UK_U; }
			else if(strNameSub == _T("U10-35 35") || strNameSub == _T("U10"))  { rArg.Init(6); return AutoGenFuncTndn_UK_U; }
			else if(strNameSub == _T("U11-35 35") || strNameSub == _T("U11"))  { rArg.Init(7); return AutoGenFuncTndn_UK_U; }
			else if(strNameSub == _T("U12-35 35") || strNameSub == _T("U12"))  { rArg.Init(8); return AutoGenFuncTndn_UK_U; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_Y])
	{
		if(strNameSub.Find(_T("Y")) == 0)
		{
			if(strNameSub == _T("Y1"))      { rArg.Init(0); return AutoGenFuncTndn_UK_Y; }
			else if(strNameSub == _T("Y2")) { rArg.Init(0); return AutoGenFuncTndn_UK_Y; }
			else if(strNameSub == _T("Y3")) { rArg.Init(1); return AutoGenFuncTndn_UK_Y; }
			else if(strNameSub == _T("Y4")) { rArg.Init(2); return AutoGenFuncTndn_UK_Y; }
			else if(strNameSub == _T("Y5")) { rArg.Init(3); return AutoGenFuncTndn_UK_Y; }
			else if(strNameSub == _T("Y6")) { rArg.Init(4); return AutoGenFuncTndn_UK_Y; }
			else if(strNameSub == _T("Y7")) { rArg.Init(5); return AutoGenFuncTndn_UK_Y; }
			else if(strNameSub == _T("Y8")) { rArg.Init(6); return AutoGenFuncTndn_UK_Y; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_YE])
	{
		if(strNameSub.Find(_T("YE")) == 0)
		{
			if(strNameSub == _T("YE1"))         { rArg.Init(0, FALSE); return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE2"))    { rArg.Init(0, FALSE); return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE3"))    { rArg.Init(1, FALSE); return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE4"))    { rArg.Init(2, FALSE); return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE5"))    { rArg.Init(3, FALSE); return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE6"))    { rArg.Init(4, FALSE); return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE7"))    { rArg.Init(5, FALSE); return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE8"))    { rArg.Init(6, FALSE); return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE1-1"))  { rArg.Init(0, TRUE);  return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE2-1"))  { rArg.Init(0, TRUE);  return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE3-1"))  { rArg.Init(1, TRUE);  return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE4-1"))  { rArg.Init(2, TRUE);  return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE5-1"))  { rArg.Init(3, TRUE);  return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE6-1"))  { rArg.Init(4, TRUE);  return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE7-1"))  { rArg.Init(5, TRUE);  return AutoGenFuncTndn_UK_YE; }
			else if(strNameSub == _T("YE8-1"))  { rArg.Init(6, TRUE);  return AutoGenFuncTndn_UK_YE; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_W])
	{
		if(strNameSub.Find(_T("W")) == 0)
		{
			if(strNameSub == _T("W1"))       { rArg.Init(0);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W3"))  { rArg.Init(1);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W5"))  { rArg.Init(2);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W7"))  { rArg.Init(3);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W8"))  { rArg.Init(4);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W9"))  { rArg.Init(5);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W10")) { rArg.Init(6);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W11")) { rArg.Init(7);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W12")) { rArg.Init(8);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W13")) { rArg.Init(9);  return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W14")) { rArg.Init(10); return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W15")) { rArg.Init(11); return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W16")) { rArg.Init(12); return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W17")) { rArg.Init(13); return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W18")) { rArg.Init(14); return AutoGenFuncTndn_UK_W; }
			else if(strNameSub == _T("W19")) { rArg.Init(15); return AutoGenFuncTndn_UK_W; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_UMB])
	{
		if(strNameSub.Find(_T("UMB")) == 0)
		{
			if(strNameSub == _T("UMB1"))        { rArg.Init(6);	 return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB680")) { rArg.Init(7);	 return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB2"))   { rArg.Init(8);	 return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB3"))   { rArg.Init(9);	 return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB4"))   { rArg.Init(11); return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB5"))   { rArg.Init(12); return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB6"))   { rArg.Init(14); return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB7"))   { rArg.Init(15); return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB8"))   { rArg.Init(17); return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB9"))   { rArg.Init(19); return AutoGenFuncTndn_UK_UMB; }
			else if(strNameSub == _T("UMB10"))  { rArg.Init(20); return AutoGenFuncTndn_UK_UMB; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_MY])
	{
		if(strNameSub.Find(_T("MY")) == 0)
		{
			if(strNameSub == _T("MY1"))      { rArg.Init(0); return AutoGenFuncTndn_UK_MY; }
			else if(strNameSub == _T("MY2")) { rArg.Init(1); return AutoGenFuncTndn_UK_MY; }
			else if(strNameSub == _T("MY3")) { rArg.Init(2); return AutoGenFuncTndn_UK_MY; }
			else if(strNameSub == _T("MY4")) { rArg.Init(3); return AutoGenFuncTndn_UK_MY; }
			else if(strNameSub == _T("MY5")) { rArg.Init(4); return AutoGenFuncTndn_UK_MY; }
			else if(strNameSub == _T("MY6")) { rArg.Init(5); return AutoGenFuncTndn_UK_MY; }
			else if(strNameSub == _T("MY7")) { rArg.Init(6); return AutoGenFuncTndn_UK_MY; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_MYE])
	{
		if(strNameSub.Find(_T("MYE")) == 0)
		{
			if(strNameSub == _T("MYE1"))        { rArg.Init(0, FALSE); return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE2"))   { rArg.Init(1, FALSE); return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE3"))   { rArg.Init(2, FALSE); return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE4"))   { rArg.Init(3, FALSE); return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE5"))   { rArg.Init(4, FALSE); return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE6"))   { rArg.Init(5, FALSE); return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE7"))   { rArg.Init(6, FALSE); return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE1-1")) { rArg.Init(0, TRUE);  return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE2-1")) { rArg.Init(1, TRUE);  return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE3-1")) { rArg.Init(2, TRUE);  return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE4-1")) { rArg.Init(3, TRUE);  return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE5-1")) { rArg.Init(4, TRUE);  return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE6-1")) { rArg.Init(5, TRUE);  return AutoGenFuncTndn_UK_MYE; }
			else if(strNameSub == _T("MYE7-1")) { rArg.Init(6, TRUE);  return AutoGenFuncTndn_UK_MYE; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_SD])
	{
		if(strNameSub.Find(_T("SD")) == 0)
		{
			if(strNameSub == _T("SD1(1)"))      { rArg.Init(1 , TRUE, 1);   return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD2(1)")) { rArg.Init(3 , TRUE, 1);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD3(1)")) { rArg.Init(5 , TRUE, 1);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD4(1)")) { rArg.Init(7 , TRUE, 1);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD5(1)")) { rArg.Init(9 , TRUE, 1);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD6(1)")) { rArg.Init(11, TRUE, 1);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD7(1)")) { rArg.Init(13, TRUE, 1);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD8(1)")) { rArg.Init(15, TRUE, 1);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD1(2)")) { rArg.Init(1 , TRUE, 2);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD2(2)")) { rArg.Init(3 , TRUE, 2);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD3(2)")) { rArg.Init(5 , TRUE, 2);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD4(2)")) { rArg.Init(7 , TRUE, 2);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD5(2)")) { rArg.Init(9 , TRUE, 2);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD6(2)")) { rArg.Init(11, TRUE, 2);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD7(2)")) { rArg.Init(13, TRUE, 2);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD8(2)")) { rArg.Init(15, TRUE, 2);   return AutoGenFuncTndn_UK_SD; }   
			else if(strNameSub == _T("SD1(3)")) { rArg.Init(1 , TRUE, 3);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD2(3)")) { rArg.Init(3 , TRUE, 3);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD3(3)")) { rArg.Init(5 , TRUE, 3);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD4(3)")) { rArg.Init(7 , TRUE, 3);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD5(3)")) { rArg.Init(9 , TRUE, 3);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD6(3)")) { rArg.Init(11, TRUE, 3);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD7(3)")) { rArg.Init(13, TRUE, 3);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD8(3)")) { rArg.Init(15, TRUE, 3);   return AutoGenFuncTndn_UK_SD; }   
			else if(strNameSub == _T("SD1(4)")) { rArg.Init(1 , TRUE, 4);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD2(4)")) { rArg.Init(3 , TRUE, 4);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD3(4)")) { rArg.Init(5 , TRUE, 4);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD4(4)")) { rArg.Init(7 , TRUE, 4);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD5(4)")) { rArg.Init(9 , TRUE, 4);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD6(4)")) { rArg.Init(11, TRUE, 4);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD7(4)")) { rArg.Init(13, TRUE, 4);	return AutoGenFuncTndn_UK_SD; }
			else if(strNameSub == _T("SD8(4)")) { rArg.Init(15, TRUE, 4);	return AutoGenFuncTndn_UK_SD; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_SYE])
	{
		if(strNameSub.Find(_T("SYE")) == 0)
		{
			if(strNameSub == _T("SYE1"))        { rArg.Init(5,  FALSE); return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE2"))   { rArg.Init(6,  FALSE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE3"))   { rArg.Init(7,  FALSE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE4"))   { rArg.Init(8,  FALSE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE5"))   { rArg.Init(9,  FALSE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE6"))   { rArg.Init(10, FALSE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE1-1")) { rArg.Init(5,  TRUE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE2-1")) { rArg.Init(6,  TRUE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE3-1")) { rArg.Init(7,  TRUE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE4-1")) { rArg.Init(8,  TRUE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE5-1")) { rArg.Init(9,  TRUE);	return AutoGenFuncTndn_UK_SYE; }
			else if(strNameSub == _T("SYE6-1")) { rArg.Init(10, TRUE);	return AutoGenFuncTndn_UK_SYE; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_TYE_S] || strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_TYE_B])
	{
		if(strNameSub.Find(_T("TYE")) == 0)
		{
			if(strNameSub == _T("TYE1"))         { rArg.Init(1,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE2"))    { rArg.Init(2,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE3"))    { rArg.Init(3,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE4"))    { rArg.Init(4,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE5"))    { rArg.Init(5,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE6"))    { rArg.Init(6,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE7"))    { rArg.Init(7,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE8"))    { rArg.Init(8,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE9"))    { rArg.Init(9,  FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE10"))   { rArg.Init(10, FALSE); return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE11"))   { rArg.Init(11, FALSE); return AutoGenFuncTndn_UK_TYE_BS; }   
			else if(strNameSub == _T("TYE1-1"))  { rArg.Init(1,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE2-1"))  { rArg.Init(2,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE3-1"))  { rArg.Init(3,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE4-1"))  { rArg.Init(4,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE5-1"))  { rArg.Init(5,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE6-1"))  { rArg.Init(6,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE7-1"))  { rArg.Init(7,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE8-1"))  { rArg.Init(8,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE9-1"))  { rArg.Init(9,  TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE10-1")) { rArg.Init(10, TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
			else if(strNameSub == _T("TYE11-1")) { rArg.Init(11, TRUE);	 return AutoGenFuncTndn_UK_TYE_BS; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_TY_S] || strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_TY_B])
	{
		if(strNameSub.Find(_T("TY")) == 0)
		{
			if(strNameSub == _T("TY1"))       { rArg.Init(1);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY2"))  { rArg.Init(2);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY3"))  { rArg.Init(3);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY4"))  { rArg.Init(4);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY5"))  { rArg.Init(5);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY6"))  { rArg.Init(6);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY7"))  { rArg.Init(7);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY8"))  { rArg.Init(8);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY9"))  { rArg.Init(9);  return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY10")) { rArg.Init(10); return AutoGenFuncTndn_UK_TY_BS; }
			else if(strNameSub == _T("TY11")) { rArg.Init(11); return AutoGenFuncTndn_UK_TY_BS; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_T])
	{
		if(strNameSub.Find(_T("T")) == 0)
		{
			if(strNameSub == _T("T1"))       { rArg.Init(4);  return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T2"))  { rArg.Init(5);  return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T3"))  { rArg.Init(8);  return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T4"))  { rArg.Init(9);  return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T5"))  { rArg.Init(10); return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T6"))  { rArg.Init(11); return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T7"))  { rArg.Init(12); return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T8"))  { rArg.Init(13); return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T9"))  { rArg.Init(14); return AutoGenFuncTndn_UK_T; }
			else if(strNameSub == _T("T10")) { rArg.Init(15); return AutoGenFuncTndn_UK_T; }
		}
	}
	else if(strName == m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_U_SU])
	{
		if(strNameSub.Find(_T("U")) == 0)
		{
			if(strNameSub == _T("U3"))        { rArg.Init(3);  return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("U5"))   { rArg.Init(4);  return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("U7"))   { rArg.Init(5);  return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("U8"))   { rArg.Init(6);  return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("U9"))   { rArg.Init(7);  return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("U10"))  { rArg.Init(8);  return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("U11"))  { rArg.Init(9);  return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("U12"))  { rArg.Init(10); return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("SU11")) { rArg.Init(9);  return AutoGenFuncTndn_UK_U_SU; }
			else if(strNameSub == _T("SU12")) { rArg.Init(10); return AutoGenFuncTndn_UK_U_SU; }
			// 		else if(strNameSub == _T("U600")) { rArg.Init(0);  return pFunc; } //이거 세개는 일단 정확하지 않아서 보류...
			// 		else if(strNameSub == _T("U700")) { rArg.Init(1);  return pFunc; }
			// 		else if(strNameSub == _T("U1"))   { rArg.Init(2);  return pFunc; }
		}
	}

	// 아직 정의되지 않은 자동 생성 타입이다...
	return NULL;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_SY(const T_AUTOTNDN_ARG& tArg)
{
	int nAdditionalNum = tArg.nAdditionalNum;

	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	//제일 아래 1줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);
	for(i = 0 ; i < 5; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.12 + (i * 0.045), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12 + (i * 0.045), FALSE, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.375, FALSE, TRUE);

	//제일 아래 2줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);
	for(i = 0 ; i < 6; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.075 + (i * 0.045), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.075 + (i * 0.045), FALSE, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.35, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.35, FALSE, TRUE);

	//제일 아래 3줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);
	for(i = 0 ; i < 6; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.075 + (i * 0.045), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.075 + (i * 0.045), FALSE, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.35, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.35, FALSE, TRUE);

	//제일 아래 4줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);
	for(i = 0 ; i < 5; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.12 + (i * 0.045), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.12 + (i * 0.045), FALSE, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.35, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.35, FALSE, TRUE);

	//제일 아래 5줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.26, TRUE, FALSE);
	for(i = 0 ; i < 4; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.165 + (i * 0.045), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.165 + (i * 0.045), FALSE, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.35, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.35, FALSE, TRUE);

	//제일 아래 6줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.31, TRUE, FALSE);
	for(i = 0 ; i < 2; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.255 + (i * 0.045), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.255 + (i * 0.045), FALSE, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.35, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.35, FALSE, TRUE);

	if(nAdditionalNum < 0 || nAdditionalNum > 5)
	{
		ASSERT(0);
		nAdditionalNum = 0;
	}

	//세로줄
	for(i = 0 ; i < 6 + nAdditionalNum; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.9 + (i * 0.1), TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.35, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.35, FALSE, TRUE);
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_TYE_BS(const T_AUTOTNDN_ARG& tArg)
{
	int nAdditionalNum = tArg.nAdditionalNum;
	BOOL bRev = tArg.bRev;

	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	BOOL bSymm;
	if(bRev)  bSymm = FALSE;
	else      bSymm = TRUE;

	double dStempLen[15];
	int    nSize;

	//제일 아래 1줄.
	SetTempAry(nSize, dStempLen, 0.081, 0.049, 0.049, 0.049, 0.045, 0.045, 0.057, 0.057, 0.045, 0.045, 0.049, 0.049, 0.049, TEMP_ARY_END);
	SetLine(0.06, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 2줄.
	SetTempAry(nSize, dStempLen, 0.060, 0.070, 0.049, 0.049, 0.045, 0.045, 0.057, 0.057, 0.045, 0.045, 0.049, 0.049, TEMP_ARY_END);
	SetLine(0.11, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 3줄.
	SetTempAry(nSize, dStempLen, 0.130, 0.049, 0.049, 0.045, 0.045, 0.057, 0.057, 0.045, 0.045, TEMP_ARY_END);
	SetLine(0.16, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 4줄.
	SetTempAry(nSize, dStempLen, 0.06, TEMP_ARY_END);
	SetLine(0.175, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 5줄.
	SetTempAry(nSize, dStempLen, 0.070, 0.045, 0.045, 0.050, 0.063, 0.045, 0.057, TEMP_ARY_END);
	SetLine(0.217, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 6줄.
	SetTempAry(nSize, dStempLen, 0.060, 0.05, 0.1, TEMP_ARY_END);
	for(i = 0 ; i < 2; i++)
	{
		SetLine(0.260 + ((double)i * 0.045), dStempLen, nSize, &tempInfo, bSymm);
	}

	//나머지 세로줄.
	SetTempAry(nSize, dStempLen, 0.060, 0.05, 0.1, 0.1365, 0.057, TEMP_ARY_END);
	for(i = 0 ; i < nAdditionalNum; i++)
	{
		SetLine(0.350 + ((double)i * 0.050), dStempLen, nSize, &tempInfo, bSymm);
	}

	return TRUE;	
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_TY_BS( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	//제일 아래 1줄.
	double dStempLen[10];
	int    nSize;

	SetTempAry(nSize, dStempLen, 0.081, 0.049, 0.049, 0.049, 0.045, 0.045, 0.057, TEMP_ARY_END);
	SetLine(0.06, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.06, dStempLen, nSize - 1, &tempInfo, FALSE);

	SetTempAry(nSize, dStempLen, 0.13, 0.049, 0.049, 0.045, 0.045, 0.057, TEMP_ARY_END);
	SetLine(0.11, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.11, dStempLen, nSize - 1, &tempInfo, FALSE);

	SetTempAry(nSize, dStempLen, 0.228, 0.045, 0.045, 0.057, TEMP_ARY_END);
	SetLine(0.16, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.16, dStempLen, nSize - 1, &tempInfo, FALSE);

	STRAIGHT_DT(tempInfo, FALSE, 0.217, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, 0.375, TRUE, TRUE);

	double dTemp;
	for(i = 0 ; i < nAdditionalNum; i++)
	{
		dTemp = 0.35 + ((double)i * 0.05);
		STRAIGHT_DT(tempInfo, FALSE, dTemp, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.3465, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.3465, FALSE, TRUE);
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_YE(const T_AUTOTNDN_ARG& tArg)
{ 
	int nAdditionalNum = tArg.nAdditionalNum;
	BOOL bRev = tArg.bRev;

	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	BOOL bSymm;
	if(bRev)  bSymm = FALSE;
	else      bSymm = TRUE;

	//제일 아래 1줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);
	for(i = 0 ; i < 5; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), bSymm, TRUE);
	}

	STRAIGHT_DT(tempInfo, TRUE, 0.375, TRUE, TRUE);
	for(i = 0 ; i < 5; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.45 + (i * 0.05), bSymm, TRUE);
	}

	//제일 아래 2줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, 0.06, bSymm, TRUE);
	for(i = 0 ; i < 12; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), bSymm, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.06, !bSymm, TRUE);

	//제일 아래 3줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, 0.06, bSymm, TRUE);
	for(i = 0 ; i < 12; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), bSymm, TRUE);
	}

	//제일 아래 4줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, 0.06, bSymm, TRUE);
	for(i = 0 ; i < 11; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), bSymm, TRUE);
	}

	//제일 아래 5줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.26, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, 0.06, bSymm, TRUE);
	for(i = 0 ; i < 9; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), bSymm, TRUE);
	}

	if(nAdditionalNum < 0 || nAdditionalNum > 6)
	{
		ASSERT(0);
		nAdditionalNum = 0;
	}

	//세로줄
	for(i = 0 ; i < nAdditionalNum; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.8 + (i * 0.1), TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.08 + (i * 0.002), bSymm, TRUE);
		if(i != 0)
			STRAIGHT_DT(tempInfo, TRUE, 0.12 + (i * 0.002), bSymm, TRUE);
		if(i != 0)
			STRAIGHT_DT(tempInfo, TRUE, 0.42 + (i * 0.005), bSymm, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.46 + (i * 0.005), bSymm, TRUE);
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_Y( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	//제일 아래 1줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);
	for(i = 0 ; i < 5; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), FALSE, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.375, FALSE, TRUE);

	//제일 아래 2줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, 0.06, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.06, FALSE, TRUE);
	for(i = 0 ; i < 6; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), FALSE, TRUE);
	}

	//제일 아래 3줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);
	for(i = 0 ; i < 12; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.1 + (i * 0.05), TRUE, TRUE);
	}

	//제일 아래 4줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);
	for(i = 0 ; i < 10; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.15 + (i * 0.05), TRUE, TRUE);
	}

	//제일 아래 5줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.26, TRUE, FALSE);
	for(i = 0 ; i < 6; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.25 + (i * 0.05), TRUE, TRUE);
	}

	if(nAdditionalNum < 0 || nAdditionalNum > 6)
	{
		ASSERT(0);
		nAdditionalNum = 0;
	}

	//세로줄
	for(i = 0 ; i < nAdditionalNum; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.8 + (i * 0.1), TRUE, FALSE);
		if(i == 0)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.32, TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.32, FALSE, TRUE);
		}
		else
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.35 - (i * 0.01), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.29 - (i * 0.01), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.35 - (i * 0.01), FALSE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.29 - (i * 0.01), FALSE, TRUE);
		}
	}

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_M( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	//제일 아래 1줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);
	for(i = 0 ; i < 7; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.11 + (i * 0.05), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.11 + (i * 0.05), FALSE, TRUE);
	}
	STRAIGHT_DT(tempInfo, TRUE, 0.485, FALSE, TRUE);

	//제일 아래 2줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);
	for(i = 0 ; i < 7; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.16 + (i * 0.05), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.16 + (i * 0.05), FALSE, TRUE);
	}

	//제일 아래 3줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);
	for(i = 0 ; i < 2; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.425 + (i * 0.04), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.425 + (i * 0.04), FALSE, TRUE);
	}

	int toNum = 0;
	if(nAdditionalNum < 0 || nAdditionalNum > 10)
	{
		ASSERT(0);
		nAdditionalNum = 0;
	}

	switch (nAdditionalNum)
	{
	case 0:    toNum = 6;  break; //M1
	case 1:    toNum = 7;  break; //M680
	case 2:    toNum = 8;  break; //M2
	case 3:    toNum = 9;  break; //M3
	case 4:    toNum = 11; break; //M4
	case 5:    toNum = 12; break; //M5
	case 6:    toNum = 14; break; //M6
	case 7:    toNum = 16; break; //M7
	case 8:    toNum = 17; break; //M8
	case 9:    toNum = 19; break; //M9
	case 10:   toNum = 20; break; //M10
	default:   toNum = 6;  break;
	}

	//세로줄
	for(i = 0 ; i < toNum; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.33 + (i * 0.05), TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.505, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.505, FALSE, TRUE);
	}

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_U( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	//   if(nAdditionalNum == 0) //UK-U 단면의 U1은 정보가 없다.
	//     return FALSE;

	double nStartPosOrg;
	switch (nAdditionalNum)
	{
	case 0:
		nStartPosOrg = 0.105; break;
	case 1:
		nStartPosOrg = 0.1193; break;
	case 2:
		nStartPosOrg = 0.13357; break;
	case 3:
		nStartPosOrg = 0.14786; break;
	case 4:
		nStartPosOrg = 0.16214; break;
	case 5:
		nStartPosOrg = 0.17643; break;
	case 6:
		nStartPosOrg = 0.19071; break;
	case 7:
		nStartPosOrg = 0.20500; break;
	case 8:
		nStartPosOrg = 0.21929; break;
	default:
		return FALSE;
	}  

	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dStartPos;
	double dStartPos2;
	//제일 아래 1줄.
	dStartPos = nStartPosOrg;
	STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);
	for(i = 0 ; i < 9; i++)
	{
		switch (i)
		{
		case 0: dStartPos += 0.065; break;
		case 1: dStartPos += 0.045; break;
		default:dStartPos += 0.050; break;
		}
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, FALSE, TRUE);
	}

	//제일 아래 2줄.
	dStartPos = nStartPosOrg;
	STRAIGHT_DT(tempInfo, FALSE, 0.11, TRUE, FALSE);
	for(i = 0 ; i < 9; i++)
	{
		switch (i)
		{
		case 0: dStartPos += 0.053; break;
		case 1: dStartPos += 0.048; break;
		case 2: dStartPos += 0.045; break;
		case 3: dStartPos += 0.050; break;
		case 4:
		case 5:
		case 6: dStartPos += 0.055; break;
		case 7:
		case 8: dStartPos += 0.050; break;
		}
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, FALSE, TRUE);
	}

	//제일 아래 3줄.
	dStartPos = nStartPosOrg;
	STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);
	for(i = 0 ; i < 4; i++)
	{
		switch (i)
		{
		case 0: dStartPos += 0.045; break;
		case 1: dStartPos += 0.050; break;
		case 2: dStartPos += 0.060; break;
		case 3: dStartPos += 0.065; break;
		}
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, FALSE, TRUE);
	}

	//제일 아래 4줄.
	dStartPos = nStartPosOrg;
	STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);
	for(i = 0 ; i < 3; i++)
	{
		switch (i)
		{
		case 0: dStartPos += 0.038; break;
		case 1: dStartPos += 0.083; break;
		case 2: dStartPos += 0.060; break;
		}
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, FALSE, TRUE);
	}


	// 대각선 부분 시작...  그중에서 안쪽.
	int nSize = nAdditionalNum + 3;
	dStartPos = nStartPosOrg; //horz
	dStartPos2 = 0.0;         //vert
	for(i = 0 ; i < nSize; i++)
	{
		if(i == nSize - 3)
		{
			dStartPos += 0.001;  
			dStartPos2 += 0.23;
		}
		else if(i == nSize - 2)
		{
			dStartPos += 0.035;  
			dStartPos2 += 0.12;      
		}
		else if(i == nSize - 1)
		{
			dStartPos += 0.009;  
			dStartPos2 += 0.15;      
		}
		else if(i == 0)
		{
			dStartPos += 0.07;
			dStartPos2 += 0.34;
		}
		else if(i == 1)
		{
			dStartPos -= 0.012;  
			dStartPos2 += 0.09;
		}
		else
		{
			dStartPos -= 0.014;  
			dStartPos2 += 0.1;
		}

		//추가로.. 크기 3, 2 에서는 동작이 이상할 수도 있다.
		if(i == 0)
		{
			if(nSize == 3)
			{
				dStartPos += 0.0860;
				dStartPos2 += 0.23;
			}
		}
		else if(i == 1 && nSize == 4)
		{
			dStartPos2 -= 0.01;
		}

		STRAIGHT_DT(tempInfo, FALSE, dStartPos2, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, FALSE, TRUE);
	}

	// 대각선 부분 시작...  그중에서 바깥쪽.
	dStartPos = nStartPosOrg; //horz
	dStartPos2 = 0.0;         //vert
	nSize = nAdditionalNum + 4;
	for(i = 0 ; i < nSize; i++)
	{
		if(i == 0)
		{
			dStartPos += 0.016;
			dStartPos2 += 0.38;
		}
		else if(i == nSize - 1)
		{
			dStartPos2 += 0.15;
		}
		else
		{
			dStartPos -= 0.014;  
			dStartPos2 += 0.1;
		}
		STRAIGHT_DT(tempInfo, FALSE, dStartPos2, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, dStartPos, FALSE, TRUE);
	}

	if(nAdditionalNum == 0)//0일때는 예외적인 한줄 추가.
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.34, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.175, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.175, FALSE, TRUE);
	}

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_MY( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	//제일 아래 1줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.06, TRUE, FALSE);
	for(i = 0 ; i < 17; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.065 + (i * 0.055), TRUE, TRUE);
	}

	//제일 아래 2줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.105, TRUE, FALSE);
	STRAIGHT_DT(tempInfo, TRUE, 0.485 - 0.045 - 0.057, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.485 - 0.045,         TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.485,                 TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.485 + 0.045,         TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE, 0.485 + 0.045 + 0.057, TRUE, TRUE);

	for(i = 0 ; i < nAdditionalNum + 1; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.25 + (i * 0.05), TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.485 - 0.102, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.485,         TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.485 + 0.102, TRUE, TRUE);
	}

	//세로줄.
	STRAIGHT_DT(tempInfo, FALSE, 0.16, TRUE, FALSE);
	for(i = 0 ; i < 2; i++)
	{
		STRAIGHT_DT(tempInfo, TRUE, 0.425 + (i * 0.04), TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.425 + (i * 0.04), FALSE, TRUE);
	}

	return TRUE;   
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_MYE( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	BOOL bRev = tArg.bRev;

	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	BOOL bSymm;
	if(bRev)  bSymm = FALSE;
	else      bSymm = TRUE;

	//제일 아래 1줄.
	double dStempLen[17];
	int    nSize;
	SetTempAry(nSize, dStempLen, 0.075, 0.05, 0.05, 0.055, 0.055, 0.055, 0.055, 0.045, 0.045, 0.045, 0.045, 0.055, 0.055, 0.055, 0.055, 0.055, 0.055, TEMP_ARY_END);
	SetLine(0.06, dStempLen, nSize, &tempInfo, bSymm);

	//맨 아래 2째줄
	SetTempAry(nSize, dStempLen, 0.060, 0.065, 0.050, 0.055, 0.055, 0.055, 0.043, 0.057, 0.045, 0.045, 0.057, TEMP_ARY_END);
	SetLine(0.105, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 3줄.
	SetTempAry(nSize, dStempLen, 0.06, 0.065, 0.05, 0.055, 0.055, 0.098, TEMP_ARY_END);
	SetLine(0.15, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 4줄.
	SetTempAry(nSize, dStempLen, 0.06, 0.15, TEMP_ARY_END);
	SetLine(0.2, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 5줄.
	SetTempAry(nSize, dStempLen, 0.06, 0.15, 0.275, TEMP_ARY_END);
	SetLine(0.25, dStempLen, nSize, &tempInfo, bSymm);

	//추가 세로줄.
	SetTempAry(nSize, dStempLen, 0.06, 0.15, 0.173, 0.102, 0.102, TEMP_ARY_END);
	for(i = 0 ; i < nAdditionalNum; i++)
	{
		SetLine(0.3 + ((double)i * 0.05), dStempLen, nSize, &tempInfo, bSymm);
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_UMB( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	//제일 아래 1줄.
	double dStempLen[10];
	int    nSize;

	SetTempAry(nSize, dStempLen, 0.110, 0.050, 0.050, 0.050, 0.050, 0.050, 0.050, 0.075, TEMP_ARY_END);
	SetLine(0.06, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.06, dStempLen, nSize - 1, &tempInfo, FALSE);

	SetTempAry(nSize, dStempLen, 0.080, 0.080, 0.050, 0.050, 0.050, 0.050, 0.050, 0.050, TEMP_ARY_END);
	SetLine(0.11, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.11, dStempLen, nSize, &tempInfo, FALSE);

	SetTempAry(nSize, dStempLen, 0.065, 0.055, TEMP_ARY_END);
	SetLine(0.2, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.2, dStempLen, nSize, &tempInfo, FALSE);

	for(i = 0 ; i < nAdditionalNum; i++)
	{
		SetLine(0.33 + ((double)i * 0.05), dStempLen, nSize, &tempInfo, TRUE);
		SetLine(0.33 + ((double)i * 0.05), dStempLen, nSize, &tempInfo, FALSE);
	}

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_SYE( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	BOOL bRev = tArg.bRev;

	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	BOOL bSymm;
	if(bRev)  bSymm = FALSE;
	else      bSymm = TRUE;

	double dStempLen[15];
	int    nSize;

	//제일 아래 1줄.
	SetTempAry(nSize, dStempLen, 0.075, 0.045, 0.045, 0.045, 0.050, 0.050, 0.065, TEMP_ARY_END);
	SetLine(0.06, dStempLen, nSize, &tempInfo, bSymm);
	SetLine(0.06, dStempLen, nSize-1, &tempInfo, !bSymm);

	//제일 아래 2 ~ 4줄.
	SetTempAry(nSize, dStempLen, 0.060, 0.050, 0.050, 0.050, 0.050, 0.050, 0.040, TEMP_ARY_END);
	for(i = 0 ; i < 3 ; i++)
	{
		SetLine(0.11 + ((double)i * 0.05), dStempLen, nSize, &tempInfo, bSymm);
		SetLine(0.11 + ((double)i * 0.05), dStempLen, nSize, &tempInfo, !bSymm);
	}

	//제일 아래 5줄.
	SetTempAry(nSize, dStempLen, 0.060, 0.050, 0.050, 0.050, 0.050, 0.050, 0.040, 0.050, TEMP_ARY_END);
	SetLine(0.26, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 6줄.
	SetTempAry(nSize, dStempLen, 0.060, 0.050, 0.050, 0.050, 0.335, TEMP_ARY_END);
	SetLine(0.31, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 7줄.
	SetTempAry(nSize, dStempLen, 0.075, 0.135, 0.190, TEMP_ARY_END);
	SetLine(0.4, dStempLen, nSize, &tempInfo, bSymm);

	//제일 아래 8, 9, 10, 11, 12 줄. 기울어진 부분.
	SetTempAry(nSize, dStempLen, 0.060, 0.100, 0.050, 0.090, 0.140, TEMP_ARY_END);
	for(i = 4; i >= 0; i--)
	{
		dStempLen[3] += 0.005;
		dStempLen[4] -= 0.005;
		dStempLen[4] -= 0.005;
		if(i == 0)      SetLine(0.53, dStempLen, nSize, &tempInfo, bSymm);
		else if(i == 1) SetLine(0.63, dStempLen, nSize, &tempInfo, bSymm);
		else if(i == 2) SetLine(0.73, dStempLen, nSize, &tempInfo, bSymm);
		else if(i == 3) SetLine(0.83, dStempLen, nSize, &tempInfo, bSymm);
		else if(i == 4) SetLine(0.93, dStempLen, nSize, &tempInfo, bSymm);
	}

	SetTempAry(nSize, dStempLen, 0.060, 0.100, 0.050, 0.090, 0.140, TEMP_ARY_END);
	for(i = 0 ; i < nAdditionalNum; i++)
	{
		SetLine(1.030 + ((double)i * 0.1), dStempLen, nSize, &tempInfo, bSymm);
	}
	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_SD( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int nType = tArg.nType;

	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dStempLen[20];
	int    nSize;

	if(nType == 1)      SetTempAry(nSize, dStempLen, 0.1125, 0.045, 0.045, 0.045, TEMP_ARY_END);
	else if(nType == 2) SetTempAry(nSize, dStempLen, 0.110, 0.05, 0.05, 0.05, 0.065, 0.05, TEMP_ARY_END);
	else if(nType == 3) SetTempAry(nSize, dStempLen, 0.115, 0.05, 0.05, 0.05, 0.05, 0.07, 0.05, 0.05, TEMP_ARY_END);
	else if(nType == 4) SetTempAry(nSize, dStempLen, 0.115, 0.045, 0.05, 0.05, 0.05, 0.07, 0.05, 0.05, 0.05, 0.05, 0.07, 0.05, 0.05, TEMP_ARY_END);
	else return FALSE;

	//1번째줄.
	SetLine(0.06, dStempLen, nSize,   &tempInfo, FALSE);
	SetLine(0.06, dStempLen, nSize-1, &tempInfo, TRUE);

	if(nType == 1)      SetTempAry(nSize, dStempLen, 0.0975, 0.060, 0.045, 0.045, TEMP_ARY_END);
	else if(nType == 2) SetTempAry(nSize, dStempLen, 0.095, 0.065, 0.05, 0.05, 0.05, 0.065, TEMP_ARY_END);
	else if(nType == 3) SetTempAry(nSize, dStempLen, 0.095, 0.07, 0.05, 0.05, 0.05, 0.05, 0.07, 0.05, TEMP_ARY_END);
	else if(nType == 4) SetTempAry(nSize, dStempLen, 0.095, 0.065, 0.05, 0.05, 0.05, 0.05, 0.07, 0.05, 0.05, 0.07, 0.05, 0.05, 0.05, TEMP_ARY_END);
	else return FALSE;

	//2번째줄.
	SetLine(0.110, dStempLen, nSize,   &tempInfo, FALSE);
	SetLine(0.110, dStempLen, nSize-1, &tempInfo, TRUE);

	//3번째줄.
	SetLine(0.16, dStempLen,  nSize,   &tempInfo, FALSE);
	SetLine(0.16, dStempLen,  nSize-1, &tempInfo, TRUE);

	//4번째 줄 부터~
	nSize = 0;
	if(nType == 1)      SetTempAry(nSize, dStempLen, 0.0975, TEMP_ARY_END);
	else if(nType == 2) SetTempAry(nSize, dStempLen, 0.095, 0.215, TEMP_ARY_END);
	else if(nType == 3) SetTempAry(nSize, dStempLen, 0.095, 0.270, TEMP_ARY_END);
	else if(nType == 4) SetTempAry(nSize, dStempLen, 0.095, 0.265, 0.240, TEMP_ARY_END);
	else return FALSE;

	double dTemp;
	for(i = 0 ; i < nAdditionalNum; i++)
	{
		dTemp = 0.23 + ((double)i * 0.05);
		SetLine(dTemp, dStempLen,  nSize, &tempInfo, FALSE);
		SetLine(dTemp, dStempLen,  nSize, &tempInfo, TRUE);
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_W( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dStempLen[15];
	double dStartP1 = 0.102 + (0.014 * (double)nAdditionalNum);
	int    nSize;

	//제일 아래 1줄.
	SetTempAry(nSize, dStempLen, 0.070 + dStartP1, 0.0425, 0.0425, 0.0525, 0.0525, 0.0525, 0.050, 0.050, 0.050, 0.050, 0.050, 0.050, 0.050, 0.050, TEMP_ARY_END);
	SetLine(0.06, dStempLen, nSize, &tempInfo, FALSE);
	SetLine(0.06, dStempLen, nSize, &tempInfo, TRUE);

	//제일 아래 2줄.
	SetTempAry(nSize, dStempLen, 0.050 + dStartP1, 0.05, 0.055, 0.0525, 0.0525, 0.0525, 0.050, 0.050, 0.050, 0.050, 0.050, 0.050, 0.050, 0.050, TEMP_ARY_END);
	SetLine(0.11, dStempLen, nSize, &tempInfo, FALSE);
	SetLine(0.11, dStempLen, nSize, &tempInfo, TRUE);

	//제일 아래 3줄.
	SetTempAry(nSize, dStempLen, 0.043 + dStartP1, 0.050, 0.045, 0.045, 0.045, TEMP_ARY_END);
	SetLine(0.16, dStempLen, nSize, &tempInfo, FALSE);
	SetLine(0.16, dStempLen, nSize, &tempInfo, TRUE);

	//제일 아래 4줄.
	SetTempAry(nSize, dStempLen, 0.036 + dStartP1, 0.057, 0.045, 0.045, TEMP_ARY_END);
	SetLine(0.21, dStempLen, nSize, &tempInfo, FALSE);
	SetLine(0.21, dStempLen, nSize, &tempInfo, TRUE);

	//중간부분... 늘어나는곳 web...
	SetTempAry(nSize, dStempLen, 0.095 + dStartP1, TEMP_ARY_END);
	double dTemp;
	double dInterval = 0.0139;
	for(i = 0 ; i < nAdditionalNum + 1; i++)
	{
		dTemp = 0.33 + ((double)i * 0.1);
		dStempLen[0] -= dInterval;

		//규칙성 예외 처리 조건들.
		if(nAdditionalNum == 6  && i == nAdditionalNum) continue;
		if(nAdditionalNum == 7  && i == nAdditionalNum) continue;
		if(nAdditionalNum == 8  && i == nAdditionalNum) continue;
		if(i == 0)                                                               dTemp += 0.01;
		if((9 <= nAdditionalNum && nAdditionalNum <= 14) && i == nAdditionalNum) dTemp += 0.035;
		if((9 <= nAdditionalNum && nAdditionalNum <= 15) && i == nAdditionalNum) dStempLen[0] += 0.02;

		SetLine(dTemp, dStempLen, nSize, &tempInfo, FALSE);
		SetLine(dTemp, dStempLen, nSize, &tempInfo, TRUE);
	}

	SetTempAry(nSize, dStempLen, 0.025 + dStartP1, TEMP_ARY_END);
	for(i = 0 ; i < nAdditionalNum + 1; i++)
	{
		dStempLen[0] -= dInterval;
		SetLine(0.38 + ((double)i * 0.1), dStempLen, nSize, &tempInfo, FALSE);
		SetLine(0.38 + ((double)i * 0.1), dStempLen, nSize, &tempInfo, TRUE);
	}

	//여기는 web부분과 젤위 나란히 부분이 아닌 예외적인 부분.
	if(nAdditionalNum == 0) //W1
	{
		SETPOINT_SYMM(0.480, 0.1,    dStempLen, tempInfo);
		SETPOINT_SYMM(0.515, 0.175,  dStempLen, tempInfo);
		SETPOINT_SYMM(0.530, 0.0915, dStempLen, tempInfo);
	}
	else if(nAdditionalNum == 1 || nAdditionalNum == 2) //W3
	{
		SETPOINT_SYMM(0.615 + ((double)(nAdditionalNum-1) * 0.1), 0.178,  dStempLen, tempInfo);
		SETPOINT_SYMM(0.630 + ((double)(nAdditionalNum-1) * 0.1), 0.0915, dStempLen, tempInfo);
	}
	else if(nAdditionalNum == 3 || nAdditionalNum == 4 || nAdditionalNum == 5) //W7 W8 W9
	{
		SETPOINT_SYMM(0.765 + ((double)(nAdditionalNum-3) * 0.1), 0.188,  dStempLen, tempInfo);
		SETPOINT_SYMM(0.830 + ((double)(nAdditionalNum-3) * 0.1), 0.0915, dStempLen, tempInfo);
		SETPOINT_SYMM(0.880 + ((double)(nAdditionalNum-3) * 0.1), 0.2382, dStempLen, tempInfo);
	}
	else if(nAdditionalNum == 6 || nAdditionalNum == 7 || nAdditionalNum == 8) //W10 W11 W12
	{
		SETPOINT_SYMM(1.015 + ((double)(nAdditionalNum-6) * 0.1), 0.190, dStempLen, tempInfo);
	}
	else if(nAdditionalNum == 12) //W16
	{
		SETPOINT_SYMM(1.730, 0.0915,  dStempLen, tempInfo);
		SETPOINT_SYMM(1.780, 0.2952, dStempLen, tempInfo);
	}
	else if(nAdditionalNum == 13) //W17
	{
		SETPOINT_SYMM(1.830, 0.0915,  dStempLen, tempInfo);
		SETPOINT_SYMM(1.880, 0.3092, dStempLen, tempInfo);
	}
	else if(nAdditionalNum == 14 || nAdditionalNum == 15) //W18 W19
	{
		if(nAdditionalNum == 14)      SetTempAry(nSize, dStempLen, 0.0915, 0.2017, TEMP_ARY_END);
		else if(nAdditionalNum == 15) SetTempAry(nSize, dStempLen, 0.0915, 0.2157, TEMP_ARY_END);

		SetLine(1.930 + ((double)(nAdditionalNum-14) * 0.1), dStempLen, nSize, &tempInfo, FALSE);
		SetLine(1.930 + ((double)(nAdditionalNum-14) * 0.1), dStempLen, nSize, &tempInfo, TRUE);
	}

	//젤위 나란히 되어있는 부분.
	int nStep;
	if(0 <= nAdditionalNum && nAdditionalNum <= 5)        nStep = 2;
	else if(6 <= nAdditionalNum && nAdditionalNum <= 11)  nStep = 3;
	else if(12 <= nAdditionalNum && nAdditionalNum <= 15) nStep = 2;
	else return FALSE;
	nSize = 0;
	dStempLen[nSize++] = 0.0915;
	if(nAdditionalNum == 0 || nAdditionalNum == 1 || nAdditionalNum == 2)      dStempLen[nSize++] = 0.1402;
	else if(nAdditionalNum == 3 || nAdditionalNum == 4 || nAdditionalNum == 5) dStempLen[nSize++] = 0.1467;
	else if(nAdditionalNum == 6 || nAdditionalNum == 7 || nAdditionalNum == 8) dStempLen[nSize++] = 0.1537;
	else if(nAdditionalNum == 9)  dStempLen[nSize++] = 0.1607;
	else if(nAdditionalNum == 10) dStempLen[nSize++] = 0.1745;
	else if(nAdditionalNum == 11) dStempLen[nSize++] = 0.1897;
	else if(nAdditionalNum == 12) dStempLen[nSize++] = 0.2037;
	else if(nAdditionalNum == 13) dStempLen[nSize++] = 0.2177;
	else if(nAdditionalNum == 14) dStempLen[nSize++] = 0.2317;
	else if(nAdditionalNum == 15) dStempLen[nSize++] = 0.2457;
	for(i = 0 ; i < nStep; i++)
	{
		dTemp = ((double)nAdditionalNum * 0.1) + 0.730 - ((double)i * 0.1);
		SetLine(dTemp, dStempLen, nSize, &tempInfo, FALSE);
		SetLine(dTemp, dStempLen, nSize, &tempInfo, TRUE);
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_T( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	//제일 아래 1줄.
	double dStempLen[10];
	int    nSize;

	SetTempAry(nSize, dStempLen, 0.0725, 0.045, 0.045, 0.040, 0.045, TEMP_ARY_END);
	SetLine(0.05, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.05, dStempLen, nSize - 1, &tempInfo, FALSE);  
	SetLine(0.09, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.09, dStempLen, nSize - 1, &tempInfo, FALSE);

	double dHeightTemp[] = {0.24, 0.27, 0.30, 0.35, 0.38, 0.41, 0.45, 0.49, 0.53, 0.57, 0.61, 0.65, 0.69, 0.73, 0.77};

	for(i = 0 ; i < nAdditionalNum; i++)
	{
		STRAIGHT_DT(tempInfo, FALSE, dHeightTemp[i], TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.2475, TRUE, TRUE);
	}

	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_UK_U_SU( const T_AUTOTNDN_ARG& tArg )
{
	int nAdditionalNum = tArg.nAdditionalNum;
	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	double dMid = (m_dSectSizeW_Org / 2.0); // 끝에서 가운데 까지 거리.
	double dTemp = dMid - 0.485; // 끝에서 하단부 끝까지 거리

	//제일 아래 1줄.
	double dStempLen[10];
	int    nSize;

	SetTempAry(nSize, dStempLen, dTemp + 0.065, 0.045, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, TEMP_ARY_END);
	SetLine(0.06, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.06, dStempLen, nSize, &tempInfo, FALSE);

	SetTempAry(nSize, dStempLen, dTemp + 0.053, 0.048, 0.045, 0.050, 0.055, 0.055, 0.055, 0.050, 0.050, TEMP_ARY_END);
	SetLine(0.11, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.11, dStempLen, nSize, &tempInfo, FALSE);

	//SetTempAry(nSize, dStempLen, dTemp + 0.045, 0.050, 0.060, 0.065, TEMP_ARY_END); //원래 U단면...
	SetTempAry(nSize, dStempLen, dTemp + 0.045, 0.050, 0.050, 0.050, TEMP_ARY_END); //보기에 모양이 약간 달라서 이렇게 처리.
	SetLine(0.16, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.16, dStempLen, nSize, &tempInfo, FALSE);

	SetTempAry(nSize, dStempLen, dTemp + 0.038, 0.083, 0.060, TEMP_ARY_END);
	SetLine(0.21, dStempLen, nSize, &tempInfo, TRUE);
	SetLine(0.21, dStempLen, nSize, &tempInfo, FALSE);

	if(nAdditionalNum >= 3) //바깥쪽.
	{
		STRAIGHT_DT(tempInfo, FALSE, 0.34, TRUE,  FALSE);
		STRAIGHT_DT(tempInfo, TRUE,  dMid - 0.415, TRUE,  TRUE);
		STRAIGHT_DT(tempInfo, TRUE,  dMid - 0.415, FALSE, TRUE);

		double dTemp_H;
		double dTemp_W;

		//웹 바깥쪽
		dTemp_H = 0.48;
		dTemp_W = dMid - 0.483;
		for(i = 0 ; i < nAdditionalNum + 1; i++)
		{
			STRAIGHT_DT(tempInfo, FALSE, dTemp_H, TRUE,  FALSE);
			STRAIGHT_DT(tempInfo, TRUE,  dTemp_W, TRUE,  TRUE);
			STRAIGHT_DT(tempInfo, TRUE,  dTemp_W, FALSE, TRUE);

			if(i == nAdditionalNum - 1)  dTemp_H += 0.15;
			else {                       dTemp_H += 0.1;  dTemp_W -= 0.014; }
		}

		//웹 안쪽.
		dTemp_H = 0.43;
		dTemp_W = dMid - 0.427;
		for(i = 0 ; i < nAdditionalNum; i++)
		{
			if(i == nAdditionalNum - 1)      { dTemp_H += 0.15; dTemp_W += 0.009; }
			else if(i == nAdditionalNum - 2) { dTemp_H += 0.12; dTemp_W += 0.038; }
			else if(i == nAdditionalNum - 3) { dTemp_H += 0.13; dTemp_W += 0.013; }
			else                             { dTemp_H += 0.1;  dTemp_W -= 0.014; }

			STRAIGHT_DT(tempInfo, FALSE, dTemp_H, TRUE,  FALSE);
			STRAIGHT_DT(tempInfo, TRUE,  dTemp_W, TRUE,  TRUE);
			STRAIGHT_DT(tempInfo, TRUE,  dTemp_W, FALSE, TRUE);
		}
	}

	return TRUE;
}