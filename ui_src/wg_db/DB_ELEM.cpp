#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_MATL.h"
#include "DB_SECT.h"
#include "DB_THIK.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"
#include "AttrCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"

CDB_ELEM::CDB_ELEM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;

	m_elem.InitHashTable(HASHSIZEELEM);
	m_elem_dummy.InitHashTable(HASHSIZEWOPN);
	m_elemlist.InitHashTable(HASHSIZELISTELEM);

	for(int i=0; i<MAX_ELTYP_NUM+1; i++)m_arEltypeCount[i]=0;

	m_matlCount.InitHashTable(HASHSIZEMATL);
	m_sectCount.InitHashTable(HASHSIZESECT);
	m_thikCount.InitHashTable(HASHSIZETHIK);
	m_widCount.InitHashTable(1001);

	m_elem_delBuff.InitHashTable(HASHSIZEWOPN);
}

CDB_ELEM::~CDB_ELEM()
{
	DelAllList();
}

CDB_ELEM::CDB_ELEM(CDB_ELEM& src)
{
	*this = src;
}

CDB_ELEM& CDB_ELEM::operator= (CDB_ELEM& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;
	m_pDoc = 0;  // 복사되는 것은 Stage와 무관하므로 NULL로 설정

	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;
	m_elem.RemoveAll();
	pos=src.m_elem.GetStartPosition();
	while(pos != NULL)
	{
		src.m_elem.GetNextAssoc(pos,KeyElem,DataElem);
		m_elem.SetAt(KeyElem, DataElem);
	}

	m_elem_dummy.RemoveAll();
	int nDummy = 0;
	pos=src.m_elem_dummy.GetStartPosition();
	while (pos != NULL)
	{
		src.m_elem_dummy.GetNextAssoc(pos,KeyElem,nDummy);
		m_elem_dummy.SetAt(KeyElem, nDummy);
	}

	DelAllList();
	ElemList* pList;
	pos=src.m_elemlist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_elemlist.GetNextAssoc(pos,KeyElem,pList);
		ElemList* pListNew = new ElemList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_elemlist.SetAt(KeyElem, pListNew);
	}

	//int m_arEltypeCount[MAX_ELTYP_NUM+1];
	memcpy(m_arEltypeCount, src.m_arEltypeCount, sizeof(m_arEltypeCount));
	
	//CMap<T_MATL_K,T_MATL_K,int,int> m_matlCount;
	//CMap<T_SECT_K,T_SECT_K,int,int> m_sectCount;
	//CMap<T_THIK_K,T_THIK_K,int,int> m_thikCount;
	//CMap<int,int,int,int>           m_widCount;
	int nCount;
	T_MATL_K KeyMatl;
	T_SECT_K KeySect;
	T_THIK_K KeyThik;
	int      wid;
	m_matlCount.RemoveAll();
	pos=src.m_matlCount.GetStartPosition();
	while(pos != NULL)
	{
		src.m_matlCount.GetNextAssoc(pos,KeyMatl,nCount);
		m_matlCount.SetAt(KeyMatl, nCount);
	}
	m_sectCount.RemoveAll();
	pos=src.m_sectCount.GetStartPosition();
	while(pos != NULL)
	{
		src.m_sectCount.GetNextAssoc(pos,KeySect,nCount);
		m_sectCount.SetAt(KeySect, nCount);
	}
	m_thikCount.RemoveAll();
	pos=src.m_thikCount.GetStartPosition();
	while(pos != NULL)
	{
		src.m_thikCount.GetNextAssoc(pos,KeyThik,nCount);
		m_thikCount.SetAt(KeyThik, nCount);
	}
	m_widCount.RemoveAll();
	pos=src.m_widCount.GetStartPosition();
	while(pos != NULL)
	{
		src.m_widCount.GetNextAssoc(pos,wid,nCount);
		m_widCount.SetAt(wid, nCount);
	}

	return *this;
}

void CDB_ELEM::Add(T_ELEM_K Key,T_ELEM_D& rData,CDB_NODE* pNode,CDB_MATL* pMatl,CDB_SECT* pSect,CDB_THIK* pThik)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nMatlNum, nSectNum, nThikNum, nWidNum;

	T_ELEM_D data;
	BOOL bExist=m_elem.Lookup(Key, data);

	if(bExist)
	{
		for(int i = 0; i<D_ELEM_MAXNOD; i++)
		{
			if(pNode != NULL && data.elnod[i] > 0)pNode->DelListItem(data.elnod[i], LT_ELEM_CMD, Key);
		}
		
		if (pDoc->m_pAttrCtrl->IsNeedMatl(data.eltyp))
		{
			if (pMatl != NULL) pMatl->DelListItem(data.elmat, LT_ELEM_CMD, Key);
			if (m_matlCount.Lookup(data.elmat, nMatlNum))
			{
				if (nMatlNum == 1) m_matlCount.RemoveKey(data.elmat);
				else m_matlCount.SetAt(data.elmat, nMatlNum-1);
			}
			else ASSERT(0);
		}
		if (pDoc->m_pAttrCtrl->IsNeedSect(data.eltyp))
		{
			if (pSect != NULL) pSect->DelListItem(data.elpro, LT_ELEM_CMD, Key);
			if (m_sectCount.Lookup(data.elpro, nSectNum))
			{
				if (nSectNum == 1) m_sectCount.RemoveKey(data.elpro);
				else m_sectCount.SetAt(data.elpro, nSectNum-1);
			}
			else ASSERT(0);
		}
		if (pDoc->m_pAttrCtrl->IsNeedThik(data.eltyp))
		{
			if (pThik != NULL) pThik->DelListItem(data.elpro, LT_ELEM_CMD, Key);
			if (m_thikCount.Lookup(data.elpro, nThikNum))
			{
				if (nThikNum == 1) m_thikCount.RemoveKey(data.elpro);
				else m_thikCount.SetAt(data.elpro, nThikNum-1);
			}
			else ASSERT(0);
		}
		if (pDoc->m_pAttrCtrl->IsWall(data.eltyp))
		{
			if (m_widCount.Lookup(data.nWallId, nWidNum))
			{
				if (nWidNum == 1) m_widCount.RemoveKey(data.nWallId);
				else m_widCount.SetAt(data.nWallId, nWidNum-1);
			}
			else ASSERT(0);
		}
		m_arEltypeCount[data.eltyp]--;
	}
	m_elem.SetAt(Key,rData);
	
	if (pDoc->m_pAttrCtrl->IsDummyElem(rData.eltyp))  m_elem_dummy.SetAt(Key, 0);
	if (bExist)   // Type이 변경된 경우.
	{
		if (pDoc->m_pAttrCtrl->IsDummyElem(data.eltyp ) && 
			 !pDoc->m_pAttrCtrl->IsDummyElem(rData.eltyp))  m_elem_dummy.RemoveKey(Key);
	}

#ifdef _TOWER
	ASSERT(rData.eltyp >= 0 && rData.eltyp <= MAX_ELTYP_NUM);
#else
	ASSERT(rData.eltyp > 0 && rData.eltyp <= MAX_ELTYP_NUM);
#endif
	m_arEltypeCount[rData.eltyp]++;

	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
	for(int i = 0; i<D_ELEM_MAXNOD; i++)
	{
		if(pNode != NULL && rData.elnod[i] > 0)pNode->AddListItem(rData.elnod[i], LT_ELEM_CMD, Key);
	}
	if (pDoc->m_pAttrCtrl->IsNeedMatl(rData.eltyp))
	{
		if (pMatl != NULL)pMatl->AddListItem(rData.elmat, LT_ELEM_CMD, Key);
		if (m_matlCount.Lookup(rData.elmat, nMatlNum))
			m_matlCount.SetAt(rData.elmat, nMatlNum+1);
		else m_matlCount.SetAt(rData.elmat, 1);
	}
	if (pDoc->m_pAttrCtrl->IsNeedSect(rData.eltyp))
	{
		if (pSect != NULL)pSect->AddListItem(rData.elpro, LT_ELEM_CMD, Key);
		if (m_sectCount.Lookup(rData.elpro, nSectNum))
			m_sectCount.SetAt(rData.elpro, nSectNum+1);
		else m_sectCount.SetAt(rData.elpro, 1);
	}
	if (pDoc->m_pAttrCtrl->IsNeedThik(rData.eltyp))
	{
		if (pThik != NULL)pThik->AddListItem(rData.elpro, LT_ELEM_CMD, Key);
		if (m_thikCount.Lookup(rData.elpro, nThikNum))
			m_thikCount.SetAt(rData.elpro, nThikNum+1);
		else m_thikCount.SetAt(rData.elpro, 1);
	}
	if (pDoc->m_pAttrCtrl->IsWall(rData.eltyp))
	{
		if (m_widCount.Lookup(rData.nWallId, nWidNum))
			m_widCount.SetAt(rData.nWallId, nWidNum+1);
		else m_widCount.SetAt(rData.nWallId, 1);
	}
}

BOOL CDB_ELEM::Del(T_ELEM_K Key,CDB_NODE* pNode,CDB_MATL* pMatl,CDB_SECT* pSect,CDB_THIK* pThik)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nMatlNum, nSectNum, nThikNum, nWidNum;

	T_ELEM_K key;
	T_ELEM_D data;
	if(!m_elem.Lookup(Key, data))return FALSE;
	BOOL ret=m_elem.RemoveKey(Key);
	if (pDoc->m_pAttrCtrl->IsDummyElem(data.eltyp)) m_elem_dummy.RemoveKey(Key);
	ASSERT(ret);

	if(ret)
	{
#ifdef _TOWER
		ASSERT(data.eltyp >= 0 && data.eltyp <= MAX_ELTYP_NUM);
#else
		ASSERT(data.eltyp > 0 && data.eltyp <= MAX_ELTYP_NUM);
#endif
		m_arEltypeCount[data.eltyp]--;
		for(int i=0; i<D_ELEM_MAXNOD; i++)
		{
			if(pNode != NULL && data.elnod[i] > 0)pNode->DelListItem(data.elnod[i], LT_ELEM_CMD, Key);
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ELEM_D tdata;
			if(Get(m_nLastNum-1,tdata))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,tdata);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		if (pDoc->m_pAttrCtrl->IsNeedMatl(data.eltyp))
		{
			if (pMatl != NULL)pMatl->DelListItem(data.elmat, LT_ELEM_CMD, Key);
			if (m_matlCount.Lookup(data.elmat, nMatlNum))
			{
				if (nMatlNum == 1) m_matlCount.RemoveKey(data.elmat);
				else m_matlCount.SetAt(data.elmat, nMatlNum-1);
			}
			else ASSERT(0);
		}
		if (pDoc->m_pAttrCtrl->IsNeedSect(data.eltyp))
		{
			if (pSect != NULL)pSect->DelListItem(data.elpro, LT_ELEM_CMD, Key);
			if (m_sectCount.Lookup(data.elpro, nSectNum))
			{
				if (nSectNum == 1) m_sectCount.RemoveKey(data.elpro);
				else m_sectCount.SetAt(data.elpro, nSectNum-1);
			}
			else ASSERT(0);
		}
		if (pDoc->m_pAttrCtrl->IsNeedThik(data.eltyp))
		{
			if (pThik != NULL)pThik->DelListItem(data.elpro, LT_ELEM_CMD, Key);
			if (m_thikCount.Lookup(data.elpro, nThikNum))
			{
				if (nThikNum == 1) m_thikCount.RemoveKey(data.elpro);
				else m_thikCount.SetAt(data.elpro, nThikNum-1);
			}
			else ASSERT(0);
		}
		if (pDoc->m_pAttrCtrl->IsWall(data.eltyp))
		{
			if (m_widCount.Lookup(data.nWallId, nWidNum))
			{
				if (nWidNum == 1) m_widCount.RemoveKey(data.nWallId);
				else m_widCount.SetAt(data.nWallId, nWidNum-1);
			}
			else ASSERT(0);
		}
	}
	return ret;
}

void CDB_ELEM::AddList(T_ELEM_K Key)
{
	ElemList* pList = new ElemList;
	m_elemlist.SetAt(Key, pList);
}

BOOL CDB_ELEM::DelList(T_ELEM_K Key)
{
	ElemList* pList;
	BOOL bExist=m_elemlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_elemlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_ELEM::DelAllList()
{
	T_ELEM_K Key;
	ElemList* pList;
	POSITION pos=m_elemlist.GetStartPosition();
	while(pos != NULL)
	{
		m_elemlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_elemlist.RemoveAll();
}

void CDB_ELEM::AddListItem(T_ELEM_K Key, int nCmd, int nKey)
{
	ElemList* pList;
	//VERIFY(GetList(Key, pList));
	if (m_elemlist.Lookup(Key, pList) == FALSE) {
		VERIFY(FALSE);
		return;
	}

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);

	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForElem(Key, nCmd, nKey);
}

BOOL CDB_ELEM::DelListItem(T_ELEM_K Key, int nCmd, int nKey)
{
	ElemList* pList;
	//VERIFY(GetList(Key, pList));
	if (m_elemlist.Lookup(Key, pList) == FALSE) {
		VERIFY(FALSE);
		return FALSE;
	}

	BOOL bDeleted = FALSE;
	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
			break;
		}
	}
	if (!bDeleted) return FALSE;

	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->DelListItemForElem(Key, nCmd, nKey);
	return TRUE;
}

void CDB_ELEM::AddListItem(CArray<T_ELEM_K, T_ELEM_K>& aNewElemK, 
													 CArray<T_ELEM_K, T_ELEM_K>& aOldElemK,
													 int nCmd, int nKey, BOOL bExist)
{
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapNew;
	T_ELEM_K KeyElemTemp;
	if (bExist)
	{
		mapNew.InitHashTable(997);
		mapOld.InitHashTable(997);
		for(int i = 0; i<aOldElemK.GetSize(); i++)mapOld.SetAt(aOldElemK[i], aOldElemK[i]);
		for(int i = 0; i<aNewElemK.GetSize(); i++)mapNew.SetAt(aNewElemK[i], aNewElemK[i]);
		for(int i = 0; i<aOldElemK.GetSize(); i++)
		{
			if(!mapNew.Lookup(aOldElemK[i], KeyElemTemp))
				VERIFY(DelListItem(aOldElemK[i], nCmd, nKey));
		}
	}
	for(int i=0; i<aNewElemK.GetSize(); i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(aNewElemK[i], KeyElemTemp))
				AddListItem(aNewElemK[i], nCmd, nKey);
		}
		else
			AddListItem(aNewElemK[i], nCmd, nKey);
	}
}

BOOL CDB_ELEM::DelListItem(CArray<T_ELEM_K, T_ELEM_K>& aOldElemK, 
													 int nCmd, int nKey)
{
	for(int i=0; i<aOldElemK.GetSize(); i++)
	{
		if (!DelListItem(aOldElemK[i], nCmd, nKey)) return FALSE;
	}
	return TRUE;
}

//--------------------------------------------------------------------
CMap<T_ELEM_K,T_ELEM_K,T_ELEM_D,T_ELEM_D&>::CPair* CDB_ELEM::GetP(T_ELEM_K Key)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		double dAge;
		if (!m_pDoc->m_pStagCtrl->m_elem->Lookup(Key, dAge)) return nullptr;
	}
	return m_elem.PLookup(Key);
}

BOOL CDB_ELEM::Get(T_ELEM_K Key,T_ELEM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		double dAge;
		if (!m_pDoc->m_pStagCtrl->m_elem->Lookup(Key, dAge)) return FALSE;
	}
	return m_elem.Lookup(Key,rData);
}

BOOL CDB_ELEM::ExistDummy(T_ELEM_K Key)
{
	int nDummy = 0;
	return m_elem_dummy.Lookup(Key, nDummy);
}

int CDB_ELEM::GetCount(BOOL bWithoutDummyElem/*=TRUE*/)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		int nCount = m_pDoc->m_pStagCtrl->m_elem->GetCount();
		if (bWithoutDummyElem) nCount -= m_pDoc->m_pStagCtrl->m_elem_dummy->GetCount();
		return nCount;
	}

	int nCount = m_elem.GetCount();
	if (bWithoutDummyElem) nCount -= m_elem_dummy.GetCount();

	return nCount;
}

int CDB_ELEM::GetCountEltyp(int eltyp)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_arEltypeCount[eltyp];
	}
	return m_arEltypeCount[eltyp];
}

POSITION CDB_ELEM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_elem->GetStartPosition();
	}
	return m_elem.GetStartPosition();
}

void CDB_ELEM::GetNext(POSITION& rNextPosition,T_ELEM_K& rKey,T_ELEM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		double dTempAge;
		m_pDoc->m_pStagCtrl->m_elem->GetNextAssoc(rNextPosition, rKey, dTempAge);
		m_elem.Lookup(rKey, rData);
		return;
	}
	m_elem.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_ELEM::GetList(T_ELEM_K Key,ElemList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_elemlist->Lookup(Key, rpList);
	}
	return m_elemlist.Lookup(Key,rpList);
}

int CDB_ELEM::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_elemlist->GetCount();
	}
	return m_elemlist.GetCount();
}


BOOL CDB_ELEM::IsUsedMatl(T_MATL_K MatlKey)
{
	int nNum;
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->m_matlCount->Lookup(MatlKey, nNum);
	return m_matlCount.Lookup(MatlKey, nNum);
}

POSITION CDB_ELEM::GetStartMatlCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->m_matlCount->GetStartPosition();
	return m_matlCount.GetStartPosition();
}

void CDB_ELEM::GetNextMatlCount(POSITION& rNextPosition,T_MATL_K &rMatlKey, int& rNum)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->m_matlCount->GetNextAssoc(rNextPosition, rMatlKey, rNum);
	else m_matlCount.GetNextAssoc(rNextPosition, rMatlKey, rNum);
}

BOOL CDB_ELEM::IsUsedSect(T_SECT_K SectKey)
{
	int nNum;
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->m_sectCount->Lookup(SectKey, nNum);
	return m_sectCount.Lookup(SectKey, nNum);
}

POSITION CDB_ELEM::GetStartSectCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->m_sectCount->GetStartPosition();
	return m_sectCount.GetStartPosition();
}

void CDB_ELEM::GetNextSectCount(POSITION& rNextPosition,T_SECT_K &rSectKey, int& rNum)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->m_sectCount->GetNextAssoc(rNextPosition, rSectKey, rNum);
	else m_sectCount.GetNextAssoc(rNextPosition, rSectKey, rNum);
}

BOOL CDB_ELEM::IsUsedThik(T_THIK_K ThikKey)
{
	int nNum;
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->m_thikCount->Lookup(ThikKey, nNum);
	return m_thikCount.Lookup(ThikKey, nNum);
}

POSITION CDB_ELEM::GetStartThikCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->m_thikCount->GetStartPosition();
	return m_thikCount.GetStartPosition();
}

void CDB_ELEM::GetNextThikCount(POSITION& rNextPosition,T_THIK_K &rThikKey, int& rNum)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->m_thikCount->GetNextAssoc(rNextPosition, rThikKey, rNum);
	else m_thikCount.GetNextAssoc(rNextPosition, rThikKey, rNum);
}

int CDB_ELEM::GetWallID_LastNumPlusOne()
{
	int nSize = m_widCount.GetCount();
	if (nSize == 0) return 1;
	if (nSize == 9999) return 9999;

	CArray<int, int> aWallID;
	aWallID.SetSize(nSize);

	int nLastNum = 0;
	int i = 0, wid, nCount;
	POSITION pos = m_widCount.GetStartPosition();
	while (pos)
	{
		m_widCount.GetNextAssoc(pos, wid, nCount);
		if (wid > nLastNum) nLastNum = wid;
		aWallID[i++] = wid;
	}
	if (nLastNum == 9999) // 사용되지 않은 번호 리턴
	{
		qsort(aWallID.GetData(), nSize, sizeof(int), CCompFunc::intAsc);
		for(int i = 0; i < nSize; i++)
			if (aWallID[i] != i+1) return i+1;
		return 9999;
	}
	else return nLastNum+1;
}

int CDB_ELEM::PopAll_DelBuff(CMap<T_ELEM_K,T_ELEM_K,int,int>& rmElem_delBuff)
{
	rmElem_delBuff.RemoveAll();
	if (m_elem_delBuff.GetCount() == 0)  return 0;

	rmElem_delBuff.InitHashTable(m_elem_delBuff.GetHashTableSize());

	T_ELEM_K ElemK = 0;
	int nTemp = 0;
	POSITION pos = m_elem_delBuff.GetStartPosition();
	while(pos)
	{
		m_elem_delBuff.GetNextAssoc(pos, ElemK, nTemp);
		rmElem_delBuff.SetAt(ElemK, nTemp);
	}

	// m_elem_delBuff 비우기.
	m_elem_delBuff.RemoveAll();

	return rmElem_delBuff.GetCount();
}

void CDB_ELEM::PushElem_DelBuff(const T_ELEM_K ElemK)
{
	m_elem_delBuff.SetAt(ElemK, 0);
}