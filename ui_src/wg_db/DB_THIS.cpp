#include "stdafx.h"
#include "DB_THIS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THIS::CDB_THIS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_this.InitHashTable(HASHSIZETHIS);
	m_thislist.InitHashTable(HASHSIZELISTTHIS);
}

CDB_THIS::~CDB_THIS()
{
	DelAllList();
}

void CDB_THIS::Add(T_THIS_K Key,T_THIS_D& rData)
{
	m_this.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_THIS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THIS::Del(T_THIS_K Key)
{
	BOOL ret=m_this.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THIS_K key;
			T_THIS_D data;
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

void CDB_THIS::AddList(T_THIS_K Key)
{
	ThisList* pList = new ThisList;
	m_thislist.SetAt(Key, pList);
}

BOOL CDB_THIS::DelList(T_THIS_K Key)
{
	ThisList* pList;
	BOOL bExist=m_thislist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_thislist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_THIS::DelAllList()
{
	T_THIS_K Key;
	ThisList* pList;
	POSITION pos=m_thislist.GetStartPosition();
	while(pos != NULL)
	{
		m_thislist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_thislist.RemoveAll();
}

void CDB_THIS::AddListItem(T_THIS_K Key, int nCmd, int nKey)
{
	ThisList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_THIS::DelListItem(T_THIS_K Key, int nCmd, int nKey)
{
	ThisList* pList;
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
BOOL CDB_THIS::Get(T_THIS_K Key,T_THIS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_this.Lookup(Key,rData);
}

int CDB_THIS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_this.GetCount();
}

POSITION CDB_THIS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_this.GetStartPosition();
}

void CDB_THIS::GetNext(POSITION& rNextPosition,T_THIS_K& rKey,T_THIS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_this.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_THIS::GetList(T_THIS_K Key,ThisList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thislist.Lookup(Key,rpList);
}

int CDB_THIS::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thislist.GetCount();
}
