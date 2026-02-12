#include "stdafx.h"
#include "DB_FSSF.h"
#include "DB_ELEM.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FSSF::CDB_FSSF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_fssf.InitHashTable(HASHSIZEFSSF);
	m_fssflist.InitHashTable(HASHSIZEFSSF);
}

CDB_FSSF::~CDB_FSSF()
{
	DelAllList();
}

void CDB_FSSF::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_fssf.RemoveAll();
	m_fssflist.RemoveAll();
}

void CDB_FSSF::Add(T_FSSF_K Key,T_FSSF_D& rData,CDB_ELEM* pElem)
{
	T_FSSF_D Data;
	BOOL bExist=m_fssf.Lookup(Key, Data);
	m_fssf.SetAt(Key,rData);
	if(!bExist)
		pElem->AddListItem(Key, LT_FSSF_CMD, Key);
}

BOOL CDB_FSSF::Del(T_FSSF_K Key,CDB_ELEM* pElem)
{
	T_FSSF_D Data;
	BOOL bExist=m_fssf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fssf.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pElem->DelListItem(Key, LT_FSSF_CMD, Key));
		}
		return ret;
	}

	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_FSSF::Get(T_FSSF_K Key,T_FSSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_fssf.Lookup(Key,rData);
}

int CDB_FSSF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fssf.GetCount();
}

POSITION CDB_FSSF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fssf.GetStartPosition();
}

void CDB_FSSF::GetNext(POSITION& rNextPosition,T_FSSF_K& rKey,T_FSSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_fssf.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_FSSF::AddList(T_FSSF_K Key)
{
	FssfList* pList = new FssfList;
	m_fssflist.SetAt(Key, pList);
}

BOOL CDB_FSSF::DelList(T_FSSF_K Key)
{
	FipaList* pList;
	BOOL bExist=m_fssflist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_fssflist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_FSSF::DelAllList()
{
	T_FSSF_K Key;
	FssfList* pList;
	POSITION pos=m_fssflist.GetStartPosition();
	while(pos != NULL)
	{
		m_fssflist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_fssflist.RemoveAll();
}

void CDB_FSSF::AddListItem(T_FSSF_K Key, int nCmd, int nKey)
{
	FssfList* pList;
	VERIFY(m_fssflist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_FSSF::DelListItem(T_FSSF_K Key, int nCmd, int nKey)
{
	FssfList* pList;
	VERIFY(m_fssflist.Lookup(Key, pList));

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