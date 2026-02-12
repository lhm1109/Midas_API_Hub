#ifndef __DB_EBSV_DB_H__
#define __DB_EBSV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_EBSV
{
public:
	CDB_EBSV();
	virtual ~CDB_EBSV();
	CDBDoc* m_pDoc;

public:
	void Add(T_EBSV_K Key, T_EBSV_D& rData/*, CDB_ELEM* pElem*/);
	BOOL Del(T_EBSV_K Key/*, CDB_ELEM* pElem*/);

public:
	BOOL Get(T_EBSV_K Key, T_EBSV_D& rData);
	//{return m_ebsv.Lookup(Key,rData);}
	int GetCount();
	//{return m_ebsv.GetCount();}
	POSITION GetStart();
	//{return m_ebsv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_EBSV_K& rKey, T_EBSV_D& rData);
	//{m_ebsv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_EBSV_K, T_EBSV_K, T_EBSV_D, T_EBSV_D&>m_ebsv;
};

#endif

