#ifndef __DB_PHIB_DB_H__
#define __DB_PHIB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PHIB
{
public:
	CDB_PHIB();
	virtual ~CDB_PHIB();
	CDBDoc* m_pDoc;

public:
	void Add(T_PHIB_K Key,T_PHIB_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PHIB_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PHIB_K Key,T_PHIB_D& rData);
		//{return m_PHIB.Lookup(Key,rData);}
	int GetCount();
		//{return m_PHIB.GetCount();}
	POSITION GetStart();
		//{return m_PHIB.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PHIB_K& rKey,T_PHIB_D& rData);
		//{m_PHIB.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PHIB_K,T_PHIB_K,T_PHIB_D,T_PHIB_D&>m_PHIB;
};

#endif

