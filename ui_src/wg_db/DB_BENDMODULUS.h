#ifndef __DB_BENDMODULUS_DB_H__
#define __DB_BENDMODULUS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_BENDMODULUS
{
public:
	CDB_BENDMODULUS();
	virtual ~CDB_BENDMODULUS();
	CDBDoc* m_pDoc;

public:
	void Add(T_BENDMODULUS_K Key,T_BENDMODULUS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_BENDMODULUS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_BENDMODULUS_K Key,T_BENDMODULUS_D& rData);
		//{return m_BENDMODULUS.Lookup(Key,rData);}
	int GetCount();
		//{return m_BENDMODULUS.GetCount();}
	POSITION GetStart();
		//{return m_BENDMODULUS.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BENDMODULUS_K& rKey,T_BENDMODULUS_D& rData);
		//{m_BENDMODULUS.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_BENDMODULUS_K,T_BENDMODULUS_K,T_BENDMODULUS_D,T_BENDMODULUS_D&>m_BENDMODULUS;
};

#endif

