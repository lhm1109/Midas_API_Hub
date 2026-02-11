#ifndef __DB_DOHV_DB_H__
#define __DB_DOHV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DOHV
{
public:
	CDB_DOHV();
	virtual ~CDB_DOHV();
	CDBDoc* m_pDoc;

public:
	void Add(T_DOHV_K Key,T_DOHV_D& rData);
	BOOL Del(T_DOHV_K Key);

public:
	BOOL Get(T_DOHV_K Key,T_DOHV_D& rData);
		//{return m_dohv.Lookup(Key,rData);}
	int GetCount();
		//{return m_dohv.GetCount();}
	POSITION GetStart();
		//{return m_dohv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DOHV_K& rKey,T_DOHV_D& rData);
		//{m_dohv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DOHV_K,T_DOHV_K,T_DOHV_D,T_DOHV_D&>m_dohv;
};

#endif

