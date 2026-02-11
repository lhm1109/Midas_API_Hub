#include "stdafx.h"
#include "stdafx.h"
#include "DB_P1LT.h"
#include "DB_LNKF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LNKF::CDB_LNKF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lnkf.InitHashTable(HASHSIZELNKF);
	m_index.Initialize(HASHSIZELNKF);

	m_nStartNum=1;
	m_nLastNum=0;
}

CDB_LNKF::~CDB_LNKF()
{

}

void CDB_LNKF::Add(T_LNKF_K Key,T_LNKF_D& rData,CDB_P1LT* pP1lt)
{
	T_LNKF_D Data;
	BOOL bExist=m_lnkf.Lookup(Key, Data);
	m_lnkf.SetAt(Key,rData);
	if (bExist)
	{
		m_index.Del(T_UINT3_INDEX_K(Data.nLinkType, Data.LinkKey, Data.nFactorKind, CHashKeySet::UINT3_2b22b8b));
		if (Data.nFactorKind == 1)
			pP1lt->DelListItem(Data.TypeKey, LT_LNKF_CMD, Key);
	}
	if (rData.nFactorKind == 1) pP1lt->AddListItem(rData.TypeKey, LT_LNKF_CMD, Key);
	m_index.Add(T_UINT3_INDEX_K(rData.nLinkType, rData.LinkKey, rData.nFactorKind, CHashKeySet::UINT3_2b22b8b), Key);

	if(Key == m_nStartNum)
	{
		T_LNKF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_LNKF_CMD, Key);
	}
}

BOOL CDB_LNKF::Del(T_LNKF_K Key,CDB_P1LT* pP1lt)
{
	T_LNKF_D Data;
	m_lnkf.Lookup(Key, Data);
	BOOL ret=m_lnkf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (Data.nFactorKind == 1) pP1lt->DelListItem(Data.TypeKey, LT_LNKF_CMD, Key);
		m_index.Del(T_UINT3_INDEX_K(Data.nLinkType, Data.LinkKey, Data.nFactorKind, CHashKeySet::UINT3_2b22b8b));

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LNKF_K key;
			T_LNKF_D data;
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

		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_LNKF_CMD, Key);
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LNKF::Get(T_LNKF_K Key,T_LNKF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_lnkf->Lookup(Key, uTemp)) return FALSE;
	}
	return m_lnkf.Lookup(Key,rData);
}

int CDB_LNKF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_lnkf->GetCount();
	}
	return m_lnkf.GetCount();
}

POSITION CDB_LNKF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_lnkf->GetStartPosition();
	}
	return m_lnkf.GetStartPosition();
}

void CDB_LNKF::GetNext(POSITION& rNextPosition,T_LNKF_K& rKey,T_LNKF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_lnkf->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_lnkf.Lookup(rKey, rData);
		return;
	}
	m_lnkf.GetNextAssoc(rNextPosition,rKey,rData);
}
