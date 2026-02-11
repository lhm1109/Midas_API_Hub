#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_NODE.h"
#include "DB_PNLA.h"
#include "DB_LDGR.h"
#include "DB_GRUP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PNLA::CDB_PNLA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_pnla.InitHashTable(HASHSIZEPNLA);
}

CDB_PNLA::~CDB_PNLA()
{

}

void CDB_PNLA::Add(T_PNLA_K Key,T_PNLA_D& rData,CDB_NODE* pNode,CDB_STLD* pStld,CDB_LDGR* pLdgr,CDB_GRUP* pGrup)
{
	T_PNLA_D Data;
	BOOL bExist=m_pnla.Lookup(Key, Data);
	if(bExist)
	{
		if (Data.nElemSelection == 0 && Data.bLoadingArea)
		{
			for(int i=0; i<D_PNLA_MAXNOD; i++)
			{
				if(Data.nConnectivity[i] == 0)break;
				pNode->DelListItem(Data.nConnectivity[i], LT_PNLA_CMD, Key);
			}
		}
		pStld->DelListItem(Data.LoadCaseKey, LT_PNLA_CMD, Key);
		pLdgr->DelListItem(Data.GroupKey, LT_PNLA_CMD, Key);
		if (Data.nElemSelection == 1)
			pGrup->DelListItem(Data.ElemGroupKey, LT_PNLA_CMD, Key);
	}
	m_pnla.SetAt(Key,rData);
	if (rData.nElemSelection == 0 && rData.bLoadingArea)
	{
		for(int i=0; i<D_PNLA_MAXNOD; i++)
		{
			if(rData.nConnectivity[i] == 0)break;
			pNode->AddListItem(rData.nConnectivity[i], LT_PNLA_CMD, Key);
		}
	}
	pStld->AddListItem(rData.LoadCaseKey, LT_PNLA_CMD, Key);
	pLdgr->AddListItem(rData.GroupKey, LT_PNLA_CMD, Key);
	if (rData.nElemSelection == 1)
		pGrup->AddListItem(rData.ElemGroupKey, LT_PNLA_CMD, Key);
	if(Key == m_nStartNum)
	{
		T_PNLA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_pnla.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_PNLA_CMD, Key, rData.GroupKey);
	}
}

BOOL CDB_PNLA::Del(T_PNLA_K Key,CDB_NODE* pNode,CDB_STLD* pStld,CDB_LDGR* pLdgr,CDB_GRUP* pGrup)
{
	T_PNLA_D Data;
	BOOL bExist=m_pnla.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pnla.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (Data.nElemSelection == 0 && Data.bLoadingArea)
			{
				for(int i=0; i<D_PNLA_MAXNOD; i++)
				{
					if(Data.nConnectivity[i] == 0)break;
					VERIFY(pNode->DelListItem(Data.nConnectivity[i], LT_PNLA_CMD, Key));
				}
			}
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PNLA_CMD, Key));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PNLA_CMD, Key));
			if (Data.nElemSelection == 1)
				VERIFY(pGrup->DelListItem(Data.ElemGroupKey, LT_PNLA_CMD, Key));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_PNLA_CMD, Key);
			}

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_PNLA_K key;
				T_PNLA_D data;
				if(m_pnla.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_pnla.GetStartPosition();
					while(pos != NULL)
					{
						m_pnla.GetNextAssoc(pos,key,data);
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
BOOL CDB_PNLA::Get(T_PNLA_K Key,T_PNLA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_pnla->Lookup(Key, StagLdgrKey)) return FALSE;
	}
	return m_pnla.Lookup(Key,rData);
}

int CDB_PNLA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_pnla->GetCount();
	}
	return m_pnla.GetCount();
}

POSITION CDB_PNLA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_pnla->GetStartPosition();
	}
	return m_pnla.GetStartPosition();
}

void CDB_PNLA::GetNext(POSITION& rNextPosition,T_PNLA_K& rKey,T_PNLA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_pnla->GetNextAssoc(rNextPosition, rKey, StagLdgrKey);
		m_pnla.Lookup(rKey, rData);
		return;
	}
	m_pnla.GetNextAssoc(rNextPosition,rKey,rData);
}

