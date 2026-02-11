#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_P1LT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_P1LT::CDB_P1LT()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nStartNum=1;
	m_nLastNum=0;
	m_p1lt.InitHashTable(HASHSIZEP1LT);
	m_p1ltlist.InitHashTable(HASHSIZELISTP1LT);
}

CDB_P1LT::~CDB_P1LT()
{
	DelAllList();
}

void CDB_P1LT::Add(T_P1LT_K Key,T_P1LT_D& rData)
{
	m_p1lt.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_P1LT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_P1LT::Del(T_P1LT_K Key)
{
	T_P1LT_D Data;
	BOOL bExist=m_p1lt.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_p1lt.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_P1LT_K key;
				T_P1LT_D data;
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


void CDB_P1LT::AddList(T_P1LT_K Key)
{
	P1ltList* pList = new P1ltList;
	m_p1ltlist.SetAt(Key, pList);
}

BOOL CDB_P1LT::DelList(T_P1LT_K Key)
{
	P1ltList* pList;
	BOOL bExist=m_p1ltlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_p1ltlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_P1LT::DelAllList()
{
	T_P1LT_K Key;
	P1ltList* pList;
	POSITION pos=m_p1ltlist.GetStartPosition();
	while(pos != NULL)
	{
		m_p1ltlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_p1ltlist.RemoveAll();
}

void CDB_P1LT::AddListItem(T_P1LT_K Key, int nCmd, int nKey)
{
	P1ltList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_p1ltlist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_P1LT::DelListItem(T_P1LT_K Key, int nCmd, int nKey)
{
	P1ltList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_p1ltlist.Lookup(Key, pList));

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
	return TRUE;
}

//-------------------------------------------------------------------------
BOOL CDB_P1LT::Get(T_P1LT_K Key,T_P1LT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
	}
	return m_p1lt.Lookup(Key,rData);
}

int CDB_P1LT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_p1lt.GetCount();
}

POSITION CDB_P1LT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_p1lt.GetStartPosition();
}

void CDB_P1LT::GetNext(POSITION& rNextPosition,T_P1LT_K& rKey,T_P1LT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
	}
	m_p1lt.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_P1LT::GetList(T_P1LT_K Key,P1ltList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		return m_pDoc->m_pStagCtrl->m_p1ltlist->Lookup(Key,rpList);
	}
	return m_p1ltlist.Lookup(Key,rpList);
}

int CDB_P1LT::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_p1ltlist->GetCount();
	}
	return m_p1ltlist.GetCount();
}
