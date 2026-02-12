#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THRI.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THRI::CDB_THRI()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thri.InitHashTable(HASHSIZETHRI);
}

CDB_THRI::~CDB_THRI()
{

}

void CDB_THRI::Add(T_THRI_K Key,T_THRI_D& rData,CDB_THIS* pThis)
{
	T_THRI_D Data;
	BOOL bExist=m_thri.Lookup(Key, Data);
	m_thri.SetAt(Key,rData);
#ifdef _MEC
	if (!bExist && !m_pDoc->IsDummyInitLoadThis(rData.ThisK))
	{
		pThis->AddListItem(rData.ThisK, LT_THRI_CMD, Key);
	}
#else
	if (!bExist)pThis->AddListItem(rData.ThisK, LT_THRI_CMD, Key);
#endif
	if(Key == m_nStartNum)
	{
		T_THRI_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_thri.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THRI::Del(T_THRI_K Key,CDB_THIS* pThis)
{
	T_THRI_D Data;
	BOOL bExist=m_thri.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thri.RemoveKey(Key);
		ASSERT(ret);
#ifdef _MEC
		if (ret && !m_pDoc->IsDummyInitLoadThis(Data.ThisK))
		{
			VERIFY(pThis->DelListItem(Data.ThisK, LT_THRI_CMD, Key));
		}
#else
		if(ret)
		{
			VERIFY(pThis->DelListItem(Data.ThisK, LT_THRI_CMD, Key));
		}
#endif

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THRI_K key;
			T_THRI_D data;
			if(m_thri.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thri.GetStartPosition();
				while(pos != NULL)
				{
					m_thri.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THRI::Get(T_THRI_K Key,T_THRI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thri.Lookup(Key,rData);
}

int CDB_THRI::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thri.GetCount();
}

POSITION CDB_THRI::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thri.GetStartPosition();
}

void CDB_THRI::GetNext(POSITION& rNextPosition,T_THRI_K& rKey,T_THRI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thri.GetNextAssoc(rNextPosition,rKey,rData);
}

