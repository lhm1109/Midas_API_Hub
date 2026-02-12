#include "stdafx.h"
#include "DB_HHND.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HHND::CDB_HHND()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_hhnd.InitHashTable(HASHSIZEHHND);
}

CDB_HHND::~CDB_HHND()
{
}

void CDB_HHND::Add(T_HHND_K Key,T_HHND_D& rData, CDB_NODE* pNode, CDB_ELEM* pElem)
{
	T_HHND_D Data;
	BOOL bExist = m_hhnd.Lookup(Key, Data);
	m_hhnd.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_HHND_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// nType == 0 인 경우는 Node만 쓰이고, nType == 1인 경우는 Node, Element 모두 쓰인다.
	if (bExist)
	{
		if (pNode) pNode->DelListItem(Data.nNodeKey, LT_HHND_CMD, Key);
		if (pElem && Data.nType != 0) pElem->AddListItem(Data.nElemKey, LT_HHND_CMD, Key);
	}
	if (pNode) pNode->AddListItem(rData.nNodeKey, LT_HHND_CMD, Key);
	if (pElem && rData.nType != 0) pElem->AddListItem(rData.nElemKey, LT_HHND_CMD, Key);

	// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_HHND_CMD, Key);
	}
}

BOOL CDB_HHND::Del(T_HHND_K Key, CDB_NODE* pNode, CDB_ELEM* pElem)
{
	T_HHND_D Data;
	m_hhnd.Lookup(Key, Data);
	BOOL ret=m_hhnd.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Data.nType == 0)  // Node
		{
			if(pNode != NULL)VERIFY(pNode->DelListItem(Data.nNodeKey, LT_HHND_CMD, Key));
		}
		else  // Elem + Node
		{
			if(pNode != NULL)VERIFY(pNode->DelListItem(Data.nNodeKey, LT_HHND_CMD, Key));
			if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nElemKey, LT_HHND_CMD, Key));
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_HHND_K key;
			T_HHND_D data;
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
		// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_HHND_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
/* stage mode에서도 모두 active하고 Delete, Modify 지원.
	 stage mode에서 Node List 관리는 active된 Node인 경우만 처리
BOOL CDB_HHND::Get(T_HHND_K Key,T_HHND_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_hhnd->Lookup(Key, uTemp)) return FALSE;
	}
	return m_hhnd.Lookup(Key,rData);
}

int CDB_HHND::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_hhnd->GetCount();
	}
	return m_hhnd.GetCount();
}

POSITION CDB_HHND::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_hhnd->GetStartPosition();
	}
	return m_hhnd.GetStartPosition();
}

void CDB_HHND::GetNext(POSITION& rNextPosition,T_HHND_K& rKey,T_HHND_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_hhnd->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_hhnd.Lookup(rKey, rData);
		return;
	}
	m_hhnd.GetNextAssoc(rNextPosition,rKey,rData);
}
*/