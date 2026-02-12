#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_NODE.h"
#include "DB_FBLD.h"
#include "DB_FBLA.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FBLA::CDB_FBLA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_fbla.InitHashTable(HASHSIZEFBLA);
}

CDB_FBLA::~CDB_FBLA()
{

}

void CDB_FBLA::Add(T_FBLA_K Key,T_FBLA_D& rData,CDB_FBLD* pFbld,CDB_NODE* pNode,CDB_LDGR* pLdgr)
{
	T_FBLA_D Data;
	BOOL bExist=m_fbla.Lookup(Key, Data);
	if(bExist)
	{
		pFbld->DelListItem(Data.LoadTypeKey, LT_FBLA_CMD, Key);
		for(int i=0; i<D_FBLA_MAXNOD; i++)
		{
			if(Data.nConnectivity[i] == 0)break;
			pNode->DelListItem(Data.nConnectivity[i], LT_FBLA_CMD, Key);
		}
		pLdgr->DelListItem(Data.GroupKey, LT_FBLA_CMD, Key);
	}
	m_fbla.SetAt(Key,rData);
	pFbld->AddListItem(rData.LoadTypeKey, LT_FBLA_CMD, Key);
	for(int i=0; i<D_FBLA_MAXNOD; i++)
	{
		if(rData.nConnectivity[i] == 0)break;
		pNode->AddListItem(rData.nConnectivity[i], LT_FBLA_CMD, Key);
	}
	pLdgr->AddListItem(rData.GroupKey, LT_FBLA_CMD, Key);
	if(Key == m_nStartNum)
	{
		T_FBLA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_fbla.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_FBLA_CMD, Key, rData.GroupKey);
	}
}

BOOL CDB_FBLA::Del(T_FBLA_K Key,CDB_FBLD* pFbld,CDB_NODE* pNode,CDB_LDGR* pLdgr)
{
	T_FBLA_D Data;
	BOOL bExist=m_fbla.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fbla.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pFbld->DelListItem(Data.LoadTypeKey, LT_FBLA_CMD, Key));
			for(int i=0; i<D_FBLA_MAXNOD; i++)
			{
				if(Data.nConnectivity[i] == 0)break;
				VERIFY(pNode->DelListItem(Data.nConnectivity[i], LT_FBLA_CMD, Key));
			}
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_FBLA_CMD, Key));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_FBLA_CMD, Key);
			}

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_FBLA_K key;
				T_FBLA_D data;
				if(m_fbla.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_fbla.GetStartPosition();
					while(pos != NULL)
					{
						m_fbla.GetNextAssoc(pos,key,data);
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
BOOL CDB_FBLA::Get(T_FBLA_K Key,T_FBLA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_fbla->Lookup(Key, StagLdgrKey)) return FALSE;
	}
	return m_fbla.Lookup(Key,rData);
}

int CDB_FBLA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_fbla->GetCount();
	}
	return m_fbla.GetCount();
}

POSITION CDB_FBLA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_fbla->GetStartPosition();
	}
	return m_fbla.GetStartPosition();
}

void CDB_FBLA::GetNext(POSITION& rNextPosition,T_FBLA_K& rKey,T_FBLA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_fbla->GetNextAssoc(rNextPosition, rKey, StagLdgrKey);
		m_fbla.Lookup(rKey, rData);
		return;
	}
	m_fbla.GetNextAssoc(rNextPosition,rKey,rData);
}

