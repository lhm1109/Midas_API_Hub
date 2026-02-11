#include "stdafx.h"
#include "DB_RFST.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RFST::CDB_RFST()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_rfst.InitHashTable(HASHSIZERFST);
	m_rfstlist.InitHashTable(HASHSIZELISTRFST);
}

CDB_RFST::~CDB_RFST()
{
	DelAllList();
}

CDB_RFST::CDB_RFST(CDB_RFST& src)
{
	*this = src;
}

CDB_RFST& CDB_RFST::operator= (CDB_RFST& src)
{
	POSITION pos;

	m_nStartNum = src.m_nStartNum;
	m_nLastNum = src.m_nLastNum;

	T_RFST_K KeySect;
	T_RFST_D DataSect;
	m_rfst.RemoveAll();
	pos = src.m_rfst.GetStartPosition();
	while (pos != NULL)
	{
		src.m_rfst.GetNextAssoc(pos, KeySect, DataSect);
		m_rfst.SetAt(KeySect, DataSect);
	}

	DelAllList();
	SectList* pList;
	pos = src.m_rfstlist.GetStartPosition();
	while (pos != NULL)
	{
		src.m_rfstlist.GetNextAssoc(pos, KeySect, pList);
		SectList* pListNew = new SectList;
		POSITION pos2 = pList->GetHeadPosition();
		while (pos2 != NULL)
		{
			T_LIST_DATA ListData = pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_rfstlist.SetAt(KeySect, pListNew);
	}

	return *this;
}

void CDB_RFST::Add(T_RFST_K Key, T_RFST_D& rData)
{
	T_RFST_D data;
	if (!m_rfst.Lookup(Key, data))
	{
	}
	m_rfst.SetAt(Key, rData);
	if (Key == m_nStartNum)
	{
		while (TRUE)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_RFST::Del(T_RFST_K Key)
{
	BOOL ret = m_rfst.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_RFST_K key;
			T_RFST_D data;
			if (Get(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = GetStart();
				while (pos != NULL)
				{
					GetNext(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
	}
	return ret;
}

void CDB_RFST::AddList(T_RFST_K Key)
{
	SectList* pList = new SectList;
	m_rfstlist.SetAt(Key, pList);
}

BOOL CDB_RFST::DelList(T_RFST_K Key)
{
	SectList* pList;
	BOOL bExist = m_rfstlist.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist)return FALSE;
	m_rfstlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_RFST::DelAllList()
{
	T_RFST_K Key;
	SectList* pList;
	POSITION pos = m_rfstlist.GetStartPosition();
	while (pos != NULL)
	{
		m_rfstlist.GetNextAssoc(pos, Key, pList);
		delete pList;
	}
	m_rfstlist.RemoveAll();
}

void CDB_RFST::AddListItem(T_RFST_K Key, int nCmd, int nKey)
{
	SectList* pList;
	if (!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	pList->AddTail(ListData);
}

BOOL CDB_RFST::DelListItem(T_RFST_K Key, int nCmd, int nKey)
{
	SectList* pList;
	if (!GetList(Key, pList))return TRUE;

	T_LIST_DATA ListData;
	POSITION pos = pList->GetHeadPosition(), posPrv;
	while (pos != NULL)
	{
		posPrv = pos;
		ListData = pList->GetNext(pos);
		if (ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}

