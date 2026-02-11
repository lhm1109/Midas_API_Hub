#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_DNOD.h"
#include "Qsort.h"

CDB_DNOD::CDB_DNOD()
{
	m_dnod.InitHashTable(HASHSIZEDNOD);
}

CDB_DNOD::~CDB_DNOD()
{
	T_DNOD_D data;
	if(Get(data))delete data.parDnod;
}

void CDB_DNOD::Add(T_DNOD_D& rData,CDB_NODE* pNode)
{
	T_DNOD_D data;
	BOOL bExist=Get(data);
	if(bExist)  // Modify
	{
		T_NODE_K KeyNode;
		int nCount=data.parDnod->GetSize();
		for(int i=0; i<nCount; i++)
		{
			KeyNode=data.parDnod->GetAt(i);
			VERIFY(pNode->DelListItem(KeyNode, LT_DNOD_CMD, 1));
		}
		data.parDnod->RemoveAll();
	}
	else      // new
		data.parDnod=new T_DNOD_DT;
	data.parDnod->Copy(*rData.parDnod);

	CQSort::QSortUInt(data.parDnod->GetData(), data.parDnod->GetSize());
	m_dnod.SetAt(1,data);
	T_NODE_K KeyNode;
	int nCount=data.parDnod->GetSize();
	for(int i=0; i<nCount; i++)
	{
		KeyNode=data.parDnod->GetAt(i);
		pNode->AddListItem(KeyNode, LT_DNOD_CMD, 1);
	}
}

BOOL CDB_DNOD::Del(CDB_NODE* pNode)
{
	T_DNOD_D data;
	if(!Get(data))return FALSE;

	BOOL ret=m_dnod.RemoveKey(1);
	ASSERT(ret);
	if(ret)
	{
		T_NODE_K KeyNode;
		int nCount=data.parDnod->GetSize();
		for(int i=0; i<nCount; i++)
		{
			KeyNode=data.parDnod->GetAt(i);
			VERIFY(pNode->DelListItem(KeyNode, LT_DNOD_CMD, 1));
		}
		data.parDnod->RemoveAll();
	}
	return ret;
}

