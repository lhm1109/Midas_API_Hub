#ifndef __DB_CMFV_DB_H__
#define __DB_CMFV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CMFV
{
public:
	CDB_CMFV();
	virtual ~CDB_CMFV();
	CDBDoc* m_pDoc;

public:
	void Add(T_CMFV_K Key, T_CMFV_D& rData);
	BOOL Del(T_CMFV_K Key);

public:
	BOOL Get(T_CMFV_K Key, T_CMFV_D& rData);
	//{return m_cmfv.Lookup(Key,rData);}
	int GetCount();
	//{return m_cmfv.GetCount();}
	POSITION GetStart();
	//{return m_cmfv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_CMFV_K& rKey, T_CMFV_D& rData);
	//{m_cmfv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CMFV_K, T_CMFV_K, T_CMFV_D, T_CMFV_D&>m_cmfv;
};

#endif

