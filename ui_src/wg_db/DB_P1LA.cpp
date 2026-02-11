#include "stdafx.h"
#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_P1LT.h"
#include "DB_P1LA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_P1LA::CDB_P1LA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_p1la.InitHashTable(HASHSIZEP1LA);
	m_index.Initialize(HASHSIZEP1LA);

	m_nStartNum=1;
	m_nLastNum=0;
}

CDB_P1LA::~CDB_P1LA()
{

}

void CDB_P1LA::Add(T_P1LA_K Key,T_P1LA_D& rData,CDB_P1LT* pP1lt,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_P1LA_D Data;
	BOOL bExist=m_p1la.Lookup(Key, Data);
	m_p1la.SetAt(Key,rData);
	if (bExist)
	{
		m_index.Del(T_UINT12UINT20_INDEX_K(Data.nLoadItem, Data.NodeElemK));
		pP1lt->DelListItem(Data.TypeKey, LT_P1LA_CMD, Key);
		if (Data.nLoadItem == 0 || Data.nLoadItem == 1)
			pNode->DelListItem(Data.NodeElemK, LT_P1LA_CMD, Key);
		else if (Data.nLoadItem == 2)
			pElem->DelListItem(Data.NodeElemK, LT_P1LA_CMD, Key);
		else ASSERT(0);
	}
	pP1lt->AddListItem(rData.TypeKey, LT_P1LA_CMD, Key);
	if (rData.nLoadItem == 0 || rData.nLoadItem == 1) 
		pNode->AddListItem(rData.NodeElemK, LT_P1LA_CMD, Key);
	else if (rData.nLoadItem == 2)
		pElem->AddListItem(rData.NodeElemK, LT_P1LA_CMD, Key);
	else ASSERT(0);
	m_index.Add(T_UINT12UINT20_INDEX_K(rData.nLoadItem, rData.NodeElemK), Key);

	if(Key == m_nStartNum)
	{
		T_P1LA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_P1LA_CMD, Key);
	}
}

BOOL CDB_P1LA::Del(T_P1LA_K Key,CDB_P1LT* pP1lt,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_P1LA_D Data;
	m_p1la.Lookup(Key, Data);
	BOOL ret=m_p1la.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		pP1lt->DelListItem(Data.TypeKey, LT_P1LA_CMD, Key);
		if (Data.nLoadItem == 0 || Data.nLoadItem == 1)
			VERIFY(pNode->DelListItem(Data.NodeElemK, LT_P1LA_CMD, Key));
		else if (Data.nLoadItem == 2)
			VERIFY(pElem->DelListItem(Data.NodeElemK, LT_P1LA_CMD, Key));
		else ASSERT(0);
		m_index.Del(T_UINT12UINT20_INDEX_K(Data.nLoadItem, Data.NodeElemK));

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_P1LA_K key;
			T_P1LA_D data;
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

		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_P1LA_CMD, Key);
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_P1LA::Get(T_P1LA_K Key,T_P1LA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_p1la->Lookup(Key, uTemp)) return FALSE;
	}
	return m_p1la.Lookup(Key,rData);
}

int CDB_P1LA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_p1la->GetCount();
	}
	return m_p1la.GetCount();
}

POSITION CDB_P1LA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_p1la->GetStartPosition();
	}
	return m_p1la.GetStartPosition();
}

void CDB_P1LA::GetNext(POSITION& rNextPosition,T_P1LA_K& rKey,T_P1LA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_p1la->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_p1la.Lookup(rKey, rData);
		return;
	}
	m_p1la.GetNextAssoc(rNextPosition,rKey,rData);
}
