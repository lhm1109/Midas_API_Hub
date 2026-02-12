#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_GSTP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GSTP::CDB_GSTP()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nStartNum=1;
	m_nLastNum=0;
	m_gstp.InitHashTable(HASHSIZEGSTP);
	m_gstplist.InitHashTable(HASHSIZELISTGSTP);
}

CDB_GSTP::~CDB_GSTP()
{
	DelAllList();
}

void CDB_GSTP::Add(T_GSTP_K Key,T_GSTP_D& rData)
{
	m_gstp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_GSTP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GSTP::Del(T_GSTP_K Key)
{
	T_GSTP_D Data;
	BOOL bExist=m_gstp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gstp.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_GSTP_K key;
				T_GSTP_D data;
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


void CDB_GSTP::AddList(T_GSTP_K Key)
{
	GstpList* pList = new GstpList;
	m_gstplist.SetAt(Key, pList);
}

BOOL CDB_GSTP::DelList(T_GSTP_K Key)
{
	GstpList* pList;
	BOOL bExist=m_gstplist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_gstplist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_GSTP::DelAllList()
{
	T_GSTP_K Key;
	GstpList* pList;
	POSITION pos=m_gstplist.GetStartPosition();
	while(pos != NULL)
	{
		m_gstplist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_gstplist.RemoveAll();
}

void CDB_GSTP::AddListItem(T_GSTP_K Key, int nCmd, int nKey)
{
	GstpList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_gstplist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForGstp(Key, nCmd, nKey);
}

BOOL CDB_GSTP::DelListItem(T_GSTP_K Key, int nCmd, int nKey)
{
	GstpList* pList;
	//VERIFY(GetList(Key, pList));
	VERIFY(m_gstplist.Lookup(Key, pList));

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
		return m_pDoc->m_pStagCtrl->DelListItemForGstp(Key, nCmd, nKey);
	return TRUE;
}

//-------------------------------------------------------------------------
BOOL CDB_GSTP::GetList(T_GSTP_K Key,GstpList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_gstplist->Lookup(Key,rpList);
	}
	return m_gstplist.Lookup(Key,rpList);
}

int CDB_GSTP::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_gstplist->GetCount();
	}
	return m_gstplist.GetCount();
}
