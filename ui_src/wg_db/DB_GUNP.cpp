#include "stdafx.h"
#include "DB_GUNP.h"
#include "DB_NPLN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GUNP::CDB_GUNP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_gunp.InitHashTable(HASHSIZEGUNP);
}

CDB_GUNP::~CDB_GUNP()
{
}

void CDB_GUNP::Add(T_GUNP_K Key,T_GUNP_D& rData, CDB_NPLN* pNpln)
{
	T_GUNP_D Data;
	BOOL bExist=m_gunp.Lookup(Key, Data);
 	m_gunp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_GUNP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUNP::Del(T_GUNP_K Key, CDB_NPLN* pNpln)
{
	T_GUNP_D Data;
	BOOL bExist=m_gunp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gunp.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUNP_K key;
			T_GUNP_D data;
			if(m_gunp.Lookup(m_nLastNum-1,data))
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
		return ret;
	}
	return bExist;
}

BOOL CDB_GUNP::Get(T_GUNP_K Key, T_GUNP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gunp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gunp.Lookup(Key, rData);
}
int CDB_GUNP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gunp->GetCount();
	}
	return m_gunp.GetCount();
}
POSITION CDB_GUNP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gunp->GetStartPosition();
	}
	return m_gunp.GetStartPosition();
}
void CDB_GUNP::GetNext(POSITION& rNextPosition, T_GUNP_K& rKey, T_GUNP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gunp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gunp.Lookup(rKey, rData);
		return;
	}
	m_gunp.GetNextAssoc(rNextPosition, rKey, rData);
}