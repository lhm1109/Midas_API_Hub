#include "stdafx.h"
#include "DB_EQMF.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_EQMF::CDB_EQMF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_eqmf.InitHashTable(HASHSIZEEQMF);
	m_index.Initialize(HASHSIZEEQMF);
}

CDB_EQMF::~CDB_EQMF()
{
}

void CDB_EQMF::Add(T_EQMF_K Key, T_EQMF_D& rData)
{
	T_EQMF_D Data;
	BOOL bExist = m_eqmf.Lookup(Key, Data);
	m_eqmf.SetAt(Key,rData);
	if (bExist)
		m_index.Del(T_UINT12UINT20_INDEX_K(Data.nType, Data.ElemKey));
	m_index.Add(T_UINT12UINT20_INDEX_K(rData.nType, rData.ElemKey), Key);
	if(Key == m_nStartNum)
	{
		T_EQMF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum, data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_EQMF::Del(T_EQMF_K Key)
{
	T_EQMF_D Data;
	BOOL bExist=m_eqmf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_eqmf.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{      
			m_index.Del(T_UINT12UINT20_INDEX_K(Data.nType, Data.ElemKey));

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_EQMF_K key;
				T_EQMF_D data;
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
BOOL CDB_EQMF::Get(T_EQMF_K Key, T_EQMF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT nTemp;
		if (!m_pDoc->m_pStagCtrl->m_eqmf->Lookup(Key, nTemp)) return FALSE;
	}
	return m_eqmf.Lookup(Key, rData);
}

int CDB_EQMF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_eqmf->GetCount();
	}
	return m_eqmf.GetCount();
}

POSITION CDB_EQMF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_eqmf->GetStartPosition();
	}
	return m_eqmf.GetStartPosition();
}

void CDB_EQMF::GetNext(POSITION& rNextPosition,T_EQMF_K& rKey,T_EQMF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT nTemp;
		m_pDoc->m_pStagCtrl->m_eqmf->GetNextAssoc(rNextPosition, rKey, nTemp);
		m_eqmf.Lookup(rKey, rData);
		return;
	}
	m_eqmf.GetNextAssoc(rNextPosition,rKey,rData);
}
