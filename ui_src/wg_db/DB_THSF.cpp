#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THSF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THSF::CDB_THSF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_thsf.InitHashTable(HASHSIZETHRG);
}

CDB_THSF::~CDB_THSF()
{

}

void CDB_THSF::Add(T_THSF_K Key,T_THSF_D& rData,CDB_THIS* pThis)
{
	T_THSF_D Data;
	BOOL bExist=m_thsf.Lookup(Key, Data);
	m_thsf.SetAt(Key,rData);
	//if(!bExist)pThis->AddListItem(rData.ThisK, LT_THSF_CMD, Key);
	if(Key == m_nStartNum)
	{
		T_THSF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_thsf.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THSF::Del(T_THSF_K Key,CDB_THIS* pThis)
{
	T_THSF_D Data;
	BOOL bExist=m_thsf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thsf.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			//VERIFY(pThis->DelListItem(Data.ThisK, LT_THSF_CMD, Key));
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THSF_K key;
			T_THSF_D data;
			if(m_thsf.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thsf.GetStartPosition();
				while(pos != NULL)
				{
					m_thsf.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THSF::Get(T_THSF_K Key,T_THSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thsf.Lookup(Key,rData);
}

int CDB_THSF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thsf.GetCount();
}

POSITION CDB_THSF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thsf.GetStartPosition();
}

void CDB_THSF::GetNext(POSITION& rNextPosition,T_THSF_K& rKey,T_THSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thsf.GetNextAssoc(rNextPosition,rKey,rData);
}

