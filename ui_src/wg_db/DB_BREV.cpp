#include "stdafx.h"
#include "DB_BREV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BREV::CDB_BREV()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_brev.InitHashTable(HASHSIZEBREV);
	m_nStartNum = 1;
	m_nLastNum  = 0;
}

CDB_BREV::~CDB_BREV()
{

}

void CDB_BREV::Add(T_BREV_K Key,T_BREV_D& rData)
{
	m_brev.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_BREV_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_brev.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_BREV::Del(T_BREV_K Key)
{
	BOOL ret=m_brev.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_BREV_K key;
			T_BREV_D data;
			if(m_brev.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_brev.GetStartPosition();
				while(pos != NULL)
				{
					m_brev.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
			
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_BREV::Get(T_BREV_K Key,T_BREV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_brev.Lookup(Key,rData);
}

int CDB_BREV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_brev.GetCount();
}

POSITION CDB_BREV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_brev.GetStartPosition();
}

void CDB_BREV::GetNext(POSITION& rNextPosition,T_BREV_K& rKey,T_BREV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_brev.GetNextAssoc(rNextPosition,rKey,rData);
}

