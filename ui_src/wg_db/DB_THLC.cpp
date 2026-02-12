#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THLC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THLC::CDB_THLC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thlc.InitHashTable(HASHSIZETHLC);
}

CDB_THLC::~CDB_THLC()
{

}

void CDB_THLC::Add(T_THLC_K Key,T_THLC_D& rData,CDB_THIS* pThis)
{
	T_THLC_D Data;
	BOOL bExist=m_thlc.Lookup(Key, Data);
	m_thlc.SetAt(Key,rData);

	for(int i=0; i<rData.aThisK.GetSize(); i++)
		if(!bExist)pThis->AddListItem(rData.aThisK[i], LT_THLC_CMD, Key);

	if(Key == m_nStartNum)
	{
		T_THLC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_thlc.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THLC::Del(T_THLC_K Key,CDB_THIS* pThis)
{
	T_THLC_D Data;
	BOOL bExist=m_thlc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thlc.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			for(int i=0; i<Data.aThisK.GetSize(); i++)
				VERIFY(pThis->DelListItem(Data.aThisK[i], LT_THLC_CMD, Key));
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THLC_K key;
			T_THLC_D data;
			if(m_thlc.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thlc.GetStartPosition();
				while(pos != NULL)
				{
					m_thlc.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THLC::Get(T_THLC_K Key,T_THLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thlc.Lookup(Key,rData);
}

int CDB_THLC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thlc.GetCount();
}

POSITION CDB_THLC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thlc.GetStartPosition();
}

void CDB_THLC::GetNext(POSITION& rNextPosition,T_THLC_K& rKey,T_THLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thlc.GetNextAssoc(rNextPosition,rKey,rData);
}

