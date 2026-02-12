#ifndef __DB_RPRS_DB_H__
#define __DB_RPRS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RPRS
{
public:
	CDB_RPRS();
	virtual ~CDB_RPRS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RPRS_K Key,T_RPRS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RPRS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RPRS_K Key,T_RPRS_D& rData);
		//{return m_rprs.Lookup(Key,rData);}
	int GetCount();
		//{return m_rprs.GetCount();}
	POSITION GetStart();
		//{return m_rprs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RPRS_K& rKey,T_RPRS_D& rData);
		//{m_rprs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RPRS_K,T_RPRS_K,T_RPRS_D,T_RPRS_D&>m_rprs;
};

#endif

