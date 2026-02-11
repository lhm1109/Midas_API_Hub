#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THEF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THEF::CDB_THEF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thef.InitHashTable(HASHSIZETHEF);
}

CDB_THEF::~CDB_THEF()
{

}

void CDB_THEF::Add(T_THEF_K Key,T_THEF_D& rData,CDB_THIS* pThis)
{
	T_THEF_D Data;
	BOOL bExist=m_thef.Lookup(Key, Data);
	m_thef.SetAt(Key,rData);
#ifdef _MEC
	if (!bExist && !m_pDoc->IsDummyInitLoadThis(rData.ThisK))
	{
		pThis->AddListItem(rData.ThisK, LT_THEF_CMD, Key);
	}
#else
	if(!bExist)pThis->AddListItem(rData.ThisK, LT_THEF_CMD, Key);
#endif
	if(Key == m_nStartNum)
	{
		T_THEF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_thef.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THEF::Del(T_THEF_K Key,CDB_THIS* pThis)
{
	T_THEF_D Data;
	BOOL bExist=m_thef.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thef.RemoveKey(Key);
		ASSERT(ret);
#ifdef _MEC
		if (ret && !m_pDoc->IsDummyInitLoadThis(Data.ThisK))
		{
			VERIFY(pThis->DelListItem(Data.ThisK, LT_THEF_CMD, Key));
		}
#else
		if(ret)
		{
			VERIFY(pThis->DelListItem(Data.ThisK, LT_THEF_CMD, Key));
		}
#endif
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THEF_K key;
			T_THEF_D data;
			if(m_thef.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thef.GetStartPosition();
				while(pos != NULL)
				{
					m_thef.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THEF::Get(T_THEF_K Key,T_THEF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thef.Lookup(Key,rData);
}

int CDB_THEF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thef.GetCount();
}

POSITION CDB_THEF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thef.GetStartPosition();
}

void CDB_THEF::GetNext(POSITION& rNextPosition,T_THEF_K& rKey,T_THEF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thef.GetNextAssoc(rNextPosition,rKey,rData);
}

