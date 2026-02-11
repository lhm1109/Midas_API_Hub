#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_MGDR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MGDR::CDB_MGDR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mgdr.InitHashTable(HASHSIZEMGDR);
}

CDB_MGDR::~CDB_MGDR()
{
}

void CDB_MGDR::Add(T_MGDR_K Key,T_MGDR_D& rData,CDB_NODE* pNode)
{
	T_MGDR_D Data;
	BOOL bExist=m_mgdr.Lookup(Key, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNew;
	T_NODE_K KeyNodeTemp;
	if(bExist)
	{
		mapNew.InitHashTable(HASHSIZENODE);
		mapOld.InitHashTable(HASHSIZENODE);
		for(int i = 0; i<Data.aData.GetSize(); i++)mapOld.SetAt(Data.aData[i].KeyNode, Data.aData[i].KeyNode);
		for(int i = 0; i<rData.aData.GetSize(); i++)mapNew.SetAt(rData.aData[i].KeyNode, rData.aData[i].KeyNode);
		int nCount=Data.aData.GetSize();
		for(int i = 0; i<nCount; i++)
		{
			if(!mapNew.Lookup(Data.aData[i].KeyNode, KeyNodeTemp))
				pNode->DelListItem(Data.aData[i].KeyNode, LT_MGDR_CMD, Key);
		}
	}
	int nCount=rData.aData.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.aData[i].KeyNode, KeyNodeTemp))
				pNode->AddListItem(rData.aData[i].KeyNode, LT_MGDR_CMD, Key);
		}
		else
			pNode->AddListItem(rData.aData[i].KeyNode, LT_MGDR_CMD, Key);
	}
	m_mgdr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MGDR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MGDR::Del(T_MGDR_K Key,CDB_NODE* pNode)
{
	T_MGDR_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_mgdr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.aData.GetSize();
		for(int i=0; i<nCount; i++)pNode->DelListItem(data.aData[i].KeyNode, LT_MGDR_CMD, Key);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MGDR_K key;
			T_MGDR_D data;
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
BOOL CDB_MGDR::Get(T_MGDR_K Key,T_MGDR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mgdr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mgdr.Lookup(Key,rData);
}

int CDB_MGDR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mgdr->GetCount();
	}
	return m_mgdr.GetCount();
}

POSITION CDB_MGDR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mgdr->GetStartPosition();
	}
	return m_mgdr.GetStartPosition();
}

void CDB_MGDR::GetNext(POSITION& rNextPosition,T_MGDR_K& rKey,T_MGDR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mgdr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mgdr.Lookup(rKey, rData);
		return;
	}
	m_mgdr.GetNextAssoc(rNextPosition,rKey,rData);
}
