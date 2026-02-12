#include "stdafx.h"
#include "DB_VSEC.h"
#include "DB_VBEM.h"

#include "DBDoc.h"

CDB_VBEM::CDB_VBEM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_vbem.InitHashTable(HASHSIZEVBEM);
	m_vbemlist.InitHashTable(HASHSIZELISTVBEM);
}

CDB_VBEM::~CDB_VBEM()
{
	DelAllList();
}

void CDB_VBEM::Add(T_VBEM_K Key,T_VBEM_D& rData,CDB_VSEC* pVsec)
{
	T_VBEM_D Data;
	BOOL bExist=m_vbem.Lookup(Key, Data);
	if(bExist)
	{
		if (pVsec)
		{
			VERIFY(pVsec->DelListItem(Data.Vsec1, LT_VBEM_CMD, Key));
			VERIFY(pVsec->DelListItem(Data.Vsec2, LT_VBEM_CMD, Key));
		}
	}
	m_vbem.SetAt(Key,rData);
	if (pVsec)
	{
		pVsec->AddListItem(rData.Vsec1, LT_VBEM_CMD, Key);
		pVsec->AddListItem(rData.Vsec2, LT_VBEM_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_VBEM_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_vbem.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

}

BOOL CDB_VBEM::Del(T_VBEM_K Key,CDB_VSEC* pVsec)
{
	T_VBEM_D Data;
	BOOL bExist=m_vbem.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_vbem.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pVsec)
			{
				VERIFY(pVsec->DelListItem(Data.Vsec1, LT_VBEM_CMD, Key));
				VERIFY(pVsec->DelListItem(Data.Vsec2, LT_VBEM_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_VBEM_K key;
			T_VBEM_D data;
			if(m_vbem.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_vbem.GetStartPosition();
				while(pos != NULL)
				{
					m_vbem.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_VBEM::Get(T_VBEM_K Key,T_VBEM_D& rData)
{
	return m_vbem.Lookup(Key,rData);
}

int CDB_VBEM::GetCount()
{
	return m_vbem.GetCount();
}

POSITION CDB_VBEM::GetStart()
{
	return m_vbem.GetStartPosition();
}

void CDB_VBEM::GetNext(POSITION& rNextPosition,T_VBEM_K& rKey,T_VBEM_D& rData)
{
	m_vbem.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_VBEM::AddList(T_VBEM_K Key)
{
	VbemList* pList = new VbemList;
	m_vbemlist.SetAt(Key, pList);
}

BOOL CDB_VBEM::DelList(T_VBEM_K Key)
{
	VbemList* pList;
	BOOL bExist=m_vbemlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_vbemlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_VBEM::DelAllList()
{
	T_VBEM_K Key;
	VbemList* pList;
	POSITION pos=m_vbemlist.GetStartPosition();
	while(pos != NULL)
	{
		m_vbemlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_vbemlist.RemoveAll();
}

BOOL CDB_VBEM::GetList(T_VBEM_K Key, PolcList*& rpList)
{
	return m_vbemlist.Lookup(Key,rpList);
}

int CDB_VBEM::GetCountList()
{
	return m_vbemlist.GetCount();
}


void CDB_VBEM::AddListItem(T_VBEM_K Key, int nCmd, int nKey)
{
	VbemList* pList;
	VERIFY(GetList(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_VBEM::DelListItem(T_VBEM_K Key, int nCmd, int nKey)
{
	VbemList* pList;
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