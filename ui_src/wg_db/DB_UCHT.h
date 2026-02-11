#ifndef __DB_UCHT_DB_H__
#define __DB_UCHT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_UCHT
{
public:
	CDB_UCHT();
	virtual ~CDB_UCHT();
	CDBDoc* m_pDoc;

public:
	T_UCHT_K m_nLastNum;
	T_UCHT_K m_nStartNum;

public:
	void Add(T_UCHT_K Key,T_UCHT_D& rData);
	BOOL Del(T_UCHT_K Key);

public:
	BOOL Get(T_UCHT_K Key,T_UCHT_D& rData)
		{return m_ucht.Lookup(Key,rData);}
	int GetCount()
		{return m_ucht.GetCount();}
	POSITION GetStart()
		{return m_ucht.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UCHT_K& rKey,T_UCHT_D& rData)
		{m_ucht.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UCHT_K,T_UCHT_K,T_UCHT_D,T_UCHT_D&>m_ucht;
};

#endif
