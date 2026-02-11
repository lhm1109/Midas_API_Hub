#include "stdafx.h"
#include "DB_EARE.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_EARE::CDB_EARE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_eare.InitHashTable(HASHSIZEEARE);
	m_nStartNum = 1;
	m_nLastNum  = 0;

}

CDB_EARE::~CDB_EARE()
{

}

void CDB_EARE::Add(T_EARE_K Key,T_EARE_D& rData,CDB_ELEM* pElem, CDB_NODE* pNode)
{
	T_EARE_D Data;
	BOOL bExist=m_eare.Lookup(Key, Data);
	if(bExist)
	{
		if (pElem) pElem->DelListItem(Data.ElemK, LT_EARE_CMD, Key);
		if (pNode) pNode->DelListItem(Data.NodeK, LT_EARE_CMD, Key);

	}

	m_eare.SetAt(Key,rData);
	if (pElem) pElem->AddListItem(rData.ElemK, LT_EARE_CMD, Key);
	if (pNode) pNode->AddListItem(rData.NodeK, LT_EARE_CMD, Key);

	if(Key == m_nStartNum)
	{
		T_EARE_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_eare.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_EARE::Del(T_EARE_K Key,CDB_ELEM* pElem,CDB_NODE* pNode)
{
	T_EARE_D Data;
	BOOL bExist=m_eare.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_eare.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)VERIFY(pElem->DelListItem(Data.ElemK, LT_EARE_CMD, Key));
			if (pNode)VERIFY(pNode->DelListItem(Data.NodeK, LT_EARE_CMD, Key));
		}
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_EARE_K key;
			T_EARE_D data;
			if(m_eare.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_eare.GetStartPosition();
				while(pos != NULL)
				{
					m_eare.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_EARE::Get(T_EARE_K Key,T_EARE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_eare->Lookup(Key, StagBngrKey)) return FALSE;
	}
	return m_eare.Lookup(Key,rData);
}

int CDB_EARE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_eare->GetCount();
	}
	return m_eare.GetCount();
}

POSITION CDB_EARE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_eare->GetStartPosition();
	}
	return m_eare.GetStartPosition();
}

void CDB_EARE::GetNext(POSITION& rNextPosition,T_EARE_K& rKey,T_EARE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_eare->GetNextAssoc(rNextPosition, rKey, StagBngrKey);
		m_eare.Lookup(rKey, rData);
		return;
	}
	m_eare.GetNextAssoc(rNextPosition,rKey,rData);
}

