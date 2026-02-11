#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_NLLP.h"
#include "DB_NLNK.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NLNK::CDB_NLNK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_nlnk.InitHashTable(HASHSIZENLNK);
	m_nlnklist.InitHashTable(HASHSIZELISTNLNK);
}

CDB_NLNK::~CDB_NLNK()
{

}

void CDB_NLNK::Add(T_NLNK_K Key,T_NLNK_D& rData,CDB_NODE* pNode,CDB_NLLP* pNllp,CDB_BNGR* pBngr)
{
	T_NLNK_D Data;
	BOOL bExist=m_nlnk.Lookup(Key, Data);
	if(bExist)
	{
		if (pNode)
		{
			VERIFY(pNode->DelListItem(Data.Node1, LT_NLNK_CMD, Key));
			VERIFY(pNode->DelListItem(Data.Node2, LT_NLNK_CMD, Key));
		}
		if (pNllp) pNllp->DelListItem(Data.PropKey, LT_NLNK_CMD, Key);
		if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_NLNK_CMD, Key);
	}
	m_nlnk.SetAt(Key,rData);
	if (pNode)
	{
		pNode->AddListItem(rData.Node1, LT_NLNK_CMD, Key);
		pNode->AddListItem(rData.Node2, LT_NLNK_CMD, Key);
	}
	if (pNllp) pNllp->AddListItem(rData.PropKey, LT_NLNK_CMD, Key);
	if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_NLNK_CMD, Key);
	if(Key == m_nStartNum)
	{
		T_NLNK_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_nlnk.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_NLNK_CMD, Key, rData.GroupKey);
	}
}

BOOL CDB_NLNK::Del(T_NLNK_K Key,CDB_NODE* pNode,CDB_NLLP* pNllp,CDB_BNGR* pBngr)
{
	T_NLNK_D Data;
	BOOL bExist=m_nlnk.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_nlnk.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pNode)
			{
				VERIFY(pNode->DelListItem(Data.Node1, LT_NLNK_CMD, Key));
				VERIFY(pNode->DelListItem(Data.Node2, LT_NLNK_CMD, Key));
			}
			if (pNllp) pNllp->DelListItem(Data.PropKey, LT_NLNK_CMD, Key);
			if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_NLNK_CMD, Key);

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_NLNK_CMD, Key);
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_NLNK_K key;
			T_NLNK_D data;
			if(m_nlnk.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_nlnk.GetStartPosition();
				while(pos != NULL)
				{
					m_nlnk.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_NLNK::Get(T_NLNK_K Key,T_NLNK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_nlnk->Lookup(Key, StagBngrKey)) return FALSE;
	}
	return m_nlnk.Lookup(Key,rData);
}

int CDB_NLNK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nlnk->GetCount();
	}
	return m_nlnk.GetCount();
}

POSITION CDB_NLNK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nlnk->GetStartPosition();
	}
	return m_nlnk.GetStartPosition();
}

void CDB_NLNK::GetNext(POSITION& rNextPosition,T_NLNK_K& rKey,T_NLNK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_nlnk->GetNextAssoc(rNextPosition, rKey, StagBngrKey);
		m_nlnk.Lookup(rKey, rData);
		return;
	}
	m_nlnk.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_NLNK::AddList(T_NLNK_K Key)
{
	NlnkList* pList = new NlnkList;
	m_nlnklist.SetAt(Key, pList);
}

BOOL CDB_NLNK::DelList(T_NLNK_K Key)
{
	NlnkList* pList;
	BOOL bExist=m_nlnklist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_nlnklist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_NLNK::DelAllList()
{
	T_NLNK_K Key;
	NlnkList* pList;
	POSITION pos=m_nlnklist.GetStartPosition();
	while(pos != NULL)
	{
		m_nlnklist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_nlnklist.RemoveAll();
}

BOOL CDB_NLNK::GetList(T_NLNK_K Key, PolcList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_nlnklist.Lookup(Key,rpList);
}

int CDB_NLNK::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_nlnklist.GetCount();
}

void CDB_NLNK::AddListItem(T_NLNK_K Key, int nCmd, int nKey)
{
	NlnkList* pList;
	//VERIFY(GetList(Key, pList));
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_NLNK::DelListItem(T_NLNK_K Key, int nCmd, int nKey)
{
	NlnkList* pList;
	//VERIFY(GetList(Key, pList));
	if(!GetList(Key, pList))return TRUE;
	
	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}