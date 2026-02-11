#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_NLLP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NLLP::CDB_NLLP()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nStartNum=1;
	m_nLastNum=0;
	m_nllp.InitHashTable(HASHSIZENLLP);
	m_nllpSeq.InitHashTable(HASHSIZENLLP);
	m_nllplist.InitHashTable(HASHSIZELISTNLLP);
}

CDB_NLLP::~CDB_NLLP()
{
	DelAllList();
}

void CDB_NLLP::Add(T_NLLP_K Key,T_NLLP_D& rData)
{
	m_nllp.SetAt(Key,rData);
	m_nllpSeq.SetAt(rData.nSeq, Key);
	if(Key == m_nStartNum)
	{
		T_NLLP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_NLLP::Del(T_NLLP_K Key)
{
	T_NLLP_D Data;
	BOOL bExist=m_nllp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_nllp.RemoveKey(Key);
		m_nllpSeq.RemoveKey(Data.nSeq);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_NLLP_K key;
				T_NLLP_D data;
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

void CDB_NLLP::AddList(T_NLLP_K Key)
{
	NllpList* pList = new NllpList;
	m_nllplist.SetAt(Key, pList);
}

BOOL CDB_NLLP::DelList(T_NLLP_K Key)
{
	NllpList* pList;
	BOOL bExist=m_nllplist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_nllplist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_NLLP::DelAllList()
{
	T_NLLP_K Key;
	NllpList* pList;
	POSITION pos=m_nllplist.GetStartPosition();
	while(pos != NULL)
	{
		m_nllplist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_nllplist.RemoveAll();
}

void CDB_NLLP::AddListItem(T_NLLP_K Key, int nCmd, int nKey)
{
	NllpList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_nllplist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForNllp(Key, nCmd, nKey);
}

BOOL CDB_NLLP::DelListItem(T_NLLP_K Key, int nCmd, int nKey)
{
	NllpList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_nllplist.Lookup(Key, pList));

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
		return m_pDoc->m_pStagCtrl->DelListItemForNllp(Key, nCmd, nKey);
	return TRUE;
}

//-------------------------------------------------------------------------
BOOL CDB_NLLP::GetList(T_NLLP_K Key,NllpList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nllplist->Lookup(Key,rpList);
	}
	return m_nllplist.Lookup(Key,rpList);
}

int CDB_NLLP::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nllplist->GetCount();
	}
	return m_nllplist.GetCount();
}
