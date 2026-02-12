#include "stdafx.h"
#include "DB_MTCS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MTCS::CDB_MTCS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mtcs.InitHashTable(HASHSIZEMTCS);
	m_mtcslist.InitHashTable(HASHSIZELISTMTCS);
}

CDB_MTCS::CDB_MTCS(CDB_MTCS& src)
{
	*this = src;
}

CDB_MTCS& CDB_MTCS::operator= (CDB_MTCS& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;
	m_pDoc = 0;  // 복사되는 것은 Stage와 무관하므로 NULL로 설정

	T_MTCS_K KeyMtcs;
	T_MTCS_D DataMtcs;
	m_mtcs.RemoveAll();
	pos=src.m_mtcs.GetStartPosition();
	while(pos != NULL)
	{
		src.m_mtcs.GetNextAssoc(pos,KeyMtcs,DataMtcs);
		m_mtcs.SetAt(KeyMtcs, DataMtcs);
	}

	DelAllList();
	MtcsList* pList;
	pos=src.m_mtcslist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_mtcslist.GetNextAssoc(pos,KeyMtcs,pList);
		MtcsList* pListNew = new MtcsList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_mtcslist.SetAt(KeyMtcs, pListNew);
	}

	return *this;
}

CDB_MTCS::~CDB_MTCS()
{
	DelAllList();
}

void CDB_MTCS::Add(T_MTCS_K Key,T_MTCS_D& rData)
{
	m_mtcs.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MTCS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MTCS::Del(T_MTCS_K Key)
{
	BOOL ret=m_mtcs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MTCS_K key;
			T_MTCS_D data;
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

void CDB_MTCS::AddList(T_MTCS_K Key)
{
	MtcsList* pList = new MtcsList;
	m_mtcslist.SetAt(Key, pList);
}

BOOL CDB_MTCS::DelList(T_MTCS_K Key)
{
	MtcsList* pList;
	BOOL bExist=m_mtcslist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_mtcslist.RemoveKey(Key);

	delete pList;

	return TRUE;
}

void CDB_MTCS::DelAllList()
{
	T_MTCS_K Key;
	MtcsList* pList;
	POSITION pos=m_mtcslist.GetStartPosition();
	while(pos != NULL)
	{
		m_mtcslist.GetNextAssoc(pos,Key,pList);

		delete pList;
	}
	m_mtcslist.RemoveAll();
}

void CDB_MTCS::AddListItem(T_MTCS_K Key, int nCmd, int nKey)
{
	MtcsList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_mtcslist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	POSITION pos = pList->AddTail(ListData);
}

BOOL CDB_MTCS::DelListItem(T_MTCS_K Key, int nCmd, int nKey)
{
	MtcsList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_mtcslist.Lookup(Key, pList));

	BOOL bDeleted = FALSE;

	T_LIST_DATA ListData;
	//POSITION pos=pList->GetHeadPosition(), posPrv;
	POSITION pos=pList->GetTailPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		//ListData=pList->GetNext(pos);
		ListData=pList->GetPrev(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
			break;
		}
	}
	if (!bDeleted) return FALSE;

	return TRUE;
}

void CDB_MTCS::AddListItem(CArray<T_MTCS_K, T_MTCS_K>& aNewMtcsK, 
													 CArray<T_MTCS_K, T_MTCS_K>& aOldMtcsK,
													 int nCmd, int nKey, BOOL bExist)
{
	CMap<T_MTCS_K, T_MTCS_K, T_MTCS_K, T_MTCS_K>mapOld;
	CMap<T_MTCS_K, T_MTCS_K, T_MTCS_K, T_MTCS_K>mapNew;
	T_MTCS_K KeyMtcsTemp;
	if (bExist)
	{
		mapNew.InitHashTable(997);
		mapOld.InitHashTable(997);
		for(int i = 0; i<aOldMtcsK.GetSize(); i++)mapOld.SetAt(aOldMtcsK[i], aOldMtcsK[i]);
		for(int i = 0; i<aNewMtcsK.GetSize(); i++)mapNew.SetAt(aNewMtcsK[i], aNewMtcsK[i]);
		for(int i = 0; i<aOldMtcsK.GetSize(); i++)
		{
			if(!mapNew.Lookup(aOldMtcsK[i], KeyMtcsTemp))
				VERIFY(DelListItem(aOldMtcsK[i], nCmd, nKey));
		}
	}
	for(int i=0; i<aNewMtcsK.GetSize(); i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(aNewMtcsK[i], KeyMtcsTemp))
				AddListItem(aNewMtcsK[i], nCmd, nKey);
		}
		else
			AddListItem(aNewMtcsK[i], nCmd, nKey);
	}
}

BOOL CDB_MTCS::DelListItem(CArray<T_MTCS_K, T_MTCS_K>& aOldMtcsK, 
													 int nCmd, int nKey)
{
	for(int i=0; i<aOldMtcsK.GetSize(); i++)
	{
		if (!DelListItem(aOldMtcsK[i], nCmd, nKey)) return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------------
BOOL CDB_MTCS::Get(T_MTCS_K Key,T_MTCS_D& rData)
{
	return m_mtcs.Lookup(Key,rData);
}

int CDB_MTCS::GetCount()
{
	return m_mtcs.GetCount();
}

POSITION CDB_MTCS::GetStart()
{
	return m_mtcs.GetStartPosition();
}

void CDB_MTCS::GetNext(POSITION& rNextPosition,T_MTCS_K& rKey,T_MTCS_D& rData)
{
	m_mtcs.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_MTCS::GetList(T_MTCS_K Key,MtcsList*& rpList)
{
	return m_mtcslist.Lookup(Key,rpList);
}

int CDB_MTCS::GetCountList()
{
	return m_mtcslist.GetCount();
}
