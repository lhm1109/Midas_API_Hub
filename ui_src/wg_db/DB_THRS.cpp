#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THRS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THRS::CDB_THRS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thrs.InitHashTable(HASHSIZETHRS);
}

CDB_THRS::~CDB_THRS()
{

}

void CDB_THRS::Add(T_THRS_K Key,T_THRS_D& rData,CDB_THIS* pThis)
{
	T_THRS_D Data;
	BOOL bExist=m_thrs.Lookup(Key, Data);
	m_thrs.SetAt(Key,rData);
#ifdef _MEC
	if (!bExist && !m_pDoc->IsDummyInitLoadThis(rData.ThisK))
	{
		pThis->AddListItem(rData.ThisK, LT_THRS_CMD, Key);
	}
#else
	if(!bExist)pThis->AddListItem(rData.ThisK, LT_THRS_CMD, Key);
#endif
	if(Key == m_nStartNum)
	{
		T_THRS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_thrs.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THRS::Del(T_THRS_K Key,CDB_THIS* pThis)
{
	T_THRS_D Data;
	BOOL bExist=m_thrs.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thrs.RemoveKey(Key);
		ASSERT(ret);
#ifdef _MEC
		if (ret && !m_pDoc->IsDummyInitLoadThis(Data.ThisK))
		{
			VERIFY(pThis->DelListItem(Data.ThisK, LT_THRS_CMD, Key));
		}
#else
		if(ret)
		{
			VERIFY(pThis->DelListItem(Data.ThisK, LT_THRS_CMD, Key));
		}
#endif
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THRS_K key;
			T_THRS_D data;
			if(m_thrs.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thrs.GetStartPosition();
				while(pos != NULL)
				{
					m_thrs.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THRS::Get(T_THRS_K Key,T_THRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thrs.Lookup(Key,rData);
}

int CDB_THRS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thrs.GetCount();
}

POSITION CDB_THRS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thrs.GetStartPosition();
}

void CDB_THRS::GetNext(POSITION& rNextPosition,T_THRS_K& rKey,T_THRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thrs.GetNextAssoc(rNextPosition,rKey,rData);
}

