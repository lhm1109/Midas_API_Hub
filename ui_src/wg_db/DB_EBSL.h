#ifndef __DB_EBSL_DB_H__
#define __DB_EBSL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_EBSL
{
public:
	CDB_EBSL();
	virtual ~CDB_EBSL();
	CDBDoc* m_pDoc;

public:
	void Add(T_EBSL_K Key, T_EBSL_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_EBSL_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_EBSL_K Key, T_EBSL_D& rData);
	//{return m_ebsl.Lookup(Key,rData);}
	int GetCount();
	//{return m_ebsl.GetCount();}
	POSITION GetStart();
	//{return m_ebsl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_EBSL_K& rKey, T_EBSL_D& rData);
	//{m_ebsl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_EBSL_K, T_EBSL_K, T_EBSL_D, T_EBSL_D&>m_ebsl;
};

#endif

