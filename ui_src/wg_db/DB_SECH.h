#ifndef __DB_SECH_DB_H__
#define __DB_SECH_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_SECH
{
public:
	CDB_SECH();
	virtual ~CDB_SECH();
	CDBDoc* m_pDoc;

public:
	void Add(T_SECH_K Key,T_SECH_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SECH_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SECH_K Key,T_SECH_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SECH_K& rKey,T_SECH_D& rData);

protected:
	CMap<T_SECH_K,T_SECH_K,T_SECH_D,T_SECH_D&>m_SECH;
};

#endif