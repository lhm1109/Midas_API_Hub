#include "stdafx.h"
#include "DB_RBST.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RBST::CDB_RBST()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbst.InitHashTable(HASHSIZERBST);
}

CDB_RBST::CDB_RBST(CDB_RBST& src)
{
	*this = src;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbst.InitHashTable(HASHSIZERBST);
}

CDB_RBST::~CDB_RBST()
{
	DelAllList();
}

CDB_RBST& CDB_RBST::operator= (CDB_RBST& src)
{
	POSITION pos;
	
	T_RBST_K KeyRbst;
	T_RBST_D DataRbst;
	m_rbst.RemoveAll();
	pos=src.m_rbst.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rbst.GetNextAssoc(pos,KeyRbst,DataRbst);
		m_rbst.SetAt(KeyRbst, DataRbst);
	}
	
	DelAllList();
	RbstList* pList;
	pos=src.m_rbstlist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rbstlist.GetNextAssoc(pos,KeyRbst,pList);
		RbstList* pListNew = new RbstList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rbstlist.SetAt(KeyRbst, pListNew);
	}
	
	return *this;
}

void CDB_RBST::Add(T_RBST_K Key,T_RBST_D& rData)
{
	m_rbst.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_RBST_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_RBST::Del(T_RBST_K Key)
{
	BOOL ret=m_rbst.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_RBST_K key;
			T_RBST_D data;
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

//-------------------------------------------------------------------------
BOOL CDB_RBST::Get(T_RBST_K Key,T_RBST_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rbst.Lookup(Key,rData);
}

int CDB_RBST::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbst.GetCount();
}

POSITION CDB_RBST::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbst.GetStartPosition();
}

void CDB_RBST::GetNext(POSITION& rNextPosition,T_RBST_K& rKey,T_RBST_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rbst.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_RBST::AddList(T_RBST_K Key)
{
	RbstList* pList = new RbstList;
	m_rbstlist.SetAt(Key, pList);
}

BOOL CDB_RBST::DelList(T_RBST_K Key)
{
	RbstList* pList;
	BOOL bExist=m_rbstlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_rbstlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_RBST::DelAllList()
{
	T_RBST_K Key;
	RbstList* pList;
	POSITION pos=m_rbstlist.GetStartPosition();
	while(pos != NULL)
	{
		m_rbstlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_rbstlist.RemoveAll();
}

void CDB_RBST::AddListItem(T_RBST_K Key, int nCmd, int nKey)
{
	RbstList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_RBST::DelListItem(T_RBST_K Key, int nCmd, int nKey)
{
	RbstList* pList;
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