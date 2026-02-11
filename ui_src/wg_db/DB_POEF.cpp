#include "stdafx.h"
#include "DB_POLC.h"
#include "DB_POEF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POEF::CDB_POEF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_poef.InitHashTable(HASHSIZEPOEF);
}

CDB_POEF::~CDB_POEF()
{

}

void CDB_POEF::Add(T_POEF_K Key,T_POEF_D& rData,CDB_POLC* pPolc)
{
	T_POEF_D Data;
	BOOL bExist=m_poef.Lookup(Key, Data);
	m_poef.SetAt(Key,rData);
#ifdef _MEC
	if (!bExist && !m_pDoc->IsDummyInitLoadPolc(rData.PolcK))
	{
		pPolc->AddListItem(rData.PolcK, LT_POEF_CMD, Key);
	}
#else
	if(!bExist)pPolc->AddListItem(rData.PolcK, LT_POEF_CMD, Key);
#endif
	if(Key == m_nStartNum)
	{
		T_POEF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_poef.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_POEF::Del(T_POEF_K Key,CDB_POLC* pPolc)
{
	T_POEF_D Data;
	BOOL bExist=m_poef.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_poef.RemoveKey(Key);
		ASSERT(ret);
#ifdef _MEC
		if (ret && !m_pDoc->IsDummyInitLoadPolc(Data.PolcK))
		{
			VERIFY(pPolc->DelListItem(Data.PolcK, LT_POEF_CMD, Key));
		}
#else
		if(ret)
		{
			VERIFY(pPolc->DelListItem(Data.PolcK, LT_POEF_CMD, Key));
		}
#endif

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_POEF_K key;
			T_POEF_D data;
			if(m_poef.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_poef.GetStartPosition();
				while(pos != NULL)
				{
					m_poef.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_POEF::Get(T_POEF_K Key,T_POEF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_poef.Lookup(Key,rData);
}

int CDB_POEF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_poef.GetCount();
}

POSITION CDB_POEF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_poef.GetStartPosition();
}

void CDB_POEF::GetNext(POSITION& rNextPosition,T_POEF_K& rKey,T_POEF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_poef.GetNextAssoc(rNextPosition,rKey,rData);
}

