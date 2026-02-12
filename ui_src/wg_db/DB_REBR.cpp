#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_REBR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_REBR::CDB_REBR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebr.InitHashTable(HASHSIZEREBR);
}

CDB_REBR::CDB_REBR(CDB_REBR& src)
{
	*this = src;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebr.InitHashTable(HASHSIZEREBR);
}

CDB_REBR::~CDB_REBR()
{
	DelAllList();
}

CDB_REBR& CDB_REBR::operator= (CDB_REBR& src)
{
	POSITION pos;
	
	T_REBR_K KeyRebr;
	T_REBR_D DataRebr;
	m_rebr.RemoveAll();
	pos=src.m_rebr.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rebr.GetNextAssoc(pos,KeyRebr.keymap,DataRebr);
		m_rebr.SetAt(KeyRebr.keymap, DataRebr);
	}
	
	DelAllList();
	RebrList* pList;
	pos=src.m_rebrlist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rebrlist.GetNextAssoc(pos,KeyRebr.keymap,pList);
		RebrList* pListNew = new RebrList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rebrlist.SetAt(KeyRebr.keymap, pListNew);
	}
	
	return *this;
}

void CDB_REBR::Add(T_REBR_K Key,T_REBR_D& rData,CDB_SECT* pSect)
{
	T_REBR_D Data;
	BOOL bExist=m_rebr.Lookup(Key.keymap, Data);
	m_rebr.SetAt(Key.keymap,rData);
	if(!bExist)pSect->AddListItem(Key.key.sectK, LT_REBR_CMD, Key.keymap);
}

BOOL CDB_REBR::Del(T_REBR_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rebr.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key.key.sectK, LT_REBR_CMD, Key.keymap));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_REBR::Get(T_REBR_K Key,T_REBR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rebr.Lookup(Key.keymap,rData);
}

int CDB_REBR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebr.GetCount();
}

POSITION CDB_REBR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebr.GetStartPosition();
}

void CDB_REBR::GetNext(POSITION& rNextPosition,T_REBR_K& rKey,T_REBR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rebr.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

void CDB_REBR::AddList(T_REBR_KEY Key)
{
	RebrList* pList = new RebrList;
	m_rebrlist.SetAt(Key, pList);
}

BOOL CDB_REBR::DelList(T_REBR_KEY Key)
{
	RebrList* pList;
	BOOL bExist=m_rebrlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_rebrlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_REBR::DelAllList()
{
	T_REBR_KEY Key;
	RebrList* pList;
	POSITION pos=m_rebrlist.GetStartPosition();
	while(pos != NULL)
	{
		m_rebrlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_rebrlist.RemoveAll();
}

void CDB_REBR::AddListItem(T_REBR_K Key, int nCmd, int nKey)
{
	RebrList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_REBR::DelListItem(T_REBR_K Key, int nCmd, int nKey)
{
	RebrList* pList;
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