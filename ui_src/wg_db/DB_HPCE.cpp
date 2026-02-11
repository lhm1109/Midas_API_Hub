#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_HPCE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HPCE::CDB_HPCE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_hpce.InitHashTable(HASHSIZEHPCE);
}

CDB_HPCE::~CDB_HPCE()
{
}

void CDB_HPCE::Add(T_HPCE_K Key,T_HPCE_D& rData,CDB_NODE* pNode)
{
	T_HPCE_D Data;
	BOOL bExist=m_hpce.Lookup(Key, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNew;
	T_NODE_K KeyNodeTemp;
	if(bExist)
	{
		mapNew.InitHashTable(HASHSIZENODE);
		mapOld.InitHashTable(HASHSIZENODE);
		for(int i = 0; i<Data.aNode.GetSize(); i++)mapOld.SetAt(Data.aNode[i].KeyNode, Data.aNode[i].KeyNode);
		for(int i = 0; i<rData.aNode.GetSize(); i++)mapNew.SetAt(rData.aNode[i].KeyNode, rData.aNode[i].KeyNode);
		int nCount=Data.aNode.GetSize();
		for(int i = 0; i<nCount; i++)
		{
			if(!mapNew.Lookup(Data.aNode[i].KeyNode, KeyNodeTemp))
				pNode->DelListItem(Data.aNode[i].KeyNode, LT_HPCE_CMD, Key);
		}
	}
	int nCount=rData.aNode.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.aNode[i].KeyNode, KeyNodeTemp))
				pNode->AddListItem(rData.aNode[i].KeyNode, LT_HPCE_CMD, Key);
		}
		else
			pNode->AddListItem(rData.aNode[i].KeyNode, LT_HPCE_CMD, Key);
	}
	m_hpce.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_HPCE_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_HPCE::Del(T_HPCE_K Key,CDB_NODE* pNode)
{
	T_HPCE_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_hpce.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.aNode.GetSize();
		for(int i=0; i<nCount; i++)pNode->DelListItem(data.aNode[i].KeyNode, LT_HPCE_CMD, Key);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_HPCE_K key;
			T_HPCE_D data;
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
BOOL CDB_HPCE::Get(T_HPCE_K Key,T_HPCE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_hpce->Lookup(Key, uTemp)) return FALSE;
	}
	return m_hpce.Lookup(Key,rData);
}

int CDB_HPCE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_hpce->GetCount();
	}
	return m_hpce.GetCount();
}

POSITION CDB_HPCE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_hpce->GetStartPosition();
	}
	return m_hpce.GetStartPosition();
}

void CDB_HPCE::GetNext(POSITION& rNextPosition,T_HPCE_K& rKey,T_HPCE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_hpce->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_hpce.Lookup(rKey, rData);
		return;
	}
	m_hpce.GetNextAssoc(rNextPosition,rKey,rData);
}
