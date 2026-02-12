#ifndef __DB_BSPN_DB_H__
#define __DB_BSPN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_BSPN
{
public:
	CDB_BSPN();
	virtual ~CDB_BSPN();
	CDBDoc* m_pDoc;

public:
	T_BSPN_K m_nStartNum;
	T_BSPN_K m_nLastNum;

public:
	void Add(T_BSPN_K Key,T_BSPN_D& rData);
	BOOL Del(T_BSPN_K Key);

public:
	BOOL Get(T_BSPN_K Key,T_BSPN_D& rData);
		//{return m_bspn.Lookup(Key,rData);}
	int GetCount();
		//{return m_bspn.GetCount();}
	POSITION GetStart();
		//{return m_bspn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BSPN_K& rKey,T_BSPN_D& rData);
		//{m_bspn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_BSPN_K,T_BSPN_K,T_BSPN_D,T_BSPN_D&>m_bspn;
};

#endif
