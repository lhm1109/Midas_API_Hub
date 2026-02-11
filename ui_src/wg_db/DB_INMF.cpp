#include "stdafx.h"
#include "DB_INMF.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_INMF::CDB_INMF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_inmf.InitHashTable(HASHSIZEINMF);
	m_index.Initialize(HASHSIZEINMF);
}

CDB_INMF::~CDB_INMF()
{
}

void CDB_INMF::Add(T_INMF_K Key, T_INMF_D& rData)
{
	T_INMF_D Data;
	BOOL bExist = m_inmf.Lookup(Key, Data);
	m_inmf.SetAt(Key,rData);
	if (bExist)
		m_index.Del(T_UINT12UINT20_INDEX_K(Data.nType, Data.ElemKey));
	m_index.Add(T_UINT12UINT20_INDEX_K(rData.nType, rData.ElemKey), Key);
	if(Key == m_nStartNum)
	{
		T_INMF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum, data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_INMF::Del(T_INMF_K Key)
{
	T_INMF_D Data;
	BOOL bExist=m_inmf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_inmf.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{      
			m_index.Del(T_UINT12UINT20_INDEX_K(Data.nType, Data.ElemKey));

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_INMF_K key;
				T_INMF_D data;
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
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_INMF::Get(T_INMF_K Key, T_INMF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_inmf->Lookup(Key, uTemp)) return FALSE;
	}
	return m_inmf.Lookup(Key, rData);
}

int CDB_INMF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_inmf->GetCount();
	}
	return m_inmf.GetCount();
}

POSITION CDB_INMF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_inmf->GetStartPosition();
	}
	return m_inmf.GetStartPosition();
}

void CDB_INMF::GetNext(POSITION& rNextPosition,T_INMF_K& rKey,T_INMF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_inmf->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_inmf.Lookup(rKey, rData);
		return;
	}
	m_inmf.GetNextAssoc(rNextPosition,rKey,rData);
}
