#ifndef __DB_RIPC_DB_H__
#define __DB_RIPC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_RIPC
{
public:
	CDB_RIPC();
	virtual ~CDB_RIPC();

public:
	T_RIPC_K m_nStartNum;
	T_RIPC_K m_nLastNum;

public:
	void Add(T_RIPC_K Key, T_RIPC_D& rData);
	BOOL Del(T_RIPC_K Key);

public:
	BOOL Get(T_RIPC_K Key,T_RIPC_D& rData)
		{return m_ripc.Lookup(Key,rData);}
	int GetCount()
		{return m_ripc.GetCount();}
	POSITION GetStart()
		{return m_ripc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RIPC_K& rKey,T_RIPC_D& rData)
		{m_ripc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RIPC_K,T_RIPC_K,T_RIPC_D,T_RIPC_D&>m_ripc;
};

#endif