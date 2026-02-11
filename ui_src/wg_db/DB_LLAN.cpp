#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_LLAN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LLAN::CDB_LLAN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_llan.InitHashTable(HASHSIZELLAN);
}

CDB_LLAN::~CDB_LLAN()
{
}

void CDB_LLAN::Add(T_LLAN_K Key,T_LLAN_D& rData,CDB_ELEM* pElem)
{
	T_LLAN_D Data;
	BOOL bExist=m_llan.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapNew;
	mapNew.InitHashTable(101);
	T_ELEM_K KeyElemTemp;
	if(bExist)
	{
		//int nCount=Data.arLaneData.GetSize();
		//for(int i=0; i<nCount; i++)pElem->DelListItem(Data.arLaneData[i].KeyElem, LT_LLAN_CMD, Key);
		mapOld.InitHashTable(101);
		int nCount=Data.arLaneData.GetSize();
		for(int i=0; i<nCount; i++)mapOld.SetAt(Data.arLaneData[i].KeyElem, Data.arLaneData[i].KeyElem);
		nCount=rData.arLaneData.GetSize();
		for(int i = 0; i<nCount; i++)mapNew.SetAt(rData.arLaneData[i].KeyElem, rData.arLaneData[i].KeyElem);
		for(int i = 0; i<Data.arLaneData.GetSize(); i++)
		{
			if(!mapNew.Lookup(Data.arLaneData[i].KeyElem, KeyElemTemp))
				pElem->DelListItem(Data.arLaneData[i].KeyElem, LT_LLAN_CMD, Key);
		}
	}
	int nCount=rData.arLaneData.GetSize();
	for(int i=0; i<nCount; i++)
	{
		//pElem->AddListItem(rData.arLaneData[i].KeyElem, LT_LLAN_CMD, Key);
		if(bExist)
		{
			if(!mapOld.Lookup(rData.arLaneData[i].KeyElem, KeyElemTemp))
			{
				pElem->AddListItem(rData.arLaneData[i].KeyElem, LT_LLAN_CMD, Key);
				mapOld.SetAt(rData.arLaneData[i].KeyElem, rData.arLaneData[i].KeyElem);
			}
		}
		else
		{
			// 중복해서 등록 안 되도록 수정
			if(!mapNew.Lookup(rData.arLaneData[i].KeyElem, KeyElemTemp)) 
			{
				pElem->AddListItem(rData.arLaneData[i].KeyElem, LT_LLAN_CMD, Key);
				mapNew.SetAt(rData.arLaneData[i].KeyElem, rData.arLaneData[i].KeyElem);
			}
		}
	}
	m_llan.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_LLAN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_LLAN::Del(T_LLAN_K Key,CDB_ELEM* pElem)
{
	T_LLAN_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_llan.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.arLaneData.GetSize();
		for(int i=0; i<nCount; i++)pElem->DelListItem(data.arLaneData[i].KeyElem, LT_LLAN_CMD, Key);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LLAN_K key;
			T_LLAN_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LLAN::Get(T_LLAN_K Key,T_LLAN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_llan->Lookup(Key, uTemp)) return FALSE;
	}
	return m_llan.Lookup(Key,rData);
}

int CDB_LLAN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_llan->GetCount();
	}
	return m_llan.GetCount();
}

POSITION CDB_LLAN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_llan->GetStartPosition();
	}
	return m_llan.GetStartPosition();
}

void CDB_LLAN::GetNext(POSITION& rNextPosition,T_LLAN_K& rKey,T_LLAN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_llan->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_llan.Lookup(rKey, rData);
		return;
	}
	m_llan.GetNextAssoc(rNextPosition,rKey,rData);
}
