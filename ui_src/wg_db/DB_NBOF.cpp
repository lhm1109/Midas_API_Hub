#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_NBOF.h"
#include "DB_LDGR.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NBOF::CDB_NBOF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_nbof.InitHashTable(HASHSIZENBOF);
}
CDB_NBOF::~CDB_NBOF()
{

}
void CDB_NBOF::Add(T_NBOF_K Key,T_NBOF_D& rData,CDB_STLD* pStld,CDB_LDGR* pLdgr)
{
	T_NBOF_D Data;
	BOOL bExist=m_nbof.Lookup(Key, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_NBOF_CMD, Key));
	if(bExist && Data.LdgrKey != rData.LdgrKey)
		VERIFY(pLdgr->DelListItem(Data.LdgrKey, LT_NBOF_CMD, Key));
	m_nbof.SetAt(Key,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_NBOF_CMD, Key);
	if(bExist && Data.LdgrKey != rData.LdgrKey)
		pLdgr->AddListItem(rData.LdgrKey, LT_NBOF_CMD, Key);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_NBOF_CMD, Key);
		pLdgr->AddListItem(rData.LdgrKey, LT_NBOF_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_NBOF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_nbof.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_NBOF_CMD, Key, rData.LdgrKey);
	}
}

BOOL CDB_NBOF::Del(T_NBOF_K Key,CDB_STLD* pStld,CDB_LDGR* pLdgr)
{
	T_NBOF_D Data;
	BOOL bExist=m_nbof.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_nbof.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_NBOF_CMD, Key));
			VERIFY(pLdgr->DelListItem(Data.LdgrKey, LT_NBOF_CMD, Key));
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_NBOF_K key;
				T_NBOF_D data;
				if(m_nbof.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_nbof.GetStartPosition();
					while(pos != NULL)
					{
						m_nbof.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_NBOF_CMD, Key);
			}
		}
		return ret;
	}
	return bExist;
}

void CDB_NBOF::AddItem(T_NBOF_K Key,T_NODE_K KeyNode)
{
	T_NBOF_D Data;
	if(!m_nbof.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return;
	}

	// 기존 데이터에 없는 Node Key만 들어와야 됨
	if(KeyNode != 0)
	{
		Data.arKeyNode.Add(KeyNode);
		m_nbof.SetAt(Key,Data);
	}
}

BOOL CDB_NBOF::DelItem(T_NBOF_K Key,T_NODE_K KeyNode)
{
	T_NBOF_D Data;
	if(!m_nbof.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if(KeyNode != 0)
	{
		int nGroup=Data.arKeyNode.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.arKeyNode[i] == KeyNode)
			{
				Data.arKeyNode.RemoveAt(i);
				m_nbof.SetAt(Key,Data);
				return TRUE;
			}
		}
	}
	ASSERT(FALSE);
	return FALSE;
}

//--------------------------------------------------------------------------
BOOL CDB_NBOF::Get(T_NBOF_K Key,T_NBOF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_nbof.Lookup(Key,rData);
}

int CDB_NBOF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_nbof.GetCount();
}

POSITION CDB_NBOF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_nbof.GetStartPosition();
}

void CDB_NBOF::GetNext(POSITION& rNextPosition,T_NBOF_K& rKey,T_NBOF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}  
	m_nbof.GetNextAssoc(rNextPosition,rKey,rData);
}
