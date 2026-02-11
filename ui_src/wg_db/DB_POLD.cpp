#include "stdafx.h"
#include "DB_POLD.h"
#include "DB_STLD.h"
#include "DB_NODE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POLD::CDB_POLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_pold.InitHashTable(HASHSIZEPOLD);
	m_poldlist.InitHashTable(HASHSIZELISTPOLD);
}

CDB_POLD::~CDB_POLD()
{
	DelAllList();
}

void CDB_POLD::Add(T_POLD_K Key,T_POLD_D& rData, CDB_STLD* pStld, CDB_NODE* pNode)
{
	T_POLD_D Data;
	BOOL bExist = m_pold.Lookup(Key, Data);
	/* List에 추가하지 않는다. Stld 삭제시 Lcom처럼 Loop 돌며 처리한다.
	if (bExist && Data.nLoadPatternType == 0 && pStld)
	{
		for (int i = 0; i < Data.aLoadPattern.GetSize(); i++)
			pStld->DelListItem(Data.aLoadPattern[i].LoadCaseKey, LT_POLD_CMD, Key);
	}
	*/
	if (bExist && Data.nControlOption == 2 && pNode)
	{
		pNode->DelListItem(Data.MasterNode, LT_POLD_CMD, Key);
	}
	m_pold.SetAt(Key,rData);
	/*
	if (rData.nLoadPatternType == 0 && pStld)
	{
		for (int i = 0; i < rData.aLoadPattern.GetSize(); i++)
			pStld->AddListItem(rData.aLoadPattern[i].LoadCaseKey, LT_POLD_CMD, Key);
	}
	*/
	if (rData.nControlOption == 2 && pNode)
	{
		pNode->AddListItem(rData.MasterNode, LT_POLD_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_POLD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_POLD::Del(T_POLD_K Key, CDB_STLD* pStld, CDB_NODE* pNode)
{
	T_POLD_D Data;
	m_pold.Lookup(Key, Data);
	BOOL ret=m_pold.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		/*  List에 추가하지 않는다. Stld 삭제시 Lcom처럼 Loop 돌며 처리한다.
		if (Data.nLoadPatternType == 0 && pStld)
		{
			for (int i = 0; i < Data.aLoadPattern.GetSize(); i++)
				pStld->DelListItem(Data.aLoadPattern[i].LoadCaseKey, LT_POLD_CMD, Key);
		}
		*/
		if (Data.nControlOption == 2 && pNode)
		{
			pNode->DelListItem(Data.MasterNode, LT_POLD_CMD, Key);
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_POLD_K key;
			T_POLD_D data;
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

void CDB_POLD::AddList(T_POLD_K Key)
{
	PoldList* pList = new PoldList;
	m_poldlist.SetAt(Key, pList);
}

BOOL CDB_POLD::DelList(T_POLD_K Key)
{
	PoldList* pList;
	BOOL bExist=m_poldlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_poldlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_POLD::DelAllList()
{
	T_POLD_K Key;
	PoldList* pList;
	POSITION pos=m_poldlist.GetStartPosition();
	while(pos != NULL)
	{
		m_poldlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_poldlist.RemoveAll();
}

void CDB_POLD::AddListItem(T_POLD_K Key, int nCmd, int nKey)
{
	PoldList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_POLD::DelListItem(T_POLD_K Key, int nCmd, int nKey)
{
	PoldList* pList;
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
BOOL CDB_POLD::Get(T_POLD_K Key,T_POLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pold.Lookup(Key,rData);
}

int CDB_POLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pold.GetCount();
}

POSITION CDB_POLD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pold.GetStartPosition();
}

void CDB_POLD::GetNext(POSITION& rNextPosition,T_POLD_K& rKey,T_POLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_pold.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_POLD::GetList(T_POLD_K Key,PoldList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_poldlist.Lookup(Key,rpList);
}

int CDB_POLD::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_poldlist.GetCount();
}
