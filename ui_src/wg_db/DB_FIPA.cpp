#include "stdafx.h"
#include "DB_FIPA.h"
#include "DB_ELEM.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FIPA::CDB_FIPA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_fipa.InitHashTable(HASHSIZEFIPA);
	m_fipalist.InitHashTable(HASHSIZEFIPA);
}

CDB_FIPA::~CDB_FIPA()
{
	DelAllList();
}

void CDB_FIPA::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_fipa.RemoveAll();
	m_fipalist.RemoveAll();
}

void CDB_FIPA::Add(T_FIPA_K Key,T_FIPA_D& rData,CDB_ELEM* pElem)
{
	T_FIPA_D Data;
	BOOL bExist=m_fipa.Lookup(Key, Data);
	m_fipa.SetAt(Key,rData);
	if(!bExist)
		pElem->AddListItem(Key, LT_FIPA_CMD, Key);
}

BOOL CDB_FIPA::Del(T_FIPA_K Key,CDB_ELEM* pElem)
{
	T_FIPA_D Data;
	BOOL bExist=m_fipa.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fipa.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pElem->DelListItem(Key, LT_FIPA_CMD, Key));
		}
		return ret;
	}

	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_FIPA::Get(T_FIPA_K Key,T_FIPA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_fipa.Lookup(Key,rData);
}

int CDB_FIPA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fipa.GetCount();
}

POSITION CDB_FIPA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fipa.GetStartPosition();
}

void CDB_FIPA::GetNext(POSITION& rNextPosition,T_FIPA_K& rKey,T_FIPA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_fipa.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_FIPA::AddList(T_FIPA_K Key)
{
	FipaList* pList = new FipaList;
	m_fipalist.SetAt(Key, pList);
}

BOOL CDB_FIPA::DelList(T_FIPA_K Key)
{
	FipaList* pList;
	BOOL bExist=m_fipalist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_fipalist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_FIPA::DelAllList()
{
	T_FIPA_K Key;
	FipaList* pList;
	POSITION pos=m_fipalist.GetStartPosition();
	while(pos != NULL)
	{
		m_fipalist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_fipalist.RemoveAll();
}

void CDB_FIPA::AddListItem(T_FIPA_K Key, int nCmd, int nKey)
{
	FipaList* pList;
	VERIFY(m_fipalist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_FIPA::DelListItem(T_FIPA_K Key, int nCmd, int nKey)
{
	ElemList* pList;
	VERIFY(m_fipalist.Lookup(Key, pList));

	BOOL bDeleted = FALSE;
	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
			break;
		}
	}
	if (!bDeleted) return FALSE;
	return TRUE;
}