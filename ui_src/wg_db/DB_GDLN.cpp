#include "stdafx.h"
#include "DB_GDLN.h"

CDB_GDLN::CDB_GDLN()
{
	m_nStartNum=1;
	m_gdln.InitHashTable(HASHSIZEGDLN);
}

CDB_GDLN::~CDB_GDLN()
{
	T_GDLN_K key;
	T_GDLN_D data;
	POSITION pos=GetStart();
	while(pos != NULL)
	{
		GetNext(pos, key, data);
		delete data.pXGrid;
		delete data.pYGrid;
	}
}

void CDB_GDLN::Add(T_GDLN_K Key,T_GDLN_D& rData)
{
	T_GDLN_D data;
	if(Get(Key, data))  // Modify
	{
		data.pXGrid->RemoveAll();
		data.pYGrid->RemoveAll();
	}
	else                // new
	{
		data.pXGrid=new T_GDLN_DT;
		data.pYGrid=new T_GDLN_DT;
	}
	data.pXGrid->Copy(*rData.pXGrid);
	data.pYGrid->Copy(*rData.pYGrid);
//  memcpy(data.GridName,rData.GridName,sizeof(data.GridName));
	data.GridName=rData.GridName;
	data.xAxisAngle = rData.xAxisAngle;
	data.yAxisAngle = rData.yAxisAngle;
	data.originPos[0] = rData.originPos[0];
	data.originPos[1] = rData.originPos[1];
	data.originPos[2] = rData.originPos[2];

	m_gdln.SetAt(Key,data);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
}

BOOL CDB_GDLN::Del(T_GDLN_K Key)
{
	T_GDLN_D data;
	if(!Get(Key, data))return FALSE;

	BOOL bRet=m_gdln.RemoveKey(Key);
	ASSERT(bRet);

	if(Key < m_nStartNum)m_nStartNum=Key;
	delete data.pXGrid;
	delete data.pYGrid;

	return bRet;
}

