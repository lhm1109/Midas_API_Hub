#include "stdafx.h"
#include "DB_LCTB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LCTB::CDB_LCTB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_lctb.InitHashTable(HASHSIZELCTB);
}

CDB_LCTB::~CDB_LCTB()
{
}

void CDB_LCTB::Add(T_LCTB_K Key,T_LCTB_D& rData)
{

	T_LCTB_D Data;
	BOOL bExist = m_lctb.Lookup(Key, Data);
	m_lctb.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_LCTB_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
	// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_LCTB_CMD, Key);
	}
}

BOOL CDB_LCTB::Del(T_LCTB_K Key)
{
	BOOL ret=m_lctb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LCTB_K key;
			T_LCTB_D data;
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
		// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_LCTB_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LCTB::Get(T_LCTB_K Key,T_LCTB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;    
		UINT uTemp;    
		if (!m_pDoc->m_pStagCtrl->m_lctb->Lookup(Key, uTemp)) return FALSE;
	}
	return m_lctb.Lookup(Key,rData);
}

int CDB_LCTB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{    
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_lctb->GetCount();    
	}
	return m_lctb.GetCount();
}

POSITION CDB_LCTB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{    
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_lctb->GetStartPosition();    
	}
	return m_lctb.GetStartPosition();
}

void CDB_LCTB::GetNext(POSITION& rNextPosition,T_LCTB_K& rKey,T_LCTB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{    
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }    
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_lctb->GetNextAssoc(rNextPosition, rKey, uTemp);    
		m_lctb.Lookup(rKey, rData);
		return;
	}
	m_lctb.GetNextAssoc(rNextPosition,rKey,rData);
}
