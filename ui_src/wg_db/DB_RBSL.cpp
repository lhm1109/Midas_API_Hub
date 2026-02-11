#include "stdafx.h"
#include "DB_RBSL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RBSL::CDB_RBSL()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbsl.InitHashTable(HASHSIZERBSL);
}

CDB_RBSL::CDB_RBSL(CDB_RBSL& src)
{
	*this = src;
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbsl.InitHashTable(HASHSIZERBSL);
}

CDB_RBSL::~CDB_RBSL()
{
	DelAllList();
}

CDB_RBSL& CDB_RBSL::operator= (CDB_RBSL& src)
{
	POSITION pos;
	
	T_RBSL_K KeyRbsl;
	T_RBSL_D DataRbsl;
	m_rbsl.RemoveAll();
	pos=src.m_rbsl.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rbsl.GetNextAssoc(pos,KeyRbsl,DataRbsl);
		m_rbsl.SetAt(KeyRbsl, DataRbsl);
	}
	
	DelAllList();
	RbslList* pList;
	pos=src.m_rbsllist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rbsllist.GetNextAssoc(pos,KeyRbsl,pList);
		RbslList* pListNew = new RbslList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rbsllist.SetAt(KeyRbsl, pListNew);
	}
	
	return *this;
}

void CDB_RBSL::Add(T_RBSL_K Key,T_RBSL_D& rData)
{
	m_rbsl.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_RBSL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_RBSL::Del(T_RBSL_K Key)
{
	BOOL ret=m_rbsl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_RBSL_K key;
			T_RBSL_D data;
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
BOOL CDB_RBSL::Get(T_RBSL_K Key,T_RBSL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rbsl.Lookup(Key,rData);
}

int CDB_RBSL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbsl.GetCount();
}

POSITION CDB_RBSL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbsl.GetStartPosition();
}

void CDB_RBSL::GetNext(POSITION& rNextPosition,T_RBSL_K& rKey,T_RBSL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rbsl.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_RBSL::AddList(T_RBSL_K Key)
{
	RbslList* pList = new RbslList;
	m_rbsllist.SetAt(Key, pList);
}

BOOL CDB_RBSL::DelList(T_RBSL_K Key)
{
	RbslList* pList;
	BOOL bExist=m_rbsllist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_rbsllist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_RBSL::DelAllList()
{
	T_RBSL_K Key;
	RbslList* pList;
	POSITION pos=m_rbsllist.GetStartPosition();
	while(pos != NULL)
	{
		m_rbsllist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_rbsllist.RemoveAll();
}

void CDB_RBSL::AddListItem(T_RBSL_K Key, int nCmd, int nKey)
{
	RbslList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_RBSL::DelListItem(T_RBSL_K Key, int nCmd, int nKey)
{
	RbslList* pList;
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