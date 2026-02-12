#ifndef __DB_RROV_DB_H__
#define __DB_RROV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RROV
{
public:
	CDB_RROV();
	virtual ~CDB_RROV();
	CDBDoc* m_pDoc;

public:
	void Add(T_RROV_K Key,T_RROV_D& rData);
	BOOL Del(T_RROV_K Key);

public:
	BOOL Get(T_RROV_K Key,T_RROV_D& rData);
		//{return m_rrov.Lookup(Key,rData);}
	int GetCount();
		//{return m_rrov.GetCount();}
	POSITION GetStart();
		//{return m_rrov.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RROV_K& rKey,T_RROV_D& rData);
		//{m_rrov.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RROV_K,T_RROV_K,T_RROV_D,T_RROV_D&>m_rrov;
};

#endif

