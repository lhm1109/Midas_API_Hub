#include "stdafx.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BNGR::CDB_BNGR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_bngr.InitHashTable(HASHSIZEBNGR);
	m_bngrlist.InitHashTable(HASHSIZELISTBNGR);
	AddList(0); // add default group
}

CDB_BNGR::~CDB_BNGR()
{
	DelAllList();
}

void CDB_BNGR::Add(T_BNGR_K Key,T_BNGR_D& rData)
{
	// 새로 추가된 경우 기존 Group의 GroupId가 추가된 GroupId보다 
	// 크거나 같으면 GroupId를 1씩 증가
	T_BNGR_D data;
	BOOL bExist = m_bngr.Lookup(Key, data);
	if (!bExist)  
	{
		T_BNGR_K key;
		T_BNGR_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId >= rData.nGroupId) 
			{
				data.nGroupId++;
				m_bngr.SetAt(key, data);
			}
		}
	}
	m_bngr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_BNGR::Del(T_BNGR_K Key)
{
	T_BNGR_D Data;
	m_bngr.Lookup(Key, Data);
	BOOL ret=m_bngr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_BNGR_K key;
			T_BNGR_D data;
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

		T_BNGR_K key;
		T_BNGR_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId > Data.nGroupId)
			{
				data.nGroupId--;
				m_bngr.SetAt(key, data);
			}
		}
	}
	return ret;
}

void CDB_BNGR::AddList(T_BNGR_K Key)
{
	BngrList* pList = new BngrList;
	m_bngrlist.SetAt(Key, pList);
}

BOOL CDB_BNGR::DelList(T_BNGR_K Key)
{
	BngrList* pList;
	BOOL bExist=m_bngrlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_bngrlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_BNGR::DelAllList()
{
	T_BNGR_K Key;
	BngrList* pList;
	POSITION pos=m_bngrlist.GetStartPosition();
	while(pos != NULL)
	{
		m_bngrlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_bngrlist.RemoveAll();
}

void CDB_BNGR::AddListItem(T_BNGR_K Key, int nCmd, int nKey)
{
	BngrList* pList;
	//if(!GetList(Key, pList))return;
	if (!m_bngrlist.Lookup(Key, pList)) return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForBngr(Key, nCmd, nKey);
}

BOOL CDB_BNGR::DelListItem(T_BNGR_K Key, int nCmd, int nKey)
{
	BngrList* pList;
	//if(!GetList(Key, pList))return TRUE;
	if (!m_bngrlist.Lookup(Key, pList)) return TRUE;

	BOOL bDeleted = FALSE;
	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
			break;
		}
	}
	if (!bDeleted) return FALSE;

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->DelListItemForBngr(Key, nCmd, nKey);
	return TRUE;
}

BOOL CDB_BNGR::GetList(T_BNGR_K Key,BngrList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_bngrlist->Lookup(Key,rpList);
	}
	return m_bngrlist.Lookup(Key,rpList);
}
