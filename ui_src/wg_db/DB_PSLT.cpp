#include "stdafx.h"
#include "DB_PSLT.h"

CDB_PSLT::CDB_PSLT()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pslt.InitHashTable(HASHSIZEPSLT);
}

CDB_PSLT::~CDB_PSLT()
{
	DelAllList();
}

void CDB_PSLT::Add(T_PSLT_K Key,T_PSLT_D& rData)
{
	T_PSLT_D Data;
	BOOL bExist=m_pslt.Lookup(Key, Data);
	m_pslt.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PSLT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PSLT::Del(T_PSLT_K Key)
{
	T_PSLT_D Data;
	BOOL bExist=m_pslt.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pslt.RemoveKey(Key);
		ASSERT(ret);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PSLT_K key;
			T_PSLT_D data;
			if(m_pslt.Lookup(m_nLastNum-1,data))
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
		return ret;
	}
	return bExist;
}


void CDB_PSLT::AddList(T_PSLT_K Key)
{
	PsltList* pList = new PsltList;
	m_psltlist.SetAt(Key, pList);
}

BOOL CDB_PSLT::DelList(T_PSLT_K Key)
{
	PsltList* pList;
	BOOL bExist=m_psltlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_psltlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_PSLT::DelAllList()
{
	T_PRES_KEY Key;
	PsltList* pList;
	POSITION pos=m_psltlist.GetStartPosition();
	while(pos != NULL)
	{
		m_psltlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_psltlist.RemoveAll();
}

void CDB_PSLT::AddListItem(T_PSLT_K Key, int nCmd, int nKey)
{
	PsltList* pList;
	if(m_psltlist.Lookup(Key, pList))
	{
		T_LIST_DATA ListData;
		ListData.nCmd=nCmd;
		ListData.nKey=nKey;
		pList->AddTail(ListData);
	}
}


BOOL CDB_PSLT::DelListItem(T_PSLT_K Key, int nCmd, int nKey,BOOL bOneElemAllPres)
{
	PsltList* pList;
	if(m_psltlist.Lookup(Key, pList))
	{
		BOOL bDeleted = FALSE;
		T_LIST_DATA ListData;
		POSITION pos=pList->GetHeadPosition(), posPrv;
		while(pos != NULL)
		{
			posPrv=pos;
			ListData=pList->GetNext(pos);
			if(bOneElemAllPres)
			{
				T_PRES_K OldKey,NewKey;
				OldKey.keymap = ListData.nKey;
				NewKey.keymap = nKey;
				if(ListData.nCmd == nCmd && OldKey.key.entity == NewKey.key.entity)
				{
					pList->RemoveAt(posPrv);
					bDeleted = TRUE;
					//break;
				}
			}
			else
			{
				if(ListData.nCmd == nCmd && ListData.nKey == nKey)
				{
					pList->RemoveAt(posPrv);
					bDeleted = TRUE;
					break;
				}
			}
	
		}
		if (!bDeleted) return FALSE;
	}
	else
		ASSERT(FALSE);
	
	return TRUE;
}