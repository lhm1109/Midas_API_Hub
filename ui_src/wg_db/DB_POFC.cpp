#include "stdafx.h"
#include "DB_POFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POFC::CDB_POFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_pofc.InitHashTable(HASHSIZEPOFC);
	m_pofclist.InitHashTable(HASHSIZELISTPOLC);
}

CDB_POFC::~CDB_POFC()
{
	DelAllList();
}

void CDB_POFC::Add(T_POFC_K Key,T_POFC_D& rData)
{
	m_pofc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_POFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_POFC::Del(T_POFC_K Key)
{
	T_POFC_D Data;
	m_pofc.Lookup(Key, Data);
	BOOL ret=m_pofc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{    
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_POFC_K key;
			T_POFC_D data;
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

void CDB_POFC::AddList(T_POFC_K Key)
{
	PofcList* pList = new PofcList;
	m_pofclist.SetAt(Key, pList);
}

BOOL CDB_POFC::DelList(T_POFC_K Key)
{
	PofcList* pList;
	BOOL bExist=m_pofclist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_pofclist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_POFC::DelAllList()
{
	T_POFC_K Key;
	PofcList* pList;
	POSITION pos=m_pofclist.GetStartPosition();
	while(pos != NULL)
	{
		m_pofclist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_pofclist.RemoveAll();
}

void CDB_POFC::AddListItem(T_POFC_K Key, int nCmd, int nKey)
{
	PofcList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_POFC::DelListItem(T_POFC_K Key, int nCmd, int nKey)
{
	PofcList* pList;
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
BOOL CDB_POFC::Get(T_POFC_K Key,T_POFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pofc.Lookup(Key,rData);
}

int CDB_POFC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pofc.GetCount();
}

POSITION CDB_POFC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pofc.GetStartPosition();
}

void CDB_POFC::GetNext(POSITION& rNextPosition,T_POFC_K& rKey,T_POFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_pofc.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_POFC::GetList(T_POFC_K Key, PofcList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pofclist.Lookup(Key,rpList);
}

int CDB_POFC::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pofclist.GetCount();
}
