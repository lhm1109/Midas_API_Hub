#include "stdafx.h"
#include "DB_NMDV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NMDV::CDB_NMDV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_nmdv.InitHashTable(HASHSIZENMDV);
}

CDB_NMDV::~CDB_NMDV()
{
}

// PostCtrl에서 복사본 만들어 사용할 때 사용
void CDB_NMDV::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_nStartNum=1;
	m_nLastNum=0;
	m_nmdv.RemoveAll();
}

void CDB_NMDV::Add(T_NMDV_K Key, T_NMDV_D& rData)
{
	m_nmdv.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_NMDV_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_NMDV::Del(T_NMDV_K Key)
{
	BOOL ret=m_nmdv.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if (Key < m_nStartNum)m_nStartNum=Key;
		if (Key==m_nLastNum)
		{
			T_NMDV_K key;
			T_NMDV_D data;
			if (Get(m_nLastNum-1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while (pos != NULL)
				{
					GetNext(pos, key, data);
					if (key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_NMDV::Get(T_NMDV_K Key, T_NMDV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_nmdv.Lookup(Key, rData);
}

int CDB_NMDV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_nmdv.GetCount();
}

POSITION CDB_NMDV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_nmdv.GetStartPosition();
}

void CDB_NMDV::GetNext(POSITION& rNextPosition, T_NMDV_K& rKey, T_NMDV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_nmdv.GetNextAssoc(rNextPosition, rKey, rData);
}
