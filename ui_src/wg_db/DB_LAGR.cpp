#include "stdafx.h"
#include "DB_LAGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LAGR::CDB_LAGR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_lagr.InitHashTable(HASHSIZELAGR);
	m_lagrlist.InitHashTable(HASHSIZELISTLAGR);
	AddList(0); // add default group
}

CDB_LAGR::~CDB_LAGR()
{
	DelAllList();
}

void CDB_LAGR::Add(T_LAGR_K Key,T_LAGR_D& rData)
{
	// 새로 추가된 경우 기존 Group의 GroupId가 추가된 GroupId보다 
	// 크거나 같으면 GroupId를 1씩 증가
	T_LAGR_D data;
	BOOL bExist = m_lagr.Lookup(Key, data);
	if (!bExist)  
	{
		T_LAGR_K key;
		T_LAGR_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId >= rData.nGroupId) 
			{
				data.nGroupId++;
				m_lagr.SetAt(key, data);
			}
		}
	}
	m_lagr.SetAt(Key,rData);
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

BOOL CDB_LAGR::Del(T_LAGR_K Key)
{
	T_LAGR_D Data;
	m_lagr.Lookup(Key, Data);
	BOOL ret=m_lagr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LAGR_K key;
			T_LAGR_D data;
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

		T_LAGR_K key;
		T_LAGR_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId > Data.nGroupId)
			{
				data.nGroupId--;
				m_lagr.SetAt(key, data);
			}
		}
	}
	return ret;
}

void CDB_LAGR::AddList(T_LAGR_K Key)
{
	LagrList* pList = new LagrList;
	m_lagrlist.SetAt(Key, pList);
}

BOOL CDB_LAGR::DelList(T_LAGR_K Key)
{
	LagrList* pList;
	BOOL bExist=m_lagrlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_lagrlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_LAGR::DelAllList()
{
	T_LAGR_K Key;
	LagrList* pList;
	POSITION pos=m_lagrlist.GetStartPosition();
	while(pos != NULL)
	{
		m_lagrlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_lagrlist.RemoveAll();
}

void CDB_LAGR::AddListItem(T_LAGR_K Key, int nCmd, int nKey)
{
	LaplList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_LAGR::DelListItem(T_LAGR_K Key, int nCmd, int nKey)
{
	LaplList* pList;
	if(!GetList(Key, pList))return TRUE;
	
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

BOOL CDB_LAGR::GetList(T_LAGR_K Key,LagrList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		ASSERT(0);
		//return m_pDoc->m_pStagCtrl->m_lagrlist->Lookup(Key,rpList);
	}
	return m_lagrlist.Lookup(Key,rpList);
}
