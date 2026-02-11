#include "stdafx.h"
#include "DB_POLC.h"
#include "DB_PORI.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PORI::CDB_PORI()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_pori.InitHashTable(HASHSIZEPORI);
}

CDB_PORI::~CDB_PORI()
{

}

void CDB_PORI::Add(T_PORI_K Key,T_PORI_D& rData,CDB_POLC* pPolc)
{
	T_PORI_D Data;
	BOOL bExist=m_pori.Lookup(Key, Data);
	m_pori.SetAt(Key,rData);
#ifdef _MEC
	if (!bExist && !m_pDoc->IsDummyInitLoadPolc(rData.PolcK))
	{
		pPolc->AddListItem(rData.PolcK, LT_PORI_CMD, Key);
	}
#else
	if(!bExist)pPolc->AddListItem(rData.PolcK, LT_PORI_CMD, Key);
#endif
	if(Key == m_nStartNum)
	{
		T_PORI_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_pori.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PORI::Del(T_PORI_K Key,CDB_POLC* pPolc)
{
	T_PORI_D Data;
	BOOL bExist=m_pori.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pori.RemoveKey(Key);
		ASSERT(ret);
#ifdef _MEC
		if (ret && !m_pDoc->IsDummyInitLoadPolc(Data.PolcK))
		{
			VERIFY(pPolc->DelListItem(Data.PolcK, LT_PORI_CMD, Key));
		}
#else
		if(ret)
		{
			VERIFY(pPolc->DelListItem(Data.PolcK, LT_PORI_CMD, Key));
		}
#endif
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PORI_K key;
			T_PORI_D data;
			if(m_pori.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_pori.GetStartPosition();
				while(pos != NULL)
				{
					m_pori.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PORI::Get(T_PORI_K Key,T_PORI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pori.Lookup(Key,rData);
}

int CDB_PORI::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pori.GetCount();
}

POSITION CDB_PORI::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pori.GetStartPosition();
}

void CDB_PORI::GetNext(POSITION& rNextPosition,T_PORI_K& rKey,T_PORI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_pori.GetNextAssoc(rNextPosition,rKey,rData);
}

