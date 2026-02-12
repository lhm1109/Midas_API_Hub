#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_GSTY.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GSTY::CDB_GSTY()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_gsty.InitHashTable(HASHSIZEGSTY);
	m_index.Initialize(HASHSIZEGSTY);

	m_nStartNum=1;
	m_nLastNum=0;
}

CDB_GSTY::~CDB_GSTY()
{

}

void CDB_GSTY::Add(T_GSTY_K Key,T_GSTY_D& rData,CDB_ELEM* pElem)
{
	T_GSTY_D Data;
	BOOL bExist=m_gsty.Lookup(Key, Data);
	m_gsty.SetAt(Key,rData);
	if (bExist)
	{
		m_index.Del(T_UINT16UINT16_INDEX_K(Data.nTgtType, Data.nTgtKey));
		if (Data.nTgtKey == 0) pElem->DelListItem(Data.nTgtKey, LT_GSTY_CMD, Key);
	}
	if (rData.nTgtType == 0) pElem->AddListItem(rData.nTgtKey, LT_GSTY_CMD, Key);
	m_index.Add(T_UINT16UINT16_INDEX_K(rData.nTgtType, rData.nTgtKey), Key);

	if(Key == m_nStartNum)
	{
		T_GSTY_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_GSTY_CMD, Key);
	}
}

BOOL CDB_GSTY::Del(T_GSTY_K Key,CDB_ELEM* pElem)
{
	T_GSTY_D Data;
	m_gsty.Lookup(Key, Data);
	BOOL ret=m_gsty.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (Data.nTgtType == 0)
			VERIFY(pElem->DelListItem(Data.nTgtKey, LT_GSTY_CMD, Key));
		m_index.Del(T_UINT16UINT16_INDEX_K(Data.nTgtType, Data.nTgtKey));

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GSTY_K key;
			T_GSTY_D data;
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

		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_GSTY_CMD, Key);
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_GSTY::Get(T_GSTY_K Key,T_GSTY_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gsty->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gsty.Lookup(Key,rData);
}

int CDB_GSTY::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gsty->GetCount();
	}
	return m_gsty.GetCount();
}

POSITION CDB_GSTY::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gsty->GetStartPosition();
	}
	return m_gsty.GetStartPosition();
}

void CDB_GSTY::GetNext(POSITION& rNextPosition,T_GSTY_K& rKey,T_GSTY_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gsty->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gsty.Lookup(rKey, rData);
		return;
	}
	m_gsty.GetNextAssoc(rNextPosition,rKey,rData);
}
