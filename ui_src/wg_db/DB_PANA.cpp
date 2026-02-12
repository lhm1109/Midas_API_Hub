#include "stdafx.h"
#include "DB_PANA.h"

#include "DB_STLD.h"
#include "DB_SPLC.h"
#include "DB_THIS.h"
#include "DB_POLC.h"


CDB_PANA::CDB_PANA()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_pana.InitHashTable(HASHSIZEPANA);
	
	//AddList(0); // add default group
}

CDB_PANA::~CDB_PANA()
{
	//DelAllList();
}

//void CDB_PANA::Add(T_PANA_K Key, T_PANA_D& rData, CDB_STLD* pStld, CDB_SPLC* pSplc, CDB_THIS* pThis, CDB_POLC* pPolc)
//{
//	T_PANA_D data;
//	BOOL bExist = m_pana.Lookup(Key, data);
//
//	if (bExist)
//	{
//		//다르면 제거한다.
//		if(data.nAnalType == D_PANA_ANALCASE_PUSHOVER)
//		{
//			if (data.LCKey != 0 && data.LCKey != rData.LCKey)
//			{
//				VERIFY(pPolc->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//			}
//		}
//		else if (data.nAnalType == D_PANA_ANALCASE_SPECTRUM)
//		{
//// 			if (data.LCKey != 0 && data.LCKey != rData.LCKey)
//// 			{
//// 				VERIFY(pSplc->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//// 			}
//		}
//		else if (data.nAnalType == D_PANA_ANALCASE_LINEAR_HISTORY || data.nAnalType == D_PANA_ANALCASE_NONLINEAR_HISTORY)
//		{
//			if (data.LCKey != 0 && data.LCKey != rData.LCKey)
//			{
//				VERIFY(pThis->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//			}
//		}
//		else
//		{
//			if (data.LCKey != 0 && data.LCKey != rData.LCKey)
//				VERIFY(pStld->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//		}
//	}
//
//	if (rData.nAnalType == D_PANA_ANALCASE_PUSHOVER)
//	{
//		if (rData.LCKey != 0)
//		{
//			pPolc->AddListItem(rData.LCKey, LT_PANA_CMD, Key);
//		}
//	}
//	else if (rData.nAnalType == D_PANA_ANALCASE_SPECTRUM)
//	{
//// 			if (data.LCKey != 0 && data.LCKey != rData.LCKey)
//// 			{
//// 				VERIFY(pSplc->AddListItem(data.LCKey, LT_PANA_CMD, Key));
//// 			}
//	}
//	else if (rData.nAnalType == D_PANA_ANALCASE_LINEAR_HISTORY || rData.nAnalType == D_PANA_ANALCASE_NONLINEAR_HISTORY)
//	{
//		if (rData.LCKey != 0)
//		{
//			pThis->AddListItem(rData.LCKey, LT_PANA_CMD, Key);
//		}
//	}
//	else
//	{
//		if (rData.LCKey != 0)
//			pStld->AddListItem(rData.LCKey, LT_PANA_CMD, Key);
//	}
//	
//	m_pana.SetAt(Key, rData);
//
//	if (Key == m_nStartNum)
//	{
//		T_PANA_D data;
//		while (TRUE)
//		{
//			m_nStartNum++;
//			if (!Get(m_nStartNum, data))break;
//		}
//	}
//	if (Key > m_nLastNum)m_nLastNum = Key;
//}
//
//BOOL CDB_PANA::Del(T_PANA_K Key, CDB_STLD* pStld, CDB_SPLC* pSplc, CDB_THIS* pThis, CDB_POLC* pPolc)
//{
//	T_PANA_D data;
//	BOOL bExist = m_pana.Lookup(Key, data);
//
//	BOOL ret = m_pana.RemoveKey(Key);
//	ASSERT(ret);
//	if (ret)
//	{
//		if (bExist)
//		{
//			//if (data.LCKey != 0) VERIFY(pStld->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//			if (data.nAnalType == D_PANA_ANALCASE_PUSHOVER)
//			{
//				if (data.LCKey != 0 )
//				{
//					VERIFY(pPolc->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//				}
//			}
//			else if (data.nAnalType == D_PANA_ANALCASE_SPECTRUM)
//			{
//				// 			if (data.LCKey != 0 && data.LCKey != rData.LCKey)
//				// 			{
//				// 				VERIFY(pSplc->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//				// 			}
//			}
//			else if (data.nAnalType == D_PANA_ANALCASE_LINEAR_HISTORY || data.nAnalType == D_PANA_ANALCASE_NONLINEAR_HISTORY)
//			{
//				if (data.LCKey != 0)
//				{
//					VERIFY(pThis->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//				}
//			}
//			else
//			{
//				if (data.LCKey != 0)
//					VERIFY(pStld->DelListItem(data.LCKey, LT_PANA_CMD, Key));
//			}
//
//			if (Key < m_nStartNum)m_nStartNum = Key;
//			if (Key == m_nLastNum)
//			{
//				T_PANA_K key;
//				T_PANA_D data;
//				if (Get(m_nLastNum - 1, data))
//				{
//					m_nLastNum--;
//				}
//				else
//				{
//					m_nLastNum = 0;
//					POSITION pos = GetStart();
//					while (pos != NULL)
//					{
//						GetNext(pos, key, data);
//						if (key > m_nLastNum)m_nLastNum = key;
//					}
//				}
//			}
//			
//		}
//	}
//	return ret;
//}
