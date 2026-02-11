#ifndef __DB_THAT_DB_H__
#define __DB_THAT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_THAT
{
public:
	CDB_THAT();
	virtual ~CDB_THAT();
	CDBDoc* m_pDoc;

public:
	void Add(T_THAT_K Key,T_THAT_D& rData);
	BOOL Del(T_THAT_K Key);

public:
	BOOL Get(T_THAT_K Key,T_THAT_D& rData);
		//{return m_that.Lookup(Key,rData);}
	int GetCount();
		//{return m_that.GetCount();}
	POSITION GetStart();
		//{return m_that.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THAT_K& rKey,T_THAT_D& rData);
		//{m_that.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THAT_K,T_THAT_K,T_THAT_D,T_THAT_D&>m_that;
};

#endif