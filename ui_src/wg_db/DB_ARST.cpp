#include "stdafx.h"
#include "DB_ARST.h"

#include "DBDoc.h"
#include "DB_MATL.h"
#include "DB_RFST.h"
#include "DB_ELEM.h"
#include "StagCtrl.h"

CDB_ARST::CDB_ARST()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_arst.InitHashTable(HASHSIZEARST);
	m_rfstCount.InitHashTable(HASHSIZERFST);
}

CDB_ARST::~CDB_ARST()
{

}

void CDB_ARST::Add(T_ARST_K Key, T_ARST_D& rData, CDB_MATL* pMatl, CDB_RFST* pRfst, CDB_ELEM* pElem)
{
	T_ARST_D data;
	int nRfstNum;
	if(m_arst.Lookup(Key, data))
	{
		//if(pMatl)
		//{
		//	VERIFY(pMatl->DelListItem(data.matlK, LT_ARST_CMD, Key));
		//}
		if(pRfst)
		{
			VERIFY(pRfst->DelListItem(data.ReinforceSectKey, LT_ARST_CMD, Key));
		}
		if(m_rfstCount.Lookup(data.ReinforceSectKey, nRfstNum))
		{
			if(nRfstNum == 1) 
				m_rfstCount.RemoveKey(data.ReinforceSectKey);
			else 
				m_rfstCount.SetAt(data.ReinforceSectKey, nRfstNum - 1);
		}
	}
	m_arst.SetAt(Key, rData);
	//if (pMatl)	pMatl->AddListItem(rData.matlK, LT_ARST_CMD, Key);
	if (pRfst)	pRfst->AddListItem(rData.ReinforceSectKey, LT_ARST_CMD, Key);
	if (pElem)	pElem->AddListItem(Key, LT_ARST_CMD, Key);
	if (m_rfstCount.Lookup(rData.ReinforceSectKey, nRfstNum))
		m_rfstCount.SetAt(rData.ReinforceSectKey, nRfstNum + 1);
	else 
		m_rfstCount.SetAt(rData.ReinforceSectKey, 1);
}

BOOL CDB_ARST::Del(T_ARST_K Key, CDB_MATL* pMatl, CDB_RFST* pRfst, CDB_ELEM* pElem)
{
	int nRfstNum;
	T_ARST_D Data;
	BOOL bExist = m_arst.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret = m_arst.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			//if (pMatl)	VERIFY(pMatl->DelListItem(Data.matlK, LT_ARST_CMD, Key));
			if (pRfst)	VERIFY(pRfst->DelListItem(Data.ReinforceSectKey, LT_ARST_CMD, Key));
			if (pElem)	VERIFY(pElem->DelListItem(Key, LT_ARST_CMD, Key));
			if(m_rfstCount.Lookup(Data.ReinforceSectKey, nRfstNum))
			{
				if(nRfstNum == 1)
					m_rfstCount.RemoveKey(Data.ReinforceSectKey);
				else 
					m_rfstCount.SetAt(Data.ReinforceSectKey, nRfstNum - 1);
			}
			else 
				ASSERT(0);
		}
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_ARST::Get(T_ARST_K Key,T_ARST_D& rData)
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_arst.Lookup(Key, rData);
}

POSITION CDB_ARST::GetStart()
{
	//if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	return m_pDoc->m_pStagCtrl->m_arst->GetStartPosition();
	//}
	return m_arst.GetStartPosition();
}

void CDB_ARST::GetNext(POSITION& rNextPosition, T_ARST_K& rKey, T_ARST_D& rData)
{
	m_arst.GetNextAssoc(rNextPosition, rKey, rData);
}

BOOL CDB_ARST::IsUsedRfst(T_RFST_K RfstKey)
{
	int nNum;
	return m_rfstCount.Lookup(RfstKey, nNum);
}

int CDB_ARST::GetCount()
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_arst.GetCount();
}
