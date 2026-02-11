#ifndef __DB_CGSV_DB_H__
#define __DB_CGSV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CGSV
{
public:
	CDB_CGSV();
	virtual ~CDB_CGSV();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGSV_K Key, T_CGSV_D& rData);
	BOOL Del(T_CGSV_K Key);

public:
	BOOL Get(T_CGSV_K Key, T_CGSV_D& rData);
	//{return m_cgsv.Lookup(Key,rData);}
	int GetCount();
	//{return m_cgsv.GetCount();}
	POSITION GetStart();
	//{return m_cgsv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_CGSV_K& rKey, T_CGSV_D& rData);
	//{m_cgsv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGSV_K, T_CGSV_K, T_CGSV_D, T_CGSV_D&>m_cgsv;
};

#endif

