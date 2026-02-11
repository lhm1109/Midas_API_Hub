#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_STBK.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STBK::CDB_STBK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_stbk.InitHashTable(HASHSIZESTBK);
}

CDB_STBK::~CDB_STBK()
{

}

void CDB_STBK::Add(T_STBK_K Key, T_STBK_D& rData, CDB_NODE* pNode, CDB_LDGR* pLdgr)
{
	T_STBK_D Data;
	BOOL bExist=m_stbk.Lookup(Key, Data);

	if(bExist && Data.LoadGrpKey != rData.LoadGrpKey && pLdgr)
		VERIFY(pLdgr->DelListItem(Data.LoadGrpKey, LT_STBK_CMD, Key));

	m_stbk.SetAt(Key,rData);

	if(bExist && Data.LoadGrpKey != rData.LoadGrpKey && pLdgr)
		pLdgr->AddListItem(rData.LoadGrpKey, LT_STBK_CMD, Key);

	if(!bExist)
	{
		if(pLdgr) pLdgr->AddListItem(rData.LoadGrpKey, LT_STBK_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_STBK_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_stbk.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_STBK_CMD, Key, rData.LoadGrpKey);
	}
}

BOOL CDB_STBK::Del(T_STBK_K Key, CDB_NODE* pNode, CDB_LDGR* pLdgr)
{
	T_STBK_D Data;
	BOOL bExist=m_stbk.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_stbk.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pLdgr) VERIFY(pLdgr->DelListItem(Data.LoadGrpKey, LT_STBK_CMD, Key));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_STBK_CMD, Key);
			}

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_STBK_K key;
				T_STBK_D data;
				if(m_stbk.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_stbk.GetStartPosition();
					while(pos != NULL)
					{
						m_stbk.GetNextAssoc(pos,key,data);
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
BOOL CDB_STBK::Get(T_STBK_K Key,T_STBK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_stbk->Lookup(Key, StagLdgrKey)) return FALSE;
	}
	return m_stbk.Lookup(Key,rData);
}

int CDB_STBK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_stbk->GetCount();
	}
	return m_stbk.GetCount();
}

POSITION CDB_STBK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_stbk->GetStartPosition();
	}
	return m_stbk.GetStartPosition();
}

void CDB_STBK::GetNext(POSITION& rNextPosition,T_STBK_K& rKey,T_STBK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_stbk->GetNextAssoc(rNextPosition, rKey, StagLdgrKey);
		m_stbk.Lookup(rKey, rData);
		return;
	}
	m_stbk.GetNextAssoc(rNextPosition,rKey,rData);
}

