#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_TDNA.h"
#include "DB_TDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_TDNA::CDB_TDNA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdna.InitHashTable(HASHSIZETDNA);
	m_tdntCount.InitHashTable(HASHSIZETDNT);
}

CDB_TDNA::~CDB_TDNA()
{

}

void CDB_TDNA::Add(T_TDNA_K Key,T_TDNA_D& rData,CDB_ELEM* pElem,CDB_TDGR* pTdgr)
{
	T_TDNA_D Data;
	BOOL bExist=m_tdna.Lookup(Key, Data);

	if(bExist && Data.GroupKey != rData.GroupKey && pTdgr)
		VERIFY(pTdgr->DelListItem(Data.GroupKey, LT_TDNA_CMD, Key));

	CMap<T_ELEM_K, T_ELEM_K, int, int>mapElemOld;
	CMap<T_ELEM_K, T_ELEM_K, int, int>mapElemNew;
	int  nTemp;
	T_ELEM_K ElemKey;

	mapElemNew.InitHashTable(HASHSIZEELEM);
	for(int i = 0; i<rData.aElemList.GetSize(); i++)mapElemNew.SetAt(rData.aElemList[i], 0);
	// 501에서 없어짐
	//for(int i = 0; i<rData.aProfile.GetSize(); i++)
	//  if (rData.aProfile[i].nInputOption == 0)
	//    mapElemNew.SetAt(rData.aProfile[i].RefElemKey, 0);

	if(bExist)
	{
		mapElemOld.InitHashTable(HASHSIZEELEM);
		for(int i = 0; i<Data.aElemList.GetSize(); i++)mapElemOld.SetAt(Data.aElemList[i], 0);
		// 501에서 없어짐
		//for(int i = 0; i<Data.aProfile.GetSize(); i++)
		//  if (Data.aProfile[i].nInputOption == 0)
		//    mapElemOld.SetAt(Data.aProfile[i].RefElemKey, 0);
		
		POSITION pos = mapElemOld.GetStartPosition();
		while (pos)
		{
			mapElemOld.GetNextAssoc(pos, ElemKey, nTemp);
			if (!mapElemNew.Lookup(ElemKey, nTemp))
				pElem->DelListItem(ElemKey, LT_TDNA_CMD, Key);
		}

		// tdnt count 유지
		if (Data.TendonTypeKey != rData.TendonTypeKey)
		{
			int nCount;
			VERIFY(m_tdntCount.Lookup(Data.TendonTypeKey, nCount));
			if (nCount == 1) m_tdntCount.RemoveKey(Data.TendonTypeKey);
			else m_tdntCount.SetAt(Data.TendonTypeKey, nCount-1);
			if (!m_tdntCount.Lookup(rData.TendonTypeKey, nCount)) nCount=1;
			else nCount++;
			m_tdntCount.SetAt(rData.TendonTypeKey, nCount);
		}
	}
	else
	{
		// tdnt count 유지
		int nCount;
		if (!m_tdntCount.Lookup(rData.TendonTypeKey, nCount)) nCount=1;
		else nCount++;
		m_tdntCount.SetAt(rData.TendonTypeKey, nCount);
	}
	m_tdna.SetAt(Key,rData);

	if(bExist && Data.GroupKey != rData.GroupKey && pTdgr)
		pTdgr->AddListItem(rData.GroupKey, LT_TDNA_CMD, Key);
	if (!bExist)
	{
		if(pTdgr) pTdgr->AddListItem(rData.GroupKey, LT_TDNA_CMD, Key);
	}

	POSITION pos = mapElemNew.GetStartPosition();
	while (pos)
	{
		mapElemNew.GetNextAssoc(pos, ElemKey, nTemp);
		if (!mapElemOld.Lookup(ElemKey, nTemp))
			pElem->AddListItem(ElemKey, LT_TDNA_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_TDNA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_tdna.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDNA::Del(T_TDNA_K Key,CDB_ELEM* pElem,CDB_TDGR* pTdgr)
{
	T_TDNA_D Data;
	BOOL bExist=m_tdna.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_tdna.RemoveKey(Key);

		// tdnt count 유지
		int nCount;
		VERIFY(m_tdntCount.Lookup(Data.TendonTypeKey, nCount));
		if (nCount == 1) m_tdntCount.RemoveKey(Data.TendonTypeKey);
		else m_tdntCount.SetAt(Data.TendonTypeKey, nCount-1);

		T_ELEM_K ElemKey;
		int nTemp;
		CMap<T_ELEM_K, T_ELEM_K, int, int>mapElemOld;
		mapElemOld.InitHashTable(HASHSIZEELEM);
		for(int i = 0; i<Data.aElemList.GetSize(); i++)mapElemOld.SetAt(Data.aElemList[i], 0);
		// 501에서 없어짐
		//for(int i = 0; i<Data.aProfile.GetSize(); i++)
		//  if (Data.aProfile[i].nInputOption == 0)
		//    mapElemOld.SetAt(Data.aProfile[i].RefElemKey, 0);

		ASSERT(ret);
		if(ret)
		{
			POSITION pos = mapElemOld.GetStartPosition();
			while (pos)
			{
				mapElemOld.GetNextAssoc(pos, ElemKey, nTemp);
				pElem->DelListItem(ElemKey, LT_TDNA_CMD, Key);
			}

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_TDNA_K key;
				T_TDNA_D data;
				if(m_tdna.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_tdna.GetStartPosition();
					while(pos != NULL)
					{
						m_tdna.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}

			if(pTdgr) pTdgr->DelListItem(Data.GroupKey, LT_TDNA_CMD, Key);
		}
		return ret;
	}
	return bExist;
}

//------------------------------------------------------------------
BOOL CDB_TDNA::Get(T_TDNA_K Key,T_TDNA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT nTemp;
		if (!m_pDoc->m_pStagCtrl->m_tdna->Lookup(Key, nTemp)) return FALSE;
	}
	return m_tdna.Lookup(Key,rData);
}

int CDB_TDNA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_tdna->GetCount();
	}
	return m_tdna.GetCount();
}

int CDB_TDNA::GetCountFromBaseDB()
{
	return m_tdna.GetCount();
}
POSITION CDB_TDNA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_tdna->GetStartPosition();
	}
	return m_tdna.GetStartPosition();
}

void CDB_TDNA::GetNext(POSITION& rNextPosition,T_TDNA_K& rKey,T_TDNA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT nTemp;
		m_pDoc->m_pStagCtrl->m_tdna->GetNextAssoc(rNextPosition, rKey, nTemp);
		m_tdna.Lookup(rKey, rData);
		return;
	}
	m_tdna.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_TDNA::IsUsedTdnt(T_TDNT_K TdntKey)
{
	int nNum;
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->m_tdntCount->Lookup(TdntKey, nNum);
	return m_tdntCount.Lookup(TdntKey, nNum);
}
BOOL CDB_TDNA::GetFromBaseDB(T_TDNA_K Key,T_TDNA_D& rData)
{
	return m_tdna.Lookup(Key,rData);
}
POSITION CDB_TDNA::GetStartFromBaseDB()
{
	return m_tdna.GetStartPosition();
}
void CDB_TDNA::GetNextFromBaseDB(POSITION& rNextPosition,T_TDNA_K& rKey,T_TDNA_D& rData)
{
	m_tdna.GetNextAssoc(rNextPosition,rKey,rData);
}

