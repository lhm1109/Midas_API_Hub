#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_SMPT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SMPT::CDB_SMPT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_smpt.InitHashTable(HASHSIZESMPT);
	m_smptlist.InitHashTable(HASHSIZELISTSMPT);
}

CDB_SMPT::~CDB_SMPT()
{
	DelAllList();
}

void CDB_SMPT::Add(T_SMPT_K Key,T_SMPT_D& rData,CDB_NODE* pNode)
{
	T_SMPT_D Data;
	BOOL bExist=m_smpt.Lookup(Key, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNew;
	T_NODE_K KeyNodeTemp;
	if(bExist)
	{
		mapNew.InitHashTable(HASHSIZENODE);
		mapOld.InitHashTable(HASHSIZENODE);
		for(int i = 0; i<Data.aNodeData.GetSize(); i++)mapOld.SetAt(Data.aNodeData[i], Data.aNodeData[i]);
		for(int i = 0; i<rData.aNodeData.GetSize(); i++)mapNew.SetAt(rData.aNodeData[i], rData.aNodeData[i]);
		for(int i = 0; i<Data.aNodeData.GetSize(); i++)
		{
			if(!mapNew.Lookup(Data.aNodeData[i], KeyNodeTemp))
				VERIFY(pNode->DelListItem(Data.aNodeData[i], LT_SMPT_CMD, Key));
		}
	}
	m_smpt.SetAt(Key,rData);
	for(int i=0; i<rData.aNodeData.GetSize(); i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.aNodeData[i], KeyNodeTemp))
				pNode->AddListItem(rData.aNodeData[i], LT_SMPT_CMD, Key);
		}
		else
			pNode->AddListItem(rData.aNodeData[i], LT_SMPT_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_SMPT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SMPT::Del(T_SMPT_K Key,CDB_NODE* pNode)
{
	T_SMPT_D Data;
	BOOL bExist=m_smpt.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_smpt.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			for(int i=0; i<Data.aNodeData.GetSize(); i++)
			{
				VERIFY(pNode->DelListItem(Data.aNodeData[i], LT_SMPT_CMD, Key));
			}
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_SMPT_K key;
				T_SMPT_D data;
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
	return FALSE;
}

void CDB_SMPT::AddList(T_SMPT_K Key)
{
	SmptList* pList = new SmptList;
	m_smptlist.SetAt(Key, pList);
}

BOOL CDB_SMPT::DelList(T_SMPT_K Key)
{
	SmptList* pList;
	BOOL bExist=m_smptlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_smptlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_SMPT::DelAllList()
{
	T_SMPT_K Key;
	SmptList* pList;
	POSITION pos=m_smptlist.GetStartPosition();
	while(pos != NULL)
	{
		m_smptlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_smptlist.RemoveAll();
}

void CDB_SMPT::AddListItem(T_SMPT_K Key, int nCmd, int nKey)
{
	SmptList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_SMPT::DelListItem(T_SMPT_K Key, int nCmd, int nKey)
{
	SmptList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}

//-------------------------------------------------------------------------
BOOL CDB_SMPT::Get(T_SMPT_K Key,T_SMPT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_smpt->Lookup(Key, uTemp)) return FALSE;
	}
	return m_smpt.Lookup(Key,rData);
}

int CDB_SMPT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_smpt->GetCount();
	}
	return m_smpt.GetCount();
}

POSITION CDB_SMPT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_smpt->GetStartPosition();
	}
	return m_smpt.GetStartPosition();
}

void CDB_SMPT::GetNext(POSITION& rNextPosition,T_SMPT_K& rKey,T_SMPT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_smpt->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_smpt.Lookup(rKey, rData);
		return;
	}
	m_smpt.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_SMPT::GetList(T_SMPT_K Key,SmptList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		return m_pDoc->m_pStagCtrl->m_smptlist->Lookup(Key, rpList);
	}
	return m_smptlist.Lookup(Key,rpList);
}

int CDB_SMPT::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_smptlist->GetCount();
	}
	return m_smptlist.GetCount();
}
