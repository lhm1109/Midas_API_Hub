#ifndef __DB_IMEL_DB_H__
#define __DB_IMEL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_IMEL
{
public:
	CDB_IMEL();
	virtual ~CDB_IMEL();
	CDBDoc* m_pDoc;

public:
	void Add(T_IMEL_K Key,T_IMEL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_IMEL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_IMEL_K Key,T_IMEL_D& rData);
		//{return m_IMEL.Lookup(Key,rData);}
	int GetCount();
		//{return m_IMEL.GetCount();}
	POSITION GetStart();
		//{return m_IMEL.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_IMEL_K& rKey,T_IMEL_D& rData);
		//{m_IMEL.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_IMEL_K,T_IMEL_K,T_IMEL_D,T_IMEL_D&>m_imel;
};

#endif

