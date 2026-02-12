#include "stdafx.h"
#include "DB_SPLC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPLC::CDB_SPLC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_splc.InitHashTable(HASHSIZESPLC);
}

CDB_SPLC::~CDB_SPLC()
{
}

// PostCtrl에서 복사본 만들어 사용할 때 사용
void CDB_SPLC::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_nStartNum=1;
	m_nLastNum=0;
	m_splc.RemoveAll();
}

void CDB_SPLC::Add(T_SPLC_K Key,T_SPLC_D& rData)
{
	m_splc.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_SPLC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SPLC::Del(T_SPLC_K Key)
{
	BOOL ret=m_splc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SPLC_K key;
			T_SPLC_D data;
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
BOOL CDB_SPLC::Get(T_SPLC_K Key,T_SPLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_splc.Lookup(Key,rData);
}

int CDB_SPLC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_splc.GetCount();
}

POSITION CDB_SPLC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_splc.GetStartPosition();
}

void CDB_SPLC::GetNext(POSITION& rNextPosition,T_SPLC_K& rKey,T_SPLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_splc.GetNextAssoc(rNextPosition,rKey,rData);
}
