#ifndef __DB_UTPL_DB_H__
#define __DB_UTPL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_UTPL
{
public:
	CDB_UTPL();
	virtual ~CDB_UTPL();
	CDBDoc* m_pDoc;

public:
	T_UTPL_K m_nLastNum;
	T_UTPL_K m_nStartNum;

public:
	void Add(T_UTPL_K Key,T_UTPL_D& rData);
	BOOL Del(T_UTPL_K Key);

public:
	BOOL Get(T_UTPL_K Key,T_UTPL_D& rData)
		{return m_utpl.Lookup(Key,rData);}
	int GetCount()
		{return m_utpl.GetCount();}
	POSITION GetStart()
		{return m_utpl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UTPL_K& rKey,T_UTPL_D& rData)
		{m_utpl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UTPL_K,T_UTPL_K,T_UTPL_D,T_UTPL_D&>m_utpl;
};

#endif
