#ifndef __DB_ULCT_DB_H__
#define __DB_ULCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_ULCT
{
public:
	CDB_ULCT();
	virtual ~CDB_ULCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_ULCT_K Key, T_ULCT_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_ULCT_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_ULCT_K Key, T_ULCT_D& rData);
	//{return m_ulct.Lookup(Key,rData);}
	int GetCount();
	//{return m_ulct.GetCount();}
	POSITION GetStart();
	//{return m_ulct.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_ULCT_K& rKey, T_ULCT_D& rData);
	//{m_ulct.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ULCT_K, T_ULCT_K, T_ULCT_D, T_ULCT_D&>m_ulct;
};

#endif