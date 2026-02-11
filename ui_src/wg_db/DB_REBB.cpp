#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_REBB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_REBB::CDB_REBB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebb.InitHashTable(HASHSIZEREBB);
}

CDB_REBB::CDB_REBB(CDB_REBB& src)
{
	*this = src;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebb.InitHashTable(HASHSIZEREBB);
}

CDB_REBB::~CDB_REBB()
{
	DelAllList();
}

CDB_REBB& CDB_REBB::operator= (CDB_REBB& src)
{
	POSITION pos;
	
	T_REBB_K KeyRebb;
	T_REBB_D DataRebb;
	m_rebb.RemoveAll();
	pos=src.m_rebb.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rebb.GetNextAssoc(pos,KeyRebb.keymap,DataRebb);
		m_rebb.SetAt(KeyRebb.keymap, DataRebb);
	}
	
	DelAllList();
	RebbList* pList;
	pos=src.m_rebblist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rebblist.GetNextAssoc(pos,KeyRebb.keymap,pList);
		RebbList* pListNew = new RebbList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rebblist.SetAt(KeyRebb.keymap, pListNew);
	}
	
	return *this;
}

void CDB_REBB::Add(T_REBB_K Key,T_REBB_D& rData,CDB_SECT* pSect)
{
	T_REBB_D Data;
	BOOL bExist=m_rebb.Lookup(Key.keymap, Data);
	m_rebb.SetAt(Key.keymap,rData);
	if(!bExist)pSect->AddListItem(Key.key.sectK, LT_REBB_CMD, Key.keymap);
}

BOOL CDB_REBB::Del(T_REBB_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rebb.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key.key.sectK, LT_REBB_CMD, Key.keymap));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_REBB::Get(T_REBB_K Key,T_REBB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rebb.Lookup(Key.keymap,rData);
}

int CDB_REBB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebb.GetCount();
}

POSITION CDB_REBB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebb.GetStartPosition();
}

void CDB_REBB::GetNext(POSITION& rNextPosition,T_REBB_K& rKey,T_REBB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rebb.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

void CDB_REBB::AddList(T_REBB_KEY Key)
{
	RebbList* pList = new RebbList;
	m_rebblist.SetAt(Key, pList);
}

BOOL CDB_REBB::DelList(T_REBB_KEY Key)
{
	RebbList* pList;
	BOOL bExist=m_rebblist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_rebblist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_REBB::DelAllList()
{
	T_REBB_KEY Key;
	RebbList* pList;
	POSITION pos=m_rebblist.GetStartPosition();
	while(pos != NULL)
	{
		m_rebblist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_rebblist.RemoveAll();
}

void CDB_REBB::AddListItem(T_REBB_K Key, int nCmd, int nKey)
{
	RebbList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_REBB::DelListItem(T_REBB_K Key, int nCmd, int nKey)
{
	RebbList* pList;
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