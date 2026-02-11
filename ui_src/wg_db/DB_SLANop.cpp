#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_SLANop.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SLANop::CDB_SLANop()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_slanop.InitHashTable(HASHSIZESLANop);
}

CDB_SLANop::~CDB_SLANop()
{
}

void CDB_SLANop::Add(T_SLANop_K Key,T_SLANop_D& rData,CDB_NODE* pNode)
{
	T_SLANop_D Data;
	BOOL bExist=m_slanop.Lookup(Key, Data);
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
				pNode->DelListItem(Data.arLaneData[i].KeyNode, LT_SLANop_CMD, Key);
		}
	}
	int nCount=rData.arLaneData.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.arLaneData[i].KeyNode, KeyNodeTemp))
				pNode->AddListItem(rData.arLaneData[i].KeyNode, LT_SLANop_CMD, Key);
		}
		else
			pNode->AddListItem(rData.arLaneData[i].KeyNode, LT_SLANop_CMD, Key);
	}
	m_slanop.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SLANop_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SLANop::Del(T_SLANop_K Key,CDB_NODE* pNode)
{
	T_SLANop_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_slanop.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.arLaneData.GetSize();
		for(int i=0; i<nCount; i++)pNode->DelListItem(data.arLaneData[i].KeyNode, LT_SLANop_CMD, Key);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SLANop_K key;
			T_SLANop_D data;
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
BOOL CDB_SLANop::Get(T_SLANop_K Key,T_SLANop_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_slanop->Lookup(Key, uTemp)) return FALSE;
	}
	return m_slanop.Lookup(Key,rData);
}

int CDB_SLANop::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_slanop->GetCount();
	}
	return m_slanop.GetCount();
}

POSITION CDB_SLANop::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_slanop->GetStartPosition();
	}
	return m_slanop.GetStartPosition();
}

void CDB_SLANop::GetNext(POSITION& rNextPosition,T_SLANop_K& rKey,T_SLANop_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_slanop->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_slanop.Lookup(rKey, rData);
		return;
	}
	m_slanop.GetNextAssoc(rNextPosition,rKey,rData);
}
