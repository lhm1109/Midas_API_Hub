#ifndef __DB_LENV_DB_H__
#define __DB_LENV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_LENV
{
public:
	CDB_LENV();
	virtual ~CDB_LENV();
	CDBDoc* m_pDoc;

public:
	void Add(T_LENV_K Key,T_LENV_D& rData);
	BOOL Del(T_LENV_K Key);

public:
	BOOL Get(T_LENV_K Key,T_LENV_D& rData);
		//{return m_lenv.Lookup(Key,rData);}
	int GetCount();
		//{return m_lenv.GetCount();}
	POSITION GetStart();
		//{return m_lenv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LENV_K& rKey,T_LENV_D& rData);
		//{m_lenv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LENV_K,T_LENV_K,T_LENV_D,T_LENV_D&>m_lenv;
};

#endif

