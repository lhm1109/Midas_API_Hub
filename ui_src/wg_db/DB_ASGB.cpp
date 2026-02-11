#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ASGB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ASGB::CDB_ASGB()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nStartNum=1;
	m_nLastNum=0;
	m_asgb.InitHashTable(HASHSIZEASGB);
	m_asgbSeq.InitHashTable(HASHSIZEASGB);
	m_asgblist.InitHashTable(HASHSIZELISTASGB);
}

CDB_ASGB::~CDB_ASGB()
{
	DelAllList();
}

void CDB_ASGB::Add(T_ASGB_K Key,T_ASGB_D& rData)
{
	m_asgb.SetAt(Key,rData);
	m_asgbSeq.SetAt(rData.nSeq, Key);
	if(Key == m_nStartNum)
	{
		T_ASGB_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_ASGB::Del(T_ASGB_K Key)
{
	T_ASGB_D Data;
	BOOL bExist=m_asgb.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_asgb.RemoveKey(Key);
		m_asgbSeq.RemoveKey(Data.nSeq);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_ASGB_K key;
				T_ASGB_D data;
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

void CDB_ASGB::AddList(T_ASGB_K Key)
{
	AsgbList* pList = new AsgbList;
	m_asgblist.SetAt(Key, pList);
}

BOOL CDB_ASGB::DelList(T_ASGB_K Key)
{
	AsgbList* pList;
	BOOL bExist=m_asgblist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_asgblist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_ASGB::DelAllList()
{
	T_ASGB_K Key;
	AsgbList* pList;
	POSITION pos=m_asgblist.GetStartPosition();
	while(pos != NULL)
	{
		m_asgblist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_asgblist.RemoveAll();
}

void CDB_ASGB::AddListItem(T_ASGB_K Key, int nCmd, int nKey)
{
	AsgbList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_asgblist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForAsgb(Key, nCmd, nKey);
}

BOOL CDB_ASGB::DelListItem(T_ASGB_K Key, int nCmd, int nKey)
{
	AsgbList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_asgblist.Lookup(Key, pList));

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
		return m_pDoc->m_pStagCtrl->DelListItemForAsgb(Key, nCmd, nKey);
	return TRUE;
}

//-------------------------------------------------------------------------
BOOL CDB_ASGB::GetList(T_ASGB_K Key,AsgbList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_asgblist->Lookup(Key,rpList);
	}
	return m_asgblist.Lookup(Key,rpList);
}

int CDB_ASGB::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_asgblist->GetCount();
	}
	return m_asgblist.GetCount();
}
