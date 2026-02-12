#include "stdafx.h"
#include "DB_JDAC.h"
#include "DB_NODE.h"

CDB_JDAC::CDB_JDAC()
{
	m_JDAC.InitHashTable(HASHSIZEJDAC);
}

CDB_JDAC::~CDB_JDAC()
{
	
}

void CDB_JDAC::Add(T_JDAC_D& rData, CDB_NODE* pNode)
{
	T_JDAC_K Key = 1;
	T_JDAC_D Data;
	BOOL bExist = m_JDAC.Lookup(1, Data);
	
	if (pNode && bExist && rData.nIncrementMethod==1 && rData.nDispCtrlOption==1)
	{
		if (Data.MasterNode)
			pNode->DelListItem(Data.MasterNode, LT_JDAC_CMD, Key);
	}    
	m_JDAC.SetAt(Key,rData);
	
	if (pNode && rData.nIncrementMethod==1 && rData.nDispCtrlOption==1)
	{
		if (rData.MasterNode)
			pNode->AddListItem(rData.MasterNode, LT_JDAC_CMD, Key);
	}    
}

BOOL CDB_JDAC::Del(CDB_NODE* pNode)
{
	T_JDAC_K Key = 1;
	T_JDAC_D Data;
	m_JDAC.Lookup(Key, Data);
	BOOL ret=m_JDAC.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (pNode && Data.nIncrementMethod==1 && Data.nDispCtrlOption==1)
		{
			ASSERT(Data.MasterNode != NULL);
			pNode->DelListItem(Data.MasterNode, LT_POLC_CMD, Key);
		}     
	}
	return ret;
}
