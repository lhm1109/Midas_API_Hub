#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_FBLD.h"

#include "DBDoc.h"
#include "DispOptColorObj.h"
#include "DispOptMgr.h"
#include "StagCtrl.h"

CDB_FBLD::CDB_FBLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_fbld.InitHashTable(HASHSIZEFBLD);
	m_fbldlist.InitHashTable(HASHSIZELISTFBLD);
	m_fbldcolor.InitHashTable(HASHSIZELISTFBLD);
}

CDB_FBLD::~CDB_FBLD()
{
	DelAllList();
}

void CDB_FBLD::Add(T_FBLD_K Key,T_FBLD_D& rData,CDB_STLD* pStld)
{
	T_FBLD_D Data;
	BOOL bExist=m_fbld.Lookup(Key, Data);
	if (!bExist)
	{
		T_COLOR_D DataColor;
		DataColor.Initialize();
		DataColor.WireFrame =RGB(166,202,240);
		DataColor.HiddenFill=RGB(166,202,240);
		DataColor.HiddenEdge=RGB(166,202,240);
		DataColor.bBlending = TRUE;
		DataColor.dBlendingFactor = 0.25;
		m_fbldcolor.SetAt(Key, DataColor);
	}
	if(bExist)
	{
		for(int i=0; i<FBLD_NUM; i++)
		{
			if(Data.LoadCaseKey[i] == 0)break;
			BOOL bExistLoad=FALSE;
			for(int j=0; j<FBLD_NUM; j++)
			{
				if(rData.LoadCaseKey[j] == 0)break;
				if(rData.LoadCaseKey[j] == Data.LoadCaseKey[i])
				{
					bExistLoad=TRUE;
					break;
				}
			}
			if(!bExistLoad)VERIFY(pStld->DelListItem(Data.LoadCaseKey[i], LT_FBLD_CMD, Key));
		}
	}
	m_fbld.SetAt(Key,rData);
	for(int i=0; i<FBLD_NUM; i++)
	{
		if(rData.LoadCaseKey[i] == 0)break;
		BOOL bExistLoad=FALSE;
		if(bExist)
		{
			for(int j=0; j<FBLD_NUM; j++)
			{
				if(Data.LoadCaseKey[j] == 0)break;
				if(Data.LoadCaseKey[j] == rData.LoadCaseKey[i])
				{
					bExistLoad=TRUE;
					break;
				}
			}
		}
		if(!bExistLoad)pStld->AddListItem(rData.LoadCaseKey[i], LT_FBLD_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_FBLD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_FBLD::Del(T_FBLD_K Key,CDB_STLD* pStld)
{
	T_FBLD_D Data;
	BOOL bExist=m_fbld.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fbld.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(m_fbldcolor.RemoveKey(Key));
			for(int i=0; i<FBLD_NUM; i++)
			{
				if(Data.LoadCaseKey[i] == 0)break;
				VERIFY(pStld->DelListItem(Data.LoadCaseKey[i], LT_FBLD_CMD, Key));
			}
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_FBLD_K key;
				T_FBLD_D data;
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
	return FALSE;
}


void CDB_FBLD::AddList(T_FBLD_K Key)
{
	FbldList* pList = new FbldList;
	m_fbldlist.SetAt(Key, pList);
}

BOOL CDB_FBLD::DelList(T_FBLD_K Key)
{
	FbldList* pList;
	BOOL bExist=m_fbldlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_fbldlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_FBLD::DelAllList()
{
	T_FBLD_K Key;
	FbldList* pList;
	POSITION pos=m_fbldlist.GetStartPosition();
	while(pos != NULL)
	{
		m_fbldlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_fbldlist.RemoveAll();
}

void CDB_FBLD::AddListItem(T_FBLD_K Key, int nCmd, int nKey)
{
	FbldList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_fbldlist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForFbld(Key, nCmd, nKey);
}

BOOL CDB_FBLD::DelListItem(T_FBLD_K Key, int nCmd, int nKey)
{
	FbldList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_fbldlist.Lookup(Key, pList));

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

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->DelListItemForFbld(Key, nCmd, nKey);
	return TRUE;
}

//-------------------------------------------------------------------------
BOOL CDB_FBLD::GetList(T_FBLD_K Key,FbldList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_fbldlist->Lookup(Key,rpList);
	}
	return m_fbldlist.Lookup(Key,rpList);
}

int CDB_FBLD::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_fbldlist->GetCount();
	}
	return m_fbldlist.GetCount();
}
