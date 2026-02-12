#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_DMGB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DMGB::CDB_DMGB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_dmgb.InitHashTable(HASHSIZEDMGB);
	m_dmgbSeq.InitHashTable(HASHSIZEDMGB);
	m_dmgblist.InitHashTable(HASHSIZELISTDMGB);
}

CDB_DMGB::~CDB_DMGB()
{
	DelAllList();
}

void CDB_DMGB::Add(T_DMGB_K Key,T_DMGB_D& rData)
{
	m_dmgb.SetAt(Key,rData);
	m_dmgbSeq.SetAt(rData.nSeq, Key);
	if(Key == m_nStartNum)
	{
		T_DMGB_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DMGB::Del(T_DMGB_K Key)
{
	T_DMGB_D Data;
	BOOL bExist=m_dmgb.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_dmgb.RemoveKey(Key);
		m_dmgbSeq.RemoveKey(Data.nSeq);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_DMGB_K key;
				T_DMGB_D data;
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

void CDB_DMGB::AddList(T_DMGB_K Key)
{
	DmgbList* pList = new DmgbList;
	m_dmgblist.SetAt(Key, pList);
}

BOOL CDB_DMGB::DelList(T_DMGB_K Key)
{
	DmgbList* pList;
	BOOL bExist=m_dmgblist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_dmgblist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_DMGB::DelAllList()
{
	T_DMGB_K Key;
	DmgbList* pList;
	POSITION pos=m_dmgblist.GetStartPosition();
	while(pos != NULL)
	{
		m_dmgblist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_dmgblist.RemoveAll();
}

void CDB_DMGB::AddListItem(T_DMGB_K Key, int nCmd, int nKey)
{
	DmgbList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_dmgblist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForDmgb(Key, nCmd, nKey);
}

BOOL CDB_DMGB::DelListItem(T_DMGB_K Key, int nCmd, int nKey)
{
	DmgbList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_dmgblist.Lookup(Key, pList));

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
		return m_pDoc->m_pStagCtrl->DelListItemForDmgb(Key, nCmd, nKey);
	return TRUE;
}

//-------------------------------------------------------------------------
BOOL CDB_DMGB::GetList(T_DMGB_K Key,DmgbList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_dmgblist->Lookup(Key,rpList);
	}
	return m_dmgblist.Lookup(Key,rpList);
}

int CDB_DMGB::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_dmgblist->GetCount();
	}
	return m_dmgblist.GetCount();
}
