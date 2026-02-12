#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_STRD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STRD::CDB_STRD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_strd.InitHashTable(HASHSIZESTRD);
}

CDB_STRD::~CDB_STRD()
{
}

void CDB_STRD::Add(T_STRD_K Key,T_STRD_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_STRD_D Data;
	BOOL bExist=m_strd.Lookup(Key, Data);
	if(bExist)
	{
		if(Data.nFunctionType == 1 || Data.nFunctionType == 2)
		{
			if(pNode != NULL)VERIFY(pNode->DelListItem(Data.nEntity, LT_STRD_CMD, Key));
		}
		else if(Data.nFunctionType >= 3 && Data.nFunctionType <= 7)
		{
			if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_STRD_CMD, Key));
		}
		else if(Data.nFunctionType == 8)
		{
		}
		else
			ASSERT(FALSE);
	}
	m_strd.SetAt(Key,rData);
	if(rData.nFunctionType == 1 || rData.nFunctionType == 2)
	{
		if(pNode != NULL)pNode->AddListItem(rData.nEntity, LT_STRD_CMD, Key);
	}
	else if(rData.nFunctionType >= 3 && rData.nFunctionType <= 7)
	{
		if(pElem != NULL)pElem->AddListItem(rData.nEntity, LT_STRD_CMD, Key);
	}
	else if(rData.nFunctionType == 8)
	{
	}
	else
		ASSERT(FALSE);

	if(Key == m_nStartNum)
	{
		T_STRD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_STRD_CMD, Key);
	}
}

BOOL CDB_STRD::Del(T_STRD_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_STRD_D Data;
	m_strd.Lookup(Key, Data);
	BOOL ret=m_strd.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Data.nFunctionType == 1 || Data.nFunctionType == 2)
		{
			if(pNode != NULL)VERIFY(pNode->DelListItem(Data.nEntity, LT_STRD_CMD, Key));
		}
		else if(Data.nFunctionType >= 3 && Data.nFunctionType <= 7)
		{
			if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_STRD_CMD, Key));
		}
		else if(Data.nFunctionType == 8)
		{
		}
		else
			ASSERT(FALSE);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_STRD_K key;
			T_STRD_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_STRD_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STRD::Get(T_STRD_K Key,T_STRD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_strd->Lookup(Key, uTemp)) return FALSE;
	}
	return m_strd.Lookup(Key,rData);
}

int CDB_STRD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_strd->GetCount();
	}
	return m_strd.GetCount();
}

POSITION CDB_STRD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_strd->GetStartPosition();
	}
	return m_strd.GetStartPosition();
}

void CDB_STRD::GetNext(POSITION& rNextPosition,T_STRD_K& rKey,T_STRD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_strd->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_strd.Lookup(rKey, rData);
		return;
	}
	m_strd.GetNextAssoc(rNextPosition,rKey,rData);
}
