#include "stdafx.h"
#include "DB_CGLP.h"
#include "DB_NLLP.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGLP::CDB_CGLP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_cglp.InitHashTable(HASHSIZECGLP);
}

CDB_CGLP::~CDB_CGLP()
{

}

void CDB_CGLP::Add(T_CGLP_K Key,T_CGLP_D& rData,CDB_NLLP* pNllp, CDB_BNGR* pBngr)
{
	T_CGLP_D Data;
	BOOL bExist=m_cglp.Lookup(Key, Data);
	if(bExist)
	{
		if (pNllp) pNllp->DelListItem(Data.GLinkPropK, LT_CGLP_CMD, Key);
		if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_CGLP_CMD, Key);
	}
	m_cglp.SetAt(Key,rData);
	if (pNllp) pNllp->AddListItem(rData.GLinkPropK, LT_CGLP_CMD, Key);
	if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_CGLP_CMD, Key);

	if(Key == m_nStartNum)
	{
		T_CGLP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_cglp.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	/* 시공단계에서 입력이 안된다. (관련코딩:EditData의 CheckCglp)
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_CGLP_CMD, Key, rData.GroupKey);
	}
	*/
}

BOOL CDB_CGLP::Del(T_CGLP_K Key,CDB_NLLP* pNllp, CDB_BNGR* pBngr)
{
	T_CGLP_D Data;
	BOOL bExist=m_cglp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_cglp.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pNllp) pNllp->DelListItem(Data.GLinkPropK, LT_CGLP_CMD, Key);
			if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_CGLP_CMD, Key);

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			/* 시공단계에서 삭제가 안 된다. (관련 코딩: EditData의 DelCglp)
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_CGLP_CMD, Key);
			}
			*/
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CGLP_K key;
			T_CGLP_D data;
			if(m_cglp.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_cglp.GetStartPosition();
				while(pos != NULL)
				{
					m_cglp.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_CGLP::Get(T_CGLP_K Key,T_CGLP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_cglp->Lookup(Key, StagBngrKey)) return FALSE;
	}
	return m_cglp.Lookup(Key,rData);
}

int CDB_CGLP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cglp->GetCount();
	}
	return m_cglp.GetCount();
}

POSITION CDB_CGLP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cglp->GetStartPosition();
	}
	return m_cglp.GetStartPosition();
}

void CDB_CGLP::GetNext(POSITION& rNextPosition,T_CGLP_K& rKey,T_CGLP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_cglp->GetNextAssoc(rNextPosition, rKey, StagBngrKey);
		m_cglp.Lookup(rKey, rData);
		return;
	}
	m_cglp.GetNextAssoc(rNextPosition,rKey,rData);
}
