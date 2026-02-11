#ifndef __DB_GILD_DB_H__
#define __DB_GILD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_GILD
{
public:
	CDB_GILD();
	virtual ~CDB_GILD();
	CDBDoc* m_pDoc;

public:
	void Add(T_GILD_K Key,T_GILD_D& rData);
	BOOL Del(T_GILD_K Key);

public:
	BOOL Get(T_GILD_K Key,T_GILD_D& rData);
		//{return m_gild.Lookup(Key,rData);}
	int GetCount();
		//{return m_gild.GetCount();}
	POSITION GetStart();
		//{return m_gild.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GILD_K& rKey,T_GILD_D& rData);
		//{m_gild.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GILD_K,T_GILD_K,T_GILD_D,T_GILD_D&>m_gild;
};

#endif