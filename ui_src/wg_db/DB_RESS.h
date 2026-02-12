#ifndef __DB_RESS_DB_H__
#define __DB_RESS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RESS
{
public:
	CDB_RESS();
	virtual ~CDB_RESS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RESS_K Key,T_RESS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RESS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RESS_K Key,T_RESS_D& rData);
		//{return m_ress.Lookup(Key,rData);}
	int GetCount();
		//{return m_ress.GetCount();}
	POSITION GetStart();
		//{return m_ress.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RESS_K& rKey,T_RESS_D& rData);
		//{m_ress.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RESS_K,T_RESS_K,T_RESS_D,T_RESS_D&>m_ress;
};

#endif

