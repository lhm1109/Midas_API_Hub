#include "stdafx.h"
#include "DB_EPSE.h"

#include "DBDoc.h"
#include "StagCtrl.h"
#include "DB_STLD.h"
#include "DB_LDGR.h"
#include "DB_POSP.h"
#include "DB_LAPL.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"

CDB_EPSE::CDB_EPSE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_epse.InitHashTable(HASHSIZEEPSE);
	m_epselist.InitHashTable(HASHSIZELISTEPSE);
	AddList(0); // add default group
}

CDB_EPSE::~CDB_EPSE()
{
	DelAllList();
}

void CDB_EPSE::Add(T_EPSE_K Key, T_EPSE_D& rData, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode)
{
	T_EPSE_D data;
	BOOL bExist = m_epse.Lookup(Key, data);

	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapElemNew;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapElemOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapNodeNew;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapNodeOld;

	int nSize;
	T_ELEM_K KeyElemTemp;
	T_NODE_K KeyNodeTemp;

	if (bExist)
	{
		//다르면 제거한다.
		if (data.KeyStld != rData.KeyStld)
			VERIFY(pStld->DelListItem(data.KeyStld, LT_EPSE_CMD, Key));
		//     if(data.KeyLdgr != rData.KeyLdgr)
		//       VERIFY(pLdgr->DelListItem(data.KeyLdgr, LT_EPSE_CMD, Key));
		if (data.KeyLapl != rData.KeyLapl)
			VERIFY(pLapl->DelListItem(data.KeyLapl, LT_EPSE_CMD, Key));

		for(int i = 0; i < rData.arKeysDefineElem.GetSize(); i++) mapElemNew.SetAt(rData.arKeysDefineElem[i], rData.arKeysDefineElem[i]);
		for(int i = 0; i < data.arKeysDefineElem.GetSize(); i++) mapElemOld.SetAt(data.arKeysDefineElem[i], data.arKeysDefineElem[i]);
		for(int i = 0; i < rData.arKeysDefineNode.GetSize(); i++) mapNodeNew.SetAt(rData.arKeysDefineNode[i], rData.arKeysDefineNode[i]);
		for(int i = 0; i < data.arKeysDefineNode.GetSize(); i++) mapNodeOld.SetAt(data.arKeysDefineNode[i], data.arKeysDefineNode[i]);

		nSize = data.arKeysDefineElem.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			if (!mapElemNew.Lookup(data.arKeysDefineElem[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(data.arKeysDefineElem[i], LT_EPSE_CMD, Key));
		}
		nSize = data.arKeysDefineNode.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			if (!mapNodeNew.Lookup(data.arKeysDefineNode[i], KeyNodeTemp))
				VERIFY(pNode->DelListItem(data.arKeysDefineNode[i], LT_EPSE_CMD, Key));
		}
	}

	m_epse.SetAt(Key, rData);
	if (Key == m_nStartNum)
	{
		while (TRUE)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))
				break;
		}
	}
	if (Key > m_nLastNum)
		m_nLastNum = Key;

	if (bExist)
	{
		if (data.KeyStld != rData.KeyStld)
			pStld->AddListItem(rData.KeyStld, LT_EPSE_CMD, Key);
		//     if(data.KeyLdgr != rData.KeyLdgr)
		//       pLdgr->AddListItem(rData.KeyLdgr, LT_EPSE_CMD, Key);
		if (data.KeyLapl != rData.KeyLapl)
			pLapl->AddListItem(rData.KeyLapl, LT_EPSE_CMD, Key);

		nSize = rData.arKeysDefineElem.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			if (!mapElemOld.Lookup(rData.arKeysDefineElem[i], KeyElemTemp))
				pElem->AddListItem(rData.arKeysDefineElem[i], LT_EPSE_CMD, Key);
		}
		nSize = rData.arKeysDefineNode.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			if (!mapNodeOld.Lookup(rData.arKeysDefineNode[i], KeyNodeTemp))
				pNode->AddListItem(rData.arKeysDefineNode[i], LT_EPSE_CMD, Key);
		}
	}
	else
	{
		pStld->AddListItem(rData.KeyStld, LT_EPSE_CMD, Key);
		//pLdgr->AddListItem(rData.KeyLdgr, LT_EPSE_CMD, Key);
		pLapl->AddListItem(rData.KeyLapl, LT_EPSE_CMD, Key);

		nSize = rData.arKeysDefineElem.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			pElem->AddListItem(rData.arKeysDefineElem[i], LT_EPSE_CMD, Key);
		}
		nSize = rData.arKeysDefineNode.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			pNode->AddListItem(rData.arKeysDefineNode[i], LT_EPSE_CMD, Key);
		}
	}
}

BOOL CDB_EPSE::Del(T_EPSE_K Key, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode)
{
	T_EPSE_D Data;
	BOOL bExist = m_epse.Lookup(Key, Data);

	BOOL ret = m_epse.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if (bExist)
		{
			int nSize;

			if (Data.KeyStld != 0) VERIFY(pStld->DelListItem(Data.KeyStld, LT_EPSE_CMD, Key));
			//if(Data.KeyLdgr != 0) VERIFY(pLdgr->DelListItem(Data.KeyLdgr, LT_EPSE_CMD, Key));
			if (Data.KeyLapl != 0) VERIFY(pLapl->DelListItem(Data.KeyLapl, LT_EPSE_CMD, Key));

			nSize = Data.arKeysDefineElem.GetSize();
			for(int i = 0; i < nSize; i++) VERIFY(pElem->DelListItem(Data.arKeysDefineElem[i], LT_EPSE_CMD, Key));
			nSize = Data.arKeysDefineNode.GetSize();
			for(int i = 0; i < nSize; i++) VERIFY(pNode->DelListItem(Data.arKeysDefineNode[i], LT_EPSE_CMD, Key));
		}

		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_EPSE_K key;
			T_EPSE_D data;
			if (Get(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = GetStart();
				while (pos != NULL)
				{
					GetNext(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
	}
	return ret;
}

void CDB_EPSE::AddList(T_EPSE_K Key)
{
	EpseList* pList = new EpseList;
	m_epselist.SetAt(Key, pList);
}

BOOL CDB_EPSE::DelList(T_EPSE_K Key)
{
	EpseList* pList;
	BOOL bExist = m_epselist.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist)return FALSE;
	m_epselist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_EPSE::DelAllList()
{
	T_EPSE_K Key;
	EpseList* pList;
	POSITION pos = m_epselist.GetStartPosition();
	while (pos != NULL)
	{
		m_epselist.GetNextAssoc(pos, Key, pList);
		delete pList;
	}
	m_epselist.RemoveAll();
}

void CDB_EPSE::AddListItem(T_EPSE_K Key, int nCmd, int nKey)
{
	LaplList* pList;
	if (!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	pList->AddTail(ListData);
}

BOOL CDB_EPSE::DelListItem(T_EPSE_K Key, int nCmd, int nKey)
{
	LaplList* pList;
	if (!GetList(Key, pList))return TRUE;

	T_LIST_DATA ListData;
	POSITION pos = pList->GetHeadPosition(), posPrv;
	while (pos != NULL)
	{
		posPrv = pos;
		ListData = pList->GetNext(pos);
		if (ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDB_EPSE::GetList(T_EPSE_K Key, EpseList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		ASSERT(0);
		//return m_pDoc->m_pStagCtrl->m_epselist->Lookup(Key,rpList);
	}
	return m_epselist.Lookup(Key, rpList);
}

