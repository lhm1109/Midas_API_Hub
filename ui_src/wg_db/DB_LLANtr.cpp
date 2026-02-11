#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_LLANtr.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LLANtr::CDB_LLANtr()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_llantr.InitHashTable(HASHSIZELLANtr);
}

CDB_LLANtr::~CDB_LLANtr()
{
}

void CDB_LLANtr::Add(T_LLANtr_K Key,T_LLANtr_D& rData,CDB_ELEM* pElem)
{
	T_LLANtr_D Data;
	BOOL bExist = m_llantr.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapNew;
	mapNew.InitHashTable(101);
	T_ELEM_K KeyElemTemp;
	if(bExist)
	{
		mapOld.InitHashTable(101);
		int nCount = Data.aLaneData.GetSize();
		for(int i=0; i<nCount; ++i) mapOld.SetAt(Data.aLaneData[i].ElemK, Data.aLaneData[i].ElemK);
		nCount = rData.aLaneData.GetSize();
		for(int i = 0; i<nCount; ++i) mapNew.SetAt(rData.aLaneData[i].ElemK, rData.aLaneData[i].ElemK);
		for(int i = 0; i<Data.aLaneData.GetSize(); ++i)
		{
			if(!mapNew.Lookup(Data.aLaneData[i].ElemK, KeyElemTemp))
				pElem->DelListItem(Data.aLaneData[i].ElemK, LT_LLANtr_CMD, Key);
		}
	}
	int nCount = rData.aLaneData.GetSize();
	for(int i=0; i<nCount; ++i)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.aLaneData[i].ElemK, KeyElemTemp))
			{
				pElem->AddListItem(rData.aLaneData[i].ElemK, LT_LLANtr_CMD, Key);
				mapOld.SetAt(rData.aLaneData[i].ElemK, rData.aLaneData[i].ElemK);
			}
		}
		else
		{
			if(!mapNew.Lookup(rData.aLaneData[i].ElemK, KeyElemTemp)) 
			{
				pElem->AddListItem(rData.aLaneData[i].ElemK, LT_LLANtr_CMD, Key);
				mapNew.SetAt(rData.aLaneData[i].ElemK, rData.aLaneData[i].ElemK);
			}
		}
	}
	m_llantr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_LLANtr_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum) m_nLastNum=Key;
}

BOOL CDB_LLANtr::Del(T_LLANtr_K Key,CDB_ELEM* pElem)
{
	T_LLANtr_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_llantr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.aLaneData.GetSize();
		for(int i=0; i<nCount; i++) pElem->DelListItem(data.aLaneData[i].ElemK, LT_LLANtr_CMD, Key);

		if(Key < m_nStartNum) m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LLANtr_K key;
			T_LLANtr_D data;
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
					if(key > m_nLastNum) m_nLastNum=key;
				}
			}
		}
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LLANtr::Get(T_LLANtr_K Key,T_LLANtr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_llantr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_llantr.Lookup(Key,rData);
}

int CDB_LLANtr::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_llantr->GetCount();
	}
	return m_llantr.GetCount();
}

POSITION CDB_LLANtr::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_llantr->GetStartPosition();
	}
	return m_llantr.GetStartPosition();
}

void CDB_LLANtr::GetNext(POSITION& rNextPosition,T_LLANtr_K& rKey,T_LLANtr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_llantr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_llantr.Lookup(rKey, rData);
		return;
	}
	m_llantr.GetNextAssoc(rNextPosition,rKey,rData);
}
