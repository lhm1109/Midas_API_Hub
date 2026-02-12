#include "stdafx.h"
#include "DB_POLC.h"
#include "DB_STLD.h"
#include "DB_NODE.h"
#include "DB_POFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POLC::CDB_POLC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_polc.InitHashTable(HASHSIZEPOLC);
	m_polclist.InitHashTable(HASHSIZELISTPOLC);
}

CDB_POLC::~CDB_POLC()
{
	DelAllList();
}

void CDB_POLC::Add(T_POLC_K Key,T_POLC_D& rData, CDB_STLD* pStld, CDB_NODE* pNode, CDB_POFC* pPofc)
{
	T_POLC_D Data;
	BOOL bExist = m_polc.Lookup(Key, Data);

	if (pNode && bExist && rData.nIncrementMethod==1 && rData.nDispCtrlOption==1)
	{
		if (Data.MasterNode)
			pNode->DelListItem(Data.MasterNode, LT_POLC_CMD, Key);
	}
	if(pPofc && bExist && rData.nIncrementMethod==0 && rData.nStepCtrlOption==2)
	{
		if (Data.IncFuncK)
			pPofc->DelListItem(Data.IncFuncK, LT_POLC_CMD, Key);
	}

	m_polc.SetAt(Key,rData);
	
	if (pNode && rData.nIncrementMethod==1 && rData.nDispCtrlOption==1)
	{
		if (rData.MasterNode)
			pNode->AddListItem(rData.MasterNode, LT_POLC_CMD, Key);
	}
	
	if(pPofc && rData.nIncrementMethod==0 && rData.nStepCtrlOption==2)
	{
		if (rData.IncFuncK)
			pPofc->AddListItem(rData.IncFuncK, LT_POLC_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_POLC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_POLC::Del(T_POLC_K Key, CDB_STLD* pStld, CDB_NODE* pNode, CDB_POFC* pPofc)
{
	T_POLC_D Data;
	m_polc.Lookup(Key, Data);
	BOOL ret=m_polc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (pNode && Data.nIncrementMethod==1 && Data.nDispCtrlOption==1)
		{
			ASSERT(Data.MasterNode != NULL);
			pNode->DelListItem(Data.MasterNode, LT_POLC_CMD, Key);
		}

		if(Data.nIncrementMethod==0)
		{
			if(Data.nStepCtrlOption==2)
			{
				ASSERT(Data.IncFuncK != NULL);
				pPofc->DelListItem(Data.IncFuncK, LT_POLC_CMD, Key);
			}
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_POLC_K key;
			T_POLC_D data;
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

void CDB_POLC::AddList(T_POLC_K Key)
{
	PolcList* pList = new PolcList;
	m_polclist.SetAt(Key, pList);
}

BOOL CDB_POLC::DelList(T_POLC_K Key)
{
	PolcList* pList;
	BOOL bExist=m_polclist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_polclist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_POLC::DelAllList()
{
	T_POLC_K Key;
	PolcList* pList;
	POSITION pos=m_polclist.GetStartPosition();
	while(pos != NULL)
	{
		m_polclist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_polclist.RemoveAll();
}

void CDB_POLC::AddListItem(T_POLC_K Key, int nCmd, int nKey)
{
	PolcList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_POLC::DelListItem(T_POLC_K Key, int nCmd, int nKey)
{
	PolcList* pList;
	VERIFY(GetList(Key, pList));

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

//-------------------------------------------------------------------------
BOOL CDB_POLC::Get(T_POLC_K Key,T_POLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_polc.Lookup(Key,rData);
}

int CDB_POLC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_polc.GetCount();
}

POSITION CDB_POLC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_polc.GetStartPosition();
}

void CDB_POLC::GetNext(POSITION& rNextPosition,T_POLC_K& rKey,T_POLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_polc.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_POLC::GetList(T_POLC_K Key, PolcList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_polclist.Lookup(Key,rpList);
}

int CDB_POLC::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_polclist.GetCount();
}
