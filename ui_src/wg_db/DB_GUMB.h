#ifndef __DB_GUMB_DB_H__
#define __DB_GUMB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_MEMB;
class CDB_GUMB
{
public:
	CDB_GUMB();
	virtual ~CDB_GUMB();
	CDBDoc* m_pDoc;

public:
	T_GUMB_K m_nStartNum;
	T_GUMB_K m_nLastNum;

public:
	void Add(T_GUMB_K Key, T_GUMB_D& rData, CDB_MEMB* pMemb);
	BOOL Del(T_GUMB_K Key, CDB_MEMB* pMemb);

public:
	BOOL Get(T_GUMB_K Key, T_GUMB_D& rData);
	//{return m_gumb.Lookup(Key,rData);}
	int GetCount();
	//{return m_gumb.GetCount();}
	POSITION GetStart();
	//{return m_gumb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUMB_K& rKey, T_GUMB_D& rData);
	//{m_gumb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUMB_K,T_GUMB_K,T_GUMB_D,T_GUMB_D&>m_gumb;
};

#endif