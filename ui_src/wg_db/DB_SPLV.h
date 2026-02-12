#ifndef __DB_SPLV_DB_H__
#define __DB_SPLV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SPLV
{
public:
	CDB_SPLV();
	virtual ~CDB_SPLV();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPLV_K Key,T_SPLV_D& rData);
	BOOL Del(T_SPLV_K Key);

public:
	BOOL Get(T_SPLV_K Key,T_SPLV_D& rData);
		//{return m_splv.Lookup(Key,rData);}
	int GetCount();
		//{return m_splv.GetCount();}
	POSITION GetStart();
		//{return m_splv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPLV_K& rKey,T_SPLV_D& rData);
		//{m_splv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPLV_K,T_SPLV_K,T_SPLV_D,T_SPLV_D&>m_splv;
};

#endif

