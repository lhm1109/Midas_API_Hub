#include "stdafx.h"
#include "DB_RBMS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RBMS::CDB_RBMS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbms.InitHashTable(HASHSIZERBMS);
}

CDB_RBMS::CDB_RBMS(CDB_RBMS& src)
{
	*this = src;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbms.InitHashTable(HASHSIZERBMS);
}

CDB_RBMS::~CDB_RBMS()
{
	DelAllList();
}

CDB_RBMS& CDB_RBMS::operator= (CDB_RBMS& src)
{
	POSITION pos;
	
	T_RBMS_K KeyRbms;
	T_RBMS_D DataRbms;
	m_rbms.RemoveAll();
	pos=src.m_rbms.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rbms.GetNextAssoc(pos,KeyRbms,DataRbms);
		m_rbms.SetAt(KeyRbms, DataRbms);
	}
	
	DelAllList();
	RbmsList* pList;
	pos=src.m_rbmslist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rbmslist.GetNextAssoc(pos,KeyRbms,pList);
		RbmsList* pListNew = new RbmsList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rbmslist.SetAt(KeyRbms, pListNew);
	}
	
	return *this;
}

void CDB_RBMS::Add(T_RBMS_K Key,T_RBMS_D& rData)
{
	m_rbms.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_RBMS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_RBMS::Del(T_RBMS_K Key)
{
	BOOL ret=m_rbms.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_RBMS_K key;
			T_RBMS_D data;
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
BOOL CDB_RBMS::Get(T_RBMS_K Key,T_RBMS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rbms.Lookup(Key,rData);
}

int CDB_RBMS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbms.GetCount();
}

POSITION CDB_RBMS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbms.GetStartPosition();
}

void CDB_RBMS::GetNext(POSITION& rNextPosition,T_RBMS_K& rKey,T_RBMS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rbms.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_RBMS::AddList(T_RBMS_K Key)
{
	RbmsList* pList = new RbmsList;
	m_rbmslist.SetAt(Key, pList);
}

BOOL CDB_RBMS::DelList(T_RBMS_K Key)
{
	RbmsList* pList;
	BOOL bExist=m_rbmslist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_rbmslist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_RBMS::DelAllList()
{
	T_RBMS_K Key;
	RbmsList* pList;
	POSITION pos=m_rbmslist.GetStartPosition();
	while(pos != NULL)
	{
		m_rbmslist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_rbmslist.RemoveAll();
}

void CDB_RBMS::AddListItem(T_RBMS_K Key, int nCmd, int nKey)
{
	RbmsList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_RBMS::DelListItem(T_RBMS_K Key, int nCmd, int nKey)
{
	RbmsList* pList;
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