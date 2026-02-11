#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ULCB.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ULCB::CDB_ULCB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ulcb.InitHashTable(HASHSIZEULCB);
}

CDB_ULCB::~CDB_ULCB()
{

}

void CDB_ULCB::Add(T_ULCB_K Key,T_ULCB_D& rData,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_ULCB_D Data;
	BOOL bExist=m_ulcb.Lookup(Key, Data);

	if(bExist && Data.ElemKey != rData.ElemKey)
		VERIFY(pElem->DelListItem(Data.ElemKey, LT_ULCB_CMD, Key));
	if(bExist && Data.LoadGrpKey != rData.LoadGrpKey)
		VERIFY(pLdgr->DelListItem(Data.LoadGrpKey, LT_ULCB_CMD, Key));
	
	m_ulcb.SetAt(Key,rData);

	if(bExist && Data.ElemKey != rData.ElemKey)
		pElem->AddListItem(rData.ElemKey, LT_ULCB_CMD, Key);
	if(bExist && Data.LoadGrpKey != rData.LoadGrpKey)
		pLdgr->AddListItem(rData.LoadGrpKey, LT_ULCB_CMD, Key);

	if(!bExist)
	{
		if(pElem) pElem->AddListItem(rData.ElemKey, LT_ULCB_CMD, Key);
		if(pLdgr) pLdgr->AddListItem(rData.LoadGrpKey, LT_ULCB_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_ULCB_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_ulcb.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_ULCB_CMD, Key, rData.LoadGrpKey);
	}
}

BOOL CDB_ULCB::Del(T_ULCB_K Key,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_ULCB_D Data;
	BOOL bExist=m_ulcb.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ulcb.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pLdgr) VERIFY(pLdgr->DelListItem(Data.LoadGrpKey, LT_ULCB_CMD, Key));
			if (pElem) VERIFY(pElem->DelListItem(Data.ElemKey, LT_ULCB_CMD, Key));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_ULCB_CMD, Key);
			}

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_ULCB_K key;
				T_ULCB_D data;
				if(m_ulcb.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_ulcb.GetStartPosition();
					while(pos != NULL)
					{
						m_ulcb.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_ULCB::Get(T_ULCB_K Key,T_ULCB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_ulcb->Lookup(Key, StagLdgrKey)) return FALSE;
	}
	return m_ulcb.Lookup(Key,rData);
}

int CDB_ULCB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ulcb->GetCount();
	}
	return m_ulcb.GetCount();
}

POSITION CDB_ULCB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ulcb->GetStartPosition();
	}
	return m_ulcb.GetStartPosition();
}

void CDB_ULCB::GetNext(POSITION& rNextPosition,T_ULCB_K& rKey,T_ULCB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_ulcb->GetNextAssoc(rNextPosition, rKey, StagLdgrKey);
		m_ulcb.Lookup(rKey, rData);
		return;
	}
	m_ulcb.GetNextAssoc(rNextPosition,rKey,rData);
}
