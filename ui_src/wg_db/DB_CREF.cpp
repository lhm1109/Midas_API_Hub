#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_ELNK.h"
#include "DB_NLNK.h"
#include "DB_CREF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

#include "..\wg_base\AfxTemplEx.h"

CDB_CREF::CDB_CREF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_cref.InitHashTable(HASHSIZECREF);
}

CDB_CREF::~CDB_CREF()
{
}

#define M_OP_ADD 1
#define M_OP_MOD 0
#define M_OP_DEL -1
void CDB_CREF::MakeList(int nOP, const T_CREF_D* pCurCrefD, const T_CREF_D* pNewCrefD, 
												CArray<UINT, UINT>* paAddList, CArray<UINT, UINT>* paDelList)
{
	int i=0;
	BOOL bAddMaster = FALSE;
	switch(nOP)
	{
	case M_OP_ADD:
		ASSERT(pNewCrefD);
		if(pNewCrefD->nCalcItem == 0 || pNewCrefD->nCalcItem == 1)
		{
			if (pNewCrefD->MasterK != 0)
			{
				for(i = 0; i < pNewCrefD->aList.GetSize(); i++)
					if (pNewCrefD->aList[i] == pNewCrefD->MasterK) break;
				if (i == pNewCrefD->aList.GetSize()) bAddMaster = TRUE;
			}
			paAddList->Copy(pNewCrefD->aList);
			if (bAddMaster) paAddList->Add(pNewCrefD->MasterK);
		}
		else if(pNewCrefD->nCalcItem == 2 || pNewCrefD->nCalcItem == 3 || pNewCrefD->nCalcItem == 4 )
		{
			if(pNewCrefD->MasterK != 0)
			{
				for(i = 0; i < pNewCrefD->aElemBase.GetSize(); i++)
					if (pNewCrefD->aElemBase[i].ElemK == pNewCrefD->MasterK) break;
				if (i == pNewCrefD->aElemBase.GetSize()) bAddMaster = TRUE;
			}
			for(int i = 0; i < pNewCrefD->aElemBase.GetSize(); i++)
			{
				paAddList->Add(pNewCrefD->aElemBase[i].ElemK);
			}      
			if(bAddMaster) paAddList->Add(pNewCrefD->MasterK);
		}
		else ASSERT(FALSE);
		break;
	case M_OP_MOD:
		// 삭제할 것, 추가할 것 구분
		{
			paAddList->RemoveAll();
			paDelList->RemoveAll();
			if(pNewCrefD->nCalcItem == 0 || pNewCrefD->nCalcItem == 1)
			{
				if (pNewCrefD->MasterK != 0)
				{
					for(i = 0; i < pNewCrefD->aList.GetSize(); i++)
						if (pNewCrefD->aList[i] == pNewCrefD->MasterK) break;
					if (i == pNewCrefD->aList.GetSize()) bAddMaster = TRUE;
				}
			
				CMapEx<UINT, UINT, int, int> mCur;
				mCur.InitHashTable(101);
				for(i = 0; i < pCurCrefD->aList.GetSize(); i++)
					mCur.SetAt(pCurCrefD->aList[i], 0);
				if (pCurCrefD->MasterK != 0) mCur.SetAt(pCurCrefD->MasterK, 0);
				for(i = 0; i < pNewCrefD->aList.GetSize(); i++)
				{
					if (!mCur.Exist(pNewCrefD->aList[i])) paAddList->Add(pNewCrefD->aList[i]);
					else mCur.RemoveKey(pNewCrefD->aList[i]);  // 교집합.
				}
				if (bAddMaster)
				{
					if (!mCur.Exist(pNewCrefD->MasterK)) paAddList->Add(pNewCrefD->MasterK);
					else mCur.RemoveKey(pNewCrefD->MasterK);
				}
				mCur.GetKeyList(*paDelList);
			}
			else if(pNewCrefD->nCalcItem == 2 || pNewCrefD->nCalcItem == 3 || pNewCrefD->nCalcItem == 4)
			{
				if (pNewCrefD->MasterK != 0)
				{
					for(i = 0; i < pNewCrefD->aElemBase.GetSize(); i++)
						if (pNewCrefD->aElemBase[i].ElemK == pNewCrefD->MasterK) break;
					if (i == pNewCrefD->aElemBase.GetSize()) bAddMaster = TRUE;
				}
			
				CMapEx<UINT, UINT, int, int> mCur;
				mCur.InitHashTable(101);
				for(i = 0; i < pCurCrefD->aElemBase.GetSize(); i++)
					mCur.SetAt(pCurCrefD->aElemBase[i].ElemK, 0);
				if (pCurCrefD->MasterK != 0) mCur.SetAt(pCurCrefD->MasterK, 0);
				for(i = 0; i < pNewCrefD->aElemBase.GetSize(); i++)
				{
					if (!mCur.Exist(pNewCrefD->aElemBase[i].ElemK)) paAddList->Add(pNewCrefD->aElemBase[i].ElemK);
					else mCur.RemoveKey(pNewCrefD->aElemBase[i].ElemK);  // 교집합.
				}
				if (bAddMaster)
				{
					if (!mCur.Exist(pNewCrefD->MasterK)) paAddList->Add(pNewCrefD->MasterK);
					else mCur.RemoveKey(pNewCrefD->MasterK);
				}
				mCur.GetKeyList(*paDelList);
			}
			else ASSERT(FALSE);
		}
		break;
	case M_OP_DEL:
		ASSERT(pCurCrefD);
		if(pCurCrefD->nCalcItem == 0 || pCurCrefD->nCalcItem == 1)
		{
			if (pCurCrefD->MasterK != 0)
			{
				for(i = 0; i < pCurCrefD->aList.GetSize(); i++)
					if (pCurCrefD->aList[i] == pCurCrefD->MasterK) break;
				if (i == pCurCrefD->aList.GetSize()) bAddMaster = TRUE;
			}
			paDelList->Copy(pCurCrefD->aList);
			if (bAddMaster) paDelList->Add(pCurCrefD->MasterK);
		}
		else if(pCurCrefD->nCalcItem == 2 || pCurCrefD->nCalcItem == 3 || pCurCrefD->nCalcItem == 4)
		{
			if (pCurCrefD->MasterK != 0)
			{
				for(i = 0; i < pCurCrefD->aElemBase.GetSize(); i++)
					if (pCurCrefD->aElemBase[i].ElemK == pCurCrefD->MasterK) break;
				if (i == pCurCrefD->aElemBase.GetSize()) bAddMaster = TRUE;
			}
			for(int i = 0; i < pCurCrefD->aElemBase.GetSize(); i++)
				paDelList->Add(pCurCrefD->aElemBase[i].ElemK);
			if (bAddMaster) paDelList->Add(pCurCrefD->MasterK);
		}
		else ASSERT(FALSE);
		break;
	}
}

void CDB_CREF::Add(T_CREF_K Key,T_CREF_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem, CDB_ELNK* pElnk, CDB_NLNK* pNlnk)
{
	T_CREF_D Data;
	BOOL bExist=m_cref.Lookup(Key, Data);
	CArray<UINT, UINT> aAddList, aDelList;
	if (bExist) MakeList(M_OP_MOD, &Data, &rData, &aAddList, &aDelList);
	else        MakeList(M_OP_ADD, 0    , &rData, &aAddList, 0);
	if(bExist)
	{
		int nCount = aDelList.GetSize();
		for(int i=0; i<nCount; i++)
		{
			if (Data.nCalcItem == 0 || Data.nCalcItem == 1)
				pNode->DelListItem(aDelList[i], LT_CREF_CMD, Key); 
			else if (Data.nCalcItem == 2)
				pElem->DelListItem(aDelList[i], LT_CREF_CMD, Key);
			else if (Data.nCalcItem == 3)
				pElnk->DelListItem(aDelList[i], LT_CREF_CMD, Key);
			else if (Data.nCalcItem == 4)
				pNlnk->DelListItem(aDelList[i], LT_CREF_CMD, Key);
			else ASSERT(0);
		}
	}
	m_cref.SetAt(Key,rData);

	int nCount = aAddList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (rData.nCalcItem == 0 || rData.nCalcItem == 1)
			pNode->AddListItem(aAddList[i], LT_CREF_CMD, Key);
		else if (rData.nCalcItem == 2)
			pElem->AddListItem(aAddList[i], LT_CREF_CMD, Key);
		else if (rData.nCalcItem == 3)
			pElnk->AddListItem(aAddList[i], LT_CREF_CMD, Key);
		else if (rData.nCalcItem == 4)
			pNlnk->AddListItem(aAddList[i], LT_CREF_CMD, Key);
		else ASSERT(0);
	}

	if(Key == m_nStartNum)
	{
		T_CREF_D data;
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
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_CREF_CMD, Key);
	}
}

BOOL CDB_CREF::Del(T_CREF_K Key,CDB_NODE* pNode,CDB_ELEM* pElem,CDB_ELNK* pElnk,CDB_NLNK* pNlnk)
{
	T_CREF_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_cref.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		CArray<UINT, UINT> aDelList;
		MakeList(M_OP_DEL, &data, 0, 0, &aDelList);
		int nCount=aDelList.GetSize();
		for(int i=0; i<nCount; i++)
		{
			if (data.nCalcItem == 0 || data.nCalcItem == 1)
				pNode->DelListItem(aDelList[i], LT_CREF_CMD, Key);
			else if (data.nCalcItem == 2)
				pElem->DelListItem(aDelList[i], LT_CREF_CMD, Key);
			else if (data.nCalcItem == 3)
				pElnk->DelListItem(aDelList[i], LT_CREF_CMD, Key);
			else if (data.nCalcItem == 4)
				pNlnk->DelListItem(aDelList[i], LT_CREF_CMD, Key);
			else ASSERT(0);
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CREF_K key;
			T_CREF_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_CREF_CMD, Key);
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CREF::Get(T_CREF_K Key,T_CREF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cref->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cref.Lookup(Key,rData);
}

int CDB_CREF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cref->GetCount();
	}
	return m_cref.GetCount();
}

POSITION CDB_CREF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cref->GetStartPosition();
	}
	return m_cref.GetStartPosition();
}

void CDB_CREF::GetNext(POSITION& rNextPosition,T_CREF_K& rKey,T_CREF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cref->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cref.Lookup(rKey, rData);
		return;
	}
	m_cref.GetNextAssoc(rNextPosition,rKey,rData);
}
