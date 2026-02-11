#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_TSGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_TSGR::CDB_TSGR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_tsgr.InitHashTable(HASHSIZETSGR);
}

CDB_TSGR::~CDB_TSGR()
{
}

void CDB_TSGR::Add(T_TSGR_K Key,T_TSGR_D& rData,CDB_ELEM* pElem)
{
	T_TSGR_D Data;
	BOOL bExist=m_tsgr.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapNew;
	T_ELEM_K KeyElemTemp;
	if(bExist)
	{
		mapNew.InitHashTable(HASHSIZEELEM);
		mapOld.InitHashTable(HASHSIZEELEM);
		for(int i = 0; i<Data.aElemList.GetSize(); i++)mapOld.SetAt(Data.aElemList[i], Data.aElemList[i]);
		for(int i = 0; i<rData.aElemList.GetSize(); i++)mapNew.SetAt(rData.aElemList[i], rData.aElemList[i]);
		for(int i = 0; i<Data.aElemList.GetSize(); i++)
		{
			if(!mapNew.Lookup(Data.aElemList[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(Data.aElemList[i], LT_TSGR_CMD, Key));
		}
	}
	m_tsgr.SetAt(Key,rData);
	for(int i=0; i<rData.aElemList.GetSize(); i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.aElemList[i], KeyElemTemp))
				pElem->AddListItem(rData.aElemList[i], LT_TSGR_CMD, Key);
		}
		else
			pElem->AddListItem(rData.aElemList[i], LT_TSGR_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_TSGR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_tsgr.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TSGR::Del(T_TSGR_K Key,CDB_ELEM* pElem)
{
	T_TSGR_D Data;
	BOOL bExist=m_tsgr.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_tsgr.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			for(int i=0; i<Data.aElemList.GetSize(); i++)
			{
				VERIFY(pElem->DelListItem(Data.aElemList[i], LT_TSGR_CMD, Key));
			}
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_TSGR_K key;
				T_TSGR_D data;
				if(m_tsgr.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_tsgr.GetStartPosition();
					while(pos != NULL)
					{
						m_tsgr.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}
		}
		return ret;
	}
	return FALSE;
}

//-------------------------------------------------------------------------
BOOL CDB_TSGR::Get(T_TSGR_K Key,T_TSGR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_tsgr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_tsgr.Lookup(Key,rData);
}

int CDB_TSGR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_tsgr->GetCount();
	}
	return m_tsgr.GetCount();
}

POSITION CDB_TSGR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_tsgr->GetStartPosition();
	}
	return m_tsgr.GetStartPosition();
}

void CDB_TSGR::GetNext(POSITION& rNextPosition,T_TSGR_K& rKey,T_TSGR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_tsgr->GetNextAssoc(rNextPosition,rKey,uTemp);
		m_tsgr.Lookup(rKey,rData);
		return;
	}
	m_tsgr.GetNextAssoc(rNextPosition,rKey,rData);
}
