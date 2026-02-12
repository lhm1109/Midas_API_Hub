#ifndef __DB_MVHL_DB_H__
#define __DB_MVHL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVHL
{
public:
	CDB_MVHL();
	virtual ~CDB_MVHL();
	CDBDoc* m_pDoc;

public:
	T_MVHL_K m_nStartNum;
	T_MVHL_K m_nLastNum;

public:
	void Add(T_MVHL_K Key,T_MVHL_D& rData);
	BOOL Del(T_MVHL_K Key);

public:
	BOOL Get(T_MVHL_K Key,T_MVHL_D& rData);
		//{return m_mvhl.Lookup(Key,rData);}
	int GetCount();
		//{return m_mvhl.GetCount();}
	POSITION GetStart();
		//{return m_mvhl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MVHL_K& rKey,T_MVHL_D& rData);
		//{m_mvhl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MVHL_K,T_MVHL_K,T_MVHL_D,T_MVHL_D&>m_mvhl;
};

#endif