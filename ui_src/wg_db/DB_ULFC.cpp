#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_ULFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ULFC::CDB_ULFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ulfc.InitHashTable(HASHSIZEULFC);
}

CDB_ULFC::~CDB_ULFC()
{
}

void CDB_ULFC::Add(T_ULFC_K Key,T_ULFC_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_ULFC_D Data;
	BOOL bExist=m_ulfc.Lookup(Key, Data);
	if(bExist)
	{
		if(Data.nConstraintType == 0 || Data.nConstraintType == 1)
		{
			if(pNode != NULL) VERIFY(pNode->DelListItem(Data.ObjectID, LT_ULFC_CMD, Key));
			if(Data.nEqualityCond == 0 && !Data.bValue)
			{
				if(Data.ObjectID != Data.OtherObject)
					if(pNode != NULL) VERIFY(pNode->DelListItem(Data.OtherObject, LT_ULFC_CMD, Key));
			}
		}
		else if(Data.nConstraintType == 2 || Data.nConstraintType == 3)
		{
			if(pElem != NULL) VERIFY(pElem->DelListItem(Data.ObjectID, LT_ULFC_CMD, Key));
			if(Data.nEqualityCond == 0 && !Data.bValue)
			{
				if(Data.ObjectID != Data.OtherObject)
					if(pElem != NULL) VERIFY(pElem->DelListItem(Data.OtherObject, LT_ULFC_CMD, Key));
			}
		}
		else
			ASSERT(FALSE);
	}
	m_ulfc.SetAt(Key,rData);
	if(rData.nConstraintType == 0 || rData.nConstraintType == 1)
	{
		if(pNode != NULL) pNode->AddListItem(rData.ObjectID, LT_ULFC_CMD, Key);
		if(rData.nEqualityCond == 0 && !rData.bValue)
		{
			if(rData.ObjectID != rData.OtherObject)
				if(pNode != NULL) pNode->AddListItem(rData.OtherObject, LT_ULFC_CMD, Key);
		}
	}
	else if(rData.nConstraintType == 2 || rData.nConstraintType == 3)
	{
		if(pElem != NULL) pElem->AddListItem(rData.ObjectID, LT_ULFC_CMD, Key);
		if(rData.nEqualityCond == 0 && !rData.bValue)
		{
			if(rData.ObjectID != rData.OtherObject)
				if(pElem != NULL) pElem->AddListItem(rData.OtherObject, LT_ULFC_CMD, Key);
		}
	}
	else
		ASSERT(FALSE);

	if(Key == m_nStartNum)
	{
		T_ULFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_ULFC_CMD, Key);
	}
}

BOOL CDB_ULFC::Del(T_ULFC_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_ULFC_D Data;
	m_ulfc.Lookup(Key, Data);
	BOOL ret=m_ulfc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Data.nConstraintType == 0 || Data.nConstraintType == 1)
		{
			if(pNode != NULL) VERIFY(pNode->DelListItem(Data.ObjectID, LT_ULFC_CMD, Key));
			if(Data.nEqualityCond == 0 && !Data.bValue)
			{
				if(Data.ObjectID != Data.OtherObject)
					if(pNode != NULL) VERIFY(pNode->DelListItem(Data.OtherObject, LT_ULFC_CMD, Key));
			}
		}
		else if(Data.nConstraintType == 2 || Data.nConstraintType == 3)
		{
			if(pElem != NULL) VERIFY(pElem->DelListItem(Data.ObjectID, LT_ULFC_CMD, Key));
			if(Data.nEqualityCond == 0 && !Data.bValue)
			{
				if(Data.ObjectID != Data.OtherObject)
					if(pElem != NULL) VERIFY(pElem->DelListItem(Data.OtherObject, LT_ULFC_CMD, Key));
			}
		}
		else
			ASSERT(FALSE);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ULFC_K key;
			T_ULFC_D data;
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
		// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_ULFC_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ULFC::Get(T_ULFC_K Key,T_ULFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ulfc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ulfc.Lookup(Key,rData);
}

int CDB_ULFC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag())
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ulfc->GetCount();
	}
	return m_ulfc.GetCount();
}

POSITION CDB_ULFC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ulfc->GetStartPosition();
	}
	return m_ulfc.GetStartPosition();
}

void CDB_ULFC::GetNext(POSITION& rNextPosition,T_ULFC_K& rKey,T_ULFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ulfc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ulfc.Lookup(rKey, rData);
		return;
	}
	m_ulfc.GetNextAssoc(rNextPosition,rKey,rData);
}
