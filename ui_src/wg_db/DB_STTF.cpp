#include "stdafx.h"
#include "DB_STTF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STTF::CDB_STTF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_sttf.InitHashTable(HASHSIZESTTF);
}

CDB_STTF::~CDB_STTF()
{
}

void CDB_STTF::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_nStartNum=1;
	m_nLastNum=0;
	m_sttf.RemoveAll();
}

void CDB_STTF::Add(T_STTF_K Key,T_STTF_D& rData)
{
	m_sttf.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_STTF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_STTF::Del(T_STTF_K Key)
{
	BOOL ret=m_sttf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_STTF_K key;
			T_STTF_D data;
			if(Get(m_nLastNum-1,data))
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
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STTF::Get(T_STTF_K Key,T_STTF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_sttf.Lookup(Key,rData);
}

int CDB_STTF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_sttf.GetCount();
}

POSITION CDB_STTF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_sttf.GetStartPosition();
}

void CDB_STTF::GetNext(POSITION& rNextPosition,T_STTF_K& rKey,T_STTF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_sttf.GetNextAssoc(rNextPosition,rKey,rData);
}


