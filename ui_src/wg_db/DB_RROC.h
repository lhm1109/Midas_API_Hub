#ifndef __DB_RROC_DB_H__
#define __DB_RROC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RROC
{
public:
	CDB_RROC();
	virtual ~CDB_RROC();
	CDBDoc* m_pDoc;

public:
	void Add(T_RROC_K Key,T_RROC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RROC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RROC_K Key,T_RROC_D& rData);
	//{return m_rrop.Lookup(Key,rData);}
	int GetCount();
	//{return m_rrop.GetCount();}
	POSITION GetStart();
	//{return m_rrop.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RROC_K& rKey,T_RROC_D& rData);
	//{m_rrop.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RROC_K,T_RROC_K,T_RROC_D,T_RROC_D&>m_rroc;
};

#endif

