#ifndef __DB_RCBE_DB_H__
#define __DB_RCBE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RCBE
{
public:
	CDB_RCBE();
	virtual ~CDB_RCBE();
	CDBDoc* m_pDoc;

public:
	void Add(T_RCBE_K Key,T_RCBE_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RCBE_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RCBE_K Key,T_RCBE_D& rData);
		//{return m_rcbe.Lookup(Key,rData);}
	int GetCount();
		//{return m_rcbe.GetCount();}
	POSITION GetStart();
		//{return m_rcbe.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RCBE_K& rKey,T_RCBE_D& rData);
		//{m_rcbe.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RCBE_K,T_RCBE_K,T_RCBE_D,T_RCBE_D&>m_rcbe;
};

#endif

