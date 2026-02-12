#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_STMP.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STMP::CDB_STMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stmp.InitHashTable(HASHSIZESTMP);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_STMP::~CDB_STMP()
{

}

void CDB_STMP::Add(T_STMP_K Key,T_STMP_D& rData,CDB_STLD* pStld,CDB_LDGR* pLdgr)
{
	T_STMP_D Data;
	BOOL bExist=m_stmp.Lookup(Key, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_STMP_CMD, Key));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_STMP_CMD, Key));
	m_stmp.SetAt(Key,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_STMP_CMD, Key);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_STMP_CMD, Key);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_STMP_CMD, Key);
		pLdgr->AddListItem(rData.GroupKey, LT_STMP_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_STMP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_stmp.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_STMP_CMD, Key, rData.GroupKey);
	}
}

BOOL CDB_STMP::Del(T_STMP_K Key,CDB_STLD* pStld,CDB_LDGR* pLdgr)
{
	T_STMP_D Data;
	BOOL bExist=m_stmp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_stmp.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_STMP_CMD, Key));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_STMP_CMD, Key));
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_STMP_CMD, Key);
			}

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_STMP_K key;
				T_STMP_D data;
				if(m_stmp.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_stmp.GetStartPosition();
					while(pos != NULL)
					{
						m_stmp.GetNextAssoc(pos,key,data);
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
BOOL CDB_STMP::Get(T_STMP_K Key,T_STMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_stmp->Lookup(Key, StagLdgrKey)) return FALSE;
	}
	return m_stmp.Lookup(Key,rData);
}

int CDB_STMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_stmp->GetCount();
	}
	return m_stmp.GetCount();
}

POSITION CDB_STMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_stmp->GetStartPosition();
	}
	return m_stmp.GetStartPosition();
}

void CDB_STMP::GetNext(POSITION& rNextPosition,T_STMP_K& rKey,T_STMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_stmp->GetNextAssoc(rNextPosition, rKey, StagLdgrKey);
		m_stmp.Lookup(rKey, rData);
		return;
	}
	m_stmp.GetNextAssoc(rNextPosition,rKey,rData);
}
