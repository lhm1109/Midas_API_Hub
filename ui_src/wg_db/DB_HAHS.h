#ifndef __DB_HAHS_DB_H__
#define __DB_HAHS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_HAHS
{
public:
	CDB_HAHS();
	virtual ~CDB_HAHS();
	CDBDoc* m_pDoc;

public:
	void Add(T_HAHS_K Key,T_HAHS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_HAHS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_HAHS_K Key,T_HAHS_D& rData);
		//{return m_hahs.Lookup(Key,rData);}
	int GetCount();
		//{return m_hahs.GetCount();}
	POSITION GetStart();
		//{return m_hahs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HAHS_K& rKey,T_HAHS_D& rData);
		//{m_hahs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_HAHS_K,T_HAHS_K,T_HAHS_D,T_HAHS_D&>m_hahs;
};

#endif