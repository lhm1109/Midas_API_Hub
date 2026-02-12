#include "stdafx.h"
#include "DB_SMLC.h"
#include "DB_SMPT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SMLC::CDB_SMLC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_smlc.InitHashTable(HASHSIZESMLC);
}

CDB_SMLC::~CDB_SMLC()
{
}

void CDB_SMLC::Add(T_SMLC_K Key,T_SMLC_D& rData,CDB_SMPT* pSmpt)
{
	T_SMLC_D Data;
	BOOL bExist=m_smlc.Lookup(Key, Data);
	CMap<T_SMPT_K, T_SMPT_K, T_SMPT_K, T_SMPT_K>mapOld;
	CMap<T_SMPT_K, T_SMPT_K, T_SMPT_K, T_SMPT_K>mapNew;
	T_SMPT_K KeySmptTemp;
	if(bExist && pSmpt != NULL)
	{
		mapNew.InitHashTable(HASHSIZESMPT);
		mapOld.InitHashTable(HASHSIZESMPT);
		for(int i = 0; i<Data.aPointData.GetSize(); i++)mapOld.SetAt(Data.aPointData[i], Data.aPointData[i]);
		for(int i = 0; i<rData.aPointData.GetSize(); i++)mapNew.SetAt(rData.aPointData[i], rData.aPointData[i]);
		for(int i = 0; i<Data.aPointData.GetSize(); i++)
		{
			if(!mapNew.Lookup(Data.aPointData[i], KeySmptTemp))
				VERIFY(pSmpt->DelListItem(Data.aPointData[i], LT_SMLC_CMD, Key));
		}
	}
	m_smlc.SetAt(Key,rData);
	if(pSmpt != NULL)
	{
		for(int i=0; i<rData.aPointData.GetSize(); i++)
		{
			if(bExist)
			{
				if(!mapOld.Lookup(rData.aPointData[i], KeySmptTemp))
					pSmpt->AddListItem(rData.aPointData[i], LT_SMLC_CMD, Key);
			}
			else
				pSmpt->AddListItem(rData.aPointData[i], LT_SMLC_CMD, Key);
		}
	}
	if(Key == m_nStartNum)
	{
		T_SMLC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SMLC::Del(T_SMLC_K Key,CDB_SMPT* pSmpt)
{
	T_SMLC_D Data;
	BOOL bExist=m_smlc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_smlc.RemoveKey(Key);
		ASSERT(ret);
		if(ret && pSmpt != NULL)
		{
			for(int i=0; i<Data.aPointData.GetSize(); i++)
			{
				VERIFY(pSmpt->DelListItem(Data.aPointData[i], LT_SMLC_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SMLC_K key;
			T_SMLC_D data;
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
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SMLC::Get(T_SMLC_K Key,T_SMLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_smlc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_smlc.Lookup(Key,rData);
}

int CDB_SMLC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_smlc->GetCount();
	}
	return m_smlc.GetCount();
}

POSITION CDB_SMLC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_smlc->GetStartPosition();
	}
	return m_smlc.GetStartPosition();
}

void CDB_SMLC::GetNext(POSITION& rNextPosition,T_SMLC_K& rKey,T_SMLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_smlc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_smlc.Lookup(rKey, rData);
		return;
	}
	m_smlc.GetNextAssoc(rNextPosition,rKey,rData);
}
