#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THCR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THCR::CDB_THCR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thcr.InitHashTable(HASHSIZETHCR);
}

CDB_THCR::~CDB_THCR()
{

}

void CDB_THCR::Add(T_THCR_K Key,T_THCR_D& rData)
{
	T_THCR_D Data;
	BOOL bExist=m_thcr.Lookup(Key, Data);
	m_thcr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_THCR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_thcr.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THCR::Del(T_THCR_K Key)
{
	T_THCR_D Data;
	BOOL bExist=m_thcr.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thcr.RemoveKey(Key);
		ASSERT(ret);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THCR_K key;
			T_THCR_D data;
			if(m_thcr.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thcr.GetStartPosition();
				while(pos != NULL)
				{
					m_thcr.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THCR::Get(T_THCR_K Key,T_THCR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thcr.Lookup(Key,rData);
}

int CDB_THCR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thcr.GetCount();
}

POSITION CDB_THCR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thcr.GetStartPosition();
}

void CDB_THCR::GetNext(POSITION& rNextPosition,T_THCR_K& rKey,T_THCR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thcr.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_THCR::AddList(T_THCR_K Key)
{
	DBDataList* pList = new DBDataList;
	m_thcrlist.SetAt(Key, pList);
}

BOOL CDB_THCR::DelList(T_THCR_K Key)
{
	DBDataList* pList;
	BOOL bExist = m_thcrlist.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist)return FALSE;
	m_thcrlist.RemoveKey(Key);

	delete pList;
	pList = NULL;

	return TRUE;
}

void CDB_THCR::DelAllList()
{
	T_SIPA_K Key;
	DBDataList* pList;
	POSITION pos = m_thcrlist.GetStartPosition();
	while (pos != NULL)
	{
		m_thcrlist.GetNextAssoc(pos, Key, pList);
		delete pList;
	}
	m_thcrlist.RemoveAll();
}

void CDB_THCR::AddListItem(T_THCR_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_thcrlist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	POSITION pos = pList->AddTail(ListData);
}

BOOL CDB_THCR::DelListItem(T_THCR_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_thcrlist.Lookup(Key, pList));

	BOOL bDeleted = FALSE;

	T_LIST_DATA ListData;
	POSITION pos = pList->GetTailPosition(), posPrv;
	while (pos != NULL)
	{
		posPrv = pos;
		ListData = pList->GetPrev(pos);
		if (ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
			break;
		}
	}
	if (!bDeleted) return FALSE;

	return TRUE;
}

BOOL CDB_THCR::GetList(T_THCR_K Key, DBDataList*& rpList)
{
	return m_thcrlist.Lookup(Key, rpList);
}