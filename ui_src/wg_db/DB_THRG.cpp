#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THRG.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THRG::CDB_THRG()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thrg.InitHashTable(HASHSIZETHRG);
}

CDB_THRG::~CDB_THRG()
{

}

void CDB_THRG::Add(T_THRG_K Key,T_THRG_D& rData,CDB_THIS* pThis)
{
	T_THRG_D Data;
	BOOL bExist=m_thrg.Lookup(Key, Data);
	m_thrg.SetAt(Key,rData);
#ifdef _MEC
	if (!bExist && !m_pDoc->IsDummyInitLoadThis(rData.ThisK))
	{
		pThis->AddListItem(rData.ThisK, LT_THRG_CMD, Key);
	}
#else
	if(!bExist)pThis->AddListItem(rData.ThisK, LT_THRG_CMD, Key);
#endif
	if(Key == m_nStartNum)
	{
		T_THRG_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_thrg.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THRG::Del(T_THRG_K Key,CDB_THIS* pThis)
{
	T_THRG_D Data;
	BOOL bExist=m_thrg.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thrg.RemoveKey(Key);
		ASSERT(ret);
#ifdef _MEC
		if (ret && !m_pDoc->IsDummyInitLoadThis(Data.ThisK))
		{
			VERIFY(pThis->DelListItem(Data.ThisK, LT_THRG_CMD, Key));
		}
#else
		if(ret)
		{
			VERIFY(pThis->DelListItem(Data.ThisK, LT_THRG_CMD, Key));
		}
#endif
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THRG_K key;
			T_THRG_D data;
			if(m_thrg.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thrg.GetStartPosition();
				while(pos != NULL)
				{
					m_thrg.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THRG::Get(T_THRG_K Key,T_THRG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thrg.Lookup(Key,rData);
}

int CDB_THRG::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thrg.GetCount();
}

POSITION CDB_THRG::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thrg.GetStartPosition();
}

void CDB_THRG::GetNext(POSITION& rNextPosition,T_THRG_K& rKey,T_THRG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thrg.GetNextAssoc(rNextPosition,rKey,rData);
}

