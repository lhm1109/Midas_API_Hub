#include "stdafx.h"
#include "DB_MVHLtr.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVHLtr::CDB_MVHLtr()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvhltr.InitHashTable(HASHSIZEMVHLtr);
}

CDB_MVHLtr::~CDB_MVHLtr()
{
}

void CDB_MVHLtr::Add(T_MVHLtr_K Key,T_MVHLtr_D& rData)
{
	m_mvhltr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVHLtr_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVHLtr::Del(T_MVHLtr_K Key)
{
	BOOL ret=m_mvhltr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVHLtr_K key;
			T_MVHLtr_D data;
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
BOOL CDB_MVHLtr::Get(T_MVHLtr_K Key,T_MVHLtr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvhltr.Lookup(Key,rData);
}

int CDB_MVHLtr::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvhltr.GetCount();
}

POSITION CDB_MVHLtr::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvhltr.GetStartPosition();
}

void CDB_MVHLtr::GetNext(POSITION& rNextPosition,T_MVHLtr_K& rKey,T_MVHLtr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_mvhltr.GetNextAssoc(rNextPosition,rKey,rData);
}
