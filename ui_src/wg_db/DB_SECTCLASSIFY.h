#ifndef __DB_SECTCLASSIFY_DB_H__
#define __DB_SECTCLASSIFY_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SECTCLASSIFY
{
public:
	CDB_SECTCLASSIFY();
	virtual ~CDB_SECTCLASSIFY();
	CDBDoc* m_pDoc;

public:
	void Add(T_SECTCLASSIFY_K Key,T_SECTCLASSIFY_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SECTCLASSIFY_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SECTCLASSIFY_K Key,T_SECTCLASSIFY_D& rData);
		//{return m_SECTCLASSIFY.Lookup(Key,rData);}
	int GetCount();
		//{return m_SECTCLASSIFY.GetCount();}
	POSITION GetStart();
		//{return m_SECTCLASSIFY.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SECTCLASSIFY_K& rKey,T_SECTCLASSIFY_D& rData);
		//{m_SECTCLASSIFY.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SECTCLASSIFY_K,T_SECTCLASSIFY_K,T_SECTCLASSIFY_D,T_SECTCLASSIFY_D&>m_SECTCLASSIFY;
};

#endif

