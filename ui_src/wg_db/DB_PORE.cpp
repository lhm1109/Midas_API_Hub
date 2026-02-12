#include "stdafx.h"
#include "DB_POLC.h"
#include "DB_PORE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PORE::CDB_PORE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_pore.InitHashTable(HASHSIZEPORE);
}

CDB_PORE::~CDB_PORE()
{

}

void CDB_PORE::Add(T_PORE_K Key,T_PORE_D& rData,CDB_POLC* pPolc)
{
	T_PORE_D Data;
	BOOL bExist=m_pore.Lookup(Key, Data);
	m_pore.SetAt(Key,rData);
#ifdef _MEC
	if (!bExist && !m_pDoc->IsDummyInitLoadPolc(rData.PolcK))
	{
		pPolc->AddListItem(rData.PolcK, LT_PORE_CMD, Key);
	}
#else
	if(!bExist)pPolc->AddListItem(rData.PolcK, LT_PORE_CMD, Key);
#endif
	if(Key == m_nStartNum)
	{
		T_PORE_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_pore.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PORE::Del(T_PORE_K Key,CDB_POLC* pPolc)
{
	T_PORE_D Data;
	BOOL bExist=m_pore.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pore.RemoveKey(Key);
		ASSERT(ret);
#ifdef _MEC
		if (ret && !m_pDoc->IsDummyInitLoadPolc(Data.PolcK))
		{
			VERIFY(pPolc->DelListItem(Data.PolcK, LT_PORE_CMD, Key));
		}
#else
		if(ret)
		{
			VERIFY(pPolc->DelListItem(Data.PolcK, LT_PORE_CMD, Key));
		}
#endif
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PORE_K key;
			T_PORE_D data;
			if(m_pore.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_pore.GetStartPosition();
				while(pos != NULL)
				{
					m_pore.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PORE::Get(T_PORE_K Key,T_PORE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pore.Lookup(Key,rData);
}

int CDB_PORE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pore.GetCount();
}

POSITION CDB_PORE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pore.GetStartPosition();
}

void CDB_PORE::GetNext(POSITION& rNextPosition,T_PORE_K& rKey,T_PORE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_pore.GetNextAssoc(rNextPosition,rKey,rData);
}

