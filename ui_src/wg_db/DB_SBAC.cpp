#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_SBAC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SBAC::CDB_SBAC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_sbac.InitHashTable(HASHSIZESBAC);
}

CDB_SBAC::~CDB_SBAC()
{
}

void CDB_SBAC::Add(T_SBAC_K Key,T_SBAC_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_SBAC_D Data;
	BOOL bExist=m_sbac.Lookup(Key, Data);
	if(bExist)
	{
		if(Data.nConstraintType == 0 || Data.nConstraintType == 1)
		{
			if(pNode != NULL) VERIFY(pNode->DelListItem(Data.ObjectID, LT_SBAC_CMD, Key));
			if(Data.nEqualityCond == 0 && !Data.bValue)
			{
				if(Data.ObjectID != Data.OtherObject)
					if(pNode != NULL) VERIFY(pNode->DelListItem(Data.OtherObject, LT_SBAC_CMD, Key));
			}
		}
		else if(Data.nConstraintType == 2 || Data.nConstraintType == 3)
		{
			if(pElem != NULL) VERIFY(pElem->DelListItem(Data.ObjectID, LT_SBAC_CMD, Key));
			if(Data.nEqualityCond == 0 && !Data.bValue)
			{
				if(Data.ObjectID != Data.OtherObject)
					if(pElem != NULL) VERIFY(pElem->DelListItem(Data.OtherObject, LT_SBAC_CMD, Key));
			}
		}
		else
			ASSERT(FALSE);
	}
	m_sbac.SetAt(Key,rData);
	if(rData.nConstraintType == 0 || rData.nConstraintType == 1)
	{
		if(pNode != NULL) pNode->AddListItem(rData.ObjectID, LT_SBAC_CMD, Key);
		if(rData.nEqualityCond == 0 && !rData.bValue)
		{
			if(rData.ObjectID != rData.OtherObject)
				if(pNode != NULL) pNode->AddListItem(rData.OtherObject, LT_SBAC_CMD, Key);
		}
	}
	else if(rData.nConstraintType == 2 || rData.nConstraintType == 3)
	{
		if(pElem != NULL) pElem->AddListItem(rData.ObjectID, LT_SBAC_CMD, Key);
		if(rData.nEqualityCond == 0 && !rData.bValue)
		{
			if(rData.ObjectID != rData.OtherObject)
				if(pElem != NULL) pElem->AddListItem(rData.OtherObject, LT_SBAC_CMD, Key);
		}
	}
	else
		ASSERT(FALSE);

	if(Key == m_nStartNum)
	{
		T_SBAC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

//   // Stage Mode에서도 Add/Del/Mod 될 수 있으므로
//   // Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
//   if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//   {
//     if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_SBAC_CMD, Key);
//   }
}

BOOL CDB_SBAC::Del(T_SBAC_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_SBAC_D Data;
	m_sbac.Lookup(Key, Data);
	BOOL ret=m_sbac.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Data.nConstraintType == 0 || Data.nConstraintType == 1)
		{
			if(pNode != NULL) VERIFY(pNode->DelListItem(Data.ObjectID, LT_SBAC_CMD, Key));
			if(Data.nEqualityCond == 0 && !Data.bValue)
			{
				if(Data.ObjectID != Data.OtherObject)
					if(pNode != NULL) VERIFY(pNode->DelListItem(Data.OtherObject, LT_SBAC_CMD, Key));
			}
		}
		else if(Data.nConstraintType == 2 || Data.nConstraintType == 3)
		{
			if(pElem != NULL) VERIFY(pElem->DelListItem(Data.ObjectID, LT_SBAC_CMD, Key));
			if(Data.nEqualityCond == 0 && !Data.bValue)
			{
				if(Data.ObjectID != Data.OtherObject)
					if(pElem != NULL) VERIFY(pElem->DelListItem(Data.OtherObject, LT_SBAC_CMD, Key));
			}
		}
		else
			ASSERT(FALSE);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SBAC_K key;
			T_SBAC_D data;
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
//     // Stage Mode에서도 Add/Del/Mod 될 수 있으므로
//     // Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
//     if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//     {
//       m_pDoc->m_pStagCtrl->DeactiveEtc(LT_SBAC_CMD, Key);
//     }
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SBAC::Get(T_SBAC_K Key,T_SBAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;

//   if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//   {
//     if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
//     UINT uTemp;
//     if (!m_pDoc->m_pStagCtrl->m_sbac->Lookup(Key, uTemp)) return FALSE;
//   }
	return m_sbac.Lookup(Key,rData);
}

int CDB_SBAC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;

//   if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag())
//   {
//     if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
//     return m_pDoc->m_pStagCtrl->m_sbac->GetCount();
//   }
	return m_sbac.GetCount();
}

POSITION CDB_SBAC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return NULL;

//   if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//   {
//     if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
//     return m_pDoc->m_pStagCtrl->m_sbac->GetStartPosition();
//   }
	return m_sbac.GetStartPosition();
}

void CDB_SBAC::GetNext(POSITION& rNextPosition,T_SBAC_K& rKey,T_SBAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		rNextPosition = NULL;
		rKey = 0;
		rData.Initialize();

		return;
	}

//   if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//   {
//     if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
//     UINT uTemp;
//     m_pDoc->m_pStagCtrl->m_sbac->GetNextAssoc(rNextPosition, rKey, uTemp);
//     m_sbac.Lookup(rKey, rData);
//     return;
//   }
	m_sbac.GetNextAssoc(rNextPosition,rKey,rData);
}
