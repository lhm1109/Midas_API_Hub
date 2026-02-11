#ifndef __DB_RROS_DB_H__
#define __DB_RROS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RROS
{
public:
	CDB_RROS();
	virtual ~CDB_RROS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RROS_K Key,T_RROS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RROS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RROS_K Key,T_RROS_D& rData);
		//{return m_rros.Lookup(Key,rData);}
	int GetCount();
		//{return m_rros.GetCount();}
	POSITION GetStart();
		//{return m_rros.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RROS_K& rKey,T_RROS_D& rData);
		//{m_rros.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RROS_K,T_RROS_K,T_RROS_D,T_RROS_D&>m_rros;
};

#endif

