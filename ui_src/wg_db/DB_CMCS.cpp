#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_CMCS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CMCS::CDB_CMCS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cmcs.InitHashTable(HASHSIZECMCS);
}

CDB_CMCS::~CDB_CMCS()
{

}

void CDB_CMCS::Add(T_CMCS_K Key,T_CMCS_D& rData,CDB_NODE* pNode)
{
	T_CMCS_D Data;
	BOOL bExist=m_cmcs.Lookup(Key, Data);
	m_cmcs.SetAt(Key,rData);
	if(!bExist && pNode)pNode->AddListItem(Key, LT_CMCS_CMD, Key);
}

BOOL CDB_CMCS::Del(T_CMCS_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_cmcs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pNode->DelListItem(Key, LT_CMCS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CMCS::Get(T_CMCS_K Key,T_CMCS_D& rData)
{
	return m_cmcs.Lookup(Key,rData);
}

int CDB_CMCS::GetCount()
{
	return m_cmcs.GetCount();
}

POSITION CDB_CMCS::GetStart()
{
	return m_cmcs.GetStartPosition();
}

void CDB_CMCS::GetNext(POSITION& rNextPosition,T_CMCS_K& rKey,T_CMCS_D& rData)
{
	m_cmcs.GetNextAssoc(rNextPosition,rKey,rData);
}
