#include "stdafx.h"
#include "DB_RBMW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RBMW::CDB_RBMW()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbmw.InitHashTable(HASHSIZERBMW);
}

CDB_RBMW::CDB_RBMW(CDB_RBMW& src)
{
	*this = src;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbmw.InitHashTable(HASHSIZERBMW);
}

CDB_RBMW::~CDB_RBMW()
{
	DelAllList();
}

CDB_RBMW& CDB_RBMW::operator= (CDB_RBMW& src)
{
	POSITION pos;
	
	T_RBMW_K KeyRbmw;
	T_RBMW_D DataRbmw;
	m_rbmw.RemoveAll();
	pos=src.m_rbmw.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rbmw.GetNextAssoc(pos,KeyRbmw,DataRbmw);
		m_rbmw.SetAt(KeyRbmw, DataRbmw);
	}
	
	DelAllList();
	RbmwList* pList;
	pos=src.m_rbmwlist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rbmwlist.GetNextAssoc(pos,KeyRbmw,pList);
		RbmwList* pListNew = new RbmwList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rbmwlist.SetAt(KeyRbmw, pListNew);
	}
	
	return *this;
}

void CDB_RBMW::Add(T_RBMW_K Key,T_RBMW_D& rData)
{
	m_rbmw.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_RBMW_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_RBMW::Del(T_RBMW_K Key)
{
	BOOL ret=m_rbmw.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_RBMW_K key;
			T_RBMW_D data;
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
BOOL CDB_RBMW::Get(T_RBMW_K Key,T_RBMW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rbmw.Lookup(Key,rData);
}

int CDB_RBMW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbmw.GetCount();
}

POSITION CDB_RBMW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbmw.GetStartPosition();
}

void CDB_RBMW::GetNext(POSITION& rNextPosition,T_RBMW_K& rKey,T_RBMW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rbmw.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_RBMW::AddList(T_RBMW_K Key)
{
	RbmwList* pList = new RbmwList;
	m_rbmwlist.SetAt(Key, pList);
}

BOOL CDB_RBMW::DelList(T_RBMW_K Key)
{
	RbmwList* pList;
	BOOL bExist=m_rbmwlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_rbmwlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_RBMW::DelAllList()
{
	T_RBMW_K Key;
	RbmwList* pList;
	POSITION pos=m_rbmwlist.GetStartPosition();
	while(pos != NULL)
	{
		m_rbmwlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_rbmwlist.RemoveAll();
}

void CDB_RBMW::AddListItem(T_RBMW_K Key, int nCmd, int nKey)
{
	RbmwList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_RBMW::DelListItem(T_RBMW_K Key, int nCmd, int nKey)
{
	RbmwList* pList;
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