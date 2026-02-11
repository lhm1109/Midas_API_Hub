#include "stdafx.h"
#include "DB_VBEM.h"
#include "DB_SECV.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_SECV::CDB_SECV()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_secv.InitHashTable(HASHSIZESECV);
	m_secvlist.InitHashTable(HASHSIZESECVITEM);
}

CDB_SECV::~CDB_SECV()
{
	DelAllList();
}

void CDB_SECV::Add(T_SECV_K Key,T_SECV_D& rData, CDB_VBEM* pVbem)
{
	T_SECV_D Data;
	BOOL bExist = m_secv.Lookup(Key, Data);
	m_secv.SetAt(Key, rData);
	if (!bExist && pVbem != NULL)pVbem->AddListItem(Key, LT_SECV_CMD, Key);
}

BOOL CDB_SECV::Del(T_SECV_K Key, CDB_VBEM* pVbem)
{
	T_SECV_D Data;
	BOOL bExist = m_secv.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_secv.RemoveKey(Key);
		ASSERT(ret);
		if (ret)
		{
			if (pVbem != NULL)VERIFY(pVbem->DelListItem(Key, LT_SECV_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

void CDB_SECV::AddList(T_SECV_K Key)
{
	SecvList* pList = new SecvList;
	m_secvlist.SetAt(Key, pList);
}

BOOL CDB_SECV::DelList(T_SECV_K Key)
{
	SecvList* pList;
	BOOL bExist=m_secvlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_secvlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_SECV::DelAllList()
{
	T_SECV_K Key;
	SecvList* pList;
	POSITION pos=m_secvlist.GetStartPosition();
	while(pos != NULL)
	{
		m_secvlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_secvlist.RemoveAll();
}

void CDB_SECV::AddListItem(T_SECV_K Key, int nCmd, int nKey)
{
	SecvList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_SECV::DelListItem(T_SECV_K Key, int nCmd, int nKey)
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
