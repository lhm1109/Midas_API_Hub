#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_BCVT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BCVT::CDB_BCVT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_bcvt.InitHashTable(HASHSIZEBCVT);
}

CDB_BCVT::~CDB_BCVT()
{
}

void CDB_BCVT::Add(T_BCVT_K Key,T_BCVT_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_BCVT_D Data;
	BOOL bExist = m_bcvt.Lookup(Key, Data);
	if (rData.nType == 0 || rData.nType == 1) // 새것이 Node
	{
		CArray<UINT, UINT> aKey;
		if (Data.nType == 0 || Data.nType == 1)
		{
			aKey.Copy(Data.arNodeElemKey);  // 이전 것이 Node
		}
		else
		{
			if (pElem) pElem->DelListItem(Data.arNodeElemKey, LT_BCVT_CMD, Key);
		}
		if (pNode) pNode->AddListItem(rData.arNodeElemKey, aKey, LT_BCVT_CMD, Key, bExist);
	}
	else if (rData.nType == 2 || rData.nType == 3 || rData.nType == 4 || rData.nType == 5) //   // 새것이 Elem
	{
		CArray<UINT, UINT> aKey;
		if (rData.nType == 2 || rData.nType == 3 || rData.nType == 4 || rData.nType == 5)
		{
			aKey.Copy(Data.arNodeElemKey); // 이전 것이 Elem
		}			
		else
		{
			if (pNode) pNode->DelListItem(Data.arNodeElemKey, LT_BCVT_CMD, Key);
		}
		if (pElem) pElem->AddListItem(rData.arNodeElemKey, aKey, LT_BCVT_CMD, Key, bExist);
	}
	m_bcvt.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_BCVT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
	// Final Stage에서 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_BCVT_CMD, Key);
	}
}

BOOL CDB_BCVT::Del(T_BCVT_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_BCVT_D Data;
	m_bcvt.Lookup(Key, Data);
	BOOL ret=m_bcvt.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (Data.nType == 0 || Data.nType == 1)
		{
			if (pNode)
			{
				for (int i = 0; i < Data.arNodeElemKey.GetSize(); i++)
					VERIFY(pNode->DelListItem(Data.arNodeElemKey[i], LT_BCVT_CMD, Key));
			}
		}
		else if (Data.nType == 2 || Data.nType == 3 || Data.nType == 4 || Data.nType == 5)
		{
			if (pElem)
			{
				for (int i = 0; i < Data.arNodeElemKey.GetSize(); i++)
					VERIFY(pElem->DelListItem(Data.arNodeElemKey[i], LT_BCVT_CMD, Key));
			}
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_BCVT_K key;
			T_BCVT_D data;
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
		// Final Stage에서 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_ULFC_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_BCVT::Get(T_BCVT_K Key,T_BCVT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_bcvt->Lookup(Key, uTemp)) return FALSE;
	}
	return m_bcvt.Lookup(Key,rData);
}

int CDB_BCVT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_bcvt->GetCount();
	}
	return m_bcvt.GetCount();
}

POSITION CDB_BCVT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_bcvt->GetStartPosition();
	}
	return m_bcvt.GetStartPosition();
}

void CDB_BCVT::GetNext(POSITION& rNextPosition,T_BCVT_K& rKey,T_BCVT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_bcvt->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_bcvt.Lookup(rKey, rData);
		return;
	}
	m_bcvt.GetNextAssoc(rNextPosition,rKey,rData);
}
