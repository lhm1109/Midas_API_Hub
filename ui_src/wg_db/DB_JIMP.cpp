#include "stdafx.h" 
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"
#include "DB_JIMP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_JIMP::CDB_JIMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_jimp.InitHashTable(HASHSIZEJIMP);
	m_index.Initialize(HASHSIZEJIMP);

	m_nStartNum=1;
	m_nLastNum=0;
}

CDB_JIMP::~CDB_JIMP()
{

}

void CDB_JIMP::Add(T_JIMP_K Key,T_JIMP_D& rData,CDB_ELEM* pElem,CDB_NODE* pNode)
{
	T_JIMP_D Data;
	BOOL bExist=m_jimp.Lookup(Key, Data);
	m_jimp.SetAt(Key,rData);
	if (bExist)
	{
		m_index.Del(T_UINT3_INDEX_K(Data.nTgtType, Data.nTgtKey, Data.nFactorType, CHashKeySet::UINT3_2b22b8b));
		if (rData.nFactorType == 0)
		{
			if     (Data.nTgtType == 0 && Data.nTgtKey != 0) pElem->DelListItem(Data.nTgtKey, LT_JIMP_CMD, Key);
			else if(Data.nTgtType == 1 && Data.nTgtKey != 0) pNode->DelListItem(Data.nTgtKey, LT_JIMP_CMD, Key);
		}
	}

	if (rData.nFactorType == 0)
	{
		if      (rData.nTgtType == 0) pElem->AddListItem(rData.nTgtKey, LT_JIMP_CMD, Key);
		else if (rData.nTgtType == 1) pNode->AddListItem(rData.nTgtKey, LT_JIMP_CMD, Key);
	}
	m_index.Add(T_UINT3_INDEX_K(rData.nTgtType, rData.nTgtKey, rData.nFactorType, CHashKeySet::UINT3_2b22b8b), Key);

	if(Key == m_nStartNum)
	{
		T_JIMP_D data;
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
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_JIMP_CMD, Key);
	}
}

BOOL CDB_JIMP::Del(T_JIMP_K Key,CDB_ELEM* pElem,CDB_NODE* pNode)
{
	T_JIMP_D Data;
	m_jimp.Lookup(Key, Data);
	BOOL ret=m_jimp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (Data.nFactorType == 0)
		{
			if (Data.nTgtType == 0)
				VERIFY(pElem->DelListItem(Data.nTgtKey, LT_JIMP_CMD, Key));
			else if (Data.nTgtType == 1)
				VERIFY(pNode->DelListItem(Data.nTgtKey, LT_JIMP_CMD, Key));
		}
		m_index.Del(T_UINT3_INDEX_K(Data.nTgtType, Data.nTgtKey, Data.nFactorType, CHashKeySet::UINT3_2b22b8b));

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_JIMP_K key;
			T_JIMP_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_JIMP_CMD, Key);
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_JIMP::Get(T_JIMP_K Key,T_JIMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_jimp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_jimp.Lookup(Key,rData);
}

int CDB_JIMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_jimp->GetCount();
	}
	return m_jimp.GetCount();
}

POSITION CDB_JIMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_jimp->GetStartPosition();
	}
	return m_jimp.GetStartPosition();
}

void CDB_JIMP::GetNext(POSITION& rNextPosition,T_JIMP_K& rKey,T_JIMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_jimp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_jimp.Lookup(rKey, rData);
		return;
	}
	m_jimp.GetNextAssoc(rNextPosition,rKey,rData);
}
