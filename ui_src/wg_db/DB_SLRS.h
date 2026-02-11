#ifndef __DB_SLRS_DB_H__
#define __DB_SLRS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SLRS
{
public:
	CDB_SLRS();
	virtual ~CDB_SLRS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SLRS_K Key,T_SLRS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SLRS_K Key,CDB_ELEM* pElem);  

public:
	BOOL Get(T_SLRS_K Key,T_SLRS_D& rData);
		//{return m_slrs.Lookup(Key,rData);}
	int GetCount();
		//{return m_slrs.GetCount();}
	POSITION GetStart();
		//{return m_slrs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SLRS_K& rKey,T_SLRS_D& rData);
		//{m_slrs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SLRS_K,T_SLRS_K,T_SLRS_D,T_SLRS_D&>m_slrs;
};

#endif

