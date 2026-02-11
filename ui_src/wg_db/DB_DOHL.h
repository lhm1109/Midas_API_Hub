#ifndef __DB_DOHL_DB_H__
#define __DB_DOHL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DOHL
{
public:
	CDB_DOHL();
	virtual ~CDB_DOHL();
	CDBDoc* m_pDoc;

public:
	void Add(T_DOHL_K Key,T_DOHL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DOHL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DOHL_K Key,T_DOHL_D& rData);
		//{return m_dohl.Lookup(Key,rData);}
	int GetCount();
		//{return m_dohl.GetCount();}
	POSITION GetStart();
		//{return m_dohl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DOHL_K& rKey,T_DOHL_D& rData);
		//{m_dohl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DOHL_K,T_DOHL_K,T_DOHL_D,T_DOHL_D&>m_dohl;
};

#endif

