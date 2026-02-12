#include "stdafx.h"
#include "DB_MVHC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVHC::CDB_MVHC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvhc.InitHashTable(HASHSIZEMVHC);
}

CDB_MVHC::~CDB_MVHC()
{
}

// PostCtrl에서 복사본 만들어 사용할 때 사용
void CDB_MVHC::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvhc.RemoveAll();
}

void CDB_MVHC::Add(T_MVHC_K Key,T_MVHC_D& rData)
{
	m_mvhc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVHC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVHC::Del(T_MVHC_K Key)
{
	BOOL ret=m_mvhc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVHC_K key;
			T_MVHC_D data;
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
BOOL CDB_MVHC::Get(T_MVHC_K Key,T_MVHC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvhc.Lookup(Key,rData);
}

int CDB_MVHC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvhc.GetCount();
}

POSITION CDB_MVHC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvhc.GetStartPosition();
}

void CDB_MVHC::GetNext(POSITION& rNextPosition,T_MVHC_K& rKey,T_MVHC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_mvhc.GetNextAssoc(rNextPosition,rKey,rData);
}
