#include "stdafx.h"
#include "DB_THFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"    // for T_POS_INT_PAIR, CCompFunc::ComparePosIntPairA

CDB_THFC::CDB_THFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thfc.InitHashTable(HASHSIZETHFC);
}

CDB_THFC::~CDB_THFC()
{
}

void CDB_THFC::Add(T_THFC_K Key,T_THFC_D& rData)
{
	m_thfc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_THFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THFC::Del(T_THFC_K Key)
{
	BOOL ret=m_thfc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THFC_K key;
			T_THFC_D data;
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
BOOL CDB_THFC::Get(T_THFC_K Key,T_THFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thfc.Lookup(Key,rData);
}

int CDB_THFC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thfc.GetCount();
}

POSITION CDB_THFC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thfc.GetStartPosition();
}

void CDB_THFC::GetNext(POSITION& rNextPosition,T_THFC_K& rKey,T_THFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thfc.GetNextAssoc(rNextPosition,rKey,rData);
}

// (속도개선) 복사연산 없이 주소상의 데이터를 바로 읽어서 처리
// key에는 단위변호나이 필요없기 때문에 가능하다고 판단.
int CDB_THFC::GetThfcKeyList_Direct(CArray<T_THFC_K, T_THFC_K>& rThfcKeyList)
{
	rThfcKeyList.RemoveAll();

	int nCount = GetCount();
	if(nCount == 0) return 0;

	rThfcKeyList.SetSize(nCount);

	CArray<T_THFC_K, T_THFC_K> aThfcKey;
	CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&> aPosIdPair;
	aThfcKey.SetSize(nCount);
	aPosIdPair.SetSize(nCount);


	//typedef CMap<T_THFC_K,T_THFC_K,T_THFC_D,T_THFC_D&> CThfcMap;
	auto pCurVal = m_thfc.PGetFirstAssoc();
	int i = 0;
	while (pCurVal != NULL)
	{
		aThfcKey[i] = pCurVal->key;
		aPosIdPair[i].nPos = i;
		aPosIdPair[i].nVal = pCurVal->value.nThisFuncId;

		pCurVal = m_thfc.PGetNextAssoc(pCurVal);
		i++;
	}

	// sort and copy
	int ix;
	qsort((void*)aPosIdPair.GetData(), nCount, sizeof(T_POS_INT_PAIR), CCompFunc::ComparePosIntPairA);
	for(int i = 0; i < nCount; i++)
	{
		ix = aPosIdPair[i].nPos;
		rThfcKeyList[i] = aThfcKey[ix];
	}

	return rThfcKeyList.GetCount();
}