#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_CRAN.h"

#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_CRAN::CDB_CRAN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_cran.InitHashTable(HASHSIZECRAN);
}

CDB_CRAN::~CDB_CRAN()
{
}

void CDB_CRAN::Add(T_CRAN_K Key,T_CRAN_D& rData,CDB_NODE* pNode)
{
	T_CRAN_D Data;
	BOOL bExist=m_cran.Lookup(Key, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapOld2;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNew;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNew2;
	T_NODE_K KeyNodeTemp;
	if(bExist)
	{
		mapNew.InitHashTable(HASHSIZENODE);
		mapOld.InitHashTable(HASHSIZENODE);
		mapNew2.InitHashTable(HASHSIZENODE);
		mapOld2.InitHashTable(HASHSIZENODE);
		for(int i = 0; i<Data.arNode.GetSize(); i++)mapOld.SetAt(Data.arNode[i], Data.arNode[i]);
		for(int i = 0; i<Data.arNode2.GetSize(); i++)mapOld2.SetAt(Data.arNode2[i], Data.arNode2[i]);
		for(int i = 0; i<rData.arNode.GetSize(); i++)mapNew.SetAt(rData.arNode[i], rData.arNode[i]);
		for(int i = 0; i<rData.arNode2.GetSize(); i++)mapNew2.SetAt(rData.arNode2[i], rData.arNode2[i]);
		int nCount=Data.arNode.GetSize();
		for(int i = 0; i<nCount; i++)
		{
			if(!mapNew.Lookup(Data.arNode[i], KeyNodeTemp))
				pNode->DelListItem(Data.arNode[i], LT_CRAN_CMD, Key);
		}
		int nCount2=Data.arNode2.GetSize();
		for(int i = 0; i<nCount2; i++)
		{
			if(!mapNew2.Lookup(Data.arNode2[i], KeyNodeTemp))
				pNode->DelListItem(Data.arNode2[i], LT_CRAN_CMD, Key);
		}
	}

	int nCount=rData.arNode.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.arNode[i], KeyNodeTemp))
				pNode->AddListItem(rData.arNode[i], LT_CRAN_CMD, Key);
		}
		else
			pNode->AddListItem(rData.arNode[i], LT_CRAN_CMD, Key);
	}
	int nCount2=rData.arNode2.GetSize();
	for(int i = 0; i<nCount2; i++)
	{
		if(bExist)
		{
			if(!mapOld2.Lookup(rData.arNode2[i], KeyNodeTemp))
				pNode->AddListItem(rData.arNode2[i], LT_CRAN_CMD, Key);
		}
		else
			pNode->AddListItem(rData.arNode2[i], LT_CRAN_CMD, Key);
	}

	m_cran.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_CRAN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_CRAN::Del(T_CRAN_K Key,CDB_NODE* pNode)
{
	T_CRAN_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_cran.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.arNode.GetSize();
		for(int i=0; i<nCount; i++)pNode->DelListItem(data.arNode[i], LT_CRAN_CMD, Key);
		int nCount2=data.arNode2.GetSize();
		for(int i=0; i<nCount2; i++)pNode->DelListItem(data.arNode2[i], LT_CRAN_CMD, Key);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CRAN_K key;
			T_CRAN_D data;
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

BOOL CDB_CRAN::Get(T_CRAN_K Key,T_CRAN_D& rData)
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cran->Lookup(Key, uTemp)) return FALSE;
	}*/
	return m_cran.Lookup(Key,rData);
}

int CDB_CRAN::GetCount()
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cran->GetCount();
	}*/
	return m_cran.GetCount();
}

POSITION CDB_CRAN::GetStart()
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cran->GetStartPosition();
	}*/
	return m_cran.GetStartPosition();
}

void CDB_CRAN::GetNext(POSITION& rNextPosition,T_CRAN_K& rKey,T_CRAN_D& rData)
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cran->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cran.Lookup(rKey, rData);
		return;
	}*/
	m_cran.GetNextAssoc(rNextPosition,rKey,rData);
}
