#include "stdafx.h"
#include "DB_VBEM.h"
#include "DB_SEVO.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_SEVO::CDB_SEVO()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sevo.InitHashTable(HASHSIZESEVO);
	m_sevolist.InitHashTable(HASHSIZESEVOITEM);
}

CDB_SEVO::~CDB_SEVO()
{
	DelAllList();
}

void CDB_SEVO::Add(T_SEVO_K Key,T_SEVO_D& rData, CDB_VBEM* pVbem)
{
	T_SEVO_D Data;
	BOOL bExist = m_sevo.Lookup(Key, Data);
	m_sevo.SetAt(Key, rData);
	if (!bExist && pVbem != NULL)pVbem->AddListItem(Key, LT_SEVO_CMD, Key);
}

BOOL CDB_SEVO::Del(T_SEVO_K Key, CDB_VBEM* pVbem)
{
	T_SEVO_D Data;
	BOOL bExist = m_sevo.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_sevo.RemoveKey(Key);
		ASSERT(ret);
		if (ret)
		{
			if (pVbem != NULL)VERIFY(pVbem->DelListItem(Key, LT_SEVO_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

void CDB_SEVO::AddList(T_SEVO_K Key)
{
	SecvList* pList = new SecvList;
	m_sevolist.SetAt(Key, pList);
}

BOOL CDB_SEVO::DelList(T_SEVO_K Key)
{
	SecvList* pList;
	BOOL bExist=m_sevolist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_sevolist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_SEVO::DelAllList()
{
	T_SEVO_K Key;
	SecvList* pList;
	POSITION pos=m_sevolist.GetStartPosition();
	while(pos != NULL)
	{
		m_sevolist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_sevolist.RemoveAll();
}

void CDB_SEVO::AddListItem(T_SEVO_K Key, int nCmd, int nKey)
{
	SecvList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_SEVO::DelListItem(T_SEVO_K Key, int nCmd, int nKey)
{
	SecvList* pList;
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
