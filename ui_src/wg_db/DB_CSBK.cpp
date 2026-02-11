#include "stdafx.h"
//#include "DB_CSBK.h"
//
//#include "DBDoc.h"
//#include "StagCtrl.h"
//
//CDB_CSBK::CDB_CSBK()
//{
//	m_nStartNum = 1;
//	m_nLastNum = 0;
//	m_pDoc = CDBDoc::GetDocPoint();
//	m_csbk.InitHashTable(HASHSIZECSBK);
//}
//
//CDB_CSBK::~CDB_CSBK()
//{
//}
//
//void CDB_CSBK::Add(T_CSBK_K Key, T_CSBK_D& rData) // CDB_CSPA* pCspa
//{
//	T_CSBK_D Data;
//	BOOL bExist = m_csbk.Lookup(Key, Data);
//
//	if (bExist)
//	{
//		// pCspa 에 Key 를 지우는 동작 ?
//	}
//	else
//	{
//		T_CSBK_K key;
//		T_CSBK_D data;
//		POSITION pos = GetStart();
//		while (pos)
//		{
//			GetNext(pos, key, data);
//			m_csbk.SetAt(key, data);
//		}
//	}
//
//	m_csbk.SetAt(Key, rData);
//	if (Key == m_nStartNum)
//	{
//		T_CSBK_D data;
//		while (TRUE)
//		{
//			m_nStartNum++;
//			if (!Get(m_nStartNum, data))break;
//		}
//	}
//	if (Key > m_nLastNum)m_nLastNum = Key;
//}
//
//BOOL CDB_CSBK::Del(T_CSBK_K Key)
//{
//	T_CSBK_D Data;
//	BOOL bExist = m_csbk.Lookup(Key, Data);
//	if (bExist)
//	{
//		BOOL ret = m_csbk.RemoveKey(Key);
//		ASSERT(ret);
//		if (ret)
//		{
//			if (Key < m_nStartNum)m_nStartNum = Key;
//			if (Key == m_nLastNum)
//			{
//				T_CSBK_K key;
//				T_CSBK_D data;
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
//		}
//		return ret;
//	}
//
//	return FALSE;
//}
//
//
////-------------------------------------------------------------------------
//BOOL CDB_CSBK::Get(T_CSBK_K Key, T_CSBK_D& rData)
//{
//	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//	{
//		if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsMinMaxStag())
//		{
//			return FALSE;
//		}
//		else
//		{
//			//return m_pDoc->m_pStagCtrl->m_csbk.Lookup(Key, rData);
//			return m_csbk.Lookup(Key, rData);
//		}
//	}
//
//	return m_csbk.Lookup(Key, rData);
//}
//
//int CDB_CSBK::GetCount()
//{
//	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//	{
//		if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsMinMaxStag())
//		{
//			return 0;
//		}
//		else
//		{
//			//return m_pDoc->m_pStagCtrl->m_csbk.GetCount();
//			return m_csbk.GetCount();
//		}
//	}
//
//	return m_csbk.GetCount();
//}
//
//POSITION CDB_CSBK::GetStart()
//{
//	return m_csbk.GetStartPosition();
//}
//void CDB_CSBK::GetNext(POSITION& rNextPosition, T_CSBK_K& rKey, T_CSBK_D& rData)
//{
//	m_csbk.GetNextAssoc(rNextPosition, rKey, rData);
//}