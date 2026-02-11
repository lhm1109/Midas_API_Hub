#include "stdafx.h"
#include "DB_LLANjp.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LLANjp::CDB_LLANjp()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_llanjp.InitHashTable(HASHSIZELLANjp);
}

CDB_LLANjp::~CDB_LLANjp()
{

}

void CDB_LLANjp::Add(T_LLANjp_K Key,T_LLANjp_D& rData,CDB_ELEM* pElem)
{
	T_LLANjp_D Data;
	BOOL bExist= m_llanjp.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapNew;
	mapNew.InitHashTable(101);
	T_ELEM_K KeyElemTemp;
	if(bExist)
	{
		//int nCount=Data.arLaneData.GetSize();
		//for(int i=0; i<nCount; i++)pElem->DelListItem(Data.arLaneData[i].KeyElem, LT_LLAN_CMD, Key);
		mapOld.InitHashTable(101);
		int nCount=Data.arLaneData.GetSize();
		for(int i=0; i<nCount; i++)mapOld.SetAt(Data.arLaneData[i].KeyElem, Data.arLaneData[i].KeyElem);
		nCount=rData.arLaneData.GetSize();
		for(int i = 0; i<nCount; i++)mapNew.SetAt(rData.arLaneData[i].KeyElem, rData.arLaneData[i].KeyElem);
		for(int i = 0; i<Data.arLaneData.GetSize(); i++)
		{
			if(!mapNew.Lookup(Data.arLaneData[i].KeyElem, KeyElemTemp))
				pElem->DelListItem(Data.arLaneData[i].KeyElem, LT_LLANjp_CMD, Key);
		}
	}
	int nCount=rData.arLaneData.GetSize();
	for(int i=0; i<nCount; i++)
	{
		//pElem->AddListItem(rData.arLaneData[i].KeyElem, LT_LLAN_CMD, Key);
		if(bExist)
		{
			if(!mapOld.Lookup(rData.arLaneData[i].KeyElem, KeyElemTemp))
			{
				pElem->AddListItem(rData.arLaneData[i].KeyElem, LT_LLANjp_CMD, Key);
				mapOld.SetAt(rData.arLaneData[i].KeyElem, rData.arLaneData[i].KeyElem);
			}
		}
		else
		{
			if(!mapNew.Lookup(rData.arLaneData[i].KeyElem, KeyElemTemp)) 
			{
				pElem->AddListItem(rData.arLaneData[i].KeyElem, LT_LLANjp_CMD, Key);
				mapNew.SetAt(rData.arLaneData[i].KeyElem, rData.arLaneData[i].KeyElem);
			}
		}
	}
	m_llanjp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_LLANjp_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_LLANjp::Del(T_LLANjp_K Key,CDB_ELEM* pElem)
{
	T_LLANjp_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret= m_llanjp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.arLaneData.GetSize();
		for(int i=0; i<nCount; i++)pElem->DelListItem(data.arLaneData[i].KeyElem, LT_LLANjp_CMD, Key);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LLANjp_K key;
			T_LLANjp_D data;
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

//-------------------------------------------------------------------------
BOOL CDB_LLANjp::Get(T_LLANjp_K Key,T_LLANjp_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_llanjp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_llanjp.Lookup(Key,rData);
}

int CDB_LLANjp::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_llanjp->GetCount();
	}
	return m_llanjp.GetCount();
}

POSITION CDB_LLANjp::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_llanjp->GetStartPosition();
	}
	return m_llanjp.GetStartPosition();
}

void CDB_LLANjp::GetNext(POSITION& rNextPosition,T_LLANjp_K& rKey,T_LLANjp_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_llanjp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_llanjp.Lookup(rKey, rData);
		return;
	}
	m_llanjp.GetNextAssoc(rNextPosition,rKey,rData);
}
