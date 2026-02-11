#ifndef __DB_RPRV_DB_H__
#define __DB_RPRV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RPRV
{
public:
	CDB_RPRV();
	virtual ~CDB_RPRV();
	CDBDoc* m_pDoc;

public:
	void Add(T_RPRV_K Key,T_RPRV_D& rData);
	BOOL Del(T_RPRV_K Key);

public:
	BOOL Get(T_RPRV_K Key,T_RPRV_D& rData);
	//{return m_rprv.Lookup(Key,rData);}
	int GetCount();
	//{return m_rprv.GetCount();}
	POSITION GetStart();
	//{return m_rprv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RPRV_K& rKey,T_RPRV_D& rData);
	//{m_rprv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RPRV_K,T_RPRV_K,T_RPRV_D,T_RPRV_D&>m_rprv;
};

#endif

