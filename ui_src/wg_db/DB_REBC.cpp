#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_REBC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_REBC::CDB_REBC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebc.InitHashTable(HASHSIZEREBC);
}

CDB_REBC::CDB_REBC(CDB_REBC& src)
{
	*this = src;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebc.InitHashTable(HASHSIZEREBC);
}

CDB_REBC::~CDB_REBC()
{
	DelAllList();
}

CDB_REBC& CDB_REBC::operator= (CDB_REBC& src)
{
	POSITION pos;
	
	T_REBC_K KeyRebc;
	T_REBC_D DataRebc;
	m_rebc.RemoveAll();
	pos=src.m_rebc.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rebc.GetNextAssoc(pos,KeyRebc.keymap,DataRebc);
		m_rebc.SetAt(KeyRebc.keymap, DataRebc);
	}
	
	DelAllList();
	RebcList* pList;
	pos=src.m_rebclist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rebclist.GetNextAssoc(pos,KeyRebc.keymap,pList);
		RebcList* pListNew = new RebcList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rebclist.SetAt(KeyRebc.keymap, pListNew);
	}
	
	return *this;
}

void CDB_REBC::Add(T_REBC_K Key,T_REBC_D& rData,CDB_SECT* pSect)
{
	T_REBC_D Data;
	BOOL bExist=m_rebc.Lookup(Key.keymap, Data);
	m_rebc.SetAt(Key.keymap,rData);
	if(!bExist)pSect->AddListItem(Key.key.sectK, LT_REBC_CMD, Key.keymap);
}

BOOL CDB_REBC::Del(T_REBC_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rebc.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key.key.sectK, LT_REBC_CMD, Key.keymap));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_REBC::Get(T_REBC_K Key,T_REBC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rebc.Lookup(Key.keymap,rData);
}

int CDB_REBC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebc.GetCount();
}

POSITION CDB_REBC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebc.GetStartPosition();
}

void CDB_REBC::GetNext(POSITION& rNextPosition,T_REBC_K& rKey,T_REBC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rebc.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

void CDB_REBC::AddList(T_REBC_KEY Key)
{
	RebcList* pList = new RebcList;
	m_rebclist.SetAt(Key, pList);
}

BOOL CDB_REBC::DelList(T_REBC_KEY Key)
{
	RebcList* pList;
	BOOL bExist=m_rebclist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_rebclist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_REBC::DelAllList()
{
	T_REBC_KEY Key;
	RebcList* pList;
	POSITION pos=m_rebclist.GetStartPosition();
	while(pos != NULL)
	{
		m_rebclist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_rebclist.RemoveAll();
}

void CDB_REBC::AddListItem(T_REBC_K Key, int nCmd, int nKey)
{
	RebcList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_REBC::DelListItem(T_REBC_K Key, int nCmd, int nKey)
{
	RebcList* pList;
	if(!GetList(Key, pList))return TRUE;
	
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