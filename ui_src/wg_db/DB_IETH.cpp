#include "stdafx.h"
#include "DB_IETH.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IETH::CDB_IETH()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ieth.InitHashTable(HASHSIZEIETH);
	m_iethlist.InitHashTable(HASHSIZELISTIETH);
}

CDB_IETH::~CDB_IETH()
{
	DelAllList();
}

void CDB_IETH::Add(T_IETH_K Key,T_IETH_D& rData)
{
	m_ieth.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_IETH_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;  
}

BOOL CDB_IETH::Del(T_IETH_K Key)
{
	BOOL ret=m_ieth.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_IETH_K key;
			T_IETH_D data;
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

void CDB_IETH::AddList(T_IETH_K Key)
{
	PoldList* pList = new PoldList;
	m_iethlist.SetAt(Key, pList);
}

BOOL CDB_IETH::DelList(T_IETH_K Key)
{
	PoldList* pList;
	BOOL bExist=m_iethlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_iethlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_IETH::DelAllList()
{
	T_IETH_K Key;
	PoldList* pList;
	POSITION pos=m_iethlist.GetStartPosition();
	while(pos != NULL)
	{
		m_iethlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_iethlist.RemoveAll();
}

void CDB_IETH::AddListItem(T_IETH_K Key, int nCmd, int nKey)
{
	PoldList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_IETH::DelListItem(T_IETH_K Key, int nCmd, int nKey)
{
	PoldList* pList;
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
BOOL CDB_IETH::Get(T_IETH_K Key,T_IETH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_ieth.Lookup(Key,rData);
}

int CDB_IETH::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ieth.GetCount();
}

POSITION CDB_IETH::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ieth.GetStartPosition();
}

void CDB_IETH::GetNext(POSITION& rNextPosition,T_IETH_K& rKey,T_IETH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_ieth.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_IETH::GetList(T_IETH_K Key,PoldList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_iethlist.Lookup(Key,rpList);
}

int CDB_IETH::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_iethlist.GetCount();
}
