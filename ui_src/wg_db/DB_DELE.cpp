#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DELE.h"
#include "Qsort.h"

CDB_DELE::CDB_DELE()
{
	m_dele.InitHashTable(HASHSIZEDELE);
}

CDB_DELE::~CDB_DELE()
{
	T_DELE_D data;
	if(Get(data))delete data.parDele;
}

void CDB_DELE::Add(T_DELE_D& rData,CDB_ELEM* pElem)
{
	T_DELE_D data;
	BOOL bExist=Get(data);
	if(bExist)  // Modify
	{
		T_ELEM_K KeyElem;
		int nCount=data.parDele->GetSize();
		for(int i=0; i<nCount; i++)
		{
			KeyElem=data.parDele->GetAt(i);
			VERIFY(pElem->DelListItem(KeyElem, LT_DELE_CMD, 1));
		}
		data.parDele->RemoveAll();
	}
	else      // new
		data.parDele=new T_DELE_DT;
	data.parDele->Copy(*rData.parDele);

	CQSort::QSortUInt(data.parDele->GetData(), data.parDele->GetSize());
	m_dele.SetAt(1,data);
	T_ELEM_K KeyElem;
	int nCount=data.parDele->GetSize();
	for(int i=0; i<nCount; i++)
	{
		KeyElem=data.parDele->GetAt(i);
		pElem->AddListItem(KeyElem, LT_DELE_CMD, 1);
	}
}

BOOL CDB_DELE::Del(CDB_ELEM* pElem)
{
	T_DELE_D data;
	if(!Get(data))return FALSE;

	BOOL ret=m_dele.RemoveKey(1);
	ASSERT(ret);
	if(ret)
	{
		T_ELEM_K KeyElem;
		int nCount=data.parDele->GetSize();
		for(int i=0; i<nCount; i++)
		{
			KeyElem=data.parDele->GetAt(i);
			VERIFY(pElem->DelListItem(KeyElem, LT_DELE_CMD, 1));
		}
		data.parDele->RemoveAll();
	}
	return ret;
}

