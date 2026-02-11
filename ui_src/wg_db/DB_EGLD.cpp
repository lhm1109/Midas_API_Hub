#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_EGLD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EGLD::CDB_EGLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;

	m_egld.InitHashTable(HASHSIZEEGLD);
}

CDB_EGLD::~CDB_EGLD()
{

}

void CDB_EGLD::Add(T_EGLD_K Key,T_EGLD_D& rData,CDB_STLD* pStld)
{
	T_EGLD_D Data;
	BOOL bExist=m_egld.Lookup(Key, Data);
	m_egld.SetAt(Key,rData);
	if(!bExist)
	{
		if(rData.nLoadCaseType == D_LOADCASE_STATIC)
			pStld->AddListItem(rData.StldKey, LT_EGLD_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_EGLD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_EGLD::Del(T_EGLD_K Key,CDB_STLD* pStld)
{
	T_EGLD_D Data;
	BOOL bExist=m_egld.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_egld.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Data.nLoadCaseType == D_LOADCASE_STATIC)
				VERIFY(pStld->DelListItem(Data.StldKey, LT_EGLD_CMD, Key));

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_EGLD_K key;
				T_EGLD_D data;
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
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_EGLD::Get(T_EGLD_K Key,T_EGLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_egld.Lookup(Key,rData);
}

int CDB_EGLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_egld.GetCount();
}

POSITION CDB_EGLD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_egld.GetStartPosition();
}

void CDB_EGLD::GetNext(POSITION& rNextPosition,T_EGLD_K& rKey,T_EGLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_egld.GetNextAssoc(rNextPosition,rKey,rData);
}
