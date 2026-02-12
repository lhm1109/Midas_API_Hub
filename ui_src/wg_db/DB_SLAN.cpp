#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_SLAN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SLAN::CDB_SLAN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_slan.InitHashTable(HASHSIZESLAN);
}

CDB_SLAN::~CDB_SLAN()
{
}

void CDB_SLAN::Add(T_SLAN_K Key,T_SLAN_D& rData,CDB_NODE* pNode)
{
	T_SLAN_D Data;
	BOOL bExist=m_slan.Lookup(Key, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNew;
	T_NODE_K KeyNodeTemp;
	if(bExist)
	{
		mapNew.InitHashTable(HASHSIZENODE);
		mapOld.InitHashTable(HASHSIZENODE);
		for(int i = 0; i<Data.arLaneData.GetSize(); i++)mapOld.SetAt(Data.arLaneData[i].KeyNode, Data.arLaneData[i].KeyNode);
		for(int i = 0; i<rData.arLaneData.GetSize(); i++)mapNew.SetAt(rData.arLaneData[i].KeyNode, rData.arLaneData[i].KeyNode);
		int nCount=Data.arLaneData.GetSize();
		for(int i = 0; i<nCount; i++)
		{
			if(!mapNew.Lookup(Data.arLaneData[i].KeyNode, KeyNodeTemp))
				pNode->DelListItem(Data.arLaneData[i].KeyNode, LT_SLAN_CMD, Key);
		}
	}
	int nCount=rData.arLaneData.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.arLaneData[i].KeyNode, KeyNodeTemp))
				pNode->AddListItem(rData.arLaneData[i].KeyNode, LT_SLAN_CMD, Key);
		}
		else
			pNode->AddListItem(rData.arLaneData[i].KeyNode, LT_SLAN_CMD, Key);
	}
	m_slan.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SLAN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SLAN::Del(T_SLAN_K Key,CDB_NODE* pNode)
{
	T_SLAN_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_slan.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.arLaneData.GetSize();
		for(int i=0; i<nCount; i++)pNode->DelListItem(data.arLaneData[i].KeyNode, LT_SLAN_CMD, Key);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SLAN_K key;
			T_SLAN_D data;
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
BOOL CDB_SLAN::Get(T_SLAN_K Key,T_SLAN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_slan->Lookup(Key, uTemp)) return FALSE;
	}
	return m_slan.Lookup(Key,rData);
}

int CDB_SLAN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_slan->GetCount();
	}
	return m_slan.GetCount();
}

POSITION CDB_SLAN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_slan->GetStartPosition();
	}
	return m_slan.GetStartPosition();
}

void CDB_SLAN::GetNext(POSITION& rNextPosition,T_SLAN_K& rKey,T_SLAN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_slan->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_slan.Lookup(rKey, rData);
		return;
	}
	m_slan.GetNextAssoc(rNextPosition,rKey,rData);
}
