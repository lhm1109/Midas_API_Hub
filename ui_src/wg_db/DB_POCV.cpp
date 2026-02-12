#include "stdafx.h"
#include "DB_POLC.h"
#include "DB_POCV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POCV::CDB_POCV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_pocv.InitHashTable(HASHSIZEPOCV);
}

CDB_POCV::~CDB_POCV()
{

}

void CDB_POCV::Add(T_POCV_K Key,T_POCV_D& rData,CDB_POLC* pPolc)
{
	T_POCV_D Data;
	BOOL bExist=m_pocv.Lookup(Key, Data);
	m_pocv.SetAt(Key,rData);
	if(!bExist)pPolc->AddListItem(Key, LT_POCV_CMD, Key);
	if(Key == m_nStartNum)
	{
		T_POCV_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_pocv.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_POCV::Del(T_POCV_K Key,CDB_POLC* pPolc)
{
	T_POCV_D Data;
	BOOL bExist=m_pocv.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pocv.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pPolc->DelListItem(Key, LT_POCV_CMD, Key));
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_POCV_K key;
			T_POCV_D data;
			if(m_pocv.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_pocv.GetStartPosition();
				while(pos != NULL)
				{
					m_pocv.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_POCV::Get(T_POCV_K Key,T_POCV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pocv.Lookup(Key,rData);
}

int CDB_POCV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pocv.GetCount();
}

POSITION CDB_POCV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pocv.GetStartPosition();
}

void CDB_POCV::GetNext(POSITION& rNextPosition,T_POCV_K& rKey,T_POCV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_pocv.GetNextAssoc(rNextPosition,rKey,rData);
}

