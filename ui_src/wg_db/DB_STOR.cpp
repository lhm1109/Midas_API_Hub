#include "stdafx.h"
#include "DB_STOR.h"

//#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_STOR::CDB_STOR()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_stor.InitHashTable(HASHSIZESTOR);
	m_storlist.InitHashTable(HASHSIZELISTSTOR);
}

CDB_STOR::~CDB_STOR()
{
	DelAllList();
}

void CDB_STOR::Add(T_STOR_K Key,T_STOR_D& rData)
{
	m_stor.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_STOR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_STOR::Del(T_STOR_K Key)
{
	BOOL ret=m_stor.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_STOR_K key;
			T_STOR_D data;
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

void CDB_STOR::AddList(T_STOR_K Key)
{
	StorList* pList = new StorList;
	m_storlist.SetAt(Key, pList);
}

BOOL CDB_STOR::DelList(T_STOR_K Key)
{
	StorList* pList;
	BOOL bExist=m_storlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_storlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_STOR::DelAllList()
{
	T_STOR_K Key;
	StorList* pList;
	POSITION pos=m_storlist.GetStartPosition();
	while(pos != NULL)
	{
		m_storlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_storlist.RemoveAll();
}

void CDB_STOR::AddListItem(T_STOR_K Key, int nCmd, int nKey)
{
	StorList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_STOR::DelListItem(T_STOR_K Key, int nCmd, int nKey)
{
	StorList* pList;
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

//-------------------------------------------------------------------------
/*
BOOL CDB_STOR::Get(T_STOR_K Key,T_STOR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_stor.Lookup(Key,rData);
}

int CDB_STOR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_stor.GetCount();
}

POSITION CDB_STOR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_stor.GetStartPosition();
}

void CDB_STOR::GetNext(POSITION& rNextPosition,T_STOR_K& rKey,T_STOR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_stor.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_STOR::GetList(T_STOR_K Key,StorList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_storlist.Lookup(Key,rpList);
}

int CDB_STOR::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_storlist.GetCount();
}
*/