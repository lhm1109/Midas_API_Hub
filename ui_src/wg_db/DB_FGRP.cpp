#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_FGRP.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_FGRP::CDB_FGRP()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_fgrp.InitHashTable(HASHSIZEFGRP);
	m_fgrplist.InitHashTable(HASHSIZELISTFGRP);
}

CDB_FGRP::~CDB_FGRP()
{
	DelAllList();
}

void CDB_FGRP::Add(T_FGRP_K Key,T_FGRP_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	if(pNode==NULL || pElem==NULL) { ASSERT(0); return; }

	// 중복된 절점/요소 없앤다. (MxT Export시 문제 발생)
	if(rData.nFounType!=FOUN_MAT)
	{
		CArrayUtil::RemoveDup(rData.aKey, TRUE);
	}
	
	T_FGRP_D Data;
	BOOL bExist=m_fgrp.Lookup(Key, Data);
	m_fgrp.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_FGRP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_FGRP::Del(T_FGRP_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_FGRP_D Data;
	BOOL bExist=m_fgrp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fgrp.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_FGRP_K key;
				T_FGRP_D data;
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
	return bExist;
}

void CDB_FGRP::AddList(T_FGRP_K Key)
{
	FgrpList* pList = new FgrpList;
	m_fgrplist.SetAt(Key, pList);
}

BOOL CDB_FGRP::DelList(T_FGRP_K Key)
{
	FgrpList* pList;
	BOOL bExist=m_fgrplist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_fgrplist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_FGRP::DelAllList()
{
	T_FGRP_K Key;
	FgrpList* pList;
	POSITION pos=m_fgrplist.GetStartPosition();
	while(pos != NULL)
	{
		m_fgrplist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_fgrplist.RemoveAll();
}

void CDB_FGRP::AddListItem(T_FGRP_K Key, int nCmd, int nKey)
{
	FgrpList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_FGRP::DelListItem(T_FGRP_K Key, int nCmd, int nKey)
{
	FgrpList* pList;
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
