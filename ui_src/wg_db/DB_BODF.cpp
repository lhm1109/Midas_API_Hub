#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_BODF.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BODF::CDB_BODF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_bodf.InitHashTable(HASHSIZEBODF);
}

CDB_BODF::~CDB_BODF()
{

}

void CDB_BODF::Add(T_BODF_K Key,T_BODF_D& rData,CDB_STLD* pStld,CDB_LDGR* pLdgr)
{
	T_BODF_D Data;
	BOOL bExist=m_bodf.Lookup(Key, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_BODF_CMD, Key));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_BODF_CMD, Key));
	m_bodf.SetAt(Key,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_BODF_CMD, Key);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_BODF_CMD, Key);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_BODF_CMD, Key);
		pLdgr->AddListItem(rData.GroupKey, LT_BODF_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_BODF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_bodf.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_BODF_CMD, Key, rData.GroupKey);
	}
}

BOOL CDB_BODF::Del(T_BODF_K Key,CDB_STLD* pStld,CDB_LDGR* pLdgr)
{
	T_BODF_D Data;
	BOOL bExist=m_bodf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_bodf.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_BODF_CMD, Key));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_BODF_CMD, Key));
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_BODF_K key;
				T_BODF_D data;
				if(m_bodf.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_bodf.GetStartPosition();
					while(pos != NULL)
					{
						m_bodf.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_BODF_CMD, Key);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_BODF::Get(T_BODF_K Key,T_BODF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_bodf->Lookup(Key, StagLdgrKey)) return FALSE;
	}
	return m_bodf.Lookup(Key,rData);
}

int CDB_BODF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_bodf->GetCount();
	}
	return m_bodf.GetCount();
}

POSITION CDB_BODF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_bodf->GetStartPosition();
	}
	return m_bodf.GetStartPosition();
}

void CDB_BODF::GetNext(POSITION& rNextPosition,T_BODF_K& rKey,T_BODF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_bodf->GetNextAssoc(rNextPosition, rKey, StagLdgrKey);
		m_bodf.Lookup(rKey, rData);
		return;
	}
	m_bodf.GetNextAssoc(rNextPosition,rKey,rData);
}
