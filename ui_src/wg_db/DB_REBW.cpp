#include "stdafx.h"
#include "DB_STOR.h"
#include "DB_REBW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_REBW::CDB_REBW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebw.InitHashTable(HASHSIZEREBW);
}

CDB_REBW::CDB_REBW(CDB_REBW& src)
{
	*this = src;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebw.InitHashTable(HASHSIZEREBW);
}

CDB_REBW::~CDB_REBW()
{
	DelAllList();
}

CDB_REBW& CDB_REBW::operator= (CDB_REBW& src)
{
	POSITION pos;
	
	T_REBW_K KeyRebw;
	T_REBW_D DataRebw;
	m_rebw.RemoveAll();
	pos=src.m_rebw.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rebw.GetNextAssoc(pos,KeyRebw.keymap,DataRebw);
		m_rebw.SetAt(KeyRebw.keymap, DataRebw);
	}
	
	DelAllList();
	RebwList* pList;
	pos=src.m_rebwlist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rebwlist.GetNextAssoc(pos,KeyRebw.keymap,pList);
		RebwList* pListNew = new RebwList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rebwlist.SetAt(KeyRebw.keymap, pListNew);
	}
	
	return *this;
}

void CDB_REBW::Add(T_REBW_K Key,T_REBW_D& rData,CDB_STOR* pStor)
{
	T_REBW_D Data;
	BOOL bExist=m_rebw.Lookup(Key.keymap, Data);
	m_rebw.SetAt(Key.keymap,rData);
	if(!bExist)pStor->AddListItem(Key.key.storid, LT_REBW_CMD, Key.keymap);
}

BOOL CDB_REBW::Del(T_REBW_K Key,CDB_STOR* pStor)
{
	T_REBW_D Data;
	BOOL bExist=m_rebw.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_rebw.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)VERIFY(pStor->DelListItem(Key.key.storid, LT_REBW_CMD, Key.keymap));
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_REBW::Get(T_REBW_K Key,T_REBW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rebw.Lookup(Key.keymap,rData);
}

int CDB_REBW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebw.GetCount();
}

POSITION CDB_REBW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebw.GetStartPosition();
}

void CDB_REBW::GetNext(POSITION& rNextPosition,T_REBW_K& rKey,T_REBW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rebw.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

void CDB_REBW::AddList(T_REBW_KEY Key)
{
	RebwList* pList = new RebwList;
	m_rebwlist.SetAt(Key, pList);
}

BOOL CDB_REBW::DelList(T_REBW_KEY Key)
{
	RebwList* pList;
	BOOL bExist=m_rebwlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_rebwlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_REBW::DelAllList()
{
	T_REBW_KEY Key;
	RebwList* pList;
	POSITION pos=m_rebwlist.GetStartPosition();
	while(pos != NULL)
	{
		m_rebwlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_rebwlist.RemoveAll();
}

void CDB_REBW::AddListItem(T_REBW_K Key, int nCmd, int nKey)
{
	RebwList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_REBW::DelListItem(T_REBW_K Key, int nCmd, int nKey)
{
	RebwList* pList;
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