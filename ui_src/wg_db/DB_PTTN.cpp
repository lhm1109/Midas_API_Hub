#include "stdafx.h"
#include "DB_PTTN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PTTN::CDB_PTTN()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nStartNum=1;
	m_nLastNum=0;
	m_pttn.InitHashTable(HASHSIZEPTTN);
	m_tdntCount2.InitHashTable(HASHSIZEPTTN);
}

CDB_PTTN::~CDB_PTTN()
{
}

void CDB_PTTN::Add(T_PTTN_K Key, T_PTTN_D& rData)
{
	T_PTTN_D Data;
	BOOL bExist=m_pttn.Lookup(Key, Data);

	//////////////////////////////////////////////////////////////////////////
	if (bExist)
	{
		// tdnt count 유지
		if (Data.TdntK != rData.TdntK)
		{
			int nCount;
			VERIFY(m_tdntCount2.Lookup(Data.TdntK, nCount));
			if (nCount == 1) m_tdntCount2.RemoveKey(Data.TdntK);
			else m_tdntCount2.SetAt(Data.TdntK, nCount - 1);
			if (!m_tdntCount2.Lookup(rData.TdntK, nCount)) nCount = 1;
			else nCount++;
			m_tdntCount2.SetAt(rData.TdntK, nCount);
		}
	}
	else
	{
		// tdnt count 유지
		int nCount;
		if (!m_tdntCount2.Lookup(rData.TdntK, nCount)) nCount = 1;
		else nCount++;
		m_tdntCount2.SetAt(rData.TdntK, nCount);
	}
	//////////////////////////////////////////////////////////////////////////

	m_pttn.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_PTTN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PTTN::Del(T_PTTN_K Key)
{
	T_PTTN_D Data;
	BOOL bExist=m_pttn.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pttn.RemoveKey(Key);
		ASSERT(ret);

		//////////////////////////////////////////////////////////////////////////
		// tdnt count 유지
		int nCount;
		VERIFY(m_tdntCount2.Lookup(Data.TdntK, nCount));
		if (nCount == 1) m_tdntCount2.RemoveKey(Data.TdntK);
		else m_tdntCount2.SetAt(Data.TdntK, nCount - 1);
		//////////////////////////////////////////////////////////////////////////

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PTTN_K key;
			T_PTTN_D data;
			if(m_pttn.Lookup(m_nLastNum-1,data))
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
		return ret;
	}
	return bExist;
}

BOOL CDB_PTTN::IsUsedTdnt(T_TDNT_K TdntKey)
{
	int nNum;
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->m_tdntCount2->Lookup(TdntKey, nNum);
	return m_tdntCount2.Lookup(TdntKey, nNum);
}