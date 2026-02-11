#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_WVLD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_WVLD::CDB_WVLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_wvld.InitHashTable(HASHSIZEWVLD);
}

CDB_WVLD::~CDB_WVLD()
{

}

void CDB_WVLD::Add(T_WVLD_K Key,T_WVLD_D& rData,CDB_THIS* pThis)
{
	T_WVLD_D TempData;
	BOOL bExist=m_wvld.Lookup(Key, TempData);
	m_wvld.SetAt(Key,rData);  

	if (bExist && TempData.bThis)
		pThis->DelListItem(rData.ThisK, LT_WVLD_CMD, Key);
	if (rData.bThis)
		pThis->AddListItem(rData.ThisK, LT_WVLD_CMD, Key); 

	if(Key == m_nStartNum)
	{
		//T_WVLD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,TempData))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_WVLD::Del(T_WVLD_K Key,CDB_THIS* pThis)
{
	T_WVLD_D TempData;
	BOOL bExist=m_wvld.Lookup(Key, TempData);
	if(bExist)
	{
		BOOL ret=m_wvld.RemoveKey(Key);
		ASSERT(ret);    
		if(ret)
		{
			if(TempData.bThis)
				VERIFY(pThis->DelListItem(TempData.ThisK, LT_WVLD_CMD, Key));  // time history load case를 사용하는 경우만 ...
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_WVLD_K key;
				T_WVLD_D data;
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
BOOL CDB_WVLD::Get(T_WVLD_K Key,T_WVLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_wvld.Lookup(Key,rData);
}
int CDB_WVLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wvld.GetCount();
}

POSITION CDB_WVLD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wvld.GetStartPosition();
}

void CDB_WVLD::GetNext(POSITION& rNextPosition,T_WVLD_K& rKey,T_WVLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_wvld.GetNextAssoc(rNextPosition,rKey,rData);
}
