#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"
#include "DB_GIMP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GIMP::CDB_GIMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_gimp.InitHashTable(HASHSIZEGIMP);
	m_index.Initialize(HASHSIZEGIMP);

	m_nStartNum=1;
	m_nLastNum=0;
}

CDB_GIMP::~CDB_GIMP()
{

}

void CDB_GIMP::Add(T_GIMP_K Key,T_GIMP_D& rData,CDB_ELEM* pElem,CDB_NODE* pNode)
{
	T_GIMP_D Data;
	BOOL bExist=m_gimp.Lookup(Key, Data);
	m_gimp.SetAt(Key,rData);
	if (bExist)
	{
		m_index.Del(T_UINT16UINT16_INDEX_K(Data.nTgtType, Data.nTgtKey));
		if(Data.nTgtType == 0 && Data.nTgtKey != 0) pElem->DelListItem(Data.nTgtKey, LT_GIMP_CMD, Key);
		else if(Data.nTgtType == 1 && Data.nTgtKey != 0) pNode->DelListItem(Data.nTgtKey, LT_GIMP_CMD, Key);
	}
	if (rData.nTgtType == 0) pElem->AddListItem(rData.nTgtKey, LT_GIMP_CMD, Key);
	else if (rData.nTgtType == 1) pNode->AddListItem(rData.nTgtKey, LT_GIMP_CMD, Key);
	m_index.Add(T_UINT16UINT16_INDEX_K(rData.nTgtType, rData.nTgtKey), Key);

	if(Key == m_nStartNum)
	{
		T_GIMP_D data;
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
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_GIMP_CMD, Key);
	}
}

BOOL CDB_GIMP::Del(T_GIMP_K Key,CDB_ELEM* pElem,CDB_NODE* pNode)
{
	T_GIMP_D Data;
	m_gimp.Lookup(Key, Data);
	BOOL ret=m_gimp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (Data.nTgtType == 0)
			VERIFY(pElem->DelListItem(Data.nTgtKey, LT_GIMP_CMD, Key));
		else if (Data.nTgtType == 1)
			VERIFY(pNode->DelListItem(Data.nTgtKey, LT_GIMP_CMD, Key));
		m_index.Del(T_UINT16UINT16_INDEX_K(Data.nTgtType, Data.nTgtKey));

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GIMP_K key;
			T_GIMP_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_GIMP_CMD, Key);
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_GIMP::Get(T_GIMP_K Key,T_GIMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gimp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gimp.Lookup(Key,rData);
}

int CDB_GIMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gimp->GetCount();
	}
	return m_gimp.GetCount();
}

POSITION CDB_GIMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gimp->GetStartPosition();
	}
	return m_gimp.GetStartPosition();
}

void CDB_GIMP::GetNext(POSITION& rNextPosition,T_GIMP_K& rKey,T_GIMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gimp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gimp.Lookup(rKey, rData);
		return;
	}
	m_gimp.GetNextAssoc(rNextPosition,rKey,rData);
}
