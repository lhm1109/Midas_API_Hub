#include "stdafx.h"
#include "DB_STLD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STLD::CDB_STLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_stld.InitHashTable(HASHSIZESTLD);
	m_stldlist.InitHashTable(HASHSIZELISTSTLD);
}

CDB_STLD::~CDB_STLD()
{
	DelAllList();
}

// PostCtrl에서 복사본 만들어 사용할 때 사용
void CDB_STLD::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_nStartNum=1;
	m_nLastNum=0;
	m_stld.RemoveAll();
	DelAllList();
}

void CDB_STLD::Add(T_STLD_K Key,T_STLD_D& rData)
{
	m_stld.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_STLD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_STLD::Del(T_STLD_K Key)
{
	BOOL ret=m_stld.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_STLD_K key;
			T_STLD_D data;
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

void CDB_STLD::AddList(T_STLD_K Key)
{
	StldList* pList = new StldList;
	m_stldlist.SetAt(Key, pList);
}

BOOL CDB_STLD::DelList(T_STLD_K Key)
{
	StldList* pList;
	BOOL bExist=m_stldlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_stldlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_STLD::DelAllList()
{
	T_STLD_K Key;
	StldList* pList;
	POSITION pos=m_stldlist.GetStartPosition();
	while(pos != NULL)
	{
		m_stldlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_stldlist.RemoveAll();
}

void CDB_STLD::AddListItem(T_STLD_K Key, int nCmd, int nKey)
{
	StldList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_stldlist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForStld(Key, nCmd, nKey);
}

BOOL CDB_STLD::DelListItem(T_STLD_K Key, int nCmd, int nKey)
{
	StldList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_stldlist.Lookup(Key, pList));

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
		return m_pDoc->m_pStagCtrl->DelListItemForStld(Key, nCmd, nKey);
	return TRUE;
}

//--------------------------------------------------------------------------
BOOL CDB_STLD::GetList(T_STLD_K Key,StldList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_stldlist->Lookup(Key,rpList);
	}
	return m_stldlist.Lookup(Key,rpList);
}

int CDB_STLD::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_stldlist->GetCount();
	}
	return m_stldlist.GetCount();
}
