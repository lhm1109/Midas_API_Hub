#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELNK.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ELNK::CDB_ELNK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_elnk.InitHashTable(HASHSIZEELNK);
	m_elnklist.InitHashTable(HASHSIZELISTELNK);
}

CDB_ELNK::~CDB_ELNK()
{
	DelAllList();
}

void CDB_ELNK::Add(T_ELNK_K Key,T_ELNK_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_ELNK_D Data;
	BOOL bExist=m_elnk.Lookup(Key, Data);
	if(bExist)
	{
		if (pNode)
		{
			VERIFY(pNode->DelListItem(Data.Node1, LT_ELNK_CMD, Key));
			VERIFY(pNode->DelListItem(Data.Node2, LT_ELNK_CMD, Key));
		}
		if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_ELNK_CMD, Key);
	}
	m_elnk.SetAt(Key,rData);
	if (pNode)
	{
		pNode->AddListItem(rData.Node1, LT_ELNK_CMD, Key);
		pNode->AddListItem(rData.Node2, LT_ELNK_CMD, Key);
	}
	if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_ELNK_CMD, Key);
	if(Key == m_nStartNum)
	{
		T_ELNK_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_elnk.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_ELNK_CMD, Key, rData.GroupKey);
	}
}

BOOL CDB_ELNK::Del(T_ELNK_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_ELNK_D Data;
	BOOL bExist=m_elnk.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_elnk.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pNode)
			{
				VERIFY(pNode->DelListItem(Data.Node1, LT_ELNK_CMD, Key));
				VERIFY(pNode->DelListItem(Data.Node2, LT_ELNK_CMD, Key));
			}
			if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_ELNK_CMD, Key);

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_ELNK_CMD, Key);
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ELNK_K key;
			T_ELNK_D data;
			if(m_elnk.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_elnk.GetStartPosition();
				while(pos != NULL)
				{
					m_elnk.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_ELNK::Get(T_ELNK_K Key,T_ELNK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_elnk->Lookup(Key, StagBngrKey)) return FALSE;
	}
	return m_elnk.Lookup(Key,rData);
}

int CDB_ELNK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_elnk->GetCount();
	}
	return m_elnk.GetCount();
}

POSITION CDB_ELNK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_elnk->GetStartPosition();
	}
	return m_elnk.GetStartPosition();
}

void CDB_ELNK::GetNext(POSITION& rNextPosition,T_ELNK_K& rKey,T_ELNK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_elnk->GetNextAssoc(rNextPosition, rKey, StagBngrKey);
		m_elnk.Lookup(rKey, rData);
		return;
	}
	m_elnk.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_ELNK::AddList(T_ELNK_K Key)
{
	ElnkList* pList = new ElnkList;
	m_elnklist.SetAt(Key, pList);
}

BOOL CDB_ELNK::DelList(T_ELNK_K Key)
{
	ElnkList* pList;
	BOOL bExist=m_elnklist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_elnklist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_ELNK::DelAllList()
{
	T_ELNK_K Key;
	ElnkList* pList;
	POSITION pos=m_elnklist.GetStartPosition();
	while(pos != NULL)
	{
		m_elnklist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_elnklist.RemoveAll();
}

BOOL CDB_ELNK::GetList(T_ELNK_K Key, PolcList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_elnklist.Lookup(Key,rpList);
}

int CDB_ELNK::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_elnklist.GetCount();
}


void CDB_ELNK::AddListItem(T_ELNK_K Key, int nCmd, int nKey)
{
	ElnkList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_ELNK::DelListItem(T_ELNK_K Key, int nCmd, int nKey)
{
	ElnkList* pList;
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