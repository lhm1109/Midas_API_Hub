#include "stdafx.h"
#include "DB_POLC.h"
#include "DB_PORG.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PORG::CDB_PORG()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_porg.InitHashTable(HASHSIZEPORG);
}

CDB_PORG::~CDB_PORG()
{

}

void CDB_PORG::Add(T_PORG_K Key,T_PORG_D& rData,CDB_POLC* pPolc)
{
	T_PORG_D Data;
	BOOL bExist=m_porg.Lookup(Key, Data);
	m_porg.SetAt(Key,rData);
#ifdef _MEC
	if (!bExist && !m_pDoc->IsDummyInitLoadPolc(rData.PolcK))
	{
		pPolc->AddListItem(rData.PolcK, LT_PORG_CMD, Key);
	}
#else
	if(!bExist)pPolc->AddListItem(rData.PolcK, LT_PORG_CMD, Key);
#endif
	if(Key == m_nStartNum)
	{
		T_PORG_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_porg.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PORG::Del(T_PORG_K Key,CDB_POLC* pPolc)
{
	T_PORG_D Data;
	BOOL bExist=m_porg.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_porg.RemoveKey(Key);
		ASSERT(ret);
#ifdef _MEC
		if (ret && !m_pDoc->IsDummyInitLoadPolc(Data.PolcK))
		{
			VERIFY(pPolc->DelListItem(Data.PolcK, LT_PORG_CMD, Key));
		}
#else
		if(ret)
		{
			VERIFY(pPolc->DelListItem(Data.PolcK, LT_PORG_CMD, Key));
		}
#endif
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PORG_K key;
			T_PORG_D data;
			if(m_porg.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_porg.GetStartPosition();
				while(pos != NULL)
				{
					m_porg.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PORG::Get(T_PORG_K Key,T_PORG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_porg.Lookup(Key,rData);
}

int CDB_PORG::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_porg.GetCount();
}

POSITION CDB_PORG::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_porg.GetStartPosition();
}

void CDB_PORG::GetNext(POSITION& rNextPosition,T_PORG_K& rKey,T_PORG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_porg.GetNextAssoc(rNextPosition,rKey,rData);
}

